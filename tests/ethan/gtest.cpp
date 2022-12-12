#include <gtest/gtest.h>
#include "PythonSetup.h"
#include "Arithmetic.cpp"
#include "BuildBasics.cpp"
#include "Commands.cpp"
#include "Escape.cpp"
#include "Functions.cpp"
#include "Namespacing.cpp"
#include "Preprocessor.cpp"

// python cleanup must remain last suite
// #include "PythonCleanup.cpp"

int main(int argc, char **argv) {
    pythonStart();
    testing::InitGoogleTest(&argc, argv);
    int rez = RUN_ALL_TESTS();
    pythonCleanup();

    return rez;
}