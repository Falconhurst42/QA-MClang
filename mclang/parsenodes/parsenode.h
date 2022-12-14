
#ifndef __PARSENODE_H__
#define __PARSENODE_H__

#include "general/loc.h"
#include <vector>

class BCManager;

enum ParseNodeType {
    // Error type
    PNODE_ERRTYPE,
    // Main program node
    PNODE_PROGRAM,
    // Function definition
    PNODE_FUNC,
    // Inserted commands
    PNODE_CMD,
    // Blocks of code, to group commands
    PNODE_CODEBLOCK,
    // Execute-statement
    PNODE_EXEC_STMT,
    // If-statement
    PNODE_IF,
    // Function call
    PNODE_CALL,
    // Namespace setting
    PNODE_NAMESPACE,
    // Expressions
    PNODE_ADD, PNODE_SUB, PNODE_MUL, PNODE_DIV, PNODE_MOD, PNODE_ASSIGN_ADD,
    PNODE_ASSIGN_SUB, PNODE_ASSIGN_MUL, PNODE_ASSIGN_DIV, PNODE_ASSIGN_MOD,
    PNODE_ASSIGN,
    // Increment/decrement
    PNODE_INC_PREFIX, PNODE_INC_SUFFIX, PNODE_DEC_PREFIX, PNODE_DEC_SUFFIX,
    // Unary operators
    PNODE_UNARY_PLUS, PNODE_UNARY_MINUS,
    // Variable initialization
    PNODE_VARINIT,
    // Global variable
    PNODE_GLOBALVAR,
    // Variable name/word and literals
    PNODE_WORD, PNODE_NUM, PNODE_STR,
    // Comparison operators
    PNODE_LT, PNODE_LTE, PNODE_GT, PNODE_GTE, PNODE_EQ, PNODE_NEQ,
    // Logical operators
    PNODE_NOT, PNODE_AND, PNODE_OR,
    // Loops
    PNODE_LOOP,
    // Comma expression
    PNODE_COMMA,
    // Return statement
    PNODE_RETURN
};

class ParseNode {

public:

    /**
     * Constructor
     * @param type The node type
     * @param loc The location of the parse node
     */
    ParseNode(ParseNodeType type, Loc loc);

    /**
     * Destructor
     */
    virtual ~ParseNode();

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const = 0;

    /**
     * Get the type of the parse node
     * @return The type of this parse node
     */
    ParseNodeType getType() const;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) = 0;

    /**
     * Get the location of the parse node
     * @return The location as a "Loc" type
     */
    Loc getLoc() const;

protected:

    // Node type
    ParseNodeType type;

    // The location of the parse node
    Loc loc;

};

#endif