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

#if !defined(_WIN32_WCE_NO_PRINTING)

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Print/Print Setup dialog

BEGIN_MESSAGE_MAP(CPrintDialog, CCommonDialog)
	//{{AFX_MSG_MAP(CPrintDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPrintDialog::CPrintDialog(BOOL bPrintSetupOnly, 
	DWORD dwFlags, CWnd* pParentWnd) 
	: m_pd(m_pdActual), CCommonDialog(pParentWnd) 
{
	ASSERT(bPrintSetupOnly == FALSE); 
	memset(&m_pdActual, 0, sizeof(m_pdActual)); 

	m_pd.cbStruct = sizeof(m_pdActual);
	m_nIDHelp = AFX_IDD_PRINT;
   	m_nFromPage = 1; 
   	m_nToPage   = 0xffff; 
   	m_nMinPage  = 1; 
   	m_nMaxPage  = 0xffff; 
   	m_nCopies   = 1;
}

CPrintDialog::CPrintDialog(PRINTDLG& pdInit)
	: m_pd(pdInit), CCommonDialog(NULL)
{
}

CPrintDialog* CPrintDialog::AttachOnSetup()
{
	ASSERT_VALID(this);

	CPrintDialog* pDlgSetup;

	pDlgSetup = new CPrintDialog(m_pd);
	pDlgSetup->m_hWnd = NULL;
	pDlgSetup->m_pParentWnd = m_pParentWnd;
	pDlgSetup->m_nIDHelp = AFX_IDD_PRINTSETUP;
	return pDlgSetup;
}

int CPrintDialog::DoModal()
{
	ASSERT_VALID(this);
	
	m_pd.hwndOwner = PreModal(); 

	if(m_pd.hdc != NULL)
	{
		DeleteDC(m_pd.hdc);
		m_pd.hdc = NULL;
	}

	PRINTDLG pd = m_pd;
	int nResult = ::PrintDlg(&pd);
	m_pd = pd;

	PostModal();

	return nResult ? nResult : IDCANCEL;
}

int CPrintDialog::GetCopies() const
{
	return m_nCopies;
}

#define REG_PRINTER_VALUE_SIZE 40

CString CPrintDialog::GetDeviceName() const
{
	HKEY    hKeyPrinters;
	LPTSTR  lpszPrinters = _T("Printers");
	TCHAR   szValueName[REG_PRINTER_VALUE_SIZE] = _T("");
	TCHAR	szValue[REG_PRINTER_VALUE_SIZE] = _T("");

	if(WCE_FCTN(RegOpenKey)(HKEY_LOCAL_MACHINE, lpszPrinters, &hKeyPrinters) == ERROR_SUCCESS )
	{
		DWORD dwSizeValueName = REG_PRINTER_VALUE_SIZE;
		DWORD dwSizeValue = REG_PRINTER_VALUE_SIZE;
		DWORD dwType = REG_SZ;
	    int iKeyPrinter = 0;

		while(RegEnumValue(hKeyPrinters, iKeyPrinter, szValueName, &dwSizeValueName, NULL, &dwType, 
			  (LPBYTE)szValue, &dwSizeValue) == ERROR_SUCCESS)
		{
			if(!lstrcmp(szValueName, _T("DefaultPrinter")))
				break;
		
			iKeyPrinter++;
			dwSizeValueName = REG_PRINTER_VALUE_SIZE;
			dwSizeValue = REG_PRINTER_VALUE_SIZE;
		}
		RegCloseKey(hKeyPrinters);
	}
	return CString(szValue);
}

CString CPrintDialog::GetPortName() const
{
	CString csPrinter = GetDeviceName();

	HKEY    hKeyPrinters;
	TCHAR   lpszPrinter[REG_PRINTER_VALUE_SIZE * 2] = _T("Printers\\");
	TCHAR   szValueName[REG_PRINTER_VALUE_SIZE] = _T("");
	TCHAR	szValue[REG_PRINTER_VALUE_SIZE] = _T("");

	lstrcat(lpszPrinter, (LPCTSTR)csPrinter.GetBuffer(REG_PRINTER_VALUE_SIZE));
	csPrinter.ReleaseBuffer();

	if(WCE_FCTN(RegOpenKey)(HKEY_LOCAL_MACHINE, (LPCTSTR)lpszPrinter, &hKeyPrinters) == ERROR_SUCCESS )
	{
		DWORD dwSizeValueName = REG_PRINTER_VALUE_SIZE;
		DWORD dwSizeValue = REG_PRINTER_VALUE_SIZE;
		DWORD dwType = REG_SZ;
	    int iKeyPrinter = 0;

		while(RegEnumValue(hKeyPrinters, iKeyPrinter, szValueName, &dwSizeValueName, NULL, &dwType, 
			  (LPBYTE)szValue, &dwSizeValue) == ERROR_SUCCESS)
		{
			if(!lstrcmp(szValueName, _T("Port")))
				break;
		
			iKeyPrinter++;
			dwSizeValueName = REG_PRINTER_VALUE_SIZE;
			dwSizeValue = REG_PRINTER_VALUE_SIZE;
		}
		RegCloseKey(hKeyPrinters);
	}
	return CString(szValue);
}

BOOL CPrintDialog::GetDefaults()
{
	m_pd.dwFlags |= PD_RETURNDEFAULTDC;

	if(m_pd.hdc != NULL)
	{
		DeleteDC(m_pd.hdc);
		m_pd.hdc = NULL;
	}

	PRINTDLG pd = m_pd;
	int nResult = ::PrintDlg(&pd);
	m_pd = pd;

	return nResult;
}

// WinCE-only: substitute for CPageSetupDialog::GetPaperSize()
CSize CPrintDialog::GetPaperSize() const
{
	CSize size;
 
	if(m_pd.dwFlags & PD_SELECTA4) // 21.0 cm x 29.7 cm
	{
		if(m_pd.dwFlags & PD_INHUNDREDTHSOFMILLIMETERS)
		{
			size.cx = 21000;  // 21.0 * 10 * 100
			size.cy = 29700;  // 29.7 * 10 * 100
		}
		else // PD_INTHOUSANDTHSOFINCHES assumed
		{
			size.cx = 210*254; // 21.0 * 2.54 * 1000
			size.cy = 297*254; // 29.7 * 2.54 * 1000
		}
	}
	else // PD_SELECTLETTER assumed  11.0 in x 8.5 in
	{
		if(m_pd.dwFlags & PD_INHUNDREDTHSOFMILLIMETERS)
		{
			size.cx = (int)(8500.0 / 2.54); // 8.5 / 2.54 * 10 * 100; 
			size.cy = (int)(11000.0 / 2.54); // 11.0 / 2.54 * 10 * 100; 
		}
		else // PD_INTHOUSANDTHSOFINCHES assumed
		{
			size.cx = 8500;   // 8.5 * 1000
			size.cy = 11000;  // 11.0 * 1000
		}
	}

	return size;
}

// WinCE-only: substitute for CPageSetupDialog::GetMargins()
void CPrintDialog::GetMargins(LPRECT lpRectMargins, LPRECT lpRectMinMargins) const
{
	if (lpRectMargins != NULL)
		WCE_FCTN(memcpy)(lpRectMargins, &m_pd.rcMargin, sizeof(RECT));
	if (lpRectMinMargins != NULL)
		WCE_FCTN(memcpy)(lpRectMinMargins, &m_pd.rcMinMargin, sizeof(RECT));
}

////////////////////////////////////////////////////////////////////////////
// CPrintDialog diagnostics

#ifdef _DEBUG
void CPrintDialog::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);

	dc << "m_pd.hwndOwner = " << (UINT)m_pd.hwndOwner;
	if (m_pd.hdc != NULL)
		dc << "\nm_pd.hdc = " << CDC::FromHandle(m_pd.hdc);
	dc << "\nm_pd.dwFlags = " << (LPVOID)m_pd.dwFlags;
	dc << "\n";
}
#endif //_DEBUG

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(CPrintDialog, CCommonDialog)

////////////////////////////////////////////////////////////////////////////
#endif // _WIN32_WCE_NO_PRINTING
