#include <fstream>
#include "gmock/gmock.h"
#include "SSDFileStorageDevice.h"

using namespace testing;
using namespace std;

class SSDFileStorageDeviceFixture : public Test {
public:
protected:
    const char* FILE_NAME = "ssd_nand.txt";
    const int FILE_STORAGE_CAPACITY = 100;
    SSDFileStorageDevice fSsd = { FILE_NAME, FILE_STORAGE_CAPACITY };
};

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageDummyInstanceCreationTC) {
    SSDFileStorageDevice fSsdDummy;
    EXPECT_NE(&fSsdDummy, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC) {
    EXPECT_NE(&fSsd, nullptr);
}

TEST_F(SSDFileStorageDeviceFixture, ssdFileStorageCreationTC4ssd_nand_txt) {
    std::ifstream check_file;
    
    std::remove(FILE_NAME);
    check_file.open(FILE_NAME, std::ios::binary);
    bool isFileOpend = check_file.is_open();
    EXPECT_EQ(isFileOpend, false);

    SSDFileStorageDevice fSsdTemp = { FILE_NAME, FILE_STORAGE_CAPACITY };
    if (fSsdTemp.openFile()) 
        std::cout << FILE_NAME << " file created." << std::endl;
    else 
        std::cout << FILE_NAME << " file is not created." << std::endl;
    fSsdTemp.closeFile();

    check_file.open(FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
    isFileOpend = check_file.is_open();
    EXPECT_EQ(isFileOpend, true);
    std::remove(FILE_NAME);
    std::cout << FILE_NAME << " file is deleted." << std::endl;
}