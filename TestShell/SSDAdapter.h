#pragma once

#include "ISSDAdapter.h"

class SSDAdapter : public ISSDAdapter{
	void wirteLba(const int lba, const int data) override;
	int readLba(const int lba) override;
	void fullWrite(const int data) override;
	void fullRead(void) override;
};