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
#if !defined(_WIN32_WCE)
#include <dlgs.h>
#endif // _WIN32_WCE

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

#if defined(_WIN32_WCE)
UINT _afxMsgLBSELCHANGE = 0;
UINT _afxMsgSHAREVI = 0;
UINT _afxMsgFILEOK = 0;
UINT _afxMsgCOLOROK = 0;
UINT _afxMsgHELP = 0;
UINT _afxMsgSETRGB = 0;
#else // _WIN32_WCE
AFX_COMDAT UINT _afxMsgLBSELCHANGE = 0;
AFX_COMDAT UINT _afxMsgSHAREVI = 0;
AFX_COMDAT UINT _afxMsgFILEOK = 0;
AFX_COMDAT UINT _afxMsgCOLOROK = 0;
AFX_COMDAT UINT _afxMsgHELP = 0;
AFX_COMDAT UINT _afxMsgSETRGB = 0;
#endif // _WIN32_WCE

BEGIN_MESSAGE_MAP(CCommonDialog, CDialog)
	//{{AFX_MSG_MAP(CCommonDialog)
	ON_WM_HELPINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

UINT CALLBACK
_AfxCommDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (hWnd == NULL)
		return 0;

	_AFX_THREAD_STATE* pThreadState = _afxThreadState.GetData();
#if defined(_WIN32_WCE)
	if(pThreadState->m_pWndInit != NULL)
	{
		// WinCE: We need to do our MFC hooking stuff now.  Note: since
		// wce_FirstDlgProc repumps the message, we'll get back here, but
		// since wce_FirstDlgProc NULL's pThreadState->m_pWndInit, regular 
		// processing of the message in this function will occur upon reentrancy. 
		// Because *this* call (the first one) doesn't really count, we 
		// return immediately with TRUE.  We assume that the OS-owned dialog
		// procedure that called the hook _AfxCommDlgProc is treating the
		// return value 1 as a "message processed, don't process further."
		// We lose the return value of the "repumped message."
		wce_FirstDlgProc(hWnd, message, wParam, lParam);
		return 1; 
	}
#endif // _WIN32_WCE
	if (pThreadState->m_pAlternateWndInit != NULL)
	{
		ASSERT_KINDOF(CFileDialog,pThreadState->m_pAlternateWndInit);
		pThreadState->m_pAlternateWndInit->SubclassWindow(hWnd);
		pThreadState->m_pAlternateWndInit = NULL;
	}
	ASSERT(pThreadState->m_pAlternateWndInit == NULL);

	if (message == WM_INITDIALOG)
	{
		_afxMsgLBSELCHANGE = ::RegisterWindowMessage(LBSELCHSTRING);
		_afxMsgSHAREVI = ::RegisterWindowMessage(SHAREVISTRING);
		_afxMsgFILEOK = ::RegisterWindowMessage(FILEOKSTRING);
		_afxMsgCOLOROK = ::RegisterWindowMessage(COLOROKSTRING);
		_afxMsgHELP = ::RegisterWindowMessage(HELPMSGSTRING);
		_afxMsgSETRGB = ::RegisterWindowMessage(SETRGBSTRING);
		return (UINT)AfxDlgProc(hWnd, message, wParam, lParam);
	}

	if (message == _afxMsgHELP ||
	   (message == WM_COMMAND && LOWORD(wParam) == pshHelp))
	{
		// just translate the message into the AFX standard help command.
		SendMessage(hWnd, WM_COMMAND, ID_HELP, 0);
		return 1;
	}

	if (message < 0xC000)
	{
		// not a ::RegisterWindowMessage message
		return 0;
	}

	// assume it is already wired up to a permanent one
	CDialog* pDlg = (CDialog*)CWnd::FromHandlePermanent(hWnd);
	ASSERT(pDlg != NULL);
	ASSERT_KINDOF(CDialog, pDlg);

	if (pDlg->IsKindOf(RUNTIME_CLASS(CFileDialog)))
	{
		// If we're exploring then we are not interested in the Registered messages
		if (((CFileDialog*)pDlg)->m_ofn.Flags & OFN_EXPLORER)
			return 0;
	}

	// RegisterWindowMessage - does not copy to lastState buffer, so
	// CWnd::GetCurrentMessage and CWnd::Default will NOT work
	// while in these handlers

	// Dispatch special commdlg messages through our virtual callbacks
	if (message == _afxMsgSHAREVI)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);
		return ((CFileDialog*)pDlg)->OnShareViolation((LPCTSTR)lParam);
	}
	else if (message == _afxMsgFILEOK)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);

		if (afxData.bWin4)
			((CFileDialog*)pDlg)->m_pofnTemp = (OPENFILENAME*)lParam;

		BOOL bResult = ((CFileDialog*)pDlg)->OnFileNameOK();

		((CFileDialog*)pDlg)->m_pofnTemp = NULL;

		return bResult;
	}
	else if (message == _afxMsgLBSELCHANGE)
	{
		ASSERT_KINDOF(CFileDialog, pDlg);
		((CFileDialog*)pDlg)->OnLBSelChangedNotify(wParam, LOWORD(lParam),
				HIWORD(lParam));
		return 0;
	}
#if !defined(_WIN32_WCE_NO_COLORDLG)
	else if (message == _afxMsgCOLOROK)
	{
		ASSERT_KINDOF(CColorDialog, pDlg);
		return ((CColorDialog*)pDlg)->OnColorOK();
	}
	else if (message == _afxMsgSETRGB)
	{
		// nothing to do here, since this is a SendMessage
		return 0;
	}
#endif // _WIN32_WCE_NO_COLORDLG
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// CCommonDialog - common dialog helper class

void CCommonDialog::OnOK()
{
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

#if defined(_WIN32_WCE)
// WinCE: Give the "registered window message" processing a chance
	if (IsKindOf(RUNTIME_CLASS(CFileDialog)))
	{
		if(_AfxCommDlgProc(m_hWnd, _afxMsgFILEOK, 0, (LPARAM)&((CFileDialog*)this)->m_ofn))
			return;
	}
#if !defined(_WIN32_WCE_NO_COLORDLG)
	else if (IsKindOf(RUNTIME_CLASS(CColorDialog)))
	{
		if(_AfxCommDlgProc(m_hWnd, _afxMsgCOLOROK, 0, 0))
			return;
	}
#endif // _WIN32_WCE_NO_COLORDLG	
#endif // _WIN32_WCE
	// Common dialogs do not require ::EndDialog
	Default();
}

void CCommonDialog::OnCancel()
{
	ASSERT_VALID(this);

	// Common dialogs do not require ::EndDialog
	Default();
}

BOOL CCommonDialog::OnHelpInfo(HELPINFO*)
{
	return Default();
}

////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32_WCE)
// WinCE: we want runtime class information so we can use IsKindOf (wcewnd.cpp)
IMPLEMENT_DYNAMIC(CCommonDialog, CDialog)
#endif
