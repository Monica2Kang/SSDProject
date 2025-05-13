#pragma once
#include <fstream>

class SSDFileStorageDevice {
public:
    SSDFileStorageDevice() = default;
    SSDFileStorageDevice(std::string filename, int maxLbaCapacity) : 
        filename{ filename }, maxLbaCapacity{ maxLbaCapacity }, maxMapCapacity{ maxLbaCapacity },
        fileOpened{ false }, upperLbaLimit{ maxLbaCapacity - 1 } {};

    bool openFile(void);
    void closeFile(void);

    bool writeData(const int lba, const int data);
    bool readData(const int lba, int &data);
    bool removeFile(void);

private:
    bool _openFile(void);
    void _createFile(void);
    bool _checkLbaBoundary(const int lba) const;
    bool _isFileOpened(void) const { return fileOpened; }
    void _setFileOpened(const bool input) { fileOpened = input; }
    bool _readFile(const int lba, int& data);
    void _writeFile(const int lba, const int data);

private:
    std::string filename;
    std::fstream fileHandle;
    bool fileOpened;
    int maxLbaCapacity;
    int maxMapCapacity;
    int upperLbaLimit;
    int lowerLbaLimit = 0;
    static const int TOUCHED_FLAG = 1;
};