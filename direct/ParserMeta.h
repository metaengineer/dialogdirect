#include "Parser.h"
#include <commctrl.h>
#include "Fonts.h"
#include <list>
#include <string>
#include "Bitmaps.h"

#ifndef UTIL_METAPARSER_H
#define UTIL_METAPARSER_H

class CMenuControl // abstract
{
protected:
	HINSTANCE ParentInstance;
	HWND ParentWnd;
	HWND CtlWnd;
	BOOL IsActive;
	BOOL InputEnabled;
	BOOL IsVisible;
	LPSTR act;
	UINT actlen;
	LPSTR CtlName;
	UINT namelen;
public:
	LONG a;
	LONG b;
	int VarLink;
	int Rqux;
	int Rquy;
	std::list<std::pair<int,LPCSTR> > Hash;
public:
	CMenuControl(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	void SetAdditional(LONG param1, LONG param2, LPCSTR str);
	virtual void Visibility(BOOL bShow);
	virtual void Activity(BOOL bAct);
	virtual void AssignList(std::list<std::pair<int,LPCSTR>> ls);
	virtual HWND GetHandle(UINT wh);
	LPCSTR GetName();
	LPCSTR GetAct();
	BOOL RegexName(int in);
	virtual ~CMenuControl();
};

class CMenuControlButton : public CMenuControl
{
public:
	CMenuControlButton(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	~CMenuControlButton();
};

class CMenuControlCheckbox : public CMenuControl
{
public:
	CMenuControlCheckbox(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	~CMenuControlCheckbox();
};

class CMenuControlField : public CMenuControl
{
public:
	CMenuControlField(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	~CMenuControlField();
};

class CMenuControlImage : public CMenuControl
{
public:
	CMenuControlImage(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	~CMenuControlImage();
};

class CMenuControlSlider : public CMenuControl
{
protected:
	HWND OwnWndL;
	HWND OwnWndR;
public:
	CMenuControlSlider(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	HWND GetHandle(UINT wh);
	void Visibility(BOOL bShow);
	void Activity(BOOL bAct);
	~CMenuControlSlider();
};

class CMenuControlText : public CMenuControl
{
public:
	CMenuControlText(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname);
	CMenuControlText(HINSTANCE pins, HWND pwnd, int x, int y, WORD wi, WORD group, int var1, LPCSTR cname);
	~CMenuControlText();
};

class CMenuList
{
public:
	CMenuList();
	~CMenuList();
};

struct SMenuMenu
{
	CMenuControlButton *ThisEntry;
	std::string label;
	SMenuMenu *snext;
	SMenuMenu(HINSTANCE pins, HWND pwnd, UINT order, UINT x, UINT y, LPCSTR name);
	~SMenuMenu();
};

struct SMenuList
{
	CMenuControlButton *ThisEntry;
	std::string name;
	SMenuList *snext;
	SMenuList(HINSTANCE pins, HWND pwnd, UINT order, UINT x, UINT y, LPCSTR name);
	~SMenuList();
};

struct SMenuTabs
{
	CMenuControlButton *ThisTab;
	SMenuTabs *snext;
	SMenuTabs(HINSTANCE pins, HWND pwnd, UINT order, LPCSTR name);
	~SMenuTabs();
};

struct SMenuControls
{
	CMenuControl *ThisThing;
	SMenuControls *snext;
	SMenuControls(CMenuControl *anything);
	void ShowGroup(int gr);
	~SMenuControls();
};

struct SFrameChain
{
	BOOL HasBg;
	CBitmapTexture *back;
	DWORD flags;
	LPSTR name;
	DWORD namelen;
	SMenuMenu *menu;
	SMenuTabs *tabs;
	SMenuList *list;
	SMenuControls *ctrls;
	SFrameChain *snext;
	DWORD bglen;
	LPSTR bg;
	SFrameChain(LPCSTR fn);
	void Add(LPCSTR fn);
	SFrameChain *Find(LPCSTR fn);
	~SFrameChain();
};

class CParserMeta
{
public:
	CParserMeta(LPCSTR name);
	~CParserMeta();
};

#endif
