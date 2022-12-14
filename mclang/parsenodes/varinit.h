
#ifndef __PARSENODE_VARINIT_H__
#define __PARSENODE_VARINIT_H__

#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/parsenode.h"
#include <vector>
#include <string>

class BCManager;
class ExprNode;

class VarInitNode : public ParseNode {

public:

    /**
     * Constructor
     * @param varType The variable type to make the variable
     * @param childExpr Child node of the initialization, which should be an
     * assignment, or just the name of the variable
     * @param loc The location of the parse node
     */
    VarInitNode(Type varType, ParseNode *childExpr, Loc loc);

    /**
     * Destructor
     */
    virtual ~VarInitNode() override;

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

protected:

    // The type of the initialized variable
    Type varType;

    // The child expression of this node, which should be an assignment
    // expression or a comma expression
    ParseNode *childExpr;

    // List of child expressions, in case of comma expression
    std::vector<ParseNode *> childExprList;

private:

    /**
     * Generate bytecode for this parse node, given it is a constant
     * initialization
     * @param man The main bytecode manager
     * @param child expression to use for initialization
     */
    void constBytecode(BCManager &man, ParseNode *child);

    /**
     * Generate bytecode for this parse node, given it is a non-constant
     * initialization
     * @param man The main bytecode manager
     * @param child expression to use for initialization
     */
    void nonConstBytecode(BCManager &man, ParseNode *child);

    /**
     * Check if a variable was already defined
     * @param varName The variable name
     * @param man The main bytecode manager
     * @return A boolean indicating if the variable is in the context stack
     */
    bool hasNameConflict(std::string varName, BCManager &man) const;

};

#endif