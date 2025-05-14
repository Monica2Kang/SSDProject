#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include "SSDCmdBuffer.h"
#include "SSDDevice.h"
#include "SSDCmdBufferOutput.h"

unsigned int SSDCmdBuffer::readData(const int lba)
{
    if (_checkLbaOutOfRange(lba)) {
        throw std::invalid_argument("Out of LBA Range.");
    }

    // 최신 명령부터 확인
    for (auto it = cmdBuffer.rbegin(); it != cmdBuffer.rend(); ++it) {
        const Command& cmd = *it;

        bool retFlag;
        unsigned int retVal = _getBufferedDataIfHit(cmd, lba, retFlag);
        if (retFlag) return retVal;
    }

    // cmdBuffer에 명령이 없으면 SSD 디바이스에서 직접 읽기
    return SSD_DEVICE.readData(lba);
}

unsigned int SSDCmdBuffer::_getBufferedDataIfHit(const Command& cmd, const int lba, bool& retFlag) {
    retFlag = true;
    if (_isCmdWrite(cmd) && _isLbaTheSame(cmd, lba)) {
        return cmd.dataOrRange;
    }

    if (_isCmdErase(cmd)) {
        int eraseStart = cmd.lba;
        int eraseEnd = eraseStart + static_cast<int>(cmd.dataOrRange) - 1;
        if (lba >= eraseStart && lba <= eraseEnd) {
            return 0;
        }
    }

    retFlag = false;
    return {};
}

bool SSDCmdBuffer::_isLbaTheSame(const Command& cmd, const int lba) const {
    return cmd.lba == lba;
}

bool SSDCmdBuffer::_isCmdWrite(const Command& cmd) const {
    return cmd.type == CommandType::WRITE;
}

bool SSDCmdBuffer::_isCmdErase(const Command& cmd) const {
    return cmd.type == CommandType::ERASE;
}

void SSDCmdBuffer::writeData(const int lba, const unsigned int data) {
    if (_checkLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Range.");
    _removeOverwrittenWrites(lba);
    cmdBuffer.push_back({ CommandType::WRITE, lba, data });
    _optimize();
    _exportBufferToFiles();
    _saveBufferToFile();
}

void SSDCmdBuffer::eraseData(const int lba, const int range) {
    if (_checkEraseLbaRangeInvalid(lba, range)) throw std::invalid_argument("Out of Erase LBA Range.");
    cmdBuffer.push_back({ CommandType::ERASE, lba, static_cast<unsigned int>(range) });
    _removeWritesCoveredByErase(lba, range);
    _optimize();
    _exportBufferToFiles();
    _saveBufferToFile();
}

const std::vector<Command>& SSDCmdBuffer::getBuffer(void) const {
    return cmdBuffer;
}

void SSDCmdBuffer::flushData(void) {
    for (const auto& cmd : cmdBuffer) {
        switch (cmd.type) {
        case CommandType::WRITE:
            SSD_DEVICE.writeData(cmd.lba, cmd.dataOrRange);
            break;
        case CommandType::ERASE:
            SSD_DEVICE.eraseData(cmd.lba, static_cast<int>(cmd.dataOrRange));
            break;
        default:
            throw std::runtime_error("Unknown command type in buffer.");
        }
    }
    cmdBuffer.clear();
    _exportBufferToFiles();
    _saveBufferToFile();
}

bool SSDCmdBuffer::_checkEraseLbaRangeInvalid(int lba, int range) const {
    return lba < LBA_LOWER_LIMIT || lba + range - 1 > LBA_UPPER_LIMIT || range < MIN_ERASE_RANGE || range > MAX_ERASE_RANGE;
}

bool SSDCmdBuffer::_checkLbaOutOfRange(int lba) const {
    return lba < LBA_LOWER_LIMIT || lba > LBA_UPPER_LIMIT;
}

void SSDCmdBuffer::_optimize(void) {
    _mergeErases();
    //_resolveEraseWriteConflicts();
    _clearBufferIfNeeded();
}

void SSDCmdBuffer::_exportBufferToFiles(void) {
    //SSDCmdBufferOutput output;
    std::vector<std::string> fileNames;

    for (const auto& cmd : cmdBuffer) {
        std::string fileName;

        if (cmd.type == CommandType::WRITE) {
            std::ostringstream oss;
            oss << "W_" << cmd.lba << "_0x"
                << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << cmd.dataOrRange;
            fileName = oss.str();
        }
        else if (cmd.type == CommandType::ERASE) {
            fileName = "E_" + std::to_string(cmd.lba) + "_" + std::to_string(cmd.dataOrRange);
        }
        else {
            fileName = "UNKNOWN_CMD";
        }

        fileNames.emplace_back(fileName);
    }

    output.createFilesInFolder(fileNames);
}

void SSDCmdBuffer::_clearBufferIfNeeded(void) {
    if (cmdBuffer.size() > MAX_CMDBUF_SIZE) {
        flushData();
        _exportBufferToFiles();
        //_saveBufferToFile();
    }
}

void SSDCmdBuffer::_removeOverwrittenWrites(const int lba) {
    cmdBuffer.erase(std::remove_if(cmdBuffer.begin(), cmdBuffer.end(), [lba](const Command& c) {
        return c.type == CommandType::WRITE && c.lba == lba;
        }), cmdBuffer.end());
}

void SSDCmdBuffer::_removeWritesCoveredByErase(const int startLba, const int lbaRange) {
    int endLba = startLba + lbaRange - 1;
    cmdBuffer.erase(std::remove_if(cmdBuffer.begin(), cmdBuffer.end(), [startLba, endLba](const Command& c) {
        return c.type == CommandType::WRITE && c.lba >= startLba && c.lba <= endLba;
        }), cmdBuffer.end());
}

// merge erase commands with adjacent or overlapping lba ranges
void SSDCmdBuffer::_mergeErases(void) {
    std::vector<Command> erases;

    // Extract erase commands
    for (const auto& cmd : cmdBuffer) {
        if (cmd.type == CommandType::ERASE)
            erases.push_back(cmd);
    }

    std::sort(erases.begin(), erases.end(), [](const Command& a, const Command& b) {
        return a.lba < b.lba;
        });

    std::vector<Command> merged;
    for (const auto& cmd : erases) {
        if (merged.empty()) {
            merged.push_back(cmd);
        }
        else {
            Command& last = merged.back();
            int end1 = last.lba + last.dataOrRange;
            int end2 = cmd.lba + cmd.dataOrRange;

            if (cmd.lba <= end1) {
                if ((end2 - last.lba) <= MAX_ERASE_RANGE) {
                    int newStart = last.lba;
                    int newEnd = std::max(end1, end2);
                    last.lba = newStart;
                    last.dataOrRange = newEnd - newStart;
                }
                else {
                    Command newCmd;
                    newCmd.type = CommandType::ERASE;
                    newCmd.lba = end1;
                    newCmd.dataOrRange = end2 - end1;
                    merged.push_back(newCmd);
                }
            }
            else {
                merged.push_back(cmd);
            }
        }
    }

    // Remove old erases and append merged
    cmdBuffer.erase(std::remove_if(cmdBuffer.begin(), cmdBuffer.end(), [](const Command& c) {
        return c.type == CommandType::ERASE;
        }), cmdBuffer.end());

    cmdBuffer.insert(cmdBuffer.end(), merged.begin(), merged.end());
}

void SSDCmdBuffer::_saveBufferToFile() const {
    std::ofstream ofs(_bufferFileName.c_str());
    if (!ofs) return;

    for (const auto& cmd : cmdBuffer) {
        ofs << cmd.toString() << "\n";
    }
}

void SSDCmdBuffer::_loadBufferFromFile() {
    std::ifstream ifs(_bufferFileName.c_str());
    if (!ifs) return;

    std::string line;
    while (std::getline(ifs, line)) {
        if (!line.empty()) {
            cmdBuffer.push_back(Command::fromString(line));
        }
    }

    _exportBufferToFiles();  // 복원된 상태를 시각화 (파일로)
}

void SSDCmdBuffer::clearForTestOnly(void) {
    cmdBuffer.clear();
    _saveBufferToFile();
    _exportBufferToFiles();
}
