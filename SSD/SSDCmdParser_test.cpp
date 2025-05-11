#include "gmock/gmock.h"
#include "SSDCmdParser.h"
#include <string>

using std::string;

TEST(SSDCmdParser, ReadSuccess) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "R", "0" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "0x00000000";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, ReadFailInvalidLBA) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "R", "100" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, WriteSuccess) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, WriteFailInvalidLBA) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "100", "0x12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, WriteFailDecimalValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "12345678" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, WriteFailBigValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x1FFFFFFFF" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}

TEST(SSDCmdParser, WriteFailSmallValue) {
    SSDCmdParser parser;
    const char* argv[] = { "SSD.exe", "W", "0", "0x1" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    string actual = parser.checkParsing(argc, argv);
    string expected = "ERROR";
    EXPECT_EQ(expected, actual);
}