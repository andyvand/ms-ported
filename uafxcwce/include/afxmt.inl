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

// Inlines for AFXMT.H

#ifdef _AFXMT_INLINE

_AFXMT_INLINE CSyncObject::operator HANDLE() const
	{ return m_hObject;}

#if !defined(_WIN32_WCE) || (defined(_WIN32_WCE) && (_WIN32_WCE >= 300))
_AFXMT_INLINE BOOL CSemaphore::Unlock()
	{ return Unlock(1, NULL); }
#endif // _WIN32_WCE

_AFXMT_INLINE BOOL CEvent::SetEvent()
	{ ASSERT(m_hObject != NULL); return ::SetEvent(m_hObject); }
_AFXMT_INLINE BOOL CEvent::PulseEvent()
	{ ASSERT(m_hObject != NULL); return ::PulseEvent(m_hObject); }
_AFXMT_INLINE BOOL CEvent::ResetEvent()
	{ ASSERT(m_hObject != NULL); return ::ResetEvent(m_hObject); }

_AFXMT_INLINE CSingleLock::~CSingleLock()
	{ Unlock(); }
_AFXMT_INLINE BOOL CSingleLock::IsLocked()
	{ return m_bAcquired; }

_AFXMT_INLINE BOOL CMultiLock::IsLocked(DWORD dwObject)
	{ ASSERT(dwObject >= 0 && dwObject < m_dwCount);
		 return m_bLockedArray[dwObject]; }

_AFXMT_INLINE CCriticalSection::CCriticalSection() : CSyncObject(NULL)
	{ ::InitializeCriticalSection(&m_sect); }
_AFXMT_INLINE CCriticalSection::operator CRITICAL_SECTION*()
	{ return (CRITICAL_SECTION*) &m_sect; }
_AFXMT_INLINE CCriticalSection::~CCriticalSection()
	{ ::DeleteCriticalSection(&m_sect); }
_AFXMT_INLINE BOOL CCriticalSection::Lock()
	{ ::EnterCriticalSection(&m_sect); return TRUE; }
_AFXMT_INLINE BOOL CCriticalSection::Lock(DWORD /* dwTimeout */)
	{ return Lock(); }
_AFXMT_INLINE BOOL CCriticalSection::Unlock()
	{ ::LeaveCriticalSection(&m_sect); return TRUE; }

#endif //_AFXMT_INLINE
