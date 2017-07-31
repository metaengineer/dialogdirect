#ifndef IMAGECLASSES_PCX_H
#define IMAGECLASSES_PCX_H

struct PCXHeader
{
	BYTE Manufacturer;
	BYTE Version;
	BYTE Encoding;
	BYTE BitsPerPixel;
	WORD XMin;
	WORD YMin;
	WORD XMax;
	WORD YMax;
	WORD VertDPI;
	BYTE Palette[48];
	BYTE Reserved;
	BYTE ColorPlanes;
	WORD PaletteType;
	WORD BytesPerLine;
	WORD HScrSize;
	WORD VScrSize;
	BYTE Filler[56];
};

BOOL UtilLoadPCX(LPCSTR file, DWORD **pvData, LONG *ix, LONG *iy);
BOOL UtilSavePCX(LPCSTR file, DWORD *pDataRgb, LONG ix, LONG iy, int bytes);

#endif
