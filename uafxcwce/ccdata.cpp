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

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#if defined(_WIN32_WCE)
#if !defined(_WIN32_WCE_NO_DELAYLOAD)
#pragma comment(lib, "commctrl.lib")
#endif // _WIN32_WCE_NO_DELAYLOAD
#else // _WIN32_WCE
#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "winspool.lib")
#pragma comment(lib, "advapi32.lib")
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
// AfxGetPropSheetFont

struct _AFX_PROPPAGEFONTINFO : public CNoTrackObject
{
	LPTSTR m_pszFaceName;
	WORD m_wSize;
	_AFX_PROPPAGEFONTINFO() : m_pszFaceName(NULL), m_wSize(0) {}
	~_AFX_PROPPAGEFONTINFO() { WCE_FCTN(GlobalFree)(m_pszFaceName); }
};

PROCESS_LOCAL(_AFX_PROPPAGEFONTINFO, _afxPropPageFontInfo)

#define IDD_PROPSHEET   1006
#define IDD_WIZARD      1020

BOOL AFXAPI AfxGetPropSheetFont(CString& strFace, WORD& wSize, BOOL bWizard)
{
	_AFX_PROPPAGEFONTINFO* pFontInfo = _afxPropPageFontInfo.GetData();

	// determine which font property sheet will use
	if (pFontInfo->m_wSize == 0)
	{
		ASSERT(pFontInfo->m_pszFaceName == NULL);

		HINSTANCE hInst = WCE_FCTN(GetModuleHandleA)(WCE_IF("COMMCTRL.DLL","COMCTL32.DLL"));
		if (hInst != NULL)
		{
			HRSRC hResource = ::FindResource(hInst,
				MAKEINTRESOURCE(bWizard ? IDD_WIZARD : IDD_PROPSHEET),
				RT_DIALOG);
			HGLOBAL hTemplate = LoadResource(hInst, hResource);
			if (hTemplate != NULL)
				CDialogTemplate::GetFont((DLGTEMPLATE*)hTemplate, strFace,
					wSize);
		}

		pFontInfo->m_pszFaceName = (LPTSTR)WCE_FCTN(GlobalAlloc)(GPTR, sizeof(TCHAR) *
			(strFace.GetLength() + 1));
		lstrcpy(pFontInfo->m_pszFaceName, strFace);
		pFontInfo->m_wSize = wSize;
	}

	strFace = pFontInfo->m_pszFaceName;
	wSize = pFontInfo->m_wSize;

	return (wSize != 0xFFFF);
}

/////////////////////////////////////////////////////////////////////////////
