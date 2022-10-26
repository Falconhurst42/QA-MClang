#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|              Tick/Load                |
|                                       |
|***************************************/

TEST(Functions, LoadNI) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, LoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickLoadNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, TickLoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VERIFY_BAD_BUILD(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}