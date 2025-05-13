#pragma once

#include "ISSDAdapter.h"
#include "SSDAdapter.h"
#include <string>

static const int PASS = 0;
static const int FAIL = 1;
static const string testDir = "../TestScripts";

using namespace std;

class TestScript {
public:
	TestScript() = default;
	TestScript(ISSDAdapter* obj) {
		ssdAdapter = obj;
	}

	int runTest(const string str);

	int FullWriteAndReadCompare(const int data);
	int PartialLBAWrite(const int data);
	int WriteReadAging(void);

private:
	ISSDAdapter* ssdAdapter;
	string inputTestScript;
	string ScriptFilePath;

	bool checkTestExist();
	int excuteTest();
};