#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include "TestShellLogger.h"
#include <algorithm>
#include "shell.h"

using namespace std;

Shell::Shell(ISSDAdapter* ISSDAdapter) : m_ISSDAdapter(ISSDAdapter)
{
	m_TestScript = new TestScript(m_ISSDAdapter);
}

#ifndef _DEBUG
void Shell::executeShell(void) {

	cout << "==============================================================" << endl;
	cout << "=        Test Shell (Team Amazing Reviewer)                  =" << endl;
	cout << "==============================================================" << endl;
	cout << endl;

	while(true) {
		setCommand("");

		splitAndStore();

		if (noEnterCommand()) {
			cout << "INVALID COMMAND" << endl;
			cout << endl;

			TEST_SHELL_LOG("INVALID COMMAND");
			continue;
		}

		TEST_SHELL_LOG("New Command " + parameter[COMMAND_POS]);

		if (parameter[COMMAND_POS] == "write") { // write 3 0xAAAABBBB
			if (writeApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "read") { // read 2
			if (readApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "erase") { // erase 2 1
			if (eraseApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "erase_range") { // erase_range 0 99
			if (eraseRangeApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "exit") { // exit
			if (exitApi()) break;
		}
		else if (parameter[COMMAND_POS] == "help") { // help AmazingReviewer jungyeonKim
			if (helpApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "flush") { // flush
			if (flushApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "fullwrite") { // fullwrite 0xAAAABBBB
			if (fullwriteApi()) continue;
		}
		else if (parameter[COMMAND_POS] == "fullread") { // fullread 
			if (fullreadApi()) continue;
		}
		else {
			if (testScriptApi()) {
				continue;
			}
		}

		cout << "INVALID COMMAND" << endl;
		cout << endl;

		TEST_SHELL_LOG("INVALID COMMAND");	
	}
}

void Shell::setCommand(string command)
{
	cout << "Shell> ";
	getline(std::cin, input);
}
#else
void Shell::executeShell(void) {

	splitAndStore();

	if (noEnterCommand()) {
		cout << "INVALID COMMAND" << endl;

		TEST_SHELL_LOG("INVALID COMMAND");
		return;
	}

	TEST_SHELL_LOG("New Command " + parameter[COMMAND_POS]);

	if (parameter[COMMAND_POS] == "write") { // write 3 0xAAAABBBB
		if (writeApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "read") { // read 2
		if (readApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "erase") { // erase 2 1
		if (eraseApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "erase_range") { // erase_range 0 99
		if (eraseRangeApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "exit") { // exit
		if (exitApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "help") { // help AmazingReviewer jungyeonKim
		if (helpApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "flush") { // flush
		if (flushApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "fullwrite") { // fullwrite 0xAAAABBBB
		if (fullwriteApi()) 
			return;
	}
	else if (parameter[COMMAND_POS] == "fullread") { // fullread 
		if (fullreadApi()) 
			return;
	}
	else {
		if (testScriptApi()) {
			return;
		}
	}

	cout << "INVALID COMMAND" << endl;

	TEST_SHELL_LOG("INVALID COMMAND");
}

void Shell::setCommand(string command)
{
	cout << "Shell> ";
	input = command;
}
#endif

void Shell::splitAndStore(void) {
	parameter.clear();
	stringstream ss(input);
	string word;

	while (ss >> word) {
		parameter.push_back(word);
	}
}

bool Shell::noEnterCommand(void) {
	if (parameter.size() < 1 || parameter[0].empty()) {
		return true;
	}
	return false;
}

bool Shell::writeApi(void)
{
	if (isValidParameterSize(WRITE_PARAMETER_SIZE)) {
		if (isValidLBA(LBA_POS)) {
			storeLBA();
			if (isValidData(DATA_POS)) {
				storeData(DATA_POS);
				m_ISSDAdapter->writeLba(LBA, data);
				cout << "[Write] Done\n" << endl;

				TEST_SHELL_LOG("[Write] Done");
				return true;
			}
		}
	}
	return false;
}

bool Shell::readApi(void) {
	if (isValidParameterSize(READ_PARAMETER_SIZE)) {
		if (isValidLBA(LBA_POS)) {
			storeLBA();
			int printData = m_ISSDAdapter->readLba(LBA);
			cout << "[Read] LBA " << dec << LBA << " : 0x" << uppercase << setfill('0') << setw(8) << hex << printData << endl;
			cout << endl;

			TEST_SHELL_LOG("[Read] LBA");
			return true;
		}
	}
	return false;
}

bool Shell::eraseApi(void) {
	if (isValidParameterSize(ERASE_PARAMETER_SIZE)) {
		if (isValidLBA(LBA_POS)) {
			storeLBA();
			if (isValidSize(SIZE_POS)) {
				storeSize();
				splitErase();

				cout << "[Erase] Done\n" << endl;
				return true;
			}
		}
	}
	return false;
}

void Shell::splitErase(void)
{
	int start = LBA;
	int restSize = LBASize;

	while (restSize > 0) {
		int eraseSize = std::min(restSize, CHUNK_SIZE);
		m_ISSDAdapter->erase(start, eraseSize);

		start += eraseSize;
		restSize -= eraseSize;
	}
}

bool Shell::eraseRangeApi(void) {
	if (isValidParameterSize(ERASE_PARAMETER_SIZE)) {
		if (isValidLBA(LBA_POS) && isValidLBA(END_LBA_POS)) {
			storeLBARange();
			splitErase();

			cout << "[Erase Range] Done\n" << endl;
			TEST_SHELL_LOG("Erase Range");
			return true;
		}
	}
	return false;
}

bool Shell::exitApi(void) {
	if (isValidParameterSize(EXIT_PARAMETER_SIZE)) {

		fulleraseApi();

		cout << "Bye Bye. \n" << endl;
		return true;
	}
	return false;
}

bool Shell::helpApi(void) {
	if (isValidParameterSize(HELP_PARAMETER_SIZE)) {
		cout << "\n==============================================================" << endl;
		cout << "Test Shell (Help)" << endl;
		cout << "==============================================================" << endl;
		cout << "1) Team : AmazingReviewer" << endl;
		cout << "2) Team members : Minju Kang (Leader), Namwook Kang, Janghwan Kim, Jungyeon Kim" << endl;
		cout << "3) Test Shell Command" << endl;
		cout << "3-1) write > write [LBA] [data]" << endl;
		cout << "3-2) read > read [LBA]" << endl;
		cout << "3-3) erase > erase [LBA] [SIZE]" << endl;
		cout << "3-4) erase_range > erase_range [startLBA] [endLBA]" << endl;
		cout << "3-5) flush > Execute all commands in the command buffer" << endl;
		cout << "3-6) exit > End the program" << endl;
		cout << "3-7) help > Show command guide" << endl;
		cout << "3-8) fullwrite > fullwrite[data]" << endl;
		cout << "3-9) fullread > fullread" << endl;
		cout << "==============================================================" << endl;
		cout << "==============================================================" << endl;
		cout << endl;

		cout << "[Help] Done\n" << endl;
		TEST_SHELL_LOG("Help Done");
		return true;
	}
	return false;
}

bool Shell::flushApi(void) {
	if (isValidParameterSize(FLUSH_PARAMETER_SIZE)) {
		m_ISSDAdapter->flush();

		cout << "[Flush] Done\n" << endl;
		TEST_SHELL_LOG("Flush Done");
		return true;
	}
	return false;
}

bool Shell::fullwriteApi(void) {
	if (isValidParameterSize(FULLWRITE_PARAMETER_SIZE)) {
		if (isValidData(FULLWRITE_DATA_POS)) {
			storeData(FULLWRITE_DATA_POS);
			m_ISSDAdapter->fullWrite(data);

			cout << "[Fullwrite] Done\n" << endl;
			TEST_SHELL_LOG("Fullwrite Done");
			return true;
		}
	}
	return false;
}

bool Shell::fullreadApi(void) {
	if (isValidParameterSize(FULLREAD_PARAMETER_SIZE)) {
		m_ISSDAdapter->fullRead();

		cout << "[Fullread] Done\n" << endl;
		TEST_SHELL_LOG("Fullread Done");
		return true;
	}
	return false;
}

void Shell::fulleraseApi(void) {
	for (auto lba = 0; lba < MAX_SIZE; lba += CHUNK_SIZE)
	{
		m_ISSDAdapter->erase(lba, CHUNK_SIZE);
	}
}

bool Shell::testScriptApi(void) {
	if (isValidParameterSize(TEST_SCRIPT_PARAMETER_SIZE)) {
		if (m_TestScript->isValidScenarioName(parameter[COMMAND_POS])) {
			if (!(m_TestScript->runTest(parameter[COMMAND_POS]))) {
				cout << "PASS\n" << endl;
			}
			else{
				cout << "FAIL\n" << endl;
			}
			return true;
		}
	}
	return false;
}

bool Shell::isValidParameterSize(const int size) {
	if (parameter.size() == size) {
		return true;
	}
	return false;
}

bool Shell::isValidLBA(const int pos) {
	const string str = parameter[pos];
	int isDigitCount = 0;

	if (!(str.empty()) && str.length() <= 2) {
		for (char ch : str) {
			if (isdigit(ch)) {
				isDigitCount++;
			}
		}
	}

	if (str.length() == isDigitCount) {
		return true;
	}
	
	return false;
}

bool Shell::isValidData(const int pos) {
	const string str = parameter[pos];
	int hexTypeCount = 0;

	if (!str.empty() && str.length() == DATA_LENGTH) {
		if (str[0] == '0' && str[1] == 'x') {
			for (int i = 2; i < DATA_LENGTH; i++) {
				if ((str[i] >= 'A' && str[i] <= 'F') || (str[i] >= '0' && str[i] <= '9')) {
					hexTypeCount++;
				}
			}
		}
	}

	if (hexTypeCount == 8) {
		return true;
	}

	return false;
}

bool Shell::isValidSize(const int pos) {
	const string str = parameter[SIZE_POS];
	int isDigitCount = 0;

	if (!parameter[SIZE_POS].empty()) {
		for (int i = 0; i < str.length(); i++){
			char ch = str[i];
			if (isdigit(ch) || (ch == '-' && i == 0)) {
				isDigitCount++;
			}
		}
	}

	if (str.length() == isDigitCount) {
		int tempSize = stoi(str);
		if (tempSize != 0) {
			return true;
		}
	}

	return false;
}

void Shell::storeLBA(void) { // 0 ~ 99
	LBA = stoi(parameter[LBA_POS]);
}

void Shell::storeData(const int pos) {
	data = static_cast<int>(stoul(parameter[pos], nullptr, 16));
}

void Shell::storeSize(void) {
	int tempSize = stoi(parameter[SIZE_POS]); // -INF ~ -1 || 1 ~ INF

	// -INF ~ -1
	if (tempSize < 0) {
		LBASize = abs(tempSize);
		LBA = std::max(0, LBA - LBASize + 1);
		return;
	}

	// 1 ~ INF (normal size case, exceeded size case )
	LBASize = std::min(tempSize, MAX_SIZE - LBA);
}

void Shell::storeLBARange(void) {
	LBA = stoi(parameter[LBA_POS]);
	endLBA = stoi(parameter[END_LBA_POS]);
	LBASize = endLBA - LBA + 1;
}