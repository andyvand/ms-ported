// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1999 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


// WinCE: This file is a workaround for a problem in the AlderFE implementation of longjmp.

#include "kxmips.h"

#define JbIntS0 0x30
#define JbIntS1 0x34
#define JbIntS2 0x38
#define JbIntS3 0x3c
#define JbIntS4 0x40
#define JbIntS5 0x44
#define JbIntS6 0x48
#define JbIntS7 0x4c
#define JbIntS8 0x50
#define JbIntSp 0x54
#define JbType 0x58
#define JbFir 0x5c

        LEAF_ENTRY(longjmp)
.set noreorder
	 bne     zero a1,10f
	 nop
	 li      a1,1
10:	 move    v0,a1                   // set return value
     lw      s0,JbIntS0(a0)          // restore integer registers s0 - s8
     lw      s1,JbIntS1(a0)          //
     lw      s2,JbIntS2(a0)          //
     lw      s3,JbIntS3(a0)          //
     lw      s4,JbIntS4(a0)          //
     lw      s5,JbIntS5(a0)          //
     lw      s6,JbIntS6(a0)          //
     lw      s7,JbIntS7(a0)          //
     lw      s8,JbIntS8(a0)          //
     lw      a1,JbFir(a0)            // get setjmp return address
     j       a1                      // jump back to setjmp site
     lw      sp,JbIntSp(a0)          // restore stack pointer

        .end    longjmp

