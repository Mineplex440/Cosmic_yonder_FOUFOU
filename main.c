#include "player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#define ENTER 10
#define NB_MOVE 4


int main(){

    int ch = 0; 
    setlocale(LC_CTYPE,"");
    
    initscr();
    keypad(stdscr, TRUE);
    wtimeout(stdscr, 100);
    noecho();


    int winposx = 5;
    int winposy = 2;
    int winlength = 30;
    int winwidth = 100;



    WINDOW * win = NULL;

    win = subwin(stdscr,winlength, winwidth, winposy, winposx);

    if (win == NULL){
        printf("allocation problem, try to modify the size of your terminal\n");
        exit(1);
    }


    showMenu(win, winlength, winwidth, winposx, winposy);


    
    endwin();

    return 0;
}
