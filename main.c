#include <ncurses.h>
#include <stdlib.h>

//test

int main(){

    char* ch = NULL; 

    initscr();

    printw("SALUT");

    refresh();

    getstr(ch);

    printw("%s", ch);

    getch();

    endwin();

}