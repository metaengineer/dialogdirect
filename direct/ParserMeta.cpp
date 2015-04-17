#include "stdafx.h"
#include "ParserMeta.h"

CMenuControl::CMenuControl(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname)
{
	ParentInstance=pins;
	ParentWnd=pwnd;
	Rqux=x;
	Rquy=y;
	CtlName=cname;
	IsActive=FALSE;
	InputEnabled=FALSE;
	IsVisible=FALSE;
	CtlWnd=NULL;
	a=0; b=0; act="";
}

void CMenuControl::SetAdditional(LONG param1, LONG param2, LPCSTR str)
{
	a=param1;
	b=param2;
	act=str;
}

CMenuControl::~CMenuControl()
{
	
}

void CMenuControl::Activity(BOOL bAct)
{
	if(bAct)
		SetWindowLong(CtlWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE);
	else
		SetWindowLong(CtlWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_DISABLED);	
}

void CMenuControl::Visibility(BOOL bShow)
{
	ShowWindow(CtlWnd, bShow?SW_SHOW:SW_HIDE);
}

void CMenuControl::AssignList(std::list<std::pair<int,LPCSTR> > ls)
{

}

HWND CMenuControl::GetHandle(UINT wh)
{
	return CtlWnd;
}

CMenuControlButton::CMenuControlButton(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("Button", CtlName, WS_VISIBLE|WS_CHILD, x, y, 150, 50, pwnd, NULL, pins, NULL); 
}

CMenuControlButton::~CMenuControlButton()
{
	DestroyWindow(CtlWnd);
}

CMenuControlSlider::CMenuControlSlider(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("Button", CtlName, WS_VISIBLE|WS_CHILD, x, y, 150, 50, pwnd, NULL, pins, NULL); 
	CtlWnd=CreateWindowA("Button", "<", WS_VISIBLE|WS_CHILD, x-50, y, 50, 50, pwnd, NULL, pins, NULL); 
	CtlWnd=CreateWindowA("Button", ">", WS_VISIBLE|WS_CHILD, x+150, y, 50, 50, pwnd, NULL, pins, NULL); 
}

HWND CMenuControlSlider::GetHandle(UINT wh)
{
	switch(wh)
	{
	case 0:
		return CtlWnd;
	case 1:
		return OwnWndL;
	case 2:
		return OwnWndR;
	default:
		return CtlWnd;
	}
}

void CMenuControlSlider::Activity(BOOL bAct)
{
	if(bAct)
	{
		SetWindowLong(CtlWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE);
		SetWindowLong(OwnWndL, GWL_STYLE, WS_CHILD|WS_VISIBLE);
		SetWindowLong(OwnWndR, GWL_STYLE, WS_CHILD|WS_VISIBLE);	
	}
	else
	{
		SetWindowLong(CtlWnd, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_DISABLED);	
		SetWindowLong(OwnWndL, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_DISABLED);
		SetWindowLong(OwnWndR, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_DISABLED);
	}
}

void CMenuControlSlider::Visibility(BOOL bShow)
{
	ShowWindow(CtlWnd, bShow?SW_SHOW:SW_HIDE);
	ShowWindow(OwnWndL, bShow?SW_SHOW:SW_HIDE);
	ShowWindow(OwnWndR, bShow?SW_SHOW:SW_HIDE);
}

CMenuControlSlider::~CMenuControlSlider()
{
	DestroyWindow(CtlWnd);
}

CMenuControlCheckbox::CMenuControlCheckbox(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("Checkbox", CtlName, WS_VISIBLE|WS_CHILD, x, y, 150, 50, pwnd, NULL, pins, NULL); 
}

CMenuControlCheckbox::~CMenuControlCheckbox()
{
	DestroyWindow(CtlWnd);
}

CMenuControlImage::CMenuControlImage(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("Button", CtlName, WS_VISIBLE|WS_CHILD, x, y, 80, 40, pwnd, NULL, pins, NULL); 
}

CMenuControlImage::~CMenuControlImage()
{
	DestroyWindow(CtlWnd);
}

CMenuControlField::CMenuControlField(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("EDIT", CtlName, WS_VISIBLE|WS_CHILD, x, y, 150, 40, pwnd, NULL, pins, NULL); 
}

CMenuControlField::~CMenuControlField()
{
	DestroyWindow(CtlWnd);
}

CMenuControlText::CMenuControlText(HINSTANCE pins, HWND pwnd, int x, int y, int var1, LPCSTR cname) : CMenuControl(pins, pwnd, x, y, var1, cname)
{
	CtlWnd=CreateWindowA("STATIC", CtlName, WS_VISIBLE|WS_CHILD, x, y, 150, 40, pwnd, NULL, pins, NULL); 	
}

CMenuControlText::~CMenuControlText()
{
	DestroyWindow(CtlWnd);
}

SMenuMenu::SMenuMenu(HINSTANCE pins, HWND pwnd, UINT order, UINT x, UINT y, LPCSTR name)
{
	snext=NULL;
	ThisEntry=new CMenuControlButton(pins, pwnd, x, y+50*order, 0, "=");
}

SMenuMenu::~SMenuMenu()
{
	if(ThisEntry)
		delete ThisEntry;
	if(snext)
		delete snext;
}

SMenuTabs::SMenuTabs(HINSTANCE pins, HWND pwnd, UINT order, LPCSTR name)
{
	snext=NULL;
	ThisTab=new CMenuControlButton(pins, pwnd, 100+150*order, 50, 0, name); 
}

SMenuTabs::~SMenuTabs()
{
	if(ThisTab)
		delete ThisTab;
	if(snext)
		delete snext;
}

SMenuList::SMenuList(HINSTANCE pins, HWND pwnd, UINT order, UINT x, UINT y, LPCSTR name)
{
	snext=NULL;
	ThisEntry=new CMenuControlButton(pins, pwnd, x, y+order*50, 0, name);
}

SMenuList::~SMenuList()
{
	if(ThisEntry)
		delete ThisEntry;
	if(snext)
		delete snext;
}

SMenuControls::SMenuControls(CMenuControl *anything)
{
	ThisThing=anything;
	snext=NULL;
}

void SMenuControls::ShowGroup(int gr)
{
	// hide everything
	ThisThing->Visibility(FALSE);
	if(LOBYTE(ThisThing->b)==gr)
		ThisThing->Visibility(TRUE);
	if(snext)
		snext->ShowGroup(gr);
}

SMenuControls::~SMenuControls()
{
	if(ThisThing)
		delete ThisThing;
	if(snext)
		delete snext;
}

SFrameChain::SFrameChain(LPCSTR fn)
{
	this->snext=NULL;
	this->fname=fn;
	this->ctrls=NULL;
	this->flags=0;
	this->back=NULL;
	this->bg=std::string("");
	this->list=NULL;
	this->tabs=NULL;
	this->menu=NULL;
}

void SFrameChain::Add(LPCSTR fn)
{
	if(snext)
		snext->Add(fn);
	else
		snext=new SFrameChain(fn);
}

SFrameChain *SFrameChain::Find(LPCSTR fn)
{
	if(!strcmp(fn, this->fname.c_str()))
		return this;
	if(snext)
		return snext->Find(fn);
	else
		return NULL;
}

SFrameChain::~SFrameChain()
{
	if(ctrls)
		delete ctrls;
	if(list)
		delete list;
	if(menu)
		delete menu;
	if(tabs)
		delete tabs;
	if(snext)
		delete snext;
}

CParserMeta::CParserMeta(LPCSTR name)
{

}

CParserMeta::~CParserMeta()
{

}