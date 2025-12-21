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

#ifdef AFX_DB_SEG
#pragma code_seg(AFX_DB_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

//////////////////////////////////////////////////////////////////////////////
// Implementation of CDBVariant

CDBVariant::CDBVariant()
{
	// Initialize type and value
	m_dwType = DBVT_NULL;
}

void CDBVariant::Clear()
{
	if (m_dwType == DBVT_STRING)
		delete (CString*)m_pstring;
	else if (m_dwType == DBVT_BINARY)
		delete (CLongBinary*)m_pbinary;
	else if (m_dwType == DBVT_DATE)
		delete (TIMESTAMP_STRUCT*)m_pdate;

	m_dwType = DBVT_NULL;
}

CDBVariant::~CDBVariant()
{
	Clear();
}

//////////////////////////////////////////////////////////////////////////////

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

/////////////////////////////////////////////////////////////////////////////
