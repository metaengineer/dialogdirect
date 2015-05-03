#include "stdafx.h"
#include "handler.h"
#include "stdio.h"

CInHandler::CInHandler()
{
	FileEnabledR = false;
}

bool CInHandler::CInInit(LPCSTR LandInString)
{
	LandInFile = CreateFile(LandInString,
							GENERIC_READ,
							0,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

	if (LandInFile == INVALID_HANDLE_VALUE) 
	{ 
		MessageBox(NULL, "Could not open file", "Error", MB_OK);
		return false;
	}
	FileEnabledR = true;
	return true;
}

bool CInHandler::FReadByte(char *buffer, DWORD *numbuffer)
{
	if(!FileEnabledR)
		return false;
	void *primarybuffer = buffer;
	if(ReadFile(LandInFile, primarybuffer, 1, numbuffer, NULL)==0)
		return false;
	return true;
}

int CInHandler::FReadWholeNumber()
{
	if(!FileEnabledR)
		return -1;
	char *buffer;
	DWORD *numbuffer;
	numbuffer = new DWORD;
	*numbuffer = 1;
	char justred;
	bool esho_ne_vfsio = true;
	int ivalue = 0;
	int evalue = 0;
	buffer = new char;
	while(esho_ne_vfsio)
	{
		if(ReadFile(LandInFile, buffer, 1, numbuffer, NULL)==0)
		{
			delete buffer;
			delete numbuffer;
			return evalue;
		}
		if(*numbuffer==0)
		{
			delete buffer;
			delete numbuffer;
			return -1;
		}
		justred = *buffer;
		esho_ne_vfsio = (int(justred)!=32)&&(int(justred)!=10)&&(int(justred)!=13)&&(int(justred)!=9);
		if(esho_ne_vfsio&&((int(justred)>=48)&&(int(justred)<=57)))
		{
			evalue = evalue*10;
			ivalue = int(justred)-48;
			evalue = evalue + ivalue;
		}
		else
			break;
	}
	return evalue;
}

FloatTriplet CInHandler::FReadTriplex()
{
	FloatTriplet xyzv;
	if(!FileEnabledR)
	{
		xyzv.x = -1;
		xyzv.y = -1;
		xyzv.z = -1;
		return xyzv;
	}
	xyzv.x = float(this->FReadWholeNumber());
	xyzv.y = float(this->FReadWholeNumber());
	xyzv.z = float(this->FReadWholeNumber());
	return xyzv;
}

CInHandler::~CInHandler()
{
	CloseHandle(LandInFile);
	FileEnabledR = false;
}


COutHandler::COutHandler(HWND ErrorReturn)
{
	FileEnabledW=false;
	ErrorMsgOwnership=ErrorReturn;
}

bool COutHandler::COutInitCreate(LPCSTR LandOutString)
{
	if(FileEnabledW)
		return false;
	LandOutFile = CreateFile(LandOutString,
							 GENERIC_WRITE,
							 0,
							 NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);

	if (LandOutFile == INVALID_HANDLE_VALUE) 
	{ 
		MessageBox(ErrorMsgOwnership, "Could not create file with specified path", "Handler", MB_OK | MB_ICONERROR);
		return false;
	}
	FileEnabledW=true;
	return true;
}

bool COutHandler::COutInitOpen(LPCSTR LandOutString)
{
	if(FileEnabledW)
		return false;
	LandOutFile = CreateFile(LandOutString,
							 GENERIC_WRITE,
							 0,
							 NULL,
							 OPEN_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);

	if (LandOutFile == INVALID_HANDLE_VALUE) 
	{ 
		MessageBox(ErrorMsgOwnership, "Could not open file for writing", "Handler", MB_OK | MB_ICONERROR);
		return false;
	}
	FileEnabledW=true;
	return true;
}

DWORD COutHandler::GetWriteResult()
{
	return writeresult;
}

bool COutHandler::FWriteByte(const char *buffer, DWORD num)
{
	if(!FileEnabledW)
		return false;
	if(WriteFile(LandOutFile, buffer, num, &writeresult, NULL)==0)
		return false;
	if(!writeresult)
		return false;
	return true;
}

bool COutHandler::FWriteWholeNumber(long long indata)
{
	if(!FileEnabledW)
		return false;
	bool bNegative=false;
	long long a=indata;
	if(a<0)
	{
		bNegative=true;
		a=-a;
	}
	int b=0;
	char *buffer;
	char *strint;
	buffer = new char;
	strint = new char[15];
	strint[14]='\0';
	int i=0;
	while(a)
	{
		b=a%10;
		a=a/10;
		strint[i]=char(b+48);
		if(i==13)
		{
			delete buffer;
			delete[] strint;
			return false;
		}
		i++;
	}
	if(indata==0)
	{
		strint[i]=char(48);
		i++;
	}
	bool netglukov=true;
	if(bNegative)
	{
		*buffer=char(45);
		if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
			netglukov=false;
		if(!writeresult)
			netglukov=false;
	}
	while(i>0)
	{
		i--;
		*buffer=strint[i];
		if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
			netglukov=false;
		if(!writeresult)
			netglukov=false;
	}
	*buffer=char(32);
	if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
		netglukov=false;
	delete(buffer);
	delete[] strint;
	if(!netglukov)
		return false;
	return true;
}

bool COutHandler::FWriteWholeNumberP(long long indata)
{
	bool k=FWriteWholeNumber(indata);
	FWriteByte("\r\n",2);
	return k;
}

bool COutHandler::FWriteNumberFixedPoint(float indatafl, unsigned int multiply)
{
	if(!FileEnabledW)
		return false;
	bool bNegative=false;
	if(multiply>5)
		return false;
	int mvalue=1;
	unsigned int ef;
	for (ef=1;ef<=multiply;ef++)
		mvalue=mvalue*10;
	int indata=int(indatafl*mvalue);
	if(indata<0)
	{
		bNegative=true;
		indata=-indata;
	}
	int a=indata;
	int b=0;
	char *buffer;
	char *strint;
	buffer = new char;
	strint = new char[15];
	strint[14]='\0';
	short int i=6-multiply;
	while(a)
	{
		b=a%10;
		a=a/10;
		strint[i]=char(b+48);
		if(i>13)
		{
			delete buffer;
			delete[] strint;
			return false;
		}
		i++;
	}
	while(i<13)
	{
		strint[i]=char(48);
		i++;
	}
	short int p=6;
	while(p<12)
	{
		strint[p+1]=strint[p];
		p++;
	}
	strint[6]=char(46);
	bool netglukov=true;
	if(bNegative)
	{
		*buffer='-';
		if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
			netglukov=false;
		if(!writeresult)
			netglukov=false;
	}
	while(i>0)
	{
		i--;
		*buffer=strint[i];
		if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
			netglukov=false;
		if(!writeresult)
			netglukov=false;
		if(i==(6-multiply))
			break;	
	}
	*buffer=char(32);
	if(!WriteFile(LandOutFile, buffer, 1, &writeresult, NULL))
		netglukov=false;
	delete buffer;
	delete[] strint;
	if(!netglukov)
		return false;
	return true;
}

bool COutHandler::FWriteTriplex(int x, int y, int z)
{
	if(!FileEnabledW)
		return false;
	SetFilePointer(LandOutFile, 0, NULL, FILE_END);
	if(!(this->FWriteWholeNumber(x)))
		return false;
	if(!(this->FWriteWholeNumber(y)))
		return false;
	if(!(this->FWriteWholeNumber(z)))
		return false;
	bool notnorm=false;
	char *buf1;
	buf1 = new char;
	*buf1=char(13);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	*buf1=char(10);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	delete(buf1);
	if(notnorm)
		return false;
	return true;
}

bool COutHandler::FWriteTriplexFixedPoint(float x, float y, float z, unsigned int multiply)
{
	if(!FileEnabledW)
		return false;
	SetFilePointer(LandOutFile, 0, NULL, FILE_END);
	FWriteByte("\t",1);
	if(!(this->FWriteNumberFixedPoint(x, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(y, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(z, multiply)))
		return false;
	bool notnorm=false;
	char *buf1;
	buf1 = new char;
	*buf1=char(13);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	*buf1=char(10);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	delete(buf1);
	if(notnorm)
		return false;
	return true;
}

bool COutHandler::FWriteFiveFixedPoint(float x, float y, float z, float tx1, float tx2, unsigned int multiply)
{
	if(!FileEnabledW)
		return false;
	SetFilePointer(LandOutFile, 0, NULL, FILE_END);
	FWriteByte("\t",1);
	if(!(this->FWriteNumberFixedPoint(x, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(y, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(z, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(tx1, multiply)))
		return false;
	if(!(this->FWriteNumberFixedPoint(tx2, multiply)))
		return false;
	bool notnorm=false;
	char *buf1;
	buf1 = new char;
	*buf1=char(13);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	*buf1=char(10);
	if(FAILED(this->FWriteByte(buf1, 1)))
		notnorm=true;
	delete(buf1);
	if(notnorm)
		return false;
	return true;
}

COutHandler::~COutHandler()
{
	FileEnabledW = false;
	CloseHandle(LandOutFile);
}
