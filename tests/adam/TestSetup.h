#ifndef TEST_SETUP_H
#define TEST_SETUP_H

#include <gtest/gtest.h>
#include "../universal/Datapack.h"
#include "../universal/utils.h"

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

#endif