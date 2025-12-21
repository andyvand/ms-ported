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

#define TRACE_STG 0
#define MAX_POSSIBLE_NAMELEN    512
#define MAGIC_STORAGE    0x1234
#define MAGIC_STREAM    0xabcd


class CStreamImp;

class CStorageImp : public IStorage
{
public:
    CStorageImp(
         const OLECHAR      *pwcsName,
         CStorageImp        *pParent,
         CStorageImp        *pSibling,
         CRITICAL_SECTION   *pCS,
         HRESULT *phr);

    ~CStorageImp();

    STDMETHOD(QueryInterface) (REFIID iid, LPVOID *ppvObj);
    STDMETHOD_(ULONG, AddRef) ();
    STDMETHOD_(ULONG, Release) ();

    HRESULT STDMETHODCALLTYPE CreateStream(
         const OLECHAR *pwcsName,
         DWORD grfMode,
         DWORD reserved1,
         DWORD reserved2,
         IStream **ppstm);
    HRESULT STDMETHODCALLTYPE OpenStream(
         const OLECHAR *pwcsName,
         void *reserved1,
         DWORD grfMode,
         DWORD reserved2,
         IStream **ppstm);
    HRESULT STDMETHODCALLTYPE CreateStorage(
         const OLECHAR *pwcsName,
         DWORD grfMode,
         DWORD dwStgFmt,
         DWORD reserved2,
         IStorage **ppstg);
    HRESULT STDMETHODCALLTYPE OpenStorage(
         const OLECHAR *pwcsName,
         IStorage *pstgPriority,
         DWORD grfMode,
         SNB snbExclude,
         DWORD reserved,
         IStorage **ppstg);
    HRESULT STDMETHODCALLTYPE CopyTo(
         DWORD ciidExclude,
         const IID *rgiidExclude,
         SNB snbExclude,
         IStorage *pstgDest);
    HRESULT STDMETHODCALLTYPE MoveElementTo(
         const OLECHAR *pwcsName,
         IStorage *pstgDest,
         const OLECHAR *pwcsNewName,
         DWORD grfFlags);
    HRESULT STDMETHODCALLTYPE Commit(
         DWORD grfCommitFlags);
    HRESULT STDMETHODCALLTYPE Revert( void);
    HRESULT STDMETHODCALLTYPE EnumElements(
         DWORD reserved1,
         void *reserved2,
         DWORD reserved3,
         IEnumSTATSTG **ppenum);
    HRESULT STDMETHODCALLTYPE DestroyElement(
         const OLECHAR *pwcsName);
    HRESULT STDMETHODCALLTYPE RenameElement(
         const OLECHAR *pwcsOldName,
         const OLECHAR *pwcsNewName);
    HRESULT STDMETHODCALLTYPE SetElementTimes(
         const OLECHAR *pwcsName,
         const FILETIME *pctime,
         const FILETIME *patime,
         const FILETIME *pmtime);
    HRESULT STDMETHODCALLTYPE SetClass(
         REFCLSID clsid);
    HRESULT STDMETHODCALLTYPE SetStateBits(
         DWORD grfStateBits,
         DWORD grfMask);
    HRESULT STDMETHODCALLTYPE Stat(
         STATSTG *pstatstg,
         DWORD grfStatFlag);
    HRESULT STDMETHODCALLTYPE Load(
         HANDLE hFile);
    HRESULT STDMETHODCALLTYPE Save(
         HANDLE hFile);

    friend class CStreamImp;
    friend class CEnumSTATSTGImp;

private:
    OLECHAR*        m_pszName;
    CLSID           m_clsid;
    CStreamImp*     m_pFirstChildStream;
    CStorageImp*    m_pFirstChildStorage;
    CStorageImp*    m_pNextStorage;
    CStorageImp*    m_pParentStorage;
    CRITICAL_SECTION* m_pCS;
    ULONG           m_Refs;
};


class CStreamImp : public IStream
{
public:
    CStreamImp(
        const OLECHAR       *pszName,
        CStreamImp          *pSibling,
        CStorageImp         *pParent,
        CRITICAL_SECTION    *pCS,
        HRESULT             *phr,
        PBYTE               pData = NULL,
        BOOL                fDeleteOnRelease = TRUE);

    ~CStreamImp();

    STDMETHOD(QueryInterface) (REFIID iid, LPVOID *ppvObj);
    STDMETHOD_(ULONG, AddRef) ();
    STDMETHOD_(ULONG, Release) ();
    HRESULT STDMETHODCALLTYPE Read(
        void *pv,
        ULONG cb,
        ULONG *pcbRead);
    HRESULT STDMETHODCALLTYPE Write(
        const void *pv,
        ULONG cb,
        ULONG *pcbWritten);
    HRESULT STDMETHODCALLTYPE Seek(
         LARGE_INTEGER dlibMove,
         DWORD dwOrigin,
         ULARGE_INTEGER *plibNewPosition);
    HRESULT STDMETHODCALLTYPE SetSize(
         ULARGE_INTEGER libNewSize);
    HRESULT STDMETHODCALLTYPE CopyTo(
         IStream *pstm,
         ULARGE_INTEGER cb,
         ULARGE_INTEGER *pcbRead,
         ULARGE_INTEGER *pcbWritten);
    HRESULT STDMETHODCALLTYPE Commit(
         DWORD grfCommitFlags);
    HRESULT STDMETHODCALLTYPE Revert( void);
    HRESULT STDMETHODCALLTYPE LockRegion(
         ULARGE_INTEGER libOffset,
         ULARGE_INTEGER cb,
         DWORD dwLockType);
    HRESULT STDMETHODCALLTYPE UnlockRegion(
         ULARGE_INTEGER libOffset,
         ULARGE_INTEGER cb,
         DWORD dwLockType);
    HRESULT STDMETHODCALLTYPE Stat(
         STATSTG *pstatstg,
         DWORD grfStatFlag);
    HRESULT STDMETHODCALLTYPE Clone(
         IStream **ppstm);
    HRESULT STDMETHODCALLTYPE Load(
         HANDLE hFile);
    HRESULT STDMETHODCALLTYPE Save(
         HANDLE hFile);

    friend class CStorageImp;
    friend class CEnumSTATSTGImp;

private:
    PBYTE       m_pData;
    CStreamImp* m_pNext;
    OLECHAR*    m_pszName;
    DWORD       m_dwSize;
    DWORD       m_dwCurrentPosition;
    ULONG       m_Refs;
    BOOL        m_fDeleteOnRelease;
    CStorageImp*        m_pParent;
    CRITICAL_SECTION*   m_pCS;
};


LPVOID AFXAPI wce_CoTaskMemRealloc( LPVOID pv, ULONG cbOld, ULONG cb )
{
	ASSERT( cb > 0);

	LPVOID lpnew = CoTaskMemAlloc(cb);
	if((pv != NULL) && (lpnew != NULL))
	{
		WCE_FCTN(memcpy)(lpnew, pv, cbOld);
		::CoTaskMemFree(pv);
	}

	return lpnew;
}

// DIALOGEX templates are not supported under WinCE, so we want to rewrite in
// the DIALOG header and move over the data after DLGTEMPLATEEX structure  
// and before the first dialog item.
// Note: we are referring to the DLGTEMPLATEEX that's defined in the MFC headers,
// NOT the definition in the online help (which isn't really a structure, just a 
// format description.)
// This function is only used once in occmgr.cpp.
BYTE* wce_convertDialogTemplate(DLGTEMPLATE* pNewTemplate, 
								DLGTEMPLATEEX* pOldTemplate, 
								ULONG nOldLen) 
{
	ASSERT((sizeof(DLGTEMPLATE)%2) == 0);   // check to see if structure is WORD-aligned
	ASSERT((sizeof(DLGTEMPLATEEX)%2) == 0); // check to see if structure is WORD-aligned

	BYTE* pNew = (BYTE*)pNewTemplate;
	BYTE* pOld = (BYTE*)pOldTemplate;
	int nLen;

	pNewTemplate->style = pOldTemplate->style;// & WS_EX_WCEMASK;
	pNewTemplate->dwExtendedStyle = 0;
	pNewTemplate->cdit = 0;
	pNewTemplate->x    = pOldTemplate->x;
	pNewTemplate->y    = pOldTemplate->x;
	pNewTemplate->cx   = pOldTemplate->cx;
	pNewTemplate->cy   = pOldTemplate->cy;

	nLen = nOldLen - sizeof(DLGTEMPLATEEX); // get everything after DLGTEMPLATEEX
	WCE_FCTN(memcpy)(pNew + sizeof(DLGTEMPLATE), 
		   pOld + sizeof(DLGTEMPLATEEX), 
		   nLen);                          // and shift it over in a memcpy
	pNew += sizeof(DLGTEMPLATE);

	if(!(pNewTemplate->style & DS_SETFONT))
		pNew += nLen; // No DS_SETFONT, so we're done. Point pNew at the end. 
	else
	{
		// There are two additional members (weight and italics) before the font name 
		// that needs removal. Find it by looking for the end of the title. (In this
		// case, we memcpy'd too much, but the excess will be written over here.)
		WORD *pw = (WORD*)pNew; 
		if (*pw == (WORD)-1)        // Skip menu name ordinal or string
			pw += 2; // WORDs
		else
			while (*pw++);

		if (*pw == (WORD)-1)        // Skip class name ordinal or string
			pw += 2; // WORDs
		else
			while (*pw++);

		while (*pw++);              // Skip caption string

		pw++;                       // Skip pointsize
		pw++;                       // Skip weight (will be deleted)
		pw++;                       // Skip italics (will be deleted)

		// We're at the font name, so shift it over.
		do
		{
			*(pw-2) = *pw;
		} 
		while(*pw++);
		pw -= 2; // move to the new end of the font name

		// We're now at the end, so we're done!  Get the new pNew.
		pNew = (BYTE*)pw;
	}

	return pNew;
}

// Shrink the DLGITEMTEMPLATEEX down to a DLGITEMTEMPLATE, done in the 
// similar fashion as we did for the header (rewrite structure,
// and copy over the post-structure information)
// This function is only used once in occmgr.cpp.
BYTE* wce_convertDialogItemTemplate(DLGITEMTEMPLATE* pNewItemTemplate, 
									DLGITEMTEMPLATEEX* pOldItemTemplate, 
									ULONG nOldLen)
{
	const int DLGITEMTEMPLATEEX_ERROR = 2; 
	BYTE* pNew = (BYTE*)pNewItemTemplate;
	BYTE* pOld = (BYTE*)pOldItemTemplate;
	int nLen;

	pNewItemTemplate->style = pOldItemTemplate->style;
	pNewItemTemplate->dwExtendedStyle = 0;
	pNewItemTemplate->x  = pOldItemTemplate->x;
	pNewItemTemplate->y  = pOldItemTemplate->y;
	pNewItemTemplate->cx = pOldItemTemplate->cx;
	pNewItemTemplate->cy = pOldItemTemplate->cy;
	pNewItemTemplate->id = LOWORD(pOldItemTemplate->id);

	nLen = nOldLen - sizeof(DLGITEMTEMPLATEEX);
	WCE_FCTN(memcpy)(pNew + sizeof(DLGITEMTEMPLATE), 
		   pOld + sizeof(DLGITEMTEMPLATEEX), 
		   nLen);
	pNew += sizeof(DLGITEMTEMPLATE) + nLen + DLGITEMTEMPLATEEX_ERROR;

	return pNew;
}


HRESULT AFXAPI wce_CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID pclsid)
{
#if (_WIN32_WCE >= 210)
	return CLSIDFromProgID(lpszProgID,pclsid);
#else // _WIN32_WCE
	WCHAR sz[256];
	ULONG cbValue = sizeof(sz);
	HKEY hSubKey;
	LONG lResult;

	if (lpszProgID == NULL)
		return(E_INVALIDARG);

	if (*lpszProgID == 0)
		return(CO_E_CLASSSTRING);

	_tcscpy(sz, lpszProgID);
	_tcscat(sz, _T("\\Clsid"));

	lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, sz, 0, 0, &hSubKey);
	if (lResult == ERROR_SUCCESS)
	{
		TCHAR strValueName[60];
		DWORD dwValueNameLength = sizeof(strValueName)/sizeof(TCHAR);
		DWORD dwType;
		lResult = RegEnumValue(hSubKey, 0, strValueName, &dwValueNameLength, 0, &dwType, (LPBYTE)sz, &cbValue);
		ASSERT(dwType == REG_SZ);
		ASSERT(lResult == ERROR_SUCCESS);
		RegCloseKey (hSubKey);
		return CLSIDFromString(sz, pclsid);
	}
	return E_FAIL;
#endif // _WIN32_WCE
}


void AFXAPI wce_ReleaseStgMedium( LPSTGMEDIUM pMedium )
{
#if (_WIN32_WCE >= 210)
	ReleaseStgMedium(pMedium);
#else // _WIN32_WCE
	if(pMedium == NULL)
		return;

	BOOL fPunkRel = pMedium->pUnkForRelease != NULL;

	switch (pMedium->tymed) {
		case TYMED_HGLOBAL:
			if (pMedium->hGlobal != NULL && !fPunkRel)
				VERIFY(WCE_FCTN(GlobalFree)(pMedium->hGlobal) == 0);
			break;

/* WinCE: these are not supported by the OS
		case TYMED_GDI:
			if (pMedium->hGlobal != NULL && !fPunkRel)
				DeleteObject(pMedium->hGlobal);
			break;

		case TYMED_ENHMF:
			if (pMedium->hEnhMetaFile != NULL && !fPunkRel)
			{
					VERIFY(DeleteEnhMetaFile(pMedium->hEnhMetaFile));
			};
			break;
		case TYMED_MFPICT:
			if (pMedium->hGlobal != NULL && !fPunkRel) {
				LPMETAFILEPICT  pmfp;

				if ((pmfp = (LPMETAFILEPICT)GlobalLock(pMedium->hGlobal)) == NULL)
					break;

				DeleteMetaFile(pmfp->hMF);
				GlobalUnlock(pMedium->hGlobal);
				VERIFY(WCE_FCTN(GlobalFree)(pMedium->hGlobal) == 0);
			}
			break;
*/
		case TYMED_FILE:
			if (pMedium->lpszFileName != NULL) {
//              if (!IsValidPtrIn(pMedium->lpszFileName, 1))
  //                  break;
				if (!fPunkRel) {
					DeleteFile(pMedium->lpszFileName);
				}

	//  if pUnkForRelease was NULL, we have to delete the filename string.
 
				free(pMedium->lpszFileName);
				pMedium->lpszFileName = NULL;

			}
			break;

		case TYMED_ISTREAM:
			if (pMedium->pstm != NULL)// &&
//                IsValidInterface(pMedium->pstm))
				pMedium->pstm->Release();
			break;

		case TYMED_ISTORAGE:
			if (pMedium->pstg != NULL)// &&
//                IsValidInterface(pMedium->pstg))
				pMedium->pstg->Release();
			break;

		case TYMED_NULL:
			break;

		default:
			ASSERT(FALSE);
//            AssertSz(FALSE, "Invalid medium in ReleaseStgMedium");
	}


	if (pMedium->pUnkForRelease) {
		//if (IsValidInterface(pMedium->pUnkForRelease))
			pMedium->pUnkForRelease->Release();
		pMedium->pUnkForRelease = NULL;
	}

	// NULL out to prevent unwanted use of just freed data.
	// Note: this must be done AFTER punkForRelease is called
	// because our special punkForRelease used in remoting
	// needs the tymed value.

	pMedium->tymed = TYMED_NULL;
#endif // _WIN32_WCE
}


SCODE AFXAPI wce_OleInitialize( LPVOID pvReserved )
{
#if (_WIN32_WCE < 300)
	return(::CoInitializeEx(NULL,COINIT_MULTITHREADED));
#else
	//Try to call CoInitializeEx with COINIT_APARTMENTTHREADED
	//assuming the OS image with DCOM support is running.
	//If failed with E_INVALIDARG, assume the OS image without DCOM support 
	//is running thus call CoInitializeEx with COINIT_MULTITHREADED
	SCODE sc = ::CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
	if (sc == E_INVALIDARG)
		sc = ::CoInitializeEx(NULL,COINIT_MULTITHREADED);
	return sc;
#endif //_WIN32_WCE
}

void AFXAPI wce_OleUninitialize()
{
	::CoUninitialize();
}

HRESULT AFXAPI wce_CoDisconnectObject( IUnknown*, unsigned long )
{
        return S_OK;
}



////////////////////////////////////////////////////////////////////////////
//
HRESULT AFXAPI wce_CreateStreamOnHGlobal(
    HGLOBAL     hGlobal,
    BOOL        fDeleteOnRelease,
    LPSTREAM *  ppstm)
{
    HRESULT             hr;

	DWORD dwSize = WCE_FCTN(GlobalSize)(hGlobal);

    *ppstm = new CStreamImp(
                    NULL, // WinCE: sneak in size as workaround (see ctor)
                    (CStreamImp*)dwSize,
                    NULL,
                    NULL,  // pointer to Critical Section
                    &hr,
                    (PBYTE)hGlobal,
                    fDeleteOnRelease);

    if ((*ppstm == NULL) ||
        (hr != S_OK))
    {
        if (*ppstm == NULL)
        {
            return E_OUTOFMEMORY;
        }
        else
        {
            delete ppstm;
        }
    }

    return hr;
}


//////////////////////////////////////////////////////////////////////////////
//
HRESULT AFXAPI wce_OleLoadFromStream ( LPSTREAM pStm, REFIID iidInterface,
    LPVOID FAR* ppvObj)
{
    //  Assumptions:  The name of the object class is in the stream,
    //  as a length-prefixed string.
    HRESULT         hresult = NOERROR;
    CLSID               cid;
    LPPERSISTSTREAM pPS;
    LPUNKNOWN       pUnk;

    *ppvObj = NULL;

    if ((hresult = ::ReadClassStm(pStm, &cid)) != NOERROR)
        goto errRtn;

    hresult = ::CoCreateInstance(cid, NULL,
        CLSCTX_SERVER,
        iidInterface,
        (LPVOID FAR *) &pUnk);
    if (hresult)
        goto errRtn;
    hresult = pUnk->QueryInterface(IID_IPersistStream,
        (LPVOID FAR*) &pPS);
    if (!hresult)
    {
        hresult = pPS->Load( pStm );
        pPS->Release();
    }
    if (!hresult)
        hresult = pUnk->QueryInterface(iidInterface, ppvObj );
    pUnk->Release();

errRtn:

    return hresult;
}


//////////////////////////////////////////////////////////////////////////////
//
HRESULT AFXAPI wce_OleSaveToStream ( LPPERSISTSTREAM pPStm, LPSTREAM pStm)
{

    HRESULT hresult = 0;
    CLSID   clsid;

    if (!pPStm)
    {
        hresult = ResultFromScode(OLE_E_BLANK);
        goto errRtn;
    }

    if (hresult = pPStm->GetClassID(&clsid))
        goto errRtn;

    if ((hresult = ::WriteClassStm(pStm, clsid)) != NOERROR)
        goto errRtn;

    hresult = pPStm->Save(pStm, TRUE);

errRtn:
    return hresult;
}


HRESULT AFXAPI wce_OleTranslateColor(OLE_COLOR clr, HPALETTE hpal, COLORREF* lpcolorref)
{
	if(lpcolorref != NULL)
	{
		if(clr & 0x80000000L) 
		{
			// GetSysColor for WinCE requires the 2nd MSB to be set in 
			// addition to the low byte index.
			*lpcolorref = GetSysColor((clr & 0x000000FFL) | 0x40000000L);
		}
		else 
		{
			// Note: if the high byte is 0x02, then the normal OleTranslateColor
			// will use the optional palette.  We are ignoring that case and
			// returning the RGB value directly.
			*lpcolorref = (COLORREF)(clr & 0x00FFFFFFL);
		}
	}
	return S_OK;
}


HRESULT AFXAPI wce_OleCreateFontIndirect(void* lpFontDesc, REFIID riid, LPVOID* lplpvObj)
{
	return CCeFont::CreateFont((LPFONTDESC)lpFontDesc, riid, lplpvObj);
}



//////////////////////////////////////////////////////////////////////////////
//
LRESULT CALLBACK wce_IMMWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// All this window proc does is gobble up any messages that get sent via TranslateAccelerator
	// below.  See comment below.
	return 0;
}

////////////////////////////////////////////////////////////////////////////
//
//  CStreamImp::CStreamImp
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//

CStreamImp::CStreamImp(
    const OLECHAR       *pszName,
    CStreamImp          *pSibling,
    CStorageImp         *pParent,
    CRITICAL_SECTION    *pCS,
    HRESULT             *phr,
    PBYTE               pData,
    BOOL                fDeleteOnRelease)
{
    m_dwSize = 0;
    if (pData != NULL)
    {
// WinCE: we pass in the size via *pSibling (otherwise NULL in this workaround context)
		// m_dwSize = CoTaskMemSize(pData);	
		m_dwSize = (DWORD)pSibling;
		pSibling = NULL;
    }
    m_fDeleteOnRelease = fDeleteOnRelease;
    m_dwCurrentPosition = 0;
    m_pData = pData;
    m_Refs = 1;
    m_pParent = pParent;
    m_pNext = pSibling;
//    m_pCS = pCS;
    m_pCS = NULL;

    if (pszName != NULL)
    {
        m_pszName = new OLECHAR[lstrlen(pszName) + 1];
        if (m_pszName == NULL)
        {
            *phr = STG_E_INSUFFICIENTMEMORY;
            return;
        }
        lstrcpy(m_pszName, pszName);
    }
    else
    {
        m_pszName = NULL;
    }

    *phr = S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
CStreamImp::~CStreamImp()
{
    if (m_fDeleteOnRelease)
    {
        CoTaskMemFree(m_pData);
    }
    delete[] m_pszName;
    if (m_pParent == NULL)
    {
//        DeleteCriticalSection(m_pCS);
          if ( m_pCS != NULL ) 
               delete m_pCS;
    }
}

////////////////////////////////////////////////////////////////////////////
//
ULONG CStreamImp::AddRef()
{
    if (m_pParent != NULL)
    {
        return m_pParent->AddRef();
    }

    InterlockedIncrement((LONG*)&m_Refs);
    return m_Refs;
}

////////////////////////////////////////////////////////////////////////////
//
ULONG CStreamImp::Release()
{
    ULONG Refs;

    if (m_pParent != NULL)
    {
        return m_pParent->Release();
    }

    Refs = m_Refs;
    if (InterlockedDecrement((LONG*)&m_Refs) == 0)
    {
        delete this;
        return 0;
    }

    return Refs;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::QueryInterface(REFIID riid, LPVOID *ppv)
{
    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_IStream))
    {
        *ppv = (LPUNKNOWN) this ;
        AddRef();
        return S_OK;
    }

    *ppv = NULL;
    return E_NOINTERFACE ;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Read(
        void *pv,
        ULONG cb,
        ULONG *pcbRead)
{
    ULONG cbRead = 0;

    DEBUGMSG(TRACE_STG, (L"CStreamImp::Read(%X,%d)\n",pv,cb));

    //
    //  Make sure the current position is within the data
    //

//    EnterCriticalSection(m_pCS);

    if (m_dwSize > m_dwCurrentPosition)
    {
        if (cb > (m_dwSize - m_dwCurrentPosition))
        {
            cbRead = m_dwSize - m_dwCurrentPosition;
        }
        else
        {
            cbRead = cb;
        }

        WCE_FCTN(memcpy)(
            pv,
            m_pData + m_dwCurrentPosition,
            cbRead);

        m_dwCurrentPosition += cbRead;
    }

//    LeaveCriticalSection(m_pCS);

    if (pcbRead != NULL)
    {
        *pcbRead = cbRead;
    }

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Write(
        const void *pv,
        ULONG cb,
        ULONG *pcbWritten)
{
    DEBUGMSG(TRACE_STG, (L"CStreamImp::Write\n"));

//    EnterCriticalSection(m_pCS);

    if ((m_dwCurrentPosition + cb) > (unsigned)m_dwSize)
    {
        //
        //  We try and grow ourselves here
        //

        PBYTE pData = (PBYTE)CoTaskMemRealloc(
                    m_pData,
                    (m_dwCurrentPosition + cb));

        if (pData == NULL)
        {
//            LeaveCriticalSection(m_pCS);
            return STG_E_MEDIUMFULL;
        }

        m_dwSize = m_dwCurrentPosition + cb;
        m_pData = pData;
    }

    //
    //  Copy the data
    //

    WCE_FCTN(memcpy)(m_pData + m_dwCurrentPosition, pv, cb);

	if (pcbWritten != NULL)
    {
        *pcbWritten = cb;
    }

    //
    //  Update our position
    //

    m_dwCurrentPosition += cb;

//    LeaveCriticalSection(m_pCS);

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Seek(
     LARGE_INTEGER dlibMove,
     DWORD dwOrigin,
     ULARGE_INTEGER *plibNewPosition)
{
    LONG lMove = (LONG)dlibMove.QuadPart;

    DEBUGMSG(
        TRACE_STG,
        (L"CStreamImp::Seek(%ld,%X,%ld)\n",lMove,
        dwOrigin,m_dwCurrentPosition));

//    EnterCriticalSection(m_pCS);

    switch (dwOrigin)
    {
    case STREAM_SEEK_CUR:


        lMove += (LONG)m_dwCurrentPosition;

        // Fall through

    case STREAM_SEEK_SET:

        // Make sure we don't go past the begining

        if (lMove < 0)
        {
//            LeaveCriticalSection(m_pCS);
            return STG_E_INVALIDPOINTER;
        }

        m_dwCurrentPosition = (DWORD)lMove;
        break;

    case STREAM_SEEK_END:

        // Make sure we don't go past the begining
        if (lMove > (LONG)m_dwSize)
        {
//            LeaveCriticalSection(m_pCS);
            return STG_E_INVALIDPOINTER;
        }

        m_dwCurrentPosition = (DWORD)((LONG)m_dwSize - lMove);
        break;

    default:
//        LeaveCriticalSection(m_pCS);
        return STG_E_INVALIDFUNCTION;

    }

    //
    //  Return the new position if the user asked for it.
    //

    if (plibNewPosition != NULL)
    {
        plibNewPosition->QuadPart = m_dwCurrentPosition;
    }

//    LeaveCriticalSection(m_pCS);
    return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::SetSize(
     ULARGE_INTEGER libNewSize)
{
    PBYTE pData;

    DEBUGMSG(TRACE_STG, (L"CStreamImp::SetSize\n"));

//    EnterCriticalSection(m_pCS);

    pData = (PBYTE)CoTaskMemRealloc(
                m_pData,
                libNewSize.LowPart);

    if (pData == NULL)
    {
//        LeaveCriticalSection(m_pCS);
        return STG_E_MEDIUMFULL;
    }

    m_dwSize = libNewSize.LowPart;
    m_pData = pData;

//    LeaveCriticalSection(m_pCS);

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::CopyTo(
     IStream *pstm,
     ULARGE_INTEGER cb,
     ULARGE_INTEGER *pcbRead,
     ULARGE_INTEGER *pcbWritten)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Commit(
     DWORD grfCommitFlags)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Revert( void)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::LockRegion(
     ULARGE_INTEGER libOffset,
     ULARGE_INTEGER cb,
     DWORD dwLockType)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::UnlockRegion(
     ULARGE_INTEGER libOffset,
     ULARGE_INTEGER cb,
     DWORD dwLockType)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Stat(
     STATSTG *pstatstg,
     DWORD grfStatFlag)
{
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Clone(
     IStream **ppstm)
{
    if (ppstm != NULL)
    {
        *ppstm = (IStream*)m_pData;
    }
    return E_NOTIMPL;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Load(HANDLE hFile)
{
    DWORD    dwMagic,dwSize,dwRW;

    DEBUGMSG(TRACE_STG, (L"CStreamImp::Load(%s)\n", m_pszName));

    //    must be a brand new CStream to call serialize
    if (m_pszName != NULL || m_pData != NULL)
    {
        return STG_E_ACCESSDENIED;
    }

    //    make sure the magic number
    if (!ReadFile(hFile, &dwMagic, sizeof(DWORD), &dwRW, NULL))
    {
        return    STG_E_FILEALREADYEXISTS;
    }

    if (dwMagic != MAGIC_STREAM)
    {
        return    STG_E_FILEALREADYEXISTS;
    }

    //    read the name size and name
    if (!ReadFile(hFile, &dwSize, sizeof(DWORD), &dwRW, NULL))
    {
        return STG_E_FILEALREADYEXISTS;
    }

    if (dwSize<=0 || dwSize>MAX_POSSIBLE_NAMELEN)
    {
        return STG_E_FILEALREADYEXISTS;
    }

    m_pszName = new WCHAR[dwSize/sizeof(WCHAR)];

    if (m_pszName == NULL)
    {
        return STG_E_INSUFFICIENTMEMORY;
    }
    if (!ReadFile(hFile, m_pszName, dwSize, &dwRW, NULL) || dwRW != dwSize)
    {
        return STG_E_FILEALREADYEXISTS;
    }

    //    read the data size and data
    if (!ReadFile(hFile, &m_dwSize, sizeof(DWORD), &dwRW, NULL) || m_dwSize < 0)
    {
        return STG_E_FILEALREADYEXISTS;
    }

    if (m_dwSize != 0)
    {
        m_pData = (PBYTE)CoTaskMemAlloc(m_dwSize);

        if (m_pData == NULL)
        {
            return STG_E_INSUFFICIENTMEMORY;
        }

        if (!ReadFile(hFile, m_pData, m_dwSize, &dwRW, NULL) ||
            (dwRW != m_dwSize))
        {
            DEBUGMSG(TRACE_STG,(L"ReadFile Failed %ld %ld %X\n",dwRW, dwSize,GetLastError()));
            return STG_E_FILEALREADYEXISTS;
        }
    }

    return S_OK;
}

////////////////////////////////////////////////////////////////////////////
//
HRESULT CStreamImp::Save(HANDLE hFile)
{
    DWORD    dwMagic,dwSize,dwRW;

    if (m_pszName == NULL)
    {
        return STG_E_ACCESSDENIED;
    }

    //    first write the storage magic number
    dwMagic = MAGIC_STREAM;
    if (!WriteFile(hFile, &dwMagic, sizeof(DWORD), &dwRW, NULL))
    {
        return STG_E_MEDIUMFULL;
    }

    //    write the size of the name first
    dwSize = (lstrlen(m_pszName)+1)*sizeof(WCHAR);
    if(!WriteFile(hFile, &dwSize, sizeof(DWORD), &dwRW, NULL))
    {
        return STG_E_MEDIUMFULL;
    }

    //    write the name
    if (!WriteFile(hFile, m_pszName, dwSize, &dwRW, NULL))
    {
        return STG_E_MEDIUMFULL;
    }


    //    write the data size and data
    if (!WriteFile(hFile, &m_dwSize, sizeof(DWORD), &dwRW, NULL))
    {
        return STG_E_MEDIUMFULL;
    }

    if (m_dwSize && !m_pData)
    {
        return STG_E_MEDIUMFULL;
    }

    if (m_pData)
    {
        if(!WriteFile(hFile, m_pData, m_dwSize, &dwRW, NULL))
        {
            return STG_E_MEDIUMFULL;
        }
    }

    DEBUGMSG(TRACE_STG, (L"CStreamImp::Save(%s)\n",m_pszName));

    return    S_OK;
}



AFX_MODULE_STATE* AFXAPI _AfxGetOleModuleState();

void AFXAPI wce_UnregisterOleWindowClasses()
{
	AFX_MANAGE_STATE(_AfxGetOleModuleState());
	// unregister Window classes
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	AfxLockGlobals(CRIT_REGCLASSLIST);
	LPTSTR lpsz = pModuleState->m_szUnregisterList;
	while (*lpsz != 0)
	{
		LPTSTR lpszEnd = _tcschr(lpsz, '\n');
		ASSERT(lpszEnd != NULL);
		*lpszEnd = 0;
		UnregisterClass(lpsz, AfxGetInstanceHandle());
		lpsz = lpszEnd + 1;
	}
	pModuleState->m_szUnregisterList[0] = 0;
	AfxUnlockGlobals(CRIT_REGCLASSLIST);
}



//***************************************************************
//***	wce_RegGetMiscStatus - Used to get the MiscStatus information 
//***	from the registery for a CLSID.
//*****************************************************************
HRESULT AFXAPI wce_RegGetMiscStatus(REFCLSID clsid, DWORD *pdwStatus)
{
	OLECHAR *szClsid;
	WCHAR sz[256];
	ULONG cbValue = sizeof(sz);
	
	if (FAILED(StringFromCLSID(clsid, &szClsid)))
	{
		return REGDB_E_CLASSNOTREG;
	}

	wcscpy(sz, L"CLSID");
	wcscat(sz, L"\\");
	wcscat(sz, szClsid);
	wcscat(sz, L"\\MiscStatus\\1");

	HKEY hSubKey;

	LONG lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, sz, 0, 0, &hSubKey);

	if(lResult != ERROR_SUCCESS)
	{
		return REGDB_E_READREGDB;
	}

	TCHAR strValueName[30];
	DWORD dwValueNameLength = sizeof(strValueName);
	DWORD dwType;

	lResult = RegEnumValue(hSubKey, 0, strValueName, &dwValueNameLength, 0, &dwType, (LPBYTE)sz, &cbValue);

	if(dwType != REG_SZ || lResult != ERROR_SUCCESS)
	{
		RegCloseKey (hSubKey);	
		return REGDB_E_READREGDB;
	}
	
	*pdwStatus = _ttoi(sz);

	RegCloseKey (hSubKey);	
	
	return S_OK;
}

