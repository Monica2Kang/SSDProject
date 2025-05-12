#pragma once

static const int PASS = 0;
static const int FAIL = 1;

class TestScript {
public:
	int FullWriteAndReadCompare(void) {
		return PASS;
	}

	int PartialLBAWrite(void) {
		return PASS;
	}

	int WriteReadAging(void) {
		return PASS;
	}
};