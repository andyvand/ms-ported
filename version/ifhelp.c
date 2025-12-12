#include <windows.h>
#include "verpriv.h"
#include "wchar.h"

#if defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#undef WX86
#endif

/* Determine if a file is in use by Windows
 */
BOOL FileInUse(LPWSTR lpszFilePath, LPWSTR lpszFileName)
{
    HANDLE hFile;
    BOOL bResult = FALSE;

    //
    // Attempt to open the file exclusively.
    //

    hFile = CreateFile(lpszFilePath,
                       GENERIC_WRITE | GENERIC_READ,
                       0,
                       NULL,
                       OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL,
                       NULL);

    if (hFile == INVALID_HANDLE_VALUE) {

        //
        // If the last error is access denied,
        // then the file is in use by someone
        // else.  Return TRUE in this case.
        //

        if (GetLastError() == ERROR_SHARING_VIOLATION)
            bResult = TRUE;

    } else {

        //
        // CreateFile successfully opened the file.
        // Close the handle and return FALSE.
        //

        CloseHandle(hFile);
    }

    return bResult;
}


/* Take a Dir and Filename and make a full path from them
 */
DWORD MakeFileName(LPWSTR lpDst, LPWSTR lpDir, LPWSTR lpFile, int cchDst)
{
  DWORD wDirLen;
  WCHAR cTemp;

#if __STDC_WANT_SECURE_LIB__
  wcscpy_s(lpDst, cchDst, lpDir);
#else
  wcsncpy(lpDst, lpDir, cchDst);
#endif

  lpDst[cchDst-1] = TEXT('\0');
  wDirLen=(DWORD)wcslen(lpDst);

  if ( wDirLen && (cTemp=*(lpDst+wDirLen-1))!=TEXT('\\') && cTemp!=TEXT(':'))
      lpDst[wDirLen++] = TEXT('\\');

#if __STDC_WANT_SECURE_LIB__
  wcscpy_s(lpDst + wDirLen, cchDst - wDirLen, lpFile);
#else
  wcsncpy(lpDst+wDirLen, lpFile, cchDst - wDirLen);
#endif

  lpDst[cchDst-1] = TEXT('\0');

  return(wDirLen);
}


/* Given a filename and a list of directories, find the first directory
 * that contains the file, and copy it into the buffer.  Note that in the
 * library version, you can give an environment style path, but not in the
 * DLL version.
 */
INT
GetDirOfFile(LPWSTR lpszFileName,
    LPWSTR lpszPathName,
    DWORD wSize,
    LPWSTR *lplpszDirs)
{
  WCHAR szFileName[_MAX_PATH+1];
  HANDLE hfRes;
  INT nFileLen = 0;
  INT nPathLen = 0;
  BOOL bDoDefaultOpen = TRUE;
  LPWSTR *lplpFirstDir;
  LPWSTR lpszDir;

  nFileLen = (INT)wcslen(lpszFileName);

  for (lplpFirstDir=lplpszDirs; *lplpFirstDir && bDoDefaultOpen;
        ++lplpFirstDir)
    {
      lpszDir = *lplpFirstDir;

      if (nFileLen+wcslen(lpszDir) >= _MAX_PATH-1)
          continue;
      MakeFileName(szFileName, lpszDir, lpszFileName, ARRAYSIZE(szFileName));

TryOpen:
    nPathLen = 0;  // Re-init for this path.

    if ((hfRes = CreateFile(szFileName, GENERIC_READ,
            FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_FLAG_SEQUENTIAL_SCAN, NULL)) != (HANDLE)-1)
        {
          CloseHandle(hfRes);
          for (lpszDir=szFileName; *lpszDir; lpszDir++)
              if (*lpszDir == TEXT('\\'))
                  nPathLen = (INT)(lpszDir - (LPWSTR)szFileName);

          /* This gets rid of the '\' if this is not the root of a drive
           */
          if (nPathLen <= 3)
              ++nPathLen;

          /* Account for the terminating NULL, and make sure wSize is in bounds
           * then NULL terminate the string in the appropriate place so that
           * we can just do an wcscpy.
           */
          --wSize;
          szFileName[(int)wSize<nPathLen ? wSize : nPathLen] = 0;

#if __STDC_WANT_SECURE_LIB__
          wcscpy_s(lpszPathName, wSize, szFileName);
#else
          wcsncpy(lpszPathName, szFileName, wSize);
#endif

          return(nPathLen);
        }
    }

  if (bDoDefaultOpen)
    {
      bDoDefaultOpen = FALSE;
#if __STDC_WANT_SECURE_LIB__
      wcscpy_s(szFileName, sizeof(szFileName), lpszFileName);
#else
      wcsncpy(szFileName, lpszFileName, sizeof(szFileName));
#endif

      goto TryOpen;
    }

  return(0);
}


#define GetWindowsDir(x,y,z) GetWindowsDirectory(y,z)
#define GetSystemDir(x,y,z) GetSystemDirectory(y,z)

#ifdef __MINGW32__
DWORD
APIENTRY
VerFindFileW(
        DWORD wFlags,
        LPWSTR lpszFileName,
        LPWSTR lpszWinDir,
        LPWSTR lpszAppDir,
        LPWSTR lpszCurDir,
        PUINT puCurDirLen,
        LPWSTR lpszDestDir,
        PUINT puDestDirLen
        )
#else
DWORD
APIENTRY
VerFindFileW(
        DWORD wFlags,
        LPCWSTR lpszFileName,
        LPCWSTR lpszWinDir,
        LPCWSTR lpszAppDir,
        LPWSTR lpszCurDir,
        PUINT puCurDirLen,
        LPWSTR lpszDestDir,
        PUINT puDestDirLen
        )
#endif
{
  static WORD wSharedDirLen = 0;
  static WCHAR gszSharedDir[_MAX_PATH+1];

  WCHAR szSysDir[_MAX_PATH+1], cTemp;
  WCHAR szWinDir[_MAX_PATH+1];
  WCHAR szCurDir[_MAX_PATH+1];
  LPWSTR lpszDir, lpszDirs[4];
  WORD wDestLen, wWinLen, wRetVal = 0, wTemp;
  int nRet;

#ifdef WX86
  //  Save a copy of the 'from Wx86' flag and clear it.
  BOOLEAN UseKnownWx86Dll = NtCurrentTeb()->Wx86Thread.UseKnownWx86Dll;
  NtCurrentTeb()->Wx86Thread.UseKnownWx86Dll = FALSE;
#endif

  /* We want to really look in the Windows directory; we don't trust the app
   */
  GetWindowsDir(lpszWinDir ? lpszWinDir : "", szWinDir, _MAX_PATH);
  lpszWinDir = szWinDir;

#ifdef WX86
  NtCurrentTeb()->Wx86Thread.UseKnownWx86Dll = UseKnownWx86Dll;
#endif

  if (!GetSystemDir(lpszWinDir, szSysDir, _MAX_PATH))
#if __STDC_WANT_SECURE_LIB__
      wcscpy_s(szSysDir, sizeof(szSysDir), lpszWinDir);
#else
      wcscpy(szSysDir, lpszWinDir);
#endif

#ifdef WX86
  NtCurrentTeb()->Wx86Thread.UseKnownWx86Dll = FALSE;
#endif

  if (wFlags & VFFF_ISSHAREDFILE) {
     lpszDirs[0] = (LPWSTR)lpszWinDir;
     lpszDirs[1] = (LPWSTR)szSysDir;
     lpszDirs[2] = (LPWSTR)lpszAppDir;
  } else {
     lpszDirs[0] = (LPWSTR)lpszAppDir;
     lpszDirs[1] = (LPWSTR)lpszWinDir;
     lpszDirs[2] = (LPWSTR)szSysDir;
  }

  lpszDirs[3] = NULL;

  if (!(wTemp=(WORD)GetDirOfFile((LPWSTR)lpszFileName, (LPWSTR)szCurDir, _MAX_PATH, (LPWSTR *)lpszDirs)))
      *szCurDir = 0;
  if (*puCurDirLen > wTemp)
#if __STDC_WANT_SECURE_LIB__
      wcscpy_s(lpszCurDir, sizeof(szCurDir), szCurDir);
#else
      wcsncpy(lpszCurDir, szCurDir, sizeof(szCurDir));
#endif
  else
      wRetVal |= VFF_BUFFTOOSMALL;
  *puCurDirLen = wTemp + 1;

  if (lpszDestDir)
    {
      if (wFlags & VFFF_ISSHAREDFILE)
        {
          if (!wSharedDirLen)
            {
              if ((wWinLen = (WORD)wcslen(lpszWinDir)) &&
                    (*(lpszWinDir-1) == L'\\'))
                {
                  if (szSysDir[wWinLen-1] == TEXT('\\'))
                      goto doCompare;
                }
              else if (szSysDir[wWinLen] == TEXT('\\'))
                {
doCompare:
                  cTemp = szSysDir[wWinLen];
                  szSysDir[wWinLen] = 0;
                  nRet = _wcsicmp(lpszWinDir, szSysDir);
                  szSysDir[wWinLen] = cTemp;
                  if(nRet)
                      goto doCopyWinDir;
#if __STDC_WANT_SECURE_LIB__
                  wcscpy_s(gszSharedDir, sizeof(gszSharedDir), szSysDir);
#else
                  wcsncpy(gszSharedDir, szSysDir, sizeof(gszSharedDir));
#endif
                }
              else
                {
doCopyWinDir:
#if __STDC_WANT_SECURE_LIB__
                  wcscpy_s(gszSharedDir, sizeof(gszSharedDir), lpszWinDir);
#else
                  wcsncpy(gszSharedDir, lpszWinDir, sizeof(gszSharedDir));
#endif
                }
              wSharedDirLen = (WORD)wcslen(gszSharedDir);
            }

          wDestLen = wSharedDirLen;
          lpszDir = gszSharedDir;
        }
      else
        {
          wDestLen = (WORD)wcslen(lpszAppDir);
          lpszDir = (LPWSTR)lpszAppDir;
        }

      if (*puDestDirLen > wDestLen)
        {
#if __STDC_WANT_SECURE_LIB__
          wcscpy_s(lpszDestDir, _MAX_PATH-1, lpszDir);
#else
          wcsncpy(lpszDestDir, lpszDir, _MAX_PATH-1);
#endif

          if ((wWinLen = (WORD)wcslen(lpszDestDir)) &&
                *(lpszDestDir-1)==TEXT('\\'))
              lpszDestDir[wWinLen-1] = 0;

          if (_wcsicmp(lpszCurDir, lpszDestDir))
              wRetVal |= VFF_CURNEDEST;
        }
      else
          wRetVal |= VFF_BUFFTOOSMALL;
      *puDestDirLen = wDestLen + 1;
    }

  if (*szCurDir)
    {
      MakeFileName(szSysDir, szCurDir, (LPWSTR)lpszFileName, ARRAYSIZE(szSysDir));
      if (FileInUse(szSysDir, (LPWSTR)lpszFileName))
          wRetVal |= VFF_FILEINUSE;
    }

  return(wRetVal);
}
