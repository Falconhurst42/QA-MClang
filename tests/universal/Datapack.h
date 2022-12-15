#ifndef DATAPACK_H
#define DATAPACK_H

// Datapack.h
// Defines a class to model a MCLang datapack
//   handles the creation and file handling of a datapack
//   provides aliases for many `utils.h` functions which automatically feed in the information from the Datapack
// Structure:
//    Sub Types:            represent coponents of a datapack
//       Variable
//       Function
//       Namespace
//    Member Access/Edit:   Getters and Setters for member variables
//    Parsing & Generation: Parses datapack to text and generates a .mcl file to build from
//    Util Mirrors:         Aliases for `utils` functions with information from the datapack automatically fed in
//       Path Generators
//       MC Functions
//       Python Interfaces
//       Cleanup
//    Building:             Function to perform a build of the datapack
// Ethan Worth (created), 10/19/2022
// Ethan Worth (updated), 11/12/2022
// Ethan Worth (updated), 11/13/2022
// Ethan Worth (updated), 11/14/2022
// Ethan Worth (updated), 11/16/2022

#include "utils.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

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
        
        // Converts Variable info to a string
        // Return (string) - variable as text to be put in .mcl file
        std::string getDeclaration() const;
    };

    struct Function {
        std::string rtype, name;
        std::vector<Variable> args;
        std::string body;

        Function(std::string rt, std::string na, std::vector<Variable> ar, std::string bo) :
            rtype(rt), name(na), args(ar), body(bo) {}

        // Converts Function info to a string
        // Return (string) - function as text to be put in .mcl file
        std::string getText() const;

        // Formats name like MCLang would to create a .mcfunction file
        // Return (string) - formatted name
        std::string getFormattedName() const;
    };

    struct Namespace {
        std::string space;

        Namespace(std::string sp = NO_NAMESPACE) : space(sp) {}

        // Converts Namespace info to a string
        // Return (string) - namespace as text to be put in .mcl file
        std::string getNamespacer() const {
            return "namespace " + space + ";";
        }

        operator std::string() const { return space; }
    };

    struct BuildResult : CommandResult {
        std::string dash_n; // necessary to handle -n args

        BuildResult(CommandResult cr, std::string dn = NO_NAMESPACE) : CommandResult(cr), dash_n(dn) {}
        BuildResult(std::string ou = "", int co = -1, std::string dn = NO_NAMESPACE) : CommandResult(ou, co), dash_n(dn) {}
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
            vars(vs) {
                rez.code = -1;
            }

/***************************************|
|                                       |
|          Member Access/Edit           |
|                                       |
|***************************************/

// getter/setter for functions by name
// Arguments
//   _name (string)   - name of the function
// Returns (Function) - Function (by reference)
// State
//   properties of fucntion could be changed through the reference
Function& getFoo(std::string _name) {
    return *(std::find_if(foos.begin(), foos.end(), [_name](Function f){ return f.name == _name; }));
}

// getter/setter for variable by name
// Arguments
//   _name (string)   - name of the variable
// Returns (Variable) - Variable (by reference)
// State
//   properties of variable could be changed through the reference
Variable& getVar(std::string _name) {
    return *(std::find_if(vars.begin(), vars.end(), [_name](Variable v){ return v.name == _name; }));
}


// removes given function by name
// Arguments
//   _name (string)   - name of the function
// State
//   function will be removed from foos (if it exists)
void dropFoo(std::string _name) {
    foos.erase(std::find_if(foos.begin(), foos.end(), [_name](Function f){ return f.name == _name; }));
}

// removes given variable by name
// Arguments
//   _name (string)   - name of the variable
// State
//   variable will be removed from foos (if it exists)
void dropVar(std::string _name) {
    vars.erase(std::find_if(vars.begin(), vars.end(), [_name](Variable v){ return v.name == _name; }));
}

/***************************************|
|                                       |
|         Parsing & Generation          |
|                                       |
|***************************************/
    // Converts datapack info to text of a .mcl file
    // Returns (string) - text of a .mcl file which could be compiled
    std::string generateText() const;

    // determines the effective namespace. Accounts for in-file namespace and dash_n args of last build
    // Returns (string) - namespace
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
    // get location of `.mcl` file, alias for util
    // Return (string) - path to source file of this datapack
    std::string _makeSourcePath() const {
        return makeSourcePath(name);
    }
    // get location of datapack, alias for util
    // Return (string) - path to compiled datapack
    std::string _makeCompiledPath() const {
        return makeCompiledPath(name);
    }
    // get location of python, alias for util
    // Return (string) - path to python datapack
    std::string _makePythonPath() const {
        return makePythonPath(name);
    }
    // get location of function .json files (for load, tick), alias for util
    // Return (string) - path to tags directory of compiled datapack
    std::string _makeTagsPath() const {
        return makeTagsPath(name);
    }
    // get location of generated `.mcfunction` files, alias for util
    // Return (string) - path to functions directory of compiled datapack
    std::string _makeFunctionsPath() const {
        return makeFunctionsPath(name, getPrimaryNamespace());
    }
    
    /***************************************|
    |                                       |
    |              MC Functions             |
    |                                       |
    |***************************************/
    // Check whether compiled datapack contains given .mcfunction file
    // Arguments
    //   function (string) - name of funtion file in question
    // Return (bool)       - whether function file exists
    bool _MCFunctionExists(std::string function) const {
        return MCFunctionExists(name, getPrimaryNamespace(), function);
    }

    // Lists the names of function files in a compiled datapack, no .mcfunction
    // Return (vector<string>) - list of names 
    std::vector<std::string> _listMCFunctionNames() const {
        return listMCFunctionNames(name, getPrimaryNamespace());
    }

    // Gets the contents of a function file from compiled pack
    //   function (string) - name of cuntion in question
    // Return (string)     - contents of the file, "" if file does not exist
    std::string _getMCFunction(std::string function) const {
        return getMCFunction(name, getPrimaryNamespace(), function);
    }

    // checks whether any function file in a datapack contains the given string
    // Arguments
    //   content (string) - string to search for
    // Return (bool)      - whether string was found
    bool _anyFunctionContain(std::string content) const {
        return anyFunctionContain(name, getPrimaryNamespace(), content);
    }
    
    /***************************************|
    |                                       |
    |            Python Interface           |
    |                                       |
    |***************************************/
    // move pack to python folder to be handled by Python script
    void _pythonizePack() {
        pythonizePack(name);
    }
    //system((std::string("cp -r ") + _makeCompiledPath() + " " + _makePythonPath()).c_str());

    // await python output for pack
    // Return (string) - contents of the output file
    std::string _awaitPythonOutput() {
        return awaitPythonOutput(name);
    }

    // remove python output for pack
    void _cleanupPythonFiles() {
        cleanupPythonFiles(name);
    }
    //system((std::string("rm ") + makePythonPath(name + ".txt")).c_str());

    /***************************************|
    |                                       |
    |                Cleanup                |
    |                                       |
    |***************************************/
    // cleans up any current build
    void _cleanupBuildFiles();

/***************************************|
|                                       |
|               Building                |
|                                       |
|***************************************/
    // builds the Datapack
    // Argument
    //   n (string)   - optional argument for namespace
    // Returns (bool) - whether build succeeded
    // State
    //   saves command result to rez
    //   places source file at `_makeSourcePath()`
    //   outputs datapack to `_makeCompiledPath()`
    bool build(std::string n = NO_NAMESPACE);

/***************************************|
|                                       |
|              Destructor               |
|                                       |
|***************************************/
    // deconstructor cleans up all build files
    ~Datapack() {
        _cleanupBuildFiles();
    }
};

#include "Datapack.cpp"

#endif