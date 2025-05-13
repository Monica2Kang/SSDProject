#include "SSDFileStorageDevice.h"
#include "SSDDevice.h"
#include "SSDFileOutput.h"

SSDDevice::SSDDevice() {
    initializeCellData();
    fSsd.openFile();
}

SSDDevice::~SSDDevice() {
    fSsd.closeFile();
}

int SSDDevice::readData(const int lba) {
    SSDFileOutput fLog;

    if (isLbaOutOfRange(lba)) {
        fLog.logError();
        throw std::invalid_argument("Out of LBA Range.");
    }
    int data = 0;
    if (false == fSsd.readData(lba, data)) {
        fLog.logData(0x0);
        throw std::exception("Untouched Data.");
    }
    fLog.logData(static_cast<unsigned int>(data));

    return data;
}

void SSDDevice::printError(void) {
    SSDFileOutput fLog;
    fLog.logError();
}

void SSDDevice::writeData(const int lba, const int data) {
    if (isLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Range.");
    fSsd.writeData(lba, data);
    cellData[lba] = data;
}

void SSDDevice::reinitializeFile(void) {
    fSsd.removeFile();
    fSsd.openFile();
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
