
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/varinit.h"
#include "parsenodes/word.h"
#include <string>

VarInitNode::VarInitNode(Type varType, ParseNode *childExpr, Loc loc) :
ParseNode(PNODE_VARINIT, loc), varType(varType), childExpr(childExpr) {

}

VarInitNode::~VarInitNode() {
    delete childExpr;
}

std::vector<ParseNode *> VarInitNode::children() const {
    return {childExpr};
}

void VarInitNode::bytecode(BCManager &man) {
    if (childExpr->getType() == PNODE_COMMA)
        childExprList = childExpr->children();
    else
        childExprList = {childExpr};
    if (varType == Type("str"))
        MCLError("Non-constant strings are not supported.", loc);
    for (ParseNode *child : childExprList) {
        if (varType.isConst)
            constBytecode(man, child);
        else
            nonConstBytecode(man, child);
    }
}

void VarInitNode::constBytecode(BCManager &man, ParseNode *child) {
    // Get the name of the constant and evaluate the assignment expression
    if (child->getType() != PNODE_ASSIGN)
        MCLError("Expected constant variable assignment.", loc);
    std::string varName = ((AssignNode *)child)->getVarName();
    ParseNode *expr = ((AssignNode *)child)->getExpr();
    expr->bytecode(man);
    if (man.ret.type != varType)
        MCLError("Cannot assign value of type \"" + man.ret.type.str()
        + "\" to constant variable of type \"" + varType.str() + "\".", loc);
    if (hasNameConflict(varName, man))
        MCLError("Initialization of already defined variable \"" + varName
        + "\"", loc);
    man.ctx.setConst(varName, man.ret.value);
    man.ctx.pushVar(Var(varType, varName));
    man.ret.type = Type("void");
    man.ret.value = "";
}

void VarInitNode::nonConstBytecode(BCManager &man, ParseNode *child) {
    // Check if the type of the child expression is correct and get the variable
    // name
    std::string varName;
    if (child->getType() == PNODE_ASSIGN)
        varName = ((AssignNode *)child)->getVarName();
    else if (child->getType() == PNODE_WORD)
        varName = ((WordNode *)child)->getContent();
    else
        MCLError("Invalid expression after initialization.", loc);
    // If the child expression is just the variable name, initialize this
    // variable (if it hasn't already) by adding 0
    if (child->getType() == PNODE_WORD)
        man.write(BCInstr(INSTR_ADDI, varName, "0"));
    if (hasNameConflict(varName, man))
        MCLError("Initialization of already defined variable \"" + varName
        + "\"", loc);
    // Register variable with type
    man.ctx.pushVar(Var(varType, varName));
    if (child->getType() == PNODE_ASSIGN)
        child->bytecode(man);
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool VarInitNode::hasNameConflict(std::string varName, BCManager &man) const {
    Var var(Type("int"), "??");
    return man.ctx.findVar(varName, var);
}