#include "SSDFileOutput.h"

SSDFileOutput::SSDFileOutput() {
    // file open with overwrite mode
    std::ofstream outfile(filename, std::ios::out);
    _openFile();
    _closeFile();
}

SSDFileOutput::~SSDFileOutput() {
    _closeFile();
}

void SSDFileOutput::printData(const unsigned int value) {
    _openFile();
    outfile << "0x" << std::uppercase << std::setfill('0') << std::setw(OUTPUT_DIGIT)
        << std::hex << value << std::endl;
    _closeFile();
}

void SSDFileOutput::printError(void) {
    _openFile();
    outfile << "ERROR" << std::endl;
    _closeFile();
}


void SSDFileOutput::printDone(void) {
    _openFile();
    outfile << "" << std::endl;
    _closeFile();
}

void SSDFileOutput::_openFile(void) {
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the SSDFileOutput text file");
    }
}

void SSDFileOutput::_closeFile(void) {
    if (outfile.is_open()) {
        outfile.close();
    }
}