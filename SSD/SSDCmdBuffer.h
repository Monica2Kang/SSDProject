#pragma once
#include "SSDDevice.h"

enum class CommandType { WRITE, ERASE };

struct Command {
    CommandType type;
    int lba;
    int dataOrRange; // data (for Write), range (for Erase)

    std::string toString() const {
        if (type == CommandType::WRITE)
            return "Write " + std::to_string(lba) + " " + std::to_string(dataOrRange);
        else
            return "Erase " + std::to_string(lba) + " " + std::to_string(dataOrRange);
    }
};

class SSDCmdBuffer {
public:
    void addWrite(const int lba, const int data);
    void addErase(const int lba, const int range);
    void clear(void);
    const std::vector<Command>& getBuffer(void) const;

private:
    bool _checkEraseLbaRangeInvalid(const int lba, const int range) const;
    bool _checkLbaOutOfRange(const int lba) const;
    void _optimize(void);
    void _printBuffer(void) const;
    void _clearBufferIfNeeded(void);
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