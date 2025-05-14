#pragma once
#include <fstream>

// Singleton
#define SSD_FILE_STORAGE_DEVICE SSDFileStorageDevice::getInstance()

class SSDFileStorageDevice {
public:
    static SSDFileStorageDevice& getInstance(void) {
        static const int LBA_CAPACITY = 100;
        const char* FILE_NAME = "ssd_nand.txt";
        
        static SSDFileStorageDevice instance{ FILE_NAME, LBA_CAPACITY };
        return instance;
    }

public:
    //bool openFile(void);
    //void closeFile(void);

    bool writeData(const int lba, const unsigned int data);
    bool readData(const int lba, unsigned int &data);
    bool removeFile(void);
    void createFile(void);

private:
    //bool _openFile(void);
    bool _checkLbaBoundary(const int lba) const;
    //bool _isFileOpened(void) const { return fileOpened; }
    //void _setFileOpened(const bool input) { fileOpened = input; }
    bool _readFile(const int lba, unsigned int& data);
    void _writeFile(const int lba, const int data);

private:
    SSDFileStorageDevice() = default;
    SSDFileStorageDevice(std::string filename, int maxLbaCapacity) : 
        filename{ filename }, maxLbaCapacity{ maxLbaCapacity }, maxMapCapacity{ maxLbaCapacity },
        upperLbaLimit{ maxLbaCapacity - 1 } {
        createFile();
    };

private:
    std::string filename;
    //std::fstream fileHandle;
    //bool fileOpened;
    int maxLbaCapacity;
    int maxMapCapacity;
    int upperLbaLimit;
    int lowerLbaLimit = 0;
    static const int TOUCHED_FLAG = 1;
};