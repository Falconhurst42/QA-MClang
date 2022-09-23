
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/func.h"
#include "parsenodes/return.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

ReturnNode::ReturnNode(ParseNode *childExpr, Loc loc) :
ParseNode(PNODE_RETURN, loc), childExpr(childExpr) {

}

ReturnNode::~ReturnNode() {
    
}

std::vector<ParseNode *> ReturnNode::children() const {
    if (childExpr != nullptr)
        return {childExpr};
    return {};
}

void ReturnNode::bytecode(BCManager &man) {
    expectedType = man.curFuncNode->getReturnType();
    if (childExpr != nullptr) {
        childExpr->bytecode(man);
        if (!(expectedType <= man.ret.type))
            invalidTypeError(man.ret.type);
        // NOTE: The return type is always non-constant
        // NOTE: Because a constant return type is not possible, strings cannot
        // appear here
        if (man.ret.type.isConst)
            man.write(BCInstr(INSTR_SET, "0retv", man.ret.value));
        else
            man.write(BCInstr(INSTR_COPY, "0retv", man.ret.value));
    } else if (expectedType != Type("void")) {
        invalidTypeError(Type("void"));
    }
    // It is important this value is set to 1 after the expression is evaluated,
    // as the expression might contain a call and a call sets 0ret to 0
    // afterwards!
    man.write(BCInstr(INSTR_SET, "0ret", "1"));
    man.ret.type = Type("void");
    man.ret.value = "";
    man.setTopWriteStack(false);
}

void ReturnNode::invalidTypeError(Type givenType) const {
    MCLError("Expression of type \"" + givenType.str() + "\" given in return "
    "statement of function with return type \"" + expectedType.str() + "\".",
    loc);
}