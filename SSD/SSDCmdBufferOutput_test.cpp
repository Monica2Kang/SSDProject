#include "gmock/gmock.h"
#include "SSDCmdBufferOutput.h"

using namespace testing;
using namespace std;

class SSDCmdBufferOutputFixture : public SSDCmdBufferOutput, public Test {
public:
    SSDCmdBufferOutput bufOutput;

    vector<string> INPUT_FILE_LIST = {
        fileEmptyName, fileEmptyName, fileEmptyName, fileEmptyName, fileEmptyName
    };
    vector<string> TEST_FILE_LIST = {
        "W_0_0x11112222",
        "E_0_10",
        "W_1_0x22223333",
        "E_80_10",
        "W_0_0x00002222"
    };
};

TEST_F(SSDCmdBufferOutputFixture, CreateFilesSuccessList) {
    for (int index = 0; index < MAX_FILE_COUNT; index++) {
        INPUT_FILE_LIST[index] = TEST_FILE_LIST[index];
        bufOutput.createFilesInFolder(INPUT_FILE_LIST);
        vector<string> expected = bufOutput.getFileListForDebug();
        EXPECT_EQ(expected, INPUT_FILE_LIST);
        expected.clear();
    }
}