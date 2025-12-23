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
// This is a part of the Active Template Library(Windows CE)
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef _ATL_DLL

namespace ATL
{
// ATLAPI Atl...() are moved here from atlcom.h in order to prevent multiple compilation
#ifndef _ATL_DLL
ATLAPI AtlIPersistStreamInit_Load(LPSTREAM pStm, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	ATLASSERT(pMap != NULL);
	HRESULT hr = S_OK;
	DWORD dwVer = 0;
	hr = pStm->Read(&dwVer, sizeof(DWORD), NULL);
	if (FAILED(hr))
		return hr;
	if (dwVer > _ATL_VER)
		return E_FAIL;

	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		// check if raw data entry
		if (pMap[i].dwSizeData != 0)
		{
			void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
			hr = pStm->Read(pData, pMap[i].dwSizeData, NULL);
			if (FAILED(hr))
				return hr;
			continue;
		}

		CComVariant var;

		hr = var.ReadFromStream(pStm);
		if (FAILED(hr))
			break;

		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				hr = E_FAIL;
				break;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			hr = E_FAIL;
			break;
		}
	}
	return hr;
}

ATLAPI AtlIPersistStreamInit_Save(LPSTREAM pStm,
	BOOL /* fClearDirty */, ATL_PROPMAP_ENTRY* pMap,
	void* pThis, IUnknown* pUnk)
{
	ATLASSERT(pMap != NULL);
	DWORD dw = _ATL_VER;
	HRESULT hr = pStm->Write(&dw, sizeof(DWORD), NULL);
	if (FAILED(hr))
		return hr;

	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		// check if raw data entry
		if (pMap[i].dwSizeData != 0)
		{
			void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
			hr = pStm->Write(pData, pMap[i].dwSizeData, NULL);
			if (FAILED(hr))
				return hr;
			continue;
		}

		CComVariant var;
		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				hr = E_FAIL;
				break;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			hr = E_FAIL;
			break;
		}

		hr = var.WriteToStream(pStm);
		if (FAILED(hr))
			break;
	}
	return hr;
}

ATLAPI AtlIPersistPropertyBag_Load(LPPROPERTYBAG pPropBag, LPERRORLOG pErrorLog, ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	USES_CONVERSION;
	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		CComVariant var;

		// If raw entry skip it - we don't handle it for property bags just yet
		if (pMap[i].dwSizeData != 0)
		{
			void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
			HRESULT hr = pPropBag->Read(pMap[i].szDesc, &var, pErrorLog);
			if (SUCCEEDED(hr))
			{
				// check the type - we only deal with limited set
				switch (pMap[i].vt)
				{
				case VT_UI1:
				case VT_I1:
					*((BYTE*)pData) = var.bVal;
					break;
				case VT_BOOL:
					*((VARIANT_BOOL*)pData) = var.boolVal;
					break;
				case VT_UI2:
					*((short*)pData) = var.iVal;
					break;
				case VT_UI4:
				case VT_INT:
				case VT_UINT:
					*((long*)pData) = var.lVal;
					break;
				}
			}
			continue;
		}

		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				return E_FAIL;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			return E_FAIL;
		}

		HRESULT hr = pPropBag->Read(pMap[i].szDesc, &var, pErrorLog);
		if (FAILED(hr))
		{
			if (hr == E_INVALIDARG)
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Property %s not in Bag\n"), OLE2CT(pMap[i].szDesc));
			}
			else
			{
				// Many containers return different ERROR values for Member not found
				ATLTRACE2(atlTraceCOM, 0, _T("Error attempting to read Property %s from PropertyBag \n"), OLE2CT(pMap[i].szDesc));
			}
			continue;
		}

		if (FAILED(CComDispatchDriver::PutProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			return E_FAIL;
		}
	}
	return S_OK;
}

ATLAPI AtlIPersistPropertyBag_Save(LPPROPERTYBAG pPropBag,
	BOOL /* fClearDirty */, BOOL /* fSaveAllProperties */,
	ATL_PROPMAP_ENTRY* pMap, void* pThis, IUnknown* pUnk)
{
	if (pPropBag == NULL)
	{
		ATLTRACE2(atlTraceCOM, 0, _T("PropBag pointer passed in was invalid\n"));
		return E_POINTER;
	}

	CComPtr<IDispatch> pDispatch;
	const IID* piidOld = NULL;
	for (int i = 0; pMap[i].pclsidPropPage != NULL; i++)
	{
		if (pMap[i].szDesc == NULL)
			continue;

		CComVariant var;

		// If raw entry skip it - we don't handle it for property bags just yet
		if (pMap[i].dwSizeData != 0)
		{
			void* pData = (void*) (pMap[i].dwOffsetData + (DWORD)pThis);
			// check the type - we only deal with limited set
			bool bTypeOK = false;
			switch (pMap[i].vt)
			{
			case VT_UI1:
			case VT_I1:
				var.bVal = *((BYTE*)pData);
				bTypeOK = true;
				break;
			case VT_BOOL:
				var.boolVal = *((VARIANT_BOOL*)pData);
				bTypeOK = true;
				break;
			case VT_UI2:
				var.iVal = *((short*)pData);
				bTypeOK = true;
				break;
			case VT_UI4:
			case VT_INT:
			case VT_UINT:
				var.lVal = *((long*)pData);
				bTypeOK = true;
				break;
			}
			if (bTypeOK)
			{
				var.vt = pMap[i].vt;
				HRESULT hr = pPropBag->Write(pMap[i].szDesc, &var);
				if (FAILED(hr))
					return hr;
			}
			continue;
		}

		if (pMap[i].piidDispatch != piidOld)
		{
			pDispatch.Release();
			if (FAILED(pUnk->QueryInterface(*pMap[i].piidDispatch, (void**)&pDispatch)))
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Failed to get a dispatch pointer for property #%i\n"), i);
				return E_FAIL;
			}
			piidOld = pMap[i].piidDispatch;
		}

		if (FAILED(CComDispatchDriver::GetProperty(pDispatch, pMap[i].dispid, &var)))
		{
			ATLTRACE2(atlTraceCOM, 0, _T("Invoked failed on DISPID %x\n"), pMap[i].dispid);
			return E_FAIL;
		}

		if (var.vt == VT_UNKNOWN || var.vt == VT_DISPATCH)
		{
			if (var.punkVal == NULL)
			{
				ATLTRACE2(atlTraceCOM, 0, _T("Warning skipping empty IUnknown in Save\n"));
				continue;
			}
		}

		HRESULT hr = pPropBag->Write(pMap[i].szDesc, &var);
		if (FAILED(hr))
			return hr;
	}
	return S_OK;
}

ATLAPI AtlGetObjectSourceInterface(IUnknown* punkObj, GUID* plibid, IID* piid, unsigned short* pdwMajor, unsigned short* pdwMinor)
{
	HRESULT hr = E_FAIL;
	if (punkObj != NULL)
	{
		CComPtr<IDispatch> spDispatch;
		hr = punkObj->QueryInterface(IID_IDispatch, (void**)&spDispatch);
		if (SUCCEEDED(hr))
		{
			CComPtr<ITypeInfo> spTypeInfo;
			hr = spDispatch->GetTypeInfo(0, 0, &spTypeInfo);
			if (SUCCEEDED(hr))
			{
				CComPtr<ITypeLib> spTypeLib;
				hr = spTypeInfo->GetContainingTypeLib(&spTypeLib, 0);
				if (SUCCEEDED(hr))
				{
					TLIBATTR* plibAttr;
					hr = spTypeLib->GetLibAttr(&plibAttr);
					if (SUCCEEDED(hr))
					{
						memcpy(plibid, &plibAttr->guid, sizeof(GUID));
						*pdwMajor = plibAttr->wMajorVerNum;
						*pdwMinor = plibAttr->wMinorVerNum;
						spTypeLib->ReleaseTLibAttr(plibAttr);
						// First see if the object is willing to tell us about the
						// default source interface via IProvideClassInfo2
						CComPtr<IProvideClassInfo2> spInfo;
						hr = punkObj->QueryInterface(IID_IProvideClassInfo2, (void**)&spInfo);
						if (SUCCEEDED(hr) && spInfo != NULL)
							hr = spInfo->GetGUID(GUIDKIND_DEFAULT_SOURCE_DISP_IID, piid);
						else
						{
							// No, we have to go hunt for it
							CComPtr<ITypeInfo> spInfoCoClass;
							// If we have a clsid, use that
							// Otherwise, try to locate the clsid from IPersist
							CComPtr<IPersist> spPersist;
							CLSID clsid;
							hr = punkObj->QueryInterface(IID_IPersist, (void**)&spPersist);
							if (SUCCEEDED(hr))
							{
								hr = spPersist->GetClassID(&clsid);
								if (SUCCEEDED(hr))
								{
									hr = spTypeLib->GetTypeInfoOfGuid(clsid, &spInfoCoClass);
									if (SUCCEEDED(hr))
									{
										TYPEATTR* pAttr=NULL;
										spInfoCoClass->GetTypeAttr(&pAttr);
										if (pAttr != NULL)
										{
											HREFTYPE hRef;
											for (int i = 0; i < pAttr->cImplTypes; i++)
											{
												int nType;
												hr = spInfoCoClass->GetImplTypeFlags(i, &nType);
												if (SUCCEEDED(hr))
												{
													if (nType == (IMPLTYPEFLAG_FDEFAULT | IMPLTYPEFLAG_FSOURCE))
													{
														// we found it
														hr = spInfoCoClass->GetRefTypeOfImplType(i, &hRef);
														if (SUCCEEDED(hr))
														{
															CComPtr<ITypeInfo> spInfo;
															hr = spInfoCoClass->GetRefTypeInfo(hRef, &spInfo);
															if (SUCCEEDED(hr))
															{
																TYPEATTR* pAttrIF;
																spInfo->GetTypeAttr(&pAttrIF);
																if (pAttrIF != NULL)
																{
																	memcpy(piid, &pAttrIF->guid, sizeof(GUID));
																}
																spInfo->ReleaseTypeAttr(pAttrIF);
															}
														}
														break;
													}
												}
											}
											spInfoCoClass->ReleaseTypeAttr(pAttr);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return hr;
}
#endif // !_ATL_DLL

#if defined(_MIPS_)
extern "C" void _asm(char *, ...);

#ifdef __cplusplus
extern "C" void __asm(const char *, ...);
#else
extern void __asm(const char *,...);
#endif

#endif

// Note: __stdcall maps to __cdecl under WinCE (except for _WIN32_WCE_EMULATION)

#if !defined(_WIN32_WCE_CEF)	// Use helper__AtlceDispatchCall in CEFRTL.DLL

extern "C" __declspec(naked) void __stdcall 
_AtlceDispatchCall(DWORD /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)
{
#if defined(_WIN32_WCE_CEPC) || defined(_M_IX86)
	_asm
	{
		pop     edx         // edx = return address
		pop     eax         // eax = pfn
		pop     ecx         // ecx = pArgs
		mov     esp,ecx     // esp = pArgs
		push	edx         // put the return address at top of stack
		jmp		eax			// jump to the target function
	}
#elif defined(x86) && !defined(_WIN32_WCE_EMULATION) // CE/PC only
	_asm
	{
		pop     edx         // edx = return address
		pop     eax         // eax = pfn
		pop     ecx         // ecx = pArgs
		mov     esp,ecx     // esp = pArgs
		push	edx         // put the return address at top of stack
		jmp		eax			// jump to the target function
	}
#elif defined(_WIN32_WCE_EMULATION)  // Emulation only
	_asm
	{
		pop     edx         // edx = return address
		pop     eax         // eax = pfn
		pop     ecx         // ecx = pArgs
		add     ecx,[esp]   // ecx += nSizeArgs (=scratch area)
		mov     [ecx],edx   // scratch[0] = return address
		sub     ecx,[esp]   // ecx = pArgs (again)
		mov     esp,ecx     // esp = pArgs (usually already correct)
		call    eax         // call member function
		ret                 // esp[0] should = scratch[0] = return address
	}
#elif defined(_MIPS_) && (defined(_MIPS16_) || defined(MIPS16))
	__emit(0x65bd);		// move   sp,a1
	__emit(0x6744);		// move   v0,a0
	__emit(0x9400);		// lw     a0,0(sp)
	__emit(0x9501);		// lw     a1,4(sp)
	__emit(0x9602);		// lw     a2,8(sp)
	__emit(0xea00);		// jr     v0
	__emit(0x9703);		// lw     a3,0xC(sp)
#elif (defined(_SH4_) || defined(SH4) || defined(MIPSII_FP) || defined(MIPSIV_FP))
	// We need to deal with the function arguments, packaged on the stack, in a special way. 
	// So AtlceDispatchCall will not be used for these processors. Rather, a special function will be called
	// instead of AtlceDispatchCall. The reason for special handling is because of the way
	// floating point values are passed on the stack. At present, none of the other architectures
	// bother about floating point values at all. As a result, for these processors, we need to generate
	// a thunk on the fly, encode and execute it dynamically. At present, for other
	// architectures, we are statically creating a thunk that will work well for int arguments.
	// Eventually other architectures will have to follow these processors's example.
#elif defined(MIPSIV)
	__asm("addiu     %sp,%a1,0x0");      // sp = pArgs
	__asm("addiu     %t6,%a0,0x0");      // t6 = pfn (save it)
	__asm("ld        %a0,0x0(%sp)");     // a0 = param0
	__asm("ld        %a1,0x8(%sp)");     // a1 = param1
	__asm("ld        %a2,0x10(%sp)");    // a2 = param2
	__asm("ld        %a3,0x18(%sp)");    // a3 = param3
	__asm("j         %t6");              // ip = pfn (jump to target function)
#elif defined(_MIPS_) || defined(MIPSII)
	__asm("addiu     %sp,%a1,0x0");      // sp = pArgs
	__asm("addiu     %t6,%a0,0x0");      // t6 = pfn (save it)
	__asm("lw        %a0,0x0(%sp)");     // a0 = param0
	__asm("lw        %a1,0x4(%sp)");     // a1 = param1
	__asm("lw        %a2,0x8(%sp)");     // a2 = param2
	__asm("lw        %a3,0xc(%sp)");     // a3 = param3
	__asm("j         %t6");              // ip = pfn (jump to target function)
#elif (defined(_SH3_) || defined(SH3))
	__asm("mov   R5, sp");  			  // sp = pStack
	__asm("mov   R4, R1");                // R1 = pfn (save it)
	__asm("mov.l @sp, R4");               // R4 = arg1 ('this' pointer)
	__asm("mov   #4, R0");                // R5 = arg2
	__asm("mov.l @(R0,sp), R5");
	__asm("mov   #8, R0");                // R6 = arg3
	__asm("mov.l @(R0,sp), R6");
	__asm("mov  #12, R0");                // R7 = arg4
	// __asm("mov.l @(R0,sp), R7"); moved to delay slot
	__asm("jmp  @R1\n"                    // jump to target function
		  "mov.l @(R0,sp), R7");
#elif defined(_ARM_) && defined(ARMV4T)
	// Set up the stack pointer. Note that the first four dwords of arguments
	// are saved on the callee's stack so 16 is added to the pointer to
	// the arguments.
	__emit(0x468d);     // mov  sp, r1
	__emit(0xb004);     // add  sp, #16
	// Save the function pointer in r12 (the only available scratch register)
	__emit(0x4684);     // mov  r12, r0
	// Copy four words of arguments into r0-r3
	__emit(0x6808);     // ldr  r0, [r1]
	__emit(0x688a);     // ldr  r2, [r1, #8]
	__emit(0x68cb);     // ldr  r3, [r1, #12]
	__emit(0x6849);     // ldr  r1, [r1, #4]
	// Jump to the function
	__emit(0x4760);     // mov  pc, r12
#elif defined(_ARM_) && defined(ARMV4I)
	// Set up the stack pointer. Note that the first four dwords of arguments
	// are saved on the callee's stack so 16 is added to the pointer to
	// the arguments.
	__emit(0xe281d010);     // add  sp, r1, #16
	// Save the function pointer in r12 (the only available scratch register)
	__emit(0xe1a0c000);     // mov  r12, r0
	// Copy four words of arguments into r0-r3
	__emit(0xe5910000);     // ldr  r0, [r1]
	__emit(0xe5912008);     // ldr  r2, [r1, #8]
	__emit(0xe591300c);     // ldr  r3, [r1, #12]
	__emit(0xe5911004);     // ldr  r1, [r1, #4]
	// Jump to the function
	__emit(0xe12fff1c);     // bx   r12
#elif defined(_ARM_) && defined(ARMV4)
	// Set up the stack pointer. Note that the first four dwords of arguments
	// are saved on the callee's stack so 16 is added to the pointer to
	// the arguments.
	__emit(0xe281d010);     // add  sp, r1, #16
	// Save the function pointer in r12 (the only available scratch register)
	__emit(0xe1a0c000);     // mov  r12, r0
	// Copy four words of arguments into r0-r3
	__emit(0xe5910000);     // ldr  r0, [r1]
	__emit(0xe5912008);     // ldr  r2, [r1, #8]
	__emit(0xe591300c);     // ldr  r3, [r1, #12]
	__emit(0xe5911004);     // ldr  r1, [r1, #4]
	// Jump to the function
	__emit(0xe1a0f00c);     // mov  pc, r12
#elif defined(_PPC_)
	__emit(0x7C812378); // mr          r1,r4
	__emit(0x7C6903A6); // mtctr       r3
	__emit(0x80610000); // lwz         r3,0(r1)
	__emit(0x80810004); // lwz         r4,4(r1)
	__emit(0x80A10008); // lwz         r5,8(r1)
	__emit(0x80C1000C); // lwz         r6,0Ch(r1)
	__emit(0x80E10010); // lwz         r7,10h(r1)
	__emit(0x81010014); // lwz         r8,14h(r1)
	__emit(0x81210018); // lwz         r9,18h(r1)
	__emit(0x8141001C); // lwz         r10,1Ch(r1)
	__emit(0x3821FFF8); // addi        r1,r1,FFF8h 
	__emit(0x4E800420); // bctr
#else
	#error No _AtlceDispatchCall implementation found for this CPU.
#endif // end of WinCE CPUs
}
#endif // _WIN32_WCE_CEF

} //namespace ATL

#endif // !_ATL_DLL

////////////////////////////////////////////////////////////////////////////
//
//  CStreamImp::CStreamImp
//
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//
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
        m_dwSize = ::LocalSize(pData);
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

        memcpy(
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

    memcpy(m_pData + m_dwCurrentPosition, pv, cb);

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

///////////////////////////////////////////////////////////////////////////////////////////

namespace ATL
{

HRESULT wce_ATL_OleLoadFromStream ( LPSTREAM pStm, REFIID iidInterface, LPVOID FAR* ppvObj)
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

HRESULT wce_ATL_OleSaveToStream ( LPPERSISTSTREAM pPStm, LPSTREAM pStm)
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

HRESULT wce_ATL_CLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID pclsid)
{
#if (_WIN32_WCE >= 210)
	return CLSIDFromProgID(lpszProgID,pclsid);
#else // _WIN32_WCE >= 210
    // lookup lpsz\\clsid and call CLSIDFromString on the result;
    WCHAR sz[256];
    ULONG cbValue = sizeof(sz);
	HKEY hSubKey;
	LONG lResult;

    if (lpszProgID == NULL)
    {
        return(E_INVALIDARG);
    }

    if (*lpszProgID == 0)
    {
        return(CO_E_CLASSSTRING);
    }
    _tcscpy(sz, lpszProgID);
    _tcscat(sz, _T("\\Clsid"));

	lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, sz, 0, 0, &hSubKey);
	if (lResult == ERROR_SUCCESS)
	{
		TCHAR strValueName[60];
		DWORD dwValueNameLength = sizeof(strValueName);
		DWORD dwType;
		lResult = RegEnumValue(hSubKey, 0, strValueName, &dwValueNameLength, 0, &dwType, (LPBYTE)sz, &cbValue);
		ATLASSERT(dwType == REG_SZ);
		ATLASSERT(lResult == ERROR_SUCCESS);
		RegCloseKey (hSubKey);
		return CLSIDFromString(sz, pclsid);
	}

    return E_FAIL;
#endif // _WIN32_WCE >= 210
}


HRESULT wce_ATL_ProgIDFromCLSID(REFCLSID clsid, LPOLESTR *lplpszProgID)
{
	LPOLESTR lpszCLSID = NULL;
    WCHAR szKeyCLSID[64];
	static WCHAR szProgID[64];
	HKEY hKey;
	LONG lResult;

	*lplpszProgID = NULL;

	if(StringFromCLSID(clsid, &lpszCLSID) == S_OK)
	{
		_tcscpy(szKeyCLSID, _T("CLSID\\"));
		_tcscat(szKeyCLSID, lpszCLSID);

		lResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, szKeyCLSID, 0, 0, &hKey);
		if (lResult == ERROR_SUCCESS)
		{
			TCHAR strValueName[60];
			DWORD dwValueNameLength = sizeof(strValueName)/sizeof(strValueName[0]), dwType, cbValue;
			lResult = RegEnumValue(hKey, 0, strValueName, &dwValueNameLength, 0, &dwType, 
								   (LPBYTE)szProgID, &cbValue);
			RegCloseKey (hKey);
			if(lResult == ERROR_SUCCESS)
				*lplpszProgID = szProgID;
		}

		// Free lpszCLSID
		/*LPMALLOC lpIMalloc = NULL; 
		if(CoGetMalloc(MEMCTX_TASK, &lpIMalloc) == NOERROR) 
		{ 
			lpIMalloc->Free((LPVOID)lpszCLSID); 
			lpIMalloc->Release(); 
		} */
		// There's no CoGetMalloc in WCE.  Assuming it uses malloc (same as GlobalAlloc), so use free.
		free((LPVOID)lpszCLSID);
	}

	return (*lplpszProgID != NULL)? S_OK : E_FAIL;
}

int wce_ATL_MulDiv(int nNumber, int nNumerator, int nDenominator)
{
	__int64 x;

	x = (__int64)nNumber * (__int64)nNumerator;
	x /= (__int64)nDenominator;

	return (int)x;
}

HRESULT wce_ATL_CreateStreamOnHGlobal(
    HGLOBAL     hGlobal,
    BOOL        fDeleteOnRelease,
    LPSTREAM *  ppstm)
{
    HRESULT             hr;

    *ppstm = new CStreamImp(
                    NULL,
                    NULL,
                    NULL,
                    NULL,  // pointer to Critical Section
                    &hr,
                    (PBYTE)hGlobal,
                    fDeleteOnRelease);

    if ((*ppstm == NULL) || (hr != S_OK))
    {
        if (*ppstm == NULL)
            return E_OUTOFMEMORY;
        else
            delete ppstm;
    }

    return hr;
}

} //namespace ATL
