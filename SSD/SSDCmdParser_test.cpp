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