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
// electronic documentation  provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

#include "wcedb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// The propid's HIWORD is the application-defined identifier (ident)
// The propid's LOWORD is the datatype (CEVT_* values)

inline void SET_PROPID_CEVT(CEPROPID& propid, WORD wType)
{
	propid = MAKELONG(wType,HIWORD(propid));
}

inline WORD GET_PROPID_CEVT(CEPROPID propid) 
{
	return LOWORD(propid);
}

inline void SET_PROPID_IDENT(CEPROPID& propid,WORD wIdent)
{
	propid = MAKELONG(LOWORD(propid),wIdent);
}

inline WORD GET_PROPID_IDENT(CEPROPID propid) 
{
	return HIWORD(propid);
}

inline CEPROPID MAKE_PROPID(WORD wType,WORD wIndent)
{
	return MAKELONG(wType,wIndent);
}

//////////////////////////////////////////////////////////////////////
// CCeDBDatabase construction/Destruction
//////////////////////////////////////////////////////////////////////

CCeDBDatabase::CCeDBDatabase()
{
	m_CEOID  = NULL;
	m_hHandle = INVALID_HANDLE_VALUE;
	m_bAutoSeekNext = FALSE;
	m_bEOF = FALSE; // maintained via SeekNext() (so you should set
	                // m_bAutoSeekNext if you want to use EOF() when 
	                // reading records in sequentially)
}


CCeDBDatabase::~CCeDBDatabase()
{
	if(m_hHandle != INVALID_HANDLE_VALUE)
		Close();
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Basic operations
//////////////////////////////////////////////////////////////////////

CEOID CCeDBDatabase::Create(LPCWSTR szName,
                              DWORD dwIdent /*= 0*/,
                              int nNumSortProps /*= 0*/,
                              const CCeDBProp* pSortProps /*= NULL*/)
{
	AssertValidDbaseName(szName);  
	ASSERT(m_hHandle == INVALID_HANDLE_VALUE);
	ASSERT((nNumSortProps >= 0) && (nNumSortProps <= 4));
	if(nNumSortProps == 0)
		m_CEOID = CeCreateDatabase((LPWSTR)szName,dwIdent,0,NULL); 
	else
	{
		ASSERT(pSortProps != NULL);
		int i;

	// The WinCE database doesn't allow multiple sort keys on a single property,
	// so check for it
#ifdef _DEBUG
		for(i=0; i < nNumSortProps-1; i++)
			for(int j=i+1; j < nNumSortProps; j++)
				if(pSortProps[i].GetIdent() == pSortProps[j].GetIdent())
					ASSERT(FALSE); // A single property appeared more than once in the sort order!
#endif

		SORTORDERSPEC Spec[4];
		for(i=0; i < nNumSortProps; i++)
		{
			Spec[i].propid = pSortProps[i].GetPropid();
			Spec[i].dwFlags = (DWORD)pSortProps[i].m_wSortFlags;
		}

		WORD wCnt = (WORD)nNumSortProps;
		m_CEOID = CeCreateDatabase((LPWSTR)szName,dwIdent,wCnt,Spec); 
	}
	
	return m_CEOID;
}



BOOL CCeDBDatabase::Open(CEOID CeOID,
	                      const CCeDBProp* pKeyProp /*= NULL*/, 
	                      const CWnd* pWndNotify /*= NULL*/)
{
	HWND hWnd;
	CEPROPID propid = pKeyProp?pKeyProp->GetPropid():0;

	ASSERT(m_hHandle == INVALID_HANDLE_VALUE);

	if(pWndNotify == NULL)
		hWnd = NULL;
	else
	{
#ifdef _DEBUG
		pWndNotify->AssertValid();
#endif
		hWnd = pWndNotify->GetSafeHwnd();
	}

	m_CEOID = CeOID;
	m_bEOF = FALSE;
	m_hHandle = CeOpenDatabase(&m_CEOID,NULL,propid,0,hWnd);
	return (m_hHandle != INVALID_HANDLE_VALUE);
}


BOOL CCeDBDatabase::Open(LPCWSTR szName,
	                      const CCeDBProp* pKeyProp /*= NULL*/, 
	                      const CWnd* pWndNotify /*= NULL*/)
{
	CEPROPID propid = pKeyProp?pKeyProp->GetPropid():0;
	HWND hWnd;

	ASSERT(m_hHandle == INVALID_HANDLE_VALUE);

	if(pWndNotify == NULL)
		hWnd = NULL;
	else
	{
#ifdef _DEBUG
		pWndNotify->AssertValid();
#endif
		hWnd = pWndNotify->GetSafeHwnd();
	}
	m_CEOID = 0;
	m_bEOF = FALSE;
	m_hHandle = CeOpenDatabase(&m_CEOID,(LPWSTR)szName,propid,0,hWnd);
	return (m_hHandle != INVALID_HANDLE_VALUE);
}


BOOL CCeDBDatabase::Close() // Note, closing is optional, since the
                           // destructor calls this automatically.
{
	if(m_hHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	if(!CloseHandle(m_hHandle))
		return FALSE;

	m_hHandle = INVALID_HANDLE_VALUE;
	return TRUE;
}

BOOL CCeDBDatabase::Delete() // Note, we might want to make this a static function instead.
                              // It's silly to open it, then delete it, but then again, we 
							  // can take advantage of ref by name.
{
	ASSERT(m_CEOID != NULL);

	Close();

	if(!CeDeleteDatabase(m_CEOID))
		return FALSE;

	m_CEOID = FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Static information functions (i.e. no open database used)
//////////////////////////////////////////////////////////////////////

BOOL CCeDBDatabase::Exists(LPCWSTR szName)
{
	AssertValidDbaseName(szName);
	
	CEOID CeOID = NULL;

	// Note: this is a hack because we actually try to open it
	// instead of using an existence function (I'm not sure if
	// it exists.)
	HANDLE hHandle = CeOpenDatabase(&CeOID,(LPWSTR)szName,0,0,NULL);

	// Need to find out what POD does when you give it a valid
	// CeOID that is NOT a database.  If so, return FALSE.

	// Otherwise, this "hack" created a new resource, get rid of it
	if(hHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hHandle); // not concerned about failure here
		return TRUE;
	}

	if(::GetLastError() == ERROR_FILE_NOT_FOUND)
		return FALSE;

	// ASSUMPTION NOTE:
	// The other expected error is ERROR_NOT_ENOUGH_MEMORY,
	// we are assuming that the file exists in this case, i.e.
	// CeOpenDatabase() DID find a file, BUT couldn't open it.

	return TRUE;
}



BOOL CCeDBDatabase::Exists(CEOID CeOID)
{
	ASSERT(CeOID != NULL);

	// Note: this is a hack because we actually try to open it
	// instead of using an existence function (I'm not sure if
	// it exists.)
	HANDLE hHandle = CeOpenDatabase(&CeOID,NULL,0,0,NULL);

	// Need to find out what POD does when you give it a valid
	// CeOID that is NOT a database.  If so, return FALSE.

	// Otherwise, this "hack" created a new resource, get rid of it
	if(hHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hHandle); // not concerned about failure here
		return TRUE;
	}

	if(::GetLastError() == ERROR_FILE_NOT_FOUND)
		return FALSE;

	// ASSUMPTION NOTE:
	// The other expected error is ERROR_NOT_ENOUGH_MEMORY,
	// we are assuming that the file exists in this case, i.e.
	// CeOpenDatabase() DID find a file, BUT couldn't open it.

	return FALSE;
}

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Database enumeration
//////////////////////////////////////////////////////////////////////

CCeDBEnum::CCeDBEnum(DWORD dwIdent /*= 0*/)
{
	m_hHandle = CeFindFirstDatabase(dwIdent);
}

CEOID CCeDBEnum::Next()
{
	if(m_hHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	return CeFindNextDatabase(m_hHandle);
}

CCeDBEnum::~CCeDBEnum()
{
	if(m_hHandle != INVALID_HANDLE_VALUE)
		CloseHandle(m_hHandle); 
}

//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Read/write/delete operations 
//////////////////////////////////////////////////////////////////////

// O(1)
BOOL CCeDBDatabase::ReadCurrRecord(CCeDBRecord* pRecord,
	 					             int nNumFilterProps /*= 0*/,
								     CCeDBProp* PropFilterArray /*= NULL*/)
{
	AssertValid();

	if(m_bAutoSeekNext && m_bEOF)
	{
		::SetLastError(ERROR_HANDLE_EOF);
		return FALSE; // no more records to read
	}

	CEOID CeOID;
	DWORD dwSize;
	CEPROPID* inputPropIDArray = NULL;

	if(nNumFilterProps > 0)
	{
		TRY
		{
			inputPropIDArray = new CEPROPID[nNumFilterProps];
		}
		CATCH(CException,e)
		{
			::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return FALSE;
		}
		END_CATCH;

		for(int nCnt=0; nCnt < nNumFilterProps; nCnt++)
			inputPropIDArray[nCnt] = PropFilterArray[nCnt].GetPropid(); 
	}

	pRecord->FreeRecord();   // clear up this record before filling in again...
	                         // this will delete strings and blobs used by
	                         // the current properties in this record
	
	WORD wNumPropIdents = (WORD)nNumFilterProps;

	// NOTE: this will return the last record even if the seek has gone off the edge.

	CeOID = CeReadRecordProps(m_hHandle,CEDB_ALLOWREALLOC,
		                        &wNumPropIdents,inputPropIDArray,
								&pRecord->m_pbBuffer,&dwSize);

	int nNumProps = (int)wNumPropIdents;

	if(CeOID != NULL)
	{
		CEPROPVAL* PropValArray = (CEPROPVAL*)pRecord->m_pbBuffer;
		CCeDBProp* pProp;
		for(int n=0; n < (int)nNumProps; n++)
		{
			pProp = new CCeDBProp;
			*pProp = CCeDBProp(PropValArray[n],CCeDBProp::NoCopy);
			pProp->m_bDeletePtr = TRUE;
			pRecord->AddProp(pProp);
		}
		pRecord->m_iCEOID = CeOID;
		// Note: lpwstr and blob properties point into pRecord->m_pbBuffer.
		// That was why we passed in CCePDBProp::NoCopy.  The record
		// will free up the buffer when destructed or when Free() is called.

		if(m_bAutoSeekNext)
			SeekNext();
	}

	if(inputPropIDArray != NULL)
		delete[] inputPropIDArray;

	return (CeOID != NULL);
}




// private function that implements the above.

CEOID CCeDBDatabase::WriteRecordProps(CCeDBRecord *pRecord,
									  int nMode /*= CCeDBDatabase::ModifyProps*/,
	 					              int nNumFilterProps /*= 0*/, 
								      CCeDBProp *PropFilterArray /*= NULL*/)
{
	AssertValid();

	CEOID CeOID;
	CEPROPVAL* inputPropValArray = NULL;
	BOOL bAll = FALSE;
	int nNumRecordPropsAvail = pRecord->m_pPropArray.GetSize();
	CCeDBProp* pProp;

	if(nNumFilterProps == 0) // zero means all
	{
		nNumFilterProps = nNumRecordPropsAvail;
		bAll = TRUE;

		if(nNumFilterProps == 0)
		{
			::SetLastError(ERROR_NO_DATA);
			return NULL;
		}
	}

	TRY
	{
		inputPropValArray = new CEPROPVAL[nNumFilterProps];
	}
	CATCH(CException,e)
	{
		::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return NULL;
	}
	END_CATCH;


	if(bAll)
	{
		for(int nCnt1=0; nCnt1 < (int)nNumFilterProps; nCnt1++)
		{
			pProp = pRecord->GetPropFromIndex(nCnt1);
			CEPROPID propid = pProp->m_CePropVal.propid;
			inputPropValArray[nCnt1] = pProp->m_CePropVal;
		}
	}
	else
	{
		ASSERT(PropFilterArray != NULL);

		int nSkip = 0;
		for(int nCnt1=0; nCnt1 < nNumFilterProps; nCnt1++)
		{
			for(int nCnt2=0; nCnt2 < nNumRecordPropsAvail; nCnt2++)
			{
				pProp = pRecord->GetPropFromIndex(nCnt2);
				if(GET_PROPID_IDENT(PropFilterArray[nCnt1].m_CePropVal.propid) == 
				   GET_PROPID_IDENT(pProp->m_CePropVal.propid))
				{
					inputPropValArray[nCnt1-nSkip] = pProp->m_CePropVal;
					break;
				}
			}

			if(nCnt2 == nNumRecordPropsAvail) // requested ident not in record, so inc. skip
				nSkip++;
		}

		nNumFilterProps -= nSkip; // adjust for skipped idents
	}


	if(nNumFilterProps == 0)
	{
		::SetLastError(ERROR_NO_DATA);
		delete[] inputPropValArray;
		return NULL;
	}

	if (pRecord->m_iCEOID == NULL)  
	{
		if(nMode == NewRecord)
		{
			CeOID = CeWriteRecordProps(m_hHandle,0,(WORD)nNumFilterProps,inputPropValArray);
		}
		else
		{
			if(nMode == DeleteProps)
			{
				for(int nCnt=0; nCnt < nNumFilterProps; nCnt++)
					inputPropValArray[nCnt].wFlags = CEDB_PROPDELETE;
			}

			CeOID = CeWriteRecordProps(m_hHandle,GetCurrRecord(),
		                             (WORD)nNumFilterProps,inputPropValArray);
			if(nMode == DeleteProps)
				ASSERT(CeOID == GetCurrRecord());
		}
	}
	else
	{
		if(nMode == DeleteProps)
		{
			for(int nCnt=0; nCnt < nNumFilterProps; nCnt++)
				inputPropValArray[nCnt].wFlags = CEDB_PROPDELETE;
		}

		CeOID = CeWriteRecordProps(m_hHandle, pRecord->m_iCEOID,
	                             (WORD)nNumFilterProps,inputPropValArray);
		if(nMode == DeleteProps)
			ASSERT(CeOID == pRecord->m_iCEOID);

	}

	
	if(CeOID != NULL)
	{
		if(m_bAutoSeekNext)
			SeekNext();
	}

	if(inputPropValArray != NULL)
		delete[] inputPropValArray;

	return (CeOID);
}


BOOL CCeDBDatabase::DeleteCurrRecordProps(int nNumProps /*= 0*/,
										   CCeDBProp *pPropArray /*= NULL*/)
{
	BOOL bRetVal;

	CCeDBRecord rec;	

	if(!ReadCurrRecord(&rec))  // inefficient
		return FALSE;

	if(nNumProps == 0) // zero properties means delete all, so find out the properties. 
	{                  // note: this is pretty inefficient

		nNumProps = rec.GetNumProps();
		if(nNumProps == 0) 
			return TRUE; 

		TRY
		{
			pPropArray = new CCeDBProp[nNumProps];
		}
		CATCH(CException,e)
		{
			::SetLastError(ERROR_NOT_ENOUGH_MEMORY);
			return FALSE;
		}
		END_CATCH;

		for(int n=0; n < nNumProps; n++)
			pPropArray[n] = *rec.GetPropFromIndex(n);

		bRetVal = (WriteRecordProps(&rec,DeleteProps,nNumProps,pPropArray) != NULL);
		delete[] pPropArray;
	}
	else
	{
		bRetVal = (WriteRecordProps(&rec,DeleteProps,nNumProps,pPropArray) != NULL);
	}

	return bRetVal;
}



//////////////////////////////////////////////////////////////////////
// Queries 
//////////////////////////////////////////////////////////////////////

// Note: AssertValid() is only needed in GetInfo() and SetInfo(), as
// the other Get and Set methods call them.

CEDBASEINFO CCeDBDatabase::GetInfo(BOOL* pbOK) const
{
	AssertValid();

    CEOIDINFO CeObjectInfo;

	if(!CeOidGetInfo(m_CEOID,&CeObjectInfo))
	{
		*pbOK = FALSE;
		memset(&CeObjectInfo.infDatabase,0,sizeof(CEDBASEINFO));
		return CeObjectInfo.infDatabase;
	}
	
	ASSERT(CeObjectInfo.wObjType == OBJTYPE_DATABASE);

	*pbOK = TRUE;

	return CeObjectInfo.infDatabase;
}

BOOL CCeDBDatabase::SetInfo(const CEDBASEINFO* pCeDBaseInfo)
{
#if (_WIN32_WCE >= 300)
#ifdef _DEBUG
	CObject::AssertValid();
#endif
	ASSERT(m_CEOID != NULL);
	ASSERT(m_hHandle == INVALID_HANDLE_VALUE);
#else
	AssertValid();
#endif	

	return CeSetDatabaseInfo(m_CEOID,(CEDBASEINFO*)pCeDBaseInfo);
}



WORD CCeDBDatabase::GetNumRecords() const   
{ 
	BOOL bOk;
	return GetInfo(&bOk).wNumRecords;    
}

// No SetNumRecords() counterpart

DWORD CCeDBDatabase::GetSize() const         
{ 
	BOOL bOk;
	return GetInfo(&bOk).dwSize;         
}

// No SetSize() counterpart


void CCeDBDatabase::GetName(LPWSTR szName) const         
{ 
	BOOL bOK;
	wcscpy(szName,GetInfo(&bOK).szDbaseName);
}

BOOL CCeDBDatabase::SetName(LPCWSTR wszName)
{
	AssertValidDbaseName(wszName);

	CEDBASEINFO CeDBaseInfo;
	memset(&CeDBaseInfo,0,sizeof(CeDBaseInfo));
	wcsncpy(CeDBaseInfo.szDbaseName,wszName,CEDB_MAXDBASENAMELEN);
	CeDBaseInfo.dwFlags = CEDB_VALIDNAME;

	return SetInfo(&CeDBaseInfo);
}


DWORD CCeDBDatabase::GetIdent() const
{ 
	BOOL bOK;
	return GetInfo(&bOK).dwDbaseType;
}

BOOL CCeDBDatabase::SetIdent(DWORD dwIdent)
{
	CEDBASEINFO CeDBaseInfo;
	memset(&CeDBaseInfo,0,sizeof(CeDBaseInfo));
	CeDBaseInfo.dwDbaseType = dwIdent;
	CeDBaseInfo.dwFlags = CEDB_VALIDTYPE;

	return SetInfo(&CeDBaseInfo);
}


FILETIME CCeDBDatabase::GetLastModified() const
{ 
	BOOL bOK;
	return GetInfo(&bOK).ftLastModified; 
}

BOOL CCeDBDatabase::SetLastModified(FILETIME ftLastModified) 
{
	CEDBASEINFO CeDBaseInfo;
	memset(&CeDBaseInfo,0,sizeof(CeDBaseInfo));
	
	CeDBaseInfo.ftLastModified = ftLastModified;
	CeDBaseInfo.dwFlags = CEDB_VALIDMODTIME;

	return SetInfo(&CeDBaseInfo);
}



// Note: pSortProps is assumed to be an array of at least CEDB_MAXSORTORDER elements!
void CCeDBDatabase::GetSortProps(int *pnNumProps,CCeDBProp *pSortProps) const
{
	ASSERT(pnNumProps != NULL);
	ASSERT(pSortProps != NULL);
	
	BOOL bOK;
	CEDBASEINFO dbInfo = GetInfo(&bOK);
	if(!bOK)
	{
		*pnNumProps = 0;
		return;
	}

	SORTORDERSPEC *pSpec;

	*pnNumProps = (int)dbInfo.wNumSortOrder;
	ASSERT((*pnNumProps >= 0) && (*pnNumProps <= CEDB_MAXSORTORDER));

	for(int i=0; i < *pnNumProps; i++)
	{
		pSpec = &dbInfo.rgSortSpecs[i];
		pSortProps[i] = CCeDBProp(CCeDBProp::CEVTtoType(GET_PROPID_CEVT(pSpec->propid)),
		                           GET_PROPID_IDENT(pSpec->propid),
		                           (WORD)pSpec->dwFlags);
	}
} 


BOOL CCeDBDatabase::SetSortProps(int nNumProps,const CCeDBProp *pSortProps)
{
	ASSERT((nNumProps >= 0) && (nNumProps <= CEDB_MAXSORTORDER));
	
	CEDBASEINFO CeDBaseInfo;
	int i;

	CeDBaseInfo.dwFlags       = CEDB_VALIDSORTSPEC;
	CeDBaseInfo.wNumSortOrder = (WORD)nNumProps;

	if(nNumProps >= 0)
		ASSERT(pSortProps != NULL);

	// The WinCE database doesn't allow multiple sort keys on a single property,
	// so check for it
#ifdef _DEBUG
	for(i=0; i < nNumProps-1; i++)
		for(int j=i+1; j < nNumProps; j++)
			if(pSortProps[i].GetIdent() == pSortProps[j].GetIdent())
				ASSERT(FALSE); // A single property appeared more than once in the sort order!
#endif

	for(i=0; i < nNumProps; i++)
	{
		CeDBaseInfo.rgSortSpecs[i].propid = pSortProps[i].GetPropid();
		CeDBaseInfo.rgSortSpecs[i].dwFlags = (DWORD)pSortProps[i].m_wSortFlags;
	}

	return SetInfo(&CeDBaseInfo);
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Seeking operations
//////////////////////////////////////////////////////////////////////


// Note: the only seek operation that sets m_bEOF to TRUE is SeekNext(),
// when it is unsuccessful.
// m_bEOF is set to FALSE on all seek operations that are successful.

// O(dwIndex) if bFromEnd == FALSE, O(n) if bfromEnd == TRUE
CEOID CCeDBDatabase::SeekToIndex(DWORD dwIndex,BOOL bFromEnd /*= FALSE*/) 
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID;
	
	ASSERT(dwIndex >= 0);
	if(bFromEnd)
	{
		NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_END,-(signed long)dwIndex,&dwNewIndex);
	}
	else
	{
		NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_BEGINNING,dwIndex,&dwNewIndex);
	}

	if(NewCeOID != 0)
		m_bEOF = FALSE;

	return NewCeOID;
}

// O(n)
CEOID CCeDBDatabase::SeekToRecord(CEOID CeOID)
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_CEOID,CeOID,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;

	return NewCeOID; // may be zero if not found
}


// O(1)
// This also maintains the EOF flag
CEOID CCeDBDatabase::SeekNext()
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_CURRENT,1,&dwNewIndex);

	m_bEOF = (NewCeOID == 0);

	return NewCeOID;
} 

// O(n)
CEOID CCeDBDatabase::SeekPrev()
{
	AssertValid();

	DWORD dwOldIndex,dwNewIndex;
	CeSeekDatabase(m_hHandle,CEDB_SEEK_CURRENT,0,&dwOldIndex); 
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_CURRENT,-1,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;

	if(dwNewIndex == dwOldIndex)
		return NULL;
	else
		return NewCeOID;
} 
 

// O(n)
CEOID CCeDBDatabase::SeekFirstEqual(CCeDBProp& Prop)
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_VALUEFIRSTEQUAL,(DWORD)&Prop.m_CePropVal,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;
	return NewCeOID;
}

// O(1)
CEOID CCeDBDatabase::SeekNextEqual(CCeDBProp& Prop)
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_VALUENEXTEQUAL,(DWORD)&Prop.m_CePropVal,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;
	return NewCeOID;
}

// O(n)
CEOID CCeDBDatabase::SeekValueSmaller(CCeDBProp& Prop)
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_VALUESMALLER,(DWORD)&Prop.m_CePropVal,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;
	return NewCeOID;
}

// O(n)
CEOID CCeDBDatabase::SeekValueGreater(CCeDBProp& Prop)
{
	AssertValid();

	DWORD dwNewIndex;
	CEOID NewCeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_VALUEGREATER,(DWORD)&Prop.m_CePropVal,&dwNewIndex);
	if(NewCeOID != 0)
		m_bEOF = FALSE;
	return NewCeOID;
}

// O(1)
DWORD CCeDBDatabase::GetCurrIndex() const
{
	AssertValid();

	DWORD dwCurrIndex;
	CEOID CeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_CURRENT,0,&dwCurrIndex);

	if(CeOID == 0)
		return (DWORD)-1;
	else
		return dwCurrIndex;
}

// O(1)
CEOID CCeDBDatabase::GetCurrRecord() const
{
	AssertValid();

	DWORD dwCurrIndex;
	CEOID CeOID = CeSeekDatabase(m_hHandle,CEDB_SEEK_CURRENT,0,&dwCurrIndex);

	return CeOID;
}


//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Diagnostics
//////////////////////////////////////////////////////////////////////

void CCeDBDatabase::AssertValid() const
{
#ifdef _DEBUG
	CObject::AssertValid();
#endif
	ASSERT(m_CEOID != NULL);
	ASSERT(m_hHandle != INVALID_HANDLE_VALUE);
}

void CCeDBDatabase::AssertValidDbaseName(LPCWSTR szName)
{
	int nLen = wcslen(szName);
	ASSERT(nLen > 0);
	ASSERT(nLen <= CEDB_MAXDBASENAMELEN-1);
}

//////////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////////
// Constructors/destructor for CCeDBProp
//////////////////////////////////////////////////////////////////////

// Default constructor places the value (long)0 
// Note: in 32-bit Windows, long is the same as int
CCeDBProp::CCeDBProp()
{
	m_bDeletePtr = FALSE; // assume that the user is allocating the memory for this
	                      // (set to TRUE if a CCeDBProp* gets added to a record and
	                      // it was allocated from within WCEDB.CPP)
	InitProp(); // Note, all constructors need to call InitProp() first because
	        // each of the Set... helper functions call Delete(), which
	        // will free pointers.
	SetLong(0);
}


// The following constructor creates valueless properties useful for
// specifying sort keys.
CCeDBProp::CCeDBProp(enum CCeDBProp::enType nType,WORD wIdent /*= 0*/,
					   WORD wSortFlags /*= CCeDBProp::Sort_Ascending*/)
{
	m_bDeletePtr = FALSE; // see comment in CCeDBProp()
	InitProp();
	SetType(nType);
	SetIdent(wIdent);
	SetSortFlags(wSortFlags);
};


// The following constructor is used when reading a whole record from
// the database with CeReadRecordProps(), which creates the CEPROPVAL's
// with the wIdents for you.  If the CEPROPVAL doesn't have the identifier
// in it, SetIdent() should be used to set it.
CCeDBProp::CCeDBProp(CEPROPVAL CePropVal,enum CCeDBProp::enCopyMode nMode /*= Copy*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	if(CePropVal.propid == CEVT_LPWSTR)
	{
		SetString(CePropVal.val.lpwstr,nMode);
		SetIdent(GET_PROPID_IDENT(CePropVal.propid));
	}
	else if(CePropVal.propid == CEVT_BLOB)
	{
		SetBlob(CePropVal.val.blob,nMode);
		SetIdent(GET_PROPID_IDENT(CePropVal.propid));
	}
	else
	{
		m_CePropVal = CePropVal; // nMode doesn't apply
	}
}

CCeDBProp::CCeDBProp(short iVal,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetShort(iVal);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(USHORT uiVal,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetUShort(uiVal);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(long lVal,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE; // see comment in CCeDBProp()
	InitProp();
	SetLong(lVal);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(ULONG ulVal,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetULong(ulVal);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(FILETIME filetime,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetFiletime(filetime);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(LPWSTR lpwstr,WORD wIdent /*= 0*/,enum CCeDBProp::enCopyMode nMode /*= CCeDBProp::Copy*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	ASSERT(lpwstr != NULL); // if you pass in 0, this constructor gets used.  
	InitProp();
	if(lpwstr != NULL)
	{
		SetString(lpwstr,nMode);
		SetIdent(wIdent);

	}
}

CCeDBProp::CCeDBProp(CEBLOB blob,WORD wIdent /*= 0*/,enum CCeDBProp::enCopyMode nMode /*= CCeDBProp::Copy*/)
{
	m_bDeletePtr = FALSE;   // see comment in CCeDBProp()
	InitProp();
	SetBlob(blob,nMode);
	SetIdent(wIdent);
}

#if (_WIN32_WCE > 201)
CCeDBProp::CCeDBProp(BOOL bBool,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetBool(bBool);
	SetIdent(wIdent);
}

CCeDBProp::CCeDBProp(DOUBLE dVal,WORD wIdent /*= 0*/)
{
	m_bDeletePtr = FALSE;  // see comment in CCeDBProp()
	InitProp();
	SetDouble(dVal);
	SetIdent(wIdent);
}
#endif // _WIN32_WCE

// copy constructor
CCeDBProp::CCeDBProp(CCeDBProp& prop)
{
	m_CePropVal = prop.m_CePropVal;
	m_bOwnsStringOrBlob = prop.m_bOwnsStringOrBlob; // transfer ownership to this, since prop
	                                                // is likely to be a temporary object
	m_wSortFlags = prop.m_wSortFlags;
	prop.m_bOwnsStringOrBlob = FALSE; // take away ownership from prop
	
	// m_bDeletePtr is not affected here
}

// assignment operator
CCeDBProp& CCeDBProp::operator=(const CCeDBProp& prop)
{
	FreeProp(); // free original object
	m_CePropVal = prop.m_CePropVal;
	m_wSortFlags = prop.m_wSortFlags;
	m_bOwnsStringOrBlob = FALSE; // leave ownership in prop 

	// m_bDeletePtr is not affected here
	return *this;
}

CCeDBProp::~CCeDBProp()
{
	FreeProp();
}

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Get/Set operations on CCeDBProp
//////////////////////////////////////////////////////////////////////


void CCeDBProp::SetIdent(WORD wIdent)
{
	SET_PROPID_IDENT(m_CePropVal.propid,wIdent);
}

WORD CCeDBProp::GetIdent() const
{
	return GET_PROPID_IDENT(m_CePropVal.propid);
}


/*private:*/ void CCeDBProp::SetCEVT(WORD wType)
{
	SET_PROPID_CEVT(m_CePropVal.propid,wType);
}

/*private:*/ WORD CCeDBProp::GetCEVT() const
{
	return GET_PROPID_CEVT(m_CePropVal.propid);
}


void CCeDBProp::SetType(CCeDBProp::enType nType)
{
	SET_PROPID_CEVT(m_CePropVal.propid,TypeToCEVT(nType));
}


CCeDBProp::enType CCeDBProp::GetType() const
{
	return CEVTtoType(GET_PROPID_CEVT(m_CePropVal.propid));
}


/*private: static */ CCeDBProp::enType CCeDBProp::CEVTtoType(WORD wType)
{
	if (wType == 0)
		return CCeDBProp::Type_Empty;
	else if(wType == CEVT_I2)
		return CCeDBProp::Type_Short;
	else if(wType == CEVT_UI2)
		return CCeDBProp::Type_UShort;
	else if(wType == CEVT_I4)
		return CCeDBProp::Type_Long;
	else if(wType == CEVT_UI4)
		return CCeDBProp::Type_ULong;
	else if(wType == CEVT_FILETIME)
		return CCeDBProp::Type_Filetime;
	else if(wType == CEVT_LPWSTR)
		return CCeDBProp::Type_String;
	else if(wType == CEVT_BLOB)
		return CCeDBProp::Type_Blob;

#if (_WIN32_WCE > 201)
	else if(wType == CEVT_BOOL)
		return CCeDBProp::Type_Bool;
	else if(wType == CEVT_R8)
		return CCeDBProp::Type_Double;
#endif // _WIN32_WCE

	else 
		ASSERT(FALSE);

	return CCeDBProp::enType(0); 
}

/*private: static*/ WORD CCeDBProp::TypeToCEVT(CCeDBProp::enType nType)
{
	ASSERT((nType >= Type_Short) && (nType <= Type_Empty));

#if (_WIN32_WCE > 201)
	static WORD wTypeMap[] = 
	{
		CEVT_I2,CEVT_UI2,CEVT_I4,CEVT_UI4,
		CEVT_FILETIME,CEVT_LPWSTR,CEVT_BLOB,CEVT_BOOL,CEVT_R8, 0
	};
#else
	static WORD wTypeMap[] = 
	{
		CEVT_I2,CEVT_UI2,CEVT_I4,CEVT_UI4,
		CEVT_FILETIME,CEVT_LPWSTR,CEVT_BLOB, 0
	};
#endif
	return wTypeMap[nType];
}




/*private:*/ CEPROPID CCeDBProp::GetPropid() const 
{
	return m_CePropVal.propid;
}

// Note: there's no SetPropid().  We use SetType() and SetIdent() instead.


void CCeDBProp::SetSortFlags(WORD wSortFlags)
{
	ASSERT(0 == (wSortFlags & ~( Sort_Ascending  |
	                             Sort_Descending |
								 Sort_CaseInsensitive |
								 Sort_UnknownFirst |
								 Sort_GenericOrder ) ) );
	m_wSortFlags = wSortFlags;
}

WORD CCeDBProp::GetSortFlags() const
{
	return m_wSortFlags;
}

void CCeDBProp::SetShort(short iVal)
{
	FreeProp(); // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_I2);
	m_CePropVal.val.iVal = iVal;
}

short CCeDBProp::GetShort() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_I2);
	return m_CePropVal.val.iVal;
}


void CCeDBProp::SetUShort(USHORT uiVal)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_UI2);
	m_CePropVal.val.uiVal = uiVal;
}

USHORT CCeDBProp::GetUShort() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_UI2);
	return m_CePropVal.val.uiVal;
}

void CCeDBProp::SetLong(long lVal)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_I4);
	m_CePropVal.val.lVal = lVal;
}

long CCeDBProp::GetLong() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_I4);
	return m_CePropVal.val.lVal;
}

#if (_WIN32_WCE > 201)
void CCeDBProp::SetBool(BOOL bBool)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_BOOL);
	m_CePropVal.val.boolVal = bBool;
}

BOOL CCeDBProp::GetBool() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_BOOL);
	return m_CePropVal.val.boolVal;
}

void CCeDBProp::SetDouble(DOUBLE dDouble)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_R8);
	m_CePropVal.val.dblVal = dDouble;
}

DOUBLE CCeDBProp::GetDouble() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_R8);
	return m_CePropVal.val.dblVal;
}
#endif // _WIN32_WCE

void CCeDBProp::SetULong(ULONG ulVal)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_UI4);
	m_CePropVal.val.ulVal = ulVal;
}

ULONG CCeDBProp::GetULong() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_UI4);
	return m_CePropVal.val.ulVal;
}

void CCeDBProp::SetFiletime(FILETIME filetime)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_FILETIME);
	m_CePropVal.val.filetime = filetime;
}

FILETIME CCeDBProp::GetFiletime() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_FILETIME);
	return m_CePropVal.val.filetime;
}

void CCeDBProp::SetString(LPWSTR lpwstr,UINT nMode /*= CCeDBProp::Copy*/)
{
	FreeProp();  // calls InitProp()
	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_LPWSTR);

	int nLen = wcslen(lpwstr)+sizeof(WCHAR);
	// Note: the length is not enforced by this wrapper method, but WinCE does have
	// a maximum set on its database records.

	switch(nMode)
	{
		case Copy:
			m_CePropVal.val.lpwstr = new WCHAR[nLen]; // throwable  
			wcscpy(m_CePropVal.val.lpwstr,lpwstr);
			m_bOwnsStringOrBlob = TRUE;
		break;

		case NoCopy:
			m_CePropVal.val.lpwstr = lpwstr;
			m_bOwnsStringOrBlob = FALSE;
		break;

		default:
			ASSERT(FALSE);
		break;
	}
}

LPWSTR CCeDBProp::GetString() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_LPWSTR);
	return m_CePropVal.val.lpwstr;
}

void CCeDBProp::SetBlob(CEBLOB blob,UINT nMode /*= CCeDBProp::Copy*/)
{
	FreeProp();  // calls InitProp()

	SET_PROPID_CEVT(m_CePropVal.propid,CEVT_BLOB);
	m_CePropVal.val.blob.dwCount = blob.dwCount;
	// Note: the length is not enforced by this wrapper method, but WinCE does have
	// a maximum set on its database records.

	switch(nMode)
	{
		case Copy:
			m_CePropVal.val.blob.lpb = new BYTE[blob.dwCount]; // throwable
			WCE_FCTN(memcpy)(m_CePropVal.val.blob.lpb,blob.lpb,blob.dwCount);
			m_bOwnsStringOrBlob = TRUE;
		break;

		case NoCopy:
			m_CePropVal.val.blob.lpb = blob.lpb;
			m_bOwnsStringOrBlob = FALSE;
		break;
	}
}

CEBLOB CCeDBProp::GetBlob() const
{
	ASSERT(GET_PROPID_CEVT(m_CePropVal.propid) == CEVT_BLOB);
	return m_CePropVal.val.blob;
}

//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Other operations on CCeDBProp
//////////////////////////////////////////////////////////////////////

// Note: don't call InitProp() if there is currently data being pointed to by this class.
// InitProp() is private member function.
/*private:*/ 
void CCeDBProp::InitProp() 
{
	memset(&m_CePropVal,0,sizeof(CEPROPVAL));
	m_bOwnsStringOrBlob = FALSE;
	m_wSortFlags = 0;
}

void CCeDBProp::FreeProp()
{
	if((m_CePropVal.propid == CEVT_LPWSTR) && m_bOwnsStringOrBlob)
		delete[] m_CePropVal.val.lpwstr;
	else if((m_CePropVal.propid == CEVT_BLOB) && m_bOwnsStringOrBlob)
		delete[] m_CePropVal.val.blob.lpb;

	WORD wSaveIdent = GET_PROPID_IDENT(m_CePropVal.propid);
	InitProp();
	SET_PROPID_IDENT(m_CePropVal.propid,wSaveIdent);
}

//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Implementation of CCeRecord
//////////////////////////////////////////////////////////////////////

CCeDBRecord::CCeDBRecord()
{
	m_pPropArray.SetSize(0);
	m_pbBuffer = NULL;
	m_iCEOID = 0;
}

CCeDBRecord::~CCeDBRecord()
{
	FreeRecord(); 
}

// copy constructor
CCeDBRecord::CCeDBRecord(CCeDBRecord& rec)
{
	m_pPropArray.RemoveAll();
	m_pPropArray.Copy(rec.m_pPropArray);
	m_pbBuffer = NULL;  // no ownership of memory (let the original object keep it)
	m_iCEOID = rec.m_iCEOID;
}

// assignment operator
CCeDBRecord& CCeDBRecord::operator=(const CCeDBRecord& rec)
{
	FreeRecord(); // free original object
	m_pPropArray.RemoveAll();
	m_pPropArray.Copy(rec.m_pPropArray);
	m_pbBuffer = NULL; // no ownership of memory (let the original object keep it)
	m_iCEOID = rec.m_iCEOID;
	return *this;
}


int CCeDBRecord::GetNumProps() const
{
	return m_pPropArray.GetSize();
}

BOOL CCeDBRecord::AddProp(CCeDBProp *pProp)
{
	TRY
	{
		m_pPropArray.Add(pProp);
		pProp->m_bOwnsStringOrBlob = FALSE; // only want one ownership instance
	}
	CATCH(CMemoryException,e)
	{
		return FALSE;
	}
	END_CATCH

	return TRUE;
}

BOOL CCeDBRecord::AddProps(CCeDBProp* pPropArray,int nNumProps)
{
	for(int n=0; n < nNumProps; n++)
	{
		TRY
		{
			int nIndex = m_pPropArray.Add(pPropArray+n);
		}
		CATCH(CException,e)
		{
			return FALSE;
		}
		END_CATCH;
			
		pPropArray[n].m_bOwnsStringOrBlob = FALSE; // only want one ownership instance
	}
	return TRUE;
}

void CCeDBRecord::DeleteAllProps()
{
	FreeRecord();
}

void CCeDBRecord::DeleteProp(WORD wPropIdent)
{
	int nLength = m_pPropArray.GetSize();

	for(int n=0; n < nLength; n++)
	{
		if(GET_PROPID_IDENT(((CCeDBProp*)m_pPropArray[n])->m_CePropVal.propid) == wPropIdent)
		{
			if(((CCeDBProp*)m_pPropArray[n])->m_bDeletePtr)
				free(m_pPropArray[n]);
			while(n < nLength-1)
			{
				m_pPropArray[n] = m_pPropArray[n+1];
				n++;
			}
			m_pPropArray.SetSize(--nLength);
			break;
		}
	}

	if(nLength == 0) // delete m_pbBuffer now (if there is one)
	{
		LocalFree(m_pbBuffer);
		m_pbBuffer = NULL;
	}
}

CCeDBProp* CCeDBRecord::GetPropFromIdent(WORD wPropIdent) const // query by wIdent?
{
	for(int n=0; n < m_pPropArray.GetSize(); n++)
	{
		if(GET_PROPID_IDENT(((CCeDBProp*)m_pPropArray[n])->m_CePropVal.propid) == wPropIdent)
			return *(((CCeDBProp **)m_pPropArray.GetData())+n);
	}

	return NULL;
}


CCeDBProp* CCeDBRecord::GetPropFromIndex(int n) const 
{
	ASSERT((n >= 0) && (n < m_pPropArray.GetSize()));

	return *(((CCeDBProp **)m_pPropArray.GetData())+n);
}


void CCeDBRecord::FreeRecord()
{
	int nLength = m_pPropArray.GetSize();
	for(int n=0; n < nLength; n++)
	{
		if(((CCeDBProp*)m_pPropArray[n])->m_bDeletePtr)
			free(m_pPropArray[n]);
	}

	m_pPropArray.SetSize(0); 
	m_iCEOID = 0;

	if(m_pbBuffer != NULL)
	{
		LocalFree(m_pbBuffer);
		m_pbBuffer = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CCeDBDatabase,CObject);
IMPLEMENT_DYNAMIC(CCeDBProp,CObject);

