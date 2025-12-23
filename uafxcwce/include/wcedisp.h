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
// This is a part of the Active Template Library(Windows CE).
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#ifndef _WCEDISP_H_
#define _WCEDISP_H_

/////////////////////////////////////////////////////////////////////////////
// Platform specific defines

#if defined(MIPSIV) || defined(MIPSIV_FP)
    // stack arguments are packed in 8-byte slots
	#define _STACK_INT      __int64
	#define _STACK_LONG     __int64
#else
	#define _STACK_INT      int
	#define _STACK_LONG     long
#endif // MIPSIV || MIPSIV_FP
	#define _STACK_FLOAT    float
	#define _STACK_DOUBLE   double
#if defined(MIPSIV) || defined(MIPSIV_FP)
    // stack arguments are packed in 8-byte slots
	#define _STACK_PTR      __int64
#else
	#define _STACK_PTR      void*
#endif // MIPSIV || MIPSIV_FP
#if defined(_WIN32_WCE_CEPC) || defined(_WIN32_WCE_EMULATION) || defined(_M_IX86)
	#define _SCRATCH_SIZE   16
	#define _STACK_OFFSET   0
	#define _STACK_MIN      0
#elif defined(_SHX_) || defined(_SH3_) || defined(_SH4_)
	#define _ALIGN_DOUBLES  4	
	#define _SCRATCH_SIZE   0
	#define _STACK_OFFSET   0
	#define _STACK_MIN      32      // 4 32-bit registers
	#define _RETVAL_FIRST
#elif defined(_ARM_) || defined(_THUMB_) || defined(ARMV4) || defined(ARMV4T) || defined(ARMV4I)
	#define _ALIGN_DOUBLES  4
	#define _SCRATCH_SIZE   0
	#define _STACK_OFFSET   0
	#define _STACK_MIN      32      // 4 32-bit registers
#elif defined(_MIPS_) || defined(_MIPS16_) || defined(MIPSII) || defined(MIPSII_FP) || defined(MIPSIV) || defined(MIPSIV_FP)
	#define _ALIGN_DOUBLES  8
	#define _SCRATCH_SIZE   0
	#define _STACK_OFFSET   0
	#define _STACK_MIN      32      // 4 32-bit registers
#elif defined(_PPC_)
	#define _ALIGN_DOUBLES  8
	#define _SHADOW_DOUBLES 13
	#define _SCRATCH_SIZE   (_SHADOW_DOUBLES*sizeof(double))
	#define _STACK_OFFSET   0
	#define _STACK_MIN      (64+32) // 8 32-bit registers, 32 bytes param space
	#define _RETVAL_FIRST
#elif defined(_WIN32_WCE_CEF)
	#include <cefrtl.h>
	#define _ALIGN_STACK	s_cache.nAlignStack		
	#define _ALIGN_DOUBLES	s_cache.nAlignDoubles	
	#define _SHADOW_DOUBLES	s_cache.nShadowDoubles	
	#define _SCRATCH_SIZE	s_cache.nScratchSize		
	#define _STACK_OFFSET	s_cache.nStackOffset		
	#define _STACK_MIN		s_cache.nStackMin		
	#define _RETVAL_FIRST 	s_cache.bRetValFirst		
	static struct dispCallInfoCache : public dispCallInfo 
	{
		dispCallInfoCache() { helper__GetDispCallInfo(this); }
	} s_cache;
	#define _AtlceDispatchCall helper__AtlceDispatchCall
#else
	#pragma message("Warning: no _AtlceDispatchCall information set for this CPU.")
#endif

#define VT_MFCVALUE         0xFFF       // special value for DISPID_VALUE
#define VT_MFCBYREF         0x40        // indicates VT_BYREF type
#define VT_MFCMARKER        0xFF        // delimits named parameters (INTERNAL USE)

extern "C" DWORD __stdcall _AtlceDispatchCall(DWORD pfn, void* pArgs, UINT nSizeArgs);

static const UINT _afxByValue[] =
{
	0,                          // VTS_EMPTY
	0,                          // VTS_NULL
	sizeof(_STACK_INT),         // VTS_I2
	sizeof(_STACK_LONG),        // VTS_I4
	sizeof(_STACK_FLOAT),       // VTS_R4
	sizeof(_STACK_DOUBLE),      // VTS_R8
	sizeof(CY),                 // VTS_CY
	sizeof(DATE),               // VTS_DATE
	sizeof(LPCOLESTR),          // VTS_WBSTR (VT_BSTR)
	sizeof(LPDISPATCH),         // VTS_DISPATCH
	sizeof(SCODE),              // VTS_SCODE
	sizeof(BOOL),               // VTS_BOOL
	sizeof(const VARIANT*),     // VTS_VARIANT
	sizeof(LPUNKNOWN),           // VTS_UNKNOWN
};

// size of arguments on stack when pushed by reference
static const UINT _afxByRef[] =
{
	0,                          // VTS_PEMPTY
	0,                          // VTS_PNULL
	sizeof(short*),             // VTS_PI2
	sizeof(long*),              // VTS_PI4
	sizeof(float*),             // VTS_PR4
	sizeof(double*),            // VTS_PR8
	sizeof(CY*),                // VTS_PCY
	sizeof(DATE*),              // VTS_PDATE
	sizeof(BSTR*),              // VTS_PBSTR
	sizeof(LPDISPATCH*),        // VTS_PDISPATCH
	sizeof(SCODE*),             // VTS_PSCODE
	sizeof(VARIANT_BOOL*),      // VTS_PBOOL
	sizeof(VARIANT*),           // VTS_PVARIANT
	sizeof(LPUNKNOWN*),         // VTS_PUNKNOWN
	sizeof(BYTE*),              // VTS_PUI1
};

static const UINT _afxRetVal[] =
{
	0,                          // VT_EMPTY
	0,                          // VT_NULL
	0,                          // VT_I2
	0,                          // VT_I4
	0,                          // VT_R4
	0,                          // VT_R8
	sizeof(CY*),                // VT_CY
	0,                          // VT_DATE (same as VT_R8)
	0,                          // VT_BSTR
	0,                          // VT_DISPATCH
	0,                          // VT_ERROR
	0,                          // VT_BOOL
	sizeof(VARIANT*),           // VT_VARIANT
	0,                          // VT_UNKNOWN
	0,                          // VT_UI1
};

#endif // _WCEDISP_H_
