#include <string>
#include <vector>
#include "SSDCmdParser.h"

using std::string;
using std::vector;

bool SSDCmdParser::checkParsing(int argc, const char* argv[]) {
    //make tokens
    std::vector<std::string> tokens;
    for (int i = 0; i < argc; ++i) {
        tokens.emplace_back(argv[i]);
    }

    //parse each tokens
    string command = tokens[1];
    int LBA = std::stoi(tokens[2]);

    if (command == "W") {
        string value = tokens[3];
        if (value.find("0x") == string::npos || value.length() != 10) {
            return PARSING_FAILED; 
        }

        int hexValue = std::stoul(value, nullptr, 16);
        m_device.writeData(LBA, hexValue);
        return PARSING_SUCCESS;
    }

    if (command == "R") {
        m_device.readData(LBA);
        return PARSING_SUCCESS;
    }

    return PARSING_FAILED;
}

