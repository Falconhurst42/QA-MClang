#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/PackFiles.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"
#include <fstream>

/***************************************|
|                                       |
|            Trivial Build              |
|                                       |
|***************************************/

TEST(BuildBasics, HelloWorld) {
    /*Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld";
    src.build();
    VERIFY_GOOD_BUILD(src);*/
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld";
    PackFiles pf(src);
    pf.VERIFY_GOOD_BUILD();

    // built files cleaned up by destructor when pf falls out of scope
    system((std::string("cp -r ") + src._makeCompiledPath() + " " + src._makePythonPath()).c_str());
    system("python3 standin.py");
}

/***************************************|
|                                       |
|             Invalid Path              |
|                                       |
|***************************************/

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
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