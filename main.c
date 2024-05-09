#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>

typedef struct{
    int length;
    int width;
    int nbevent;
    int nbdoor;
}Room;

void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth){
    wclear(win);
    mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)-10, "匚ㄖ丂爪丨匚  ㄚㄖ几ᗪ乇尺\n");
    int ch = 0;
    while(ch != 10){

        box(win, 0,0);

        wrefresh(win);
        ch = getch();
        wclear(win);
    }

    wclear(stdscr);

}

void loadSave(WINDOW * win){
    wclear(win);
}

void setting(WINDOW * win){
    wclear(win);
}

void printMenu(WINDOW *win,int x, int y, int beg, int space){

    mvwprintw(win, y, x+2, "匚ㄖ丂爪丨匚\n");

    mvwprintw(win, y+1, x+2, "ㄚㄖ几ᗪ乇尺\n");

    mvwprintw(win, y+beg, x,"PLAY\n");

    mvwprintw(win, y+(beg+(space)), x,"SAVES\n");

    mvwprintw(win, y+(beg+(space*2)),x,"PARAMETRES\n");

    mvwprintw(win, y+(beg+(space*3)), x,"QUIT\n");
}


void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space){

    for(int i = 0; i<winlength; i++){
    
    mvwprintw(win, posy, posx, "-->");
        
    printMenu(win, x, y, beg, space);

    box(win, 0,0);

    for(int j = 1; j<winlength-i-1; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-");
        }
    }

    wrefresh(win);
    usleep(30000);
    wclear(win);
    }
    
}

void quitAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space){

    for(int i = 0; i<winlength; i++){
    
    mvwprintw(win, posy, posx, "-->");
        
    printMenu(win, x, y, beg, space);

    box(win, 0,0);

    for(int j = 1; j<i; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-");
        }
    }

    wrefresh(win);
    usleep(30000);
    wclear(win);
    }
}


void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy){

    

    int chr = 0;

    int space = 4;
    int beg = 5;

    int x = (winwidth/2)-10;
    int y = 2;



    int posx, posy;
    posx = (winwidth/3)-5;
    posy = beg+y;
    
    startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);

    wclear(win);

    while(posy != y+(beg+(3*space)) || chr != 10 ){

        mvwprintw(win, posy, posx, "-->");
        
        printMenu(win, x, y, beg, space);

        
        box(win, 0,0);
        
        wrefresh(win);
        chr = getch();
        wclear(win);

        if (chr == 10 && posy == y+beg){
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            startagame(win, winposx, winposy, winlength, winwidth);
        }
        if (chr == 10 && posy == y+(beg+(space))){
            loadSave(win);
        }
        if (chr == 10 && posy == y+(beg+(space*2))){
            setting(win);
        }


        if(chr == KEY_UP && posy > y+beg){
            posy-= space;
        }
        if (chr == KEY_DOWN && posy < y+(beg+(space*3))){
            posy+= space;
        }


    }

    quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);

}


//test

int main(){

    int ch = 0; 
    setlocale(LC_CTYPE,"");
    initscr();
    keypad(stdscr, TRUE);
    noecho();


    int winposx = 5;
    int winposy = 2;
    int winlength = 30;
    int winwidth = 100;



    WINDOW * win = NULL;

    win = subwin(stdscr,winlength, winwidth, winposy, winposx);

    if (win == NULL){
        printf("allocation problem, try to modify the size of your terminal");
        exit(1);
    }


    


    //printw("TEST");
    

    //ch = getch();

    //mvprintw(1, 0,"%d", ch);

    //sleep(1);

    //refresh();

    showMenu(win, winlength, winwidth, winposx, winposy);


    
    //getch();
    
    endwin();

    return 0;
}