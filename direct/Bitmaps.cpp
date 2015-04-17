#include "stdafx.h"
#include "Bitmaps.h"
#include <new>
#include <cmath>
#include <string>

BOOL UtilLoadBitmap(LPCSTR path, DWORD **ppvData, LONG *pImagex, LONG *pImagey)
{
	HANDLE hFile = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);        
	if(hFile == INVALID_HANDLE_VALUE) 
		return FALSE;

	BITMAPFILEHEADER bmpfilehdr;  
	DWORD dwRead;
	if(!ReadFile(hFile, &bmpfilehdr, sizeof(bmpfilehdr), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<sizeof(bmpfilehdr))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	char* ptr=(char*)&bmpfilehdr.bfType;
	if (*ptr!='B' || *++ptr!='M')
	{
		CloseHandle(hFile);
		return FALSE;
	}

	BITMAPINFOHEADER bmpinfohdr;
	if(!ReadFile(hFile, &bmpinfohdr, sizeof(bmpinfohdr), &dwRead, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<sizeof(bmpinfohdr))
	{
		CloseHandle(hFile);
		return FALSE;
	}

	*pImagey=bmpinfohdr.biHeight;
	*pImagex=bmpinfohdr.biWidth;

	int iImageSize=bmpinfohdr.biSizeImage;
	switch(bmpinfohdr.biBitCount)
	{
	case 1:
		iImageSize=((*pImagex+31)/32)*4*(*pImagey)+2*sizeof(DWORD);
		break;
	case 4:
		iImageSize=((*pImagex+7)/8)*4*(*pImagey)+16*sizeof(DWORD);
		break;
	case 8:
		iImageSize=((*pImagex+3)/4)*16*(*pImagey)+256*sizeof(DWORD);
		break;
	case 16:
		iImageSize=((*pImagex+1)/2)*4*(*pImagey);
		break;
	case 24:
		iImageSize=(((*pImagex)*3+3) & ~3)*(*pImagey);
		break;
	case 32:
		iImageSize=((*pImagex)*(*pImagey)) << 2;
		break;
	}
	BYTE *buf;
	try
	{
		buf=new BYTE[iImageSize];
	}
	catch(std::bad_alloc a)
	{
		CloseHandle(hFile);
		return FALSE;	
	}
	if(!ReadFile(hFile, buf, iImageSize, &dwRead, NULL))
	{
		delete[] buf;
		CloseHandle(hFile);
		return FALSE;
	}
	if(dwRead<iImageSize)
	{
		delete[] buf;
		CloseHandle(hFile);
		return FALSE;
	}

	if(bmpinfohdr.biCompression!=BI_RGB)
	{
		delete[] buf;
		CloseHandle(hFile);
		return FALSE;	
	}
	try
	{
		*ppvData=new DWORD[(*pImagex)*(*pImagey)];	
	}
	catch(std::bad_alloc t)
	{
		delete[] buf;
		CloseHandle(hFile);
		return FALSE;		
	}
	switch(bmpinfohdr.biBitCount)
	{
	case 1:
	{
		DWORD *clrt=new DWORD[2];
		memcpy(clrt, buf, 2*sizeof(DWORD));
		BYTE *imagebits = buf+2*sizeof(DWORD);
		DWORD *btSurf;
		DWORD row=((*pImagex)+31)/32;
		for(LONG s=(*pImagey-1); s>=0; s--)
		{
			DWORD nRead=0;
			btSurf=*ppvData+s*(*pImagex);
			for(DWORD w=0; w<(row*4); w++)
			{
				WORD a=128;
				for(WORD s=0; s<8; s++)
				{
					if(nRead>=(*pImagex))
						break;
					*btSurf=(((*imagebits)&a)?(~DWORD(0)):0);
					a=a>>1;
					btSurf++;
					nRead++;
				}
				imagebits++;
			}
		}
		DWORD *uColor=*ppvData;
		for(DWORD s=0; s<((*pImagex)*(*pImagey)); s++)
		{
			*uColor=*(clrt+((*uColor)?1:0));
			uColor++;
		}
		delete[] clrt;
		break;
	}
	case 4:
	{
		DWORD *clrt=new DWORD[16];
		memcpy(clrt, buf, 16*sizeof(DWORD));
		BYTE *imagebits = buf+16*sizeof(DWORD);
		DWORD *btSurf;
		DWORD row=((*pImagex)+7)/8;
		for(LONG h=(*pImagey)-1; h>=0; h--)
		{
			DWORD nRead=0;
			btSurf=*ppvData+h*(*pImagex);
			for(DWORD w=0; w<(row*4); w++)
			{
				WORD a=15<<4;
				for(int s=4; s>=0; s-=4)
				{
					if(nRead>=(*pImagex))
						break;
					*btSurf=(((*imagebits)&a)>>s);
					a=a>>4;
					btSurf++;
					nRead++;
				}
				imagebits++;
			}
		}
		DWORD *uColor=*ppvData;
		for(DWORD s=0; s<((*pImagex)*(*pImagey)); s++)
		{
			*uColor=*(clrt+((*uColor)&0xF));
			uColor++;
		}
		delete[] clrt;
		break;
	}
	case 8:
	{
		WORD nRead=0;
		DWORD *clrt=new DWORD[256];
		memcpy(clrt, buf, 256*sizeof(DWORD));
		BYTE *imagebits = buf+256*sizeof(DWORD);
		DWORD *btSurf;
		for(LONG s=(*pImagey-1); s>=0; s--)
		{
			btSurf=*ppvData+(*pImagex)*s;
			for(DWORD g=0; g<(*pImagex); g++)
			{
				*btSurf=DWORD(*imagebits);
				btSurf++;
				imagebits++;
			}
		}
		DWORD *uColor=*ppvData;
		for(DWORD s=0; s<((*pImagex)*(*pImagey)); s++)
		{
			*uColor=*(clrt+(*uColor));
			uColor++;
		}
		delete[] clrt;
		break;
	}
	case 16:
	{
		int bytesgiven=(((*pImagex)+1)/2)*4;
		DWORD* btSurf = *ppvData;
		BYTE* imagebits = (BYTE*)(&buf[((*pImagey)-1)*bytesgiven]);
	
		for (int i=0; i<(*pImagey); i++)
		{
			WORD* tlImage=(WORD*)imagebits;
			for (int p=0; p<(*pImagex); p++)
			{
				*btSurf = (DWORD)(((*tlImage&31)<<3) | ((*tlImage&(31<<5))<<6) | 
					           ((*tlImage&(31<<10))<<9) | (0xFF<<24));
				tlImage++;
				btSurf++;
			}
			imagebits -= bytesgiven;
		}
		break;
	}
	case 24:
	{
		int bytesgiven=((*pImagex)*3+3) & ~3;
		DWORD* btSurf = *ppvData;
		BYTE* imagebits = (BYTE*)(&buf[((*pImagey)-1)*bytesgiven]);
	
		for (int i=0; i<(*pImagey); i++)
		{
			RGBTRIPLE* tlImage=(RGBTRIPLE*)imagebits;
			for (int p=0; p<(*pImagex); p++)
			{
				*btSurf = (DWORD)((tlImage->rgbtBlue) | (tlImage->rgbtGreen << 8) | 
					           (tlImage->rgbtRed << 16) | (0xFF << 24));
				tlImage++;
				btSurf++;
			}
			imagebits -= bytesgiven;
		}
		break;
	}
	case 32:
	{
		int bytesgiven=(*pImagex)<<2;
		DWORD* btSurf = *ppvData;
		BYTE* imagebits = (BYTE*)(&buf[((*pImagey)-1)*bytesgiven]);
		for (int i=0; i<(*pImagey); i++)
		{
			RGBQUAD* tlImage=(RGBQUAD*)imagebits;
			for (int p=0; p<(*pImagex); p++)
			{
				*btSurf = (DWORD)((tlImage->rgbBlue) | (tlImage->rgbGreen << 8) | 
					(tlImage->rgbRed << 16) | (tlImage->rgbReserved << 24));
				tlImage++;
				btSurf++;
			}
			imagebits -= bytesgiven;
		}
		break;
	}
	}

	delete [] buf;
	CloseHandle(hFile);
	return TRUE;	
}

BOOL UtilSaveBitmap(LPCSTR path, DWORD *pDataRgb, LONG ix, LONG iy, int SaveFmt)
{
	HANDLE tWrite=NULL;
	ULONG ssize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	ULONG tsize;
	ULONG jz=ix*iy;
	switch(SaveFmt)
	{
	case 1:
		tsize=(sizeof(RGBQUAD)<<1)+(jz+(jz%8))/8;
		break;
	case 4:
		tsize=(sizeof(RGBQUAD)<<3)+(jz+(jz%2))/2;
		break;
	case 8:
		tsize=(sizeof(RGBQUAD)<<4)+jz;
		break;
	case 16:
		tsize=jz+jz;
		break;
	case 24:
		tsize=3*jz;
		break;
	case 32:
		tsize=jz<<2;
		break;
	default:
		return FALSE;
	}
	ssize+=tsize;
	BYTE *fileBu;
	try
	{
		fileBu=new BYTE[tsize];
	}
	catch(std::bad_alloc r)
	{
		return FALSE;	
	}
	BITMAPFILEHEADER fal;
	fal.bfSize=sizeof(BITMAPFILEHEADER);
	fal.bfType=MAKEWORD('B','M');
	fal.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	fal.bfReserved1=0;
	fal.bfReserved2=0;
	BITMAPINFOHEADER sal;
	sal.biSize=sizeof(BITMAPINFOHEADER);
	sal.biWidth=ix;
	sal.biHeight=iy;
	sal.biBitCount=SaveFmt;
	sal.biPlanes=1;
	sal.biSizeImage=0;
	sal.biCompression=BI_RGB;
	sal.biClrUsed=0;
	sal.biClrImportant=0;
	sal.biXPelsPerMeter=1000;
	sal.biYPelsPerMeter=1000;
	ZeroMemory(fileBu,tsize);
	tWrite=CreateFile(path, STANDARD_RIGHTS_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if(tWrite==INVALID_HANDLE_VALUE)
	{
		delete[] fileBu;
		return FALSE;
	}
	WriteFile(tWrite, &fal, sizeof(BITMAPFILEHEADER), NULL, NULL);
	WriteFile(tWrite, &sal, sizeof(BITMAPINFOHEADER), NULL, NULL);
	//write bits1;
	switch(SaveFmt)
	{
	case 1:
		{
			DWORD *fGu=(DWORD *)(LPVOID)fileBu;
			fGu[0]=RGB(0,0,0);
			fGu[1]=RGB(255, 255, 255);
		}
		break;
	case 4:
		// let's see..
		{
			DWORD *fg=(DWORD *)(LPVOID)fileBu;
			fg[0]=RGB(0,0,0);
			fg[1]=RGB(63, 63, 63);
			fg[2]=RGB(127,127,127);
			fg[3]=RGB(255, 255, 255);
			fg[4]=RGB(255,0,0);
			fg[5]=RGB(0, 255, 0);
			fg[6]=RGB(0,0,255);
			fg[7]=RGB(255, 255, 0);
			fg[8]=RGB(255,0,255);
			fg[9]=RGB(0, 255, 255);
			fg[10]=RGB(191,191,191);
			fg[11]=RGB(127, 0, 0);
			fg[12]=RGB(0,127,0);
			fg[13]=RGB(0, 0, 127);
			fg[14]=RGB(63,127,0);
			fg[15]=RGB(127, 255, 0);
		}
		break;
	case 8:
		// grayscale then...
		{
			int ya=0;
			DWORD *fg=(DWORD *)(LPVOID)fileBu;
			for(;ya<256;ya++)
			{
				fg[ya]=RGB(ya, ya, ya);
			}
		}
		break;
	}
	BYTE *ugbu=fileBu;
	//write index
	switch(SaveFmt)
	{
	case 1:
		fileBu+=(sizeof(RGBQUAD)<<1);
		for(ULONG h=0; h<(jz+jz%8); h+=8)
		{
			fileBu[h/8]=(BYTE)(pDataRgb[h]&1+pDataRgb[h+1]&2+pDataRgb[h+2]&4+pDataRgb[h+3]&8+
				pDataRgb[h+4]&16+pDataRgb[h+5]&32+pDataRgb[h+6]&64+pDataRgb[h+7]&128);
		}
		break;
	case 4:
		fileBu+=(sizeof(RGBQUAD)<<3);
		for(ULONG h=0; h<(jz+jz%2); h+=2)
		{
			fileBu[h/2]=(BYTE)(pDataRgb[h]&15+(pDataRgb[h+1]<<4));
		}
		break;
	case 8:
		fileBu+=(sizeof(RGBQUAD)<<4);
		for(ULONG h=0; h<jz; h++)
		{
			fileBu[h]=(LOBYTE(LOWORD(pDataRgb[h]))+HIBYTE(LOWORD(pDataRgb[h]))
				+LOBYTE(HIWORD(pDataRgb[h])))/3;
			fileBu++;
		}
		break;
	case 16:
		{
			ULONG h;
		for(h=0; h<jz; h++)
		{
			fileBu[h]=LOBYTE(LOWORD(pDataRgb[h]));
			fileBu[h+1]=HIBYTE(LOWORD(pDataRgb[h]));
			fileBu+=2;
		}
		while(h%2)
		{
			fileBu[h]=0;
			h++;
		}
		}
		break;
	case 24:
		{
			ULONG h;
		for(h=0; h<jz; h++)
		{
			fileBu[h]=LOBYTE(LOWORD(pDataRgb[h]));
			fileBu[h+1]=HIBYTE(LOWORD(pDataRgb[h]));
			fileBu[h+2]=LOBYTE(HIWORD(pDataRgb[h]));
			fileBu+=3;
		}
		while(h%4)
		{
			fileBu[h]=0;
			h++;
		}
		}
		break;
	case 32:
		memcpy(fileBu, pDataRgb, jz<<2);
		break;
	}
	fileBu=ugbu;
	WriteFile(tWrite, fileBu, tsize, NULL, NULL);
	delete[] fileBu;
	return TRUE;
}

CBitmapTexture::CBitmapTexture(LPCSTR FilePath, UINT format)
{
	this->data1=NULL;
	this->flags=0;
	this->imagex=0;
	this->imagey=0;
	this->OwnBitmap=NULL;
	BOOL ld=FALSE;
	switch(format)
	{
	case 1:
		ld=UtilLoadBitmap(FilePath, &data1, &imagex, &imagey);
		break;
	case 2:
		ld=UtilLoadPCX(FilePath, &data1, &imagex, &imagey);
		break;
	case 3:
		ld=UtilLoadTarga(FilePath, &data1, &imagex, &imagey);
		break;
	}
	if(ld)
	{
		flags=1;
		OwnBitmap=CreateBitmap((int)imagex, (int)imagey, 1, 32, data1);
		if(OwnBitmap)
			flags=2;
	}
}

CBitmapTexture::CBitmapTexture(LPCSTR FileNoExt)
{
	this->data1=NULL;
	this->flags=0;
	this->imagex=0;
	this->imagey=0;
	this->OwnBitmap=NULL;
	BOOL ld=FALSE;
	std::string ring=std::string(FileNoExt);
	std::string r;
	r=ring+".tga";
	ld=UtilLoadBitmap(r.c_str(), &data1, &imagex, &imagey);
	if(!ld)
	{
		r=ring+".pcx";
		ld=UtilLoadPCX(r.c_str(), &data1, &imagex, &imagey);
		if(!ld)
		{
			r=ring+".bmp";
			ld=UtilLoadTarga(r.c_str(), &data1, &imagex, &imagey);
		}
	}
	if(ld)
	{
		flags=1;
		OwnBitmap=CreateBitmap((int)imagex, (int)imagey, 1, 32, data1);
		if(OwnBitmap)
			flags=2;
	}
}

UINT CBitmapTexture::GetInfo()
{
	return flags;
}

LONG CBitmapTexture::GetX()
{
	return imagex;
}

LONG CBitmapTexture::GetY()
{
	return imagey;
}

HBITMAP CBitmapTexture::GetBits()
{
	return OwnBitmap;
}

CBitmapTexture::~CBitmapTexture()
{
	if(flags>1)
	{
		DeleteObject(OwnBitmap);
		OwnBitmap=NULL;
	}
	if(flags)
	{
		delete[] data1;
		flags=0;
	}
}