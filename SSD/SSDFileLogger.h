#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class SSDFileLogger {

public:
    SSDFileLogger();
    ~SSDFileLogger();

    void logData(const int value);
    void logError(void);

private:
    std::ofstream outfile;
    const std::string& filename = "ssd_output.txt";
};
