.text

.globl _AfxDispatchCallNested
.globl _AfxDispatchCall

_AfxDispatchCall:
        mov     x16, x0
        subs    x2, x2, 64
        ldp     d1, d3, [x0, 0]
        ldp     d2, d3, [x3, 16]
        ldp     d4, d5, [x3, 32]
        ldp     d6, d7, [x3, 48]
        bgt     _AfxDispatchCallNested
        ldp     x6, x7, [x1, 48]
        ldp     x4, x5, [x1, 32]
        ldp     x2, x3, [x1, 16]
        ldp     x0, x1, [x1, 0]
        br      x16

_AfxDispatchCallNested:
        sub     x2, sp, x3
        and     x8, x8, 0xfffffffffffffff0
        mov     x8, sp
        add     x0, x1, 16

loop:
        subs    x2, x2, 8
        ldr     x3, [x0, x2]
        str     x3, [sp, x2]
        bgt     loop

        ldp     x6, x7, [x1, 48]
        ldp     x4, x5, [x1, 32]
        ldp     x3, x2, [x1, 16]
        ldp     x0, x1, [x1, 0]
        
        blr     x16

        mov     fp, sp

        ret
