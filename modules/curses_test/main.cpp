#include <curses.h>


int main()
{
    //start
    initscr();
    printw( "Przycisnij przycisk...\n" );
    noecho();
    //rozpoczynamy wyswietlanie menu!
    const char txt1[] = "Tekst";
    const char txt2[] = "Tekst2";
    const char txt3[] = "Wyjscie";
    int ktory = 1;
    int znak;
    const short int min_wybor = 1;
    const short int max_wybor = 3;
    do
    {
        //pobieranie znaku
        znak = getch();
        clear();
        if( znak == 'a' && ktory != min_wybor )
        {
            ktory--;
        }
        else if( znak == 'z' && ktory != max_wybor )
        {
            ktory++;
        }
        //reakcja          
        if( znak == 10 )
        {
            switch( ktory )
            {
            case 1:
                mvprintw( 0, 0, "Opcja 1" );
                break;
            case 2:
                mvprintw( 0, 0, "Opcja 2" );
                break;
            }
        }
        //wyswietlanie
        switch( ktory )
        {
        case 1:
            mvprintw( 6, 5, txt2 );
            mvprintw( 7, 5, txt3 );
            attron( A_REVERSE );
            mvprintw( 5, 5, txt1 );
            break;
           
        case 2:
            mvprintw( 5, 5, txt1 );
            mvprintw( 7, 5, txt3 );
            attron( A_REVERSE );
            mvprintw( 6, 5, txt2 );
            break;
           
        case 3:
            mvprintw( 5, 5, txt1 );
            mvprintw( 6, 5, txt2 );
            attron( A_REVERSE );
            mvprintw( 7, 5, txt3 );
            break;
        }
        attroff( A_REVERSE );
    } while( ktory != 3 || znak != 10 );
   
    move( 9, 0 );
    printw( "Koniec programu, przycisnij przycisk..." );
    getch();
    endwin();
   
}