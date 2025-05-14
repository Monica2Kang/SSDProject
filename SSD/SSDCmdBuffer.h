#pragma once
#include<sstream>
#include "SSDDevice.h"
#include "SSDCmdBufferOutput.h"

enum class CommandType { WRITE, ERASE };

struct Command {
    CommandType type;
    int lba;
    unsigned int dataOrRange; // data (for Write), range (for Erase)

    std::string toString() const {
        if (type == CommandType::WRITE)
            return "WRITE " + std::to_string(lba) + " " + std::to_string(dataOrRange);
        else
            return "ERASE " + std::to_string(lba) + " " + std::to_string(dataOrRange);
    }

    static Command fromString(const std::string& line) {
        std::istringstream iss(line);
        std::string typeStr;
        int lba;
        unsigned int dataOrRange;

        iss >> typeStr >> lba >> dataOrRange;

        if (typeStr == "WRITE") {
            return { CommandType::WRITE, lba, dataOrRange };
        }
        else if (typeStr == "ERASE") {
            return { CommandType::ERASE, lba, dataOrRange };
        }

        throw std::runtime_error("Invalid command in file: " + line);
    }
};

// Singleton
#define SSD_CMD_BUFFER SSDCmdBuffer::getInstance()

class SSDCmdBuffer {
public:
    static SSDCmdBuffer& getInstance(void) {
        static SSDCmdBuffer instance;
        return instance;
    }

public:
    SSDCmdBufferOutput output;

public:
    unsigned int readData(const int lba);
    void writeData(const int lba, const unsigned int data);
    void eraseData(const int lba, const int range);
    void flushData(void);
    const std::vector<Command>& getBuffer(void) const;
    const std::string _bufferFileName = "cmd_buffer.txt";
    void clearForTestOnly(void);

private:
    void _saveBufferToFile() const;
    void _loadBufferFromFile();
    bool _isLbaTheSame(const Command& cmd, const int lba) const;
    bool _isCmdWrite(const Command& cmd) const;
    bool _isCmdErase(const Command& cmd) const;
    unsigned int _getBufferedDataIfHit(const Command& cmd, const int lba, bool& retFlag);
    bool _checkEraseLbaRangeInvalid(const int lba, const int range) const;
    bool _checkLbaOutOfRange(const int lba) const;
    void _optimize(void);
    void _exportBufferToFiles(void);
    void _clearBufferIfNeeded(void);
    void _removeOverwrittenWrites(const int lba);
    void _removeWritesCoveredByErase(const int startLba, const int endLba);
    void _mergeErases(void);
    void _resolveEraseWriteConflicts(void);

public:
    SSDCmdBuffer() {
        _loadBufferFromFile();  // 프로그램 실행 시 복원
    };
    ~SSDCmdBuffer() = default;
    SSDCmdBuffer(const SSDCmdBuffer&) = delete;
    SSDCmdBuffer& operator=(const SSDCmdBuffer&) = delete;

private:
    static const int LBA_CAPACITY = 100;
    static const int LBA_UPPER_LIMIT = LBA_CAPACITY - 1;
    static const int LBA_LOWER_LIMIT = 0;
    static const int MAX_ERASE_RANGE = 10;
    static const int MIN_ERASE_RANGE = 0;

    static const int MAX_CMDBUF_SIZE = 5;
    std::vector<Command> cmdBuffer;
};