#include "gmock/gmock.h"
#include "shell.h"

using namespace testing;
using namespace std;

class ShellFixture : public Test {
public:

};

TEST(ShellFixture, creationTest) {
	Shell instance;
	EXPECT_NE(&instance, nullptr);
}