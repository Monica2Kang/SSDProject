#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

class SSDCmdBufferOutput {
public:
    SSDCmdBufferOutput();
    ~SSDCmdBufferOutput();
    
private:
    std::ofstream outfile;
    const char* folderName = "buffer";
    const std::string& fileEmptyName = "empty.txt";
    const int MAX_FILE_COUNT = 5;
};