// PythonCleanup (placeholder)
// Suite for functions which passed a pack to python to perform verification on the results
//    defunct until python integration finished
// Ethan (created), 11/13/2022, 0.3.4-alpha
// Ethan (updated), 11/14/2022, 0.3.4-alpha
// Ethan (updated), 11/21/2022, 0.3.4-alpha
// Ethan (updated), 11/29/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"
#include "PythonSetup.h"
#include <fstream>
#include <string>

// one test for every test that sent a pack to python

// Really here to test python integration
// Ethan, 11/13/2022, Should Pass
TEST(PythonCleanup, BuildBasics_HelloWorld) {
    std::string out = python_packs["BuildBasics_HelloWorld"]._awaitPythonOutput();
    EXPECT_EQ(out, std::to_string(10));
    python_packs["BuildBasics_HelloWorld"]._cleanupPythonFiles();
    python_packs.erase("BuildBasics_HelloWorld");
}