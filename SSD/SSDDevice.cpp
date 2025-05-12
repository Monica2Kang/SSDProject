#include "SSDDevice.h"

int SSDDevice::readData(int lba) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    return 0;
}

void SSDDevice::writeData(int lba, const int data) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
}

bool SSDDevice::isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

