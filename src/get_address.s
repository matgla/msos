.thumb
.syntax unified 
.arch armv7-m


.global get_address 
.thumb_func
.align 1
.type get_address, %function
get_address:
    push    {r9, lr}
    push    {r0, r1}
    mov     r0, #0x40000000
    add     r0, #0x6c00
    ldr     r1, [r0, #8]
    lsl     r1, r1, #16
    ldr     r0, [r0, #12]
    orr     r1, r0 
    mov     r0, pc
    bx      r1
    mov     r9, r0
    pop     {r0, r1}
#    bl      _Z11object_mainv_dl_original
    pop     {r9, pc}

