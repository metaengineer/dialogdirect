#include "ErrorStream.h"
#include <string>

const int NUM_STRINGS = 32;

CMessageCache::CMessageCache(HWND mon)
{
	blocks=new char *[NUM_STRINGS];
	lngs=new WORD[NUM_STRINGS];
	for(int s=0; s<NUM_STRINGS; s++)
	{
		blocks[s]=0;
		lngs[s]=0;
	}
	cursor=0;
	monitor=mon;
}

void CMessageCache::_Revolve()
{
	for(int s=1; s<NUM_STRINGS; s++)
	{
		blocks[s-1]=blocks[s];
		lngs[s-1]=lngs[s];
	}
}

void CMessageCache::_RebuildField()
{
	std::string rex="";
	for(int s=0; s<cursor; s++)
	{
		rex+=blocks[s];
	}
	LRESULT frrr=SendMessageA(monitor, WM_SETTEXT, 0, (LPARAM)(LPVOID)rex.c_str());

}

void CMessageCache::WriteToFile(HANDLE file)
{
	DWORD ho;
	for(int s=0; s<cursor; s++)
	{
		WriteFile(file, blocks[s], lngs[s], &ho, NULL);
		WriteFile(file,"\r\n", 2, &ho, NULL);
	}
}

void CMessageCache::DropMessage(LPCSTR dro, BYTE lvl)
{
	if(cursor<NUM_STRINGS)
	{
		size_t nt=strlen(dro)+1;
		blocks[cursor]=new char[WORD(nt)];
		strcpy_s(blocks[cursor], WORD(nt), dro);
		lngs[cursor]=WORD(nt);
		cursor++;
	}
	else
	{
		delete[] blocks[0];
		_Revolve();
		size_t nt=strlen(dro)+1;
		blocks[cursor]=new char[WORD(nt)];
		strcpy_s(blocks[cursor], WORD(nt), dro);
		lngs[cursor]=WORD(nt);
		// TODO: maybe this is too slow and needs better method
	}
	_RebuildField();
	if(lvl==3)
	{
		throw AutoStopException(cursor);
	}
}

CMessageCache::~CMessageCache()
{
	for(int s=0; s<NUM_STRINGS; s++)
	{
		if(blocks[s])
			delete[] blocks[s];
	}
	delete[] blocks;
	delete[] lngs;
}
