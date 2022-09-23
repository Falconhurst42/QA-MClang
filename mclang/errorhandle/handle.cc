
#include "compiler/compiler.h"
#include "errorhandle/colors.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include <iostream>
#include <string>

MCLError::MCLError(ErrType level, std::string txt, Compiler *comp, Loc loc) :
level(level), txt(txt), comp(comp), loc(loc) {
    print();
}

MCLError::MCLError(std::string txt, Loc loc) : level(ERR_ERR), txt(txt),
comp(nullptr), loc(loc) {
    print();
}

void MCLError::print() const {
    if (!shouldBeDisplayed())
        return;
    std::string locText = loc.filename;
    if (loc.line == 0)
        locText += ":??";
    else
        locText += ":" + std::to_string(loc.line);
    if (loc.col != 0 && loc.line != 0)
        locText += ":" + std::to_string(loc.col);
    colorText(std::cerr, locText + ": ", "white bold");
    switch (level) {
        case ERR_NOTE:
            colorText(std::cerr, "note: ", "cyan bold");
        break;

        case ERR_WARN:
        case ERR_MINOR_WARN:
            colorText(std::cerr, "warning: ", "magenta bold");
        break;

        case ERR_ERR:
        default:
            colorText(std::cerr, "error: ", "red bold");
        break;
    }
    std::cerr << std::endl << "    " << txt << std::endl;
    if (level == ERR_ERR)
        exit(1);
}

bool MCLError::shouldBeDisplayed() const {
    if (comp == nullptr)
        return true;
    switch (comp->warnSetting) {
        case Compiler::WARNSETTING_ALL:
            if (level == ERR_NOTE || level == ERR_MINOR_WARN)
                return true;
            [[fallthrough]];
        case Compiler::WARNSETTING_MAJOR:
            if (level == ERR_WARN)
                return true;
            [[fallthrough]];
        case Compiler::WARNSETTING_NONE:
        default:
            if (level == ERR_ERR)
                return true;
        break;
    }
    return false;
}