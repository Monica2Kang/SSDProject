#include "SSDAdapter.h"
#include <stdexcept>
#include "TestShellLogger.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

#define OP_SUPPORT_PERF_ENHANCED (1)

using namespace std;

namespace {
    constexpr const char* SSD_EXE_PATH_1 = "..\\x64\\Release\\SSD.exe";
    constexpr const char* SSD_EXE_PATH_2 = ".\\SSD.exe";
    constexpr const char* SSD_OUTPUT_FILE = "ssd_output.txt";
    constexpr int MAX_LBA = 100;

    std::string toHexString(int value) {
        std::stringstream ss;
        ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << value;
        return ss.str();
    }

    bool readValidLineFromFile(const std::string& filePath, std::string& outputLine) {
        std::ifstream file(filePath);
        if (!file.is_open()) return false;

        if (!std::getline(file, outputLine)) return false;

        outputLine.erase(remove_if(outputLine.begin(), outputLine.end(), ::isspace), outputLine.end());
        if (outputLine == "ERROR") return false;

        return true;
    }
}

void SSDAdapter::writeLba(const int lba, const int data) {
    const std::string argument = "W " + std::to_string(lba) + " " + toHexString(data);
    _executeSSDCommand(argument);
}

bool SSDAdapter::writeLba(std::string lba, std::string data) {
    const std::string argument = "W " + lba + " " + data;
    _executeSSDCommand(argument);
    return _checkCmdExecutionResultFromSSDOutputFile();
}

int SSDAdapter::readLba(const int lba) {
    const std::string argument = "R " + std::to_string(lba);
    _executeSSDCommand(argument);
    return _readDataFromSSDOutputFile();
}

bool SSDAdapter::readLba(std::string lba, int& readData) {
    const std::string argument = "R " + lba;
    _executeSSDCommand(argument);
    if (!_checkReadCmdExecutionResultFromSSDOutputFile()) return false;
    readData = _readDataFromSSDOutputFile();
    return true;
}

void SSDAdapter::erase(const int lba, const int size) {
    const std::string argument = "E " + std::to_string(lba) + " " + std::to_string(size);
    _executeSSDCommand(argument);
}

bool SSDAdapter::erase(std::string lba, std::string size) {
    const std::string argument = "E " + lba + " " + size;
    _executeSSDCommand(argument);
    return _checkCmdExecutionResultFromSSDOutputFile();
}

void SSDAdapter::fullWrite(const int data) {
    for (int lba = 0; lba < MAX_LBA; ++lba) {
        writeLba(lba, data);
    }
}

void SSDAdapter::fullRead() {
    for (int lba = 0; lba < MAX_LBA; ++lba) {
        int data = readLba(lba);
        std::cout << "LBA : " << lba << " Data : " << toHexString(data) << std::endl;
    }
}

void SSDAdapter::flush() {
    _executeSSDCommand("F");
}

#if (OP_SUPPORT_PERF_ENHANCED == 0)
void SSDAdapter::_executeSSDCommand(const std::string argument) {
    TEST_SHELL_LOG(argument);
    std::string command = std::string("\"") + SSD_EXE_PATH_1 + "\" " + argument;

    std::ifstream ssdFile(SSD_EXE_PATH_1);
    if (!ssdFile.good()) {
        std::ifstream ssdFile2(SSD_EXE_PATH_2);
        if (!ssdFile2.good()) {
            throw std::runtime_error("SSD Execution File Error - File not found.");
        }
        command = std::string("\"") + SSD_EXE_PATH_2 + "\" " + argument;
    }

    if (std::system(command.c_str()) == 1) {
        throw std::runtime_error("SSD Execution File Error - File return error.");
    }
}
#else
void SSDAdapter::_executeSSDCommand(const std::string argument) {
    const std::string fullCommand = std::string("\"") + SSD_EXE_PATH_1 + "\" " + argument;

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    std::unique_ptr<char[]> commandBuffer(new char[fullCommand.size() + 1]);
    strcpy_s(commandBuffer.get(), fullCommand.size() + 1, fullCommand.c_str());

    if (!CreateProcessA(nullptr, commandBuffer.get(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        throw std::runtime_error("CreateProcess failed with error: " + std::to_string(GetLastError()));
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
#endif

int SSDAdapter::_readDataFromSSDOutputFile() {
    std::string line;
    if (!readValidLineFromFile(SSD_OUTPUT_FILE, line)) {
        throw std::runtime_error("SSD Output Error - No valid data.");
    }
    return std::stoul(line, nullptr, 16);
}

bool SSDAdapter::_checkReadCmdExecutionResultFromSSDOutputFile() {
    std::string line;
    return readValidLineFromFile(SSD_OUTPUT_FILE, line);
}

bool SSDAdapter::_checkCmdExecutionResultFromSSDOutputFile() {
    std::string line;
    return readValidLineFromFile(SSD_OUTPUT_FILE, line);
}