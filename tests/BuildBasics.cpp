#ifndef BB
#define BB
// BuildBasics
// Verifies fundamental behavior of MCLang build
// Ethan (created), 10/19/2022, 0.3.4-alpha
// Ethan (updated), 10/26/2022, 0.3.4-alpha
// Ethan (updated), 11/14/2022, 0.3.4-alpha
// Ethan (updated), 12/15/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/PackFiles.h"
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"
// #include "PythonSetup.h"
#include <fstream>
#include <thread>

/***************************************|
|                                       |
|            Trivial Build              |
|                                       |
|***************************************/

// Verifies MCLang will suitably build a basic "Hello World" file
// Ethan (created), 10/19/2022, Should Pass
// Ethan (updated), 10/26/2022, Should Pass
// Ethan (updated), 11/14/2022, Should Pass
// Ethan (updated), 12/15/2022, Should Pass
TEST(BuildBasics, HelloWorld) {
    Datapack src(HELLO_FILE);
    src.name = "BuildBasics_HelloWorld";
    PackFiles pf(src);
    pf.VERIFY_GOOD_BUILD();

    // built files cleaned up by destructor when pf falls out of scope
    //python_packs["BuildBasics_HelloWorld"] = src;
    //python_packs["BuildBasics_HelloWorld"]._pythonizePack();
}

/***************************************|
|                                       |
|             Invalid Path              |
|                                       |
|***************************************/

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
// Ethan (created), 10/19/2022, Should Pass
TEST(BuildBasics, InvalidFile) {
    // find path with no `.mcl` file
    std::string name = "f";
    if(fileExists(makeSourcePath(name))) {
        name += "f";
    }
    CommandResult br = buildFile(makeSourcePath(name), makeCompiledPath(name));
    EXPECT_NE(br.code, 0);
    EXPECT_TRUE(inStr(br.output, "Could not open given file"));\
    // cleanup files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}

#endif