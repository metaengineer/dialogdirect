#include "stdafx.h"

#ifndef IMAGECLASSES_TARGA_H
#define IMAGECLASSES_TARGA_H

struct TargaHeaderA
{
	BYTE IdLength;
	BYTE HasIndices;
	BYTE Format;
	WORD StartIndex;
	BYTE NumColors;
	BYTE ColorBits;
	WORD XMin;
	WORD YMin;
	WORD XSize;
	WORD YSize;
	BYTE ImgDepth;
	BYTE AlphaMask;
};

BOOL UtilLoadTarga(LPCSTR file, DWORD **pvData, LONG *ix, LONG *iy);

#endif