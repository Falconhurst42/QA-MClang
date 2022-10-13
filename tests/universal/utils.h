// Utilies for GTest testing of MCLang
// Ethan Worth
// 10/10/2022

#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <sys/stat.h>

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/
const std::string DEF_FILENAME = "source";
const std::string SRC_EXT = ".mcl",
                  SOURCE_PATH = "../files/sourcefiles/",
                  DATA_PATH = "../files/datafiles/";
const std::string NO_NAMESPACE = "",
                  DEF_NAMESPACE = "dp";
const std::vector<std::string> TAGGED_FUNCT_NAMES = {"load", "tick"};

/***************************************|
|                                       |
|            Path Generators            |
|                                       |
|***************************************/
// get location of `.mcl` file
inline std::string makeSourcePath(std::string filename) {
    return SOURCE_PATH + filename + SRC_EXT;
}
// get location of datapack
inline std::string makeCompiledPath(std::string filename) {
    return DATA_PATH + filename;
}
// get location of function .json files (for load, tick)
inline std::string makeTagsPath(std::string filename, std::string namesp) {
    return makeCompiledPath(filename) + "/data/minecraft/tags/functions";
}
// get location of generated `.mcfunction` files
inline std::string makeFunctionsPath(std::string filename, std::string namesp) {
    return makeCompiledPath(filename) + "/data/" + namesp + "/functions";
}

/***************************************|
|                                       |
|             File Checkers             |
|                                       |
|***************************************/

struct stat info;

// Checks if the given path points to a valid file
// https://stackoverflow.com/a/18101042
inline bool fileExists(std::string path) {
    if( stat( path.c_str(), &info ) != 0 ) // check access
        return false;
    else if( info.st_mode & S_IFMT )  // S_IFMT() ??might not?? exist on my windows 
        return true;
    else
        return false;
}
// Checks if the given path points to a valid directory
// https://stackoverflow.com/a/18101042
bool directoryExists(std::string path) {
    if( stat( path.c_str(), &info ) != 0 ) // check access
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
        return true;
    else
        return false;
}
std::string getFileContents(std::string filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    std::string output;
 
    // check that the file is open
    if (in.is_open()) {
        // resize output string to proper size
        in.seekg(0, std::ios::end);
        output.reserve(in.tellg());
        in.seekg(0, std::ios::beg);
    
        // copy from file to string
        std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(output));
    
        // close file
        in.close();
    }
    return output;
}

/***************************************|
|                                       |
|            General Utils              |
|                                       |
|***************************************/

// checks if `src` string contains `search` string
inline bool inStr(std::string src, std::string search) {
    return std::search(src.begin(), src.end(), search.begin(), search.end()) == src.end();
}

/***************************************|
|                                       |
|           Execute Commands            |
|                                       |
|***************************************/

// generic struct for storing results of a command
struct CommandResult {
    std::string output;
    int code;

    CommandResult(std::string ou, int co) : output(ou), code(co) {}
};

// executes the given command and returns the result
//  Getting command output: https://stackoverflow.com/a/478960
//  Getting command result: https://stackoverflow.com/a/52165057
CommandResult execCommand(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    auto pipe(popen(cmd.c_str(), "r"));
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        result += buffer.data();
    
    return CommandResult(result, pclose(pipe));
}

// builds the file with the given path and returns the result
//  optional args for output folder and namespace command line arguments
//  to skip -o but provide -n, use `build(fff, "", nnn)`
CommandResult buildFile(std::string srcpath, std::string o = "", std::string n = NO_NAMESPACE) {
#ifdef _WIN32
    std::string cmd = "..\\..\\build\\main.exe " + srcpath;
#else
    std::string cmd = "../../build/main " + srcpath;
#endif
    if(o != "")
        cmd.append(" -o " + o);
    if(n != NO_NAMESPACE)
        cmd.append(" -n " + n);
    return execCommand(cmd);
}

void cleanupBuildFiles(std::string sourcepath, std::string outpath) {
    // remove source file (if it exists)
    if(fileExists(sourcepath))
        system(("rm " + sourcepath).c_str());

    // remove compiled pack
    if(directoryExists(outpath))
        system(("rm -r " + outpath).c_str());
}

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

        std::string getDeclaration() const {
            return type + " " + name;
        }
    };

    struct Function {
        std::string rtype, name;
        std::vector<Variable> args;
        std::string body;

        Function(std::string rt, std::string na, std::vector<Variable> ar, std::string bo) :
            rtype(rt), name(na), args(ar), body(bo) {}

        std::string getText() const {
            std::string out("");
            out += rtype + " " + name + "(";
            for(const Variable& a : args)
                out += a.getDeclaration() + ", ";
            if(!args.empty()) {
                out.pop_back(); out.pop_back();
            }
            out += ") {\n" + body + "\n}";
            return out;
        }
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
    std::string generateText() const {
        std::string out("");
        if(space.space != NO_NAMESPACE) {
            out += space.getNamespacer() + "\n\n";
        }
        for(const Variable& v : vars) {
            out += v.getDeclaration() + ";\n";
        }
        for(const Function& f : foos) {
            out += f.getText() + "\n\n";
        }
        return out;
    }

    // determines the effective namespace. Accounts for in-file namespace and dash_n args of last build
    std::string getPrimaryNamespace() const {
        // in-file namespace trumps
        if(space.space != NO_NAMESPACE)
            return space.space;
        // check for dash_n arg from last build
        if(rez.code != -1 && rez.dash_n != NO_NAMESPACE)
            return rez.dash_n;
        // default
        return DEF_NAMESPACE;
    }
    
/***************************************|
|                                       |
|           Path Generators             |
|                                       |
|***************************************/
    // get location of `.mcl` file
    std::string getSourcePath() const {
        return makeSourcePath(name);
    }
    // get location of datapack
    std::string getCompiledPath() const {
        return makeCompiledPath(name);
    }
    // get location of function .json files (for load, tick)
    std::string getTagsPath() const {
        return makeTagsPath(name, getPrimaryNamespace());
    }
    // get location of generated `.mcfunction` files
    std::string getFunctionsPath() const {
        return makeFunctionsPath(name, getPrimaryNamespace());
    }

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
    bool build(std::string n = NO_NAMESPACE) {
        // make file
        std::ofstream f(getSourcePath());
        if(!f.is_open())
            throw std::runtime_error("Could not create file: " + getSourcePath());
        
        // pass text
        f << generateText();
        f.close();

        // cleanup previous build
        cleanupBuild();

        // run build
        rez = BuildResult(buildFile(getSourcePath(), getCompiledPath(), n), n);

        // return success
        return rez.code == 0;
    }

    // cleans up a given build
    void cleanupBuild() {
        if(rez.code != -1) {
            // clean up files
            cleanupBuildFiles(getSourcePath(), getCompiledPath());
            
            // mark as cleaned
            rez.code = -1;
        }
    }

/***************************************|
|                                       |
|              Destructor               |
|                                       |
|***************************************/
    ~Datapack() {
        cleanupBuild();
    }
};