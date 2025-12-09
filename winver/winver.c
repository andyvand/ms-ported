/*---------------------------------------------------------------------------
 |   WINVER.C - Windows Version program
 |
 |   History:
 |  03/08/89 Toddla     Created
 |
 *--------------------------------------------------------------------------*/

#ifdef __aarch64__
#undef  __stdcall
#define __stdcall
#endif

#include <windows.h>
#include <port1632.h>
#include <stdio.h>
#include "winverp.h"
#include <shellapi.h>
#include <strsafe.h>

#ifndef __ANONYMOUS_DEFINED
#define __ANONYMOUS_DEFINED
#ifdef __MINGW32__
#define _ANONYMOUS_UNION  __MINGW_EXTENSION
#define _ANONYMOUS_STRUCT __MINGW_EXTENSION
#else
#define _ANONYMOUS_UNION
#define _ANONYMOUS_STRUCT
#endif

#ifndef NONAMELESSUNION
#define _UNION_NAME(x)
#define _STRUCT_NAME(x)
#else /* NONAMELESSUNION */
#define _UNION_NAME(x)  x
#define _STRUCT_NAME(x) x
#endif
#endif	/* __ANONYMOUS_DEFINED */

#ifndef ARRAYSIZE
    #define ARRAYSIZE(x)                    (sizeof(x) / sizeof(x[0]))
#endif

#ifndef MM_SHARED_USER_DATA_VA
#define MM_SHARED_USER_DATA_VA 0x7FFE0000
#endif

#ifndef MAX_WOW64_SHARED_ENTRIES
#define MAX_WOW64_SHARED_ENTRIES 16
#endif

#ifndef USER_SHARED_DATA
#define USER_SHARED_DATA ((KUSER_SHARED_DATA * const)MM_SHARED_USER_DATA_VA)
#endif

#ifndef PROCESSOR_FEATURE_MAX
#define PROCESSOR_FEATURE_MAX 64
#endif

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE {
    StandardDesign,
    NEC98x86,
    EndAlternatives
} ALTERNATIVE_ARCHITECTURE_TYPE;

typedef enum _NT_PRODUCT_TYPE {
    NtProductWinNt = 1,
    NtProductLanManNt,
    NtProductServer
} NT_PRODUCT_TYPE, * PNT_PRODUCT_TYPE;

typedef struct _KSYSTEM_TIME {
    ULONG LowPart;
    LONG High1Time;
    LONG High2Time;
} KSYSTEM_TIME, * PKSYSTEM_TIME;

typedef struct _SHARED_DATA {
    ULONG TickCountLowDeprecated;
    ULONG TickCountMultiplier;
    volatile KSYSTEM_TIME InterruptTime;
    volatile KSYSTEM_TIME SystemTime;
    volatile KSYSTEM_TIME TimeZoneBias;
    USHORT ImageNumberLow;
    USHORT ImageNumberHigh;
    WCHAR NtSystemRoot[260];
    ULONG MaxStackTraceDepth;
    ULONG CryptoExponent;
    ULONG TimeZoneId;
    ULONG LargePageMinimum;
    ULONG Reserved2[7];
    NT_PRODUCT_TYPE NtProductType;
    BOOLEAN ProductTypeIsValid;
    ULONG NtMajorVersion;
    ULONG NtMinorVersion;
    BOOLEAN ProcessorFeatures[PROCESSOR_FEATURE_MAX];
    ULONG Reserved1;
    ULONG Reserved3;
    volatile ULONG TimeSlip;
    ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
    ULONG AltArchitecturePad[1];
    LARGE_INTEGER SystemExpirationDate;
    ULONG SuiteMask;
    BOOLEAN KdDebuggerEnabled;
#if (NTDDI_VERSION >= NTDDI_WINXPSP2)
    UCHAR NXSupportPolicy;
#endif
    volatile ULONG ActiveConsoleId;
    volatile ULONG DismountCount;
    ULONG ComPlusPackage;
    ULONG LastSystemRITEventTickCount;
    ULONG NumberOfPhysicalPages;
    BOOLEAN SafeBootMode;
#if (NTDDI_VERSION >= NTDDI_WIN7)
    _ANONYMOUS_UNION union {
        UCHAR TscQpcData;
        _ANONYMOUS_STRUCT struct {
            UCHAR TscQpcEnabled : 1;
            UCHAR TscQpcSpareFlag : 1;
            UCHAR TscQpcShift : 6;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    UCHAR TscQpcPad[2];
#endif
#if (NTDDI_VERSION >= NTDDI_VISTA)
    _ANONYMOUS_UNION union {
        ULONG SharedDataFlags;
        _ANONYMOUS_STRUCT struct {
            ULONG DbgErrorPortPresent : 1;
            ULONG DbgElevationEnabled : 1;
            ULONG DbgVirtEnabled : 1;
            ULONG DbgInstallerDetectEnabled : 1;
            ULONG DbgSystemDllRelocated : 1;
            ULONG DbgDynProcessorEnabled : 1;
            ULONG DbgSEHValidationEnabled : 1;
            ULONG SpareBits : 25;
        } DUMMYSTRUCTNAME2;
    } DUMMYUNIONNAME2;
#else
    ULONG TraceLogging;
#endif
    ULONG DataFlagsPad[1];
    ULONGLONG TestRetInstruction;
    ULONG SystemCall;
    ULONG SystemCallReturn;
    ULONGLONG SystemCallPad[3];
    _ANONYMOUS_UNION union {
        volatile KSYSTEM_TIME TickCount;
        volatile ULONG64 TickCountQuad;
        _ANONYMOUS_STRUCT struct {
            ULONG ReservedTickCountOverlay[3];
            ULONG TickCountPad[1];
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME3;
    ULONG Cookie;
    ULONG CookiePad[1];
#if (NTDDI_VERSION >= NTDDI_WS03)
    LONGLONG ConsoleSessionForegroundProcessId;
    ULONG Wow64SharedInformation[MAX_WOW64_SHARED_ENTRIES];
#endif
#if (NTDDI_VERSION >= NTDDI_VISTA)
#if (NTDDI_VERSION >= NTDDI_WIN7)
    USHORT UserModeGlobalLogger[16];
#else
    USHORT UserModeGlobalLogger[8];
    ULONG HeapTracingPid[2];
    ULONG CritSecTracingPid[2];
#endif
    ULONG ImageFileExecutionOptions;
#if (NTDDI_VERSION >= NTDDI_VISTASP1)
    ULONG LangGenerationCount;
#else
    /* 4 bytes padding */
#endif
    ULONGLONG Reserved5;
    volatile ULONG64 InterruptTimeBias;
#endif
#if (NTDDI_VERSION >= NTDDI_WIN7)
    volatile ULONG64 TscQpcBias;
    volatile ULONG ActiveProcessorCount;
    volatile USHORT ActiveGroupCount;
    USHORT Reserved4;
    volatile ULONG AitSamplingValue;
    volatile ULONG AppCompatFlag;
    ULONGLONG SystemDllNativeRelocation;
    ULONG SystemDllWowRelocation;
    ULONG XStatePad[1];
    XSTATE_CONFIGURATION XState;
#endif
} KUSER_SHARED_DATA, * PKUSER_SHARED_DATA;

BOOL FileTimeToDateTimeString(
    LPFILETIME pft,
    LPTSTR     pszBuf,
    UINT       cchBuf)
{
    SYSTEMTIME st;
    int cch;

    FileTimeToLocalFileTime(pft, pft);
    if (FileTimeToSystemTime(pft, &st))
    {
        cch = GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, pszBuf, cchBuf);
        if (cch > 0)
        {
            cchBuf -= cch;
            pszBuf += cch - 1;

            *pszBuf++ = TEXT(' ');
            *pszBuf = 0;          // (in case GetTimeFormat doesn't add anything)
            cchBuf--;

            GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &st, NULL, pszBuf, cchBuf);
            return TRUE;
        }
    }    
    return FALSE;
}

/*----------------------------------------------------------------------------*\
|   WinMain( hInst, hPrev, lpszCmdLine, cmdShow )                              |
|                                                                              |
|   Description:                                                               |
|       The main procedure for the App.  After initializing, it just goes      |
|       into a message-processing loop until it gets a WM_QUIT message         |
|       (meaning the app was closed).                                          |
|                                                                              |
|   Arguments:                                                                 |
|   hInst       instance handle of this instance of the app                    |
|   hPrev       instance handle of previous instance, NULL if first            |
|       lpszCmdLine     ->null-terminated command line                         |
|       cmdShow         specifies how the window is initially displayed        |
|                                                                              |
|   Returns:                                                                   |
|       The exit code as specified in the WM_QUIT message.                     |
|                                                                              |
\*----------------------------------------------------------------------------*/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int CmdShow)
{
    TCHAR szTitle[32];
    LARGE_INTEGER Time = USER_SHARED_DATA->SystemExpirationDate;

    ((void)hInstance);
    ((void)hPrevInstance);
    ((void)lpszCmdLine);
    ((void)CmdShow);

    if (LoadString(GetModuleHandle(NULL), IDS_APPTITLE, szTitle, ARRAYSIZE(szTitle)))
    {
	  if (Time.QuadPart)
          {
            TCHAR szExtra[128];
            TCHAR szTime[128];

            if (FileTimeToDateTimeString((PFILETIME)&Time, szTime, ARRAYSIZE(szTime))
                    && LoadString(GetModuleHandle(NULL), IDS_EVALUATION, szExtra, ARRAYSIZE(szExtra)))
            {
                StringCchCat(szExtra, ARRAYSIZE(szExtra), szTime);
                ShellAbout(NULL, szTitle, szExtra, NULL);
            }
        } else
        {
            ShellAbout(NULL, szTitle, NULL, NULL);
        }
    }
    return 0;
}
