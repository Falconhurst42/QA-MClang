
#ifndef __STRMANIP_H__
#define __STRMANIP_H__

#include <map>
#include <string>

/**
 * Check if a character is alphanumeric or an underscore
 * @param c The input character
 * @return A boolean indicating if the given character is alphanumeric or an
 * underscore
 */
bool isAlphaNumUS(char c);

/**
 * Get a thousands-separated version of the given integer
 * @param num The integer to insert thousands-separators in
 * @param sep The separator to use, default is ","
 * @return The number with thousands-separators inserted, as a string
 */
std::string thousandsSep(unsigned int num, std::string sep = ",");

/**
 * Replace all "inserted values" (using "{{" and "}}") with the given constants.
 * If no constant is found matching the name inside the brackets, then it's not
 * replaced
 * @param str The original string to replace the constants in
 * @param constValues A map of constant names and their values
 * @return A string with the given constants replaced
 */
std::string replaceInsertedValues(std::string str, std::map<std::string,
std::string> constValues);

/**
 * A helper object to replace constant values in a string, used in the
 * `replaceInsertedValues` function
 */
class ReplaceConstHelper {

public:

    /**
     * Constructor
     * @param str The original string to replace the constants in
     * @param constValues A map of constant names and their values
     */
    ReplaceConstHelper(std::string str, std::map<std::string, std::string>
    constValues);

    /**
     * Destructor
     */
    ~ReplaceConstHelper();

    /**
     * Process the string and give the string with the constants replaced
     * @return The input string with the constants replaced
     */
    std::string process();

private:

    /**
     * Get the current character
     * @note Assumes that `index` is lower than the length of `str`
     */
    char cur() const;

    /**
     * Move to the next character
     * @post `index` is incremented
     */
    void next();

    /**
     * Check if we're reading past the end of the input string
     * @return A boolean indicating if `index >= str.size()`
     */
    bool atEnd() const;

    /**
     * Read in a single '{', or a full constant if this is the start of a
     * constant
     * @post Adds the read characters or constant value to `out`
     */
    void readBracket();

    /**
     * Read the second bracket in "{{", or a full constant if this is the start
     * of a constant
     * @post Adds the read characters or constant value to `out`
     */
    void readSecondBracket();

    // Index of current character being read
    unsigned int index;

    // The original string
    std::string str;

    // The constant name and value pairs
    std::map<std::string, std::string> constValues;

    // The output string
    std::string out;

};

#endif