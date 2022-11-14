#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"
#include "PythonSetup.h"
#include <fstream>
#include <string>

TEST(PythonCleanup, HelloWorld) {
    std::string out = python_packs["HelloWorld"]._awaitPythonOutput();
    EXPECT_EQ(out, std::to_string(10));
    python_packs["HelloWorld"]._cleanupPythonFiles();
    python_packs.erase("HelloWorld");
}