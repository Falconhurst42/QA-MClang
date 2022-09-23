
#ifndef __PARSENODE_IF_H__
#define __PARSENODE_IF_H__

#include "bcgen/return.h"
#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

class IfNode : public ParseNode {

public:

    /**
     * Constructor
     * @param args A list of if-statement arguments, if there is an else at the
     * end this list should have one less element than `codeblocks`, otherwise
     * it should have the same amount
     * @param codeblocks Code inside the statement blocks in the if-statements
     * @param loc The location of the parse node
     */
    IfNode(std::vector<ParseNode *> args, std::vector<CodeBlockNode *>
    codeblocks, Loc loc);

    /**
     * Destructor
     */
    virtual ~IfNode() override;

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

    /**
     * Check if this if-statement has an else-statement at the end or not
     * @return A boolean indicating if there is an else-statement at the end
     */
    bool hasElse() const;

private:

    /**
     * Generate the bytecode if there is only one if-statement and no "else if"
     * or "else"
     * @param man The main bytecode manager
     */
    void bytecodeSingleIf(BCManager &man);

    /**
     * Generate the bytecode from the "index" condition/block onwards, can also
     * generate bytecode for only the last "else if"/"else" statement
     * @param index The index to start from, default is 0
     * @note Assumes a temporary variable has already been created and `tmpId`
     * and `tmpCond` are set
     * @note Generates the condition check if the current function and generates
     * the code block in a separate function
     */
    void bytecodeTailIfs(BCManager &man, unsigned int index = 0);

    /**
     * Generate the bytecode for calling a given function, given the "Return" of
     * a condition
     * @param man The main bytecode manager
     * @param argRet The argument "Return" object
     * @param callName The function to call if the condition is met
     */
    void bytecodeCondCall(BCManager &man, Return argRet, std::string callName);

    // If-statement arguments
    std::vector<ParseNode *> ifArgs;

    // Content of the statement blocks
    std::vector<CodeBlockNode *> codeblocks;

    // Temporary ID used for keeping track if the if-statement should be
    // executed
    std::string tmpId;

    // Condition for checking if temporary ID variable is equal to 0
    std::string tmpCond;

    // Bytecode function name to call, used internally
    std::string fname;

};

#endif