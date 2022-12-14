
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parsenodes/call.h"
#include "parsenodes/cmd.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/exec.h"
#include "parsenodes/expr/arith.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/expr/bool.h"
#include "parsenodes/expr/comma.h"
#include "parsenodes/expr/compare.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/expr/increment.h"
#include "parsenodes/expr/logic.h"
#include "parsenodes/expr/num.h"
#include "parsenodes/expr/str.h"
#include "parsenodes/expr/unary.h"
#include "parsenodes/func.h"
#include "parsenodes/globalvar.h"
#include "parsenodes/if.h"
#include "parsenodes/loop.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include "parsenodes/return.h"
#include "parsenodes/varinit.h"
#include "parsenodes/word.h"
#include "parser/parser.h"
#include <iostream>
#include <string>
#include <vector>

Parser::Parser(Compiler *comp) : comp(comp), toks(nullptr), curIndex(0),
out(nullptr) {

}

Parser::~Parser() {
    delete out;
}

void Parser::genTree() {
    toks = comp->lexer->tokens();
    curIndex = 0;
    delete out;
    ParseNode *tmp = readInProgram();
    if (curIndex != toks->size())
        MCLError("Stopped reading before EOF.", cur().loc);
    out = tmp;
}

ParseNode *Parser::getTree() {
    return out;
}

Token Parser::cur() const {
    if (curIndex >= toks->size())
        return Token(TOK_ERRTYPE);
    return (*toks)[curIndex];
}

void Parser::next() {
    curIndex++;
}

void Parser::prev() {
    if (curIndex > 0)
        curIndex--;
}

bool Parser::accept(TokenType type) const {
    return cur().type == type;
}

void Parser::expect(TokenType type) {
    if (!accept(type))
        MCLError("Expected token '" + tokenTypeNames[(unsigned int)type]
        + "', instead got '" + tokenTypeNames[(unsigned int)cur().type] + "'.",
        cur().loc);
}

ParseNode *Parser::readInProgram() {
    Loc lastLoc = cur().loc;
    std::vector<ParseNode *> childNodes;
    while (true) {
        if (accept(TOK_TYPENAME) || accept(TOK_CONST))
            childNodes.push_back(readInDef());
        else if (accept(TOK_NAMESPACE))
            childNodes.push_back(readInNamespace());
        else
            break;
    }
    return new ProgramNode(childNodes, lastLoc);
}

ParseNode *Parser::readInDef() {
    Loc lastLoc = cur().loc;
    Type type = readInType();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    if (accept(TOK_LBRACE))
        return readInFunc(type, name, lastLoc);
    return readInGlobalVar(type, lastLoc);
}

ParseNode *Parser::readInGlobalVar(Type type, Loc lastLoc) {
    // To also read the name of the variable, we need to move one token back
    prev();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new GlobalVarNode(type, childExpr, lastLoc);
}

ParseNode *Parser::readInFunc(Type type, std::string funcName, Loc lastLoc) {
    expect(TOK_LBRACE), next();
    std::vector<Param> params;
    if (accept(TOK_TYPENAME) || accept(TOK_CONST)) {
        params.push_back(readInFuncParam());
        while (accept(TOK_COMMA)) {
            next();
            params.push_back(readInFuncParam());
        }
    }
    expect(TOK_RBRACE), next();
    expect(TOK_LCBRACE);
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new FuncNode(type, funcName, params, codeblock, lastLoc);
}

Param Parser::readInFuncParam() {
    Type type = readInType();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    return Param(type, name);
}

ParseNode *Parser::readInCodeBlock() {
    Loc lastLoc = cur().loc;
    std::vector<ParseNode *> childNodes;
    if (accept(TOK_LCBRACE)) {
        // Skip the '{'
        next();
        while (!accept(TOK_RCBRACE))
            childNodes.push_back(readInLine());
        // Skip the '}'
        next();
    } else {
        childNodes.push_back(readInLine());
    }
    return new CodeBlockNode(childNodes, lastLoc);
}

ParseNode *Parser::readInCmd() {
    Loc lastLoc = cur().loc;
    expect(TOK_CMD);
    ParseNode *tmp = new CmdNode(cur().content, lastLoc);
    next();
    return tmp;
}

ParseNode *Parser::readInLine() {
    if (accept(TOK_CMD))
        return readInCmd();
    if (accept(TOK_EXEC_STMT))
        return readInExec();
    if (accept(TOK_IF))
        return readInIf();
    if (accept(TOK_WHILE) || accept(TOK_FOR))
        return readInLoop();
    if (accept(TOK_TYPENAME) || accept(TOK_CONST))
        return readInVarInit();
    if (accept(TOK_RETURN))
        return readInReturn();
    // If there are no special tokens found, try to read an expression, and then
    // a semicolon
    ParseNode *expr = readInExpr();
    expect(TOK_SEMICOL), next();
    return expr;
}

ParseNode *Parser::readInExec() {
    Loc lastLoc = cur().loc;
    expect(TOK_EXEC_STMT);
    std::string execType = cur().content;
    next();
    expect(TOK_LBRACE), next();
    ParseNode *args = readInExpr();
    expect(TOK_RBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new ExecNode(execType, args, codeblock, lastLoc);
}

ParseNode *Parser::readInIf() {
    Loc lastLoc = cur().loc;
    std::vector<ParseNode *> ifArgs;
    std::vector<CodeBlockNode *> codeblocks;
    bool foundElse = false;
    do {
        // Skip the 'else'
        if (!accept(TOK_ELSE) && !accept(TOK_ELSEIF)) {
            // Expect an if-statement at the start
            expect(TOK_IF);
        }
        // "if", "else-if" or "else" statement
        if (accept(TOK_ELSEIF) || accept(TOK_IF)) {
            next();
            expect(TOK_LBRACE), next();
            ifArgs.push_back(readInExpr());
            expect(TOK_RBRACE), next();
        } else if (accept(TOK_ELSE)) {
            next();
            foundElse = true;
        } else {
            MCLError("Unexpected error unccured, invalid token found",
            cur().loc);
        }
        codeblocks.push_back((CodeBlockNode *)readInCodeBlock());
    } while ((accept(TOK_ELSEIF) || accept(TOK_ELSE)) && !foundElse);
    return new IfNode(ifArgs, codeblocks, lastLoc);
}

ParseNode *Parser::readInLoop() {
    Loc lastLoc = cur().loc;
    TokenType loopType = cur().type;
    next();
    expect(TOK_LBRACE), next();
    ParseNode *argBefore = nullptr, *arg = nullptr, *argAfter = nullptr;
    if (loopType == TOK_WHILE) {
        arg = readInExpr();
    } else {
        if (accept(TOK_SEMICOL)) {
            next();
        } else {
            if (accept(TOK_TYPENAME)) {
                argBefore = readInVarInit();
            } else {
                argBefore = readInExpr();
                expect(TOK_SEMICOL), next();
            }
        }
        if (accept(TOK_SEMICOL))
            arg = new BoolNode(true, cur().loc);
        else
            arg = readInExpr();
        expect(TOK_SEMICOL), next();
        if (!accept(TOK_RBRACE))
            argAfter = readInExpr();
    }
    expect(TOK_RBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new LoopNode(arg, codeblock, lastLoc, argBefore, argAfter);
}

ParseNode *Parser::readInExpr() {
    return readInComma();
}

ParseNode *Parser::readInComma() {
    Loc lastLoc = cur().loc;
    std::vector<ParseNode *> commaChildren = {readInAssign()};
    while (accept(TOK_COMMA)) {
        next();
        commaChildren.push_back(readInAssign());
    }
    if (commaChildren.size() == 1)
        return commaChildren.front();
    return new CommaNode(commaChildren, lastLoc);
}

ParseNode *Parser::readInAssign() {
    // Assignment is right associative
    Loc lastLoc = cur().loc;
    ParseNode *left = readInOr();
    ParseNodeType curType;
    if (acceptAssign(curType)) {
        next();
        ParseNode *right = readInAssign();
        if (left->getType() != PNODE_WORD)
            MCLError("Left side of assignment is not a variable name",
            lastLoc);
        std::string varName =  ((WordNode *)left)->getContent();
        delete left;
        return new AssignNode(curType, varName, right, lastLoc);
    }
    return left;
}

ParseNode *Parser::readInOr() {
    Loc lastLoc  = cur().loc;
    ParseNode *cur = readInAnd();
    while (accept(TOK_OR))
        next(), cur = new LogicNode(PNODE_OR, cur, readInAnd(), lastLoc);
    return cur;
}

ParseNode *Parser::readInAnd() {
    Loc lastLoc  = cur().loc;
    ParseNode *cur = readInEquality();
    while (accept(TOK_AND))
        next(), cur = new LogicNode(PNODE_AND, cur, readInEquality(), lastLoc);
    return cur;
}

ParseNode *Parser::readInEquality() {
    Loc lastLoc  = cur().loc;
    ParseNode *cur = readInInequality();
    while (accept(TOK_EQ) || accept(TOK_NEQ)) {
        ParseNodeType ptype = PNODE_EQ;
        if (accept(TOK_NEQ))
            ptype = PNODE_NEQ;
        next();
        cur = new CompareNode(ptype, cur, readInInequality(), lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInInequality() {
    Loc lastLoc  = cur().loc;
    ParseNode *cur = readInSum();
    while (accept(TOK_LT) || accept(TOK_LTE) || accept(TOK_GT)
    || accept(TOK_GTE)) {
        ParseNodeType ptype = PNODE_LT;
        if (accept(TOK_LTE))
            ptype = PNODE_LTE;
        if (accept(TOK_GT))
            ptype = PNODE_GT;
        if (accept(TOK_GTE))
            ptype = PNODE_GTE;
        next();
        cur = new CompareNode(ptype, cur, readInSum(), lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInSum() {
    // Sums are left associative
    Loc lastLoc = cur().loc;
    ParseNode *cur = readInProd();
    while (accept(TOK_ADD) || accept(TOK_SUB)) {
        ParseNodeType ptype = PNODE_ADD;
        if (accept(TOK_SUB))
            ptype = PNODE_SUB;
        next();
        ParseNode *right = readInProd();
        cur = new ArithNode(ptype, cur, right, lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInProd() {
    // Sums are left associative
    Loc lastLoc = cur().loc;
    ParseNode *cur = readInTerm();
    while (accept(TOK_MUL) || accept(TOK_DIV) || accept(TOK_MOD)) {
        ParseNodeType ptype = PNODE_MUL;
        if (accept(TOK_DIV))
            ptype = PNODE_DIV;
        if (accept(TOK_MOD))
            ptype = PNODE_MOD;
        next();
        ParseNode *right = readInTerm();
        cur = new ArithNode(ptype, cur, right, lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInTerm() {
    Loc lastLoc = cur().loc;
    TokenType curType = cur().type;
    if (curType == TOK_NOT) {
        next();
        return new LogicNode(PNODE_NOT, readInTerm(), nullptr, lastLoc);
    }
    if (curType == TOK_ADD || curType == TOK_SUB) {
        next();
        ParseNodeType ptype = curType == TOK_ADD ? PNODE_UNARY_PLUS :
        PNODE_UNARY_MINUS;
        return new UnaryNode(ptype, readInTerm(), lastLoc);
    }
    if (curType == TOK_INC || curType == TOK_DEC) {
        next();
        expect(TOK_WORD);
        std::string varName = cur().content;
        next();
        ParseNodeType ptype = curType == TOK_INC ? PNODE_INC_PREFIX :
        PNODE_DEC_PREFIX;
        return new IncrementNode(ptype, varName, lastLoc);
    }
    return readInCall();
}

ParseNode *Parser::readInCall() {
    Loc lastLoc = cur().loc;
    ParseNode *curout = readInBaseExpr();
    while (accept(TOK_LBRACE) || accept(TOK_INC) || accept(TOK_DEC)) {
        TokenType curType = cur().type;
        if (accept(TOK_LBRACE)) {
            next();
            if (curout->getType() != PNODE_WORD)
                MCLError("Expression is not callable.", lastLoc);
            std::string fname = ((WordNode *)curout)->getContent();
            delete curout;
            std::vector<ParseNode *> params;
            if (!accept(TOK_RBRACE))
                params.push_back(readInAssign());
            while (!accept(TOK_RBRACE)) {
                expect(TOK_COMMA), next();
                params.push_back(readInAssign());
            }
            expect(TOK_RBRACE), next();
            curout = new CallNode(fname, params, lastLoc);
        } else {
            next();
            if (curout->getType() != PNODE_WORD)
                MCLError("Expression cannot be incremented/decremented.",
                lastLoc);
            std::string varName = ((WordNode *)curout)->getContent();
            delete curout;
            ParseNodeType ptype = curType == TOK_INC ? PNODE_INC_SUFFIX :
            PNODE_DEC_SUFFIX;
            curout = new IncrementNode(ptype, varName, lastLoc);
        }
    }
    return curout;
}

ParseNode *Parser::readInBaseExpr() {
    Loc lastLoc = cur().loc;
    if (accept(TOK_NUM)) {
        std::string content = cur().content;
        next();
        return new NumNode(content, lastLoc);
    }
    if (accept(TOK_TRUE) || accept(TOK_FALSE)) {
        bool value = accept(TOK_TRUE);
        next();
        return new BoolNode(value, lastLoc);
    }
    if (accept(TOK_STR)) {
        std::string content = cur().content;
        next();
        return new StrNode(content, lastLoc);
    }
    if (accept(TOK_LBRACE)) {
        next();
        ParseNode *curout = readInExpr();
        expect(TOK_RBRACE), next();
        return curout;
    }
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    return new WordNode(name, lastLoc);
}

ParseNode *Parser::readInNamespace() {
    Loc lastLoc = cur().loc;
    expect(TOK_NAMESPACE), next();
    expect(TOK_WORD);
    std::string nsName = cur().content;
    next();
    expect(TOK_SEMICOL), next();
    return new NSNode(nsName, lastLoc);
}

ParseNode *Parser::readInVarInit() {
    Loc lastLoc = cur().loc;
    Type varType = readInType();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new VarInitNode(varType, childExpr, lastLoc);
}

ParseNode *Parser::readInReturn() {
    Loc lastLoc = cur().loc;
    expect(TOK_RETURN), next();
    ParseNode *childExpr = nullptr;
    if (!accept(TOK_SEMICOL))
        childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new ReturnNode(childExpr, lastLoc);
}

Type Parser::readInType() {
    Type curout;
    if (accept(TOK_CONST)) {
        curout.isConst = true;
        next();
    } else {
        curout.isConst = false;
    }
    expect(TOK_TYPENAME);
    if (cur().content == "void")
        curout.base = TYPE_VOID;
    if (cur().content == "int")
        curout.base = TYPE_INT;
    if (cur().content == "bool")
        curout.base = TYPE_BOOL;
    if (cur().content == "str")
        curout.base = TYPE_STR;
    next();
    return curout;
}

bool Parser::acceptAssign(ParseNodeType &ptype) const {
    ptype = PNODE_ERRTYPE;
    if (accept(TOK_ASSIGN))
        ptype = PNODE_ASSIGN;
    if (accept(TOK_ASSIGN_ADD))
        ptype = PNODE_ASSIGN_ADD;
    if (accept(TOK_ASSIGN_SUB))
        ptype = PNODE_ASSIGN_SUB;
    if (accept(TOK_ASSIGN_MUL))
        ptype = PNODE_ASSIGN_MUL;
    if (accept(TOK_ASSIGN_DIV))
        ptype = PNODE_ASSIGN_DIV;
    if (accept(TOK_ASSIGN_MOD))
        ptype = PNODE_ASSIGN_MOD;
    return ptype != PNODE_ERRTYPE;
}
