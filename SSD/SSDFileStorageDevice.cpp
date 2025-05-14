#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "SSDFileStorageDevice.h"

using namespace std;

SSDFileStorageDevice::SSDFileStorageDevice(std::string filename, int lbaCapacity)
    : filename(std::move(filename)), maxLbaCapacity(lbaCapacity), maxMapCapacity(lbaCapacity),
    upperLbaLimit(lbaCapacity - 1) {

    if (!_fileExists(this->filename)) {
        createFile();
    }
}


bool SSDFileStorageDevice::_fileExists(const std::string& name) {
    std::ifstream f(name, std::ios::binary);
    bool exists = f.good();
    return exists;
}

bool SSDFileStorageDevice::writeData(const int lba, const unsigned int data) {
    if (_checkLbaBoundary(lba))
        return false;
    try {
        _writeFile(lba, data);
        return true;
    }
    catch (runtime_error& e) {
        throw std::exception(e);
    }
}

bool SSDFileStorageDevice::readData(const int lba, unsigned int& data) {
    if (_checkLbaBoundary(lba))
        return false;
    try {
        return _readFile(lba, data);
    }
    catch (runtime_error& e) {
        throw std::exception(e);
    }
}

bool SSDFileStorageDevice::removeFile(void) {
    if (0 == std::remove(static_cast<const char*>(filename.c_str()))) {
        // std::cout << filename << " file is deleted." << std::endl;
    }
    fstream file;
    file.open(filename, ios::in | ios::out | ios::binary);
    bool result = !file.is_open();
    file.close();
    return result;
}

void SSDFileStorageDevice::createFile(void) {
    std::ofstream create_file(filename, std::ios::binary);
    if (!create_file.is_open()) {
        std::cerr << "Cannot Open the file" << filename << std::endl;
        return;
    }

    std::vector<int> cellData(maxLbaCapacity + maxMapCapacity, 0);
    create_file.write(reinterpret_cast<const char*>(cellData.data()), cellData.size() * sizeof(int));
    create_file.flush();
    create_file.close();
}

void SSDFileStorageDevice::_writeFile(const int lba, const int data) {
    fstream file(filename, ios::in | ios::out | ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot Open the File.");
    }
    file.seekp(lba * sizeof(int), ios::beg);
    file.write(reinterpret_cast<const char*>(&data), sizeof(data));
    file.seekp(lba * sizeof(int) + maxMapCapacity * sizeof(int), ios::beg);
    file.write(reinterpret_cast<const char*>(&TOUCHED_FLAG), sizeof(TOUCHED_FLAG));
    file.flush();
    file.close();
}

bool SSDFileStorageDevice::_readFile(const int lba, unsigned int& data) {
    //fstream file(filename, ios::in | ios::out | ios::binary);
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot Open the File.");
        //return false;
    }
        int flag = 0;
    file.seekg(lba * sizeof(int) + maxMapCapacity * sizeof(int), ios::beg);
    file.read(reinterpret_cast<char*>(&flag), sizeof(flag));
    if (TOUCHED_FLAG != flag) {
        file.close();
        return false;
    }

    file.seekg(lba * sizeof(int), ios::beg);
    file.read(reinterpret_cast<char*>(&data), sizeof(data));
    file.close();
    return true;
}

bool SSDFileStorageDevice::_checkLbaBoundary(int lba) const {
    return lba < lowerLbaLimit || upperLbaLimit < lba;
}