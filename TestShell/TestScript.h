#pragma once

#include "SSDAdapter.h"

static const int PASS = 0;
static const int FAIL = 1;

class TestScript {
public:
	int FullWriteAndReadCompare(void);
	int PartialLBAWrite(void);
	int WriteReadAging(void);

private:
	SSDAdapter ssdAdapter;
};