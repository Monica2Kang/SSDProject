#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <string>
#include <cctype>
#include <iomanip>
#include "shell.h"

using namespace std;

Shell::Shell(ISSDAdapter* ISSDAdapter) : m_ISSDAdapter(ISSDAdapter)
{
}

void Shell::executeShell(void) {
	// while(true){
		// setCommand();

	splitAndStore();

	if (noEnterCommand()) {
		cout << "INVALID COMMAND" << endl;
		return;
		//continue;
	}

	if (parameter[COMMAND_POS] == "write") { // write 3 0xAAAABBBB
		if (writeApi()) //continue;
			return;
	}
	else if (parameter[COMMAND_POS] == "read") { // read 2
		if (readApi()) //continue;
			return;
	}
	else if (parameter[COMMAND_POS] == "exit") { // exit
		if (exitApi()) //break;
			return;
	}
	else if (parameter[COMMAND_POS] == "help") { // help AmazingReviewer jungyeonKim
		if (helpApi()) //continue;
			return;
	}
	else if (parameter[COMMAND_POS] == "fullwrite") { // fullwrite 0xAAAABBBB
		if (fullwriteApi()) //continue;
			return;
	}
	else if (parameter[COMMAND_POS] == "fullread") { // fullread 
		if (fullreadApi()) //continue;
			return;
	}
	else if (parameter[COMMAND_POS] == "1_" || parameter[COMMAND_POS] == "1_FullWriteAndReadCompare") {

	}
	else if (parameter[COMMAND_POS] == "2_" || parameter[COMMAND_POS] == "2_PartialLBAWrite") {

	}
	else if (parameter[COMMAND_POS] == "3_" || parameter[COMMAND_POS] == "3_WriteReadAging") {

	}
	cout << "INVALID COMMAND" << endl;
	//}
}

void Shell::setCommand(string command)
{
	cout << "Shell> ";
	input = command;
	// getline(std::cin, input);
}

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
		cout << "exit > End the program" << endl;
		cout << "help > Show command guide" << endl;
		cout << "fullwrite [data]" << endl;
		cout << "fullread" << endl;
		return true;
	}
	return true;
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
	if (parameter[pos].empty() || parameter[pos].length() > 2) {
		return false;
	}

	for (char ch : parameter[pos]) {
		if (!isdigit(ch)) {
			return false;
		}
	}

	return true;
}

bool Shell::isValidData(const int pos) {
	const string str = parameter[pos];

	if (str.empty() || str.length() != DATA_LENGTH) {
		return false;
	}

	if (!(str[0] == '0' && str[1] == 'x')) {
		return false;
	}

	for (int i = 2; i < DATA_LENGTH; i++) {
		if (!((str[i] >= 'A' && str[i] <= 'F') || (str[i] >= '0' && str[i] <= '9'))) {
			return false;
		}
	}

	return true;
}

void Shell::storeLBA(void) {
	LBA = stoi(parameter[LBA_POS]);
}

void Shell::storeData(const int pos) {
	data = static_cast<int>(stoul(parameter[pos], nullptr, 16));
}