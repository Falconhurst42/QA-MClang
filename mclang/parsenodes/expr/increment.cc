
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/expr/increment.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

IncrementNode::IncrementNode(ParseNodeType type, std::string varName, Loc loc) :
ParseNode(type, loc), varName(varName) {

}

IncrementNode::~IncrementNode() {

}

std::vector<ParseNode *> IncrementNode::children() const {
    return {};
}

void IncrementNode::bytecode(BCManager &man) {
    Var result(Type(), "??");
    if (!man.ctx.findVar(varName, result))
        MCLError("Incrementing/decrementing uninitialized variable \""
        + varName + "\".", loc);
    varType = result.type;
    if (varType != Type("int"))
        invalidTypeError();
    std::string tmpVar = man.tmp.reserve();
    BCInstrType op = isIncrement() ? INSTR_ADDI : INSTR_SUBI;
    if (isPrefix()) {
        man.write(BCInstr(op, varName, "1"));
        man.write(BCInstr(INSTR_COPY, tmpVar, varName));
    } else {
        man.write(BCInstr(INSTR_COPY, tmpVar, varName));
        man.write(BCInstr(op, varName, "1"));
    }
    man.tmp.free(tmpVar);
    man.ret = {Type("int"), tmpVar};
}

void IncrementNode::invalidTypeError() const {
    MCLError("Increment/decrement operator does not support variable of "
    "type \"" + varType.str() + "\"", loc);
}

bool IncrementNode::isPrefix() const {
    return type == PNODE_INC_PREFIX || type == PNODE_DEC_PREFIX;
}

bool IncrementNode::isIncrement() const {
    return type == PNODE_INC_PREFIX || type == PNODE_INC_SUFFIX;
}