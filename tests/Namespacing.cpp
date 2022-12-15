// Commands
// Tests MCLangs handling of namespacing
// Ethan (created), 10/19/2022, 0.3.4-alpha
// Ethan (updated), 10/26/2022, 0.3.4-alpha
// Ethan (updated), 11/21/2022, 0.3.4-alpha

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/test_utils.h"
#include "universal/utils.h"
#include "TestSetup.h"

/***************************************|
|                                       |
|          Simple Namespacing           |
|                                       |
|***************************************/

// verifies that basic file builds with successful return code when provided -n arg
// Ethan, 10/19/2022, Should Pass
TEST(Namespacing, DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "Namespacing_DashN";
    src.build(TEST_NAMESPACE);
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code
// Ethan, 10/19/2022, Should Pass
TEST(Namespacing, Namespaced) { 
    Datapack src(HELLO_FILE);
    src.name = "Namespacing_Namespaced";
    src.space = TEST_NAMESPACE;
    src.build();
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a concurring -n arg
// Ethan, 10/19/2022, Should Pass
TEST(Namespacing, Namespaced_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "Namespacing_Namespaced_DashN";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE);
    VERIFY_GOOD_BUILD(src);

    // built files cleaned up by destructor when src falls out of scope
}

// verifies that basic file with namespace builds with successful return code when also given a conflicting -n arg
// Ethan, 10/19/2022, Should Pass
TEST(Namespacing, Conflicting_Namespaced_DashN) { 
    Datapack src(HELLO_FILE);
    src.name = "Namespacing_Conflicting_Namespaced_DashN";
    src.space = TEST_NAMESPACE;
    src.build(TEST_NAMESPACE.space + "2");
    VERIFY_GOOD_BUILD(src);
}