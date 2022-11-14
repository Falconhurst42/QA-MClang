#include <gtest/gtest.h>
#include "BuildBasics.cpp"
#include "Functions.cpp"
#include "Namespacing.cpp"
#include "PythonCleanup.cpp"
//    <pack_name,  pack_files>
// map<std::string, PackFiles> python_packs // outstanding pacakfiles which are being processed by python


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}