#include "gmock/gmock.h"
#include "TestShellLogger.h"

using namespace testing;
using namespace std;

TEST(Logger, LogTest) {
	TEST_SHELL_LOG("TEST");
	TEST_SHELL_LOG(1);
	TEST_SHELL_LOG(1, 2);
	TEST_SHELL_LOG(1, 2, 3);
}

TEST(Logger, MassLogTest) {
	const int loop = 10000;
	for (auto it = 0; it < loop; it++)
	{
		TEST_SHELL_LOG(it);
	}
}