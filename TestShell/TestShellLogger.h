#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <thread>
#include "FileManager.h"

#define TEST_SHELL_LOG(...) Logger::getInstance().log(__FUNCTION__, __VA_ARGS__)

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void Initialize(void) {
        bool bexist = FILE_MANAGER.fileExists(logFilePath);
        if (bexist == true)
        {
            FILE_MANAGER.removeFileIfExists(logFilePath);
        }

        FILE_MANAGER.createFile(logFilePath);
    }

    void log(const std::string& functionName, const std::string& message) {

        // Logging
        saveLog(functionName, message);

        // Check size and rotate log file
        checkSizeAndRotateLogFile();
    }

    void saveLog(const std::string& functionName, const std::string& message)
    {
        if (bInitialize = false)
        {
            Initialize();
            bInitialize = true;
        }

        std::ostringstream oss;
        oss << getCurrentTime() << " "
            << std::left << std::setw(NAME_SIZE) << (functionName + "()")
            << " : " << message;

        std::string logLine = oss.str();
        FILE_MANAGER.appendLine(logFilePath, logLine);
    }

    void checkSizeAndRotateLogFile()
    {
        // check size and rotate
        std::string rotatedPath = generateRotatedFileName();
        bool bRotate = FILE_MANAGER.checkSizeAndRotate(logFilePath, rotatedPath, MAX_SIZE);
        if (bRotate == true)
        {
            if (previousFilePath.empty() == false)
            {
                std::string zipPath = generateZipFileName(previousFilePath);
                FILE_MANAGER.renameFile(previousFilePath, zipPath);
            }

            previousFilePath = rotatedPath;
        }
    }

    void log(const std::string& functionName, int v1) {
        log(functionName, std::to_string(v1));
    }

    void log(const std::string& functionName, int v1, int v2) {
        log(functionName, std::to_string(v1) + ", " + std::to_string(v2));
    }

    void log(const std::string& functionName, int v1, int v2, int v3) {
        log(functionName, std::to_string(v1) + ", " + std::to_string(v2) + ", " + std::to_string(v3));
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;

    std::string logFilePath = "latest.log";
    std::string previousFilePath = "";

    const int MAX_SIZE = 10 * 1024; // 10KB
    const int NAME_SIZE = 30;
    bool bInitialize = false;

    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};

        localtime_s(&tm, &time_t);

        std::ostringstream oss;
        oss << "[" << std::setfill('0') << std::setw(2) << (tm.tm_year % 100) << "."
            << std::setw(2) << (tm.tm_mon + 1) << "."
            << std::setw(2) << tm.tm_mday << " "
            << std::setw(2) << tm.tm_hour << ":"
            << std::setw(2) << tm.tm_min << "]";
        return oss.str();
    }

    std::string generateRotatedFileName() {
        std::time_t t = std::time(nullptr);
        std::tm tm;

        localtime_s(&tm, &t);

        std::ostringstream oss;
        oss << "until_"
            << std::setfill('0') << std::setw(2) << (tm.tm_year % 100)
            << std::setw(2) << (tm.tm_mon + 1)
            << std::setw(2) << tm.tm_mday << "_"
            << tm.tm_hour << "h_"
            << tm.tm_min << "m_"
            << tm.tm_sec << "s.log";
        return oss.str();
    }

    std::string generateZipFileName(const std::string path) {
        size_t dotPos = path.find_last_of('.');
        if (dotPos == std::string::npos) return path + ".zip";
        return path.substr(0, dotPos) + ".zip";
    }
};