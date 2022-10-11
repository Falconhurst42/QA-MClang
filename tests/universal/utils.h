// Utilies for GTest testing of MCLang
// Ethan Worth
// 10/10/2022

#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <sys/stat.h>

/***************************************|
|                                       |
|              Path Utils               |
|                                       |
|***************************************/

struct stat info;

const std::string SRC_EXT = ".mcl",
                  SOURCE_PATH = "../files/sourcefiles/",
                  DATA_PATH = "../files/datafiles/", 
                  DEF_FILENAME = "gtest";

std::string makeSourcePath(std::string filename) {
    return SOURCE_PATH + filename + SRC_EXT;
}
std::string makeCompiledPath(std::string filename) {
    return DATA_PATH + filename;
}
inline bool fileCanOpen(std::string path) {
    return std::ifstream(path).is_open();
}
// https://stackoverflow.com/a/18101042
bool directoryExists(std::string path) {
    if( stat( path.c_str(), &info ) != 0 )
        return false;
    else if( info.st_mode & S_IFDIR )  // S_ISDIR() doesn't exist on my windows 
        return true;
    else
        return false;
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

// specific struct for storing results of a MCLang build
struct BuildResult : CommandResult {
    std::string filename;

    BuildResult(CommandResult cr, std::string fi = "") : CommandResult(cr.output, cr.code), filename(fi) {}
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

// builds the file with the given name and returns the result
//  optional args for output folder and namespace command line arguments
//  to skip -o but provide -n, use `build(fff, "", nnn)`
BuildResult build(std::string filename, std::string o = "", std::string n = "") {
#ifdef _WIN32
    std::string cmd = "..\\..\\build\\main.exe " + makeSourcePath(filename);
#else
    std::string cmd = "../../build/main " + makeSourcePath(filename);
#endif
    if(o != "")
        cmd.append(" -o " + o);
    if(n != "")
        cmd.append(" -n " + n);
    BuildResult out = execCommand(cmd);
    out.filename = filename;
    return out;
}

// creates and builds a file with the given text as body
//  places file at `makeSourcePath(filename)`
//  outputs datapack to `makeCompiledPath(filename)`
//  optional -n argument for namespace
BuildResult buildFromBody(std::string filebody, std::string n = "", const std::string filename = DEF_FILENAME) {
    // make file
    std::ofstream f(makeSourcePath(filename));
    if(!f.is_open())
        throw std::runtime_error("Could not create file: " + makeSourcePath(filename));
    
    f << filebody;
    f.close();

    // run build
    return build(filename, makeCompiledPath(filename), n);
}

// cleans up a given build
//  assumes source file at `makeSourcePath(filename)`
//  assumes datapack at `makeCompiledPath(filename)`
//  assumes that if rez.code != 0, no datapack was created
void cleanupBuild(BuildResult rez) {
    // remove source file (if it exists)
    if(fileCanOpen(makeSourcePath(rez.filename)))
        system(("rm " + makeSourcePath(rez.filename)).c_str());

    // remove compoiled pack
    if(rez.code == 0)
        system(("rm -r " + makeCompiledPath(rez.filename)).c_str());
}