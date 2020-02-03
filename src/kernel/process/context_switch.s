// This file is part of MSOS project. 
// Copyright (C) 2019 Mateusz Stadnik
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

//.global get_program_counter 
//get_program_counter:
//    mov r0, pc 
//    bx lr

.global context_switch 
context_switch: 
    mrs r1, psp 
    isb
    stmdb r1!, {r4 - r11, lr} 
    ldmia r0!, {r4 - r11, lr}
    msr psp, r0 
    dsb
    isb
    bx lr 

.global get_psp 
get_psp: 
    mrs r0, psp 
    bx lr 
