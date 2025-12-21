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

#ifndef __WCEALT_INL__ 
#define __WCEALT_INL__

#ifdef _AFXWIN_INLINE

_AFXWIN_INLINE BOOL AFXAPI wce_LockWindowUpdate(HWND hWnd) 
{
	return TRUE;
}

_AFXWIN_INLINE UINT AFXAPI wce_IsDlgButtonChecked(HWND hDlg, int nIDButton)
{
	return (UINT)::SendMessage(::GetDlgItem(hDlg, nIDButton), BM_GETCHECK, 0L, 0L);
}

_AFXWIN_INLINE BOOL AFXAPI wce_SetDlgItemText(HWND hDlg, int nIDDlgItem, LPCWSTR lpString)
{
	return ::SetWindowText(GetDlgItem(hDlg,nIDDlgItem),lpString);
}

_AFXWIN_INLINE LONG AFXAPI wce_SendDlgItemMessage(HWND hDlg, int nIDDlgItem, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return (long)SendMessage(GetDlgItem(hDlg, nIDDlgItem), uMsg, wParam, lParam);
}

_AFXWIN_INLINE UINT AFXAPI wce_GetDlgItemText(HWND hDlg, int nIDDlgItem, LPWSTR lpString, int nMaxCount)
{
    return ::GetWindowText(GetDlgItem(hDlg, nIDDlgItem), lpString, nMaxCount);
}

_AFXWIN_INLINE void AFXAPI wce_CheckDlgButton(HWND hDlg, int nIDButton, UINT uCheck)
{
	::SendMessage(::GetDlgItem(hDlg, nIDButton), BM_SETCHECK, uCheck, 0);
}

_AFXWIN_INLINE BOOL AFXAPI wce_IsIconic(HWND hWnd)
{
	return !IsWindowEnabled(hWnd);
}

_AFXWIN_INLINE HWND AFXAPI wce_GetTopWindow(HWND hWnd)
{
	return ::GetWindow(hWnd, GW_CHILD);
}

_AFXWIN_INLINE HWND AFXAPI wce_GetNextWindow(HWND hWnd, UINT nDirection)
{ 
	return ::GetWindow(hWnd, nDirection); 
}

_AFXWIN_INLINE HWND AFXAPI wce_GetDesktopWindow()
{
	return HWND_DESKTOP;
}

_AFXWIN_INLINE HWND AFXAPI wce_GetLastActivePopup(HWND hWnd)
{
	return ::GetActiveWindow();
}

_AFXWIN_INLINE BOOL AFXAPI wce_ScrollWindow(HWND hWnd, int xAmount, int yAmount, LPCRECT lpRect, LPCRECT lpClipRect)
{
	return (ERROR != ::ScrollWindowEx(hWnd, xAmount, yAmount, lpRect, lpClipRect, 
                                       NULL, NULL, SW_INVALIDATE | SW_ERASE));
}

_AFXWIN_INLINE BOOL AFXAPI wce_IsMenu(HMENU hMenu)
{
	MENUITEMINFO mii;
	memset((char*)&mii, 0, sizeof(MENUITEMINFO));
	mii.cbSize = sizeof(MENUITEMINFO);
	if (GetMenuItemInfo(hMenu, 0, TRUE, &mii))
		return TRUE;
	else
	{
		// consider dynamically creating a popup menu. 
		// The first menu item has not been inserted. Then
		// we just check the handle
		if (hMenu != NULL)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}

_AFXWIN_INLINE BOOL AFXAPI wce_TrackPopupMenu(HMENU hMenu, UINT uFlags, int x, int y, int nReserved, HWND hWnd, CONST RECT *prcRect)
{
	return(::TrackPopupMenuEx(hMenu, uFlags, x, y, hWnd, NULL));
}

_AFXWIN_INLINE BOOL AFXAPI wce_InvertRect(HDC hdc, const RECT* lprc)
{
	return ::PatBlt(hdc, lprc->left, lprc->top,
						 lprc->right - lprc->left,
						 lprc->bottom - lprc->top, DSTINVERT);
}

_AFXWIN_INLINE int AFXAPI wce_GetMapMode(HDC hdc)
{
	if (hdc == NULL) 
		return 0;
	return MM_TEXT;	// the only mapping mode 
}

_AFXWIN_INLINE BOOL AFXAPI wce_GetViewportOrgEx(HDC hdc, LPPOINT lpPoint)
{ 
	if (hdc == NULL)
		return FALSE; 
	lpPoint->x = 0;		// origin is always (0,0) 
	lpPoint->y = 0;
	return TRUE;
}

_AFXWIN_INLINE BOOL AFXAPI wce_GetViewportExtEx(HDC hdc, LPSIZE lpSize)
{ 
	if (hdc == NULL)
		return FALSE; 
	lpSize->cx = 1;		// extent is always 1,1 
	lpSize->cy = 1;
	return TRUE;
}

_AFXWIN_INLINE BOOL AFXAPI wce_GetWindowOrgEx(HDC hdc, LPPOINT lpPoint)
{ 
	if (hdc == NULL)
		return FALSE; 
	lpPoint->x = 0;		// origin is always (0,0) 
	lpPoint->y = 0;
	return TRUE;
}

_AFXWIN_INLINE BOOL AFXAPI wce_GetWindowExtEx(HDC hdc, LPSIZE lpSize)
{ 
	if (hdc == NULL)
		return FALSE; 
	lpSize->cx = 1;		// extent is always 1,1 
	lpSize->cy = 1;
	return TRUE;
}

_AFXWIN_INLINE BOOL AFXAPI wce_DPtoLP(HDC hdc, LPPOINT lpPoints, int nCount)
{ 
	if (hdc == NULL)
		return FALSE;
	return TRUE; // DP = LP always 
}

_AFXWIN_INLINE BOOL AFXAPI wce_LPtoDP(HDC hdc, LPPOINT lpPoints, int nCount)
{ 
	if (hdc == NULL)
		return FALSE;
	return TRUE; // DP = LP always 
}

_AFXWIN_INLINE BOOL AFXAPI wce_DrawIcon(HDC hDC, int X, int Y, HICON hIcon)
{
	return ::DrawIconEx(hDC, X, Y, hIcon, 0, 0, 0,NULL, DI_NORMAL);
}

_AFXWIN_INLINE HCURSOR AFXAPI wce_LoadCursor(HINSTANCE hInstance,LPCWSTR lpCursorName)
{
	return ::LoadCursorW(hInstance, lpCursorName);
}

_AFXWIN_INLINE HBITMAP AFXAPI wce_CreateBitmapIndirect(LPBITMAP lpBitmap)
{
	return ::CreateBitmap(lpBitmap->bmWidth,
						  lpBitmap->bmHeight,
						  lpBitmap->bmPlanes,
						  lpBitmap->bmBitsPixel,
						  lpBitmap->bmBits);
}

_AFXWIN_INLINE void * AFXAPI wce_memcpy(void *dst, const void *src, size_t size)
{
	return memmove(dst, src, size);
}

#endif //_AFXWIN_INLINE

#endif // __WCEALT_INL__
