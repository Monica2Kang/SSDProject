#include "gmock/gmock.h"
#include "shell.h"
#include "SSDAdapter.h"
#include "TestScript.h"

static const int VALID_ARG_SIZE = 2;
static const int COMMAND_POS = 1;

int main(int argc, const char* argv[]) {
#ifndef _DEBUG
	SSDAdapter ssdAdapter;
	if (argc < VALID_ARG_SIZE) {
		Shell* shell = new Shell(&ssdAdapter);
		shell->executeShell();
	}
	else {
		TestScript* testScript = new TestScript(&ssdAdapter);
		testScript->executeRunner(argv[COMMAND_POS]);
	}
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif
}