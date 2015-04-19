#include "stdafx.h"
#include "RlmRunner9.h"

const CHAR *DxWndClassName="RlmRunnerClass";

CRealm::CRealm(HINSTANCE pins, HWND pwnd, LPCSTR RlmDir, SVarStorage *vs)
{
	ParentInstance=pins;
	ParentWindow=pwnd;
	IsLoaded=FALSE;
	IsClass=FALSE;
	IsWindow=FALSE;

	WNDCLASSEX bla;
	bla.cbClsExtra=0;
	bla.cbWndExtra=0;
	bla.cbSize=sizeof(WNDCLASSEX);
	bla.hbrBackground=CreateSolidBrush(RGB(0, 0, 0));
	bla.hCursor=LoadCursor(pins, IDC_ARROW);
	bla.hIcon=NULL;
	bla.hIconSm=NULL;
	bla.hInstance=pins;
	bla.lpfnWndProc=DefWindowProc;
	bla.style=0;
	bla.lpszClassName=DxWndClassName;
	bla.lpszMenuName=NULL;

	if(RegisterClassEx(&bla))
		IsClass=TRUE;

	IsInterface=FALSE;
	if(IsClass)
	{
		RECT hfg;
		hfg.left=100;
		hfg.right=900;
		hfg.top=100;
		hfg.bottom=700;
		AdjustWindowRect(&hfg, WS_POPUP|WS_BORDER, FALSE);
		DxWnd=CreateWindow(DxWndClassName, "RealmRunner", WS_POPUP|WS_BORDER|WS_VISIBLE, hfg.left, hfg.top, hfg.right-hfg.left, hfg.bottom-hfg.top,
			ParentWindow, NULL, ParentInstance, NULL);

		if(DxWnd)
			IsWindow=TRUE;
		
		roo=Direct3DCreate9(D3D_SDK_VERSION);
		if(roo)
			IsInterface=TRUE;
	}
}	

CRealm::~CRealm()
{
	if(IsInterface)
	{
		roo->Release();
		roo=NULL;
	}
	if(IsWindow)
		DestroyWindow(DxWnd);
	if(IsClass)
		UnregisterClass(DxWndClassName, ParentInstance);
}