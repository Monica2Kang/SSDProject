#include "SSDDevice.h"

SSDDevice::SSDDevice() {
    for (int lba = 0; lba < LBA_UPPER_LIMIT; lba++) {
        cellData[lba] = 0;
    }
}
int SSDDevice::readData(int lba) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    return cellData[lba];
}

void SSDDevice::writeData(int lba, const int data) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    cellData[lba] = data;
}

bool SSDDevice::isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

