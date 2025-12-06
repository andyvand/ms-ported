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
#include <afxwinverapi.h>

// This file contains implementations of wrappers for methods used
// in MFC that are only available in later versions of Windows.

// When the minimum supported version of Windows is increased, the
// implementations of these methods just call the real Windows APIs.

#if _MFC_NTDDI_MIN >= NTDDI_VISTA
#pragma comment(lib, "propsys.lib")
#endif

#if _MFC_NTDDI_MIN >= NTDDI_WIN7
#pragma comment(lib, "dwmapi.lib")
#endif

int _AfxGetLocaleInfoEx(LPCWSTR lpLocaleName, LCTYPE LCType, LPWSTR lpLCData, int cchData)
{
	// use GetLocaleInfoEx if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", GetLocaleInfoEx, pfGetLocaleInfoEx)
	{
		return (*pfGetLocaleInfoEx)(lpLocaleName, LCType, lpLCData, cchData);
	}

	// ...otherwise fall back to using GetLocaleInfo.
	return GetLocaleInfoW(_AtlDownlevelLocaleNameToLCID(lpLocaleName), LCType, lpLCData, cchData);
}

#define COPYLCIDSTRINGTOARRAY(lcid, str, strarray, strarraylen, currentlen, currentpos, pnumlangs) \
	_AtlDownlevelLCIDToLocaleName(lcid, str, _countof(str)); \
	currentlen = wcslen(str); \
	wcscpy_s(strarray + currentpos, strarraylen - currentpos, str); \
	currentpos += currentlen + 1; \
	(*pnumlangs)++;

_Success_(return == TRUE)
BOOL _AfxGetThreadPreferredUILanguages(
	DWORD dwFlags,
	_Out_ PULONG pulNumLanguages,
	_Out_writes_to_opt_(*pcchLanguagesBuffer, *pcchLanguagesBuffer) PZZWSTR pwszLanguagesBuffer,
	_When_(pwszLanguagesBuffer == NULL, _Out_)
	_When_(pwszLanguagesBuffer != NULL, _Inout_) PULONG pcchLanguagesBuffer)
{
	// use GetThreadPreferredUILanguages if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", GetThreadPreferredUILanguages, pfGetThreadPreferredUILanguages)
	{
		return (*pfGetThreadPreferredUILanguages)(dwFlags, pulNumLanguages, pwszLanguagesBuffer, pcchLanguagesBuffer);
	}

	// ...otherwise fall back to using other methods to get the UI language.
	// Optional null values for getting numLanguages in `GetThreadPreferredUILanguages` not supported.
	_Analysis_assume_(pwszLanguagesBuffer != NULL);

	LANGID langid = 0;
	int nPrimaryLang = 0, nSubLang = 0;
	LCID lcid = 0;
	WCHAR wszLocale[7];
	size_t nCurrentLen = 0, nCurrentPos = 0;
	*pulNumLanguages = 0;

	// First get the user's UI languages
	langid = GetUserDefaultUILanguage();
	nPrimaryLang = PRIMARYLANGID(langid);
	nSubLang = SUBLANGID(langid);

	lcid = MAKELCID(MAKELANGID(nPrimaryLang, nSubLang), SORT_DEFAULT);
	COPYLCIDSTRINGTOARRAY(lcid, wszLocale, pwszLanguagesBuffer, *pcchLanguagesBuffer, nCurrentLen, nCurrentPos, pulNumLanguages);

	lcid = MAKELCID(MAKELANGID(nPrimaryLang, SUBLANG_NEUTRAL), SORT_DEFAULT);
	COPYLCIDSTRINGTOARRAY(lcid, wszLocale, pwszLanguagesBuffer, *pcchLanguagesBuffer, nCurrentLen, nCurrentPos, pulNumLanguages);

	// Then get the system's default UI languages
	langid = GetSystemDefaultUILanguage();
	nPrimaryLang = PRIMARYLANGID(langid);
	nSubLang = SUBLANGID(langid);

	lcid = MAKELCID(MAKELANGID(nPrimaryLang, nSubLang), SORT_DEFAULT);
	COPYLCIDSTRINGTOARRAY(lcid, wszLocale, pwszLanguagesBuffer, *pcchLanguagesBuffer, nCurrentLen, nCurrentPos, pulNumLanguages);

	lcid = MAKELCID(MAKELANGID(nPrimaryLang, SUBLANG_NEUTRAL), SORT_DEFAULT);
	COPYLCIDSTRINGTOARRAY(lcid, wszLocale, pwszLanguagesBuffer, *pcchLanguagesBuffer, nCurrentLen, nCurrentPos, pulNumLanguages);

	lcid = LOCALE_SYSTEM_DEFAULT;
	COPYLCIDSTRINGTOARRAY(lcid, wszLocale, pwszLanguagesBuffer, *pcchLanguagesBuffer, nCurrentLen, nCurrentPos, pulNumLanguages);

	// Ensure that the list is double-NULL-terminated
	pwszLanguagesBuffer[nCurrentPos] = 0;

	return TRUE;
}

HRESULT _AfxRegisterApplicationRestart(PCWSTR pwzCommandline, DWORD dwFlags)
{
	// use RegisterApplicationRestart if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", RegisterApplicationRestart, pfRegisterApplicationRestart)
	{
		return (*pfRegisterApplicationRestart)(pwzCommandline, dwFlags);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxRegisterApplicationRecoveryCallback(APPLICATION_RECOVERY_CALLBACK pRecoveryCallback, PVOID pvParameter, DWORD dwPingInterval, DWORD dwFlags)
{
	// use RegisterApplicationRecoveryCallback if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", RegisterApplicationRecoveryCallback, pfRegisterApplicationRecoveryCallback)
	{
		return (*pfRegisterApplicationRecoveryCallback)(pRecoveryCallback, pvParameter, dwPingInterval, dwFlags);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxApplicationRecoveryInProgress(PBOOL pbCanceled)
{
	// use ApplicationRecoveryInProgress if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", ApplicationRecoveryInProgress, pfApplicationRecoveryInProgress)
	{
		return (*pfApplicationRecoveryInProgress)(pbCanceled);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

VOID _AfxApplicationRecoveryFinished(BOOL bSuccess)
{
	// use ApplicationRecoveryFinished if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"kernel32.dll", ApplicationRecoveryFinished, pfApplicationRecoveryFinished)
	{
		return (*pfApplicationRecoveryFinished)(bSuccess);
	}

	// ...otherwise return because there is no fallback.
}

BOOL _AfxChangeWindowMessageFilter(UINT message, DWORD dwFlag)
{
	// use ChangeWindowMessageFilter if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"user32.dll", ChangeWindowMessageFilter, pfChangeWindowMessageFilter)
	{
		return (*pfChangeWindowMessageFilter)(message, dwFlag);
	}

	// ...otherwise return failure because the API is only needed for Vista+ feature support.
	return FALSE;
}

HRESULT _AfxSHCreateItemFromParsingName(_In_ PCWSTR pszPath, _In_opt_ IBindCtx *pbc, _In_ REFIID riid, _Out_ void **ppv)
{
	// use SHCreateItemFromParsingName if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"shell32.dll", SHCreateItemFromParsingName, pfSHCreateItemFromParsingName)
	{
		return (*pfSHCreateItemFromParsingName)(pszPath, pbc, riid, ppv);
	}

	// ...otherwise return failure because the API is only needed for Vista+ feature support.
	return E_FAIL;
}

HRESULT _AfxSHGetKnownFolderPath(REFKNOWNFOLDERID rfid, DWORD dwFlags, HANDLE hToken, PWSTR *ppszPath)
{
	// use SHGetKnownFolderPath if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"shell32.dll", SHGetKnownFolderPath, pfSHGetKnownFolderPath)
	{
		return (*pfSHGetKnownFolderPath)(rfid, dwFlags, hToken, ppszPath);
	}

	// ...otherwise return failure because the API is only needed for Vista+ feature support.
	return E_FAIL;
}

BOOL _AfxInitNetworkAddressControl()
{
	// use InitNetworkAddressControl if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"shell32.dll", InitNetworkAddressControl, pfInitNetworkAddressControl)
	{
		return (*pfInitNetworkAddressControl)();
	}

	// ...otherwise return failure because there is no fallback.
	return FALSE;
}

HRESULT _AfxDrawThemeTextEx(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwFlags, LPRECT pRect, const DTTOPTS *pOptions)
{
	// use DrawThemeTextEx if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"uxtheme.dll", DrawThemeTextEx, pfDrawThemeTextEx)
	{
		return (*pfDrawThemeTextEx)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwFlags, pRect, pOptions);
	}

	// ...otherwise fall back to using DrawThemeText.
	return DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwFlags, 0, pRect);
}

HRESULT _AfxBufferedPaintInit(void)
{
	// use BufferedPaintInit if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"uxtheme.dll", BufferedPaintInit, pfBufferedPaintInit)
	{
		return (*pfBufferedPaintInit)();
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxBufferedPaintUnInit(void)
{
	// use BufferedPaintUnInit if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"uxtheme.dll", BufferedPaintUnInit, pfBufferedPaintUnInit)
	{
		return (*pfBufferedPaintUnInit)();
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

_Success_(return != 0)
HPAINTBUFFER _AfxBeginBufferedPaint(HDC hdcTarget, const RECT *prcTarget, BP_BUFFERFORMAT dwFormat, _In_opt_ BP_PAINTPARAMS *pPaintParams, _Out_ HDC *phdc)
{
	// use BeginBufferedPaint if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"uxtheme.dll", BeginBufferedPaint, pfBeginBufferedPaint)
	{
		return (*pfBeginBufferedPaint)(hdcTarget, prcTarget, dwFormat, pPaintParams, phdc);
	}

	// ...otherwise return failure because there is no fallback.
	return NULL;
}

HRESULT _AfxEndBufferedPaint(HPAINTBUFFER hBufferedPaint, BOOL fUpdateTarget)
{
	// use EndBufferedPaint if it is available (only on Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"uxtheme.dll", EndBufferedPaint, pfEndBufferedPaint)
	{
		return (*pfEndBufferedPaint)(hBufferedPaint, fUpdateTarget);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

BOOL _AfxDwmDefWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
	// use DwmDefWindowProc if it is available (only on Vista+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmDefWindowProc, pfDwmDefWindowProc)
	{
		return (*pfDwmDefWindowProc)(hwnd, msg, wParam, lParam, plResult);
	}

	// ...otherwise return failure because there is no fallback.
	return FALSE;
}

HRESULT _AfxDwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS *pMarInset)
{
	// use DwmExtendFrameIntoClientArea if it is available (only on Vista+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmExtendFrameIntoClientArea, pfDwmExtendFrameIntoClientArea)
	{
		return (*pfDwmExtendFrameIntoClientArea)(hWnd, pMarInset);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxDwmIsCompositionEnabled(BOOL *pfEnabled)
{
	// use DwmIsCompositionEnabled if it is available (only on Vista+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmIsCompositionEnabled, pfDwmIsCompositionEnabled)
	{
		return (*pfDwmIsCompositionEnabled)(pfEnabled);
	}

	// ...otherwise set flag to indicate that composition is disabled.
	*pfEnabled = FALSE;
	return S_OK;
}

HRESULT _AfxDwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
	// use DwmSetWindowAttribute if it is available (only on Vista+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmSetWindowAttribute, pfDwmSetWindowAttribute)
	{
		return (*pfDwmSetWindowAttribute)(hwnd, dwAttribute, pvAttribute, cbAttribute);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxDwmSetIconicThumbnail(HWND hwnd, HBITMAP hbmp, DWORD dwSITFlags)
{
	// use DwmSetIconicThumbnail if it is available (only on Windows 7+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmSetIconicThumbnail, pfDwmSetIconicThumbnail)
	{
		return (*pfDwmSetIconicThumbnail)(hwnd, hbmp, dwSITFlags);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxDwmInvalidateIconicBitmaps(HWND hwnd)
{
	// use DwmInvalidateIconicBitmaps if it is available (only on Windows 7+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmInvalidateIconicBitmaps, pfDwmInvalidateIconicBitmaps)
	{
		return (*pfDwmInvalidateIconicBitmaps)(hwnd);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxDwmSetIconicLivePreviewBitmap(HWND hwnd, HBITMAP hbmp, POINT *pptClient, DWORD dwSITFlags)
{
	// use DwmSetIconicLivePreviewBitmap if it is available (only on Windows 7+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"dwmapi.dll", DwmSetIconicLivePreviewBitmap, pfDwmSetIconicLivePreviewBitmap)
	{
		return (*pfDwmSetIconicLivePreviewBitmap)(hwnd, hbmp, pptClient, dwSITFlags);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

HRESULT _AfxPSGetPropertyDescriptionListFromString(LPCWSTR pszPropList, REFIID riid, void **ppv)
{
	// use PSGetPropertyDescriptionListFromString if it is available (only on Vista+)...
	IFDYNAMICLOADCACHEDFUNCTIONFORMFC(L"propsys.dll", PSGetPropertyDescriptionListFromString, pfPSGetPropertyDescriptionListFromString)
	{
		return (*pfPSGetPropertyDescriptionListFromString)(pszPropList, riid, ppv);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}

BOOL _AfxIsTaskDialogSupported()
{
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"comctl32.dll", TaskDialogIndirect, pfTaskDialogIndirect)
	{
		return TRUE;
	}

	return FALSE;
}

HRESULT _AfxTaskDialogIndirect(_In_ const TASKDIALOGCONFIG *pTaskConfig, _Out_opt_ int *pnButton, _Out_opt_ int *pnRadioButton, _Out_opt_ BOOL *pfVerificationFlagChecked)
{
	// use TaskDialogIndirect if it is available (only on Windows Vista+)...
	IFDYNAMICGETCACHEDFUNCTIONFORMFC(L"comctl32.dll", TaskDialogIndirect, pfTaskDialogIndirect)
	{
		return (*pfTaskDialogIndirect)(pTaskConfig, pnButton, pnRadioButton, pfVerificationFlagChecked);
	}

	// ...otherwise return failure because there is no fallback.
	return E_FAIL;
}
