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
.arch armv7-m
.thumb

.global semaphore_wait
semaphore_wait:
    push {r1, r2, lr}
    b wait_loop

wait_loop:
    ldrex r1, [r0]
    cmp r1, #0
    IT EQ
    bleq block
    sub r1, #1
    strex r2, r1, [r0]
    cmp r2, #0
    bne wait_loop
    dmb
    pop {r1, r2, pc}

.global semaphore_post
semaphore_post:
    push {r1, r2, lr}
    b post_loop

post_loop:
    ldrex r1, [r0]
    add r1, #1
    strex r2, r1, [r0]
    cmp r2, #0
    bne post_loop
    cmp r0, #1
    dmb
    IT GE
    blge unblock
    pop {r1, r2, pc}
