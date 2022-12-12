// tests.cpp
//#include "testDatapacks.cpp"
#include "TestSetup.h"
#include <gtest/gtest.h>

//../../build/main "testBuild.mcl"
//Need to build the files first then do make then .\runtests
//should have 3 pass and 7 fail

//Newline \n
TEST(EscapeCharacters, NewLine) {
    Datapack src((std::vector<Datapack::Function>){ ESCNEWLINE });
    src.name = "EscapeCharacters_NewLine";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCNEWLINE.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCNEWLINE.name), "\\n"));

    // built files cleaned up by destructor when src falls out of scope
}

//Tab \t
TEST(EscapeCharacters, Tab) {
    Datapack src((std::vector<Datapack::Function>){ ESCTAB });
    src.name = "EscapeCharacters_Tab";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCTAB.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCTAB.name), "\\t"));

    // built files cleaned up by destructor when src falls out of scope
}

//Alert \a
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Alert) {
    Datapack src((std::vector<Datapack::Function>){ ESCALERT });
    src.name = "EscapeCharacters_Alert";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCALERT.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCALERT.name), "aAlert"));

    // built files cleaned up by destructor when src falls out of scope
}

//Backspace \b
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Backspace) {
    Datapack src((std::vector<Datapack::Function>){ ESCBACKSPACE });
    src.name = "EscapeCharacters_Backspace";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCBACKSPACE.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCBACKSPACE.name), "bBackspace"));

    // built files cleaned up by destructor when src falls out of scope
}

//NewPage \f
//Currently should fail since it isn't supported
TEST(EscapeCharacters, NewPage) {
    Datapack src((std::vector<Datapack::Function>){ ESCNEWPAGE });
    src.name = "EscapeCharacters_NewPage";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCNEWPAGE.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCNEWPAGE.name), "fNewpage"));

    // built files cleaned up by destructor when src falls out of scope
}

//Return \r
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Return) {
    Datapack src((std::vector<Datapack::Function>){ ESCRETURN });
    src.name = "EscapeCharacters_Return";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCRETURN.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCRETURN.name), "rReturn"));

    // built files cleaned up by destructor when src falls out of scope
}

//Vertical Tab \v
TEST(EscapeCharacters, VertTab) {
    Datapack src((std::vector<Datapack::Function>){ ESCVTAB });
    src.name = "EscapeCharacters_VertTab";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCVTAB.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCVTAB.name), "\\v"));

    // built files cleaned up by destructor when src falls out of scope
}

//Backslash
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Backslash) {
    Datapack src((std::vector<Datapack::Function>){ ESCBACKSLASH });
    src.name = "EscapeCharacters_Backslash";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCBACKSLASH.name));
    EXPECT_FALSE(inStr(src._getMCFunction(ESCBACKSLASH.name), "\\"));

    // built files cleaned up by destructor when src falls out of scope
}

//Single Quote \'
//Currently should fail since it isn't supported
TEST(EscapeCharacters, SingleQuote) {
    Datapack src((std::vector<Datapack::Function>){ ESCSINGQUOTE });
    src.name = "EscapeCharacters_SingleQuote";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCSINGQUOTE.name));
    EXPECT_TRUE(inStr(src._getMCFunction(ESCSINGQUOTE.name), "\'"));

    // built files cleaned up by destructor when src falls out of scope
}

//Double Quote \"
//Currently should fail since it isn't supported
TEST(EscapeCharacters, DoubleQuote) {
    Datapack src((std::vector<Datapack::Function>){ ESCDOUBQUOTE });
    src.name = "EscapeCharacters_DoubleQuote";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ESCDOUBQUOTE.name));
    EXPECT_TRUE(inStr(src._getMCFunction(ESCDOUBQUOTE.name), "\""));

    // built files cleaned up by destructor when src falls out of scope
}