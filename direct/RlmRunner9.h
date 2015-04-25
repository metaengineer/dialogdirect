#include "stdafx.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "VarStorage.h"
#include "ErrorStream.h"
#include "handler.h"

#ifndef RLMRUNNER_9_H
#define RLMRUNNER_9_H

class CRealm
{
private:
	D3DPRESENT_PARAMETERS parm; //keep it here to avoid redoing that
	SVarStorage *glob;
	IDirect3D9 *roo;
	IDirect3DDevice9 *port;
	BOOL IsLoaded;
	HINSTANCE ParentInstance;
	HWND ParentWindow;
	HWND DxWnd;
	BOOL IsClass;
	BOOL IsWindow;
	BOOL IsInterface;
	BOOL IsVar;
	LPSTR RealmDir;
	CMessageCache *sk;
	void _CheckSurfaceFmts(COutHandler *oot, D3DDEVTYPE ana, BOOL uul, UINT y);
	void _PrintCap(COutHandler *oot, D3DCAPS9 *wger);
public:
	CRealm(HINSTANCE pins, HWND pwnd, LPCSTR RlmDir);
	BOOL RegisterVars(SVarStorage *vs);
	BOOL RegisterMessageSink(CMessageCache *sink);
	BOOL Initialize();
	void OnFrame(DWORD tx);
	~CRealm();
};

#endif