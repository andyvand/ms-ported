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

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Basic Help support (for backward compatibility to MFC 2.0)

void CWinApp::OnHelp()  // use context to derive help context
{
	if (m_dwPromptContext != 0)
	{
		// do not call WinHelp when the error is failing to lauch help
		if (m_dwPromptContext != HID_BASE_PROMPT+AFX_IDP_FAILED_TO_LAUNCH_HELP)
			WinHelp(m_dwPromptContext);
		return;
	}

	// otherwise, use CWnd::OnHelp implementation
	CWnd* pWnd = AfxGetMainWnd();
	ASSERT_VALID(pWnd);
	if (!pWnd->IsFrameWnd())
		pWnd->OnHelp();
	else
		((CFrameWnd*)pWnd)->OnHelp();
}

#if !defined(_WIN32_WCE)
void CWinApp::OnHelpIndex()
{
	WinHelp(0L, HELP_INDEX);
}

void CWinApp::OnHelpFinder()
{
	WinHelp(0L, HELP_FINDER);
}

void CWinApp::OnHelpUsing()
{
	WinHelp(0L, HELP_HELPONHELP);
}

/////////////////////////////////////////////////////////////////////////////
// Context Help Mode support (backward compatibility to MFC 2.0)

void CWinApp::OnContextHelp()
{
	// just use CFrameWnd::OnContextHelp implementation
	m_bHelpMode = HELP_ACTIVE;
	CFrameWnd* pMainWnd = (CFrameWnd*)AfxGetMainWnd();
	ASSERT_VALID(pMainWnd);
	ASSERT_KINDOF(CFrameWnd, pMainWnd);
	pMainWnd->OnContextHelp();
	m_bHelpMode = pMainWnd->m_bHelpMode;
	pMainWnd->PostMessage(WM_KICKIDLE); // trigger idle update
}
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
