#ifndef __UTIL_H__
#define __UTIL_H__

/**********/
/* util.h */
/**********/
#ifndef ARRAYSIZE
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

VOID InitConst(VOID);
VOID LoadSz(WORD, TCHAR *, DWORD);
VOID ReportErr(WORD);
INT  Rnd(INT);

INT  GetDlgInt(HWND, INT, INT, INT);

VOID DoHelp(INT);
VOID DoAbout(VOID);

VOID CheckEm(WORD, BOOL);
VOID SetMenuBar(INT);

#endif /* __UTIL_H__ */
