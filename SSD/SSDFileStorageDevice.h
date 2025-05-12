#pragma once
#include <fstream>

class SSDFileStorageDevice {
public:
    SSDFileStorageDevice() = default;
    SSDFileStorageDevice(std::string filename, int maxLbaCapacity) : 
        filename{ filename }, maxLbaCapacity{ maxLbaCapacity }, 
        fileOpened{ false }, upperLbaLimit{ maxLbaCapacity - 1 } {};

    bool openFile(void);
    void closeFile(void);

    bool writeData(const int lba, const int data);
    bool readData(const int lba, int &data);

private:
    bool _openFile(void);
    void _createFile(void);
    bool _isFileOpened(void) const { return fileOpened; }

private:
    std::string filename;
    std::fstream fileHandle;
    bool fileOpened;
    int maxLbaCapacity;
    int upperLbaLimit;
    int lowerLbaLimit = 0;
};