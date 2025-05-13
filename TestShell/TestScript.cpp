#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include "TestScript.h"
#include "FileManager.h"

int TestScript::FullWriteAndReadCompare(const int data) {

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

int TestScript::PartialLBAWrite(const int data) {
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

int TestScript::WriteReadAging(void) {

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
		//expectedData1 = std::rand() % 0xFFFFFFFF;
		//expectedData2 = std::rand() % 0xFFFFFFFF;

		expectedData1 = 0xBEEFCAFE;
		expectedData2 = 0xBEEFCAFE;
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

std::vector<std::string> TestScript::splitCommand(const std::string& input) {
	std::vector<std::string> result;
	std::istringstream iss(input);
	std::string token;

	while (iss >> token) {
		result.push_back(token);
	}

	return result;
}

int TestScript::runScenario(const std::string scenarioPath)
{
	bool bExist = FILE_MANAGER.fileExists(scenarioPath);
	if (bExist == false)
	{
		return FAIL;
	}

	int line = 1;

	do {
		std::string argument = FILE_MANAGER.readLine(scenarioPath, line++);
		if (argument == "") return PASS;

		std::vector<std::string> command = splitCommand(argument);

		if (command[0].compare("W") == 0)
		{
			int lba = stoi(command[1]);
			int data = strtoul(command[2].c_str(), nullptr, 16);
			ssdAdapter->writeLba(lba, data);
		}
		else if (command[0].compare("R") == 0)
		{
			int lba = stoi(command[1]);
			ssdAdapter->readLba(lba);
		}
		else if (command[0].compare("E") == 0)
		{
			int lba = stoi(command[1]);
			int size = strtoul(command[2].c_str(), nullptr, 16);
			ssdAdapter->writeLba(lba, size);
		}
		else
		{
			return FAIL;
		}

	} while (1);
	
}

void TestScript::makeScenario(void)
{
	// 
	const std::string scenarioPath = SCENARIO_DIR + "\\1_FullWriteAndReadCompare.txt";
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
	int expectedData = 0xBEEFCAFE;
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
}

std::string TestScript::_writeCommand(const int lba, const int data)
{
	std::stringstream ss;
	ss << "0x" << std::uppercase << std::hex << data;
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