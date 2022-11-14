#include <gtest/gtest.h>
#include "BuildBasics.cpp"
#include "Functions.cpp"
#include "Namespacing.cpp"
#include "PythonCleanup.cpp"
#include "PythonSetup.h"

int main(int argc, char **argv) {
    pythonStart();
    testing::InitGoogleTest(&argc, argv);
    int rez = RUN_ALL_TESTS();
    pythonCleanup();
    return rez;
}