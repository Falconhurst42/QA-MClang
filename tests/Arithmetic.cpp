// Arithmetic
// Verifies behavior of MCLang's compile-time arithmetic
// Adam (created), 11/16/2022, 0.3.4-alpha
// Adam, Ethan (ported), 12/12/2022, 0.3.4-alpha

#include "TestSetup.h"
#include "universal/Datapack.h"
#include <gtest/gtest.h>

// Test compile-time addition
// Adam, 11/16, Should Pass
TEST(Arithmetic, Addition) {
    Datapack src((std::vector<Datapack::Function>){ ADD });
    src.name = "Arithmetic_Addition";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ADD.name));
    EXPECT_TRUE(inStr(src._getMCFunction(ADD.name), "20"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time subtraction
// Adam, 11/16, Should Pass
TEST(Arithmetic, Subtraction) {
    Datapack src((std::vector<Datapack::Function>){ SUB });
    src.name = "Arithmetic_Subtraction";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(SUB.name));
    EXPECT_TRUE(inStr(src._getMCFunction(SUB.name), "20"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time multiplication
// Adam, 11/16, Should Pass
TEST(Arithmetic, Multiplication) {
    Datapack src((std::vector<Datapack::Function>){ MULT });
    src.name = "Arithmetic_Multiplication";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(MULT.name));
    EXPECT_TRUE(inStr(src._getMCFunction(MULT.name), "20"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time division
// Adam, 11/16, Should Pass
TEST(Arithmetic, Division) {
    Datapack src((std::vector<Datapack::Function>){ DIV });
    src.name = "Arithmetic_Division";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(DIV.name));
    EXPECT_TRUE(inStr(src._getMCFunction(DIV.name), "20"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time modulus
// Adam, 11/16, Should Pass
TEST(Arithmetic, Modulus) {
    Datapack src((std::vector<Datapack::Function>){ MOD });
    src.name = "Arithmetic_Modulus";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(MOD.name));
    EXPECT_TRUE(inStr(src._getMCFunction(MOD.name), "2"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time order of operations
// Adam, 11/16, Should Pass
TEST(Arithmetic, Order) {
    Datapack src((std::vector<Datapack::Function>){ PEMDAS });
    src.name = "Arithmetic_Order";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(PEMDAS.name));
    EXPECT_TRUE(inStr(src._getMCFunction(PEMDAS.name), "20"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time order of operations
// Adam, 11/16, Should Pass
TEST(Arithmetic, Order2) {
    Datapack src((std::vector<Datapack::Function>){ PEMDAS2 });
    src.name = "Arithmetic_Order2";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(PEMDAS2.name));
    EXPECT_TRUE(inStr(src._getMCFunction(PEMDAS2.name), "12"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comparison operators
// Adam, 11/16, Should Pass
TEST(Arithmetic, CompareTrue) {
    Datapack src((std::vector<Datapack::Function>){ TCOMP });
    src.name = "Arithmetic_CompareTrue";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(TCOMP.name));
    EXPECT_TRUE(inStr(src._getMCFunction(TCOMP.name), "1"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comparison operators
// Adam, 11/16, Should Pass
TEST(Arithmetic, CompareFalse) {
    Datapack src((std::vector<Datapack::Function>){ FCOMP });
    src.name = "Arithmetic_CompareFalse";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(FCOMP.name));
    EXPECT_TRUE(inStr(src._getMCFunction(FCOMP.name), "0"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comparison operators (or)
// Adam, 11/16, Should Pass
TEST(Arithmetic, CompareOr) {
    Datapack src((std::vector<Datapack::Function>){ ORCOMP });
    src.name = "Arithmetic_CompareOr";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(ORCOMP.name));
    EXPECT_TRUE(inStr(src._getMCFunction(ORCOMP.name), "1"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comparison operators (not)
// Adam, 11/16, Should Pass
TEST(Arithmetic, CompareNot) {
    Datapack src((std::vector<Datapack::Function>){ NOTCOMP });
    src.name = "Arithmetic_CompareNot";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(NOTCOMP.name));
    EXPECT_TRUE(inStr(src._getMCFunction(NOTCOMP.name), "0"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comment parsing
// Adam, 11/16, Should Pass
TEST(cLike, SingleComment) {
    Datapack src((std::vector<Datapack::Function>){ COMMENT1 });
    src.name = "Arithmetic_SingleComment";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(COMMENT1.name));
    EXPECT_FALSE(inStr(src._getMCFunction(COMMENT1.name), "10"));

    // built files cleaned up by destructor when src falls out of scope
}

// Test compile-time comment parsing
// Adam, 11/16, Should Pass
TEST(cLike, MultiComment) {
    Datapack src((std::vector<Datapack::Function>){ COMMENT2 });
    src.name = "Arithmetic_MultiComment";
    src.build();
    EXPECT_TRUE(src._MCFunctionExists(COMMENT2.name));
    EXPECT_FALSE(inStr(src._getMCFunction(COMMENT2.name), "10"));

    // built files cleaned up by destructor when src falls out of scope
}