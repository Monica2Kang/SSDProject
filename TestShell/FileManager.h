#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <direct.h>
#include <thread>
#include <sys/stat.h>
#include <cstring>

#define NOMINMAX 
#include <windows.h>

#define FILE_MANAGER FileManager::getInstance()

class FileManager {
public:
    static FileManager& getInstance() {
        static FileManager instance;
        return instance;
    }

    bool fileExists(const std::string& path) {
        std::ifstream ifs(path);
        return ifs.is_open();
    }

    bool createFile(const std::string& path) {
        std::ofstream ofs(path);
        bool success = ofs.good();
        ofs.close();
        return success;
    }

    bool removeFileIfExists(const std::string& path) {
        std::ifstream ifs(path);
        if (!ifs.is_open()) return false;
        ifs.close();

        return std::remove(path.c_str()) == 0;
    }

    std::string readLine(const std::string& path, int lineNumber) {
        std::ifstream ifs(path);
        if (!ifs.is_open()) return "";

        std::string line;
        int current = 1;
        while (std::getline(ifs, line)) {
            if (current++ == lineNumber) return line;
        }

        return "";
    }

    bool writeLine(const std::string& path, int lineNumber, const std::string& newLine) {
        std::ifstream ifs(path);
        if (!ifs.is_open()) return false;

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(ifs, line)) lines.push_back(line);
        ifs.close();

        if (lineNumber > static_cast<int>(lines.size()))
            lines.resize(lineNumber, "");

        lines[lineNumber - 1] = newLine;

        std::ofstream ofs(path);
        if (!ofs.is_open()) return false;
        for (const auto& line : lines) ofs << line << '\n';

        return true;
    }

    bool appendLine(const std::string& path, const std::string& line) {
        std::ofstream ofs(path, std::ios::app);
        if (!ofs.is_open()) return false;
        ofs << line << '\n';
        return true;
    }

    std::size_t getFileSize(const std::string& filename) {
        std::ifstream in(filename, std::ios::binary | std::ios::ate);
        if (!in.is_open()) return 0;
        return static_cast<std::size_t>(in.tellg());
    }

    bool checkSizeAndRotate(const std::string& logFilePath, const std::string& rotatedPath, int maxSize) {
        if (logFilePath.empty() || getFileSize(logFilePath) < maxSize)
            return false;

        std::rename(logFilePath.c_str(), rotatedPath.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::ofstream(logFilePath).close();

        return true;
    }

    void renameFile(const std::string& srcFilePath, const std::string& dstFilePath) {
        std::rename(srcFilePath.c_str(), dstFilePath.c_str());
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    bool directoryExists(const std::string& path) {
        struct _stat info;
        if (_stat(path.c_str(), &info) != 0) return false;
        return (info.st_mode & _S_IFDIR) != 0;
    }

    bool createDirectory(const std::string path) {
        if (directoryExists(path)) {
            return true; // 이미 존재하므로 생성하지 않음
        }

        return _mkdir(path.c_str()) == 0;
    }

    std::string findFileWithPrefix(const std::string& directory, const std::string& targetPrefix) {
        WIN32_FIND_DATAA findFileData;
        HANDLE hFind;

        std::string searchPath = directory;
        if (!searchPath.empty() && searchPath.back() != '\\') {
            searchPath += "\\";
        }
        searchPath += "*"; // 모든 파일 검색

        hFind = FindFirstFileA(searchPath.c_str(), &findFileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return ""; // 디렉토리 못 찾음
        }

        do {
            std::string filename = findFileData.cFileName;

            // "." 과 ".."는 무시
            if (filename == "." || filename == "..") {
                continue;
            }

            // prefix 비교
            if (filename.compare(0, targetPrefix.size(), targetPrefix) == 0) {
                // regular file인지 확인
                if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    FindClose(hFind);
                    return directory + "\\" + filename;
                }
            }

        } while (FindNextFileA(hFind, &findFileData) != 0);

        FindClose(hFind);
        return "";
    }

private:

    FileManager() = default;
    ~FileManager() = default;
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
};