#pragma once
#include "ISSDAdapter.h"
#include <vector>
#include <string>

class Shell {
public:
	const int DATA_LENGTH = 10;
	const int COMMAND_POS = 0;
	const int LBA_POS = 1;
	const int DATA_POS = 2;
	const int FULLWRITE_DATA_POS = 1;
	const int WRITE_PARAMETER_SIZE = 3;
	const int READ_PARAMETER_SIZE = 2;
	const int EXIT_PARAMETER_SIZE = 1;
	const int HELP_PARAMETER_SIZE = 3;
	const int FULLWRITE_PARAMETER_SIZE = 2;
	const int FULLREAD_PARAMETER_SIZE = 1;

	Shell(ISSDAdapter* ISSDAdapter);
	void executeShell(void);
	void setCommand(std::string command);
	void splitAndStore(void);
	bool noEnterCommand(void);
	bool writeApi(void);
	bool readApi(void);
	bool exitApi(void);
	bool helpApi(void);
	bool fullwriteApi(void);
	bool fullreadApi(void);
	bool isValidParameterSize(const int size);
	bool isValidLBA(const int pos);
	bool isValidData(const int pos);
	void storeLBA(void);
	void storeData(const int pos);

protected:
	ISSDAdapter* m_ISSDAdapter;
	std::string input;
	std::vector<std::string> parameter;
	int LBA;
	int data;
};