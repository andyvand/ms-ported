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

#ifndef __WCETHUNK_H__
#define __WCETHUNK_H__

#if defined(_WIN32_WCE_NO_DELAYLOAD)
#pragma warning(disable: 4005)

#if !defined(_WIN32_WCE_NO_COMMDLG_THUNKS)

struct AFX_COMMDLG_CALL
{
#if !defined(_WIN32_WCE_NO_COLORDLG)
	BOOL (APIENTRY* pfnChooseColor)(LPCHOOSECOLOR);
#endif // _WIN32_WCE_NO_COLORDLG
#if !defined(_WIN32_WCE_NO_PRINTING)
	DWORD (APIENTRY* pfnCommDlgExtendedError)(VOID);
	BOOL (APIENTRY* pfnPrintDlg)(LPPRINTDLG);
WCE_DEL BOOL (APIENTRY* pfnPageSetupDlg)(LPPAGESETUPDLG);
#endif // _WIN32_WCE_NO_PRINTING
};

extern AFX_DATADEF AFX_COMMDLG_CALL _afxCommDlg;
#if !defined(_WIN32_WCE_NO_COLORDLG)
#define ChooseColor         _afxCommDlg.pfnChooseColor
#endif // _WIN32_WCE_NO_COLORDLG
#if !defined(_WIN32_WCE_NO_PRINTING)
#define CommDlgExtendedError _afxCommDlg.pfnCommDlgExtendedError
#define PrintDlg             _afxCommDlg.pfnPrintDlg
WCE_DEL #define PageSetupDlg _afxCommDlg.pfnPageSetupDlg
#endif // _WIN32_WCE_NO_PRINTING

#endif // _WIN32_WCE_NO_COMMDLG_THUNKS



#if !defined(_WIN32_WCE_NO_WINSOCK_THUNKS)

#include <winsock.h>
struct AFX_WINSOCK_CALL
{
	SOCKET (WINSOCKAPI* pfnaccept)(SOCKET s, struct sockaddr FAR *addr, int FAR *addrlen);
	u_short (WINSOCKAPI * pfnhtons)(u_short hostshort);
	unsigned long (WINSOCKAPI * pfninet_addr)(const char FAR * cp);
	int (WINSOCKAPI * pfnclosesocket)(SOCKET s);
	int (WINSOCKAPI * pfngetsockname)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	int (WINSOCKAPI * pfngetpeername)(SOCKET s, struct sockaddr FAR *name, int FAR * namelen);
	u_short (WINSOCKAPI * pfnntohs)(u_short netshort);
	char* (WINSOCKAPI * pfninet_ntoa)(struct in_addr in);
	int (WINSOCKAPI * pfnWSAGetLastError)(void);
	void (WINSOCKAPI * pfnWSASetLastError)(int iError);
	u_long (WINSOCKAPI * pfnhtonl)(u_long hostlong);
	SOCKET (WINSOCKAPI * pfnsocket)(int af, int type, int protocol);
	struct hostent* (WINSOCKAPI * pfngethostbyname)(const char FAR * name);
	int (WINSOCKAPI * pfnrecv)(SOCKET s, char FAR * buf, int len, int flags);
	int (WINSOCKAPI * pfnsend)(SOCKET s, const char FAR * buf, int len, int flags);
	int (WINSOCKAPI * pfnWSAAsyncSelect)(SOCKET s, HWND hWnd, u_int wMsg, long lEvent);
	int (WINSOCKAPI * pfnrecvfrom)(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR *from, int FAR * fromlen);
	int (WINSOCKAPI * pfnsendto)(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR *to, int tolen);
	int (WINSOCKAPI * pfnconnect)(SOCKET s, const struct sockaddr FAR *name, int namelen);
	int (WINSOCKAPI * pfnbind)(SOCKET s, const struct sockaddr FAR *addr, int namelen);
	int (WINSOCKAPI * pfnsetsockopt)(SOCKET s, int level, int optname, const char FAR * optval, int optlen);
	int (WINSOCKAPI * pfngetsockopt)(SOCKET s, int level, int optname, char FAR * optval, int FAR *optlen);
	int (WINSOCKAPI * pfnioctlsocket)(SOCKET s, long cmd, u_long FAR *argp);
	int (WINSOCKAPI * pfnlisten)(SOCKET s, int backlog);
	int (WINSOCKAPI * pfnshutdown)(SOCKET s, int how);
WCE_INS int (WINSOCKAPI * pfnselect)(int nfds, fd_set *readfds, fd_set *writefds,fd_set *exceptfds, const struct timeval *timeout);
WCE_INS int (WINSOCKAPI * pfn__WSAFDIsSet)(SOCKET s, fd_set * set);
};

extern AFX_DATADEF AFX_WINSOCK_CALL _afxWinSock;
#define accept              _afxWinSock.pfnaccept
#define htons               _afxWinSock.pfnhtons
#define inet_addr           _afxWinSock.pfninet_addr
#define closesocket         _afxWinSock.pfnclosesocket
#define getsockname         _afxWinSock.pfngetsockname
#define getpeername         _afxWinSock.pfngetpeername
#define ntohs               _afxWinSock.pfnntohs
#define inet_ntoa           _afxWinSock.pfninet_ntoa
#define htonl               _afxWinSock.pfnhtonl
#define socket              _afxWinSock.pfnsocket
#define gethostbyname       _afxWinSock.pfngethostbyname
#define recv                _afxWinSock.pfnrecv
#define send                _afxWinSock.pfnsend
#define recvfrom            _afxWinSock.pfnrecvfrom
#define sendto              _afxWinSock.pfnsendto
#define connect             _afxWinSock.pfnconnect
#define bind                _afxWinSock.pfnbind
#define setsockopt          _afxWinSock.pfnsetsockopt
#define getsockopt          _afxWinSock.pfngetsockopt
#define ioctlsocket         _afxWinSock.pfnioctlsocket
#define listen              _afxWinSock.pfnlisten
#define shutdown            _afxWinSock.pfnshutdown
#define select				_afxWinSock.pfnselect
#define	__WSAFDIsSet	    _afxWinSock.pfn__WSAFDIsSet
#endif // _WIN32_WCE_NO_WINSOCK_THUNKS


#if !defined(_WIN32_WCE_NO_WININET_THUNKS)

struct AFX_WININET_CALL
{
	HINTERNET (WINAPI* pfnInternetOpen)(
		LPCTSTR lpszAgent, DWORD dwAccessType,
		LPCTSTR lpszProxy, LPCTSTR lpszProxyBypass, DWORD dwFlags);
	HINTERNET (WINAPI* pfnInternetCloseHandle)(HINTERNET hInternet);
	HINTERNET (WINAPI* pfnInternetOpenUrl)(HINTERNET hInternet,
		LPCTSTR pstrUrl, LPCTSTR pstrHeaders, DWORD dwHeadersLength,
		DWORD dwFlags, DWORD dwContext);
	HINTERNET (WINAPI* pfnInternetReadFile)(HINTERNET hInternet,
		LPVOID lpBuffer, DWORD dwNumberOfBytesToRead,
		LPDWORD lpdwNumberOfBytesRead);
	HINTERNET (WINAPI* pfnInternetWriteFile)(HINTERNET hInternet,
		LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite,
		LPDWORD lpdwNumberOfBytesWritten);
	INTERNET_STATUS_CALLBACK (WINAPI* pfnInternetSetStatusCallback)(
		HINTERNET hInternet, INTERNET_STATUS_CALLBACK lpfnInternetCallback);
	BOOL (WINAPI* pfnInternetQueryOption)(HINTERNET hInternet,
		DWORD dwOption, LPVOID lpBuffer, LPDWORD lpdwBufferLength);
	BOOL (WINAPI* pfnInternetSetOption)(HINTERNET hInternet,
		DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength);
	BOOL (WINAPI* pfnInternetSetOptionEx)(HINTERNET hInternet,
		DWORD dwOption, LPVOID lpBuffer, DWORD dwBufferLength, DWORD dwFlags);
	BOOL (WINAPI* pfnInternetGetLastResponseInfo)(LPDWORD lpdwError,
		LPTSTR pstrBuffer, LPDWORD lpdwBufferLength);
	BOOL (WINAPI* pfnInternetFindNextFile)(HINTERNET hFind,
		LPVOID lpvFindData);
	HINTERNET (WINAPI* pfnInternetConnect)(HINTERNET hInternet,
		LPCTSTR pstrServerName, INTERNET_PORT nServerPort,
		LPCTSTR pstrUsername, LPCTSTR pstrPassword, DWORD dwService,
		DWORD dwFlags, DWORD dwContext);
	DWORD (WINAPI* pfnInternetSetFilePointer)(HINTERNET hFile,
		LONG lDistanceToMove, PVOID reserved, DWORD dwMoveMethod,
		DWORD dwContext);
	BOOL (WINAPI* pfnInternetQueryDataAvailable)(HINTERNET hFile,
		LPDWORD lpdwNumberOfBytesAvailable, DWORD dwFlags, DWORD dwContext);

#if !defined(_WIN32_WCE_NO_FTP)
	HINTERNET (WINAPI* pfnFtpFindFirstFile)(HINTERNET hFtpSession,
		LPCTSTR pstrSearchFile, LPWIN32_FIND_DATA lpFindFileData,
		DWORD dwFlags, DWORD dwContext);
	BOOL (WINAPI* pfnFtpGetFile)(HINTERNET hFtpSession,
		LPCTSTR pstrRemoteFile, LPCTSTR pstrNewFile, BOOL fFailIfExists,
		DWORD dwFlagsAndAttributes, DWORD dwFlags, DWORD dwContext);
	BOOL (WINAPI* pfnFtpPutFile)(HINTERNET hFtpSession,
		LPCTSTR pstrLocalFile, LPCTSTR pstrNewRemoteFile, DWORD dwFlags,
		DWORD dwContext);
	BOOL (WINAPI* pfnFtpDeleteFile)(HINTERNET hFtpSession,
		LPCTSTR pstrFileName);
	BOOL (WINAPI* pfnFtpRenameFile)(HINTERNET hFtpSession,
		LPCTSTR pstrExisting, LPCTSTR pstrNew);
	BOOL (WINAPI* pfnFtpCreateDirectory)(HINTERNET hFtpSession,
		LPCTSTR pstrDirectory);
	BOOL (WINAPI* pfnFtpRemoveDirectory)(HINTERNET hFtpSession,
		LPCTSTR pstrDirectory);
	BOOL (WINAPI* pfnFtpSetCurrentDirectory)(HINTERNET hFtpSession,
		LPCTSTR pstrDirectory);
	BOOL (WINAPI* pfnFtpGetCurrentDirectory)(HINTERNET hFtpSession,
		LPCTSTR pstrCurrentDirectory, LPDWORD lpdwCurrentDirectory);
	BOOL (WINAPI* pfnFtpCommand)(HINTERNET hFtpSession,
		BOOL fExpectResponse, DWORD dwFlags, LPCTSTR pstrCommand,
		DWORD dwContext);
	HINTERNET (WINAPI* pfnFtpOpenFile)(HINTERNET hFtpSession,
		LPCTSTR pstrFileName, DWORD dwAccess, DWORD dwFlags, DWORD dwContext);
#endif

	HINTERNET (WINAPI* pfnHttpOpenRequest)(HINTERNET hHttpSession,
		LPCTSTR pstrVerb, LPCTSTR pstrObjectName, LPCTSTR pstrVersion,
		LPCTSTR pstrReferrer, LPCTSTR FAR * lppstrAcceptTypes,
		DWORD dwFlags, DWORD dwContext);
	BOOL (WINAPI* pfnHttpAddRequestHeaders)(HINTERNET hHttpRequest,
		LPCTSTR pstrHeaders, DWORD dwHeadersLength, DWORD dwModifiers);
	BOOL (WINAPI* pfnHttpSendRequest)(HINTERNET hHttpRequest,
		LPCTSTR pstrHeaders, DWORD dwHeadersLength, LPVOID lpOptional,
		DWORD dwOptionalLength);
	BOOL (WINAPI* pfnHttpQueryInfo)(HINTERNET hHttpRequest,
		DWORD dwInfoLevel, LPVOID lpvBuffer, LPDWORD lpdwBufferLength,
		LPDWORD dwIndex);
	DWORD (WINAPI*pfnInternetErrorDlg)(HWND hWnd, HINTERNET hRequest,
		 DWORD dwError, DWORD dwFlags, LPVOID * lppvData);
	BOOL (WINAPI*pfnInternetCrackUrl)(LPCTSTR lpszUrl, DWORD dwUrlLength,
		DWORD dwFlags, LPURL_COMPONENTS lpUrlComponents);
	BOOL (WINAPI*pfnInternetCanonicalizeUrl)(LPCTSTR lpszUrl, LPTSTR lpszBuffer, 
		LPDWORD lpdwBufferLength, DWORD dwFlags);
};

extern AFX_DATADEF AFX_WININET_CALL _afxWinInet;
#undef  InternetOpen
#define InternetOpen                _afxWinInet.pfnInternetOpen
#define InternetCloseHandle         _afxWinInet.pfnInternetCloseHandle
#undef  InternetOpenUrl
#define InternetOpenUrl             _afxWinInet.pfnInternetOpenUrl
#define InternetReadFile            _afxWinInet.pfnInternetReadFile
#define InternetWriteFile           _afxWinInet.pfnInternetWriteFile
#undef	InternetSetStatusCallback
#define InternetSetStatusCallback   _afxWinInet.pfnInternetSetStatusCallback
#undef  InternetSetOption
#define InternetSetOption           _afxWinInet.pfnInternetSetOption
#undef  InternetSetOptionEx
#define InternetSetOptionEx         _afxWinInet.pfnInternetSetOptionEx
#undef  InternetQueryOption
#define InternetQueryOption         _afxWinInet.pfnInternetQueryOption
#undef  InternetGetLastResponseInfo
#define InternetGetLastResponseInfo _afxWinInet.pfnInternetGetLastResponseInfo
#undef  InternetFindNextFile
#define InternetFindNextFile        _afxWinInet.pfnInternetFindNextFile
#undef  InternetConnect
#define InternetConnect             _afxWinInet.pfnInternetConnect
#define InternetSetFilePointer      _afxWinInet.pfnInternetSetFilePointer
#define InternetQueryDataAvailable  _afxWinInet.pfnInternetQueryDataAvailable
#if !defined(_WIN32_WCE_NO_FTP)
#define FtpFindFirstFile            _afxWinInet.pfnFtpFindFirstFile
#define FtpGetFile                  _afxWinInet.pfnFtpGetFile
#define FtpPutFile                  _afxWinInet.pfnFtpPutFile
#define FtpDeleteFile               _afxWinInet.pfnFtpDeleteFile
#define FtpRenameFile               _afxWinInet.pfnFtpRenameFile
#define FtpCreateDirectory          _afxWinInet.pfnFtpCreateDirectory
#define FtpRemoveDirectory          _afxWinInet.pfnFtpRemoveDirectory
#define FtpGetCurrentDirectory      _afxWinInet.pfnFtpGetCurrentDirectory
#define FtpSetCurrentDirectory      _afxWinInet.pfnFtpSetCurrentDirectory
#define FtpCommand                  _afxWinInet.pfnFtpCommand
#define FtpOpenFile                 _afxWinInet.pfnFtpOpenFile
#endif // _WIN32_WCE_NO_FTP
#undef  HttpOpenRequest
#define HttpOpenRequest             _afxWinInet.pfnHttpOpenRequest
#undef  HttpAddRequestHeaders
#define HttpAddRequestHeaders       _afxWinInet.pfnHttpAddRequestHeaders
#undef  HttpSendRequest
#define HttpSendRequest             _afxWinInet.pfnHttpSendRequest
#undef  HttpQueryInfo
#define HttpQueryInfo               _afxWinInet.pfnHttpQueryInfo
#define InternetErrorDlg            _afxWinInet.pfnInternetErrorDlg
#undef  InternetCrackUrl
#define InternetCrackUrl            _afxWinInet.pfnInternetCrackUrl
#undef  InternetCanonicalizeUrl
#define InternetCanonicalizeUrl     _afxWinInet.pfnInternetCanonicalizeUrl
#endif // _WIN32_WCE_NO_WININET_THUNKS

#pragma warning(default: 4005)
#endif // _WIN32_WCE_NO_DELAYLOAD
#endif // __WCETHUNK_H__
