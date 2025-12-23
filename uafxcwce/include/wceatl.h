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
// This is a part of the Active Template Library(Windows CE).
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.
 
#ifndef _WCEATL_H_
#define _WCEATL_H_

#define _ATL_NO_UUIDOF
#define _ATL_NO_DEBUG_CRT			// no Debug run-time support
#if (_WIN32_WCE < 300)
	#define _ATL_NO_DOCHOSTUIHANDLER	// no DOC host
#endif //_WIN32_WCE

#define MIDL_INTERFACE(x)   struct __declspec(uuid(x)) __declspec(novtable)		// missed definition from rpcndr.h

#if !defined(_ASSERTE)
	#ifdef DEBUG
		#define _ASSERTE(x)   if (!(x)) DebugBreak()
	#else
		#define _ASSERTE(x)
	#endif // DEBUG
#endif // ASSERTE

#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))

// unsupported flags
#define WS_MINIMIZE			0
#define WS_OVERLAPPEDWINDOW 0	
#define WS_EX_MDICHILD		0
#define WS_EX_APPWINDOW		0

#undef RDW_FRAME
#define RDW_FRAME			0

//GUID declarations
struct __declspec(uuid("00000000-0000-0000-C000-000000000046")) IUnknown;
struct __declspec(uuid("00000001-0000-0000-C000-000000000046")) IClassFactory;
struct __declspec(uuid("0000000c-0000-0000-C000-000000000046")) IStream;
struct __declspec(uuid("00020400-0000-0000-C000-000000000046")) IDispatch;
struct __declspec(uuid("00020401-0000-0000-C000-000000000046")) ITypeInfo;
struct __declspec(uuid("00020402-0000-0000-C000-000000000046")) ITypeLib;
struct __declspec(uuid("00020412-0000-0000-C000-000000000046")) ITypeInfo2;

struct __declspec(uuid("A6BC3AC0-DBAA-11CE-9DE3-00AA004BB851")) IProvideClassInfo2;

struct __declspec(uuid("B196B284-BAB4-101A-B69C-00AA00341D07")) IConnectionPointContainer;
struct __declspec(uuid("B196B286-BAB4-101A-B69C-00AA00341D07")) IConnectionPoint;
struct __declspec(uuid("B196B287-BAB4-101A-B69C-00AA00341D07")) IEnumConnections;
struct __declspec(uuid("B196B288-BAB4-101A-B69C-00AA00341D07")) IOleControl;
struct __declspec(uuid("B196B289-BAB4-101A-B69C-00AA00341D07")) IOleControlSite;
struct __declspec(uuid("B196B28B-BAB4-101A-B69C-00AA00341D07")) ISpecifyPropertyPages;

struct __declspec(uuid("9BFBBC02-EFF1-101A-84ED-00AA00341D07")) IPropertyNotifySink;

struct __declspec(uuid("1C2056CC-5EF4-101B-8BC8-00AA003E3B29")) IOleInPlaceObjectWindowless;

struct __declspec(uuid("922EADA0-3424-11CF-B670-00AA004CD6D8")) IOleInPlaceSiteWindowless;

struct __declspec(uuid("3AF24292-0C96-11CE-A0CF-00AA00600AB8")) IViewObjectEx;
struct __declspec(uuid("55980BA0-35AA-11CF-B671-00AA004CD6D8")) IPointerInactive;

struct __declspec(uuid("0000010C-0000-0000-C000-000000000046")) IPersist;
struct __declspec(uuid("0000010e-0000-0000-C000-000000000046")) IDataObject;
struct __declspec(uuid("0000010f-0000-0000-C000-000000000046")) IAdviseSink;

struct __declspec(uuid("00000109-0000-0000-C000-000000000046")) IPersistStream;
struct __declspec(uuid("00000110-0000-0000-C000-000000000046")) IDataAdviseHolder;
struct __declspec(uuid("00000111-0000-0000-C000-000000000046")) IOleAdviseHolder;
struct __declspec(uuid("00000112-0000-0000-C000-000000000046")) IOleObject;
struct __declspec(uuid("00000113-0000-0000-C000-000000000046")) IOleInPlaceObject;
struct __declspec(uuid("00000114-0000-0000-C000-000000000046")) IOleWindow;
struct __declspec(uuid("00000116-0000-0000-C000-000000000046")) IOleInPlaceFrame;
struct __declspec(uuid("00000115-0000-0000-C000-000000000046")) IOleInPlaceUIWindow;
struct __declspec(uuid("00000117-0000-0000-C000-000000000046")) IOleInPlaceActiveObject;
struct __declspec(uuid("00000118-0000-0000-C000-000000000046")) IOleClientSite;
struct __declspec(uuid("00000119-0000-0000-C000-000000000046")) IOleInPlaceSite;
struct __declspec(uuid("0000011b-0000-0000-C000-000000000046")) IOleContainer;

struct __declspec(uuid("22F03340-547D-101B-8E65-08002B2BD119")) ICreateErrorInfo;
struct __declspec(uuid("1CF2B120-547D-101B-8E65-08002B2BD119")) IErrorInfo;

struct __declspec(uuid("9C2CAD80-3424-11CF-B670-00AA004CD6D8")) IOleInPlaceSiteEx;
struct __declspec(uuid("FC4801A3-2BA9-11CF-A229-00AA003D7352")) IObjectWithSite;
struct __declspec(uuid("BEF6E003-A874-101A-8BBA-00AA00300CAB")) IFontDisp;
struct __declspec(uuid("7FD52380-4E07-101B-AE2D-08002B2EC713")) IPersistStreamInit;
struct __declspec(uuid("BEF6E002-A874-101A-8BBA-00AA00300CAB")) IFont;

#pragma warning(disable: 4615) // pragma warning(push) is unrecognized


#define GMEM_FIXED LMEM_FIXED

#if (_WIN32_WCE < 300)
#define GHND       LMEM_FIXED | LMEM_ZEROINIT
#define GlobalAlloc LocalAlloc
#define GlobalLock
#define GlobalUnlock
#define GlobalHandle (void*)
#define GlobalFree LocalFree
#endif // _WIN32_WCE


#define FreeResource(x)
#define UnlockResource(x)

#endif // _WCEATL_H_
