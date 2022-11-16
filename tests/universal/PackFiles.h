#ifndef PACK_FILES_H
#define PACK_FILES_H

#include "Datapack.h"
#include "utils.h"
#include <string>

struct PackFiles {
    Datapack dp;
    std::string name;
    Datapack::Namespace namesp;
    Datapack::BuildResult rez;

    // builds the Datapack into PackFiles
    //  optional -n argument for namespace
    //  saves command result to Rez
    //  places file at `makeSourcePath(name)`
    //  outputs datapack to `makeCompiledPath(name)`
    PackFiles(Datapack _dp, std::string n = NO_NAMESPACE);


/***************************************|
|                                       |
|             Util Mirrors              |
|                                       |
|***************************************/
    /***************************************|
    |                                       |
    |           Path Generators             |
    |                                       |
    |***************************************/
    // get location of `.mcl` file
    std::string _makeSourcePath() const {
        return makeSourcePath(name);
    }
    // get location of datapack
    std::string _makeCompiledPath() const {
        return makeCompiledPath(name);
    }
    // get location of datapack
    std::string _makePythonPath() const {
        return makePythonPath(name);
    }
    // get location of function .json files (for load, tick)
    std::string _makeTagsPath() const {
        return makeTagsPath(name);
    }
    // get location of generated `.mcfunction` files
    std::string _makeFunctionsPath() const {
        return makeFunctionsPath(name, namesp);
    }
    /***************************************|
    |                                       |
    |              MC Functions             |
    |                                       |
    |***************************************/
    bool _MCFunctionExists(std::string function) const {
        return MCFunctionExists(name, namesp, function);
    }

    // lists the function names, not paths, no .mcfunction, just ["1", "2", "main", "tick", etc...]
    std::vector<std::string> _listMCFunctionNames() const {
        return listMCFunctionNames(name, namesp);
    }

    std::string _getMCFunction(std::string function) const {
        return getMCFunction(name, namesp, function);
    }

    bool _anyFunctionContain(std::string content) const {
        return anyFunctionContain(name, namesp, content);
    }

/***************************************|
|                                       |
|                 GTests                |
|                                       |
|***************************************/
    // run all tests associated with a successful build
    void VERIFY_GOOD_BUILD() const;

    // verify build result indicates success
    void VERIFY_BUILD_RESULT_SUCCESS() const;

    // verify file structure
    void VERIFY_FILE_STRUCTURE() const;

    // verify function files are created
    void VERIFY_FUNCTION_FILES() const;

    // verify function files are created
    void VERIFY_TAGGED_FUNCT(const Datapack::Function& f, std::string TAG_PATH) const;

    // verify build result indicates failure
    //  - without successful return code
    //  - with output containing `err`
    //  - datapack folder not created
    void VERIFY_BAD_BUILD(const Datapack& src, std::string err = "") const;

private:
    // cleans up the pack files
    void _cleanupBuildFiles();
public:
    // destructor cleans up files
    ~PackFiles() {
        _cleanupBuildFiles();
    }
};

#include "PackFiles.cpp"

#endif