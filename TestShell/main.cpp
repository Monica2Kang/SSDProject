#include "gmock/gmock.h"
#include "shell.h"
#include "SSDAdapter.h"
#include "TestScript.h"

int main() {
#ifndef _DEBUG
	SSDAdapter ssdAdapter;
	//TestScript* testScript = new TestScript(&ssdAdapter);
	//int result = testScript->runTest("1_");
	//cout << "Result : " << result;
	Shell* shell = new Shell(&ssdAdapter);
	shell->executeShell();
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif
}