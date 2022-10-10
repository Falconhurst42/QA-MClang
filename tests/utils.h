#include <string>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <algorithm>

struct CommandResult {
    std::string output;
    int code;
};

// Getting command output: https://stackoverflow.com/a/478960
// Getting command result: https://stackoverflow.com/a/52165057
CommandResult execCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    auto pipe(popen(cmd, "r"));
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        result += buffer.data();
    
    return (CommandResult){result, pclose(pipe)};
}

const std::string SOURCE_PATH = "files/sourcefiles/",
                  SRC_EXT = ".mcl",
                  DATA_PATH = "files/datafiles/";

std::string makeSourcePath(std::string filename) {
    return SOURCE_PATH + filename + SRC_EXT;
}
std::string makeDataPath(std::string filename) {
    return DATA_PATH + filename;
}

CommandResult buildFile(std::string filename) {
#ifdef _WIN32
    return execCommand(("..\\build\\main.exe " + makeSourcePath(filename) + " -o " + makeDataPath(filename)).c_str());
#else
    CommandResult out = execCommand(("../build/main " + makeSourcePath(filename) + " -o " + makeDataPath(filename)).c_str());
    if(out.code == 0)
        system(("rm -r " + makeDataPath(filename)).c_str());
    return out;
#endif
}

bool inStr(std::string src, std::string search) {
    return std::search(src.begin(), src.end(), search.begin(), search.end()) == src.end();
}