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
#if !defined(_WIN32_WCE_NO_FINDREPLACE)

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
class CFindReplaceBar : public CDialogBar
{
// Construction
public:
	CFindReplaceBar(CFindReplaceDialog* pDlg);

// Attributes
public:

// Operations
public:
//virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );
virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindReplaceBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFindReplaceBar();

protected:
	CFindReplaceDialog* m_pFindReplaceDlg;
// Generated message map functions
protected:
	//{{AFX_MSG(CFindReplaceBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnFind();
	afx_msg void OnOK();
	afx_msg void OnReplace();
	afx_msg void OnReplaceAll();
	DECLARE_MESSAGE_MAP()
};
#endif // _WIN32_WCE_PSPC
/////////////////////////////////////////////////////////////////////////////
// CFindReplaceDialog message handlers

BEGIN_MESSAGE_MAP(CFindReplaceDialog, CCommonDialog)
	//{{AFX_MSG_MAP(CFindReplaceDialog)
	ON_CONTROL(BN_CLICKED, ID_BUTTON1, OnReplace)
	ON_CONTROL(BN_CLICKED, ID_BUTTON2, OnReplaceAll)
	ON_CONTROL(BN_CLICKED, ID_BUTTON_HELP, OnHelp)
	ON_CONTROL(EN_CHANGE,  ID_EDIT1, OnFindTextChanged)
	ON_WM_CLOSE()
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	ON_CONTROL(BN_CLICKED, ID_BUTTON_FIND, OnFind)
	ON_MESSAGE( WM_FINDREPLACEBAR, OnFindReplaceBar )
	ON_WM_SETTINGCHANGE()
#endif // _WIN32_WCE_PSPC
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFindReplaceDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindReplaceDialog)
	//}}AFX_DATA_MAP
}

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
void CFindReplaceDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	if(IsWindowVisible())
		CDialog::OnSettingChange(uFlags, lpszSection);
	else
	{
		CWnd* pTopLevelParentWnd = (CWnd::FromHandle(m_fr.hwndOwner))->GetTopLevelParent();
		ASSERT(pTopLevelParentWnd != NULL);
		ASSERT(::IsWindow(pTopLevelParentWnd->m_hWnd));
		::SendMessage(pTopLevelParentWnd->m_hWnd, WM_SETTINGCHANGE, uFlags, (LPARAM)lpszSection);
	}
}

void CFindReplaceDialog::OnFind() 
{
	// Check this is a valid call
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_FINDNEXT;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat, 
		m_fr.wFindWhatLen);

	// If find/replace dialog, get 'Replace with' value
	if (m_nIDHelp == AFX_IDD_REPLACE)
		GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

	// Get 'Direction' state
	m_fr.Flags |= FR_DOWN;
	if (m_nIDHelp == AFX_IDD_FIND)
	{
		if (IsDlgButtonChecked(ID_RADIO1))
			m_fr.Flags &= ~FR_DOWN; // "up" direction (for Find dialog only)
	}

	// create the find/replace bar if CFindReplaceDialog still exists when returning from parent window
	m_bShowBar = TRUE;
	CWnd* pBtnWnd = GetDlgItem(ID_BUTTON1);
	if(pBtnWnd == NULL)
		PostMessage(WM_FINDREPLACEBAR, _AFXCE_FINDBAR);
	else
		if(pBtnWnd->IsWindowVisible() && pBtnWnd->IsWindowEnabled())
			PostMessage(WM_FINDREPLACEBAR);
		else
			PostMessage(WM_FINDREPLACEBAR, _AFXCE_FINDBAR);

	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);
}

BOOL CFindReplaceDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pWndEmptyCB = new CCeCommandBar;
	if(m_pWndEmptyCB != NULL)
		((CCeCommandBar*)m_pWndEmptyCB)->CreateEx(this);

	return TRUE;
}

void CFindReplaceDialog::OnOK() 
{
	if(!GetDlgItem(ID_BUTTON_FIND)->IsWindowEnabled())
	{
		CFindReplaceDialog::OnClose();
		return;
	}

	if (m_nIDHelp == AFX_IDD_FIND)	
		CFindReplaceDialog::OnFind();
	else
		CFindReplaceDialog::OnReplace();
}

LRESULT CFindReplaceDialog::OnFindReplaceBar(WPARAM wParam, LPARAM lParam)
{
 	if(m_pFindReplaceBar == NULL && m_bShowBar)
	{
		m_pFindReplaceBar = new CFindReplaceBar(this);
		if(m_pFindReplaceBar == NULL)
		{
			CFindReplaceDialog::OnClose();
			return 0;
		}
		ShowWindow(FALSE);
		m_pFindReplaceBar->m_bAutoDelete = TRUE;

		if(m_pWndEmptyCB != NULL)
		{
			m_pWndEmptyCB->DestroyWindow();
			delete m_pWndEmptyCB;
			m_pWndEmptyCB = NULL;
		}
		
		CWnd* pTopLevelParentWnd = (CWnd::FromHandle(m_fr.hwndOwner))->GetTopLevelParent();
		ASSERT(pTopLevelParentWnd != NULL);
			
		m_pFindReplaceBar->Create(pTopLevelParentWnd, _T("WCE_FINDREPLACEBAR"), CBRS_BOTTOM, 0xE821);
		
		if(pTopLevelParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
		{
			((CFrameWnd*)pTopLevelParentWnd)->RecalcLayout(FALSE);
			CView* pView = ((CFrameWnd*)pTopLevelParentWnd)->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CEditView)))
			{
				CEdit &edit = ((CEditView*)pView)->GetEditCtrl();
				int nStartChar, nEndChar;
				edit.GetSel(nStartChar, nEndChar);
				edit.SetSel(nStartChar, nEndChar);
			}
		}
		else
		{
			CRect rect;
			pTopLevelParentWnd->GetClientRect(&rect);
			CRect barRect;
			m_pFindReplaceBar->GetWindowRect(&barRect);
			rect.top = rect.bottom - barRect.bottom + barRect.top; 
			m_pFindReplaceBar->MoveWindow(rect, TRUE);
			m_pFindReplaceBar->ShowWindow(SW_SHOW);
			m_pFindReplaceBar->UpdateWindow();
		}

		if(wParam == _AFXCE_FINDBAR)
		{
			(m_pFindReplaceBar->GetDlgItem(ID_BUTTON1))->EnableWindow(FALSE);
			(m_pFindReplaceBar->GetDlgItem(ID_BUTTON2))->EnableWindow(FALSE);
		}
		else
		{
			(m_pFindReplaceBar->GetDlgItem(ID_BUTTON1))->EnableWindow(TRUE);
			(m_pFindReplaceBar->GetDlgItem(ID_BUTTON2))->EnableWindow(TRUE);
		}
		pTopLevelParentWnd->SetFocus();

		return 1;
	}

	return 0;
}
#else // _WIN32_WCE_PSPC
void CFindReplaceDialog::OnOK() 
{
	// Check this is a valid call
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_FINDNEXT;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat, 
		m_fr.wFindWhatLen);

	// If find/replace dialog, get 'Replace with' value
	if (m_nIDHelp == AFX_IDD_REPLACE)
		GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

	// Get 'Direction' state
	m_fr.Flags |= FR_DOWN;
	if (m_nIDHelp == AFX_IDD_FIND)
	{
		if (IsDlgButtonChecked(ID_RADIO1))
			m_fr.Flags &= ~FR_DOWN; // "up" direction (for Find dialog only)
	}

	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);
}
#endif // _WIN32_WCE_PSPC

void CFindReplaceDialog::OnClose() 
{
	ASSERT_VALID(this);

	// Do not modify the flags, except to add
	// FR_DIALOGTERM
	m_fr.Flags |= FR_DIALOGTERM;

	// Delete other flags which may be left from
	// the last button press
	if ((m_fr.Flags & FR_FINDNEXT) == FR_FINDNEXT)
		m_fr.Flags &= ~FR_FINDNEXT;
	if ((m_fr.Flags & FR_REPLACE) == FR_REPLACE)
		m_fr.Flags &= ~FR_REPLACE;
	if ((m_fr.Flags & FR_REPLACEALL) == FR_REPLACEALL)
		m_fr.Flags &= ~FR_REPLACEALL;

	// Notify parent window of user input to find replace dialog
	// NOTE: This activates CEditView::OnFindReplaceCmd(), which
	//       in turn calls CFindReplaceDialog::GetNotifier().
	//       
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	if(m_pWndEmptyCB != NULL)
	{
		m_pWndEmptyCB->DestroyWindow();
		delete m_pWndEmptyCB;
		m_pWndEmptyCB = NULL;
	}

	if(m_pFindReplaceBar)
	{
		m_pFindReplaceBar->ShowWindow(FALSE);
		::SetFocus(m_fr.hwndOwner);
		CFrameWnd* pFrameWnd = m_pFindReplaceBar->GetTopLevelFrame();
		if(pFrameWnd != NULL)
			pFrameWnd->RecalcLayout(TRUE); 
		m_pFindReplaceBar->DestroyWindow();
		m_pFindReplaceBar = NULL;
	}
#endif // _WIN32_WCE_PSPC

	EndDialog(IDCANCEL);
}

void CFindReplaceDialog::OnCancel() 
{
	CFindReplaceDialog::OnClose();
}

void CFindReplaceDialog::EndDialog(int nEndID)
{
	ASSERT_VALID(this);

	m_nModalResult = nEndID;
	// For modeless, call DestroyWindow().
	// NOTE: For modal, we would have called PostMessage(WM_COMMAND, IDCANCEL);
	DestroyWindow();
}


void CFindReplaceDialog::OnReplace()
{
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed (psh1).\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_REPLACE | FR_DOWN;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat,
		m_fr.wFindWhatLen);

	// Get 'Replace with' value
	GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	// create the find/replace bar if CFindReplaceDialog still exists when returning from parent window
	m_bShowBar = TRUE;
	PostMessage(WM_FINDREPLACEBAR);
#endif // _WIN32_WCE_PSPC

	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);
}

void CFindReplaceDialog::OnReplaceAll()
{
	ASSERT_VALID(this);

	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed (psh1).\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	// Prepare m_fr for parent notification
	m_fr.Flags = FR_ENABLEHOOK | FR_REPLACEALL | FR_DOWN;

	// Get 'Find what' value
	GetDlgItem(ID_EDIT1)->GetWindowText(m_fr.lpstrFindWhat,
		m_fr.wFindWhatLen);

	// Get 'Replace with' value
	GetDlgItem(ID_EDIT2)->GetWindowText(m_fr.lpstrReplaceWith,
		m_fr.wReplaceWithLen);

	// Get state of 'Match whole word' check box
	if (((CButton*)GetDlgItem(ID_CHECK1))->GetCheck() > 0)
		m_fr.Flags |= FR_WHOLEWORD;

	// Get state of 'Match case' check box
	if (((CButton*)GetDlgItem(ID_CHECK2))->GetCheck() > 0)
		m_fr.Flags |= FR_MATCHCASE;

	// Notify parent window of user input to find replace dialog
	::SendMessage(m_fr.hwndOwner, RegisterWindowMessage(FINDMSGSTRING),
		0, (LPARAM)&m_fr);

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	// close the find/replace dialog
	// first check if it was not already destroyed because the replace failed
	if(::IsWindow(m_hWnd))
		CFindReplaceDialog::OnClose();
#endif // _WIN32_WCE_PSPC
}

void CFindReplaceDialog::OnHelp()
{
	SendMessage(m_nIDHelp);
}

void CFindReplaceDialog::OnFindTextChanged()
{
	CString strFind;

	GetDlgItemText(ID_EDIT1, strFind);
	BOOL bEnable = !strFind.IsEmpty();
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	GetDlgItem(ID_BUTTON_FIND)->EnableWindow(bEnable);
#else // _WIN32_WCE_PSPC
	GetDlgItem(IDOK)->EnableWindow(bEnable);
#endif // _WIN32_WCE_PSPC
	if (m_nIDHelp != AFX_IDD_FIND)
	{
		GetDlgItem(ID_BUTTON1)->EnableWindow(bEnable);
		GetDlgItem(ID_BUTTON2)->EnableWindow(bEnable);
	}
}


HWND AFXAPI wce_FindText(LPFINDREPLACE lpfr)
{
	// Create a modeless dialog.
	HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_FINDDLG"), RT_DIALOG );
	if(!hInst)
		return (NULL);
	HWND hwnd = ::CreateDialog(hInst, _T("WCE_FINDDLG"), lpfr->hwndOwner, 
		            (DLGPROC)lpfr->lpfnHook );

	// Check it has been created successfully
	ASSERT(hwnd != NULL);

	// Set the value for the 'Find what' edit box
	if (SetWindowText(
		GetDlgItem(hwnd, ID_EDIT1),	// Handle of ID_EDIT1
		lpfr->lpstrFindWhat))
		TRACE0("Setting ID_EDIT1 successful\n");

	// Hide/Show 'Whole word only' check box
	if (lpfr->Flags & FR_HIDEWHOLEWORD) 
		ShowWindow(GetDlgItem(hwnd, ID_CHECK1), SW_HIDE);
	else
	{
		// Disable/Enable 'Whole word only' check box
		if (lpfr->Flags & FR_NOWHOLEWORD) 
			EnableWindow(GetDlgItem(hwnd, ID_CHECK1), FALSE);
		else
			ShowWindow(GetDlgItem(hwnd, ID_CHECK1), SW_SHOW);
	}

	// Hide/Show 'Match case' check box
	if (lpfr->Flags & FR_HIDEMATCHCASE) 
		ShowWindow(GetDlgItem(hwnd, ID_CHECK2), SW_HIDE);
	else
	{
		// Disable/Enable 'Match case' check box
		if (lpfr->Flags & FR_NOMATCHCASE) 
			EnableWindow(GetDlgItem(hwnd, ID_CHECK2), FALSE);
		else
			ShowWindow(GetDlgItem(hwnd, ID_CHECK2), SW_SHOW);
	}

	// Check Up/Down radio button
	if (lpfr->Flags & FR_DOWN) 
	{
		SendMessage(GetDlgItem(hwnd, ID_RADIO1), BM_SETCHECK, 0, 0);
		SendMessage(GetDlgItem(hwnd, ID_RADIO2), BM_SETCHECK, 1, 0);
	}
	else
	{
		SendMessage(GetDlgItem(hwnd, ID_RADIO1), BM_SETCHECK, 1, 0);
		SendMessage(GetDlgItem(hwnd, ID_RADIO2), BM_SETCHECK, 0, 0);
	}

	// Hide/Show 'Direction' radio buttons
	if (lpfr->Flags & FR_HIDEUPDOWN) 
	{
		ShowWindow(GetDlgItem(hwnd, ID_GROUP1), SW_HIDE);
		ShowWindow(GetDlgItem(hwnd, ID_RADIO1), SW_HIDE);
		ShowWindow(GetDlgItem(hwnd, ID_RADIO2), SW_HIDE);
	}
	else
	{
		// Disable/Enable 'Direction radio buttons'
		if (lpfr->Flags & FR_NOUPDOWN) 
		{
			EnableWindow(GetDlgItem(hwnd, ID_GROUP1), FALSE);
			EnableWindow(GetDlgItem(hwnd, ID_RADIO1), FALSE);
			EnableWindow(GetDlgItem(hwnd, ID_RADIO2), FALSE);
		}
		else
		{
			ShowWindow(GetDlgItem(hwnd, ID_GROUP1), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_RADIO1), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_RADIO2), SW_SHOW);
		}
	}

	// Show/Hide Help button
	if (lpfr->Flags & FR_SHOWHELP) 
		ShowWindow(GetDlgItem(hwnd, ID_BUTTON_HELP), SW_SHOW);
	else
		ShowWindow(GetDlgItem(hwnd, ID_BUTTON_HELP), SW_HIDE);
	
	SetFocus(GetDlgItem(hwnd, ID_EDIT1));

	// Show find dialog
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}


HWND AFXAPI wce_ReplaceText(LPFINDREPLACE lpfr)
{
	// Create a modeless dialog.
	HINSTANCE hInst = AfxFindResourceHandle( _T("WCE_REPLACEDLG"), RT_DIALOG );
	if(!hInst)
		return (NULL);
	HWND hWnd = ::CreateDialog(hInst, _T("WCE_REPLACEDLG"), lpfr->hwndOwner, 
		            (DLGPROC)lpfr->lpfnHook );
	
	// Check it has been created successfully
	ASSERT(hWnd != NULL);

	// Set the value for the 'Find what' edit box
	if (SetWindowText(
		GetDlgItem(hWnd, ID_EDIT1),	// Handle of ID_EDIT1
		lpfr->lpstrFindWhat))
		TRACE0("Setting ID_EDIT1 successful\n");

	// Set the value for the 'Replace with' edit box
	if (SetWindowText(
		GetDlgItem(hWnd, ID_EDIT2),	// Handle of ID_EDIT2
		lpfr->lpstrReplaceWith))
		TRACE0("Setting ID_EDIT2 successful\n");

	// Hide/Show 'Whole word only' check box
	if ((lpfr->Flags & FR_HIDEWHOLEWORD) == FR_HIDEWHOLEWORD)
		ShowWindow(GetDlgItem(hWnd, ID_CHECK1), SW_HIDE);
	else
	{
		// Disable/Enable 'Whole word only' check box
		if ((lpfr->Flags & FR_NOWHOLEWORD) == FR_NOWHOLEWORD)
			EnableWindow(GetDlgItem(hWnd, ID_CHECK1), FALSE);
		else
			ShowWindow(GetDlgItem(hWnd, ID_CHECK1), SW_SHOW);
	}

	// Hide/Show 'Match case' check box
	if ((lpfr->Flags & FR_HIDEMATCHCASE) == FR_HIDEMATCHCASE)
		ShowWindow(GetDlgItem(hWnd, ID_CHECK2), SW_HIDE);
	else
	{
		// Disable/Enable 'Match case' check box
		if ((lpfr->Flags & FR_NOMATCHCASE) == FR_NOMATCHCASE)
			EnableWindow(GetDlgItem(hWnd, ID_CHECK2), FALSE);
		else
			ShowWindow(GetDlgItem(hWnd, ID_CHECK2), SW_SHOW);
	}

	// Show/Hide Help button
	if ((lpfr->Flags & FR_SHOWHELP) == FR_SHOWHELP)
		ShowWindow(GetDlgItem(hWnd, ID_BUTTON_HELP), SW_SHOW);
	else
		ShowWindow(GetDlgItem(hWnd, ID_BUTTON_HELP), SW_HIDE);

	// Show find/replace dialog
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
/////////////////////////////////////////////////////////////////////////////
// CFindReplaceBar

CFindReplaceBar::CFindReplaceBar(CFindReplaceDialog* pDlg)
{
	m_pFindReplaceDlg = pDlg;
}

CFindReplaceBar::~CFindReplaceBar()
{
}


BEGIN_MESSAGE_MAP(CFindReplaceBar, CDialogBar)
	//{{AFX_MSG_MAP(CFindReplaceBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_BUTTON_FIND, OnFind)
	ON_COMMAND(ID_BUTTON1, OnReplace)
	ON_COMMAND(ID_BUTTON2, OnReplaceAll)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFindReplaceBar message handlers
void CFindReplaceBar::OnFind() 
{
	m_pFindReplaceDlg->OnFind();
}

void CFindReplaceBar::OnOK() 
{
	m_pFindReplaceDlg->OnClose(); //rely on this one to do the work!!!
}

void CFindReplaceBar::OnReplace() 
{
	m_pFindReplaceDlg->OnReplace();	
}

void CFindReplaceBar::OnReplaceAll() 
{
	m_pFindReplaceDlg->OnReplaceAll();
}

void CFindReplaceBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, FALSE);
}

#endif // _WIN32_WCE_PSPC

/////////////////////////////////////////////////////////////////////////////
#endif // _WIN32_WCE_NO_FINDREPLACE
