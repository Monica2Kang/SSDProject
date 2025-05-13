#include "gmock/gmock.h"
#include <string>
#include "SSDCmdParser.h"
#include "SSDDevice.h"

using std::string;
using namespace testing;

class SSDCmdParserFixture : public SSDCmdParser, public Test {
public:
    SSDCmdParser parser;
    const char* EXE_FILE_NAME = "SSD.exe";
    const char* READ_COMMAND = "R";
    const char* WRITE_COMMAND = "W";
    const char* ERASE_COMMAND = "E";
    const char* FLUSH_COMMAND = "F";

    const char* DEFAULT_LBA = "0";
    const char* OVER_LBA = "100";
    const char* DEFAULT_VALUE = "0x1234ABCD";
};

TEST_F(SSDCmdParserFixture, InvalidCommand) {
    const char* argv[] = { EXE_FILE_NAME };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadSuccess) {
    const char* argv[] = { EXE_FILE_NAME, READ_COMMAND, DEFAULT_LBA };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadFailInvalidLBA) {
    const char* argv[] = { EXE_FILE_NAME, READ_COMMAND, OVER_LBA };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parser.checkParsing(argc, argv), std::invalid_argument);
}

TEST_F(SSDCmdParserFixture, WriteSuccess) {
    const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, DEFAULT_LBA, DEFAULT_VALUE };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailInvalidLBA) {
    const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, OVER_LBA, DEFAULT_VALUE };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parser.checkParsing(argc, argv), std::invalid_argument);
}

TEST_F(SSDCmdParserFixture, WriteFailDecimalValue) {
    const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, DEFAULT_LBA, "12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailBigValue) {

    const char* argv[] = { EXE_FILE_NAME, WRITE_COMMAND, DEFAULT_LBA, "0x1FFFFFFFF" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailSmallValue) {
    const char* argv[] = { EXE_FILE_NAME,WRITE_COMMAND, DEFAULT_LBA, "0x1" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, EraseSuccessSizeZero) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, "0"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, EraseSuccessSizePositive) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, "5"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, EraseFailMinuxSize) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, "-1"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, EraseFailOverSize) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, DEFAULT_LBA, "11"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

/* ex)  90 10 : 90~99 -> success
*       91 10 : 91~100 -> fail
*       99 2  : 99~100 -> fail
*/
TEST_F(SSDCmdParserFixture, EraseFailOutOfRange1) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, "91", "10"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, EraseFailOutOfRange2) {
    const char* argv[] = { EXE_FILE_NAME, ERASE_COMMAND, "99", "2"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, FlushSuccess) {
    const char* argv[] = { EXE_FILE_NAME, FLUSH_COMMAND};
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}