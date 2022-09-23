
#ifndef __PARSENODE_INCREMENT_H__
#define __PARSENODE_INCREMENT_H__

#include "bcgen/instr.h"
#include "bcgen/return.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

/**
 * Increment/decrement parse node
 */
class IncrementNode : public ParseNode {

public:

    /**
     * Constructor
     * @param type The increment/decrement node type, can be both suffix and
     * prefix
     * @param varName The variable to increment/decrement
     * @param loc The location of the parse node
     */
    IncrementNode(ParseNodeType type, std::string varName, Loc loc);

    /**
     * Destructor
     */
    virtual ~IncrementNode() override;

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

    // The variable name to increment/decrement
    std::string varName;

    // The type of the variable to increment/decrement
    Type varType;

    /**
     * Throw an error for unsupported variable type
     */
    void invalidTypeError() const;

    /**
     * Check if the current node is a prefix increment/decrement
     * @return A boolean indicating if the current node is a prefix
     */
    bool isPrefix() const;

    /**
     * Check if the current node is an increment
     * @return A boolean indicating if the current node is an increment, and not
     * a decrement
     */
    bool isIncrement() const;

};

#endif