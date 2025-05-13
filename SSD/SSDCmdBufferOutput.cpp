#include <windows.h>
#include <shlwapi.h>  // PathFileExists
#include <direct.h>   // _mkdir
#include <vector>
#include "SSDCmdBufferOutput.h"

using namespace std;

SSDCmdBufferOutput::SSDCmdBufferOutput() {
    // file open with overwrite mode
    _createFolder();
    _clearFilesInFolder();
}

SSDCmdBufferOutput::~SSDCmdBufferOutput() {

}

void SSDCmdBufferOutput::createFilesInFolder(vector<string>& files) {
    _clearFilesInFolder();
    for (int index = files.size(); index < MAX_FILE_COUNT; index++) {
        files.emplace_back(fileEmptyName);
    }
    _clearFileNameForDebug();

    int fileNumber = 1;
    for (string fileName : files) {
        string filePath = string(folderName) + "\\" + to_string(fileNumber++) + "_" + fileName;
        _storeFileNameForDebug(fileName);
        ofstream file(filePath);
        if (file.is_open()) {
            file.close();
        }
        else {
            throw runtime_error("Cannot create file");
        }
    }
}

vector<string> SSDCmdBufferOutput::getFileListForDebug(void) {
    return m_fileListForDebug;
}

bool SSDCmdBufferOutput::_folderExists(void) {
    struct stat info;
    return (stat(folderName, &info) == 0 && (info.st_mode & S_IFDIR));
}

void SSDCmdBufferOutput::_createFolder(void) {
    if (!_folderExists()) {
        if (_mkdir(folderName) != 0) {
            throw runtime_error("Cannot create the buffer folder");
        }
    }
    //else {
    //    throw runtime_error("buffer folder is already created");
    //}
}

void SSDCmdBufferOutput::_clearFilesInFolder(void) {
    string searchPath = string(folderName) + "\\*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        throw runtime_error("buffer folder is already empty");
        return;
    }

    do {
        string fileName = findData.cFileName;
        if (fileName == "." || fileName == "..") continue;

        string filePath = string(folderName) + "\\" + fileName;
        if (!DeleteFileA(filePath.c_str())) {
            throw runtime_error("Cannot delete file");
        }
    } while (FindNextFileA(hFind, &findData));
    FindClose(hFind);
}

void SSDCmdBufferOutput::_storeFileNameForDebug(string fileName) {
    m_fileListForDebug.emplace_back(fileName);
}

void SSDCmdBufferOutput::_clearFileNameForDebug(void) {
    m_fileListForDebug.clear();
}
