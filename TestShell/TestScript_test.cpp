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
};

class TestScriptFixture : public Test {
public:
	MockSSDAdapter mockSSDAdapter;
	SSDAdapter ssdAdapter;

public:
	const int TEST_DATA = 0xBEEFCAFE;
};

TEST_F(TestScriptFixture, FullWriteAndReadCompare1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int MAX_LBA = 100;
	const int expectedData = 0xBEEFCAFE;
	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(MAX_LBA);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(MAX_LBA)
		.WillRepeatedly(Return(expectedData));

	int result = mockScript->FullWriteAndReadCompare(expectedData);
}

TEST_F(TestScriptFixture, PartialLBAWrite1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int loop = 150;
	const int expectedData = 0xBEEFCAFE;
	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(loop);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));


	int result = mockScript->PartialLBAWrite(expectedData);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, WriteReadAging1) {
	TestScript* mockScript = new TestScript(&mockSSDAdapter);
	const int loop = 60;
	const int expectedData = 0xBEEFCAFE;

	EXPECT_CALL(mockSSDAdapter, writeLba)
		.Times(loop);

	EXPECT_CALL(mockSSDAdapter, readLba)
		.Times(loop)
		.WillRepeatedly(Return(expectedData));

	int result = mockScript->WriteReadAging();

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, FullWriteAndReadCompare2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->FullWriteAndReadCompare(TEST_DATA);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, PartialLBAWrite2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->PartialLBAWrite(TEST_DATA);

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, WriteReadAging2) {
	TestScript* script = new TestScript(&ssdAdapter);
	int result = script->WriteReadAging();

	EXPECT_EQ(result, PASS);
}