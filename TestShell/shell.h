#pragma once
#include "ISSDAdapter.h"
#include "TestScript.h"
#include <vector>
#include <string>

class Shell {
public:
	const int DATA_LENGTH = 10;
	const int COMMAND_POS = 0;
	const int LBA_POS = 1;
	const int END_LBA_POS = 2;
	const int DATA_POS = 2;
	const int SIZE_POS = 2;
	const int FULLWRITE_DATA_POS = 1;
	const int WRITE_PARAMETER_SIZE = 3;
	const int READ_PARAMETER_SIZE = 2;
	const int ERASE_PARAMETER_SIZE = 3;
	const int EXIT_PARAMETER_SIZE = 1;
	const int HELP_PARAMETER_SIZE = 1;
	const int FLUSH_PARAMETER_SIZE = 1;
	const int FULLWRITE_PARAMETER_SIZE = 2;
	const int FULLREAD_PARAMETER_SIZE = 1;
	const int PASS = 0;
	const int FAIL = 1;
	const int MAX_SIZE = 100;
	const int CHUNK_SIZE = 10;

	Shell(ISSDAdapter* ISSDAdapter);
	void executeShell(void);
	void setCommand(std::string command);

protected:
	ISSDAdapter* m_ISSDAdapter;
	TestScript* m_TestScript;
	std::string input;
	std::vector<std::string> parameter;
	int LBA;
	int data;
	int LBASize;
	int endLBA;
	

private:
	void splitAndStore(void);
	bool noEnterCommand(void);
	bool writeApi(void);
	bool readApi(void);
	bool eraseApi(void);
	void splitErase(void);
	bool eraseRangeApi(void);
	bool exitApi(void);
	bool helpApi(void);
	bool flushApi(void);
	bool fullwriteApi(void);
	bool fullreadApi(void);
	bool isValidParameterSize(const int size);
	bool isValidLBA(const int pos);
	bool isValidData(const int pos);
	bool isValidSize(const int pos);
	void storeLBA(void);
	void storeData(const int pos);
	void storeSize(void);
	void storeLBARange(void);
};