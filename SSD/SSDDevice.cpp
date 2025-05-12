#include "SSDDevice.h"

int SSDDevice::readData(int lba) {
    if (lba >= MAX_LBA) throw std::invalid_argument("Out of LBA Range");
    return 0;
}

void SSDDevice::writeData(int LBA, int data) {
}
