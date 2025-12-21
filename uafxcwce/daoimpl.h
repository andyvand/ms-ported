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

// Note: must include AFXDAO.H first

/////////////////////////////////////////////////////////////////////////////
// _AFX_DAO_STATE

#undef AFX_DATA
#define AFX_DATA

class _AFX_DAO_STATE : public CNoTrackObject
{
public:
	// Handle initialization and cleanup of OLE and DAO
	_AFX_DAO_STATE();
	virtual ~_AFX_DAO_STATE();

	// MFC/DAO global data
	BOOL m_bOleInitialized;
	DAODBEngine* m_pDAODBEngine;
	CMapPtrToPtr m_mapWorkspaces;
};

#undef AFX_DATA
#define AFX_DATA

/////////////////////////////////////////////////////////////////////////////
