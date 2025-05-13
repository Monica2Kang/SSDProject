#include "SSDFileOutput.h"

SSDFileOutput::SSDFileOutput() {
    // file open with overwrite mode
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
}

SSDFileOutput::~SSDFileOutput() {
    if (outfile.is_open()) {
        outfile.close();
    }
}

void SSDFileOutput::logData(const unsigned int value) {
    outfile << "0x" << std::uppercase << std::setfill('0') << std::setw(OUTPUT_DIGIT)
        << std::hex << value << std::endl;
}

void SSDFileOutput::logError(void) {
    outfile << "ERROR" << std::endl;
}