// direct.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "direct.h"
#include "objbase.h"
#include "Ustring.h"
#include "RlmRunner9.h"
#include <string>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Bitmaps.h"
#include "MetaScreen.h"

#define MAX_LOADSTRING 255
const char FlavorDir[]="flavor\\";
const char DefFolder[]="C:\\Apps\\metaproduct\\direct\\";
int CurrentMode=0;

SFrameChain *frames=NULL;
SFrameChain *CurrentFrame=NULL;
SVarStorage *vars=NULL;
CRealm *program=NULL;
std::string wrkf;

// Global Variables:
HINSTANCE hInst;								// current instance
HWND OwnWnd=NULL;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int _RunProgram(SVarStorage *vs, LPCSTR wdir);
int _StopProgram();
void _ParseAction(LPSTR str);

// classes

SVarStorage::SVarStorage(UINT qu)
{
	varq=0;
	init=FALSE;
	if((qu>0)&&(qu<=1000))
	{
		store=new float[qu];
		varq=qu-1;
		init=TRUE;
	}
}

BOOL SVarStorage::CheckInit()
{
	return init;
}

BOOL SVarStorage::CheckVar(UINT no)
{
	return (init&&(varq>=no)); 
}

BOOL SVarStorage::SetVar(UINT no, float dat)
{
	if(init&&(varq>=no))
	{
		store[no]=dat;
		return TRUE;
	}
	return FALSE;
}

float SVarStorage::GetVar(UINT no)
{
	if(init&&(varq>=no))
	{
		return store[no];
	}
	return 0.0f;
}

SVarStorage::~SVarStorage()
{
	if(init)
	{
		delete[] store;
		init=FALSE;
	}
}

void _ParseAction(LPSTR str)
{
	char *uj;
	uj=strtok(str," ");
	if(!strcmp(uj,"exit"))
	{
		PostQuitMessage(0);
		return;
	}
	if(!strcmp(uj,"run"))
	{
		_RunProgram(vars, wrkf.c_str());
		return;
	}
	if(!strcmp(uj,"goto"))
	{
		uj=strtok(NULL, " ");
		if(!uj)
			return;
		if(!frames)
			return;
		SFrameChain *iio=frames->Find(uj);
		if(iio)
			CurrentFrame=iio;
	}
	if(!strcmp(uj,"group"))
	{
		uj=strtok(NULL, " ");
		if(!uj)
			return;
		if(!frames)
			return;
		frames->ctrls->ShowGroup(atoi(uj));
	}
	if(!strcmp(uj, "set"))
	{
		if(vars)
			return;
		char *ya=strtok(NULL, " ");
		char *wa=strtok(NULL, " ");
		if(!ya)
			return;
		if(!wa)
			return;
		LONG hkl=atoi(ya);
		float rkl=atof(wa);
		vars->SetVar(hkl, rkl);
	}
}

int _RunProgram(SVarStorage *vs, LPCSTR wdir)
{
	program=new CRealm(hInst, OwnWnd, wdir, vs);
	return 1;
}

int _StopProgram()
{
	if(program)
		delete program;
	return 0;
}

BOOL _StringExpectSymbols(LPCSTR bu, UINT *cu, LPCSTR tmp)
{
	UINT zwa=*cu;
	UINT kwa=0;
	UINT sd=strlen(tmp);
	while(bu[zwa])
	{
		if(bu[zwa]!=tmp[kwa])
			return FALSE;
		zwa++;
		kwa++;
		if(kwa==(sd-1))
		{
			*cu=zwa;
			return TRUE;
		}
	}
	return FALSE;
}

// program

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);

	// trying to retrieve working dir (don't know other ways to do that)
	{
		UINT eks=0;
		BOOL hapath=FALSE;
		while(lpCmdLine[eks])
		{
			if(lpCmdLine[eks]==' ')
				continue;
			if(lpCmdLine[eks]=='/')
			{
				if(_StringExpectSymbols(lpCmdLine, &eks, "/wd=\""))
					hapath=TRUE;
				break;
			}
			eks++;
		}
		wrkf=std::string(DefFolder);
		if(hapath)
		{
			UINT kk=eks+1;
			while(lpCmdLine[kk])
			{
				if(lpCmdLine[kk]=='\"')
				{
					UINT y=kk-eks-1;
					if(y)
					{
						LPTSTR ggf=new char[y+1];
						strncpy_s(ggf, y+1, &(lpCmdLine[eks+1]), _TRUNCATE);
						wrkf=std::string(ggf);	
						delete[] ggf;
					}
				}
				kk++;
			}
		}
	}

 	MSG msg;
	SecureZeroMemory(&msg, sizeof(MSG));
	
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIRECT, szWindowClass, MAX_LOADSTRING);
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetSysColorBrush(COLOR_GRAYTEXT);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	DWORD dTiming=0;
	DWORD dFrame=0;
	ATOM at= RegisterClassEx(&wcex);
	if(at)
	{
		INITCOMMONCONTROLSEX dgs;
		dgs.dwSize=sizeof(INITCOMMONCONTROLSEX);
		dgs.dwICC=ICC_STANDARD_CLASSES|ICC_INTERNET_CLASSES;
		InitCommonControlsEx(&dgs);

		HRESULT ugum=CoInitializeEx(NULL, COINIT_MULTITHREADED);
		// Perform application initialization:
		HWND hWnd=NULL;

		hInst = hInstance; // Store instance handle in our global variable
		RECT gf;
		gf.top=10;
		gf.bottom=610;
		gf.left=10;
		gf.right=810;
		AdjustWindowRect(&gf, WS_POPUP|WS_BORDER, FALSE);

		OwnWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			gf.left, gf.top, gf.right-gf.left, gf.bottom-gf.top, HWND_DESKTOP, NULL, hInstance, NULL);

		if(OwnWnd)
		{
			// STARTING SCRIPT
			std::string yov=wrkf;
			yov+="flavor.txt";
			CAppTextScriptReader *scri=new CAppTextScriptReader();
			BOOL hhh=scri->InitTextScript(yov.c_str());
			if(hhh)
				MessageBox(OwnWnd, "TRUE", "Debug", MB_OK|MB_ICONINFORMATION);
			// TODO: script
			delete scri;
			// SCRIPT END
			UpdateWindow(hWnd);
			BOOL bContinue=TRUE;
			BOOL kpout;
			// Main message loop:
			while (bContinue)
			{
				kpout=PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				if(msg.message==WM_QUIT)
					bContinue=FALSE;
				else
				{
					if(kpout)
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					// realtime operations
					dFrame=dTiming;
					dTiming=GetTickCount();
					//
				}
			}
			//destroy
			_StopProgram();
			if(frames)
				delete frames;
			if(vars)
				delete vars;
		}
		UnregisterClass(szWindowClass, hInst);
		CoUninitialize();
	}
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		// TODO: user input messages
		switch (wmId)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		case BN_CLICKED:
		{
			HWND hctrl=(HWND)lParam;
			if(!CurrentFrame)
				break;
			SMenuControls *bna=CurrentFrame->ctrls;
			SMenuList *bnl=CurrentFrame->list;
			SMenuMenu *bnm=CurrentFrame->menu;
			SMenuTabs *bnt=CurrentFrame->tabs;
			while(bna||bnl||bnm||bnt)
			{
				if(bna)
				{
					CMenuControl *cma=bna->ThisThing;
					if(hctrl==cma->GetHandle(0))
					{
						LPCSTR ddf=cma->CtlName;
						if(strlen(ddf)>2)
						{
							LPSTR fff=new char[strlen(ddf)+1];
							strcpy(fff, ddf);
							_ParseAction(fff);
							delete[] fff;
						}
					}
					else
					{
						if(hctrl==cma->GetHandle(1))
						{
							if(vars)
								vars->SetVar(cma->VarLink,max(cma->a,vars->GetVar(cma->VarLink)-1.0f));
						}
						if(hctrl==cma->GetHandle(2))
						{
							if(vars)
								vars->SetVar(cma->VarLink,min(cma->b,vars->GetVar(cma->VarLink)+1.0f));
						}
					}
					bna=bna->snext;
				}
				if(bnl)
				{
					CMenuControlButton *anl=bnl->ThisEntry;
					if(hctrl==anl->GetHandle(0))
					{
						LPCSTR ddf=anl->CtlName;
						if(strlen(ddf)>2)
						{
							LPSTR fff=new char[strlen(ddf)+1];
							strcpy(fff, ddf);
							_ParseAction(fff);
							delete[] fff;
						}
					}
					bnl=bnl->snext;
				}
				if(bnm)
				{
					CMenuControlButton *anm=bnm->ThisEntry;
					if(hctrl==anm->GetHandle(0))
					{
						LPCSTR ddf=anm->CtlName;
						if(strlen(ddf)>2)
						{
							LPSTR fff=new char[strlen(ddf)+1];
							strcpy(fff, ddf);
							_ParseAction(fff);
							delete[] fff;
						}
					}
					bnm=bnm->snext;
				}
				if(bnt)
				{
					CMenuControlButton *ant=bnt->ThisTab;
					if(hctrl==ant->GetHandle(0))
					{
						LPCSTR ddf=ant->CtlName;
						if(strlen(ddf)>2)
						{
							LPSTR fff=new char[strlen(ddf)+1];
							strcpy(fff, ddf);
							_ParseAction(fff);
							delete[] fff;
						}
					}
					bnt=bnt->snext;
				}
			}
		}
		default:

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{	hdc = BeginPaint(hWnd, &ps);
		HDC fsar=CreateCompatibleDC(hdc);
		if(CurrentFrame)
		{
			CBitmapTexture *uu=CurrentFrame->back;
			if(uu)
			{
				if(uu->GetInfo()>1)
				{
					HBITMAP uiu=uu->GetBits();
					SelectObject(fsar, uiu);
					BOOL kj=BitBlt(hdc, 0, 0, 800, 600, fsar, 0, 0, SRCCOPY);
					kj++;
				}
			}
		}
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		DeleteObject(fsar);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box. leave it there just in case dialogs needed
/*INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
*/