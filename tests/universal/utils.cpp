// utils.cpp
// Utilies for GTest testing of MCLang
// Structure:
//    File Checkers:    functions for determining if a files and directories exists as well as accessing their contents
//    Params: Reads     certain default paths/names from `universal/configs.json`
//    Path Generators:  standardized functions for generating relative paths to source/compiled files
//       MCFunctions:       Specialized functions for interacting with .mcfunction files created by a datapack
//       Python Interface:  Specialized function for managing communication with Python
//    General Utils:    generally useful functs
//    Execute Commands: Functionality to robustly execute system commands and return their results. 
//                      Special functions for building particular source files using MCLang and for deleting the compiled files afterwards
//       `CommandResult`:   Struct to store the output (`str`) and return code (`int`) of a command execution
// Ethan Worth (created), 10/26/2022
// Ethan Worth (updated), 11/8/2022
// Ethan Worth (updated), 11/13/2022
// Ethan Worth (updated), 11/14/2022
// Ethan Worth (updated), 11/16/2022
// Ethan Worth (updated), 11/21/2022
// Ethan Worth (updated), 12/7/2022

#include "utils.h"

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
#include <dirent.h>
#include <chrono>
#include <thread>
#include <ctype.h>

/***************************************|
|                                       |
|             File Checkers             |
|                                       |
|***************************************/

// global struct for sys/stat.h
struct stat stat_info;

// Checks if the given path points to a valid directory
// https://stackoverflow.com/a/18101042
bool directoryExists(std::string path) {
    if( stat( path.c_str(), &stat_info ) != 0 ) // check access
        return false;
    else if( stat_info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
        return true;
    else
        return false;
}

// Checks if the given path points to a valid file
// https://stackoverflow.com/a/18101042
inline bool fileExists(std::string path) {
    if( stat( path.c_str(), &stat_info ) != 0 ) // check access
        return false;
    else if( stat_info.st_mode & S_IFMT )  // S_IFMT() ??might not?? exist on my windows 
        return true;
    else
        return false;
}

// using Unix `dirent.h` per https://stackoverflow.com/a/612176
std::vector<std::string> getDirectoryContents(std::string path) {
    std::vector<std::string> out;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL) {
        /* record all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            out.push_back(ent->d_name);
        }
        closedir (dir);
        /* pop off '.' and '..' */
        assert(out.size() >= 2);
        std::copy(out.begin()+2, out.end(), out.begin());
        out.resize(out.size()-2);
    } else {
        /* could not open directory */
        perror ("");
    }
    return out;
}

// if file does not exist, returns ""
std::string getFileContents(std::string filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    std::string out;
 
    // check that the file is open
    if (in.is_open()) {
        // resize output string to proper size
        in.seekg(0, std::ios::end);
        out.reserve(static_cast<std::string::size_type>(in.tellg()));
        in.seekg(0, std::ios::beg);
    
        // copy from file to string
        std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(out));
    
        // close file
        in.close();
    }
    return out;
}

// delays and waits until file exists
std::string awaitFileContents(std::string filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    std::string out;
 
    // loop until file opens
    while (!in.is_open()) {
        // sleep for .3s
        std::this_thread::sleep_for(WAIT_SLEEP_MS);
        in.open(filepath, std::ios::in | std::ios::binary);
    }
    
    // resize output string to proper size
    in.seekg(0, std::ios::end);
    out.reserve(static_cast<std::string::size_type>(in.tellg()));
    in.seekg(0, std::ios::beg);

    // copy from file to string
    std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(out));

    // close file
    in.close();
    return out;
}

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/
size_t findNameAnchor(std::string name) {
    std::string search = "\"" + name + "\":";
    size_t i = CONFIG_CONTENTS.find(search);
    if(i != std::string::npos)
        i += search.size();
    return i;
}

// does not handle escaped characters
std::string getStringParam(std::string name) {
    // find name anchor
    size_t i = findNameAnchor(name);
    // if anchor found
    if(i != std::string::npos) {
        // find start of string
        i = CONFIG_CONTENTS.find("\"", i);
        // substr to end of string
        return CONFIG_CONTENTS.substr(i+1, CONFIG_CONTENTS.find("\"", i+1)-i-1);
    }
    // base case
    return "";
}

// does not handle escaped characters
std::vector<std::string> getStrVecParam(std::string name) {
    std::vector<std::string> out;
    // find name anchor
    size_t i = findNameAnchor(name);
    // if name anchor found
    if(i != std::string::npos) {
        // find list anchor
        i = CONFIG_CONTENTS.find("[", i);
        // substr list contents
        std::string conts = CONFIG_CONTENTS.substr(i+1, CONFIG_CONTENTS.find("]", i+1)-i-1);
        // iterate over strings
        size_t j = 0;
        // pre-find nex string start
        i = conts.find("\"", j);
        // while there is a next string start
        while(i != std::string::npos) {
            // find string end
            j = conts.find("\"", i+1)+1;
            // substr and push
            out.push_back(conts.substr(i+1, j-i-2));
            // find next str start
            i = conts.find("\"", j);
        }
    }
    // return output
    return out;
}

uint64_t getUIntParam(std::string name) {
    // find name anchor
    size_t i = findNameAnchor(name);
    // if anchor found
    if(i != std::string::npos) {
        // advance to digit
        while(++i < CONFIG_CONTENTS.size() && !isdigit(CONFIG_CONTENTS[i]));
        // if digit found
        if(i < CONFIG_CONTENTS.size()) {
            // substr and convert
            return strtoull(CONFIG_CONTENTS.substr(i, CONFIG_CONTENTS.find("\n", i+1)-i).c_str(), nullptr, 0);
        }
    }
    return 0;
}

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
inline std::string makeCompiledPath(std::string packname) {
    return DATA_PATH + packname;
}
// get location of python
inline std::string makePythonPath(std::string packname) {
    return PYTHON_PATH + packname;
}
// get location of function .json files (for load, tick)
inline std::string makeTagsPath(std::string packname) {
    return makeCompiledPath(packname) + "/data/minecraft/tags/functions";
}
// get location of generated `.mcfunction` files
inline std::string makeFunctionsPath(std::string packname, std::string namesp) {
    return makeCompiledPath(packname) + "/data/" + namesp + "/functions";
}

/***************************************|
|                                       |
|              MC Functions             |
|                                       |
|***************************************/
    bool MCFunctionExists(std::string packname, std::string namesp, std::string function) {
        return fileExists(makeFunctionsPath(packname, namesp) + "/" + function + FOO_EXT);
    }

    std::vector<std::string> listMCFunctionNames(std::string packname, std::string namesp) {
        std::vector<std::string> out = getDirectoryContents(makeFunctionsPath(packname, namesp));
        for(std::string& foo : out)
            foo = foo.substr(0, foo.find(FOO_EXT, 0));
        return out;
    }

    std::string getMCFunction(std::string packname, std::string namesp, std::string function) {
        return getFileContents(makeFunctionsPath(packname, namesp) + "/" + function + FOO_EXT);
    }

    bool anyFunctionContain(std::string packname, std::string namesp, std::string content) {
        // range-based forloop will only call the function once
        // for each mcfunction
        for(std::string function : listMCFunctionNames(packname, namesp))
            if(inStr(getMCFunction(packname, namesp, function), content))
                return true;
        return false;
    } 
    
/***************************************|
|                                       |
|            Python Interface           |
|                                       |
|***************************************/
    void pythonizePack(std::string name) {
        system((std::string("cp -r ") + makeCompiledPath(name) + " " + makePythonPath(name)).c_str());
    }

    std::string awaitPythonOutput(std::string name) {
        return awaitFileContents(makePythonPath(name + PYTHON_EXT));
    }

    void cleanupPythonFiles(std::string name) {
        system((std::string("rm ") + makePythonPath(name + PYTHON_EXT)).c_str());
    }

/***************************************|
|                                       |
|            General Utils              |
|                                       |
|***************************************/

// checks if `src` string contains `search` string
bool inStr(std::string src, std::string search) {
    return std::search(src.begin(), src.end(), search.begin(), search.end()) != src.end();
}

// my own jank implementation of pretty int printing
std::string thousandsSep(size_t n) {
    std::string out = std::to_string(n);
    size_t count = 1;
    n = out.size()-1;
    while(n > 0) {
        if(count % 3 == 0) {
            count = 1;
            out.insert(n, ",");
            n--;
            // prevent underflow
            if(n == 0)
                n++;
        }
        n--, count++;
    }
    return out;
}

//52165057

/***************************************|
|                                       |
|           Execute Commands            |
|                                       |
|***************************************/

// executes the given command and returns the result
//  Getting command output: https://stackoverflow.com/a/478960
//  Getting command result: https://stackoverflow.com/a/52165057
//  Getting cerr: https://stackoverflow.com/a/23330438
CommandResult execCommand(std::string cmd) {
    std::array<char, 128> buffer;
    std::string result;
    auto pipe(popen((cmd + " 2> cerr.txt").c_str(), "r"));
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        result += buffer.data();
    
    return CommandResult(result + getFileContents("cerr.txt"), pclose(pipe));
}

CommandResult makeAndBuildFile(std::string filename, std::string contents) {
    // cleanup previous build
    cleanupBuildFiles(makeSourcePath(filename), makeCompiledPath(filename));

    // make file
    std::ofstream f(makeSourcePath(filename));
    if(!f.is_open())
        throw std::runtime_error("Could not create file: " + makeSourcePath(filename));
    // pass text
    f << contents;
    f.close();

    // run and return build
    return buildFile(makeSourcePath(filename), makeCompiledPath(filename));
}

// builds the file with the given path and returns the result
//  optional args for output folder and namespace command line arguments
//  to skip -o but provide -n, use `build(fff, "", nnn)`
CommandResult buildFile(std::string srcpath, std::string o, std::string n) {
#ifdef _WIN32
    std::string cmd = "..\\build\\main.exe " + srcpath;
#else
    std::string cmd = "../build/main " + srcpath;
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