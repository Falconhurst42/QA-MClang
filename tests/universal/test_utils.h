#ifndef TEST_UTILS_H
#define TEST_UTILS_H

// test_utils.h
// Defines functions which perform multiple related GTests on a Datapack
// Ethan Worth (created), 10/19/2022
// Ethan Worth (updated), 10/26/2022
// Ethan Worth (updated), 11/8/2022
// Ethan Worth (updated), 11/12/2022
// Ethan Worth (updated), 11/16/2022
// Ethan Worth (updated), 12/7/2022
// Ethan Worth (updated), 12/12/2022

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../universal/Datapack.h"

// Performs a universal suite of tests corresponding to a successfully built datapack
//  - with successful return code
//  - without any output (unless warnings)
//  - datapack folders created
//  - function files created
//  - special function tag files created
// Arugments
//   src (Datapack)         - datapck in question
//   warnings (bool, false) - whether datapack compiled with warnings
void VERIFY_GOOD_BUILD(const Datapack& scr, bool warnings = false);

// Performs a universal suite of tests corresponding to an unsuccessfully built datapack
// Arugments
//   src (Datapack)        - datapck in question
//   warnings (string, "") - expected contents of error message
void VERIFY_BAD_BUILD(const Datapack& scr, const std::string& err = "");

// verifies that file has built properly
//  - with successful return code
//  - without any output (unless warnings)
//  - datapack folder created
void VERIFY_GOOD_BUILD(const Datapack& src, bool warnings) {
    // verify success code
    EXPECT_EQ(src.rez.code, 0);
    // verify no output
    if(!warnings) {
        EXPECT_EQ(src.rez.output, "");
    }

    // verify file structure
    const std::string BASE_PATH = src._makeCompiledPath(),
                      NAMES = src.getPrimaryNamespace();
    EXPECT_TRUE(directoryExists(BASE_PATH));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + NAMES));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/" + NAMES + "/functions"));
    EXPECT_TRUE(directoryExists(BASE_PATH + "/data/minecraft"));
    EXPECT_TRUE(fileExists(BASE_PATH + "/pack.mcmeta"));

    // verify function files are created
    const std::string FUNCT_PATH = src._makeFunctionsPath();
    const std::string TAG_PATH = src._makeTagsPath();
    for(const Datapack::Function& f : src.foos) {
        // functs with return-types don't get the same name
        if(f.rtype == "void") {
            EXPECT_TRUE(src._MCFunctionExists(f.getFormattedName()));
        }
        // if f.name is in TAGGED_FUNCT_NAMES...
        if(std::find(TAGGED_FUNCT_NAMES.cbegin(), TAGGED_FUNCT_NAMES.cend(), f.getFormattedName()) != TAGGED_FUNCT_NAMES.cend()) {
            // check for file
            EXPECT_TRUE(fileExists(TAG_PATH + "/" + f.getFormattedName() + ".json"));
            // check file contents
            EXPECT_EQ(getFileContents(TAG_PATH + "/" + f.getFormattedName() + ".json"), "{\"values\":[\"" + NAMES + ":" + f.getFormattedName() + "\"]}");
        }
    }

    // do inaccessible MC functions end up in compiled?

    // if !var.empty() || has_load --> check load
        // check for var decls

    // scan individual functions
    /*std::vector<std::string> foos = getDirectoryContents(FUNCT_PATH);
    std::cout << FUNCT_PATH << std::endl;
    for(std::string foo : foos) {
        std::cout << "   " << foo << std::endl;
    }*/
}

// verifies that file has built properly
//  - without successful return code
//  - with output containing `err`
//  - datapack folder not created
void VERIFY_BAD_BUILD(const Datapack& src, const std::string& err) {
    // verify success code
    EXPECT_NE(src.rez.code, 0);
    // verify err in output
    //std::cout << "\"" << src.rez.output << '"' << std::endl;
    if(err != "") {
        EXPECT_TRUE(inStr(src.rez.output, err));
    }

    // verify file structure
    const std::string BASE_PATH = src._makeCompiledPath();
    EXPECT_FALSE(directoryExists(BASE_PATH));
}

#endif