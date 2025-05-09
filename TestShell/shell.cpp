#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Shell {
public:
	void executeShell(void) {
		string input;
		while (input != "exit") {
			cout << "Shell> ";
			getline(std::cin, input);

			stringstream ss(input);
			string cmd;
			ss >> cmd;

			// command valid check
			if (cmd != "write" || cmd != "read" || cmd != "exit" || cmd != "help" || cmd != "fullwrite" || cmd != "fullread") {
				cout << "INVALID COMMAND" << endl;
				continue;
			}

			if (cmd == "write") { // write 3 0xAAAABBBB
				// command argument valid check

			}
			else if (cmd == "read") { // read 2

			}
			else if (cmd == "exit") { // exit
				break;
			}
			else if (cmd == "help") { // help AmazingReviewer jungyeonKim

			}
			else if (cmd == "fullwrite") { // fullwrite 0xAAAABBBB

			}
			else if (cmd == "fullread") { // fullread 

			}
			else {
				cout << "INVALID COMMAND" << endl;
			}
		}
	}
};