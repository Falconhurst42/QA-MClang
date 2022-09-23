
#include "strmanip.h"
#include <map>
#include <string>

bool isAlphaNumUS(char c) {
    return c == '_' || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
    || ('0' <= c && c <= '9');
}

std::string thousandsSep(unsigned int num, std::string sep) {
    std::string out = "";
    unsigned int index = 0;
    while (num > 0) {
        if (index % 3 == 0 && index > 0)
            out = sep + out;
        out = std::to_string(num % 10) + out;
        num /= 10, index++;
    }
    if (out == "")
        out = "0";
    return out;
}

std::string replaceInsertedValues(std::string str, std::map<std::string,
std::string> constValues) {
    ReplaceConstHelper helper(str, constValues);
    return helper.process();
}

ReplaceConstHelper::ReplaceConstHelper(std::string str, std::map<std::string,
std::string> constValues) : str(str), constValues(constValues) {

}

ReplaceConstHelper::~ReplaceConstHelper() {

}

std::string ReplaceConstHelper::process() {
    index = 0, out = "";
    while (!atEnd()) {
        if (cur() == '{')
            readBracket();
        else
            out.push_back(cur()), next();
    }
    return out;
}

char ReplaceConstHelper::cur() const {
    return str[index];
}

void ReplaceConstHelper::next() {
    index++;
}

bool ReplaceConstHelper::atEnd() const {
    return index >= str.size();
}

void ReplaceConstHelper::readBracket() {
    next();
    if (!atEnd() && cur() == '{')
        readSecondBracket();
    else
        out.push_back('{');
}

void ReplaceConstHelper::readSecondBracket() {
    next();
    std::string name = "";
    while (!atEnd() && isAlphaNumUS(cur()))
        name.push_back(cur()), next();
    if (atEnd() || cur() != '}') {
        out.append("{{" + name);
        return;
    }
    next();
    if (atEnd() || cur() != '}') {
        out.append("{{" + name + "}");
        return;
    }
    next();
    if (constValues.count(name) > 0)
        out.append(constValues.find(name)->second);
    else
        out.append("{{" + name + "}}");
}