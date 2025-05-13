#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "shell.h"

using namespace testing;
using namespace std;

class MockSSDAdapter : public ISSDAdapter {
public:
	MOCK_METHOD(void, writeLba, (const int lba, const int data), (override));
	MOCK_METHOD(int, readLba, (const int lba), (override));
	MOCK_METHOD(void, erase, (const int lba, const int size), (override));
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
	EXPECT_CALL(ssdAdpater, writeLba(writeLba, data))
		.Times(1);

	instance.executeShell();
}

TEST(ShellFixture, validReadApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "read 2";
	instance.setCommand(command);

	int lba = 2;
	int data = 252645135; // 0x0F0F0F0F
	EXPECT_CALL(ssdAdpater, readLba(lba))
		.Times(1)
		.WillOnce(Return(data));

	instance.executeShell();
}

TEST(ShellFixture, validExitApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "exit";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, validHelpApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "help";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, validFullwriteApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "fullwrite 0x7FFFFFFF";
	instance.setCommand(command);

	int data = 0x7FFFFFFF;
	EXPECT_CALL(ssdAdpater, fullWrite(data))
		.Times(1);

	instance.executeShell();
}

TEST(ShellFixture, validFullreadApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "fullread";
	instance.setCommand(command);

	EXPECT_CALL(ssdAdpater, fullRead())
		.Times(1);

	instance.executeShell();
}

TEST(ShellFixture, validFullWriteAndReadCompare) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "1_";
	instance.setCommand(command);

	const int MAX_LBA = 100;
	const int expectedData = 0xBEEFCAFE;
	
	EXPECT_CALL(ssdAdpater, writeLba)
		.Times(MAX_LBA);

	EXPECT_CALL(ssdAdpater, readLba)
		.Times(MAX_LBA)
		.WillRepeatedly(Return(expectedData));

	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, validPartialLBAWrite) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "2_";
	instance.setCommand(command);

	const int loop = 150;
	const int expectedData = 0xBEEFCAFE;


	EXPECT_CALL(ssdAdpater, writeLba)
		.Times(loop);

	EXPECT_CALL(ssdAdpater, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));


	EXPECT_NO_THROW(instance.executeShell());
}

TEST(ShellFixture, validWriteReadAging) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "3_";
	instance.setCommand(command);

	const int loop = 60;
	const int expectedData = 0xBEEFCAFE;

	EXPECT_CALL(ssdAdpater, writeLba)
		.Times(loop);

	EXPECT_CALL(ssdAdpater, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));

	EXPECT_NO_THROW(instance.executeShell());
}