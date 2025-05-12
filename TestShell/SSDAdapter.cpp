#include "SSDAdapter.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <iomanip>

using namespace std;

void SSDAdapter::writeLba(const int lba, const int data)
{
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::hex << data;
    std::string argument = "W " + std::to_string(lba) + " " + ss.str();

    _ExecuteSSDCommand(argument);

    _CheckExecutionResultFromSSDOutputFile();
}

int SSDAdapter::readLba(const int lba)
{
	return _ReadDataFromSSDOutputFile();
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

void SSDAdapter::_ExecuteSSDCommand(const std::string argument)
{
    std::string exePath = "..\\x64\\Release\\SSD.exe";
    std::string command = exePath + " " + argument;

    
    std::ifstream file(exePath);
    if (file.good() == false) {
        throw std::runtime_error("SSD Execution File Error - File not found.");

    }

    int result = system(command.c_str());
    if (result == 1)
    {
        throw std::runtime_error("SSD Execution File Error - File return error.");
    }
}

int SSDAdapter::_ReadDataFromSSDOutputFile(void)
{
    std::string line = _CheckExecutionResultFromSSDOutputFile();
    int readData = std::stoul(line, nullptr, 16);
   

    return readData;
}

string SSDAdapter::_CheckExecutionResultFromSSDOutputFile(void)
{
    std::string filePath = "..\\SSD\\ssd_output.txt";

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