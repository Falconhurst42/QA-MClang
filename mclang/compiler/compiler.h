
#ifndef __COMPILER_H__
#define __COMPILER_H__

class Lexer;
class Parser;
class BCManager;
class BCConverter;
class FileManager;
class Preprocessor;

#define COMPILER_STAT_LEFTPAD 40

#include "bcconvert/bcconvert.h"
#include <map>
#include <string>

/**
 * Main compiler component, that handles all the subcomponents
 * Keeps track of certain things that need to be communicated between components
 */
class Compiler {

public:

    /**
     * Constructor
     */
    Compiler();

    /**
     * Destructor
     */
    ~Compiler();

    /**
     * Compile the stored input
     * @pre `input` variable is set
     * @post All components are now valid pointers
     */
    void compile();

    // Input filename
    std::string filename;

    // Namespace to use in the output
    std::string ns;

    // Datpack output folder
    std::string outputFolder;

    // Debug mode switch
    bool debugMode;

    // File output enable/disable
    bool fileOutput;

    // Scoreboard objective name
    std::string scoreboardName;

    // Minecraft version
    std::string mcVersion;

    // Datapack description
    std::string description;

    // Amount of warnings that should be shown to the user
    enum {
        WARNSETTING_ALL, WARNSETTING_MAJOR, WARNSETTING_NONE
    } warnSetting;

    // Switch to show statistics about the generated datapack
    bool showStatistics;

    // Generated statistics
    struct {
        unsigned int bcInstrCount;
        unsigned int cmdCount;
        unsigned int cmdFunctionCount;
        unsigned int funcCallCount;
        unsigned int stackOpCount;
    } stats;

    // NOTE: These components may still may null values when the compilation is
    // not done
    // Preprocessor compoenent
    Preprocessor *prep;
    // Lexer component
    Lexer *lexer;
    // Parser component
    Parser *parser;
    // Bytecode generator manager component
    BCManager *bcMan;
    // Bytecode converter component
    BCConverter *bcConvert;

private:

    /**
     * Run the preprocessor
     * @post `prep` is no longer null
     */
    void runPreprocessor();

    /**
     * Run the lexer
     * @post `lexer` is no longer null
     */
    void runLexer();

    /**
     * Run the parser
     * @post `parser` is no longer null
     */
    void runParser();

    /**
     * Generate bytecode
     * @post `bcMan` is no longer null
     */
    void runBCGenerator();

    /**
     * Convert bytecode to commands
     * @post `bcConvert` is no longer null
     */
    void runBCConverter();

    /**
     * Run the file manager to output commands to files
     */
    void runFileOutput();

    /**
     * Reset statistics values to base values
     */
    void resetStatistics();

    /**
     * Display gathered statistics about the generated datapack
     */
    void displayStatistics() const;

    /**
     * Display a single statistic
     * @param txt The statistic description
     * @param num The statistic value
     */
    void displayStatistic(std::string txt, unsigned int num) const;

    // Commands output from the bytecode converter
    std::vector<CmdFunc> cmds;

};

#endif