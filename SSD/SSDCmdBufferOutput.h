#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

class SSDCmdBufferOutput {
public:
    SSDCmdBufferOutput();
    ~SSDCmdBufferOutput();
    void createFilesInFolder(std::vector<std::string>& files);
    std::vector<std::string> getFileListForDebug(void);

    const std::string& fileEmptyName = "empty";
    const int MAX_FILE_COUNT = 5;
    
private:
    bool _folderExists(void);
    void _createFolder(void);
    void _clearFilesInFolder(void);

    void _storeFileNameForDebug(std::string fileName);
    void _clearFileNameForDebug(void);

    const char* folderName = "buffer";
    std::vector<std::string> m_fileListForDebug;
};