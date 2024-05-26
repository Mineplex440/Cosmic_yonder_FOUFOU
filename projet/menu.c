#include "game.h"
#include "menu.h"
#include "create.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10
#define NB_MOVE 4

void quitAnim(WINDOW * win, int winlength, int winwidth){
    /*Function which display a quit animation on the screen,
    she take 3 argument a window her length and width */

    box(win, 0, 0);


    for(int j = 1; j<winlength-1; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-");
        }
    

        wrefresh(win);
        usleep(50000);
    }    
}

void printItem(int winwidth, int winlength, WINDOW * win, int * stop, Item i){

    int x = 0;

    WINDOW * item = NULL;

    item = subwin(win, 9, 40, (winlength/2)-2, (winwidth/2)-10);

    if(item == NULL){
        printf("Error with printItem");
        exit(13);
    }

    int t = time(NULL);


    wclear(item);

    mvwprintw(item, 3, 7, "You picked up a %s !", i.name);

    box(item, 0, 0);

    wrefresh(item);

    sleep(1);

    wclear(item);

    int te = time(NULL);
    *stop += (te-t);
}


void printinv(int width, int length, WINDOW * win, WINDOW * showstat,Item * inventory, int y, int x, int * equipedsword, int * equipedshield){

    /*Function that will open few window to display the player's inventory and the stat of the  item, She take 9 argument the width and length of a WINDOW win, another WINDOW who show the stat, the player inventory, an x and y coordonate, two variables to know if a sword and a shield are equiped  */

    box(win,0,0);
    box(showstat, 0, 0);

    for(int i = 0; i<10; i++){
            if(inventory[i].type == 0){
                mvwprintw(win, 3+(7*(i/5)) , 6 + ((i%5)*9), "🔫");
            }
            else if(inventory[i].type == 1){
                mvwprintw(win, 3+(7*(i/5)) , 6 + ((i%5)*9), "🛡");
            }
            else if(inventory[i].type == 2){
                mvwprintw(win, 3+(7*(i/5)) , 6 + ((i%5)*9), "🍹");
            }
            else if(inventory[i].type == 3){
                mvwprintw(win, 3+(7*(i/5)) , 6 + ((i%5)*9), "💡");
            }
            else{
                mvwprintw(win, 3+(7*(i/5)) , 6 + ((i%5)*9), "🚫");
            } 
    }

    if(*equipedsword != -1){
        mvwprintw(win, 3+(7*(*equipedsword/5)), 6 + (9*(*equipedsword%5)) + 2, "💚");
    }
    if(*equipedshield != -1){
        mvwprintw(win, 3+(7*(*equipedshield/5)), 6 + (9*(*equipedshield%5)) + 2, "💚");
    }

    mvwprintw(win,(3+(y*7)) , 3+(x*9), "->");

    if(inventory[(y*5)+(x)].type == 0 || inventory[(y*5)+(x)].type == 3){
        mvwprintw(showstat, 1, 3, "name : %s", inventory[(y*5)+(x)].name);
        mvwprintw(showstat, 3, 3, "atk : + %d", inventory[(y*5)+(x)].buff);
    }
    else if(inventory[(y*5)+(x)].type == 1){
        mvwprintw(showstat, 1, 3, "name : %s", inventory[(y*5)+(x)].name);
        mvwprintw(showstat, 3, 3, "def : + %d", inventory[(y*5)+(x)].buff);
    }
    else if(inventory[(y*5)+(x)].type == 2){
        mvwprintw(showstat, 1, 3, "name : %s", inventory[(y*5)+(x)].name);
        if(inventory[(y*5)+(x)].typebuff == 1){
            mvwprintw(showstat, 3, 3, "buff : swift");
        }
        else if(inventory[(y*5)+(x)].typebuff == 2){
            mvwprintw(showstat, 3, 3, "buff : instant heal");
        }
        mvwprintw(showstat, 5, 3, "buff : + %d", inventory[(y*5)+(x)].buff);
    }

    

}

void interactwithobject(WINDOW * win, int * equipedsword, int * equipedshield, int y, int x, Player * play, Item * inventory){
	/*Function that allows the player to equip or not equip items in the inventory, 
She take 7 arguments a WINDOW win, two variables to know if a sword and a shield    are equiped, a coordonate y and x for positions in the inventory display, a Player * player  and his inventory Item * inventory  */


    int posy = 0;

    int ch = 0;


    if((y*5)+(x) == *equipedsword){
        while(ch != ENTER){
            wclear(win);
            
            mvwprintw(win, 1, 4, "REMOVE");
            mvwprintw(win, 3, 4, "DROP");
            mvwprintw(win, 5, 4, "RETURN");
            mvwprintw(win, (2*posy)+1, 1, "->");

            box(win, 0, 0);

            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 2){
                posy++;
            }

            if(ch == ENTER && posy == 0){
                play->playerStat.atck -= (inventory+((y*5)+(x)))->buff;
                *equipedsword = -1;
            }

            if(ch == ENTER && posy == 1){
                play->playerStat.atck -= (inventory+((y*5)+(x)))->buff;
                (inventory+((y*5)+(x)))->type = -1;
                (inventory+((y*5)+(x)))->buff = -1;
                (inventory+((y*5)+(x)))->typebuff = -1;
                (inventory+((y*5)+(x)))->name[0] = '\0';

                *equipedsword = -1;
            }

        }
    }
    else if((y*5)+(x) == *equipedshield){

        while(ch != ENTER){
            wclear(win);
            mvwprintw(win, 1, 4, "REMOVE");
            mvwprintw(win, 3, 4, "DROP");
            mvwprintw(win, 5, 4, "RETURN");
            mvwprintw(win, (2*posy)+1, 1, "->");
            box(win, 0, 0);
            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 2){
                posy++;
            }

            if(ch == ENTER && posy == 0){
                play->playerStat.def -= (inventory+((y*5)+(x)))->buff;
                *equipedshield = -1;
            }

            if(ch == ENTER && posy == 1){
                play->playerStat.def -= (inventory+((y*5)+(x)))->buff;
                (inventory+((y*5)+(x)))->type = -1;
                (inventory+((y*5)+(x)))->buff = -1;
                (inventory+((y*5)+(x)))->typebuff = -1;
                (inventory+((y*5)+(x)))->name[0] = '\0';
                *equipedshield = -1;
            }

        }

    }

    else{

        while(ch != ENTER){
            wclear(win);
            if((inventory+((y*5)+(x)))->type == 2){
                mvwprintw(win, 1, 4, "DRINK");
            }
            else{
                mvwprintw(win, 1, 4, "EQUIP");
                
            }
            mvwprintw(win, 3, 4, "DROP");
            mvwprintw(win, 5, 4, "RETURN");
            mvwprintw(win, (2*posy)+1, 1, "->");
            box(win, 0, 0);
            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 2){
                posy++;
            }

            if(ch == ENTER && posy == 0){
                if((inventory+((y*5)+(x)))->type == 0 || (inventory+((y*5)+(x)))->type == 3){
                    if(*equipedsword != -1){
                        play->playerStat.atck -= (inventory+(*equipedsword))->buff;

                        play->playerStat.atck += (inventory+((y*5)+(x)))->buff;

                        *equipedsword = (y*5)+(x);

                    }
                    else{

                        play->playerStat.atck += (inventory+((y*5)+(x)))->buff;

                        *equipedsword = (y*5)+(x);
                    }
                }
                if((inventory+((y*5)+(x)))->type == 1){
                    if(*equipedshield != -1){
                        play->playerStat.def -= (inventory+(*equipedshield))->buff;

                        play->playerStat.def += (inventory+((y*5)+(x)))->buff;

                        *equipedshield = (y*5)+(x);

                    }
                    else{

                        play->playerStat.def += (inventory+((y*5)+(x)))->buff;

                        *equipedshield = (y*5)+(x);
                    }
                }

                if((inventory+((y*5)+(x)))->type == 2){
                    if((inventory+((y*5)+(x)))->typebuff == 1){

                        play->playerStat.speed += (inventory+((y*5)+(x)))->buff;

                        (inventory+((y*5)+(x)))->type = -1;
                        (inventory+((y*5)+(x)))->buff = -1;
                        (inventory+((y*5)+(x)))->typebuff = -1;
                        (inventory+((y*5)+(x)))->name[0] = '\0';


                    }

                    else if((inventory+((y*5)+(x)))->typebuff == 2){

                        play->playerStat.pv += (inventory+((y*5)+(x)))->buff;

                        if(play->playerStat.pv > play->playerStat.tot_pv){
                            play->playerStat.pv = play->playerStat.tot_pv;
                        }

                        (inventory+((y*5)+(x)))->type = -1;
                        (inventory+((y*5)+(x)))->buff = -1;
                        (inventory+((y*5)+(x)))->typebuff = -1;
                        (inventory+((y*5)+(x)))->name[0] = '\0';

                    }
                }
            }

            if(ch == ENTER && posy == 1){
                (inventory+((y*5)+(x)))->type = -1;
                (inventory+((y*5)+(x)))->buff = -1;
                (inventory+((y*5)+(x)))->typebuff = -1;
                (inventory+((y*5)+(x)))->name[0] = '\0';
            }

        }

    }

    wclear(win);
    wrefresh(win);

}

void openinventory(int winwidth, int winlength, WINDOW * win, Item * inventory, Player * pla, int * stop, int * equipedsword, int * equipedshield){
	/*Function which is triggered when the inventory is opened, it allows you to open and   navigate in the inventory she take 8 argument : the width and the length of a window, a  WINDOW * win, Item * inventory which is the player inventory, the Player * player, an int stop   which count the amount of time spend in the inventory,  two variables to know if a sword and   a shield are equiped */

    int x = 0;
    int y = 0;

    WINDOW * openIven = NULL;

    openIven = subwin(win, winlength/2, winwidth/2, 3+5, 7+5);

    if(openIven == NULL){
        printf("Error with openinventory");
        exit(15);
    }

    WINDOW * showstat = NULL;

    showstat = subwin(win, 8, 30, (winlength/3)+5, ((winwidth/2)+7)+5);

    if(showstat == NULL){
        printf("Error with showstat");
        exit(16);
    }

    WINDOW * actionmenu = NULL;

    actionmenu = subwin(win, 7, 30, (winlength/3)-2, ((winwidth/2)+7)+5);

    if(actionmenu == NULL){
        printf("Error with actionmenu");
        exit(17);
    }


    int t = time(NULL);

    int ch = 0;

    while(ch != 'i'){

        wclear(openIven);
        wclear(showstat);
        wclear(actionmenu);

        printinv( winlength/2, winwidth/2, openIven, showstat,inventory, y, x, equipedsword, equipedshield);

        box(actionmenu, 0, 0);

        wrefresh(openIven);
        wrefresh(showstat);
        wrefresh(actionmenu);

        ch = getch();

        if(ch == KEY_UP && y > 0){
            y--;
        }

        if(ch == KEY_LEFT && x > 0){
            x--;
        }

        if(ch == KEY_DOWN && y < 1){
            y++;
        }

        if(ch == KEY_RIGHT && x < 4){
            x++;
        }

        if(ch == ENTER){
            if(inventory[(y*5)+(x)].type == -1){
                mvwprintw(actionmenu, 3, 2, "You can do nothing here !");
                wrefresh(actionmenu);
                sleep(1);
                ch = getch();
                while(ch != ERR){
                    ch = getch();
                }
                wclear(actionmenu);
            }
            else{
                interactwithobject(actionmenu, equipedsword, equipedshield, y , x, pla, inventory);
            }
        }



    }

    wclear(actionmenu);
    wclear(openIven);
    wclear(showstat);

    wrefresh(openIven);
    wrefresh(showstat);
    wrefresh(actionmenu);

    
    int te = time(NULL);
    *stop += (te-t);

    
}



void setting(WINDOW * win, int winwidth, int winlendth ,int * maxroomwidth, int * maxroomlength, int * minroom){

    int ch = 0;

    int exitCond = -1;

    int y = 0;

    while(exitCond == -1){

        wclear(win);

        if(y == 0){
            mvwprintw(win, 5, winlendth/5, "Maximum Room width : <-[%d]-> ", *maxroomwidth);
            mvwprintw(win, 5 + 4, winlendth/5, "Maximum Room length : %d ", *maxroomlength);
            mvwprintw(win, 5 + 8, winlendth/5, "Minimum of room : %d ", *minroom);
            mvwprintw(win, 5 + 10, winlendth/4, "<┘ TO LEAVE");
        }

        if(y == 1){

            mvwprintw(win, 5, winlendth/5, "Maximum Room width : %d ", *maxroomwidth);
            mvwprintw(win, 5 + 4, winlendth/5, "Maximum Room length : <-[%d]-> ", *maxroomlength);
            mvwprintw(win, 5 + 8, winlendth/5, "Minimum of room : %d ", *minroom);
            mvwprintw(win, 5 + 10, winlendth/4, "<┘ TO LEAVE");

        }

        if(y == 2){

            mvwprintw(win, 5, winlendth/5, "Maximum Room width : %d ", *maxroomwidth);
            mvwprintw(win, 5 + 4, winlendth/5, "Maximum Room length : %d ", *maxroomlength);
            mvwprintw(win, 5 + 8, winlendth/5, "Minimum of room : <-[%d]-> ", *minroom);
            mvwprintw(win, 5 + 10, winlendth/4, "<┘ TO LEAVE");

        }

        box(win, 0, 0);

        wrefresh(win);

        ch = getch();

        if(ch == KEY_UP && y > 0){
            y--;
        }
        if(ch == KEY_DOWN && y < 2){
            y++;
        }

        if(ch == KEY_LEFT){
            if(y == 0 && *maxroomwidth > 3 ){
                (*maxroomwidth)--;
            }

            if(y == 1 && *maxroomlength > 3 ){
                (*maxroomlength)--;
            }

            if(y == 2 && *minroom > 12){
                (*minroom)--;
            }
        }

        if(ch == KEY_RIGHT){
            if(y == 0  && *maxroomwidth < 15){
                (*maxroomwidth)++;
            }

            if(y == 1 &&  *maxroomlength < 30){
                (*maxroomlength)++;
            }

            if(y == 2 &&  *minroom < 30){
                (*minroom)++;
            }
        }




        if(ch == ENTER){
            exitCond = 1;
        }


    }


    


}




void printMenu(WINDOW *win,int x, int y, int beg, int space){

    //Function which display the menu option 

    mvwprintw(win, y, x+2, "匚ㄖ丂爪丨匚\n");

    mvwprintw(win, y+1, x+2, "ㄚㄖ几ᗪ乇尺\n");

    mvwprintw(win, y+beg, x,"PLAY\n");

    mvwprintw(win, y+(beg+(space)), x,"SAVES\n");

    mvwprintw(win, y+(beg+(space*2)),x,"SETTINGS\n");

    mvwprintw(win, y+(beg+(space*3)), x,"QUIT\n");
}




void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space){
	
   //Starting transition when you select an option in the menu

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
    usleep(50000);
    wclear(win);
    }
    
}









void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy, int * maxroomwidth, int * maxroomlength, int * minroom){

    /*Function which will ensure the display of the menu and its options as well as navigation within it
she take 8 arguments, a WINDOW * win his length and width, the position x and y of options, the width max of a room, and length, minimum amount of room*/

    int chr = 0;

    char * name = NULL;

    

    int x = (winwidth/2)-10;
    int y = 2;
    
    
    int space = 4;
    int beg = y+3;


    int posx, posy;
    posx = (winwidth/3)-5;
    posy = beg+y;
    
    startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);


    while(posy != y+(beg+(3*space)) || chr != ENTER ){
        
        wclear(win);

        mvwprintw(win, posy, posx, "-->");
        
        printMenu(win, x, y, beg, space);

        box(win, 0,0);
        
        wrefresh(win);

        chr = getch();

        

        if (chr == ENTER && posy == y+beg){
            quitAnim(win, winlength, winwidth);
            startagame(win, winposx, winposy, winlength, winwidth, *maxroomwidth, *maxroomlength, *minroom, -1, "NULL");
            clear();
            refresh();
            startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
        }
        if (chr == ENTER && posy == y+(beg+(space))){
            quitAnim(win, winlength, winwidth);
            

            name = createName(win, winwidth, winlength);


            //startagame(win, winposx, winposy, winlength, winwidth, *maxroomwidth, *maxroomlength, *minroom, 1, name);
            clear();
            refresh();
            startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
        }
        if (chr == ENTER && posy == y+(beg+(space*2))){
            quitAnim(win, winlength, winwidth);
            setting(win, winlength, winwidth ,maxroomwidth, maxroomlength, minroom);
            clear();
            refresh();
            startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            
        }


        if(chr == KEY_UP && posy > y+beg){
            posy-= space;
        }
        if (chr == KEY_DOWN && posy < y+(beg+(space*3))){
            posy+= space;
        }

    
    }
    
    quitAnim(win, winlength, winwidth);

}


int pauseMenu(int winwidth, int winlength, int winposx, int winposy,WINDOW * win, int * stop){
    /*Function which assure the use of a menu during the game with two option : resume which make you go back to the game
    or save and quit which save your progression and then leave the game.
    This function take 6 arguments a window, her length and width also her x and y position on the screen and an intger pointer stop 
    who keep the time spended on the menu. */

    int y = 0;

    int exit = -1;

    int ch = 0;

    int t = time(NULL);

    wclear(win);

    while(exit == -1){

       

        mvwprintw(win, (winlength/2)-(winlength/4), (winwidth/2)-(winwidth/4), "RESUME");

        mvwprintw(win, (winlength/2)+(winlength/4), (winwidth/2)-(winwidth/4), "SAVE AND QUIT");

        mvwprintw(win, (winlength/2)-(winlength/4) + ((winlength/4)*2*y), (winwidth/2)-(winwidth/4) - 4, "-->"); // Display menu

        box(win, 0, 0);

        wrefresh(win);

        ch = getch();
        
        wclear(win);

        if(ch == KEY_UP && y > 0){
            y--;
        }
        if(ch == KEY_DOWN && y < 1){ // This part assure the move of the arrow
            y++;
        }

        if(ch == ENTER && y == 0){
            exit = 0;
        }

        if(ch == ENTER && y == 1){ // This part assure the exit option depending on the position of the arrow and therefore the option chosen
            exit = 1;
        }

        if(ch == 27){ // If the player press esc he just leave the menu
            exit = 0;
        }

        

    }

    int te = time(NULL);
    *stop += (te-t);

    if(exit == 1)
    {
        return 1;
    }
    else{
        return -1; // This part manage the return option depending the option choosen 1 resume  -1 save and quit
    }
    
    
}