#ifndef TEST_SETUP_H
#define TEST_SETUP_H

#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/utils.h"
#include <fstream>

const Datapack::Variable N_VAR = {"int", "n"},
                         I_VAR = {"int", "i"};

const Datapack::Function HELLO_WORLD = {
    "void",
    "hello_world",
    std::vector<Datapack::Variable>(),
    "    /say Hello world!"
}, LOAD_N_I = {
    "void",
    "load",
    std::vector<Datapack::Variable>(),
    std::string("    n = 0;") + "\n" +
    "    i = 0;"
}, TICK_N_I = {
    "void",
    "tick",
    std::vector<Datapack::Variable>(),
    std::string("    n = n - 1;") + "\n" +
    "    i = i + 1;"
};

const Datapack::Namespace TEST_NAMESPACE("gtest");

const Datapack HELLO_FILE = {
    (std::vector<Datapack::Function>){ HELLO_WORLD }
}, NI_TICK_LOAD_FILE = {
    (std::vector<Datapack::Function>){ LOAD_N_I, TICK_N_I },
    (std::vector<Datapack::Variable>){ N_VAR, I_VAR }
};

#endif