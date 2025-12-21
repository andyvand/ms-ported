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

// Note: must include AFXDB.H first

#if !defined(_WIN32_WCE_NO_WINSOCK)
#undef AFX_DATA
#define AFX_DATA AFX_NET_DATA

/////////////////////////////////////////////////////////////////////////////
// _AFX_SOCK_STATE

#undef AFX_DATA
#define AFX_DATA

class _AFX_SOCK_STATE : public CNoTrackObject
{
public:
	DWORD m_dwReserved1;    // reserved for version 4.1 only
	void (AFXAPI *m_pfnSockTerm)(void); // set once initialized
	virtual ~_AFX_SOCK_STATE();
};

EXTERN_PROCESS_LOCAL(_AFX_SOCK_STATE, _afxSockState)

#undef AFX_DATA
#define AFX_DATA

/////////////////////////////////////////////////////////////////////////////
#endif // _WIN32_WCE_NO_WINSOCK
