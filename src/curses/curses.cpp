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
constexpr const char* reverse = "\033[7m";
constexpr const char* noreverse = "\033[27m";
constexpr const char* nobold = "\033[22m";
constexpr const char* nounderline = "\033[24m";
constexpr const char* color_black = "\033[1;30m";
constexpr const char* color_red = "\033[1;31m";
constexpr const char* color_reset = "\033[0m";
constexpr const char* bgcolor_red = "\033[1;41m";
constexpr const char* bgcolor_black = "\033[1;40m";
constexpr const char* bgcolor_reset = "\033[0m";

    // Background Black: \u001b[40m
    // Background Red: \u001b[41m
    // Background Green: \u001b[42m
    // Background Yellow: \u001b[43m
    // Background Blue: \u001b[44m
    // Background Magenta: \u001b[45m
    // Background Cyan: \u001b[46m
    // Background White: \u001b[47m


struct Color 
{
    short fg;
    short bg;
    short id;
};

constexpr int color_pairs = 16;
Color colors[color_pairs]; 
int index = 0;

}

WINDOW* stdscr = nullptr;

int move(int y, int x)
{
    printf("\033[%d;%dH", x, y);
    return 0;
}

int clear()
{
    printf("\033[H\033[J");
    return 0;
}

WINDOW* initscr()
{
    stdscr = &window;
    clear();
    move(0, 0);
    std::memset(&colors, -1, sizeof(colors));
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
    attroff(A_COLOR);
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
}

int mvprintw(int row, int column, const char *str, ...)
{
    move(column, row);
    va_list arg;
    va_start (arg, str);
    int done = __vfprintf_(writer, 1, str, arg, 0);
    va_end (arg);
    return done;
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
    if (attr & A_COLOR)
    {
        int id = attr & 0xF;
        auto color = colors[id];
        printf("color index: %d, fg: %d, bg: %d\n", id, color.fg, color.bg);
        switch (color.fg)
        {
            case COLOR_BLACK: printf(color_black); break;
            case COLOR_RED: printf(color_red); break;
        }

        switch (color.bg)
        {
            case COLOR_BLACK: printf(bgcolor_black); break;
            case COLOR_RED: printf(bgcolor_red); break;
        }

        return 0;
    }
    if (attr & A_BOLD)
    {
        printf(bold);
    }
    if (attr & A_UNDERLINE)
    {
        printf(underline);
    }
    if (attr & A_REVERSE)
    {
        printf(reverse);
    }
    return 0;
}

int attroff(int attr)
{
    if (attr & A_COLOR)
    {
        printf(color_reset);
        printf(bgcolor_reset);
        return 0;
    }

    if (attr & A_BOLD)
    {
        printf(nobold);
    }
    if (attr & A_UNDERLINE)
    {
        printf(nounderline);
    }
    if (attr & A_REVERSE)
    {
        printf(noreverse);
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

int start_color(void)
{
    return 0;
}

bool has_colors()
{
    return TRUE;
}


int init_pair(short id, short fg, short bg)
{
    if (index + 1 < color_pairs)
    {
        colors[index] = {
            .fg = fg,
            .bg = bg,
            .id = id
        };
        ++index;
    }
    return -1;
}

int COLOR_PAIR(int id)
{
    for (int i = 0; i < color_pairs; ++i)
    {
        if (colors[i].id == id)
        {
            return A_COLOR | i;
        }
    }
    return -1;
}
