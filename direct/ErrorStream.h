#include "common.h"
#include "autostop.h"

#ifndef RLMRUNNER_ERROR_STREAM
#define RLMRUNNER_ERROR_STREAM

class CMessageCache
{
private:
	char **blocks;
	WORD *lngs;
	int cursor;
	HWND monitor;
	void _Revolve();
	void _RebuildField();
public:
	CMessageCache(HWND mon);
	void WriteToFile(HANDLE file);
	void DropMessage(LPCSTR dro, BYTE lvl);
	~CMessageCache();
};

#endif
