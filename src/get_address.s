.thumb 
.syntax unified 
.arch armv7-m 

.equ BKP_BASE,  0x40006c00

.global get_address 
get_address: 
    ldr r3, =BKP_BASE 
    ldr r0, [r3, #8]
    lsl r0, r0, #16 
    ldr r1, [r3, #12] 
    orr r0, r1 
    bx lr 

 .L3:
    .word 1073769472 
