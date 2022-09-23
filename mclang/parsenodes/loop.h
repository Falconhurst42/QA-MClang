
#ifndef __PARSENODE_LOOP_H__
#define __PARSENODE_LOOP_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>

class BCFunc;
class BCManager;
class CodeBlockNode;

/**
 * Parse node for while and for-loops
 */
class LoopNode : public ParseNode {

public:

    /**
     * Constructor
     * @param arg The main loop argument, is checked for at the start of every
     * loop
     * @param codeblock Code inside the loop
     * @param loc The location of the parse node
     * @param beforeArg Code to execute before entering the loop, but still
     * inside the loop context, may be null
     * @param afterArg Code to execute after every loop, may be null
     */
    LoopNode(ParseNode *arg, CodeBlockNode *codeblock, Loc loc,
    ParseNode *beforeArg = nullptr, ParseNode *afterArg = nullptr);

    /**
     * Destructor
     */
    virtual ~LoopNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

private:

    /**
     * Generate bytecode for before statement
     * @param man The main bytecode manager
     */
    void bytecodeBefore(BCManager &man);

    /**
     * Generate bytecode for the codeblock in the loop
     * @param man The main bytecode manager
     */
    void bytecodeCodeblock(BCManager &man);

    /**
     * Generate bytecode for the condition to check and loop call
     * @param man The main bytecode manager
     */
    void bytecodeCondition(BCManager &man);

    /**
     * Generate bytecode for after statement
     * @param man The main bytecode manager
     */
    void bytecodeAfter(BCManager &man);

    // Main loop argument
    ParseNode *arg;

    // Code inside the loop
    CodeBlockNode *codeblock;

    // Code executed before the loop starts, in the loop context
    ParseNode *beforeArg;

    // Code executed after every loop
    ParseNode *afterArg;

    // Content of the statement blocks
    std::vector<CodeBlockNode *> codeblocks;

    // Loop condition and code block bytecode functions
    BCFunc *loopCondFunc, *blockFunc;

};

#endif