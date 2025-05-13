#pragma once
#include <stdexcept>
#include "SSDFileStorageDevice.h"

class SSDDevice {
public:
    SSDDevice();
    ~SSDDevice();
    int readData(const int lba);
    void writeData(const int lba, const int data);
    void eraseData(const int lba, const int range);
    void reinitializeFile(void);
    void printError(void);

private:
    void _initializeCellData(void);
    bool _isLbaOutOfRange(const int lba) const;
    bool _isEraseRangeInvalid(const int range) const;

private:
    static const int LBA_CAPACITY = 100;
    static const int LBA_UPPER_LIMIT = LBA_CAPACITY - 1;
    static const int LBA_LOWER_LIMIT = 0;
    static const int MAX_ERASE_RANGE = 10;
    static const int MIN_ERASE_RANGE = 0;

    const char* FILE_NAME = "ssd_nand.txt";
    int cellData[LBA_CAPACITY];
    SSDFileStorageDevice fSsd{ FILE_NAME, LBA_CAPACITY };
};
