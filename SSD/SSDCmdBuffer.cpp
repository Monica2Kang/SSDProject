//#include <sys/stat.h>
//#include <sys/types.h>
#include "SSDCmdBuffer.h"

SSDCmdBuffer::SSDCmdBuffer() {
    // file open with overwrite mode
    outfile.open(fileEmptyName, std::ios::out);
    if (!outfile.is_open()) {
        throw std::runtime_error("Cannot open the Log file");
    }
}

SSDCmdBuffer::~SSDCmdBuffer() {
    if (outfile.is_open()) {
        outfile.close();
    }
}
