#include "gmock/gmock.h"
#include "shell.h"
#include "SSDAdapter.h"
#include "TestScript.h"

int main() {
#ifndef _DEBUG
	SSDAdapter ssdAdapter;
	Shell* shell = new Shell(&ssdAdapter);
	shell->executeShell();
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif
}