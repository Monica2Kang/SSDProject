#include <string>
#include <vector>
#include <iostream>
#include "SSDCmdParser.h"
#include "SSDCmdBuffer.h"

using namespace std;
//#define m_device SSDDevice::getInstance()
#define m_device SSDCmdBuffer::getInstance()
#define m_fileOutput SSDFileOutput::getInstance()

bool SSDCmdParser::checkParsing(int argc, const char* argv[]) {
    //make tokens
    if (argc < VALID_COMMAND_SIZE) return PARSING_FAILED;

    std::vector<std::string> tokens;
    for (int i = 0; i < argc; ++i) {
        tokens.emplace_back(argv[i]);
    }
    //cout << argv[1] << " " << argv[2] << "\n";
    
    //parse each tokens
    string command = tokens[1];
    int lba = (tokens.size() > VALID_COMMAND_SIZE) ? std::stoi(tokens[2]) : 0;

    if (command == "W") {
        string value = (tokens.size() > VALID_COMMAND_SIZE) ? tokens[3] : "0";
        if (value.find("0x") == string::npos || value.length() != 10) {
            m_fileOutput.printError();
            return PARSING_FAILED; 
        }

        try {
            unsigned int hexValue = static_cast<unsigned int>(std::stoul(value, nullptr, 16));
            m_device.writeData(lba, hexValue);
            m_fileOutput.printDone();
            return PARSING_SUCCESS;
        }
        catch (invalid_argument& e){
            m_fileOutput.printError();
            return PARSING_FAILED;
        }
    }

    if (command == "R") {
        try {
            unsigned int outputData = m_device.readData(lba);
            m_fileOutput.printData(outputData);
            return PARSING_SUCCESS;
        }
        catch (invalid_argument& e){
            m_fileOutput.printError();
            return PARSING_FAILED;
        }
        catch (exception& e){
            m_fileOutput.printData(0x0);
            return PARSING_FAILED;
        }
    }

    if (command == "E") {
        int size = std::stoi(tokens[3]);
        if (size == 0) return PARSING_SUCCESS; //Do nothing

        if (size < MIN_ERASE_SIZE || size > MAX_ERASE_SIZE || lba + size > MAX_LBA) {
            m_fileOutput.printError();
            return PARSING_FAILED;
        }
        
        try {
            m_device.eraseData(lba, size);
            return PARSING_SUCCESS;
        }
        catch (invalid_argument& e) {
            m_fileOutput.printError();
            return PARSING_FAILED;
        }
    }
    
    if (command == "F") {
        m_device.flushData();
        return PARSING_SUCCESS;
    }
    return PARSING_FAILED;
}