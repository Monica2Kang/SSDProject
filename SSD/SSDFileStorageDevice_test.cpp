#include <iostream>
#include <fstream>
#include "gmock/gmock.h"

#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

class SSDFileStorageDeviceFixture : public Test {
public:
    const char* FILE_NAME = "ssd_nand.txt";
    const char* FILE_NAME_TEMP = "ssd_nand_temp.txt";
    const int FILE_STORAGE_CAPACITY = 100;
    SSDFileStorageDevice fSsd = { FILE_NAME, FILE_STORAGE_CAPACITY };
public:
    void removeSSDFile(const char* filename) {
        std::ifstream ssdFile;
        std::remove(filename);
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_FALSE(fileOpened);
    }
    void createSSDFile(const char* filename) {
        SSDFileStorageDevice ssdFile = { filename, FILE_STORAGE_CAPACITY };
        if (ssdFile.openFile())
            std::cout << FILE_NAME << " file created." << std::endl;
        else
            std::cout << FILE_NAME << " file is not created." << std::endl;
        ssdFile.closeFile();
    }
    void openSSDFile(const char* filename) {
        std::ifstream ssdFile;
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
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

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC_ssd_nand_txt) {
    removeSSDFile(FILE_NAME_TEMP);
    createSSDFile(FILE_NAME_TEMP);
    openSSDFile(FILE_NAME_TEMP);
    removeSSDFile(FILE_NAME_TEMP);
}