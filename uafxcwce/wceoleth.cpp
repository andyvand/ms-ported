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
#if !defined(_WIN32_WCE_NO_OLE_THUNKS)

inline HINSTANCE AfxLoadOle(FARPROC* proc, LPCSTR lpsz)
	{ return AfxLoadDll(&_afxOleState->m_hInstOLE, WCE_OLE32_DLL, proc, lpsz); }
inline HINSTANCE AfxLoadOleAut(FARPROC* proc, LPCSTR lpsz)
	{ return AfxLoadDll(&_afxOleState->m_hInstOLEAUT, WCE_OLEAUT32_DLL, proc, lpsz); }
#define OLELOAD(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_COM(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_COMI(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_DEF(x)\
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_DF(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_MFCONV(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_WLM1(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLELOAD_M_WLM2(x) \
	AfxLoadOle((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLEAUTLOAD(x) \
	AfxLoadOleAut((FARPROC*)&_afxOLE.pfn##x, #x)
#define OLEAUTLOAD_M1(x) \
	AfxLoadOleAut((FARPROC*)&_afxOLE.pfn##x, #x)

void STDAPICALLTYPE AfxThunkSysFreeString(BSTR bstr)
{
	OLEAUTLOAD(SysFreeString);
	_afxOLE.pfnSysFreeString(bstr);
}

BSTR STDAPICALLTYPE AfxThunkSysAllocStringByteLen(const char FAR* psz, unsigned int len)
{
	OLEAUTLOAD(SysAllocStringByteLen);
	return _afxOLE.pfnSysAllocStringByteLen(psz, len);
}

HRESULT STDAPICALLTYPE AfxThunkVariantCopy(VARIANTARG FAR* pvargDest, VARIANTARG FAR* pvargSrc)
{
	OLEAUTLOAD(VariantCopy);
	return _afxOLE.pfnVariantCopy(pvargDest, pvargSrc);
}

HRESULT STDAPICALLTYPE AfxThunkVariantClear(VARIANTARG FAR* pvarg)
{
	OLEAUTLOAD(VariantClear);
	return _afxOLE.pfnVariantClear(pvarg);
}

HRESULT STDAPICALLTYPE AfxThunkVariantChangeType(VARIANTARG FAR* pvargDest, VARIANTARG FAR* pvarSrc, unsigned short wFlags, VARTYPE vt)
{
	OLEAUTLOAD(VariantChangeType);
	return _afxOLE.pfnVariantChangeType(pvargDest, pvarSrc, wFlags, vt);
}

BSTR STDAPICALLTYPE AfxThunkSysAllocStringLen(const OLECHAR FAR* psz, unsigned int len)
{
	OLEAUTLOAD(SysAllocStringLen);
	return _afxOLE.pfnSysAllocStringLen(psz, len);
}

unsigned int STDAPICALLTYPE AfxThunkSysStringLen(BSTR bstr)
{
	OLEAUTLOAD(SysStringLen);
	return _afxOLE.pfnSysStringLen(bstr);
}

int STDAPICALLTYPE AfxThunkSysReAllocStringLen(BSTR FAR* pbstr, const OLECHAR FAR* psz, unsigned int len)
{
	OLEAUTLOAD(SysReAllocStringLen);
	return _afxOLE.pfnSysReAllocStringLen(pbstr, psz, len);
}

BSTR STDAPICALLTYPE AfxThunkSysAllocString(const OLECHAR FAR* psz)
{
	OLEAUTLOAD(SysAllocString);
	return _afxOLE.pfnSysAllocString(psz);
}

unsigned int STDAPICALLTYPE AfxThunkSysStringByteLen(BSTR bstr)
{
	OLEAUTLOAD(SysStringByteLen);
	return _afxOLE.pfnSysStringByteLen(bstr);
}

HRESULT STDAPICALLTYPE AfxThunkVarCyFromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, CY FAR* pcyOut)
{
	OLEAUTLOAD(VarCyFromStr);
	return _afxOLE.pfnVarCyFromStr(strIn, lcid, dwFlags, pcyOut);
}

HRESULT STDAPICALLTYPE AfxThunkVarBstrFromCy(CY cyIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut)
{
	OLEAUTLOAD(VarBstrFromCy);
	return _afxOLE.pfnVarBstrFromCy(cyIn, lcid, dwFlags, pbstrOut);
}

HRESULT STDAPICALLTYPE AfxThunkVarDateFromStr(OLECHAR FAR* strIn, LCID lcid, unsigned long dwFlags, DATE FAR* pdateOut)
{
	OLEAUTLOAD(VarDateFromStr);
	return _afxOLE.pfnVarDateFromStr(strIn, lcid, dwFlags, pdateOut);
}

HRESULT STDAPICALLTYPE AfxThunkVarBstrFromDate(DATE dateIn, LCID lcid, unsigned long dwFlags, BSTR FAR* pbstrOut)
{
	OLEAUTLOAD(VarBstrFromDate);
	return _afxOLE.pfnVarBstrFromDate(dateIn, lcid, dwFlags, pbstrOut);
}

HRESULT STDAPICALLTYPE AfxThunkLoadTypeLib(const OLECHAR FAR *szFile, ITypeLib FAR* FAR* pptlib)
{
	OLEAUTLOAD_M1(LoadTypeLib);
	return _afxOLE.pfnLoadTypeLib(szFile, pptlib);
}

HRESULT STDAPICALLTYPE AfxThunkLoadRegTypeLib(REFGUID guid, WORD wVerMajor, WORD wVerMinor, LCID lcid, ITypeLib FAR* FAR* pptlib)
{
	OLEAUTLOAD_M1(LoadRegTypeLib);
	return _afxOLE.pfnLoadRegTypeLib(guid, wVerMajor, wVerMinor, lcid, pptlib);
}

HRESULT STDAPICALLTYPE AfxThunkRegisterTypeLib(ITypeLib FAR* ptlib, OLECHAR FAR *szFullPath, OLECHAR FAR *szHelpDir)
{
	OLEAUTLOAD(RegisterTypeLib);
	return _afxOLE.pfnRegisterTypeLib(ptlib, szFullPath, szHelpDir);
}

int STDAPICALLTYPE AfxThunkDosDateTimeToVariantTime(unsigned short wDosDate, unsigned short wDosTime, double FAR* pvtime)
{
	OLEAUTLOAD(DosDateTimeToVariantTime);
	return _afxOLE.pfnDosDateTimeToVariantTime(wDosDate, wDosTime, pvtime);
}

SAFEARRAY FAR* STDAPICALLTYPE AfxThunkSafeArrayCreate(VARTYPE vt, unsigned int cDims, SAFEARRAYBOUND FAR* rgsabound)
{
	OLEAUTLOAD(SafeArrayCreate);
	return _afxOLE.pfnSafeArrayCreate(vt, cDims, rgsabound);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayRedim(SAFEARRAY FAR* psa, SAFEARRAYBOUND FAR* psaboundNew)
{
	OLEAUTLOAD(SafeArrayRedim);
	return _afxOLE.pfnSafeArrayRedim(psa, psaboundNew);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayAccessData(SAFEARRAY FAR* psa, void HUGEP* FAR* ppvData)
{
	OLEAUTLOAD(SafeArrayAccessData);
	return _afxOLE.pfnSafeArrayAccessData(psa, ppvData);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayUnaccessData(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayUnaccessData);
	return _afxOLE.pfnSafeArrayUnaccessData(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayGetUBound(SAFEARRAY FAR* psa, unsigned int nDim, long FAR* plUbound)
{
	OLEAUTLOAD(SafeArrayGetUBound);
	return _afxOLE.pfnSafeArrayGetUBound(psa, nDim, plUbound);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayGetLBound(SAFEARRAY FAR* psa, unsigned int nDim, long FAR* plLbound)
{
	OLEAUTLOAD(SafeArrayGetLBound);
	return _afxOLE.pfnSafeArrayGetLBound(psa, nDim, plLbound);
}

unsigned int STDAPICALLTYPE AfxThunkSafeArrayGetElemsize(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayGetElemsize);
	return _afxOLE.pfnSafeArrayGetElemsize(psa);
}

unsigned int STDAPICALLTYPE AfxThunkSafeArrayGetDim(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayGetDim);
	return _afxOLE.pfnSafeArrayGetDim(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayCopy(SAFEARRAY FAR* psa,
	SAFEARRAY FAR* FAR* ppsaOut)
{
	OLEAUTLOAD(SafeArrayCopy);
	return _afxOLE.pfnSafeArrayCopy(psa, ppsaOut);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayAllocData(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayAllocData);
	return _afxOLE.pfnSafeArrayAllocData(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayAllocDescriptor(unsigned int cDims,
	SAFEARRAY FAR* FAR* ppsaOut)
{
	OLEAUTLOAD(SafeArrayAllocDescriptor);
	return _afxOLE.pfnSafeArrayAllocDescriptor(cDims, ppsaOut);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayGetElement(SAFEARRAY FAR* psa,
	long FAR* rgIndices, void FAR* pvData)
{
	OLEAUTLOAD(SafeArrayGetElement);
	return _afxOLE.pfnSafeArrayGetElement(psa, rgIndices, pvData);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayPutElement(SAFEARRAY FAR* psa,
	long FAR* rgIndices, void FAR* pvData)
{
	OLEAUTLOAD(SafeArrayPutElement);
	return _afxOLE.pfnSafeArrayPutElement(psa, rgIndices, pvData);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayLock(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayLock);
	return _afxOLE.pfnSafeArrayLock(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayUnlock(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayUnlock);
	return _afxOLE.pfnSafeArrayUnlock(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayDestroy(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayDestroy);
	return _afxOLE.pfnSafeArrayDestroy(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayDestroyData(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayDestroyData);
	return _afxOLE.pfnSafeArrayDestroyData(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayDestroyDescriptor(SAFEARRAY FAR* psa)
{
	OLEAUTLOAD(SafeArrayDestroyDescriptor);
	return _afxOLE.pfnSafeArrayDestroyDescriptor(psa);
}

HRESULT STDAPICALLTYPE AfxThunkSafeArrayPtrOfIndex(SAFEARRAY FAR* psa,
	long FAR* rgIndices, void HUGEP* FAR* ppvData)
{
	OLEAUTLOAD(SafeArrayPtrOfIndex);
	return _afxOLE.pfnSafeArrayPtrOfIndex(psa, rgIndices, ppvData);
}


///////////////////////////////////////////////////////////////////////////////
// OLE32

HRESULT STDAPICALLTYPE AfxThunkReadFmtUserTypeStg(LPSTORAGE pstg, CLIPFORMAT FAR* pcf, LPOLESTR FAR* lplpszUserType)
{
	OLELOAD_M_WLM1(ReadFmtUserTypeStg);
	return _afxOLE.pfnReadFmtUserTypeStg(pstg, pcf, lplpszUserType);
}

HRESULT STDAPICALLTYPE AfxThunkReadClassStg(LPSTORAGE pStg, CLSID FAR* pclsid)
{
	OLELOAD_M_DEF(ReadClassStg);
	return _afxOLE.pfnReadClassStg(pStg, pclsid);
}

HRESULT STDAPICALLTYPE AfxThunkCreateFileMoniker(LPCOLESTR lpszPathName, LPMONIKER FAR* ppmk)
{
	OLELOAD_M_DEF(CreateFileMoniker);
	return _afxOLE.pfnCreateFileMoniker(lpszPathName, ppmk);
}

HRESULT STDAPICALLTYPE AfxThunkStgIsStorageFile(const OLECHAR * pwcsName)
{
	OLELOAD_M_WLM1(StgIsStorageFile);
	return _afxOLE.pfnStgIsStorageFile(pwcsName);
}

HRESULT STDAPICALLTYPE AfxThunkStgOpenStorage(const OLECHAR * pwcsName, IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, IStorage ** ppstgOpen)
{
	OLELOAD_M_WLM1(StgOpenStorage);
	return _afxOLE.pfnStgOpenStorage(pwcsName, pstgPriority, grfMode, snbExclude, reserved, ppstgOpen);
}

HRESULT STDAPICALLTYPE AfxThunkDoDragDrop(LPDATAOBJECT pDataObj, LPDROPSOURCE pDropSource, DWORD dwOKEffects, LPDWORD pdwEffect)
{
	OLELOAD_M_DEF(DoDragDrop);
	return _afxOLE.pfnDoDragDrop(pDataObj, pDropSource, dwOKEffects, pdwEffect);
}

HRESULT STDAPICALLTYPE AfxThunkCoLockObjectExternal(LPUNKNOWN pUnk, BOOL fLock, BOOL fLastUnlockReleases)
{
	OLELOAD_M_COM(CoLockObjectExternal);
	return _afxOLE.pfnCoLockObjectExternal(pUnk, fLock, fLastUnlockReleases);
}

HRESULT STDAPICALLTYPE AfxThunkRegisterDragDrop(HWND hwnd, LPDROPTARGET pDropTarget)
{
	OLELOAD_M_WLM1(RegisterDragDrop);
	return _afxOLE.pfnRegisterDragDrop(hwnd, pDropTarget);
}

HRESULT STDAPICALLTYPE AfxThunkOleRegGetUserType (REFCLSID clsid, DWORD dwFormOfType, LPOLESTR * pszUserType)
{
	OLELOAD_M_DEF(OleRegGetUserType);
	return _afxOLE.pfnOleRegGetUserType(clsid, dwFormOfType, pszUserType);
}

HRESULT STDAPICALLTYPE AfxThunkStgCreateDocfile(const OLECHAR * pwcsName, DWORD grfMode, DWORD reserved, IStorage** ppstgOpen)
{
	OLELOAD_M_WLM1(StgCreateDocfile);
	return _afxOLE.pfnStgCreateDocfile(pwcsName, grfMode, reserved, ppstgOpen);
}

HRESULT STDAPICALLTYPE AfxThunkRevokeDragDrop(HWND hwnd)
{
	OLELOAD_M_WLM1(RevokeDragDrop);
	return _afxOLE.pfnRevokeDragDrop(hwnd);
}

HRESULT STDAPICALLTYPE AfxThunkCoRegisterClassObject(REFCLSID rclsid, LPUNKNOWN pUnk, DWORD dwClsContext, DWORD flags, LPDWORD lpdwRegister)
{
	OLELOAD_M_COM(CoRegisterClassObject);
	return _afxOLE.pfnCoRegisterClassObject(rclsid, pUnk, dwClsContext, flags, lpdwRegister);
}

HRESULT STDAPICALLTYPE AfxThunkCoRevokeClassObject(DWORD dwRegister)
{
	OLELOAD_M_COM(CoRevokeClassObject);
	return _afxOLE.pfnCoRevokeClassObject(dwRegister);
}

HRESULT STDAPICALLTYPE AfxThunkOleTranslateAccelerator(LPOLEINPLACEFRAME lpFrame, LPOLEINPLACEFRAMEINFO lpFrameInfo, LPMSG lpmsg)
{
	OLELOAD_M_WLM1(OleTranslateAccelerator);
	return _afxOLE.pfnOleTranslateAccelerator(lpFrame, lpFrameInfo, lpmsg);
}

BOOL STDAPICALLTYPE AfxThunkIsAccelerator(HACCEL hAccel, INT cAccelEntries, LPMSG lpMsg, WORD* lpwCmd)
{
	OLELOAD_M_WLM1(IsAccelerator);
	return _afxOLE.pfnIsAccelerator(hAccel, cAccelEntries, lpMsg, lpwCmd);
}

HOLEMENU STDAPICALLTYPE AfxThunkOleCreateMenuDescriptor(HMENU hmenuCombined, LPOLEMENUGROUPWIDTHS lpMenuWidths)
{
	OLELOAD_M_WLM2(OleCreateMenuDescriptor);
	return _afxOLE.pfnOleCreateMenuDescriptor(hmenuCombined, lpMenuWidths);
}

HRESULT STDAPICALLTYPE AfxThunkOleDestroyMenuDescriptor(HOLEMENU holemenu)
{
	OLELOAD_M_WLM2(OleDestroyMenuDescriptor);
	return _afxOLE.pfnOleDestroyMenuDescriptor(holemenu);
}

HRESULT STDAPICALLTYPE AfxThunkGetRunningObjectTable(DWORD reserved, LPRUNNINGOBJECTTABLE FAR* pprot)
{
	OLELOAD_M_DEF(GetRunningObjectTable);
	return _afxOLE.pfnGetRunningObjectTable(reserved, pprot);
}

HRESULT STDAPICALLTYPE AfxThunkWriteClassStg(LPSTORAGE pStg, REFCLSID rclsid)
{
	OLELOAD_M_DEF(WriteClassStg);
	return _afxOLE.pfnWriteClassStg(pStg, rclsid);
}

HRESULT STDAPICALLTYPE AfxThunkOleQueryLinkFromData(LPDATAOBJECT pSrcDataObject)
{
	OLELOAD_M_DEF(OleQueryLinkFromData);
	return _afxOLE.pfnOleQueryLinkFromData(pSrcDataObject);
}

HRESULT STDAPICALLTYPE AfxThunkCoRegisterMessageFilter(LPMESSAGEFILTER lpMessageFilter, LPMESSAGEFILTER * lplpMessageFilter)
{
	OLELOAD_M_COM(CoRegisterMessageFilter);
	return _afxOLE.pfnCoRegisterMessageFilter(lpMessageFilter, lplpMessageFilter);
}

HRESULT STDAPICALLTYPE AfxThunkCoCreateInstance(REFCLSID rclsid, LPUNKNOWN * pUnkOuter, DWORD dwClsContext, REFIID riid, LPVOID* ppv)
{
	OLELOAD_M_COM(CoCreateInstance);
	return _afxOLE.pfnCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
}

HRESULT STDAPICALLTYPE AfxThunkCreateBindCtx(DWORD reserved, LPBC FAR* ppbc)
{
	OLELOAD_M_DEF(CreateBindCtx);
	return _afxOLE.pfnCreateBindCtx(reserved, ppbc);
}

HRESULT STDAPICALLTYPE AfxThunkStringFromCLSID(REFCLSID rclsid, LPOLESTR FAR* lplpsz)
{
	OLELOAD_M_COMI(StringFromCLSID);
	return _afxOLE.pfnStringFromCLSID(rclsid, lplpsz);
}

HRESULT STDAPICALLTYPE AfxThunkCoDisconnectObject(LPUNKNOWN pUnk, DWORD dwReserved)
{
	OLELOAD_M_COM(CoDisconnectObject);
	return _afxOLE.pfnCoDisconnectObject(pUnk, dwReserved);
}

HRESULT STDAPICALLTYPE AfxThunkOleRegEnumVerbs (REFCLSID clsid, LPENUMOLEVERB FAR* ppenum)
{
	OLELOAD_M_DEF(OleRegEnumVerbs);
	return _afxOLE.pfnOleRegEnumVerbs(clsid, ppenum);
}

void STDAPICALLTYPE AfxThunkOleUninitialize(void)
{
	OLELOAD_M_WLM1(OleUninitialize);
	_afxOLE.pfnOleUninitialize();
}

HRESULT STDAPICALLTYPE AfxThunkCreateOleAdviseHolder(LPOLEADVISEHOLDER FAR* ppOAHolder)
{
	OLELOAD_M_COMI(CreateOleAdviseHolder);
	return _afxOLE.pfnCreateOleAdviseHolder(ppOAHolder);
}

HRESULT STDAPICALLTYPE AfxThunkCreateDataAdviseHolder(LPDATAADVISEHOLDER FAR* ppDAHolder)
{
	OLELOAD_M_WLM1(CreateDataAdviseHolder);
	return _afxOLE.pfnCreateDataAdviseHolder(ppDAHolder);
}

HRESULT STDAPICALLTYPE AfxThunkOleGetAutoConvert(REFCLSID clsidOld, LPCLSID pClsidNew)
{
	OLELOAD_M_DEF(OleGetAutoConvert);
	return _afxOLE.pfnOleGetAutoConvert(clsidOld, pClsidNew);
}

HRESULT STDAPICALLTYPE AfxThunkCoGetClassObject(REFCLSID rclsid, DWORD dwClsContext, LPVOID pvReserved, REFIID riid, LPVOID* ppv)
{
	OLELOAD_M_COMI(CoGetClassObject);
	return _afxOLE.pfnCoGetClassObject(rclsid, dwClsContext, pvReserved, riid, ppv);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateDefaultHandler(REFCLSID clsid, LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* lplpvObj)
{
	OLELOAD_M_DEF(OleCreateDefaultHandler);
	return _afxOLE.pfnOleCreateDefaultHandler(clsid, pUnkOuter, riid, lplpvObj);
}

HRESULT STDAPICALLTYPE AfxThunkReadClassStm(LPSTREAM pStm, CLSID FAR* pclsid)
{
	OLELOAD_M_DEF(ReadClassStm);
	return _afxOLE.pfnReadClassStm(pStm, pclsid);
}

HRESULT STDAPICALLTYPE AfxThunkOleLoadFromStream(LPSTREAM pStm, REFIID iidInterface, LPVOID FAR* ppvObj)
{
	OLELOAD_M_DEF(OleLoadFromStream);
	return _afxOLE.pfnOleLoadFromStream(pStm, iidInterface, ppvObj);
}

int STDAPICALLTYPE AfxThunkStringFromGUID2(REFGUID rguid, LPOLESTR lpsz, int cbMax)
{
	OLELOAD_M_COMI(StringFromGUID2);
	return _afxOLE.pfnStringFromGUID2(rguid, lpsz, cbMax);
}

void STDAPICALLTYPE AfxThunkCoUninitialize(void)
{
	OLELOAD_M_WLM1(CoUninitialize);
	_afxOLE.pfnCoUninitialize();
}

HRESULT STDAPICALLTYPE AfxThunkCoInitialize(LPVOID pvReserved)
{
	OLELOAD_M_WLM1(CoInitialize);
	return _afxOLE.pfnCoInitialize(pvReserved);
}

HRESULT STDAPICALLTYPE AfxThunkCoInitializeEx(LPVOID pvReserved, DWORD dwCoInit)
{
	OLELOAD_M_WLM1(CoInitializeEx);
	return _afxOLE.pfnCoInitializeEx(pvReserved, dwCoInit);
}

void STDAPICALLTYPE AfxThunkCoFreeUnusedLibraries(void)
{
	OLELOAD_M_COMI(CoFreeUnusedLibraries);
	_afxOLE.pfnCoFreeUnusedLibraries();
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateFromData(LPDATAOBJECT pSrcDataObj, REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreateFromData);
	return _afxOLE.pfnOleCreateFromData(pSrcDataObj, riid, renderopt, pFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleSetContainedObject(LPUNKNOWN pUnknown, BOOL fContained)
{
	OLELOAD_M_DEF(OleSetContainedObject);
	return _afxOLE.pfnOleSetContainedObject(pUnknown, fContained);
}

HRESULT STDAPICALLTYPE AfxThunkOleLockRunning(LPUNKNOWN pUnknown, BOOL fLock, BOOL fLastUnlockCloses)
{
	OLELOAD_M_DEF(OleLockRunning);
	return _afxOLE.pfnOleLockRunning(pUnknown, fLock, fLastUnlockCloses);
}

LPVOID STDAPICALLTYPE AfxThunkCoTaskMemAlloc(ULONG cb)
{
	OLELOAD_M_COMI(CoTaskMemAlloc);
	return _afxOLE.pfnCoTaskMemAlloc(cb);
}

LPVOID STDAPICALLTYPE AfxThunkCoTaskMemRealloc(LPVOID pv, ULONG cb)
{
	return NULL;
}

HRESULT STDAPICALLTYPE AfxThunkCLSIDFromString(LPOLESTR lpsz, LPCLSID pclsid)
{
	OLELOAD_M_COMI(CLSIDFromString);
	return _afxOLE.pfnCLSIDFromString(lpsz, pclsid);
}

HRESULT STDAPICALLTYPE AfxThunkCLSIDFromProgID (LPCOLESTR lpszProgID, LPCLSID lpclsid)
{
	OLELOAD_M_COM(CLSIDFromProgID);
	return _afxOLE.pfnCLSIDFromProgID(lpszProgID, lpclsid);
}

HRESULT STDAPICALLTYPE AfxThunkOleIsCurrentClipboard(LPDATAOBJECT pDataObj)
{
	OLELOAD_M_DEF(OleIsCurrentClipboard);
	return _afxOLE.pfnOleIsCurrentClipboard(pDataObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleFlushClipboard(void)
{
	OLELOAD_M_DEF(OleFlushClipboard);
	return _afxOLE.pfnOleFlushClipboard();
}

HRESULT STDAPICALLTYPE AfxThunkOleSetClipboard(LPDATAOBJECT pDataObj)
{
	OLELOAD_M_DEF(OleSetClipboard);
	return _afxOLE.pfnOleSetClipboard(pDataObj);
}

BOOL STDAPICALLTYPE AfxThunkOleIsRunning(LPOLEOBJECT pObject)
{
	OLELOAD_M_DEF(OleIsRunning);
	return _afxOLE.pfnOleIsRunning(pObject);
}

HRESULT STDAPICALLTYPE AfxThunkOleRun(LPUNKNOWN pUnknown)
{
	OLELOAD_M_DEF(OleRun);
	return _afxOLE.pfnOleRun(pUnknown);
}

HRESULT STDAPICALLTYPE AfxThunkOleGetClipboard(LPDATAOBJECT FAR* ppDataObj)
{
	OLELOAD_M_DEF(OleGetClipboard);
	return _afxOLE.pfnOleGetClipboard(ppDataObj);
}

HRESULT STDAPICALLTYPE AfxThunkCoTreatAsClass(REFCLSID clsidOld, REFCLSID clsidNew)
{
	OLELOAD_M_COMI(CoTreatAsClass);
	return _afxOLE.pfnCoTreatAsClass(clsidOld, clsidNew);
}

HRESULT STDAPICALLTYPE AfxThunkOleQueryCreateFromData(LPDATAOBJECT pSrcDataObject)
{
	OLELOAD_M_DEF(OleQueryCreateFromData);
	return _afxOLE.pfnOleQueryCreateFromData(pSrcDataObject);
}

HRESULT STDAPICALLTYPE AfxThunkOleSetMenuDescriptor (HOLEMENU holemenu, HWND hwndFrame, HWND hwndActiveObject, LPOLEINPLACEFRAME lpFrame, LPOLEINPLACEACTIVEOBJECT lpActiveObj)
{
	OLELOAD_M_WLM2(OleSetMenuDescriptor);
	return _afxOLE.pfnOleSetMenuDescriptor(holemenu, hwndFrame, hwndActiveObject, lpFrame, lpActiveObj);
}

HRESULT STDAPICALLTYPE AfxThunkCreateItemMoniker(LPCOLESTR lpszDelim, LPCOLESTR lpszItem, LPMONIKER* ppmk)
{
	OLELOAD_M_DEF(CreateItemMoniker);
	return _afxOLE.pfnCreateItemMoniker(lpszDelim, lpszItem, ppmk);
}

HRESULT STDAPICALLTYPE AfxThunkCreateGenericComposite(LPMONIKER pmkFirst, LPMONIKER pmkRest, LPMONIKER* ppmkComposite)
{
	OLELOAD_M_DEF(CreateGenericComposite);
	return _afxOLE.pfnCreateGenericComposite(pmkFirst, pmkRest, ppmkComposite);
}

HRESULT STDAPICALLTYPE AfxThunkCreateStreamOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPSTREAM* ppstm)
{
	OLELOAD_M_WLM1(CreateStreamOnHGlobal);
	return _afxOLE.pfnCreateStreamOnHGlobal(hGlobal, fDeleteOnRelease, ppstm);
}

HRESULT STDAPICALLTYPE AfxThunkOleSaveToStream(LPPERSISTSTREAM pPStm, LPSTREAM pStm)
{
	OLELOAD_M_DEF(OleSaveToStream);
	return _afxOLE.pfnOleSaveToStream(pPStm, pStm);
}

HRESULT STDAPICALLTYPE AfxThunkWriteClassStm(LPSTREAM pStm, REFCLSID rclsid)
{
	OLELOAD_M_DEF(WriteClassStm);
	return _afxOLE.pfnWriteClassStm(pStm, rclsid);
}

void STDAPICALLTYPE AfxThunkCoTaskMemFree(LPVOID pv)
{
	OLELOAD_M_COMI(CoTaskMemFree);
	_afxOLE.pfnCoTaskMemFree(pv);
}

HGLOBAL STDAPICALLTYPE AfxThunkOleGetIconOfClass(REFCLSID rclsid, LPOLESTR lpszLabel, BOOL fUseTypeAsLabel)
{
	OLELOAD_M_WLM1(OleGetIconOfClass);
	return _afxOLE.pfnOleGetIconOfClass(rclsid, lpszLabel, fUseTypeAsLabel);
}

void STDAPICALLTYPE AfxThunkReleaseStgMedium(LPSTGMEDIUM lp)
{
	OLELOAD_M_WLM1(ReleaseStgMedium);
	_afxOLE.pfnReleaseStgMedium(lp);
}

HRESULT STDAPICALLTYPE AfxThunkGetHGlobalFromILockBytes (LPLOCKBYTES plkbyt, HGLOBAL FAR* phglobal)
{
	OLELOAD_M_WLM1(GetHGlobalFromILockBytes);
	return _afxOLE.pfnGetHGlobalFromILockBytes(plkbyt, phglobal);
}

HRESULT STDAPICALLTYPE AfxThunkStgOpenStorageOnILockBytes(LPLOCKBYTES plkbyt, LPSTORAGE pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved, LPSTORAGE* ppstgOpen)
{
	OLELOAD_M_DF(StgOpenStorageOnILockBytes);
	return _afxOLE.pfnStgOpenStorageOnILockBytes(plkbyt, pstgPriority, grfMode, snbExclude, reserved, ppstgOpen);
}

HRESULT STDAPICALLTYPE AfxThunkCreateILockBytesOnHGlobal(HGLOBAL hGlobal, BOOL fDeleteOnRelease, LPLOCKBYTES* pplkbyt)
{
	OLELOAD_M_WLM1(CreateILockBytesOnHGlobal);
	return _afxOLE.pfnCreateILockBytesOnHGlobal(hGlobal, fDeleteOnRelease, pplkbyt);
}

HRESULT STDAPICALLTYPE AfxThunkStgCreateDocfileOnILockBytes(LPLOCKBYTES plkbyt, DWORD grfMode, DWORD reserved, LPSTORAGE* ppstgOpen)
{
	OLELOAD_M_DF(StgCreateDocfileOnILockBytes);
	return _afxOLE.pfnStgCreateDocfileOnILockBytes(plkbyt, grfMode, reserved, ppstgOpen);
}

HRESULT STDAPICALLTYPE AfxThunkOleSave(LPPERSISTSTORAGE pPS, LPSTORAGE pStg, BOOL fSameAsLoad)
{
	OLELOAD_M_DEF(OleSave);
	return _afxOLE.pfnOleSave(pPS, pStg, fSameAsLoad);
}

HRESULT STDAPICALLTYPE AfxThunkOleLoad(LPSTORAGE pStg, REFIID riid, LPOLECLIENTSITE pClientSite, LPVOID* ppvObj)
{
	OLELOAD_M_DEF(OleLoad);
	return _afxOLE.pfnOleLoad(pStg, riid, pClientSite, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreate(REFCLSID rclsid, REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreate);
	return _afxOLE.pfnOleCreate(rclsid, riid, renderopt, pFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateLinkToFile(LPCOLESTR lpszFileName, REFIID riid, DWORD renderopt, LPFORMATETC lpFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreateLinkToFile);
	return _afxOLE.pfnOleCreateLinkToFile(lpszFileName, riid, renderopt, lpFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateFromFile(REFCLSID rclsid, LPCOLESTR lpszFileName, REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreateFromFile);
	return _afxOLE.pfnOleCreateFromFile(rclsid, lpszFileName, riid, renderopt, pFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateStaticFromData(LPDATAOBJECT pSrcDataObj, REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreateStaticFromData);
	return _afxOLE.pfnOleCreateStaticFromData(pSrcDataObj, riid, renderopt, pFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkOleCreateLinkFromData(LPDATAOBJECT pSrcDataObj, REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj)
{
	OLELOAD_M_WLM1(OleCreateLinkFromData);
	return _afxOLE.pfnOleCreateLinkFromData(pSrcDataObj, riid, renderopt, pFormatEtc, pClientSite, pStg, ppvObj);
}

HRESULT STDAPICALLTYPE AfxThunkSetConvertStg(LPSTORAGE pStg, BOOL fConvert)
{
	OLELOAD_M_DEF(SetConvertStg);
	return _afxOLE.pfnSetConvertStg(pStg, fConvert);
}

HANDLE STDAPICALLTYPE AfxThunkOleDuplicateData(HANDLE hSrc, CLIPFORMAT cfFormat, UINT uiFlags)
{
	OLELOAD_M_WLM1(OleDuplicateData);
	return _afxOLE.pfnOleDuplicateData(hSrc, cfFormat, uiFlags);
}

HRESULT STDAPICALLTYPE AfxThunkWriteFmtUserTypeStg (LPSTORAGE pstg, CLIPFORMAT cf, LPOLESTR lpszUserType)
{
	OLELOAD_M_WLM1(WriteFmtUserTypeStg);
	return _afxOLE.pfnWriteFmtUserTypeStg(pstg, cf, lpszUserType);
}

HRESULT STDAPICALLTYPE AfxThunkOleRegGetMiscStatus(REFCLSID clsid, DWORD dwAspect, DWORD* pdwStatus)
{
	OLELOAD_M_DEF(OleRegGetMiscStatus);
	return _afxOLE.pfnOleRegGetMiscStatus(clsid, dwAspect, pdwStatus);
}

HRESULT STDAPICALLTYPE AfxThunkCoGetMalloc(DWORD dwMemContext, LPMALLOC * ppMalloc)
{
	OLELOAD_M_COMI(CoGetMalloc);
	return _afxOLE.pfnCoGetMalloc(dwMemContext, ppMalloc);
}

HRESULT STDAPICALLTYPE AfxThunkStgIsStorageILockBytes(LPLOCKBYTES plkbyt)
{
	OLELOAD_M_DF(StgIsStorageILockBytes);
	return _afxOLE.pfnStgIsStorageILockBytes(plkbyt);
}

AFX_DATADEF AFX_OLE_CALL _afxOLE =
{
	// OLE32.DLL entry points
	AfxThunkReadFmtUserTypeStg,
	AfxThunkReadClassStg,
	AfxThunkCreateFileMoniker,
	AfxThunkStgIsStorageFile,
	AfxThunkStgOpenStorage,
	AfxThunkDoDragDrop,
	AfxThunkCoLockObjectExternal,
	AfxThunkRegisterDragDrop,
	AfxThunkOleRegGetUserType,
	AfxThunkStgCreateDocfile,
	AfxThunkRevokeDragDrop,
	AfxThunkCoRegisterClassObject,
	AfxThunkCoRevokeClassObject,
	AfxThunkOleTranslateAccelerator,
	AfxThunkIsAccelerator,
	AfxThunkOleCreateMenuDescriptor,
	AfxThunkOleDestroyMenuDescriptor,
	AfxThunkGetRunningObjectTable,
	AfxThunkWriteClassStg,
	AfxThunkOleQueryLinkFromData,
	AfxThunkCoRegisterMessageFilter,
	AfxThunkCoCreateInstance,
	AfxThunkCreateBindCtx,
	AfxThunkStringFromCLSID,
	AfxThunkCoDisconnectObject,
	AfxThunkOleRegEnumVerbs,
	AfxThunkOleUninitialize,
	AfxThunkCreateOleAdviseHolder,
	AfxThunkCreateDataAdviseHolder,
	AfxThunkOleGetAutoConvert,
	AfxThunkCoGetClassObject,
	AfxThunkOleCreateDefaultHandler,
WCE_DEL	AfxThunkCreateDataCache,
	AfxThunkReadClassStm,
	AfxThunkOleLoadFromStream,
	AfxThunkStringFromGUID2,
	AfxThunkCoUninitialize,
	AfxThunkCoInitialize,
	WCE_IF(AfxThunkCoInitializeEx,AfxThunkOleInitialize),
	AfxThunkCoFreeUnusedLibraries,
	AfxThunkOleCreateFromData,
	AfxThunkOleSetContainedObject,
	AfxThunkOleLockRunning,
	AfxThunkCoTaskMemAlloc,
WCE_INS AfxThunkCoTaskMemRealloc,
	AfxThunkCLSIDFromString,
	AfxThunkCLSIDFromProgID,
	AfxThunkOleIsCurrentClipboard,
	AfxThunkOleFlushClipboard,
	AfxThunkOleSetClipboard,
	AfxThunkOleIsRunning,
	AfxThunkOleRun,
	AfxThunkOleGetClipboard,
	AfxThunkCoTreatAsClass,
	AfxThunkOleQueryCreateFromData,
	AfxThunkOleSetMenuDescriptor,
	AfxThunkCreateItemMoniker,
	AfxThunkCreateGenericComposite,
	AfxThunkCreateStreamOnHGlobal,
	AfxThunkOleSaveToStream,
	AfxThunkWriteClassStm,
	AfxThunkCoTaskMemFree,
	AfxThunkOleGetIconOfClass,
	AfxThunkReleaseStgMedium,
	AfxThunkGetHGlobalFromILockBytes,
	AfxThunkStgOpenStorageOnILockBytes,
	AfxThunkCreateILockBytesOnHGlobal,
	AfxThunkStgCreateDocfileOnILockBytes,
	AfxThunkOleSave,
	AfxThunkOleLoad,
	AfxThunkOleCreate,
	AfxThunkOleCreateLinkToFile,
	AfxThunkOleCreateFromFile,
	AfxThunkOleCreateStaticFromData,
	AfxThunkOleCreateLinkFromData,
	AfxThunkSetConvertStg,
	AfxThunkOleDuplicateData,
	AfxThunkWriteFmtUserTypeStg,
	AfxThunkOleRegGetMiscStatus,
	AfxThunkCoGetMalloc,
	AfxThunkStgIsStorageILockBytes,

	// OLEAUT32.DLL entry points
	AfxThunkSysFreeString,
	AfxThunkSysAllocStringByteLen,
	AfxThunkVariantCopy,
	AfxThunkVariantClear,
	AfxThunkVariantChangeType,
	AfxThunkSysAllocStringLen,
	AfxThunkSysStringLen,
	AfxThunkSysReAllocStringLen,
	AfxThunkSysAllocString,
	AfxThunkSysStringByteLen,
	AfxThunkVarCyFromStr,
	AfxThunkVarBstrFromCy,
	AfxThunkVarDateFromStr,
	AfxThunkVarBstrFromDate,
	AfxThunkLoadTypeLib,
	AfxThunkLoadRegTypeLib,
	AfxThunkRegisterTypeLib,
	AfxThunkDosDateTimeToVariantTime,
	AfxThunkSafeArrayCreate,
	AfxThunkSafeArrayRedim,
	AfxThunkSafeArrayAccessData,
	AfxThunkSafeArrayUnaccessData,
	AfxThunkSafeArrayGetUBound,
	AfxThunkSafeArrayGetLBound,
	AfxThunkSafeArrayGetElemsize,
	AfxThunkSafeArrayGetDim,
	AfxThunkSafeArrayCopy,
	AfxThunkSafeArrayAllocData,
	AfxThunkSafeArrayAllocDescriptor,
	AfxThunkSafeArrayGetElement,
	AfxThunkSafeArrayPutElement,
	AfxThunkSafeArrayLock,
	AfxThunkSafeArrayUnlock,
	AfxThunkSafeArrayDestroy,
	AfxThunkSafeArrayDestroyData,
	AfxThunkSafeArrayDestroyDescriptor,
	AfxThunkSafeArrayPtrOfIndex,
};

#endif // _WIN32_WCE_NO_OLE_THUNKS
#endif // _WIN32_WCE_NO_DELAYLOAD
