#pragma once

class SSDFileStorageDevice {
public:
    SSDFileStorageDevice() = default;
    SSDFileStorageDevice(std::string filename, int maxLbaCapcity) : filename{ filename }, maxLbaCapcity{ maxLbaCapcity } {};
    bool writeData(int lba, int data);
    bool readData(int lba, int &data);

private:
    std::string filename;
    int maxLbaCapcity;
};