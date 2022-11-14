// Utils
// Utilies for GTest testing of MCLang
// Ethan Worth
// 10/10/2022

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

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/
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
        out.reserve(in.tellg());
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
        std::this_thread::sleep_for(WAIT_SLEEP_TIME);
        in.open(filepath, std::ios::in | std::ios::binary);
    }
    
    // resize output string to proper size
    in.seekg(0, std::ios::end);
    out.reserve(in.tellg());
    in.seekg(0, std::ios::beg);

    // copy from file to string
    std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(out));

    // close file
    in.close();
    return out;
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
            foo = foo.substr(foo.find(FOO_EXT, 0));
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
inline bool inStr(std::string src, std::string search) {
    return std::search(src.begin(), src.end(), search.begin(), search.end()) == src.end();
}

/***************************************|
|                                       |
|           Execute Commands            |
|                                       |
|***************************************/

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
CommandResult buildFile(std::string srcpath, std::string o, std::string n) {
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