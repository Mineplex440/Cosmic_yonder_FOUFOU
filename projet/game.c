#include "game.h"
#include "menu.h"
#include "room.h"
#include "func.h"
#include "create.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10
#define NB_MOVE 4








void iscolide(char ** map, int map_width, int map_length, Room * actual_room,int posx, int posy, int pressed){

    /*
    int isTruey = 0;
    int isTruepy = 0;
    int isTruex = 0;
    int isTruepx = 0;

        if(pressed == 2){
        do{
            isTruey = 0;
            isTruepy = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                if(map[(map_width/2) + posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos] == '_' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos] == '|' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos] == 'p' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos + actual_room->length+1] == '_' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos + actual_room->length+1] == '|' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+ posx - actual_room->nbdoor[2].pos + actual_room->length+1] == 'p'){
                    isTruey++;
                }
            }
            if(isTruey > 0){
                actual_room->width--;
            }
            if(actual_room->nbdoor[1].pos > 1 && isTruey > 0){
                actual_room->nbdoor[1].pos --;
            }
            if(actual_room->nbdoor[3].pos > 1 && isTruey > 0){
                actual_room->nbdoor[3].pos --;
            }
        
        }while(isTruey > 0);

        do{
            isTruex = 0;
            isTruepx = 0;
            for(int i = 0; i<actual_room->length+2; i++){
                if(map[(map_width/2)+ posy - 2 - 1 - actual_room->width][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == '_'|| map[(map_width/2)+ posy - 2 - 1 - actual_room->width][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == '|' || map[(map_width/2)+ posy - 2 - 1 - actual_room->width][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == 'p' || map[(map_width/2)+ posy - 2][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == '_' || map[(map_width/2) + posy - 2 ][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == '|' || map[(map_width/2)+ posy - 2][(map_length/2)+ posx + i - actual_room->nbdoor[2].pos] == 'p'){
                    if(i<actual_room->nbdoor[2].pos){
                        isTruepx++;
                    }
                    else{
                        isTruex++; 
                    }
                    
                }
            }
            if(isTruex > 0){
                actual_room->length--;
            }

            if(actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos <= actual_room->length && isTruex > 0){
                actual_room->nbdoor[0].pos --;
            }
            if(actual_room->nbdoor[2].pos > 1 && actual_room->nbdoor[2].pos <= actual_room->length && isTruex > 0){
                actual_room->nbdoor[2].pos ++;
            }

        }while(isTruex > 0);
    }


    if(pressed == 3){
        do{
            isTruey = 0;
            isTruepy = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 ] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 ] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 ] == 'p' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 + actual_room->length+1] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 + actual_room->length+1] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + 2 + actual_room->length+1] == 'p'){
                    if(i<actual_room->nbdoor[3].pos){
                        isTruepy++;
                    }
                    else{
                        isTruey++;
                    }
                }
            }
            if(isTruey > 0){
                actual_room->width--;
            }

            if(actual_room->nbdoor[1].pos > 1  && actual_room->nbdoor[1].pos <= actual_room->width && isTruey > 0){
                actual_room->nbdoor[1].pos --;
            }
            if(actual_room->nbdoor[3].pos > 1 && actual_room->nbdoor[3].pos <= actual_room->width && isTruepy > 0){
                actual_room->nbdoor[3].pos ++;
            }
            else if(actual_room->nbdoor[1].pos > 1  && actual_room->nbdoor[3].pos <= actual_room->width && isTruey > 0){
                actual_room->nbdoor[3].pos --;
            }
        
        }while(isTruey > 0);

        do{
            isTruex = 0;
            isTruepx = 0;
            for(int i = 0; i<actual_room->length+2; i++){
                if(map[(map_width/2)+posy - actual_room->nbdoor[3].pos][(map_length/2)+posx + i + 2 ] == '_' || map[(map_width/2)+posy - actual_room->nbdoor[3].pos][(map_length/2)+posx + i + 2 ] == '|' || map[(map_width/2)+posy - actual_room->nbdoor[3].pos][(map_length/2)+posx + i + 2 ] == 'p' || map[(map_width/2)+posy - actual_room->nbdoor[3].pos + actual_room->width+1][(map_length/2)+posx + i + 2 ] == '_' || map[(map_width/2)+posy - actual_room->nbdoor[3].pos + actual_room->width+1][(map_length/2)+posx + i + 2 ] == '|' || map[(map_width/2)+posy - actual_room->nbdoor[3].pos + actual_room->width+1][(map_length/2)+posx + i + 2 ] == 'p'){
                    isTruex++;
                }
            }
            if(isTruex > 0){
                actual_room->length--;
            }

            if(actual_room->nbdoor[0].pos > 1 && isTruex > 0){
                actual_room->nbdoor[0].pos --;
            }
            if(actual_room->nbdoor[2].pos > 1 && isTruex > 0){
                actual_room->nbdoor[2].pos --;
            }

        }while(isTruex > 0);
    }


    if(pressed == 0){
        do{
            isTruey = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                if(map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos] == '_' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos] == '|' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos] == 'p' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + actual_room->length+1] == '_' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + actual_room->length+1] == '|' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + actual_room->length+1] == 'p'){
                    isTruey++;
                }
            }
            if(isTruey > 0){
                actual_room->width--;
            }

            if(actual_room->nbdoor[1].pos > 1 && isTruey > 0){
                actual_room->nbdoor[1].pos --;
            }
            if(actual_room->nbdoor[3].pos > 1 && isTruey > 0){
                actual_room->nbdoor[3].pos --;
            }
        
        }while(isTruey > 0);

        do{
            isTruex = 0;
            for(int i = 0; i<actual_room->length+2; i++){
                if(map[(map_width/2)+posy + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == '_' || map[(map_width/2)+posy + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == '|' || map[(map_width/2)+posy + 2][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == 'p' || map[(map_width/2)+posy + 2 + actual_room->width+1][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == '_' || map[(map_width/2)+posy + 2 + actual_room->width+1][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == '|' || map[(map_width/2)+posy + 2 + actual_room->width+1][(map_length/2)+posx - actual_room->nbdoor[0].pos + i] == 'p'){
                    isTruex++;
                }
            }
            if(isTruex > 0){
                actual_room->length--;
            }

            if(actual_room->nbdoor[0].pos > 1 && isTruex > 0){
                actual_room->nbdoor[0].pos --;
            }
            if(actual_room->nbdoor[2].pos > 1 && isTruex > 0){
                actual_room->nbdoor[2].pos --;
            }

        }while(isTruex > 0);


    }
    
    
    if(pressed == 1){

        do{
            isTruey = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2 - 1 - actual_room->length] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2 - 1 - actual_room->length] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2 - 1 - actual_room->length] == 'p' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx - 2] == 'p'){
                    isTruey++;
                }
            }
            if(isTruey > 0){
                actual_room->width--;
            }
            if(actual_room->nbdoor[1].pos > 1 && isTruey > 0){
                actual_room->nbdoor[1].pos --;
            }
            if(actual_room->nbdoor[3].pos > 1 && isTruey > 0){
                actual_room->nbdoor[3].pos --;
            }
        
        }while(isTruey > 0);

        do{
            isTruex = 0;
            for(int i = 0; i<actual_room->length+2; i++){
                if(map[(map_width/2)+posy - actual_room->nbdoor[1].pos][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == '_' || map[(map_width/2)+posy - actual_room->nbdoor[1].pos][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == '|' || map[(map_width/2)+posy - actual_room->nbdoor[1].pos][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == 'p' || map[(map_width/2)+posy - actual_room->nbdoor[1].pos + actual_room->width + 1][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == '_' || map[(map_width/2)+posy - actual_room->nbdoor[1].pos + actual_room->width + 1][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == '|' || map[(map_width/2)+posy - actual_room->nbdoor[1].pos + actual_room->width + 1][(map_length/2)+posx + i - 2 - 1 - actual_room->length] == 'p'){
                    isTruex++;
                }
            }
            if(isTruex > 0){
                actual_room->length--;
            }
            if(actual_room->nbdoor[0].pos > 1 && isTruex > 0){
                actual_room->nbdoor[0].pos --;
            }
            if(actual_room->nbdoor[2].pos > 1 && isTruex > 0){
                actual_room->nbdoor[2].pos --;
            }


        }while(isTruex > 0);
    }
    */
    int first = 0;


    if(pressed == 2){

        do{
        first = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){  
                if(map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == '_' ||map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == '|' || map[(map_width/2)+ posy + i - 2 - 1 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'p'){

                    first++;
                    if(actual_room->nbdoor[2].pos > j && actual_room->nbdoor[2].pos > 1 && actual_room->nbdoor[2].pos <= actual_room->length){
                        actual_room->nbdoor[2].pos--;
                    }
                    else if(actual_room->nbdoor[2].pos < j && actual_room->nbdoor[2].pos > 1 && actual_room->nbdoor[2].pos <= actual_room->length){
                        actual_room->nbdoor[2].pos++;
                    }

                }
            }
        }
        if(first > 0){
            if(actual_room->length > 8){
                if(actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }
                if(actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }
                actual_room->length --;
            }
            else{
                actual_room->length = 0;
            }
            if(actual_room->width > 4){
                if(actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }
                actual_room->width--;
            }
            else{
                actual_room->width = 0;
            }
        }

        }while(first > 0);
        
    }


    if(pressed == 3){

        do{
        first = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+ posx + j + 2 ] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 2 ] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 2 ] == 'p'){
                    first++;
                    if(actual_room->nbdoor[3].pos > i && actual_room->nbdoor[3].pos > 1 && actual_room->nbdoor[3].pos <= actual_room->width){
                        actual_room->nbdoor[3].pos--;
                    }
                    else if(actual_room->nbdoor[3].pos < i && actual_room->nbdoor[3].pos > 1 && actual_room->nbdoor[3].pos <= actual_room->width){
                        actual_room->nbdoor[3].pos++;
                    }
                }
            }
        }
        if (first > 0){
            if(actual_room->length > 8){
                if(actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }
                actual_room->length --;
            }
            else{
                actual_room->length = 0;
            }
            if(actual_room->width > 4){
                if(actual_room->nbdoor[0].pos > 1){
                        actual_room->nbdoor[0].pos--;
                }
                if(actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }
                actual_room->width--;
            }
            else{
                actual_room->width = 0;
            }
        }

        }while(first > 0);
        
    }


    if(pressed == 0){
        

        do{
            first = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                for(int j = 0; j<actual_room->length+2; j++){
                    if(map[(map_width/2)+posy + i + 2][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == '_' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == '|' || map[(map_width/2)+posy + i + 2][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'p'){
                        first++;
                        if(actual_room->nbdoor[0].pos > j && actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos <= actual_room->length){
                            actual_room->nbdoor[0].pos--;
                        }
                        else if(actual_room->nbdoor[0].pos < j && actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos <= actual_room->length){
                            actual_room->nbdoor[0].pos++;
                        }
                    }
                }
            }
            if(first > 0){
                if(actual_room->length > 8){
                    if(actual_room->nbdoor[1].pos > 1){
                        actual_room->nbdoor[1].pos--;
                    }
                    if(actual_room->nbdoor[3].pos > 1){
                        actual_room->nbdoor[3].pos--;
                    }
                    actual_room->length --;
                }
                else{
                    actual_room->length = 0;
                }
                if(actual_room->width > 4){
                    if(actual_room->nbdoor[2].pos > 1){
                        actual_room->nbdoor[2].pos--;
                    }
                    actual_room->width--;
                }
                else{
                    actual_room->width = 0;
                }
            }

        }while(first > 0);
        
    }


    if(pressed == 1){

        do{
        first = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - 1 - actual_room->length] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - 1 - actual_room->length] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - 1 - actual_room->length] == 'p'){
                    if(actual_room->nbdoor[1].pos > i && actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos <= actual_room->length){
                            actual_room->nbdoor[1].pos--;
                    }
                    else if(actual_room->nbdoor[1].pos < i && actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos <= actual_room->length){
                            actual_room->nbdoor[1].pos++;
                    }
                    first++;
                }
            }
        }
        if(first > 0){


            if(actual_room->length > 8){
                actual_room->length --;
                if(actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }
            }
            else{
                actual_room->length = 0;
            }
            if(actual_room->width > 4){
                if(actual_room->nbdoor[0].pos > 1){
                        actual_room->nbdoor[0].pos--;
                }
                if(actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }
                actual_room->width--;
            }
            else{
                actual_room->width = 0;
            }


        }
        

        }while(first > 0);
    }
}











void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth){
    

    wclear(win);
    

    int ch = 0;

    int seed = giveSeed(win, winwidth, winlength);


    srand(seed);

    Player j;

    j.name = createName(win, winwidth, winlength);

    int lenght_max_room =  18;
    int width_max_room = 9;

    



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
    
    tot_room -= 1; 
    j.posx = 0;
    j.posy = 0;
    

    


    int size_map_width = (((tot_room+2) * width_max_room) * 2);
    int size_map_length = ((tot_room+2)*lenght_max_room)*2;


    char ** map = NULL;

    map = calloc(size_map_width+1 ,sizeof(char*));

    if(map == NULL){
        printf("Error with the allocation of the room");
        exit(7);
    }

    for(int i = 0; i<size_map_width+1; i++){
        map[i] = NULL;
        map[i] = malloc(size_map_length+1 * sizeof(char));
        if(map[i] == NULL){
            printf("Error with the allocation of the %d room", i);
            exit(8);
        }
    }
    
    room[place] = createRoom(width_max_room, lenght_max_room, tot_room ,-1 ,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);

    int placementx = (tot_room+2)/2;
    int placementy = (tot_room+2)/2;
    int canplaceroom[4] = {0, 0, 0, 0};
    int comptRoom = 0;

    char ** placement = NULL;

    placement = malloc(sizeof(char*) * (tot_room+2));

    if(placement == NULL){
        printf("Error with the alloction of the char ** placement");
        exit(9);
    }

    for(int i = 0; i<tot_room+2; i++){
        placement[i] = NULL;
        placement[i] = malloc(sizeof(char) * (tot_room+2));
        if(placement[i] == NULL){
            printf("Error with the alloction of the %d placement", i);
            exit(10);
        }
        for(int j = 0; j<tot_room+2; j++){
            placement[i][j] = -1;
        }
    }

    placement[placementy][placementx] = room[place].nb;

    placeRoom(map, size_map_width, size_map_length, -1, room[place], j.posx, j.posy, winwidth, winlength);

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


        mvwprintw(stdscr, 3,  winwidth+winposx+2, "howmuchroom  : %d", (room+place)->nbdoor[0].howmuchroom);
        mvwprintw(stdscr, 4,  winwidth+winposx+2, "pos : %d", (room+place)->nbdoor[0].pos);
        mvwprintw(stdscr, 5,  winwidth+winposx+2, "wall : %d", (room+place)->nbdoor[0].wall);
        mvwprintw(stdscr, 6,  winwidth+winposx+2, "remote : %d", (room+place)->nbdoor[0].remote);

        mvwprintw(stdscr, 8,  winwidth+winposx+2, "howmuchroom  : %d", (room+place)->nbdoor[1].howmuchroom);
        mvwprintw(stdscr, 9,  winwidth+winposx+2, "pos : %d", (room+place)->nbdoor[1].pos);
        mvwprintw(stdscr, 10,  winwidth+winposx+2, "wall : %d", (room+place)->nbdoor[1].wall);
        mvwprintw(stdscr, 11,  winwidth+winposx+2, "remote : %d", (room+place)->nbdoor[1].remote);

        mvwprintw(stdscr, 13,  winwidth+winposx+2, "howmuchroom  : %d", (room+place)->nbdoor[2].howmuchroom);
        mvwprintw(stdscr, 14,  winwidth+winposx+2, "pos : %d", (room+place)->nbdoor[2].pos);
        mvwprintw(stdscr, 15,  winwidth+winposx+2, "wall : %d", (room+place)->nbdoor[2].wall);
        mvwprintw(stdscr, 16,  winwidth+winposx+2, "remote : %d", (room+place)->nbdoor[2].remote);

        mvwprintw(stdscr, 18,  winwidth+winposx+2, "howmuchroom  : %d", (room+place)->nbdoor[3].howmuchroom);
        mvwprintw(stdscr, 19,  winwidth+winposx+2, "pos : %d", (room+place)->nbdoor[3].pos);
        mvwprintw(stdscr, 20,  winwidth+winposx+2, "wall : %d", (room+place)->nbdoor[3].wall);
        mvwprintw(stdscr, 21,  winwidth+winposx+2, "remote : %d", (room+place)->nbdoor[3].remote);
        
        mvwprintw(stdscr, 23,  winwidth+winposx+2, "tot room : %d", tot_room);
        mvwprintw(stdscr, 24,  winwidth+winposx+2, "tot door : %d", tot_door);
        mvwprintw(stdscr, 25,  winwidth+winposx+2, "nb door : %d", nb_door);

        


        box(win, 0,0);

        printmap(win, winlength, winwidth, map, j.posy, j.posx, size_map_width, size_map_length);


        
        // PLACE ROOM...

        
        
        wrefresh(win);
        wrefresh(stdscr);


        
        ch = getch();

        if(ch == KEY_DOWN && map[(size_map_width/2) + j.posy + 1][(size_map_length/2) + j.posx] != '_' && map[(size_map_width/2) + j.posy + 1][(size_map_length/2) + j.posx] != 'p'){
            j.posy++;
            if (map[(size_map_width/2) + j.posy + 1][(size_map_length/2) + j.posx] == 'p'){
                place_before = room[place].nb;
                place = room[place].nbdoor[2].remote;
                placementy += 1;
                if(room[place].nbdoor == NULL){

                /*
                    canplaceroom[0] = 0;
                    if(placement[placementy+1][placementx+1] != -1){
                        if(room[placement[placementy+1][placementx+1]].nbdoor[0].wall != 1){
                            canplaceroom[1] += 1; 
                        }
                        if(room[placement[placementy+1][placementx+1]].nbdoor[4].wall != 1){
                            canplaceroom[2] += 1; 
                        }
                    }
                    else{
                        canplaceroom[1] += 1;
                        canplaceroom[2] += 1;
                    }


                    if(placement[placementy+1][placementx-1] != -1){
                        if(room[placement[placementy+1][placementx-1]].nbdoor[0].wall != 1){
                            canplaceroom[3] += 1; 
                        }
                        if(room[placement[placementy+1][placementx-1]].nbdoor[1].wall != 1){
                            canplaceroom[2] += 1; 
                        }
                    }
                    else{
                        canplaceroom[3] += 1;
                        canplaceroom[2] += 1;
                    }


                    if(placement[placementy-1][placementx+1] != -1){
                        if(room[placement[placementy-1][placementx+1]].nbdoor[2].wall != 1){
                            canplaceroom[1] += 1; 
                        }
                    }
                    else{
                        canplaceroom[1] += 1;
                    }


                    if(placement[placementy-1][placementx-1] != -1){
                        if(room[placement[placementy-1][placementx-1]].nbdoor[2].wall != 1){
                            canplaceroom[4] += 1; 
                        }
                    }
                    else{
                        canplaceroom[3] += 1;
                    }

                    if(placement[placementy][placementx-1] == -1){
                        canplaceroom[3] += 1;
                    }

                    if(placement[placementy+1][placementx] == -1){
                        canplaceroom[2] += 1;
                    }

                    if(placement[placementy][placementx+1] != -1){
                        canplaceroom[1] += 1;
                    }

                    for(int i = 0; i<4; i++){
                        if( canplaceroom[i] == 3){
                            comptRoom += 1;
                        }
                    }
                */
               

                    tot_door = (room[place_before].nbdoor[2].howmuchroom)-1;
                /*
                    if(comptRoom == 0){
                        if(room[count].nbdoor[0].wall == 1){
                            room[count].nbdoor[0].howmuchroom += tot_door;
                        }
                        if(room[count].nbdoor[1].wall == 1){
                            room[count].nbdoor[1].howmuchroom += tot_door;
                        }
                        if(room[count].nbdoor[2].wall == 1){
                            room[count].nbdoor[2].howmuchroom += tot_door;
                        }
                        if(room[count].nbdoor[3].wall == 1){
                            room[count].nbdoor[3].howmuchroom += tot_door;
                        }
                        
                        tot_door = 0;
                    }

                */
                    if(tot_door >= 3){
                        nb_door = ((rand()%100)%3)+1;
                    }
                    else if(tot_door == 2){
                        nb_door = ((rand()%100)%2)+1;
                    }
                    else if (tot_door == 1){
                        nb_door = 1;
                    }
                    else if (tot_door == 0) {
                        nb_door = 0;
                    }

                    room[place] = createRoom(width_max_room, lenght_max_room, tot_room,0,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);
                    placeRoom(map, size_map_width, size_map_length, 2, room[place], j.posx, j.posy, winwidth, winlength);
                }
                j.posy += 3;
            }
        }
        
        if(ch == KEY_UP && map[(size_map_width/2) + j.posy - 1][(size_map_length/2) + j.posx] != '_' && map[(size_map_width/2) + j.posy - 1][(size_map_length/2) + j.posx] != 'p'){
            j.posy--;
            if (map[(size_map_width/2) + j.posy - 1][(size_map_length/2) + j.posx] == 'p'){
                place_before = room[place].nb;
                place = room[place].nbdoor[0].remote;
                placementy -= 1;
                if(room[place].nbdoor == NULL){
                    tot_door = (room[place_before].nbdoor[0].howmuchroom)-1;
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
                    room[place] = createRoom(width_max_room, lenght_max_room, tot_room,2,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);
                    placeRoom(map, size_map_width, size_map_length, 0, room[place], j.posx, j.posy, winwidth, winlength);
                }
                j.posy -= 3;
            }
        }

        if(ch == KEY_LEFT && map[(size_map_width/2) + j.posy ][(size_map_length/2) + j.posx - 1] != '|' && map[(size_map_width/2) + j.posy ][(size_map_length/2) + j.posx - 1] != 'p'){
            j.posx--;
            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx -1] == 'p'){
                place_before = room[place].nb;
                place = room[place].nbdoor[3].remote;
                placementx -= 1;
                if(room[place].nbdoor == NULL){
                    tot_door = (room[place_before].nbdoor[3].howmuchroom)-1;
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
                    room[place] = createRoom(width_max_room, lenght_max_room, tot_room,1,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);
                    placeRoom(map, size_map_width, size_map_length, 3, room[place], j.posx, j.posy, winwidth, winlength);
                }
                j.posx -= 3;
            }
        }
        if(ch == KEY_RIGHT && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx + 1] != '|' && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx + 1] != 'p'){
            j.posx++;
            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx + 1] == 'p'){
                place_before = room[place].nb;
                place = room[place].nbdoor[1].remote;
                placementx += 1;
                if(room[place].nbdoor == NULL){
                    tot_door = (room[place_before].nbdoor[1].howmuchroom)-1;
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
                    room[place] = createRoom(width_max_room, lenght_max_room, tot_room, 3, nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);
                    placeRoom(map, size_map_width, size_map_length, 1, room[place], j.posx, j.posy, winwidth, winlength);
                }
                j.posx += 3;
                
            }
        }

        /*
        if (ch == KEY_UP && room[place].nbdoor[0].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[0].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = (room[place_before].nbdoor[0].howmuchroom)-1;
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
                room[place] = createRoom(width_max_room, lenght_max_room, tot_room,2,nb_door, tot_door, place, place_before, &count);
            }
            
                
        }
        if (ch == KEY_DOWN && room[place].nbdoor[2].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[2].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = (room[place_before].nbdoor[2].howmuchroom)-1;
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
                room[place] = createRoom(width_max_room, lenght_max_room, tot_room,0,nb_door, tot_door, place, place_before, &count);
            }
        }
        if (ch == KEY_LEFT && room[place].nbdoor[3].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[3].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = (room[place_before].nbdoor[3].howmuchroom)-1;
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
                room[place] = createRoom(width_max_room, lenght_max_room, tot_room,1,nb_door, tot_door, place, place_before, &count);
            }
        }
        if (ch == KEY_RIGHT && room[place].nbdoor[1].wall == 1){
            place_before = room[place].nb;
            place = room[place].nbdoor[1].remote;
            if(room[place].nbdoor == NULL ){
                tot_door = (room[place_before].nbdoor[1].howmuchroom)-1;
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
                
                room[place] = createRoom(width_max_room, lenght_max_room, tot_room,3, nb_door, tot_door, place, place_before, &count);
            }
        }
        
        */




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

