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

#ifdef AFX_CORE4_SEG
#pragma code_seg(AFX_CORE4_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditView

#define new DEBUG_NEW

AFX_STATIC const UINT _afxMsgFindReplace = ::RegisterWindowMessage(FINDMSGSTRING);

#ifdef _UNICODE

AFX_STATIC_DATA HFONT _afxUnicodeFont = 0;

void AFX_CDECL AfxEditviewTerm()
{
	AfxDeleteObject((HGDIOBJ*)&_afxUnicodeFont);
}
char _afxEditviewTerm = (char)atexit(&AfxEditviewTerm);

#endif //_UNICODE

BEGIN_MESSAGE_MAP(CEditView, CCtrlView)
	//{{AFX_MSG_MAP(CEditView)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateNeedClip)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateNeedText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPLACE, OnUpdateNeedText)
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REPEAT, OnUpdateNeedFind)
#endif // _WIN32_WCE_NO_FINDREPLACE
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateNeedSel)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateNeedSel)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnEditChange)
	ON_WM_CREATE()
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_REPLACE, OnEditReplace)
	ON_COMMAND(ID_EDIT_REPEAT, OnEditRepeat)
#endif // _WIN32_WCE_NO_FINDREPLACE
	ON_WM_DESTROY()
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_CHAR()
#endif //_WIN32_WCE_PSPC
	//}}AFX_MSG_MAP
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	// Special registered message for Find and Replace
	ON_REGISTERED_MESSAGE(_afxMsgFindReplace, OnFindReplaceCmd)
#endif // _WIN32_WCE_NO_FINDREPLACE
	// Standard Print commands (print only - not preview)
#if !defined(_WIN32_WCE_NO_PRINTING)
	ON_COMMAND(ID_FILE_PRINT, CCtrlView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CCtrlView::OnFilePrint)
#endif // _WIN32_WCE_NO_PRINTING
#if defined(_WIN32_WCE)
// WinCE: Trap the escape key because it causes problems with CEditView
WCE_INS	ON_WM_GETDLGCODE()
#endif // _WIN32_WCE
END_MESSAGE_MAP()

const AFX_DATADEF DWORD CEditView::dwStyleDefault =
	AFX_WS_DEFAULT_VIEW |
	WS_HSCROLL | WS_VSCROLL |
	ES_AUTOHSCROLL | ES_AUTOVSCROLL |
	ES_MULTILINE | ES_NOHIDESEL;

// Operating system specific maximum buffer limit
const AFX_DATADEF UINT CEditView::nMaxSize = 1024U*1024U-1;

#if !defined(_WIN32_WCE_NO_FINDREPLACE)  
/////////////////////////////////////////////////////////////////////////////
// _AFX_EDIT_STATE

_AFX_EDIT_STATE::_AFX_EDIT_STATE()
{
	// Note: it is only necessary to initialize non-zero data.

	bNext = TRUE;
}

_AFX_EDIT_STATE::~_AFX_EDIT_STATE()
{
}

EXTERN_PROCESS_LOCAL(_AFX_EDIT_STATE, _afxEditState)
#endif // _WIN32_WCE_NO_FINDREPLACE

/////////////////////////////////////////////////////////////////////////////
// CEditView construction/destruction

// pass a NULL style because dwStyleDefault stays for backward compatibility
CEditView::CEditView() : CCtrlView(_T("EDIT"), NULL)
{
	m_nTabStops = 8*4;  // default 8 character positions
#if !defined(_WIN32_WCE_NO_PRINTING)
	m_hPrinterFont = NULL;
	m_hMirrorFont = NULL;
#endif // _WIN32_WCE_NO_PRINTING
	m_pShadowBuffer = NULL;
	m_nShadowSize = 0;
}

CEditView::~CEditView()
{
	ASSERT(m_hWnd == NULL);
	ASSERT(m_pShadowBuffer == NULL || WCE_IF(TRUE,afxData.bWin95));
	delete[] m_pShadowBuffer;
}

BOOL CEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	m_dwDefaultStyle = dwStyleDefault;
	return CCtrlView::PreCreateWindow(cs);
}

int CEditView::OnCreate(LPCREATESTRUCT lpcs)
{
	if (CCtrlView::OnCreate(lpcs) != 0)
		return -1;

#ifdef _UNICODE
	AfxLockGlobals(CRIT_EDITVIEW);
	if (_afxUnicodeFont == NULL)
	{
		// get unicode font same size as system font
		HFONT hSystemFont = (HFONT)GetStockObject(SYSTEM_FONT);
		LOGFONT systemFont;
		VERIFY(::GetObject(hSystemFont, sizeof(LOGFONT), (void*)&systemFont));

		// default size and facename, but allow customization
		LOGFONT logFont; memset(&logFont, 0, sizeof(LOGFONT));
		logFont.lfHeight = systemFont.lfHeight;
		logFont.lfWeight = systemFont.lfWeight;
		logFont.lfCharSet = DEFAULT_CHARSET;
		lstrcpy(logFont.lfFaceName, _T("Lucida Sans Unicode"));
		AfxCustomLogFont(AFX_IDS_UNICODE_FONT, &logFont);

		// attempt to create the font
		_afxUnicodeFont = ::CreateFontIndirect(&logFont);
		if (_afxUnicodeFont == NULL)
			TRACE1("Unable to create unicode font '%s'.\n", logFont.lfFaceName);
	}
	AfxUnlockGlobals(CRIT_EDITVIEW);
	// set unicode font instead of using system font
	if (_afxUnicodeFont != NULL)
		SendMessage(WM_SETFONT, (WPARAM)_afxUnicodeFont);
#endif

	GetEditCtrl().LimitText(nMaxSize);
	GetEditCtrl().SetTabStops(m_nTabStops);

	return 0;
}

#if defined(_WIN32_WCE)
// WinCE: the edit control processes escape and return keys differently if it thinks
// it is parented by a a dialog box.  Handling WM_GETDLGCODE on our own fixes this problem.
LRESULT CEditView::OnGetDlgCode()
{
	return (DLGC_WANTCHARS | DLGC_HASSETSEL | DLGC_WANTARROWS | DLGC_WANTALLKEYS);
}
#endif // _WIN32_WCE

void CEditView::OnDestroy()
{
	_AFX_EDIT_STATE* pEditState = _afxEditState;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	if (pEditState->pFindReplaceDlg != NULL && ::IsWindow(pEditState->pFindReplaceDlg->m_hWnd))
	{
		pEditState->pFindReplaceDlg->SendMessage(WM_CLOSE);
	}
#endif //_WIN32_WCE_PSPC

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	pEditState->pFindReplaceDlg = NULL;
#endif // _WIN32_WCE_NO_FINDREPLACE

	CView::OnDestroy();
}

// EDIT controls always turn off WS_BORDER and draw it themselves
#if defined(_WIN32_WCE)  
void CEditView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	::AdjustWindowRectEx(lpClientRect, GetStyle() & ~(WS_HSCROLL|WS_VSCROLL), FALSE,
		GetExStyle() & ~(WS_EX_CLIENTEDGE));
}
#else // _WIN32_WCE
void CEditView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	if (nAdjustType != 0)
	{
		// default behavior for in-place editing handles scrollbars
		DWORD dwStyle = GetStyle();
		if (dwStyle & WS_VSCROLL)
			lpClientRect->right += afxData.cxVScroll - CX_BORDER;
		if (dwStyle & WS_HSCROLL)
			lpClientRect->bottom += afxData.cyHScroll - CY_BORDER;
		return;
	}

	::AdjustWindowRectEx(lpClientRect, GetStyle() | WS_BORDER, FALSE,
		GetExStyle() & ~(WS_EX_CLIENTEDGE));
}
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
// CEditView document like functions

void CEditView::DeleteContents()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	SetWindowText(NULL);
	ASSERT_VALID(this);
}

void CEditView::Serialize(CArchive& ar)
	// Read and write CEditView object to archive, with length prefix.
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	if (ar.IsStoring())
	{
		UINT nLen = GetBufferLength();
		ar << (DWORD)nLen;
		WriteToArchive(ar);
	}
	else
	{
		DWORD dwLen;
		ar >> dwLen;
		if (dwLen > nMaxSize)
			AfxThrowArchiveException(CArchiveException::badIndex);
		UINT nLen = (UINT)dwLen;
		ReadFromArchive(ar, nLen);
	}
	ASSERT_VALID(this);
}

void CEditView::ReadFromArchive(CArchive& ar, UINT nLen)
	// Read certain amount of text from the file, assume at least nLen
	// characters (not bytes) are in the file.
{
	ASSERT_VALID(this);

	LPVOID hText = LocalAlloc(LMEM_MOVEABLE, (nLen+1)*sizeof(TCHAR));
	if (hText == NULL)
		AfxThrowMemoryException();

	LPTSTR lpszText = (LPTSTR)LocalLock(hText);
	ASSERT(lpszText != NULL);
	if (ar.Read(lpszText, nLen*sizeof(TCHAR)) != nLen*sizeof(TCHAR))
	{
		LocalUnlock(hText);
		LocalFree(hText);
		AfxThrowArchiveException(CArchiveException::endOfFile);
	}
	// Replace the editing edit buffer with the newly loaded data
	lpszText[nLen] = '\0';
#ifndef _UNICODE
	if (afxData.bWin95)
	{
		// set the text with SetWindowText, then free
		BOOL bResult = ::SetWindowText(m_hWnd, lpszText);
		LocalUnlock(hText);
		LocalFree(hText);

		// make sure that SetWindowText was successful
		if (!bResult || ::GetWindowTextLength(m_hWnd) < (int)nLen)
			AfxThrowMemoryException();

		// remove old shadow buffer
		delete[] m_pShadowBuffer;
		m_pShadowBuffer = NULL;
		m_nShadowSize = 0;

		ASSERT_VALID(this);
		return;
	}
#endif
#if defined(_WIN32_WCE)
// WinCE: it is necessary to maintain a shadow buffer since
// EM_GETHANDLE is not supported.
	BOOL bResult = ::SetWindowText(m_hWnd, lpszText);
	LocalUnlock(hText);
	LocalFree(hText);

	// make sure that SetWindowText was successful
	if (!bResult || ::GetWindowTextLength(m_hWnd) < (int)nLen)
		AfxThrowMemoryException();

	// remove old shadow buffer
	delete[] m_pShadowBuffer;
	m_pShadowBuffer = NULL;
	m_nShadowSize = 0;

	ASSERT_VALID(this);
#else // _WIN32_WCE
	LocalUnlock(hText);
	HLOCAL hOldText = GetEditCtrl().GetHandle();
	ASSERT(hOldText != NULL);
	LocalFree(hOldText);
	GetEditCtrl().SetHandle((HLOCAL)(UINT)(DWORD)hText);
	Invalidate();
	ASSERT_VALID(this);
#endif // _WIN32_WCE
}

void CEditView::WriteToArchive(CArchive& ar)
	// Write just the text to an archive, no length prefix.
{
	ASSERT_VALID(this);
	LPCTSTR lpszText = LockBuffer();
	ASSERT(lpszText != NULL);
	UINT nLen = GetBufferLength();
	TRY
	{
		ar.Write(lpszText, nLen*sizeof(TCHAR));
	}
	CATCH_ALL(e)
	{
		UnlockBuffer();
		THROW_LAST();
	}
	END_CATCH_ALL
	UnlockBuffer();
	ASSERT_VALID(this);
}

void CEditView::SerializeRaw(CArchive& ar)
	// Read/Write object as stand-alone file.
{
	ASSERT_VALID(this);
	if (ar.IsStoring())
	{
		WriteToArchive(ar);
	}
	else
	{
		CFile* pFile = ar.GetFile();
		ASSERT(pFile->GetPosition() == 0);
		DWORD nFileSize = pFile->GetLength();
		if (nFileSize/sizeof(TCHAR) > nMaxSize)
		{
			AfxMessageBox(AFX_IDP_FILE_TOO_LARGE);
			AfxThrowUserException();
		}
		// ReadFromArchive takes the number of characters as argument
		ReadFromArchive(ar, (UINT)nFileSize/sizeof(TCHAR));
	}
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Printing Helpers

AFX_STATIC UINT AFXAPI _AfxEndOfLine(LPCTSTR lpszText, UINT nLen, UINT nIndex)
{
	ASSERT(AfxIsValidAddress(lpszText, nLen, FALSE));
	LPCTSTR lpsz = lpszText + nIndex;
	LPCTSTR lpszStop = lpszText + nLen;
	while (lpsz < lpszStop && *lpsz != '\r')
		++lpsz;
	return lpsz - lpszText;
}

#if !defined(_WIN32_WCE_NO_PRINTING)
AFX_STATIC UINT AFXAPI _AfxNextLine(LPCTSTR lpszText, UINT nLen, UINT nIndex)
{
	ASSERT(AfxIsValidAddress(lpszText, nLen, FALSE));
	LPCTSTR lpsz = lpszText + nIndex;
	LPCTSTR lpszStop = lpszText + nLen;
	while (lpsz < lpszStop && *lpsz == '\r')
		++lpsz;
	if (lpsz < lpszStop && *lpsz == '\n')
		++lpsz;
	return lpsz - lpszText;
}

AFX_STATIC UINT AFXAPI
_AfxClipLine(CDC* pDC, int aCharWidths[256], int cxLine, int nTabStop,
	LPCTSTR lpszText, UINT nIndex, UINT nIndexEnd)
{
	ASSERT_VALID(pDC);
	ASSERT(nIndex < nIndexEnd);
	ASSERT(AfxIsValidAddress(lpszText, nIndexEnd, FALSE));

	TEXTMETRIC tm;
	::GetTextMetrics(pDC->m_hDC, &tm);

	// make an initial guess on the number of characters that will fit
	int cx = 0;
	LPCTSTR lpszStart = lpszText + nIndex;
	LPCTSTR lpszStop = lpszText + nIndexEnd;
	LPCTSTR lpsz = lpszStart;
	while (lpsz < lpszStop)
	{
		if (*lpsz == '\t')
			cx += nTabStop - (cx % nTabStop);
		else
		{
#ifdef _UNICODE
			if (*lpsz <= 0xFF)
				cx += aCharWidths[(BYTE)*lpsz];
			else
				cx += tm.tmAveCharWidth;
#else //_UNICODE
			if (_afxDBCS && _istlead(*lpsz))
			{
				++lpsz;
				cx += tm.tmAveCharWidth;
			}
			else
				cx += aCharWidths[(BYTE)*lpsz];
#endif //!_UNICODE
		}
		++lpsz;
		if (cx > cxLine)
			break;
	}

	// adjust for errors in the guess
#if defined(_WIN32_WCE)
	cx = pDC->GetTextExtent(lpszStart, lpsz-lpszStart).cx;
#else // _WIN32_WCE
	cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
#endif // _WIN32_WCE
	if (cx > cxLine)
	{
		// remove characters until it fits
		do
		{
			ASSERT(lpsz != lpszStart);
			if (_afxDBCS)
				lpsz = _tcsdec(lpszStart, lpsz);
			else
				--lpsz;
#if defined(_WIN32_WCE)
			cx = pDC->GetTextExtent(lpszStart, lpsz-lpszStart).cx;
#else // _WIN32_WCE
			cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
#endif // _WIN32_WCE
		} while (cx > cxLine);
	}
	else if (cx < cxLine)
	{
		// add characters until it doesn't fit
		while (lpsz < lpszStop)
		{
			lpsz = _tcsinc(lpsz);
			ASSERT(lpsz <= lpszStop);
#if defined(_WIN32_WCE)
			cx = pDC->GetTextExtent(lpszStart, lpsz-lpszStart).cx;
#else // _WIN32_WCE
			cx = pDC->GetTabbedTextExtent(lpszStart, lpsz-lpszStart, 1, &nTabStop).cx;
#endif // _WIN32_WCE
			if (cx > cxLine)
			{
				if (_afxDBCS)
					lpsz = _tcsdec(lpszStart, lpsz);
				else
					--lpsz;
				break;
			}
		}
	}

	// return index of character just past the last that would fit
	return lpsz - lpszText;
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Printing support

BOOL CEditView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CEditView::OnBeginPrinting(CDC* pDC, CPrintInfo*)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	// initialize page start vector
	ASSERT(m_aPageStart.GetSize() == 0);
	m_aPageStart.Add(0);
	ASSERT(m_aPageStart.GetSize() > 0);

	if (m_hPrinterFont == NULL)
	{
		// get current screen font object metrics
		CFont* pFont = GetFont();
		LOGFONT lf;
		LOGFONT lfSys;
		if (pFont == NULL)
			return;
		VERIFY(pFont->GetObject(sizeof(LOGFONT), &lf));
		VERIFY(::GetObject(::GetStockObject(SYSTEM_FONT), sizeof(LOGFONT),
			&lfSys));
		if (lstrcmpi((LPCTSTR)lf.lfFaceName, (LPCTSTR)lfSys.lfFaceName) == 0)
			return;

		// map to printer font metrics
		HDC hDCFrom = ::GetDC(NULL);
		lf.lfHeight = ::WCE_FCTN(MulDiv)(lf.lfHeight, pDC->GetDeviceCaps(LOGPIXELSY),
			::GetDeviceCaps(hDCFrom, LOGPIXELSY));
		lf.lfWidth = ::WCE_FCTN(MulDiv)(lf.lfWidth, pDC->GetDeviceCaps(LOGPIXELSX),
			::GetDeviceCaps(hDCFrom, LOGPIXELSX));
		::ReleaseDC(NULL, hDCFrom);

		// create it, if it fails we just use the printer's default.
		m_hMirrorFont = ::CreateFontIndirect(&lf);
		m_hPrinterFont = m_hMirrorFont;
	}
	ASSERT_VALID(this);
}

BOOL CEditView::PaginateTo(CDC* pDC, CPrintInfo* pInfo)
	// attempts pagination to pInfo->m_nCurPage, TRUE == success
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	CRect rectSave = pInfo->m_rectDraw;
	UINT nPageSave = pInfo->m_nCurPage;
	ASSERT(nPageSave > 1);
	ASSERT(nPageSave >= (UINT)m_aPageStart.GetSize());
	VERIFY(pDC->SaveDC() != 0);
	pDC->IntersectClipRect(0, 0, 0, 0);
	pInfo->m_nCurPage = m_aPageStart.GetSize();
	while (pInfo->m_nCurPage < nPageSave)
	{
		ASSERT(pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize());
		OnPrepareDC(pDC, pInfo);
		ASSERT(pInfo->m_bContinuePrinting);
		pInfo->m_rectDraw.SetRect(0, 0,
			pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
		pDC->DPtoLP(&pInfo->m_rectDraw);
		OnPrint(pDC, pInfo);
		if (pInfo->m_nCurPage == (UINT)m_aPageStart.GetSize())
			break;
		++pInfo->m_nCurPage;
	}
	BOOL bResult = pInfo->m_nCurPage == nPageSave;
	pDC->RestoreDC(-1);
	pInfo->m_nCurPage = nPageSave;
	pInfo->m_rectDraw = rectSave;
	ASSERT_VALID(this);
	return bResult;
}

void CEditView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);  // overriding OnPaint -- never get this.

	if (pInfo->m_nCurPage > (UINT)m_aPageStart.GetSize() &&
		!PaginateTo(pDC, pInfo))
	{
		// can't paginate to that page, thus cannot print it.
		pInfo->m_bContinuePrinting = FALSE;
	}
	ASSERT_VALID(this);
}

UINT CEditView::PrintInsideRect(CDC* pDC, RECT& rectLayout,
	UINT nIndexStart, UINT nIndexStop)
	// worker function for laying out text in a rectangle.
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	BOOL bWordWrap = (GetStyle() & ES_AUTOHSCROLL) == 0;

	// get buffer and real starting and ending postions
	UINT nLen = GetBufferLength();
	if (nIndexStart >= nLen)
		return nLen;
	LPCTSTR lpszText = LockBuffer();
	if (nIndexStop > nLen)
		nIndexStop = nLen;
	ASSERT(nIndexStart < nLen);

	// calculate text & tab metrics
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cyChar = tm.tmHeight + tm.tmExternalLeading;
#if defined(_WIN32_WCE)
		int nTabStop = pDC->GetTextExtent(_T("\t"), 1).cx;
#else // _WIN32_WCE
	int nTabStop = m_nTabStops *
		pDC->GetTabbedTextExtent(_T("\t"), 1, 0, NULL).cx / 8 / 4;
#endif // _WIN32_WCE
	int aCharWidths[256];
#if defined(_WIN32_WCE)
	for(TCHAR szChar[2]=_T("\0"); *szChar <= 255; (*szChar)++)
		aCharWidths[*szChar] = pDC->GetTextExtent(szChar,1).cx;
#else // _WIN32_WCE
	pDC->GetCharWidth(0, 255, aCharWidths);
#endif // _WIN32_WCE

	int y = rectLayout.top;
	UINT cx = rectLayout.right - rectLayout.left;
	UINT nIndex = nIndexStart;

	VERIFY(pDC->SaveDC() != 0);
	BOOL bLayoutOnly = pDC->IntersectClipRect(&rectLayout) == NULLREGION;

	do
	{
		UINT nIndexEnd = _AfxEndOfLine(lpszText, nIndexStop, nIndex);
		if (nIndex == nIndexEnd)
		{
			y += cyChar;
		}
		else if (bWordWrap)
		{
			// word-wrap printing
			do
			{
				UINT nIndexWrap = _AfxClipLine(pDC, aCharWidths,
					cx, nTabStop, lpszText, nIndex, nIndexEnd);
				UINT nIndexWord = nIndexWrap;
				if (nIndexWord != nIndexEnd)
				{
					while (nIndexWord > nIndex &&
					  !_istspace(lpszText[nIndexWord]))
					{
						nIndexWord--;
					}
					if (nIndexWord == nIndex)
						nIndexWord = nIndexWrap;
				}
				CRect rect(rectLayout.left, y, rectLayout.right, y+cyChar);
				if (!bLayoutOnly && pDC->RectVisible(rect))
				{
#if defined(_WIN32_WCE)
					pDC->ExtTextOut(rect.left, y, 0, NULL,
						(LPCTSTR)(lpszText+nIndex), nIndexWord-nIndex, NULL);
#else // _WIN32_WCE
					pDC->TabbedTextOut(rect.left, y,
						(LPCTSTR)(lpszText+nIndex), nIndexWord-nIndex, 1,
						&nTabStop, rect.left);
#endif // _WIN32_WCE
				}
				y += cyChar;
				nIndex = nIndexWord;
				while (nIndex < nIndexEnd && _istspace(lpszText[nIndex]))
					nIndex++;
			} while (nIndex < nIndexEnd && y+cyChar <= rectLayout.bottom);

			nIndexEnd = nIndex;
		}
		else
		{
			// non-word wrap printing (much easier and faster)
			CRect rect(rectLayout.left, y, rectLayout.right, y+cyChar);
			if (!bLayoutOnly && pDC->RectVisible(rect))
			{
				UINT nIndexClip = _AfxClipLine(pDC, aCharWidths, cx, nTabStop,
					lpszText, nIndex, nIndexEnd);
				if (nIndexClip < nIndexEnd)
				{
					if (_istlead(*(lpszText+nIndexClip)))
						nIndexClip++;
					nIndexClip++;
				}
#if defined(_WIN32_WCE)
					pDC->ExtTextOut(rect.left, y, 0, NULL,
						(LPCTSTR)(lpszText+nIndex), nIndexClip-nIndex, NULL);
#else // _WIN32_WCE
				pDC->TabbedTextOut(rect.left, y,
					(LPCTSTR)(lpszText+nIndex), nIndexClip-nIndex, 1,
					&nTabStop, rect.left);
#endif // _WIN32_WCE
			}
			y += cyChar;
		}
		nIndex = _AfxNextLine(lpszText, nIndexStop, nIndexEnd);
	}
	while (nIndex < nIndexStop && y+cyChar <= rectLayout.bottom);

	pDC->RestoreDC(-1);
	UnlockBuffer();
	ASSERT_VALID(this);

	rectLayout.bottom = y;
	return nIndex;
}

void CEditView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);
	ASSERT(pInfo != NULL);
	ASSERT(pInfo->m_bContinuePrinting);

	CFont* pOldFont = NULL;
	if (m_hPrinterFont != NULL)
		pOldFont = pDC->SelectObject(CFont::FromHandle(m_hPrinterFont));
	pDC->SetBkMode(TRANSPARENT);

	UINT nPage = pInfo->m_nCurPage;
	ASSERT(nPage <= (UINT)m_aPageStart.GetSize());
	UINT nIndex = m_aPageStart[nPage-1];

	// print as much as possible in the current page.
	nIndex = PrintInsideRect(pDC, pInfo->m_rectDraw, nIndex, GetBufferLength());

	if (pOldFont != NULL)
		pDC->SelectObject(pOldFont);

	// update pagination information for page just printed
	if (nPage == (UINT)m_aPageStart.GetSize())
	{
		if (nIndex < GetBufferLength())
			m_aPageStart.Add(nIndex);
	}
	else
	{
		ASSERT(nPage+1 <= (UINT)m_aPageStart.GetSize());
		ASSERT(nIndex == m_aPageStart[nPage+1-1]);
	}
}

void CEditView::OnEndPrinting(CDC*, CPrintInfo*)
{
	ASSERT_VALID(this);

	m_aPageStart.RemoveAll();
	if (m_hMirrorFont != NULL && m_hPrinterFont == m_hMirrorFont)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hMirrorFont);
		m_hPrinterFont = NULL;
	}
}
#endif // _WIN32_WCE_NO_PRINTING

/////////////////////////////////////////////////////////////////////////////
// CEditView commands

void CEditView::OnUpdateNeedSel(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	pCmdUI->Enable(nStartChar != nEndChar);
	ASSERT_VALID(this);
}

void CEditView::OnUpdateNeedClip(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
#if defined(_WIN32_WCE)
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT)    ||
	               ::IsClipboardFormatAvailable(CF_OEMTEXT) ||
	               ::IsClipboardFormatAvailable(CF_UNICODETEXT)); 
#else // _WIN32_WCE
	pCmdUI->Enable(::IsClipboardFormatAvailable(CF_TEXT));
#endif // _WIN32_WCE
	ASSERT_VALID(this);
}

void CEditView::OnUpdateNeedText(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetWindowTextLength() != 0);
	ASSERT_VALID(this);
}

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
void CEditView::OnUpdateNeedFind(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	pCmdUI->Enable(GetWindowTextLength() != 0 &&
		!pEditState->strFind.IsEmpty());
	ASSERT_VALID(this);
}
#endif // _WIN32_WCE_NO_FINDREPLACE

void CEditView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	ASSERT_VALID(this);
	pCmdUI->Enable(GetEditCtrl().CanUndo());
	ASSERT_VALID(this);
}

BOOL CEditView::OnEditChange()
{
	ASSERT_VALID(this);
	GetDocument()->SetModifiedFlag();
	ASSERT_VALID(this);
	return FALSE;   // continue routing
}

void CEditView::OnEditCut()
{
	ASSERT_VALID(this);
	GetEditCtrl().Cut();
	ASSERT_VALID(this);
}

void CEditView::OnEditCopy()
{
	ASSERT_VALID(this);
	GetEditCtrl().Copy();
	ASSERT_VALID(this);
}

void CEditView::OnEditPaste()
{
	ASSERT_VALID(this);
	GetEditCtrl().Paste();
	ASSERT_VALID(this);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	// the search is restarted from current cursor location if the user changes text
	// while the find/replace dlg bar is up
	if (pEditState->pFindReplaceDlg != NULL && pEditState->pFindReplaceDlg->IsBarVisible())
	{
		int nStartChar, nEndChar;
		GetEditCtrl().GetSel(nStartChar, nEndChar);
		pEditState->nStartingPos = nStartChar;
		pEditState->bJustStarted = TRUE;
	}
#endif // _WIN32_WCE_PSPC
}

void CEditView::OnEditClear()
{
	ASSERT_VALID(this);
	GetEditCtrl().Clear();
	ASSERT_VALID(this);
}

void CEditView::OnEditUndo()
{
	ASSERT_VALID(this);
	GetEditCtrl().Undo();
	ASSERT_VALID(this);
}

void CEditView::OnEditSelectAll()
{
	ASSERT_VALID(this);
	GetEditCtrl().SetSel(0, -1);
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView Font Handling

LRESULT CEditView::OnSetFont(WPARAM, LPARAM)
{
	ASSERT_VALID(this);
	Default();
	GetEditCtrl().SetTabStops(m_nTabStops);
	ASSERT_VALID(this);
	return 0;
}

#if !defined(_WIN32_WCE_NO_PRINTING)
void CEditView::SetPrinterFont(CFont* pFont)
{
	ASSERT_VALID(this);
	m_hPrinterFont = (HFONT)pFont->GetSafeHandle();
	ASSERT_VALID(this);
}

CFont* CEditView::GetPrinterFont() const
{
	ASSERT_VALID(this);
	return CFont::FromHandle(m_hPrinterFont);
}
#endif // _WIN32_WCE_NO_PRINTING

/////////////////////////////////////////////////////////////////////////////
// CEditView attributes

LPCTSTR CEditView::LockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
#ifndef _UNICODE
	if (afxData.bWin95)
	{
		// under Win32s, it is necessary to maintain a shadow buffer
		//  it is only updated when the control contents have been changed.
		if (m_pShadowBuffer == NULL || GetEditCtrl().GetModify())
		{
			ASSERT(m_pShadowBuffer != NULL || m_nShadowSize == 0);
			UINT nSize = GetWindowTextLength()+1;
			if (nSize > m_nShadowSize)
			{
				// need more room for shadow buffer
				CEditView* pThis = (CEditView*)this;
				delete[] m_pShadowBuffer;
				pThis->m_pShadowBuffer = NULL;
				pThis->m_nShadowSize = 0;
				pThis->m_pShadowBuffer = new TCHAR[nSize];
				pThis->m_nShadowSize = nSize;
			}

			// update the shadow buffer with GetWindowText
			ASSERT(m_nShadowSize >= nSize);
			ASSERT(m_pShadowBuffer != NULL);
			GetWindowText(m_pShadowBuffer, nSize);

			// turn off edit control's modify bit
			GetEditCtrl().SetModify(FALSE);
		}
		return m_pShadowBuffer;
	}
#endif
	// else -- running under non-subset Win32 system
#if defined(_WIN32_WCE)
// WinCE: it is necessary to maintain a shadow buffer since
// EM_GETHANDLE is not supported.
	if (m_pShadowBuffer == NULL || GetEditCtrl().GetModify())
	{
		ASSERT(m_pShadowBuffer != NULL || m_nShadowSize == 0);
		UINT nSize = GetWindowTextLength()+1;
		if (nSize > m_nShadowSize)
		{
			// need more room for shadow buffer
			CEditView* pThis = (CEditView*)this;
			delete[] m_pShadowBuffer;
			pThis->m_pShadowBuffer = NULL;
			pThis->m_nShadowSize = 0;
			pThis->m_pShadowBuffer = new TCHAR[nSize];
			pThis->m_nShadowSize = nSize;
		}

		// update the shadow buffer with GetWindowText
		ASSERT(m_nShadowSize >= nSize);
		ASSERT(m_pShadowBuffer != NULL);
		GetWindowText(m_pShadowBuffer, nSize);

		// turn off edit control's modify bit
		GetEditCtrl().SetModify(FALSE);
	}
	return m_pShadowBuffer;
#else // _WIN32_WCE
	HLOCAL hLocal = GetEditCtrl().GetHandle();
	ASSERT(hLocal != NULL);
	LPCTSTR lpszText = (LPCTSTR)LocalLock(hLocal);
	ASSERT(lpszText != NULL);
	ASSERT_VALID(this);
	return lpszText;
#endif // _WIN32_WCE
}

void CEditView::UnlockBuffer() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
#ifndef _UNICODE
	if (afxData.bWin95)
		return;
#endif
#if !defined(_WIN32_WCE)
// WinCE: LockBuffer doesn't really lock the buffer. A shadow buffer was used instead.
	HLOCAL hLocal = GetEditCtrl().GetHandle();
	ASSERT(hLocal != NULL);
	LocalUnlock(hLocal);
#endif // _WIN32_WCE
}

// this function returns the length in characters
UINT CEditView::GetBufferLength() const
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	LPCTSTR lpszText = LockBuffer();
	UINT nLen = lstrlen(lpszText);
	UnlockBuffer();
	return nLen;
}

void CEditView::GetSelectedText(CString& strResult) const
{
	ASSERT_VALID(this);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	ASSERT((UINT)nEndChar <= GetBufferLength());
	LPCTSTR lpszText = ((CEditView*)this)->LockBuffer();
	UINT nLen = _AfxEndOfLine(lpszText, nEndChar, nStartChar) - nStartChar;
	WCE_FCTN(memcpy)(strResult.GetBuffer(nLen), lpszText + nStartChar,
		nLen * sizeof(TCHAR));
	strResult.ReleaseBuffer(nLen);
	UnlockBuffer();
	ASSERT_VALID(this);
}

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
/////////////////////////////////////////////////////////////////////////////
// CEditView Find & Replace

void CEditView::OnEditFind()
{
	ASSERT_VALID(this);
	OnEditFindReplace(TRUE);
	ASSERT_VALID(this);
}

void CEditView::OnEditReplace()
{
	ASSERT_VALID(this);
	OnEditFindReplace(FALSE);
	ASSERT_VALID(this);
}

void CEditView::OnEditRepeat()
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (!FindText(pEditState->strFind,
		pEditState->bNext,
		pEditState->bCase))
	{
		OnTextNotFound(pEditState->strFind);
	}
	ASSERT_VALID(this);
}

void CEditView::OnEditFindReplace(BOOL bFindOnly)
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (pEditState->pFindReplaceDlg != NULL)
	{
#if !(defined (_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300))
		if (pEditState->bFindOnly == bFindOnly)
		{
			pEditState->pFindReplaceDlg->SetActiveWindow();
			pEditState->pFindReplaceDlg->ShowWindow(SW_SHOW);
			return;
		}
		ASSERT(pEditState->bFindOnly != bFindOnly);
#endif // _WIN32_WCE_PSPC
		pEditState->pFindReplaceDlg->SendMessage(WM_CLOSE);
		ASSERT(pEditState->pFindReplaceDlg == NULL);
		ASSERT_VALID(this);
	}

	CString strFind;
	GetSelectedText(strFind);
	if (strFind.IsEmpty())
		strFind = pEditState->strFind;
	CString strReplace = pEditState->strReplace;
	pEditState->pFindReplaceDlg = new CFindReplaceDialog;
	ASSERT(pEditState->pFindReplaceDlg != NULL);
	DWORD dwFlags = FR_HIDEWHOLEWORD;
	if (pEditState->bNext)
		dwFlags |= FR_DOWN;
	if (pEditState->bCase)
		dwFlags |= FR_MATCHCASE;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	pEditState->nStartingPos = nStartChar;
	pEditState->bJustStarted = TRUE;
#endif // _WIN32_WCE_PSPC

	if (!pEditState->pFindReplaceDlg->Create(bFindOnly, strFind,
		strReplace, dwFlags, this))
	{
		pEditState->pFindReplaceDlg = NULL;
		ASSERT_VALID(this);
		return;
	}

	pEditState->pFindReplaceDlg->SetActiveWindow();
	pEditState->pFindReplaceDlg->ShowWindow(SW_SHOW);
	ASSERT(pEditState->pFindReplaceDlg != NULL);
	pEditState->bFindOnly = bFindOnly;
	ASSERT_VALID(this);
}

void CEditView::OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase)
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	pEditState->strFind = lpszFind;
	pEditState->bCase = bCase;
	pEditState->bNext = bNext;

	if (!FindText(pEditState->strFind, bNext, bCase))
		OnTextNotFound(pEditState->strFind);
	ASSERT_VALID(this);
}

void CEditView::OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
	LPCTSTR lpszReplace)
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	pEditState->strFind = lpszFind;
	pEditState->strReplace = lpszReplace;
	pEditState->bCase = bCase;
	pEditState->bNext = bNext;

	if (!InitializeReplace())
		return;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	UINT nStartingPos = pEditState->nStartingPos;
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	UINT nBeforeReplaceStart = nStartChar;
	UINT nBeforeReplaceEnd = nEndChar;
#endif

	GetEditCtrl().ReplaceSel(pEditState->strReplace);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	UINT nAfterReplaceStart = nStartChar;
	if(nStartingPos == nBeforeReplaceStart && nStartingPos == nAfterReplaceStart && nStartingPos == nBeforeReplaceStart)
		pEditState->bJustStarted = TRUE;

	if(nStartingPos > nBeforeReplaceStart)
		pEditState->nStartingPos += nAfterReplaceStart - nBeforeReplaceEnd;

	if(!pEditState->bJustStarted && nStartingPos > nBeforeReplaceStart && nStartingPos < nAfterReplaceStart)
		pEditState->nStartingPos = nAfterReplaceStart;
#endif

	FindText(pEditState->strFind, bNext, bCase);
	ASSERT_VALID(this);
}

void CEditView::OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bCase)
{
	ASSERT_VALID(this);
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	pEditState->strFind = lpszFind;
	pEditState->strReplace = lpszReplace;
	pEditState->bCase = bCase;
	pEditState->bNext = TRUE;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	BOOL bJustStarted = pEditState->bJustStarted;
#endif

	if (!InitializeReplace() &&
		!SameAsSelected(pEditState->strFind, pEditState->bCase))
	{
		// initial find was not successful
		return;
	}

	do
	{
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
		int nStartChar, nEndChar;
		GetEditCtrl().GetSel(nStartChar, nEndChar);
		UINT nBeforeReplaceStart = nStartChar;
		UINT nBeforeReplaceEnd = nEndChar;
#endif	
		
		GetEditCtrl().ReplaceSel(pEditState->strReplace);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
		GetEditCtrl().GetSel(nStartChar, nEndChar);
		UINT nAfterReplaceStart = nStartChar;
		UINT nStartingPos = pEditState->nStartingPos;
		if(bJustStarted && nStartingPos == nAfterReplaceStart && nStartingPos == nBeforeReplaceStart)
			pEditState->bJustStarted = TRUE;

		if(nStartingPos > nBeforeReplaceStart)
			pEditState->nStartingPos += nAfterReplaceStart - nBeforeReplaceEnd;

		if(!bJustStarted && nStartingPos > nBeforeReplaceStart && nStartingPos < nAfterReplaceStart)
			break;
#endif
	} while (FindText(pEditState->strFind, 1, bCase));

	ASSERT_VALID(this);
}

BOOL CEditView::InitializeReplace()
	// helper to do find first if no selection
{
	ASSERT_VALID(this);

	_AFX_EDIT_STATE* pEditState = _afxEditState;

	// do find next if no selection
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	if (nStartChar == nEndChar)
	{
		if (!FindText(pEditState->strFind, pEditState->bNext,
			pEditState->bCase))
		{
			// text not found
			OnTextNotFound(pEditState->strFind);
		}
		return FALSE;
	}

	if (!SameAsSelected(pEditState->strFind, pEditState->bCase))
	{
		if (!FindText(pEditState->strFind, pEditState->bNext,
			pEditState->bCase))
		{
			// text not found
			OnTextNotFound(pEditState->strFind);
		}
		return FALSE;
	}

	ASSERT_VALID(this);
	return TRUE;
}

LRESULT CEditView::OnFindReplaceCmd(WPARAM, LPARAM lParam)
{
	ASSERT_VALID(this);

	_AFX_EDIT_STATE* pEditState = _afxEditState;
	CFindReplaceDialog* pDialog = CFindReplaceDialog::GetNotifier(lParam);
	ASSERT(pDialog != NULL);
	ASSERT(pDialog == pEditState->pFindReplaceDlg);
	if (pDialog->IsTerminating())
	{
		pEditState->pFindReplaceDlg = NULL;
	}
	else if (pDialog->FindNext())
	{
		OnFindNext(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase());
	}
	else if (pDialog->ReplaceCurrent())
	{
		ASSERT(!pEditState->bFindOnly);
		OnReplaceSel(pDialog->GetFindString(),
			pDialog->SearchDown(), pDialog->MatchCase(),
			pDialog->GetReplaceString());
	}
	else if (pDialog->ReplaceAll())
	{
		ASSERT(!pEditState->bFindOnly);
		OnReplaceAll(pDialog->GetFindString(), pDialog->GetReplaceString(),
			pDialog->MatchCase());
	}
	ASSERT_VALID(this);
	return 0;
}

typedef int (WINAPI* AFX_COMPARE_PROC)(LPCTSTR str1, LPCTSTR str2);

BOOL CEditView::SameAsSelected(LPCTSTR lpszCompare, BOOL bCase)
{
	// check length first
	size_t nLen = lstrlen(lpszCompare);
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	if (nLen != (size_t)(nEndChar - nStartChar))
		return FALSE;

	// length is the same, check contents
	CString strSelect;
	GetSelectedText(strSelect);
	return (bCase && lstrcmp(lpszCompare, strSelect) == 0) ||
		(!bCase && lstrcmpi(lpszCompare, strSelect) == 0);
}

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
BOOL CEditView::FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase)
{
	ASSERT_VALID(this);
	ASSERT(lpszFind != NULL);
	ASSERT(*lpszFind != '\0');

	_AFX_EDIT_STATE* pEditState = _afxEditState;
	UINT nStartingPos = pEditState->nStartingPos;
	BOOL bJustStarted = pEditState->bJustStarted;

	UINT nLen = GetBufferLength();
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	UINT nStart = nStartChar;
	int iDir = bNext ? +1 : -1;

	CWaitCursor wait;
	LPCTSTR lpszText = LockBuffer();

	if (iDir < 0)
	{
		if (nStart == nStartingPos && !bJustStarted)
			return FALSE;

		// always go back one for search backwards
		if(_tcsdec(lpszText, lpszText + nStart) != NULL)
			nStart -= (lpszText+nStart) - _tcsdec(lpszText, lpszText+nStart);
		else
			nStart = nLen;
	}
	else if (nStartChar != nEndChar && SameAsSelected(lpszFind, bCase) && !bJustStarted)
	{
		// easy to go backward/forward with SBCS
		if (_istlead(lpszText[nStart]))
			nStart++;
		nStart += iDir;
	}

	// wrap around search completed
	if (nStart == nStartingPos && !bJustStarted)
		return FALSE;

	// handle search with nStart past end of buffer
	size_t nLenFind = lstrlen(lpszFind);

	if (nStart+nLenFind-1 >= nLen && iDir > 0 && nStart >= nStartingPos)
		nStart = 0;


	// start the search at nStart
	LPCTSTR lpsz = lpszText + nStart;
	AFX_COMPARE_PROC pfnCompare = bCase ? lstrcmp : lstrcmpi;

	if (_afxDBCS)
	{
		// double-byte string search
		LPCTSTR lpszStop;
		BOOL bContinue = TRUE;
		BOOL bSearchedEnd = FALSE;
		while(bContinue)
		{
			bContinue = FALSE;

			if(lpsz == lpszText + nStartingPos && !bJustStarted)
			{
				UnlockBuffer();
				return FALSE;
			}
			if (iDir > 0)
			{
				if(lpsz >= lpszText + nStartingPos)
				{
					if(bSearchedEnd)
					{
						UnlockBuffer();
						pEditState->bJustStarted = FALSE;
						return FALSE;
					}
					// start at current and find _first_ occurrance
					lpszStop = lpszText + nLen;

					if(nStartingPos == 0 || nStartingPos == nLen)
						bContinue = FALSE;
					else
						bContinue = TRUE;

					if(lpsz == lpszText + nLen && nStartingPos == nLen)
					{
						lpsz = lpszText;
						lpszStop = lpszText + nLen;
					}

					bSearchedEnd = TRUE;
				}
				else
				{
					if(bSearchedEnd)
					{
						lpsz = lpszText;
						bSearchedEnd = FALSE;
					}

					lpszStop = lpszText + nStartingPos;
					bContinue = FALSE;
				}
			}
			else
			{
				if(lpsz <= lpszText + nStartingPos)
				{
					if(bSearchedEnd)
					{
						UnlockBuffer();
						pEditState->bJustStarted = FALSE;
						return FALSE;
					}
					// start at top and find _last_ occurrance
					lpszStop = lpsz;
					lpsz = lpszText;

					if(nStartingPos == 0 || nStartingPos == nLen)
						bContinue = FALSE;
					else
						bContinue = TRUE;

					if(lpsz == lpszText && nStartingPos == 0)
					{
						lpsz = lpszText + nStartingPos;
						lpszStop = lpszText + nLen; 
					}
				
					bSearchedEnd = TRUE;
				}
				else
				{
					lpszStop = lpsz;
					lpsz = lpszText + nStartingPos;
					if(bSearchedEnd)
					{
						// start at starting pos and find _last_ occurrance
						lpszStop = lpszText + nLen; 
						bSearchedEnd = FALSE;
					}
					bContinue = FALSE;
				}
			}

			LPCTSTR lpszFound = NULL;
			while (lpsz < lpszStop)
			{
				if (!bCase || (*lpsz == *lpszFind &&
					(!_istlead(*lpsz) || lpsz[1] == lpszFind[1])))
				{
					LPTSTR lpch = (LPTSTR)(lpsz + nLenFind);
					TCHAR chSave = *lpch;
					*lpch = '\0';
					int nResult = (*pfnCompare)(lpsz, lpszFind);
					*lpch = chSave;
					if (nResult == 0)
					{
						lpszFound = lpsz;
						if (iDir > 0)
							break;
					}
				}
				lpsz = _tcsinc(lpsz);
				if(lpsz >= lpszStop && bContinue)
					if(iDir > 0)
						lpsz = lpszText;
					else
						lpsz = _tcsinc(lpszText + nStartingPos);
			}

			if (lpszFound != NULL)
			{
				UnlockBuffer();
				int n = (int)(lpszFound - lpszText);
				GetEditCtrl().SetFocus();
				GetEditCtrl().SetSel(n, n+nLenFind);
				pEditState->bJustStarted = FALSE;
				return TRUE;
			}
			
		}
		UnlockBuffer();
	}
	else
	{
		// single-byte string search
		UINT nCompare;
		BOOL bContinue = TRUE;
		BOOL bSearchedEnd = FALSE;
		while(bContinue)
		{
			bContinue = FALSE;
			if(nStart == nStartingPos && !bJustStarted)
			{
				UnlockBuffer();
				return FALSE;
			}
			if (iDir > 0)
			{
				if(nStart >= nStartingPos)
				{
					if(bSearchedEnd)
					{
						UnlockBuffer();
						pEditState->bJustStarted = FALSE;
						return FALSE;
					}
					nCompare = nLen - nStart;

					if(nStartingPos == 0 || nStartingPos == nLen)
						bContinue = FALSE;
					else
						bContinue = TRUE;

					if(nStart == nLen && nStartingPos == nLen)
					{
						lpsz = lpszText;
						nStart = 0;
						nCompare = nLen;
					}

					if(nCompare == 0)
					{
						nStart = 0;
						lpsz = lpszText;
					}
					
					bSearchedEnd = TRUE;
				}
				else
				{
					if(bSearchedEnd)
					{
						nStart = 0;
						lpsz = lpszText;
						bSearchedEnd = FALSE;
					}
					nCompare = nStartingPos - nStart;
					bContinue = FALSE;
				}
			}
			else
			{
				if(nStart <= nStartingPos)
				{
					if(bSearchedEnd)
					{
						UnlockBuffer();
						pEditState->bJustStarted = FALSE;
						return FALSE;
					}
					nCompare = nStart + 1;
					if(nStartingPos == 0 || nStartingPos == nLen)
						bContinue = FALSE;
					else
						bContinue = TRUE;

					if(nStart == 0 && nStartingPos == 0)
					{
						lpsz = lpszText + nLen;
						nStart = nLen;
						nCompare = nLen;
					}

					if(nCompare == 0)
					{
						lpsz = lpszText + nLen; 
						nStart = nLen;
					}
					
					bSearchedEnd = TRUE;
				}
				else
				{
					if(bSearchedEnd)
					{
						// start at bottom and find _last_ occurrance
						lpsz = lpszText + nLen; 
						nStart = nLen;
						bSearchedEnd = FALSE;
					}
					nCompare = nStart - nStartingPos + 1;
					bContinue = FALSE;
				}
			}

			while (nCompare > 0)
			{
				ASSERT(lpsz >= lpszText);

				LPTSTR lpch = (LPTSTR)(lpsz + nLenFind);
				TCHAR chSave = *lpch;
				*lpch = '\0';
				int nResult = (*pfnCompare)(lpsz, lpszFind);
				*lpch = chSave;
				if (nResult == 0)
				{
					UnlockBuffer();
					int n = (int)(lpsz - lpszText);
					GetEditCtrl().SetFocus();
					GetEditCtrl().SetSel(n, n+nLenFind);
					ASSERT_VALID(this);
					pEditState->bJustStarted = FALSE;
					return TRUE;
				}

				// restore character at end of search
				*lpch = chSave;

				// move on to next substring
  				nCompare--;
				lpsz += iDir;
				if(iDir > 0 || nStart >= 1)
					if(nStart + iDir < nLen)
						nStart = nStart +iDir;
					else
						nStart = 0;
				else
					nStart = nLen;
			}
		}
		UnlockBuffer();
	}

	ASSERT_VALID(this);
	pEditState->bJustStarted = FALSE;
	return FALSE;
}

#else // _WIN32_WCE_PSPC
BOOL CEditView::FindText(LPCTSTR lpszFind, BOOL bNext, BOOL bCase)
{
	ASSERT_VALID(this);
	ASSERT(lpszFind != NULL);
	ASSERT(*lpszFind != '\0');

	UINT nLen = GetBufferLength();
	int nStartChar, nEndChar;
	GetEditCtrl().GetSel(nStartChar, nEndChar);
	UINT nStart = nStartChar;
	int iDir = bNext ? +1 : -1;

	// can't find a match before the first character
	if (nStart == 0 && iDir < 0)
		return FALSE;

	CWaitCursor wait;
	LPCTSTR lpszText = LockBuffer();

	if (iDir < 0)
	{
		// always go back one for search backwards
		nStart -= (lpszText+nStart) -
			_tcsdec(lpszText, lpszText+nStart);
	}
	else if (nStartChar != nEndChar && SameAsSelected(lpszFind, bCase))
	{
		// easy to go backward/forward with SBCS
		if (_istlead(lpszText[nStart]))
			nStart++;
		nStart += iDir;
	}

	// handle search with nStart past end of buffer
	size_t nLenFind = lstrlen(lpszFind);
	if (nStart+nLenFind-1 >= nLen)
	{
		if (iDir < 0 && nLen >= nLenFind)
		{
			if (_afxDBCS)
			{
				// walk back to previous character n times
				nStart = nLen;
				int n = nLenFind;
				while (n--)
				{
					nStart -= (lpszText+nStart) -
						_tcsdec(lpszText, lpszText+nStart);
				}
			}
			else
			{
				// single-byte character set is easy and fast
				nStart = nLen - nLenFind;
			}
			ASSERT(nStart+nLenFind-1 <= nLen);
		}
		else
		{
			UnlockBuffer();
			return FALSE;
		}
	}

	// start the search at nStart
	LPCTSTR lpsz = lpszText + nStart;
	AFX_COMPARE_PROC pfnCompare = bCase ? lstrcmp : lstrcmpi;

	if (_afxDBCS)
	{
		// double-byte string search
		LPCTSTR lpszStop;
		if (iDir > 0)
		{
			// start at current and find _first_ occurrance
			lpszStop = lpszText + nLen - nLenFind + 1;
		}
		else
		{
			// start at top and find _last_ occurrance
			lpszStop = lpsz;
			lpsz = lpszText;
		}

		LPCTSTR lpszFound = NULL;
		while (lpsz <= lpszStop)
		{
			if (!bCase || (*lpsz == *lpszFind &&
				(!_istlead(*lpsz) || lpsz[1] == lpszFind[1])))
			{
				LPTSTR lpch = (LPTSTR)(lpsz + nLenFind);
				TCHAR chSave = *lpch;
				*lpch = '\0';
				int nResult = (*pfnCompare)(lpsz, lpszFind);
				*lpch = chSave;
				if (nResult == 0)
				{
					lpszFound = lpsz;
					if (iDir > 0)
						break;
				}
			}
			lpsz = _tcsinc(lpsz);
		}
		UnlockBuffer();

		if (lpszFound != NULL)
		{
			int n = (int)(lpszFound - lpszText);
			GetEditCtrl().SetSel(n, n+nLenFind);
			return TRUE;
		}
	}
	else
	{
		// single-byte string search
		UINT nCompare;
		if (iDir < 0)
			nCompare = (UINT)(lpsz - lpszText) + 1;
		else
			nCompare = nLen - (UINT)(lpsz - lpszText) - nLenFind + 1;

		while (nCompare > 0)
		{
			ASSERT(lpsz >= lpszText);
			ASSERT(lpsz+nLenFind-1 <= lpszText+nLen-1);

			LPTSTR lpch = (LPTSTR)(lpsz + nLenFind);
			TCHAR chSave = *lpch;
			*lpch = '\0';
			int nResult = (*pfnCompare)(lpsz, lpszFind);
			*lpch = chSave;
			if (nResult == 0)
			{
				UnlockBuffer();
				int n = (int)(lpsz - lpszText);
				GetEditCtrl().SetSel(n, n+nLenFind);
				ASSERT_VALID(this);
				return TRUE;
			}

			// restore character at end of search
			*lpch = chSave;

			// move on to next substring
			nCompare--;
			lpsz += iDir;
		}
		UnlockBuffer();
	}

	ASSERT_VALID(this);
	return FALSE;
}
#endif //_WIN32_WCE_PSPC

void CEditView::OnTextNotFound(LPCTSTR)
{
	ASSERT_VALID(this);
	MessageBeep(0);
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (pEditState->pFindReplaceDlg != NULL)
	{
		if (pEditState->pFindReplaceDlg->IsBarVisible())
		{
			AfxMessageBox(AFXCE_IDS_END_SEARCH);
			pEditState->pFindReplaceDlg->SendMessage(WM_CLOSE);
		}
		else
		{
			pEditState->pFindReplaceDlg->m_bShowBar = FALSE;
			CString str;
			if (!str.LoadString(AFXCE_IDS_NOT_FOUND))
				AfxMessageBox(AFXCE_IDS_NOT_FOUND);
			else
				pEditState->pFindReplaceDlg->MessageBox(str);

			pEditState->bJustStarted = TRUE;
		}
	}
#endif //_WIN32_WCE_PSPC
}
#endif // _WIN32_WCE_NO_FINDREPLACE

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
void CEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CCtrlView::OnLButtonUp(nFlags, point);

	_AFX_EDIT_STATE* pEditState = _afxEditState;
	// the search is restarted from current cursor location if the user changes the caret position
	// while the find/replace dlg bar is up
	if (pEditState->pFindReplaceDlg != NULL && pEditState->pFindReplaceDlg->IsBarVisible())
	{
		int nStartChar, nEndChar;
		GetEditCtrl().GetSel(nStartChar, nEndChar);
		pEditState->nStartingPos = nStartChar;
		pEditState->bJustStarted = TRUE;
	}
}

void CEditView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCtrlView::OnKeyUp(nChar, nRepCnt, nFlags);

	_AFX_EDIT_STATE* pEditState = _afxEditState;
	// the search is restarted from current cursor location if the user changes the caret position
	// while the find/replace dlg bar is up
	if (pEditState->pFindReplaceDlg != NULL && pEditState->pFindReplaceDlg->IsBarVisible())
		if(nChar == VK_LEFT || nChar == VK_RIGHT || nChar == VK_UP || nChar == VK_DOWN || nChar == VK_HOME || nChar == VK_END || nChar == VK_DELETE)
		{
			int nStartChar, nEndChar;
			GetEditCtrl().GetSel(nStartChar, nEndChar);
			pEditState->nStartingPos = nStartChar;
			pEditState->bJustStarted = TRUE;
		}
}

void CEditView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CCtrlView::OnChar(nChar, nRepCnt, nFlags);

	_AFX_EDIT_STATE* pEditState = _afxEditState;
	// the search is restarted from current cursor location if the user changes text
	// while the find/replace dlg bar is up
	if (pEditState->pFindReplaceDlg != NULL && pEditState->pFindReplaceDlg->IsBarVisible())
	{
		int nStartChar, nEndChar;
		GetEditCtrl().GetSel(nStartChar, nEndChar);
		pEditState->nStartingPos = nStartChar;
		pEditState->bJustStarted = TRUE;
	}
}

void CEditView::DismissFindReplaceBar()
{
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (pEditState->pFindReplaceDlg != NULL)
		if (pEditState->pFindReplaceDlg->IsBarVisible())
			pEditState->pFindReplaceDlg->SendMessage(WM_CLOSE);
}
#endif //_WIN32_WCE_PSPC
/////////////////////////////////////////////////////////////////////////////
// CEditView Tab Stops

void CEditView::SetTabStops(int nTabStops)
{
	ASSERT_VALID(this);
	m_nTabStops = nTabStops;
	GetEditCtrl().SetTabStops(m_nTabStops);
	Invalidate();
	ASSERT_VALID(this);
}

/////////////////////////////////////////////////////////////////////////////
// CEditView diagnostics

#ifdef _DEBUG
void CEditView::AssertValid() const
{
	CCtrlView::AssertValid();
#if !defined(_WIN32_WCE_NO_PRINTING)
	ASSERT_VALID(&m_aPageStart);
	if (m_hPrinterFont != NULL)
		ASSERT_VALID(CFont::FromHandle(m_hPrinterFont));
	if (m_hMirrorFont != NULL)
		ASSERT_VALID(CFont::FromHandle(m_hMirrorFont));
#endif // _WIN32_WCE_NO_PRINTING
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (pEditState->pFindReplaceDlg != NULL)
		ASSERT_VALID(pEditState->pFindReplaceDlg);
#endif // _WIN32_WCE_NO_FINDREPLACE
}

void CEditView::Dump(CDumpContext& dc) const
{
	CCtrlView::Dump(dc);

	dc << "m_nTabStops = " << m_nTabStops;
#if !defined(_WIN32_WCE_NO_PRINTING)
	if (m_hPrinterFont != NULL)
		dc << "\nm_hPrinterFont " << (UINT)m_hPrinterFont;
	if (m_hMirrorFont != NULL)
		dc << "\nm_hMirrorFont " << (UINT)m_hMirrorFont;
	dc << "\nm_aPageStart: " << &m_aPageStart;
#endif // _WIN32_WCE_NO_PRINTING
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	dc << "\nstatic member data:";
	_AFX_EDIT_STATE* pEditState = _afxEditState;
	if (pEditState->pFindReplaceDlg != NULL)
	{
		dc << "\npFindReplaceDlg = "
			<< (void*)pEditState->pFindReplaceDlg;
		dc << "\nbFindOnly = " << pEditState->bFindOnly;
	}
	dc << "\nstrFind = " << pEditState->strFind;
	dc << "\nstrReplace = " << pEditState->strReplace;
	dc << "\nbCase = " << pEditState->bCase;
	dc << "\nbNext = " << pEditState->bNext;
#endif // _WIN32_WCE_NO_FINDREPLACE

	dc << "\n";
}
#endif //_DEBUG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNCREATE(CEditView, CCtrlView)

#pragma warning(disable: 4074)
#pragma init_seg(lib)

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
PROCESS_LOCAL(_AFX_EDIT_STATE, _afxEditState)
#endif // _WIN32_WCE_NO_FINDREPLACE
/////////////////////////////////////////////////////////////////////////////
