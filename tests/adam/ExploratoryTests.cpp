// ExploratoryTests.cpp
#include "testDatapacks.cpp"
#include <gtest/gtest.h>

//../../build/main "ExploratoryTesting.mcl"

//Testing inaccesibality
TEST(Inaccessability, FalseIf) {
    ASSERT_EQ(true, fileExists("unreachable"));
    ASSERT_EQ(false, findString("unreachable", "This should not be seen"));
}

//Testing statements after return
TEST(Inaccessability, postReturn) {
    ASSERT_EQ(true, fileExists("test1"));
    ASSERT_EQ(false, findString("test1", "Hidden"));
}

//Testing false algebraic statement
TEST(Inaccessability, FalseMath) {
    ASSERT_EQ(true, fileExists("test2"));
    ASSERT_EQ(false, findString("test2", "Hidden"));
}

//Testing return statement in always true if statement
TEST(Inaccessability, ReturnInsideIf) {
    ASSERT_EQ(true, fileExists("test3"));
    ASSERT_EQ(false, findString("test3", "this should not exist"));
}

//Testing entering if statements with false variable
TEST(Inaccessability, FalseBool) {
    ASSERT_EQ(true, fileExists("test4"));
    ASSERT_EQ(false, findString("test4", "No access"));
}
