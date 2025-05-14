#pragma once
#include "SSDDevice.h"
#include "SSDCmdBufferOutput.h"

enum class CommandType { WRITE, ERASE };

struct Command {
    CommandType type;
    int lba;
    unsigned int dataOrRange; // data (for Write), range (for Erase)

    std::string toString() const {
        if (type == CommandType::WRITE)
            return "Write " + std::to_string(lba) + " " + std::to_string(dataOrRange);
        else
            return "Erase " + std::to_string(lba) + " " + std::to_string(dataOrRange);
    }
};

class SSDCmdBuffer {
public:
    SSDCmdBufferOutput output;
public:
    void writeData(const int lba, const unsigned int data);
    void eraseData(const int lba, const int range);
    void flushData(void);
    const std::vector<Command>& getBuffer(void) const;

private:
    bool _checkEraseLbaRangeInvalid(const int lba, const int range) const;
    bool _checkLbaOutOfRange(const int lba) const;
    void _optimize(void);
    void _exportBufferToFiles(void);
    void _clearBufferIfNeeded(void);
    void _removeOverwrittenWrites(const int lba);
    void _removeWritesCoveredByErase(const int startLba, const int endLba);
    // merge erase commands with adjacent or overlapping lba ranges
    void _mergeErases(void);
    // split erase if it overlaps with write
    void _resolveEraseWriteConflicts(void);

private:
    static const int LBA_CAPACITY = 100;
    static const int LBA_UPPER_LIMIT = LBA_CAPACITY - 1;
    static const int LBA_LOWER_LIMIT = 0;
    static const int MAX_ERASE_RANGE = 10;
    static const int MIN_ERASE_RANGE = 0;

    static const int MAX_CMDBUF_SIZE = 5;
    std::vector<Command> cmdBuffer;
};