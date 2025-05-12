#include "gmock/gmock.h"
#include "TestScript.h"

using namespace testing;
using namespace std;

class TestScriptFixture : public Test {
public:
	TestScript script;
};

TEST_F(TestScriptFixture, FullWriteAndReadCompare) {
	int result = script.FullWriteAndReadCompare();

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, PartialLBAWrite) {
	int result = script.PartialLBAWrite();

	EXPECT_EQ(result, PASS);
}

TEST_F(TestScriptFixture, WriteReadAging) {
	int result = script.WriteReadAging();

	EXPECT_EQ(result, PASS);
}