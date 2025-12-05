// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////

// This function is separated from afxmem.cpp so that ASAN can take 
// precedence over it smoothly.
// If ASAN implementation is ever modified such that ASAN's replacement would
// not be appropriate we should once again induce a linker error.

// Currently ASAN has the same behavior as below function. If this function
// changes then ASAN precedence may need to be revisited

void __cdecl operator delete[](void* p)
{
	::operator delete(p);
}

