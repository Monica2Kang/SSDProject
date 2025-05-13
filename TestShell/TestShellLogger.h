#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <mutex>

#define TEST_SHELL_LOG(...) Logger::getInstance().log(__FUNCTION__, __VA_ARGS__)

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& functionName, const std::string& message) {
        std::ostringstream oss;
        oss << getCurrentTime() << " "
            << std::left << std::setw(NAME_SIZE) << (functionName + "()")
            << " : " << message;

        std::string logLine = oss.str();
        {
            std::ofstream ofs(logFilePath, std::ios::app);
            if (ofs.is_open()) {
                ofs << logLine << std::endl;
            }
        }

        rotateLogFileIfNeeded();
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
    const int MAX_SIZE = 10 * 1024; // 10KB
    const int NAME_SIZE = 30;

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

    std::size_t getFileSize(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary | std::ios::ate);
        if (!in.is_open()) return 0;
        return static_cast<std::size_t>(in.tellg());
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

    void rotateLogFileIfNeeded() {
        if (logFilePath.empty() || getFileSize(logFilePath) < MAX_SIZE)
            return;

        std::string rotatedPath = generateRotatedFileName();
        std::rename(logFilePath.c_str(), rotatedPath.c_str());
        std::ofstream(logFilePath).close(); // 💡 optionally recreate empty log file immediately
    }
};