#pragma once
#include "ISSDAdapter.h"
#include "SSDAdapter.h"
#include "FileManager.h"

static const int PASS = 0;
static const int FAIL = 1;

class TestScript {
public:
	TestScript() = default;
	TestScript(ISSDAdapter* obj) {
		ssdAdapter = obj;

		// creates scenario folder
		//FILE_MANAGER.createDirectory(SCENARIO_DIR);
		//makeScenario();

		//const std::string scenarioPath = SCENARIO_DIR + "\\1_FullWriteAndReadCompare.txt";
		//runScenario(scenarioPath);

	}

	int FullWriteAndReadCompare(const int data);
	int PartialLBAWrite(const int data);
	int WriteReadAging(void);

	int runScenario(const std::string scenarioPath);
	void makeScenario(void);
	std::vector<std::string> splitCommand(const std::string& input);

private:
	ISSDAdapter* ssdAdapter;
	
	const std::string SCENARIO_DIR = "SSDTestScenario";

	std::string _writeCommand(const int lba, const int data);
	std::string _readCommand(const int lba);
	std::string _eraseCommand(const int lba, const int size);
};