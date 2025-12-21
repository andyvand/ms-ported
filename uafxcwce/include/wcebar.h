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

#ifndef __WCEBAR_H__
#define __WCEBAR_H__

#define CMDBAR_END (WORD)-1

#if !defined(_WIN32_WCE_NO_CONTROLBARS)

class CCeCommandBar : public CToolBar
{
	DECLARE_DYNAMIC(CCeCommandBar)

	// Attributes
public:

// Constructors
public:
	CCeCommandBar();
	virtual ~CCeCommandBar();
	void ResetCommandBar();

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_TOOLBAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		CRect rcBorders = CRect(0, 0, 0, 0),
		UINT nID = AFX_IDW_TOOLBAR);

	CMenu* LoadSHMenuBar(LPCTSTR lpszMenuName);
	CMenu* LoadSHMenuBar(UINT nResourceID)
		{ return LoadSHMenuBar(MAKEINTRESOURCE(nResourceID)); }

	CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	CSize CalcDynamicLayout(int nLength, DWORD nMode);
#endif // _WIN32_WCE_PSPC

// Operations
	CComboBox* InsertComboBox(int nWidth, WORD idComboBox,
                              int nButton = CMDBAR_END, 
	                          DWORD dwStyle = CBS_DROPDOWNLIST | WS_VSCROLL);

	
	CComboBox* InsertComboBox(CComboBox* pComboBox, int nWidth, WORD idComboBox,
                              int nButton = CMDBAR_END, 
	                          DWORD dwStyle = CBS_DROPDOWNLIST | WS_VSCROLL);

	CMenu* InsertMenuBar(UINT nResourceID, int nButton = CMDBAR_END)
		{ return InsertMenuBar(MAKEINTRESOURCE(nResourceID), nButton); }
	CMenu* InsertMenuBar(LPCTSTR lpszMenuName, int nButton = CMDBAR_END);
	CMenu* GetMenuBar(int nButton) const; 
	BOOL   DrawMenuBar(int nButton) const;

	BOOL   AddAdornments(DWORD dwFlags = 0);
	BOOL   InsertSeparator(int nWidth = 6, int nButton = CMDBAR_END);
	void   WrapAtButton(int nButton);

	int    GetNumButtons() const 
		{ return ::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0); }

private:
	void RemoveCustomControls();
	void SetToCustomControlButton(int nButton);
	static BOOL IsCustomControlButton(const TBBUTTON& button);
	static CCeCommandBar* GetDefaultCommandBar(CFrameWnd *, BOOL bAutoCreateMenu = TRUE);

	CObArray m_pComboBoxArray;
	CObArray m_pMenuArray;
	BOOL     m_bAppendBitmaps;
	BOOL     m_bAdornmentsAdded;
	HWND     m_hBCComboBox; // Use for backward compatibility only

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	WORD     m_wNextMenuID;
public:
	BOOL     m_bShowSharedNewButton;

protected:
	//{{AFX_MSG(CCeCommandBar)
	afx_msg virtual void OnSharedNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif // _WIN32_WCE_PSPC

	friend class CFrameWnd; // Used for old command bar function support.
	friend class CToolBar;
};

#endif // _WIN32_WCE_NO_CONTROLBARS

#endif // __WCEBAR_H__
