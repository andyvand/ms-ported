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

#ifdef AFX_AUX_SEG
#pragma code_seg(AFX_AUX_SEG)
#endif


// AfxIsValidString() returns TRUE if the passed pointer
// references a string of at least the given length in characters.
// A length of -1 (the default parameter) means that the string
// buffer's minimum length isn't known, and the function will
// return TRUE no matter how long the string is. The memory
// used by the string can be read-only.

BOOL AFXAPI AfxIsValidString(LPCWSTR lpsz, int nLength /* = -1 */)
{
	if (lpsz == NULL)
		return FALSE;
	return afxData.bWin95 || ::WCE_FCTN(IsBadStringPtrW)(lpsz, nLength) == 0;
}

// As above, but for ANSI strings.

BOOL AFXAPI AfxIsValidString(LPCSTR lpsz, int nLength /* = -1 */)
{
	if (lpsz == NULL)
		return FALSE;
#if defined(_WIN32_WCE)
// WinCE: our workaround is to check to see if the first character 
// and last character are readable
	if(IsBadReadPtr(lpsz, 1))
		return FALSE;
	return ::IsBadReadPtr(lpsz + min((UINT)nLength, (UINT)strlen(lpsz)), 1) == 0;
#else // _WIN32_WCE
	return afxData.bWin95 || ::WCE_FCTN(IsBadStringPtrA)(lpsz, nLength) == 0;
#endif // _WIN32_WCE
}

// AfxIsValidAddress() returns TRUE if the passed parameter points
// to at least nBytes of accessible memory. If bReadWrite is TRUE,
// the memory must be writeable; if bReadWrite is FALSE, the memory
// may be const.

BOOL AFXAPI AfxIsValidAddress(const void* lp, UINT nBytes,
	BOOL bReadWrite /* = TRUE */)
{
	// simple version using Win-32 APIs for pointer validation.
	return (lp != NULL && !IsBadReadPtr(lp, nBytes) &&
		(!bReadWrite || !IsBadWritePtr((LPVOID)lp, nBytes)));
}

/////////////////////////////////////////////////////////////////////////////
