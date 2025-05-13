#pragma once
#include <stdexcept>
#include "SSDFileStorageDevice.h"

class SSDDevice {
public:
    SSDDevice();
    ~SSDDevice();
    int readData(const int lba);
    void writeData(const int lba, const int data);
    void reinitializeFile(void);
    void printError(void);

private:
    void initializeCellData(void);
    bool isLbaOutOfRange(const int lba) const;

private:
    static const int LBA_CAPACITY = 100;
    static const int LBA_UPPER_LIMIT = LBA_CAPACITY - 1;
    static const int LBA_LOWER_LIMIT = 0;
    const char* FILE_NAME = "ssd_nand.txt";
    int cellData[LBA_CAPACITY];
    SSDFileStorageDevice fSsd{ FILE_NAME, LBA_CAPACITY };
};
