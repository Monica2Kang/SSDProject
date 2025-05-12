#pragma once

#include "ISSDAdapter.h"
#include <string>
class SSDAdapter : public ISSDAdapter{
public:
	void wirteLba(const int lba, const int data) override;
	int readLba(const int lba) override;
	void fullWrite(const int data) override;
	void fullRead(void) override;

private:
	void _ExecuteSSDCommand(std::string command);
};