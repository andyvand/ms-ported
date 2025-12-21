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

#if !defined(_WIN32_WCE_NO_WINSOCK)

// WinCE: CCeSocket creates 1-2 worker threads, but they need to access
// data structures that are set up by the main thread (AfxSocketInit()).
#define _afxSockThreadState wce_GetSockThreadState()

AFX_MODULE_THREAD_STATE* wce_GetSockThreadState()
{
	static AFX_MODULE_THREAD_STATE* pMainThreadState = NULL;

	if(pMainThreadState == NULL)
		pMainThreadState = AfxGetModuleThreadState();
	return pMainThreadState;
}

HWND AfxGetCeSocketWindow()
{
	HWND hWndSocket = wce_GetSockThreadState()->m_hSocketWindow;
	ASSERT(hWndSocket != NULL);
	return hWndSocket;
}


unsigned int  AFX_CDECL ListenThread( void* pvParams );
unsigned int  AFX_CDECL DataThread( void* pvParams );

CWinThread*		CCeSocket::m_pListenThread = 0;
CWinThread*		CCeSocket::m_pDataThread   = 0;
LONG			CCeSocket::m_iSocketCount  = 0;					     
CPtrList		CCeSocket::m_PtrList;
CCriticalSection gCeSocketCS; 


CCeSocket::CCeSocket( PURPOSE_E iPurpose )
{
	ASSERT((iPurpose==FOR_LISTENING) || (iPurpose==FOR_DATA));

	m_iPurpose       = iPurpose;
    m_bConnectCalled = FALSE;
    m_bIsConnected   = FALSE;
	m_bQuit          = FALSE;

 	// Initialize the fd_set's.
 	// NOTE: Because m_ReadFds and m_WriteFds are global, we really don't want
 	//       to reset them everytime that the constructor gets called.
	FD_ZERO(&m_ReadFds);
    FD_ZERO(&m_WriteFds);

	// If this is a data socket, then add it to the list.

	if (m_iPurpose == FOR_DATA)
	{
		POSITION pos;

		gCeSocketCS.Lock();
		pos = m_PtrList.AddTail(this);
		ASSERT(pos != NULL);
		gCeSocketCS.Unlock();
	}

	// Increment socket counter.
	InterlockedIncrement(&m_iSocketCount);

	return;
}



void CCeSocket::BeginThread( PURPOSE_E iPurpose )
{
    // Start-up the listening or data thread.
	switch (iPurpose)  
	{
	case FOR_LISTENING:
		// If we haven't already done so, startup the listening thread.
		if (m_pListenThread == 0)
		{
			gCeSocketCS.Lock();
			m_pListenThread = AfxBeginThread( ListenThread, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
			ASSERT(m_pListenThread);
			m_pListenThread->m_bAutoDelete = FALSE;
			m_pListenThread->ResumeThread();
			gCeSocketCS.Unlock();
		}
		break;
	case FOR_DATA:
		// If we haven't already done so, startup the data thread.
		if (m_pDataThread == 0)
		{
			gCeSocketCS.Lock();
			m_pDataThread = AfxBeginThread( DataThread, (LPVOID)NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED );
			ASSERT(m_pDataThread);
			m_pDataThread->m_bAutoDelete = FALSE;
			m_pDataThread->ResumeThread();
			gCeSocketCS.Unlock();
		}
		break;
	default:
		ASSERT(FALSE);
	}
}




void CCeSocket::EndThreads()
{
	ASSERT(m_iSocketCount==0);

	// Conditionally, kill listening thread.
	if (m_pListenThread)
	{
		delete m_pListenThread;
		m_pListenThread = 0;
	}

	// Conditionally, kill data thread.
	if (m_pDataThread)
	{
		delete m_pDataThread;
		m_pDataThread = 0;
	}
}




CCeSocket::~CCeSocket() 
{
	m_bQuit = TRUE;
	// If this is a data socket, then remove it from the list.

	if (m_iPurpose == FOR_DATA)
	{
		POSITION pos = NULL;

		gCeSocketCS.Lock();
		if ( (pos = m_PtrList.Find(this)) != NULL)
		{
			m_PtrList.RemoveAt(pos);
		}		
		gCeSocketCS.Unlock();
	}

	InterlockedDecrement(&m_iSocketCount);

	// Close the socket.
	CSocket::Close();

	if (m_iSocketCount <= 0)
	{
		//
		// we have to kill all the threads, if the threads exist.
		// The safe way here is to wait till the threads exit gracefully.
		// 
		DWORD dwCode;

		if (m_pDataThread)
		{
			while (::GetExitCodeThread (m_pDataThread->m_hThread,  &dwCode) )
			{
				if (dwCode == STILL_ACTIVE)
				{
					Sleep(50);
				}
				else
				{
					break;
				}
			}
			gCeSocketCS.Lock();
			delete m_pDataThread;
			m_pDataThread = 0;
			gCeSocketCS.Unlock();
		}

		if (m_pListenThread)
		{
			while (::GetExitCodeThread (m_pListenThread->m_hThread,  &dwCode) )
			{ 
				if (dwCode == STILL_ACTIVE)
				{
					Sleep(50);
				}
				else
				{
					break;
				}		
			}
			gCeSocketCS.Lock();
			delete m_pListenThread;
			m_pListenThread = 0;
			gCeSocketCS.Unlock();
		}
	}

	return;
}



 
BOOL CCeSocket::Create( UINT nSocketPort )
{
	timeval tv = { 0 /*sec*/, 1000 /*usec*/ };

	// NOTE: This is where m_hSocket gets filled-in for listening sockets.
	if (! CAsyncSocket::Create( nSocketPort ))
	{
		ASSERT(FALSE);
		return (FALSE);
	}
	if (m_hSocket == INVALID_SOCKET)
	{
		ASSERT(FALSE);					  
		return (FALSE);
	}

	// Make sure that the handle is attached.
	if (! LookupHandle( m_hSocket, FALSE ))
	{
		ASSERT(FALSE);
		Attach( m_hSocket );
	}

	FD_SET(m_hSocket, &m_ReadFds );
	int iRetCode = select( 0, &m_ReadFds, NULL, NULL, &tv );
	if (iRetCode == SOCKET_ERROR)
	{
		// It could be that one of the descriptor sets contains an entry
		// that is no longer a socket (e.g. when a socket is closed).
		// This is okay.  It occurs when a connection is closed at the
		// other end.
		int iErr = ::GetLastError();
		if (iErr != WSAENOTSOCK)
		{
			ASSERT(FALSE);
			return (FALSE);
		}
	}

	m_bConnectCalled = FALSE; // Wait until ConnectHelper() is called before
	                          // letting the thread call DoConnect()

    // Start-up the listening and data thread.
	BeginThread( m_iPurpose );
	if(m_iPurpose != FOR_DATA)
	{
		BeginThread( FOR_DATA );
	}
	
	// If this is a FOR_DATA socket, then return now.
	if (m_iPurpose == FOR_DATA)
	{
		return (TRUE);
	}
	// Set the listening socket to non-blocking.
	if (! SetNonblocking())
	{
		ASSERT(FALSE);
	}

	return (TRUE);
}


BOOL CCeSocket::ConnectHelper(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	m_bConnectCalled = FALSE;
	BOOL bResult = CSocket::ConnectHelper(lpSockAddr,nSockAddrLen);
	if (bResult) m_bConnectCalled = TRUE;
	return bResult;
}




BOOL CCeSocket::IsAlive()
{
	int  iResult;											  
	int	 iLen = sizeof(int);					   

	if (getsockopt( m_hSocket, SOL_SOCKET, SO_TYPE, (char*)&iResult, &iLen ) == SOCKET_ERROR)
		return (FALSE);

	return (TRUE);
}




BOOL CCeSocket::IsListening()
{
	int  iResult;
	int	 iLen = sizeof(int);

	if (getsockopt( m_hSocket, SOL_SOCKET, SO_ACCEPTCONN, (char*)&iResult, &iLen ) == SOCKET_ERROR)
	{
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
			ASSERT(FALSE);
		return (FALSE);
	}

	if (iResult)
		return (TRUE);

	return (FALSE);
}



BOOL CCeSocket::IsReadReady()
{
	unsigned long ulResult;

	if (ioctlsocket( m_hSocket, FIONREAD, &ulResult ) == SOCKET_ERROR)
	{
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
			ASSERT(FALSE);
		return (FALSE);
	}

	if (ulResult > 0)
		return (TRUE);

	return (FALSE);
}




BOOL CCeSocket::SetForRead()
{
	FD_CLR( m_hSocket, &m_ReadFds );
	FD_SET( m_hSocket, &m_ReadFds );

	timeval tv = { 0 /*sec*/, 1000 /*usec*/ };
	int iRetCode = select( 0, &m_ReadFds, NULL, NULL, &tv );
	if (iRetCode == SOCKET_ERROR)
	{
		// It could be that one of the descriptor sets contains an entry
		// that is no longer a socket (e.g. when a socket is closed).
		// This is okay.  It occurs when a connection is closed at the
		// other end.
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
		{
			ASSERT(FALSE);
			return (FALSE);
		}
	}

	return (TRUE);
}




BOOL CCeSocket::SetForWrite()
{
	FD_CLR( m_hSocket, &m_WriteFds );
	FD_SET( m_hSocket, &m_WriteFds );

	timeval tv = { 0 /*sec*/, 1000 /*usec*/ };
	int iRetCode = select( 0, NULL, &m_WriteFds, NULL, &tv );
	if (iRetCode == SOCKET_ERROR)
	{
		// It could be that one of the descriptor sets contains an entry
		// that is no longer a socket (e.g. when a socket is closed).
		// This is okay.  It occurs when a connection is closed at the
		// other end.
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
		{
			ASSERT(FALSE);
			return (FALSE);
		}
	}

	return (TRUE);
}




BOOL CCeSocket::SetForReadnWrite()
{
	FD_CLR( m_hSocket, &m_ReadFds );
	FD_SET( m_hSocket, &m_ReadFds );
	FD_CLR( m_hSocket, &m_WriteFds );
	FD_SET( m_hSocket, &m_WriteFds );

	timeval tv = { 0 /*sec*/, 1000 /*usec*/ };
	int iRetCode = select( 0, &m_ReadFds, &m_WriteFds, NULL, &tv );
	if (iRetCode == SOCKET_ERROR)
	{
		// It could be that one of the descriptor sets contains an entry
		// that is no longer a socket (e.g. when a socket is closed).
		// This is okay.  It occurs when a connection is closed at the
		// other end.
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
		{
			ASSERT(FALSE);
			return (FALSE);
		}
	}

	return (TRUE);
}



BOOL CCeSocket::DoAccept()
{
	// If not a valid socket, then pause and return.
	if (! IsAlive())
	{
		Sleep( 100 );
		return (FALSE);
	}

	// Make sure that this *is* a listening socket.
	if (! IsListening())
	{
		Sleep( 100 );
		return (FALSE);
	}

	// If this socket is not checked for readability, then return.
	if (! FD_ISSET( m_hSocket, &m_ReadFds ))
	{
		Sleep( 100 );
		// Reset for next connection.					   			  
		SetForRead();
		return (FALSE);
	}

	// Make sure that the handle is attached.
	// NOTE: This should already have been done in CreateListeningSocket().
	if (! LookupHandle( m_hSocket, FALSE ))	  
		Attach( m_hSocket );

	// Send an FD_ACCEPT message.
	::SendMessage( AfxGetCeSocketWindow(), WM_SOCKET_NOTIFY, 
		           (WPARAM)m_hSocket, (LPARAM)FD_ACCEPT );
	

	// Reset for next connection.
	SetForRead();

	return (TRUE);
}




BOOL CCeSocket::DoConnect()
{
 	// If not a valid socket, then pause and return.
 	if (! IsAlive())
 	{
 		Sleep( 100 );
 		return (FALSE);
 	}

 	// Make sure that this is *not* a listening socket.
 	if (IsListening())
 	{
 		Sleep( 100 );
 		return (FALSE);
 	}

 	// If this socket is not checked for writability, then return.
 	if (! FD_ISSET( m_hSocket, &m_WriteFds ))
 	{
 		Sleep( 100 );
 		// Reset for next connection.
 		SetForReadnWrite();
		return (FALSE); 
	}

 	// Set m_bIsConnected flag.
 	m_bIsConnected = TRUE;

 	// Make sure that the handle is attached.
 	// NOTE: This should already have been done in CreateListeningSocket().
 	if (! LookupHandle( m_hSocket, FALSE ))
 		Attach( m_hSocket );

 	// Send an FD_CONNECT message.
	if(!m_bQuit)
	{
		::PostMessage( AfxGetCeSocketWindow(), WM_SOCKET_NOTIFY,
 		           (WPARAM)m_hSocket, (LPARAM)FD_CONNECT );
	}

 	return (TRUE);
}

																	  


BOOL CCeSocket::DoRead()
{
	// If *not* a valid socket, then pause and return.
	if (! IsAlive())
	{
		Sleep( 100 );
		return (FALSE);
	}

	// Make sure that this is *not* a listening socket.
	if (IsListening())
	{
		Sleep( 100 );
		return (FALSE);
	}

	// Check if the socket is tagged for readability. If not,
	// then return now.
	if (! FD_ISSET( m_hSocket, &m_ReadFds ))
	{
		return (FALSE);
	}
 	// Set m_bIsConnected flag.
 	m_bIsConnected = TRUE;

	// We have a readable socket.  Make sure that it is attached.
	if (! LookupHandle( m_hSocket, FALSE ))
	{
		Attach( m_hSocket );
	}
	// If there is data ready to be read, then send a WM_SOCKET_NOTIFY/FD_READ message.
	if (IsReadReady())
	{
		::PostMessage( AfxGetCeSocketWindow(), WM_SOCKET_NOTIFY,
			           (WPARAM)m_hSocket, (LPARAM)FD_READ );
	}
	return (FALSE);
}




BOOL CCeSocket::SetNonblocking()
{
	unsigned long ulResult = 1;  // nonzero => non-blocking

	if (ioctlsocket( m_hSocket, FIONBIO, &ulResult ) == SOCKET_ERROR)
	{
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
			ASSERT(FALSE);
		return (FALSE);
	}

	return (TRUE);
}




BOOL CCeSocket::SetBlocking()
{
	unsigned long ulResult = 0;  // zero => blocking

	if (ioctlsocket( m_hSocket, FIONBIO, &ulResult ) == SOCKET_ERROR)
	{
		int iErr = WSAGetLastError();
		if (iErr != WSAENOTSOCK)
			ASSERT(FALSE);
		return (FALSE);
	}

	return (TRUE);
}

#if (_WIN32_WCE >= 300)
BOOL CCeSocket::Accept(CAsyncSocket& rConnectedSocket, SOCKADDR* lpSockAddr, int* lpSockAddrLen)
{
	m_bConnectCalled = TRUE; 
	return CSocket::Accept(rConnectedSocket, lpSockAddr, lpSockAddrLen);
}
#endif // _WIN32_WCE


//  
//  L I S T E N   &   D A T A   T H R E A D S
//



//  
unsigned int AFX_CDECL ListenThread( LPVOID pvParams )
{
    CCeSocket* pListenSocket = (CCeSocket*)pvParams;

    ASSERT(pListenSocket);

    // Keep checking the "listening" socket for an accept.
    while (pListenSocket->m_pListenThread && !pListenSocket->m_bQuit)
    {
		// Do [blocking] accept.
		// May send an (WM_SOCKET_NOTIFY/FD_ACCEPT) window message.
		if ((pListenSocket) && (pListenSocket->m_hSocket != INVALID_SOCKET))
		{
			pListenSocket->DoAccept();
		}
    }

    return (0);
}



//  
unsigned int AFX_CDECL DataThread( void* pvParams )
{
	int iCount = 0;
	timeval      tv          = { 0 /*sec*/, 1000 /*usec*/ };

	while (CCeSocket::m_iSocketCount > 0)
	{		
		// Check the "client" sockets for a read.
		gCeSocketCS.Lock();
		iCount = CCeSocket::m_PtrList.GetCount();
		gCeSocketCS.Unlock();

		if (iCount < 1)
		{
			Sleep( 100);
			continue;
		}

		// 
		// loop through the list to doread
		//
		gCeSocketCS.Lock();

		for(POSITION pos = CCeSocket::m_PtrList.GetHeadPosition(); pos != NULL;)
		{
			CCeSocket* pSocket = (CCeSocket*)CCeSocket::m_PtrList.GetNext(pos);
			ASSERT(pSocket);

			if(!pSocket->m_bConnectCalled)
			{
				continue;
			}

			if (pSocket->m_hSocket == INVALID_SOCKET)
			{
				// NOTE: A DataSocket's m_hSocket gets filled-in by CAsyncSocket::Accept(pSocket).
				//       If we reach this point then CAsyncSocket::Accept(pSocket) has not yet
				//       been called.
				continue;
			}

			// Set the socket to non-blocking.
			if (!pSocket->SetNonblocking())
			{
				continue;
			}
			// May send an (WM_SOCKET_NOTIFY/FD_READ) window message.
			BOOL bRetCode = pSocket->DoRead();
			if (bRetCode)
			{
				FD_CLR(pSocket->m_hSocket, &pSocket->m_ReadFds );
				FD_SET(pSocket->m_hSocket, &pSocket->m_ReadFds );
			}
 			// May send an (WM_SOCKET_NOTIFY/FD_CONNECT) window message.
			// NOTE: For use by data sockets on the client side.
 			if (!pSocket->m_bIsConnected)
			{
				Sleep(100);
				if(!pSocket->m_bQuit)
				{
 					pSocket->DoConnect();    // why do the connect here
				}
			}
		}

		gCeSocketCS.Unlock();

		gCeSocketCS.Lock();
		iCount = CCeSocket::m_PtrList.GetCount();
		gCeSocketCS.Unlock();

		if (iCount < 1)
		{
			Sleep(100);
			continue;
		}

		// Reset for next connection.
		// NOTE: The Reset loop is separate from the Read loop because a
		//       socket may get closed during the course of socket_DoRead().
		//       Hence, we need to iterate through an updated pClientList.

		gCeSocketCS.Lock();

		for(pos = CCeSocket::m_PtrList.GetHeadPosition(); pos != NULL;)
		{
			CCeSocket* pSocket = (CCeSocket*)CCeSocket::m_PtrList.GetNext(pos);
			if (pSocket->m_hSocket == INVALID_SOCKET)
			{
				// NOTE: A DataSocket's m_hSocket gets filled-in by AsyncSocket::Accept(pSocket).
				//       If we reach this point then AsyncSocket::Accept(pSocket) has not yet
				//       been called.
				continue;
			}

			FD_CLR(pSocket->m_hSocket, &pSocket->m_ReadFds );
			FD_SET(pSocket->m_hSocket, &pSocket->m_ReadFds );
 			FD_CLR(pSocket->m_hSocket, &pSocket->m_WriteFds );

 			if (! pSocket->m_bIsConnected)
			{
 				FD_SET(pSocket->m_hSocket, &pSocket->m_WriteFds );
			}

 			int iRetCode = select( 0, &pSocket->m_ReadFds, &pSocket->m_WriteFds, NULL, &tv );
			if (iRetCode == SOCKET_ERROR)
			{
				int iErr = WSAGetLastError();
				if (iErr == WSAENOTSOCK)
				{
					ASSERT(FALSE);
				}
			}
		}
		gCeSocketCS.Unlock();
	}

	return (0);
}

#endif // _WIN32_WCE_NO_WINSOCK

IMPLEMENT_DYNAMIC(CCeSocket, CSocket)
