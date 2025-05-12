#pragma once
#include <string>
#include "ISSDAdapter.h"

using namespace std;

class SSDAdapter : public ISSDAdapter{
public:
	void writeLba(const int lba, const int data) override;
	int readLba(const int lba) override;
	void fullWrite(const int data) override;
	void fullRead(void) override;

private:
	void _ExecuteSSDCommand(const string command);
	int _ReadDataFromSSDOutputFile(void);
	string _CheckExecutionResultFromSSDOutputFile(void);
};