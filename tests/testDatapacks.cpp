//Adam Callahan
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


//check if the file is present
bool fileExists(string fileName) {
    ifstream file;
    file.open ("./out_datapack/data/dp/functions/test_" + fileName + ".mcfunction");
    if (file.good()) {
        return true;
    }
    return false;
}

//Get string out of final
string searchFile(string fileName) {
    if (fileExists(fileName)) {
        ifstream file;
        file.open ("./out_datapack/data/dp/functions/test_" + fileName + ".mcfunction");
        string s = "";
        string line = "";
        while ( getline(file, line)) {
            s += line;
        }
        return s;
    }
    return "";
}

//Search to see if the character got escaped or not
bool testEscape(string fileName, string testChar) {
    string s = testChar;
    string test = searchFile(fileName); //take in the file as a string
    bool b = false;
    if (test.find(s) == -1) {   //if the string has a newline in it return false
        b = true;
    }
    return b;
}

bool mathTests(string fileName, int answer) {
    string x = "-1";
    if (fileExists(fileName)) {
        x = to_string(answer);
        string test = searchFile(fileName); //take in the file as a string
        if (test.find(x) != -1) {
            return true;
        }
    }
    return false;
}
