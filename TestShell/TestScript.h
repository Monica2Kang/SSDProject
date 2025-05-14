#pragma once

#include "ISSDAdapter.h"
#include "SSDAdapter.h"
#include <string>

static const int PASS = 0;
static const int FAIL = 1;
static const string testDir = "..\\TestScenarios";

using namespace std;

class TestScript {
public:
	TestScript() = default;
	TestScript(ISSDAdapter* ISSDAdapter);

	int runTest(const string str);
	void makeScenario(void);

	int fullWriteAndReadCompare(const int data);
	int partialLBAWrite(const int data);
	int writeReadAging(void);
	int eraseAndWriteAging(void);

private:
	ISSDAdapter* ssdAdapter;
	string inputTestScript;
	string ScriptFilePath;

	bool _checkTestExist();
	int _excuteTest();

	std::string _writeCommand(const int lba, const int data);
	std::string _readCommand(const int lba);
	std::string _eraseCommand(const int lba, const int size);
};