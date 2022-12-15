// BasicMath
// Verifies behavior of MCLang's run-time arithmetic
// Brendan (created), 0.3.4-alpha
// Adam, Ethan (ported), 12/15/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"

// Test run-time subtraction
// Brendan, Should Pass
TEST(BasicMath, SubractOne) {
    Datapack src(SUB_FILE);
    src.name = "VerifyBuild_SubOne";
    src.space = TEST_NAMESPACE;
    src.build();
    const std::string folderString = src._getMCFunction(SUB_ONE.getFormattedName());
    EXPECT_TRUE(inStr(folderString, "scoreboard players remove x mclang 1" ));
}

// Test run-time addition
// Brendan, Should Pass
TEST(BasicMath, AddOne) {
    Datapack src(ADD_FILE);
    src.name = "VerifyBuild_AddOne";
    src.space = TEST_NAMESPACE;
    src.build();
    const std::string folderString = src._getMCFunction(ADD_ONE.getFormattedName());
    EXPECT_TRUE(inStr(folderString, "scoreboard players add x mclang 1"));
}

// Test run-time division
// Brendan, Should Pass
TEST(BasicMath, DivFour) {
    Datapack src(DIV_FILE);
    src.name = "VerifyBuild_DivFour";
    src.space = TEST_NAMESPACE;
    src.build();
    const std::string folderString = src._getMCFunction(DIV_FOUR.getFormattedName());
    EXPECT_TRUE(inStr(folderString, "scoreboard players set 0itmp mclang 4"));
    EXPECT_TRUE(inStr(folderString, "scoreboard players operation 0tmp0 mclang /= 0itmp mclang"));    
}

// Test run-time mutliplication
// Brendan, Should Pass
TEST(BasicMath, MultFour) {
    Datapack src(MULT_FILE);
    src.name = "VerifyingBuild_MultFour";
    src.space = TEST_NAMESPACE;
    src.build();
    const std::string folderString = src._getMCFunction(MULT_FOUR.getFormattedName());
    EXPECT_TRUE(inStr(folderString, "scoreboard players set 0itmp mclang 3"));
    EXPECT_TRUE(inStr(folderString, "scoreboard players operation 0tmp0 mclang *= 0itmp mclang")); 
}

// Test conditional summoning
// Brendan, Should Pass
TEST(MCfuncs, summon) {
    Datapack src(SUMMON_FILE);
    src.name = "VerifyingBuild_SummonOce";
    src.space = TEST_NAMESPACE;
    src.build();
    const std::string folderString = src._getMCFunction(SUMMON_OCE.getFormattedName());
    EXPECT_TRUE(inStr(folderString, "execute if score 0ret mclang matches 0 run function gtest:3"));
}