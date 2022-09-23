
#include "errorhandle/colors.h"
#include <map>
#include <string>

// Colors are only supported on Linux
#ifdef OS_WINDOWS

void colorText(std::ostream &os, const std::string &txt, std::string color) {
    std::pair<int, int> colorNums;
    std::string curColor = "";
    color.push_back(' ');
    bool isBold = false;
    for (unsigned int i = 0; i < color.size(); i++) {
        if (color[i] == ' ') {
            if (curColor == "bold")
                isBold = true;
            else if (colorNames.find(curColor) != colorNames.end())
                colorNums = colorNames.find(curColor)->second;
            curColor = "";
        } else {
            curColor.push_back(color[i]);
        }
    }
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, isBold ? colorNums.second |
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY :
    colorNums.first);
    os << txt;
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE |
    FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

#else

void colorText(std::ostream &os, const std::string &txt, std::string color) {
    std::string curColor = "";
    color.push_back(' ');
    for (unsigned int i = 0; i < color.size(); i++) {
        if (color[i] == ' ' && colorNames.count(curColor) > 0) {
            std::string ctxt = "\033[" + colorNames.find(curColor)->second;
            os << std::string("\033[") << ctxt << std::string("m");
            curColor = "";
        } else {
            curColor.push_back(color[i]);
        }
    }
    os << txt << std::string("\033[0m");
}

#endif