
#ifndef __PARSENODE_ASSIGN_H__
#define __PARSENODE_ASSIGN_H__

#include "bcgen/instr.h"
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include <string>
#include <vector>

class BCManager;

/**
 * Assignment operators, which are =, +=, -=, *=, /= and %=
 */
class AssignNode : public ExprNode {

public:

    /**
     * Constructor
     * @param type The parse node type, has to be an assignment
     * @param varName The variable name at the left of the expression
     * @param expr The expression to assign to the variable
     * @param loc The location of the parse node
     */
    AssignNode(ParseNodeType type, std::string varName, ParseNode *expr,
    Loc loc);

    /**
     * Destructor
     */
    virtual ~AssignNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

    /**
     * Get the name of the variable being assigned
     * @return The name as a string
     */
    std::string getVarName() const;

    /**
     * Get the assigned expression parse node
     * @return The pointer stored in `left`
     */
    ParseNode *getExpr() const;

private:

    /**
     * Generate bytecode for a normal assignment
     * @param man The main bytecode manager
     */
    void bytecodeNormal(BCManager &man);

    /**
     * Generate bytecode for an arithmatics assignment, e.g. +=, *=
     * @param man The main bytecode manager
     */
    void bytecodeArith(BCManager &man);

    /**
     * Throw an error for unsupported return types
     * @param man The main bytecode manager
     * @note Uses `man.ret` to determine expression type
     */
    void invalidTypeError(BCManager &man) const;

    /**
     * Check if the variable to be assigned is initialized
     * @param man The main bytecode manager
     * @param varType The variable type will be put in this variable
     * @return Boolean indicating if the variable was found
     */
    bool isInitialized(BCManager &man, Type &varType) const;

    /**
     * Get the non-constant bytecode arithmatic operation associated with this
     * assignment node
     * @return The non-constant arithmatic instruction type
     */
    BCInstrType getArithInstr();

    /**
     * Get the constant bytecode arithmatic operation associated with this
     * assignment node
     * @return The constant arithmatic instruction type
     */
    BCInstrType getConstArithInstr();

    // The name of the variable
    std::string varName;

    // The type of the variable, found when generating bytecode
    Type varType;

    // NOTE: Expression can be accessed with `left` pointer

};

#endif