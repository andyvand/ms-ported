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
#include <ctype.h>

wchar_t* AFXAPI wce_AsciiToWide(wchar_t* ws, const char* s)
{
	wchar_t* pszSave = ws;
    while(*s) 
	{
		*ws = (wchar_t) *s;
		s++;
		ws++;
	}
	*ws = 0;

	return pszSave;
}

wchar_t* AFXAPI wce_AsciiToWide(const char* s)
{
	TCHAR *ws = new TCHAR[strlen(s)+1];
	if(s == NULL)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}
	
	return wce_AsciiToWide(ws, s);
}


int AFXAPI wce_wsprintfA(LPSTR szBuf, LPCSTR szFormat, UINT nArg);

// NOTE: Required by WCSTOD.OBJ in LIBC.LIB.
extern "C" int WINAPI wctomb(char *s, wchar_t wchar)
{
    *s = (char)wchar;
    return sizeof(char);
}


LPSTR AFXAPI wce_T2CAHelper(LPSTR lpszDest, LPCTSTR lpcszSrc)
{
	LPSTR lpszReturn = lpszDest;
	LPTSTR lpszSrc = (LPTSTR)lpcszSrc;
    if ((lpszSrc != NULL) && (lpszDest != NULL))
	{
		do {
			*lpszDest++ = (char) *lpszSrc;
		} while(*lpszSrc++);
	}
	return lpszReturn;
}

FARPROC AFXAPI wce_GetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
    TCHAR szProcName[256] = _T("");
	wce_AsciiToWide(szProcName, lpProcName);
    return GetProcAddressW(hModule, szProcName);
}

BOOL AFXAPI wce_SystemParametersInfo(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
{
	ASSERT(uiAction == SPI_GETWORKAREA);
	ASSERT(uiParam == NULL);
	ASSERT(fWinIni == NULL);

	// making assumption that pvParam is a CRect* (see call to SystemParametersInfo in wincore.cpp)
	CRect* pRect = (CRect*)pvParam; 
	pRect->SetRect(0, 0, 
		wce_GetSystemMetrics(SM_CXSCREEN), 
		wce_GetSystemMetrics(SM_CYSCREEN));
	return TRUE;
}

HMODULE AFXAPI wce_LoadLibraryA(LPCSTR lpLibFileName)
{
	HMODULE hInst           = 0;
	wchar_t szDLL[MAX_PATH] = _T("");

    wce_AsciiToWide(szDLL, lpLibFileName);
	hInst = LoadLibraryW(szDLL);

	return (HMODULE)hInst;
}											

BOOL AFXAPI wce_WinHelp(HWND hWndMain, LPCTSTR lpszHelpPath, UINT uCommand, DWORD dwData)
{
	TCHAR szHelpPathEx[MAX_PATH*2] = _T("");
	PROCESS_INFORMATION rProcInfo;

	ASSERT(lpszHelpPath != NULL);
	if (lpszHelpPath == NULL)
		return FALSE;

	_tcscpy(szHelpPathEx, _T("file:"));
	_tcscat(szHelpPathEx, lpszHelpPath);

	return CreateProcess(_T("PegHelp.exe"), (LPWSTR)szHelpPathEx,
		       NULL, NULL, FALSE, 0, NULL, NULL, NULL, &rProcInfo);
}

void AFXAPI wce_WaitMessage()
{
	MSG msg;

	while(!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		Sleep(100);
}

// Note: we aren't handling variable arguments in the wsprintfA workaround. 
// The only calls we're servicing are found in afxtrace.cpp.

int AFXAPI wce_wsprintfA(LPSTR szBuf, LPCSTR szFormat, UINT nArg)
{ 
	TCHAR *szwBuf 	 = new TCHAR[80]; //this is hardcoded to match the string size in AfxTraceMSg.
							   
	TCHAR *szwFormat = wce_AsciiToWide(szFormat);

	int nRetVal = 0;

	if((szwBuf != NULL) && (szwBuf != szwFormat))
	{
		nRetVal = wsprintf(szwBuf, szwFormat, nArg);

		// now "convert" the string back to ansi.
		// this only works because of the fact that we call this from only one place
		// and it is guaranteed to have non-localized data passed in.
   
		CHAR  * s 	= szBuf;
		TCHAR * ws 	= szwBuf;

    	while(*ws) 
		{
			*s = (char) *ws;
			s++;
			ws++;
		}
		*s = 0;
	}


	delete [] szwBuf;
	delete [] szwFormat;
	return nRetVal;
}

int AFXAPI wce_GetClipboardFormatNameA(UINT format, LPSTR lpszFormatName, int cchMaxCount)
{
	TCHAR * lpwszFormatName = new TCHAR[cchMaxCount];

	int nRetVal = 0;
	
	if(lpwszFormatName != NULL)
	{
		nRetVal = GetClipboardFormatName(format, lpwszFormatName, cchMaxCount); 
		
		lpwszFormatName[min(cchMaxCount-1,nRetVal)] = 0; //force a null term to act like desktop code.

		WideCharToMultiByte(CP_ACP, NULL, lpwszFormatName, wcslen(lpwszFormatName)+1,
							lpszFormatName, cchMaxCount, NULL, NULL);
	}

	   
	delete [] lpwszFormatName;

	return nRetVal;
}



BOOL AFXAPI wce_IsBadStringPtrA(LPCSTR lpsz, UINT ucchMax)
{
// WinCE: our workaround is to check to see if the first character 
// and last character are readable
	if(IsBadReadPtr(lpsz, 1))
		return FALSE;

	int nLen = (ucchMax == -1) ? strlen(lpsz) : 
		min((int)ucchMax, strlen(lpsz));
	return ::IsBadReadPtr(lpsz + nLen, 1);
}

BOOL AFXAPI wce_IsBadStringPtrW(LPCWSTR lpsz, UINT ucchMax)
{
// WinCE: our workaround is to check to see if the first character 
// and last character are readable
	if(IsBadReadPtr(lpsz, 1))
		return FALSE;

	int nLen = (ucchMax == -1) ? wcslen(lpsz) : 
		min((int)ucchMax, wcslen(lpsz));
	return ::IsBadReadPtr(lpsz + nLen, 1);
}

#if defined(_WIN32_WCE_EMULATION)
extern "C" DWORD WINAPI GetCurrentDirectoryW(DWORD nBufferLength, LPTSTR lpBuffer); 
BOOL AFXAPI wce_CheckEmulationLibs(HINSTANCE hInstance)
{
	TCHAR szCurrDir[MAX_PATH] = _T("");  
	TCHAR szMsg[256];
	HANDLE hLoadLibraryInstance;

	// Is MFC Dll in the emulation directory on the hard disk?
	if(!::GetModuleFileName(hInstance, szCurrDir, _MAX_PATH))
	{	
		VERIFY(::GetCurrentDirectoryW(MAX_PATH, szCurrDir));  // NOTE: Previously set by emulation's COREDLL.DLL.
		_stprintf(szMsg, _T("Initialization of the MFC application failed.  ")
						  _T("Is the EXE or DLL in the correct emulation directory?\n(e.g. %s)"),
						  szCurrDir);
		::MessageBox(NULL, szMsg, _T("Error"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	// Is the MFC Dll in the emulation directory on the hard disk?
	TCHAR* szSlash = wcsrchr(szCurrDir,'\\');
	if((hLoadLibraryInstance = LoadLibrary((szSlash == NULL)?szCurrDir:szSlash+1)) == NULL)
	{
		_stprintf(szMsg, _T("Initialization of the MFC application failed.  ")
						  _T("The MFC Dll is not in the emulation object store.\n(e.g. %s)"),
						  szCurrDir);
		::MessageBox(NULL, szMsg, _T("Error"), MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;
}
#endif // _WIN32_WCE_EMULATION

void AFXAPI wce_ReportDebugBreak(TCHAR* szFile, int nLine)
{
	TCHAR szBuf[100];
	wsprintf(szBuf,_T("Breakpoint at %s, line %d"),szFile,nLine);
	::MessageBox(NULL,szBuf,NULL,MB_OK);
}

UINT AFXAPI wce_GetMenuItemID(HMENU hMenu, int nPos)
{	
	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(mii));
	mii.cbSize = sizeof(mii); 
	mii.fMask  = MIIM_ID; 
	::GetMenuItemInfo(hMenu, nPos, TRUE, &mii);

	return mii.wID; 
}

HMENU AFXAPI wce_GetMenu(HWND hWnd)
{
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	if(pWnd == NULL)
		return NULL;

	return pWnd->m_hMenu;
}

BOOL AFXAPI wce_SetMenu(HWND hWnd, HMENU hMenu)
{
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	if(pWnd == NULL)
		return FALSE;

	pWnd->m_hMenu = hMenu;
	return TRUE;
}

UINT AFXAPI wce_GetMenuState(HMENU hMenu, UINT uId, UINT uFlags)
{
	MENUITEMINFO mii;

	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask  = MIIM_STATE;

	if (uFlags & MF_BYPOSITION)
		::GetMenuItemInfo(hMenu, uId, TRUE, &mii);
	else
		::GetMenuItemInfo(hMenu, uId, FALSE, &mii);

	return mii.fState;
}

BOOL AFXAPI wce_ModifyMenu(HMENU   hMenu,      // handle of menu 
                     UINT    uPosition,  // menu item to modify 
                     UINT    uFlags,     // menu item flags 
                     UINT    uIDNewItem, // menu item identifier or handle of drop-down menu or submenu 
                     LPCTSTR lpNewItem) // menu item content 
{
	// Handle MF_BYCOMMAND case
	if ((uFlags & MF_BYPOSITION) != MF_BYPOSITION)
	{	
		int nMax = wce_GetMenuItemCount(hMenu);
		int nCount = 0;
		while (uPosition != wce_GetMenuItemID(hMenu, nCount) && (nCount < nMax))
			nCount++;
		ASSERT(nCount < nMax);
		uPosition = nCount;
		uFlags |= MF_BYPOSITION;
	}

	if (!::DeleteMenu(hMenu, uPosition, uFlags))
		return FALSE;

	return ::InsertMenu(hMenu, uPosition, uFlags, uIDNewItem, lpNewItem);
}

int AFXAPI wce_GetMenuItemCount(HMENU hMenu)
{
	const int MAX_NUM_ITEMS = 256;
	int  iPos, iCount;

	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);

	iCount = 0;
	for (iPos = 0; iPos < MAX_NUM_ITEMS; iPos++)
	{
		if(!GetMenuItemInfo(hMenu, (UINT)iPos, TRUE, &mii))
			break;
		iCount++;
	}

	return iCount;
}

int AFXAPI wce_GetMenuString(HMENU hMenu, UINT uIDItem, LPWSTR lpString, int nMaxCount, UINT uFlag)
{ 
	MENUITEMINFO mii;
	memset((char *)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);

	if (!GetMenuItemInfo(hMenu, 0, TRUE, &mii))
		return 0;
	
	mii.fMask      = MIIM_TYPE;  // to get dwTypeData
	mii.fType      = MFT_STRING; // to get dwTypeData
	mii.dwTypeData = lpString;
	mii.cch        = nMaxCount;

	if (uFlag == MF_BYPOSITION)
		::GetMenuItemInfo(hMenu, uIDItem, TRUE, &mii);
	else
	{
		ASSERT(uFlag == MF_BYCOMMAND);
		::GetMenuItemInfo(hMenu, uIDItem, FALSE, &mii);
	}

	if (mii.dwTypeData != NULL)
		return _tcslen(lpString);

	return 0;
}

HPEN AFXAPI wce_CreatePen(int nPenStyle, int nWidth, COLORREF crColor)
{ 
	LOGPEN logPen;
	
	if(nPenStyle == PS_DOT)
		nPenStyle = PS_DASH;
	logPen.lopnStyle   = nPenStyle;
	if(nWidth == 0)
		nWidth = 1;
	logPen.lopnWidth.x = nWidth;
	logPen.lopnWidth.y = 1;
	logPen.lopnColor   = crColor;

	return ::CreatePenIndirect(&logPen); 
}

HBRUSH AFXAPI wce_CreateBrushIndirect(const LOGBRUSH* lplb)
{
	if (lplb->lbStyle == BS_SOLID)
		return ::CreateSolidBrush(lplb->lbColor);
	if (lplb->lbStyle == BS_NULL)	
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	if (lplb->lbStyle == BS_DIBPATTERNPT)
	{
		ASSERT(lplb->lbColor == DIB_RGB_COLORS);
		return ::CreateDIBPatternBrushPt((void*)lplb->lbHatch, lplb->lbColor);
	}
	TRACE0("LOGBRUSH style member is invalid");
	return NULL;
}

HFONT AFXAPI wce_CreateFont(int nHeight, int nWidth, int nEscapement,
		int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline,
		BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision,
		BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily,
		LPCTSTR lpszFacename)
{
	LOGFONT logFont;
	logFont.lfHeight = nHeight;
	logFont.lfWidth = nWidth;
	logFont.lfEscapement = nEscapement;
	logFont.lfOrientation = nOrientation;
	logFont.lfWeight = nWeight;
	logFont.lfItalic = bItalic;
	logFont.lfUnderline = bUnderline;
	logFont.lfStrikeOut = cStrikeOut;
	logFont.lfCharSet = nCharSet;
	logFont.lfOutPrecision = nOutPrecision;
	logFont.lfClipPrecision = nClipPrecision;
	logFont.lfQuality = nQuality;
	logFont.lfPitchAndFamily = nPitchAndFamily;
	lstrcpyn(logFont.lfFaceName, lpszFacename, _countof(logFont.lfFaceName));

	return ::CreateFontIndirect(&logFont);
}

HRGN AFXAPI wce_CreateRectRgn(int x1, int y1, int x2, int y2)
{
	CRect rect(x1,y1,x2,y2);
	return ::CreateRectRgnIndirect(&rect); 
}

BOOL AFXAPI wce_SetRectRgn(HRGN hrgn, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
{
	BOOL bRetVal = TRUE;
	HRGN hrgnCopy;
	hrgnCopy = ::wce_CreateRectRgn(nLeftRect, nTopRect, nRightRect, nBottomRect); 
	if (hrgnCopy == NULL)
		return FALSE;
	if (::CombineRgn(hrgn, hrgnCopy, NULL, RGN_COPY) == ERROR)
		bRetVal = FALSE;
	::DeleteObject(hrgnCopy);
	return bRetVal;
}

BOOL AFXAPI wce_GetBrushOrgEx(HDC hdc, LPPOINT lppt)
{
	if (::SetBrushOrgEx(hdc, 0, 0, lppt) == FALSE)
		return FALSE;
	return ::SetBrushOrgEx(hdc, lppt->x, lppt->y, NULL);
}

int AFXAPI wce_OffsetClipRgn(HDC hdc, int nXOffset, int nYOffset)
{
	int nRetVal;
	HRGN hrgn;
	hrgn = ::wce_CreateRectRgn(0,0,1,1); 
	if (::GetClipRgn(hdc, hrgn) == -1)
		nRetVal = ERROR;
	else
	{
		if (::OffsetRgn(hrgn, nXOffset, nYOffset) == ERROR)
			nRetVal = ERROR;
		else
			nRetVal = ::SelectClipRgn(hdc,hrgn);	
	}
	::DeleteObject(hrgn);
	return nRetVal;
}

int AFXAPI wce_ExtSelectClipRgn(HDC hdc, HRGN hrgn, int fnMode)
{
	ASSERT((hrgn != NULL) || (fnMode == RGN_COPY));
	int nRetVal;
	if (fnMode == RGN_COPY)
		nRetVal = ::SelectClipRgn(hdc, hrgn);	
	else
	{
		HRGN hrgnClip;
		hrgnClip = ::wce_CreateRectRgn(0,0,1,1); 
		if (::GetClipRgn(hdc, hrgnClip) == -1)
			nRetVal = ERROR;
		else 
		{
			if (::CombineRgn(hrgnClip, hrgnClip, hrgn, fnMode) == ERROR)
				nRetVal = ERROR;
			else 
				nRetVal = ::SelectClipRgn(hdc, hrgnClip);
		}
		::DeleteObject(hrgnClip);
	}
	return nRetVal;
}

int AFXAPI wce_ExcludeUpdateRgn(HDC hDC, HWND hWnd)
{
	int nRetVal;
	HRGN hrgn;
	hrgn = ::wce_CreateRectRgn(0,0,1,1); 
	nRetVal = ::GetUpdateRgn(hWnd, hrgn, FALSE);
	if (nRetVal != ERROR)
	{
		if (::wce_ExtSelectClipRgn(hDC, hrgn, RGN_DIFF) == ERROR)
			nRetVal = ERROR;
	}
	::DeleteObject(hrgn);
	return nRetVal;
}

BOOL AFXAPI wce_PolyPolyline(HDC hdc, const POINT* lppt, const DWORD* lpdwPolyPoints, DWORD cCount)
{
	if (cCount < 0)
		return FALSE;
	BOOL bResult = TRUE;
	int startIndex = 0;
	for (DWORD i=0; i < cCount && bResult; i++) 
	{
		ASSERT(lpdwPolyPoints[i] >= 2);
		bResult = ::Polyline(hdc, &(lppt[startIndex]), lpdwPolyPoints[i]);
		startIndex += lpdwPolyPoints[i]; // jump to next polyline
	}
	return bResult;
}

BOOL AFXAPI wce_PolyPolygon(HDC hdc, const POINT* lpPoints, const int* lpPolyCounts, int nCount)
{
	if (nCount < 2)
		return FALSE;
	BOOL bResult = TRUE;
	int startIndex = 0;
	for (int i=0; i < nCount && bResult; i++) 
	{
		ASSERT(lpPolyCounts[i] >= 2);
		bResult = ::Polygon(hdc, &(lpPoints[startIndex]), lpPolyCounts[i]);
		startIndex += lpPolyCounts[i]; // jump to next polygon
	}
	return bResult;
}

int AFXAPI wce_FrameRect(HDC hdc, const RECT* lprc, HBRUSH hbr)
{
	// Fill a "line-size" rectangle for each edge of the frame, using hbr
	// Note that right/bottom borders not painted by FillRect (or FrameRect)
	RECT rectEdge;	
	if (::SetRect(&rectEdge, lprc->left, lprc->top, lprc->right, 
				  lprc->top + 1) == FALSE) // Top edge of frame
		return FALSE;
	if (::FillRect(hdc, &rectEdge, hbr) == FALSE)
		return FALSE;
	
	if (::SetRect(&rectEdge, lprc->right - 1, lprc->top, lprc->right, 
				  lprc->bottom) == FALSE) // Right edge of frame
		return FALSE;
	if (::FillRect(hdc, &rectEdge, hbr) == FALSE)
		return FALSE;
	if (::SetRect(&rectEdge, lprc->left, lprc->bottom - 1, lprc->right, 
				  lprc->bottom) == FALSE) // Bottom edge of frame
		return FALSE;
	if (::FillRect(hdc, &rectEdge, hbr) == FALSE)
		return FALSE;
	if (::SetRect(&rectEdge, lprc->left, lprc->top, lprc->left + 1, 
				  lprc->bottom) == FALSE) // Left edge of frame
		return FALSE;
	return ::FillRect(hdc, &rectEdge, hbr);
}

LONG AFXAPI wce_RegCreateKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult)
{
	long lResult = 0;
	DWORD dwDisposition = 0;

	lResult = RegCreateKeyExW(hKey, lpSubKey, 0, NULL, NULL, KEY_ALL_ACCESS, NULL, 
		                      phkResult, &dwDisposition);
	return lResult;
}

LONG AFXAPI wce_RegQueryValue(HKEY hKey, LPCWSTR lpRegValue, LPWSTR lpValue, PLONG lpcbValue)
{
	long  lResult     = 0;
	DWORD dwValueType = 0;

	lResult = RegQueryValueExW((HKEY)hKey, (LPTSTR)lpRegValue, (LPDWORD)NULL, 
		(LPDWORD)&dwValueType, (LPBYTE)lpValue, (LPDWORD)lpcbValue);
	return lResult;
}

LONG AFXAPI wce_RegOpenKey(HKEY     hKey,	        // handle of open key 
                     LPCTSTR  lpSubKey,	    // address of name of subkey to open 
                     PHKEY  phkResult) 	// address of handle of open key 
{
	long lResult = 0;
	HANDLE hBuffer = NULL;

	lResult = RegOpenKeyExW(hKey, lpSubKey, 0, KEY_ALL_ACCESS, phkResult);
	if(lResult == ERROR_SUCCESS)
		return lResult;

	// Workaround:  the KEY_ALL_ACCESS above contains KEY_CREATE_SUB_KEY, except
	// that it's not supported in WinCE.  So if an error results, check to see if its
	// a value that's meant to be converted to a key (which is the behavior of the 
	// desktop RegOpenKey()).

	BYTE *lpBuffer;
	DWORD dwType;
	DWORD dwSize;
	long lPrevResult = lResult;
	
	lResult = RegQueryValueEx(hKey, lpSubKey, 0, &dwType, NULL, &dwSize);
	if(lResult != ERROR_SUCCESS)
		return lResult;

	ASSERT(dwSize > 0);
	hBuffer = LocalAlloc(LMEM_FIXED, dwSize);
	if(hBuffer == NULL)
		return ERROR_NOT_ENOUGH_MEMORY;

	lpBuffer = (BYTE*)WCE_FCTN(GlobalLock)(hBuffer);
	lResult = RegQueryValueEx(hKey, lpSubKey, 0, &dwType, lpBuffer, &dwSize);
	if(lResult != ERROR_SUCCESS)
	{
		lResult = lPrevResult;
		goto wceROK_ERROR;
	}

	lResult = RegDeleteValue(hKey, lpSubKey);
	if(lResult != ERROR_SUCCESS)
	{
		lResult = lPrevResult;
		goto wceROK_ERROR;
	}

	DWORD dwDisposition;
	lResult = RegCreateKeyEx(hKey, lpSubKey, 0, NULL, NULL, KEY_ALL_ACCESS, NULL, 
		                     phkResult, &dwDisposition);
	if(lResult != ERROR_SUCCESS)
	{
		lResult = lPrevResult;
		goto wceROK_ERROR;
	}

	lResult = RegSetValueEx(*phkResult, lpSubKey, 0, dwType, lpBuffer, dwSize);
	if(lResult != ERROR_SUCCESS)
	{
		lResult = lPrevResult;
		goto wceROK_ERROR;
	}

wceROK_ERROR:
	if(hBuffer != NULL)
	{
		LocalLock(hBuffer);
		LocalFree(hBuffer);
	}

	return lResult;

}

// RegSetValueEx is used as a workaround  for RegSetValue, but is not behaviorally
// equivalent. If the specified key doesn't exist and it's not a value, RegSetValue 
// creates a key, and applies the value to it.  
// RegSetValueEx doesn't do this, it rather creates another value under the specified
// prarent key.  Our override is to detect this circumstance, and creating a key
// manually before calling RegSetValueEx.
// Note: this workaround does not claim to be equivalent to RegSetValue.
LONG AFXAPI wce_RegSetValue(HKEY hKey, LPCTSTR lpSubKey, DWORD dwType, LPCTSTR lpData,  
					 DWORD cbData)
{
	LONG lResult;
	HKEY hSubKey;

	// If lpSubKey is NULL or an empty string, just delegate to the Ex function
	if((lpSubKey == NULL) || (*lpSubKey == _T('\0')))
	{
		return RegSetValueEx(hKey, lpSubKey, 0 /*reserved*/, dwType,
			(CONST BYTE*)lpData, (cbData + 1) * sizeof(TCHAR));
	}

	// NOTE: in the following, we do not want to pass in KEY_CREATE_SUB_KEY (nevermind the
	// fact that that particular flag isn't supported in WinCE 2.0).  We just want to do a 
	// test and don't want an automatic conversion from a value to a key.
	lResult = RegOpenKeyEx(hKey, lpSubKey, 0, 0, &hSubKey);
	if(lResult != ERROR_SUCCESS) 
	{	// it's not a key, so it's either a value or emptiness. So that means another test. 
		DWORD dwType,dwSize;
		lResult = RegQueryValueEx(hKey, lpSubKey, 0, &dwType, NULL, &dwSize);
		if(lResult == ERROR_SUCCESS) // it's a value
		{
			// We can call RegSetValueEx now because we want to keep it as a value
			return RegSetValueEx(hKey, lpSubKey, 0 /*reserved*/, dwType,
				(CONST BYTE*)lpData, (cbData + 1) * sizeof(TCHAR));
		}
		else // We have emptiness... create a key
		{
			DWORD dwDisposition;
			lResult = RegCreateKeyEx(hKey, lpSubKey, 0, NULL, NULL, KEY_ALL_ACCESS, NULL, 
									 &hSubKey, &dwDisposition);
			if(lResult != ERROR_SUCCESS)
				return lResult;
		}
	}

	// At this point, we have a hSubKey.  Set its default value.  
	// This can only be done with strings.
	ASSERT(dwType == REG_SZ);
	return RegSetValueEx(hSubKey, _T(""), 0 /*reserved*/, dwType,
		(CONST BYTE*)lpData, (cbData + 1) * sizeof(TCHAR));
}


LONG AFXAPI wce_RegEnumKey(HKEY    hKey,	    // handle of key to query 
                     DWORD   dwIndex,	// index of subkey to query 
                     LPTSTR  lpName,	// address of buffer for subkey name  
                     DWORD   cbName)	// size of subkey buffer 
{
	long      lResult;
	FILETIME  rFileTime;

	lResult = RegEnumKeyExW(hKey, dwIndex, lpName, &cbName, NULL, NULL,
                            NULL, &rFileTime);
	return lResult;
}


// Returns key for HKEY_CURRENT_USER\"Software"\RegistryKey\ProfileName,
// creating it if it doesn't exist.
// Responsibility of the caller to call RegCloseKey() on the returned HKEY.
// INI strings are not localized.
static const TCHAR szSoftware[]    = _T("\\Software");
static const TCHAR szRegistryKey[] = _T("Microsoft\\MFC_WCE");
static const TCHAR szProfileName[] = _T("afx.ini");
HKEY AFXAPI wce_GetAppRegistryKey()
{
	HKEY hAppKey     = NULL;
	HKEY hSoftKey    = NULL;
	HKEY hCompanyKey = NULL;
	long lResult     = 0;

	if (RegOpenKeyEx(HKEY_CURRENT_USER, szSoftware, 0, KEY_ALL_ACCESS, &hSoftKey) == ERROR_SUCCESS)
	{
		DWORD dw;
		if (RegCreateKeyEx(hSoftKey, szRegistryKey, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
			&hCompanyKey, &dw) == ERROR_SUCCESS)
		{
			long lResult = RegCreateKeyEx(hCompanyKey, szProfileName, 0, REG_NONE, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
				&hAppKey, &dw);
			ASSERT(lResult == ERROR_SUCCESS);
		}
		else
			ASSERT(FALSE);
	}
	if (hSoftKey != NULL)
	{
		lResult = RegCloseKey(hSoftKey);
		ASSERT(lResult == ERROR_SUCCESS);
	}
	if (hCompanyKey != NULL)
	{
		lResult = RegCloseKey(hCompanyKey);
		ASSERT(lResult == ERROR_SUCCESS);
	}

	return hAppKey;
}


// Returns key for:
//      HKEY_CURRENT_USER\"Software"\RegistryKey\AppName\lpszSection
// creating it if it doesn't exist.
// Responsibility of the caller to call RegCloseKey() on the returned HKEY.
HKEY AFXAPI wce_GetSectionKey(LPCTSTR lpszSection)
{
	long lResult = 0;

	ASSERT(lpszSection != NULL);

	HKEY hSectionKey = NULL;
	HKEY hAppKey = wce_GetAppRegistryKey();
	if (hAppKey == NULL)
		return NULL;

	DWORD dw;
	lResult = RegCreateKeyEx(hAppKey, lpszSection, 0, REG_NONE,
		REG_OPTION_NON_VOLATILE, KEY_WRITE|KEY_READ, NULL,
		&hSectionKey, &dw);
	ASSERT(lResult == ERROR_SUCCESS);

	lResult = RegCloseKey(hAppKey);
	ASSERT(lResult == ERROR_SUCCESS);

	return hSectionKey;
}



UINT AFXAPI wce_GetSystemDirectory(LPWSTR lpBuffer, UINT uSize)
{ 
	wcscpy(lpBuffer, _T("\\WINDOWS")); 
	return (UINT)wcslen(lpBuffer); 
}

UINT AFXAPI wce_GetSystemDirectoryA(LPSTR lpBuffer, UINT uSize)
{
        strcpy(lpBuffer,    "\\WINDOWS" );
        return (UINT)strlen(lpBuffer);
}

TCHAR* AFXAPI wce_GetNextArg(TCHAR* pszArgList, TCHAR szArg[])
{
	TCHAR*  pszSpace     = NULL;
	TCHAR*  pszTab       = NULL;
	TCHAR*  pszQuote     = NULL;
	TCHAR*  pszNextArg   = NULL;
	int     iWhiteCount  = 0;

	if ((pszArgList == NULL) || (lstrlen(pszArgList) == 0))
		return NULL;

	// count white space
	iWhiteCount = 0;
	pszSpace    = &pszArgList[0];
	while ((*pszSpace++ == (short)' ') || (*pszSpace++ == (short)'\t'))
		iWhiteCount++;

	// copy next arg to szArg
	lstrcpy(szArg, &pszArgList[iWhiteCount]);

	// null-terminate it
	// point pszNextArg at next arg
	pszQuote = szArg;		
	if (*szArg == (short)'"')
	{
		pszSpace = _tcschr(szArg+sizeof(TCHAR), (int)'"');
		if (pszSpace != NULL)
			pszQuote = pszSpace;
	}
	pszSpace = _tcschr(pszQuote, (int)' ');
	pszTab   = _tcschr(pszQuote, (int)'\t');
	if ((pszSpace == NULL) && (pszTab != NULL))
	{
		pszSpace = pszTab;
	}
	else if ((pszSpace != NULL) && (pszTab != NULL))
	{
		if (pszTab < pszSpace)
			pszSpace = pszTab;
	}
	if (pszSpace)
	{
		*pszSpace = 0;
		pszNextArg = pszSpace + 1;
	}
	else
		pszNextArg = &szArg[lstrlen(szArg)+1];

	// Get rid of quote around strings if there is any
	if (szArg[lstrlen(szArg)-1] == (short)'"')
		szArg[lstrlen(szArg)-1] = 0;
	if (*szArg == (short)'"')
		while (*szArg) 
			*szArg++ = *(szArg+1);

	// return pointer to beginning of next arg in list
	return pszNextArg;
}


int AFXAPI wce_GetArgCount(TCHAR* pszArgList)
{

	TCHAR*  pszSpace     = NULL;
	TCHAR*  pszTab       = NULL;
	TCHAR*  pszNextArg   = NULL;
	int     iWhiteCount  = 0;
	int     iArgCount    = 0;

	if ((pszArgList == NULL) || (lstrlen(pszArgList) == 0))
		return 0;

	iArgCount  = 0;
	pszNextArg = pszArgList;
	while (pszNextArg != NULL)
	{
		// gobble/count white space
		iWhiteCount = 0;
		pszSpace    = pszNextArg;
		while ((*pszSpace++ == (short)' ') || (*pszSpace++ == (short)'\t'))
			iWhiteCount++;
		// point pszNextArg past the white space
		pszNextArg = &pszNextArg[iWhiteCount];
		// if its not null, then increment the count
		if (pszNextArg != NULL)
			iArgCount++;
		// for next iteration, point pszNextArg at next arg
		if (*pszNextArg == (short)'"')
		{
			pszSpace = _tcschr(pszNextArg+sizeof(TCHAR), (int)'"');
			if (pszSpace != NULL)
				pszNextArg = pszSpace;
		}
		pszSpace = _tcschr(pszNextArg, (int)' ');
		pszTab   = _tcschr(pszNextArg, (int)'\t');
		if ((pszSpace == NULL) && (pszTab != NULL))
		{
			pszSpace = pszTab;
		}
		else if ((pszSpace != NULL) && (pszTab != NULL))
		{
			if (pszTab < pszSpace)
				pszSpace = pszTab;
		}
		if (pszSpace)
			pszNextArg = pszSpace + 1;
		else
			break;  // i.e. we're done
	}

	return iArgCount;
}

BOOL AFXAPI wce_ArgvW(int argc, char* argv[])
{
	TCHAR szTmp[MAX_PATH*2] = _T("");

	if (!argv)
		return TRUE;

	for (int i = 0; i < argc; i++)
	{
		if (argv[i])
		{
			// Convert the char* to wchar_t*.
			wce_AsciiToWide(szTmp, argv[i]);
			// Free the previous pointer.
			free(argv[i]);
			// Reset argv[i] to point at the wchar_t*.
			argv[i] = (char*)_tcsdup(szTmp);
			ASSERT(argv[i] != NULL);
			if (!argv[i])
				return FALSE;
		}
	}

	return TRUE;
}

DWORD AFXAPI wce_GetObjectStoreFreeSpace()
{ 
    STORE_INFORMATION stinfo;
	
	if (GetStoreInformation(&stinfo))
		return stinfo.dwFreeSize;

	return 0;
}

short AFXAPI wce_GetFileTitleW(LPCTSTR lpszFile,    // pointer to full path and filename for file
                         LPTSTR  lpszTitle,   // pointer to buffer that receives filename
                         WORD cbBuf)         // length of buffer
{
	TCHAR* pszSlashPlus1     = 0;
	int    iLen              = 0;

	if ((lpszFile == NULL) || (_tcslen(lpszFile)==0))
		return -1;

	// Find last backslash (if present).
	// Point at first char after last backslash.
	pszSlashPlus1 = (TCHAR *)_tcsrchr(lpszFile, (int)'\\');
	if (pszSlashPlus1 == NULL)
		pszSlashPlus1 = (TCHAR*)lpszFile;
	else
		pszSlashPlus1 = pszSlashPlus1 + 1;

	// Compute the length of new string and compare with cbBuf.
	iLen = _tcslen(pszSlashPlus1);
	if (iLen > (int)cbBuf)
		return -1;

	// Copy basename to lpszTitle.
	_tcscpy(lpszTitle, pszSlashPlus1);

	return 0;
}


UINT AFXAPI wce_GetTempFileName(LPCTSTR lpszPathName,       // address of directory name for temporary file
                          LPCTSTR lpszPrefixString,   // address of filename prefix
						  UINT	  uUnique,			  // dummy
                          LPTSTR  lpszTempFileName)  // address of buffer that receives the new filename
{
	TCHAR szPathNameCopy[_MAX_PATH] = _T("");

	uUnique = uUnique;

	// Remove trailing backslash.
	// NOTE: Make copy of lpszPathName (since we may need to modify it).
	_tcscpy(szPathNameCopy, lpszPathName);
	int iLen = _tcslen(szPathNameCopy);
	if (szPathNameCopy[iLen-1] == _T('\\'))
		szPathNameCopy[iLen-1] = _T('\0');

	// Check for duplicates. If a file with the resulting filename exists, the number is 
	// increased by one and the test for existence is repeated. Testing continues until a 
	// unique filename is found.
	
	CFileStatus status;
	time_t lnSecsSuffix = wce_time(NULL) & 0x000FFFFF;

	do {
		wsprintf(lpszTempFileName, _T("%s\\%s%5X.tmp"), szPathNameCopy, lpszPrefixString, 
			       lnSecsSuffix);
		++lnSecsSuffix;
	} while (CFile::GetStatus(lpszTempFileName, status));
		
	lnSecsSuffix--;
	return (UINT)lnSecsSuffix;
}


DWORD AFXAPI wce_GetFullPathName(LPCTSTR lpFileName, DWORD nBufferLength, LPWSTR lpBuffer, LPWSTR* lppFilePart)
{
	ASSERT(AfxIsValidString(lpFileName));
	ASSERT(lppFilePart != NULL);

	int nLen = _tcslen(lpFileName);
	ASSERT(AfxIsValidAddress(lpBuffer, nLen, TRUE));
	_tcscpy(lpBuffer, lpFileName);

	LPTSTR lpszSlash = _tcsrchr(lpBuffer, (int)'\\');
	if (lpszSlash == NULL)
		*lppFilePart = lpBuffer;
	else
		*lppFilePart = lpszSlash+1;

	return (DWORD)nLen;
}

BOOL AFXAPI wce_GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{ 
	SCROLLINFO si;

	memset( &si, 0, sizeof(SCROLLINFO) );
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_RANGE;

	if(!GetScrollInfo( hWnd, nBar, &si))
		return (FALSE);

	*lpMinPos = si.nMin;
	*lpMaxPos = si.nMax;

	return TRUE; 
}

int AFXAPI wce_GetScrollPos( HWND hWnd, int nBar)
{ 
	BOOL       bIsOK = FALSE;
	SCROLLINFO si;

	memset(&si, 0, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;

	if(!GetScrollInfo(hWnd, nBar, &si))
		return 0;

	return si.nPos; 
}

void AFXAPI wce_ScrollChildren(HWND hWnd, int cx, int cy)
{
	// WinCE does not perform any scrolling if the window is
	// not visible.  This leaves child windows unscrolled.
	// To account for this oversight, the child windows are moved
	// directly instead.
	HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
	if (hWndChild != NULL)
	{
		for (; hWndChild != NULL;
			hWndChild = ::wce_GetNextWindow(hWndChild, GW_HWNDNEXT))
		{
			RECT rect;
			::GetWindowRect(hWndChild, &rect);
			::ScreenToClient(hWnd,(POINT*)&rect.left);
			::SetWindowPos(hWndChild, NULL,	rect.left-cx, rect.top-cy,
				0, 0, SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
		}
	}
}


// Added the following workaround for InvalidateRgn.
BOOL AFXAPI wce_InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase)
{
	BOOL bReturn;
	HDC hdc = ::GetDC(hWnd);
	if(hdc == NULL) // if no DC, just erase the whole thing
		bReturn = ::InvalidateRect(hWnd,NULL,bErase);
	else
	{
		// Using the DC, get the smallest rectangle that encloses the region.
		RECT rect;
		SelectObject(hdc,hRgn);
		::GetClipBox(hdc,&rect);
		bReturn = ::InvalidateRect(hWnd,&rect,bErase);
		::ReleaseDC(hWnd,hdc);
	}

	return bReturn;
}

BOOL AFXAPI wce_EnumChildWindows( HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam )
{
    if (hWndParent == NULL)
            return (FALSE);

    HWND hWndChild = ::GetWindow( hWndParent, GW_CHILD );
    for (; hWndChild != NULL; hWndChild = ::wce_GetNextWindow(hWndChild, GW_HWNDNEXT))
    {
		lpEnumFunc( hWndChild, lParam );
        wce_EnumChildWindows( hWndChild, lpEnumFunc, lParam );
    }

    return (TRUE);
}

BOOL AFXAPI wce_RedrawWindow(HWND hWnd, CONST RECT *prcUpdate, HRGN hrgnUpdate, UINT afuRedraw)
{
    ::InvalidateRect(hWnd, prcUpdate, afuRedraw & RDW_ERASE ? TRUE : FALSE);
    if ((afuRedraw & RDW_ERASENOW) || (afuRedraw & RDW_UPDATENOW)) 
        ::UpdateWindow(hWnd);
    return TRUE;
}

BOOL AFXAPI wce_ShowScrollBar(HWND hWnd, int nBar, BOOL bShow)
{
// WinCE: this workaround doesn't work with x86em
#if defined(_WIN32_WCE_EMULATION)
	return TRUE;
#else // _WIN32_WCE_EMULATION
	DWORD dwStyle = ::GetWindowLong(hWnd, GWL_STYLE);
	if(bShow)
		dwStyle |= ((nBar == SB_HORZ) ? WS_HSCROLL : WS_VSCROLL);
	else
		dwStyle &= ~((nBar == SB_HORZ) ? WS_HSCROLL : WS_VSCROLL);
	return (::SetWindowLong(hWnd, GWL_STYLE, dwStyle) != 0);
#endif // _WIN32_WCE_EMULATION
	return TRUE;
}

int AFXAPI wce_GetSystemMetrics(int nIndex)
{
	if(nIndex == SM_DBCSENABLED)
		return TRUE;
	else
		return GetSystemMetrics(nIndex);
}

void AFXAPI wce_SendWM_CREATEToDialog(HWND hWnd)
{
	CREATESTRUCT cs; // We "recreate" this by doing detective work.
	TCHAR szDummy[2];

	int nLen = GetWindowText(hWnd, szDummy, 1); // query len
	cs.lpszName = new TCHAR[nLen+1];            // create buffer
	if(cs.lpszName == NULL)
		return; // If this fails, forget it.
	GetWindowText(hWnd, (TCHAR*)cs.lpszName, nLen); // un-const cast

	cs.lpszClass  = new TCHAR[64];
	GetClassName(hWnd, (TCHAR*)cs.lpszClass, 64);

	cs.style = GetWindowLong(hWnd, GWL_STYLE);
	cs.dwExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	cs.hwndParent = GetParent(hWnd);
	cs.hInstance = AfxGetInstanceHandle();

	RECT rectDlg;
	GetWindowRect(hWnd, &rectDlg);
	if(cs.hwndParent != NULL) // child window?
	{
		RECT rectParent;
		GetWindowRect(cs.hwndParent, &rectParent);
		OffsetRect(&rectDlg,-rectParent.left,-rectParent.right);
	}
	cs.x = rectDlg.left;
	cs.y = rectDlg.top;
	cs.cx = rectDlg.right-rectDlg.left;
	cs.cy = rectDlg.bottom-rectDlg.top;
	
	cs.hMenu = NULL;
	cs.lpCreateParams = 0; // We lost the real lParam when WM_CREATE got eaten by WinCE

	SendMessage(hWnd,WM_CREATE,0,(LPARAM)&cs);

	delete[] (TCHAR*)cs.lpszName;
	delete[] (TCHAR*)cs.lpszClass;
}

BOOL CALLBACK wce_FirstDlgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// This gets called only once--on the first message sent to the dialog window.

	// We don't own the dlgproc for property sheet, so we have to use the first message
	// for the first property page as our CBT Hook opportunity.  We fake the
	// missed WM_INITDIALOG hook.  MFC *will* handle this in dlgprop.cpp (a subset of
	// the original OnInitDialog implemention) and won't delegate back to the real
	// dlgproc for the property sheet (if it did, it'd be processing WM_INITDIALOG for
	// the second time.)
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	CWnd *pWnd = pThreadState->m_pWndInit;
	if(pThreadState->m_pWndInitPropertySheet != NULL)
	{
		HWND hWndPropertySheet = ::GetParent(hWnd);
		ASSERT(hWndPropertySheet != NULL);
		ASSERT(pThreadState->m_pWndInit != NULL);

		CWnd *pWndInitFirstPropPage = pThreadState->m_pWndInit;
		pThreadState->m_pWndInit    = pThreadState->m_pWndInitPropertySheet;
		_AfxCbtFilterHook(HCBT_CREATEWND, (WPARAM)hWndPropertySheet, NULL);
		pThreadState->m_pWndInit    = pWndInitFirstPropPage;

		((CPropertySheet*)pThreadState->m_pWndInitPropertySheet)->OnInitDialog();
		pThreadState->m_pWndInitPropertySheet = NULL;
	}

	// Make sure the standard dialog box loop is the OS-owned one
	::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)::DefDlgProc);

	// Let MFC subclass the dialog procedure.
	if(!pWnd->IsKindOf(RUNTIME_CLASS(CCommonDialog)))
	{
		// Common dialogs have OS-owned dialog procedures.  That's why 
		// MFC hooks with _AfxCommDlgProc. We don't want this relationship broken.
		::SetWindowLong(hWnd,DWL_DLGPROC,(LONG)AfxDlgProc); 
	}
	
	// Do normal WNDPROC subclassing now.  Note: after this call, m_pWndInit will
	// be NULL.  This is important in avoiding reentrancy via _AfxCommDlgProc.
	_AfxCbtFilterHook(HCBT_CREATEWND,(WPARAM)hWnd,NULL);

	// Delegate this message to the new subclassed WndProc
	WNDPROC afxWndProc = AfxGetAfxWndProc();

	// A nuance of WinCE: WM_CREATE on dialog boxes don't make it to the dialog procedure.
    // MFC depends on this on at least one count: the CFormView (which is a dialog) wants
    // to have its OnCreate() called.  Since we're at the actual first message, now 
    // is the best time to fake the WM_CREATE.  
	if( (pWnd != NULL) && (pWnd->IsKindOf(RUNTIME_CLASS( CFormView ) ) ) )
		wce_SendWM_CREATEToDialog(hWnd);

	// Repump the message through the entire chain.
	return afxWndProc(hWnd,nMsg,wParam,lParam);
};

#define WCE_CLASS_PREFIX _T("WCE_")

// Register a "hybrid" class (from the current one), substituting a new
// ClassName and WndProc.  This ensures that the first message is sent
// to wce_FirstDefWindowProc.
// NOTE: It is okay to "own" the ThreadState here because the baseline does 
//       something similar in AfxHookWindowCreate() with pThreadState->m_pWndInit.
BOOL AFXAPI wce_PreCreateWindow( CREATESTRUCT& cs)
{
	// Standalone menus are not supported in ::CreateWindowEx, so we need to filter it out.
	// We'll cache it later in wce_PostCreateWindow.
	if(HIWORD(cs.hMenu) != NULL)  // if it is a control ID, then its HIWORD == NULL
		cs.hMenu = NULL;

	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

	// Get the old class.  If it doesn't exist, then return FALSE.
	WNDCLASS wndclass;
	if (!GetClassInfo( cs.hInstance, cs.lpszClass, &wndclass))
		return FALSE;

	// Don't do anything with classes that begin with "Afx."  The 
	// wce_FirstDefWindowProc is already set for those in 
	// AfxRegisterClass() and AfxEndDeferRegisterClass()
	if(_tcsnicmp(cs.lpszClass,_T("Afx"),3) == 0)
	{
		pThreadState->m_pOldProc = ::DefWindowProc;
		// We leave cs.lpszClass alone
		return TRUE; // don't do anything on MFC windows
	}

	// Compose a new class name (e.g. "WCE_oldname"). We account for both
	// ways new might fail (exceptions vs. NULL ptr)
	TRY 
	{
		wndclass.lpszClassName = new TCHAR[_tcslen(cs.lpszClass) + 
	                                       _tcslen(WCE_CLASS_PREFIX) + 1];
	} 
	CATCH_ALL(e) 
	{
		return FALSE;
	} END_CATCH_ALL

	if(wndclass.lpszClassName == NULL)
		return FALSE;

	_tcscpy( (TCHAR*)wndclass.lpszClassName, WCE_CLASS_PREFIX );
	_tcscat( (TCHAR*)wndclass.lpszClassName, cs.lpszClass );
																			  
	// Change the window procedure, saving the old one.
	WNDPROC OldProc      = wndclass.lpfnWndProc;
	wndclass.lpfnWndProc = (WNDPROC)wce_FirstDefWindowProc;

	// Register the new class (if not already registered)
	if (!AfxRegisterClass(&wndclass))
	{
		delete[] (void*)wndclass.lpszClassName;
		return FALSE;
	}

	// If successful, commit our change to pThreadState and cs.
	pThreadState->m_pOldProc = OldProc;
	cs.lpszClass = wndclass.lpszClassName;

	// Note: cs.lpszClass will be deleted in wce_PostCreateWindow
	return TRUE;
}



LRESULT CALLBACK wce_FirstDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// This gets called only once.  We first plug in the normal DefWindowProc (because
	// The hook will add it to the chain after plugging in AfxWndProc.)  And then we
	// delegate to AfxWndProc.
	// Note: it's possible that the ::CreateEx returns without a chance to call this.  That
	// case is taken care of after CreateEx (does the same thing here minus the message
	// delegation.)

	// Retrieve the old WndProc.
	_AFX_THREAD_STATE* pThreadState = _afxThreadState.GetData();
	ASSERT(pThreadState);

	// Retrieve MFC's WndProc
	WNDPROC afxWndProc = AfxGetAfxWndProc();

	// Set current WndProc to oldproc.  After the hook function is finished, then oldproc
	// will be installed as the m_pfnSuper.

	ASSERT(pThreadState->m_pOldProc != NULL);
	::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)pThreadState->m_pOldProc);
	pThreadState->m_pOldProc = NULL;

	// Fake the call to _AfxCbtFilterHook
	_AfxCbtFilterHook( HCBT_CREATEWND, (WPARAM)hWnd, NULL );

	// Delegate.
	return afxWndProc(hWnd,nMsg,wParam,lParam);
}


void AFXAPI wce_PostCreateWindow( CREATESTRUCT& cs, HWND hWnd, HMENU nIDorHMenu)
{
	// Check to see if the hook has been invoked (sometimes we can have no messages
	// sent right away, which means wce_FirstDefWindowProc hasn't gotten called yet.)
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	if(pThreadState->m_pWndInit != NULL)
	{
		if(hWnd != NULL) 
		{
			ASSERT(pThreadState->m_pOldProc != NULL);
			::SetWindowLong(hWnd,GWL_WNDPROC,(LONG)pThreadState->m_pOldProc);
			pThreadState->m_pOldProc = NULL;
			_AfxCbtFilterHook( HCBT_CREATEWND, (WPARAM)hWnd, NULL );
		}	
		else
		{
			pThreadState->m_pWndInit = NULL;  
			pThreadState->m_pOldProc = NULL;
		}
	}

	if(_tcsnicmp(cs.lpszClass,WCE_CLASS_PREFIX,4) == 0)
		delete[] (void*)cs.lpszClass; // from new in wce_PreCreateWindow

	// Set the menu (wce_SetMenu just caches it away in CWnd)
	if((hWnd != NULL) && (HIWORD(nIDorHMenu) != NULL))
		wce_SetMenu(hWnd, nIDorHMenu);
}


LRESULT AFXAPI CALLBACK wce_NullWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// This window procedure is here to eat all messages after a window
	// has been detached.  The reason for this is that the WM_DESTROY
	// message below is "close" to being the last message, but we treat
	// it as such.  After that, we no longer have a CWnd->hWnd attachment,
	// so we use this WNDPROC to eat up any remaining messages.
	return 0;
}

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
BOOL AFXAPI wce_IsSipUp()
{
	SIPINFO si;
	memset(&si, 0, sizeof(SIPINFO));
	si.cbSize = sizeof(SIPINFO);
	if (!SHSipInfo(SPI_GETSIPINFO, 0, &si, 0))
	{
		TRACE0("Warning: Can not get the SIP Info.");
		return FALSE;
	}
	else
	{
		return ((si.fdwFlags & SIPF_ON) != 0);
	}
}
#endif // _WIN32_WCE_PSPC

// Workaround: the width of the property sheet is too big (with an error 
// proportional to the desired width). We correct for this here.
void AFXAPI wce_AdjustPropertySheetSize(CPropertySheet *pSheet)
{
	int nMaxWidth = 0, nMaxHeight = 0;

	ASSERT(pSheet->m_hWnd != NULL);

    for (int i = 0; i < pSheet->GetPageCount(); i++)
    {
		const DLGTEMPLATE* pTemplate;
        CPropertyPage* pPage = pSheet->GetPage(i);
		CSize size;

		if (pPage->m_psp.dwFlags & PSP_DLGINDIRECT)
		{
			pTemplate = pPage->m_psp.pResource;
		}
		else
		{
			HRSRC hResource = ::FindResource(pPage->m_psp.hInstance,
				pPage->m_psp.pszTemplate, RT_DIALOG);
			HGLOBAL hTemplate = LoadResource(pPage->m_psp.hInstance,
				hResource);
			pTemplate = (LPCDLGTEMPLATE)LockResource(hTemplate);
		}

		CDialogTemplate dlgTemplate(pTemplate);
		dlgTemplate.GetSizeInPixels(&size);
		if(size.cx > nMaxWidth)
			nMaxWidth = size.cx;
		if(size.cy > nMaxHeight)
			nMaxHeight = size.cy;
	}


	CTabCtrl* pWndTab = (CTabCtrl*)pSheet->GetDlgItem(AFX_IDC_TAB_CONTROL);
	if(pWndTab != NULL)
	{
		CRect rectWndTab, rectWndDlg;

		pSheet->GetWindowRect(rectWndDlg);
		pWndTab->GetWindowRect(rectWndTab);
		int dx = (rectWndTab.left-rectWndDlg.left) + 
		         (rectWndTab.right-rectWndDlg.right);
		int dy = (rectWndTab.top-rectWndDlg.top) + 
		         (rectWndTab.bottom-rectWndDlg.bottom);

		CRect rectDisplayTab = rectWndTab;
		pWndTab->AdjustRect(FALSE,&rectDisplayTab);
		int nButtonHeight = rectDisplayTab.top-rectWndTab.top;
		int nHSlop = 2;
		int nVSlop = 12;

		int nWidth  = nMaxWidth  + 
			::GetSystemMetrics(SM_CXEDGE)*2 +
			::GetSystemMetrics(SM_CXDLGFRAME)*2 + nHSlop;
		int nHeight = nMaxHeight + nButtonHeight + nVSlop + 
			::GetSystemMetrics(SM_CYEDGE)*2 + 
			::GetSystemMetrics(SM_CYDLGFRAME)*2 +
			::GetSystemMetrics(SM_CYCAPTION);

		pSheet->SetWindowPos(NULL, 0, 0, nWidth, nHeight,
				 	         SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		pWndTab->SetWindowPos(NULL, 0, 0, nWidth - dx, nHeight - dy,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

#if !defined(_WIN32_WCE_NO_WININET)
BOOL AFXAPI wce_InternetSetOptionEx(HINTERNET hInternet, DWORD dwOption,
                             LPVOID lpBuffer, DWORD dwBufferLength, DWORD)
{
	return InternetSetOption(hInternet, dwOption, lpBuffer, dwBufferLength);
}
#endif // _WIN32_WCE_NO_WININET

// Inline functions

#ifndef _AFX_ENABLE_INLINES

#ifdef AFX_DBG2_SEG
#pragma code_seg(AFX_DBG2_SEG)
#endif

static const char _szWceInl1Inl[] = "wcealt.inl";
#undef THIS_FILE
#define THIS_FILE _szWceAltInl
#define _AFXWIN_INLINE
#include <wcealt.inl>

#endif //!_AFX_ENABLE_INLINES

