// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

.text

.globl _AfxParseCall

// void AFXISAPI _AfxParseCall( AFX_PMSG pfn, void* pStack, UINT nSizeArgs )

_AfxParseCall:
	// at this point RCX contains value of pfn, RDX contains value of pArgs 
	// and R8 contains value of nSizeArgs.
	
	// get the return address
	mov (%rsp), %rax

	// save the return address
	mov %rax, 8(%rdx)

	// set the new stack pointer
	lea 8(%rdx), %rsp

	// save the pfn
	mov %rcx, %rax

	// set the first four float/double arguments
	movsd (%rdx), %xmm0
	movsd 8(%rdx), %xmm1
	movsd 16(%rdx), %xmm2
	movsd 24(%rdx), %xmm3

	// set the first four integer arguments [except for RDX]
	mov (%rdx), %rcx
	mov 16(%rdx), %r8
	mov 24(%rdx), %r9

	// Finally load up RDX
	mov 8(%rdx), %rdx

	// jump to the function
	jmp *(%rax)
    ret
