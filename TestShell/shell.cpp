#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include "shell.h"

using namespace std;

Shell::Shell(ISSDAdapter* ISSDAdapter) : m_ISSDAdapter(ISSDAdapter)
{
	m_TestScript = new TestScript(m_ISSDAdapter);
}

#ifndef _DEBUG
void Shell::executeShell(void) {
	while(true) {
		setCommand("");

		splitAndStore();

		if (noEnterCommand()) {
			cout << "INVALID COMMAND" << endl;
			cout << endl;
			continue;
		}

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
		else if (parameter[COMMAND_POS] == "1_" || parameter[COMMAND_POS] == "1_FullWriteAndReadCompare") {
			const int expectedData = 0xBEEFCAFE;
			if (!(m_TestScript->FullWriteAndReadCompare(expectedData))) {
				cout << "PASS\n" << endl;
				continue;
			}
			cout << "FAIL\n" << endl;
			continue;
		}
		else if (parameter[COMMAND_POS] == "2_" || parameter[COMMAND_POS] == "2_PartialLBAWrite") {
			const int expectedData = 0xBEEFCAFE;
			if (!(m_TestScript->PartialLBAWrite(expectedData))) {
				cout << "PASS\n" << endl;
				continue;
			}
			cout << "FAIL\n" << endl;
			continue;
		}
		else if (parameter[COMMAND_POS] == "3_" || parameter[COMMAND_POS] == "3_WriteReadAging") {
			if (!(m_TestScript->WriteReadAging())) {
				cout << "PASS\n" << endl;
				continue;
			}
			cout << "FAIL\n" << endl;
			continue;
		}

		cout << "INVALID COMMAND" << endl;
		cout << endl;
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
		return;
	}

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
	else if (parameter[COMMAND_POS] == "1_" || parameter[COMMAND_POS] == "1_FullWriteAndReadCompare") {
		const int expectedData = 0xBEEFCAFE;
		if (!(m_TestScript->FullWriteAndReadCompare(expectedData))) { 
			cout << "PASS\n" << endl;
			return; 
		}
		cout << "FAIL\n" << endl;
		return;
	}
	else if (parameter[COMMAND_POS] == "2_" || parameter[COMMAND_POS] == "2_PartialLBAWrite") {
		const int expectedData = 0xBEEFCAFE;
		if (!(m_TestScript->PartialLBAWrite(expectedData))) { 
			cout << "PASS\n" << endl;
			return; 
		}
		cout << "FAIL\n" << endl;
		return;
	}
	else if (parameter[COMMAND_POS] == "3_" || parameter[COMMAND_POS] == "3_WriteReadAging") {
		if (!(m_TestScript->WriteReadAging())) { 
			cout << "PASS\n" << endl;
			return;
		}
		cout << "FAIL\n" << endl;
		return;
	}
	cout << "INVALID COMMAND" << endl;
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
			cout << "[Read] LBA " << LBA << " : 0x" << uppercase << setfill('0') << setw(8) << hex << printData << endl;
			cout << endl;
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
			return true;
		}
	}
	return false;
}

bool Shell::exitApi(void) {
	if (isValidParameterSize(EXIT_PARAMETER_SIZE)) {
		return true;
	}
	return false;
}

bool Shell::helpApi(void) {
	if (isValidParameterSize(HELP_PARAMETER_SIZE)) {
		cout << "Team : AmazingReviewer" << endl;
		cout << "Minju Kang, Namwook Kang, Janghwan Kim, Jungyeon Kim" << endl;
		cout << "write > write [LBA] [data]" << endl;
		cout << "read > read [LBA]" << endl;
		cout << "erase > erase [LBA] [SIZE]" << endl;
		cout << "exit > End the program" << endl;
		cout << "help > Show command guide" << endl;
		cout << "fullwrite > fullwrite[data]" << endl;
		cout << "fullread > fullread" << endl;
		cout << endl;
		return true;
	}
	return false;
}

bool Shell::flushApi(void) {
	if (isValidParameterSize(FLUSH_PARAMETER_SIZE)) {
		// m_ISSDAdapter->flush();
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
			return true;
		}
	}
	return false;
}

bool Shell::fullreadApi(void) {
	if (isValidParameterSize(FULLREAD_PARAMETER_SIZE)) {
		m_ISSDAdapter->fullRead();
		cout << endl;
		return true;
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
		for (char ch : str) {
			if (isdigit(ch)) {
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
	int tempSize = stoi(parameter[SIZE_POS]); // 1 ~ INF
	int totalSize = LBA + tempSize;

	if (totalSize > MAX_SIZE) {
		LBASize = MAX_SIZE - LBA;
	}
	else { // totalSize <= MAX_SIZE
		LBASize = tempSize;
	}
}

void Shell::storeLBARange(void) {
	LBA = stoi(parameter[LBA_POS]);
	endLBA = stoi(parameter[END_LBA_POS]);
	LBASize = endLBA - LBA + 1;
}