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

#ifndef __WCESOCK_H__
#define __WCESOCK_H__


// WM_SOCKET_NOTIFY and WM_SOCKET_DEAD are used internally by MFC's
// Windows sockets implementation.  For more information, see sockcore.cpp
#undef  WM_SOCKET_NOTIFY
#define WM_SOCKET_NOTIFY    0x0373   // defined in AFXPRIV.H

// Special global function to return the socket window belonging to the
// thread state.
HWND AfxGetCeSocketWindow();

//////////////////////////////////////////////////////////////////////////
//
class CCeSocket : public CSocket
{
	DECLARE_DYNAMIC(CCeSocket);

public:
	enum PURPOSE_E {FOR_LISTENING=0, FOR_DATA=1};

private:
	void   BeginThread( PURPOSE_E iPurpose=FOR_LISTENING );
	void   EndThreads();

	BOOL   IsAlive();
	BOOL   IsListening();
	BOOL   IsReadReady();
	BOOL   SetForRead();
    BOOL   SetForWrite();
    BOOL   SetForReadnWrite();
	BOOL   DoAccept();
	BOOL   DoConnect();
	BOOL   DoRead();

private:
	BOOL   SetNonblocking();
	BOOL   SetBlocking();

private:
	fd_set					m_ReadFds;
	fd_set					m_WriteFds;

	static CWinThread*		m_pListenThread;
	static CWinThread*		m_pDataThread;
	static LONG				m_iSocketCount;
	static CPtrList			m_PtrList;
	PURPOSE_E				m_iPurpose;
	BOOL					m_bQuit;
	BOOL					m_bConnectCalled;
	BOOL					m_bIsConnected;

public:
	CCeSocket( PURPOSE_E iPurpose=FOR_DATA );
	virtual ~CCeSocket();
	virtual BOOL Create( UINT nSocketPort=0 );

	// Overridable callbacks
	virtual void OnAccept( int nErrorCode )
		{ CSocket::OnAccept(nErrorCode); }
	virtual void OnConnect( int nErrorCode )
		{ CSocket::OnConnect(nErrorCode); }
	virtual BOOL ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen);
#if (_WIN32_WCE >= 300)
	virtual BOOL Accept(CAsyncSocket& rConnectedSocket,
		SOCKADDR* lpSockAddr = NULL, int* lpSockAddrLen = NULL);
#endif // _WIN32_WCE

	// Friends
	friend unsigned int  AFX_CDECL ListenThread( void* pvParams );
	friend unsigned int  AFX_CDECL DataThread( void* pvParams );

	friend class CSocket;
};


// Although WinCE 2.10 implements WSAStartup() and WCECleanup() as API's, we define them as macros for backward
// compatibility.
// update:
// Previously, CE’s Winsock 1.1 did not require WSAStartup to be called so MFC could get away with defining its own.
// For Winsock 2.0 WSAStartup has to be called so we have removed the MFC local definition.

//	#undef WSAStartup
//	#define WSAStartup(wVersionRequired,lpWSAData)	\
//			((lpWSAData)->wVersion=wVersionRequired,\
//			(lpWSAData)->wHighVersion=wVersionRequired,\
//			(lpWSAData)->szDescription[0]='\0',\
//			(lpWSAData)->szSystemStatus[0]='\0',\
//			(lpWSAData)->iMaxSockets=10,\
//			(lpWSAData)->iMaxUdpDg=0,\
//			(lpWSAData)->lpVendorInfo=NULL,(0))
//	#undef WSACleanup
//	#define WSACleanup()			(0)

#endif // __WCESOCK_H__
