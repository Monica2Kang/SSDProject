//#include <iostream>
//#include <vector>
//#include <algorithm>
//#include <string>
//#include "SSDCmdBuffer.h"

#include <iostream>
#include "gmock/gmock.h"
#include "SSDCmdBuffer.h"

using namespace testing;
using namespace std;

class SSDCmdBufferFixture : public Test {
public:
    SSDCmdBuffer cbuf;

protected:
    struct LBA_RANGE_DATA_EXP {
        int lba;
        int rangeOrData;
        bool expected;
    };

    const vector<LBA_RANGE_DATA_EXP> lrde4Erases = {
        {0, 10, true},
        {98, 3, false},
        {1, 9, true},
        {32, 11, false},
        {77, 7, true},
        {65, 0, true},
        {101, 0, false},
        {57, 3, true},
        {98, 2, true},
        {102, 1, false},
        {-1, 0, false},
        {89, 10, true},
        {99, 2, false},
        {100, 2, false},
    };

    const vector<LBA_RANGE_DATA_EXP> lrde4Writes = {
        {0, static_cast<int>(0xDEADBEEF), true},
        {98, static_cast<int>(0x12345678), true},
        {1, static_cast<int>(0x9), true},
        {32, static_cast<int>(0xDEADCAFE), true},
        {77, static_cast<int>(0xBEEFCAFE), true},
        {65, static_cast<int>(0x0), true},
        {101, static_cast<int>(0x1082), false},
        {57, static_cast<int>(33789), true},
        {98, static_cast<int>(0xAAAA5555), true},
        {102, static_cast<int>(1), false},
        {-1, static_cast<int>(0), false},
        {89, static_cast<int>(10), true},
        {99, static_cast<int>(2), true},
        {100, static_cast<int>(2), false},
    };
};

TEST_F(SSDCmdBufferFixture, CmdBufferInstanceCreationTC) {
    EXPECT_NE(&cbuf, nullptr);
}

TEST_F(SSDCmdBufferFixture, CmdBufferOutOfBound4EraseTC) {
    for (auto lrde : lrde4Erases) {
        if (lrde.expected) {
            EXPECT_NO_THROW(cbuf.addErase(lrde.lba, lrde.rangeOrData));
        }
        else {
            EXPECT_THROW(cbuf.addErase(lrde.lba, lrde.rangeOrData), invalid_argument);
        }
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferOutOfBound4WriteTC) {
    for (auto lrde : lrde4Writes) {
        if (lrde.expected) {
            EXPECT_NO_THROW(cbuf.addWrite(lrde.lba, lrde.rangeOrData));
        }
        else {
            EXPECT_THROW(cbuf.addWrite(lrde.lba, lrde.rangeOrData), invalid_argument);
        }
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC0) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1}, 
        {CommandType::ERASE, 1, 1}, 
    };
    Command eraseCmdAnswer{ CommandType::ERASE, 0, 2 };

    for (auto cmd : eraseCmd) {
        cbuf.addErase(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    Command actual = *buffer.begin();

    EXPECT_TRUE(0 == std::memcmp(&actual, &eraseCmdAnswer, sizeof(eraseCmdAnswer)));
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC1) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 2, 1},
        {CommandType::ERASE, 1, 1},
        {CommandType::ERASE, 4, 1},
        {CommandType::ERASE, 3, 1},
    };
    Command eraseCmdAnswer{ CommandType::ERASE, 0, 5 };

    for (auto cmd : eraseCmd) {
        cbuf.addErase(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    Command actual = *buffer.begin();

    EXPECT_TRUE(0 == std::memcmp(&actual, &eraseCmdAnswer, sizeof(eraseCmdAnswer)));
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC2) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 2, 1},
        {CommandType::ERASE, 1, 1},
        {CommandType::ERASE, 4, 1},
        {CommandType::ERASE, 3, 1},
        {CommandType::ERASE, 3, 1},
        {CommandType::ERASE, 1, 1},
        {CommandType::ERASE, 4, 1},
    };
    Command eraseCmdAnswer{ CommandType::ERASE, 0, 5 };

    for (auto cmd : eraseCmd) {
        cbuf.addErase(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    Command actual = *buffer.begin();

    EXPECT_TRUE(0 == std::memcmp(&actual, &eraseCmdAnswer, sizeof(eraseCmdAnswer)));
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC3) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 48, 3},
        {CommandType::ERASE, 1, 4},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::ERASE, 0, 5},
        {CommandType::ERASE, 48, 6},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : eraseCmd) {
        cbuf.addErase(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseWriteSplitCmdTC0) {
    vector<Command> commandList = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 48, 3},
        {CommandType::ERASE, 1, 4},
        {CommandType::WRITE, 3, static_cast<int>(0xBEEFCAFE)},
        //{CommandType::WRITE, 50, 0xA5A5A5A5},

    };
    vector<Command> eraseCmdExpected = {
        {CommandType::WRITE, 3, static_cast<int>(0xBEEFCAFE)},
        {CommandType::ERASE, 0, 3},
        {CommandType::ERASE, 4, 1},
        {CommandType::ERASE, 48, 6},
        //{CommandType::WRITE, 50, 0xA5A5A5A5},
        //{CommandType::ERASE, 51, 2},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.addErase(cmd.lba, cmd.dataOrRange);
        else
            cbuf.addWrite(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}


