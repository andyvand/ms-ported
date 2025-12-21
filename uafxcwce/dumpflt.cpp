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
#include <float.h>

#ifdef AFX_DBG1_SEG
#pragma code_seg(AFX_DBG1_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Diagnostic Stream output for floating point numbers

#ifdef _DEBUG
CDumpContext& CDumpContext::operator<<(float f)
{
	char szBuffer[32];
	_gcvt(f, FLT_DIG, szBuffer);
	*this << szBuffer;
	return *this;
}

CDumpContext& CDumpContext::operator<<(double d)
{
	char szBuffer[32];
	_gcvt(d, DBL_DIG, szBuffer);
	*this << szBuffer;
	return *this;
}
#endif

/////////////////////////////////////////////////////////////////////////////
