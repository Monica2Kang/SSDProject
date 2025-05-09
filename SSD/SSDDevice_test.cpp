#include <iostream>
#include "gmock/gmock.h"
#include "SSDDevice.h"

using namespace testing;
using namespace std;

class SSDDeviceFixture : public Test {
public:
    SSDDevice ssd;
};

TEST_F(SSDDeviceFixture, ssdCreationTC) {

    EXPECT_NE(&ssd, nullptr);
}