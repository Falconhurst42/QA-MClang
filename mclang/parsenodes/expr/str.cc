
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/strmanip.h"
#include "general/types.h"
#include "parsenodes/expr/str.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

StrNode::StrNode(std::string content, Loc loc) : ParseNode(PNODE_STR, loc),
content(content) {

}

StrNode::~StrNode() {

}

std::vector<ParseNode *> StrNode::children() const {
    return {};
}

void StrNode::bytecode(BCManager &man) {
    replaceConstants(man);
    man.ret = {Type("const str"), processedContent};
}

void StrNode::replaceConstants(const BCManager &man) {
    processedContent = replaceInsertedValues(content, man.ctx.getConstValues());
}