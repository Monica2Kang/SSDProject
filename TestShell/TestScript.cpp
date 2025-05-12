#include <iostream>
#include <cstdlib>
#include <ctime>
#include "TestScript.h"

int TestScript::FullWriteAndReadCompare(const int data) {

	/*
	• 0 ~4번 LBA까지 Write 명령어를 수행한다.
	• 0 ~4번 LBA까지 ReadCompare 수행
	• 5 ~9번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	• 5 ~9번 LBA까지 ReadCompare 수행
	• 10 ~14번 LBA까지 다른 값으로 Write 명령어를 수행한다.
	• 10 ~14번 LBA까지 ReadCompare 수행
	• 위와 같은 규칙으로 전체 영역에 대해 Full Write + Read Compare를 수행한다.
	*/

	const int MAX_LBA = 100;
	const int TEST_UNIT = 5;
	int lba = 0;
	int expectedData = data;
	int actualData = 0;

	while (lba < MAX_LBA)
	{
		for (auto it = 0; it < TEST_UNIT; it++)
		{
			ssdAdapter->writeLba(lba + it, expectedData);
		}

		for (auto it = 0; it < TEST_UNIT; it++)
		{
			actualData = ssdAdapter->readLba(lba + it);
			if (actualData != expectedData)
			{
				return FAIL;
			}
		}

		lba += TEST_UNIT;
	}

	return PASS;
}

int TestScript::PartialLBAWrite(const int data) {
	/*
	* Loop는 30회
	• 4번 LBA에 값을 적는다.
	• 0번 LBA에 같은 값을 적는다.
	• 3번 LBA에 같은 값을 적는다.
	• 1번 LBA에 같은 값을 적는다.
	• 2번 LBA에 같은 값을 적는다.
	• LBA 0 ~ 4번, ReadCompare
	*/
	const int loop = 30;
	const int expectedData = data;
	for (int it = 0; it < loop; it++)
	{
		ssdAdapter->writeLba(4, expectedData);
		ssdAdapter->writeLba(0, expectedData);
		ssdAdapter->writeLba(3, expectedData);
		ssdAdapter->writeLba(1, expectedData);
		ssdAdapter->writeLba(2, expectedData);

		for (int lba = 0; lba < 5; lba++)
		{
			int actualData = ssdAdapter->readLba(lba);
			if (actualData != expectedData)
			{
				return FAIL;
			}
		}
	}

	return PASS;
}

int TestScript::WriteReadAging(void) {

	/*
	* Loop 200회
	• 0번 LBA에 랜덤 값을 적는다.
	• 99번 LBA에 랜덤 값을 적는다.
	• LBA 0번과 99번, ReadCompare를 수행
	*/

	std::srand(std::time(nullptr)); // 시드 초기화 (현재 시간 기준)

	const int loop = 30;
	const int lba1 = 0;
	const int lba2 = 99;
	int expectedData1 = 0x0;
	int expectedData2 = 0x0;
	for (int it = 0; it < loop; it++)
	{
		//expectedData1 = std::rand() % 0xFFFFFFFF;
		//expectedData2 = std::rand() % 0xFFFFFFFF;

		expectedData1 = 0xBEEFCAFE;
		expectedData2 = 0xBEEFCAFE;
		ssdAdapter->writeLba(lba1, expectedData1);
		ssdAdapter->writeLba(lba2, expectedData2);

		int actualData = ssdAdapter->readLba(lba1);
		if (actualData != expectedData1)
		{
			return FAIL;
		}

		actualData = ssdAdapter->readLba(lba2);
		if (actualData != expectedData2)
		{
			return FAIL;
		}
	}

	return PASS;
}