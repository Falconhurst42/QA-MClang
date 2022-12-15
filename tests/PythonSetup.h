#ifndef PYTHON_SETUP_H
#define PYTHON_SETUP_H

// PythonSetup (placeholder)
// Header to assist python integration
//    defunct until python integration finished
// Ethan (created), 11/13/2022, 0.3.4-alpha
// Ethan (updated), 11/14/2022, 0.3.4-alpha
// Ethan (updated), 11/21/2022, 0.3.4-alpha
// Ethan (updated), 11/29/2022, 0.3.4-alpha

#include "universal/Datapack.h"
#include <fstream>
#include <thread>
#include <map>
#include <string>

void pythonStart();
void pythonCleanup();

// names of outstanding packs which are being processed by python
// <test_name, pack_name>
std::map<std::string, Datapack> python_packs;
// thread which runs python
std::thread th;

void pythonStart() {
    th = std::thread(system, "python3 standin.py");
    // std::thread temp = std::thread(system, "python.exe C:/MCLang/standin.py");
    // temp.join();
}

void pythonCleanup() {
    // EXPECT_TRUE(python_packs.empty());
    std::ofstream out(makePythonPath("TESTS.DONE"));
    out.close();
    th.join();
}

#endif