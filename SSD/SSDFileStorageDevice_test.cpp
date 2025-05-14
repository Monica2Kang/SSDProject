#include <iostream>
#include <fstream>
#include "gmock/gmock.h"
#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

//#define SSD_FILE_STORAGE_DEVICE SSD_FILE_STORAGE_DEVICE;

class SSDFileStorageDeviceFixture : public Test {
public:
    void openSSDFile(const char* filename) {
        std::ifstream ssdFile;
        ssdFile.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        bool fileOpened = ssdFile.is_open();
        EXPECT_TRUE(fileOpened);
    }
    void doInRangeBoundaryCheck(const bool expectation) {
        //SSD_FILE_STORAGE_DEVICE.openFile();
        for (LBA_DATA lba_data : inRangeLbaData) {
            bool result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, lba_data.data);
            if (expectation)
                EXPECT_TRUE(result);
            else
                EXPECT_FALSE(result);
        }
        //SSD_FILE_STORAGE_DEVICE.closeFile();
    }
    void doOutOfRangeBoundaryCheck(const bool expectation)     {
        //SSD_FILE_STORAGE_DEVICE.openFile();
        for (LBA_DATA lba_data : outOfRangeLbaData) {
            bool result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, lba_data.data);
            if (expectation)
                EXPECT_TRUE(result);
            else
                EXPECT_FALSE(result);
        }
        //SSD_FILE_STORAGE_DEVICE.closeFile();
    }
    void doReadDataConfirmation(void) {
        //SSD_FILE_STORAGE_DEVICE.openFile();

        for (LBA_DATA lba_data : inRangeLbaData) {
            unsigned int readData;
            bool result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, readData);
            EXPECT_FALSE(result);
            SSD_FILE_STORAGE_DEVICE.writeData(lba_data.lba, lba_data.data);
            result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, readData);
            EXPECT_TRUE(result);
            //EXPECT_TRUE(result);
            //EXPECT_EQ(readData, 0x00);
        }
        for (LBA_DATA lba_data : outOfRangeLbaData) {
            unsigned int readData;
            bool result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, readData);
            EXPECT_FALSE(result);
        }
        //SSD_FILE_STORAGE_DEVICE.closeFile();
    }
    void removeAndCreateFile(void) {
        SSD_FILE_STORAGE_DEVICE.removeFile();
        SSD_FILE_STORAGE_DEVICE.createFile();
    }

protected:
    struct LBA_DATA {
        int lba;
        unsigned int data;
    };

    const char* FILE_NAME = "ssd_nand.txt";
    const char* FILE_NAME_TEMP = "ssd_nand_temp.txt";
    const int FILE_STORAGE_CAPACITY = 100;
    //SSDFileStorageDevice SSD_FILE_STORAGE_DEVICE = { FILE_NAME, FILE_STORAGE_CAPACITY };


    const vector<LBA_DATA> inRangeLbaData = {
        {0,  0x100},
        {1,  0x1AFAED},
        {98, 0xB1E8F0E2},
        {57, 0xDEADBEEF},
        {32, 0xABCDEF},
        {89, 0xBEEF1082},
        {99, 0xFFFFFFFF},
    };
    const vector<LBA_DATA> outOfRangeLbaData = {
        {-1, 0x100},
        {-100, 0x1AFAED},
        {132, 0xABCDEF},
        {5557, 0xDEADBEEF},
        {889, 0xBEEF1082},
        {100, 0xB1E8F0E2},
        {101, 0x1082},
        {-15557, 0xDEADBEEF},
    };
};

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageDummyInstanceCreationTC) {
    SSDFileStorageDevice& instance = SSDFileStorageDevice::getInstance();
    EXPECT_NE(&instance, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC) {
    EXPECT_NE(&SSD_FILE_STORAGE_DEVICE, nullptr);
}

// ssdFileReadDataTCs
TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4InBoundCheck) {
    SSD_FILE_STORAGE_DEVICE.removeFile();
    SSD_FILE_STORAGE_DEVICE.createFile();
    //SSD_FILE_STORAGE_DEVICE.openFile();
    doInRangeBoundaryCheck(false);

    //SSD_FILE_STORAGE_DEVICE.openFile();
    for (LBA_DATA lba_data : inRangeLbaData) {
        SSD_FILE_STORAGE_DEVICE.writeData(lba_data.lba, lba_data.data);
        bool result = SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, lba_data.data);
        EXPECT_TRUE(result);
    }
    //SSD_FILE_STORAGE_DEVICE.closeFile();

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
    SSD_FILE_STORAGE_DEVICE.removeFile();
    for (LBA_DATA lba_data : inRangeLbaData) {
        EXPECT_THROW(SSD_FILE_STORAGE_DEVICE.writeData(lba_data.lba, lba_data.data), exception);
    }
    SSD_FILE_STORAGE_DEVICE.createFile();
    doInRangeBoundaryCheck(false); // 여기선 false가 맞음. 한번도 writeData() 하지 않았으므로.
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileReadDataTC4FileNotOpened) {
    SSD_FILE_STORAGE_DEVICE.removeFile();
    for (LBA_DATA lba_data : inRangeLbaData) {
        EXPECT_THROW(SSD_FILE_STORAGE_DEVICE.readData(lba_data.lba, lba_data.data), exception);
    }
    SSD_FILE_STORAGE_DEVICE.createFile();
    doInRangeBoundaryCheck(false); // 여기선 false가 맞음. 한번도 writeData() 하지 않았으므로.
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
