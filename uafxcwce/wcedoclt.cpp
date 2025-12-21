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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

class CCeDocListCB : public CCeCommandBar
{
	DECLARE_DYNAMIC(CCeDocListCB)

public:
	CCeDocListCB() {};
	virtual ~CCeDocListCB() {};

	CCeDocList* m_pWndDocList;
	virtual BOOL OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo );


protected:
	//{{AFX_MSG(CCeCommandBar)
	afx_msg void OnSharedNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////
// CCeDocList
CCeDocList::CCeDocList() 
{
	m_pWndDocListCB = NULL;
	m_pDocTemplate = NULL;
}

CCeDocList::CCeDocList(CCeDocListDocTemplate* pDocTemplate) 
{
	m_pWndDocListCB = NULL;
	m_pDocTemplate = pDocTemplate;
}

CCeDocList::~CCeDocList() 
{ 
	if (::IsWindow(m_hWnd))
		OnClose();
}

BEGIN_MESSAGE_MAP(CCeDocList, CWnd)
    //{{AFX_MSG_MAP(CCeDocList)
    ON_NOTIFY_REFLECT(DLN_ITEMACTIVATED, OnItemActivated)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCeDocList::Create(CWnd* pParentWnd, LPCTSTR lpszFilterList, LPCTSTR lpszFolder, DWORD dwFlags)
{
	ASSERT(m_hWnd == NULL);

	memset(&m_dlc, 0, sizeof(m_dlc)); // initialize structure to 0
	m_dlc.dwStructSize = sizeof(m_dlc);
	m_dlc.pszFolder    = m_szFolder;
	m_dlc.wFilterIndex = 1;
	m_dlc.dwFlags      = dwFlags;
	m_dlc.wId          = AFXCE_ID_DOCLIST;

	m_szFolder[0] = '\0';

	// setup initial folder name
	if (lpszFolder != NULL) 
	{
		lstrcpyn(m_szFolder, lpszFolder, _countof(m_szFolder));
		m_dlc.pszFolder = m_szFolder;
	}

	// Translate filter list into commdlg format (lots of \0)
	if (lpszFilterList != NULL)
	{
		m_strFilterList = CString(TEXT("All Files|*.*|")) + CString(lpszFilterList);
		LPTSTR pch = m_strFilterList.GetBuffer(0); // modify the buffer in place
		// MFC delimits with '|' not '\0'
		while ((pch = _tcschr(pch, '|')) != NULL)
			*pch++ = '\0';
		m_dlc.pstrFilter = m_strFilterList;
		// do not call ReleaseBuffer() since the string contains '\0' characters
	}

	m_dlc.hwndParent = pParentWnd->GetSafeHwnd();
	if (m_dlc.hwndParent == NULL)
	{
#ifdef _DEBUG
		TRACE0("Warning: DocList creation failed: needs a parent window.");
#endif
		return FALSE;
	}

	HWND hWndDL = ::DocList_Create(&m_dlc);
	if (hWndDL == NULL)
	{
#ifdef _DEBUG
		TRACE1("Warning: DocList creation failed: GetLastError returns 0x%8.8X\n",
			GetLastError());
#endif
		return FALSE;
	}

	
	VERIFY(Attach(hWndDL)); 
	//VERIFY(SubclassWindow(hWndDL));

	OnCreate(); // the doclist control doesn't like to be subclassed, so we call it manually

	return TRUE;
}

BOOL CCeDocList::Refresh()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_REFRESH, 0, 0L);
	return TRUE;
}

HRESULT CCeDocList::Update()
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, DLM_UPDATE, 0, 0L);
}

void CCeDocList::DisableUpdate()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_DISABLEUPDATES, 0, 0L);
}

void CCeDocList::EnableUpdate()
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_ENABLEUPDATES, 0, 0L);
}

BOOL CCeDocList::SetSortOrder()
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, DLM_SETSORTORDER, 0, 0L);
}

HRESULT CCeDocList::SetFilterIndex(int nIndex)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, DLM_SETFILTERINDEX, (WPARAM)nIndex, 0L);
}

int CCeDocList::GetFilterIndex()
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, DLM_GETFILTERINDEX, 0, 0L);
}

BOOL CCeDocList::GetSelectedPathname(LPTSTR pszPath, WORD nSize)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, DLM_GETSELPATHNAME, (WPARAM) nSize, (LPARAM) pszPath);
}

void CCeDocList::SetSelectedPathname(LPTSTR pszPath)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_SETSELPATHNAME, 0, (LPARAM) pszPath);
}

HRESULT CCeDocList::SelectItem(LPTSTR pszPath, BOOL fVisible)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, DLM_SELECTITEM, (WPARAM) fVisible, (LPARAM) pszPath);
}

void CCeDocList::SetSelection(int wItem)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_SETSELECT, (WPARAM) wItem, 0L);
}

HRESULT CCeDocList::DeleteSelection() 
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, DLM_DELETESEL, 0, 0L);
}

HRESULT CCeDocList::SetOneItem(int nItem, PAstruct *pPA)
{
	ASSERT(::IsWindow(m_hWnd));
	return ::SendMessage(m_hWnd, DLM_SETONEITEM, (WPARAM) nItem, (LPARAM) pPA);
}

void CCeDocList::SetFolder(LPTSTR pszFolder)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_SETFOLDER, 0, (LPARAM) pszFolder);
}

int CCeDocList::GetSelectCount()
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, DLM_GETSELCOUNT, 0, 0L);
}

BOOL CCeDocList::GetNextWaveFile(int *pnItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, DLM_GETNEXTWAVE, 0, (LPARAM) pnItem);
}

BOOL CCeDocList::GetPrevWaveFile(int *pnItem)
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, DLM_GETPREVWAVE, 0, (LPARAM) pnItem);
}

BOOL CCeDocList::RenameMoveSelectedItems()
{
	ASSERT(::IsWindow(m_hWnd));
	return (BOOL)::SendMessage(m_hWnd, DLM_RENAMEMOVE, 0, 0);
}

void CCeDocList::SendIR(LPTSTR pszPath)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_SENDIR, 0, (LPARAM) pszPath);
}

void CCeDocList::ReceiveIR(LPTSTR pszPath)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_RECEIVEIR, 0, (LPARAM) pszPath);
}

void CCeDocList::SendEMail(LPTSTR pszPath)
{
	ASSERT(::IsWindow(m_hWnd));
	::SendMessage(m_hWnd, DLM_SENDEMAIL, 0, (LPARAM) pszPath);
}

int CCeDocList::GetItemCount()
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, DLM_GETITEMCOUNT, 0, 0L);
}

int CCeDocList::GetNextItem(int nStart, UINT nFlags)
{
	ASSERT(::IsWindow(m_hWnd));
	return (int)::SendMessage(m_hWnd, DLM_GETNEXTITEM, (WPARAM) nStart, MAKELPARAM(nFlags, 0));
}

BOOL CCeDocList::SetItemState(int nItem, UINT nState, UINT nStateMask)
{
	ASSERT(::IsWindow(m_hWnd));
	LV_ITEM _ms_lvi;
	_ms_lvi.stateMask = nStateMask;
	_ms_lvi.state = nState;
	return (BOOL)::SendMessage(m_hWnd, DLM_SETITEMSTATE, (WPARAM)nItem, 
		(LPARAM)(LV_ITEM FAR *) &_ms_lvi);
}

void CCeDocList::OnCreate()
{
	SetDlgCtrlID(AFXCE_ID_DOCLIST);
	m_pWndDocListCB = new CCeDocListCB;
	if(m_pWndDocListCB != NULL)
	{
		m_pWndDocListCB->m_pWndDocList = this;
		m_pWndDocListCB->m_bShowSharedNewButton = TRUE;
		VERIFY(m_pWndDocListCB->CreateEx(this));
		CWnd* pOwner = GetParent();
		if(pOwner != NULL)
		{
			DLNHDR dlhmhdr = {{m_hWnd, m_dlc.wId, DLN_CE_CREATE}, 0, NULL};
			ASSERT(::IsWindow(pOwner->m_hWnd));
			::SendMessage(pOwner->m_hWnd, WM_NOTIFY, DLN_CE_CREATE, (LPARAM)&dlhmhdr);
		}
	}
}

void CCeDocList::OnClose()
{
	if(m_pWndDocListCB != NULL)
	{
		m_pWndDocListCB->DestroyWindow();
		delete m_pWndDocListCB;
		m_pWndDocListCB = NULL;
	}

	CWnd* pOwner = GetParent();
	if(pOwner != NULL)
	{
		DLNHDR dlhmhdr = {{m_hWnd, m_dlc.wId, DLN_CE_DESTROY}, 0, NULL};
		ASSERT(::IsWindow(pOwner->m_hWnd));
		::SendMessage(pOwner->m_hWnd, WM_NOTIFY, DLN_CE_DESTROY, (LPARAM)&dlhmhdr);
	}

	::DestroyWindow(Detach());
}


BEGIN_MESSAGE_MAP(CCeDocListCB, CCeCommandBar)
	//{{AFX_MSG_MAP(CNotepadApp)
	ON_COMMAND(IDM_SHAREDNEW, CCeDocListCB::OnSharedNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCeDocListCB::OnSharedNew()
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp!=NULL);

	CWnd *pWndApp = pApp->GetMainWnd();
	ASSERT(::IsWindow(pWndApp->m_hWnd));
	pWndApp->PostMessage(WM_COMMAND, ID_FILE_NEW);

	m_pWndDocList->OnClose();
}

/* Temporary workaround for non-working subclassing*/
BOOL CCeDocListCB::OnCmdMsg( UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo )
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp!=NULL);

	CWnd *pWndApp = pApp->GetMainWnd();
	ASSERT(::IsWindow(pWndApp->m_hWnd));

	if (nCode != IDM_SHAREDNEW && pWndApp->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// If the object(s) in the extended command route don't handle
	// the command, then let the base class OnCmdMsg handle it.
	return CCeCommandBar::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CCeDocList::OnItemActivated(DLNHDR* pNotify, LRESULT* result)
{
	CWinApp* pApp = AfxGetApp();
	ASSERT(pApp != NULL);

	if(pApp->OpenDocumentFile(pNotify->pszPath) != NULL)
	{
		// Only dismiss the DocList if the open was successful
		OnClose();
	}
}

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(CCeDocList, CWnd)
IMPLEMENT_DYNAMIC(CCeDocListCB, CCeCommandBar)

#endif // _WIN32_WCE_PSPC

