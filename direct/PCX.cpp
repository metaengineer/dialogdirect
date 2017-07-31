#include "PCX.h"
#include <new>

BOOL UtilLoadPCX(LPCSTR file, DWORD **pvData, LONG *ix, LONG *iy)
{
	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	PCXHeader rex;
	DWORD dwRead;
	if(!ReadFile(hFile, &rex, sizeof(rex), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<sizeof(rex))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	*iy=rex.YMax-rex.YMin+1;
	*ix=rex.XMax-rex.XMin+1;

	int iImageSize=(*iy)*rex.BytesPerLine;
	int PaletteSize=0;
	LONG sz=(*ix)*(*iy);

	BYTE *buf;
	try
	{
		buf=new BYTE[iImageSize+((rex.BitsPerPixel==8)?769:0)];
	}
	catch(std::bad_alloc a)
	{
		CloseHandle(hFile);
		return FALSE;
	}
	try
	{
		*pvData=new DWORD[sz];
	}
	catch(std::bad_alloc t)
	{
		delete[] buf;
		CloseHandle(hFile);
		return FALSE;
	}
	if(!ReadFile(hFile, buf, iImageSize, &dwRead, NULL))
	{
		delete[] buf;
		delete[] (*pvData);
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<iImageSize)
	{
		delete[] buf;
		delete[] (*pvData);
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}

	switch(rex.BitsPerPixel)
	{
	case 1:
	{
		BYTE ou;
		BYTE ow;
		DWORD *ff=*pvData;
		for(LONG s=0; s<(sz+sz%8); s+=8)
		{
			ou=buf[s/8];
			for(int fq=0; fq<8; fq++)
			{
				ow=(ou&(1<<fq))?3:0;
				ff[s+fq]=MAKELONG(MAKEWORD(rex.Palette[ow+2],rex.Palette[ow+1]),
					MAKEWORD(rex.Palette[ow],0));
			}
		}
		break;
	}
	case 4:
	{
		BYTE ou;
		BYTE ow;
		DWORD *ff=*pvData;
		for(LONG s=0; s<(sz+sz%2); s+=2)
		{
			ou=buf[s/2];
			ow=LOBYTE(ou);
			ff[s]=MAKELONG(MAKEWORD(rex.Palette[ow],rex.Palette[ow+1]),
				MAKEWORD(rex.Palette[ow+2],0));
			ow=HIBYTE(ou);
			ff[s+1]=MAKELONG(MAKEWORD(rex.Palette[ow],rex.Palette[ow+1]),
				MAKEWORD(rex.Palette[ow+2],0));
		}
		break;
	}
	case 8:
	{
		BYTE *bSurf=buf+dwRead;
		if(!ReadFile(hFile, bSurf, 769, &dwRead, NULL))
		{
			delete[] buf;
			delete[] (*pvData);
			*pvData=NULL;
			CloseHandle(hFile);
			return FALSE;
		}
		if(dwRead<769)
		{
			delete[] buf;
			delete[] (*pvData);
			*pvData=NULL;
			CloseHandle(hFile);
			return FALSE;
		}
		BYTE rj;
		for(LONG s=0; s<sz; s++)
		{
			rj=buf[s]*3;
			(*pvData)[s]=MAKELONG(MAKEWORD(bSurf[rj+3],bSurf[rj+2]),MAKEWORD(bSurf[rj+1],0));
		}
		break;
	}
	case 24:
	{
		BYTE *bSurf=buf;
		for(LONG h=0; h<sz; h++)
		{
			(*pvData)[h]=MAKELONG(MAKEWORD(bSurf[0],bSurf[1]),MAKEWORD(bSurf[2],0));
			bSurf+=3;
		}
		break;
	}
	}

	delete [] buf;
	CloseHandle(hFile);
	return TRUE;
}

BOOL UtilSavePCX(LPCSTR file, DWORD *pDataRgb, LONG ix, LONG iy, int bytes)
{
	return FALSE;
}
