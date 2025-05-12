#include "gmock/gmock.h"
#include "SSDCmdParser.h"
#include <string>

using std::string;
using namespace testing;

class SSDCmdParserFixture : public Test {
public:
};

TEST_F(SSDCmdParserFixture, ReadSuccess) {
    SSDCmdParser parser; 
    const char* argv[] = { "SSD.exe", "R", "0" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "0x00000000";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, ReadFailInvalidLBA) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "R", "100" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteSuccess) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailInvalidLBA) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "100", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailDecimalValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailBigValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x1FFFFFFFF" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST_F(SSDCmdParserFixture, WriteFailSmallValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x1" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}