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
#ifndef __AFXDLGS_H__
#define __AFXDLGS_H__

#ifndef __AFXWIN_H__
	#include <afxwin.h>
#endif

#ifndef _INC_COMMDLG
	#include <commdlg.h>    // common dialog APIs
#endif

// Avoid mapping GetFileTitle to GetFileTitle[A/W]
#ifdef GetFileTitle
#undef GetFileTitle
AFX_INLINE short APIENTRY GetFileTitle(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf)
#ifdef UNICODE
	{ return ::WCE_FCTN(GetFileTitleW)(lpszFile, lpszTitle, cbBuf); }
#else
	{ return ::GetFileTitleA(lpszFile, lpszTitle, cbBuf); }
#endif
#endif

#ifndef _AFX_NO_RICHEDIT_SUPPORT
	#ifndef _RICHEDIT_
		#include <richedit.h>
	#endif
#endif

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, on)
#endif

#ifndef _AFX_NOFORCE_LIBS

/////////////////////////////////////////////////////////////////////////////
// Win32 libraries

#endif //!_AFX_NOFORCE_LIBS

/////////////////////////////////////////////////////////////////////////////

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

/////////////////////////////////////////////////////////////////////////////
// AFXDLGS - MFC Standard dialogs

// Classes declared in this file

	// CDialog
		class CCommonDialog;  // implementation base class

			// modeless dialogs
			class CFindReplaceDialog; // Find/FindReplace dialog

			// modal dialogs
			class CFileDialog;    // FileOpen/FileSaveAs dialogs
			class CColorDialog;   // Color picker dialog
			class CFontDialog;    // Font chooser dialog
			class CPrintDialog;   // Print/PrintSetup dialogs
			class CPageSetupDialog; // Page Setup dialog

	// CWnd
	class CPropertySheet;     // implements tabbed dialogs
		class CPropertySheetEx;

	// CDialog
		class CPropertyPage;  // Used with CPropertySheet for tabbed dialogs
			class CPropertyPageEx;

/////////////////////////////////////////////////////////////////////////////

#undef AFX_DATA
#define AFX_DATA AFX_CORE_DATA

/////////////////////////////////////////////////////////////////////////////
// CCommonDialog - base class for all common dialogs

#ifdef _AFXDLL
class CCommonDialog : public CDialog
#else
class AFX_NOVTABLE CCommonDialog : public CDialog
#endif
{
WCE_INS DECLARE_DYNAMIC(CCommonDialog) // WinCE: need this for IsKindOf
public:
	CCommonDialog(CWnd* pParentWnd);

// Implementation
protected:
	virtual void OnOK();
	virtual void OnCancel();

	//{{AFX_MSG(CCommonDialog)
	afx_msg BOOL OnHelpInfo(HELPINFO*);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CFileDialog - used for FileOpen... or FileSaveAs...

class CFileDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFileDialog)

public:
// Attributes
	OPENFILENAME m_ofn; // open file parameter block

// Constructors
	CFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();

	// Helpers for parsing file name after successful return
	// or during Overridable callbacks if OFN_EXPLORER is set
	CString GetPathName() const;  // return full path and filename
	CString GetFileName() const;  // return only filename
	CString GetFileExt() const;   // return only ext
	CString GetFileTitle() const; // return file title
	BOOL GetReadOnlyPref() const; // return TRUE if readonly checked

	// Enumerating multiple file selections
	POSITION GetStartPosition() const;
	CString GetNextPathName(POSITION& pos) const;

	// Helpers for custom templates
	void SetTemplate(UINT nWin3ID, UINT nWin4ID);
	void SetTemplate(LPCTSTR lpWin3ID, LPCTSTR lpWin4ID);

	// Other operations available while the dialog is visible
	CString GetFolderPath() const; // return full path
	void SetControlText(int nID, LPCSTR lpsz);
	void HideControl(int nID);
	void SetDefExt(LPCSTR lpsz);

// Overridable callbacks
protected:
	friend UINT CALLBACK _AfxCommDlgProc(HWND, UINT, WPARAM, LPARAM);
	virtual UINT OnShareViolation(LPCTSTR lpszPathName);
	virtual BOOL OnFileNameOK();
	virtual void OnLBSelChangedNotify(UINT nIDBox, UINT iCurSel, UINT nCode);

	// only called back if OFN_EXPLORER is set
	virtual void OnInitDone();
	virtual void OnFileNameChange();
	virtual void OnFolderChange();
	virtual void OnTypeChange();

// Implementation
#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL m_bOpenFileDialog;       // TRUE for file open, FALSE for file save
	CString m_strFilter;          // filter string
						// separate fields with '|', terminate with '||\0'
	TCHAR m_szFileTitle[64];       // contains file title after return
	TCHAR m_szFileName[_MAX_PATH]; // contains full path name after return

	OPENFILENAME*  m_pofnTemp;

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};

#if !defined(_WIN32_WCE)
/////////////////////////////////////////////////////////////////////////////
// CFontDialog - used to select a font

class CFontDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFontDialog)

public:
// Attributes
	// font choosing parameter block
	CHOOSEFONT m_cf;

// Constructors
	CFontDialog(LPLOGFONT lplfInitial = NULL,
		DWORD dwFlags = CF_EFFECTS | CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	CFontDialog(const CHARFORMAT& charformat,
		DWORD dwFlags = CF_SCREENFONTS,
		CDC* pdcPrinter = NULL,
		CWnd* pParentWnd = NULL);
#endif
// Operations
	virtual int DoModal();

	// Get the selected font (works during DoModal displayed or after)
	void GetCurrentFont(LPLOGFONT lplf);

	// Helpers for parsing information after successful return
	CString GetFaceName() const;  // return the face name of the font
	CString GetStyleName() const; // return the style name of the font
	int GetSize() const;          // return the pt size of the font
	COLORREF GetColor() const;    // return the color of the font
	int GetWeight() const;        // return the chosen font weight
	BOOL IsStrikeOut() const;     // return TRUE if strikeout
	BOOL IsUnderline() const;     // return TRUE if underline
	BOOL IsBold() const;          // return TRUE if bold font
	BOOL IsItalic() const;        // return TRUE if italic font
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	void GetCharFormat(CHARFORMAT& cf) const;
#endif

// Implementation
	LOGFONT m_lf; // default LOGFONT to store the info
#ifndef _AFX_NO_RICHEDIT_SUPPORT
	DWORD FillInLogFont(const CHARFORMAT& cf);
#endif

#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	TCHAR m_szStyleName[64]; // contains style name after return
};
#endif // _WIN32_WCE

#if !defined(_WIN32_WCE_NO_COLORDLG)
/////////////////////////////////////////////////////////////////////////////
// CColorDialog - used to select a color

class CColorDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CColorDialog)

public:
// Attributes
	// color chooser parameter block
	CHOOSECOLOR m_cc;

// Constructors
	CColorDialog(COLORREF clrInit = 0, DWORD dwFlags = 0,
			CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();

	// Set the current color while dialog is displayed
WCE_DEL void SetCurrentColor(COLORREF clr);

	// Helpers for parsing information after successful return
	COLORREF GetColor() const;
	static COLORREF* PASCAL GetSavedCustomColors();

// Overridable callbacks
protected:
	friend UINT CALLBACK _AfxCommDlgProc(HWND, UINT, WPARAM, LPARAM);
	virtual BOOL OnColorOK();       // validate color

// Implementation

#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

#ifndef _AFX_NO_GRAYDLG_SUPPORT
protected:
	//{{AFX_MSG(CColorDialog)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif //!_AFX_NO_GRAYDLG_SUPPORT
};

// for backward compatibility clrSavedCustom is defined as GetSavedCustomColors
#define clrSavedCustom GetSavedCustomColors()
#endif // _WIN32_WCE_NO_COLORDLG

#if !defined(_WIN32_WCE)
/////////////////////////////////////////////////////////////////////////////
// Page Setup dialog

class CPageSetupDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CPageSetupDialog)

public:
// Attributes
	PAGESETUPDLG m_psd;

// Constructors
	CPageSetupDialog(DWORD dwFlags = PSD_MARGINS | PSD_INWININIINTLMEASURE,
		CWnd* pParentWnd = NULL);

// Attributes
	LPDEVMODE GetDevMode() const;   // return DEVMODE
	CString GetDriverName() const;  // return driver name
	CString GetDeviceName() const;  // return device name
	CString GetPortName() const;    // return output port name
	HDC CreatePrinterDC();
	CSize GetPaperSize() const;
	void GetMargins(LPRECT lpRectMargins, LPRECT lpRectMinMargins) const;

// Operations
	virtual int DoModal();

// Overridables
	virtual UINT PreDrawPage(WORD wPaper, WORD wFlags, LPPAGESETUPDLG pPSD);
	virtual UINT OnDrawPage(CDC* pDC, UINT nMessage, LPRECT lpRect);

// Implementation
protected:
	static UINT CALLBACK PaintHookProc(HWND hWnd, UINT message, WPARAM wParam,
		LPARAM lParam);

#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif
};
#endif // _WIN32_WCE

#if !defined(_WIN32_WCE_NO_PRINTING)
/////////////////////////////////////////////////////////////////////////////
// CPrintDialog - used for Print... and PrintSetup...

class CPrintDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CPrintDialog)

public:
// Attributes
	// print dialog parameter block (note this is a reference)
	PRINTDLG& m_pd;
// WinCE: the following values replace the original PRINTDLG members
WCE_INS WORD m_nFromPage;
WCE_INS WORD m_nToPage;
WCE_INS WORD m_nMinPage;
WCE_INS WORD m_nMaxPage;
WCE_INS WORD m_nCopies; 

// Constructors
	CPrintDialog(BOOL bPrintSetupOnly,
		// TRUE for Print Setup, FALSE for Print Dialog
#if defined(_WIN32_WCE)
		DWORD dwFlags = 0,
#else // _WIN32_WCE
		DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS
			| PD_HIDEPRINTTOFILE | PD_NOSELECTION,
#endif // _WIN32_WCE
		CWnd* pParentWnd = NULL);

// Operations
	virtual int DoModal();

	// GetDefaults will not display a dialog but will get
	// device defaults
	BOOL GetDefaults();

	// Helpers for parsing information after successful return
	int GetCopies() const;          // num. copies requested
WCE_DEL BOOL PrintCollate() const;      // TRUE if collate checked
	BOOL PrintSelection() const;    // TRUE if printing selection
	BOOL PrintAll() const;          // TRUE if printing all pages
WCE_DEL BOOL PrintRange() const;        // TRUE if printing page range
WCE_DEL int GetFromPage() const;        // starting page if valid
WCE_DEL int GetToPage() const;          // starting page if valid
WCE_DEL LPDEVMODE GetDevMode() const;   // return DEVMODE
WCE_DEL CString GetDriverName() const;  // return driver name
	CString GetDeviceName() const;  // return device name
	CString GetPortName() const;    // return output port name
	HDC GetPrinterDC() const;       // return HDC (caller must delete)
WCE_INS CSize GetPaperSize() const;
WCE_INS void GetMargins(LPRECT lpRectMargins, LPRECT lpRectMinMargins) const;
WCE_INS BOOL PrintDraft() const;

	// This helper creates a DC based on the DEVNAMES and DEVMODE structures.
	// This DC is returned, but also stored in m_pd.hDC as though it had been
	// returned by CommDlg.  It is assumed that any previously obtained DC
	// has been/will be deleted by the user.  This may be
	// used without ever invoking the print/print setup dialogs.

WCE_DEL HDC CreatePrinterDC();

// Implementation

#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	PRINTDLG m_pdActual; // the Print/Print Setup need to share this
protected:
	// The following handle the case of print setup... from the print dialog
	CPrintDialog(PRINTDLG& pdInit);
	virtual CPrintDialog* AttachOnSetup();

	//{{AFX_MSG(CPrintDialog)
WCE_DEL afx_msg void OnPrintSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // _WIN32_WCE_NO_PRINTING

#if !defined(_WIN32_WCE_NO_FINDREPLACE)
/////////////////////////////////////////////////////////////////////////////
// Find/FindReplace modeless dialogs
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
#define WM_FINDREPLACEBAR       (WM_USER + 700)
#define _AFXCE_FINDBAR			0x1
class CFindReplaceBar;
#endif // _WIN32_WCE_PSPC
class CFindReplaceDialog : public CCommonDialog
{
	DECLARE_DYNAMIC(CFindReplaceDialog)

public:
// Attributes
	FINDREPLACE m_fr;
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	BOOL m_bShowBar; // user sets it to TRUE to get the FindReplace bar/to FALSE not to get it
					 // set to TRUE by default

	BOOL IsBarVisible(){return (m_pFindReplaceBar != NULL) ? TRUE : FALSE;}// TRUE if the find/replace bar
																		   // is displayed. FALSE otherwise
#endif // _WIN32_WCE_PSPC

// Constructors
	CFindReplaceDialog();
	// Note: you must allocate these on the heap.
	//  If you do not, you must derive and override PostNcDestroy()

	BOOL Create(BOOL bFindDialogOnly, // TRUE for Find, FALSE for FindReplace
			LPCTSTR lpszFindWhat,
			LPCTSTR lpszReplaceWith = NULL,
			DWORD dwFlags = FR_DOWN,
			CWnd* pParentWnd = NULL);

	// find/replace parameter block
	static CFindReplaceDialog* PASCAL GetNotifier(LPARAM lParam);

// Operations
	// Helpers for parsing information after successful return
	CString GetReplaceString() const;// get replacement string
	CString GetFindString() const;   // get find string
	BOOL SearchDown() const;         // TRUE if search down, FALSE is up
	BOOL FindNext() const;           // TRUE if command is find next
	BOOL MatchCase() const;          // TRUE if matching case
	BOOL MatchWholeWord() const;     // TRUE if matching whole words only
	BOOL ReplaceCurrent() const;     // TRUE if replacing current string
	BOOL ReplaceAll() const;         // TRUE if replacing all occurrences
	BOOL IsTerminating() const;      // TRUE if terminating dialog

// Implementation
protected:
	virtual void PostNcDestroy();

#ifdef _DEBUG
public:
	virtual void Dump(CDumpContext& dc) const;
#endif

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
	virtual BOOL OnInitDialog();
#endif // _WIN32_WCE_PSPC

protected:
	TCHAR m_szFindWhat[128];
	TCHAR m_szReplaceWith[128];
#if defined(_WIN32_WCE)
public:
	void EndDialog(int nEndID);
protected:
	// for dialog data exchange and validation
	virtual void DoDataExchange(CDataExchange* pDX);

	// Generated message map functions
	//{{AFX_MSG(CFindReplaceDialog)
	afx_msg void OnOK() ;
	afx_msg void OnClose() ;
	afx_msg void OnCancel() ;
	afx_msg void OnReplace();
	afx_msg void OnReplaceAll();
	afx_msg void OnHelp();
	afx_msg void OnFindTextChanged();
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	afx_msg void OnFind();
	afx_msg LRESULT OnFindReplaceBar(WPARAM, LPARAM);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
#endif // _WIN32_WCE_PSPC
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
#endif // _WIN32_WCE
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300) 
	friend class CFindReplaceBar;
	CFindReplaceBar* m_pFindReplaceBar;
#endif // _WIN32_WCE_PSPC
};
#endif // _WIN32_WCE_NO_FINDREPLACE

////////////////////////////////////////////////////////////////////////////
// CPropertyPage -- one page of a tabbed dialog

#if defined(_WIN32_WCE)
#define AFX_OLDPROPSHEETPAGE PROPSHEETPAGE
#define AFX_OLDPROPSHEETHEADER PROPSHEETHEADER
#else // _WIN32_WCE
// MFC needs to use the original Win95 version of the PROPSHEETPAGE structure.

typedef struct _AFX_OLDPROPSHEETPAGE {
        DWORD           dwSize;
        DWORD           dwFlags;
        HINSTANCE       hInstance;
        union {
            LPCTSTR          pszTemplate;
#ifdef _WIN32
            LPCDLGTEMPLATE  pResource;
#else
            const VOID FAR *pResource;
#endif
        } DUMMYUNIONNAME;
        union {
            HICON       hIcon;
            LPCSTR      pszIcon;
        } DUMMYUNIONNAME2;
        LPCTSTR          pszTitle;
        DLGPROC         pfnDlgProc;
        LPARAM          lParam;
        LPFNPSPCALLBACK pfnCallback;
        UINT FAR * pcRefParent;
} AFX_OLDPROPSHEETPAGE;

// same goes for PROPSHEETHEADER

typedef struct _AFX_OLDPROPSHEETHEADER {
        DWORD           dwSize;
        DWORD           dwFlags;
        HWND            hwndParent;
        HINSTANCE       hInstance;
        union {
            HICON       hIcon;
            LPCTSTR     pszIcon;
        }DUMMYUNIONNAME;
        LPCTSTR         pszCaption;

        UINT            nPages;
        union {
            UINT        nStartPage;
            LPCTSTR     pStartPage;
        }DUMMYUNIONNAME2;
        union {
            LPCPROPSHEETPAGE ppsp;
            HPROPSHEETPAGE FAR *phpage;
        }DUMMYUNIONNAME3;
        PFNPROPSHEETCALLBACK pfnCallback;
} AFX_OLDPROPSHEETHEADER;
#endif // _WIN32_WCE

class CPropertyPage : public CDialog
{
	DECLARE_DYNAMIC(CPropertyPage)

// Construction
public:
	CPropertyPage();
	CPropertyPage(UINT nIDTemplate, UINT nIDCaption = 0);
	CPropertyPage(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);
	void Construct(UINT nIDTemplate, UINT nIDCaption = 0);
	void Construct(LPCTSTR lpszTemplateName, UINT nIDCaption = 0);

// Attributes
	AFX_OLDPROPSHEETPAGE m_psp;

// Operations
	void CancelToClose();
	void SetModified(BOOL bChanged = TRUE);
	LRESULT QuerySiblings(WPARAM wParam, LPARAM lParam);

// Overridables
public:
	virtual BOOL OnApply();
	virtual void OnReset();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnSetActive();
	virtual BOOL OnKillActive();
	virtual BOOL OnQueryCancel();

WCE_DEL	virtual LRESULT OnWizardBack();
WCE_DEL	virtual LRESULT OnWizardNext();
WCE_DEL	virtual BOOL OnWizardFinish();

// Implementation
public:
	virtual ~CPropertyPage();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void EndDialog(int nEndID); // called for error scenarios

protected:
	// private implementation data
	CString m_strCaption;
	BOOL m_bFirstSetActive;

	// implementation helpers
	void CommonConstruct(LPCTSTR lpszTemplateName, UINT nIDCaption);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG*);
WCE_DEL	LRESULT MapWizardResult(LRESULT lToMap);
	BOOL IsButtonEnabled(int iButton);

#if !defined(_WIN32_WCE) || (defined(_WIN32_WCE) && (_WIN32_WCE >= 300))
void PreProcessPageTemplate(PROPSHEETPAGE& psp, BOOL bWizard);
#endif // _WIN32_WCE
#ifndef _AFX_NO_OCC_SUPPORT
	void Cleanup();
	const DLGTEMPLATE* InitDialogInfo(const DLGTEMPLATE* pTemplate);
#endif

	// Generated message map functions
	//{{AFX_MSG(CPropertyPage)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CPropertySheet;
    friend class CPropertySheetEx;
};

#if !defined(_WIN32_WCE)
class CPropertyPageEx : public CPropertyPage
{
    DECLARE_DYNAMIC(CPropertyPageEx)

// Construction
public:
    CPropertyPageEx();
    CPropertyPageEx(UINT nIDTemplate, UINT nIDCaption = 0, 
		UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);
    CPropertyPageEx(LPCTSTR lpszTemplateName, UINT nIDCaption = 0, 
		UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);
    void Construct(UINT nIDTemplate, UINT nIDCaption = 0, 
		UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);
    void Construct(LPCTSTR lpszTemplateName, UINT nIDCaption = 0, 
		UINT nIDHeaderTitle = 0, UINT nIDHeaderSubTitle = 0);

// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	// private implementation data
    CString m_strHeaderTitle;    // this is displayed in the header
    CString m_strHeaderSubTitle; //

	// implementation helpers
    void CommonConstruct(LPCTSTR lpszTemplateName, UINT nIDCaption, 
		UINT nIDHeaderTitle, UINT nIDHeaderSubTitle);

	friend class CPropertySheet;
    friend class CPropertySheetEx;
};
#endif // _WIN32_WCE

////////////////////////////////////////////////////////////////////////////
// CPropertySheet -- a tabbed "dialog" (really a popup-window)

class CTabCtrl; // forward reference (see afxcmn.h)

class CPropertySheet : public CWnd
{
	DECLARE_DYNAMIC(CPropertySheet)

// Construction
public:
	CPropertySheet();
	CPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL,
		UINT iSelectPage = 0);
	CPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
		UINT iSelectPage = 0);
	void Construct(UINT nIDCaption, CWnd* pParentWnd = NULL,
		UINT iSelectPage = 0);
	void Construct(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
		UINT iSelectPage = 0);

	// for modeless creation
	BOOL Create(CWnd* pParentWnd = NULL, DWORD dwStyle = (DWORD)-1,
		DWORD dwExStyle = 0);
	// the default style, expressed by passing -1 as dwStyle, is actually:
	// WS_SYSMENU | WS_POPUP | WS_CAPTION | DS_MODALFRAME | DS_CONTEXT_HELP | WS_VISIBLE

// Attributes
public:
	AFX_OLDPROPSHEETHEADER m_psh;

	int GetPageCount() const;
	CPropertyPage* GetActivePage() const;
	int GetActiveIndex() const;
	CPropertyPage* GetPage(int nPage) const;
	int GetPageIndex(CPropertyPage* pPage);
	BOOL SetActivePage(int nPage);
	BOOL SetActivePage(CPropertyPage* pPage);
	void SetTitle(LPCTSTR lpszText, UINT nStyle = 0);
	CTabCtrl* GetTabControl() const;

WCE_DEL	void SetWizardMode();
WCE_DEL	void SetFinishText(LPCTSTR lpszText);
WCE_DEL	void SetWizardButtons(DWORD dwFlags);

	void EnableStackedTabs(BOOL bStacked);

// Operations
public:
	virtual int DoModal();
	void AddPage(CPropertyPage* pPage);
	void RemovePage(CPropertyPage* pPage);
	void RemovePage(int nPage);
	void EndDialog(int nEndID); // used to terminate a modal dialog
	BOOL PressButton(int nButton);

// Implementation
public:
	virtual ~CPropertySheet();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void BuildPropPageArray();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL ContinueModal();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);
    AFX_OLDPROPSHEETHEADER* GetPropSheetHeader();   // should be virtual, but can't break binary compat yet
    BOOL IsWizard() const;

protected:
	CPtrArray m_pages;      // array of CPropertyPage pointers
	CString m_strCaption;   // caption of the pseudo-dialog
	CWnd* m_pParentWnd;     // parent window of property sheet
	BOOL m_bStacked;        // EnableStackedTabs sets this
	BOOL m_bModeless;       // TRUE when Create called instead of DoModal

	// Generated message map functions
	//{{AFX_MSG(CPropertySheet)
WCE_DEL	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg LRESULT HandleInitDialog(WPARAM, LPARAM);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnCommandHelp(WPARAM, LPARAM);
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM);
	afx_msg LRESULT OnSetDefID(WPARAM, LPARAM);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	friend class CPropertyPage;
};

#if !defined(_WIN32_WCE)
////////////////////////////////////////////////////////////////////////////
// CPropertySheetEx -- a tabbed "dialog" (really a popup-window), extended
//                     for IE4

class CPropertySheetEx : public CPropertySheet
{
    DECLARE_DYNAMIC(CPropertySheetEx)

// Construction
public:
    CPropertySheetEx();
    CPropertySheetEx(UINT nIDCaption, CWnd* pParentWnd = NULL,
        UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
        HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
    CPropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
        UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
        HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
    void Construct(UINT nIDCaption, CWnd* pParentWnd = NULL,
        UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
        HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);
	void Construct(LPCTSTR pszCaption, CWnd* pParentWnd = NULL,
        UINT iSelectPage = 0, HBITMAP hbmWatermark = NULL,
        HPALETTE hpalWatermark = NULL, HBITMAP hbmHeader = NULL);

// Attributes
public:
    PROPSHEETHEADER m_psh;

// Operations
public:
    void AddPage(CPropertyPageEx* pPage);

// Implementation
public:
    virtual ~CPropertySheetEx();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
    void CommonConstruct(CWnd* pParentWnd, UINT iSelectPage,
        HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader);
	virtual void BuildPropPageArray();
	void SetWizardMode();

    friend class CPropertyPage;
    friend class CPropertyPageEx;
};
#endif // _WIN32_WCE

#if defined(_WIN32_WCE)
class CCeSaveModifiedDlg : public CDialog
{
public:
	//{{AFX_DATA(CCeSaveModifiedDlg)
	enum { IDD = AFXCE_IDD_SAVEMODIFIEDDLG };
	//}}AFX_DATA
	CCeSaveModifiedDlg(CString strPrompt) : 
		CDialog(CCeSaveModifiedDlg::IDD),
		m_strPrompt(strPrompt)
		{ 
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 300)
			m_bFullScreen = FALSE;
#endif // _WIN32_WCE_PSPC
		}
	~CCeSaveModifiedDlg() { }

	CString m_strPrompt;

protected:
	BOOL OnInitDialog();
	void OnYes();
	void OnNo();
	void OnCancel();
	void OnSaveAs();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//{{AFX_MSG(CCeSaveModifiedDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// Inline function declarations

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#ifdef _AFX_ENABLE_INLINES
#define _AFXDLGS_INLINE AFX_INLINE
#include <afxdlgs.inl>
#endif

#undef AFX_DATA
#define AFX_DATA

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif
#ifndef _AFX_FULLTYPEINFO
#pragma component(mintypeinfo, off)
#endif

#endif //__AFXDLGS_H__

/////////////////////////////////////////////////////////////////////////////
