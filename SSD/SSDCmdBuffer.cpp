#ifdef _WIN32
#include <direct.h>
#define mkdir(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif
#include "SSDCmdBuffer.h"

SSDCmdBuffer::SSDCmdBuffer() {
    // file open with overwrite mode
    if (mkdir(folderName) != 0) {
        throw std::runtime_error("Cannot create the folder");
    }

    for (int index = 0; index < MAX_FILE_COUNT; index++) {
        std::string fileName = std::string(folderName) + "/" + std::to_string(index) + "_" + fileEmptyName;
        outfile.open(fileName, std::ios::out);
        if (!outfile.is_open()) {
            throw std::runtime_error("Cannot open the buffer txt file");
        }
    }
}

SSDCmdBuffer::~SSDCmdBuffer() {
    for (int index = 0; index < MAX_FILE_COUNT; index++) {
        std::string fileName = std::string(folderName) + "/" + std::to_string(index) + "_" + fileEmptyName;        
        if (outfile.is_open()) {
            outfile.close();
        }
    }
}
