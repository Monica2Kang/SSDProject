#include <iostream>
#include "gmock/gmock.h"
#include "SSDDevice.h"

using namespace testing;
using namespace std;

class SSDDeviceFixture : public Test {
public:
    SSDDevice ssd;
    void validArgumentTest4readData(const int lba) {
        EXPECT_NO_THROW(ssd.readData(lba));
    }
    void invalidArgumentTest4readData(const int lba) {
        EXPECT_THROW(ssd.readData(lba), invalid_argument);
    }
    void validArgumentTest4writeData(const int lba) {
        EXPECT_NO_THROW(ssd.writeData(lba, SAMPLE_DATA));
    }
    void invalidArgumentTest4writeData(const int lba) {
        EXPECT_THROW(ssd.writeData(lba, SAMPLE_DATA), invalid_argument);
    }

    std::string intToHexString(const unsigned int value) const {
        std::ostringstream oss;
        oss << "0x" << std::uppercase << std::setfill('0') << std::setw(OUTPUT_DIGIT)
            << std::hex << value;
        return oss.str();
    }

    bool containsError() const {
        std::ifstream infile(FILE_NAME_OUTPUT);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot Open the File.");
        }

        std::string line;
        while (std::getline(infile, line)) {
            if (line.find("ERROR") != std::string::npos) {
                return true;
            }
        }
        return false;
    }

    bool containsValue(int value) const {
        std::ifstream infile(FILE_NAME_OUTPUT);
        if (!infile.is_open()) {
            throw std::runtime_error("Cannot Open the File.");
        }

        std::string target = intToHexString(static_cast<unsigned int>(value));
        std::string line;
        while (std::getline(infile, line)) {
            if (line.find(target) != std::string::npos) {
                return true;
            }
        }
        return false;
    }

protected:
    struct LBA_DATA {
        int lba;
        int data;
    };
    struct LBA_RANGE {
        int lba;
        int range;
        bool expected;
    };

    const int OUTPUT_DIGIT = 8;
    const char* FILE_NAME_OUTPUT = "ssd_output.txt";
    static const int SAMPLE_DATA = static_cast<int>(0x1082);
    const vector<LBA_DATA> inRangeLbaDatas = {
        {0, static_cast<int>(0x100)},
        {1, static_cast<int>(0x1AFAED)},
        {32, static_cast<int>(0xABCDEF)},
        {57, static_cast<int>(0xDEADBEEF)},
        {89, static_cast<int>(0xBEEF1082)},
        {98, static_cast<int>(0xB1E8F0E2)},
    };

    const vector<LBA_RANGE> eraseLbaRanges = {
        {0, static_cast<int>(0), true },
        {1, static_cast<int>(10), true },
        {32, static_cast<int>(11), false },
        {57, static_cast<int>(3), true },
        {89, static_cast<int>(5), true },
        {99, static_cast<int>(0), true },
        {100, static_cast<int>(5), false },
        {101, static_cast<int>(5), false },
        {-1, static_cast<int>(5), false },
        {98, static_cast<int>(-1), false },
        {198, static_cast<int>(3), false },
    };
};

TEST_F(SSDDeviceFixture, ssdCreationTC) {
    EXPECT_NE(&ssd, nullptr);
}

TEST_F(SSDDeviceFixture, ssdReadDataTC) {
    ssd.reinitializeFile();
    EXPECT_THROW(ssd.readData(-1), invalid_argument);
    EXPECT_THROW(ssd.readData(0), exception);
    //ssd.writeData(0, 0);
    //int actual = ssd.readData(0);
    //EXPECT_EQ(0, actual);
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAInRange) {
    vector<int> lba = { 0, 1, 10, 20, 30, 53, 75, 97, 98, 99 };
    for (int addr : lba) {
        ssd.writeData(addr, addr);
        validArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4LBAOutOfRangeMinus) {
    vector<int> lba = { -1, -2, -110, -253,-1082, -337, -4 };
    for (int addr : lba) {
        invalidArgumentTest4readData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAInRange) {
    vector<int> lba = { 0, 1, 10, 20, 30, 53, 75, 97, 98, 99 };
    for (int addr : lba) {
        validArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAOutOfRange) {
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteDataTC4LBAOutOfRangeMinus) {
    vector<int> lba = { -1, -2, -110, -253,-1082, -337, -4 };
    for (int addr : lba) {
        invalidArgumentTest4writeData(addr);
    }
}

TEST_F(SSDDeviceFixture, ssdWriteReadDataConfirmTC) {
    ssd.writeData(0, static_cast <int>(0x0));
    int actual = ssd.readData(0);
    EXPECT_EQ(0, actual);

    ssd.writeData(0, SAMPLE_DATA);
    actual = ssd.readData(0);
    EXPECT_EQ(SAMPLE_DATA, actual);
}

TEST_F(SSDDeviceFixture, ssdWriteReadDataConfirmTC4Multi) {
    for (LBA_DATA lba_data : inRangeLbaDatas) {
        ssd.writeData(lba_data.lba, lba_data.data);
        int actual = ssd.readData(lba_data.lba);
        EXPECT_EQ(lba_data.data, actual);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4UntouchedLba) {
    ssd.reinitializeFile();
    for (auto inRangeLbaData : inRangeLbaDatas) {
        EXPECT_THROW(ssd.readData(inRangeLbaData.lba), exception);
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4FileOutputCheckData) {
    for (LBA_DATA lba_data : inRangeLbaDatas) {
        ssd.writeData(lba_data.lba, lba_data.data);
        int actual = ssd.readData(lba_data.lba);
        EXPECT_EQ(lba_data.data, actual);
        EXPECT_TRUE(containsValue(actual));
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4FileOutputCheckUntouched) {
    ssd.reinitializeFile();
    for (auto inRangeLbaData : inRangeLbaDatas) {
        EXPECT_THROW(ssd.readData(inRangeLbaData.lba), exception);
        EXPECT_TRUE(containsValue(0x0));
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4FileOutputCheckError) {
    ssd.reinitializeFile();
    vector<int> lba = { 100, 101, 110, 253, 337, 1553, 25675 };
    for (int addr : lba) {
        EXPECT_THROW(ssd.readData(addr), exception);
        EXPECT_TRUE(containsError());
    }
}

TEST_F(SSDDeviceFixture, ssdReadDataTC4FileOutputCheckForcedError) {
    for (LBA_DATA lba_data : inRangeLbaDatas) {
        ssd.writeData(lba_data.lba, lba_data.data);
        int actual = ssd.readData(lba_data.lba);
        EXPECT_EQ(lba_data.data, actual);
        EXPECT_TRUE(containsValue(actual));

        ssd.printError();
        EXPECT_TRUE(containsError());
    }
}

TEST_F(SSDDeviceFixture, ssdEraseDataTC4RangeCheck) {
    for (LBA_RANGE lba_range : eraseLbaRanges) {
        if (lba_range.expected) {
            EXPECT_NO_THROW(ssd.eraseData(lba_range.lba, lba_range.range));
        }
        else {
            EXPECT_THROW(ssd.eraseData(lba_range.lba, lba_range.range), invalid_argument);
            EXPECT_TRUE(containsError());
        }
    }
}