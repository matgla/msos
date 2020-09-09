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

extern "C"
{
    struct WINDOW
    {
        int max_x;
        int max_y;
    };

    WINDOW* initscr(void);
    void endwin(void);
    int getmaxy(WINDOW* window);
    int getmaxx(WINDOW* window);

    #define getmaxyx(win,y,x) (y = getmaxy(win), x = getmaxx(win))

    int printw(char *, ...);
    int refresh(void);

    extern WINDOW* stdscr;
}
