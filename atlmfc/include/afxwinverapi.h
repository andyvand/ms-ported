// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifndef __AFXWINVERAPI_H__
#define __AFXWINVERAPI_H__

#pragma once

#include <__atlmfc_core.h>
#pragma warning(push)
#pragma warning(disable : _ATLMFC_DISABLED_WARNINGS)

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, off)
#endif

#ifdef _AFX_PACKING
#pragma pack(push, _AFX_PACKING)
#endif

// This file contains declarations of wrappers for methods used
// in MFC that are only available in later versions of Windows.

// When the minimum supported version of Windows is increased, the
// implementations of these methods just call the real Windows APIs.

// Minimum supported versions of Windows:
// Windows XP SP2 for x86 and x64, Windows 8 for ARM

#if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define _MFC_NTDDI_MIN NTDDI_WINXPSP2
#else
#define _MFC_NTDDI_MIN NTDDI_WIN8
#endif

// Use this macro for loading a local cached function from a DLL that is known to be loaded (e.g. KERNEL32)
#define IFDYNAMICGETCACHEDFUNCTIONFORMFC(libraryname, functionname, functionpointer) \
	static volatile auto functionpointer##_cache = reinterpret_cast<decltype(::functionname)*>(NULL); \
	auto functionpointer = reinterpret_cast<decltype(::functionname)*>(functionpointer##_cache); \
	if (functionpointer == reinterpret_cast<decltype(::functionname)*>(NULL)) \
	{ \
		HMODULE hLibrary = ::LoadLibraryW(libraryname); \
		if (hLibrary != NULL) \
		{ \
			functionpointer = reinterpret_cast<decltype(::functionname)*>(::GetProcAddress(hLibrary, #functionname)); \
			functionpointer##_cache = reinterpret_cast<decltype(::functionname)*>(::EncodePointer((PVOID)functionpointer)); \
		} \
	} \
	else \
	{ \
		functionpointer = reinterpret_cast<decltype(::functionname)*>(::DecodePointer((PVOID)functionpointer)); \
	} \
	if (functionpointer != reinterpret_cast<decltype(::functionname)*>(NULL))

// Use this macro for loading a local cached function from a DLL that may not already be loaded (e.g. DWMAPI)
#define IFDYNAMICLOADCACHEDFUNCTIONFORMFC(libraryname, functionname, functionpointer) \
	static volatile auto functionpointer##_cache = reinterpret_cast<decltype(::functionname)*>(NULL); \
	auto functionpointer = reinterpret_cast<decltype(::functionname)*>(functionpointer##_cache); \
	if (functionpointer == reinterpret_cast<decltype(::functionname)*>(NULL)) \
	{ \
		HINSTANCE hLibrary = AtlLoadSystemLibraryUsingFullPath(libraryname); \
		if (hLibrary != NULL) \
		{ \
			functionpointer = reinterpret_cast<decltype(::functionname)*>(::GetProcAddress(hLibrary, #functionname)); \
			functionpointer##_cache = reinterpret_cast<decltype(::functionname)*>(::EncodePointer((PVOID)functionpointer)); \
		} \
	} \
	else \
	{ \
		functionpointer = reinterpret_cast<decltype(::functionname)*>(::DecodePointer((PVOID)functionpointer)); \
	} \
	if (functionpointer != reinterpret_cast<decltype(::functionname)*>(NULL))

// Wrappers for Win32 APIs that are only supported on higher versions of Windows
int _AfxGetLocaleInfoEx(LPCWSTR lpLocaleName, LCTYPE LCType, LPWSTR lpLCData, int cchData);
_Success_(return == TRUE)
BOOL _AfxGetThreadPreferredUILanguages(
	DWORD dwFlags,
	_Out_ PULONG pulNumLanguages,
	_Out_writes_to_opt_(*pcchLanguagesBuffer, *pcchLanguagesBuffer) PZZWSTR pwszLanguagesBuffer,
	_When_(pwszLanguagesBuffer == NULL, _Out_)
	_When_(pwszLanguagesBuffer != NULL, _Inout_) PULONG pcchLanguagesBuffer);

HRESULT _AfxRegisterApplicationRestart(PCWSTR pwzCommandline, DWORD dwFlags);
HRESULT _AfxRegisterApplicationRecoveryCallback(APPLICATION_RECOVERY_CALLBACK pRecoveryCallback, PVOID pvParameter, DWORD dwPingInterval, DWORD dwFlags);
HRESULT _AfxApplicationRecoveryInProgress(PBOOL pbCanceled);
VOID    _AfxApplicationRecoveryFinished(BOOL bSuccess);

BOOL _AfxChangeWindowMessageFilter(UINT message, DWORD dwFlag);

HRESULT _AfxSHCreateItemFromParsingName(_In_ PCWSTR pszPath, _In_opt_ IBindCtx *pbc, _In_ REFIID riid, _Out_ void **ppv);
HRESULT _AfxSHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath);

BOOL _AfxInitNetworkAddressControl();

HRESULT _AfxDrawThemeTextEx(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwFlags, LPRECT pRect, const DTTOPTS *pOptions);

#if (NTDDI_VERSION >= NTDDI_VISTA)
HRESULT      _AfxBufferedPaintInit(void);
HRESULT      _AfxBufferedPaintUnInit(void);
_Success_(return != 0)
HPAINTBUFFER _AfxBeginBufferedPaint(HDC hdcTarget, const RECT *prcTarget, BP_BUFFERFORMAT dwFormat, _In_opt_ BP_PAINTPARAMS *pPaintParams, _Out_ HDC *phdc);
HRESULT      _AfxEndBufferedPaint(HPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget);
#endif // (NTDDI_VERSION >= NTDDI_VISTA)

BOOL    _AfxDwmDefWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
HRESULT _AfxDwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset);
HRESULT _AfxDwmIsCompositionEnabled(BOOL *pfEnabled);
HRESULT _AfxDwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT _AfxDwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags);
HRESULT _AfxDwmInvalidateIconicBitmaps( HWND hwnd);
HRESULT _AfxDwmSetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags);

HRESULT _AfxPSGetPropertyDescriptionListFromString(LPCWSTR pszPropList, REFIID riid, void **ppv);

BOOL    _AfxIsTaskDialogSupported();
#if (NTDDI_VERSION >= NTDDI_VISTA)
HRESULT _AfxTaskDialogIndirect(_In_ const TASKDIALOGCONFIG *pTaskConfig, _Out_opt_ int *pnButton, _Out_opt_ int *pnRadioButton, _Out_opt_ BOOL *pfVerificationFlagChecked);
#endif // (NTDDI_VERSION >= NTDDI_VISTA)

#ifdef _AFX_PACKING
#pragma pack(pop)
#endif

#undef AFX_DATA
#define AFX_DATA

#ifdef _AFX_MINREBUILD
#pragma component(minrebuild, on)
#endif

// _ATLMFC_DISABLED_WARNINGS
#pragma warning(pop)

#endif // __AFXWINVERAPI_H__

/////////////////////////////////////////////////////////////////////////////
