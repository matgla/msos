.thumb
.syntax unified
.arch armv7-m


.global call_external
.thumb_func
.align 1
.type call_external, %function
call_external:
    mov     pc, r0
