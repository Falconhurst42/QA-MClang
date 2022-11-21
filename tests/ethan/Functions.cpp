#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|             Name Casing               |
|                                       |
|***************************************/

TEST(Functions, CamelCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_CamelCaseName";
    src.getFoo("hello_world").name = "helloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists("hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, PascalCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_PascalCaseName";
    src.getFoo("hello_world").name = "HelloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(".hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, UpperCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_UpperCaseName";
    src.getFoo("hello_world").name = "HELLO_WORLD";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(".h.e.l.l.o_.w.o.r.l.d"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|            Starting Char              |
|                                       |
|***************************************/
TEST(Functions, StartWithUnderscore) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_StartWithUnderscore";
    src.getFoo("hello_world").name = "_helloworld";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, StartWithDigit) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_StartWithDigit";
    src.getFoo("hello_world").name = "1helloworld";
    src.build();
    VERIFY_BAD_BUILD(src, "Expected token 'WORD', instead got 'NUM'.");

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|            Similar Names              |
|                                       |
|***************************************/
TEST(Functions, CaseDiff) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_SameName";
    src.foos.push_back(src.getFoo("hello_world"));
    // will find original funct, not that it matters
    src.getFoo("hello_world").name = "Hello_World";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, CaseDiffPeriod) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_CaseDiffPeriod";
    src.foos.push_back(src.getFoo("hello_world"));
    // will find original funct, not that it matters
    src.getFoo("hello_world").name = "Hello_World";
    // will find copied funct, not that it matters
    src.getFoo("hello_world").name = ".hello_.world";
    src.build();
    VERIFY_BAD_BUILD(src, "Unrecognized token.");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, SameName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_SameName";
    src.foos.push_back(src.getFoo("hello_world"));
    src.build();
    VERIFY_BAD_BUILD(src, "Function with same name and parameter types was already defined.");

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|              Tick/Load                |
|                                       |
|***************************************/

TEST(Functions, LoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_LoadNI";
    src.dropFoo("tick");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, LoadNI_NoVars) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_LoadNI_NoVars";
    src.dropFoo("tick");
    src.dropVar("n");
    src.dropVar("i");
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_TickNI";
    src.dropFoo("load");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI_NoVars) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_TickNI_NoVars";
    src.dropFoo("load");
    src.dropVar("n");
    src.dropVar("i");
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickLoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}