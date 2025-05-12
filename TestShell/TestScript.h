#pragma once

#include "ISSDAdapter.h"
#include "SSDAdapter.h"

static const int PASS = 0;
static const int FAIL = 1;

class TestScript {
public:
	TestScript() = default;
	TestScript(ISSDAdapter* obj) {
		ssdAdapter = obj;
	}

	int FullWriteAndReadCompare(int data);
	int PartialLBAWrite(int data);
	int WriteReadAging(void);

private:
	ISSDAdapter* ssdAdapter;
};