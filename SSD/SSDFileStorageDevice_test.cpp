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
        if (0 == std::remove(filename)) {
            std::cout << filename << " file is deleted." << std::endl;
        }
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_FALSE(fileOpened);
    }
    void createSSDFile(const char* filename) {
        SSDFileStorageDevice ssdFile = { filename, FILE_STORAGE_CAPACITY };
        if (ssdFile.openFile())
            std::cout << filename << " file is created." << std::endl;
        else
            std::cout << filename << " file is not created." << std::endl;
        ssdFile.closeFile();
    }
    void openSSDFile(const char* filename) {
        std::ifstream ssdFile;
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_TRUE(fileOpened);
    }

protected:
    struct LBA_DATA {
        int lba;
        int data;
    };
};

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageDummyInstanceCreationTC) {
    SSDFileStorageDevice fSsdDummy;
    EXPECT_NE(&fSsdDummy, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC) {
    EXPECT_NE(&fSsd, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTCwithActualFile) {
    removeSSDFile(FILE_NAME_TEMP);
    createSSDFile(FILE_NAME_TEMP);
    openSSDFile(FILE_NAME_TEMP);
    removeSSDFile(FILE_NAME_TEMP);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4InBoundCheck) {
    vector<LBA_DATA> lba_datas = {
        {0, static_cast<int>(0x100)},
        {1, static_cast<int>(0x1AFAED)},
        {32, static_cast<int>(0xABCDEF)},
        {57, static_cast<int>(0xDEADBEEF)},
        {89, static_cast<int>(0xBEEF1082)},
        {98, static_cast<int>(0xB1E8F0E2)},
        {99, static_cast<int>(0xFFFFFFFF)},
    };

    for (LBA_DATA lba_data : lba_datas) {
        int readData;
        bool result = fSsd.readData(lba_data.lba, readData);
        EXPECT_TRUE(result);
    }
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4OutOfBoundCheck) {
    vector<LBA_DATA> lba_datas = {
        {-1, static_cast<int>(0x100)},
        {-100, static_cast<int>(0x1AFAED)},
        {132, static_cast<int>(0xABCDEF)},
        {5557, static_cast<int>(0xDEADBEEF)},
        {889, static_cast<int>(0xBEEF1082)},
        {100, static_cast<int>(0xB1E8F0E2)},
        {101, static_cast<int>(0x1082)},
        {-5557, static_cast<int>(0xDEADBEEF)},
    };

    for (LBA_DATA lba_data : lba_datas) {
        int readData;
        bool result = fSsd.readData(lba_data.lba, readData);
        EXPECT_FALSE(result);
    }
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteDataTC4InBoundCheck) {
    vector<LBA_DATA> lba_datas = {
        {0, static_cast<int>(0x100)},
        {1, static_cast<int>(0x1AFAED)},
        {32, static_cast<int>(0xABCDEF)},
        {57, static_cast<int>(0xDEADBEEF)},
        {89, static_cast<int>(0xBEEF1082)},
        {98, static_cast<int>(0xBEEFCAFE)},
        {99, static_cast<int>(0xFFFFFFFF)},
    };

    for (LBA_DATA lba_data : lba_datas) {
        bool result = fSsd.readData(lba_data.lba, lba_data.data);
        EXPECT_TRUE(result);
    }
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteDataTC4OutOfBoundCheck) {
    vector<LBA_DATA> lba_datas = {
        {-1, static_cast<int>(0x100)},
        {-100, static_cast<int>(0x1AFAED)},
        {132, static_cast<int>(0xABCDEF)},
        {5557, static_cast<int>(0xDEADBEEF)},
        {889, static_cast<int>(0xBEEF1082)},
        {100, static_cast<int>(0xB1E8F0E2)},
        {101, static_cast<int>(0x1082)},
        {-5557, static_cast<int>(0xDEADBEEF)},
    };

    for (LBA_DATA lba_data : lba_datas) {
        bool result = fSsd.readData(lba_data.lba, lba_data.data);
        EXPECT_FALSE(result);
    }
}

