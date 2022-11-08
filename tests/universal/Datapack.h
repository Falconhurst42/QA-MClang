#ifndef DATAPACK_H
#define DATAPACK_H

#include "utils.h"
#include <string>
#include <vector>
#include <stdexcept>


/*
Dev notes:
 - Util-mirrors will be named as `_nameOfMirroredFunction(args)`

*/

/***************************************|
|                                       |
|            DATAPACK CLASS             |
|                                       |
|***************************************/
struct Datapack {
/***************************************|
|                                       |
|              Sub-Types                |
|                                       |
|***************************************/
    struct Variable {
        std::string type, name;

        std::string getDeclaration() const;
    };

    struct Function {
        std::string rtype, name;
        std::vector<Variable> args;
        std::string body;

        Function(std::string rt, std::string na, std::vector<Variable> ar, std::string bo) :
            rtype(rt), name(na), args(ar), body(bo) {}

        std::string getText() const;
    };

    struct Namespace {
        std::string space;

        Namespace(std::string sp) : space(sp) {}

        std::string getNamespacer() const {
            return "namespace " + space + ";";
        }

        operator std::string() const { return space; }
    };

    struct BuildResult : CommandResult {
        std::string dash_n; // necessary to handle -n args

        BuildResult(CommandResult cr, std::string dn = NO_NAMESPACE) : CommandResult(cr), dash_n(dn) {}
        BuildResult(std::string ou, int co, std::string dn = NO_NAMESPACE) : CommandResult(ou, co), dash_n(dn) {}
    };

/***************************************|
|                                       |
|           Member Variables            |
|                                       |
|***************************************/
    std::string name;
    Namespace space;
    std::vector<Function> foos;
    std::vector<Variable> vars;
    BuildResult rez;            // code == -1 means no current build
                                // any other code means a build currently exists

/***************************************|
|                                       |
|              Constructor              |
|                                       |
|***************************************/
    Datapack(std::vector<Function> fs = std::vector<Function>(), 
        std::vector<Variable> vs = std::vector<Variable>(),
        Namespace sp = NO_NAMESPACE) :
            name(DEF_FILENAME),
            space(sp),
            foos(fs),
            vars(vs),
            rez("", -1, NO_NAMESPACE) {
                rez.code = -1;
            }
    
/***************************************|
|                                       |
|         Parsing & Generation          |
|                                       |
|***************************************/
    std::string generateText() const;

    // determines the effective namespace. Accounts for in-file namespace and dash_n args of last build
    std::string getPrimaryNamespace() const;
    
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
    // get location of function .json files (for load, tick)
    std::string _makeTagsPath() const {
        return makeTagsPath(name);
    }
    // get location of generated `.mcfunction` files
    std::string _makeFunctionsPath() const {
        return makeFunctionsPath(name, getPrimaryNamespace());
    }
    
    /***************************************|
    |                                       |
    |              MC Functions             |
    |                                       |
    |***************************************/
    inline bool _MCFunctionExists(std::string function) const {
        return MCFunctionExists(name, getPrimaryNamespace(), function);
    }

    // lists the function names, not paths, no .mcfunction
    inline std::vector<std::string> _listMCFunctionNames() const {
        return listMCFunctionNames(name, getPrimaryNamespace());
    }

    inline std::string _getMCFunction(std::string function) const {
        return getMCFunction(name, getPrimaryNamespace(), function);
    }

    inline bool _anyFunctionContain(std::string content) const {
        return anyFunctionContain(name, getPrimaryNamespace(), content);
    }

    /***************************************|
    |                                       |
    |                Cleanup                |
    |                                       |
    |***************************************/
    // cleans up a given build
    void _cleanupBuildFiles();

/***************************************|
|                                       |
|               Building                |
|                                       |
|***************************************/
    // builds the Datapack and returns whether the build returned a successful code
    //  saves command result to Rez
    //  places file at `makeSourcePath(name)`
    //  outputs datapack to `makeCompiledPath(name)`
    //  optional -n argument for namespace
    bool build(std::string n = NO_NAMESPACE);

/***************************************|
|                                       |
|              Destructor               |
|                                       |
|***************************************/
    ~Datapack() {
        _cleanupBuildFiles();
    }
};

#include "Datapack.cpp"

#endif