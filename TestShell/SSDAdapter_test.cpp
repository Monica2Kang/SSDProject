#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "SSDAdapter.h"

using namespace testing;
using namespace std;

class MockSSDAdapter : public ISSDAdapter {
public:
	MOCK_METHOD(void, writeLba, (const int lba, const int data), (override));
	MOCK_METHOD(int, readLba, (const int lba), (override));
	MOCK_METHOD(void, fullWrite, (const int data), (override));
	MOCK_METHOD(void, fullRead, (), (override));
};

class SSDAdapterFixture : public Test {
public:
	MockSSDAdapter ssdMockAdpater;
	SSDAdapter ssdAdpater;

public:
	const int DEFAULT_DATA = 0xBEEFCAFE;
	const int MAX_LBA = 100;
};

TEST_F(SSDAdapterFixture, InterfaceTest) {
	ISSDAdapter* ssdAdpater = new SSDAdapter();

	EXPECT_NE(ssdAdpater, nullptr);
}

TEST_F(SSDAdapterFixture, ApiTestWriteLba) {
	const int writeLba = 0;

	EXPECT_CALL(ssdMockAdpater, writeLba(writeLba, DEFAULT_DATA))
		.Times(1);

	ssdMockAdpater.writeLba(writeLba, DEFAULT_DATA);
}

TEST_F(SSDAdapterFixture, ApiTestWriteLbaRange) {
	const int writeLba = 0;
	const int size = 10;

	EXPECT_CALL(ssdMockAdpater, writeLba)
		.Times(size);

	for (auto i = 0; i < size; i++) {
		ssdMockAdpater.writeLba(writeLba + i, DEFAULT_DATA);
	}
}

TEST_F(SSDAdapterFixture, ApiTestReadLba) {
	const int readLba = 0;

	EXPECT_CALL(ssdMockAdpater, readLba(readLba))
		.Times(1)
		.WillOnce(Return(DEFAULT_DATA));

	int returnData = ssdMockAdpater.readLba(readLba);
	
	EXPECT_EQ(returnData, DEFAULT_DATA);
}

TEST_F(SSDAdapterFixture, ApiTestFullWrite) {
	EXPECT_CALL(ssdMockAdpater, fullWrite(DEFAULT_DATA))
		.Times(1);

	ssdMockAdpater.fullWrite(DEFAULT_DATA);
}

TEST_F(SSDAdapterFixture, ApiTestFullRead) {
	EXPECT_CALL(ssdMockAdpater, fullRead())
		.Times(1);

	ssdMockAdpater.fullRead();
}

TEST_F(SSDAdapterFixture, AdpaterTestWriteLba) {
	const int writeLba = 0;

	ssdAdpater.writeLba(writeLba, DEFAULT_DATA);

	EXPECT_NO_THROW();
}


TEST_F(SSDAdapterFixture, WriteAndVerifyTest) {
	const int writeLba = 0;

	ssdAdpater.writeLba(writeLba, DEFAULT_DATA);

	int expectedValue = ssdAdpater.readLba(writeLba);

	EXPECT_EQ(expectedValue, DEFAULT_DATA);
	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, OverWriteAndVerifyTest) {
	const int writeLba = 0;
	const int firstData = 0xDEADBEEF;

	ssdAdpater.writeLba(writeLba, firstData);
	ssdAdpater.writeLba(writeLba, DEFAULT_DATA);

	int expectedValue = ssdAdpater.readLba(writeLba);

	EXPECT_EQ(expectedValue, DEFAULT_DATA);
	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestFullWrite) {
	ssdAdpater.fullWrite(DEFAULT_DATA);

	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestReadLba) {
	const int readLba = 0;
	int expectedValue = ssdAdpater.readLba(readLba);

	EXPECT_EQ(expectedValue, DEFAULT_DATA);
	EXPECT_NO_THROW();
}

TEST_F(SSDAdapterFixture, AdpaterTestFullRead) {
	ssdAdpater.fullRead();

	EXPECT_NO_THROW();
}