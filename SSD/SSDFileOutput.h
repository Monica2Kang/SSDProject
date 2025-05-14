#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

// Singleton
#define SSD_FILE_OUTPUT SSDFileOutput::getInstance()

class SSDFileOutput {
public:
    static SSDFileOutput& getInstance(void) {
        static SSDFileOutput instance;
        return instance;
    }

public:

    void printData(const unsigned int value);
    void printError(void);
    void printDone(void);

private:
    SSDFileOutput();
    ~SSDFileOutput();
    SSDFileOutput(const SSDFileOutput&) = delete;
    SSDFileOutput& operator=(const SSDFileOutput&) = delete;

private:
    std::ofstream outfile;
    const std::string& filename = "ssd_output.txt";
    const int OUTPUT_DIGIT = 8;
};
