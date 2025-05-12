#include "gmock/gmock.h"
#include <string>
#include "SSDCmdParser.h"
#include "SSDDevice.h"

using std::string;
using namespace testing;

namespace {
    const bool PARSING_FAILED = false;
    const bool PARSING_SUCCESS = true;
}

class SSDCmdParserFixture : public Test {
public:
    SSDCmdParser parser;
};

TEST_F(SSDCmdParserFixture, InvalidCommand) {
    const char* argv[] = { "SSD.exe" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadSuccess) {
    const char* argv[] = { "SSD.exe", "R", "0" };
    int argc = sizeof(argv) / sizeof(argv[0]);
    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadFailInvalidLBA) {
    const char* argv[] = { "SSD.exe", "R", "100" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parser.checkParsing(argc, argv), std::invalid_argument);
}

TEST_F(SSDCmdParserFixture, WriteSuccess) {
    const char* argv[] = { "SSD.exe", "W", "0", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_SUCCESS;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailInvalidLBA) {
    const char* argv[] = { "SSD.exe", "W", "100", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    EXPECT_THROW(parser.checkParsing(argc, argv), std::invalid_argument);
}

TEST_F(SSDCmdParserFixture, WriteFailDecimalValue) {
    const char* argv[] = { "SSD.exe", "W", "0", "12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailBigValue) {

    const char* argv[] = { "SSD.exe", "W", "0", "0x1FFFFFFFF" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailSmallValue) {
    const char* argv[] = { "SSD.exe", "W", "0", "0x1" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    bool actual = parser.checkParsing(argc, argv);
    bool expected = PARSING_FAILED;
    EXPECT_EQ(expected, actual);
}