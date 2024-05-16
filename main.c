#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10
#define NB_MOVE 4

typedef struct{
    int hours;
    int min;
    int sec;
}Time;

typedef struct{
    int nbmove;
    int power;
    int accuracy;
}Spell;

typedef struct{
    int pv;
    int exp;
    int level;
    int strenth;
    Spell move[NB_MOVE];
}Stat;

typedef struct{
    int posx;
    int posy;
    char * name;
    Stat playerStat;
    char ** inventory;
}Player;

typedef struct{
    int howmuchroom;
    int remote;
    int wall;
    int pos;
}Door;

typedef struct{
    int pv;
    //...
}Mob;

typedef struct{
    int placex;
    int placey;
    int typeEvent;
}Event;

typedef struct{
    int nb;
    int length;
    int width;
    int nbevent;
    Event * event;
    Door * nbdoor;
    char ** room;
}Room;



int isInt(char * ch){
    int i = 0;
    while(ch[i]!='\0'){
        if(ch[i]<= 47 || ch[i] >=58){
            return 0;
        }
        i++;
    }
    return 1;
}



int giveSeed(WINDOW * win, int winwidth, int winlength){

    char cseed[7];
    int seed = -1;

    echo();


    mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");        
    mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE THE SEED : ");
    wrefresh(win);
    wgetnstr(win, cseed, 6);
    wclear(win);


    while(isInt(cseed)!=1 || atoi(cseed) > 999999 || atoi(cseed) <= 0){
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE A CORRECT SEED, BETWEEN 1 AND 999999 : ");
        wrefresh(win);
        wgetnstr(win, cseed, 6);
        wclear(win);
    }


    noecho();

    seed = atoi(cseed);

    return seed;
}




void giveName(WINDOW * win, char ch[], int winlength, int winwidth){

    echo();
    
    mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
    mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE YOUR NAME : ");
    wrefresh(win);
    wgetnstr(win, ch, 35);
    

    while(ch[0] == '\0'){

        wclear(win);
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE A CORRECT NAME : ");
        wrefresh(win);
        wgetnstr(win, ch, 35);

    }

    noecho(); 
}




char* createName(WINDOW * win, int winwidth, int winlength){
    char ch[36];

    int c = 0;
     
    wclear(win);

    giveName(win, ch, winlength, winwidth);


    do{ //Confirm the name

        wclear(win);
        mvwprintw(win, (winlength/2)-1, (winwidth/2)-20, "[r] TO RESET |");
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        mvwprintw(win, (winlength/2), (winwidth/2)-5, " YOU CONFIRM THE PSEUDO : %s ?", ch);

        wrefresh(win);


        c = getch();
        

        if(c == 'r'){
            
            wclear(win);
            
            giveName(win, ch, winlength, winwidth);

        }

    }while(c != ENTER || ch[0] == '\0');


    int namesize = strlen(ch);

    char * name = NULL;

    name = malloc((namesize+1)*sizeof(char));

    if(name == NULL){
        printf("problem with the allocation of the name\n");
        exit(2);
    }

    for(int i = 0; i<namesize+1; i++){
        name[i] = ch[i];
    }

    wclear(win);

    return name;

}



Time createTime(int time){

    Time tim;

    tim.sec = time;
    tim.min = 0;
    tim.hours = 0;

    if(tim.sec >= 60){
        tim.min = tim.sec / 60;
        tim.sec = tim.sec%60;
        
    }

    if(tim.min >= 60){

        tim.hours = tim.min / 60;
        tim.min = tim.min % 60;
        
    }

    return tim;
}




Door * placeNbDoor(int realtot_door, int pressed, int previous_room, int length, int width, int toremove, int res, int tot_door, int nb_door, int * count){
    Door *d = NULL;

    int pos = 0;

    d = malloc(sizeof(Door)*4);

    if(d == NULL){
        printf("Error with the Door allocation");
        exit(8);
    }

    if( pressed < 0){
        for(int i = 0; i<4; i++){
            if(nb_door > 0 && *count <= realtot_door){
                pos = (rand()%100)%4;

                while(d[pos].wall == 1){
                    pos = (rand()%100)%4;
                }

                d[pos].wall = 1;
                *count += 1;
                d[pos].remote = *count;
                if(i == 3){
                    d[pos].howmuchroom = ((rand()%100)%res)+1;
                }
                else{
                   d[pos].howmuchroom = (rand()%100)%(toremove)+1; 
                }
                
                if(pos == 1 || pos == 3){
                    d[pos].pos = ((rand()%100)%(width-1))+1;
                }
                else{
                    d[pos].pos = ((rand()%100)%(length-1))+1;
                }


                nb_door--;
            }        
        }
    }
    else{
        d[pressed].wall = 1;
        d[pressed].howmuchroom = 1;
        if(pressed == 1 || pressed == 3){
            d[pressed].pos = ((rand()%100)%(width-1))+1;
        }
        else{
            d[pressed].pos = ((rand()%100)%(length-1))+1;
        }
        d[pressed].remote = previous_room;

        for(int i = 0; i<3; i++){
            if(nb_door > 0  && *count <= realtot_door){
                pos = (rand()%100)%4;

                while(d[pos].wall == 1){
                    pos = (rand()%100)%4;
                }

                d[pos].wall = 1;
                *count += 1;
                d[pos].remote = *count;
                d[pos].howmuchroom = (rand()%100)%(toremove)+1;
                if(pos == 1 || pos == 3){
                    d[pos].pos = ((rand()%100)%(width-1))+1;
                }
                else{
                    d[pos].pos = ((rand()%100)%(length-1))+1;
                }


                nb_door--;
            }        
        }


    }

    return d;
}






Room createRoom(int realtot_door, int pressed, int nb_door, int tot_door, int nb_room, int previous_room,int * count){

    Room law;

    int toremove = tot_door/nb_door;
    int res = tot_door%nb_door;

    law.nb = nb_room;

    law.length = ((rand()%100)%9)+3;

    law.width = ((rand()%100)%9)+3;



    law.nbdoor = placeNbDoor(realtot_door ,pressed, previous_room, law.length, law.width, toremove, res, tot_door,nb_door, count);

    



    law.nbevent = (rand()%2)+1;




    law.room = NULL;

    law.room = malloc(sizeof(char*)*(law.width + 2));

    if(law.room == NULL){
        printf("Error with the room allocation \n");
        exit(4);
    }

    for (int i = 0; i<law.width+2; i++){
        law.room[i] = NULL;
        law.room[i] = malloc(sizeof(char)*(law.length + 2));
        if (law.room[i] == NULL){
            printf("Error with the room allocation \n");
            exit(5);
        }
    }

    for(int i = 1; i<law.length+1; i++){
        law.room[0][i] = '_';
        law.room[law.width+1][i] = '_';
    }
    for(int i = 1; i<law.width+1; i++){
        law.room[i][0] = '|';
        law.room[i][law.length+1] = '|';
    }
    law.room[0][0] = 'a';
    law.room[0][law.length+1] = 'b';
    law.room[law.width+1][0] = 'c';
    law.room[law.width+1][law.length+1] = 'd';


    if(law.nbdoor[0].wall == 1){
        law.room[0][law.nbdoor[0].pos] = 'p';
    }
    if(law.nbdoor[1].wall == 1){
        law.room[law.nbdoor[1].pos][law.length+1] = 'p';
    }
    if(law.nbdoor[2].wall == 1){
        law.room[law.width+1][law.nbdoor[2].pos] = 'p';
    }
    if(law.nbdoor[3].wall == 1){
        law.room[law.nbdoor[3].pos][0] = 'p';
    }






    return law;




}


void printmap(WINDOW * win, int winlength, int winwidth, Room law){
    for (int i = 0; i<law.width+2; i++){
        for(int j = 0; j <law.length+2; j++){
            mvwprintw(win, (winlength/2) - (law.width/2) + i , (winwidth/2) - (law.length/2) + j, "%c", law.room[i][j]);
        }
    }
}







void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth){
    

    wclear(win);
    

    int ch = 0;

    int seed = giveSeed(win, winwidth, winlength);

    srand(seed);

    Player j;

    j.name = createName(win, winwidth, winlength);

    



    int tot_room = ((rand()%100)%10)+10;
    int tot_door = tot_room -1;
    int nb_door = 4;
    int place = 0;
    int place_before = -1;
    int count = 0;

    Room * room = NULL;

    room = calloc(tot_room, sizeof(Room));

    if(room == NULL){
        printf("Error with the allocation of the room");
        exit(6);
    }

    room[place] = createRoom(tot_room ,-1 ,nb_door, tot_door, place, place_before, &count);

    j.posx = winwidth/2;
    j.posy = winlength/2;


    char ** map = NULL;

    map = malloc(sizeof(char*) * ((room->width)+2));

    if(map == NULL){
        printf("Error with the allocation of the room");
        exit(7);
    }

    int t = (time(NULL));

    int te = time(NULL);

    Time tim;




    while(ch != ENTER){


        te = time(NULL);
        tim = createTime(te-t);    
        mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)-13, "匚ㄖ丂爪丨匚  ㄚㄖ几ᗪ乇尺\n");
        mvwprintw(stdscr, winposy-1, winposx+1, "CURRENT SEED : %d", seed);
        mvwprintw(stdscr, winlength+winposy+1, winposx+1, "TIME : %d h, %d min, %d sec", tim.hours, tim.min, tim.sec);
        mvwprintw(stdscr, 0, winwidth+winposx+2, "We are on the room %d", (room+place)->nb);
        mvwprintw(stdscr, 1,  winwidth+winposx+2, "length : %d", (room+place)->length);
        mvwprintw(stdscr, 2,  winwidth+winposx+2, "width : %d", (room+place)->width);
        mvwprintw(stdscr, 3,  winwidth+winposx+2, "%d", (room+place)->nbdoor[0].howmuchroom);
        mvwprintw(stdscr, 4,  winwidth+winposx+2, "%d", (room+place)->nbdoor[0].pos);
        mvwprintw(stdscr, 5,  winwidth+winposx+2, "%d", (room+place)->nbdoor[0].wall);
        mvwprintw(stdscr, 6,  winwidth+winposx+2, "%d", (room+place)->nbdoor[0].remote);

        


        box(win, 0,0);

        printmap(win, winlength, winwidth, room[place]);

        mvwprintw(win, j.posy, j.posx, "☃");
        // PLACE ROOM...

        
        
        wrefresh(win);
        wrefresh(stdscr);


        
        ch = getch();

        if (ch == KEY_UP && room[place].nbdoor[0].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[0].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = room[place_before].nbdoor[0].howmuchroom;
                if(tot_door >= 3){
                    nb_door = ((rand()%100)%3)+1;
                }
                else if(tot_door == 2){
                    nb_door = ((rand()%100)%2)+1;
                }
                else if (tot_door == 1){
                    nb_door = 1;
                }
                else{
                    nb_door = 0;
                }
                room[place] = createRoom(tot_room,2,nb_door, tot_door, place, place_before, &count);
            }
            
                
        }
        if (ch == KEY_DOWN && room[place].nbdoor[2].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[2].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = room[place_before].nbdoor[2].howmuchroom;
                if(tot_door >= 3){
                    nb_door = ((rand()%100)%3)+1;
                }
                else if(tot_door == 2){
                    nb_door = ((rand()%100)%2)+1;
                }
                else if (tot_door == 1){
                    nb_door = 1;
                }
                else{
                    nb_door = 0;
                }
                room[place] = createRoom(tot_room,0,nb_door, tot_door, place, place_before, &count);
            }
        }
        if (ch == KEY_LEFT && room[place].nbdoor[3].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[3].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = room[place_before].nbdoor[3].howmuchroom;
                if(tot_door >= 3){
                    nb_door = ((rand()%100)%3)+1;
                }
                else if(tot_door == 2){
                    nb_door = ((rand()%100)%2)+1;
                }
                else if (tot_door == 1){
                    nb_door = 1;
                }
                else{
                    nb_door = 0;
                }
                room[place] = createRoom(tot_room,1,nb_door, tot_door, place, place_before, &count);
            }
        }
        if (ch == KEY_RIGHT && room[place].nbdoor[1].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[1].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = room[place_before].nbdoor[1].howmuchroom;
                if(tot_door >= 3){
                    nb_door = ((rand()%100)%3)+1;
                }
                else if(tot_door == 2){
                    nb_door = ((rand()%100)%2)+1;
                }
                else if (tot_door == 1){
                    nb_door = 1;
                }
                else{
                    nb_door = 0;
                }
                
                room[place] = createRoom(tot_room,3, nb_door, tot_door, place, place_before, &count);
            }
        }
        
        




        wclear(win);
        wclear(stdscr);


        

    }




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

    mvwprintw(win, y+(beg+(space*2)),x,"SETTINGS\n");

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
    usleep(50000);
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
    usleep(50000);
    wclear(win);
    }
}





void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy){

    

    int chr = 0;

    

    int x = (winwidth/2)-10;
    int y = 2;
    
    
    int space = 4;
    int beg = y+3;


    int posx, posy;
    posx = (winwidth/3)-5;
    posy = beg+y;
    
    startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);

    wclear(win);

    while(posy != y+(beg+(3*space)) || chr != ENTER ){

        mvwprintw(win, posy, posx, "-->");
        
        printMenu(win, x, y, beg, space);

        
        box(win, 0,0);
        
        wrefresh(win);

        chr = getch();

        wclear(win);

        if (chr == ENTER && posy == y+beg){
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            startagame(win, winposx, winposy, winlength, winwidth);
        }
        if (chr == ENTER && posy == y+(beg+(space))){
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            loadSave(win);
        }
        if (chr == ENTER && posy == y+(beg+(space*2))){
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
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
