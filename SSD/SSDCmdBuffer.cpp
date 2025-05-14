#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "SSDCmdBuffer.h"

//enum class CommandType { WRITE, ERASE };

void SSDCmdBuffer::writeData(const int lba, const unsigned int data) {
    if (_checkLbaOutOfRange(lba)) throw std::invalid_argument("Out of LBA Range.");
    _removeOverwrittenWrites(lba);
    cmdBuffer.push_back({ CommandType::WRITE, lba, data });
    _optimize();
}

void SSDCmdBuffer::eraseData(const int lba, const int range) {
    if (_checkEraseLbaRangeInvalid(lba, range)) throw std::invalid_argument("Out of Erase LBA Range.");
    cmdBuffer.push_back({ CommandType::ERASE, lba, static_cast<unsigned int>(range) });
    _removeWritesCoveredByErase(lba, range);
    _optimize();
}

const std::vector<Command>& SSDCmdBuffer::getBuffer(void) const {
    return cmdBuffer;
}

void SSDCmdBuffer::clear(void) {
    cmdBuffer.clear();
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

void SSDCmdBuffer::_printBuffer(void) const {
    std::cout << "Current buffer:\n";
    for (const auto& cmd : cmdBuffer) {
        std::cout << cmd.toString() << "\n";
    }
    std::cout << std::endl;
}

void SSDCmdBuffer::_clearBufferIfNeeded(void) {
    if (cmdBuffer.size() > MAX_CMDBUF_SIZE) {
        cmdBuffer.clear();
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

// split erase if it overlaps with write
void SSDCmdBuffer::_resolveEraseWriteConflicts() {
    std::vector<Command> resolved;
    for (const auto& cmd : cmdBuffer) {
        if (cmd.type == CommandType::ERASE) {
            bool splitNeeded = false;
            for (const auto& write : cmdBuffer) {
                if (write.type != CommandType::WRITE) continue;
                int eraseStart = cmd.lba;
                int eraseEnd = cmd.lba + cmd.dataOrRange - 1;
                if (write.lba >= eraseStart && write.lba <= eraseEnd) {
                    // split erase
                    int part1Len = write.lba - eraseStart;
                    int part2Len = eraseEnd - write.lba;

                    if (part1Len > 0)
                        resolved.push_back({ CommandType::ERASE, eraseStart, static_cast<unsigned int>(part1Len) });
                    if (part2Len > 0)
                        resolved.push_back({ CommandType::ERASE, write.lba + 1, static_cast<unsigned int>(part2Len) });
                    splitNeeded = true;
                    break;
                }
            }
            if (!splitNeeded) {
                resolved.push_back(cmd);
            }
        }
        else {
            resolved.push_back(cmd);
        }
    }
    cmdBuffer = resolved;
}
