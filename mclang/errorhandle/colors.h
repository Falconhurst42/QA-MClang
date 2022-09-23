
#ifndef __ERRORHANDLE_COLORS_H__
#define __ERRORHANDLE_COLORS_H__

#include <map>
#include <string>

#ifdef OS_WINDOWS

#include <windows.h>

// First in the pair is foreground color, second is background color
// When background is set, the foreground is set to white
const std::map<std::string, std::pair<int, int>> colorNames = {
    {"black", {0, 40}},
    {
        "red", {
            FOREGROUND_RED | FOREGROUND_INTENSITY,
            BACKGROUND_RED | BACKGROUND_INTENSITY
        }
    }, {
        "green", {
            FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BACKGROUND_GREEN | BACKGROUND_INTENSITY
        }
    }, {
        "yellow", {
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY
        }
    }, {
        "blue", {
            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BACKGROUND_BLUE | BACKGROUND_INTENSITY
        }
    },
    {
        "magenta", {
            FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY
        }
    }, {
        "cyan", {
            FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_INTENSITY
        }
    }, {
        "white", {
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE |
            FOREGROUND_INTENSITY,
            BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE |
            BACKGROUND_INTENSITY
        }
    }
};

#else

const std::map<std::string, std::string> colorNames = {
    {"black", "30"},
    {"red", "31"},
    {"green", "32"},
    {"yellow", "33"},
    {"blue", "34"},
    {"magenta", "35"},
    {"cyan", "36"},
    {"white", "37"},
    {"bold", "1"}
};

#endif

/**
 * Output text in the given color to an output stream
 * @param os The output stream to send the colored text to
 * @param txt The text to output
 * @param color The color names of the text, see above names. Provided separated
 * by spaces to allow combinations like "red bold"
 */
void colorText(std::ostream &os, const std::string &txt, std::string color);

#endif