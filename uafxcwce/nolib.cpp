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

/////////////////////////////////////////////////////////////////////////////
// global data

// The following symbol used to force inclusion of this module
#if defined(_WIN32_WCE)
#if defined(_WIN32_WCE_NO_PREFIXED_SYMBOLS) 
extern "C" { int _afxForceEXCLUDE; }
#else // _WIN32_WCE_NO_PREFIXED_SYMBOLS
extern "C" { int __afxForceEXCLUDE; }
#endif // _WIN32_WCE_NO_PREFIXED_SYMBOLS
#else // _WIN32_WCE
#ifdef _X86_
extern "C" { int _afxForceEXCLUDE; }
#else
extern "C" { int __afxForceEXCLUDE; }
#endif
#endif // _WIN32_WCE

// Win32 library excludes
#ifndef _AFXDLL
	#pragma comment(linker, "/disallowlib:mfc42d.lib")
	#pragma comment(linker, "/disallowlib:mfco42d.lib")
	#pragma comment(linker, "/disallowlib:mfcd42d.lib")
	#pragma comment(linker, "/disallowlib:mfcn42d.lib")
	#pragma comment(linker, "/disallowlib:mfcs42d.lib")
	#pragma comment(linker, "/disallowlib:mfc42.lib")
	#pragma comment(linker, "/disallowlib:mfcs42.lib")
	#pragma comment(linker, "/disallowlib:mfc42ud.lib")
	#pragma comment(linker, "/disallowlib:mfco42ud.lib")
	#pragma comment(linker, "/disallowlib:mfcd42ud.lib")
	#pragma comment(linker, "/disallowlib:mfcn42ud.lib")
	#pragma comment(linker, "/disallowlib:mfcs42ud.lib")
	#pragma comment(linker, "/disallowlib:mfc42u.lib")
	#pragma comment(linker, "/disallowlib:mfcs42u.lib")
	#ifndef _UNICODE
		#pragma comment(linker, "/disallowlib:uafxcwd.lib")
		#pragma comment(linker, "/disallowlib:uafxcw.lib")
		#ifdef _DEBUG
			#pragma comment(linker, "/disallowlib:nafxcw.lib")
		#else
			#pragma comment(linker, "/disallowlib:nafxcwd.lib")
		#endif
	#else
		#pragma comment(linker, "/disallowlib:nafxcwd.lib")
		#pragma comment(linker, "/disallowlib:nafxcw.lib")
		#ifdef _DEBUG
			#pragma comment(linker, "/disallowlib:uafxcw.lib")
		#else
			#pragma comment(linker, "/disallowlib:uafxcwd.lib")
		#endif
	#endif
#else
	#pragma comment(linker, "/disallowlib:nafxcwd.lib")
	#pragma comment(linker, "/disallowlib:nafxcw.lib")
	#pragma comment(linker, "/disallowlib:uafxcwd.lib")
	#pragma comment(linker, "/disallowlib:uafxcw.lib")
	#ifndef _UNICODE
		#pragma comment(linker, "/disallowlib:mfc42ud.lib")
		#pragma comment(linker, "/disallowlib:mfco42ud.lib")
		#pragma comment(linker, "/disallowlib:mfcd42ud.lib")
		#pragma comment(linker, "/disallowlib:mfcn42ud.lib")
		#pragma comment(linker, "/disallowlib:mfcs42ud.lib")
		#pragma comment(linker, "/disallowlib:mfc42u.lib")
		#pragma comment(linker, "/disallowlib:mfcs42u.lib")
		#ifdef _DEBUG
			#pragma comment(linker, "/disallowlib:mfc42.lib")
			#pragma comment(linker, "/disallowlib:mfcs42.lib")
		#else
			#pragma comment(linker, "/disallowlib:mfc42d.lib")
			#pragma comment(linker, "/disallowlib:mfco42d.lib")
			#pragma comment(linker, "/disallowlib:mfcd42d.lib")
			#pragma comment(linker, "/disallowlib:mfcn42d.lib")
			#pragma comment(linker, "/disallowlib:mfcs42d.lib")
		#endif
	#else
		#pragma comment(linker, "/disallowlib:mfc42d.lib")
		#pragma comment(linker, "/disallowlib:mfco42d.lib")
		#pragma comment(linker, "/disallowlib:mfcd42d.lib")
		#pragma comment(linker, "/disallowlib:mfcn42d.lib")
		#pragma comment(linker, "/disallowlib:mfcs42d.lib")
		#pragma comment(linker, "/disallowlib:mfc42.lib")
		#pragma comment(linker, "/disallowlib:mfcs42.lib")
		#ifdef _DEBUG
			#pragma comment(linker, "/disallowlib:mfc42u.lib")
			#pragma comment(linker, "/disallowlib:mfcs42u.lib")
		#else
			#pragma comment(linker, "/disallowlib:mfc42ud.lib")
			#pragma comment(linker, "/disallowlib:mfco42ud.lib")
			#pragma comment(linker, "/disallowlib:mfcd42ud.lib")
			#pragma comment(linker, "/disallowlib:mfcn42ud.lib")
			#pragma comment(linker, "/disallowlib:mfcs42ud.lib")
		#endif
	#endif
#endif

/////////////////////////////////////////////////////////////////////////////
