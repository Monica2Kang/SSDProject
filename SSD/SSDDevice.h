#pragma once

class SSDDevice {
public:
    int readSSD(int LBA);
    void writeSSD(int LBA, int data);

private:
    static const int MAX_LBA = 100;
    int cellData[MAX_LBA];
};