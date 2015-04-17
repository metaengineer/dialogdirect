#include "stdafx.h"

#ifndef UTIL_SCRIPTPARSER_H
#define UTIL_SCRIPTPARSER_H

class CAppTextScriptReader
{
private:
	HANDLE hFile;
	BOOL IsActive;
	BOOL _ExpectLetter(char lt);
public:
	CAppTextScriptReader();
	BOOL InitTextScript(LPCSTR file);
	BOOL ReadByte(CHAR *buffer, DWORD *numbuffer);
	void MoveNextWord();
	ULONG PreRecordIdentifier();
	ULONG PreRecordIdentifierBraces(char brace);
	void RecordIdentifier(char *buf, ULONG chsize);
	void RecordIdentifierBraces(char *buf, ULONG chsize, char brace);
	
	BOOL ExpectWord(LPCSTR word, int count);
	LONG ReadWholeNumber();
	double ReadDouble(int precision);

	~CAppTextScriptReader();
};

#endif