#pragma once
#include <stdexcept>

class SSDDevice {
public:
    SSDDevice();
    int readData(int LBA);
    void writeData(int LBA, int data);

private:
    bool isLbaOutOfRange(const int lba) const ;

private:
    static const int LBA_CAPACITY = 100;
    static const int LBA_UPPER_LIMIT = LBA_CAPACITY - 1;
    static const int LBA_LOWER_LIMIT = 0;
    int cellData[LBA_CAPACITY];
};

//class SSDDeviceException : public exception {
//public:
//    explicit SSDDeviceException(char const* _exception) : exception{ _exception } {}
//};
