// tests.cpp
#include "testDatapacks.cpp"
#include <gtest/gtest.h>

//../../build/main "testBuild.mcl"
//Need to build the files first then do make then .\runtests
//should have 3 pass and 7 fail
 
//Newline \n
TEST(EscapeCharacters, NewLine) {
    ASSERT_EQ(true, fileExists("newline"));
    ASSERT_EQ(true, testEscape("newline", "\\n"));
}

//Tab \t
TEST(EscapeCharacters, Tab) {
    ASSERT_EQ(true, fileExists("tab"));
    ASSERT_EQ(true, testEscape("tab", "\\t"));
}

//Alert \a
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Alert) {
    ASSERT_EQ(true, fileExists("alert"));
    ASSERT_EQ(true, testEscape("alert", "aAlert"));
}

//Backspace \b
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Backspace) {
    ASSERT_EQ(true, fileExists("backspace"));
    ASSERT_EQ(true, testEscape("backspace", "bBackspace"));
}

//NewPage \f
//Currently should fail since it isn't supported
TEST(EscapeCharacters, NewPage) {
    ASSERT_EQ(true, fileExists("newpage"));
    ASSERT_EQ(true, testEscape("newpage", "fNewpage"));
}

//Return \r
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Return) {
    ASSERT_EQ(true, fileExists("return"));
    ASSERT_EQ(true, testEscape("return", "rReturn"));
}

//Vertical Tab \v
TEST(EscapeCharacters, VertTabe) {
    ASSERT_EQ(true, fileExists("vtab"));
    ASSERT_EQ(true, testEscape("vtab", "\\v"));
}

//Backslash
//Currently should fail since it isn't supported
TEST(EscapeCharacters, Backslash) {
    ASSERT_EQ(true, fileExists("backslash"));
    ASSERT_EQ(true, testEscape("backslash", "\\"));
}

//Single Quote \'
//Currently should fail since it isn't supported
TEST(EscapeCharacters, SingleQuote) {
    ASSERT_EQ(true, fileExists("singquote"));
    ASSERT_EQ(true, testEscape("singquote", "\'"));
}

//Double Quote \"
//Currently should fail since it isn't supported
TEST(EscapeCharacters, DoubleQuote) {
    ASSERT_EQ(true, fileExists("doubquote"));
    ASSERT_EQ(true, testEscape("doubquote", "\""));
}