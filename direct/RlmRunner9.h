#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "VarStorage.h"
#include "ErrorStream.h"

#ifndef RLMRUNNER_9_H
#define RLMRUNNER_9_H

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
	LPSTR RealmDir;
public:
	CRealm(HINSTANCE pins, HWND pwnd, LPCSTR RlmDir);
	BOOL RegisterVars(SVarStorage *vs);
	BOOL RegisterMessageSink(CMessageCache *sink);
	BOOL Initialize();
	void OnFrame(DWORD tx);
	~CRealm();
};

#endif