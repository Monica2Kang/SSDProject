#include "SSDCmdParser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

string SSDCmdParser::checkParsing(int argc, const char* argv[]) const {
    //make tokens
    std::vector<std::string> tokens;
    for (int i = 0; i < argc; ++i) {
        tokens.emplace_back(argv[i]);
    }

    //parse each tokens
    string ssd = tokens[0];
    if (ssd != "SSD.exe") return "ASSERT";

    string command = tokens[1];
    if (!(command == "R" || command == "W")) return "ASSERT";

    unsigned int LBA = std::stoi(tokens[2]);
    if (LBA > 99) return "ERROR";

    if (command == "W") {
        return "";
    }

    //if(command == "R")
    return "0x00000000"; //case : no data
}