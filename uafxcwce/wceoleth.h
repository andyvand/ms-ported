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

#ifndef __WCEOLETH_H__
#define __WCEOLETH_H__

#if defined(_WIN32_WCE_NO_DELAYLOAD)
#pragma warning(disable: 4005)

#if !defined(_WIN32_WCE_NO_OLE_THUNKS)
struct AFX_OLE_CALL
{
	// main OLE32.DLL entry points
	HRESULT (STDAPICALLTYPE* pfnReadFmtUserTypeStg)(LPSTORAGE pstg,
		CLIPFORMAT FAR* pcf, LPOLESTR FAR* lplpszUserType);
	HRESULT (STDAPICALLTYPE* pfnReadClassStg)(LPSTORAGE pStg, CLSID FAR* pclsid);
	HRESULT (STDAPICALLTYPE* pfnCreateFileMoniker)(LPCOLESTR lpszPathName,
		LPMONIKER FAR* ppmk);
	HRESULT (STDAPICALLTYPE* pfnStgIsStorageFile)(const OLECHAR * pwcsName);
	HRESULT (STDAPICALLTYPE* pfnStgOpenStorage)(const OLECHAR * pwcsName,
		IStorage *pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved,
		IStorage ** ppstgOpen);
	HRESULT (STDAPICALLTYPE* pfnDoDragDrop)(LPDATAOBJECT pDataObj, LPDROPSOURCE pDropSource,
		DWORD dwOKEffects, LPDWORD pdwEffect);
	HRESULT (STDAPICALLTYPE* pfnCoLockObjectExternal)(LPUNKNOWN pUnk, BOOL fLock, BOOL fLastUnlockReleases);
	HRESULT (STDAPICALLTYPE* pfnRegisterDragDrop)(HWND hwnd, LPDROPTARGET pDropTarget);
	HRESULT (STDAPICALLTYPE* pfnOleRegGetUserType )(REFCLSID clsid, DWORD dwFormOfType,
		LPOLESTR * pszUserType);
	HRESULT (STDAPICALLTYPE* pfnStgCreateDocfile)(const OLECHAR * pwcsName, DWORD grfMode,
		DWORD reserved, IStorage** ppstgOpen);
	HRESULT (STDAPICALLTYPE* pfnRevokeDragDrop)(HWND hwnd);;
	HRESULT (STDAPICALLTYPE* pfnCoRegisterClassObject)(REFCLSID rclsid, LPUNKNOWN pUnk,
		DWORD dwClsContext, DWORD flags, LPDWORD lpdwRegister);
	HRESULT (STDAPICALLTYPE* pfnCoRevokeClassObject)(DWORD dwRegister);
	HRESULT (STDAPICALLTYPE* pfnOleTranslateAccelerator)(LPOLEINPLACEFRAME lpFrame,
		LPOLEINPLACEFRAMEINFO lpFrameInfo, LPMSG lpmsg);
	BOOL (STDAPICALLTYPE* pfnIsAccelerator)(HACCEL hAccel, INT cAccelEntries,
		LPMSG lpMsg, WORD* lpwCmd);
	HOLEMENU (STDAPICALLTYPE* pfnOleCreateMenuDescriptor)(HMENU hmenuCombined,
		LPOLEMENUGROUPWIDTHS lpMenuWidths);
	HRESULT (STDAPICALLTYPE* pfnOleDestroyMenuDescriptor )(HOLEMENU holemenu);
	HRESULT (STDAPICALLTYPE* pfnGetRunningObjectTable)(DWORD reserved, LPRUNNINGOBJECTTABLE FAR* pprot);
	HRESULT (STDAPICALLTYPE* pfnWriteClassStg)(LPSTORAGE pStg, REFCLSID rclsid);
	HRESULT (STDAPICALLTYPE* pfnOleQueryLinkFromData)(LPDATAOBJECT pSrcDataObject);
	HRESULT (STDAPICALLTYPE* pfnCoRegisterMessageFilter)(LPMESSAGEFILTER lpMessageFilter,
		LPMESSAGEFILTER * lplpMessageFilter);
	HRESULT (STDAPICALLTYPE* pfnCoCreateInstance)(REFCLSID rclsid, LPUNKNOWN * pUnkOuter,
		DWORD dwClsContext, REFIID riid, LPVOID* ppv);
	HRESULT (STDAPICALLTYPE* pfnCreateBindCtx)(DWORD reserved, LPBC FAR* ppbc);
	HRESULT (STDAPICALLTYPE* pfnStringFromCLSID)(REFCLSID rclsid, LPOLESTR FAR* lplpsz);
	HRESULT (STDAPICALLTYPE* pfnCoDisconnectObject)(LPUNKNOWN pUnk, DWORD dwReserved);
	HRESULT (STDAPICALLTYPE* pfnOleRegEnumVerbs )(REFCLSID clsid, LPENUMOLEVERB FAR* ppenum);
	void (STDAPICALLTYPE* pfnOleUninitialize)(void);
	HRESULT (STDAPICALLTYPE* pfnCreateOleAdviseHolder)(LPOLEADVISEHOLDER FAR* ppOAHolder);
	HRESULT (STDAPICALLTYPE* pfnCreateDataAdviseHolder)(LPDATAADVISEHOLDER FAR* ppDAHolder);
	HRESULT (STDAPICALLTYPE* pfnOleGetAutoConvert)(REFCLSID clsidOld, LPCLSID pClsidNew);
	HRESULT (STDAPICALLTYPE* pfnCoGetClassObject)(REFCLSID rclsid, DWORD dwClsContext,
		LPVOID pvReserved, REFIID riid, LPVOID* ppv);
	HRESULT (STDAPICALLTYPE* pfnOleCreateDefaultHandler)(REFCLSID clsid,
		LPUNKNOWN pUnkOuter, REFIID riid, LPVOID* lplpvObj);
WCE_DEL	HRESULT (STDAPICALLTYPE* pfnCreateDataCache)( LPUNKNOWN pUnkOuter, REFCLSID clsid, REFIID riid, LPVOID* lplpvObj);
	HRESULT (STDAPICALLTYPE* pfnReadClassStm)(LPSTREAM pStm, CLSID FAR* pclsid);
	HRESULT (STDAPICALLTYPE* pfnOleLoadFromStream)(LPSTREAM pStm, REFIID iidInterface, LPVOID FAR* ppvObj);
	int (STDAPICALLTYPE* pfnStringFromGUID2)(REFGUID rguid, LPOLESTR lpsz, int cbMax);
	void (STDAPICALLTYPE* pfnCoUninitialize)(void);
	HRESULT (STDAPICALLTYPE* pfnCoInitialize)(LPVOID pvReserved);
WCE_INS HRESULT (STDAPICALLTYPE* pfnCoInitializeEx)(LPVOID pvReserved, DWORD dwCoInit);
WCE_DEL	HRESULT (STDAPICALLTYPE* pfnOleInitialize)(LPVOID pvReserved);
	void (STDAPICALLTYPE* pfnCoFreeUnusedLibraries)(void);
	HRESULT (STDAPICALLTYPE* pfnOleCreateFromData)(LPDATAOBJECT pSrcDataObj,
		REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc,
		LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleSetContainedObject)(LPUNKNOWN pUnknown, BOOL fContained);
	HRESULT (STDAPICALLTYPE* pfnOleLockRunning)(LPUNKNOWN pUnknown, BOOL fLock, BOOL fLastUnlockCloses);
	LPVOID (STDAPICALLTYPE* pfnCoTaskMemAlloc)(ULONG cb);
WCE_INS LPVOID (STDAPICALLTYPE* pfnCoTaskMemRealloc)(LPVOID pv, ULONG cb);
	HRESULT (STDAPICALLTYPE* pfnCLSIDFromString)(LPOLESTR lpsz, LPCLSID pclsid);
	HRESULT (STDAPICALLTYPE* pfnCLSIDFromProgID )(LPCOLESTR lpszProgID, LPCLSID lpclsid);
	HRESULT (STDAPICALLTYPE* pfnOleIsCurrentClipboard)(LPDATAOBJECT pDataObj);
	HRESULT (STDAPICALLTYPE* pfnOleFlushClipboard)(void);
	HRESULT (STDAPICALLTYPE* pfnOleSetClipboard)(LPDATAOBJECT pDataObj);
	BOOL (STDAPICALLTYPE* pfnOleIsRunning)(LPOLEOBJECT pObject);;
	HRESULT (STDAPICALLTYPE* pfnOleRun)(LPUNKNOWN pUnknown);
	HRESULT (STDAPICALLTYPE* pfnOleGetClipboard)(LPDATAOBJECT FAR* ppDataObj);
	HRESULT (STDAPICALLTYPE* pfnCoTreatAsClass)(REFCLSID clsidOld, REFCLSID clsidNew);
	HRESULT (STDAPICALLTYPE* pfnOleQueryCreateFromData)(LPDATAOBJECT pSrcDataObject);
	HRESULT (STDAPICALLTYPE* pfnOleSetMenuDescriptor )(HOLEMENU holemenu,
		HWND hwndFrame, HWND hwndActiveObject, LPOLEINPLACEFRAME lpFrame,
		LPOLEINPLACEACTIVEOBJECT lpActiveObj);
	HRESULT (STDAPICALLTYPE* pfnCreateItemMoniker)(LPCOLESTR lpszDelim,
		LPCOLESTR lpszItem, LPMONIKER* ppmk);
	HRESULT (STDAPICALLTYPE* pfnCreateGenericComposite)(LPMONIKER pmkFirst,
		LPMONIKER pmkRest, LPMONIKER* ppmkComposite);
	HRESULT (STDAPICALLTYPE* pfnCreateStreamOnHGlobal)(HGLOBAL hGlobal,
		BOOL fDeleteOnRelease, LPSTREAM* ppstm);
	HRESULT (STDAPICALLTYPE* pfnOleSaveToStream)(LPPERSISTSTREAM pPStm, LPSTREAM pStm);
	HRESULT (STDAPICALLTYPE* pfnWriteClassStm)(LPSTREAM pStm, REFCLSID rclsid);
	void (STDAPICALLTYPE* pfnCoTaskMemFree)(LPVOID pv);
	HGLOBAL (STDAPICALLTYPE* pfnOleGetIconOfClass)(REFCLSID rclsid, LPOLESTR lpszLabel,
		BOOL fUseTypeAsLabel);
	void (STDAPICALLTYPE* pfnReleaseStgMedium)(LPSTGMEDIUM);
	HRESULT (STDAPICALLTYPE* pfnGetHGlobalFromILockBytes )(LPLOCKBYTES plkbyt, HGLOBAL FAR* phglobal);
	HRESULT (STDAPICALLTYPE* pfnStgOpenStorageOnILockBytes)(LPLOCKBYTES plkbyt,
		LPSTORAGE pstgPriority, DWORD grfMode, SNB snbExclude, DWORD reserved,
		LPSTORAGE* ppstgOpen);
	HRESULT (STDAPICALLTYPE* pfnCreateILockBytesOnHGlobal)(HGLOBAL hGlobal,
		BOOL fDeleteOnRelease, LPLOCKBYTES* pplkbyt);
	HRESULT (STDAPICALLTYPE* pfnStgCreateDocfileOnILockBytes)(LPLOCKBYTES plkbyt,
		DWORD grfMode, DWORD reserved, LPSTORAGE* ppstgOpen);
	HRESULT (STDAPICALLTYPE* pfnOleSave)(LPPERSISTSTORAGE pPS, LPSTORAGE pStg, BOOL fSameAsLoad);
	HRESULT (STDAPICALLTYPE* pfnOleLoad)(LPSTORAGE pStg, REFIID riid,
		LPOLECLIENTSITE pClientSite, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleCreate)(REFCLSID rclsid, REFIID riid,
		DWORD renderopt, LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite,
		LPSTORAGE pStg, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleCreateLinkToFile)(LPCOLESTR lpszFileName,
		REFIID riid, DWORD renderopt, LPFORMATETC lpFormatEtc,
		LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleCreateFromFile)(REFCLSID rclsid,
		LPCOLESTR lpszFileName, REFIID riid, DWORD renderopt,
		LPFORMATETC pFormatEtc, LPOLECLIENTSITE pClientSite, LPSTORAGE pStg,
		LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleCreateStaticFromData)(LPDATAOBJECT pSrcDataObj,
		REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc,
		LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnOleCreateLinkFromData)(LPDATAOBJECT pSrcDataObj,
		REFIID riid, DWORD renderopt, LPFORMATETC pFormatEtc,
		LPOLECLIENTSITE pClientSite, LPSTORAGE pStg, LPVOID* ppvObj);
	HRESULT (STDAPICALLTYPE* pfnSetConvertStg)(LPSTORAGE pStg, BOOL fConvert);
	HANDLE (STDAPICALLTYPE* pfnOleDuplicateData)(HANDLE hSrc, CLIPFORMAT cfFormat,
		UINT uiFlags);
	HRESULT (STDAPICALLTYPE* pfnWriteFmtUserTypeStg )(LPSTORAGE pstg, CLIPFORMAT cf, LPOLESTR lpszUserType);
	HRESULT (STDAPICALLTYPE* pfnOleRegGetMiscStatus)(REFCLSID clsid, DWORD dwAspect,
		DWORD* pdwStatus);
	HRESULT (STDAPICALLTYPE* pfnCoGetMalloc)(DWORD dwMemContext, LPMALLOC * ppMalloc);
	HRESULT (STDAPICALLTYPE* pfnStgIsStorageILockBytes)(LPLOCKBYTES plkbyt);

	// OLEAUT32.DLL entry points
	void (STDAPICALLTYPE* pfnSysFreeString)(BSTR);
	BSTR (STDAPICALLTYPE* pfnSysAllocStringByteLen)(const char FAR* psz,
		unsigned int len);
	HRESULT (STDAPICALLTYPE* pfnVariantCopy)(VARIANTARG FAR* pvargDest,
		VARIANTARG FAR* pvargSrc);
	HRESULT (STDAPICALLTYPE* pfnVariantClear)(VARIANTARG FAR* pvarg);
	HRESULT (STDAPICALLTYPE* pfnVariantChangeType)(VARIANTARG FAR* pvargDest,
		VARIANTARG FAR* pvarSrc, unsigned short wFlags, VARTYPE vt);
	BSTR (STDAPICALLTYPE* pfnSysAllocStringLen)(const OLECHAR FAR*,
		unsigned int);
	unsigned int (STDAPICALLTYPE* pfnSysStringLen)(BSTR);
	int (STDAPICALLTYPE* pfnSysReAllocStringLen)(BSTR FAR*, const OLECHAR FAR*,
		unsigned int);
	BSTR (STDAPICALLTYPE* pfnSysAllocString)(const OLECHAR FAR*);
	unsigned int (STDAPICALLTYPE* pfnSysStringByteLen)(BSTR bstr);
	HRESULT (STDAPICALLTYPE* pfnVarCyFromStr)(OLECHAR FAR* strIn, LCID lcid,
		unsigned long dwFlags, CY FAR* pcyOut);
	HRESULT (STDAPICALLTYPE* pfnVarBstrFromCy)(CY cyIn, LCID lcid,
		unsigned long dwFlags, BSTR FAR* pbstrOut);
	HRESULT (STDAPICALLTYPE* pfnVarDateFromStr)(OLECHAR FAR* strIn, LCID lcid,
		unsigned long dwFlags, DATE FAR* pdateOut);
	HRESULT (STDAPICALLTYPE* pfnVarBstrFromDate)(DATE dateIn, LCID lcid,
		unsigned long dwFlags, BSTR FAR* pbstrOut);
	HRESULT (STDAPICALLTYPE* pfnLoadTypeLib)(const OLECHAR FAR *szFile,
		ITypeLib FAR* FAR* pptlib);
	HRESULT (STDAPICALLTYPE* pfnLoadRegTypeLib)(REFGUID guid,
		WORD wVerMajor, WORD wVerMinor, LCID lcid, ITypeLib FAR* FAR* pptlib);
	HRESULT (STDAPICALLTYPE* pfnRegisterTypeLib)(ITypeLib FAR* ptlib,
		OLECHAR FAR *szFullPath, OLECHAR FAR *szHelpDir);
	int (STDAPICALLTYPE* pfnDosDateTimeToVariantTime)(unsigned short wDosDate,
		unsigned short wDosTime, double FAR* pvtime);
	SAFEARRAY FAR* (STDAPICALLTYPE* pfnSafeArrayCreate)(VARTYPE vt,
		unsigned int cDims, SAFEARRAYBOUND FAR* rgsabound);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayRedim)(SAFEARRAY FAR* psa,
		SAFEARRAYBOUND FAR* psaboundNew);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayAccessData)(SAFEARRAY FAR* psa,
		void HUGEP* FAR* ppvData);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayUnaccessData)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayGetUBound)(SAFEARRAY FAR* psa,
		unsigned int nDim, long FAR* plUbound);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayGetLBound)(SAFEARRAY FAR* psa,
		unsigned int nDim, long FAR* plLbound);
	unsigned int (STDAPICALLTYPE* pfnSafeArrayGetElemsize)(SAFEARRAY FAR* psa);
	unsigned int (STDAPICALLTYPE* pfnSafeArrayGetDim)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayCopy)(SAFEARRAY FAR* psa,
		SAFEARRAY FAR* FAR* ppsaOut);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayAllocData)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayAllocDescriptor)(unsigned int cDims,
		SAFEARRAY FAR* FAR* ppsaOut);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayGetElement)(SAFEARRAY FAR* psa,
		long FAR* rgIndices, void FAR* pvData);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayPutElement)(SAFEARRAY FAR* psa,
		long FAR* rgIndices, void FAR* pvData);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayLock)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayUnlock)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayDestroy)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayDestroyData)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayDestroyDescriptor)(SAFEARRAY FAR* psa);
	HRESULT (STDAPICALLTYPE* pfnSafeArrayPtrOfIndex)(SAFEARRAY FAR* psa,
		long FAR* rgIndices, void HUGEP* FAR* ppvData);
};

extern AFX_DATADEF AFX_OLE_CALL _afxOLE;

#define ReadFmtUserTypeStg           _afxOLE.pfnReadFmtUserTypeStg
#define ReadClassStg                 _afxOLE.pfnReadClassStg
#define CreateFileMoniker            _afxOLE.pfnCreateFileMoniker
#define StgIsStorageFile             _afxOLE.pfnStgIsStorageFile
#define StgOpenStorage               _afxOLE.pfnStgOpenStorage
#define CoLockObjectExternal         _afxOLE.pfnCoLockObjectExternal
#define RegisterDragDrop             _afxOLE.pfnRegisterDragDrop
#define OleRegGetUserType            _afxOLE.pfnOleRegGetUserType
#define StgCreateDocfile             _afxOLE.pfnStgCreateDocfile
#define RevokeDragDrop               _afxOLE.pfnRevokeDragDrop
#define CoRegisterClassObject        _afxOLE.pfnCoRegisterClassObject
#define CoRevokeClassObject          _afxOLE.pfnCoRevokeClassObject
#define OleTranslateAccelerator      _afxOLE.pfnOleTranslateAccelerator
#define IsAccelerator                _afxOLE.pfnIsAccelerator
#define OleCreateMenuDescriptor      _afxOLE.pfnOleCreateMenuDescriptor
#define OleDestroyMenuDescriptor     _afxOLE.pfnOleDestroyMenuDescriptor
#define GetRunningObjectTable        _afxOLE.pfnGetRunningObjectTable
#define WriteClassStg                _afxOLE.pfnWriteClassStg
#define OleQueryLinkFromData         _afxOLE.pfnOleQueryLinkFromData
#define CoRegisterMessageFilter      _afxOLE.pfnCoRegisterMessageFilter
#define CoCreateInstance             _afxOLE.pfnCoCreateInstance
#define CreateBindCtx                _afxOLE.pfnCreateBindCtx
#define StringFromCLSID              _afxOLE.pfnStringFromCLSID
#define CoDisconnectObject           _afxOLE.pfnCoDisconnectObject
#define OleRegEnumVerbs              _afxOLE.pfnOleRegEnumVerbs
#define OleUninitialize              _afxOLE.pfnOleUninitialize
#define CreateOleAdviseHolder        _afxOLE.pfnCreateOleAdviseHolder
#define CreateDataAdviseHolder       _afxOLE.pfnCreateDataAdviseHolder
#define OleGetAutoConvert            _afxOLE.pfnOleGetAutoConvert
#define CoGetClassObject             _afxOLE.pfnCoGetClassObject
#define OleCreateDefaultHandler      _afxOLE.pfnOleCreateDefaultHandler
#define CreateDataCache              _afxOLE.pfnCreateDataCache
#define ReadClassStm                 _afxOLE.pfnReadClassStm
#define OleLoadFromStream            _afxOLE.pfnOleLoadFromStream
#define StringFromGUID2              _afxOLE.pfnStringFromGUID2
#define CoUninitialize               _afxOLE.pfnCoUninitialize
#define CoInitialize                 _afxOLE.pfnCoInitialize
#define CoInitializeEx               _afxOLE.pfnCoInitializeEx
#define CoFreeUnusedLibraries        _afxOLE.pfnCoFreeUnusedLibraries
#define OleCreateFromData            _afxOLE.pfnOleCreateFromData
#define OleSetContainedObject        _afxOLE.pfnOleSetContainedObject
#define OleLockRunning               _afxOLE.pfnOleLockRunning
#define CoTaskMemAlloc               _afxOLE.pfnCoTaskMemAlloc
#define CoTaskMemRealloc             _afxOLE.pfnCoTaskMemRealloc
#define CLSIDFromString              _afxOLE.pfnCLSIDFromString
#define CLSIDFromProgID              _afxOLE.pfnCLSIDFromProgID
#define OleIsCurrentClipboard        _afxOLE.pfnOleIsCurrentClipboard
#define OleFlushClipboard            _afxOLE.pfnOleFlushClipboard
#define OleSetClipboard              _afxOLE.pfnOleSetClipboard
#define OleIsRunning                 _afxOLE.pfnOleIsRunning
#define OleRun                       _afxOLE.pfnOleRun
#define OleGetClipboard              _afxOLE.pfnOleGetClipboard
#define CoTreatAsClass               _afxOLE.pfnCoTreatAsClass
#define OleQueryCreateFromData       _afxOLE.pfnOleQueryCreateFromData
#define OleSetMenuDescriptor         _afxOLE.pfnOleSetMenuDescriptor
#define CreateItemMoniker            _afxOLE.pfnCreateItemMoniker
#define CreateGenericComposite       _afxOLE.pfnCreateGenericComposite
#define CreateStreamOnHGlobal        _afxOLE.pfnCreateStreamOnHGlobal
#define OleSaveToStream              _afxOLE.pfnOleSaveToStream
#define WriteClassStm                _afxOLE.pfnWriteClassStm
#define CoTaskMemFree                _afxOLE.pfnCoTaskMemFree
#define OleGetIconOfClass            _afxOLE.pfnOleGetIconOfClass
#define ReleaseStgMedium             _afxOLE.pfnReleaseStgMedium
#define GetHGlobalFromILockBytes     _afxOLE.pfnGetHGlobalFromILockBytes
#define StgOpenStorageOnILockBytes   _afxOLE.pfnStgOpenStorageOnILockBytes
#define CreateILockBytesOnHGlobal    _afxOLE.pfnCreateILockBytesOnHGlobal
#define StgCreateDocfileOnILockBytes _afxOLE.pfnStgCreateDocfileOnILockBytes
#define OleSave                      _afxOLE.pfnOleSave
#define OleLoad                      _afxOLE.pfnOleLoad
#define OleCreate                    _afxOLE.pfnOleCreate
#define OleCreateLinkToFile          _afxOLE.pfnOleCreateLinkToFile
#define OleCreateFromFile            _afxOLE.pfnOleCreateFromFile
#define OleCreateStaticFromData      _afxOLE.pfnOleCreateStaticFromData
#define OleCreateLinkFromData        _afxOLE.pfnOleCreateLinkFromData
#define SetConvertStg                _afxOLE.pfnSetConvertStg
#define OleDuplicateData             _afxOLE.pfnOleDuplicateData
#define WriteFmtUserTypeStg          _afxOLE.pfnWriteFmtUserTypeStg
#define OleRegGetMiscStatus          _afxOLE.pfnOleRegGetMiscStatus
#define CoGetMalloc                  _afxOLE.pfnCoGetMalloc
#define StgIsStorageILockBytes       _afxOLE.pfnStgIsStorageILockBytes
#define SysFreeString                _afxOLE.pfnSysFreeString
#define SysAllocStringByteLen        _afxOLE.pfnSysAllocStringByteLen
#define VariantCopy                  _afxOLE.pfnVariantCopy
#define VariantClear                 _afxOLE.pfnVariantClear
#define VariantChangeType            _afxOLE.pfnVariantChangeType
#define SysAllocStringLen            _afxOLE.pfnSysAllocStringLen
#define SysStringLen                 _afxOLE.pfnSysStringLen
#define SysReAllocStringLen          _afxOLE.pfnSysReAllocStringLen
#define SysAllocString               _afxOLE.pfnSysAllocString
#define SysStringByteLen             _afxOLE.pfnSysStringByteLen
#define VarCyFromStr                 _afxOLE.pfnVarCyFromStr
#define VarBstrFromCy                _afxOLE.pfnVarBstrFromCy
#define VarDateFromStr               _afxOLE.pfnVarDateFromStr
#define VarBstrFromDate              _afxOLE.pfnVarBstrFromDate
#define LoadTypeLib                  _afxOLE.pfnLoadTypeLib
#define LoadRegTypeLib               _afxOLE.pfnLoadRegTypeLib
#define RegisterTypeLib              _afxOLE.pfnRegisterTypeLib
#define DosDateTimeToVariantTime     _afxOLE.pfnDosDateTimeToVariantTime
#define SafeArrayCreate              _afxOLE.pfnSafeArrayCreate
#define SafeArrayRedim               _afxOLE.pfnSafeArrayRedim
#define SafeArrayAccessData          _afxOLE.pfnSafeArrayAccessData
#define SafeArrayUnaccessData        _afxOLE.pfnSafeArrayUnaccessData
#define SafeArrayGetUBound           _afxOLE.pfnSafeArrayGetUBound
#define SafeArrayGetLBound           _afxOLE.pfnSafeArrayGetLBound
#define SafeArrayGetElemsize         _afxOLE.pfnSafeArrayGetElemsize
#define SafeArrayGetDim              _afxOLE.pfnSafeArrayGetDim
#define SafeArrayCopy                _afxOLE.pfnSafeArrayCopy
#define SafeArrayAllocData           _afxOLE.pfnSafeArrayAllocData
#define SafeArrayAllocDescriptor     _afxOLE.pfnSafeArrayAllocDescriptor
#define SafeArrayGetElement          _afxOLE.pfnSafeArrayGetElement
#define SafeArrayPutElement          _afxOLE.pfnSafeArrayPutElement
#define SafeArrayLock                _afxOLE.pfnSafeArrayLock
#define SafeArrayUnlock              _afxOLE.pfnSafeArrayUnlock
#define SafeArrayDestroy             _afxOLE.pfnSafeArrayDestroy
#define SafeArrayDestroyData         _afxOLE.pfnSafeArrayDestroyData
#define SafeArrayDestroyDescriptor   _afxOLE.pfnSafeArrayDestroyDescriptor
#define SafeArrayPtrOfIndex          _afxOLE.pfnSafeArrayPtrOfIndex
#if defined(_AFX_OLE_IMPL)
//DoDragDrop
inline HRESULT STDAPICALLTYPE DoDragDrop(LPDATAOBJECT pDataObj,
	LPDROPSOURCE pDropSource, DWORD dwOKEffects, LPDWORD pdwEffect)
{
	return _afxOLE.pfnDoDragDrop(pDataObj, pDropSource, dwOKEffects,
		pdwEffect);
}
#endif

#endif // _WIN32_WCE_NO_OLE_THUNKS

#pragma warning(default: 4005)
#endif // _WIN32_WCE_NO_DELAYLOAD
#endif // __WCEOLETH_H__

