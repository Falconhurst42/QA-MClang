// Commands
// Tests MCLangs handling of functions, especial naming
// Ethan (created), 10/19/2022, 0.3.4-alpha
// Ethan (updated), 10/26/2022, 0.3.4-alpha
// Ethan (updated), 11/12/2022, 0.3.4-alpha
// Ethan (updated), 11/21/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|             Name Casing               |
|                                       |
|***************************************/

// Checks handling of camel case function names
// Ethan, 11/12/2022, Should Pass
TEST(Functions, CamelCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_CamelCaseName";
    src.getFoo("hello_world").name = "helloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists("hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Checks handling of pascal case function names
// Ethan, 11/12/2022, Should Pass
TEST(Functions, PascalCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_PascalCaseName";
    src.getFoo("hello_world").name = "HelloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(".hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Checks handling of all uppercase function names
// Ethan, 11/12/2022, Should Pass
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

// Checks handling of function names beginning with an underscore
// Ethan, 11/21/2022, Should Pass
TEST(Functions, StartWithUnderscore) {
    Datapack src(HELLO_FILE);
    src.name = "Functions_StartWithUnderscore";
    src.getFoo("hello_world").name = "_helloworld";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Checks handling of function names (trying to) begin with an digit
// Ethan, 11/21/2022, Should Pass
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
// Checks handling of functions with similar names
// Ethan, 11/21/2022, Should Pass
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

// Checks handling of functions with similar names (trying to trick it with periods)
// Ethan, 11/21/2022, Should Pass
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

// Checks handling of functions with the same name (build fails)
// Ethan, 11/21/2022, Should Pass
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

// Checks handling of load function
// Ethan, 10/19/2022, Should Pass
TEST(Functions, LoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_LoadNI";
    src.dropFoo("tick");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Checks handling of load function (without initializing variables)
// Ethan, 10/19/2022, Should Pass
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

// Checks handling of tick function
// Ethan, 10/19/2022, Should Pass
TEST(Functions, TickNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_TickNI";
    src.dropFoo("load");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// Checks handling of tick function (without initializing variables)
// Ethan, 10/19/2022, Should Pass
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

// Checks handling of tick and load function
// Ethan, 10/19/2022, Should Pass
TEST(Functions, TickLoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "Functions_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}