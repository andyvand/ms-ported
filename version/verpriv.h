/***************************************************************************
 *  VERPRIV.H
 *
 *    Private include file for the version stamping API.  Includes
 *    WINDOWS.H if this is being made to be a DLL.
 *
 ***************************************************************************/

#ifndef VERPRIV_H
#define VERPRIV_H
#undef DBCS

#ifdef __aarch64__
#undef  __cdecl
#define __cdecl

#undef  __stdcall
#define __stdcall
#endif

#include <stdlib.h>
#include <winternl.h>
#include <windows.h>

#define SEEK_START      0
#define SEEK_CURRENT 1
#define SEEK_END     2

/* ----- Function prototypes ----- */
#define FileClose(a)       LZClose(a)
#define FileRead(a, b, c)  LZRead((a), (b), (c))
#define FileSeek(a, b, c)  LZSeek((a), (b), (c))


BOOL
APIENTRY
VerpQueryValue(
   LPCVOID pb,
   LPVOID lpSubBlockX,
   INT    nIndex,
   LPVOID *lplpKey,
   LPVOID *lplpBuffer,
   PUINT  puLen,
   BOOL  bUnicodeNeeded
   );

#endif /* VERPRIV_H */

#if !defined(_MSC_VER) || (_MSC_VER >= 1900)
extern NTSYSAPI ULONG NTAPI RtlxUnicodeStringToAnsiSize(IN PCUNICODE_STRING UnicodeString);
#else
#ifndef LPWSTR
#define LPWSTR PWSTR
#endif

#ifndef UNICODE_STRING
typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWCH Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#endif

#ifndef NT_SUCCESS
#define NT_SUCCESS(X) ((X) == 0)
#endif

#ifndef ANSI_STRING
typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;
typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;
typedef const PSTRING PCANSI_STRING;
#endif

#ifndef NTSTATUS
#define NTSTATUS int
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1900)
extern NTSYSAPI ULONG NTAPI RtlInitUnicodeString(PUNICODE_STRING UnicodeString, LPVOID Out);
extern NTSYSAPI ULONG NTAPI RtlInitAnsiString(PANSI_STRING UnicodeString, LPVOID Out);
extern NTSYSAPI ULONG NTAPI RtlFreeUnicodeString(PUNICODE_STRING);
extern NTSYSAPI ULONG NTAPI RtlFreeAnsiString(PANSI_STRING);
extern NTSYSAPI ULONG NTAPI RtlAnsiStringToUnicodeString(PUNICODE_STRING UnicodeString, PCANSI_STRING AnsiString, BOOL param);
extern NTSYSAPI ULONG NTAPI RtlUnicodeStringToAnsiString(PANSI_STRING AnsiString, PCUNICODE_STRING UnicodeString, BOOL param);
#endif

extern NTSYSAPI ULONG NTAPI RtlxUnicodeStringToAnsiSize(PCUNICODE_STRING UnicodeString);
#endif

#if 0
#define LOG_DATA 1
#endif

#ifdef LOG_DATA
#   pragma message(__FILE__"(43) : warning !!!! : remove debug code before checking in" )
extern void LogThisData( DWORD id, char *szMsg, DWORD dwLine, DWORD dwData );

#   define LogData( sz, dw1, dw2 )   LogThisData(GetCurrentThreadId(), sz "            ", dw1, dw2 )
#else
#   define LogData( sz, dw1, dw2 )
#endif



#ifndef ARRAYSIZE
#   define ARRAYSIZE(sz)   (sizeof(sz) / sizeof((sz)[0]))
#endif
