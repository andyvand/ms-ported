#include "sol.h"

#ifndef _WIN32_WCE
#include <io.h>
#else
#include <stdio.h>
#endif

#include <string.h>
#include "assert.h"

VSZASSERT

#ifdef DEBUG

MDBG rgmdbg[imdbgMax];
WORD lvl = 0;
INT imdbgCur = 0;


VOID InitDebug()
{
    INT imdbg;
    MDBG *pmdbg;

    for(imdbg = 0; imdbg < imdbgMax; imdbg++)
    {
        pmdbg = &rgmdbg[imdbg];
        pmdbg->pgmcol = NULL;
        pmdbg->lvl = 0;
        pmdbg->msg = 0xcccc;
        pmdbg->wp1 = 0xcccc;
        pmdbg->wp2 = 0xcccc;
        pmdbg->wResult = 0xcccc;
    }

    imdbgCur = 0;
    lvl = 0;
}



WORD ILogMsg(VOID *pgmcol, INT msg, WPARAM wp1, LPARAM wp2, BOOL fGm)
{
    MDBG *pmdbg;
    WORD imdbgRet;

    Assert(FInRange(imdbgCur, 0, imdbgMax-1));
    pmdbg = &rgmdbg[imdbgCur];

    Assert(fGm == 0 || fGm == 1);
    pmdbg->pgmcol = pgmcol;
    pmdbg->msg = msg + (fGm << 15);
    pmdbg->wp1 = wp1;
    pmdbg->wp2 = wp2;
    pmdbg->wResult = 0xcccc;
    pmdbg->lvl = lvl;
    lvl++;
    imdbgRet = imdbgCur++;
    imdbgCur %= imdbgMax;
    Assert(FInRange(imdbgCur, 0, imdbgMax-1));
    return imdbgRet;
}



VOID LogMsgResult(INT imdbg, LRESULT wResult)
{
    lvl--;

    Assert(FInRange(imdbg, 0, imdbgMax-1));
    rgmdbg[imdbg].wResult = wResult;
}

#ifdef _WIN32_WCE
VOID WriteCrlf(FILE *fh)
{
	fwrite(TEXT("\x0d\n"), 1, sizeof(TCHAR) * 2, fh);
}
#else
VOID WriteCrlf(INT fh)
{
    _write(fh, TEXT("\x0d\n"), sizeof(TCHAR) * 2);
}
#endif

#ifdef _WIN32_WCE
VOID WriteSz(FILE *fh, TCHAR *sz)
#else
VOID WriteSz(INT fh, TCHAR *sz)
#endif
{
#ifdef _WIN32_WCE
	fwrite(sz, 1, lstrlen(sz), fh);
#else
	_write(fh, sz, lstrlen(sz));
#endif
    WriteCrlf(fh);
}

#ifdef _WIN32_WCE
VOID WriteIField(FILE *fh, TCHAR *szField, INT_PTR ifld)
#else
VOID WriteIField(INT fh, TCHAR *szField, INT_PTR ifld)
#endif
{
    TCHAR szBuf[128];
    INT cch;

#ifdef _WIN32_WCE
	fwrite(szField, 1, szField, fh);
	fwrite(TEXT(" = "), 1, sizeof(TCHAR) * 3, fh);
	cch = CchDecodeInt(szBuf, ifld);
	fwrite(szBuf, 1, cch, fh);
#else
	_write(fh, szField, lstrlen(szField));
    _write(fh, TEXT(" = "), sizeof(TCHAR) *3);
    cch = CchDecodeInt(szBuf, ifld);
    _write(fh, szBuf, cch);
#endif

    WriteCrlf(fh);
}



#ifdef _WIN32_WCE
VOID DumpCol(FILE *fh, COL *pcol)
#else
VOID DumpCol(INT fh, COL *pcol)
#endif
{
    INT icrd;
    MOVE *pmove;

    WriteCrlf(fh);
    WriteIField(fh, TEXT(">>> *pcol"), (INT_PTR) pcol);
    WriteIField(fh, TEXT("pcolcls->tcls"), pcol->pcolcls->tcls);
    WriteIField(fh, TEXT("icrdMac"), pcol->icrdMac);
    WriteIField(fh, TEXT("pmove"), (INT_PTR) pcol->pmove);
    if(pcol->pmove != NULL)
    {
        pmove = pcol->pmove;
        WriteIField(fh, TEXT("pmove->icrdSel"), pmove->icrdSel);
        WriteIField(fh, TEXT("pmove->ccrdSel"), pmove->ccrdSel);
        WriteIField(fh, TEXT("pmove->delHit.x"), pmove->delHit.dx);
        WriteIField(fh, TEXT("pmove->delHit.y"), pmove->delHit.dy);
        WriteIField(fh, TEXT("pmove->fHdc"), pmove->fHdc);
        WriteIField(fh, TEXT("pmove->dyCol"), pmove->dyCol);
        WriteIField(fh, TEXT("pmove->hdcScreen"), (INT_PTR)pmove->hdcScreen);

        WriteIField(fh, TEXT("pmove->hdcCol"), (INT_PTR)pmove->hdcCol);
        WriteIField(fh, TEXT("pmove->hbmColOld"), (INT_PTR)pmove->hbmColOld);

        WriteIField(fh, TEXT("pmove->hdcScreenSave"), (INT_PTR)pmove->hdcScreenSave);
        WriteIField(fh, TEXT("pmove->hbmScreenSaveOld"), (INT_PTR)pmove->hbmScreenSaveOld);

        WriteIField(fh, TEXT("pmove->hdcT"), (INT_PTR)pmove->hdcT);
        WriteIField(fh, TEXT("pmove->hbmT"), (INT_PTR)pmove->hbmT);
    }
    for(icrd = 0; icrd < pcol->icrdMac; icrd++)
    {
        WriteIField(fh, TEXT("\t-card "), icrd);
        WriteIField(fh, TEXT("\t    cd"), pcol->rgcrd[icrd].cd);
        WriteIField(fh, TEXT("\t   fUp"), pcol->rgcrd[icrd].fUp);
        WriteIField(fh, TEXT("\t  pt.x"), pcol->rgcrd[icrd].pt.x);
        WriteIField(fh, TEXT("\t  pt.y"), pcol->rgcrd[icrd].pt.y);
    }
}

#ifdef _WIN32_WCE
VOID DumpGm(FILE *fh, GM *pgm)
#else
VOID DumpGm(INT fh, GM *pgm)
#endif
{
    INT icol;

    WriteIField(fh, TEXT("pgm"), (INT_PTR) pgm);
    WriteIField(fh, TEXT("udr.fAvail"), pgm->udr.fAvail);
    WriteIField(fh, TEXT("udr.sco"), pgm->udr.sco);
    WriteIField(fh, TEXT("udr.icol1"), pgm->udr.icol1);
    WriteIField(fh, TEXT("udr.icol2"), pgm->udr.icol2);
    WriteIField(fh, TEXT("udr.rgpcol[1]"), (INT_PTR) pgm->udr.rgpcol[1]);
    WriteIField(fh, TEXT("udr.rgpcol[2]"), (INT_PTR) pgm->udr.rgpcol[2]);
    WriteIField(fh, TEXT("fDealt"), pgm->fDealt);
    WriteIField(fh, TEXT("fWon"), pgm->fWon);
    WriteIField(fh, TEXT("fInput"), pgm->fInput);
    WriteIField(fh, TEXT("sco"), pgm->sco);
    WriteIField(fh, TEXT("iqsecScore"), pgm->iqsecScore);
    WriteIField(fh, TEXT("dqsecScore"), pgm->dqsecScore);
    WriteIField(fh, TEXT("ccrdDeal"), pgm->ccrdDeal);
    WriteIField(fh, TEXT("irep"), pgm->irep);
    WriteIField(fh, TEXT("ptMousePrev->x"), pgm->ptMousePrev.x);
    WriteIField(fh, TEXT("ptMousePrev->y"), pgm->ptMousePrev.y);
    WriteIField(fh, TEXT("fButtonDown"), pgm->fButtonDown);
    WriteIField(fh, TEXT("icolKbd"), pgm->icolKbd);
    WriteIField(fh, TEXT("icrdKbd"), pgm->icrdKbd);
    WriteIField(fh, TEXT("icolSel"), pgm->icolSel);
    WriteIField(fh, TEXT("icolHilight"), pgm->icolHilight);
    WriteIField(fh, TEXT("icolMac"), pgm->icolMac);
    WriteIField(fh, TEXT("icolMax"), pgm->icolMax);
    for(icol = 0; icol < pgm->icolMac; icol++)
        DumpCol(fh, pgm->rgpcol[icol]);
}

TCHAR *PchDecodeWp(TCHAR *pch, INT_PTR wp)
{
    INT icol;

    if(pgmCur == NULL)
            return pch;
    if((GM *) wp == pgmCur)
            return PszCopy(TEXT("(pgmCur)"), pch);

    for(icol = 0; icol < pgmCur->icolMac; icol++)
    {
        if((struct _col *) wp == pgmCur->rgpcol[icol])
        {
            pch = PszCopy(TEXT("(col "), pch);
            pch += CchDecodeInt(pch, icol);
            *pch++ = TEXT(')');
            break;
        }
    }
    return pch;
}


VOID DumpRgmdbg(TCHAR *szFile, INT li)
{
    OFSTRUCT of;
#ifdef _WIN32_WCE
	FILE *fh = NULL;
#else
    INT fh;
#endif
    INT imdbg;
    USHORT lvl;
    MDBG *pmdbg;
    TCHAR szBuf[128];
    TCHAR *pch;


#if defined(_WIN32_WCE)
	if ((fh = fopen("sol.dbg", "wb")) == NULL)
#else
    if((fh = (OpenFile("sol.dbg", &of, OF_CREATE|OF_WRITE))) == -1)
#endif
            return;

    WriteSz(fh, TEXT("Assertion Failure"));
    WriteIField(fh, szFile, li);
    WriteSz(fh, szVer);
    WriteIField(fh, TEXT("Game #"), igmCur);
    WriteCrlf(fh);



    /* write game and col structs */
    if(pgmCur != NULL)
            DumpGm(fh, pgmCur);

    imdbg = imdbgCur;
    do
    {
        Assert(FInRange(imdbgCur, 0, imdbgMax-1));
        pmdbg = &rgmdbg[imdbg];
        Assert(pmdbg->lvl < 60);
		for (lvl = 0; lvl < pmdbg->lvl; lvl++)
#ifdef _WIN32_WCE
			fwrite(TEXT("\t"), 1, 1, fh);
#else
            _write(fh, TEXT("\t"), 1);
#endif
        pch = PchDecodeWp(szBuf, (INT_PTR) pmdbg->pgmcol);
        pmdbg->msg &= 0x7fff;
        *pch++ = TEXT(' ');
        pch += CchDecodeInt(pch, pmdbg->msg);
        *pch++ = TEXT(',');
        *pch++ = TEXT(' ');
        pch += CchDecodeInt(pch, pmdbg->wp1);
        pch = PchDecodeWp(pch, pmdbg->wp1);
        *pch++ = TEXT(',');
        *pch++ = TEXT(' ');
        pch += CchDecodeInt(pch, pmdbg->wp2);
        pch = PchDecodeWp(pch, pmdbg->wp2);
        *pch++ = TEXT(' ');
        *pch++ = TEXT('-');
        *pch++ = TEXT('>');
        *pch++ = TEXT(' ');
        pch += CchDecodeInt(pch, pmdbg->wResult);
        _write(fh, szBuf, (int)(pch-szBuf));
        WriteCrlf(fh);

        imdbg--;
        if(imdbg < 0)
            imdbg = imdbgMax-1;
        Assert(FInRange(imdbg, 0, imdbgMax-1));
    } while(imdbg != imdbgCur);

#ifdef _WIN32_WCE
	fclose(fh);
#else
    _close(fh);
#endif
}





/* Puts msg on bottom of screen.  Useful because I don't have a debug console now */
VOID DisplayMsg(TCHAR *sz, INT msgc, INT wp1, INT wp2)
{

    INT y;
    INT x;
    HDC hdc;
    INT cch;
    TCHAR szInt[20];
    TEXTMETRIC tm;
	HRGN hrgn = { 0 };

    if(fScreenShots)
        return;

    x = 0;

	GetWindowRgn(hwndApp, hrgn);
    hdc = GetDC(hwndApp);
    GetTextMetrics(hdc, (LPTEXTMETRIC)&tm);
    y = 0;/*        dyScreen - tm.tmHeight; */

	ExtTextOut(hdc, x, y, 0, NULL, TEXT("                         "), 24, NULL);
    ExtTextOut(hdc, x, y, 0, NULL, sz, cch = lstrlen(sz), NULL);
    x += (cch+1) * tm.tmMaxCharWidth;

    cch = CchDecodeInt(szInt, msgc);
    ExtTextOut(hdc, x, y, 0, NULL, szInt, cch, NULL);
    x += (cch+1) * tm.tmAveCharWidth;

    cch = CchDecodeInt(szInt, wp1);
	ExtTextOut(hdc, x, y, 0, NULL, szInt, cch, NULL);
    x += (cch+1) * tm.tmAveCharWidth;

    cch = CchDecodeInt(szInt, wp2);
    ExtTextOut(hdc, x, y, 0, NULL, szInt, cch, NULL);
    x += (cch+1) * tm.tmAveCharWidth;

    ReleaseDC(hwndApp, hdc);
}

VOID PrintCardMacs(GM *pgm)
{
    INT icol;
    TCHAR sz[20];
    INT cch;
    HDC hdc;
	HRGN hrgn = { 0 };

	GetWindowRgn(hwndApp, hrgn);
    hdc = GetDC(hwndApp);
    for(icol = 0; icol < pgm->icolMac; icol++)
    {
        cch = CchDecodeInt(sz, pgm->rgpcol[icol]->icrdMac);
		ExtTextOut(hdc, 30 * icol, 10, 0, NULL, sz, cch, NULL);
    }
    ReleaseDC(hwndApp, hdc);
}



INT_PTR APIENTRY GameNo(HWND hdlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    BOOL fTranslated;
    INT igmNext;

    if (iMessage == WM_COMMAND)
    {
        if( GET_WM_COMMAND_ID( wParam, lParam ) == IDOK )
        {
            igmNext = GetDlgItemInt(hdlg, ideGameNoEdit, &fTranslated, fFalse);
            if(fTranslated && igmNext >= 0)
                igmCur = igmNext;
            else
            {
                Error(TEXT("Invalid game number"));
                return fFalse;
            }
        }
        if(wParam == IDOK || wParam == IDCANCEL)
            EndDialog(hdlg, wParam == IDOK);
        return fTrue;
    }
    else if (iMessage == WM_INITDIALOG)
    {
        SetDlgItemInt(hdlg, ideGameNoEdit, igmCur, fFalse);
        return fTrue;
    }
    else
        return fFalse;
}



BOOL FSetGameNo()
{
    BOOL fResult;

    if(fResult = (BOOL)DialogBox(hinstApp,
                                 MAKEINTRESOURCE(iddGameNo),
                                 hwndApp,
                                 GameNo))

        srand(igmCur);
    return fResult;
}






BOOL FValidCol(COL *pcol)
{
    INT icol;

    if(pcol == NULL)
        return fFalse;

    for(icol = 0; icol < pgmCur->icolMax; icol++)
    {
        if(pcol == pgmCur->rgpcol[icol])
        {
            if(pcol->pcolcls == NULL)
                return fFalse;
            if(pcol->icrdMac > pcol->icrdMax)
                return fFalse;

            return fTrue;
        }
    }
    return fFalse;
}


TCHAR *vszFile;
INT vli;

TCHAR vszLi[32];

INT_PTR APIENTRY AssertDlgProc(HANDLE hdlg, UINT wm, WPARAM wParam, LPARAM lParam)
{

    switch(wm)
    {
        case WM_INITDIALOG:
            SetWindowText(GetDlgItem(hdlg, FILENUM), vszFile);
            CchDecodeInt(vszLi, vli);
            SetWindowText(GetDlgItem(hdlg, LINE), vszLi);
            CchDecodeInt(vszLi, igmCur);
            SetWindowText(GetDlgItem(hdlg, GAMENO), vszLi);
         /* SetWindowText(GetDlgItem(hdlg, SOLVERSION), szVer); */
            break;
        case WM_COMMAND:
            switch( GET_WM_COMMAND_ID( wParam, lParam ))
            {
                default:
                    return fFalse;
                case IDOK:
                    EndDialog(hdlg, 0);
                    break;
                case EXIT:
                    EndDialog(hdlg, 1);
                    break;
             }
            break;

        default:
                return fFalse;
    }

    return fTrue;
}


VOID AssertFailed(TCHAR *szFile, INT li)
{
    INT f;

    vszFile = szFile;
    vli = li;

    f = (INT)DialogBox(hinstApp,
                       MAKEINTRESOURCE(ASSERTFAILED),
                       hwndApp,
                       AssertDlgProc);


    DumpRgmdbg(szFile, li);
}

#endif
