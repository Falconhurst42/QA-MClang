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
#include <chrono>


#include <stdio.h>

/***************************************|
|                                       |
|             File Checkers             |
|                                       |
|***************************************/

// Checks if the given path points to a valid directory
// https://stackoverflow.com/a/18101042
bool directoryExists(std::string path);

// Checks if the given path points to a valid file
// https://stackoverflow.com/a/18101042
bool fileExists(std::string path);

// using Unix `dirent.h` per https://stackoverflow.com/a/612176
std::vector<std::string> getDirectoryContents(std::string path);

std::string getFileContents(std::string filepath);
std::string awaitFileContents(std::string filepath);

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/

const std::string CONFIG_PATH = "../universal/configs.json";
const std::string CONFIG_CONTENTS = getFileContents(CONFIG_PATH);

size_t findNameAnchor(std::string name);
std::string getStringParam(std::string name);
std::vector<std::string> getStrVecParam(std::string name);
uint64_t getUIntParam(std::string name);

const std::string SOURCE_PATH = getStringParam("SOURCE_PATH"),
                  DATA_PATH = getStringParam("DATA_PATH"),
                  PYTHON_PATH = getStringParam("PYTHON_PATH"),
                  SRC_EXT = getStringParam("SRC_EXT"),
                  FOO_EXT = getStringParam("FOO_EXT"),
                  PYTHON_EXT = getStringParam("PYTHON_EXT");
const std::string DEF_FILENAME = getStringParam("DEF_FILENAME"),
                  NO_NAMESPACE = getStringParam("NO_NAMESPACE"),
                  DEF_NAMESPACE = getStringParam("DEF_NAMESPACE");
const std::vector<std::string> TAGGED_FUNCT_NAMES = getStrVecParam("TAGGED_FUNCT_NAMES");
const std::chrono::milliseconds WAIT_SLEEP_MS = std::chrono::milliseconds(getUIntParam("WAIT_SLEEP_MS"));

/***************************************|
|                                       |
|            Path Generators            |
|                                       |
|***************************************/
// get location of `.mcl` file
std::string makeSourcePath(std::string filename);

// get location of datapack
std::string makeCompiledPath(std::string packname);

// get location of python
std::string makePythonPath(std::string packname);

// get location of function .json files (for load, tick)
std::string makeTagsPath(std::string packname);

// get location of generated `.mcfunction` files
std::string makeFunctionsPath(std::string packname, std::string namesp);

    /***************************************|
    |                                       |
    |              MC Functions             |
    |                                       |
    |***************************************/
    bool MCFunctionExists(std::string packname, std::string namesp, std::string function);

    // lists the function names, not paths, no .mcfunction
    std::vector<std::string> listMCFunctionNames(std::string packname, std::string namesp);

    std::string getMCFunction(std::string packname, std::string namesp, std::string function);

    bool anyFunctionContain(std::string packname, std::string namesp, std::string content);
    
    /***************************************|
    |                                       |
    |            Python Interface           |
    |                                       |
    |***************************************/
    // move pack to python folder to be handled by Python script
    void pythonizePack(std::string name);
    //system((std::string("cp -r ") + pf._makeCompiledPath() + " " + pf._makePythonPath()).c_str());

    // await python output for pack with given name
    std::string awaitPythonOutput(std::string name);

    // remove python output for pack with given name
    void cleanupPythonFiles(std::string name);
    //system((std::string("rm ") + makePythonPath(python_packs["HelloWorld"].name + ".txt")).c_str());

/***************************************|
|                                       |
|            General Utils              |
|                                       |
|***************************************/

// checks if `src` string contains `search` string
bool inStr(std::string src, std::string search);

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