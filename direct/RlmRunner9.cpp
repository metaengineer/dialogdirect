#include "RlmRunner9.h"
#include <string>

const CHAR *DxWndClassName="RlmRunnerClass";

CRealm::CRealm(HINSTANCE pins, HWND pwnd, LPCSTR RlmDir)
{
	glob=NULL;
	IsVar=FALSE;
	roo=NULL;
	port=NULL;
	ParentInstance=pins;
	ParentWindow=pwnd;
	IsLoaded=FALSE;
	IsClass=FALSE;
	IsWindow=FALSE;
	IsInterface=FALSE;
	sk=NULL;
	size_t sdr=strlen(RlmDir)+1;
	RealmDir=new char[sdr];
	strcpy_s(RealmDir, sdr, RlmDir);

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

	if(IsClass)
	{
		RECT hfg;
		hfg.left=100;
		hfg.right=900;
		hfg.top=100;
		hfg.bottom=700;
		AdjustWindowRect(&hfg, WS_POPUP|WS_CAPTION, FALSE);
		DxWnd=CreateWindow(DxWndClassName, "RealmRunner", WS_OVERLAPPED|WS_VISIBLE, hfg.left, hfg.top, hfg.right-hfg.left, hfg.bottom-hfg.top,
			ParentWindow, NULL, ParentInstance, NULL);

		if(DxWnd)
			IsWindow=TRUE;

		roo=Direct3DCreate9(D3D_SDK_VERSION);
		if(roo)
			IsInterface=TRUE;
	}
}

BOOL CRealm::RegisterVars(SVarStorage *vs)
{
	glob=vs;
	IsVar=TRUE;
	RECT dis;
	dis.left=100;
	dis.right=900;
	dis.top=100;
	dis.bottom=700;
	switch((UINT)glob->GetVar(1))
	{
	case 0:
		dis.left=100;
		dis.right=740;
		dis.top=100;
		dis.bottom=580;
		break;
	case 2:
		dis.left=10;
		dis.right=1024;
		dis.top=35;
		dis.bottom=768;
		break;
	}
	AdjustWindowRect(&dis, WS_POPUP|WS_CAPTION, FALSE);
	MoveWindow(DxWnd, dis.left, dis.top, dis.right-dis.left, dis.bottom-dis.top, TRUE);
	return TRUE;
}

void CRealm::_CheckSurfaceFmts(COutHandler *oot, D3DDEVTYPE ana, BOOL uul, UINT y)
{
	HRESULT cr;
	cr=roo->CheckDeviceType(y, ana, D3DFMT_R5G6B5, D3DFMT_R5G6B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_R5G6B5, D3DFMT_X8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_R5G6B5, D3DFMT_A8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X1R5G5B5, D3DFMT_X1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X1R5G5B5, D3DFMT_X8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X1R5G5B5, D3DFMT_A8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A1R5G5B5, D3DFMT_R5G6B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A1R5G5B5, D3DFMT_A1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A1R5G5B5, D3DFMT_X8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A1R5G5B5, D3DFMT_A8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X8R8G8B8, D3DFMT_A1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A8R8G8B8, D3DFMT_R5G6B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A8R8G8B8, D3DFMT_X1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A8R8G8B8, D3DFMT_A1R5G5B5, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	cr=roo->CheckDeviceType(y, ana, D3DFMT_A8R8G8B8, D3DFMT_A8R8G8B8, uul);
	oot->FWriteByte(cr?"-":"+",1);
	oot->FWriteByte("\r\n",2);
}

void CRealm::_PrintCap(COutHandler *oot, D3DCAPS9 *wger)
{
	oot->FWriteByte("NEXT\r\n",6);
	oot->FWriteWholeNumberP(wger->Caps);
    oot->FWriteWholeNumberP(wger->Caps2);
    oot->FWriteWholeNumberP(wger->Caps3);
    oot->FWriteWholeNumberP(wger->PresentationIntervals);
    oot->FWriteWholeNumberP(wger->CursorCaps);
    oot->FWriteWholeNumberP(wger->DevCaps);
    oot->FWriteWholeNumberP(wger->PrimitiveMiscCaps);
    oot->FWriteWholeNumberP(wger->RasterCaps);
    oot->FWriteWholeNumberP(wger->ZCmpCaps);
    oot->FWriteWholeNumberP(wger->SrcBlendCaps);
    oot->FWriteWholeNumberP(wger->DestBlendCaps);
    oot->FWriteWholeNumberP(wger->AlphaCmpCaps);
    oot->FWriteWholeNumberP(wger->ShadeCaps);
    oot->FWriteWholeNumberP(wger->TextureCaps);
    oot->FWriteWholeNumberP(wger->TextureFilterCaps);
    oot->FWriteWholeNumberP(wger->CubeTextureFilterCaps);
    oot->FWriteWholeNumberP(wger->VolumeTextureFilterCaps);
    oot->FWriteWholeNumberP(wger->TextureAddressCaps);
    oot->FWriteWholeNumberP(wger->VolumeTextureAddressCaps);
    oot->FWriteWholeNumberP(wger->LineCaps);
    oot->FWriteWholeNumberP(wger->MaxTextureWidth);
    oot->FWriteWholeNumberP(wger->MaxTextureHeight);
    oot->FWriteWholeNumberP(wger->MaxVolumeExtent);
    oot->FWriteWholeNumberP(wger->MaxTextureRepeat);
    oot->FWriteWholeNumberP(wger->MaxTextureAspectRatio);
    oot->FWriteWholeNumberP(wger->MaxAnisotropy);

	oot->FWriteNumberFixedPoint(wger->MaxVertexW,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->GuardBandLeft,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->GuardBandTop,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->GuardBandRight,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->GuardBandBottom,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->ExtentsAdjust,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->MaxPointSize,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->PixelShader1xMaxValue,4);oot->FWriteByte("\r\n",2);
    oot->FWriteNumberFixedPoint(wger->MaxNpatchTessellationLevel,4);oot->FWriteByte("\r\n",2);

	oot->FWriteWholeNumberP(wger->StencilCaps);
    oot->FWriteWholeNumberP(wger->FVFCaps);
    oot->FWriteWholeNumberP(wger->TextureOpCaps);
    oot->FWriteWholeNumberP(wger->MaxTextureBlendStages);
    oot->FWriteWholeNumberP(wger->MaxSimultaneousTextures);
    oot->FWriteWholeNumberP(wger->VertexProcessingCaps);
    oot->FWriteWholeNumberP(wger->MaxActiveLights);
    oot->FWriteWholeNumberP(wger->MaxUserClipPlanes);
    oot->FWriteWholeNumberP(wger->MaxVertexBlendMatrices);
    oot->FWriteWholeNumberP(wger->MaxVertexBlendMatrixIndex);
    oot->FWriteWholeNumberP(wger->MaxPrimitiveCount);
    oot->FWriteWholeNumberP(wger->MaxVertexIndex);
    oot->FWriteWholeNumberP(wger->MaxStreams);
    oot->FWriteWholeNumberP(wger->MaxStreamStride);
    oot->FWriteWholeNumberP(wger->VertexShaderVersion);
    oot->FWriteWholeNumberP(wger->MaxVertexShaderConst);
    oot->FWriteWholeNumberP(wger->PixelShaderVersion);
    oot->FWriteWholeNumberP(wger->DevCaps2);
    oot->FWriteWholeNumberP(wger->MasterAdapterOrdinal);
    oot->FWriteWholeNumberP(wger->AdapterOrdinalInGroup);
    oot->FWriteWholeNumberP(wger->NumberOfAdaptersInGroup);
    oot->FWriteWholeNumberP(wger->DeclTypes);
    oot->FWriteWholeNumberP(wger->NumSimultaneousRTs);
    oot->FWriteWholeNumberP(wger->StretchRectFilterCaps);

	oot->FWriteWholeNumberP(wger->VertexTextureFilterCaps);
    oot->FWriteWholeNumberP(wger->MaxVShaderInstructionsExecuted);
    oot->FWriteWholeNumberP(wger->MaxPShaderInstructionsExecuted);
    oot->FWriteWholeNumberP(wger->MaxVertexShader30InstructionSlots);
    oot->FWriteWholeNumberP(wger->MaxPixelShader30InstructionSlots);
}

BOOL CRealm::Initialize()
{
	// for now: check DEVCAPS with REF and HAL modes, output them to file
	// no directx functions beyond Jun 2005 release
	if(!IsInterface)
		return FALSE;
	UINT nuads=roo->GetAdapterCount();
	COutHandler *oua=new COutHandler(ParentWindow);
	std::string wrkf=RealmDir;
	wrkf+="debug.txt";
	oua->COutInitCreate(wrkf.c_str());
	oua->FWriteByte("adapters ",9);
	oua->FWriteWholeNumber(nuads);
	oua->FWriteByte("\r\n\r\n",4);
	for(UINT y=0; y<nuads; y++)
	{
		oua->FWriteByte("adapter ",8);
		oua->FWriteWholeNumber(y);
		oua->FWriteByte("\r\n\r\n",4);
		D3DDEVTYPE ana=D3DDEVTYPE_REF;
		BOOL uul=FALSE;
		_CheckSurfaceFmts(oua, ana, uul, y);
		uul=TRUE;
		_CheckSurfaceFmts(oua, ana, uul, y);
		ana=D3DDEVTYPE_HAL;
		uul=FALSE;
		_CheckSurfaceFmts(oua, ana, uul, y);
		uul=TRUE;
		_CheckSurfaceFmts(oua, ana, uul, y);

		D3DADAPTER_IDENTIFIER9 er;
		roo->GetAdapterIdentifier(y, 0, &er);
		oua->FWriteByte(er.DeviceName, strlen(er.DeviceName));
		oua->FWriteByte("\r\n",2);
		oua->FWriteByte(er.Description, strlen(er.Description));
		oua->FWriteByte("\r\n",2);
		oua->FWriteByte(er.Driver, strlen(er.Driver));
		oua->FWriteByte("\r\n",2);

		D3DCAPS9 rfe;
		ana=D3DDEVTYPE_REF;
		roo->GetDeviceCaps(y, ana, &rfe);
		_PrintCap(oua, &rfe);
		ana=D3DDEVTYPE_HAL;
		roo->GetDeviceCaps(y, ana, &rfe);
		_PrintCap(oua, &rfe);
		sk->DropMessage("device information logged -> debug.txt\r\n", 0);
	}
	delete oua;

	// try device
	// if needed to disable hardware acceleration, add flavor code for that: var3=1
	D3DDEVTYPE kvo=IsVar?((BOOL)glob->GetVar(3)?D3DDEVTYPE_HAL:D3DDEVTYPE_REF):D3DDEVTYPE_HAL;
	parm.Windowed=(IsVar?(!((BOOL)glob->GetVar(2))):TRUE);
	parm.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
	parm.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;
	UINT har=(IsVar?(UINT)glob->GetVar(1):1);

	switch(har)
	{
	case 0:
		parm.BackBufferWidth=640;
		parm.BackBufferHeight=480;
		break;
	case 2:
		parm.BackBufferWidth=1024;
		parm.BackBufferHeight=768;
		break;
	default:
		parm.BackBufferWidth=800;
		parm.BackBufferHeight=600;
	break;
	}

	parm.BackBufferCount=1;
	parm.AutoDepthStencilFormat=D3DFMT_D16;
	parm.EnableAutoDepthStencil=FALSE; // for now
	parm.BackBufferWidth=0;
	parm.BackBufferHeight=0;
	parm.SwapEffect=D3DSWAPEFFECT_FLIP;
	parm.BackBufferFormat=D3DFMT_R5G6B5;
	parm.Flags=0;
	parm.hDeviceWindow=DxWnd;
	parm.MultiSampleQuality=0;
	parm.MultiSampleType=D3DMULTISAMPLE_NONE;
	if(parm.Windowed)
	{
		parm.BackBufferFormat=D3DFMT_UNKNOWN;
		parm.SwapEffect=D3DSWAPEFFECT_DISCARD;
	}
	else
	{
		// try a better format
		HRESULT cr=roo->CheckDeviceType(D3DADAPTER_DEFAULT, kvo, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, parm.Windowed);
		if(cr)
			parm.BackBufferFormat=D3DFMT_X8R8G8B8;
	}
	HRESULT frfrr;
	frfrr=roo->CreateDevice(D3DADAPTER_DEFAULT /*can't output to non-primary anyway*/,
		kvo, DxWnd, D3DCREATE_MIXED_VERTEXPROCESSING /*don't forget this was here*/,
		&parm, &port);
	if(SUCCEEDED(frfrr))
		IsLoaded=TRUE; // not setting anything important here
	return FALSE;
}

BOOL CRealm::RegisterMessageSink(CMessageCache *sink)
{
	sink->DropMessage("error log\r\n", 0);
	sk=sink;
	return FALSE;
}

void CRealm::OnFrame(DWORD tx)
{
	if(!IsLoaded)
		return;
	HRESULT study=port->TestCooperativeLevel();
	if(SUCCEEDED(study))
	{

	}
	else
	{
		if(study==D3DERR_DEVICENOTRESET)
		{
			port->Reset(&parm);
		}
	}
}

CRealm::~CRealm()
{
	if(IsLoaded)
	{
		port->Release();
		port=NULL;
	}
	if(IsInterface)
	{
		roo->Release();
		roo=NULL;
	}
	if(IsWindow)
		DestroyWindow(DxWnd);
	if(IsClass)
		UnregisterClass(DxWndClassName, ParentInstance);
	delete[] RealmDir;
}
