#ifndef __STDLIB_RAW_FILE_H__
#define __STDLIB_RAW_FILE_H__
#include <string>
#include <map>
const std::map<std::string, std::string> stdlibContent = {
{"random", R"!--!DELIM!--!(
int randomValue;

// TODO: Change this to generate actual random numbers...
void random() {
    randomValue = 42;
}
)!--!DELIM!--!"},
{"stdlib", R"!--!DELIM!--!(
void tp(const str entity, const int x, const int y, const int z) {
    /tp {{entity}} {{x}} {{y}} {{z}}
}

void tprel(const str entity, const int dx, const int dy, const int dz) {
    /tp {{entity}} ~{{x}} ~{{y}} ~{{z}}
}

void tpall(const int x, const int y, const int z) {
    /tp @a {{x}} {{y}} {{z}}
}
)!--!DELIM!--!"},
};
#endif
