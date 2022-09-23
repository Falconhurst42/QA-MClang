
#ifndef __PARSENODE_UNARY_H__
#define __PARSENODE_UNARY_H__

#include "bcgen/instr.h"
#include "bcgen/return.h"
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

class BCManager;

/**
 * Unary plus and minus
 */
class UnaryNode : public ExprNode {

public:

    /**
     * Constructor
     * @param type The unary node type
     * @param child The child node of the expression
     * @param loc The location of the parse node
     */
    UnaryNode(ParseNodeType type, ParseNode *child, Loc loc);

    /**
     * Destructor
     */
    virtual ~UnaryNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

private:

    // Return of the child expression, once found
    Return ret;

    /**
     * Throw an error for unsupported return types
     * @note Assumes `ret` is correct
     */
    void invalidTypeError() const;

};

#endif