//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
// This is a part of the Microsoft Foundation Classes C++ library.
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include <stdarg.h>

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _AFXDLL
	#error file must be compiled with _AFXDLL
#endif

/////////////////////////////////////////////////////////////////////////////
// _AFXDLL support

static AFX_EXTENSION_MODULE coreDLL;
#ifdef _AFX_OLE_IMPL
AFX_MODULE_STATE* AFXAPI _AfxGetOleModuleState();
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynLinkLibrary class

IMPLEMENT_DYNAMIC(CDynLinkLibrary, CCmdTarget)

// Constructor - will wire into the current application's list
CDynLinkLibrary::CDynLinkLibrary(AFX_EXTENSION_MODULE& state, BOOL bSystem)
{
#ifndef _AFX_NO_OLE_SUPPORT
	m_factoryList.Construct(offsetof(COleObjectFactory, m_pNextFactory));
#endif
	m_classList.Construct(offsetof(CRuntimeClass, m_pNextClass));

	// copy info from AFX_EXTENSION_MODULE struct
	ASSERT(state.hModule != NULL);
	m_hModule = state.hModule;
	m_hResource = state.hResource;
	m_classList.m_pHead = state.pFirstSharedClass;
#ifndef _AFX_NO_OLE_SUPPORT
	m_factoryList.m_pHead = state.pFirstSharedFactory;
#endif
	m_bSystem = bSystem;

	// insert at the head of the list (extensions will go in front of core DLL)
	DEBUG_ONLY(m_pNextDLL = NULL);
	AfxLockGlobals(CRIT_DYNLINKLIST);
	m_pModuleState->m_libraryList.AddHead(this);
	AfxUnlockGlobals(CRIT_DYNLINKLIST);
}

CDynLinkLibrary::CDynLinkLibrary(HINSTANCE hModule, HINSTANCE hResource)
{
#ifndef _AFX_NO_OLE_SUPPORT
	m_factoryList.Construct(offsetof(COleObjectFactory, m_pNextFactory));
#endif
	m_classList.Construct(offsetof(CRuntimeClass, m_pNextClass));

	m_hModule = hModule;
	m_hResource = hResource;
	m_classList.m_pHead = NULL;
#ifndef _AFX_NO_OLE_SUPPORT
	m_factoryList.m_pHead = NULL;
#endif
	m_bSystem = FALSE;

	// insert at the head of the list (extensions will go in front of core DLL)
	DEBUG_ONLY(m_pNextDLL = NULL);
	AfxLockGlobals(CRIT_DYNLINKLIST);
	m_pModuleState->m_libraryList.AddHead(this);
	AfxUnlockGlobals(CRIT_DYNLINKLIST);
}

#ifdef AFX_TERM_SEG
#pragma code_seg(AFX_TERM_SEG)
#endif

CDynLinkLibrary::~CDynLinkLibrary()
{
	// remove this frame window from the list of frame windows
	AfxLockGlobals(CRIT_DYNLINKLIST);
	m_pModuleState->m_libraryList.Remove(this);
	AfxUnlockGlobals(CRIT_DYNLINKLIST);
}

/////////////////////////////////////////////////////////////////////////////
// CDynLinkLibrary diagnostics

#ifdef _DEBUG
void CDynLinkLibrary::AssertValid() const
{
	ASSERT(m_hModule != NULL);
}

void CDynLinkLibrary::Dump(CDumpContext& dc) const
{
	CCmdTarget::Dump(dc);

	dc << "m_hModule = " << (UINT)m_hModule;
	dc << "\nm_hResource = " << (UINT)m_hResource;

	if (m_hModule != NULL)
	{
		TCHAR szName[_MAX_PATH];
		GetModuleFileName(m_hModule, szName, _countof(szName));
		dc << "\nmodule name = " << szName;
	}
	else
		dc << "\nmodule name is unknown";

	dc << "\n";
}
#endif //_DEBUG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
// special initialization and helper functions

// This is called in an extension DLL's DllMain
//  It makes a copy of the DLL's HMODULE, as well as a copy of the
//  runtime class objects that have been initialized by this
//  extension DLL as part of normal static object construction
//  executed before DllMain is entered.

BOOL AFXAPI AfxInitExtensionModule(AFX_EXTENSION_MODULE& state, HMODULE hModule)
{
	// only initialize once
	if (state.bInitialized)
	{
		AfxInitLocalData(hModule);
		return TRUE;
	}
	state.bInitialized = TRUE;

	// save the current HMODULE information for resource loading
	ASSERT(hModule != NULL);
	state.hModule = hModule;
	state.hResource = hModule;

	// save the start of the runtime class list
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	state.pFirstSharedClass = pModuleState->m_classList.GetHead();
	pModuleState->m_classList.m_pHead = pModuleState->m_pClassInit;

#ifndef _AFX_NO_OLE_SUPPORT
	// save the start of the class factory list
	state.pFirstSharedFactory = pModuleState->m_factoryList.GetHead();
	pModuleState->m_factoryList.m_pHead = pModuleState->m_pFactoryInit;
#endif

	return TRUE;
}

#ifdef AFX_TERM_SEG
#pragma code_seg(AFX_TERM_SEG)
#endif

void AFXAPI AfxTermExtensionModule(AFX_EXTENSION_MODULE& state, BOOL bAll)
{
	// make sure initialized
	if (!state.bInitialized)
		return;

	// search for CDynLinkLibrary matching state.hModule and delete it
	ASSERT(state.hModule != NULL);
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL; )
	{
		CDynLinkLibrary* pNextDLL = pDLL->m_pNextDLL;
		if (bAll || pDLL->m_hModule == state.hModule)
			delete pDLL;    // will unwire itself
		pDLL = pNextDLL;
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);

	// delete any local storage attached to this module
	AfxTermLocalData(state.hModule, TRUE);

	// remove any entries from the CWnd message map cache
	AfxResetMsgCache();
}

/////////////////////////////////////////////////////////////////////////////
// special LoadLibrary and FreeLibrary for loading MFC extension DLLs

HINSTANCE AFXAPI AfxLoadLibrary(LPCTSTR lpszModuleName)
{
	ASSERT(lpszModuleName != NULL);
	AfxLockGlobals(CRIT_LOCKSHARED);
	HINSTANCE hInstLib = LoadLibrary(lpszModuleName);
	AfxUnlockGlobals(CRIT_LOCKSHARED);
	return hInstLib;
}

BOOL AFXAPI AfxFreeLibrary(HINSTANCE hInstLib)
{
	AfxLockGlobals(CRIT_LOCKSHARED);
	BOOL bResult = FreeLibrary(hInstLib);
	AfxUnlockGlobals(CRIT_LOCKSHARED);
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////
// Resource helpers

#ifdef AFX_CORE2_SEG
#pragma code_seg(AFX_CORE2_SEG)
#endif

HINSTANCE AFXAPI AfxFindResourceHandle(LPCTSTR lpszName, LPCTSTR lpszType)
{
	ASSERT(lpszName != NULL);
	ASSERT(lpszType != NULL);

	HINSTANCE hInst;

	// first check the main module state
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	if (!pModuleState->m_bSystem)
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, lpszType) != NULL)
			return hInst;
	}

	// check for non-system DLLs in proper order
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (!pDLL->m_bSystem && pDLL->m_hResource != NULL &&
			::FindResource(pDLL->m_hResource, lpszName, lpszType) != NULL)
		{
			// found it in a DLL
			AfxUnlockGlobals(CRIT_DYNLINKLIST);
			return pDLL->m_hResource;
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);

	// check language specific resource next
	hInst = pModuleState->m_appLangDLL;
	if (hInst != NULL && ::FindResource(hInst, lpszName, lpszType) != NULL)
		return hInst;

	// check the main system module state
	if (pModuleState->m_bSystem)
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, lpszType) != NULL)
			return hInst;
	}

	// check for system DLLs in proper order
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (pDLL = pModuleState->m_libraryList; pDLL != NULL; pDLL = pDLL->m_pNextDLL)
	{
		if (pDLL->m_bSystem && pDLL->m_hResource != NULL &&
			::FindResource(pDLL->m_hResource, lpszName, lpszType) != NULL)
		{
			// found it in a DLL
			AfxUnlockGlobals(CRIT_DYNLINKLIST);
			return pDLL->m_hResource;
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);

	// if failed to find resource, return application resource
	return AfxGetResourceHandle();
}

// AfxLoadString must not only check for the appropriate string segment
//   in the resource file, but also that the string is non-zero
int AFXAPI AfxLoadString(UINT nID, LPTSTR lpszBuf, UINT nMaxBuf)
{
	ASSERT(AfxIsValidAddress(lpszBuf, nMaxBuf*sizeof(TCHAR)));

	LPCTSTR lpszName = MAKEINTRESOURCE((nID>>4)+1);
	HINSTANCE hInst;
	int nLen;

	// first check the main module state
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	if (!pModuleState->m_bSystem)
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, RT_STRING) != NULL &&
			(nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf)) != 0)
		{
			// found a non-zero string in app
			return nLen;
		}
	}

	// check non-system DLLs in proper order
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (CDynLinkLibrary* pDLL = pModuleState->m_libraryList; pDLL != NULL;
		pDLL = pDLL->m_pNextDLL)
	{
		if (!pDLL->m_bSystem && (hInst = pDLL->m_hResource) != NULL &&
		  ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		  (nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf)) != 0)
		{
			AfxUnlockGlobals(CRIT_DYNLINKLIST);
			return nLen;
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);

	// check language specific DLL next
	hInst = pModuleState->m_appLangDLL;
	if (hInst != NULL && ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		(nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf)) != 0)
	{
		// found a non-zero string in language DLL
		return nLen;
	}

	// check the system module state
	if (pModuleState->m_bSystem)
	{
		hInst = AfxGetResourceHandle();
		if (::FindResource(hInst, lpszName, RT_STRING) != NULL &&
			(nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf)) != 0)
		{
			// found a non-zero string in app
			return nLen;
		}
	}

	// check system DLLs in proper order
	AfxLockGlobals(CRIT_DYNLINKLIST);
	for (pDLL = pModuleState->m_libraryList; pDLL != NULL; pDLL = pDLL->m_pNextDLL)
	{
		if (pDLL->m_bSystem && (hInst = pDLL->m_hResource) != NULL &&
		  ::FindResource(hInst, lpszName, RT_STRING) != NULL &&
		  (nLen = ::LoadString(hInst, nID, lpszBuf, nMaxBuf)) != 0)
		{
			AfxUnlockGlobals(CRIT_DYNLINKLIST);
			return nLen;
		}
	}
	AfxUnlockGlobals(CRIT_DYNLINKLIST);

	// did not find it
	lpszBuf[0] = '\0';
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Library initialization and cleanup

#ifdef _DEBUG
#define MSVCRT_DLL "MSVCRTD.DLL"
#else
#define MSVCRT_DLL "MSVCRT.DLL"
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

extern "C" BOOL WINAPI RawDllMain(HINSTANCE, DWORD dwReason, LPVOID);
extern "C" BOOL (WINAPI* _pRawDllMain)(HINSTANCE, DWORD, LPVOID) = &RawDllMain;

extern "C"
BOOL WINAPI RawDllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#if !defined(_WIN32_WCE)
		// Prevent the C runtime DLL from being unloaded prematurely
		LoadLibraryA(MSVCRT_DLL);

#ifdef _UNICODE
		// give error message and exit if running Unicode on non-Unicode system
		if (GetVersion() & 0x80000000)
		{
			// Note: this message is for programmers who can read english
			::MessageBoxA(NULL,
				"This application or DLL can not be loaded "
				"on Windows 95 or on Windows 3.1.  It takes advantage "
				"of Unicode features only available on Windows NT.",
				"MFC Runtime Module", MB_ICONSTOP|MB_OK);
			return FALSE; // and fail
		}
#endif

		SetErrorMode(SetErrorMode(0) |
			SEM_FAILCRITICALERRORS|SEM_NOOPENFILEERRORBOX);
#endif // _WIN32_WCE

		// add a reference to thread local storage data
		AfxTlsAddRef();

		// make sure we have enough memory to attempt to start (8kb)
		void* pMinHeap = LocalAlloc(NONZEROLPTR, 0x2000);
		if (pMinHeap == NULL)
			return FALSE;   // fail if memory alloc fails
		LocalFree(pMinHeap);

		// cause early initialization of _afxCriticalSection
		if (!AfxCriticalInit())
			return FALSE;

#ifdef _AFX_OLE_IMPL
		// set module state before initialization
		AFX_MODULE_STATE* pModuleState = _AfxGetOleModuleState();
		_AFX_THREAD_STATE* pState = AfxGetThreadState();
		pState->m_pPrevModuleState = AfxSetModuleState(pModuleState);
#endif
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
#ifdef _AFX_OLE_IMPL
		_AFX_THREAD_STATE* pThreadState = _afxThreadState.GetDataNA();
		if (pThreadState != NULL)
		{
			// restore previously-saved module state
			VERIFY(AfxSetModuleState(pThreadState->m_pPrevModuleState) ==
				_AfxGetOleModuleState());
			DEBUG_ONLY(pThreadState->m_pPrevModuleState = NULL);
		}
#endif

		// free up the _afxCriticalSection
		AfxCriticalTerm();

#if !defined(_WIN32_WCE)
		// Now it's OK for C runtime DLL to be unloaded (see LoadLibrary above)
		FreeLibrary(GetModuleHandleA(MSVCRT_DLL));
#endif // _WIN32_WCE

		// remove reference from thread local data
		AfxTlsRelease();
	}

	return TRUE;    // ok
}

extern "C"
#if defined(_WIN32_WCE)
BOOL WINAPI DllMain(HANDLE wce_hInstance, DWORD dwReason, LPVOID)
{
	HINSTANCE hInstance = (HINSTANCE)wce_hInstance;
#else // _WIN32_WCE
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
#endif // _WIN32_WCE
	if (dwReason == DLL_PROCESS_ATTACH)
	{
#ifdef _AFX_OLE_IMPL
		BOOL bRegister = !coreDLL.bInitialized;

		// shared initialization
		AFX_MODULE_STATE* pModuleState = _AfxGetOleModuleState();
		pModuleState->m_hCurrentInstanceHandle = hInstance;
		pModuleState->m_hCurrentResourceHandle = hInstance;
		pModuleState->m_pClassInit = pModuleState->m_classList.GetHead();
		pModuleState->m_pFactoryInit = pModuleState->m_factoryList.GetHead();
#endif

		// initialize this DLL's extension module
		VERIFY(AfxInitExtensionModule(coreDLL, hInstance));
#if defined(_WIN32_WCE_EMULATION)
		if(!wce_CheckEmulationLibs(hInstance))
			AfxAbort();
#endif // _WIN32_WCE_EMULATION

#ifdef _AFX_OLE_IMPL
		AfxWinInit(hInstance, NULL, &afxChNil, 0);

		// Register class factories in context of private module state
		if (bRegister)
			COleObjectFactory::RegisterAll();
#endif

#ifdef _AFX_OLE_IMPL
		// restore previously-saved module state
		VERIFY(AfxSetModuleState(AfxGetThreadState()->m_pPrevModuleState) ==
			_AfxGetOleModuleState());
		DEBUG_ONLY(AfxGetThreadState()->m_pPrevModuleState = NULL);
#endif

		// wire up this DLL into the resource chain
		CDynLinkLibrary* pDLL = new CDynLinkLibrary(coreDLL, TRUE);
		ASSERT(pDLL != NULL);
#ifndef _AFX_NO_OLE_SUPPORT
		pDLL->m_factoryList.m_pHead = NULL;
#endif

		// load language specific DLL
#if defined(_WIN32_WCE)
		HINSTANCE hLangDLL = wce_LoadLibraryA("\\WINDOWS\\" WCE_MFC_FILENAME(I.DLL));
#else // _WIN32_WCE
		// the DLL must be in the "system directory"
		static const char szPrefix[] = "\\MFC42";
		static const char szLOC[] = "LOC";
		static const char szDLL[] = ".DLL";
		char szLangDLL[_MAX_PATH+14]; // Note: 8.3 name
		GetSystemDirectoryA(szLangDLL, _countof(szLangDLL));
		lstrcatA(szLangDLL, szPrefix);

		// try MFC42LOC.DLL
		lstrcatA(szLangDLL, szLOC);
		lstrcatA(szLangDLL, szDLL);
		HINSTANCE hLangDLL = LoadLibraryA(szLangDLL);
#endif // _WIN32_WCE
		AFX_MODULE_STATE* pState = AfxGetModuleState();
		pState->m_appLangDLL = hLangDLL;

#ifdef _AFX_OLE_IMPL
		// copy it to the private OLE state too
		pModuleState->m_appLangDLL = hLangDLL;
#endif
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		// free language specific DLL
		AFX_MODULE_STATE* pState = AfxGetModuleState();
		if (pState->m_appLangDLL != NULL)
		{
			::FreeLibrary(pState->m_appLangDLL);
			pState->m_appLangDLL = NULL;
		}

		// free the DLL info blocks
		CDynLinkLibrary* pDLL;
		while ((pDLL = pState->m_libraryList) != NULL)
			delete pDLL;
		ASSERT(pState->m_libraryList.IsEmpty());

		// cleanup module state for this process
		AfxTermExtensionModule(coreDLL);

#ifdef _AFX_OLE_IMPL
		// set module state for cleanup
		ASSERT(AfxGetThreadState()->m_pPrevModuleState == NULL);
		AfxGetThreadState()->m_pPrevModuleState =
			AfxSetModuleState(_AfxGetOleModuleState());
#endif

		// cleanup module state in OLE private module state
		AfxTermExtensionModule(coreDLL, TRUE);

		// free any local data for this process/thread
		AfxTermLocalData(NULL, TRUE);
	}
	else if (dwReason == DLL_THREAD_DETACH)
	{
		AfxTermThread();
	}

	return TRUE;    // ok
}

////////////////////////////////////////////////////////////////////////////
// Special initialization entry point for controls

void AFXAPI AfxCoreInitModule()
{
	ASSERT(AfxGetModuleState() != AfxGetAppModuleState());

	// construct new dynlink library in this context for core resources
	CDynLinkLibrary* pDLL = new CDynLinkLibrary(coreDLL, TRUE);
	ASSERT(pDLL != NULL);
#ifndef _AFX_NO_OLE_SUPPORT
	pDLL->m_factoryList.m_pHead = NULL;
#endif

	// borrow resources from language specific DLL if loaded
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	AFX_MODULE_STATE* pAppState = AfxGetAppModuleState();
	if (pModuleState->m_appLangDLL == NULL)
		pModuleState->m_appLangDLL = pAppState->m_appLangDLL;
}

////////////////////////////////////////////////////////////////////////////
// COM entry points

#ifdef _AFX_OLE_IMPL
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	return AfxDllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllCanUnloadNow(void)
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	return AfxDllCanUnloadNow();
}

////////////////////////////////////////////////////////////////////////////
// Server registration

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return SELFREG_E_CLASS;

	return S_OK;
}

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return SELFREG_E_CLASS;

	return S_OK;
}

// This CWinApp is required so this module state has a CWinApp object!
CWinApp _afxOleWinApp;

/////////////////////////////////////////////////////////////////////////////
// static-linked version of AfxWndProc for use by this module

#undef AfxWndProc

LRESULT CALLBACK
AfxWndProcDllOle(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	return AfxWndProc(hWnd, nMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

// force initialization early
#pragma warning(disable: 4074)
#pragma init_seg(lib)

static AFX_MODULE_STATE _afxOleModuleState(TRUE, &AfxWndProcDllOle,
	_MFC_VER, TRUE);

AFX_MODULE_STATE* AFXAPI _AfxGetOleModuleState()
{
	return &_afxOleModuleState;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// Special code to wire up vector deleting destructors

#ifdef AFX_VDEL_SEG
#pragma code_seg(AFX_VDEL_SEG)
#endif
static void _AfxForceVectorDelete()
{
#ifdef _DEBUG
	ASSERT(FALSE);  // never called
#endif

	new CBitmap[2];
	new CBitmapButton[2];
	new CBrush[2];
	new CButton[2];
	new CByteArray[2];
	new CCmdTarget[2];
	new CComboBox[2];
	new CDC[2];
	new CDWordArray[2];
	new CDialog[2];
#if !defined(_WIN32_WCE_NO_CONTROLBARS)
	new CDialogBar[2];
#endif // _WIN32_WCE_NO_CONTROLBARS
	new CEdit[2];
	new CFile[2];
	new CFont[2];
	new CFrameWnd[2];
	new CGdiObject[2];
	new CListBox[2];
#if !defined(_WIN32_WCE_NO_OWNERDRAW_LBS)
    new CCheckListBox[2];
#endif // _WIN32_WCE_NO_OWNERDRAW_LBS
	new CMapPtrToPtr[2];
	new CMapPtrToWord[2];
	new CMapStringToOb[2];
	new CMapStringToPtr[2];
	new CMapStringToString[2];
	new CMapWordToOb[2];
	new CMapWordToPtr[2];
	new CMemFile[2];
	new CMenu[2];
WCE_DEL	new CMetaFileDC[2];
	new CObArray[2];
	new CObList[2];
	new CPalette[2];
	new CPen[2];
	new CPtrArray[2];
	new CPtrList[2];
	new CRectTracker[2];
	new CRgn[2];
	new CScrollBar[2];
WCE_DEL	new CSharedFile[2];
	new CSplitterWnd[2];
	new CStatic[2];
#if !defined(_WIN32_WCE_NO_CONTROLBARS)
	new CStatusBar[2];
#endif // _WIN32_WCE_NO_CONTROLBARS
	new CStdioFile[2];
	new CString[2];
	new CStringArray[2];
	new CStringList[2];
	new CThreadSlotData[2];
	new CTime[2];
	new CTimeSpan[2];
#if !defined(_WIN32_WCE_NO_CONTROLBARS)
	new CToolBar[2];
#endif // _WIN32_WCE_NO_CONTROLBARS
	new CUIntArray[2];
	new CWnd[2];
	new CWordArray[2];

WCE_DEL new CFileFind[2];
WCE_DEL	new CInternetSession[2];

WCE_DEL	new CDragListBox[2];
#if !defined(_WIN32_WCE_NO_CONTROLBARS)
	new CStatusBarCtrl[2];
#endif // _WIN32_WCE_NO_CONTROLBARS
	new CListCtrl[2];
	new CTreeCtrl[2];
	new CSpinButtonCtrl[2];
	new CSliderCtrl[2];
	new CProgressCtrl[2];
	new CHeaderCtrl[2];
WCE_DEL	new CHotKeyCtrl[2];
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	new CToolTipCtrl[2];
#endif // _WIN32_WCE_NO_TOOLTIPS
	new CTabCtrl[2];
WCE_DEL	new CAnimateCtrl[2];
	new CImageList[2];
#if !defined(_WIN32_WCE_NO_CONTROLBARS)
	new CToolBarCtrl[2];
#endif // _WIN32_WCE
WCE_DEL	new CRichEditCtrl[2];

	new CMirrorFile[2];
#if !defined(_WIN32_WCE_NO_DOCKBARS)
	new CDockState[2];
#endif // _WIN32_WCE_NO_DOCKBARS

	new CListView[2];
	new CTreeView[2];
	new CCommandLineInfo[2];
	new CDocManager[2];

WCE_DEL	new CPageSetupDialog[2];

WCE_DEL	new CSemaphore[2];
WCE_DEL	new CMutex[2];
WCE_DEL	new CEvent[2];
WCE_DEL	new CCriticalSection[2];

#ifdef _AFX_OLE_IMPL
// WinCE: this code doesn't get invoked because dllinit.cpp isn't in OLECE.DLL (non-monolithic build)
	new COleDataSource[2];
	new COleDispatchDriver[2];
	new COleDropSource[2];
	new CMonikerFile[2];
	new COleResizeBar[2];
	new CAsyncMonikerFile[2];
	new CCachedDataPathProperty[2];
	new CDataPathProperty[2];
	new COleStreamFile[2];
	new COleTemplateServer[2];
	new COleDataObject[2];
	new COleDropTarget[2];
	new COleIPFrameWnd[2];

	new COleDocIPFrameWnd[2];

	new COleVariant[2];
	new CRichEditView[2];
	new CRichEditCntrItem[2];
#endif

#ifdef _AFX_DB_IMPL
	new CDatabase[2];
	new CLongBinary[2];

	new CDaoWorkspace[2];
	new CDaoException[2];
	new CDaoDatabase[2];
	new CDaoRecordset[2];
#endif

// Net
#if !defined(_WIN32_WCE_NO_WINSOCK)
#ifdef _AFX_NET_IMPL
	new CAsyncSocket[2];
	new CSocket[2];
#endif
#endif // _WIN32_WCE_NO_WINSOCK
}
void (*_afxForceVectorDelete_mfc)() = &_AfxForceVectorDelete;

void _AfxBinaryCompatibleStubFunction()
{
	ASSERT(FALSE);
}

#ifndef _UNICODE
HINSTANCE AFXAPI AfxGetResourceHandleCompat()
{ 
	return AfxGetResourceHandle(); 
}
#endif

/////////////////////////////////////////////////////////////////////////////
