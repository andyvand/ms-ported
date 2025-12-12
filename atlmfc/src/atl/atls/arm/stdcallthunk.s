// This is a part of the Active Template Library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

.globl CComStdCallThunkHelper
CComStdCallThunkHelper:
        ldr     r12, [r0, 8]
        ldr     r0, [r0, 4]
        bx      r12
