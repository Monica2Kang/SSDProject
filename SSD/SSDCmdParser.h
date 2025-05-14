#pragma once
#include <string>
#include <vector>
#include "SSDDevice.h"
#include "SSDFileOutput.h"
#include "SSDCmdBuffer.h"

using std::string;
using std::vector;

class SSDCmdParser {
public:
    SSDCmdParser(void) = default;
    bool checkParsing(int argc, const char* argv[]);

    static const bool PARSING_FAILED = false;
    static const bool PARSING_SUCCESS = true;
    static const int MAX_ERASE_SIZE = 10;
    static const int MIN_ERASE_SIZE = 0;

private:
    //SSDDevice m_device;
    //SSDFileOutput m_fileOutput;
    //SSDCmdBuffer m_device;

    static const int VALID_COMMAND_SIZE = 2;
    static const int MAX_LBA = 100;
};