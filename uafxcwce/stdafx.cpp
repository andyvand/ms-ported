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

#ifdef _AFXDLL
// The following symbol used to force inclusion of this module for _AFXDLL
#if defined(_WIN32_WCE)
#if defined(_WIN32_WCE_NO_PREFIXED_SYMBOLS) 
extern "C" { int _afxForceSTDAFX; }
#else // _WIN32_WCE_NO_PREFIXED_SYMBOLS
extern "C" { int __afxForceSTDAFX; }
#endif // _WIN32_WCE_NO_PREFIXED_SYMBOLS
#else // _WIN32_WCE
#ifdef _X86_
extern "C" { int _afxForceSTDAFX; }
#else
extern "C" { int __afxForceSTDAFX; }
#endif
#endif // _WIN32_WCE
#endif
