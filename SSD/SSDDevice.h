#pragma once

class SSDDevice {
public:
    int readData(int LBA);
    void writeData(int LBA, int data);

private:
    static const int MAX_LBA = 100;
    int cellData[MAX_LBA];
};
