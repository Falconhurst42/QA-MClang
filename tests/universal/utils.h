// Utilies for GTest testing of MCLang
// Ethan Worth
// 10/10/2022

#ifndef UTILS_H
#define UTILS_H

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

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/
const std::string DEF_FILENAME = "source";
const std::string SRC_EXT = ".mcl",
                  FOO_EXT = ".mcfunction",
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
inline std::string makeCompiledPath(std::string packname) {
    return DATA_PATH + packname;
}
// get location of function .json files (for load, tick)
inline std::string makeTagsPath(std::string packname, std::string namesp) {
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
std::string getMCFunction(std::string packname, std::string namesp, std::string function) {
    return getFileContents(makeFunctionsPath(packname, namesp) + "/" + function + FOO_EXT);
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

#endif