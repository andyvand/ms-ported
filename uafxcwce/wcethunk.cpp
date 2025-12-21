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
#if defined(_WIN32_WCE_NO_DELAYLOAD)

class _AFX_EXTDLL_STATE : public CNoTrackObject
{
public:
	_AFX_EXTDLL_STATE::~_AFX_EXTDLL_STATE();
	HINSTANCE m_hInstCommDlg;
	HINSTANCE m_hInstWinSock;
	HINSTANCE m_hInstWinInet;
};

PROCESS_LOCAL(_AFX_EXTDLL_STATE, _afxExtDllState)


HINSTANCE AFXAPI AfxLoadDll(HINSTANCE* volatile pInst, LPCSTR lpszDLL,
	FARPROC* pProcPtrs, LPCSTR lpszProcName)
{
	if (*pInst == NULL)
	{
		AfxLockGlobals(CRIT_DYNDLLLOAD);
		if (*pInst == NULL)
		{
			*pInst = WCE_FCTN(LoadLibraryA)(lpszDLL);
			ASSERT(*pInst != NULL);
		}
		AfxUnlockGlobals(CRIT_DYNDLLLOAD);
		if (*pInst == NULL)
		{
			TRACE1("Error: Unable to load DLL '%hs'!\n", lpszDLL);

			CString strError, strFormatted;
			VERIFY(strError.LoadString(AFX_IDP_DLL_LOAD_FAILED));
			strFormatted.Format(strError, lpszDLL);
			AfxMessageBox(strFormatted, MB_ICONEXCLAMATION);
			AfxThrowMemoryException();
		}
	}

	if (pProcPtrs != NULL)
	{
		*pProcPtrs = WCE_FCTN(GetProcAddress)(*pInst, lpszProcName);
		ASSERT(*pProcPtrs != NULL);
	}
	return *pInst;
}



#if !defined(_WIN32_WCE_NO_COMMDLG_THUNKS)

inline HINSTANCE AfxLoadCommDlg(FARPROC* proc, LPCSTR lpsz)
	{ return AfxLoadDll(&_afxExtDllState->m_hInstCommDlg, WCE_COMMDLG_DLL, proc, lpsz); }
#define COMMDLGLOAD(x) AfxLoadCommDlg((FARPROC*)&_afxCommDlg.pfn##x, #x);
#define COMMDLGLOADT(x) AfxLoadCommDlg((FARPROC*)&_afxCommDlg.pfn##x, WCE_IF(#x,#x"W"));

#if !defined(_WIN32_WCE_NO_COLORDLG)
BOOL APIENTRY AfxThunkChooseColor(LPCHOOSECOLOR lp)
{
	COMMDLGLOADT(ChooseColor);
	return _afxCommDlg.pfnChooseColor(lp);
}
#endif // _WIN32_WCE_NO_COLORDLG

#if !defined(_WIN32_WCE_NO_PRINTING)
DWORD APIENTRY AfxThunkCommDlgExtendedError()
{
	COMMDLGLOAD(CommDlgExtendedError);
	return _afxCommDlg.pfnCommDlgExtendedError();
}

BOOL APIENTRY AfxThunkPrintDlg(LPPRINTDLG lp)
{
	COMMDLGLOADT(PrintDlg);
	return _afxCommDlg.pfnPrintDlg(lp);
}

#if !defined(_WIN32_WCE)
BOOL APIENTRY AfxThunkPageSetupDlg(LPPAGESETUPDLG lp)
{
	COMMDLGLOADT(PageSetupDlg);
	return _afxCommDlg.pfnPageSetupDlg(lp);
}
#endif // _WIN32_WCE
#endif // _WIN32_WCE_NO_PRINTING

AFX_DATADEF AFX_COMMDLG_CALL _afxCommDlg =
{
#if !defined(_WIN32_WCE_NO_COLORDLG)
	AfxThunkChooseColor,
#endif // _WIN32_WCE_NO_COLORDLG
#if !defined(_WIN32_WCE_NO_PRINTING)
	AfxThunkCommDlgExtendedError,
	AfxThunkPrintDlg,
#if !defined(_WIN32_WCE)
	AfxThunkPageSetupDlg,
#endif // _WIN32_WCE
#endif // _WIN32_WCE_NO_PRINTING
};
#endif // _WIN32_WCE_NO_COMMDLG_THUNKS



#if !defined(_WIN32_WCE_NO_WINSOCK_THUNKS)
inline HINSTANCE AfxLoadWinSock(FARPROC* proc, LPCSTR lpsz)
	{ return AfxLoadDll(&_afxExtDllState->m_hInstWinSock, WCE_WINSOCK_DLL, proc, lpsz); }
#define SOCKLOAD(x) AfxLoadWinSock((FARPROC*)&_afxWinSock.pfn##x, #x);

SOCKET WINSOCKAPI AfxThunkaccept(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen)
{
	SOCKLOAD(accept);
	return _afxWinSock.pfnaccept(s, addr, addrlen);
}

u_short WINSOCKAPI AfxThunkhtons(u_short hostshort)
{
	SOCKLOAD(htons);
	return _afxWinSock.pfnhtons(hostshort);
}

unsigned long WINSOCKAPI AfxThunkinet_addr(const char FAR * cp)
{
	SOCKLOAD(inet_addr);
	return _afxWinSock.pfninet_addr(cp);
}

int WINSOCKAPI AfxThunkclosesocket(SOCKET s)
{
	SOCKLOAD(closesocket);
	return _afxWinSock.pfnclosesocket(s);
}

int WINSOCKAPI AfxThunkgetsockname(SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	SOCKLOAD(getsockname);
	return _afxWinSock.pfngetsockname(s, name, namelen);
}

int WINSOCKAPI AfxThunkgetpeername(SOCKET s, struct sockaddr FAR *name, int FAR * namelen)
{
	SOCKLOAD(getpeername);
	return _afxWinSock.pfngetpeername(s, name, namelen);
}

u_short WINSOCKAPI AfxThunkntohs(u_short netshort)
{
	SOCKLOAD(ntohs);
	return _afxWinSock.pfnntohs(netshort);
}

char* WINSOCKAPI AfxThunkinet_ntoa(struct in_addr in)
{
	SOCKLOAD(inet_ntoa);
	return _afxWinSock.pfninet_ntoa(in);
}

int WINSOCKAPI AfxThunkWSAGetLastError(void)
{
	SOCKLOAD(WSAGetLastError);
	return _afxWinSock.pfnWSAGetLastError();
}

void WINSOCKAPI AfxThunkWSASetLastError(int iError)
{
	SOCKLOAD(WSASetLastError);
	_afxWinSock.pfnWSASetLastError(iError);
}

u_long WINSOCKAPI AfxThunkhtonl(u_long hostlong)
{
	SOCKLOAD(htonl);
	return _afxWinSock.pfnhtonl(hostlong);
}

SOCKET WINSOCKAPI AfxThunksocket(int af, int type, int protocol)
{
	SOCKLOAD(socket);
	return _afxWinSock.pfnsocket(af, type, protocol);
}

struct hostent* WINSOCKAPI AfxThunkgethostbyname(const char FAR * name)
{
	SOCKLOAD(gethostbyname);
	return _afxWinSock.pfngethostbyname(name);
}

int WINSOCKAPI AfxThunkrecv(SOCKET s, char FAR * buf, int len, int flags)
{
	SOCKLOAD(recv);
	return _afxWinSock.pfnrecv(s, buf, len, flags);
}

int WINSOCKAPI AfxThunksend(SOCKET s, const char FAR * buf, int len, int flags)
{
	SOCKLOAD(send);
	return _afxWinSock.pfnsend(s, buf, len, flags);
}

int WINSOCKAPI AfxThunkWSAAsyncSelect(SOCKET s, HWND hWnd, u_int wMsg, long lEvent)
{
	SOCKLOAD(WSAAsyncSelect);
	return _afxWinSock.pfnWSAAsyncSelect(s, hWnd, wMsg, lEvent);
}

int WINSOCKAPI AfxThunkrecvfrom(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen)
{
	SOCKLOAD(recvfrom);
	return _afxWinSock.pfnrecvfrom(s, buf, len, flags, from, fromlen);
}

int WINSOCKAPI AfxThunksendto(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen)
{
	SOCKLOAD(sendto);
	return _afxWinSock.pfnsendto(s, buf, len, flags, to, tolen);
}

int WINSOCKAPI AfxThunkconnect(SOCKET s, const struct sockaddr FAR *name, int namelen)
{
	SOCKLOAD(connect);
	return _afxWinSock.pfnconnect(s, name, namelen);
}

int WINSOCKAPI AfxThunkbind(SOCKET s, const struct sockaddr FAR *addr, int namelen)
{
	SOCKLOAD(bind);
	return _afxWinSock.pfnbind(s, addr, namelen);
}

int WINSOCKAPI AfxThunksetsockopt(SOCKET s, int level, int optname, const char FAR * optval, int optlen)
{
	SOCKLOAD(setsockopt);
	return _afxWinSock.pfnsetsockopt(s, level, optname, optval, optlen);
}

int WINSOCKAPI AfxThunkgetsockopt(SOCKET s, int level, int optname, char FAR * optval, int FAR *optlen)
{
	SOCKLOAD(getsockopt);
	return _afxWinSock.pfngetsockopt(s, level, optname, optval, optlen);
}

int WINSOCKAPI AfxThunkioctlsocket(SOCKET s, long cmd, u_long FAR *argp)
{
	SOCKLOAD(ioctlsocket);
	return _afxWinSock.pfnioctlsocket(s, cmd, argp);
}

int WINSOCKAPI AfxThunklisten(SOCKET s, int backlog)
{
	SOCKLOAD(listen);
	return _afxWinSock.pfnlisten(s, backlog);
}

int WINSOCKAPI AfxThunkshutdown(SOCKET s, int how)
{
	SOCKLOAD(shutdown);
	return _afxWinSock.pfnshutdown(s, how);
}

int WINSOCKAPI AfxThunkselect(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timeval *timeout)
{
	SOCKLOAD(select);
	return _afxWinSock.pfnselect(nfds, readfds, writefds, exceptfds, timeout);
}

int WINSOCKAPI AfxThunk__WSAFDIsSet(SOCKET s, fd_set * set)
{
	SOCKLOAD(__WSAFDIsSet);
	return _afxWinSock.pfn__WSAFDIsSet(s, set);
}

AFX_DATADEF AFX_WINSOCK_CALL _afxWinSock =
{
	AfxThunkaccept,
	AfxThunkhtons,
	AfxThunkinet_addr,
	AfxThunkclosesocket,
	AfxThunkgetsockname,
	AfxThunkgetpeername,
	AfxThunkntohs,
	AfxThunkinet_ntoa,
	AfxThunkWSAGetLastError,
	AfxThunkWSASetLastError,
	AfxThunkhtonl,
	AfxThunksocket,
	AfxThunkgethostbyname,
	AfxThunkrecv,
	AfxThunksend,
	AfxThunkWSAAsyncSelect,
	AfxThunkrecvfrom,
	AfxThunksendto,
	AfxThunkconnect,
	AfxThunkbind,
	AfxThunksetsockopt,
	AfxThunkgetsockopt,
	AfxThunkioctlsocket,
	AfxThunklisten,
	AfxThunkshutdown,
	AfxThunkselect,
	AfxThunk__WSAFDIsSet,
};
#endif // _WIN32_WCE_NO_WINSOCK_THUNKS




#if !defined(_WIN32_WCE_NO_WININET_THUNKS)

#define INETLOAD(x) AfxLoadWinInet((FARPROC*)&_afxWinInet.pfn##x, #x)
#define INETLOADT(x) AfxLoadWinInet((FARPROC*)&_afxWinInet.pfn##x, #x"W")
inline HINSTANCE AfxLoadWinInet(FARPROC* proc, LPCSTR lpsz)
	{ return AfxLoadDll(&_afxExtDllState->m_hInstWinInet, WCE_WININET_DLL, proc, lpsz); }

HINTERNET WINAPI AfxThunkINetInternetOpen(
	LPCTSTR lpszAgent,
	DWORD dwAccessType,
	LPCTSTR lpszProxy,
	LPCTSTR lpszProxyBypass,
	DWORD dwFlags)
{
	INETLOADT(InternetOpen);
	return _afxWinInet.pfnInternetOpen(lpszAgent, dwAccessType,
		lpszProxy, lpszProxyBypass, dwFlags);
}

HINTERNET WINAPI AfxThunkINetInternetCloseHandle(HINTERNET hInternet)
{
	INETLOAD(InternetCloseHandle);
	return _afxWinInet.pfnInternetCloseHandle(hInternet);
}

HINTERNET WINAPI AfxThunkINetInternetOpenUrl(HINTERNET hInternet,
	LPCTSTR pstrUrl, LPCTSTR pstrHeaders, DWORD dwHeadersLength,
	DWORD dwFlags, DWORD dwContext)
{
	INETLOADT(InternetOpenUrl);
	return _afxWinInet.pfnInternetOpenUrl(hInternet,
		pstrUrl, pstrHeaders, dwHeadersLength, dwFlags, dwContext);
}

HINTERNET WINAPI AfxThunkINetInternetReadFile(HINTERNET hInternet,
	LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
	LPDWORD lpdwNumberOfBytesRead)
{
	INETLOAD(InternetReadFile);
	return _afxWinInet.pfnInternetReadFile(hInternet, lpBuffer,
		dwNumberOfBytesToRead, lpdwNumberOfBytesRead);
}

HINTERNET WINAPI AfxThunkINetInternetWriteFile(HINTERNET hInternet,
	LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
	LPDWORD lpdwNumberOfBytesWritten)
{
	INETLOAD(InternetWriteFile);
	return _afxWinInet.pfnInternetWriteFile(hInternet, lpBuffer,
		dwNumberOfBytesToWrite, lpdwNumberOfBytesWritten);
}

INTERNET_STATUS_CALLBACK WINAPI AfxThunkINetInternetSetStatusCallback(
	HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback)
{
	INETLOAD(InternetSetStatusCallback);
	return _afxWinInet.pfnInternetSetStatusCallback(hInternet,
		lpfnInternetCallback);
}

BOOL WINAPI AfxThunkINetInternetQueryOption(HINTERNET hInternet,
	DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength)
{
	INETLOADT(InternetQueryOption);
	return _afxWinInet.pfnInternetQueryOption(hInternet, dwOption,
		lpBuffer, lpdwBufferLength);
}

BOOL WINAPI AfxThunkINetInternetSetOption(HINTERNET hInternet,
	DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength)
{
	INETLOADT(InternetSetOption);
	return _afxWinInet.pfnInternetSetOption(hInternet, dwOption, lpBuffer,
		dwBufferLength);
}

BOOL WINAPI AfxThunkINetInternetSetOptionEx(HINTERNET hInternet,
	DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength, DWORD dwFlags)
{
	INETLOADT(InternetSetOptionEx);
	return _afxWinInet.pfnInternetSetOptionEx(hInternet, dwOption, lpBuffer,
		dwBufferLength, dwFlags);
}

BOOL WINAPI AfxThunkINetInternetGetLastResponseInfo(LPDWORD lpdwError,
	LPTSTR pstrBuffer, LPDWORD lpdwBufferLength)
{
	INETLOADT(InternetGetLastResponseInfo);
	return _afxWinInet.pfnInternetGetLastResponseInfo(lpdwError,
		pstrBuffer, lpdwBufferLength);
}

BOOL WINAPI AfxThunkINetInternetFindNextFile(HINTERNET hFind,
	LPVOID lpvFindData)
{
	INETLOADT(InternetFindNextFile);
	return _afxWinInet.pfnInternetFindNextFile(hFind, lpvFindData);
}

HINTERNET WINAPI AfxThunkINetInternetConnect(HINTERNET hInternet,
	LPCTSTR pstrServerName, INTERNET_PORT nServerPort, LPCTSTR pstrUsername,
	LPCTSTR pstrPassword, DWORD dwService, DWORD dwFlags, DWORD dwContext)
{
	INETLOADT(InternetConnect);
	return _afxWinInet.pfnInternetConnect(hInternet, pstrServerName,
		nServerPort, pstrUsername, pstrPassword, dwService, dwFlags,
		dwContext);
}

DWORD WINAPI AfxThunkINetInternetSetFilePointer(HINTERNET hFile,
	LONG lDistanceToMove, PVOID reserved, DWORD dwMoveMethod, DWORD dwContext)
{
	INETLOAD(InternetSetFilePointer);
	return _afxWinInet.pfnInternetSetFilePointer(hFile, lDistanceToMove,
		reserved, dwMoveMethod, dwContext);
}

BOOL WINAPI AfxThunkINetInternetQueryDataAvailable(HINTERNET hFile,
	LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext)
{
	INETLOAD(InternetQueryDataAvailable);
	return _afxWinInet.pfnInternetQueryDataAvailable(hFile,
		lpdwNumberOfBytesAvailable, dwFlags, dwContext);
}


#if !defined(_WIN32_WCE_NO_FTP)

HINTERNET WINAPI AfxThunkINetFtpFindFirstFile(HINTERNET hFtpSession,
	LPCTSTR pstrSearchFile, LPWIN32_FIND_DATA lpFindFileData,
	DWORD dwFlags, DWORD dwContext)
{
	INETLOADT(FtpFindFirstFile);
	return _afxWinInet.pfnFtpFindFirstFile(hFtpSession,
		pstrSearchFile, lpFindFileData, dwFlags, dwContext);
}

BOOL WINAPI AfxThunkINetFtpGetFile(HINTERNET hFtpSession,
	LPCTSTR pstrRemoteFile, LPCTSTR pstrNewFile, BOOL fFailIfExists,
	DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD dwContext)
{
	INETLOADT(FtpGetFile);
	return _afxWinInet.pfnFtpGetFile(hFtpSession, pstrRemoteFile,
		pstrNewFile, fFailIfExists, dwFlagsAndAttributes, dwFlags,
		dwContext);
}

BOOL WINAPI AfxThunkINetFtpPutFile(HINTERNET hFtpSession,
	LPCTSTR pstrLocalFile, LPCTSTR pstrNewRemoteFile, DWORD dwFlags,
	DWORD dwContext)
{
	INETLOADT(FtpPutFile);
	return _afxWinInet.pfnFtpPutFile(hFtpSession, pstrLocalFile,
		pstrNewRemoteFile, dwFlags, dwContext);
}

BOOL WINAPI AfxThunkINetFtpDeleteFile(HINTERNET hFtpSession,
	LPCTSTR pstrFileName)
{
	INETLOADT(FtpDeleteFile);
	return _afxWinInet.pfnFtpDeleteFile(hFtpSession, pstrFileName);
}

BOOL WINAPI AfxThunkINetFtpRenameFile(HINTERNET hFtpSession,
	LPCTSTR pstrExisting, LPCTSTR pstrNew)
{
	INETLOADT(FtpRenameFile);
	return _afxWinInet.pfnFtpRenameFile(hFtpSession, pstrExisting, pstrNew);
}

BOOL WINAPI AfxThunkINetFtpCreateDirectory(HINTERNET hFtpSession,
	LPCTSTR pstrDirectory)
{
	INETLOADT(FtpCreateDirectory);
	return _afxWinInet.pfnFtpCreateDirectory(hFtpSession, pstrDirectory);
}

BOOL WINAPI AfxThunkINetFtpRemoveDirectory(HINTERNET hFtpSession,
	LPCTSTR pstrDirectory)
{
	INETLOADT(FtpRemoveDirectory);
	return _afxWinInet.pfnFtpRemoveDirectory(hFtpSession, pstrDirectory);
}

BOOL WINAPI AfxThunkINetFtpSetCurrentDirectory(HINTERNET hFtpSession,
	LPCTSTR pstrDirectory)
{
	INETLOADT(FtpSetCurrentDirectory);
	return _afxWinInet.pfnFtpSetCurrentDirectory(hFtpSession, pstrDirectory);
}

BOOL WINAPI AfxThunkINetFtpGetCurrentDirectory(HINTERNET hFtpSession,
	LPCTSTR pstrCurrentDirectory, LPDWORD lpdwCurrentDirectory)
{
	INETLOADT(FtpGetCurrentDirectory);
	return _afxWinInet.pfnFtpGetCurrentDirectory(hFtpSession,
		pstrCurrentDirectory, lpdwCurrentDirectory);
}

BOOL WINAPI AfxThunkINetFtpCommand(HINTERNET hFtpSession,
	BOOL fExpectResponse, DWORD dwFlags, LPCTSTR pstrCommand,
	DWORD dwContext)
{
	INETLOADT(FtpCommand);
	return _afxWinInet.pfnFtpCommand(hFtpSession, fExpectResponse,
		dwFlags, pstrCommand, dwContext);
}

HINTERNET WINAPI AfxThunkINetFtpOpenFile(HINTERNET hFtpSession,
		LPCTSTR pstrFileName, DWORD dwAccess, DWORD dwFlags,
		DWORD dwContext)
{
	INETLOADT(FtpOpenFile);
	return _afxWinInet.pfnFtpOpenFile(hFtpSession, pstrFileName,
		dwAccess, dwFlags, dwContext);
}
#endif // _WIN32_WCE_NO_FTP


HINTERNET WINAPI AfxThunkINetHttpOpenRequest(HINTERNET hHttpSession,
	LPCTSTR pstrVerb, LPCTSTR pstrObjectName, LPCTSTR pstrVersion,
	LPCTSTR pstrReferrer, LPCTSTR FAR * lppstrAcceptTypes,
	DWORD dwFlags, DWORD dwContext)
{
	INETLOADT(HttpOpenRequest);
	return _afxWinInet.pfnHttpOpenRequest(hHttpSession, pstrVerb,
		pstrObjectName, pstrVersion, pstrReferrer, lppstrAcceptTypes,
		dwFlags, dwContext);
}

BOOL WINAPI AfxThunkINetHttpAddRequestHeaders(HINTERNET hHttpRequest,
	LPCTSTR pstrHeaders, DWORD dwHeadersLength, DWORD dwModifiers)
{
	INETLOADT(HttpAddRequestHeaders);
	return _afxWinInet.pfnHttpAddRequestHeaders(hHttpRequest,
		pstrHeaders, dwHeadersLength, dwModifiers);
}

BOOL WINAPI AfxThunkINetHttpSendRequest(HINTERNET hHttpRequest,
	LPCTSTR pstrHeaders, DWORD dwHeadersLength, LPVOID lpOptional,
	DWORD dwOptionalLength)
{
	INETLOADT(HttpSendRequest);
	return _afxWinInet.pfnHttpSendRequest(hHttpRequest, pstrHeaders,
		dwHeadersLength, lpOptional, dwOptionalLength);
}

BOOL WINAPI AfxThunkINetHttpQueryInfo(HINTERNET hHttpRequest,
	DWORD dwInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength,
	LPDWORD lpdwIndex)
{
	INETLOADT(HttpQueryInfo);
	return _afxWinInet.pfnHttpQueryInfo(hHttpRequest, dwInfoLevel,
		lpvBuffer, lpdwBufferLength, lpdwIndex);
}

DWORD WINAPI AfxThunkINetInternetErrorDlg(HWND hWnd, HINTERNET hRequest,
	 DWORD dwError, DWORD dwFlags, LPVOID * lppvData)
{
	INETLOADT(InternetErrorDlg);
	return _afxWinInet.pfnInternetErrorDlg(hWnd, hRequest,
		dwError, dwFlags, lppvData);
}

BOOL WINAPI AfxThunkINetCrackUrl(LPCTSTR lpszUrl, DWORD dwUrlLength,
	DWORD dwFlags, LPURL_COMPONENTS lpUrlComponents)
{
	INETLOADT(InternetCrackUrl);
	return _afxWinInet.pfnInternetCrackUrl(lpszUrl, dwUrlLength,
		dwFlags, lpUrlComponents);
}

BOOL WINAPI AfxThunkINetCanonicalizeUrl(LPCTSTR lpszUrl, LPTSTR lpszBuffer, 
	LPDWORD lpdwBufferLength, DWORD dwFlags)
{
	INETLOADT(InternetCanonicalizeUrl);
	return _afxWinInet.pfnInternetCanonicalizeUrl(lpszUrl, lpszBuffer,
		lpdwBufferLength, dwFlags);
}

AFX_DATADEF AFX_WININET_CALL _afxWinInet =
{
	{ AfxThunkINetInternetOpen, },
	{ AfxThunkINetInternetCloseHandle, },
	{ AfxThunkINetInternetOpenUrl, },
	{ AfxThunkINetInternetReadFile, },
	{ AfxThunkINetInternetWriteFile, },
	{ AfxThunkINetInternetSetStatusCallback, },
	{ AfxThunkINetInternetQueryOption, },
	{ AfxThunkINetInternetSetOption, },
	{ AfxThunkINetInternetSetOptionEx, },
	{ AfxThunkINetInternetGetLastResponseInfo, },
	{ AfxThunkINetInternetFindNextFile, },
	{ AfxThunkINetInternetConnect, },
	{ AfxThunkINetInternetSetFilePointer, },
	{ AfxThunkINetInternetQueryDataAvailable, },

#if !defined(_WIN32_WCE_NO_FTP)
	{ AfxThunkINetFtpFindFirstFile, },
	{ AfxThunkINetFtpGetFile, },
	{ AfxThunkINetFtpPutFile, },
	{ AfxThunkINetFtpDeleteFile, },
	{ AfxThunkINetFtpRenameFile, },
	{ AfxThunkINetFtpCreateDirectory, },
	{ AfxThunkINetFtpRemoveDirectory, },
	{ AfxThunkINetFtpSetCurrentDirectory, },
	{ AfxThunkINetFtpGetCurrentDirectory, },
	{ AfxThunkINetFtpCommand, },
	{ AfxThunkINetFtpOpenFile, },
#endif // _WIN32_WCE_NO_FTP

	{ AfxThunkINetHttpOpenRequest, },
	{ AfxThunkINetHttpAddRequestHeaders, },
	{ AfxThunkINetHttpSendRequest, },
	{ AfxThunkINetHttpQueryInfo, },
	{ AfxThunkINetInternetErrorDlg, },

	{ AfxThunkINetCrackUrl, },
	{ AfxThunkINetCanonicalizeUrl, }
};
#endif // _WIN32_WCE_NO_WININET_THUNKS

/////////////////////////////////////////////////////////////////////////////
// _AFX_EXTDLL_STATE

#pragma warning(disable: 4074)
#pragma init_seg(lib)

_AFX_EXTDLL_STATE::~_AFX_EXTDLL_STATE()
{
	if (m_hInstCommDlg != NULL)
		::FreeLibrary(m_hInstCommDlg);
	if (m_hInstWinSock != NULL)
		::FreeLibrary(m_hInstWinSock);
	if (m_hInstWinInet != NULL)
		::FreeLibrary(m_hInstWinInet);
}

#endif // _WIN32_WCE_NO_DELAYLOAD

