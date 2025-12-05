// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////

// This function is separated from afxmem.cpp so that ASAN can take 
// precedence over it smoothly.
// If ASAN implementation is ever modified such that ASAN's replacement would
// not be appropriate we should once again induce a linker error.

// Currently ASAN has the same behavior as below function
// (except if new fails - in this case ASAN will trigger an error).
// If this function changes when an allocation succeeds then ASAN precedence
// may need to be revisited.

// ASAN doesn’t cover this overload, so this was separated from afx_new_scalar
// because otherwise when building in debug this would cause a conflict between
// the one in ASAN and the one in afx_new_scalar (which should’ve been discarded).

// Ignore SAL warnings for returning a null pointer from a non-noexcept new operator.
#pragma warning(disable: 6387 28183 28196)

#ifndef _AFXDLL
extern _PNH _afxNewHandler;
#endif

#ifdef _DEBUG
AFX_STATIC_DATA const _PNH _pfnUninitialized = (_PNH)-1;

void* __cdecl operator new(size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
{
	void* pResult;
#ifdef _AFXDLL
	_PNH pfnNewHandler = _pfnUninitialized;
#endif
	for (;;)
	{
		pResult = _malloc_dbg(nSize, nType, lpszFileName, nLine);
		if (pResult != NULL)
			return pResult;

#ifdef _AFXDLL
		if (pfnNewHandler == _pfnUninitialized)
		{
			AFX_MODULE_THREAD_STATE* pState = AfxGetModuleThreadState();
			pfnNewHandler = pState->m_pfnNewHandler;
		}
		if (pfnNewHandler == NULL || (*pfnNewHandler)(nSize) == 0)
			break;
#else
		if (_afxNewHandler == NULL || (*_afxNewHandler)(nSize) == 0)
			break;
#endif
	}
	return pResult;

}
#endif
