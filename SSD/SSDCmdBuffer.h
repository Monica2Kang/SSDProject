#pragma once
#include <iostream>
#include <fstream>
#include <string>

class SSDCmdBuffer {
public:
    SSDCmdBuffer();
    ~SSDCmdBuffer();
    
private:
    std::ofstream outfile;
    const std::string& folderName = "buffer";
    const std::string& fileEmptyName = "empty.txt";
};