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

TEST(Functions, VerifyBuild_LoadNI) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, VerifyBuild_LoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, VerifyBuild_TickNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, VerifyBuild_TickNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, VerifyBuild_TickLoadNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(Functions, VerifyBuild_TickLoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}