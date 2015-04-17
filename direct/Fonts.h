#include "stdafx.h"
#include "wingdi.h"

#ifndef UTIL_FONTS_H
#define UTIL_FONTS_H

class CMenuFont
{
private:
	HFONT OwnFont;
	DWORD WithFont;
public:
	CMenuFont();
	CMenuFont(LPCSTR fontname);
	DWORD Check();
	HFONT Get();
	~CMenuFont();
};

#endif