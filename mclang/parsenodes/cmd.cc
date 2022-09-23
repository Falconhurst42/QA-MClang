
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/strmanip.h"
#include "general/types.h"
#include "parsenodes/cmd.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CmdNode::CmdNode(std::string cmd, Loc loc) : ParseNode(PNODE_CMD, loc), cmd(cmd)
{

}

CmdNode::~CmdNode() {
    
}

std::vector<ParseNode *> CmdNode::children() const {
    return {};
}

void CmdNode::bytecode(BCManager &man) {
    replaceConstants(man);
    if (primaryCmd() == "function")
        MCLError(ERR_WARN, "Direct function call insertion has undefined "
        "behaviour, use normal function call instead!", man.comp, loc);
    if (primaryCmd() == "execute") {
        std::string secondary;
        unsigned int i = 0;
        while (i < cmd.size() && processedCmd[i] != ' ')
            i++;
        i++;
        while (i < cmd.size() && processedCmd[i] != ' ')
            secondary.push_back(processedCmd[i]), i++;
        if (secondary != "store")
            MCLError(ERR_MINOR_WARN, "Direct usage of execute can be replaced "
            "by execute-statements.", man.comp, loc);
    }
    if (!isKnownCmd())
        MCLError(ERR_WARN, "The given command \"" + primaryCmd() + "\" is not "
        "known to be a valid command.", man.comp, loc);
    man.write(BCInstr(INSTR_CMD, processedCmd));
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool CmdNode::isKnownCmd() const {
    return knownCmds.count(primaryCmd()) > 0;
}

std::string CmdNode::primaryCmd() const {
    std::string out;
    unsigned int i = 0;
    while (i < cmd.size() && processedCmd[i] != ' ')
        out.push_back(processedCmd[i]), i++;
    return out;
}

void CmdNode::replaceConstants(const BCManager &man) {
    processedCmd = replaceInsertedValues(cmd, man.ctx.getConstValues());
}