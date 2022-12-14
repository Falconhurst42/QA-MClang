
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/relfiles.h"
#include "preprocess/preplexer.h"
#include "preprocess/preprocess.h"
#include "preprocess/preptoken.h"
#include "stdlib/stdlib.h"
#include <fstream>
#include <sstream>
#include <string>

Preprocessor::Preprocessor() : includeDepth(0), doOutput(true), curFile("") {

}

Preprocessor::~Preprocessor() {

}

void Preprocessor::processFile(std::string filename) {
    std::string tmp = curFile;
    std::ifstream file(filename);
    if (!file.is_open())
        MCLError("Could not open given file \"" + filename + "\"");
    curFile = filename;
    std::stringstream contentStream;
    contentStream << file.rdbuf();
    std::string content = contentStream.str();
    tokenStack.push_back(std::vector<PrepToken>());
    readIndexStack.push_back(0);
    PrepLexer lex(content, filename, tokenStack.back());
    lex.lex();
    readProgram();
    tokenStack.pop_back();
    readIndexStack.pop_back();
    curFile = tmp;
}

std::vector<PrepToken> &Preprocessor::getOutput() const {
    return (std::vector<PrepToken> &)out;
}

void Preprocessor::processString(const std::string &content, std::string
altFilename) {
    std::string tmp = curFile;
    curFile = "??STDLIB";
    tokenStack.push_back(std::vector<PrepToken>());
    readIndexStack.push_back(0);
    PrepLexer lex(content, altFilename, tokenStack.back());
    lex.lex();
    readProgram();
    tokenStack.pop_back();
    readIndexStack.pop_back();
    curFile = tmp;
}

std::vector<PrepToken> &Preprocessor::curTokenList() const {
    return (std::vector<PrepToken> &)tokenStack.back();
}

bool Preprocessor::atEnd() const {
    return readIndexStack.back() >= curTokenList().size();
}

PrepToken Preprocessor::cur() const {
    if (atEnd()) {
        Loc lastLoc = tokenStack.back().back().loc;
        return PrepToken(PTOK_EOF, lastLoc);
    }
    return tokenStack.back()[readIndexStack.back()];
}

void Preprocessor::next() {
    readIndexStack.back()++;
}

void Preprocessor::expect(PrepTokenType type) {
    if (cur().type != type)
        MCLError("Unexpected token.", cur().loc);
}

void Preprocessor::expect(PrepTokenType type, std::string content) {
    if (cur().type != type || cur().content != content)
        MCLError("Unexpected token.", cur().loc);
}

void Preprocessor::write(PrepToken tok) {
    if (doOutput)
        out.push_back(tok);
}

void Preprocessor::readProgram() {
    doOutput = true;
    readCodeBlock();
    if (!atEnd())
        MCLError("Stopped reading before EOF.", cur().loc);
}

void Preprocessor::readCodeBlock() {
    while (!atEnd()) {
        if (cur().type == PTOK_IDENT)
            readIdent();
        else if (cur().type == PTOK_PREP_STMT)
            readPrepStmt();
        else if (cur().type == PTOK_INCL_LIB)
            MCLError("Unexpected include library.", cur().loc);
        else
            write(cur()), next();
    }
}

void Preprocessor::readIdent() {
    expect(PTOK_IDENT);
    std::vector<PrepToken> tmp = convertIdent(cur());
    for (unsigned int i = 0; i < tmp.size(); i++)
        write(tmp[i]);
    next();
}

void Preprocessor::readPrepStmt() {
    expect(PTOK_PREP_STMT);
    std::string stmt = cur().content;
    if (stmt == "include")
        readInclude();
    else if (stmt == "define")
        readDefine();
    else
        MCLError("Unrecognized statement \"" + stmt + "\".", cur().loc);
}

void Preprocessor::readInclude() {
    expect(PTOK_PREP_STMT, "include"), next();
    if (cur().type == PTOK_STR) {
        processFile(getReferencePath(curFilename(), cur().content));
    } else if (cur().type == PTOK_INCL_LIB) {
        std::string name = cur().content;
        if (stdlibContent.count(name) == 0)
            MCLError("Invalid standard library name.", cur().loc);
        processString(stdlibContent.find(name)->second, "<" + name + ">");
    } else {
        MCLError("Expected include file.", cur().loc);
    }
    // Skip the include file text
    next();
    expect(PTOK_ENDL), next();
}

std::vector<PrepToken> Preprocessor::convertIdent(PrepToken tok) const {
    if (tok.type == PTOK_IDENT && defs.count(tok.content) > 0)
        return defs.find(tok.content)->second;
    return {tok};
}

void Preprocessor::readDefine() {
    expect(PTOK_PREP_STMT, "define"), next();
    // Indentifier to define
    expect(PTOK_IDENT);
    std::string name = cur().content;
    next();
    std::vector<PrepToken> replace;
    while (!atEnd() && cur().type != PTOK_ENDL) {
        std::vector<PrepToken> tmp = convertIdent(cur());
        replace.insert(replace.end(), tmp.begin(), tmp.end());
        next();
    }
    if (replace.empty())
        MCLError("Empty definition", cur().loc);
    defs.insert({name, replace});
    // Skip end of line
    next();
}

std::string Preprocessor::curFilename() const {
    return curFile;
}