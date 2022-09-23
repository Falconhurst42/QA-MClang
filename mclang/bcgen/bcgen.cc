
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include "parser/parser.h"
#include <string>
#include <vector>

BCFunc::BCFunc(std::string name) : name(name) {

}

BCManager::BCManager(Compiler *comp) : comp(comp), curFuncNode(nullptr),
uniqueFuncId(1), writeStackCount(0) {

}

BCManager::~BCManager() {
    for (unsigned int i = 0; i < funcList.size(); i++)
        delete funcList[i];
}

void BCManager::generate() {
    comp->parser->getTree()->bytecode(*this);
    genGlobalVarInit();
}

const std::vector<BCFunc *> *BCManager::getBytecode() const {
    return &funcList;
}

BCFunc *BCManager::curFunc() const {
    if (funcStack.size() == 0)
        return nullptr;
    return funcStack.back();
}

void BCManager::write(BCInstr instr) {
    if (!canWrite())
        return;
    comp->stats.bcInstrCount++;
    if (instr.type == INSTR_PUSH || instr.type == INSTR_POP
    || instr.type == INSTR_TOP)
        comp->stats.stackOpCount++;
    if (instr.type == INSTR_CALL)
        comp->stats.funcCallCount++;
    if (funcStack.size() == 0)
        return;
    funcStack.back()->instrList.push_back(instr);
}

void BCManager::addFunc(std::string name) {
    if (name == "")
        name = std::to_string(uniqueFuncId++);
    BCFunc *f = new BCFunc(name);
    funcList.push_back(f);
    funcStack.push_back(f);
}

void BCManager::popFunc(unsigned int index) {
    if (funcStack.size() <= index)
        return;
    for (unsigned int i = funcStack.size() - index; i < funcStack.size(); i++)
        funcStack[i - 1] = funcStack[i];
    funcStack.pop_back();
}

BCFunc *BCManager::topFunc() const {
    return funcStack.back();
}

void BCManager::setFuncStack(std::vector<BCFunc *> stack) {
    funcStack = stack;
}

void BCManager::pushWriteStack(bool val) {
    writeStack.push_back(val);
    if (!val)
        writeStackCount++;
}

void BCManager::setTopWriteStack(bool val) {
    if (writeStack.empty())
        return;
    writeStackCount += writeStack.back() - val;
    writeStack.back() = val;
}

void BCManager::popWriteStack() {
    if (writeStack.empty())
        return;
    if (!writeStack.back())
        writeStackCount--;
    writeStack.pop_back();
}

bool BCManager::canWrite() const {
    return writeStackCount == 0;
}

void BCManager::controlFlowCheck(std::string cond) {
    addFunc();
    BCFunc *newFunc = topFunc();
    popFunc();
    if (cond == "")
        write(BCInstr(INSTR_CFC, newFunc->name));
    else
        write(BCInstr(INSTR_EXEC_CALL, "if " + cond, newFunc->name));
    popFunc();
    funcStack.push_back(newFunc);
}

void BCManager::genGlobalVarInit() {
    BCFunc *loadFunc = nullptr;
    for (BCFunc *func : funcList)
        if (func->name == "load")
            loadFunc = func;
    if (loadFunc == nullptr) {
        addFunc("load");
        loadFunc = topFunc();
        popFunc();
    }
    std::vector<BCInstr> globalInits;
    // NOTE: This size is an over-estimation
    globalInits.reserve(ctx.getVars().size() + 2);
    globalInits.push_back(BCInstr(INSTR_ADDI, "0ret", "0"));
    globalInits.push_back(BCInstr(INSTR_ADDI, "0retv", "0"));
    for (const Var &var : ctx.getVars())
        if (var.type == Type("int") || var.type == Type("bool"))
            globalInits.push_back(BCInstr(INSTR_ADDI, var.name, "0"));
    loadFunc->instrList.insert(loadFunc->instrList.begin(), globalInits.begin(),
    globalInits.end());
}