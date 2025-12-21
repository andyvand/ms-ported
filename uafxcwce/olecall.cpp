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

#ifdef AFX_OLE5_SEG
#pragma code_seg(AFX_OLE5_SEG)
#endif

// Note: because of the nature of these functions, it is not possible
//  to create a 'C' or 'C++' version of them.  These functions are used
//  for the lowest level of the OLE IDispatch implementation, and need
//  to be ported to each supported platform.

extern "C" {

#if defined(_WIN32_WCE)
// WinCE: AFXAPI uses __stdcall, which maps to __cdecl under WinCE 
//        (except for _WIN32_WCE_EMULATION)
__declspec(naked) void AFXAPI
_AfxDispatchCall(AFX_PMSG /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)
{
#if defined(_WIN32_WCE_CEPC) || defined(_WIN32_WCE_EMULATION) || defined(_M_IX86)
	_asm
	{
		pop     edx         // edx = return address
		pop     eax         // eax = pfn
		pop     ecx         // ecx = pArgs
		add     ecx,[esp]   // ecx += nSizeArgs (=scratch area)
		mov     [ecx],edx   // scratch[0] = return address
		sub     ecx,[esp]   // ecx = pArgs (again)
		mov     esp,ecx     // esp = pArgs (usually already correct)
		pop     ecx         // ecx = this pointer (the CCmdTarget*)
		call    eax         // call member function
		ret                 // esp[0] should = scratch[0] = return address
	}
#elif defined(_MIPS_) && defined(_MIPS16_)
	__emit(0x65bd);		// move   sp,a1
	__emit(0x6744);		// move   v0,a0
	__emit(0x9400);		// lw     a0,0(sp)
	__emit(0x9501);		// lw     a1,4(sp)
	__emit(0x9602);		// lw     a2,8(sp)
	__emit(0xea00);		// jr     v0
	__emit(0x9703);		// lw     a3,0xC(sp)
#elif defined(_SH4_)
    // We need to deal with the function arguments, packaged on the stack,
    // in a special way. So AfxDispatchCall will not be used for these
    // processors. Rather, a special function will be called instead of
    // AfxDispatchCall. The reason for special handling is because of the
    // way floating point values are passed on the stack. At present,
    // none of the other architectures bother about floating point values
    // at all. As a result, for these processors, we need to generate a
    // thunk on the fly, encode and execute it dynamically. At present,
    // for other architectures, we are statically creating a thunk that
    // will work well for int arguments.
    __asm("nop\n");
#elif defined(MIPSII_FP) || defined(MIPSIV_FP)
    // nothing, the thunk builder will take care of things for us
#elif defined(MIPSIV)
    __asm("addiu     %sp,%a1,0x0");      // sp = pArgs
    __asm("addiu     %t6,%a0,0x0");      // t6 = pfn (save it)
    __asm("ld        %a0,0x0(%sp)");     // a0 = param0
    __asm("ld        %a1,0x8(%sp)");     // a1 = param1
    __asm("ld        %a2,0x10(%sp)");    // a2 = param2
    __asm("ld        %a3,0x18(%sp)");    // a3 = param3
    __asm("j         %t6");              // ip = pfn (jump to target function)
#elif defined(_MIPS_) || defined(MIPSII)
	__asm("addiu %sp,%a1,0x0");      // sp = pArgs
	__asm("addiu %t6,%a0,0x0");      // t6 = pfn (save it)
	__asm("lw    %a0,0x0(%sp)");     // a0 = param0
	__asm("lw    %a1,0x4(%sp)");     // a1 = param1
	__asm("lw    %a2,0x8(%sp)");     // a2 = param2
	__asm("lw    %a3,0xc(%sp)");     // a3 = param3
	__asm("j     %t6");              // ip = pfn (jump to target function)
#elif defined(_SH3_)
	__asm("mov   R5, sp");  		// sp = pStack
	__asm("mov   R4, R1");          // R1 = pfn (save it)
	__asm("mov.l @sp, R4");         // R4 = arg1 ('this' pointer)
	__asm("mov   #4, R0");          // R5 = arg2
	__asm("mov.l @(R0,sp), R5");
	__asm("mov   #8, R0");          // R6 = arg3
	__asm("mov.l @(R0,sp), R6");
	__asm("mov  #12, R0");          // R7 = arg4
	// __asm("mov.l @(R0,sp), R7"); moved to delay slot
	__asm("jmp  @R1\n"              // jump to target function
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
#elif defined(_WIN32_WCE_CEF)
	// Use helper__AfxDispatchCall in CEFRTL.DLL (see dispimpl.h)
#else
	#error No _AfxDispatchCall implementation found for this CPU.
#endif
}
#else // _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// Intel 386 version

#ifdef _X86_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY

__declspec(naked) void AFXAPI
_AfxDispatchCall(AFX_PMSG /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)
{
	_asm
	{
		pop     edx         // edx = return address
		pop     eax         // eax = pfn
		pop     ecx         // ecx = pArgs
		add     ecx,[esp]   // ecx += nSizeArgs (=scratch area)
		mov     [ecx],edx   // scratch[0] = return address
		sub     ecx,[esp]   // ecx = pArgs (again)
		mov     esp,ecx     // esp = pArgs (usually already correct)
		pop     ecx         // ecx = this pointer (the CCmdTarget*)
		call    eax         // call member function
		ret                 // esp[0] should = scratch[0] = return address
	}
}
#endif // _X86_

/////////////////////////////////////////////////////////////////////////////
// MIPS R4000 version

#ifdef _MIPS_ // DESKTOP MFC IMPLEMENTATION--DO NOT MODIFY

extern "C" void _asm(char *, ...);
void AFXAPI
_AfxDispatchCall(AFX_PMSG /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)
{
	_asm("addiu     %sp,%a1,0x0");      // sp = pArgs
	_asm("addiu     %t6,%a0,0x0");      // t6 = pfn (save it)
	_asm("lw        %a0,0x0(%sp)");     // a0 = param0
	_asm("lw        %a1,0x4(%sp)");     // a1 = param1
	_asm("lw        %a2,0x8(%sp)");     // a2 = param2
	_asm("lw        %a3,0xc(%sp)");     // a3 = param3
	_asm("j         %t6");              // ip = pfn (jump to target function)
}

#endif // _MIPS_

/////////////////////////////////////////////////////////////////////////////
// DEC Alpha AXP version

#ifdef _ALPHA_

// Note: ALPHA version is in src\alpha\olecall_.s

// The ALPHA compiler does not support inline assembly, so it
//  must be build separately with the ASAXP assembler.

#endif // _ALPHA_

#endif // _WIN32_WCE
} // end extern "C" block

/////////////////////////////////////////////////////////////////////////////
