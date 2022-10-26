#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/test_utils.h"
#include "../universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|          Simple Namespacing           |
|                                       |
|***************************************/

// verifies that basic file builds with successful return code when provided -n arg
TEST(Namespacing, DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_DashN";
    src.build(TEST_NAMESPACE);
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code
TEST(Namespacing, Namespaced) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced";
    src.space = TEST_NAMESPACE;
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a concurring -n arg
TEST(Namespacing, Namespaced_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced_DashN";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE);
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a conflicting -n arg
TEST(Namespacing, Conflicting_Namespaced_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "VerifyBuild_HelloWorld_Namespaced_DashNConflicting";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE.space + "2");
    VERIFY_GOOD_BUILD(src);
}