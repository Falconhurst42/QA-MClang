// testArithmetic.cpp
#include "testDatapacks.cpp"
#include <gtest/gtest.h>

//../../build/main "testBuildArithmetic.mcl"
//Testing compile Time arithmetic
//All should pass except the comment ones 

TEST(Arithmetic, Addition) {
    ASSERT_EQ(true, fileExists("addition"));
    ASSERT_EQ(true, mathTests("addition", 20));
}

TEST(Arithmetic, Subtraction) {
    ASSERT_EQ(true, fileExists("subtraction"));
    ASSERT_EQ(true, mathTests("subtraction", 20));
}

TEST(Arithmetic, Multiplication) {
    ASSERT_EQ(true, fileExists("multiply"));
    ASSERT_EQ(true, mathTests("multiply", 20));
}

TEST(Arithmetic, Division) {
    ASSERT_EQ(true, fileExists("divide"));
    ASSERT_EQ(true, mathTests("divide", 20));
}

TEST(Arithmetic, Modulus) {
    ASSERT_EQ(true, fileExists("modulus"));
    ASSERT_EQ(true, mathTests("modulus", 2));
}

TEST(Arithmetic, Order) {
    ASSERT_EQ(true, fileExists("order"));
    ASSERT_EQ(true, mathTests("order", 20));
}

TEST(Arithmetic, Order2) {
    ASSERT_EQ(true, fileExists("order2"));
    ASSERT_EQ(true, mathTests("order2", 12));
}

TEST(Arithmetic, CompareTrue) {
    ASSERT_EQ(true, fileExists("comptrue"));
    ASSERT_EQ(true, mathTests("comptrue", 1));
}

TEST(Arithmetic, CompareFalse) {
    ASSERT_EQ(true, fileExists("compfalse"));
    ASSERT_EQ(true, mathTests("compfalse", 0));
}

TEST(Arithmetic, CompareOr) {
    ASSERT_EQ(true, fileExists("compor"));
    ASSERT_EQ(true, mathTests("compor", 1));
}

TEST(Arithmetic, CompareNot) {
    ASSERT_EQ(true, fileExists("compnot"));
    ASSERT_EQ(true, mathTests("compnot", 0));
}

TEST(cLike, SingleComment) {
    ASSERT_EQ(true, fileExists("comment1"));
    ASSERT_EQ(true, mathTests("comment1", 10));
}

TEST(cLike, MultiComment) {
    ASSERT_EQ(true, fileExists("comment2"));
    ASSERT_EQ(true, mathTests("comment2", 10));
}