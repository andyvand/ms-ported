.text

.globl _AfxDispatchCallNested
.globl _AfxDispatchCall

_AfxDispatchCall:
        mov     r12, r0
        subs    r2, r2, 16
        ldrd    r1, r3, [r0, 0]
        ldrd    r2, r3, [r3, 4]
        ldrd    r4, r5, [r3, 8]
        ldrd    r6, r7, [r3, 12]
        bgt     _AfxDispatchCallNested
        ldrd    r6, r7, [r1, 12]
        ldrd    r4, r5, [r1, 8]
        ldrd    r2, r3, [r1, 4]
        ldrd    r0, r1, [r1, 0]
        bx      r12

_AfxDispatchCallNested:
        sub     r2, sp, r3
        and     r8, r8, 0xfffffff0
        mov     r8, sp
        add     r0, r1, 4

loop:
        subs    r2, r2, 4
        ldr     r3, [r0, r2]
        str     r3, [sp, r2]
        bgt     loop

        ldrd     r6, r7, [r1, 12]
        ldrd    r4, r5, [r1, 8]
        ldrd    r3, r2, [r1, 4]
        ldrd    r0, r1, [r1, 0]
        
        blx     r12

        mov     fp, sp
