#include <gtest/gtest.h>
#include "../universal/utils.h"
#include <fstream>

const SourceFile::Function HELLO_WORLD = {
    "void",
    "hello_world",
    std::vector<SourceFile::Variable>(),
    "    /say Hello world!"
};

const SourceFile::Namespace TEST_NAMESPACE("gtest");

const SourceFile HELLO_FILE = {
    (std::vector<SourceFile::Function>){ HELLO_WORLD }
};

// verifies that file has built properly
//  - with successful return code
//  - without any output
//  - datapack folder created
void VerifyGoodBuild(const SourceFile& src) {
    // verify success code
    EXPECT_EQ(src.rez.code, 0);
    // verify no output
    EXPECT_EQ(src.rez.output, "");

    // verify file structure
    const std::string BASE_PATH = src.getCompiledPath();
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + src.getPrimaryNamespace()));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + src.getPrimaryNamespace() + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
}

TEST(MCLangTests, VerifyBuild_HelloWorld) {
    SourceFile src(HELLO_FILE);
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file builds with successful return code when provided -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_DashN) { 
    SourceFile src(HELLO_FILE);
    src.build(TEST_NAMESPACE);
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced) { 
    SourceFile src(HELLO_FILE);
    src.space = TEST_NAMESPACE;
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a concurring -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced_DashN) { 
    SourceFile src(HELLO_FILE);
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE);
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a conflicting -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced_DashNConflicting) { 
    SourceFile src(HELLO_FILE);
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE.space + "2");
    VerifyGoodBuild(src);
}

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
TEST(MCLangTests, InvalidFile_BuildFails) {
    // find path with no `.mcl` file
    std::string name = "f";
    if(fileCanOpen(makeSourcePath(name))) {
        name += "f";
    }
    CommandResult br = buildFile(makeSourcePath(name), makeCompiledPath(name));
    EXPECT_NE(br.code, 0);
    EXPECT_TRUE(inStr(br.output, "Could not open given file"));\
    // cleanup files
    cleanupBuildFiles(makeSourcePath(name), makeCompiledPath(name));
}