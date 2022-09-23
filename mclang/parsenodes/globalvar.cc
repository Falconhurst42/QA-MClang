
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/globalvar.h"
#include "parsenodes/word.h"
#include <string>

GlobalVarNode::GlobalVarNode(Type varType, ParseNode *childExpr, Loc loc)
: ParseNode(PNODE_GLOBALVAR, loc), varType(varType),
childExpr(childExpr) {

}

GlobalVarNode::~GlobalVarNode() {
    delete childExpr;
}

std::vector<ParseNode *> GlobalVarNode::children() const {
    return {};
}

void GlobalVarNode::bytecode(BCManager &man) {
    if (varType == Type("str"))
        MCLError("Non-constant strings are not supported.", loc);
    // Get the global variable name
    std::vector<std::string> varNameList;
    std::vector<ParseNode *> childExprList;
    if (childExpr->getType() == PNODE_COMMA)
        childExprList = childExpr->children();
    else
        childExprList = {childExpr};
    for (ParseNode *child : childExprList) {
        if (varType.isConst) {
            if (child->getType() != PNODE_ASSIGN)
                MCLError("Constant global variable needs to be assigned.", loc);
            varNameList.push_back(((AssignNode *)child)->getVarName());
        } else {
            if (child->getType() != PNODE_WORD)
                MCLError("Expected global variable name without assignment "
                "value.", loc);
            varNameList.push_back(((WordNode *)child)->getContent());
        }
    }
    // Check if the global variable name isn't already in use
    Type tmp;
    for (const std::string &varName : varNameList)
        if (hasNameConflict(varName, man))
            MCLError("Global variable \"" + varName + "\" already defined",
            loc);
    // Set constant variable value
    for (unsigned int i = 0; i < childExprList.size(); i++) {
        if (varType.isConst) {
            ((AssignNode *)childExprList[i])->getExpr()->bytecode(man);
            if (man.ret.type != varType)
                MCLError("Assigning value of type \"" + man.ret.type.str()
                + "\" to global constant of type \"" + varType.str() + "\"",
                loc);
            // Return value contains the constant value here
            man.ctx.setConst(varNameList[i], man.ret.value);
        }
        // Remember that this variable is now registered
        man.ctx.pushVar(Var(varType, varNameList[i]));
    }
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool GlobalVarNode::hasNameConflict(std::string varName, BCManager &man) const {
    Var var(Type("int"), "??");
    return man.ctx.findVar(varName, var);
}