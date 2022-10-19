#include <gtest/gtest.h>
#include "BuildBasics.cpp"
#include "Functions.cpp"
#include "Namespacing.cpp"
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}