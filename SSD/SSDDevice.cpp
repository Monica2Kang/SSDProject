#include "SSDDevice.h"

int SSDDevice::readData(int lba) {
    if (LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT) throw std::invalid_argument("Out of LBA Range");
    return 0;
}

void SSDDevice::writeData(int LBA, int data) {
}
