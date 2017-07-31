#include "stdio.h"

// File writer classes

#ifndef HANDLER_H
#define HANDLER_H

struct FloatTriplet
{
	float x;
	float y;
	float z;
};

class CInHandler
{
private:
	bool FileEnabledR;
	HANDLE LandInFile;
public:
	CInHandler();
	bool CInInit(LPCSTR);
	bool FReadByte(char *buffer, DWORD *numbuffer);
	int FReadWholeNumber();
	FloatTriplet FReadTriplex();
	~CInHandler();
};

class COutHandler
{
private:
	HWND ErrorMsgOwnership;
	bool FileEnabledW;
	DWORD writeresult;
	HANDLE LandOutFile;
public:
	COutHandler(HWND ErrorReturn);
	bool COutInitCreate(LPCSTR);
	bool COutInitOpen(LPCSTR);
	DWORD GetWriteResult();
	bool FWriteByte(const char *buffer, DWORD num);
	bool FWriteWholeNumber(long long indata);
	bool FWriteWholeNumberP(long long indata);
	bool FWriteNumberFixedPoint(float indatafl, unsigned int multiply);
	bool FWriteTriplex(int x, int y, int z);
	bool FWriteTriplexFixedPoint(float x, float y, float z, unsigned int multiply);
	bool FWriteFiveFixedPoint(float x, float y, float z, float tx1, float tx2, unsigned int multiply);
	~COutHandler();
};

#endif
