// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

.globl _AfxDispatchCall
.globl CopyStackExtraSpace
.globl CopyStackEnd
.globl CopyStackLoop
.globl CallFunction

.text

//_AfxDispatchCall(AFX_PMSG /*pfn*/, void* /*pArgs*/, UINT /*nSizeArgs*/)

_AfxDispatchCall:
	// at this point RCX contains value of pfn, RDX contains value of pArgs 
	// and R8 contains value of nSizeArgs.
	
#ifdef _M_ARM64EC
	// Assume that on arm64EC, that pArgs/RDX will not be in the current frame, so we can't
	// manipulate the stack to set up our tail call.  It would be invalid to reinterpret pArgs
	// to be a stack, because the caller isn't done with the stack.
	// So, we have to make a new frame and copy stack data to it.

	// Save RBP and use it as the new frame pointer
	push %rbp
	mov %rsp, %rbp

	// do not move the stack pointer back to RDX, because pArgs is at a location behind a thunk.
	// instead, push everything between (pArgs + nSizeArgs) and pArgs to be on top of the stack.

	// check if the stack needs shadow space.
	// RAX will hold the required shadow space.
	// at least 0x20 of space must be provided to the callee.
	mov 0x20, %rax
	sub %r8, %rax
	jge CopyStackExtraSpace

	// check if the stack will be aligned after pushing nSizeArgs of data, but not the return address.
	// alignment is guaranteed if shadow space was needed in the above check.
	mov %r8, %rax
	add %rsp, %rax
	and 0x8, %rax

	// if so, 0 bytes of shadow space is needed to ensure eventual 16-byte unalignment
	// (the return address unaligns it inside the CALL, and unalignment should be the final result)
	// if not, 8 bytes of shadow space is needed to ensure eventual 16-byte unalignment
	// either way, RAX now has the correct amount of space stored in it.

CopyStackExtraSpace:
	sub %rax, %rsp

	// nSizeArgs (R8) isn't used for anything else now, so repurpose it as a counter
	add %rdx, %r8

	// adjust to point at the last argument, not past it
	sub 0x8, %r8

CopyStackLoop:
	// copy the start of the caller's stack, up to RDX

	// check how much we have pushed
	cmp %rdx, %r8

	// exit if if we have pushed everything up to and including RDX
	jb CopyStackEnd

	// if not, push next element
	push (%r8)

	// increment and continue loop
	sub 0x8, %r8
	jmp CopyStackLoop

CopyStackEnd:
#else
	// get the return address
	mov (%rsp), %rax

	// save the return address
	mov %rax, 8(%rdx)

	// set the new stack pointer
	lea 8(%rdx), %rsp
#endif

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

#ifdef _M_ARM64EC
CallFunction:
	// Finally load up RDX and call the function
	mov 8(%rdx), %rdx
	call *(%rax)

	// discard the current frame
	mov %rbp, %rsp
	pop %rbp

	// above CALL does return
	ret
#else

	// Or, finally load up RDX and jump to the function
	mov 8(%rdx), %rdx
	jmp *(%rax)
    ret

	// above JMP does not return
	// ret
#endif
