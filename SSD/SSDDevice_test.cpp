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
    void validArgumentTest4writeData(int lba) {
        EXPECT_NO_THROW(ssd.writeData(lba, SAMPLE_DATA));
    }
    void invalidArgumentTest4writeData(int lba) {
        EXPECT_THROW(ssd.writeData(lba, SAMPLE_DATA), invalid_argument);
    }

private:
    static const int SAMPLE_DATA = static_cast<int>(0x1082);
};

TEST_F(SSDDeviceFixture, ssdCreationTC) {
    EXPECT_NE(&ssd, nullptr);
}

TEST_F(SSDDeviceFixture, ssdReadDataTC) {
    int actual = ssd.readData(0);
    EXPECT_EQ(0, actual);
}

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

TEST_F(SSDDeviceFixture, ssdWriteDataTC_LBAInRange) {
    vector<int> lba = { 0, 1, 10, 20, 30, 53, 75, 97, 98, 99 };
    for (int addr : lba) {
        validArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC_LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC_LBAOutOfRangeMinus) {
    vector<int> lba = { -1, -2, -110, -253,-1082, -337, -4 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}