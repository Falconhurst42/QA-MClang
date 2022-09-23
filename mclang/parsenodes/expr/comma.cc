
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/comma.h"
#include "parsenodes/parsenode.h"
#include <vector>

CommaNode::CommaNode(std::vector<ParseNode *> childNodes, Loc loc) :
ParseNode(PNODE_COMMA, loc), childNodes(childNodes) {

}

CommaNode::~CommaNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> CommaNode::children() const {
    return childNodes;
}

void CommaNode::bytecode(BCManager &man) {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        childNodes[i]->bytecode(man);
    man.ret.type = Type("void");
    man.ret.value = "";
}