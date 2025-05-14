#pragma once
#include <string>
#include "ISSDAdapter.h"

using namespace std;

class SSDAdapter : public ISSDAdapter{
public:
	void writeLba(const int lba, const int data) override;
	int readLba(const int lba) override;
	void erase(const int lba, const int size) override;
	void flush(void) override;

	bool writeLba(std::string lba, std::string data) override;
	bool readLba(std::string lba, int& readData) override;
	bool erase(std::string lba, std::string size) override;

	void fullWrite(const int data) override;
	void fullRead(void) override;

private:
	void _executeSSDCommand(const string command);
	int _readDataFromSSDOutputFile(void);

	string _readResultFromSSDOutputFile(void);
	bool _checkReadCmdExecutionResultFromSSDOutputFile(void);
	bool _checkCmdExecutionResultFromSSDOutputFile(void);
};