#include "Parser.h"

CAppTextScriptReader::CAppTextScriptReader()
{
	hFile=NULL;
	IsActive=FALSE;
}

BOOL CAppTextScriptReader::InitTextScript(LPCSTR file)
{
	if(IsActive)
		return FALSE;
	hFile=CreateFileA(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	IsActive=TRUE;
	return TRUE;
}

BOOL CAppTextScriptReader::ReadByte(CHAR *buffer, DWORD *numbuffer)
{
	if(!IsActive)
	{
		*buffer=0;
		*numbuffer=0;
		return false;
	}
	void *primarybuffer = buffer;
	if(!ReadFile(hFile, primarybuffer, 1, numbuffer, NULL))
		return FALSE;
	if(*numbuffer==0)
		return FALSE;
	return true;
}

BOOL CAppTextScriptReader::_ExpectLetter(char lt)
{
	if(!IsActive)
		return false;
	char *buffer;
	DWORD *numbuffer;
	buffer = new char;
	*buffer=0;
	numbuffer = new DWORD;
	*numbuffer = 0;
	if(ReadFile(hFile, buffer, 1, numbuffer, NULL))
	{
		if(*numbuffer==0)
		{
			delete buffer;
			delete numbuffer;
			return false;
		}
		if(*buffer==lt)
		{
			delete buffer;
			delete numbuffer;
			return true;
		}
		else
		{
			delete buffer;
			delete numbuffer;
			return false;
		}
	}
	delete buffer;
	delete numbuffer;
	return false;
}

void CAppTextScriptReader::MoveNextWord()
{
	if(!IsActive)
	{
		return;
	}
	char *buffer;
	DWORD *numbuffer;
	buffer = new char;
	numbuffer = new DWORD;
	*numbuffer = 1;
	bool itcontinues = true;
	bool effect = false;
	while(itcontinues)
	{
		if(!ReadFile(hFile, buffer, 1, numbuffer, NULL))
		{
			itcontinues=false;
		}
		if(*numbuffer==0)
		{
			itcontinues=false;
		}
		else
			switch(int(*buffer))
			{
			case 0:
			case 9:
			case 10:
			case 13:
			case 32:
				break;
			default:
				effect=true;
				itcontinues=false;
			}
	}
	delete buffer;
	delete numbuffer;
	if(effect)
		SetFilePointer(hFile, -1, NULL, FILE_CURRENT);
}

BOOL CAppTextScriptReader::ExpectWord(LPCSTR word, int count)
{
	if(!IsActive)
		return false;
	MoveNextWord();
	int i;
	bool accumR = true;
	bool unu = true;
	for(i=0; i<count; i++)
	{
		unu = _ExpectLetter(word[i]);
		accumR = accumR && unu;
	}
	if(!accumR)
		SetFilePointer(hFile, -count, NULL, FILE_CURRENT);
	return accumR;
}

LONG CAppTextScriptReader::ReadWholeNumber()
{
	if(!IsActive)
		return -1;
	MoveNextWord();
	char *buffer;
	DWORD *numbuffer;
	numbuffer = new DWORD;
	*numbuffer = 1;
	char justred;
	bool esho_ne_vfsio = true;
	bool digit=false;
	int ivalue = 0;
	int evalue = 0;
	buffer = new char;
	while(esho_ne_vfsio)
	{
		if(!ReadFile(hFile, buffer, 1, numbuffer, NULL))
			break;
		if(!(*numbuffer))
			break;
		justred = *buffer;
		esho_ne_vfsio = (int(justred)!=32)&&(int(justred)!=10)&&(int(justred)!=13)&&(int(justred)!=9);
		if(esho_ne_vfsio&&((int(justred)>=48)&&(int(justred)<=57)))
		{
			evalue = evalue*10;
			ivalue = int(justred)-48;
			evalue = evalue + ivalue;
			digit=true;
		}
		else
			break;
	}
	delete buffer;
	delete numbuffer;
	if(!digit)
		return (-1);
	return evalue;
}

double CAppTextScriptReader::ReadDouble(int precision)
{
	if(!IsActive)
		return -1;
	MoveNextWord();

	//first part declarations
	char *buffer;
	DWORD *numbuffer;
	char justred;
	bool esho_ne_vfsio = true;
	bool esho_ne_tochka = true;
	int ivalue = 0;
	int evalue = 0;
	bool bNegative=false;

	//second part declarations
	char *precbuffer;
	DWORD *precnumbuffer;
	char precjustred;
	int schiotchik;
	int ppalue;
	int pvalue = 0;
	bool readfurther;

	//first part initializations
	numbuffer = new DWORD;
	*numbuffer = 1;
	buffer = new char;

	while(esho_ne_vfsio&&esho_ne_tochka)
	{
		if(!ReadFile(hFile, buffer, 1, numbuffer, NULL))
		{
			delete buffer;
			delete numbuffer;
			return float(evalue);
		}
		if(*numbuffer==0)
		{
			delete buffer;
			delete numbuffer;
			return float(evalue);
		}
		justred = *buffer;
		esho_ne_vfsio = (int(justred)!=32)&&(int(justred)!=10)&&(int(justred)!=13)&&(int(justred)!=9);
		esho_ne_tochka = (int(justred)!=46);
		if(int(justred)==45)
			bNegative=true;
		if((esho_ne_vfsio&&esho_ne_tochka)&&((int(justred)>=48)&&(int(justred)<=57)))
		{
			evalue*=10;
			ivalue = int(justred)-48;
			evalue+=ivalue;
		}
		else
			if(int(justred)!=45)
				break;
	}
	if(!esho_ne_tochka)
	{
		//second part initializations
		precbuffer = new char;
		precnumbuffer = new DWORD;
		*precnumbuffer = 1;
		schiotchik=1;
		ppalue=0;
		readfurther=true;

		while(schiotchik<=precision)
		{
			if(readfurther)
			{
				if(!ReadFile(hFile, precbuffer, 1, precnumbuffer, NULL))
				{
					delete precbuffer;
					delete precnumbuffer;
					return float(evalue);
				}
				if(*precnumbuffer==0)
				{
					delete precbuffer;
					delete precnumbuffer;
					return float(evalue);
				}
				precjustred = *precbuffer;
			}
			if(readfurther&&((int(precjustred)>=48)&&(int(precjustred)<=57)))
			{
				//a digit was acquired
				pvalue*=10;
				ppalue = int(precjustred)-48;
				pvalue+=ppalue;
				schiotchik++;
			}
			else
			{
				readfurther=false;
				pvalue*=10;
				schiotchik++;
			}
		}
		delete precbuffer;
		delete precnumbuffer;
	}
	int schiot;
	int invmultiply=1;
	for(schiot=1; schiot<=precision; schiot++)
		invmultiply*=10;
	delete buffer;
	delete numbuffer;
	if(bNegative)
		return (-evalue-(float(pvalue)/invmultiply));
	else
		return (evalue+(float(pvalue)/invmultiply));
}

ULONG CAppTextScriptReader::PreRecordIdentifier()
{
	if(!IsActive)
		return 0;
	char checkup;
	int chk;
	bool chktrue = true;
	unsigned int length = 0;
	MoveNextWord();
	while(chktrue)
	{
		DWORD ram = 1;
		chktrue = ReadByte(&checkup, &ram);
		chk = int(checkup);
		if((chk==32)||(chk==10)||(chk==13)||(chk==9)||(chk==0))
			break;
		if(chk)
			length++;
	}
	SetFilePointer(hFile, -long(length+1), NULL, FILE_CURRENT);
	return length;
}

ULONG CAppTextScriptReader::PreRecordIdentifierBraces(char brace)
{
	if(!IsActive)
		return 0;
	char checkup;
	int chk;
	bool chktrue = true;
	unsigned int length = 0;
	while(chktrue)
	{
		DWORD ram = 1;
		chktrue = ReadByte(&checkup, &ram);
		chk = int(checkup);
		if((chk==10)||(chk==13)||(chk==9)||(chk==0)||(UINT(chk)==brace))
			break;
		if(chk)
			length++;
	}
	SetFilePointer(hFile, -long(length+1), NULL, FILE_CURRENT);
	return length;
}

void CAppTextScriptReader::RecordIdentifier(char *buf, ULONG chsize)
{
	if(!IsActive)
	{
		ZeroMemory(buf, chsize+1);
		return;
	}
	unsigned int aa;
	char ah;
	bool chk = true;
	for (aa=0; ((aa<chsize)&&chk); aa++)
	{
		DWORD ram = 1;
		chk=ReadByte(&ah,&ram);
		buf[aa]=ah;
	}
	buf[aa]='\0';
}

void CAppTextScriptReader::RecordIdentifierBraces(char *buf, ULONG chsize, char brace)
{
	if(!IsActive)
	{
		ZeroMemory(buf, chsize+1);
		return;
	}
	unsigned int aa;
	char ah;
	bool chk = true;
	for (aa=0; ((aa<chsize)&&chk); aa++)
	{
		DWORD ram = 1;
		chk=ReadByte(&ah,&ram);
		buf[aa]=ah;
	}
	buf[aa]='\0';
	_ExpectLetter(brace);
}

CAppTextScriptReader::~CAppTextScriptReader()
{
	if(IsActive)
		CloseHandle(hFile);
}
