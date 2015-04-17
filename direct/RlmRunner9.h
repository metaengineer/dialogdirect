#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"

#ifndef RLMRUNNER_9_H
#define RLMRUNNER_9_H

extern class SVarStorage;

class CRealm
{
private:
	IDirect3D9 *roo;
	BOOL IsLoaded;
	HINSTANCE ParentInstance;
	HWND ParentWindow;
	HWND DxWnd;
	BOOL IsClass;
	BOOL IsWindow;
	BOOL IsInterface;

public:
	CRealm(HINSTANCE pins, HWND pwnd, LPCSTR RlmDir, SVarStorage *vs);
	~CRealm();
};

#endif