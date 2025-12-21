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

/////////////////////////////////////////////////////////////////////////////
// Platform specific defines

#if defined(_WIN32_WCE)
#if defined(MIPSIV) || defined(MIPSIV_FP)
    // stack arguments are packed in 8-byte slots
	#define _STACK_INT      __int64
	#define _STACK_LONG     __int64
#else
	#define _STACK_INT      int
	#define _STACK_LONG     long
#endif
	#define _STACK_FLOAT    float
	#define _STACK_DOUBLE   double
#if defined(MIPSIV) || defined(MIPSIV_FP)
    // stack arguments are packed in 8-byte slots
	#define _STACK_PTR      __int64
#else
	#define _STACK_PTR      void*
#endif
#if defined(_WIN32_WCE_CEPC) || defined(_WIN32_WCE_EMULATION) || defined(_M_IX86)
	#define _SCRATCH_SIZE   16
	#define _STACK_OFFSET   0
	#define _STACK_MIN      0
#elif defined(_MIPS_) || defined(_MIPS16_) || defined(MIPSII) || defined(MIPSII_FP) || defined(MIPSIV) || defined(MIPSIV_FP)
	#define _ALIGN_DOUBLES  8
	#define _SCRATCH_SIZE   0
	#define _STACK_OFFSET   0
	#define _STACK_MIN      32      // 4 32-bit registers
#elif defined(_SH3_) || defined(_SH4_)
	#define _ALIGN_DOUBLES  4	
	#define _SCRATCH_SIZE   0
	#define _STACK_OFFSET   0
	#define _STACK_MIN      32      // 4 32-bit registers
#elif defined(_ARM_) // applies to all variants of ARM
	#define _ALIGN_DOUBLES  4
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
	#define _AfxDispatchCall helper__AfxDispatchCall
	#define AFX_PMSG DWORD // because cefrtl.dll doesn't know about AFX_PMSG
#else
	#pragma message("Warning: no _AfxDispatchCall information set for this CPU.")
#endif
#else // _WIN32_WCE
#ifdef _X86_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY
#define _STACK_INT      int
#define _STACK_LONG     long
#define _STACK_FLOAT    float
#define _STACK_DOUBLE   double
#define _STACK_PTR      void*
#define _SCRATCH_SIZE   16
#define _STACK_OFFSET   0
#define _STACK_MIN      0
#endif

#ifdef _MIPS_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY
#define _ALIGN_DOUBLES  8
#define _STACK_INT      int
#define _STACK_LONG     long
#define _STACK_FLOAT    float
#define _STACK_DOUBLE   double
#define _STACK_PTR      void*
#define _SCRATCH_SIZE   0
#define _STACK_OFFSET   0
#define _STACK_MIN      32      // 4 32-bit registers
#endif

#ifdef _ALPHA_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY
#define _ALIGN_STACK    8
#define _STACK_INT      __int64
#define _STACK_LONG     __int64
#define _STACK_FLOAT    double
#define _STACK_DOUBLE   double
#define _STACK_PTR      __int64
#define _SCRATCH_SIZE   0
#define _STACK_OFFSET   48
#define _STACK_MIN      (48+32) // 6 32-bit registers, 32 bytes param space
#endif

#ifdef _PPC_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY
#define _ALIGN_DOUBLES  8
#define _STACK_INT      int
#define _STACK_LONG     long
#define _STACK_FLOAT    float
#define _STACK_DOUBLE   double
#define _STACK_PTR      void*
#define _SHADOW_DOUBLES 13
#define _SCRATCH_SIZE   (_SHADOW_DOUBLES*sizeof(double))
#define _STACK_OFFSET   0
#define _STACK_MIN      (64+32) // 8 32-bit registers, 32 bytes param space
#define _RETVAL_FIRST
#endif
#endif // _WIN32_WCE

/////////////////////////////////////////////////////////////////////////////
