
#ifndef __ERRORHANDLE_H__
#define __ERRORHANDLE_H__

#include "general/loc.h"
#include <string>

class Compiler;

enum ErrType {
    ERR_ERR, ERR_WARN, ERR_MINOR_WARN, ERR_NOTE
};

/**
 * Error object, shows the error/warning/note on creation
 */
class MCLError {

public:

    /**
     * Constructor
     * @param level The level of seriousness
     * @param txt The error text
     * @param comp The main compiler
     * @param loc The location of error, which also includes file information
     */
    MCLError(ErrType level, std::string txt, Compiler *comp, Loc loc =
    Loc::unknown);

    /**
     * Constructor
     * Level is set by default to "ERR_ERR" and compiler does not need to be
     * passed
     * @param txt The error text
     * @param loc The location of error, which also includes file information
     */
    MCLError(std::string txt, Loc loc = Loc::unknown);

private:

    /**
     * Print out the error on the screen
     */
    void print() const;

    /**
     * Check the current error should be displayed based on the compiler
     * settings
     * @return A boolean indicating if the error should be displayed
     * @note If there is no compiler pointer stored, always returns true
     */
    bool shouldBeDisplayed() const;

    // Level of seriousness
    ErrType level;

    // Error text
    std::string txt;

    // The main compiler, could be null
    Compiler *comp;

    // Location of the error
    Loc loc;

};

#endif