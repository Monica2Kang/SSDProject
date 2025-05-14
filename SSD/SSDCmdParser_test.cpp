#include "gmock/gmock.h"
#include <string>
#include <vector>
#include "SSDCmdParser.h"
#include "SSDDevice.h"

using namespace std;
using namespace testing;

#define ssd SSDDevice::getInstance()

class SSDCmdParserFixture : public SSDCmdParser, public Test {
public:
    void testWriteFail(const vector<string>& valueList) {
        for (string value : valueList) {
            const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, DEFAULT_LBA, value.c_str() };
            int argc = sizeof(argv) / sizeof(argv[0]);

            bool actual = parser.checkParsing(argc, argv);
            bool expected = PARSING_FAILED;
            EXPECT_EQ(expected, actual);
        }
    }

    void testEraseFail(const vector<string>& sizeList) {
        for (string size : sizeList) {
            const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, size.c_str() };
            int argc = sizeof(argv) / sizeof(argv[0]);

            bool actual = parser.checkParsing(argc, argv);
            bool expected = PARSING_FAILED;
            EXPECT_EQ(expected, actual);
        }
    }

    std::string intToHexString(const unsigned int value) const {
        std::ostringstream oss;
        oss << "0x" << std::uppercase << std::setfill('0') << std::setw(OUTPUT_DIGIT)
            << std::hex << value;
        return oss.str();
    }

    //bool containsValue(int value) const {
    bool containsValue(std::string value) const {
        std::ifstream infile(OUTPUT_FILE_NAME);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot Open the File.");
        }

        //std::string target = intToHexString(static_cast<unsigned int>(value));
        std::string line;
        while (std::getline(infile, line)) {
            if (line.find(value) != std::string::npos) {
                infile.close();
                return true;
            }
        }
        infile.close();
        return false;
    }

    bool containsError() const {
        std::ifstream infile(OUTPUT_FILE_NAME);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot Open the File.");
        }

        std::string line;
        while (std::getline(infile, line)) {
            if (line.find("ERROR") != std::string::npos) {
                infile.close();
                return true;
            }
        }
        infile.close();
        return false;
    }

public:
    SSDCmdParser parser;
    const char* EXE_FILE_NAME = "SSD.exe";
    const char* OUTPUT_FILE_NAME = "ssd_output.txt";
    const char* READ_COMMAND = "R";
    const char* WRITE_COMMAND = "W";
    const char* ERASE_COMMAND = "E";
    const char* FLUSH_COMMAND = "F";

    const char* DEFAULT_LBA = "0";
    const char* OVER_LBA = "100";
    const char* DEFAULT_VALUE = "0x1234ABCD";
    const int OUTPUT_DIGIT = 8;

    const vector<string> VALID_LBA_LIST = {
        "3", "25", "11", "95", "73", "52"
    };
    const vector<string> OVER_LBA_LIST = {
        "123", "8392", "321", "573", "2312", "213"
    };
    const vector<string> VALID_VALUE_LIST = {
        "0x00000100", "0x001AFAED", "0x00ABCDEF", "0xDEADBEEF", "0xBEEF1082", "0xB1E8F0E2"
    };
    const vector<string> DECIMEL_VALUE_LIST = {
        "2", "75", "14325", "2136422", "323467842", "4294967295"
    };
    const vector<string> BIG_VALUE_LIST = {
        "0x1FFFFFFFF", "0x334563223", "0x19593039172", "0x9392AB21DAF", "0xFF2FBED15", "0x27C25FE92AE"
    };
    const vector<string> SMALL_VALUE_LIST = {
        "0x1", "0xFF", "0x24F", "0xBA182", "0xAB21DAF", "0x5FE92AE"
    };
    const vector<string> MINUS_SIZE_LIST = {
        "-1", "-27", "-423", "-2852", "-84312", "-3728283"
    };
    const vector<string> OVER_SIZE_LIST = {
        "11", "27", "423", "2852", "84312", "3728283"
    };
    const vector<vector<string>> OVER_RANGE_LIST = {
        {"99", "10"}, {"91", "10"}, {"99", "2"}, {"95", "6"}, {"95", "7"}, {"93", "8"}
    };

    struct LBA_DATA {
        int lba;
        unsigned int data;
    };

    const vector<LBA_DATA> inRangeLbaDatas = {
        {0,  0x100},
        {1,  0x1AFAED},
        {98, 0xB1E8F0E2},
        {57, 0xDEADBEEF},
        {32, 0xABCDEF},
        {89, 0xBEEF1082},
        {99, 0xFFFFFFFF},
    };
    const vector<LBA_DATA> outOfRangeLbaDatas = {
        {-1, 0x100},
        {-100, 0x1AFAED},
        {132, 0xABCDEF},
        {5557, 0xDEADBEEF},
        {889, 0xBEEF1082},
        {100, 0xB1E8F0E2},
        {101, 0x1082},
        {-15557, 0xDEADBEEF},
    };

protected:
    void SetUp(void) override {
        for (string lba : VALID_LBA_LIST) {
            const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, lba.c_str(), DEFAULT_VALUE };
            int argc = sizeof(argv) / sizeof(argv[0]);
            parser.checkParsing(argc, argv);
        }
    }
};

TEST_F(SSDCmdParserFixture, InvalidCommand) {
    const char* argv[] = { EXE_FILE_NAME };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadSuccess) {
    for (string lba : VALID_LBA_LIST) {
        const char* argv[] = { EXE_FILE_NAME, READ_COMMAND, lba.c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_SUCCESS;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, ReadFailInvalidLBA) {
    for (string lba : OVER_LBA_LIST) {
        const char* argv[] = { EXE_FILE_NAME, READ_COMMAND, lba.c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_FAILED;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, WriteSuccess) {
    for (int index = 0; index < VALID_LBA_LIST.size(); index++) {
        string lba = VALID_LBA_LIST[index];
        string value = VALID_VALUE_LIST[index];
        const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, lba.c_str(), value.c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_SUCCESS;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, WriteFailInvalidLBA) {
    for (string lba : OVER_LBA_LIST) {
        const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, lba.c_str(), DEFAULT_VALUE };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_FAILED;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, WriteFailDecimalValue) {
    testWriteFail(DECIMEL_VALUE_LIST);
}

TEST_F(SSDCmdParserFixture, WriteFailBigValue) {
    testWriteFail(BIG_VALUE_LIST);
}

TEST_F(SSDCmdParserFixture, WriteFailSmallValue) {
    testWriteFail(SMALL_VALUE_LIST);
}

TEST_F(SSDCmdParserFixture, EraseSuccessSize) {
    for (int size = MIN_ERASE_SIZE; size <= MAX_ERASE_SIZE; size++) {
        string str = to_string(size);
        const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, str.c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_SUCCESS;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, EraseFailMinuxSize) {
    testEraseFail(MINUS_SIZE_LIST);
}

TEST_F(SSDCmdParserFixture, EraseFailOverSize) {
    testEraseFail(OVER_SIZE_LIST);
}
/* ex)  90 10 : 90~99 -> success
*       91 10 : 91~100 -> fail
*       99 2  : 99~100 -> fail
*/
TEST_F(SSDCmdParserFixture, EraseFailOutOfRange) {
    for (vector<string> range : OVER_RANGE_LIST) {
        const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, range[0].c_str(), range[1].c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_FAILED;
        EXPECT_EQ(expected, actual);
    }
}

TEST_F(SSDCmdParserFixture, FlushSuccess) {
    const char* argv[] = { EXE_FILE_NAME, FLUSH_COMMAND };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}


TEST_F(SSDCmdParserFixture, FileOutputCheckTouched) {
    for (int index = 0; index < VALID_LBA_LIST.size(); index++) {
        string lba = VALID_LBA_LIST[index];
        string value = VALID_VALUE_LIST[index];
        const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, lba.c_str(), value.c_str() };
        int argc = sizeof(argv) / sizeof(argv[0]);

        bool actual = parser.checkParsing(argc, argv);
        bool expected = PARSING_SUCCESS;
        EXPECT_EQ(expected, actual);

        const char* argvR[] = { EXE_FILE_NAME, READ_COMMAND, lba.c_str() };
        int argcR = sizeof(argvR) / sizeof(argvR[0]);

        actual = parser.checkParsing(argcR, argvR);
        expected = PARSING_SUCCESS;
        EXPECT_EQ(expected, actual);
        EXPECT_TRUE(containsValue(value.c_str()));

    }
}

TEST_F(SSDCmdParserFixture, FileOutputCheckUntouched) {
    ssd.reinitializeFile();
    for (int index = 0; index < VALID_LBA_LIST.size(); index++) {
        string lba = VALID_LBA_LIST[index];
        string value = VALID_VALUE_LIST[index];

        const char* argvR[] = { EXE_FILE_NAME, READ_COMMAND, lba.c_str() };
        int argcR = sizeof(argvR) / sizeof(argvR[0]);

        bool actual = parser.checkParsing(argcR, argvR);
        bool expected = PARSING_FAILED;
        EXPECT_EQ(expected, actual);
        EXPECT_TRUE(containsValue("0x00000000")); 
    }
}

TEST_F(SSDCmdParserFixture, FileOutputCheckError) {
    for (int index = 0; index < OVER_LBA_LIST.size(); index++) {
        string lba = OVER_LBA_LIST[index];
        string value = VALID_VALUE_LIST[index];

        const char* argvR[] = { EXE_FILE_NAME, READ_COMMAND, lba.c_str() };
        int argcR = sizeof(argvR) / sizeof(argvR[0]);

        bool actual = parser.checkParsing(argcR, argvR);
        bool expected = PARSING_FAILED;
        EXPECT_EQ(expected, actual);
        EXPECT_TRUE(containsError());
    }
}