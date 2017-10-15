#include "common.h"
#include "PCX.h"
#include "TGA.h"

#ifndef UTIL_BITMAPS_H
#define UTIL_BITMAPS_H

BOOL UtilLoadBitmap(LPCSTR path, DWORD **ppvData, LONG *pImagex, LONG *pImagey);
BOOL UtilSaveBitmap(LPCSTR path, DWORD *pDataRgb, LONG ix, LONG iy, int SaveFmt);

class CBitmapTexture
{
private:
	UINT flags;
	DWORD *data1;
	LONG imagex;
	LONG imagey;
	HBITMAP OwnBitmap;
public:
	CBitmapTexture(LPCSTR FilePath, UINT format);
	CBitmapTexture(LPCSTR FileNoExt); // any format
	UINT GetInfo();
        const DWORD *GetData();
	LONG GetX();
	LONG GetY();
	HBITMAP GetBits();
	~CBitmapTexture();
};

#endif
