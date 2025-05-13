#include <string>
#include <vector>
#include "SSDCmdParser.h"

using std::string;
using std::vector;

bool SSDCmdParser::checkParsing(int argc, const char* argv[]) {
    //make tokens
    if (argc < VALID_COMMAND_SIZE) return PARSING_FAILED;

    std::vector<std::string> tokens;
    for (int i = 0; i < argc; ++i) {
        tokens.emplace_back(argv[i]);
    }

    //parse each tokens
    string command = tokens[1];
    int lba = (tokens.size() > VALID_COMMAND_SIZE) ? std::stoi(tokens[2]) : 0;

    if (command == "W") {
        string value = tokens[3];
        if (value.find("0x") == string::npos || value.length() != 10) {
            m_device.printError();
            return PARSING_FAILED; 
        }

        int hexValue = static_cast<int>(std::stoul(value, nullptr, 16));
        m_device.writeData(lba, hexValue);
        return PARSING_SUCCESS;
    }

    if (command == "R") {
        m_device.readData(lba);
        return PARSING_SUCCESS;
    }

    if (command == "E") {
        int size = std::stoi(tokens[3]);
        if (size == 0) return PARSING_SUCCESS; //Do nothing

        if (size < MIN_SIZE || size > MAX_SIZE || lba + size > MAX_LBA) {
            m_device.printError();
            return PARSING_FAILED;
        }
        
        m_device.eraseData(lba, size);
        return PARSING_SUCCESS;
    }
    
    if (command == "F") {
        //m_device.flushData();
        return PARSING_SUCCESS;
    }
    return PARSING_FAILED;
}