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

#ifdef CoRegisterClassObject
#undef CoRegisterClassObject
#endif

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

#if defined(_WIN32_WCE)
#if !defined(_WIN32_WCE_NO_DELAYLOAD)
#pragma comment(lib, WCE_OLE32_LIB)
#pragma comment(lib, WCE_OLEAUT32_LIB)
#pragma comment(lib, "commctrl.lib")
#endif // _WIN32_WCE_NO_DELAYLOAD
#else // _WIN32_WCE
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "oledlg.lib")
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "advapi32.lib")
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
// _AFX_OLE_STATE implementation

_AFX_OLE_STATE::_AFX_OLE_STATE()
{
	// Note: it is only necessary to intialize non-zero data.
}

_AFX_OLE_STATE::~_AFX_OLE_STATE()
{
#if !defined(_WIN32_WCE_NO_DELAYLOAD)
	// unload OLE DLLs if loaded
#ifdef _AFXDLL
	if (m_hInstOLEDLG != NULL)
		FreeLibrary(m_hInstOLEDLG);
	if (m_hInstOLEAUT != NULL)
		FreeLibrary(m_hInstOLEAUT);
	if (m_hInstOLE != NULL)
		FreeLibrary(m_hInstOLE);
	if (m_hInstUrlMon != NULL)
		::FreeLibrary(m_hInstUrlMon);
#endif //_AFXDLL
#endif // _WIN32_WCE_NO_DELAYLOAD
}

#pragma warning(disable: 4074)
#pragma init_seg(lib)

PROCESS_LOCAL(_AFX_OLE_STATE, _afxOleState)

/////////////////////////////////////////////////////////////////////////////
