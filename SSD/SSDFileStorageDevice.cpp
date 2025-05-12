#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "SSDFileStorageDevice.h"

using namespace std;

bool SSDFileStorageDevice::writeData(int lba, int data) {
    return true;
}

bool SSDFileStorageDevice::readData(int lba, int& data) {
    return true;
}
