#include "SSDFileStorageDevice.h"
#include "SSDDevice.h"

SSDDevice::SSDDevice() {
    initializeCellData();
    fSsd.openFile();
}

SSDDevice::~SSDDevice() {
    initializeCellData();
    fSsd.closeFile();
}

int SSDDevice::readData(const int lba) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Range");
    int data = 0;
    fSsd.readData(lba, data);
    return data;
    //return cellData[lba];
}

void SSDDevice::writeData(const int lba, const int data) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Range");
    fSsd.writeData(lba, data);
    cellData[lba] = data;
}

void SSDDevice::initializeCellData(void) {
    for (int lba = 0; lba < LBA_UPPER_LIMIT; lba++) {
        cellData[lba] = 0;
        fSsd.writeData(lba, 0);
    }
}

bool SSDDevice::isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

