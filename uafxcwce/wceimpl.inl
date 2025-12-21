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

#ifndef __WCEIMPL_INL__
#define __WCEIMPL_INL__

#ifdef _AFXWIN_INLINE

_AFXWIN_INLINE HINSTANCE AFXAPI wce_GetModuleHandleW(LPCWSTR lpModuleName)
{
#if (_WIN32_WCE >= 210)
	return GetModuleHandleW(lpModuleName);
#else // _WIN32_WCE
	HINSTANCE hInst = LoadLibraryW(lpModuleName);
	if(hInst)
		FreeLibrary(hInst);
	return hInst;
#endif // _WIN32_WCE
}

_AFXWIN_INLINE HICON AFXAPI wce_ExtractIcon(HINSTANCE hInst, LPCWSTR lpszExeFileName, UINT nIconIndex)
{
	return (HICON)::ExtractIconEx(lpszExeFileName, nIconIndex, NULL, NULL, 1);
}

_AFXWIN_INLINE int AFXAPI wce_MulDiv(int nNumber, int nNumerator, int nDenominator)
{
	__int64 x;

	x = (__int64)nNumber * (__int64)nNumerator;
	x /= (__int64)nDenominator;

	return (int)x;
}

_AFXWIN_INLINE HINSTANCE AFXAPI wce_GetModuleHandleA(LPCSTR lpModuleName)
{
	HINSTANCE hInst;

	TCHAR *szwBuf = wce_AsciiToWide(lpModuleName);
	hInst = wce_GetModuleHandleW(szwBuf);
	delete[] szwBuf;

	return hInst;
}

_AFXWIN_INLINE short AFXAPI wce_GetFileTitle(LPCTSTR lpszFile, LPTSTR lpszTitle, WORD cbBuf)
{
	return wce_GetFileTitleW(lpszFile, lpszTitle, cbBuf);
}

_AFXWIN_INLINE HGLOBAL AFXAPI wce_GlobalAlloc(UINT uFlags, DWORD dwBytes)
{
	UINT uLocalFlags = 0;

	if (uFlags & GMEM_ZEROINIT)
		uLocalFlags |= LMEM_ZEROINIT;

	if (uFlags & GMEM_FIXED)
		uLocalFlags |= LMEM_FIXED;

	if (uFlags & GMEM_MOVEABLE)
		uLocalFlags |= LMEM_MOVEABLE;


	return (HGLOBAL)::LocalAlloc(uLocalFlags, (UINT)dwBytes);
}

_AFXWIN_INLINE HGLOBAL AFXAPI wce_GlobalFree(HGLOBAL hMem)
{
	return ::LocalFree((HLOCAL)hMem);
}
 
_AFXWIN_INLINE HGLOBAL AFXAPI wce_GlobalReAlloc(HGLOBAL hMem, DWORD dwBytes, UINT uFlags)
{
	UINT uLocalFlags = 0;

	if (uFlags & GMEM_ZEROINIT)
		uLocalFlags |= LMEM_ZEROINIT;

	if (uFlags & GMEM_FIXED)
		uLocalFlags |= LMEM_FIXED;

	if (uFlags & GMEM_MOVEABLE)
		uLocalFlags |= LMEM_MOVEABLE;

	return (HGLOBAL)::LocalReAlloc((HLOCAL)hMem, (UINT)dwBytes, uLocalFlags);
}

_AFXWIN_INLINE DWORD AFXAPI wce_GlobalSize(HGLOBAL hMem)
{
	return (DWORD)::LocalSize((HLOCAL)hMem);
}

_AFXWIN_INLINE LPVOID AFXAPI wce_GlobalLock(HGLOBAL hMem)
{
	return LocalLock((HLOCAL)hMem);
}

_AFXWIN_INLINE BOOL AFXAPI wce_GlobalUnlock(HGLOBAL hMem)
{
	return LocalUnlock((HLOCAL)hMem);
}

_AFXWIN_INLINE HGLOBAL AFXAPI wce_GlobalHandle(LPCVOID pMem)
{
	return (HGLOBAL)LocalHandle(pMem);
}

_AFXWIN_INLINE UINT AFXAPI wce_GlobalFlags(HGLOBAL hMem)
{
	return LocalFlags((HLOCAL)hMem);
}

_AFXWIN_INLINE DWORD AFXAPI wce_GetVersion()
{
	return 4;
}

_AFXWIN_INLINE void* AFXAPI wce_calloc(size_t num, size_t size)
{
	void *ptr = malloc(num*size);
	if(ptr)
		memset(ptr, 0, num*size);
	return ptr;
}

_AFXWIN_INLINE void* AFXAPI wce__expand(void* pvMemBlock, size_t iSize)
{
	return realloc(pvMemBlock, iSize);
}

extern "C" void  __cdecl exit(int);	   
_AFXWIN_INLINE void AFXAPI wce_abort()
{
	exit(3);																	 
}

_AFXWIN_INLINE unsigned long AFXAPI wce__beginthreadex(void *security, unsigned stack_size, 
                                  unsigned (__stdcall *start_address)(void *),
                                  void *arglist, unsigned initflag, 
                                  unsigned *thrdaddr)
{
	return (unsigned long)CreateThread((LPSECURITY_ATTRIBUTES)security, 
		                                (DWORD)stack_size,
		                                (LPTHREAD_START_ROUTINE)start_address, 
									    (LPVOID)arglist, 
									    (DWORD)initflag | CREATE_SUSPENDED, 
									    (LPDWORD)thrdaddr);
}

_AFXWIN_INLINE void AFXAPI wce__endthreadex(unsigned nExitCode)
{
	ExitThread((DWORD)nExitCode);
}

_AFXWIN_INLINE BOOL AFXAPI wce_GetCursorPos(LPPOINT lpPoint)
{
#if (_WIN32_WCE >= 210)
	return ::GetCursorPos(lpPoint);
#else // _WIN32_WCE
	return ::GetCaretPos(lpPoint);
#endif // _WIN32_WCE
}

_AFXWIN_INLINE HDWP AFXAPI wce_BeginDeferWindowPos(int nNumWindows)
{
	return (HDWP)1; // a fake handle
}

_AFXWIN_INLINE HDWP AFXAPI wce_DeferWindowPos(HDWP hWinPosInfo, HWND hWnd, HWND hWndInsertAfter, 
                        int x, int y, int cx, int cy, UINT uFlags)
{
	::SetWindowPos(hWnd, hWndInsertAfter, x, y, cx, cy,	uFlags);
	return (HDWP)1; // a fake handle
}

_AFXWIN_INLINE BOOL AFXAPI wce_EndDeferWindowPos(HDWP hWinPosInfo)
{	
	return TRUE;
}

#endif //_AFXWIN_INLINE

#endif  // __WCEIMPL_INL__
