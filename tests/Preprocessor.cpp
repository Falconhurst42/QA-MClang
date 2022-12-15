// Commands
// Tests MCLangs handling of preprocessor directives, specifically #define
// Ethan (created), 12/7/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|            Simple Defines             |
|                                       |
|***************************************/

// Define a void function name/declaration
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, FunctionDecDefine) {
    std::string name = "Preprocessor_FunctionDecDefine";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define foo void main()\n"
     + "foo{\n"
     + "/say \"Hello World!\"\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// Define a int function name/declaration
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, FunctionDecDefine_Int) {
    std::string name = "Preprocessor_FunctionDecDefine_Int";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define foo int main()\n"
     + "foo{\n"
     + "    /say \"Hello World!\"\n"
     + "    return 0;\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "1"), "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// Define a full MCFunction
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, MCFunctionAlias) {
    std::string name = "Preprocessor_MCFunctionAlias";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define talk /say \"Hello World!\"\n"
     + "void main() {\n"
     + "    talk\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// Define an execute clause
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, ExecuteClauseAlias) {
    std::string name = "Preprocessor_ExecuteClauseAlias";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define spreadit as(\"@a\")\n"
     + "void main() {\n"
     + "    spreadit\n"
     + "    /say \"Hello World!\"\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "execute as @a"));
    EXPECT_TRUE(anyFunctionContain(name, DEF_NAMESPACE, "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// use several defines to one-line a full program
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, OneLineIt) {
    std::string name = "Preprocessor_OneLineIt";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define foo void main()\n"
     + "#define spreadit as(\"@a\")\n"
     + "#define talk /say \"Hello World!\"\n"
     + "foo{spreadit talk}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "execute as @a"));
    EXPECT_TRUE(anyFunctionContain(name, DEF_NAMESPACE, "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

/***************************************|
|                                       |
|            Define Order               |
|                                       |
|***************************************/

// attempt to use alis before it gets defined
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, DefineAfter) {
    std::string name = "Preprocessor_DefineAfter";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "foo{\n"
     + "    /say \"Hello World!\"\n"
     + "    return 0;\n"
     + "}\n"
     + "#define foo int main()"
    );
    // doesn't work like that
    EXPECT_NE(rez.code, 0);

    // clean up built files (source will be created)
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// Use alias beforehand to mean one thing, then define it and use it to mean another
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, DefineAfterAndBefore) {
    std::string name = "Preprocessor_DefineAfterAndBefore";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "void bef() {\n"
     + "    const str defn = \"@a\";\n"
     + "    as(defn)\n"
     + "        /say \"say before\"\n"
     + "}"
     + "#define defn alt = \"@e\"\n"
     + "void aft() {\n"
     + "    const str defn;\n"
     + "    as(alt)\n"
     + "        /say \"say after\"\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "bef"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "bef"), "execute as @a"));
    EXPECT_TRUE(anyFunctionContain(name, DEF_NAMESPACE, "say \"say before\""));

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "aft"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "aft"), "execute as @e"));
    EXPECT_TRUE(anyFunctionContain(name, DEF_NAMESPACE, "say \"say after\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

/***************************************|
|                                       |
|           Multiline Define            |
|                                       |
|***************************************/

// spread a defin across multiple lines
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, MultilineIt) {
    std::string name = "Preprocessor_MultilineIt";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define foo void \\\n"
     + "main()\n"
     + "foo{\n"
     + "/say \"Hello World!\"\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// spread a define across far too many lines
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, UnreasonablyMutlilined) {
    std::string name = "Preprocessor_MultilineIt";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define foo v\\\n" + "o\\\n" + "i\\\n" + "d\\\n" + " \\\n"
     + "m\\\n" + "a\\\n" + "i\\\n" + "n\\\n"
     + "(\\\n" + ")\n"
     + "#define talk /\\\n" + "s\\\n" + "a\\\n" + "y\\\n"
     + " \\\n" + "\"\\\n"
     + "H\\\n" + "e\\\n" + "l\\\n" + "l\\\n" + "o\\\n"
     + " \\\n"
     + "W\\\n" + "o\\\n" + "r\\\n" + "l\\\n" + "d\\\n" + "!\\\n"
     + "\"\n"
     + "foo{\n"
     + "talk\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\""));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

/***************************************|
|                                       |
|            Function Manip             |
|                                       |
|***************************************/

// mix comments, MC functions, and defines
// put comment without define, it gets wrapped in command
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, CommandCommentParsed) {
    std::string name = "Preprocessor_CommandCommentParsed";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "void main() {\n"
     + "    /say \"Hello World!\" // commment here\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_TRUE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\" // commment here"));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

// mix comments, MC functions, and defines
// put comment with define, doesn't get wrapped in command
// Ethan, 12/7/2022, Should Pass
TEST(Preprocessor, CommandCommentNotParsed) {
    std::string name = "Preprocessor_CommandCommentNotParsed";
    CommandResult rez = makeAndBuildFile(name, std::string("")
     + "#define talk /say \"Hello World!\"\n"
     + "void main() {\n"
     + "    talk // commment here\n"
     + "}"
    );
    EXPECT_EQ(rez.code, 0);
    EXPECT_EQ(rez.output, "");

    EXPECT_TRUE(MCFunctionExists(name, DEF_NAMESPACE, "main"));
    EXPECT_FALSE(inStr(getMCFunction(name, DEF_NAMESPACE, "main"), "say \"Hello World!\" // commment here"));

    // clean up built files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}