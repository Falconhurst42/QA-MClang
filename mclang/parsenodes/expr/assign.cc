
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

AssignNode::AssignNode(ParseNodeType type, std::string varName, ParseNode *expr,
Loc loc) : ExprNode(type, loc, expr), varName(varName) {

}

AssignNode::~AssignNode() {
    
}

void AssignNode::bytecode(BCManager &man) {
    // This also retrieves the type of the variable
    if (!isInitialized(man, varType))
        MCLError("Use of uninitialized variable \"" + varName + "\".", loc);
    if (varType.isConst)
        MCLError("Cannot modify a constant variable.", loc);
    if (type == PNODE_ASSIGN)
        bytecodeNormal(man);
    else if (type == PNODE_ASSIGN_ADD || type == PNODE_ASSIGN_SUB || type ==
    PNODE_ASSIGN_MUL || type == PNODE_ASSIGN_DIV || type == PNODE_ASSIGN_MOD)
        bytecodeArith(man);
    else
        MCLError("Unexpected error while converting assignment bytecode.",
        loc);
}

std::string AssignNode::getVarName() const {
    return varName;
}

ParseNode *AssignNode::getExpr() const {
    return left;
}

void AssignNode::bytecodeNormal(BCManager &man) {
    if (varType == Type("int") || varType == Type("bool")) {
        left->bytecode(man);
        if (!man.ret.type.sameBase(varType))
            invalidTypeError(man);
        if (man.ret.type.isConst) {
            man.write(BCInstr(INSTR_SET, varName, man.ret.value));
        } else {
            man.write(BCInstr(INSTR_COPY, varName, man.ret.value));
        }
    } else {
        invalidTypeError(man);
    }
    man.ret = {man.ret.type, man.ret.value};
}

void AssignNode::bytecodeArith(BCManager &man) {
    std::string tmpVar = man.tmp.reserve();
    if (varType != Type("int"))
        MCLError("Cannot perform arithmatic assignment on variable of type "
        "\"" + varType.str() + "\"", loc);
    left->bytecode(man);
    if (!man.ret.type.sameBase(varType))
        invalidTypeError(man);
    if (man.ret.type.isConst)
        man.write(BCInstr(getConstArithInstr(), varName, man.ret.value));
    else
        man.write(BCInstr(getArithInstr(), varName, man.ret.value));
    man.write(BCInstr(INSTR_COPY, tmpVar, varName));
    man.tmp.free(tmpVar);
    man.ret = {varType, tmpVar};
}

void AssignNode::invalidTypeError(BCManager &man) const {
    MCLError("Cannot assign value of type \"" + man.ret.type.str()
    + "\" to variable of type \"" + varType.str() + "\".", loc);
}

bool AssignNode::isInitialized(BCManager &man, Type &varType) const {
    Var var(Type("int"), "??");
    bool result = man.ctx.findVar(varName, var);
    if (!result)
        return false;
    varType = var.type;
    return true;
}

BCInstrType AssignNode::getArithInstr() {
    if (type == PNODE_ASSIGN_ADD)
        return INSTR_ADD;
    if (type == PNODE_ASSIGN_SUB)
        return INSTR_SUB;
    if (type == PNODE_ASSIGN_MUL)
        return INSTR_MUL;
    if (type == PNODE_ASSIGN_DIV)
        return INSTR_DIV;
    if (type == PNODE_ASSIGN_MOD)
        return INSTR_MOD;
    MCLError("Unexpected error while generating assignment bytecode.", loc);
    return INSTR_ERR;
}

BCInstrType AssignNode::getConstArithInstr() {
    if (type == PNODE_ASSIGN_ADD)
        return INSTR_ADDI;
    if (type == PNODE_ASSIGN_SUB)
        return INSTR_SUBI;
    if (type == PNODE_ASSIGN_MUL)
        return INSTR_MULI;
    if (type == PNODE_ASSIGN_DIV)
        return INSTR_DIVI;
    if (type == PNODE_ASSIGN_MOD)
        return INSTR_MODI;
    MCLError("Unexpected error while generating assignment bytecode.", loc);
    return INSTR_ERR;
}