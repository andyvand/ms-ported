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

#ifndef __WCEALT_H__ 
#define __WCEALT_H__

#if !defined(_WIN32_WCE_NO_WININET)
#include <wininet.h>
#endif

// The WinCE OS headers #defines the following, 
// but it interferes with MFC member functions.
#undef TrackPopupMenu
#undef DrawIcon
#undef SendDlgItemMessage
#undef SetDlgItemText
#undef GetDlgItemText
#undef LoadCursor

// ATLCONV.H support
#ifndef _ASSERTE
#define _ASSERTE ASSERT
#endif

#define wce_T2CA(lpszSrc) \
	(lpszSrc ? wce_T2CAHelper((LPSTR)_alloca((_tcslen(lpszSrc)+1)*2), lpszSrc) : NULL)

#if defined(_WIN32_WCE_NO_OLE)
// WinCE: compensate for not including atlconv.h in afxconv.h
#ifndef _DEBUG
#define USES_CONVERSION int _convert; _convert
#else
#define USES_CONVERSION int _convert = 0;
#endif
#define A2CT(lpa) (\
	((LPCSTR)lpa == NULL) ? NULL : (\
		_convert = (lstrlenA(lpa)+1),\
		(LPCWSTR)AfxA2WHelper((LPWSTR) alloca(_convert*2), lpa, _convert)\
	)\
)
#endif // _WIN32_WCE_NO_OLE

typedef struct tagHELPINFO     
{
}  HELPINFO;
typedef void* LPHELPINFO;

// WinCE: CESYSGEN prunes the following FRP defines, 
// and INTERNET_TRANSFER_TYPE_ASCII breaks in wininet.h
#undef FTP_TRANSFER_TYPE_ASCII
#define FTP_TRANSFER_TYPE_ASCII 0x00000001
#undef FTP_TRANSFER_TYPE_BINARY
#define FTP_TRANSFER_TYPE_BINARY 0x00000002

#define MM_TEXT 1
typedef DWORD OLE_COLOR;
#define WS_OVERLAPPEDWINDOW 0 

#ifndef MF_BITMAP
#define MF_BITMAP 0x00000004L
#endif

#ifndef WS_EX_CAPTIONOKBTN
#define WS_EX_CAPTIONOKBTN 0x80000000L
#endif

#ifndef WS_EX_NODRAG
#define WS_EX_NODRAG       0x40000000L
#endif

// Forward declarations
class  CWnd;
class  CFrameWnd;
struct CCreateContext;
class  CPropertySheet;

// WCEALT.CPP
BOOL      AFXAPI wce_SystemParametersInfo(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni);
FARPROC   AFXAPI wce_GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
HMODULE   AFXAPI wce_LoadLibraryA(LPCSTR lpLibFileName);
BOOL      AFXAPI wce_WinHelp(HWND hWndMain, LPCTSTR lpszHelp, UINT uCommand, DWORD dwData); 
wchar_t*  AFXAPI wce_AsciiToWide(wchar_t* ws, const char* s);
wchar_t*  AFXAPI wce_AsciiToWide(const char* s);
PSTR      AFXAPI wce_T2CAHelper(LPSTR lpszDest, LPCTSTR lpszSrc);
BOOL      AFXAPI wce_CheckEmulationLibs(HINSTANCE hInstance);
int       AFXAPI wce_GetClipboardFormatNameA(UINT format, LPSTR lpszFormatName, int cchMaxCount);
void      AFXAPI wce_WaitMessage();
int       AFXAPI wce_wsprintfA(LPSTR szBuf, LPCSTR szFormat, UINT nArg);
BOOL      AFXAPI wce_IsBadStringPtrA(LPCSTR lpsz, UINT ucchMax);
BOOL      AFXAPI wce_IsBadStringPtrW(LPCWSTR lpsz, UINT ucchMax);
void      AFXAPI wce_ReportDebugBreak(TCHAR *szFile, int nLine);
HPEN      AFXAPI wce_CreatePen(int nPenStyle, int nWidth, COLORREF crColor);
HBRUSH    AFXAPI wce_CreateBrushIndirect(const LOGBRUSH* lplb);
HFONT     AFXAPI wce_CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation, int nWeight, BYTE bItalic, BYTE bUnderline, BYTE cStrikeOut, BYTE nCharSet, BYTE nOutPrecision, BYTE nClipPrecision, BYTE nQuality, BYTE nPitchAndFamily, LPCTSTR lpszFacename);
HRGN      AFXAPI wce_CreateRectRgn(int x1, int y1, int x2, int y2);
BOOL      AFXAPI wce_SetRectRgn(HRGN hrgn, int nLeftRect, int nTopRect, int nRightRect, int nBottomRect);
BOOL      AFXAPI wce_GetBrushOrgEx(HDC hdc, LPPOINT lppt);
int       AFXAPI wce_OffsetClipRgn(HDC hdc, int nXOffset, int nYOffset);
int       AFXAPI wce_ExtSelectClipRgn(HDC hdc, HRGN hrgn, int fnMode);
int       AFXAPI wce_ExcludeUpdateRgn(HDC hDC, HWND hWnd);
BOOL      AFXAPI wce_PolyPolyline(HDC hdc, const POINT* lppt, const DWORD* lpdwPolyPoints, DWORD cCount);
BOOL      AFXAPI wce_PolyPolygon(HDC hdc, const POINT* lpPoints, const int* lpPolyCounts, int nCount);
int       AFXAPI wce_FrameRect(HDC hdc, const RECT* lprc, HBRUSH hbr);
UINT      AFXAPI wce_GetSystemDirectory(LPWSTR lpBuffer, UINT uSize);
UINT      AFXAPI wce_GetSystemDirectoryA(LPSTR lpBuffer, UINT uSize);
TCHAR*    AFXAPI wce_GetNextArg(TCHAR* pszArgList, TCHAR szArg[]);
int       AFXAPI wce_GetArgCount(TCHAR* pszArgList);
BOOL      AFXAPI wce_ArgvW(int argc, char* argv[]);
DWORD     AFXAPI wce_GetObjectStoreFreeSpace();
UINT      AFXAPI wce_GetTempFileName(LPCTSTR lpPathName, LPCTSTR lpPrefixString, UINT uUnique, LPTSTR lpTempFileName);
DWORD     AFXAPI wce_GetFullPathName(LPCTSTR lpFileName, DWORD nBufferLength, LPTSTR lpBuffer, LPTSTR *lpFilePart);
short     AFXAPI wce_GetFileTitleW(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf);
HMENU     AFXAPI wce_GetMenu(HWND hWnd);
BOOL      AFXAPI wce_SetMenu(HWND hWnd, HMENU hMenu);
UINT      AFXAPI wce_GetMenuItemID(HMENU hMenu, int nPos);
BOOL      AFXAPI wce_ModifyMenu(HMENU hMnu, UINT uPosition, UINT uFlags, UINT uIDNewItem, LPCTSTR lpNewItem);
int       AFXAPI wce_GetMenuItemCount(HMENU hMenu);
int       AFXAPI wce_GetMenuString(HMENU hMenu, UINT uIDItem, LPWSTR lpString, int nMaxCount, UINT uFlag);
UINT      AFXAPI wce_GetMenuState(HMENU hMenu, UINT uId, UINT uFlags);
BOOL      AFXAPI wce_InvalidateRgn(HWND hWnd, HRGN hRgn, BOOL bErase);
BOOL      AFXAPI wce_PreCreateWindow(CREATESTRUCT& cs);
void      AFXAPI wce_PostCreateWindow(CREATESTRUCT& cs, HWND hWnd, HMENU nIDorHMenu);
int       AFXAPI wce_GetScrollPos(HWND hWnd, int nBar);
BOOL      AFXAPI wce_GetScrollRange(HWND hWnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos);
void      AFXAPI wce_ScrollChildren(HWND hWnd, int cx, int cy);
BOOL      AFXAPI wce_ShowScrollBar(HWND hWnd, int nBar, BOOL bShow);
BOOL      AFXAPI wce_EnumChildWindows(HWND hWndParent, WNDENUMPROC lpEnumFunc, LPARAM lParam);
int       AFXAPI wce_GetSystemMetrics(int nIndex);
BOOL      AFXAPI wce_RedrawWindow(HWND hWnd, CONST RECT *prcUpdate, HRGN hrgnUpdate, UINT afuRedraw);
void      AFXAPI wce_AdjustPropertySheetSize(CPropertySheet *pSheet);
BOOL    CALLBACK wce_FirstDlgProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wce_NullWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK wce_FirstDefWindowProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
BOOL      AFXAPI wce_WriteProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString);
DWORD     AFXAPI wce_GetProfileStringW(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpDefault, LPWSTR lpReturnedString, DWORD nSize);
LONG      AFXAPI wce_RegQueryValue(HKEY hKey, LPCWSTR lpSubKey, LPWSTR lpValue, PLONG   lpcbValue);
LONG      AFXAPI wce_RegOpenKey(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult);
LONG      AFXAPI wce_RegSetValue(HKEY hKey, LPCWSTR lpSubKey, DWORD dwType, LPCWSTR lpData, DWORD cbData);
LONG      AFXAPI wce_RegCreateKey(HKEY hKey, LPCWSTR lpSubKey, PHKEY phkResult);
LONG      AFXAPI wce_RegEnumKey(HKEY hKey, DWORD dwIndex, LPTSTR lpName, DWORD cbName);
LONG      AFXAPI wce_RegQueryValue(HKEY hKey, LPCWSTR lpSubKey, LPWSTR lpValue, PLONG   lpcbValue);
#if !defined(_WIN32_WCE_NO_WININET)
BOOL	  AFXAPI wce_InternetSetOptionEx(HINTERNET hInternet, DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength, DWORD);
#endif // _WIN32_WCE_NO_WININET

// WCETIME.CPP
#ifndef _TM_DEFINED
#define _TM_DEFINED
struct tm {
        int tm_sec;     /* seconds after the minute - [0,59] */
        int tm_min;     /* minutes after the hour - [0,59] */
        int tm_hour;    /* hours since midnight - [0,23] */
        int tm_mday;    /* day of the month - [1,31] */
        int tm_mon;     /* months since January - [0,11] */
        int tm_year;    /* years since 1900 */
        int tm_wday;    /* days since Sunday - [0,6] */
        int tm_yday;    /* days since January 1 - [0,365] */
        int tm_isdst;   /* daylight savings time flag */
        };
#endif // _TM_DEFINED
long        AFXAPI wce_GetMessageTime();
time_t      AFXAPI wce_mktime(struct tm* );
struct tm * AFXAPI wce_localtime(const time_t *);
char*       AFXAPI wce_ctime(const time_t* );
time_t		AFXAPI wce_time(time_t *);

// WCEFRDLG.CPP
#define FR_DOWN                         0x00000001
#define FR_WHOLEWORD                    0x00000002
#define FR_MATCHCASE                    0x00000004
#define FR_FINDNEXT                     0x00000008
#define FR_REPLACE                      0x00000010
#define FR_REPLACEALL                   0x00000020
#define FR_DIALOGTERM                   0x00000040
#define FR_SHOWHELP                     0x00000080
#define FR_ENABLEHOOK                   0x00000100
#define FR_ENABLETEMPLATE               0x00000200
#define FR_NOUPDOWN                     0x00000400
#define FR_NOMATCHCASE                  0x00000800
#define FR_NOWHOLEWORD                  0x00001000
#define FR_ENABLETEMPLATEHANDLE         0x00002000
#define FR_HIDEUPDOWN                   0x00004000
#define FR_HIDEMATCHCASE                0x00008000
#define FR_HIDEWHOLEWORD                0x00010000
typedef UINT (APIENTRY *LPFRHOOKPROC) (HWND, UINT, WPARAM, LPARAM);
typedef struct tagFINDREPLACEW 
{
   DWORD        lStructSize;        // size of this struct 0x20
   HWND         hwndOwner;          // handle to owner's window
   HINSTANCE    hInstance;          // instance handle of.EXE that
                                    //   contains cust. dlg. template
   DWORD        Flags;              // one or more of the FR_??
   LPWSTR       lpstrFindWhat;      // ptr. to search string
   LPWSTR       lpstrReplaceWith;   // ptr. to replace string
   WORD         wFindWhatLen;       // size of find buffer
   WORD         wReplaceWithLen;    // size of replace buffer
   LPARAM       lCustData;          // data passed to hook fn.
   LPFRHOOKPROC lpfnHook;           // ptr. to hook fn. or NULL
   LPCWSTR      lpTemplateName;     // custom template name
} FINDREPLACE, *LPFINDREPLACE;
HWND AFXAPI wce_FindText(LPFINDREPLACE lpfr);
HWND AFXAPI wce_ReplaceText(LPFINDREPLACE lpfr);

// WCEOLE.CPP
void    AFXAPI wce_ReleaseStgMedium(LPSTGMEDIUM lpstgMedium);
void    AFXAPI wce_OleUninitialize();
SCODE   AFXAPI wce_OleInitialize(LPVOID pvReserved);
HRESULT AFXAPI wce_CoDisconnectObject(IUnknown*, unsigned long);
HRESULT AFXAPI wce_CreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM* ppstm);
HRESULT AFXAPI wce_OleLoadFromStream (LPSTREAM pStm, REFIID iidInterface, LPVOID FAR* ppvObj);
HRESULT AFXAPI wce_OleSaveToStream(LPPERSISTSTREAM pPStm, LPSTREAM pStm);
HRESULT AFXAPI wce_CLSIDFromProgID(LPCOLESTR, LPCLSID);
void    AFXAPI wce_UnregisterOleWindowClasses();
LPVOID  AFXAPI wce_CoTaskMemRealloc(LPVOID pv, ULONG cbOld, ULONG cb);
HRESULT AFXAPI wce_OleTranslateColor(OLE_COLOR clr, HPALETTE hpal, COLORREF* lpcolorref);
HRESULT AFXAPI wce_OleCreateFontIndirect(void* lpFontDesc, REFIID riid, LPVOID* lplpvObj);
HRESULT AFXAPI wce_RegGetMiscStatus(REFCLSID clsid, DWORD *pdwStatus);
WINOLEAPI_(UINT) CoTaskMemSize(LPVOID);
LRESULT CALLBACK wce_IMMWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);


// WinCE: Obsolete workaround from MFCCE 2.0
void inline ATLConnectSinks(CWnd *pCtrl) 
{ 
} 

// There is no new.h for WinCE, so we define a placement new here.
inline void * operator new(size_t, void * ptr) 
{ 
	return ptr; 
}

//and a delete operator to remove the compiler warning in a TRY/Catch when /GX enabled
inline void operator delete(void *, void *) 
{ 
	return; 
}

// WCEALT.INL, WCEALT.CPP: inline functions
BOOL    AFXAPI wce_LockWindowUpdate(HWND hWnd); 
UINT    AFXAPI wce_IsDlgButtonChecked(HWND hDlg, int nIDButton);
BOOL    AFXAPI wce_SetDlgItemText(HWND hDlg, int nIDDlgItem, LPCWSTR lpString);
LONG    AFXAPI wce_SendDlgItemMessage(HWND hDlg, int nIDDlgItem, UINT uMsg, WPARAM wParam, LPARAM lParam);
UINT    AFXAPI wce_GetDlgItemText(HWND hDlg, int nIDDlgItem, LPWSTR lpString, int nMaxCount);
void    AFXAPI wce_CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck);
BOOL    AFXAPI wce_IsIconic(HWND hWnd);
HWND    AFXAPI wce_GetTopWindow(HWND hWnd);
HWND    AFXAPI wce_GetNextWindow(HWND hWnd, UINT nDirection);
HWND    AFXAPI wce_GetDesktopWindow();
HWND    AFXAPI wce_GetLastActivePopup(HWND hWnd);
BOOL    AFXAPI wce_ScrollWindow(HWND hWnd, int xAmount, int yAmount, LPCRECT lpRect, LPCRECT lpClipRect);
BOOL    AFXAPI wce_IsMenu(HMENU hMenu);
BOOL    AFXAPI wce_TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, CONST RECT *prcRect);
BOOL    AFXAPI wce_InvertRect(HDC hdc, const RECT* lprc);
int     AFXAPI wce_GetMapMode(HDC hdc);
BOOL    AFXAPI wce_GetViewportOrgEx(HDC hdc, LPPOINT lpPoint);
BOOL    AFXAPI wce_GetViewportExtEx(HDC hdc, LPSIZE lpSize);
BOOL    AFXAPI wce_GetWindowOrgEx(HDC hdc, LPPOINT lpPoint);
BOOL    AFXAPI wce_GetWindowExtEx(HDC hdc, LPSIZE lpSize);
BOOL    AFXAPI wce_DPtoLP(HDC hdc, LPPOINT lpPoints, int nCount);
BOOL    AFXAPI wce_LPtoDP(HDC hdc, LPPOINT lpPoints, int nCount);
BOOL    AFXAPI wce_DrawIcon(HDC hDC, int X, int Y, HICON hIcon);
HCURSOR AFXAPI wce_LoadCursor(HINSTANCE hInstance,LPCWSTR lpCursorName);
HBITMAP AFXAPI wce_CreateBitmapIndirect(LPBITMAP lpBitmap);
void *  AFXAPI wce_memcpy(void *, const void *, size_t);

#ifdef _AFX_ENABLE_INLINES
#define _AFXWIN_INLINE AFX_INLINE
#include <wcealt.inl>
#endif

#endif // __WCEALT_H__

