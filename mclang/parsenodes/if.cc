
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/if.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

IfNode::IfNode(std::vector<ParseNode *> args, std::vector<CodeBlockNode *>
codeblocks, Loc loc) : ParseNode(PNODE_IF, loc), ifArgs(args),
codeblocks(codeblocks) {

}

std::vector<ParseNode *> IfNode::children() const {
    std::vector<ParseNode *> out = ifArgs;
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        out.push_back(codeblocks[i]);
    return out;
}

IfNode::~IfNode() {
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        delete codeblocks[i];
}

void IfNode::bytecode(BCManager &man) {
    if (codeblocks.size() == 1) {
        bytecodeSingleIf(man);
        return;
    }
    tmpId = man.tmp.reserve();
    tmpCond = "if score " + tmpId + " " + man.comp->scoreboardName +
    " matches 0";
    man.write(BCInstr(INSTR_SET, tmpId, "0"));
    bytecodeTailIfs(man);
    man.tmp.free(tmpId);
    man.ret = {Type("void"), ""};
    man.controlFlowCheck();
}

bool IfNode::hasElse() const {
    return ifArgs.size() < codeblocks.size();
}

void IfNode::bytecodeSingleIf(BCManager &man) {
    ifArgs[0]->bytecode(man);
    man.ctx.push();
    Return argRet = man.ret;
    if (argRet.type == Type("const str")) {
        man.addFunc();
        std::string callname = man.topFunc()->name;
        codeblocks[0]->bytecode(man);
        man.popFunc();
        man.write(BCInstr(INSTR_EXEC_CALL, "if " + argRet.value, callname));
    } else if (argRet.type == Type("int") || argRet.type == Type("bool")) {
        man.addFunc();
        std::string callName = man.topFunc()->name;
        codeblocks[0]->bytecode(man);
        man.popFunc();
        man.write(BCInstr(INSTR_EXEC_CALL, "unless score " + argRet.value + " "
        + man.comp->scoreboardName + " matches 0", callName));
    } else if (argRet.type == Type("const int") || argRet.type ==
    Type("const bool")) {
        if (argRet.value != "0")
            codeblocks[0]->bytecode(man);
    }
    man.ctx.pop();
    man.controlFlowCheck();
}

void IfNode::bytecodeTailIfs(BCManager &man, unsigned int index) {
    // Check if the current statement is an "else" or "else if"
    if (index >= ifArgs.size()) {
        codeblocks[index]->bytecode(man);
        return;
    }
    ifArgs[index]->bytecode(man);
    Return argRet = man.ret;
    man.addFunc();
    std::string codeblockName = man.topFunc()->name;
    // This is not the last statement
    if (index + 1 < codeblocks.size())
        man.write(BCInstr(INSTR_SET, tmpId, "1"));
    codeblocks[index]->bytecode(man);
    man.popFunc();
    bytecodeCondCall(man, argRet, codeblockName);
    if (index + 1 < codeblocks.size()) {
        man.addFunc();
        std::string nextIfName = man.topFunc()->name;
        bytecodeTailIfs(man, index + 1);
        man.popFunc();
        man.write(BCInstr(INSTR_EXEC_CALL, tmpCond, nextIfName));
    }
}

void IfNode::bytecodeCondCall(BCManager &man, Return argRet,
std::string callName) {
    if (argRet.type == Type("const str")) {
        man.write(BCInstr(INSTR_EXEC_CALL, "if " + argRet.value, callName));
    } else if (argRet.type == Type("int") || argRet.type == Type("bool")) {
        man.write(BCInstr(INSTR_EXEC_CALL, "unless score " + argRet.value + " "
        + man.comp->scoreboardName + " matches 0", callName));
    } else {
        MCLError("Invalid type \"" + argRet.type.str() + "\" as if-statement "
        "condition", loc);
    }
}