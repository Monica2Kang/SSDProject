#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "SSDAdapter.h"

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

class SSDAdapterFixture : public Test {
public:
	MockSSDAdapter ssdMockAdpater;
	SSDAdapter ssdAdpater;
};

TEST_F(SSDAdapterFixture, InterfaceTest) {
	ISSDAdapter* ssdAdpater = new SSDAdapter();

	EXPECT_NE(ssdAdpater, nullptr);
}

TEST_F(SSDAdapterFixture, ApiTestWriteLba) {
	const int writeLba = 0;
	const int data = 0xBEEFCAFE;

	EXPECT_CALL(ssdMockAdpater, writeLba(writeLba, data))
		.Times(1);

	ssdMockAdpater.writeLba(writeLba, data);
}

TEST_F(SSDAdapterFixture, ApiTestReadLba) {
	const int readLba = 0;
	const int data = 0xBEEFCAFE;

	EXPECT_CALL(ssdMockAdpater, readLba(readLba))
		.Times(1)
		.WillOnce(Return(data));

	int returnData = ssdMockAdpater.readLba(readLba);
	
	EXPECT_EQ(returnData, data);
}

TEST_F(SSDAdapterFixture, ApiTestFullWrite) {
	const int data = 0xBEEFCAFE;

	EXPECT_CALL(ssdMockAdpater, fullWrite(data))
		.Times(1);

	ssdMockAdpater.fullWrite(data);
}

TEST_F(SSDAdapterFixture, ApiTestFullRead) {
	EXPECT_CALL(ssdMockAdpater, fullRead())
		.Times(1);

	ssdMockAdpater.fullRead();
}

TEST_F(SSDAdapterFixture, AdpaterTestWriteLba) {
	const int writeLba = 0;
	const int data = 0xBEEFCAFE;

	ssdAdpater.writeLba(writeLba, data);

	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestFullWrite) {
	const int data = 0xBEEFCAFE;

	ssdAdpater.fullWrite(data);

	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestReadLba) {
	const int readLba = 0;
	const int data = 0xBEEFCAFE;

	int expectedValue = ssdAdpater.readLba(readLba);

	EXPECT_EQ(expectedValue, data);
	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestFullRead) {
	const int data = 0xBEEFCAFE;

	ssdAdpater.fullRead();

	EXPECT_NO_THROW();
}