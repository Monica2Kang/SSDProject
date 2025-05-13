#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class SSDFileOutput {

public:
    SSDFileOutput();
    ~SSDFileOutput();

    void printData(const unsigned int value);
    void printError(void);

private:
    std::ofstream outfile;
    const std::string& filename = "ssd_output.txt";
    const int OUTPUT_DIGIT = 8;
};
