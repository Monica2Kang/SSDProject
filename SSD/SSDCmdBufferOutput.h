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
    bool _folderExists(void);
    void _createFolder(void);
    void _clearFilesInFolder(void);
    void _createFilesInFolder(void);

    std::ofstream outfile;
    const char* folderName = "buffer";
    const std::string& fileEmptyName = "empty.txt";
    const int MAX_FILE_COUNT = 5;
};