#include "SSDFileOutput.h"

SSDFileOutput::SSDFileOutput() {
    // file open with overwrite mode
    std::ofstream outfile(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
    if (outfile.is_open()) {
        outfile.close();
    }
}

SSDFileOutput::~SSDFileOutput() {
    if (outfile.is_open()) {
        outfile.close();
    }
}

void SSDFileOutput::printData(const unsigned int value) {
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
    outfile << "0x" << std::uppercase << std::setfill('0') << std::setw(OUTPUT_DIGIT)
        << std::hex << value << std::endl;
    if (outfile.is_open()) {
        outfile.close();
    }
}

void SSDFileOutput::printError(void) {
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
    outfile << "ERROR" << std::endl;
    if (outfile.is_open()) {
        outfile.close();
    }
}

void SSDFileOutput::printDone(void) {
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
    outfile << "" << std::endl;
    if (outfile.is_open()) {
        outfile.close();
    }
}
