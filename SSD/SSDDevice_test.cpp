#include <iostream>
#include "gmock/gmock.h"
#include "SSDDevice.h"

using namespace testing;
using namespace std;

class SSDDeviceFixture : public Test {
public:

};

TEST_F(SSDDeviceFixture, ssdCreationTC) {
    SSDDevice ssd;
    EXPECT_NE(&ssd, nullptr);
}