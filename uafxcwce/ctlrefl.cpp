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

#ifdef AFXCTL_CORE2_SEG
#pragma code_seg(AFXCTL_CORE2_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CReflectorWnd

BOOL CReflectorWnd::Create(const CRect& rect, HWND hWndParent)
{
	// make sure the default window class is registered
	VERIFY(AfxDeferRegisterClass(AFX_WNDOLECONTROL_REG));
	return CreateEx(0, AFX_WNDOLECONTROL, NULL,
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top, hWndParent, 0);
}

void CReflectorWnd::PostNcDestroy()
{
	if (m_pCtrl != NULL)
		m_pCtrl->OnReflectorDestroyed();
	delete this;
}

LRESULT CReflectorWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
#if defined(_WIN32_WCE)
// WinCE: Normally, the reflector and control window areas are negotiated
// when the control is created. Pocket Visual Basic, however, resizes the 
// reflector window (the only window it knows about) later on, so we need
// to keep the control window the same size.
	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS* wp = (WINDOWPOS*)lParam;
		if(m_pCtrl != NULL && m_pCtrl->m_hWnd != NULL)
			m_pCtrl->MoveWindow(0, 0, wp->cx, wp->cy);
	}
	break;
#endif // _WIN32_WCE
	case WM_COMMAND:
	case WM_NOTIFY:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
	case WM_DELETEITEM:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_COMPAREITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
WCE_DEL	case WM_PARENTNOTIFY:
		if (m_pCtrl != NULL)
			return m_pCtrl->SendMessage(OCM__BASE + uMsg, wParam, lParam);
		break;

	case WM_SETFOCUS:
		if (m_pCtrl != NULL)
		{
			m_pCtrl->SetFocus();
			return 0;
		}
		break;
	}

	return CWnd::WindowProc(uMsg, wParam, lParam);
}

void CReflectorWnd::SetControl(COleControl* pCtrl)
{
	m_pCtrl = pCtrl;
}

LRESULT CParkingWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hWndSource = NULL;

	switch (uMsg)
	{
	case WM_COMMAND:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORDLG:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
	case WM_HSCROLL:
	case WM_VSCROLL:
		hWndSource = (HWND)lParam;
		break;

	case WM_NOTIFY:
		hWndSource = ((NMHDR*)lParam)->hwndFrom;
		break;

	case WM_DRAWITEM:
		hWndSource = ((DRAWITEMSTRUCT*)lParam)->hwndItem;
		break;

	case WM_MEASUREITEM:
		m_idMap.Lookup((void*)(DWORD)HIWORD(wParam), (void*&)hWndSource);
		break;

	case WM_DELETEITEM:
		hWndSource = ((DELETEITEMSTRUCT*)lParam)->hwndItem;
		break;

	case WM_COMPAREITEM:
		hWndSource = ((COMPAREITEMSTRUCT*)lParam)->hwndItem;
		break;

#if !defined(_WIN32_WCE)
	case WM_PARENTNOTIFY:
		switch (LOWORD(wParam))
		{
		case WM_CREATE:
			m_idMap.SetAt((void*)(DWORD)HIWORD(wParam), (HWND)lParam);
			hWndSource = (HWND)lParam;
			break;

		case WM_DESTROY:
			m_idMap.RemoveKey((void*)(DWORD)HIWORD(wParam));
			hWndSource = (HWND)lParam;
			break;

		default:
			m_idMap.Lookup((void*)(DWORD)HIWORD(wParam), (void*&)hWndSource);
			break;
		}
#endif // _WIN32_WCE
	}

	if (hWndSource != NULL)
		return ::SendMessage(hWndSource, OCM__BASE + uMsg, wParam, lParam);
	else
		return CWnd::WindowProc(uMsg, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// Force any extra compiler-generated code into AFX_INIT_SEG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif
