
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/parsenode.h"
#include <vector>

CodeBlockNode::CodeBlockNode(std::vector<ParseNode *> childNodes, Loc loc) :
ParseNode(PNODE_CODEBLOCK, loc), childNodes(childNodes) {

}

CodeBlockNode::~CodeBlockNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> CodeBlockNode::children() const {
    return childNodes;
}

void CodeBlockNode::bytecode(BCManager &man) {
    man.ctx.push();
    man.pushWriteStack(true);
    for (unsigned int i = 0; i < childNodes.size() && man.canWrite(); i++)
        childNodes[i]->bytecode(man);
    man.popWriteStack();
    man.ctx.pop();
    man.ret.type = Type("void");
    man.ret.value = "";
}