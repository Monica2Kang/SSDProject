#include "gmock/gmock.h"
#include "SSDCmdParser.h"
#include "SSDDevice.h"
//#include "SSDCmdBuffer.h"
#include <string>

using std::string;

int main(int argc, const char* argv[]) {
#ifndef _DEBUG
	SSDCmdParser parser;
	parser.checkParsing(argc, argv);
	return 0;
#else
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#endif // !_DEBUG
}