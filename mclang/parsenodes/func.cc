
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <string>
#include <vector>

FuncNode::FuncNode(Type retType, std::string name, std::vector<Param> params,
CodeBlockNode *codeblock, Loc loc) : ParseNode(PNODE_FUNC, loc),
retType(retType), name(name), params(params), codeblock(codeblock) {
    for (Param &param : params)
        if (param.type == Type("str"))
            MCLError("Non-constant strings as parameters are not supported.",
            loc);
    if (retType == Type("str"))
        MCLError("Non-constant strings are not supported.", loc);
    if (retType.isConst)
        MCLError("Constant return types are not supported", loc);
}

std::vector<ParseNode *> FuncNode::children() const {
    return {(ParseNode *)codeblock};
}

FuncNode::~FuncNode() {
    delete codeblock;
}

void FuncNode::bytecode(BCManager &man) {
    (void)man;
    MCLError("Unexpected error, direct generation of function bytecode is not "
    "allowed.", loc);
}

std::string FuncNode::bytecode(BCManager &man, std::vector<std::string>
constValues) {
    checkReturnWarning(man);
    // Find generation entry associated with these constant values, if none is
    // found a new one is created
    FuncGenEntry *funcEntry = getGenerationEntry(constValues);
    if (funcEntry == nullptr)
        addGenerationEntry(man, constValues);
    funcEntry = getGenerationEntry(constValues);
    std::string callname = funcEntry->bcfunc->name;
    // Check if bytecode has already been generated
    if (funcEntry->hasGenerated)
        return callname;
    man.setFuncStack({funcEntry->bcfunc});
    // Mark the function as generated
    funcEntry->hasGenerated = true;
    man.ctx.push();
    man.write(BCInstr(INSTR_SET, "0ret", "0"));
    initParams(man, constValues);
    man.curFuncNode = this;
    codeblock->bytecode(man);
    man.ctx.pop();
    man.popFunc();
    return callname;
}

Type FuncNode::getReturnType() const {
    return retType;
}

std::string FuncNode::getName() const {
    return name;
}

std::vector<Type> FuncNode::getParamTypes() const {
    std::vector<Type> out;
    for (const Param &param : params)
        out.push_back(param.type);
    return out;
}

bool FuncNode::acceptTypes(std::vector<Type> types) const {
    if (types.size() != params.size())
        return false;
    for (unsigned int i = 0; i < types.size(); i++)
        if (!(types[i] >= params[i].type))
            return false;
    return true;
}

bool FuncNode::hasUngeneratedEntries() const {
    for (const FuncGenEntry &entry : genTable)
        if (!entry.hasGenerated)
            return true;
    return false;
}

void FuncNode::generateEntries(BCManager &man) {
    for (FuncGenEntry &entry : genTable) {
        if (!entry.hasGenerated) {
            bytecode(man, entry.constValues);
            entry.hasGenerated = true;
        }
    }
}

std::string FuncNode::addGenerationEntry(BCManager &man,
std::vector<std::string> constValues) {
    // Check if the entry already exists
    if (getGenerationEntry(constValues) != nullptr)
        return getGenerationEntry(constValues)->bcfunc->name;
    // Functions with no parameters and a void return type will receive their
    // original name, others will receive a random name
    if (getReturnType() == Type("void") && getParamTypes().empty())
        man.addFunc(getFormattedName());
    else
        man.addFunc();
    FuncGenEntry entry = {false, man.topFunc(), constValues};
    std::string callname = man.topFunc()->name;
    man.popFunc();
    genTable.push_back(entry);
    return callname;
}

void FuncNode::checkNameConflicts(BCManager &man) const {
    if (hasNameConflict(man))
        MCLError("Function with same name and parameter types was already "
        "defined.", loc);
}

std::vector<std::string> FuncNode::defaultConstValues() const {
    std::vector<std::string> out;
    for (const Param &param : params) {
        if (param.type.isConst) {
            if (param.type.base == TYPE_BOOL || param.type.base == TYPE_INT)
                out.push_back("0");
            else if (param.type.base == TYPE_STR || param.type.base ==
            TYPE_VOID)
                out.push_back("");
        }
    }
    return out;
}

bool FuncNode::hasNameConflict(BCManager &man) const {
    for (FuncNode *func : man.funcs.getFuncs())
        if (hasNameConflict(func))
            return true;
    return false;
}

bool FuncNode::hasNameConflict(FuncNode *other) const {
    if (other->getName() != name)
        return false;
    std::vector<Type> otherTypes = other->getParamTypes();
    if (otherTypes.size() != params.size())
        return false;
    for (unsigned int i = 0; i < otherTypes.size(); i++)
        if (otherTypes[i] != params[i].type)
            return false;
    return true;
}

void FuncNode::initParams(BCManager &man, std::vector<std::string> constValues)
{
    // Add parameters to context and copy them to the correct variables
    unsigned int constCount = 0;
    for (unsigned int i = 0; i < params.size(); i++) {
        if (params[i].type.isConst) {
            man.ctx.setConst(params[i].name, constValues[constCount]);
            constCount++;
        } else {
            man.write(BCInstr(INSTR_COPY, params[i].name, "__param"
            + std::to_string(i)));
        }
        Var var(Type(), "??");
        if (man.ctx.findVar(params[i].name, var))
            MCLError("Parameter name is already defined somewhere else.", loc);
        man.ctx.pushVar(Var(params[i].type, params[i].name));
    }
}

FuncGenEntry *FuncNode::getGenerationEntry(std::vector<std::string> constValues) {
    for (FuncGenEntry &entry : genTable)
        if (entry.constValues == constValues)
            return &entry;
    return nullptr;
}

std::string FuncNode::getFormattedName() const {
    // Valid Minecraft function names may not have uppercase characters in them
    std::string out = "";
    for (const char &cur : name) {
        if ('A' <= cur && cur <= 'Z')
            out.push_back('.'), out.push_back(cur - 'A' + 'a');
        else if (cur == '_' || ('a' <= cur && cur <= 'z') || ('0' <= cur
        && cur <= '9'))
            out.push_back(cur);
        else
            MCLError("Unexpected error while generating formatted function "
            "name.", loc);
    }
    return out;
}

void FuncNode::checkReturnWarning(BCManager &man) const {
    if (retType == Type("void"))
        return;
    for (const ParseNode *line : codeblock->children())
        if (line->getType() == PNODE_RETURN)
            return;
    MCLError(ERR_WARN, "No return statement in non-void function.", man.comp,
    loc);
}