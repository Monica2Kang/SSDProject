#include "SSDFileLogger.h"

SSDFileLogger::SSDFileLogger() {
    // file open with overwrite mode
    outfile.open(filename, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
}

SSDFileLogger::~SSDFileLogger() {
    if (outfile.is_open()) {
        outfile.close();
    }
}

void SSDFileLogger::logData(int value) {
    outfile << "0x" << std::uppercase << std::setfill('0') << std::setw(8)
        << std::hex << (value & 0x7FFFFFFF) << std::endl;
}

void SSDFileLogger::logError(void) {
    outfile << "ERROR" << std::endl;
}