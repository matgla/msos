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

.global switch_to_next_task
switch_to_next_task:
    bl get_next_task
    ldmia r0!, {r4-r11, lr}
    msr psp, r0
    bx lr

.global store_and_switch_to_next_task
store_and_switch_to_next_task:
    mrs r0, psp
    stmdb r0!, {r4-r11, lr}
    bl update_stack_pointer
    b switch_to_next_task
