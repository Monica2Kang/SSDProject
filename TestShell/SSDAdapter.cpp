#include "SSDAdapter.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

void SSDAdapter::writeLba(const int lba, const int data)
{
	std::stringstream ss;

	ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << data;
	std::string argument = "W " + std::to_string(lba) + " " + ss.str();

	_executeSSDCommand(argument);
}

bool SSDAdapter::writeLba(std::string lba, std::string data)
{
	std::stringstream ss;

	std::string argument = "W " + lba + " " + data;

	_executeSSDCommand(argument);

	return _checkCmdExecutionResultFromSSDOutputFile();
}

int SSDAdapter::readLba(const int lba)
{
	std::string argument = "R " + std::to_string(lba);

	_executeSSDCommand(argument);

	return _readDataFromSSDOutputFile();
}

bool SSDAdapter::readLba(std::string lba, int& readData)
{
	std::string argument = "R " + lba;

	_executeSSDCommand(argument);

	bool bReturn = _checkReadCmdExecutionResultFromSSDOutputFile();

	if (bReturn == true)
	{
		readData = _readDataFromSSDOutputFile();
	}

	return bReturn;
}

void SSDAdapter::erase(const int lba, const int size)
{
	std::string argument = "E " + std::to_string(lba) + " " + std::to_string(size);

	_executeSSDCommand(argument);
}

bool SSDAdapter::erase(std::string lba, std::string size)
{
	std::string argument = "E " + lba + " " + size;

	_executeSSDCommand(argument);

	return _checkCmdExecutionResultFromSSDOutputFile();
}

void SSDAdapter::fullWrite(const int data)
{
	const int MAX_LBA = 100;
	for (auto lba = 0; lba < MAX_LBA; lba++)
	{
		writeLba(lba, data);
	}
}

void SSDAdapter::fullRead(void)
{
	const int MAX_LBA = 100;
	int readData = 0;
	std::stringstream ss;
	for (auto lba = 0; lba < MAX_LBA; lba++)
	{
		readData = readLba(lba);

		std::stringstream ss;
		ss << "LBA : "
			<< lba
			<< " Data : "
			<< "0x"
			<< std::uppercase       // A~F 대문자로 출력
			<< std::hex
			<< std::setfill('0')    // 0으로 채움
			<< std::setw(8)         // 8자리 고정 (4바이트)
			<< readData;

		std::cout << ss.str() << std::endl;;
	}
}

void SSDAdapter::flush(void) {
	std::string argument = "F";

	_executeSSDCommand(argument);
}

void SSDAdapter::_executeSSDCommand(const std::string argument)
{
	std::string exePath = "..\\x64\\Release\\SSD.exe";
	std::string command = "\"" + exePath + "\" " + argument;


	std::ifstream ssdFile(exePath);
	if (ssdFile.good() == false) {
		std::string exePath2 = ".\\SSD.exe";
		std::ifstream ssdFile2(exePath2);
		if (ssdFile2.good() == false)
		{
			throw std::runtime_error("SSD Execution File Error - File not found.");
		}
		else
		{
			command = "\"" + exePath2 + "\" " + argument;
		}
	}

	int result = std::system(command.c_str());
	if (result == 1)
	{
		throw std::runtime_error("SSD Execution File Error - File return error.");
	}
}

int SSDAdapter::_readDataFromSSDOutputFile(void)
{
	std::string line = _readResultFromSSDOutputFile();
	int readData = std::stoul(line, nullptr, 16);


	return readData;
}

string SSDAdapter::_readResultFromSSDOutputFile(void)
{
	std::string filePath = "ssd_output.txt";

	std::ifstream file(filePath);
	if (!file.is_open()) {
		throw std::runtime_error("SSD Output File Error - File not found.");
	}

	std::string line;
	if (!std::getline(file, line)) {
		throw std::runtime_error("SSD Output File Error - File is empty.");
	}

	// 공백 제거 (필요 시)
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

	// ERROR 문자열인 경우
	if (line == "ERROR") {
		throw std::runtime_error("SSD Output Error - Error result.");
	}

	return line;
}

bool SSDAdapter::_checkReadCmdExecutionResultFromSSDOutputFile(void)
{
	std::string filePath = "ssd_output.txt";

	std::ifstream file(filePath);
	if (!file.is_open()) {
		throw std::runtime_error("SSD Output File Error - File not found.");
	}

	std::string line;
	if (!std::getline(file, line)) {
		throw std::runtime_error("SSD Output File Error - File is empty.");
	}

	// 공백 제거 (필요 시)
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

	// ERROR 문자열인 경우
	if (line == "ERROR") {
		return false;
	}
	else
	{
		return true;
	}
}

bool SSDAdapter::_checkCmdExecutionResultFromSSDOutputFile(void)
{
	std::string filePath = "ssd_output.txt";

	std::ifstream file(filePath);
	if (!file.is_open()) {
		return true;
	}

	std::string line;
	if (!std::getline(file, line)) {
		return true;
	}

	// 공백 제거 (필요 시)
	line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());

	// ERROR 문자열인 경우
	if (line == "ERROR") {
		return false;
	}
	else
	{
		return true;
	}
}