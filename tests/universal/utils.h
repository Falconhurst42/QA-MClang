#ifndef UTILS_H
#define UTILS_H

// utils.h
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
// Ethan Worth (created), 10/10/2022
// Ethan Worth (updated), 10/12/2022
// Ethan Worth (updated), 10/19/2022
// Ethan Worth (updated), 10/26/2022
// Ethan Worth (updated), 11/8/2022
// Ethan Worth (updated), 11/13/2022
// Ethan Worth (updated), 11/14/2022
// Ethan Worth (updated), 11/16/2022
// Ethan Worth (updated), 11/21/2022
// Ethan Worth (updated), 12/7/2022

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
// Arguments
//   path (string) - relative path to directory
// Return (bool)   - whether path points to directory
// References:
//   Filesystem handling: https://stackoverflow.com/a/18101042
bool directoryExists(std::string path);

// Checks if the given path points to a valid file
// Arguments
//   path (string) - relative path to file
// Return (bool)   - whether path points to file
// References:
//   Filesystem handling: https://stackoverflow.com/a/18101042
bool fileExists(std::string path);

// Get list of contents of directory
// Arguments
//   path (string)           - relative path to directory
// Return (vector<string>)   - names of all child files/directories
// References:
//   using Unix `dirent.h`: https://stackoverflow.com/a/18101042
std::vector<std::string> getDirectoryContents(std::string path);

// Attempts to get the contents of a given file
// Arguments
//   filepath (string) - path to file
// Return (string)     - complete contents of file
//                      "" if file does not exist or cannot be opened
std::string getFileContents(std::string filepath);
// Attempts to get the contents of a given file
//  if file cannot be opened or does not exist, waits until it is available
// Arguments
//   filepath (string) - path to file
// Return (string)     - complete contents of file
std::string awaitFileContents(std::string filepath);

/***************************************|
|                                       |
|                Params                 |
|                                       |
|***************************************/

const std::string CONFIG_PATH = "universal/configs.json";
const std::string CONFIG_CONTENTS = getFileContents(CONFIG_PATH);

// Gets location of anchor for given property in CONFIG_CONTENTS
// Arguments
//   name (string) - name of anchor
// Return (size_t) - location in file
size_t findNameAnchor(std::string name);
// Gets value of given string property in CONFIG_CONTENTS
// Arguments
//   name (string) - name of property
// Return (string) - value of property, "" if property not found
std::string getStringParam(std::string name);
// Gets value of given string_vector property in CONFIG_CONTENTS
// Arguments
//   name (string) - name of property
// Return (vector<string>) - value of property, empty if property not found
std::vector<std::string> getStrVecParam(std::string name);
// Gets value of given unsigned int property in CONFIG_CONTENTS
// Arguments
//   name (string) - name of property
// Return (uint64_t) - value of property, 0 if property not found
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
// get location of `.mcl` source file
// Arguments
//   filename (string) - name of file (without mcl)
// Return (string)     - relative path to file
std::string makeSourcePath(std::string filename);

// get location of compile datapack
// Arguments
//   packname (string) - name of source pack
// Return (string)     - relative path to pack folder
std::string makeCompiledPath(std::string packname);

// get location of python pack
// Arguments
//   packname (string) - name of source pack
// Return (string)     - relative path to python pack folder
std::string makePythonPath(std::string packname);

// get location of function .json files (for load, tick)
// Arguments
//   packname (string) - name of source pack
// Return (string)     - relative path to tags folder
std::string makeTagsPath(std::string packname);

// get location of generated `.mcfunction` files
// Arguments
//   packname (string) - name of source pack
//   namesp (string)   - namespace of compiled pack
// Return (string)     - relative path to function (including .mcfunction)
std::string makeFunctionsPath(std::string packname, std::string namesp);

    /***************************************|
    |                                       |
    |              MC Functions             |
    |                                       |
    |***************************************/
    
    // Check whether given compiled datapack contains given .mcfunction file
    // Arguments
    //   packname (string) - name of source pack
    //   namesp (string)   - namespace of compiled pack
    //   function (string) - name of funtion file in question
    // Return (bool)       - whether function file exists
    bool MCFunctionExists(std::string packname, std::string namesp, std::string function);

    // Lists the names of function files in a compiled datapack, no .mcfunction
    // Arguments
    //   packname (string) - name of source pack
    //   namesp (string)   - namespace of compiled pack
    //   function (string) - name of cuntion in question
    // Return (vector<string>) - list of names 
    std::vector<std::string> listMCFunctionNames(std::string packname, std::string namesp);

    // Gets the contents of a function file from compiled pack
    // Arguments
    //   packname (string) - name of source pack
    //   namesp (string)   - namespace of compiled pack
    //   function (string) - name of cuntion in question
    // Return (string)     - contents of the file, "" if file does not exist
    std::string getMCFunction(std::string packname, std::string namesp, std::string function);

    // checks whether any function file in a datapack contains the given string
    // Arguments
    //   packname (string) - name of source pack
    //   namesp (string)   - namespace of compiled pack
    //   content (string)  - string to search for
    // Return (bool)       - whether string was found
    bool anyFunctionContain(std::string packname, std::string namesp, std::string content);
    
    /***************************************|
    |                                       |
    |            Python Interface           |
    |                                       |
    |***************************************/
    // move pack to python folder to be handled by Python script
    // Arguments
    //   name (string) - name of source pack
    void pythonizePack(std::string name);
    //system((std::string("cp -r ") + pf._makeCompiledPath() + " " + pf._makePythonPath()).c_str());

    // await python output for pack with given name
    // Arguments
    //   name (string) - name of source pack
    // Return (string) - contents of the output file
    std::string awaitPythonOutput(std::string name);

    // remove python output for pack with given name
    // Arguments
    //   name (string) - name of source pack
    void cleanupPythonFiles(std::string name);
    //system((std::string("rm ") + makePythonPath(python_packs["HelloWorld"].name + ".txt")).c_str());

/***************************************|
|                                       |
|            General Utils              |
|                                       |
|***************************************/

// checks if `src` string contains `search` string
// Arguments
//   src (string)    - string to search in
//   search (string) - string to search for
// Return (bool)     - result of search
bool inStr(std::string src, std::string search);

// Formats given integer with thousands separators
// Arguments
//   n (size_t)    - (unsigned) integer to format
// Return (string) - formatted string
std::string thousandsSep(size_t n);

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

// executes the given command and records the result
// Arguments
//   cmd (string)           - command to execute
// Return (CommandResult)   - result of execution
// References:
//   Getting command output: https://stackoverflow.com/a/478960
//   Getting command result: https://stackoverflow.com/a/52165057
CommandResult execCommand(std::string cmd);

// Creates a .mcl file with given name and contents
//   then builds it and records the result
// Arguments
//   filename (string)      - name of source file
//   contents (string)      - contents of source file
// Return (CommandResult)   - result of execution
CommandResult makeAndBuildFile(std::string filename, std::string contents);

// builds the file with the given path and returns the result
//  optional args for output folder and namespace command line arguments
//  to skip -o but provide -n, use `build(fff, "", nnn)`
// Arguments
//   srcpath (string)     - path to source file
//   o (string)           - path to output folder
//   n (string)           - namespace arge for compilation
// Return (CommandResult) - result of execution
CommandResult buildFile(std::string srcpath, std::string o = "", std::string n = NO_NAMESPACE);

// Cleans up source file and pack folder from a compilation
// Arguments
//   sourcepath (string) - path to source file
//   outpath (string)    - path to compiled path
void cleanupBuildFiles(std::string sourcepath, std::string outpath);

#include "utils.cpp"

#endif