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

#pragma once

#if defined(__cplusplus)
extern "C"
{
#else 
    typedef int bool;
#endif // __cplusplus

    typedef unsigned int size_t;

    struct WINDOW
    {
        int max_x;
        int max_y;
        bool key_translation;
    };

    WINDOW* initscr(void);
    void endwin(void);
    int getmaxy(WINDOW* window);
    int getmaxx(WINDOW* window);

    #define getmaxyx(win,y,x) (y = getmaxy(win), x = getmaxx(win))

    int printw(const char *, ...);
    int mvprintw(int row, int colums, const char *, ...);
    int refresh(void);
    int getch(void);
    int noecho(void);
    int echo(void);
    int attron(int);
    int attroff(int);
    int raw(void);
    int noraw(void);
    int clear(void);
    int move(int, int);

    int keypad(WINDOW *, bool);
    int getstr_(char *, unsigned int);

    int start_color(void);
    bool has_colors();
    int init_pair(short, short, short);
    int COLOR_PAIR(int n);

    extern WINDOW* stdscr;

    #define getstr(str) getstr_(str, sizeof(str));

    #define A_BOLD 0x01
    #define A_UNDERLINE 0x02
    #define A_REVERSE 0x04
    #define A_COLOR 0x10

    #define COLOR_RED 1
    #define COLOR_BLACK 2

    #define FALSE 0
    #define TRUE 1

    #define OK 0
    #define ERR -1
#if defined(__cplusplus)
}
#endif // __cplusplus
