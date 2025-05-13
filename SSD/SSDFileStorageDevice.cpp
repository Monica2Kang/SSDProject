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

    _writeFile(lba, data);

    return true;
}

bool SSDFileStorageDevice::readData(const int lba, int& data) {
    if (_checkLbaBoundary(lba))
        return false;
    if (!_isFileOpened())
        return false;

    return _readFile(lba, data);
}

bool SSDFileStorageDevice::removeFile(void) {
    fileHandle.close();
    if (0 == std::remove(static_cast<const char*>(filename.c_str()))) {
        // std::cout << filename << " file is deleted." << std::endl;
    }
    fileHandle.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    return !fileHandle.is_open();
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
    std::vector<int> cellData(maxLbaCapacity + maxMapCapacity, 0);
    create_file.write(reinterpret_cast<const char*>(cellData.data()), cellData.size());
    //std::cout << filename << " file is created." << std::endl;
    create_file.close();
}

void SSDFileStorageDevice::_writeFile(const int lba, const int data) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    file.seekp(lba * sizeof(int));
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));
    file.seekp(lba * sizeof(int) + maxMapCapacity * sizeof(int));
    file.write(reinterpret_cast<const char*>(&TOUCHED_FLAG), sizeof(data));
    file.flush();
}

bool SSDFileStorageDevice::_readFile(const int lba, int& data) {
    fstream file(filename, ios::in | ios::out | ios::binary);
    //ifstream file(filename, ios::binary);

    int flag = 0;
    file.seekg(lba * sizeof(int) + maxMapCapacity * sizeof(int));
    file.read(reinterpret_cast<char*>(&flag), sizeof(flag));
    if (TOUCHED_FLAG != flag)
        return false;

    file.seekg(lba * sizeof(int), ios::beg);
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    return true;
}

bool SSDFileStorageDevice::_checkLbaBoundary(int lba) const {
    return lba < lowerLbaLimit || upperLbaLimit < lba;
}