#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "SSDFileStorageDevice.h"

using namespace std;

bool SSDFileStorageDevice::openFile(void) {
    if (_isFileOpened()) {
        return true;
    }
    if (!_openFile()) {
        _createFile();
    }
    return _openFile();
}

bool SSDFileStorageDevice::writeData(int lba, int data) {
    return true;
}

bool SSDFileStorageDevice::readData(int lba, int& data) {
    return true;
}

bool SSDFileStorageDevice::_openFile(void) {
    fileHandle.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    return fileHandle.is_open();
}

void SSDFileStorageDevice::_createFile(void) {
    std::ofstream create_file(filename, std::ios::binary);
    std::vector<int> cellData(maxLbaCapcity, 0);
    create_file.write(reinterpret_cast<const char*>(cellData.data()), cellData.size());
    create_file.close();
}

void SSDFileStorageDevice::closeFile(void) {
    if (fileOpened) {
        fileHandle.close();
        fileOpened = false;
    }
}