#ifndef UTIL_SCRIPTPARSER_H
#define UTIL_SCRIPTPARSER_H

#include <windows.h>

class CAppTextScriptReader
{
private:
	HANDLE hFile;
	BOOL IsActive;
public:
	BOOL _ExpectLetter(char lt);
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
