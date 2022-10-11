#include <gtest/gtest.h>
#include "../universal/utils.h"
#include <fstream>

const std::string DEF_NAMESPACE = "dp",
    TEST_NAMESPACE = "gtest",
    NAMESPACER = "namespace " + TEST_NAMESPACE + ";\n\n",
    HELLO_WORLD = std::string("") +
    "void main() {\n" +
    "    /say Hello world!\n" +
    "}";

// verifies that basic file builds with successful return code
TEST(MCLangTests, HelloWorld_BuildSucceeds) { 
    BuildResult br = buildFromBody(HELLO_WORLD);
    EXPECT_EQ(br.code, 0);
    cleanupBuild(br);
}
// verifies that basic file builds without any output
TEST(MCLangTests, HelloWorld_NoOutput) { 
    BuildResult br = buildFromBody(HELLO_WORLD);
    EXPECT_EQ(br.output, "");
    cleanupBuild(br);
}
// verifies that basic file builds and creates a datapack folder
//  assumes default namespace
TEST(MCLangTests, HelloWorld_DatapackCreated) { 
    BuildResult br = buildFromBody(HELLO_WORLD);
    const std::string BASE_PATH = makeCompiledPath(br.filename);
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + DEF_NAMESPACE));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + DEF_NAMESPACE + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
    cleanupBuild(br);
}

// verifies that basic file builds with successful return code when provided -n arg
TEST(MCLangTests, HelloWorld_DashN_BuildSucceeds) { 
    BuildResult br = buildFromBody(HELLO_WORLD, TEST_NAMESPACE);
    EXPECT_EQ(br.code, 0);
    cleanupBuild(br);
}
// verifies that basic file builds without any output when provided -n arg
TEST(MCLangTests, HelloWorld_DashN_NoOutput) { 
    BuildResult br = buildFromBody(HELLO_WORLD, TEST_NAMESPACE);
    EXPECT_EQ(br.output, "");
    cleanupBuild(br);
}
// verifies that basic file builds and creates a datapack folder when provided -n arg
//  checks for proper path given namespace
TEST(MCLangTests, HelloWorld_DashN_DatapackCreated) { 
    BuildResult br = buildFromBody(HELLO_WORLD, TEST_NAMESPACE);
    const std::string BASE_PATH = makeCompiledPath(br.filename);
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
    cleanupBuild(br);
}

// verifies that basic file with namespace builds with successful return code
TEST(MCLangTests, HelloWorld_Namespaced_BuildSucceeds) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD);
    EXPECT_EQ(br.code, 0);
    cleanupBuild(br);
}
// verifies that basic file with namespace builds without any output
TEST(MCLangTests, HelloWorld_Namespaced_NoOutput) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD);
    EXPECT_EQ(br.output, "");
    cleanupBuild(br);
}
// verifies that basic file with namespace builds and creates a datapack folder
//  checks for proper path given namespace
TEST(MCLangTests, HelloWorld_Namespaced_DatapackCreated) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD);
    const std::string BASE_PATH = makeCompiledPath(br.filename);
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
    cleanupBuild(br);
}

// verifies that basic file with namespace builds with successful return code when also given -n arg
TEST(MCLangTests, HelloWorld_Namespaced_DashN_BuildSucceeds) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE);
    EXPECT_EQ(br.code, 0);
    cleanupBuild(br);
}
// verifies that basic file with namespace builds without any output when also given -n arg
TEST(MCLangTests, HelloWorld_Namespaced_DashN_NoOutput) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE);
    EXPECT_EQ(br.output, "");
    cleanupBuild(br);
}
// verifies that basic file with namespace builds and creates a datapack folder when also given -n arg
//  checks for proper path given namespace
TEST(MCLangTests, HelloWorld_Namespaced_DashN_DatapackCreated) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE);
    const std::string BASE_PATH = makeCompiledPath(br.filename);
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
    cleanupBuild(br);
}

// verifies that basic file with namespace builds with successful return code when also given -n arg
TEST(MCLangTests, HelloWorld_Namespaced_DashNConflicting_BuildSucceeds) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE + "2");
    EXPECT_EQ(br.code, 0);
    cleanupBuild(br);
}
// verifies that basic file with namespace builds without any output when also given -n arg
TEST(MCLangTests, HelloWorld_Namespaced_DashNConflicting_NoOutput) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE + "2");
    EXPECT_EQ(br.output, "");
    cleanupBuild(br);
}
// verifies that basic file with namespace builds and creates a datapack folder when also given -n arg
//  checks that namespace in the file trumps the command line arg namespace
TEST(MCLangTests, HelloWorld_Namespaced_DashNConflicting_DatapackCreated) { 
    BuildResult br = buildFromBody(NAMESPACER + HELLO_WORLD, TEST_NAMESPACE + "2");
    const std::string BASE_PATH = makeCompiledPath(br.filename);
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + TEST_NAMESPACE + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileCanOpen(BASE_PATH + "/pack.mcmeta"));
    cleanupBuild(br);
}

// verifies that attempting to build non-existent file will fail 
//   with error including "Could not open given file"
TEST(MCLangTests, InvalidFile_BuildFails) {
    // find path with no `.mcl` file
    std::string name = "f";
    if(fileCanOpen(makeSourcePath(name))) {
        name += "f";
    }
    BuildResult br = build(name);
    EXPECT_NE(br.code, 0);
    EXPECT_TRUE(inStr(br.output, "Could not open given file"));\
    // cleanup files
    cleanupBuild(br);
}