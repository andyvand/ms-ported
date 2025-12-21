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

#ifndef __AFXEXT_H__
#define __AFXEXT_H__

#ifndef __AFXWIN_H__
	#include <afxwin.h>
#endif
#ifndef __AFXDLGS_H__
	#include <afxdlgs.h>
#endif

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
#ifndef __PROJECT_H__
	#include "projects.h"
#endif
#endif // _WIN32_WCE_PSPC

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// AFXEXT - MFC Advanced Extensions and Advanced Customizable classes

// Classes declared in this file

//CObject
	//CCmdTarget;
		//CWnd
			//CButton
				class CBitmapButton;    // Bitmap button (self-draw)

			class CControlBar;          // control bar
				class CStatusBar;       // status bar
				class CToolBar;         // toolbar
				class CDialogBar;       // dialog as control bar
				class CReBar;			// ie40 dock bar

			class CSplitterWnd;         // splitter manager

			//CView
				//CScrollView
				class CFormView;        // view with a dialog template
				class CEditView;        // simple text editor view

	//CDC
		class CMetaFileDC;              // a metafile with proxy

class CRectTracker;                     // tracker for rectangle objects

// information structures
struct CPrintInfo;          // Printing context
struct CPrintPreviewState;  // Print Preview context/state
struct CCreateContext;      // Creation context

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA

/////////////////////////////////////////////////////////////////////////////
// Simple bitmap button

// CBitmapButton - push-button with 1->4 bitmap images
class CBitmapButton : public CButton
{
	DECLARE_DYNAMIC(CBitmapButton)
public:
// Construction
	CBitmapButton();

	BOOL LoadBitmaps(LPCTSTR lpszBitmapResource,
			LPCTSTR lpszBitmapResourceSel = NULL,
			LPCTSTR lpszBitmapResourceFocus = NULL,
			LPCTSTR lpszBitmapResourceDisabled = NULL);
	BOOL LoadBitmaps(UINT nIDBitmapResource,
			UINT nIDBitmapResourceSel = 0,
			UINT nIDBitmapResourceFocus = 0,
			UINT nIDBitmapResourceDisabled = 0);
	BOOL AutoLoad(UINT nID, CWnd* pParent);

// Operations
	void SizeToContent();

// Implementation:
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	// all bitmaps must be the same size
	CBitmap m_bitmap;           // normal image (REQUIRED)
	CBitmap m_bitmapSel;        // selected image (OPTIONAL)
	CBitmap m_bitmapFocus;      // focused but not selected (OPTIONAL)
	CBitmap m_bitmapDisabled;   // disabled bitmap (OPTIONAL)

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
};

#if !defined(_WIN32_WCE_NO_CONTROLBARS)
/////////////////////////////////////////////////////////////////////////////
// Control Bars

// forward declarations (private to implementation)
class CDockBar;
class CDockContext;
class CControlBarInfo;
struct AFX_SIZEPARENTPARAMS;

// Layout Modes for CalcDynamicLayout
#define LM_STRETCH  0x01    // same meaning as bStretch in CalcFixedLayout.  If set, ignores nLength
							// and returns dimensions based on LM_HORZ state, otherwise LM_HORZ is used
							// to determine if nLength is the desired horizontal or vertical length
							// and dimensions are returned based on nLength
#define LM_HORZ     0x02    // same as bHorz in CalcFixedLayout
#define LM_MRUWIDTH 0x04    // Most Recently Used Dynamic Width
#define LM_HORZDOCK 0x08    // Horizontal Docked Dimensions
#define LM_VERTDOCK 0x10    // Vertical Docked Dimensions
#define LM_LENGTHY  0x20    // Set if nLength is a Height instead of a Width
#define LM_COMMIT   0x40    // Remember MRUWidth

#ifdef _AFXDLL
class CControlBar : public CWnd
#else
class AFX_NOVTABLE CControlBar : public CWnd
#endif
{
	DECLARE_DYNAMIC(CControlBar)
// Construction
protected:
	CControlBar();

// Attributes
public:
	int GetCount() const;

	// for styles specific to CControlBar
	DWORD GetBarStyle();
	void SetBarStyle(DWORD dwStyle);

	BOOL m_bAutoDelete;

	// getting and setting border space
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);
	CRect GetBorders() const;

	CFrameWnd* GetDockingFrame() const;
	BOOL IsFloating() const;
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);

// Operations
#if defined(_WIN32_WCE_NO_DOCKBARS)
// WinCE: Null implementations for forward compatibility
	void EnableDocking(DWORD dwDockStyle) {}
#else // _WIN32_WCE_NO_DOCKBARS
	void EnableDocking(DWORD dwDockStyle);
#endif // _WIN32_WCE_NO_DOCKBARS

// Overridables
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler) = 0;

// Implementation
public:
	virtual ~CControlBar();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void DelayShow(BOOL bShow);
	virtual BOOL IsVisible() const;
	virtual DWORD RecalcDelayShow(AFX_SIZEPARENTPARAMS* lpLayout);

	virtual BOOL IsDockBar() const;
	virtual BOOL DestroyWindow();
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

	// info about bar (for status bar and toolbar)
	int m_cxLeftBorder, m_cxRightBorder;
	int m_cyTopBorder, m_cyBottomBorder;
	int m_cxDefaultGap;         // default gap value
	UINT m_nMRUWidth;   // For dynamic resizing.

	// array of elements
	int m_nCount;
	void* m_pData;        // m_nCount elements - type depends on derived class

	// support for delayed hide/show
	enum StateFlags
		{ delayHide = 1, delayShow = 2, tempHide = 4, statusSet = 8 };
	UINT m_nStateFlags;

	// support for docking
	DWORD m_dwStyle;    // creation style (used for layout)
	DWORD m_dwDockStyle;// indicates how bar can be docked
	CFrameWnd* m_pDockSite; // current dock site, if dockable
#if !defined(_WIN32_WCE_NO_DOCKBARS)
	CDockBar* m_pDockBar;   // current dock bar, if dockable
	CDockContext* m_pDockContext;   // used during dragging
#endif // _WIN32_WCE_NO_DOCKBARS

#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	virtual BOOL PreTranslateMessage(MSG* pMsg)
#endif // _WIN32_WCE_NO_TOOLTIPS
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PostNcDestroy();

	virtual void DoPaint(CDC* pDC);
	void DrawBorders(CDC* pDC, CRect& rect);
WCE_DEL	void DrawGripper(CDC* pDC, const CRect& rect);

	// implementation helpers
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
#else
#if (_WIN32_WCE >= 300) && !defined(_WIN32_WCE_PSPC)
	virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
#endif // _WIN32_WCE
#endif // _WIN32_WCE_NO_TOOLTIPS
	void CalcInsideRect(CRect& rect, BOOL bHorz) const; // adjusts borders etc
	BOOL AllocElements(int nElements, int cbElement);
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	virtual BOOL SetStatusText(int nHit);
	void ResetTimer(UINT nEvent, UINT nTime);
#endif // _WIN32_WCE_NO_TOOLTIPS
	void EraseNonClient();

#if !defined(_WIN32_WCE_NO_DOCKBARS)
	void GetBarInfo(CControlBarInfo* pInfo);
	void SetBarInfo(CControlBarInfo* pInfo, CFrameWnd* pFrameWnd);
#endif // _WIN32_WCE_NO_DOCKBARS

	//{{AFX_MSG(CControlBar)
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	afx_msg void OnTimer(UINT nIDEvent);
#endif // _WIN32_WCE_NO_TOOLTIPS
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
WCE_DEL afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
	afx_msg LRESULT OnSizeParent(WPARAM wParam, LPARAM lParam);
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	afx_msg LRESULT OnHelpHitTest(WPARAM wParam, LPARAM lParam);
#endif // _WIN32_WCE_NO_TOOLTIPS
	afx_msg void OnInitialUpdate();
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);
#if !defined(_WIN32_WCE_NO_DOCKBARS)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt );
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
#endif // _WIN32_WCE_NO_DOCKBARS
WCE_DEL afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT nMsg);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CFrameWnd;
	friend class CDockBar;
};

/////////////////////////////////////////////////////////////////////////////
// CStatusBar control

class CStatusBarCtrl;   // forward reference (see afxcmn.h for definition)
struct AFX_STATUSPANE;  // private to implementation

class CStatusBar : public CControlBar
{
	DECLARE_DYNAMIC(CStatusBar)

// Construction
public:
	CStatusBar();
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = 0,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_STATUS_BAR);
	BOOL SetIndicators(const UINT* lpIDArray, int nIDCount);

// Attributes
public:
	void SetBorders(LPCRECT lpRect);
	void SetBorders(int cxLeft = 0, int cyTop = 0, int cxRight = 0, int cyBottom = 0);

	// standard control bar things
	int CommandToIndex(UINT nIDFind) const;
	UINT GetItemID(int nIndex) const;
	void GetItemRect(int nIndex, LPRECT lpRect) const;

	// specific to CStatusBar
	void GetPaneText(int nIndex, CString& rString) const;
	CString GetPaneText(int nIndex) const;
	BOOL SetPaneText(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate = TRUE);
	void GetPaneInfo(int nIndex, UINT& nID, UINT& nStyle, int& cxWidth) const;
	void SetPaneInfo(int nIndex, UINT nID, UINT nStyle, int cxWidth);
	UINT GetPaneStyle(int nIndex) const;
	void SetPaneStyle(int nIndex, UINT nStyle);

	// for direct access to the underlying common control
	CStatusBarCtrl& GetStatusBarCtrl() const;

// Overridables
	virtual void DrawItem(LPDRAWITEMSTRUCT);

// Implementation
public:
	virtual ~CStatusBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	BOOL PreCreateWindow(CREATESTRUCT& cs);
	BOOL AllocElements(int nElements, int cbElement);
	void CalcInsideRect(CRect& rect, BOOL bHorz) const;
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	void EnableDocking(DWORD dwDockStyle);
#endif
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

protected:
	int m_nMinHeight;

	AFX_STATUSPANE* _GetPanePtr(int nIndex) const;
	void UpdateAllPanes(BOOL bUpdateRects, BOOL bUpdateText);
	virtual BOOL OnChildNotify(UINT message, WPARAM, LPARAM, LRESULT*);

	//{{AFX_MSG(CStatusBar)
WCE_DEL afx_msg UINT OnNcHitTest(CPoint);
WCE_DEL afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
WCE_DEL afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
WCE_DEL afx_msg void OnWindowPosChanging(LPWINDOWPOS);
	afx_msg LRESULT OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMinHeight(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// Styles for status bar panes
#define SBPS_NORMAL     0x0000
#define SBPS_NOBORDERS  SBT_NOBORDERS
#define SBPS_POPOUT     SBT_POPOUT
#define SBPS_OWNERDRAW  SBT_OWNERDRAW
#define SBPS_DISABLED   0x04000000
#define SBPS_STRETCH    0x08000000  // stretch to fill status bar

/////////////////////////////////////////////////////////////////////////////
// CToolBar control

WCE_DEL HBITMAP AFXAPI AfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono = FALSE);

class CToolBarCtrl; // forward reference (see afxcmn.h for definition)
#if defined(_WIN32_WCE)
class CCeCommandBar;
#endif // _WIN32_WCE

class CToolBar : public CControlBar
{
	DECLARE_DYNAMIC(CToolBar)

// Construction
public:
	CToolBar();
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	virtual BOOL Create(CWnd* pParentWnd,
            DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
		UINT nID = AFX_IDW_TOOLBAR);
	virtual BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_BOTTOM,
		CRect rcBorders = CRect(0, 0, 0, 0),
		UINT nID = AFX_IDW_TOOLBAR);
#else // _WIN32_WCE_PSPC
	BOOL Create(CWnd* pParentWnd,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		UINT nID = AFX_IDW_TOOLBAR);
	BOOL CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle = TBSTYLE_FLAT,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		CRect rcBorders = CRect(0, 0, 0, 0),
		UINT nID = AFX_IDW_TOOLBAR);
#endif // _WIN32_WCE_PSPC

	void SetSizes(SIZE sizeButton, SIZE sizeImage);
		// button size should be bigger than image
	void SetHeight(int cyHeight);
		// call after SetSizes, height overrides bitmap size
	BOOL LoadToolBar(LPCTSTR lpszResourceName);
	WCE_IF(virtual BOOL, BOOL) LoadToolBar(UINT nIDResource);
	BOOL LoadBitmap(LPCTSTR lpszResourceName);
	BOOL LoadBitmap(UINT nIDResource);
	BOOL SetBitmap(HBITMAP hbmImageWell);
	BOOL SetButtons(const UINT* lpIDArray, int nIDCount);
		// lpIDArray can be NULL to allocate empty buttons

// Attributes
public:
	// standard control bar things
	int CommandToIndex(UINT nIDFind) const;
	UINT GetItemID(int nIndex) const;
	virtual void GetItemRect(int nIndex, LPRECT lpRect) const;
	UINT GetButtonStyle(int nIndex) const;
	void SetButtonStyle(int nIndex, UINT nStyle);

	// for changing button info
	void GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const;
	void SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage);
	BOOL SetButtonText(int nIndex, LPCTSTR lpszText);
	CString GetButtonText(int nIndex) const;
	void GetButtonText(int nIndex, CString& rString) const;

	// for direct access to the underlying common control
	CToolBarCtrl& GetToolBarCtrl() const;

// Implementation
public:
	virtual ~CToolBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#endif // _WIN32_WCE_NO_TOOLTIPS
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	void SetOwner(CWnd* pOwnerWnd);
	BOOL AddReplaceBitmap(HBITMAP hbmImageWell);
	virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HRSRC m_hRsrcImageWell; // handle to loaded resource for image well
	HINSTANCE m_hInstImageWell; // instance handle to load image well from
	HBITMAP m_hbmImageWell; // contains color mapped button images
	BOOL m_bDelayedButtonLayout; // used to manage when button layout should be done
#if defined(_WIN32_WCE)
	CCeCommandBar* m_pWndCommandBar;
#endif // _WIN32_WCE

	CSize m_sizeImage;  // current image size
	CSize m_sizeButton; // current button size

	CMapStringToPtr* m_pStringMap;  // used as CMapStringToUInt

	// implementation helpers
	void _GetButton(int nIndex, TBBUTTON* pButton) const;
	void _SetButton(int nIndex, TBBUTTON* pButton);
	CSize CalcLayout(DWORD nMode, int nLength = -1);
	CSize CalcSize(TBBUTTON* pData, int nCount);
	int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth);
	void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert = FALSE);
	void Layout(); // called for for delayed button layout

	//{{AFX_MSG(CToolBar)
WCE_DEL	afx_msg UINT OnNcHitTest(CPoint);
WCE_DEL	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
WCE_DEL	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
WCE_DEL afx_msg void OnWindowPosChanging(LPWINDOWPOS);
WCE_DEL afx_msg void OnSysColorChange();
WCE_DEL	afx_msg LRESULT OnSetButtonSize(WPARAM, LPARAM);
WCE_DEL	afx_msg LRESULT OnSetBitmapSize(WPARAM, LPARAM);
WCE_DEL	afx_msg LRESULT OnPreserveZeroBorderHelper(WPARAM, LPARAM);
WCE_DEL	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT OnSetSizeHelper(CSize& size, LPARAM lParam);
};

// Styles for toolbar buttons
#define TBBS_BUTTON     MAKELONG(TBSTYLE_BUTTON, 0) // this entry is button
#define TBBS_SEPARATOR  MAKELONG(TBSTYLE_SEP, 0)    // this entry is a separator
#define TBBS_CHECKBOX   MAKELONG(TBSTYLE_CHECK, 0)  // this is an auto check button
#define TBBS_GROUP      MAKELONG(TBSTYLE_GROUP, 0)  // marks the start of a group
#define TBBS_CHECKGROUP (TBBS_GROUP|TBBS_CHECKBOX)  // normal use of TBBS_GROUP
#define TBBS_DROPDOWN	MAKELONG(TBSTYLE_DROPDOWN, 0) // drop down style
#define TBBS_AUTOSIZE	MAKELONG(TBSTYLE_AUTOSIZE, 0) // autocalc button width
#define TBBS_NOPREFIX	MAKELONG(TBSTYLE_NOPREFIX, 0) // no accel prefix for this button

// styles for display states
#define TBBS_CHECKED    MAKELONG(0, TBSTATE_CHECKED)    // button is checked/down
#define TBBS_PRESSED    MAKELONG(0, TBSTATE_PRESSED)    // button is being depressed
#define TBBS_DISABLED   MAKELONG(0, TBSTATE_ENABLED)    // button is disabled
#define TBBS_INDETERMINATE  MAKELONG(0, TBSTATE_INDETERMINATE)  // third state
#define TBBS_HIDDEN     MAKELONG(0, TBSTATE_HIDDEN) // button is hidden
#define TBBS_WRAPPED    MAKELONG(0, TBSTATE_WRAP)   // button is wrapped at this point
#define TBBS_ELLIPSES	MAKELONG(0, TBSTATE_ELIPSES) 
#define TBBS_MARKED		MAKELONG(0, TBSTATE_MARKED)

////////////////////////////////////////////
// CDialogBar control
// This is a control bar built from a dialog template. It is a modeless
// dialog that delegates all control notifications to the parent window
// of the control bar [the grandparent of the control]

class CDialogBar : public CControlBar
{
	DECLARE_DYNAMIC(CDialogBar)

// Construction
public:
	CDialogBar();
	BOOL Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName,
			UINT nStyle, UINT nID);
	BOOL Create(CWnd* pParentWnd, UINT nIDTemplate,
			UINT nStyle, UINT nID);

// Implementation
public:
	virtual ~CDialogBar();
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	CSize m_sizeDefault;
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#endif

protected:
#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	LPCTSTR m_lpszTemplateName;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);

    //{{AFX_MSG(CDialogBar)
	DECLARE_MESSAGE_MAP()
	//}}AFX_MSG
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif
};

////////////////////////////////////////////
// CReBar control

class CReBarCtrl;

class CReBar : public CControlBar
{
    DECLARE_DYNAMIC(CReBar)

// Construction
public:
	CReBar();
    BOOL Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS,
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM,
#else // _WIN32_WCE_PSPC
        DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP,
#endif // _WIN32_WCE_PSPC
		UINT nID = AFX_IDW_REBAR);

// Attributes
public:
    // for direct access to the underlying common control
    CReBarCtrl& GetReBarCtrl() const;

// Operations
public:
    BOOL AddBar(CWnd* pBar, LPCTSTR pszText = NULL, CBitmap* pbmp = NULL, 
        DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
    BOOL AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack,
        LPCTSTR pszText = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS);
#if defined(_WIN32_WCE)
	BOOL AddAdornments(DWORD dwFlags = 0);
#endif // _WIN32_WCE

// Implementation
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
    virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
#endif // _WIN32_WCE_NO_TOOLTIPS
    virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
    virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
#ifdef _DEBUG
	void EnableDocking(DWORD dwDockStyle);
#endif

protected:
#if !defined(_WIN32_WCE_NO_TOOLTIPS)
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
#endif // _WIN32_WCE_NO_TOOLTIPS
    BOOL _AddBar(CWnd* pBar, REBARBANDINFO* pRBBI);

    //{{AFX_MSG(CReBar)
WCE_DEL afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
    afx_msg void OnPaint();
    afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult);
WCE_DEL afx_msg void OnNcPaint();
WCE_DEL afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnShowBand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRecalcParent();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
#if defined(_WIN32_WCE)
	#include <wcebar.h>
#endif // _WIN32_WCE
#endif // _WIN32_WCE_NO_CONTROLBARS

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
#include <doclist.h>
class CCeDocListCB;

/////////////////////////////////////////////////////////////////////////////
// CCeDocList
#define DLN_CE_CREATE (DLN_START+32)      // notifies parent window that CCeDocList just got created
#define DLN_CE_DESTROY (DLN_START+33)      // notifies parent window that CCeDocList just got destroyed
class CCeDocList : public CWnd
{
	DECLARE_DYNAMIC(CCeDocList)

// Constructors
public:
	CCeDocList();
	CCeDocList(CCeDocListDocTemplate*);
	virtual ~CCeDocList();
	BOOL Create(CWnd* pParentWnd, 
		        LPCTSTR lpszFilterList, 
		        LPCTSTR lpszFolder, 
				DWORD dwFlags = DLF_SHOWEXTENSION);

// Attributes
	CCeCommandBar* GetCommandBar() { return (CCeCommandBar*)m_pWndDocListCB; }

// Operations
	BOOL    Refresh();
	HRESULT Update();
	void	DisableUpdate();
	void	EnableUpdate();
	BOOL	SetSortOrder();
	HRESULT SetFilterIndex(int nIndex);
	int		GetFilterIndex();
	BOOL	GetSelectedPathname(LPTSTR pszPath, WORD nSize);
	void	SetSelectedPathname(LPTSTR pszPath);	
	HRESULT SelectItem(LPTSTR pszPath, BOOL fVisible);	
	void	SetSelection(int nItem);
	HRESULT DeleteSelection();
	HRESULT	SetOneItem(int nItem, PAstruct *pPA);
	void	SetFolder(LPTSTR pszFolder);
	int		GetSelectCount();
	BOOL	GetNextWaveFile(int* pnItem);
	BOOL	GetPrevWaveFile(int* pnItem);
	BOOL	RenameMoveSelectedItems();
	void	SendIR(LPTSTR pszPath);
	void	ReceiveIR(LPTSTR pszPath);
	void	SendEMail(LPTSTR pszPath);
	int		GetItemCount();
	int		GetNextItem(int nIStart, UINT nFlags);
	BOOL	SetItemState(int nItem, UINT nState, UINT nStateMask);

	//{{AFX_MSG(CCeDocList)
	afx_msg void OnItemActivated(DLNHDR* pNotify, LRESULT* result);		// DLN_ITEMACTIVATED
	afx_msg virtual void OnCreate();
	afx_msg virtual void OnClose();
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()

// Implementation
public:
	CCeDocListDocTemplate* m_pDocTemplate; // if a doctemplate created the doclist, here is its pointer

private:
	CString m_strFilterList;	// '|'-delimited list of filters, terminated with '||\0'
	CCeDocListCB* m_pWndDocListCB;
	DOCLISTCREATE m_dlc;		// DocList creation structure
	TCHAR m_szFolder[MAX_PATH];	// contains full path name after return
};
#endif // _WIN32_WCE_PSPC
/////////////////////////////////////////////////////////////////////////////
// Splitter Window

#define SPLS_DYNAMIC_SPLIT  0x0001
#define SPLS_INVERT_TRACKER 0x0002  // obsolete (now ignored)

class CSplitterWnd : public CWnd
{
	DECLARE_DYNAMIC(CSplitterWnd)

// Construction
public:
	CSplitterWnd();
	// Create a single view type splitter with multiple splits
	BOOL Create(CWnd* pParentWnd,
				int nMaxRows, int nMaxCols, SIZE sizeMin,
				CCreateContext* pContext,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE |
					WS_HSCROLL | WS_VSCROLL | SPLS_DYNAMIC_SPLIT,
				UINT nID = AFX_IDW_PANE_FIRST);

	// Create a multiple view type splitter with static layout
	BOOL CreateStatic(CWnd* pParentWnd,
				int nRows, int nCols,
				DWORD dwStyle = WS_CHILD | WS_VISIBLE,
				UINT nID = AFX_IDW_PANE_FIRST);

	virtual BOOL CreateView(int row, int col, CRuntimeClass* pViewClass,
			SIZE sizeInit, CCreateContext* pContext);

// Attributes
public:
	int GetRowCount() const;
	int GetColumnCount() const;

	// information about a specific row or column
	void GetRowInfo(int row, int& cyCur, int& cyMin) const;
	void SetRowInfo(int row, int cyIdeal, int cyMin);
	void GetColumnInfo(int col, int& cxCur, int& cxMin) const;
	void SetColumnInfo(int col, int cxIdeal, int cxMin);

	// for setting and getting shared scroll bar style
	DWORD GetScrollStyle() const;
	void SetScrollStyle(DWORD dwStyle);

	// views inside the splitter
	CWnd* GetPane(int row, int col) const;
	BOOL IsChildPane(CWnd* pWnd, int* pRow, int* pCol);
	BOOL IsChildPane(CWnd* pWnd, int& row, int& col); // obsolete
	int IdFromRowCol(int row, int col) const;

	BOOL IsTracking();  // TRUE during split operation

// Operations
public:
	virtual void RecalcLayout();    // call after changing sizes

// Overridables
protected:
	// to customize the drawing
	enum ESplitType { splitBox, splitBar, splitIntersection, splitBorder };
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
	virtual void OnInvertTracker(const CRect& rect);

public:
	// for customizing scrollbar regions
	virtual BOOL CreateScrollBarCtrl(DWORD dwStyle, UINT nID);

	// for customizing DYNAMIC_SPLIT behavior
	virtual void DeleteView(int row, int col);
	virtual BOOL SplitRow(int cyBefore);
	virtual BOOL SplitColumn(int cxBefore);
	virtual void DeleteRow(int rowDelete);
	virtual void DeleteColumn(int colDelete);

	// determining active pane from focus or active view in frame
	virtual CWnd* GetActivePane(int* pRow = NULL, int* pCol = NULL);
	virtual void SetActivePane(int row, int col, CWnd* pWnd = NULL);
protected:
	CWnd* GetActivePane(int& row, int& col); // obsolete

public:
	// high level command operations - called by default view implementation
	virtual BOOL CanActivateNext(BOOL bPrev = FALSE);
	virtual void ActivateNext(BOOL bPrev = FALSE);
	virtual BOOL DoKeyboardSplit();

	// synchronized scrolling
	virtual BOOL DoScroll(CView* pViewFrom, UINT nScrollCode,
		BOOL bDoScroll = TRUE);
	virtual BOOL DoScrollBy(CView* pViewFrom, CSize sizeScroll,
		BOOL bDoScroll = TRUE);

// Implementation
public:
	virtual ~CSplitterWnd();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// implementation structure
	struct CRowColInfo
	{
		int nMinSize;       // below that try not to show
		int nIdealSize;     // user set size
		// variable depending on the available size layout
		int nCurSize;       // 0 => invisible, -1 => nonexistant
	};

protected:
	// customizable implementation attributes (set by constructor or Create)
	CRuntimeClass* m_pDynamicViewClass;
	int m_nMaxRows, m_nMaxCols;

	// implementation attributes which control layout of the splitter
	int m_cxSplitter, m_cySplitter;         // size of splitter bar
	int m_cxBorderShare, m_cyBorderShare;   // space on either side of splitter
	int m_cxSplitterGap, m_cySplitterGap;   // amount of space between panes
	int m_cxBorder, m_cyBorder;             // borders in client area

	// current state information
	int m_nRows, m_nCols;
	BOOL m_bHasHScroll, m_bHasVScroll;
	CRowColInfo* m_pColInfo;
	CRowColInfo* m_pRowInfo;

	// Tracking info - only valid when 'm_bTracking' is set
	BOOL m_bTracking, m_bTracking2;
	CPoint m_ptTrackOffset;
	CRect m_rectLimit;
	CRect m_rectTracker, m_rectTracker2;
	int m_htTrack;

	// implementation routines
	BOOL CreateCommon(CWnd* pParentWnd, SIZE sizeMin, DWORD dwStyle, UINT nID);
	virtual int HitTest(CPoint pt) const;
	virtual void GetInsideRect(CRect& rect) const;
	virtual void GetHitRect(int ht, CRect& rect);
	virtual void TrackRowSize(int y, int row);
	virtual void TrackColumnSize(int x, int col);
	virtual void DrawAllSplitBars(CDC* pDC, int cxInside, int cyInside);
#if !defined(_WIN32_WCE_NO_CURSOR)
	virtual void SetSplitCursor(int ht);
#endif
	CWnd* GetSizingParent();

	// starting and stopping tracking
	virtual void StartTracking(int ht);
	virtual void StopTracking(BOOL bAccept);

	// special command routing to frame
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	//{{AFX_MSG(CSplitterWnd)
#if !defined(_WIN32_WCE_NO_CURSOR)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
#endif // _WIN32_WCE_NO_CURSOR
	afx_msg void OnMouseMove(UINT nFlags, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnCancelMode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
WCE_DEL	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
WCE_DEL	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDisplayChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFormView - generic view constructed from a dialog template

class CFormView : public CScrollView
{
	DECLARE_DYNAMIC(CFormView)

// Construction
protected:      // must derive your own class
	CFormView(LPCTSTR lpszTemplateName);
	CFormView(UINT nIDTemplate);

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnInitialUpdate();

protected:
	LPCTSTR m_lpszTemplateName;
	CCreateContext* m_pCreateContext;
	HWND m_hWndFocus;   // last window to have focus

	virtual void OnDraw(CDC* pDC);      // default does nothing
	// special case override of child window creation
	virtual BOOL Create(LPCTSTR, LPCTSTR, DWORD,
		const RECT&, CWnd*, UINT, CCreateContext*);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnActivateView(BOOL, CView*, CView*);
	virtual void OnActivateFrame(UINT, CFrameWnd*);
	BOOL SaveFocusControl();    // updates m_hWndFocus

#ifndef _AFX_NO_OCC_SUPPORT
	// data and functions necessary for OLE control containment
	_AFX_OCC_DIALOG_INFO* m_pOccDialogInfo;
	virtual BOOL SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
#endif

	//{{AFX_MSG(CFormView)
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CEditView - simple text editor view

class CEditView : public CCtrlView
{
	DECLARE_DYNCREATE(CEditView)

// Construction
public:
	CEditView();

// Attributes
public:
	static AFX_DATA const DWORD dwStyleDefault;
	// CEdit control access
	CEdit& GetEditCtrl() const;

	// presentation attributes
#if !defined(_WIN32_WCE_NO_PRINTING)
	CFont* GetPrinterFont() const;
	void SetPrinterFont(CFont* pFont);
#endif // _WIN32_WCE_NO_PRINTING
	void SetTabStops(int nTabStops);

	// other attributes
	void GetSelectedText(CString& strResult) const;

	// buffer access
	LPCTSTR LockBuffer() const;
	void UnlockBuffer() const;
	UINT GetBufferLength() const;

// Operations
public:
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	BOOL FindText(LPCTSTR lpszFind, BOOL bNext = TRUE, BOOL bCase = TRUE);
#endif // _WIN32_WCE_NO_FINDREPLACE
	void SerializeRaw(CArchive& ar);
#if !defined(_WIN32_WCE_NO_PRINTING)
	UINT PrintInsideRect(CDC* pDC, RECT& rectLayout, UINT nIndexStart,
		UINT nIndexStop);
#endif // _WIN32_WCE_NO_PRINTING

// Overrideables
protected:
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	virtual void OnFindNext(LPCTSTR lpszFind, BOOL bNext, BOOL bCase);
	virtual void OnReplaceSel(LPCTSTR lpszFind, BOOL bNext, BOOL bCase,
		LPCTSTR lpszReplace);
	virtual void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace,
		BOOL bCase);
	virtual void OnTextNotFound(LPCTSTR lpszFind);
#endif // _WIN32_WCE_NO_FINDREPLACE

// Implementation
public:
	virtual ~CEditView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	void ReadFromArchive(CArchive& ar, UINT nLen);
	void WriteToArchive(CArchive& ar);
#if !defined(_WIN32_WCE_NO_PRINTING)
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
#endif // _WIN32_WCE_NO_PRINTING
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	void DismissFindReplaceBar();
#endif // _WIN32_WCE_PSPC

	static AFX_DATA const UINT nMaxSize;
		// maximum number of characters supported

protected:
	int m_nTabStops;            // tab stops in dialog units
	LPTSTR m_pShadowBuffer;     // special shadow buffer only used in Win32s
	UINT m_nShadowSize;

#if !defined(_WIN32_WCE_NO_PRINTING)
	CUIntArray m_aPageStart;    // array of starting pages
	HFONT m_hPrinterFont;       // if NULL, mirror display font
	HFONT m_hMirrorFont;        // font object used when mirroring
#endif // _WIN32_WCE_NO_PRINTING

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
#if !defined(_WIN32_WCE_NO_PRINTING)
	// printing support
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo = NULL);
	BOOL PaginateTo(CDC* pDC, CPrintInfo* pInfo);
#endif // _WIN32_WCE_NO_PRINTING

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	// find & replace support
	void OnEditFindReplace(BOOL bFindOnly);
	BOOL InitializeReplace();
	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bCase);
#endif // _WIN32_WCE_NO_FINDREPLACE

	// special overrides for implementation
	virtual void CalcWindowRect(LPRECT lpClientRect,
		UINT nAdjustType = adjustBorder);

	//{{AFX_MSG(CEditView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateNeedSel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedClip(CCmdUI* pCmdUI);
	afx_msg void OnUpdateNeedText(CCmdUI* pCmdUI);
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	afx_msg void OnUpdateNeedFind(CCmdUI* pCmdUI);
#endif // _WIN32_WCE_NO_FINDREPLACE
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg BOOL OnEditChange();
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnEditClear();
	afx_msg void OnEditUndo();
	afx_msg void OnEditSelectAll();
#if !defined(_WIN32_WCE_NO_FINDREPLACE)
	afx_msg void OnEditFind();
	afx_msg void OnEditReplace();
	afx_msg void OnEditRepeat();
	afx_msg LRESULT OnFindReplaceCmd(WPARAM wParam, LPARAM lParam);
#endif // _WIN32_WCE_NO_FINDREPLACE
	afx_msg void OnDestroy();
WCE_INS afx_msg LRESULT OnGetDlgCode();
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
#endif // _WIN32_WCE_PSPC
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#if !defined(_WIN32_WCE)
/////////////////////////////////////////////////////////////////////////////
// CMetaFileDC

class CMetaFileDC : public CDC
{
	DECLARE_DYNAMIC(CMetaFileDC)

// Constructors
public:
	CMetaFileDC();
	BOOL Create(LPCTSTR lpszFilename = NULL);
	BOOL CreateEnhanced(CDC* pDCRef, LPCTSTR lpszFileName,
		LPCRECT lpBounds, LPCTSTR lpszDescription);

// Operations
	HMETAFILE Close();
	HENHMETAFILE CloseEnhanced();

// Implementation
public:
	virtual void SetAttribDC(HDC hDC);  // Set the Attribute DC

protected:
	virtual void SetOutputDC(HDC hDC);  // Set the Output DC -- Not allowed
	virtual void ReleaseOutputDC();     // Release the Output DC -- Not allowed

public:
	virtual ~CMetaFileDC();

// Clipping Functions (use the Attribute DC's clip region)
	virtual int GetClipBox(LPRECT lpRect) const;
	virtual BOOL PtVisible(int x, int y) const;
			BOOL PtVisible(POINT point) const;
	virtual BOOL RectVisible(LPCRECT lpRect) const;

// Text Functions
	virtual BOOL TextOut(int x, int y, LPCTSTR lpszString, int nCount);
			BOOL TextOut(int x, int y, const CString& str);
	virtual BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				LPCTSTR lpszString, UINT nCount, LPINT lpDxWidths);
			BOOL ExtTextOut(int x, int y, UINT nOptions, LPCRECT lpRect,
				const CString& str, LPINT lpDxWidths);
	virtual CSize TabbedTextOut(int x, int y, LPCTSTR lpszString, int nCount,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
			CSize TabbedTextOut(int x, int y, const CString& str,
				int nTabPositions, LPINT lpnTabStopPositions, int nTabOrigin);
	virtual int DrawText(LPCTSTR lpszString, int nCount, LPRECT lpRect,
				UINT nFormat);
			int DrawText(const CString& str, LPRECT lpRect, UINT nFormat);

// Printer Escape Functions
	virtual int Escape(int nEscape, int nCount, LPCSTR lpszInData, LPVOID lpOutData);

// Viewport Functions
	virtual CPoint SetViewportOrg(int x, int y);
			CPoint SetViewportOrg(POINT point);
	virtual CPoint OffsetViewportOrg(int nWidth, int nHeight);
	virtual CSize SetViewportExt(int x, int y);
			CSize SetViewportExt(SIZE size);
	virtual CSize ScaleViewportExt(int xNum, int xDenom, int yNum, int yDenom);

protected:
	void AdjustCP(int cx);
};
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
// CRectTracker - simple rectangular tracking rectangle w/resize handles

class CRectTracker
{
public:
// Constructors
	CRectTracker();
	CRectTracker(LPCRECT lpSrcRect, UINT nStyle);

// Style Flags
	enum StyleFlags
	{
		solidLine = 1, dottedLine = 2, hatchedBorder = 4,
		resizeInside = 8, resizeOutside = 16, hatchInside = 32,
	};

// Hit-Test codes
	enum TrackerHit
	{
		hitNothing = -1,
		hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
		hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
	};

// Attributes
	UINT m_nStyle;      // current state
	CRect m_rect;       // current position (always in pixels)
	CSize m_sizeMin;    // minimum X and Y size during track operation
	int m_nHandleSize;  // size of resize handles (default from WIN.INI)

// Operations
	void Draw(CDC* pDC) const;
	void GetTrueRect(LPRECT lpTrueRect) const;
#if !defined(_WIN32_WCE_NO_CURSOR)
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
#endif // _WIN32_WCE_NO_CURSOR
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert = FALSE,
		CWnd* pWndClipTo = NULL);
	BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bAllowInvert = TRUE);
	int HitTest(CPoint point) const;
	int NormalizeHit(int nHandle) const;

// Overridables
	virtual void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo,
		CDC* pDC, CWnd* pWnd);
	virtual void AdjustRect(int nHandle, LPRECT lpRect);
	virtual void OnChangedRect(const CRect& rectOld);
	virtual UINT GetHandleMask() const;

// Implementation
public:
	virtual ~CRectTracker();

protected:
	BOOL m_bAllowInvert;    // flag passed to Track or TrackRubberBand
	CRect m_rectLast;
	CSize m_sizeLast;
	BOOL m_bErase;          // TRUE if DrawTrackerRect is called for erasing
	BOOL m_bFinalErase;     // TRUE if DragTrackerRect called for final erase

	// implementation helpers
	int HitTestHandles(CPoint point) const;
	void GetHandleRect(int nHandle, CRect* pHandleRect) const;
	void GetModifyPointers(int nHandle, int**ppx, int**ppy, int* px, int*py);
	virtual int GetHandleSize(LPCRECT lpRect = NULL) const;
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point, CWnd* pWndClipTo);
	void Construct();
};

/////////////////////////////////////////////////////////////////////////////
// Informational data structures

#if !defined(_WIN32_WCE_NO_PRINTING)
struct CPrintInfo // Printing information structure
{
	CPrintInfo();
	~CPrintInfo();

	CPrintDialog* m_pPD;     // pointer to print dialog

WCE_DEL	BOOL m_bDocObject;       // TRUE if printing by IPrint interface
WCE_DEL	BOOL m_bPreview;         // TRUE if in preview mode
WCE_DEL	BOOL m_bDirect;          // TRUE if bypassing Print Dialog
	BOOL m_bContinuePrinting;// set to FALSE to prematurely end printing
	UINT m_nCurPage;         // Current page
WCE_DEL	UINT m_nNumPreviewPages; // Desired number of preview pages
	CString m_strPageDesc;   // Format string for page number display
	LPVOID m_lpUserData;     // pointer to user created struct
	CRect m_rectDraw;        // rectangle defining current usable page area

	// these only valid if m_bDocObject
WCE_DEL	UINT m_nOffsetPage;      // offset of first page in combined IPrint job
WCE_DEL	DWORD m_dwFlags;         // flags passed to IPrint::Print

	void SetMinPage(UINT nMinPage);
	void SetMaxPage(UINT nMaxPage);
	UINT GetMinPage() const;
	UINT GetMaxPage() const;
	UINT GetFromPage() const;
	UINT GetToPage() const;
	UINT GetOffsetPage() const;
};
#endif // _WIN32_WCE_NO_PRINTING

#if !defined(_WIN32_WCE)
struct CPrintPreviewState   // Print Preview context/state
{
	UINT nIDMainPane;          // main pane ID to hide
	HMENU hMenu;               // saved hMenu
	DWORD dwStates;            // Control Bar Visible states (bit map)
	CView* pViewActiveOld;     // save old active view during preview
	BOOL (CALLBACK* lpfnCloseProc)(CFrameWnd* pFrameWnd);
	HACCEL hAccelTable;       // saved accelerator table

// Implementation
	CPrintPreviewState();
};
#endif // _WIN32_WCE

struct CCreateContext   // Creation information structure
	// All fields are optional and may be NULL
{
	// for creating new views
	CRuntimeClass* m_pNewViewClass; // runtime class of view to create or NULL
	CDocument* m_pCurrentDoc;

	// for creating MDI children (CMDIChildWnd::LoadFrame)
	CDocTemplate* m_pNewDocTemplate;

	// for sharing view/frame state from the original view/frame
	CView* m_pLastView;
	CFrameWnd* m_pCurrentFrame;

// Implementation
	CCreateContext();
};

/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#ifdef _AFX_ENABLE_INLINES
#define _AFXEXT_INLINE AFX_INLINE
#include <afxext.inl>
#endif

#undef AFX_DATA
#define AFX_DATA

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, off)
#endif

#endif //__AFXEXT_H__

/////////////////////////////////////////////////////////////////////////////
