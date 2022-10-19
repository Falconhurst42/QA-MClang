#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"
#include <fstream>

/***************************************|
|                                       |
|            Trivial Build              |
|                                       |
|***************************************/

TEST(BuildBasics, VerifyBuild_HelloWorld) {
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|             Invalid Path              |
|                                       |
|***************************************/

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
TEST(BuildBasics, InvalidFile_BuildFails) {
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