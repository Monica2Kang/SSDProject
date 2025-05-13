#pragma once
#include <string>
#include <vector>
#include "SSDDevice.h"

using std::string;
using std::vector;

class SSDCmdParser {
public:
    SSDCmdParser(void) = default;
    bool checkParsing(int argc, const char* argv[]);

    static const bool PARSING_FAILED = false;
    static const bool PARSING_SUCCESS = true;
    static const int MAX_SIZE = 10;
    static const int MIN_SIZE = 0;

private:
    SSDDevice m_device;
    static const int VALID_COMMAND_SIZE = 2;
    static const int MAX_LBA = 100;
};