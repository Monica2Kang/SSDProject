#pragma once
#include <string>
#include <vector>
#include "SSDDevice.h"

using std::string;
using std::vector;

class SSDCmdParser {
public:
    SSDCmdParser(SSDDevice& device)
        : m_device(device) {
    }

    bool checkParsing(int argc, const char* argv[]);

private:
    SSDDevice& m_device;
    static const bool PARSING_FAILED = false;
    static const bool PARSING_SUCCESS = true;
};