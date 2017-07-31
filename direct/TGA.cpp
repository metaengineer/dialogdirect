#include "TGA.h"
#include <new>

BOOL UtilLoadTarga(LPCSTR file, DWORD **pvData, LONG *ix, LONG *iy)
{
	HANDLE hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	TargaHeaderA imhead;
	DWORD dwRead;
	if(!ReadFile(hFile, &imhead, sizeof(imhead), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<sizeof(imhead))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(imhead.IdLength)
		SetFilePointer(hFile, imhead.IdLength, NULL, FILE_CURRENT);
	BYTE *uui=NULL;
	UINT cmsz;
	if(imhead.HasIndices)
	{
		cmsz=(imhead.ColorBits>>3)*imhead.NumColors;
		try
		{
			uui=new BYTE[cmsz];
		}
		catch(std::bad_alloc t)
		{
			CloseHandle(hFile);
			return FALSE;
		}
		if(!ReadFile(hFile, uui, cmsz, &dwRead, NULL))
		{
			delete[] uui;
			uui=NULL;
			CloseHandle(hFile);
			return FALSE;
		}
		if(dwRead<cmsz)
		{
			delete[] uui;
			uui=NULL;
			CloseHandle(hFile);
			return FALSE;
		}
	}
	if((imhead.Format!=1)&&(imhead.Format!=2))
	{
		if(uui)
			delete[] uui;
		CloseHandle(hFile);
		return FALSE;
	}
	ULONG sz=imhead.XSize*imhead.YSize;
	try
	{
		*pvData=new DWORD[sz];
	}
	catch(std::bad_alloc a)
	{
		if(uui)
			delete[] uui;
		delete[] *pvData;
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}
	DWORD *ff=*pvData;
	BYTE yy=imhead.ImgDepth>>3;
	if(!ReadFile(hFile, ff, yy*sz, &dwRead, NULL))
	{
		if(uui)
			delete[] uui;
		delete[] ff;
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<yy*sz)
	{
		if(uui)
			delete[] uui;
		delete[] ff;
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}
	DWORD colo;
	DWORD *rrf;
	BYTE *raf=(BYTE *)(LPVOID)ff;
	try
	{
		rrf=new DWORD[sz];
	}
	catch(std::bad_alloc l)
	{
		if(uui)
			delete[] uui;
		delete[] ff;
		*pvData=NULL;
		CloseHandle(hFile);
		return FALSE;
	}
	*ix=imhead.XSize;
	*iy=imhead.YSize;
	LONG corr=0;
	LONG yc=*ix;
	switch(yy)
	{
	case 1:
		for(LONG hh=0; hh<sz; hh++)
		{
			if(corr==(*ix))
			{
				corr=0;
				yc+=(*ix);
			}
			colo=raf[hh];
			rrf[sz-yc+hh%(*ix)]=colo;
			corr++;
		}
		break;
	case 2:
		for(LONG hh=0; hh<(sz+sz); hh+=2)
		{
			if(corr==(*ix))
			{
				corr=0;
				yc+=(*ix);
			}
			colo=MAKEWORD(raf[hh],raf[hh+1]);
			rrf[sz-yc+(hh/2)%(*ix)]=colo;
			corr++;
		}
		break;
	case 3:
		for(LONG hh=0; hh<(sz*3); hh+=3)
		{
			if(corr==(*ix))
			{
				corr=0;
				yc+=(*ix);
			}
			colo=MAKELONG(MAKEWORD(0,raf[hh]),MAKEWORD(raf[hh+1],raf[hh+2]));
			rrf[sz-yc+(hh/3)%(*ix)]=colo;
			corr++;
		}
		break;
	case 4:
		for(LONG hh=0; hh<(sz*4); hh+=4)
		{
			if(corr==(*ix))
			{
				corr=0;
				yc+=(*ix);
			}
			colo=MAKELONG(MAKEWORD(raf[hh],raf[hh+1]),MAKEWORD(raf[hh+2],raf[hh+3]));
			rrf[sz-yc+(hh/4)%(*ix)]=colo;
			corr++;
		}
		break;
	}
	delete[] *pvData;
	*pvData=rrf;
	ff=*pvData;
	if(imhead.HasIndices)
	{
		switch(imhead.ColorBits)
		{
		case 8:
			{
				BYTE *iko=(BYTE *)(LPVOID)ff;
				BYTE er;
				for(ULONG jh=0; jh<(sz*4); jh+=4)
				{
					er=uui[iko[jh]-imhead.StartIndex];
					ff[jh/4]=MAKELONG(MAKEWORD(0,er),MAKEWORD(er,er));
				}
			}
			break;
		case 16:
			{
				WORD *iko=(WORD *)(LPVOID)ff;
				BYTE er;
				for(ULONG jh=0; jh<(sz*4); jh+=4)
				{
					er=iko[jh]-imhead.StartIndex;
					ff[jh/4]=MAKEWORD(uui[er+er],uui[er+er+1]);
				}
			}
			break;
		case 24:
			{
				BYTE *lko=(BYTE *)(LPVOID)ff;
				BYTE er;
				for(ULONG jh=0; jh<(sz*4); jh+=4)
				{
					er=(lko[jh]-imhead.StartIndex)*3;
					ff[jh/4]=MAKELONG(MAKEWORD(uui[er+2],uui[er+1]),MAKEWORD(uui[er],0));
				}
			}
			break;
		case 32:
			for(ULONG jh=0; jh<(sz*4); jh+=4)
			{
				BYTE *lko=(BYTE *)(LPVOID)ff;
				BYTE er=(ff[jh]-imhead.StartIndex)*4;
				ff[jh/4]=MAKELONG(MAKEWORD(uui[er],uui[er+1]),
					MAKEWORD(uui[er+2],uui[er+3]));
			}
			break;
		}
		delete[] uui;
	}
	CloseHandle(hFile);
	return TRUE;
}
