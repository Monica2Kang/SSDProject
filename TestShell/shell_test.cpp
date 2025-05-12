#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "shell.h"

using namespace testing;
using namespace std;

class MockSSDAdapter : public ISSDAdapter {
public:
	MOCK_METHOD(void, wirteLba, (const int lba, const int data), (override));
	MOCK_METHOD(int, readLba, (const int lba), (override));
	MOCK_METHOD(void, fullWrite, (const int data), (override));
	MOCK_METHOD(void, fullRead, (), (override));
};

class ShellFixture : public Test {
public:

};

TEST(ShellFixture, creationTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	EXPECT_NE(&instance, nullptr);
}

TEST(ShellFixture, noCommandTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, invalidCommandTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "wrate 3 0xAAAABBBB";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, invalidLBATest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "write 100 0xAAAABBBB";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, invalidDataTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "write 1 0x000000G";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, validWriteApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "write 1 0xAAAABBBB";
	instance.setCommand(command);

	int writeLba = 1;
	int data = 0xAAAABBBB;
	EXPECT_CALL(ssdAdpater, wirteLba(writeLba, data))
		.Times(1);

	instance.executeShell();
}