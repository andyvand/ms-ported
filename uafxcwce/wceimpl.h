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

/////////////////////////////////////////////////////////////////////////////
//	WCEIMPL.H
//
// THIS FILE IS FOR MFCCE IMPLEMENTATION ONLY.

#ifndef __WCEIMPL_H__
#define __WCEIMPL_H__

// Define libraries to be used in MFC and OLE DLLs.
#if (_WIN32_WCE >= 201)
	#pragma comment(lib, "corelibc.lib")
	#pragma comment(linker, "/nodefaultlib:libc.lib")
	#pragma comment(linker, "/nodefaultlib:libcd.lib")
	#pragma comment(linker, "/nodefaultlib:libcmt.lib")
	#pragma comment(linker, "/nodefaultlib:libcmtd.lib")
	#pragma comment(linker, "/nodefaultlib:oldnames.lib")
#else
#ifdef _MT
	#if defined(_DEBUG)
		#pragma comment(lib, "libcmtd.lib")
	#else
		#pragma comment(lib, "libcmt.lib")
	#endif
#else
	#error Please use the /MT switch (multithreaded C-runtime)
#endif
#endif


#if defined(_AFX_OLE_IMPL)
	#if defined(_DEBUG)
		#pragma comment(lib, WCE_MFC_FILENAME(d.lib))
	#else
		#pragma comment(lib, WCE_MFC_FILENAME(.lib))
	#endif
	#pragma comment(lib, "uuid.lib")
#endif

// New front-end compiler complains of empty statements (solitary ;'s)
#pragma warning(disable: 4390)  // empty statements are okay

// ctlcore.cpp might contain an empty switch statement depending on pruning macros
#pragma warning(disable: 4060)  // empty statements are okay

#pragma comment(lib, "coredll.lib")
#pragma comment(lib, "commctrl.lib")
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
#pragma comment(lib, "aygshell.lib")
#pragma comment(lib, "doclist.lib")
#endif // _WIN32_WCE_PSPC

typedef struct tagNCCALCSIZE_PARAMS {
    RECT       rgrc[3];
    PWINDOWPOS lppos;
} NCCALCSIZE_PARAMS, *LPNCCALCSIZE_PARAMS;

// Missing typedefs
#ifndef _TIME_T_DEFINED
typedef unsigned long time_t;
#define _TIME_T_DEFINED 
#endif
typedef HANDLE  HDWP;
typedef HANDLE  HDROP;
typedef wchar_t _TUCHAR;
typedef LPVOID  LPPRINTER_DEFAULTS;
//typedef LPVOID  LPCHOOSEFONT;
#if (_WIN32_WCE < 210)
typedef LPVOID  LPPAGESETUPDLG;
#endif // _WIN32_WCE < 210
typedef UINT    UWORD;

// Missing headers
#include "prsht.h"    

// Missing string functions aliases
#ifndef _istlead
#define _istlead(ch) (FALSE)  
#endif
#ifndef _vsnwprintf
#define _vsnwprintf(a,b,c,d) vswprintf(a,c,d)
#endif
#define lstrcmpA     strcmp
#define lstrcatA     strcat
#define _ttoi        _wtoi
#define _itot        _itow
#define _ltot        _ltow
#define _ultot       _ultow
#define lstrlenA     strlen
#define lstrcpyA     strcpy
#define lstrcpyn     _tcsncpy
#define _tcstol      wcstol
#define _tcstoul     wcstoul
#define _tcstod      wcstod
#define strtod       wcstod
#define _tcsdec      _wcsdec
#define _tcsinc      _wcsinc
__inline wchar_t * __cdecl _wcsdec(const wchar_t * start, const wchar_t * current)
	{ return (wchar_t *) ( (start>=current) ? NULL : (current-1) ); }
__inline wchar_t * __cdecl _wcsinc(const wchar_t * _pc) { return (wchar_t *)(_pc+1); }
__inline size_t    __cdecl _tclen( const wchar_t *_cpc) { return 2; } // for UNICODE

// Missing definitions: not necessary equal to their Win32 values 
// (the goal is to just have a clean compilation of MFC)
#define BS_USERBUTTON             BS_PUSHBUTTON
#if (_WIN32_WCE < 300) || (defined(_WIN32_WCE_PSPC) && (_WIN32_WCE == 300))
#define WS_THICKFRAME             WS_DLGFRAME
#endif //_WIN32_WCE
#define WS_MAXIMIZE               0
#define WS_MINIMIZE               0
#define WS_EX_CONTROLPARENT       0x00010000L
#ifndef WS_EX_LEFTSCROLLBAR
#define WS_EX_LEFTSCROLLBAR       0
#endif
#ifndef WS_EX_TOOLWINDOW
#define WS_EX_TOOLWINDOW          0
#endif
#define WS_EX_NOPARENTNOTIFY      0 
#define WM_ENTERIDLE              0x0121
#define WM_PRINT                  WM_PAINT
#define WM_NCCREATE               (0x0081)
#define WM_PARENTNOTIFY           0
#define WM_NCDESTROY              (WM_APP-1)
#ifndef SW_RESTORE
#define SW_RESTORE                (SW_SHOWNORMAL)
#endif
#define SW_NORMAL                 (SW_SHOWNORMAL)
#define SW_SHOWMINNOACTIVE	      (SW_HIDE)
#define MB_TYPEMASK               (0x0000000FL)
#define MB_ICONMASK               (0x000000F0L)
#define CTLCOLOR_SCROLLBAR        CTLCOLOR_EDIT
#define PSM_CANCELTOCLOSE         (WM_USER + 107)
#define ESB_ENABLE_BOTH           (0x0000)
#define RDW_FRAME                 (0x0400)
#define RDW_NOFRAME               (0x0800)
#ifndef DCX_CACHE
#define DCX_CACHE                 (0x00000002L)
#endif
#ifndef INTERNET_HANDLE_TYPE_GOPHER_FILE
#define INTERNET_HANDLE_TYPE_GOPHER_FILE -1
#endif
#ifndef INTERNET_SERVICE_GOPHER
#define INTERNET_SERVICE_GOPHER   0
#endif
#define WAIT_OBJECT_0             0x00000000L
#define PRF_CHILDREN              0x00000010L
#define PRF_CLIENT                0x00000004L
#define HELP_HELPFILE             (0x0000L)
#define MSGF_MENU                 2
#define pshHelp                   0x040E
#define SM_DBCSENABLED            42 
#define MF_BITMAP                 0x00000004L
#define MF_DISABLED               0
#define FW_REGULAR                FW_NORMAL
#define MB_TASKMODAL              0
#define MB_SYSTEMMODAL            MB_APPLMODAL
#define PDERR_DNDMMISMATCH	      0x1009
#define PDERR_DEFAULTDIFFERENT    0x100C
#define IDB_HIST_SMALL_COLOR      8
#define IDB_HIST_LARGE_COLOR      9
#define DEFAULT_GUI_FONT          SYSTEM_FONT
#define SFGAO_LINK                0x00010000L   
#ifndef _MAX_FNAME
#define _MAX_FNAME                 64
#endif
#ifndef SWP_NOREDRAW
#define SWP_NOREDRAW               0
#endif
#ifndef SBS_SIZEBOX
#define SBS_SIZEBOX               0
#endif
#ifndef SBS_SIZEGRIP
#define SBS_SIZEGRIP              0
#endif
#define SC_SIZE                   (0xF000)
#define WSAGETSELECTEVENT(lParam) LOWORD(lParam)
#define WSAGETSELECTERROR(lParam) HIWORD(lParam)
#define HWND_TOPMOST              ((HWND)-1)
#define HWND_NOTOPMOST 	          ((HWND)-2)
#define HCBT_CREATEWND            (3)
#define CC_SHOWHELP               0
#define PS_DOT                    2
#define PD_ALLPAGES               0
#define PD_USEDEVMODECOPIES       0
#define PD_NOSELECTION            0
#define PD_HIDEPRINTTOFILE        0
#define PD_NOPAGENUMS             0
#define CF_METAFILEPICT           3
#define CWP_ALL                   0x0000
#define CWP_SKIPINVISIBLE         0x0001
#define CWP_SKIPDISABLED          0x0002
#define CWP_SKIPTRANSPARENT       0x0004
#define MM_LOMETRIC               2
#define MM_HIMETRIC               3
#define MM_LOENGLISH              4
#define MM_HIENGLISH              5
#define MM_TWIPS                  6
#define MM_ISOTROPIC              7
#define MM_ANISOTROPIC            8
#define OLEUI_FALSE               0
#define OLEUI_SUCCESS             1     
#define OLEUI_OK                  1    
#define OLEUI_CANCEL              2     
#define KF_EXTENDED               0x0100
#define KF_DLGMODE                0x0800
#define KF_MENUMODE               0x1000
#define KF_ALTDOWN                0x2000
#define KF_REPEAT                 0x4000
#define KF_UP                     0x8000
#define IDB_STD_SMALL_MONO        2     
#define IDB_STD_LARGE_MONO        3     
#define IDB_VIEW_SMALL_MONO       6    
#define IDB_VIEW_LARGE_MONO       7  
#define SPI_GETWORKAREA           48
#define LBSELCHSTRING             TEXT("commdlg_LBSelChangedNotify")
#define SHAREVISTRING             TEXT("commdlg_ShareViolation")
#define FILEOKSTRING              TEXT("commdlg_FileNameOK")
#define COLOROKSTRING             TEXT("commdlg_ColorOK")
#define SETRGBSTRING              TEXT("commdlg_SetRGBColor")
#define HELPMSGSTRING             TEXT("commdlg_help")
#define FINDMSGSTRING             TEXT("commdlg_FindReplace")
#define DRAGLISTMSGSTRING         TEXT("commctrl_DragListMsg")

#define OFN_ENABLESIZING 0

#ifndef WM_SETCURSOR
	#define WM_SETCURSOR 0x0020
	#define IDC_ARROW           MAKEINTRESOURCE(32512)
	#define IDC_IBEAM           MAKEINTRESOURCE(32513)
	#define IDC_WAIT            MAKEINTRESOURCE(32514)
	#define IDC_CROSS           MAKEINTRESOURCE(32515)
	#define IDC_UPARROW         MAKEINTRESOURCE(32516)
	#define IDC_SIZE            MAKEINTRESOURCE(32646)
	#define IDC_ICON            MAKEINTRESOURCE(32512)
	#define IDC_SIZENWSE        MAKEINTRESOURCE(32642)
	#define IDC_SIZENESW        MAKEINTRESOURCE(32643)
	#define IDC_SIZEWE          MAKEINTRESOURCE(32644)
	#define IDC_SIZENS          MAKEINTRESOURCE(32645)
	#define IDC_SIZEALL         MAKEINTRESOURCE(32646)
	#define IDC_NO              MAKEINTRESOURCE(32648)
	#define IDC_APPSTARTING     MAKEINTRESOURCE(32650)
	#define IDC_HELP            MAKEINTRESOURCE(32651)
	#define IDC_HAND			MAKEINTRESOURCE(32649)
#endif 

#if defined(_MIPS_)
extern "C" void _asm(char *, ...);
#endif

#define GMEM_MOVEABLE             LMEM_MOVEABLE
#define GMEM_FIXED                LMEM_FIXED
#define GMEM_ZEROINIT             LMEM_ZEROINIT
#define GMEM_INVALID_HANDLE       LMEM_INVALID_HANDLE
#define GMEM_LOCKCOUNT            LMEM_LOCKCOUNT
#define GPTR                      LPTR
#if (_WIN32_WCE < 300)
#define GMEM_SHARE                0
#endif // _WIN32_WCE

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE == 201)
	#define OFN_READONLY         0 
	#define OFN_ENABLEHOOK       0
	#define OFN_ENABLETEMPLATE   0
	#define OFN_ALLOWMULTISELECT 0
#endif // _WIN32_WCE_PSPC

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	#ifndef SHA_SHOWDONE
	#define SHA_SHOWDONE 0x0001
	#endif
	#ifndef SHA_HIDEDONE
	#define SHA_HIDEDONE 0x0002
	#endif
	#ifndef SHA_AUTODONE
	#define SHA_AUTODONE 0x0003
	#endif
	#ifndef MENU_HEIGHT
	#define MENU_HEIGHT  26
	#endif
	BOOL AFXAPI wce_IsSipUp();
#endif // _WIN32_WCE_PSPC

// WCEIMPL.INL, WCEIMPL.CPP: inline functions
HINSTANCE     AFXAPI wce_GetModuleHandleW(LPCWSTR lpModuleName);
HICON         AFXAPI wce_ExtractIcon(HINSTANCE hInst, LPCWSTR lpszExeFileName, UINT nIconIndex);
int           AFXAPI wce_MulDiv(int nNumber, int nNumerator, int nDenominator);
HINSTANCE     AFXAPI wce_GetModuleHandleA(LPCSTR lpModuleName);
short         AFXAPI wce_GetFileTitle(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf);
HGLOBAL       AFXAPI wce_GlobalAlloc(UINT uFlags, DWORD dwBytes);
HGLOBAL       AFXAPI wce_GlobalFree(HGLOBAL hMem);
HGLOBAL       AFXAPI wce_GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, UINT uFlags);
DWORD         AFXAPI wce_GlobalSize(HGLOBAL hMem);
LPVOID        AFXAPI wce_GlobalLock(HGLOBAL hMem);
BOOL          AFXAPI wce_GlobalUnlock(HGLOBAL hMem);
HGLOBAL       AFXAPI wce_GlobalHandle(LPCVOID pMem);
UINT          AFXAPI wce_GlobalFlags(HGLOBAL hMem);
DWORD         AFXAPI wce_GetVersion();
void*         AFXAPI wce_calloc(size_t num, size_t size);
void*	      AFXAPI wce__expand(void* pvMemBlock, size_t iSize);
void          AFXAPI wce_abort();
unsigned long AFXAPI wce__beginthreadex(void *security, unsigned stack_size, 
                                        unsigned (__stdcall *start_address)(void *),
                                        void *arglist, unsigned initflag, 
                                        unsigned *thrdaddr);
void          AFXAPI wce__endthreadex(unsigned nExitCode);
BOOL          AFXAPI wce_GetCursorPos(LPPOINT lpPoint);
HDWP          AFXAPI wce_BeginDeferWindowPos(int nNumWindows);
HDWP          AFXAPI wce_DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, 
                                        int x, int y, int cx, int cy, UINT uFlags);
BOOL          AFXAPI wce_EndDeferWindowPos(HDWP hWinPosInfo);

#ifdef _AFX_ENABLE_INLINES
#define _AFXWIN_INLINE AFX_INLINE
#include "wceimpl.inl"
#endif

#endif // __WCEIMPL_H__
