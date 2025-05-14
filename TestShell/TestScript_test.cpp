#include "gmock/gmock.h"
#include "TestScript.h"

using namespace testing;
using namespace std;


class MockSSDAdapter : public ISSDAdapter {
public:
	MOCK_METHOD(void, writeLba, (const int lba, const int data), (override));
	MOCK_METHOD(int, readLba, (const int lba), (override));
	MOCK_METHOD(void, erase, (const int lba, const int size), (override));
	MOCK_METHOD(void, fullWrite, (const int data), (override));
	MOCK_METHOD(void, fullRead, (), (override));
	MOCK_METHOD(void, flush, (), (override));
};

class TestScriptFixture : public Test {
public:
	MockSSDAdapter mockSSDAdapter;
	SSDAdapter ssdAdapter;

public:
	const int TEST_DATA = 0xBEEFCAFE;
};

TEST_F(TestScriptFixture, DISABLED_FullWriteAndReadCompare1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int MAX_LBA = 100;
	const int expectedData = 0xBEEFCAFE;
	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(MAX_LBA);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(MAX_LBA)
		.WillRepeatedly(Return(expectedData));

	int result = mockScript->fullWriteAndReadCompare(expectedData);
}

TEST_F(TestScriptFixture, DISABLED_PartialLBAWrite1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int loop = 150;
	const int expectedData = 0xBEEFCAFE;
	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(loop);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));


	int result = mockScript->partialLBAWrite(expectedData);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, DISABLED_WriteReadAging1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int loop = 60;
	const int expectedData = 0xBEEFCAFE;

	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(loop);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));

	int result = mockScript->writeReadAging();

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, FullWriteAndReadCompare2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->fullWriteAndReadCompare(TEST_DATA);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, PartialLBAWrite2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->partialLBAWrite(TEST_DATA);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, WriteReadAging2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->writeReadAging();

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, EraseAndWriteAging) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->eraseAndWriteAging();

	EXPECT_EQ(result, PASS);
}