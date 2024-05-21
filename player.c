#include "player.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10
#define NB_MOVE 4

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



Door * placeNbDoor(int realtot_door, int pressed, int previous_room, int length, int width, int tot_door, int nb_door, int * count){
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






Room createRoom(int width_max_room, int lenght_max_room,int realtot_door, int pressed, int nb_door, int tot_door, int nb_room, int previous_room,int * count, int posx, int posy, char ** map, int map_width, int map_length){

    Room law;



    law.nb = nb_room;

    law.length = ((rand()%100)%lenght_max_room)+8;

    law.width = ((rand()%100)%width_max_room)+4;

    



    law.nbdoor = placeNbDoor(realtot_door ,pressed, previous_room, law.length, law.width, tot_door, nb_door, count);


    iscolide(map, map_width, map_length, &law, posx, posy, pressed);



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


void printmap(WINDOW * win, int winlength, int winwidth, char ** map, int posy, int posx, int mapwidth, int maplength){
    for (int i = 1; i<winlength-1; i++){
        for(int j = 1; j <winwidth-1; j++){
            if(i == (winlength/2) && j == (winwidth/2)){
                mvwprintw(win, winlength/2, winwidth/2, "☃");
            }
            else{
                mvwprintw(win, i , j, "%c", map[(mapwidth/2 )+ posy + i - (winlength/2) ][(maplength/2) + posx + j - (winwidth/2)]);  
            }
            
        }
    }
}



void placeRoom(char ** map, int map_width, int map_length, int pressed, Room actual_room, int posx, int posy, int winwidth, int winlength){

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
                map[(map_width/2)+ posy + i - 2 - 1 - actual_room.width][(map_length/2)+posx + j - actual_room.nbdoor[2].pos] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 1){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i - actual_room.nbdoor[3].pos][(map_length/2)+posx + j + 2 ] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 2){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i + 2][(map_length/2)+posx + j - actual_room.nbdoor[0].pos] = actual_room.room[i][j];
            }
        }
    }
    if(pressed == 3){
        for(int i = 0; i<actual_room.width+2; i++){
            for(int j = 0; j<actual_room.length+2; j++){
                map[(map_width/2)+posy + i - actual_room.nbdoor[1].pos][(map_length/2)+posx + j - 2 - 1 - actual_room.length] = actual_room.room[i][j];
            }
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
                    else if (tot_door == 2){
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
