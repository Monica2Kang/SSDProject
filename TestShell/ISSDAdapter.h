#pragma once
class ISSDAdapter {
public:
	virtual void writeLba(const int lba, const int data) = 0;
	virtual int readLba(const int lba) = 0;
	virtual void erase(const int lba, const int size) = 0;
	virtual void fullWrite(const int data) = 0;
	virtual void fullRead(void) = 0;
};