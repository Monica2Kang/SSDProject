#include "gmock/gmock.h"
#include "SSDCmdParser.h"
#include "SSDDevice.h"
#include <string>

using std::string;

int main(int argc, const char* argv[]) {
#ifndef _DEBUG
	SSDDevice device;
	SSDCmdParser parser{device};
	parser.checkParsing(argc, argv);
	//std::cout << result << "\n";
	return 0;
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif // !_DEBUG
}