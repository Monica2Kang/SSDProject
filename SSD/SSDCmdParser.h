#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class SSDCmdParser {
public:
    string checkParsing(int argc, const char* argv[]) const;
};