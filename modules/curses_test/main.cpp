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

#include <cstdio>
#include <unistd.h>
#include <curses.h>

int main()
{
    printf("Curses test\n");
    int xmax, ymax;

    if (initscr() == NULL)
    {
        printf("Error: initscr()\n");
    }

    getmaxyx(stdscr,ymax,xmax);
    printw("Hello!\n");
    printw("Liczba wierszy okna = %d\n",ymax);
    printw("Liczba kolumn okna = %d\n",xmax);
    refresh();
    sleep(2);
    endwin();
}

