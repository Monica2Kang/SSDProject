#include "SSDDevice.h"

SSDDevice::SSDDevice() {
    initializeCellData();
}

int SSDDevice::readData(int lba) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    return cellData[lba];
}

void SSDDevice::writeData(int lba, const int data) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Ragne");
    cellData[lba] = data;
}

void SSDDevice::initializeCellData() {
    for (int lba = 0; lba < LBA_UPPER_LIMIT; lba++) {
        cellData[lba] = 0;
    }
}

bool SSDDevice::isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

