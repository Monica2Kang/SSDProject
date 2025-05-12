#include "SSDAdapter.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;

void SSDAdapter::wirteLba(const int lba, const int data)
{
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::hex << data;
    std::string argument = "W " + std::to_string(lba) + " " + ss.str();

    _ExecuteSSDCommand(argument);
}

int SSDAdapter::readLba(const int lba)
{
	return 0;
}

void SSDAdapter::fullWrite(const int data)
{
    const int MAX_LBA = 100;
    for (auto lba = 0; lba < MAX_LBA; lba++)
    {
        wirteLba(lba, data);
    }
}

void SSDAdapter::fullRead(void)
{
}

void SSDAdapter::_ExecuteSSDCommand(std::string argument)
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