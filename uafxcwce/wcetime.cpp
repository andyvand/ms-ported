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
            
const __int64 n1SecIn100NS = (__int64)10000000;

static __int64 wce_GetDeltaSecs(FILETIME f1, FILETIME f2)
{
	// Stuff the 2 FILETIMEs into their own __int64s.
	__int64 t1 = f1.dwHighDateTime;
	t1 <<= 32;				
	t1 |= f1.dwLowDateTime;

	__int64 t2 = f2.dwHighDateTime;
	t2 <<= 32;				
	t2 |= f2.dwLowDateTime;

	// Take the difference of 64-bit ints.
	// This should be the number of 100-nanosecond intervals since Jan. 1970.
	// Divide by 10,000,000 to get the number of seconds since Jan. 1970.
	__int64 iTimeDiff = (t2 - t1) / n1SecIn100NS;
	return iTimeDiff;
}

static SYSTEMTIME wce_TmToSystemTime(tm &t)
{
	SYSTEMTIME s;

	s.wYear      = t.tm_year + 1900;
	s.wMonth     = t.tm_mon+1;
	s.wDayOfWeek = t.tm_wday;
	s.wDay       = t.tm_mday;
	s.wHour      = t.tm_hour;
	s.wMinute    = t.tm_min;
	s.wSecond    = t.tm_sec;
	s.wMilliseconds = 0; //ignored
	// t.tm_yday is not used in SYSTEMTIME

	return s;
}

static FILETIME wce_YearToFileTime(WORD wYear)
{	
	SYSTEMTIME sbase;

	sbase.wYear         = wYear;
	sbase.wMonth        = 1;
	sbase.wDayOfWeek    = 1; //assumed
	sbase.wDay          = 1;
	sbase.wHour         = 0;
	sbase.wMinute       = 0;
	sbase.wSecond       = 0;
	sbase.wMilliseconds = 0;

	FILETIME fbase;
	SystemTimeToFileTime( &sbase, &fbase );

	return fbase;
}

static FILETIME wce_Int64ToFileTime(__int64 iTime)
{
	FILETIME f;

	f.dwHighDateTime = (DWORD)((iTime >> 32) & 0x00000000FFFFFFFF);
	f.dwLowDateTime  = (DWORD)( iTime        & 0x00000000FFFFFFFF);

	return f;
}


static time_t wce_SystemTimeToYDay(SYSTEMTIME s)
{
	FILETIME fMidnightJan1 = wce_YearToFileTime(s.wYear);
	FILETIME f;              SystemTimeToFileTime(&s, &f);
	return (time_t)(wce_GetDeltaSecs(fMidnightJan1, f) / (__int64)86400);
}

static tm wce_SystemTimeToTm(SYSTEMTIME &s)
{
	tm t;

	t.tm_year  = s.wYear - 1900;
	t.tm_mon   = s.wMonth-1;
	t.tm_wday  = s.wDayOfWeek;
	t.tm_mday  = s.wDay;
	t.tm_yday  = wce_SystemTimeToYDay(s);
	t.tm_hour  = s.wHour;
	t.tm_min   = s.wMinute;
	t.tm_sec   = s.wSecond;
	t.tm_isdst = 0; // ignored

	return t;
}

static TIME_ZONE_INFORMATION g_TZInfoCache; // okay to not be thread-safe
static BOOL g_bTZInfoCacheInitialized = FALSE;
__declspec(dllexport) BOOL g_bUseDST = TRUE;

static void wce_GetTZBias(int* pTZBiasSecs = NULL, int* pDSTBiasSecs = NULL)
{
	if(!g_bTZInfoCacheInitialized)
	{
		// WinCE: GetTimeZoneInformation is expensive, so we call it only once.
		if( GetTimeZoneInformation(&g_TZInfoCache) == 0xFFFFFFFF)
			return;
		g_bTZInfoCacheInitialized = TRUE;
	}

	if(pTZBiasSecs != NULL)
	{
		*pTZBiasSecs = g_TZInfoCache.Bias * 60;
		if (g_TZInfoCache.StandardDate.wMonth != 0)
			*pTZBiasSecs += (g_TZInfoCache.StandardBias * 60);
	}

	if(pDSTBiasSecs != NULL)
	{
		if ((g_TZInfoCache.DaylightDate.wMonth != 0) && (g_TZInfoCache.DaylightBias != 0))
			*pDSTBiasSecs = (g_TZInfoCache.DaylightBias - g_TZInfoCache.StandardBias) * 60;
		else
			*pDSTBiasSecs = 0;
	}
}


typedef struct {
		int  yr;        // year of interest
		int  yd;        // day of year 
		long ms;        // milli-seconds in the day 
} transitionTime;

static void wce_cvtdate (int trantype, int year, int month, int week, int dayofweek,
			             int date, int hour, int min, int sec, int msec,
						 transitionTime* pDST)
{
	const int days[]           = {-1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364};
	const int leapYearDays[]   = {-1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
	const int DAY_MILLISEC     = (24L * 60L * 60L * 1000L);
	const int BASE_DOW         = 4; // 01-01-70 was a Thursday 
	const int LEAP_YEAR_ADJUST = 17L; // Leap years 1900 - 1970
	BOOL bIsLeapYear = ((year & 3) == 0);

	int yearday;
	int monthdow;
	int DSTBiasSecs;
	wce_GetTZBias(NULL, &DSTBiasSecs);

	yearday = 1 + (bIsLeapYear ? leapYearDays[month - 1] : days[month - 1]);
	monthdow = (yearday + ((year - 70) * 365) + ((year - 1) >> 2) -
				LEAP_YEAR_ADJUST + BASE_DOW) % 7;
	if ( monthdow <= dayofweek )
		yearday += (dayofweek - monthdow) + (week - 1) * 7;
	else 
		yearday += (dayofweek - monthdow) + week * 7;

	if ((week == 5) && (yearday > (bIsLeapYear ? leapYearDays[month] : days[month])))
		yearday -= 7;

	if ( trantype == 1 ) 
	{   // Converted date was for the start of DST
		pDST->yd = yearday;
		pDST->ms = (long)msec + (1000L * (sec + 60L * (min + 60L * hour)));
		pDST->yr = year;
	}
	else 
	{   // Converted date was for the end of DST
		pDST->yd = yearday;
		pDST->ms = (long)msec + (1000L * (sec + 60L * (min + 60L * hour)));
		if ((pDST->ms += (DSTBiasSecs * 1000L)) < 0) 
		{
			pDST->ms += DAY_MILLISEC;
			pDST->ms--;
		}
		else if (pDST->ms >= DAY_MILLISEC) 
		{
			pDST->ms -= DAY_MILLISEC;
			pDST->ms++;
		}
		pDST->yr = year;
	}
}

int wce_isindst(struct tm *pt)
{
	transitionTime DSTStart = { -1, 0, 0L }, DSTEnd = { -1, 0, 0L };

	if(!g_bUseDST) 
		return 0;

	if(!g_bTZInfoCacheInitialized)
		wce_GetTZBias();

	if((pt->tm_year != DSTStart.yr) || (pt->tm_year != DSTEnd.yr)) 
	{	
		if (g_TZInfoCache.DaylightDate.wYear != 0 || g_TZInfoCache.StandardDate.wYear != 0)
			return 0;

		wce_cvtdate(1,
					pt->tm_year,
					g_TZInfoCache.DaylightDate.wMonth,
					g_TZInfoCache.DaylightDate.wDay,
					g_TZInfoCache.DaylightDate.wDayOfWeek,
					0,
					g_TZInfoCache.DaylightDate.wHour,
					g_TZInfoCache.DaylightDate.wMinute,
					g_TZInfoCache.DaylightDate.wSecond,
					g_TZInfoCache.DaylightDate.wMilliseconds,
					&DSTStart);

		wce_cvtdate(0,
					pt->tm_year,
					g_TZInfoCache.StandardDate.wMonth,
					g_TZInfoCache.StandardDate.wDay,
					g_TZInfoCache.StandardDate.wDayOfWeek,
					0,
					g_TZInfoCache.StandardDate.wHour,
					g_TZInfoCache.StandardDate.wMinute,
					g_TZInfoCache.StandardDate.wSecond,
					g_TZInfoCache.StandardDate.wMilliseconds,
					&DSTEnd);
	}

	if (DSTStart.yd < DSTEnd.yd) 
	{
		// Northern hemisphere ordering
		if ((pt->tm_yday < DSTStart.yd) || (pt->tm_yday > DSTEnd.yd))
			return 0;
		if ((pt->tm_yday > DSTStart.yd) && (pt->tm_yday < DSTEnd.yd))
			return 1;
	}
	else 
	{
		// Southern hemisphere ordering
		if ( (pt->tm_yday < DSTEnd.yd) || (pt->tm_yday > DSTStart.yd) )
			return 1;
		if ( (pt->tm_yday > DSTEnd.yd) && (pt->tm_yday < DSTStart.yd) )
			return 0;
	}

	long ms = 1000L * (pt->tm_sec + 60L * pt->tm_min + 3600L * pt->tm_hour);

	if ( pt->tm_yday == DSTStart.yd ) 
	{
		if ( ms >= DSTStart.ms )
			return 1;
		else
			return 0;
	}
	else 
	{
		// pt->tm_yday == DSTEnd.yd
		if ( ms < DSTEnd.ms )
			return 1;
		else
			return 0;
	}
}

long AFXAPI wce_GetMessageTime()
{
	SYSTEMTIME rSysTime;
	long lTotalMsecs = 0;

	::GetSystemTime( &rSysTime );

	// Msecs = hours * 60min/hr * 60sec/min * 1000mSec/sec
	lTotalMsecs  = (long)rSysTime.wHour * 60L * 60L * 1000L;

	// Msecs = min              * 60sec/min * 1000mSec/sec
	lTotalMsecs += (long)rSysTime.wMinute * 60L * 1000L;

	// Msecs = sec                          * 1000mSec/sec
	lTotalMsecs += (long)rSysTime.wSecond * 1000L;

	// Msecs = mSecs
	lTotalMsecs += (long)rSysTime.wMilliseconds;

	return (lTotalMsecs);
}


time_t AFXAPI wce_mktime(struct tm* pt)
{
	static const FILETIME f1970 = wce_YearToFileTime(1970);

	// Convert input tm to SYSTEMTIME
	SYSTEMTIME s = wce_TmToSystemTime(*pt);

	// Fix the yday (needs to be correct in order for wce_isindst to work)
	pt->tm_yday = wce_SystemTimeToYDay(s);

	// Convert SYSTEMTIME to FILETIME.
	FILETIME f;
	SystemTimeToFileTime( &s, &f );

	// Incorporate time zone and daylight savings time
	int TZBiasSecs;
	int DSTBiasSecs;
	wce_GetTZBias(&TZBiasSecs, &DSTBiasSecs);
	if (wce_isindst(pt))
		TZBiasSecs += DSTBiasSecs;
	
	// Get the number of seconds since Jan 1, 1970
	return (time_t)(wce_GetDeltaSecs(f1970, f) + TZBiasSecs);
}

tm* AFXAPI wce_localtime(const time_t *ptime)
{
	static const __int64 iOffset = 
		wce_GetDeltaSecs(wce_YearToFileTime(1601), wce_YearToFileTime(1970));

	static tm  t;
	FILETIME   f;
	SYSTEMTIME s;
	int		   TZBiasSecs;
	int        DSTBiasSecs;

	memset(&t, 0, sizeof(tm));
	wce_GetTZBias(&TZBiasSecs, &DSTBiasSecs);

	// Set iTime to number of 100 nanosecond intervals since Jan. 1601.
	__int64 iTime = ((__int64)*ptime + iOffset - (__int64)TZBiasSecs) * n1SecIn100NS;

	// Convert iTime to FILETIME
	f = wce_Int64ToFileTime(iTime);

	// Convert the FILETIME to a SYSTEMTIME
	if(FileTimeToSystemTime(&f, &s))
	{
		struct tm t2 = wce_SystemTimeToTm(s);
		if(wce_isindst(&t2))
		{
			// Take DST into account
			f = wce_Int64ToFileTime(iTime - DSTBiasSecs * n1SecIn100NS);
			FileTimeToSystemTime(&f, &s);
		}
		t = wce_SystemTimeToTm(s);
	}

	return &t;
}

time_t AFXAPI wce_time( time_t *timer )
{
	ASSERT(timer == NULL); // not supported
	
	SYSTEMTIME s;
	GetLocalTime( &s );
	tm t = wce_SystemTimeToTm(s);

	return wce_mktime( &t );
}

char* AFXAPI wce_ctime( const time_t *ulSecsSince1970 )
{
	static TCHAR szBuf[28];
	struct tm *ptime = wce_localtime(ulSecsSince1970);

	_tcsncpy(&szBuf[0], _T("Sun Mon Tue Wed Thu Fri Sat ") + (ptime->tm_wday*4), 4);
	_tcsncpy(&szBuf[4], _T("Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ") + (ptime->tm_mon*4), 4 );
	wsprintf(&szBuf[8], _T("%02d %02d:%02d:%02d %04d\n") ,
		ptime->tm_mday, ptime->tm_hour, ptime->tm_min, ptime->tm_sec, ptime->tm_year+1900);

	// convert to ascii
	TCHAR *szWide = szBuf;
	char *szAscii = (char*)szBuf;
	do
	{
		*szAscii++ = *(char*)szWide;
	} while(*szWide++);

	return (char*)szBuf;
}

