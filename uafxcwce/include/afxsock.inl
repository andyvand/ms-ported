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

// Inlines for AFXSOCK.H

#ifdef _AFXSOCK_INLINE

#if defined(_WIN32_WCE)
#undef PASCAL
#define PASCAL
#endif // _WIN32_WCE
_AFXSOCK_INLINE CAsyncSocket::operator SOCKET() const
	{ return m_hSocket; }
_AFXSOCK_INLINE BOOL CAsyncSocket::GetPeerName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != getpeername(m_hSocket, lpSockAddr, lpSockAddrLen)); }
_AFXSOCK_INLINE BOOL CAsyncSocket::GetSockName(SOCKADDR* lpSockAddr, int* lpSockAddrLen)
	{ return (SOCKET_ERROR != getsockname(m_hSocket, lpSockAddr, lpSockAddrLen)); }
_AFXSOCK_INLINE BOOL CAsyncSocket::SetSockOpt(int nOptionName, const void* lpOptionValue, int nOptionLen, int nLevel)
	{ return (SOCKET_ERROR != setsockopt(m_hSocket, nLevel, nOptionName, (LPCSTR)lpOptionValue, nOptionLen)); }
_AFXSOCK_INLINE BOOL CAsyncSocket::GetSockOpt(int nOptionName, void* lpOptionValue, int* lpOptionLen, int nLevel)
	{ return (SOCKET_ERROR != getsockopt(m_hSocket, nLevel, nOptionName, (LPSTR)lpOptionValue, lpOptionLen)); }
_AFXSOCK_INLINE CAsyncSocket* PASCAL CAsyncSocket::FromHandle(SOCKET hSocket)
	{ return CAsyncSocket::LookupHandle(hSocket, FALSE); }
_AFXSOCK_INLINE int PASCAL CAsyncSocket::GetLastError()
	{ return WCE_IF(::GetLastError(), WSAGetLastError()); }
_AFXSOCK_INLINE BOOL CAsyncSocket::Bind(const SOCKADDR* lpSockAddr, int nSockAddrLen)
	{ return (SOCKET_ERROR != bind(m_hSocket, lpSockAddr, nSockAddrLen)); }
_AFXSOCK_INLINE BOOL CAsyncSocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
	{ return ConnectHelper(lpSockAddr, nSockAddrLen); }
_AFXSOCK_INLINE BOOL CAsyncSocket::IOCtl(long lCommand, DWORD* lpArgument)
	{ return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); }
_AFXSOCK_INLINE BOOL CAsyncSocket::Listen(int nConnectionBacklog)
	{ return (SOCKET_ERROR != listen(m_hSocket, nConnectionBacklog)); }
_AFXSOCK_INLINE int CAsyncSocket::ReceiveFrom(void* lpBuf, int nBufLen, SOCKADDR* lpSockAddr, int* lpSockAddrLen, int nFlags)
	{ return ReceiveFromHelper(lpBuf, nBufLen, lpSockAddr, lpSockAddrLen, nFlags); }
_AFXSOCK_INLINE BOOL CAsyncSocket::ShutDown(int nHow)
	{ return (SOCKET_ERROR != shutdown(m_hSocket,nHow)); }
_AFXSOCK_INLINE int CAsyncSocket::SendTo(const void* lpBuf, int nBufLen, const SOCKADDR* lpSockAddr, int nSockAddrLen, int nFlags)
	{ return SendToHelper(lpBuf, nBufLen, lpSockAddr, nSockAddrLen, nFlags); }

_AFXSOCK_INLINE BOOL CSocket::Create(UINT nSocketPort, int nSocketType, LPCTSTR lpszSocketAddress)
	{ return CAsyncSocket::Create(nSocketPort, nSocketType, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, lpszSocketAddress); }
_AFXSOCK_INLINE BOOL CSocket::IsBlocking()
	{ return (m_pbBlocking != NULL); }
_AFXSOCK_INLINE CSocket* PASCAL CSocket::FromHandle(SOCKET hSocket)
	{ return (CSocket*)CAsyncSocket::LookupHandle(hSocket, FALSE); }
_AFXSOCK_INLINE BOOL CSocket::Attach(SOCKET hSocket)
		{ return CAsyncSocket::Attach(hSocket); }

#if defined(_WIN32_WCE)
#undef PASCAL
#define PASCAL __stdcall
#endif // _WIN32_WCE
#endif //_AFXSOCK_INLINE
