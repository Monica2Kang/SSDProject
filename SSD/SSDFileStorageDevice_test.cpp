#include "gmock/gmock.h"
#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

class SSDFileStorageDeviceFixture : public Test {
public:
protected:
    SSDFileStorageDevice fSsd = { "ssd_nand.txt", 100 };
};

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageDummyInstanceCreationTC) {
    SSDFileStorageDevice fSsdDummy;
    EXPECT_NE(&fSsdDummy, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC) {
    EXPECT_NE(&fSsd, nullptr);
}