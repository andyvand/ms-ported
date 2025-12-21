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
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
#define STRIKENUM_MAX 3
typedef struct _FindAppT
{
	TCHAR *pzExeName;
	HWND hwnd;
} 
FindAppT, *pFindAppT;

BOOL CALLBACK FindApplicationWindowProc(HWND hwnd, LPARAM lParam)
{
	DWORD		dwProcessID;
	INT		    iLen;
	TCHAR	    szTempName[MAX_PATH]=TEXT("\0");
	pFindAppT	pFindApp=(pFindAppT)lParam;

	::GetWindowThreadProcessId(hwnd,&dwProcessID);
	if (!dwProcessID) 
		return TRUE;

	iLen=::GetModuleFileName((HMODULE)dwProcessID,szTempName,MAX_PATH);
	if (!iLen) 
		return TRUE;

	if (!_tcsicmp(szTempName, pFindApp->pzExeName) )
	{	
		pFindApp->hwnd=hwnd;		
		return FALSE;
	}

	return TRUE;
}
#endif
/////////////////////////////////////////////////////////////////////////////
// Standard WinMain implementation
//  Can be replaced as long as 'AfxWinInit' is called first

int AFXAPI AfxWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow)
{
	ASSERT(hPrevInstance == NULL);

	int nReturnCode = -1;
	CWinThread* pThread = AfxGetThread();
	CWinApp* pApp = AfxGetApp();

	// AFX internal initialization
	if (!AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
		goto InitFailure;

	// App global initializations (rare)
	if (pApp != NULL && !pApp->InitApplication())
		goto InitFailure;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	// WinCE: Only one application instance can be run
	HANDLE hMuTex;
	TCHAR szTempName[MAX_PATH], szExeName[MAX_PATH];
	INT	i, iValue;
	FindAppT findApp;
	BOOL bGoAway;
	
	iValue=::GetModuleFileName(NULL,szExeName,MAX_PATH);			
	_tcscpy(szTempName,szExeName);
	for(i = 0; i < iValue; i++) 
	{
		if (szTempName[i]=='\\') 
			szTempName[i]='/';
	}

	hMuTex = ::CreateMutex(NULL, FALSE, szTempName);
	if (hMuTex!=NULL)
	{
		if (::GetLastError() == ERROR_ALREADY_EXISTS) 
		{
			bGoAway = FALSE;
			memset(&findApp,0,sizeof(FindAppT));
			findApp.pzExeName=(TCHAR *)szExeName;
			iValue=pThread->GetThreadPriority();

			for (i=0; i< STRIKENUM_MAX; i++) {
				::EnumWindows(FindApplicationWindowProc,(LPARAM)&findApp);

				if (findApp.hwnd) 
				{
					::SetForegroundWindow((HWND)(((DWORD)findApp.hwnd) | 0x01));		
					bGoAway=TRUE;
					break;
				}
				// wait for other app to finish starting or stopping
				pThread->SetThreadPriority(THREAD_PRIORITY_IDLE);
				Sleep(1000);    
				pThread->SetThreadPriority(iValue);
			}
			if (bGoAway) 
				goto InitFailure;
		}
	}
#endif	

	// Perform specific initializations
	if (!pThread->InitInstance())
	{
		if (pThread->m_pMainWnd != NULL)
		{
			TRACE0("Warning: Destroying non-NULL m_pMainWnd\n");
			pThread->m_pMainWnd->DestroyWindow();
		}
		nReturnCode = pThread->ExitInstance();
		goto InitFailure;
	}
	nReturnCode = pThread->Run();

InitFailure:
#ifdef _DEBUG
	// Check for missing AfxLockTempMap calls
	if (AfxGetModuleThreadState()->m_nTempMapLock != 0)
	{
		TRACE1("Warning: Temp map lock count non-zero (%ld).\n",
			AfxGetModuleThreadState()->m_nTempMapLock);
	}
	AfxLockTempMaps();
	AfxUnlockTempMaps(-1);
#endif

	AfxWinTerm();
	return nReturnCode;
}

/////////////////////////////////////////////////////////////////////////////
