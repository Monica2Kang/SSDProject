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

	int FullWriteAndReadCompare(const int data);
	int PartialLBAWrite(const int data);
	int WriteReadAging(void);

private:
	ISSDAdapter* ssdAdapter;
};