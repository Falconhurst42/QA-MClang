
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/arith.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

ArithNode::ArithNode(ParseNodeType type, ParseNode *left, ParseNode *right,
Loc loc) : ExprNode(type, loc, left, right) {

}

ArithNode::~ArithNode() {
    
}

void ArithNode::bytecode(BCManager &man) {
    if (arithTable.count(getType()) == 0)
        MCLError("Unexpected error while converting arithmatic");
    left->bytecode(man);
    retLeft = man.ret;
    if (man.ret.type == Type("int"))
        bytecodeInt(man);
    else if (man.ret.type == Type("const int"))
        bytecodeConstInt(man);
    else if (man.ret.type == Type("const str"))
        bytecodeConstStr(man);
    else
        right->bytecode(man), retRight = man.ret, invalidTypeError();
}

void ArithNode::invalidTypeError() const {
    if (arithTable.count(getType()) == 0)
        MCLError("Unexpected error while converting arithmatic");
    std::string shortName = arithTable.find(getType())->second.shortName;
    MCLError("Operation \"" + shortName + "\" does not support operands of "
    "types \"" + retLeft.type.str() + "\" and \"" + retRight.type.str() + "\".",
    loc);
}

void ArithNode::bytecodeInt(BCManager &man) {
    std::string tmpVar = man.tmp.reserve();
    man.write(BCInstr(INSTR_COPY, tmpVar, man.ret.value));
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("int")) {
        BCInstrType instrType = arithTable.find(getType())->second.instrType;
        man.write(BCInstr(instrType, tmpVar, man.ret.value));
        man.ret = {Type("int"), tmpVar};
    } else if (man.ret.type == Type("const int")) {
        BCInstrType instrType = arithTable.find(getType())->second.instrTypeI;
        man.write(BCInstr(instrType, tmpVar, man.ret.value));
        man.ret = {Type("int"), tmpVar};
    } else {
        invalidTypeError();
    }
    man.tmp.free(tmpVar);
}

void ArithNode::bytecodeConstInt(BCManager &man) {
    std::string tmpVar = man.tmp.reserve();
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("int")) {
        BCInstrType instrTypeI = arithTable.find(getType())->second.instrTypeI;
        BCInstrType instrType = arithTable.find(getType())->second.instrType;
        if (type == PNODE_ADD || type == PNODE_MUL) {
            man.write(BCInstr(instrTypeI, man.ret.value, retLeft.value));
            man.ret = {Type("int"), man.ret.value};
        } else {
            man.write(BCInstr(INSTR_SET, tmpVar, retLeft.value));
            man.write(BCInstr(instrType, tmpVar, man.ret.value));
            man.ret = {Type("int"), tmpVar};
        }
    } else if (man.ret.type == Type("const int")) {
        std::string val = evalConsts(retLeft.value, man.ret.value);
        man.ret = {Type("const int"), val};
    } else {
        invalidTypeError();
    }
    man.tmp.free(tmpVar);
}

void ArithNode::bytecodeConstStr(BCManager &man) {
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("const str")) {
        std::string val = retLeft.value + man.ret.value;
        man.ret = {Type("const str"), val};
    } else {
        invalidTypeError();
    }
}

std::string ArithNode::evalConsts(std::string numLeft, std::string numRight)
const {
    if (type == PNODE_ADD)
        return std::to_string(std::stoi(numLeft) + std::stoi(numRight));
    if (type == PNODE_SUB)
        return std::to_string(std::stoi(numLeft) - std::stoi(numRight));
    if (type == PNODE_MUL)
        return std::to_string(std::stoi(numLeft) * std::stoi(numRight));
    if (type == PNODE_DIV)
        return std::to_string(std::stoi(numLeft) / std::stoi(numRight));
    if (type == PNODE_MOD)
        return std::to_string(std::stoi(numLeft) % std::stoi(numRight));
    MCLError("Unexpected error when evaluating constant expression.", loc);
    return "";
}