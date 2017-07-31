// direct.cpp : Defines the entry point for the application.
//

#define WIN32
#define WINNT 0x501
#define _WINDOWS
#include "common.h"
#include "direct.h"
#include "Ustring.h"
#include "RlmRunner9.h"
#include <string>
#include <stdlib.h>
#include <winbase.h>
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	//#include <crtdbg.h>
#endif

#include "Bitmaps.h"
#include "MetaScreen.h"

#define MAX_LOADSTRING 255
const char FlavorDir[]="flavor\\";
int CurrentMode=0;

SFrameChain *frames=NULL;
SFrameChain *CurrentFrame=NULL;
SVarStorage *vars=NULL;
CRealm *program=NULL;
std::string wrkf;
CBitmapTexture *uu=NULL;

// Global Variables:
HINSTANCE hInst;								// current instance
HWND OwnWnd=NULL;
HWND Messenger=NULL;
CMessageCache *cac=NULL;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int _RunProgram(SVarStorage *vs, LPCSTR wdir);
int _StopProgram();
void _ParseAction(LPSTR str);
void _LogToFile();

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
		{
			if(uu)
			{
				delete uu;
				uu=NULL;
			}
			CurrentFrame->ctrls->ShowGroup(0);
			CurrentFrame=iio;
			CurrentFrame->ctrls->ShowGroup(1);
			if(CurrentFrame->HasBg)
			{
				std::string hov=wrkf;
				hov+=CurrentFrame->bg;
				uu=new CBitmapTexture(hov.c_str());
			}
			RedrawWindow(OwnWnd, NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
		}
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
		if(!vars)
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

void _LogToFile()
{
	HANDLE hTemp=CreateFile("log.txt", STANDARD_RIGHTS_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(cac)
		cac->WriteToFile(hTemp);
	CloseHandle(hTemp);
}

int _RunProgram(SVarStorage *vs, LPCSTR wdir)
{
	// this was probably called from WM_COMMAND, so wait for that to finish
	PostMessage(OwnWnd, WM_USER+0x8000, (WPARAM)0, (LPARAM)wdir);
	PostMessage(OwnWnd, WM_USER+0x8001, (WPARAM)0, (LPARAM)(LPVOID)vs);
	return 1;
}

int _StopProgram()
{
	if(program)
	{
		delete program;
		delete cac;
		cac=NULL;
		DestroyWindow(Messenger);
		program=NULL;
	}
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
		if(hapath)
		{
			UINT kk=eks+1;
			while(lpCmdLine[kk])
			{
				switch(lpCmdLine[kk])
				{
                case '\"':
                case ' ':
                case 0:
					UINT y=kk-eks-1;
					if(y)
					{
						LPTSTR ggf=new char[y+2];
						strncpy(ggf, &(lpCmdLine[eks+1]), y);
						ggf[y]=0;
						PathAddBackslash(ggf);
						wrkf=std::string(ggf);
						delete[] ggf;
					}
					break;
				}
				kk++;
			}
		}
		else
		{
			LPSTR srr=new char[256];
			GetModuleFileName(NULL, srr, 256);
			PathRemoveFileSpec(srr);
			PathAddBackslash(srr);
			wrkf=std::string(srr);
			delete[] srr;
		}
	}

 	MSG msg;
	RtlZeroMemory(&msg, sizeof(MSG));

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
		gf.top=35;
		gf.bottom=635;
		gf.left=10;
		gf.right=810;
		AdjustWindowRect(&gf, WS_POPUP|WS_CAPTION|WS_THICKFRAME, FALSE);

		OwnWnd = CreateWindow(szWindowClass, szTitle,
			WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_THICKFRAME|WS_VISIBLE,
			gf.left, gf.top, gf.right-gf.left, gf.bottom-gf.top, HWND_DESKTOP, NULL, hInstance, NULL);

		if(OwnWnd)
		{
			// STARTING SCRIPT
			std::string yov=wrkf;
			yov+="media\\flavor.txt";
			CAppTextScriptReader *scri=new CAppTextScriptReader();
			BOOL hhh=scri->InitTextScript(yov.c_str());
			if(hhh)
			{
				DWORD fvt=0;
				if(scri->ExpectWord("vars",4))
				{
					vars=new SVarStorage(scri->ReadWholeNumber());
				}
				else
				{
					vars=new SVarStorage(0);
				}
				while(scri->ExpectWord("begin",5))
				{
					scri->ExpectWord("frame",5);
					SFrameChain **ihu=&frames;
					while(*ihu)
					{
						ihu=&((*ihu)->snext);
					}
					{
						scri->MoveNextWord();
						scri->_ExpectLetter('\"');
						ULONG le=scri->PreRecordIdentifierBraces('\"');
						char *tttt=new char[le+1];
						scri->RecordIdentifierBraces(tttt, le, '\"');
						*ihu=new SFrameChain(tttt);
						delete[] tttt; // TODO
					}
					while(!scri->ExpectWord("end",3))
					{
						if(scri->ExpectWord("bg",2))
						{
							scri->MoveNextWord();
							scri->_ExpectLetter('\"');
							if((*ihu)->HasBg)
								delete[] (*ihu)->bg;
							ULONG le=scri->PreRecordIdentifierBraces('\"');
							(*ihu)->bglen=le+1;
							(*ihu)->bg=new char[le+1];
							scri->RecordIdentifierBraces((*ihu)->bg, le, '\"');
							(*ihu)->HasBg=TRUE;
							continue;
						}
						// TODO: some chain list creation
						if(scri->ExpectWord("button",6))
						{
							scri->MoveNextWord();
							scri->_ExpectLetter('\"');
							DWORD uj=scri->PreRecordIdentifierBraces('\"');
							LPSTR bun=new char[uj+1];
							scri->RecordIdentifierBraces(bun, uj, '\"');
							SMenuControls **ysy=&((*ihu)->ctrls);
							while(*ysy)
								ysy=&((*ysy)->snext);
							int xb=scri->ReadWholeNumber();
							int yb=scri->ReadWholeNumber();
							CMenuControl *thi=new CMenuControlButton(hInst, OwnWnd,
								xb, yb, 0, bun);
							(*ysy)=new SMenuControls(thi);
							delete[] bun;
							scri->MoveNextWord();
							scri->_ExpectLetter('\"');
							DWORD uju=scri->PreRecordIdentifierBraces('\"');
							LPSTR un=new char[uju+1];
							scri->RecordIdentifierBraces(un, uju, '\"');
							thi->SetAdditional(0, MAKEWORD(1,0), un);
							(*ysy)->ShowGroup(0);
							delete[] un;
							continue;
						}
						if(scri->ExpectWord("checkbox",8))
						{

						}
						if(scri->ExpectWord("text",4))
						{
							scri->MoveNextWord();
							scri->_ExpectLetter('\"');
							DWORD uj=scri->PreRecordIdentifierBraces('\"');
							LPSTR bun=new char[uj+1];
							scri->RecordIdentifierBraces(bun, uj, '\"');
							SMenuControls **ysy=&((*ihu)->ctrls);
							while(*ysy)
								ysy=&((*ysy)->snext);
							int xq=scri->ReadWholeNumber();
							int yq=scri->ReadWholeNumber();
							WORD varg=scri->ReadWholeNumber();
							WORD wi=scri->ReadWholeNumber();
							CMenuControl *thi=new CMenuControlText(hInst, OwnWnd,
								xq, yq, wi, MAKEWORD(1, 0), varg, bun);
							(*ysy)=new SMenuControls(thi);
							delete[] bun;
							BOOL regexforth=TRUE;
							while(regexforth)
							{
								regexforth=thi->RegexName(vars->GetVar(thi->VarLink));
							}
							(*ysy)->ShowGroup(0);
							continue;
						}
						if(scri->ExpectWord("bitmap",6))
						{

						}
						if(scri->ExpectWord("field",5))
						{

						}
						if(scri->ExpectWord("slider",6))
						{

						}

						if(scri->ExpectWord("begin",5))
						{
							while(true)
							{
								if(scri->ExpectWord("menu",4))
								{
									while(!(scri->ExpectWord("end",3)))
									{
										if(scri->ExpectWord("option",6))
										{

										}
									}
									break;
								}
								if(scri->ExpectWord("list",4))
								{
									while(!(scri->ExpectWord("end",3)))
									{
										if(scri->ExpectWord("option",6))
										{

										}
									}
									break;
								}
								if(scri->ExpectWord("tabs",4))
								{
									while(!(scri->ExpectWord("end",3)))
									{
										if(scri->ExpectWord("tab",3))
										{

										}
									}
									break;
								}
								scri->ExpectWord("end",3);
								break;
							}
						}
						break;
					}
				}
			}
			delete scri;
			// SCRIPT END
			if(frames)
			{
				CurrentFrame=frames->Find("index");
				CurrentFrame->ctrls->ShowGroup(1);
				UpdateWindow(OwnWnd);
				if(CurrentFrame->HasBg)
				{
					std::string hov=wrkf;
					hov+="media\\";
					hov+=CurrentFrame->bg;
					uu=new CBitmapTexture(hov.c_str());
				}
			}
			RedrawWindow(OwnWnd, NULL, NULL, RDW_INVALIDATE|RDW_ALLCHILDREN);
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
					if(program)
					{
						try
						{
							program->OnFrame(dTiming-dFrame);
						}
						catch(AutoStopException g)
						{
							MessageBox(OwnWnd, "Saving log to text file", "Error detected", MB_OK | MB_ICONERROR);
							_LogToFile();
							PostQuitMessage(0);
						}
					}
				}
			}
			//destroy
			_StopProgram();
			if(uu)
				delete uu;
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
						LPCSTR ddf=cma->GetAct();
						if(ddf&&(strlen(ddf)>2))
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
								vars->SetVar(cma->VarLink,std::max((float)cma->a,vars->GetVar(cma->VarLink)-1.0f));
						}
						if(hctrl==cma->GetHandle(2))
						{
							if(vars)
								vars->SetVar(cma->VarLink,std::min((float)cma->b,vars->GetVar(cma->VarLink)+1.0f));
						}
					}
					bna=bna->snext;
				}
				if(bnl)
				{
					CMenuControlButton *anl=bnl->ThisEntry;
					if(hctrl==anl->GetHandle(0))
					{
						LPCSTR ddf=anl->GetAct();
						if(ddf&&(strlen(ddf)>2))
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
						LPCSTR ddf=anm->GetAct();
						if(ddf&&(strlen(ddf)>2))
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
						LPCSTR ddf=ant->GetAct();
						if(ddf&&(strlen(ddf)>2))
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
			return 0;
		}
		default:

			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case (WM_USER+0x8000):
		{
			// this was one time use, destroying
			if(uu)
				delete uu;
			uu=NULL;
			CurrentFrame=NULL;
			if(frames)
				delete frames;
			frames=NULL;
			program=new CRealm(hInst, OwnWnd, (LPCSTR)(LPVOID)lParam);
			Messenger=CreateWindowEx(0, "Edit", "",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL,
				0, 0, 800, 600, OwnWnd, NULL, hInst, NULL);
			SendMessage(Messenger, EM_SETLIMITTEXT, 32*256, 0);
			cac=new CMessageCache(Messenger);
			program->RegisterMessageSink(cac);
			return 0;
		}
	case (WM_USER+0x8001):
		{
			if(program) // in case the message was sent by something else
			{
				try
				{
					program->RegisterVars((SVarStorage *)(LPVOID)lParam);
					program->Initialize();
				}
				catch(AutoStopException g)
				{
					MessageBox(OwnWnd, "Saving log to text file", "Error detected", MB_OK | MB_ICONERROR);
					_LogToFile();
					PostQuitMessage(0);
				}
			}
			return 0;
		}
	case WM_PAINT:
		{	hdc = BeginPaint(hWnd, &ps);
		HDC fsar=CreateCompatibleDC(hdc);
		if(CurrentFrame)
		{
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
