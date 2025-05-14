#include <iostream>
#include <istream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include "TestScript.h"
#include "FileManager.h"

using namespace std;

TestScript::TestScript(ISSDAdapter* ISSDAdapter) : ssdAdapter(ISSDAdapter)
{
}

int TestScript::runTest(const string str) {
	inputTestScript= str;
	if (_checkTestExist()) {
		return _excuteTest();
	}
	else {
		return FAIL;
	}
}

bool TestScript::_checkTestExist() {
	bool isExist = false;
	std::string fullPath = FILE_MANAGER.findFileWithPrefix(testDir, inputTestScript);
	if (fullPath == "") return false;

	ScriptFilePath = testDir + "\\" + fullPath;

	isExist = FILE_MANAGER.fileExists(ScriptFilePath);
	return isExist;
}

int TestScript::_excuteTest() {
	fstream file(ScriptFilePath);
	if (!file.is_open()) {
		return FAIL;
	}

	vector<string> parameter;
	vector<int> expectedData(100, 0);
	int LBA;
	int data;
	int size;
	string line;
	bool bReturn = true;
	while (getline(file, line)) {
		parameter.clear();
		stringstream ss(line);
		string word;

		while (ss >> word) {
			parameter.push_back(word);
		}
		
		if (parameter[0] == "W") {
			LBA = stoi(parameter[1]);
			data = static_cast<int>(stoul(parameter[2], nullptr, 16));
			//cout << "[" << LBA << "] " << parameter[1] << " " << data << endl;
			bReturn = ssdAdapter->writeLba(parameter[1], parameter[2]);
			expectedData[LBA] = data;
		}
		else if (parameter[0] == "R") {
			LBA = stoi(parameter[1]);
			int resultData;
			bReturn = ssdAdapter->readLba(parameter[1], resultData);
			if (bReturn == true && resultData != expectedData[LBA]) {
				//cout << resultData << " : " << expectedData[LBA] << endl;
				return FAIL;
			}
		}
		else if (parameter[0] == "E") {
			LBA = stoi(parameter[1]);
			size = stoi(parameter[2]);
			bReturn = ssdAdapter->erase(parameter[1], parameter[2]);
			for (int i = LBA; i < LBA + size; i++) {
				expectedData[i] = 0;
			}
		}
		else {
			return FAIL;
		}

		if (bReturn == false) return FAIL;
	}

	return PASS;
}

void TestScript::makeScenario(void)
{
#if 0	// example 1
	const std::string scenarioPath = testDir + "\\1_FullWriteAndReadCompare.txt";
	bool bExist = FILE_MANAGER.fileExists(scenarioPath);
	if (bExist == true)
	{
		FILE_MANAGER.removeFileIfExists(scenarioPath);
	}
	else
	{
		FILE_MANAGER.createFile(scenarioPath);
	}

	const int MAX_LBA = 100;
	const int TEST_UNIT = 5;
	int lba = 0;
	int expectedData = 0xABCD1234;
	int actualData = 0;

	while (lba < MAX_LBA)
	{
		for (auto it = 0; it < TEST_UNIT; it++)
		{
			//ssdAdapter->writeLba(lba + it, expectedData);
			std::string argument = _writeCommand(lba + it, expectedData);
			FILE_MANAGER.appendLine(scenarioPath, argument);
		}

		for (auto it = 0; it < TEST_UNIT; it++)
		{
			//actualData = ssdAdapter->readLba(lba + it);
			std::string argument = _readCommand(lba + it);
			FILE_MANAGER.appendLine(scenarioPath, argument);
		}

		lba += TEST_UNIT;
	}
#endif

#if 0	// example 2
	const std::string scenarioPath = testDir + "\\2_PartialLBAWrite.txt";
	bool bExist = FILE_MANAGER.fileExists(scenarioPath);
	if (bExist == true)
	{
		FILE_MANAGER.removeFileIfExists(scenarioPath);
	}
	else
	{
		FILE_MANAGER.createFile(scenarioPath);
	}

	const int MAX_LBA = 100;
	const int TEST_UNIT = 5;

	const int loop = 30;
	const int expectedData = 0xABCD1234;
	for (int it = 0; it < loop; it++)
	{
		ssdAdapter->writeLba(4, expectedData);
		ssdAdapter->writeLba(0, expectedData);
		ssdAdapter->writeLba(3, expectedData);
		ssdAdapter->writeLba(1, expectedData);
		ssdAdapter->writeLba(2, expectedData);

		std::string argument = _writeCommand(4, expectedData);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _writeCommand(0, expectedData);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _writeCommand(3, expectedData);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _writeCommand(1, expectedData);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _writeCommand(2, expectedData);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		for (int lba = 0; lba < 5; lba++)
		{
			int actualData = ssdAdapter->readLba(lba);

			argument = _readCommand(lba);
			FILE_MANAGER.appendLine(scenarioPath, argument);
		}
	}
#endif

#if 0	// example 3
	const std::string scenarioPath = testDir + "\\3_WriteReadAging.txt";
	bool bExist = FILE_MANAGER.fileExists(scenarioPath);
	if (bExist == true)
	{
		FILE_MANAGER.removeFileIfExists(scenarioPath);
	}
	else
	{
		FILE_MANAGER.createFile(scenarioPath);
	}

	std::srand(std::time(nullptr)); // 시드 초기화 (현재 시간 기준)

	const int loop = 30;
	const int lba1 = 0;
	const int lba2 = 99;
	int expectedData1 = 0x0;
	int expectedData2 = 0x0;
	for (int it = 0; it < loop; it++)
	{
		expectedData1 = std::rand() % 0xFFFFFFFF;
		expectedData2 = std::rand() % 0xFFFFFFFF;

		std::string argument = _writeCommand(lba1, expectedData1);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _writeCommand(lba2, expectedData2);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _readCommand(lba1);
		FILE_MANAGER.appendLine(scenarioPath, argument);

		argument = _readCommand(lba2);
		FILE_MANAGER.appendLine(scenarioPath, argument);

	}
#endif

#if 1	// example 4
	const std::string scenarioPath = testDir + "\\4_EraseAndWriteAging.txt";
	bool bExist = FILE_MANAGER.fileExists(scenarioPath);
	if (bExist == true)
	{
		FILE_MANAGER.removeFileIfExists(scenarioPath);
	}
	else
	{
		FILE_MANAGER.createFile(scenarioPath);
	}

	const int MAX_LBA = 100;
	const int loop = 30;
	const int lba1 = 0;
	const int lba2 = 99;
	int expectedData1 = 0x1234ABCD;
	int expectedData2 = 0xABCD1234;

	std::srand(std::time(nullptr)); // 시드 초기화 (현재 시간 기준)

	std::string argument = _eraseCommand(0, 3);
	FILE_MANAGER.appendLine(scenarioPath, argument);


	for (int it = 0; it < loop; it++)
	{
		for (int lba = 2; lba < MAX_LBA;)
		{
			argument = _writeCommand(lba, expectedData1);
			FILE_MANAGER.appendLine(scenarioPath, argument);

			argument = _writeCommand(lba, expectedData2);
			FILE_MANAGER.appendLine(scenarioPath, argument);

			argument = _eraseCommand(lba, 3);
			FILE_MANAGER.appendLine(scenarioPath, argument);

			lba += 2;
		}
	}
#endif
}

std::string TestScript::_writeCommand(const int lba, const int data)
{
	std::stringstream ss;
	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << data;
	std::string argument = "W " + std::to_string(lba) + " " + ss.str();
	return argument;
}

std::string TestScript::_readCommand(const int lba)
{
	std::string argument = "R " + std::to_string(lba);

	return argument;
}

std::string TestScript::_eraseCommand(const int lba, const int size)
{
	std::string argument = "E " + std::to_string(lba) + " " + std::to_string(size);

	return argument;
}


int TestScript::fullWriteAndReadCompare(const int data) {

	/*
	• 0 ~4번 LBA까지 Write 명령어를 수행한다.
	• 0 ~4번 LBA까지 ReadCompare 수행
	• 5 ~9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	• 5 ~9번 LBA까지 ReadCompare 수행
	• 10 ~14번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	• 10 ~14번 LBA까지 ReadCompare 수행
	• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare를 수행한다.
	*/

	const int MAX_LBA = 100;
	const int TEST_UNIT = 5;
	int lba = 0;
	int expectedData = data;
	int actualData = 0;

	while (lba < MAX_LBA)
	{
		for (auto it = 0; it < TEST_UNIT; it++)
		{
			ssdAdapter->writeLba(lba + it, expectedData);
		}

		for (auto it = 0; it < TEST_UNIT; it++)
		{
			actualData = ssdAdapter->readLba(lba + it);
			if (actualData != expectedData)
			{
				return FAIL;
			}
		}

		lba += TEST_UNIT;
	}

	return PASS;
}

int TestScript::partialLBAWrite(const int data) {
	/*
	* Loop는 30회
	• 4번 LBA에 값을 적는다.
	• 0번 LBA에 같은 값을 적는다.
	• 3번 LBA에 같은 값을 적는다.
	• 1번 LBA에 같은 값을 적는다.
	• 2번 LBA에 같은 값을 적는다.
	• LBA 0 ~ 4번, ReadCompare
	*/
	const int loop = 30;
	const int expectedData = data;
	for (int it = 0; it < loop; it++)
	{
		ssdAdapter->writeLba(4, expectedData);
		ssdAdapter->writeLba(0, expectedData);
		ssdAdapter->writeLba(3, expectedData);
		ssdAdapter->writeLba(1, expectedData);
		ssdAdapter->writeLba(2, expectedData);

		for (int lba = 0; lba < 5; lba++)
		{
			int actualData = ssdAdapter->readLba(lba);
			if (actualData != expectedData)
			{
				return FAIL;
			}
		}
	}

	return PASS;
}

int TestScript::writeReadAging(void) {

	/*
	* Loop 200회
	• 0번 LBA에 랜덤 값을 적는다.
	• 99번 LBA에 랜덤 값을 적는다.
	• LBA 0번과 99번, ReadCompare를 수행
	*/

	std::srand(std::time(nullptr)); // 시드 초기화 (현재 시간 기준)

	const int loop = 30;
	const int lba1 = 0;
	const int lba2 = 99;
	int expectedData1 = 0x0;
	int expectedData2 = 0x0;
	for (int it = 0; it < loop; it++)
	{
		expectedData1 = std::rand() % 0xFFFFFFFF;
		expectedData2 = std::rand() % 0xFFFFFFFF;

		ssdAdapter->writeLba(lba1, expectedData1);
		ssdAdapter->writeLba(lba2, expectedData2);

		int actualData = ssdAdapter->readLba(lba1);
		if (actualData != expectedData1)
		{
			return FAIL;
		}

		actualData = ssdAdapter->readLba(lba2);
		if (actualData != expectedData2)
		{
			return FAIL;
		}
	}

	return PASS;
}

int TestScript::eraseAndWriteAging(void)
{
	/*
	* 0 ~ 2번 LBA 삭제
	• Loop 30회
	• 2번 LBA Write
	• 2번 LBA OverWrite
	• 2 ~ 4번 LBA 삭제
	• 4번 LBA Write
	• 4번 LBA OverWrite
	• 4 ~ 6번 LBA 삭제
	• 6번 LBA Write
	• 6번 LBA OverWrite
	• 6 ~ 8번 LBA 삭제
	*/

	const int MAX_LBA = 100;
	const int loop = 30;
	const int lba1 = 0;
	const int lba2 = 99;
	int expectedData1 = 0x1234ABCD;
	int expectedData2 = 0xABCD1234;

	std::srand(std::time(nullptr)); // 시드 초기화 (현재 시간 기준)

	ssdAdapter->erase(0, 3);

	for (int it = 0; it < loop; it++)
	{
		for (int lba = 2; lba < MAX_LBA;)
		{
			ssdAdapter->writeLba(lba, expectedData1);
			ssdAdapter->writeLba(lba, expectedData2);
			ssdAdapter->erase(lba, 3);

			lba += 2;
		}
	}

	return PASS;
}
