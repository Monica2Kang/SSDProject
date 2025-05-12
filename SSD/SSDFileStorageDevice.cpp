#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "SSDFileStorageDevice.h"

using namespace std;

bool SSDFileStorageDevice::openFile(void) {
    if (fileOpened) {
        return true;
    }

    std::ifstream check_file(filename, std::ios::binary);
    if (!check_file) {
        std::ofstream create_file(filename, std::ios::binary);
        std::vector<int> zeros(maxLbaCapcity, 0);
        create_file.write(reinterpret_cast<const char*>(zeros.data()), zeros.size());
        create_file.close();
    }

    fileHandle.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    fileOpened = fileHandle.is_open();
    return fileOpened;
}

void SSDFileStorageDevice::closeFile(void) {
    if (fileOpened) {
        fileHandle.close();
        fileOpened = false;
    }
}

bool SSDFileStorageDevice::writeData(const int lba, const int data) {
    return true;
}

bool SSDFileStorageDevice::readData(const int lba, int& data) {
    return true;
}
