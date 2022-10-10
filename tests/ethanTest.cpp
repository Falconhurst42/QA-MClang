#include <gtest/gtest.h>
#include "utils.h"

TEST(MCLangTests, RunBuild_ValidFile) { 
    CommandResult br = buildFile("fizzbuzz");
    EXPECT_EQ(br.code, 0);
    EXPECT_EQ(br.output, "");
}

TEST(MCLangTests, RunBuild_InvalidPath) { 
    CommandResult br = buildFile("fizzbuzzzzzz");
    EXPECT_NE(br.code, 0);
    EXPECT_TRUE(inStr(br.output, "Could not open given file"));
}