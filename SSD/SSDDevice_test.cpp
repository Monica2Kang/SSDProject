#include <iostream>
#include "gmock/gmock.h"
#include "SSDDevice.h"

using namespace testing;
using namespace std;

class SSDDeviceFixture : public Test {
public:
    SSDDevice ssd;
    void validArgumentTest4readData(const int lba) {
        EXPECT_NO_THROW(ssd.readData(lba));
    }
    void invalidArgumentTest4readData(const int lba) {
        EXPECT_THROW(ssd.readData(lba), invalid_argument);
    }
    void validArgumentTest4writeData(const int lba) {
        EXPECT_NO_THROW(ssd.writeData(lba, SAMPLE_DATA));
    }
    void invalidArgumentTest4writeData(const int lba) {
        EXPECT_THROW(ssd.writeData(lba, SAMPLE_DATA), invalid_argument);
    }

protected:
    struct LBA_DATA {
        int lba;
        int data;
    };
    static const int SAMPLE_DATA = static_cast<int>(0x1082);
    const vector<LBA_DATA> inRangeLbaDatas = {
    {0, static_cast<int>(0x100)},
    {1, static_cast<int>(0x1AFAED)},
    {32, static_cast<int>(0xABCDEF)},
    {57, static_cast<int>(0xDEADBEEF)},
    {89, static_cast<int>(0xBEEF1082)},
    {98, static_cast<int>(0xB1E8F0E2)},
    };
};

TEST_F(SSDDeviceFixture, ssdCreationTC) {
    EXPECT_NE(&ssd, nullptr);
}

//TEST_F(SSDDeviceFixture, ssdReadDataTC) {
//    int actual = ssd.readData(0);
//    EXPECT_EQ(0, actual);
//}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAInRange) {
    vector<int> lba = { 0, 1, 10, 20, 30, 53, 75, 97, 98, 99 };
    for (int addr : lba) {
        validArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAOutOfRangeMinus) {
    vector<int> lba = { -1, -2, -110, -253,-1082, -337, -4 };
    for (int addr : lba) {
        invalidArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAInRange) {
    vector<int> lba = { 0, 1, 10, 20, 30, 53, 75, 97, 98, 99 };
    for (int addr : lba) {
        validArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAOutOfRangeMinus) {
    vector<int> lba = { -1, -2, -110, -253,-1082, -337, -4 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteReadDataConfirmTC) {
    ssd.writeData(0, static_cast <int>(0x0));
    int actual = ssd.readData(0);
    EXPECT_EQ(0, actual);

    ssd.writeData(0, SAMPLE_DATA);
    actual = ssd.readData(0);
    EXPECT_EQ(SAMPLE_DATA, actual);
}

TEST_F(SSDDeviceFixture, ssdWriteReadDataConfirmTC4Multi) {
    for (LBA_DATA lba_data : inRangeLbaDatas) {
        ssd.writeData(lba_data.lba, lba_data.data);
        int actual = ssd.readData(lba_data.lba);
        EXPECT_EQ(lba_data.data, actual);
    }
}