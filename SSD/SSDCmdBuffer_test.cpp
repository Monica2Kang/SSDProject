#include <iostream>
#include "gmock/gmock.h"
#include "SSDCmdBuffer.h"
#include "SSDCmdBufferOutput.h"

using namespace testing;
using namespace std;

#define cbuf SSDCmdBuffer::getInstance()

class SSDCmdBufferFixture : public Test {
public:
    //SSDCmdBuffer cbuf;

    // 파일 이름 확인용
    vector<string> getFileNames() {
        //SSDCmdBufferOutput& output = SSDCmdBuffer::output;
        return cbuf.output.getFileListForDebug();
    }

    void expectFileNamesMatch(const vector<string>& actual, const vector<string>& expected) {
        ASSERT_EQ(actual.size(), expected.size());
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(actual[i], expected[i]);
        }
    }

protected:
    struct LBA_RANGE_DATA_EXP {
        int lba;
        unsigned int rangeOrData;
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
        {0, 0xDEADBEEF, true},
        {98, 0x12345678, true},
        {1, 0x9, true},
        {32, 0xDEADCAFE, true},
        {77, 0xBEEFCAFE, true},
        {65, 0x0, true},
        {101, 0x1082, false},
        {57, 33789, true},
        {98, 0xAAAA5555, true},
        {102, 1, false},
        {-1, 0, false},
        {89, 10, true},
        {99, 2, true},
        {100, 2, false},
    };
};

TEST_F(SSDCmdBufferFixture, CmdBufferInstanceCreationTC) {
    EXPECT_NE(&cbuf, nullptr);
}

TEST_F(SSDCmdBufferFixture, CmdBufferOutOfBound4EraseTC) {
    for (auto lrde : lrde4Erases) {
        if (lrde.expected) {
            EXPECT_NO_THROW(cbuf.eraseData(lrde.lba, lrde.rangeOrData));
        }
        else {
            EXPECT_THROW(cbuf.eraseData(lrde.lba, lrde.rangeOrData), invalid_argument);
        }
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferOutOfBound4WriteTC) {
    for (auto lrde : lrde4Writes) {
        if (lrde.expected) {
            EXPECT_NO_THROW(cbuf.writeData(lrde.lba, lrde.rangeOrData));
        }
        else {
            EXPECT_THROW(cbuf.writeData(lrde.lba, lrde.rangeOrData), invalid_argument);
        }
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferDuplicateWriteCmdTC0) {
    cbuf.clearForTestOnly();  // TC 마다 Buffer 초기화 필요
    vector<Command> writeCmd = {
        {CommandType::WRITE, 0, 0xBEEFCAF0},
        {CommandType::WRITE, 0, 0xBEEFCAF1},
        {CommandType::WRITE, 0, 0xBEEFCAF2},
        {CommandType::WRITE, 0, 0xBEEFCAF3},
        {CommandType::WRITE, 0, 0xBEEFCAF4},
        {CommandType::WRITE, 0, 0xBEEFCAF5},
        {CommandType::WRITE, 0, 0xBEEFCAF6},
        {CommandType::WRITE, 0, 0xBEEFCAF6},
        {CommandType::WRITE, 0, 0xBEEFCAF6},
    };


    for (const auto& cmd : writeCmd) {
        cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }

    // 최종 결과는 가장 마지막 값만 남아야 한다
    vector<Command> buffer = cbuf.getBuffer();

    ASSERT_EQ(buffer.size(), 1);  // Only the last write should remain

    Command expected = { CommandType::WRITE, 0, 0xBEEFCAF6 };
    Command actual = buffer[0];

    EXPECT_EQ(actual.type, expected.type);
    EXPECT_EQ(actual.lba, expected.lba);
    EXPECT_EQ(actual.dataOrRange, expected.dataOrRange);
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC0) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 1, 1},
    };
    Command eraseCmdAnswer{ CommandType::ERASE, 0, 2 };

    for (auto cmd : eraseCmd) {
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
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
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    Command actual = *buffer.begin();

    EXPECT_TRUE(0 == std::memcmp(&actual, &eraseCmdAnswer, sizeof(eraseCmdAnswer)));
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC14CoveringCase) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 1, 1},
        {CommandType::ERASE, 2, 1},
        {CommandType::ERASE, 1, 1},
        {CommandType::ERASE, 4, 1},
        {CommandType::ERASE, 3, 1},
        {CommandType::ERASE, 0, 10},
    };
    Command eraseCmdAnswer{ CommandType::ERASE, 0, 10 };

    for (auto cmd : eraseCmd) {
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    Command actual = *buffer.begin();

    EXPECT_TRUE(0 == std::memcmp(&actual, &eraseCmdAnswer, sizeof(eraseCmdAnswer)));
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC2) {
    cbuf.clearForTestOnly();
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
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
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
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC4) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 48, 3},
        {CommandType::ERASE, 1, 4},
        {CommandType::ERASE, 52, 4},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::ERASE, 0, 5},
        {CommandType::ERASE, 48, 8},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : eraseCmd) {
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC5) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 48, 3},
        {CommandType::ERASE, 1, 4},
        {CommandType::ERASE, 52, 4},
        {CommandType::ERASE, 52, 6},
        {CommandType::ERASE, 3, 7},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::ERASE, 0, 10},
        {CommandType::ERASE, 48, 10},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : eraseCmd) {
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeEraseCmdTC6) {
    vector<Command> eraseCmd = {
        {CommandType::ERASE, 0, 1},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 48, 3},
        {CommandType::ERASE, 1, 4},
        {CommandType::ERASE, 52, 4},
        {CommandType::ERASE, 53, 5},
        {CommandType::ERASE, 3, 7},
        {CommandType::ERASE, 5, 7},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::ERASE, 0, 10},
        {CommandType::ERASE, 10, 2},
        {CommandType::ERASE, 48, 10},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : eraseCmd) {
        cbuf.eraseData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeWriteEraseComplexCmdTC0) {
    cbuf.clearForTestOnly();
    vector<Command> commandList = {
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 7, 2},
        {CommandType::ERASE, 48, 3},
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::ERASE, 54, 3},

    };
    vector<Command> eraseCmdExpected = {
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::ERASE, 0, 9},
        {CommandType::ERASE, 48, 9},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.eraseData(cmd.lba, cmd.dataOrRange);
        else
            cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeWriteEraseComplexCmdTC1) {
    vector<Command> commandList = {
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 7, 2},
        {CommandType::ERASE, 48, 3},
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::ERASE, 54, 3},
        {CommandType::WRITE, 99, 0xBEADCAFE},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::WRITE, 99, 0xBEADCAFE},
        {CommandType::ERASE, 0, 9},
        {CommandType::ERASE, 48, 9},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.eraseData(cmd.lba, cmd.dataOrRange);
        else
            cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeWriteEraseComplexCmdTC2) {
    vector<Command> commandList = {
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 51, 3},
        {CommandType::ERASE, 7, 2},
        {CommandType::ERASE, 48, 3},
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::ERASE, 54, 3},
        {CommandType::WRITE, 99, 0xBEADCAFE},
        {CommandType::ERASE, 57, 3},
        {CommandType::WRITE, 99, 0xCAFEBEAD},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::WRITE, 99, 0xCAFEBEAD},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.eraseData(cmd.lba, cmd.dataOrRange);
        else
            cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferMergeWriteIgnoreTC0) {
    vector<Command> commandList = {
        {CommandType::WRITE, 3, 0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::WRITE, 77, 0xA5A5A5A5},
        {CommandType::WRITE, 99, 0xCAFEBEAD},
        {CommandType::WRITE, 99, 0xBEADCAFE},
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 48, 6},
        {CommandType::WRITE, 99, 0xBEADCAFE},
        {CommandType::ERASE, 70, 8},
    };
    vector<Command> eraseCmdExpected = {
        {CommandType::WRITE, 99, 0xBEADCAFE},
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 48, 6},
        {CommandType::ERASE, 70, 8},
    };
    vector<Command> eraseCmdActual;

    for (auto cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.eraseData(cmd.lba, cmd.dataOrRange);
        else
            cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }
    vector<Command> buffer = cbuf.getBuffer();
    for (auto cmd : buffer) {
        eraseCmdActual.push_back(cmd);
    }

    for (size_t i = 0; i < eraseCmdExpected.size(); ++i) {
        EXPECT_TRUE(0 == std::memcmp(&eraseCmdExpected[i], &eraseCmdActual[i], sizeof(Command)));
    }
}

TEST_F(SSDCmdBufferFixture, CmdBufferToFileConfirmation_TC1) {
    vector<Command> commandList = {
        {CommandType::WRITE, 3,  0xBEEFCAFE},
        {CommandType::WRITE, 50, 0xA5A5A5A5},
        {CommandType::WRITE, 77, 0xA5A5A5A5},
        {CommandType::WRITE, 99, 0xCAFEBEAD},
        {CommandType::WRITE, 99, 0xBEADCAFE},  // overwrite
        {CommandType::ERASE, 0, 7},
        {CommandType::ERASE, 48, 6},
        {CommandType::WRITE, 99, 0xBEADCAFE},  // re-write
        {CommandType::ERASE, 70, 8},
    };

    // 실제 동작
    for (auto& cmd : commandList) {
        if (cmd.type == CommandType::ERASE)
            cbuf.eraseData(cmd.lba, static_cast<int>(cmd.dataOrRange));
        else
            cbuf.writeData(cmd.lba, cmd.dataOrRange);
    }

    // 파일 이름 기준 기대 결과
    vector<string> expectedFileNames = {
        "1_W_99_0xBEADCAFE",
        "2_E_0_7",
        "3_E_48_6",
        "4_E_70_8",
        "5_empty",
    };

    vector<string> actualFileNames = getFileNames();
    expectFileNamesMatch(actualFileNames, expectedFileNames);
}

