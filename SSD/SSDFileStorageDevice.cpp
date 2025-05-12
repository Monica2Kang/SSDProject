#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "SSDFileStorageDevice.h"

using namespace std;

bool SSDFileStorageDevice::openFile(void) {
    return true;
}

void SSDFileStorageDevice::closeFile(void) {

}

bool SSDFileStorageDevice::writeData(const int lba, const int data) {
    return true;
}

bool SSDFileStorageDevice::readData(const int lba, int& data) {
    return true;
}
