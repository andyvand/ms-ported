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

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)

BEGIN_MESSAGE_MAP(CCeSaveModifiedDlg, CDialog)
	//{{AFX_MSG_MAP(CCeSaveModifiedDlg)
	ON_COMMAND(IDYES, OnYes)
	ON_COMMAND(IDNO, OnNo)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(ID_FILE_SAVE_AS, OnSaveAs)
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCeSaveModifiedDlg::OnInitDialog()
{
	SetDlgItemText(AFX_IDP_ASK_TO_SAVE, m_strPrompt);	
	SHINITDLGINFO	shidi;
	memset(&shidi, 0, sizeof(SHINITDLGINFO));
	shidi.dwMask = SHIDIM_FLAGS;
	shidi.dwFlags = SHIDIF_SIPDOWN;
	shidi.hDlg = m_hWnd;

	if (!SHInitDialog(&shidi))
		return FALSE;

	SetWindowText(AfxGetAppName());
	return TRUE;
}

void CCeSaveModifiedDlg::OnYes()
{
	EndDialog(IDYES); 
}

void CCeSaveModifiedDlg::OnNo()
{
	EndDialog(IDNO);
}

void CCeSaveModifiedDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}

void CCeSaveModifiedDlg::OnSaveAs()
{
	EndDialog(ID_FILE_SAVE_AS);
}

void CCeSaveModifiedDlg::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	// to avoid having a full screen dialog when handling the sip
	// do not call CDialog::OnSettingChange
	CWnd::OnSettingChange(uFlags, lpszSection);
}
#endif // _WIN32_WCE_PSPC

