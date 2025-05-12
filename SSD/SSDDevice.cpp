#include "SSDDevice.h"

int SSDDevice::readData(int lba) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    return 0;
}

bool SSDDevice::isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

void SSDDevice::writeData(int LBA, int data) {
}
