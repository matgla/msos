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
#include <termios.h>
#include "msos/libc/printf.hpp"

#include "msos/usart_printer.hpp"

namespace
{
WINDOW window;
static UsartWriter writer;

constexpr const char* bold = "\033[1m";
constexpr const char* underline = "\033[4m";
constexpr const char* nobold = "\033[22m";
constexpr const char* nounderline = "\033[24m";


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
    echo();
    printf(nobold);
    fflush(stdout);
}

int getmaxx(WINDOW* window)
{
    return window->max_x;
}

int getmaxy(WINDOW* window)
{
    return window->max_y;
}

int printw(const char *str, ...)
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

int getch(void)
{
    char buffer;
    while (!read(STDOUT_FILENO, &buffer, 1));
    return buffer;
}

int noecho(void)
{
    struct termios tattr;

    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON | ECHO);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSANOW, &tattr);
    return 0;
}

int echo(void)
{
    struct termios tattr;

    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag |= ECHO;
    tcsetattr (STDIN_FILENO, TCSANOW, &tattr);
    return 0;
}


int attron(int attr)
{
    if (attr & A_BOLD)
    {
        printf(bold);
    }
    if (attr & A_UNDERLINE)
    {
        printf(underline);
    }
    return 0;
}

int attroff(int attr)
{
    if (attr & A_BOLD)
    {
        printf(nobold);
    }
    if (attr & A_UNDERLINE)
    {
        printf(nounderline);
    }
    return 0;
}

int raw()
{
    return 0;
}

int noraw()
{
    return 0;
}

int keypad(WINDOW* window, bool bf)
{
    window->key_translation = bf;
    return 0;
}

int getstr_(char* str, size_t size)
{
    return fgets(str, size, stdin) == nullptr ? ERR : OK;
}
