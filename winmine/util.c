/**********/
/* util.c */
/**********/

#define  _WINDOWS
#include <windows.h>

#ifndef _WIN32_WCE
#include <htmlhelp.h>   // for HtmlHelp()
#endif

#include "shellapi.h"   // for ShellAbout
#include <port1632.h>
#include <strsafe.h>

#include "main.h"
#include "res.h"
#include "pref.h"
#include "util.h"
#include "sound.h"
#include "rtns.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#ifndef _WIN32_WCE
#include "dos.h"
#else
#include <time.h>
#endif

extern INT dypBorder;
extern INT dxpBorder;
extern INT dypCaption;
extern INT dypMenu;

extern TCHAR szClass[cchNameMax];
extern TCHAR szTime[cchNameMax];
extern TCHAR szDefaultName[cchNameMax];

extern HANDLE hInst;
extern HWND   hwndMain;
extern HMENU  hMenu;

extern PREF Preferences;

extern  HKEY g_hReg;
extern  TCHAR * rgszPref[iszPrefMax];
TCHAR   szIniFile[] = TEXT("entpack.ini");


/****** R N D ******/

/* Return a random number between 0 and rndMax */

INT Rnd(INT rndMax)
{
        return (rand() % rndMax);
}



/****** R E P O R T  E R R ******/

/* Report and error and exit */

VOID ReportErr(WORD idErr)
{
        TCHAR szMsg[cchMsgMax];
        TCHAR szMsgTitle[cchMsgMax];

        if (idErr < ID_ERR_MAX)
                LoadString(hInst, idErr, szMsg, cchMsgMax);
        else
                {
                LoadString(hInst, ID_ERR_UNKNOWN, szMsgTitle, cchMsgMax);
				StringCchPrintf(szMsg, sizeof(szMsg), TEXT("Error: %d"), idErr); 
                }

        LoadString(hInst, ID_ERR_TITLE, szMsgTitle, cchMsgMax);

        MessageBox(NULL, szMsg, szMsgTitle, MB_OK | MB_ICONHAND);
}


/****** L O A D  S Z ******/

VOID LoadSz(WORD id, TCHAR * sz, DWORD cch)
{
        if (LoadString(hInst, id, sz, cch) == 0)
                ReportErr(1001);
}


// Routines to read the ini file.

#ifndef _WIN32_WCE
INT ReadIniInt(INT iszPref, INT valDefault, INT valMin, INT valMax)
{
	return max(valMin, min(valMax,
		(INT) GetPrivateProfileInt(szClass, rgszPref[iszPref], valDefault, szIniFile) ) );
}

#define ReadIniBool(iszPref, valDefault) ReadIniInt(iszPref, valDefault, 0, 1)


VOID ReadIniSz(INT iszPref, TCHAR FAR * szRet)
{

	GetPrivateProfileString(szClass, rgszPref[iszPref], szDefaultName, szRet, cchNameMax, szIniFile);
}
#endif

/****** I N I T  C O N S T ******/

#ifdef _WIN32_WCE
DWORD GetCurrentTime()
{
  SYSTEMTIME systemTime = { 0 };
  FILETIME fileTime = { 0 };
  DWORD result = 0;
  GetSystemTime(&systemTime);
  if (SystemTimeToFileTime(&systemTime, &fileTime))
  {
    ULONGLONG temp = 0;
    memcpy(&temp, &fileTime, sizeof(FILETIME));
    temp -= 116444736000000000; // subtract 1970-01-01 00:00 (UTC)
    temp /= 10000000; // convert to seconds
    result = (DWORD)temp;
  }
  return result;
}
#endif

VOID InitConst(VOID)
{
INT     iAlreadyPlayed = 0;     // have we already updated the registry ?
DWORD   dwDisposition;
       
#ifdef _WIN32_WCE
        srand(GetCurrentTime());
#else
        srand(LOWORD(GetCurrentTime()));
#endif

        LoadSz(ID_GAMENAME, szClass, ARRAYSIZE(szClass));
        LoadSz(ID_MSG_SEC,  szTime, ARRAYSIZE(szTime));
        LoadSz(ID_NAME_DEFAULT, szDefaultName, ARRAYSIZE(szDefaultName));

        dypCaption = GetSystemMetrics(SM_CYCAPTION) + 1;
        dypMenu    = GetSystemMetrics(SM_CYMENU)    + 1;
        dypBorder  = GetSystemMetrics(SM_CYBORDER)  + 1;
        dxpBorder  = GetSystemMetrics(SM_CXBORDER)  + 1;

        // Open the registry key;
        if (RegCreateKeyEx(HKEY_CURRENT_USER, SZWINMINEREG, 0, NULL, 0, KEY_READ, NULL, 
                   &g_hReg, &dwDisposition) == ERROR_SUCCESS)
        {
            iAlreadyPlayed = ReadInt(iszPrefAlreadyPlayed, 0, 0, 1);
            RegCloseKey(g_hReg);
        }


        // Read it from the .ini file and write it to registry.
#ifndef _WIN32_WCE
        if (!iAlreadyPlayed)
        {
            Preferences.Height= ReadIniInt(iszPrefHeight,MINHEIGHT,DEFHEIGHT,25);
            Preferences.Width= ReadIniInt(iszPrefWidth,MINWIDTH,DEFWIDTH,30);

            Preferences.wGameType = (WORD)ReadIniInt(iszPrefGame,wGameBegin, wGameBegin, wGameExpert+1);
            Preferences.Mines    = ReadIniInt(iszPrefMines, 10, 10, 999);
            Preferences.xWindow  = ReadIniInt(iszPrefxWindow, 80, 0, 1024);
            Preferences.yWindow  = ReadIniInt(iszPrefyWindow, 80, 0, 1024);

            Preferences.fSound = ReadIniInt(iszPrefSound, 0, 0, fsoundOn);
            Preferences.fMark  = ReadIniBool(iszPrefMark,  fTrue);
            Preferences.fTick  = ReadIniBool(iszPrefTick,  fFalse);
            Preferences.fMenu  = ReadIniInt(iszPrefMenu,  fmenuAlwaysOn, fmenuAlwaysOn, fmenuOn);
	
            Preferences.rgTime[wGameBegin]  = ReadIniInt(iszPrefBeginTime, 999, 0, 999);
            Preferences.rgTime[wGameInter]  = ReadIniInt(iszPrefInterTime, 999, 0, 999);
            Preferences.rgTime[wGameExpert] = ReadIniInt(iszPrefExpertTime, 999, 0, 999);

            ReadIniSz(iszPrefBeginName, Preferences.szBegin);
            ReadIniSz(iszPrefInterName, Preferences.szInter);
            ReadIniSz(iszPrefExpertName, Preferences.szExpert);

            // set the color preference so we will use the right bitmaps
            // numcolors may return -1 on true color devices
            {
            HDC hDC = GetDC(GetDesktopWindow());
            Preferences.fColor  = ReadIniBool(iszPrefColor, (GetDeviceCaps(hDC, NUMCOLORS) != 2));
            ReleaseDC(GetDesktopWindow(),hDC);
            }

            if (FSoundOn())
                Preferences.fSound = FInitTunes();
            
            // Write it to registry.
            WritePreferences();
        }
#else
        if (!iAlreadyPlayed)
        {
            Preferences.Height= DEFHEIGHT;
            Preferences.Width= DEFWIDTH;

            Preferences.wGameType = (WORD)wGameBegin;
            Preferences.Mines    = 10;
            Preferences.xWindow  = 80;
            Preferences.yWindow  = 80;

            Preferences.fSound = 0;
            Preferences.fMark  = fTrue;
            Preferences.fTick  = fFalse;
            Preferences.fMenu  = fmenuAlwaysOn;
	
            Preferences.rgTime[wGameBegin]  = 999;
            Preferences.rgTime[wGameInter]  = 999;
            Preferences.rgTime[wGameExpert] = 999;

            // set the color preference so we will use the right bitmaps
            // numcolors may return -1 on true color devices
            {
            HDC hDC = GetDC(GetDesktopWindow());
            Preferences.fColor  = (BOOL)(GetDeviceCaps(hDC, NUMCOLORS) != 2);
            ReleaseDC(GetDesktopWindow(),hDC);
            }

            if (FSoundOn())
                Preferences.fSound = FInitTunes();
            
            // Write it to registry.
            WritePreferences();
        }
#endif
}



/* * * * * *  M E N U S  * * * * * */

/****** C H E C K  E M ******/

VOID CheckEm(WORD idm, BOOL fCheck)
{
        CheckMenuItem(hMenu, idm, fCheck ? MF_CHECKED : MF_UNCHECKED);
}

/****** S E T  M E N U  B A R ******/

VOID SetMenuBar(INT fActive)
{
        Preferences.fMenu = fActive;
        FixMenus();
#ifndef _WIN32_WCE 
		SetMenu(hwndMain, FMenuOn() ? hMenu : NULL);
#endif
        AdjustWindow(fResize);
}


/****** D O  A B O U T ******/
#ifndef _WIN32_WCE
VOID DoAbout(VOID)
{
        TCHAR szVersion[cchMsgMax];
        TCHAR szCredit[cchMsgMax];

        LoadSz(ID_MSG_VERSION, szVersion, ARRAYSIZE(szVersion));
        LoadSz(ID_MSG_CREDIT,  szCredit, ARRAYSIZE(szCredit));

        ShellAbout(hwndMain,
          szVersion, szCredit, LoadIcon(hInst, MAKEINTRESOURCE(ID_ICON_MAIN)));
}
#endif

/****** G E T  D L G  I N T ******/

INT GetDlgInt(HWND hDlg, INT dlgID, INT numLo, INT numHi)
{
        INT num;
        BOOL fFlag;

        num = GetDlgItemInt(hDlg, dlgID, &fFlag, fFalse);

        if (num < numLo)
                num = numLo;
        else if (num > numHi)
                num = numHi;

        return num;
}

