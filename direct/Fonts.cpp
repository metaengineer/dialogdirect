#include "Fonts.h"

CMenuFont::CMenuFont()
{
	WithFont=0;
	OwnFont=CreateFont(16, 12, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
	if(OwnFont)
		WithFont=1;
}

CMenuFont::CMenuFont(LPCSTR fontname)
{
	WithFont=0;
	OwnFont=CreateFont(16, 12, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, fontname);
	if(OwnFont)
		WithFont=1;
}

DWORD CMenuFont::Check()
{
	return WithFont;
}

HFONT CMenuFont::Get()
{
	return OwnFont;
}

CMenuFont::~CMenuFont()
{
	if(WithFont&1)
		DeleteObject(OwnFont);
}
