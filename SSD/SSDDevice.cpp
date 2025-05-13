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

int SSDDevice::readData(const int lba) {
    SSDFileOutput fOutput;
    if (_isLbaOutOfRange(lba)) {
        fOutput.printError();
        throw std::invalid_argument("Out of LBA Range.");
    }
    int data = 0;
    if (false == fSsd.readData(lba, data)) {
        fOutput.printData(0x0);
        throw std::exception("Untouched Data.");
    }

    fOutput.printData(static_cast<unsigned int>(data));
    return data;
}

void SSDDevice::printError(void) {
    SSDFileOutput fLog;
    fLog.printError();
}

void SSDDevice::writeData(const int lba, const int data) {
    SSDFileOutput fOutput;
    if (_isLbaOutOfRange(lba)) {
        fOutput.printError();
        throw std::invalid_argument("Out of LBA Range.");
    }
    fSsd.writeData(lba, data);
    cellData[lba] = data;
}

void SSDDevice::eraseData(const int lba, const int range) {
    SSDFileOutput fOutput;
    if (_isLbaOutOfRange(lba)) {
        fOutput.printError();
        throw std::invalid_argument("Out of LBA Range.");
    }
    if (_isEraseRangeInvalid(range)) {
        fOutput.printError();
        throw std::invalid_argument("Out of e Range.");
    }

    for (int curLba = lba; curLba < lba + range; curLba++)
        fSsd.writeData(curLba, 0x00);
}

void SSDDevice::reinitializeFile(void) {
    fSsd.removeFile();
    fSsd.openFile();
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
