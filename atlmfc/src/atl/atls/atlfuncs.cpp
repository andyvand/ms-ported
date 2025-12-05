// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the	
// Active Template Library product.

#include "stdafx.h"

// C4740 - flow in or out of inline asm code suppresses global optimization
#pragma warning( disable: 4740 )

namespace  ATL
{

/////////////////////////////////////////////////////////////////////////////
// ComStdThunk

#ifdef __i386__

extern "C"
void __stdcall CComStdCallThunkHelper()
{
	__asm("mov 4(%esp), %eax\n" // get pThunk
          "mov 4(%eax), %edx\n" // get the pThunk->pThis
          "mov %edx, 4(%esp)\n"	// replace pThunk with pThis
          "mov 8(%eax), %eax\n"	// get pThunk->pfn
          "jmp *(%eax)\n");		// jump pfn
}

#endif

}
