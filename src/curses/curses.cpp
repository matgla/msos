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

#include <cstddef>
#include <cstdio>
#include <unistd.h>
#include <stdarg.h>

#include "curses.h"

#include "sys/ioctl.h"
#include "msos/libc/printf.hpp"

#include "msos/usart_printer.hpp"

namespace 
{
WINDOW window;
static UsartWriter writer;
}

WINDOW* stdscr = nullptr;

void clear_screen()
{
    printf("\033[H\033[J");
}

void move_cursor(int x, int y)
{
    printf("\033[%d;%dH", y, x);
}

WINDOW* initscr() 
{
    stdscr = &window;
    clear_screen();
    move_cursor(0, 0);
    fflush(stdout);
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    window.max_x = w.ws_col;
    window.max_y = w.ws_row;
    return &window;
}

void endwin()
{

}

int getmaxx(WINDOW* window)
{
    return window->max_x;
}

int getmaxy(WINDOW* window)
{
    return window->max_y;
}

int printw(char *str, ...)
{
    va_list arg;
    va_start (arg, str);
    int done = __vfprintf_(writer, 1, str, arg, 0);
    va_end (arg);
    return done;
    return 0;
}

int refresh(void)
{
    return 0;
}