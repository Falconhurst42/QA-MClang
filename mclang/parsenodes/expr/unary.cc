
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/expr/unary.h"
#include "parsenodes/parsenode.h"

UnaryNode::UnaryNode(ParseNodeType type, ParseNode *child, Loc loc) :
ExprNode(type, loc, child) {

}

UnaryNode::~UnaryNode() {

}

void UnaryNode::bytecode(BCManager &man) {
    left->bytecode(man), ret = man.ret;
    if (ret.type == Type("int")) {
        if (type == PNODE_UNARY_MINUS)
            man.write(BCInstr(INSTR_MULI, man.ret.value, "-1"));
        man.ret = {Type("int"), man.ret.value};
    } else if (ret.type == Type("const int")) {
        int m = type == PNODE_UNARY_MINUS ? -1 : 1;
        man.ret = {Type("const int"), std::to_string(std::stoi(ret.value) * m)};
    } else {
        invalidTypeError();
    }
}

void UnaryNode::invalidTypeError() const {
    std::string opname = (type == PNODE_UNARY_MINUS ? "-" : "+");
    MCLError("Unary operator \"" + opname + "\" does not support operand of "
    "type \"" + ret.type.str() + "\"", loc);
}