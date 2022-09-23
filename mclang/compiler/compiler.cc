
#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "bcconvert/bcconvert.h"
#include "bcconvert/debug.h"
#include "compiler/compiler.h"
#include "filemanager/manager.h"
#include "general/strmanip.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include "preprocess/debug.h"
#include "preprocess/preprocess.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>

Compiler::Compiler() : filename(""), ns("dp"), outputFolder("out_datapack"),
debugMode(false), fileOutput(true), scoreboardName("mclang"),
mcVersion("latest"), description(""), warnSetting(WARNSETTING_MAJOR),
showStatistics(false), prep(nullptr), lexer(nullptr), parser(nullptr),
bcMan(nullptr), bcConvert(nullptr) {
    
}

Compiler::~Compiler() {
    delete prep;
    delete lexer;
    delete parser;
    delete bcMan;
    delete bcConvert;
}

void Compiler::compile() {
    resetStatistics();
    runPreprocessor();
    runLexer();
    runParser();
    runBCGenerator();
    runBCConverter();
    runFileOutput();
    if (showStatistics)
        displayStatistics();
}

void Compiler::runPreprocessor() {
    prep = new Preprocessor();
    prep->processFile(filename);
    if (debugMode) {
        std::ofstream out("mcl_prep.debug");
        out << prepDebugTable(&prep->getOutput());
        out.close();
    }
}

void Compiler::runLexer() {
    lexer = new Lexer(this);
    lexer->readIn();
    if (debugMode) {
        std::ofstream out("mcl_lexer.debug");
        out << lexerDebugTable(lexer->tokens());
        out.close();
    }
}

void Compiler::runParser() {
    parser = new Parser(this);
    parser->genTree();
    if (debugMode) {
        std::ofstream out("mcl_parser.debug");
        out << parserDebugTree(parser->getTree());
        out.close();
    }
}

void Compiler::runBCGenerator() {
    bcMan = new BCManager(this);
    bcMan->generate();
    if (debugMode) {
        std::ofstream out("mcl_bcgen.debug");
        out << bcgenInstrList(bcMan->getBytecode());
        out.close();
    }
}

void Compiler::runBCConverter() {
    bcConvert = new BCConverter(this);
    cmds = bcConvert->getRawCommands();
    // Update statistics
    for (const CmdFunc &func : cmds)
        stats.cmdCount += func.cmdList.size();
    stats.cmdFunctionCount = cmds.size();
    if (debugMode) {
        std::ofstream out("mcl_cmds.debug");
        out << bcconvertCmdList(cmds);
        out.close();
    }
}

void Compiler::runFileOutput() {
    if (fileOutput) {
        FileManager fm(this);
        fm.genDatapack(cmds);
    }
}

void Compiler::resetStatistics() {
    stats.bcInstrCount = 0, stats.cmdCount = 0, stats.cmdFunctionCount = 0,
    stats.funcCallCount = 0, stats.stackOpCount = 0;
}

void Compiler::displayStatistics() const {
    displayStatistic("Bytecode instructions", stats.bcInstrCount);
    displayStatistic("Minecraft commands", stats.cmdCount);
    displayStatistic("Generated Minecraft functions", stats.cmdFunctionCount);
    displayStatistic("Minecraft function calls", stats.funcCallCount);
    displayStatistic("Stack operations", stats.stackOpCount);
}

void Compiler::displayStatistic(std::string txt, unsigned int num) const {
    txt.append(": ");
    while (txt.size() < COMPILER_STAT_LEFTPAD)
        txt.push_back(' ');
    std::cout << txt << thousandsSep(num) << std::endl;
}