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

bool SSDFileStorageDevice::writeData(const int lba, const int data) {
    if (_checkLbaBoundary(lba))
        return false;
    if (!_isFileOpened())
        return false;
    return true;
}


bool SSDFileStorageDevice::readData(const int lba, int& data) {
    if (_checkLbaBoundary(lba))
        return false;
    if (!_isFileOpened())
        return false;
    return true;
}

void SSDFileStorageDevice::closeFile(void) {
    if (fileOpened) {
        fileHandle.close();
        _setFileOpened(false);
    }
}

bool SSDFileStorageDevice::_openFile(void) {
    fileHandle.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (fileHandle.is_open()) {
        _setFileOpened(true);
    }
    else {
        _setFileOpened(false);
    }
    return _isFileOpened();
}

void SSDFileStorageDevice::_createFile(void) {
    std::ofstream create_file(filename, std::ios::binary);
    std::vector<int> cellData(maxLbaCapacity, 0);
    create_file.write(reinterpret_cast<const char*>(cellData.data()), cellData.size());
    create_file.close();
}

bool SSDFileStorageDevice::_checkLbaBoundary(int lba) const {
    return lba < lowerLbaLimit || upperLbaLimit < lba;
}