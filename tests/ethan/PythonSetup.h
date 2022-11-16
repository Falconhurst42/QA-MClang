#ifndef PYTHON_SETUP_H
#define PYTHON_SETUP_H

#include "../universal/Datapack.h"
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
}

void pythonCleanup() {
    // EXPECT_TRUE(python_packs.empty());
    std::ofstream out(makePythonPath("TESTS.DONE"));
    out.close();
    th.join();
}

#endif