#include "SSDFileStorageDevice.h"
#include "SSDDevice.h"
#include "SSDFileOutput.h"

SSDDevice::SSDDevice() {
    _initializeCellData();
    fSsd.openFile();
}

SSDDevice::~SSDDevice() {
    fSsd.closeFile();
}

unsigned int SSDDevice::readData(const int lba) {
    if (_isLbaOutOfRange(lba)) {
        throw std::invalid_argument("Out of LBA Range.");
    }
    unsigned int data = 0;
    if (false == fSsd.readData(lba, data)) {
        throw std::exception("Untouched Data.");
    }
    return data;
}

void SSDDevice::printError(void) {
    SSD_FILE_OUTPUT.printError();
}

void SSDDevice::writeData(const int lba, const unsigned int data) {
    if (_isLbaOutOfRange(lba)) {
        throw std::invalid_argument("Out of LBA Range.");
    }
    fSsd.writeData(lba, data);
    cellData[lba] = data;
}

void SSDDevice::eraseData(const int lba, const int range) {
    if (_isLbaOutOfRange(lba)) {
        throw std::invalid_argument("Out of LBA Range.");
    }
    if (_isEraseRangeInvalid(range)) {
        throw std::invalid_argument("Out of e Range.");
    }

    for (int curLba = lba; curLba < lba + range; curLba++)
        fSsd.writeData(curLba, 0x00);
}

void SSDDevice::reinitializeFile(void) {
    fSsd.removeFile();
    fSsd.createFile();
    //fSsd.openFile();
}

void SSDDevice::_initializeCellData(void) {
    for (int lba = 0; lba < LBA_UPPER_LIMIT; lba++) {
        cellData[lba] = 0;
        fSsd.writeData(lba, 0);
    }
}

bool SSDDevice::_isLbaOutOfRange(const int lba) const {
    return LBA_UPPER_LIMIT < lba || lba < LBA_LOWER_LIMIT;
}

bool SSDDevice::_isEraseRangeInvalid(const int range) const {
    return MAX_ERASE_RANGE < range || range < MIN_ERASE_RANGE;
}