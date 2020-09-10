#include <curses.h>

int main()
{
    initscr(); //Start
    if( has_colors() == TRUE ) //1
    {
        start_color(); //2
        init_pair( 1, COLOR_RED, COLOR_BLACK ); //3
        attron( COLOR_PAIR( 1 ) ); //4
        printw( "Kolorowy napis. ;P" );
        attroff( COLOR_PAIR( 1 ) ); //Wyłączenie koloru tekstu
    }
    else
    {
        printw( "Twoja Konsolka nie obsluguje kolorow. :/ " );
    }
    getch();
    endwin(); //koniec
    return 0;
}
