#include "game.h"
#include "menu.h"
#include "room.h"
#include "create.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10
#define NB_MOVE 4







 int iscolide(char ** map, int map_width, int map_length, Room * actual_room,int posx, int posy, int pressed){
//Make sure the room to be generated doesnt collide with other room

   
    int first = 0;
    int modifdoor = 0;
    int modifl = 0; 
    int modifw = 0;


    if(pressed == 2){

        do{
        first = 0;
        modifdoor = 0;
        modifw = 0;
        modifl = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){  
                if(map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == '_' ||map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == '|' || map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'p' || map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'a' || map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'b' || map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'c' || map[(map_width/2)+ posy + i - 2 - actual_room->width][(map_length/2)+posx + j - actual_room->nbdoor[2].pos] == 'd'){

                    first++;
                    if(actual_room->nbdoor[2].pos > j && actual_room->nbdoor[2].pos > 1 && actual_room->nbdoor[2].pos <= actual_room->length){
                        modifdoor++;
                    }
                    else{
                        modifdoor++;
                    }
                }
            }
        }
        if(first > 0){
            if(actual_room->length > 3){

                /*
                if(actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }
                if(actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }
                
                */
                actual_room->length --;
                modifl++;
            }
            if(actual_room->width > 3){

                /*
                if(actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }*/
                
                actual_room->width--;
                modifw++;
            }

            if(modifdoor > 0){
                    if(actual_room->nbdoor[2].pos > 1 && actual_room->nbdoor[2].pos <= actual_room->length){
                        actual_room->nbdoor[2].pos--;
                    }
                }
            else if(modifdoor < 0){
                if(actual_room->nbdoor[2].pos > actual_room->length && actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos++;
                }
            }


            
            if(actual_room->nbdoor[1].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }

            if(actual_room->nbdoor[0].pos > actual_room->length && actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }
            if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }


            if(modifl == 0 && modifw == 0){
                return 0;
            }
            
            
        }

        }while(first > 0);
        
    }


    if(pressed == 3){

        do{
        first = 0;
        modifdoor = 0;
        modifw = 0;
        modifl = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+ posx + j + 1 ] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == 'p' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == 'a' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == 'b' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == 'c' || map[(map_width/2)+posy + i - actual_room->nbdoor[3].pos][(map_length/2)+posx + j + 1 ] == 'd'){
                    first++;
                    if(actual_room->nbdoor[3].pos > i && actual_room->nbdoor[3].pos > 1 && actual_room->nbdoor[3].pos <= actual_room->width){
                        modifdoor++;
                    }
                    else{
                        modifdoor--;
                    }
                }
            }
        }
        if (first > 0){
            if(actual_room->length > 3){
                /*if(actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }*/
                
                actual_room->length --;
                modifl++;
            }
            if(modifdoor > 0){
                if(actual_room->nbdoor[3].pos > 1 && actual_room->nbdoor[3].pos <= actual_room->width){
                        actual_room->nbdoor[3].pos--;
                    }
                }
            else if(modifdoor < 0){
                if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos++;
                }
            }
                
            
            if(actual_room->width > 3){
                /*if(actual_room->nbdoor[0].pos > 1){
                        actual_room->nbdoor[0].pos--;
                }
                if(actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }*/
                actual_room->width--;
                modifw++;
            }
            
            
            if(actual_room->nbdoor[1].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }

            if(actual_room->nbdoor[0].pos > actual_room->length && actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }
            if(actual_room->nbdoor[2].pos > actual_room->length && actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }

            if(modifl == 0 && modifw == 0){
                return 0;
            }
        }

        }while(first > 0);
        
    }


    if(pressed == 0){
        

        do{
            first = 0;
            modifdoor = 0;
            modifw = 0;
            modifl = 0;
            for(int i = 0; i<actual_room->width+2; i++){
                for(int j = 0; j<actual_room->length+2; j++){
                    if(map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == '_' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == '|' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'p' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'a' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'b' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'c' || map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room->nbdoor[0].pos] == 'd'){
                        first++;
                        if(actual_room->nbdoor[0].pos > j && actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos <= actual_room->length){
                            modifdoor++;
                        }
                        else{
                            modifdoor--;
                        }
                    }
                }
            }
            if(first > 0){
                if(actual_room->length > 3){

                    /*
                    if(actual_room->nbdoor[1].pos > 1){
                        actual_room->nbdoor[1].pos--;
                    }
                    if(actual_room->nbdoor[3].pos > 1){
                        actual_room->nbdoor[3].pos--;
                    }
                    */
                    actual_room->length --;
                    modifl++;
                }
                if(actual_room->width > 3){
                    /*
                    if(actual_room->nbdoor[2].pos > 1){
                        actual_room->nbdoor[2].pos--;
                    }
                    */
                    
                    actual_room->width--;
                    modifw ++;
                }
                if(modifdoor > 0){
                        if(actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos <= actual_room->length){
                            actual_room->nbdoor[0].pos--;
                        }
                    }
                else if(modifdoor < 0){
                    if(actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos > actual_room->length){
                        actual_room->nbdoor[0].pos++;
                    }
                }



                

                if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }

                if(actual_room->nbdoor[1].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }
                if(actual_room->nbdoor[2].pos > actual_room->length && actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }



                if(modifl == 0 && modifw == 0){
                    return 0;
                }
                
            }

        }while(first > 0);
        
    }


    if(pressed == 1){

        do{
        first = 0;
        modifdoor = 0;
        modifw = 0;
        modifl = 0;
        for(int i = 0; i<actual_room->width+2; i++){
            for(int j = 0; j<actual_room->length+2; j++){
                if(map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == '_' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == '|' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == 'p' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == 'a' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == 'b' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == 'c' || map[(map_width/2)+posy + i - actual_room->nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room->length] == 'd'){
                    if(actual_room->nbdoor[1].pos > i && actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos <= actual_room->width){
                            modifdoor++;
                    }
                    else{
                        modifdoor--;
                    }
                    first++;
                }
            }
        }
        if(first > 0){


            if(actual_room->length > 3){
                actual_room->length --;

            /*
                if(actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }
            */
                modifl++;
                
            }

            if(modifdoor > 0){
                    if(actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos <= actual_room->width){
                        actual_room->nbdoor[1].pos--;
                    }
                }
            else if(modifdoor < 0){
                if(actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos > actual_room->width){
                    actual_room->nbdoor[1].pos++;
                }

            }
                
            
            if(actual_room->width > 3){

                /*

                if(actual_room->nbdoor[0].pos > 1){
                        actual_room->nbdoor[0].pos--;
                }
                if(actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }

                */

                actual_room->width--;
                modifw++;
            }
            
            

            if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }

            if(actual_room->nbdoor[0].pos > actual_room->length && actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }
            if(actual_room->nbdoor[2].pos > actual_room->length && actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
                }

                


            if(modifl == 0 && modifw == 0){
                return 0;
            }

            


        }
        }while(first > 0);
    }

    return 1;
}



Door * placeNbDoor(int realtot_door, int pressed, int previous_room, int length, int width, int tot_door, int nb_door, int * count){
	
   /* Function that allows you to place and generate doors, She take 8 arguments : 
   the totalof door, int pressed, int previous_room, int length, int width, int total door, int    number door, int * count  */

    Door *d = NULL;

    int pos = 0;

    d = malloc(sizeof(Door)*4);

    if(d == NULL){
        printf("Error with the Door allocation");
        exit(8);
    }

    int toplaced = tot_door;

    if( pressed < 0){    
        int toremove = tot_door/(nb_door);
        for(int i = 0; i<4; i++){
            if(nb_door > 0 && tot_door > 0 && *count < realtot_door){
                pos = (rand()%100)%4;

                while(d[pos].wall == 1){
                    pos = (rand()%100)%4;
                }

                d[pos].wall = 1;
                *count += 1;
                d[pos].remote = *count;

                if(toremove > 1){
                    if(nb_door == toplaced){
                        d[pos].howmuchroom = 1;
                        toplaced -= 1;
                    }
                    else if(nb_door == 1){
                        d[pos].howmuchroom = toplaced;
                    }
                    else{
                        d[pos].howmuchroom = ((rand()%200)%toremove)+1; 
                        toplaced -=  d[pos].howmuchroom;
                    }                    
                }
                else{

                    if(nb_door > 1){
                        d[pos].howmuchroom = 1;
                        toplaced -= 1;
                    }
                    else{
                        d[pos].howmuchroom = toplaced;
                    }

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
        int toremove = tot_door/(nb_door+1);
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
            if(nb_door > 0  &&  tot_door > 0 && *count < realtot_door){ 
                pos = (rand()%100)%4;

                while(d[pos].wall == 1){
                    pos = (rand()%100)%4;
                }

                d[pos].wall = 1;
                *count += 1;
                d[pos].remote = *count;

                if(toremove > 1){
                    if(nb_door == toplaced){
                        d[pos].howmuchroom = 1;
                        toplaced -= 1;
                    }
                    else if(nb_door == 1){
                        d[pos].howmuchroom = toplaced;
                    }
                    else{
                        d[pos].howmuchroom = ((rand()%200)%toremove)+1; 
                        toplaced -=  d[pos].howmuchroom;
                    }                    
                }
                else{

                    if(nb_door > 1){
                        d[pos].howmuchroom = 1;
                        toplaced -= 1;
                    }
                    else{
                        d[pos].howmuchroom = toplaced;
                    }

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

    return d;
}






int createRoom(Room * law, int width_max_room, int lenght_max_room,int realtot_door, int pressed, int nb_door, int tot_door, int nb_room, int previous_room,int * count, int posx, int posy, char ** map, int map_width, int map_length, int * nbtask){

   /*Function which randomly creates the different rooms it takes 16 arguments : 
    Room * law, int width max of a room, int lenght max of a room,int real total door, int pressed, int   number of door, int total door, int number room, int previous room,int * count, int position x, int position y, char ** map, int map width, int map length, int * number of task*/




    law->nb = nb_room;

    law->length = ((rand()%100)%lenght_max_room)+8;

    law->width = ((rand()%100)%width_max_room)+4;

    



    law->nbdoor = placeNbDoor(realtot_door ,pressed, previous_room, law->length, law->width, tot_door, nb_door, count);


    if(iscolide(map, map_width, map_length, law, posx, posy, pressed) == 0){
        law->nb = 0;
        law->length = 0;
        law->width = 0;
        *count -= nb_door;
        law->nbdoor = 0;
        return 0;
    }



    law->nbevent = ((rand()%100)%2)+1;// Number max of event per room

    law->event = NULL;

    law->event = calloc(law->nbevent, sizeof(Event));

    if(law->event == NULL){
        printf("Error with the event alloctation");
        exit(11);
    }

    for(int i = 0; i<law->nbevent; i++){
        law->event[i].placex = ((rand()%100)%(law->length-2))+2;
        law->event[i].placey = (((rand()%100)%(law->width-2))+2);
        for(int j = 0; j<i; j++){
            if(i>0 && law->event[j].placex == law->event[i].placex && law->event[j].placey == law->event[i].placey){
                law->event[i].placex = ((rand()%100)%(law->length-2))+2;
                law->event[i].placey = (((rand()%100)%(law->width-2))+2);
            }
        }


        law->event[i].typeEvent = (rand()%500)%101;

        
        if(*nbtask < 5){
            if(law->event[i].typeEvent >= 0 && law->event[i].typeEvent<=50){
                law->event[i].typeEvent = 2;
            }
            else if (law->event[i].typeEvent >= 51 && law->event[i].typeEvent<=70){
                law->event[i].typeEvent = 1;
            }
            else{
                law->event[i].typeEvent = 0;
            }
        }
        else{
            if(law->event[i].typeEvent >= 0 && law->event[i].typeEvent<=69){
                law->event[i].typeEvent = 0;
            }
            else{
                law->event[i].typeEvent = 1;
            }
        }



        if(law->event[i].typeEvent == 2){
            (*nbtask)++;
        }
        
    }




    law->room = NULL;

    law->room = malloc(sizeof(char*)*(law->width + 2));

    if(law->room == NULL){
        printf("Error with the room allocation \n");
        exit(4);
    }

    for (int i = 0; i<law->width+2; i++){
        law->room[i] = NULL;
        law->room[i] = malloc(sizeof(char)*(law->length + 2));
        if (law->room[i] == NULL){
            printf("Error with the room allocation \n");
            exit(5);
        }
    }

    for(int i = 1; i<law->length+1; i++){
        law->room[0][i] = '_';
        law->room[law->width+1][i] = '_';
    }
    for(int i = 1; i<law->width+1; i++){
        law->room[i][0] = '|';
        law->room[i][law->length+1] = '|';
    }
    law->room[0][0] = 'a';
    law->room[0][law->length+1] = 'b';
    law->room[law->width+1][0] = 'c';
    law->room[law->width+1][law->length+1] = 'd';


    if(law->nbdoor[0].wall == 1){
        law->room[0][law->nbdoor[0].pos] = 'p';
    }
    if(law->nbdoor[1].wall == 1){
        law->room[law->nbdoor[1].pos][law->length+1] = 'p';
    }
    if(law->nbdoor[2].wall == 1){
        law->room[law->width+1][law->nbdoor[2].pos] = 'p';
    }
    if(law->nbdoor[3].wall == 1){
        law->room[law->nbdoor[3].pos][0] = 'p';
    }

    for(int i = 0; i<law->nbevent; i++){
        if(law->event[i].typeEvent == 0){
            law->room[law->event[i].placey][law->event[i].placex] = 'M';
        }
        if(law->event[i].typeEvent == 1){
            law->room[law->event[i].placey][law->event[i].placex] = 'I';
        }
        if(law->event[i].typeEvent == 2){
            law->room[law->event[i].placey][law->event[i].placex] = 'T';
        }
    }






    return 1;

}


void printmap(WINDOW * win, int winlength, int winwidth, char ** map, int posy, int posx, int mapwidth, int maplength){
     /*Function which will ensure the display of the map, she take 8 arguments 
      A window her length and width, a char ** map, the position y and x of the player and the   width and length of the map*/

    for (int i = 1; i<winlength-1; i++){
        for(int j = 1; j <winwidth-1; j++){
            if(i == (winlength/2) && j == (winwidth/2)){
                mvwprintw(win, winlength/2, winwidth/2, "☃");
            }
            else if(map[(mapwidth/2 )+ posy + i - (winlength/2) ][(maplength/2) + posx + j - (winwidth/2)] == 'I'){
                mvwprintw(win, i , j, "I");  
            }
            else if(map[(mapwidth/2 )+ posy + i - (winlength/2) ][(maplength/2) + posx + j - (winwidth/2)] == 'M'){
                mvwprintw(win, i , j, "M");  
            }
            else if(map[(mapwidth/2 )+ posy + i - (winlength/2) ][(maplength/2) + posx + j - (winwidth/2)] == 'T'){
                mvwprintw(win, i , j, "T");  
            }
            else{
                mvwprintw(win, i , j, "%c", map[(mapwidth/2 )+ posy + i - (winlength/2) ][(maplength/2) + posx + j - (winwidth/2)]);  
            }
            
        }
    }
}



void placeRoom(char ** map, int map_width, int map_length, int pressed, Room actual_room, int posx, int posy, int winwidth, int winlength){
	
/*Function which will ensure the placement of a room on the map, 
she take 9 arguments, a char ** map her width and length, int pressed, Room actual room, player position x and y the width and length of the room*/

    if(pressed == -1){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+ posy + i - (actual_room.width/2)][(map_length/2)+ posx + j - (actual_room.length/2)] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 0){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){  
                map[(map_width/2)+ posy + i - 2 - actual_room.width][(map_length/2)+posx + j - actual_room.nbdoor[2].pos] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 1){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i - actual_room.nbdoor[3].pos][(map_length/2)+posx + j + 1 ] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 2){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i + 1][(map_length/2)+posx + j - actual_room.nbdoor[0].pos] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 3){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i - actual_room.nbdoor[1].pos][(map_length/2)+posx + j - 2 - actual_room.length] = actual_room.room[i][j];
            }
        }

    }
}
