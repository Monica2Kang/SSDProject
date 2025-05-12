#include "gmock/gmock.h"
#include "ISSDAdapter.h"
#include "SSDAdapter.h"

using namespace testing;
using namespace std;

class MockSSDAdapter : public ISSDAdapter {
public:
	MOCK_METHOD(void, wirteLba, (const int lba, const int data), (override));
	MOCK_METHOD(int, readLba, (const int lba), (override));
	MOCK_METHOD(void, fullWrite, (const int data), (override));
	MOCK_METHOD(void, fullRead, (), (override));
};

class SSDAdapterFixture : public Test {
public:
	
};

TEST_F(SSDAdapterFixture, InterfaceTest) {
	ISSDAdapter* ssdAdpater = new SSDAdapter();

	EXPECT_NE(ssdAdpater, nullptr);
}

TEST_F(SSDAdapterFixture, ApiTestWriteLba) {
	MockSSDAdapter ssdAdpater;

	const int writeLba = 0;
	const int data = 0xCAFECAFE;

	EXPECT_CALL(ssdAdpater, wirteLba(writeLba, data))
		.Times(1);

	ssdAdpater.wirteLba(writeLba, data);
}

TEST_F(SSDAdapterFixture, ApiTestReadLba) {
	MockSSDAdapter ssdAdpater;

	const int readLba = 0;
	const int data = 0xCAFECAFE;

	EXPECT_CALL(ssdAdpater, readLba(readLba))
		.Times(1)
		.WillOnce(Return(data));

	int returnData = ssdAdpater.readLba(readLba);
	
	EXPECT_EQ(returnData, data);
}

TEST_F(SSDAdapterFixture, ApiTestFullWrite) {
	MockSSDAdapter ssdAdpater;

	const int data = 0xCAFECAFE;

	EXPECT_CALL(ssdAdpater, fullWrite(data))
		.Times(1);

	ssdAdpater.fullWrite(data);
}

TEST_F(SSDAdapterFixture, ApiTestFullRead) {
	MockSSDAdapter ssdAdpater;


	EXPECT_CALL(ssdAdpater, fullRead())
		.Times(1);

	ssdAdpater.fullRead();
}

TEST_F(SSDAdapterFixture, AdpaterTestWriteLba) {
	SSDAdapter ssdAdpater;

	const int writeLba = 0;
	const int data = 0xCAFECAFE;

	ssdAdpater.wirteLba(writeLba, data);

	EXPECT_NO_THROW();
}


TEST_F(SSDAdapterFixture, AdpaterTestFullWrite) {
	SSDAdapter ssdAdpater;

	const int data = 0xCAFECAFE;

	ssdAdpater.fullWrite(data);

	EXPECT_NO_THROW();
}