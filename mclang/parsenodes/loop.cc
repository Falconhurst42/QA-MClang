
#include "bcgen/bcgen.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/loop.h"
#include "parsenodes/parsenode.h"
#include <vector>

LoopNode::LoopNode(ParseNode *arg, CodeBlockNode *codeblock, Loc loc,
ParseNode *beforeArg, ParseNode *afterArg) : ParseNode(PNODE_LOOP, loc),
arg(arg), codeblock(codeblock), beforeArg(beforeArg), afterArg(afterArg) {

}

LoopNode::~LoopNode() {

}

std::vector<ParseNode *> LoopNode::children() const {
    std::vector<ParseNode *> out = {arg, codeblock};
    if (beforeArg != nullptr)
        out.push_back(beforeArg);
    if (afterArg != nullptr)
        out.push_back(afterArg);
    return out;
}

void LoopNode::bytecode(BCManager &man) {
    man.ctx.push();
    bytecodeBefore(man);
    // Loop condition function
    man.addFunc();
    loopCondFunc = man.topFunc();
    // Loop code block function
    man.addFunc();
    blockFunc = man.topFunc();
    man.ctx.push();
    bytecodeCodeblock(man);
    man.ctx.pop();
    man.write(BCInstr(INSTR_EXEC_CALL, "if score 0ret " +
    man.comp->scoreboardName + " matches 0", loopCondFunc->name));
    man.popFunc();
    bytecodeAfter(man);
    bytecodeCondition(man);
    man.popFunc();
    bytecodeCondition(man);
    man.ctx.pop();
    man.controlFlowCheck();
}

void LoopNode::bytecodeBefore(BCManager &man) {
    if (beforeArg == nullptr)
        return;
    beforeArg->bytecode(man);
}

void LoopNode::bytecodeCodeblock(BCManager &man) {
    codeblock->bytecode(man);
}

void LoopNode::bytecodeCondition(BCManager &man) {
    arg->bytecode(man);
    if (man.ret.type == Type("const str")) {
        man.write(BCInstr(INSTR_EXEC_CALL, "if " + man.ret.value,
        blockFunc->name));
    } else if (man.ret.type == Type("bool") || man.ret.type == Type("int")) {
        man.write(BCInstr(INSTR_EXEC_CALL, "unless score " + man.ret.value + " "
        + man.comp->scoreboardName + " matches 0", blockFunc->name));
    } else if (man.ret.type == Type("const bool") || man.ret.type ==
    Type("const int")) {
        // If it evaluates to false, we don't need a call
        if (man.ret.value != "0")
            man.write(BCInstr(INSTR_CALL, blockFunc->name));
    } else {
        MCLError("Loop condition of type \"" + man.ret.type.str() + "\" is not "
        "allowed.", loc);
    }
}

void LoopNode::bytecodeAfter(BCManager &man) {
    if (afterArg == nullptr)
        return;
    afterArg->bytecode(man);
}