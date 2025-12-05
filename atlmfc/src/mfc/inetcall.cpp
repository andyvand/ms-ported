// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#ifdef _AFXDLL
#include "stdafx.h"
#endif

// Note: because of the nature of these functions, it is not possible
//  to create a 'C' or 'C++' version of them.  These functions are used
//  for the lowest level of the OLE IDispatch implementation, and need
//  to be ported to each supported platform.

extern "C" {

/////////////////////////////////////////////////////////////////////////////
// Intel 386 version

#ifdef __i386__
#ifndef UINT
#define UINT unsigned int
#endif

#ifndef AFXISAPI
#define AFXISAPI __stdcall
#endif

#ifndef AFX_PISAPICMD
#define AFX_PISAPICMD void *
#endif

void AFXISAPI _AfxParseCall(AFX_PISAPICMD /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)
{
	__asm("pop %edx\n"         // edx = return address
          "pop %eax\n"         // eax = pfn
          "pop %ecx\n"         // ecx = pArgs
          "add (%esp), %ecx\n" // ecx += nSizeArgs (=scratch area)
          "mov %edx, (%ecx)\n" // scratch[0] = return address
          "sub (%esp), %ecx\n" // ecx = pArgs (again)
          "mov %ecx, %esp\n"   // esp = pArgs (usually already correct)
          "pop %ecx\n"         // ecx = this pointer (the CCmdTarget*)
          "call *(%eax)\n"     // call member function
          "ret\n");            // esp[0] should = scratch[0] = return address
}
#endif // _X86_

/////////////////////////////////////////////////////////////////////////////
// AMD 64 version

#ifdef _AMD64_

// Note: AMD64 version is in src\amd64\inetcal_.s

// The AMD64 compiler does not support inline assembly, so it
//  must be built separately with the MASM64 assembler.


#endif // _AMD64_

} // end extern "C" block

/////////////////////////////////////////////////////////////////////////////
