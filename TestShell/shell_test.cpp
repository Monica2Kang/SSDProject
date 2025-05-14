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
	MOCK_METHOD(bool, writeLba, (std::string lba, std::string data), (override));
	MOCK_METHOD(bool, readLba, (std::string lba, int& readData), (override));
	MOCK_METHOD(bool, erase, (std::string lba, std::string size), (override));
	MOCK_METHOD(void, fullWrite, (const int data), (override));
	MOCK_METHOD(void, fullRead, (), (override));
	MOCK_METHOD(void, flush, (), (override));
};

class ShellFixture : public Test {
public:
	SSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	void runScenarioTest(const string command) {
		instance.setCommand(command);
		instance.executeShell();
	}
};

TEST_F(ShellFixture, creationTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	EXPECT_NE(&instance, nullptr);
}

TEST_F(ShellFixture, noCommandTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, invalidCommandTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "wrate 3 0xAAAABBBB";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, invalidLBATest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "write 100 0xAAAABBBB";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, invalidDataTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "write 1 0x000000G";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, validWriteApiTest) {
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

TEST_F(ShellFixture, validReadApiTest) {
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

TEST_F(ShellFixture, DISABLED_validExitApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "exit";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, validHelpApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "help";
	instance.setCommand(command);

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, validFullwriteApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "fullwrite 0x7FFFFFFF";
	instance.setCommand(command);

	int data = 0x7FFFFFFF;
	EXPECT_CALL(ssdAdpater, fullWrite(data))
		.Times(1);

	instance.executeShell();
}

TEST_F(ShellFixture, validFullreadApiTest) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "fullread";
	instance.setCommand(command);

	EXPECT_CALL(ssdAdpater, fullRead())
		.Times(1);

	instance.executeShell();
}

TEST_F(ShellFixture, DISABLED_validFullWriteAndReadCompare) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "1_";
	instance.setCommand(command);

	const int MAX_LBA = 100;
	const int expectedData = 0xBEEFCAFE;
	
	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, DISABLED_validPartialLBAWrite) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "2_";
	instance.setCommand(command);

	const int loop = 150;
	const int expectedData = 0xBEEFCAFE;

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, DISABLED_validWriteReadAging) {
	MockSSDAdapter ssdAdpater;
	Shell instance{ &ssdAdpater };

	const string command = "3_";
	instance.setCommand(command);

	const int loop = 60;
	const int expectedData = 0xBEEFCAFE;

	EXPECT_NO_THROW(instance.executeShell());
}

TEST_F(ShellFixture, validTestScenario1) {

	runScenarioTest("1_");

	EXPECT_NO_THROW();
}

TEST_F(ShellFixture, validTestScenario2) {

	runScenarioTest("2_");

	EXPECT_NO_THROW();
}

TEST_F(ShellFixture, validTestScenario3) {

	runScenarioTest("3_");

	EXPECT_NO_THROW();
}

TEST_F(ShellFixture, validTestScenario4) {

	runScenarioTest("4_");

	EXPECT_NO_THROW();
}

TEST_F(ShellFixture, errorScenario1) {

	runScenarioTest("98_");

	EXPECT_NO_THROW();
}

TEST_F(ShellFixture, errorScenario2) {

	runScenarioTest("99_");

	EXPECT_NO_THROW();
}