#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include <fstream>

const Datapack::Variable N_VAR = {"int", "n"},
                         I_VAR = {"int", "i"};

const Datapack::Function HELLO_WORLD = {
    "void",
    "hello_world",
    std::vector<Datapack::Variable>(),
    "    /say Hello world!"
}, LOAD_N_I = {
    "void",
    "load",
    std::vector<Datapack::Variable>(),
    std::string("    n = 0;") + "\n" +
    "    i = 0;"
}, TICK_N_I = {
    "void",
    "tick",
    std::vector<Datapack::Variable>(),
    std::string("    n = n - 1;") + "\n" +
    "    i = i + 1;"
};

const Datapack::Namespace TEST_NAMESPACE("gtest");

const Datapack HELLO_FILE = {
    (std::vector<Datapack::Function>){ HELLO_WORLD }
};

// verifies that file has built properly
//  - with successful return code
//  - without any output
//  - datapack folder created
void VerifyGoodBuild(const Datapack& src) {
    // verify success code
    EXPECT_EQ(src.rez.code, 0);
    // verify no output
    EXPECT_EQ(src.rez.output, "");

    // verify file structure
    const std::string BASE_PATH = src.getCompiledPath(),
                      NAMES = src.getPrimaryNamespace();
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + NAMES));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + NAMES + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileExists(BASE_PATH + "/pack.mcmeta"));

    // verify function files are created
    const std::string FUNCT_PATH = src.getFunctionsPath();
    const std::string TAG_PATH = src.getTagsPath();
    for(const Datapack::Function& f : src.foos) {
        EXPECT_TRUE(fileExists(FUNCT_PATH + "/" + f.name + ".mcfunction"));
        if(std::find(TAGGED_FUNCT_NAMES.cbegin(), TAGGED_FUNCT_NAMES.cend(), f.name) != TAGGED_FUNCT_NAMES.cend()) {
            // check for file
            EXPECT_TRUE(fileExists(TAG_PATH + "/" + f.name + ".json"));
            // check file contents
            EXPECT_EQ(getFileContents(TAG_PATH + "/" + f.name + ".json"), "{\"values\":[\"" + NAMES + ":" + f.name + "\"]}");
        }
    }
}

// verifies that file has built properly
//  - without successful return code
//  - with output containing `err`
//  - datapack folder not created
void VerifyBadBuild(const Datapack& src, const std::string& err = "") {
    // verify success code
    EXPECT_NE(src.rez.code, 0);
    // verify err in output
    if(err != "")
        EXPECT_TRUE(inStr(src.rez.output, err));

    // verify file structure
    const std::string BASE_PATH = src.getCompiledPath();
    EXPECT_FALSE(directoryExists(BASE_PATH));
}

/***************************************|
|                                       |
|            Trivial Build              |
|                                       |
|***************************************/

TEST(MCLangTests, VerifyBuild_HelloWorld) {
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|              Tick/Load                |
|                                       |
|***************************************/

TEST(MCLangTests, VerifyBuild_LoadNI) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(MCLangTests, VerifyBuild_LoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(MCLangTests, VerifyBuild_TickNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(MCLangTests, VerifyBuild_TickNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

TEST(MCLangTests, VerifyBuild_TickLoadNI) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I }, (std::vector<Datapack::Variable>){ N_VAR, I_VAR });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

TEST(MCLangTests, VerifyBuild_TickLoadNI_NoVars) {
    Datapack src((std::vector<Datapack::Function>){ TICK_N_I, LOAD_N_I });
    src.name = "VerifyBuild_TickLoadNI";
    src.build();
    VerifyBadBuild(src, "Use of uninitialized variable");

    // built files cleaned up by destructor when src falls out of scope
}

/***************************************|
|                                       |
|          Simple Namespacing           |
|                                       |
|***************************************/

// verifies that basic file builds with successful return code when provided -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_DashN";
    src.build(TEST_NAMESPACE);
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced";
    src.space = TEST_NAMESPACE;
    src.build();
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a concurring -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced_DashN";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE);
    VerifyGoodBuild(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a conflicting -n arg
TEST(MCLangTests, VerifyBuild_HelloWorld_Namespaced_DashNConflicting) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced_DashNConflicting";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE.space + "2");
    VerifyGoodBuild(src);
}

/***************************************|
|                                       |
|             Invalid Path              |
|                                       |
|***************************************/

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
TEST(MCLangTests, InvalidFile_BuildFails) {
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