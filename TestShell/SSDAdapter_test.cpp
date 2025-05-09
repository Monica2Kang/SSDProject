#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "SSDAdapter.h"

using namespace testing;
using namespace std;

class SSDAdapterFixture : public Test {
public:
	
};

TEST_F(SSDAdapterFixture, InterfaceTest) {
	ISSDAdapter instance;
	EXPECT_NE(&instance, nullptr);
}