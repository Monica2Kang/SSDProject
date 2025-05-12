#include <iostream>
#include "gmock/gmock.h"
#include "SSDDevice.h"

using namespace testing;
using namespace std;

class SSDDeviceFixture : public Test {
public:
    SSDDevice ssd;
    void validArgumentTest(const int lba) {
        EXPECT_NO_THROW(ssd.readData(lba), invalid_argument);
    }
    void invalidArgumentTest(const int lba) {
        EXPECT_THROW(ssd.readData(lba), invalid_argument);
    }
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
        validArgumentTest(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest(addr);
    }
}