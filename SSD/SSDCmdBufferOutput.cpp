#include <windows.h>
#include <shlwapi.h>  // PathFileExists
#include <direct.h>   // _mkdir
#include <vector>
#include "SSDCmdBufferOutput.h"

using namespace std;

SSDCmdBufferOutput::SSDCmdBufferOutput() {
    // file open with overwrite mode

}

SSDCmdBufferOutput::~SSDCmdBufferOutput() {

}

bool SSDCmdBufferOutput::_folderExists(void) {
    struct stat info;
    return (stat(folderName, &info) == 0 && (info.st_mode & S_IFDIR));
}

void SSDCmdBufferOutput::_createFolder(void) {
    if (!_folderExists()) {
        if (mkdir(folderName) != 0) {
            throw runtime_error("Cannot create the buffer folder");
        }
    }
    else {
        throw runtime_error("buffer folder is already created");
    }
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

void SSDCmdBufferOutput::_createFilesInFolder(void) {
    string fileName = "";
    string filePath = string(folderName) + "\\" + fileName;
    ofstream file(filePath);
    if (file.is_open()) {
        file.close();
    }
    else {
        throw runtime_error("Cannot create file");
    }

}

