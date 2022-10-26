// Utils
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
inline std::string makeSourcePath(std::string filename);

// get location of datapack
inline std::string makeCompiledPath(std::string packname);

// get location of function .json files (for load, tick)
inline std::string makeTagsPath(std::string packname, std::string namesp);

// get location of generated `.mcfunction` files
inline std::string makeFunctionsPath(std::string packname, std::string namesp);

/***************************************|
|                                       |
|             File Checkers             |
|                                       |
|***************************************/

// Checks if the given path points to a valid file
// https://stackoverflow.com/a/18101042
inline bool fileExists(std::string path);

// Checks if the given path points to a valid directory
// https://stackoverflow.com/a/18101042
bool directoryExists(std::string path);

// using Unix `dirent.h` per https://stackoverflow.com/a/612176
std::vector<std::string> getDirectoryContents(std::string path);

std::string getFileContents(std::string filepath);

inline std::string getMCFunction(std::string packname, std::string namesp, std::string function);

/***************************************|
|                                       |
|            General Utils              |
|                                       |
|***************************************/

// checks if `src` string contains `search` string
inline bool inStr(std::string src, std::string search);

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
CommandResult execCommand(std::string cmd);

// builds the file with the given path and returns the result
//  optional args for output folder and namespace command line arguments
//  to skip -o but provide -n, use `build(fff, "", nnn)`
CommandResult buildFile(std::string srcpath, std::string o = "", std::string n = NO_NAMESPACE);

void cleanupBuildFiles(std::string sourcepath, std::string outpath);

#include "utils.cpp"

#endif