/***************************************************************************/
/**                  Microsoft Windows                                    **/
/**            Copyright(c) Microsoft Corp., 1991, 1992                   **/
/***************************************************************************/

/****************************************************************************

hearts.h

Aug 92, JimH
May 93, JimH    chico port

declaration of theApp class

****************************************************************************/


#ifndef	HEARTS_INC
#define	HEARTS_INC

#ifndef STRICT
#define STRICT
#endif

#ifdef __MINGW32__
#include <afx.h>
#endif

//#include <windows.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxcmn.h>			// MFC support for Windows Common Controls

#ifdef _WIN32_WCE
#include <afxdisp.h>
#endif

#include <shellapi.h>
//#include <shell.h>
#include <afxwin.h>

#ifndef _WIN32_WCE
#include <htmlhelp.h>
#endif

#include <strsafe.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
#include <stringapiset.h>
#endif

class CTheApp : public CWinApp
{
    public:
        BOOL InitInstance();
};

#endif


#if defined (WINDOWS_ME) && ! defined (USE_MIRRORING)
extern DWORD meMsgBox;
extern DWORD meSystem;
//#define MessageBox(h,s,c,f) MessageBoxEx(h,s,c,f|meMsgBox,0)
#endif
