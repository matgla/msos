// This file is part of MSOS project.
// Copyright (C) 2020 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

.syntax unified

.global _fork_p
_fork_p:
    push {r1, r2, r3, r4, r5, lr}
    ldr r5, =registers
    mov r4, r0
    ldr r2, =syscall_return_code
    mov r0, #3
    svc 0
    wfi
    isb
    dsb
    ldr r0, =syscall_return_code
    pop {r1, r2, r3, r4, r5, pc}

.global _fork
_fork:
    ldr r0, =is_pendsv_blocked
    push {r1, lr}
    mov r1, #1
    str r1, [r0]

    ldr r0, =registers
    pop {r1}
    stmia r0, {r1-r12, lr}
    mrs r0, psp
    push {r1}
    mov r1, lr
    bl _fork_p

    push {r0}
    ldr r0, =is_pendsv_blocked
    mov r1, #0
    str r1, [r0]
    pop {r0}
    pop {r1, pc}
