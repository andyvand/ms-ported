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

// Modify this file when creating custom builds of MFCCE.

#ifndef __WCE_H__
#define __WCE_H__
#pragma warning(disable: 4005)

// Define official (Microsoft-only) DLL names using the format: 
//    MFCCE/OLECE + version number + suffix + extension, 
//    where suffix = "" for release , "d" for debug, and "i" for localization.
//    The suffix and extension are specified when the macros are invoked.
#define WCE_MFC_FILENAME(ext) MAKE_FILENAME_1(MFCCE,400,ext)
#define WCE_OLE_FILENAME(ext) MAKE_FILENAME_1(OLECE,400,ext)

// For custom MFCCE builds, the above macros should be should be replaced with
// hardcoded library names that don't conflict with the names by those macros.
// Examples: (for MYMFCD.LIB, MYOLED.LIB, etc.)
//#define WCE_MFC_FILENAME(ext) MAKE_FILENAME(MYMFC,ext)
//#define WCE_OLE_FILENAME(ext) MAKE_FILENAME(MYOLE,ext)

// Specify Palm-Sized PC build
//#define _WIN32_WCE_PSPC // (forces some pruning--see below)

// MFCCE pruning flags
//#define _WIN32_WCE_NO_PRINTING
//#define _WIN32_WCE_NO_COLORDLG
//#define _WIN32_WCE_NO_WINSOCK
//#define _WIN32_WCE_NO_WININET
//#define _WIN32_WCE_NO_OLE
//#define _WIN32_WCE_NO_CURSOR
//#define _WIN32_WCE_NO_PALETTE 
//#define _WIN32_WCE_NO_FINDREPLACE
//#define _WIN32_WCE_NO_CONTROLBARS

// DLL thunking (the earlier form of delayed library loading)
// When thunking is disabled, the corresponding OS library becomes a
// dependency at link-time.
//#define _WIN32_WCE_NO_COMMDLG_THUNKS  
//#define _WIN32_WCE_NO_WINSOCK_THUNKS
//#define _WIN32_WCE_NO_WININET_THUNKS
//#define _WIN32_WCE_NO_OLE_THUNKS     

// Set the private build version number
#define _MFC_RBLD        0
#define _MFC_USER_RBLD   "0"

// Check OS version number
#if !defined(_WIN32_WCE)
	#error _WIN32_WCE is not set to 200 or greater
#elif (_WIN32_WCE < 200)
	#error Can not build for OS versions earlier than 2.0
#endif // _WIN32_WCE

// Prune for H/PC 2.0 builds
#if (_WIN32_WCE == 200) || (_WIN32_WCE == 201)
	#define _WIN32_WCE_NO_CURSOR
#endif // _WIN32_WCE

// Define helper macros used for defining the MFCCE library names at the beginning
// of this file. In the versioned case, two macro expansion steps are necessary to 
// force evaulation of _WIN32_WCE before the tokenizing operators are used.
#define MAKE_FILENAME(name, ext) #name #ext
#define MAKE_FILENAME_1(name, ver, ext) MAKE_FILENAME_2(name, ver, ext)
#define MAKE_FILENAME_2(name, ver, ext) #name #ver #ext

// Define library names that may differ for emulation
#if defined(_WIN32_WCE_EMULATION) && (_WIN32_WCE <= 201) 
	#define WCE_COMMDLG_LIB  "commdlgm.lib"
	#define WCE_COMMDLG_DLL  "commdlgm.dll"
	#define WCE_WINSOCK_LIB  "winsockm.lib"
	#define WCE_WINSOCK_DLL  "winsockm.dll"
	#define WCE_WININET_LIB  "wininetm.lib"
	#define WCE_WININET_DLL  "wininetm.dll"
	#define WCE_OLE32_LIB    "ole32m.lib"
	#define WCE_OLE32_DLL    "ole32m.dll"
	#define WCE_OLEAUT32_LIB "oleautm.lib"
	#define WCE_OLEAUT32_DLL "oleautm.dll"
#else // _WIN32_WCE_EMULATION
	#define WCE_COMMDLG_LIB  "commdlg.lib"
	#define WCE_COMMDLG_DLL  "commdlg.dll"
	#define WCE_WINSOCK_LIB  "winsock.lib"
	#define WCE_WINSOCK_DLL  "winsock.dll"
	#define WCE_WININET_LIB  "wininet.lib"
	#define WCE_WININET_DLL  "wininet.dll"
	#define WCE_OLE32_LIB    "ole32.lib"
	#define WCE_OLE32_DLL    "ole32.dll"
	#define WCE_OLEAUT32_LIB "oleaut32.lib"
	#define WCE_OLEAUT32_DLL "oleaut32.dll"
#endif // _WIN32_WCE_EMULATION
	
// Prune for Palm-Sized PC build
#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE == 201)
	#define _WIN32_WCE_NO_OLE
	#define _WIN32_WCE_NO_CURSOR
	#define _WIN32_WCE_NO_PRINTING
	#define _WIN32_WCE_NO_COLORDLG
#endif // _WIN32_WCE_PSPC

#if defined(_WIN32_WCE_PSPC) && (_WIN32_WCE >= 211)
	#define _WIN32_WCE_NO_PRINTING
	#define _WIN32_WCE_NO_COLORDLG
#endif // _WIN32_WCE_PSPC

// If Printing and ChooseColor are not used, then COMMDLG.DLL is not used.
#if defined(_WIN32_WCE_NO_PRINTING) && defined(_WIN32_WCE_NO_COLORDLG)
	#define _WIN32_WCE_NO_COMMDLG
#endif // !_WIN32_WCE_NO_PRINTING && !_WIN32_WCE_NO_COLORDLG

// Prune COMMDLG.DLL
#if defined(_WIN32_WCE_NO_COMMDLG)
	#define _INC_CDERR    // OS header
	#define _WIN32_WCE_NO_COMMDLG_THUNKS
#endif // _WIN32_WCE_NO_COMMDLG

// Prune WINSOCK.DLL
#if defined(_WIN32_WCE_NO_WINSOCK)
	#define __AFXSOCK_H_ // MFC header
	#define _WINSOCKAPI_ // OS header
	#define _AFX_NO_SOCKET_SUPPORT
	#define _WIN32_WCE_NO_WININET // _WIN32_WCE_NO_WINSOCK imples this
	#define _WIN32_WCE_NO_WINSOCK_THUNKS
#endif // _WIN32_WCE_NO_WININET

// Prune WININET.DLL
#if defined(_WIN32_WCE_NO_WININET)
	#define __AFXINET_H_ // MFC header
	#define _WININET_    // OS header
	#define _AFX_NO_INET_SUPPORT
	#define _WIN32_WCE_NO_WININET_THUNKS
#endif // _WIN32_WCE_NO_WININET

// Prune OLE32.DLL/OLEAUT32.DLL
#if defined(_WIN32_WCE_NO_OLE)
	#define __AFXOLE_H__ // MFC header
	#define __AFXCTL_H__ // MFC header
	#define _AFX_NO_OLE_SUPPORT
	#define _AFX_NO_OCC_SUPPORT
	#define _AFX_NO_OCX_SUPPORT
	#define _WIN32_WCE_NO_OLE_THUNKS
#endif // _WIN32_WCE_NO_OLE

// Prune unsupported MFC features
#define _WIN32_WCE_NO_DELAYLOAD
#define _WIN32_WCE_NO_FTP
#define _WIN32_WCE_NO_TOOLTIPS
#define _WIN32_WCE_NO_DOCKBARS
#define _WIN32_WCE_NO_GDITRANSFORM
#define _WIN32_WCE_NO_CONTEXTHELP 
#define _WIN32_WCE_NO_OWNERDRAW_LBS
#define _AFX_NO_DEBUG_CRT
#define _AFX_NO_DOCOBJECT_SUPPORT
#define _AFX_NO_RICHEDIT_SUPPORT
#define _AFX_NO_CTL3D_SUPPORT
#define _AFX_NO_DAO_SUPPORT
#define _AFX_NO_DB_SUPPORT
#define _AFX_NO_OLEDB_SUPPORT
#define _AFX_NO_CTL3D_RESOURCES
#define _AFX_NO_OCC_RESOURCES
#define _AFX_NO_MAPI_RESOURCES
#define _AFX_NO_OLE_RESOURCES
#define _AFX_NO_STOCKPPG_RESOURCES
#define _AFX_OLD_EXCEPTIONS

// Prune unsupported MFC/OS headers
#define __AFXODLGS_H__
#define __AFXDOCOB_H__
#define __AFXHTML_H__
#define _INC_STDIO
#define _INC_NEW
#define _INC_TIME
#define _INC_STDDEF
#define _INC_DLGS
#define _INC_CDERR
#define _INC_DDEMLH
#define _INC_PROCESS
#define _INC_IO
#define _INC_ERRNO
#define _INC_TYPES
#define _INC_STAT
#define _INC_FCNTL
#define _OLEDLG_H_
#define _SHLOBJ_H_
#define _OLEAUT32_
#define _WINSPOOL_
#define _WINNETWK_
#define __msdadc_h__
#define __urlmon_h__
#if (_WIN32_WCE < 300)
#define __docobj_h__
#endif//_WIN32_WCE
#define __oledb_h__
#define _DDEHEADER_INCLUDED_
#define __SQLEXT
#define __SQL

// Disable thunking for static builds or if delay loading is available
#if !defined(_AFXDLL) || !defined(_WIN32_WCE_NO_DELAYLOAD)
	#define _WIN32_WCE_NO_COMMDLG_THUNKS
	#define _WIN32_WCE_NO_WINSOCK_THUNKS
	#define _WIN32_WCE_NO_WININET_THUNKS
	#define _WIN32_WCE_NO_OLE_THUNKS
#endif

// Additional definitions per CPU type
#if defined(SH3) || defined(_SH3_) 
	#undef   SHx
	#define  SHx
	#undef   SH3
	#define  SH3
	#undef  _SH3_
	#define _SH3_
	#define _WIN32_WCE_NO_PREFIXED_SYMBOLS
#elif defined(SH4) || defined(_SH4_)
	#undef   SHx
	#define  SHx
	#undef   SH4
	#define  SH4
	#undef  _SH4_
	#define _SH4_
	#define _WIN32_WCE_NO_PREFIXED_SYMBOLS
#elif defined(MIPS) || defined(_MIPS_) 
	#undef   MIPS
	#define  MIPS
	#undef  _MIPS_
	#define _MIPS_
#elif defined(MIPS16) || defined(_MIPS16_) 
	#undef   MIPS16
	#define  MIPS16
	#undef  _MIPS16_
	#define _MIPS16_
#elif defined(PPC) || defined(_PPC_) 
	#undef   PPC
	#define  PPC
	#undef  _PPC_
	#define _PPC_
#elif defined(ARM) || defined(_ARM_) 
	#undef   ARM
	#define  ARM
	#undef  _ARM_
	#define _ARM_
#elif defined(_WIN32_WCE_CEPC) || defined(_M_IX86)
	#undef x86
	#define x86 1
	#undef _X86_ 
	#define _X86_
	#define _WIN32_WCE_NO_PREFIXED_SYMBOLS
#elif defined(_WIN32_WCE_EMULATION)
	#undef x86
	#undef _X86_
	#define _X86_
	#define _WIN32_WCE_NO_PREFIXED_SYMBOLS
#elif defined(_M_CEE)
	#undef _WIN32_WCE_CEF
	#define _WIN32_WCE_CEF
#endif

// Misc. definitions
#define  _AFX_PORTABLE
#undef   UNDER_CE
#define  UNDER_CE _WIN32_WCE
#undef  _MBCS 
#undef   UNICODE
#define  UNICODE
#undef  _UNICODE
#define _UNICODE
#undef   WIN32
#define  WIN32
#undef  _WINDOWS
#define _WINDOWS
#if (_WIN32_WCE < 210)
#undef   COREDLL
#define  COREDLL
#endif // _WIN32_WCE
#undef  _CRTIMP
#define _CRTIMP
#undef  _MT
#define _MT
#undef   STRICT
#define  STRICT
#undef   NDEBUG
#define  NDEBUG
#undef   ENABLE_TRACE
#if defined(DEBUG) || defined(_DEBUG)
	#undef   DEBUG
	#define  DEBUG
	#undef  _DEBUG
	#define _DEBUG
	#undef   ENABLE_TRACE
	#define  ENABLE_TRACE
#endif

#pragma warning(default: 4005)
#endif // __WCE_H__

