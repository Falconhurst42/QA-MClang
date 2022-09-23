
#ifndef __PARSENODE_COMMA_H__
#define __PARSENODE_COMMA_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>

class BCManager;

class CommaNode : public ParseNode {

public:

    /**
     * Constructor
     * @param childNodes The child expressions of the comma expression
     * @param loc The location of the parse node
     */
    CommaNode(std::vector<ParseNode *> childNodes, Loc loc);

    /**
     * Destructor
     */
    virtual ~CommaNode() override;

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

    // Child expressions of the comma expression
    std::vector<ParseNode *> childNodes;

};

#endif