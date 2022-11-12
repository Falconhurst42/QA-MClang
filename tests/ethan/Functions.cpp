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
    src.name = "VerifyBuild_CamelCaseName";
    src.getFoo("hello_world").name = "helloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists("hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, PascalCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_CamelCaseName";
    src.getFoo("hello_world").name = "HelloWorld";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(".hello.world"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, UpperCaseName) {
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_CamelCaseName";
    src.getFoo("hello_world").name = "HELLO_WORLD";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(".h.e.l.l.o_.w.o.r.l.d"));
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|              Tick/Load                |
|                                       |
|***************************************/

TEST(Functions, LoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "VerifyBuild_TickLoadNI";
    src.dropFoo("tick");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, LoadNI_NoVars) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "VerifyBuild_TickLoadNI";
    src.dropFoo("tick");
    src.dropVar("n");
    src.dropVar("i");
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "VerifyBuild_TickLoadNI";
    src.dropFoo("load");
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI_NoVars) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "VerifyBuild_TickLoadNI";
    src.dropFoo("load");
    src.dropVar("n");
    src.dropVar("i");
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickLoadNI) {
    Datapack src(NI_TICK_LOAD_FILE);
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}