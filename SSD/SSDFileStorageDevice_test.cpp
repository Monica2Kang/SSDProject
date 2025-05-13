#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

namespace {

}

class SSDFileStorageDeviceFixture : public Test {
public:
    void removeSSDFile(const char* filename) {
        std::ifstream ssdFile;
        if (0 == std::remove(filename)) {
            //std::cout << filename << " file is deleted." << std::endl;
        }
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_FALSE(fileOpened);
    }
    void createSSDFile(const char* filename) {
        SSDFileStorageDevice ssdFile = { filename, FILE_STORAGE_CAPACITY };
        ssdFile.openFile();
        ssdFile.closeFile();
    }
    void openSSDFile(const char* filename) {
        std::ifstream ssdFile;
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_TRUE(fileOpened);
    }
    void doInRangeBoundaryCheck(const bool expectation) {
        fSsd.openFile();
        for (LBA_DATA lba_data : inRangeLbaData) {
            bool result = fSsd.readData(lba_data.lba, lba_data.data);
            if (expectation)
                EXPECT_TRUE(result);
            else
                EXPECT_FALSE(result);
        }
        fSsd.closeFile();
    }
    void doOutOfRangeBoundaryCheck(const bool expectation)     {
        fSsd.openFile();
        for (LBA_DATA lba_data : outOfRangeLbaData) {
            bool result = fSsd.readData(lba_data.lba, lba_data.data);
            if (expectation)
                EXPECT_TRUE(result);
            else
                EXPECT_FALSE(result);
        }
        fSsd.closeFile();
    }
    void doReadDataConfirmation(void) {
        fSsd.openFile();

        for (LBA_DATA lba_data : inRangeLbaData) {
            int readData;
            bool result = fSsd.readData(lba_data.lba, readData);
            EXPECT_FALSE(result);
            fSsd.writeData(lba_data.lba, lba_data.data);
            result = fSsd.readData(lba_data.lba, readData);
            EXPECT_TRUE(result);
            //EXPECT_TRUE(result);
            //EXPECT_EQ(readData, 0x00);
        }
        for (LBA_DATA lba_data : outOfRangeLbaData) {
            int readData;
            bool result = fSsd.readData(lba_data.lba, readData);
            EXPECT_FALSE(result);
        }
        fSsd.closeFile();
    }
    void removeAndCreateFile(void) {
        fSsd.closeFile();
        fSsd.removeFile();
    }

protected:
    struct LBA_DATA {
        int lba;
        int data;
    };

    const char* FILE_NAME = "ssd_nand.txt";
    const char* FILE_NAME_TEMP = "ssd_nand_temp.txt";
    const int FILE_STORAGE_CAPACITY = 100;
    SSDFileStorageDevice fSsd = { FILE_NAME, FILE_STORAGE_CAPACITY };

    const vector<LBA_DATA> inRangeLbaData = {
        {0, static_cast<int>(0x100)},
        {1, static_cast<int>(0x1AFAED)},
        {98, static_cast<int>(0xB1E8F0E2)},
        {57, static_cast<int>(0xDEADBEEF)},
        {32, static_cast<int>(0xABCDEF)},
        {89, static_cast<int>(0xBEEF1082)},
        {99, static_cast<int>(0xFFFFFFFF)},
    };
    const vector<LBA_DATA> outOfRangeLbaData = {
        {-1, static_cast<int>(0x100)},
        {-100, static_cast<int>(0x1AFAED)},
        {132, static_cast<int>(0xABCDEF)},
        {5557, static_cast<int>(0xDEADBEEF)},
        {889, static_cast<int>(0xBEEF1082)},
        {100, static_cast<int>(0xB1E8F0E2)},
        {101, static_cast<int>(0x1082)},
        {-15557, static_cast<int>(0xDEADBEEF)},
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

// ssdFileReadDataTCs
TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4InBoundCheck) {
    fSsd.removeFile();
    fSsd.openFile();
    doInRangeBoundaryCheck(false);

    fSsd.openFile();
    for (LBA_DATA lba_data : inRangeLbaData) {
        fSsd.writeData(lba_data.lba, lba_data.data);
        bool result = fSsd.readData(lba_data.lba, lba_data.data);
        EXPECT_TRUE(result);
    }
    fSsd.closeFile();

}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4OutOfBoundCheck) {
    doOutOfRangeBoundaryCheck(false); 
}

// ssdFileWriteDataTCs
TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteDataTC4InBoundCheck) {
    doInRangeBoundaryCheck(true);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteDataTC4OutOfBoundCheck) {
    doOutOfRangeBoundaryCheck(false);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteDataTC4FileNotOpened) {
    // No file opened
    for (LBA_DATA lba_data : inRangeLbaData) {
        bool result = fSsd.readData(lba_data.lba, lba_data.data);
        EXPECT_FALSE(result);
    }
    doInRangeBoundaryCheck(true);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4FileNotOpened) {
    // No file opened
    for (LBA_DATA lba_data : inRangeLbaData) {
        bool result = fSsd.readData(lba_data.lba, lba_data.data);
        EXPECT_FALSE(result);
    }
    doInRangeBoundaryCheck(true);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4ReadData) {
    removeAndCreateFile();
    doReadDataConfirmation();
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteReadConfirmDataTC) {
    removeAndCreateFile();
    doReadDataConfirmation();
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileWriteReadConfirmDataTC_WriteBianryZero4AllRange) {
    removeAndCreateFile();
    doReadDataConfirmation();
}
