#ifndef TEST_SETUP_H
#define TEST_SETUP_H

// TestSetup.h
// Header for the creation of useful objects to be used in testing
//  specifically, Datapacks and sub-components which many be reused by tests
// Ethan (created), 10/9/2022
// Ethan (updated), 11/12/2022
// Adam, Ethan (porting), 12/12/2022
// Adam, Ethan (porting), 12/15/2022

#include <gtest/gtest.h>
#include "universal/Datapack.h"
#include "universal/utils.h"
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


const Datapack::Function ADD = {
    "void",
    "test_addition",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 10;")
    + "\nconst int n2 = 10;"
    + "\nconst int sum = n1 + n2;"
    + "\n/tellraw @a The sum is {{sum}}"
}, SUB = {
    "void",
    "test_subtraction",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 30;")
    + "\nconst int n2 = 10;"
    + "\nconst int sum = n1 - n2;"
    + "\n/tellraw @a The difference is {{sum}}"
}, MULT = {
    "void",
    "test_multiply",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 5;")
    + "\nconst int n2 = 4;"
    + "\nconst int sum = n1 * n2;"
    + "\n/tellraw @a The product is {{sum}}"
}, DIV = {
    "void",
    "test_divide",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 100;")
    + "\nconst int n2 = 5;"
    + "\nconst int sum = n1 / n2;"
    + "\n/tellraw @a The quotient is {{sum}}"
}, MOD = {
    "void",
    "test_subtraction",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 20;")
    + "\nconst int n2 = 3;"
    + "\nconst int sum = n1 % n2;"
    + "\n/tellraw @a The modulus is {{sum}}"
}, PEMDAS = {
    "void",
    "test_order",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 5;")
    + "\nconst int n2 = 2;"
    + "\nconst int sum = n2 * (n1 * n2);"
    + "\n/tellraw @a The solution is {{sum}}"
}, PEMDAS2 = {
    "void",
    "test_order",
    std::vector<Datapack::Variable>(),
    std::string("const int n1 = 5;")
    + "\nconst int n2 = 2;"
    + "\nconst int sum = n2 + n1 * n2;"
    + "\n/tellraw @a The solution is {{sum}}"
}, TCOMP = {
    "void",
    "test_comptrue",
    std::vector<Datapack::Variable>(),
    std::string("const bool n1 = true;")
    + "\nconst bool n2 = true;"
    + "\nconst bool ans = n1 && n2;"
    + "\n/tellraw @a The comparison is {{ans}}"
}, FCOMP = {
    "void",
    "test_compfalse",
    std::vector<Datapack::Variable>(),
    std::string("const bool n1 = true;")
    + "\nconst bool n2 = false;"
    + "\nconst bool ans = n1 && n2;"
    + "\n/tellraw @a The comparison is {{ans}}"
}, ORCOMP = {
    "void",
    "test_compor",
    std::vector<Datapack::Variable>(),
    std::string("const bool n1 = true;")
    + "\nconst bool n2 = false;"
    + "\nconst bool ans = n1 || n2;"
    + "\n/tellraw @a The comparison is {{ans}}"
}, NOTCOMP = {
    "void",
    "test_compnot",
    std::vector<Datapack::Variable>(),
    std::string("const bool n1 = true;")
    + "\nconst bool ans = !n1;"
    + "\n/tellraw @a The comparison is {{ans}}"
}, COMMENT1 = {
    "void",
    "test_comment1",
    std::vector<Datapack::Variable>(),
    std::string("///say There should be no 10;")
    + "\n/tellraw @a Text = 20"
}, COMMENT2 = {
    "void",
    "test_comment2",
    std::vector<Datapack::Variable>(),
    std::string("/*")
    + "\n/say There should be no 10;"
    + "\n*/"
    + "\n/tellraw @a Text = 20"
}, ESCNEWLINE = {
    "void",
    "test_newline",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \n Newline!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCTAB = {
    "void",
    "test_tab",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \t Tab!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCALERT = {
    "void",
    "test_alert",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \aAlert!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCBACKSPACE = {
    "void",
    "test_backspace",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \bBackspace!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCNEWPAGE = {
    "void",
    "test_newpage",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \fNewpage!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCRETURN = {
    "void",
    "test_return",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \rReturn!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCVTAB = {
    "void",
    "test_vtab",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \vVertical Tab!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCBACKSLASH = {
    "void",
    "test_backslash",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \\ Backslash!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCSINGQUOTE = {
    "void",
    "test_singquote",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \' Single Quote!\";")
    + "\n/tellraw @a testing: {{TEST}}"
}, ESCDOUBQUOTE = {
    "void",
    "test_doubquote",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \\\" Double Quote!\";")
    + "\n/tellraw @a testing: {{TEST}}"
};

const Datapack::Function PARSER_FAILS = {
    "void",
    "test_doubquote",
    std::vector<Datapack::Variable>(),
    std::string("const str TEST = \"Testing \" Double Quote!\";")
    + "\n/tellraw @a testing: {{TEST}}"
};

const Datapack::Function SUB_ONE = {
    "void",
    "sub_one",
    std::vector<Datapack::Variable>(),
    std::string("   int x = 1;") + "\n" + "    x--;"
};

const Datapack SUB_FILE = {
    (std::vector<Datapack::Function>) { SUB_ONE }
};

const Datapack::Function ADD_ONE = {
    "void",
    "add_one",
    std::vector<Datapack::Variable>(),
    std::string("   int x = 1;") + "\n" + "   x++;"
};

const Datapack ADD_FILE = {
    (std::vector<Datapack::Function>) { ADD_ONE }
};

const Datapack::Function DIV_FOUR = {
    "void",
    "div_four",
    std::vector<Datapack::Variable>(),
    std::string("   int x = 16;") + "\n" + "    x = x / 4;"
};

const Datapack DIV_FILE = {
    (std::vector<Datapack::Function>) { DIV_FOUR }
};

const Datapack::Function MULT_FOUR = {
    "void",
    "MULT_FOUR",
    std::vector<Datapack::Variable>(),
    std::string("   int x = 4;") + "\n" + "    x = x * 3;"
};

const Datapack MULT_FILE = {
    (std::vector<Datapack::Function>) { MULT_FOUR }
};

const Datapack::Function SUMMON_OCE = {
    "void",
    "SUMMON_OCE",
    std::vector<Datapack::Variable>(),
    std::string("   for (int i = 0; i < 10; i++) {") + "\n" + "      /summon ocelot" + "\n" + "   }"
};

const Datapack SUMMON_FILE = {
    (std::vector<Datapack::Function>) { SUMMON_OCE }
};

#endif