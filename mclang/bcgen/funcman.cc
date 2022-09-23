
#include "bcgen/funcman.h"
#include "general/types.h"
#include "parsenodes/func.h"
#include <string>
#include <unordered_map>
#include <vector>

FuncManager::FuncManager() {

}

FuncManager::~FuncManager() {

}

std::vector<FuncNode *> FuncManager::getFuncs() const {
    std::vector<FuncNode *> out;
    for (const std::pair<std::string, FuncNode *> &func : funcs)
        out.push_back(func.second);
    return out;
}

void FuncManager::push(FuncNode *func) {
    funcs.insert({func->getName(), func});
}

FuncNode *FuncManager::find(std::string name, std::vector<Type> types) {
    auto result = funcs.equal_range(name);
    for (auto it = result.first; it != result.second; it++)
        if (it->second->acceptTypes(types))
            return it->second;
    return nullptr;
}