
#ifndef __PARSENODE_RETURN_H__
#define __PARSENODE_RETURN_H__

#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class ReturnNode : public ParseNode {

public:

    /**
     * Constructor
     * @param childExpr The expression after the return statement, may be null
     * @param loc The location of the parse node
     */
    ReturnNode(ParseNode *childExpr, Loc loc);

    /**
     * Destructor
     */
    virtual ~ReturnNode() override;

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
     * Display an error for having the invalid type returned for the return type
     * given by the function
     * @param givenType The type returned by the child expression
     * @note Assumes that `expectedType` is set
     */
    void invalidTypeError(Type givenType) const;

    // The child expression of the return statement, may be null
    ParseNode *childExpr;

    // The expected type of the child expression, based on the given return type
    // of the function
    Type expectedType;

};

#endif