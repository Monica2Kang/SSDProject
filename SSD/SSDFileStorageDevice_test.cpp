#include <iostream>
#include <fstream>
#include "gmock/gmock.h"

#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

class SSDFileStorageDeviceFixture : public Test {
public:
    const char* FILE_NAME = "ssd_nand.txt";
    const int FILE_STORAGE_CAPACITY = 100;
    SSDFileStorageDevice fSsd = { FILE_NAME, FILE_STORAGE_CAPACITY };
public:
    void removeSSDFile(void) {
        std::ifstream ssdFile;
        std::remove(FILE_NAME);
        ssdFile.open(FILE_NAME, std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_FALSE(fileOpened);
    }
    void createSSDFile(void) {
        SSDFileStorageDevice ssdFile = { FILE_NAME, 100 };
        if (ssdFile.openFile())
            std::cout << FILE_NAME << " file created." << std::endl;
        else
            std::cout << FILE_NAME << " file is not created." << std::endl;
        ssdFile.closeFile();
    }
    void openSSDFile(void) {
        std::ifstream ssdFile;
        ssdFile.open(FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_TRUE(fileOpened);
    }
};

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageDummyInstanceCreationTC) {
    SSDFileStorageDevice fSsdDummy;
    EXPECT_NE(&fSsdDummy, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC) {
    EXPECT_NE(&fSsd, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTCwithActualFile) {
    removeSSDFile();
    createSSDFile();
    openSSDFile();
    removeSSDFile();

}