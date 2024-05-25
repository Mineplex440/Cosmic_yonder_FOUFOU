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
    //int nbmove;
    float power;
    float accuracy;
}Spell;

typedef struct{
    int pv;
    int tot_pv;
    int exp;
    int level;
    int atck; // Player attack
    int def; // Player defense
    int dodge; // Player dodge stat
    int speed; // Player speed
    int strenth;
    //Spell move[NB_MOVE];
}Stat;

typedef struct{
    char name[15];
    int type; // 0 = pistol, 1 = Armor, 2 = potion
    int typebuff; //if potion then 1 = swiftness, 2 = heal
    int buff; 
}Item;

typedef struct{
    int posx;
    int posy;
    char * name;
    Stat playerStat;
    Item inventory[10];
    int live;
}Player;

typedef struct{
    int howmuchroom;
    int remote;
    int wall;
    int pos;
}Door;

typedef struct{
    int pv;
    int atck;
    int dodge;
    int speed;
    int def;
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

void quitAnim(WINDOW * win, int winlength, int winwidth){

    box(win, 0, 0);


    for(int j = 1; j<winlength-1; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-");
        }
    

        wrefresh(win);
        usleep(50000);
    }
    //wclear(win);
    
}


void initPlayer(Player * p,Item * inven){ //

	p->playerStat.tot_pv = 50;
    p->playerStat.pv = p->playerStat.tot_pv;
	p->playerStat.atck = 5;
	p->playerStat.def = 3;
	p->playerStat.level = 0;
	p->playerStat.dodge = 8;
	p->playerStat.exp = 0;
	p->playerStat.speed = 2; // Initialization of the player stat
	p->live = 1;

    for(int i = 0; i<10; i++){
        if((inven+i)->type != -1){
            p->inventory[i].type = (inven+i)->type;
            p->inventory[i].buff = (inven+i)->buff;
            for(int j = 0; j<strlen((inven+i)->name); j++){
                p->inventory[i].name[j] = (inven[i].name[j]);
            }
            p->inventory[i].typebuff = (inven+i)->typebuff;
        }
        else{
            p->inventory[i].type = -1;
        }
    }
}


Item initItem(){ //Generation of the item

    Item i;
    int decider = rand()%11;
    if (decider == 0){
        strcpy(i.name, "LightSaber");
        i.type = 3;
        i.buff = 35;
    }
    else if((decider >= 1) && (decider <=3)){
        strcpy(i.name, "Pistol");
        i.type = 0;
        i.buff = 10;
    }
    else if((decider >= 4) && (decider <=6)){
        strcpy(i.name, "Shield");
        i.type = 1;
        i.buff = 15;
    }
    else{
        strcpy(i.name, "Potion");
        i.type = 2;
        decider = rand()%4;
        if(decider == 0){
            i.typebuff = 1;
            i.buff = 2;
        }
        else{
            i.typebuff = 2;
            i.buff = 40;
        }
    }
	return i;
}


Mob initMob(Player p){ //The mob is generated with the player level 
	Mob m;
	int lvl = p.playerStat.level;
	m.pv = 35 + (12*lvl);
	m.atck = rand()%3+4;
	m.def = rand()%2+1;
	m.dodge = 8;
	m.speed = rand()%4+1;

	for(int i=1;i<=lvl;i++){ // Loop who calibrate the mob to the player lvl
		m.atck += (rand()%3)+2;
		m.def += rand()%3;
		if((p.playerStat.level)%3 == 0){
		     m.dodge --;
		     m.speed ++;
		}
	}
	return m;
}

void player_lvl_up(Player * p){

	int lvl = p->playerStat.exp/100;

	while(p->playerStat.level < lvl ){
	
		p->playerStat.level++;
		p->playerStat.tot_pv += 10;
       	p->playerStat.pv = p->playerStat.tot_pv;
		p->playerStat.atck += 4;
		p->playerStat.def += 2;
		if((p->playerStat.level)%3 == 0){
		     p->playerStat.dodge --;
		     p->playerStat.speed ++;
		}
				
	}

}



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


    mvwprintw(win, (winlength/2), (winwidth/2)-18, "<┘ TO CONFIRM |");        
    wprintw(win, " GIVE THE SEED : ");
    wrefresh(win);
    wgetnstr(win, cseed, 6);
    wclear(win);


    while(isInt(cseed)!=1 || atoi(cseed) > 999999 || atoi(cseed) <= 0){
        mvwprintw(win, (winlength/2), (winwidth/2)-25, "<┘ TO CONFIRM |");
        wprintw(win, " GIVE A CORRECT SEED, BETWEEN 1 AND 999999 : ");
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
    
    mvwprintw(win, (winlength/2), (winwidth/2)-18, "<┘ TO CONFIRM |");
    wprintw(win, " GIVE YOUR NAME : ");
    wrefresh(win);
    wgetnstr(win, ch, 35);
    

    while(ch[0] == '\0'){

        wclear(win);
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        wprintw(win, " GIVE A CORRECT NAME : ");
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
        mvwprintw(win, (winlength/2)-1, (winwidth/2)-25, "[r] TO RESET |");
        mvwprintw(win, (winlength/2), (winwidth/2)-25, "<┘ TO CONFIRM |");
        wprintw(win, " YOU CONFIRM THE PSEUDO : %s ?", ch);

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

void phantomishere(int winwidth, int winlength, WINDOW * win, int * stop){

    WINDOW * phantom = NULL;

    phantom = subwin(win, 9, 60, (winlength/2)-2, (winwidth/2)-30);

    if(phantom == NULL){
        printf("Error with phantomishere");
        exit(14);
    }

    wclear(phantom);

    int t = time(NULL);

    mvwprintw(phantom, 4, 5, "A misterious power don't let you pass this door !");

    box(phantom, 0, 0);

    wrefresh(phantom);

    sleep(1);

    int te = time(NULL);

    *stop += (te-t);


}


int iscolide(char ** map, int map_width, int map_length, Room * actual_room,int posx, int posy, int pressed){

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



            

            if(actual_room->nbdoor[2].pos > actual_room->length && actual_room->nbdoor[2].pos > 1){
                    actual_room->nbdoor[2].pos--;
            }
            
            if(actual_room->nbdoor[1].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[1].pos--;
                }

            if(actual_room->nbdoor[0].pos > actual_room->length && actual_room->nbdoor[0].pos > 1){
                    actual_room->nbdoor[0].pos--;
                }
            if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[2].pos > 1){
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
            
            if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[3].pos > 1){
                    actual_room->nbdoor[3].pos--;
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



                if(actual_room->nbdoor[0].pos > 1 && actual_room->nbdoor[0].pos > actual_room->length){
                    actual_room->nbdoor[0].pos--;
                }

                if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
                    actual_room->nbdoor[3].pos--;
                }

                if(actual_room->nbdoor[1].pos > actual_room->width && actual_room->nbdoor[0].pos > 1){
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
            
            if(actual_room->nbdoor[1].pos > 1 && actual_room->nbdoor[1].pos > actual_room->width){
                actual_room->nbdoor[1].pos--;
            }

            if(actual_room->nbdoor[3].pos > actual_room->width && actual_room->nbdoor[1].pos > 1){
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






int createRoom(Room * law, int width_max_room, int lenght_max_room,int realtot_door, int pressed, int nb_door, int tot_door, int nb_room, int previous_room,int * count, int posx, int posy, char ** map, int map_width, int map_length){




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
        law->event[i].typeEvent = (rand()%100)%3; // Random pick of the type of event
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


int doyouwantchangeroom(int winwidth, int winlength, WINDOW * win, int * stop){

    int x = 0;

    WINDOW * changeroom = NULL;

    changeroom = subwin(win, 9, 40, (winlength/2)-2, (winwidth/2)-10);

    if(changeroom == NULL){
        printf("Error with doyouwantchangeroom");
        exit(12);
    }

    int t = time(NULL);

    int ch = 0;

    while(ch != ENTER){

        wclear(changeroom);

        mvwprintw(changeroom, 3, 7, "Do you want to change room ?");

        mvwprintw(changeroom, 6, 8, "YES");

        mvwprintw(changeroom, 6, 17, "NO");

        mvwprintw(changeroom, 6, 4+x, "->");

        box(changeroom, 0, 0);

        wrefresh(changeroom);

        ch = getch();

        if(ch == KEY_LEFT && x > 0){
            x -= 10;
        }
        if(ch == KEY_RIGHT && x < 10){
            x += 10;
        }

    }

    wclear(changeroom);

    wrefresh(changeroom);
    
    int te = time(NULL);
    *stop += (te-t);


    if(x == 0){
        return 1;
    }
    else{
        return 0;
    }

    free(changeroom);
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

    mvwprintw(item, 3, 7, "You dropped a %s !", i.name);

    box(item, 0, 0);

    wrefresh(item);

    sleep(1);

    wclear(item);

    int te = time(NULL);
    *stop += (te-t);
}

int doyouwantfight(int winwidth, int winlength, WINDOW * win, int * stop, int type_event){

    int x = 0;

    WINDOW * fight = NULL;

    fight = subwin(win, 9, 40, (winlength/2)-2, (winwidth/2)-10);

    if(fight == NULL){
        printf("Error with doyouwantfight");
        exit(13);
    }

    int t = time(NULL);

    int ch = 0;

    while(ch != ENTER){

        wclear(fight);

        switch(type_event){

            case 0: //if event is a fight
		        mvwprintw(fight, 3, 7, "Do you want to fight ?");
		        break;

            case 1: //if event is an item to pick up
		        mvwprintw(fight, 3, 8, "Pick up the item ?");
		        break;

	        case 2: // if event is a task
		        mvwprintw(fight, 3, 4, "Do you want to complete the task ?");
		        break; 

	    }

        mvwprintw(fight, 6, 8, "YES");

        mvwprintw(fight, 6, 17, "NO");

        mvwprintw(fight, 6, 4+x, "->");

        box(fight, 0, 0);

        wrefresh(fight);

        ch = getch();

        if(ch == KEY_LEFT && x > 0){
            x -= 10;
        }
        if(ch == KEY_RIGHT && x < 10){
            x += 10;
        }

    }

    wclear(fight);

    wrefresh(fight);
    
    int te = time(NULL);
    *stop += (te-t);


    if(x == 0){
        return 1;
    }
    else{
        return 0;
    }

    free(fight);

}

void printinv(int width, int length, WINDOW * win, WINDOW * showstat,Item * inventory, int y, int x, int * equipedsword, int * equipedshield){
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
    int posy = 0;

    int ch = 0;


    if((y*5)+(x) == *equipedsword){
        while(ch != ENTER){
            wclear(win);
            
            mvwprintw(win, 2, 4, "REMOVE");
            mvwprintw(win, 4, 4, "DROP");
            mvwprintw(win, (2*posy)+2, 1, "->");

            box(win, 0, 0);

            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 1){
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
            mvwprintw(win, 2, 4, "REMOVE");
            mvwprintw(win, 4, 4, "DROP");
            mvwprintw(win, (2*posy)+2, 1, "->");
            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 1){
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
            mvwprintw(win, 2, 4, "EQUIP");
            mvwprintw(win, 4, 4, "DROP");
            mvwprintw(win, (2*posy)+2, 1, "->");
            wrefresh(win);
            
            ch = getch();

            if(ch == KEY_UP && posy > 0){
                posy--;
            }
            if(ch == KEY_DOWN && posy < 1){
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


int fight(Mob mob, Player * player, WINDOW * win, int winlength, int winwidth, int * stop, int * equipedsword, int * equipedshield){
	/*Function that will manage the fight between a mob and the player->*
	The function takes 5 arguments : the mob and the player, with the window and his length and witdth*/

    clear();

	

    int posy = (winlength/2)+4; // Arrow coordonate


	int ex = 0; // exit condition for the while loop
	
	int chr = 0; // input
	
	int reward = 11; // for each attack turn we had +1 exp to the random pick of exp earned at the end of the fight
	
	int pvmob = mob.pv; // Initialized to assure the correct total amount of the mob vital point
	
	int def=0; // Def will be a random int taken between 0 and the stat of defense to prevent from damage on pv
	
	int def_value;
	
	WINDOW * win_a = NULL; // Declaration of the action box
	
	int winx = 25; // x position of the action box
	int winy = (winlength/2)+4; // y position of the action box
	int winl = 11; // Length of the action box
	int wind = 15; // Width of the action box
	
	win_a = subwin(win,winl, wind, winy, winx); //Window defined for the action box
	
	
	WINDOW * win_s = NULL; // Declaration of the information box
	
	int win_sx = (winwidth/2)-(winwidth/4); // x position of the information box
	int win_sy = 5; // y position of the information box
	int win_sl = 4; // Length of the information box
	int win_sd = 15; // Width of the information box
	int pvj = player->playerStat.pv;
	
	win_s = subwin(win,win_sl, win_sd, win_sy, win_sx); // Window defined for the player information
	
	
	WINDOW * win_fight = NULL; // Declaration of the fight box
	
	int win_fx = (winwidth/2)-(winwidth/4); // x position of the fight box
	int win_fy = (winlength/2)-(winlength/4)-3; // y position of the fight box
	int win_fl = 25; // Length of the fight box
	int win_fw = 60; // Width of the fight box
	
	win_fight = subwin(win,win_fl, win_fw, win_fy, win_fx); // Window defined for the fight display
	
	
	wclear(win_fight); // Clear of the area where the fight box will be printed
	
	box(win_fight, 0,0);	
	
	mvwprintw(win, (winlength/2)-5, (winwidth/2)-15, "A monster stands before you !"); // Begining of the fight
	
	wrefresh(win_fight);
	
	sleep(1);
	
	wclear(win_fight);
	
	while(ex != 1 && ex != 2 ){ // fight loop
	
	
		wclear(win_fight);
		
		//usleep(20000);
	
		box(win_fight, 0,0);	
		box(win_a, 0, 0);
		box(win_s, 0, 0);
		mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
		mvwprintw(win_fight, 1, 2, "PV %d/%d", player->playerStat.pv, player->playerStat.tot_pv );
		mvwprintw(win_fight, 1, (win_fw)-15, "Mob pv %d/%d", mob.pv, pvmob );
		mvwprintw(win_fight, (win_fl/2)+4, (win_fw/6)-3, "attack");
		mvwprintw(win_fight, (win_fl/2)+7, (win_fw/6)-3, "escape");
		mvwprintw(win_fight, (win_fl/2)+10, (win_fw/6)-3, "bag");
		mvwprintw(win_fight, posy, (win_fw /6)-7 , "-->"); // Display setup
		
		
		wrefresh(win_fight);

		chr = getch(); // Player input 
		
		if(chr == KEY_UP && posy != (win_fl/2)+4){
			posy -= 3;
		}
		if(chr == KEY_DOWN && posy != (win_fl/2)+10){ // Arrow move
			posy += 3;
		}
		if(chr == ENTER && posy == (win_fl/2)+4 ){ // attack option

			if(player->playerStat.speed>mob.speed){ // Player attack first if he has more speed

				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Your turn !");
				wrefresh(win_fight);
				sleep(1);
				if(((rand()%10)+1)%mob.dodge==0 ){ // Chance of miss the attack
				
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster dodge the attack !"); 
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def_value = mob.def;
					def = rand()%def_value;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You attack and inflict him %d damage !", player->playerStat.atck-def);//display of the fight information
					mob.pv -= player->playerStat.atck-def;
					wrefresh(win_fight);
					sleep(1);		
				}
				
				
				wclear(win_fight);
		
				//usleep(20000);
			
				box(win_fight, 0,0);	
				box(win_a, 0, 0);
				box(win_s, 0, 0);
				mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
				mvwprintw(win_fight, 1, (win_fw)-14, "Mob pv %d/%d", mob.pv, pvmob );
				mvwprintw(win_fight, 1, 2, "PV %d/%d", player->playerStat.pv, player->playerStat.tot_pv );
				mvwprintw(win_fight, (win_fl/2)+4, (win_fw/6)-3, "attack");
				mvwprintw(win_fight, (win_fl/2)+7, (win_fw/6)-3, "escape");
				mvwprintw(win_fight, (win_fl/2)+10, (win_fw/6)-3, "bag");
				mvwprintw(win_fight, posy, (win_fw /6)-7 , "-->"); // Display setup
						
				if(mob.pv>0){
					mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Monster turn ! ");
					wrefresh(win_fight);				
					sleep(1);
					

					if((rand()%10)%player->playerStat.dodge == 0){
					
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
						wrefresh(win_fight);
						sleep(1);
					}
					else{
						def = rand()%mob.def;
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
						player->playerStat.pv -= mob.atck-def;
						wrefresh(win_fight);
						sleep(1);				
					}
				}
				
				
			}
			else{ // Monster is faster case
			
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Monster turn ! ");
				wrefresh(win_fight);			
				sleep(1);
					
				if((rand()%10)%player->playerStat.dodge == 0){
					
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def = rand()%mob.def;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
					player->playerStat.pv -= mob.atck-def;
					wrefresh(win_fight);
					sleep(1);				
				}
				
				
				
				wclear(win_fight);
	
				//usleep(20000);
			
				box(win_fight, 0,0);	
				box(win_a, 0, 0);
				box(win_s, 0, 0);
				mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
				mvwprintw(win_fight, 1, (win_fw)-14, "Mob pv %d/%d", mob.pv, pvmob );
				mvwprintw(win_fight, 1, 2, "PV %d/%d", player->playerStat.pv, player->playerStat.tot_pv );
				mvwprintw(win_fight, (win_fl/2)+4, (win_fw/6)-3, "attack");
				mvwprintw(win_fight, (win_fl/2)+7, (win_fw/6)-3, "escape");
				mvwprintw(win_fight, (win_fl/2)+10, (win_fw/6)-3, "bag");
				mvwprintw(win_fight, posy, (win_fw /6)-7 , "-->"); // Display setup
				
				
				
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Your turn !");
				wrefresh(win_fight);
				sleep(1);
				
				if(player->playerStat.pv>0){
					if(((rand()%10)+1)%mob.dodge==0 ){ 
					
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster dodge the attack !");
						wrefresh(win_fight);
						sleep(1);
					}
					else{
						def = rand()%mob.def;
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You attack and inflict him %d damage !", player->playerStat.atck-def);//display of the fight information
						mob.pv -= player->playerStat.atck-def;
						wrefresh(win_fight);
						sleep(1);		
					}
				}
				
			}
			reward++;//Adding the survival bonus

            chr = getch(); //avoid the enter excess

            while(chr != ERR){
                chr = getch();
            }

		}
		if(chr == ENTER && posy == (win_fl/2)+7 ){ // Escape option
			
			if(((rand()%10)+1)%(player->playerStat.dodge-4)==0 ){ // Chance of succesfully escape
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Escape succed !");
				wrefresh(win_fight);
				sleep(1);
				ex = 1;
			}
			else{
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Escape failed !");
				wrefresh(win_fight);
				sleep(1);
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Monster turn !");
				wrefresh(win_fight);				
				sleep(1);		
				if((rand()%10)%player->playerStat.dodge == 0){
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def = rand()%mob.def;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
					player->playerStat.pv -= mob.atck-def;
					wrefresh(win_fight);
					sleep(1);				
				}
			
		
			}

            chr = getch();

            while(chr != ERR){
                chr = getch();
            }

		}
		
		if(chr == ENTER && posy == (win_fl/2)+10 ){ // Bag option
			openinventory(winwidth, winlength, win ,player->inventory, player, stop, equipedsword, equipedshield);

            wclear(win);
			
            


            chr = getch();

            while(chr != ERR){
                chr = getch();
            }

		}
		
		
		if(player->playerStat.pv<=0){
			wclear(win_fight);
			box(win_fight, 0,0);
			mvwprintw(win_fight, (win_fl/2), (win_fw/2)-5, "YOU DIED "); // Loose screen
			wrefresh(win_fight);
			sleep(2);
			player->live = 0;
			ex = 1;
		}
		if(mob.pv<=0){
			wclear(win_fight);
			box(win_fight, 0,0);
			mvwprintw(win_fight, (win_fl/2), (win_fw/2)-4, "Victory !"); // Victory Screen
			wrefresh(win_fight);
			sleep(2);
			reward += (rand()%30)+reward;
			player->playerStat.exp +=reward;
			mvwprintw(win_fight, (win_fl/2)+2, (win_fw/2)-4, "You earn +%d exp !", reward); // Victory rewards
			player_lvl_up(player); // level up check
			wrefresh(win_fight);
			sleep(1);
			ex = 2; // Exit variable 
		}
		
		
		//chr = 0; // Reset to 0 to prevent the menu from entering an option because the enter input is kept
	
		
		//wrefresh(win_fight);
	}

    wclear(win_a);
    wclear(win_s);
    wclear(win_fight);

    return ex;
	
}

int pauseMenu(int winwidth, int winlength, int winposx, int winposy,WINDOW * win, int * stop){

    int y = 0;

    int ch = 0;

    int t = time(NULL);

    wclear(win);

    while(ch != 27){

       

        mvwprintw(win, (winlength/2)-(winlength/4), (winwidth/2)-(winwidth/4), "RESUME");

        mvwprintw(win, (winlength/2)+(winlength/4), (winwidth/2)-(winwidth/4), "SAVE AND QUIT");

        mvwprintw(win, (winlength/2)-(winlength/4) + ((winlength/4)*2*y), (winwidth/2)-(winwidth/4) - 4, "-->");

        box(win, 0, 0);

        wrefresh(win);

        ch = getch();
        
        wclear(win);

        if(ch == KEY_UP && y > 0){
            y--;
        }
        if(ch == KEY_DOWN && y < 1){
            y++;
        }

        if(ch == ENTER && y == 0){
            return 0;
        }

        if(ch == ENTER && y == 1){
            return 1;
        }

        

    }

    int te = time(NULL);
    *stop += (te-t);

    return 0;
}


void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth){
    

    wclear(win);

    int exitCond = -1;
    

    int ch = 0;

    int seed = giveSeed(win, winwidth, winlength);


    srand(seed);

    Player j;

    int newXp = -1;
    
    Mob m;
    Item it;

    
    int countinv = 0;//How much place in the inventory remain
    int isininv = FALSE; //For place the item in the inventory
    for(int i = 0; i<10; i++){
        j.inventory[i].type = -1;
        j.inventory[i].name[0] = '\0';
        j.inventory[i].buff = -1;
        j.inventory[i].typebuff = -1;
    }
    
    initPlayer(&j ,j.inventory);
    player_lvl_up(&j);

    j.name = createName(win, winwidth, winlength);

    int equipedsword = -1;
    int equipedshild = -1;

    //j.playerStat.move[0].accuracy = 0.7;
    //j.playerStat.move[0].power = 10/100;

    //j.playerStat.move[1].accuracy = 0.8;
    //j.playerStat.move[1].power = 2;

    //j.playerStat.move[2].accuracy = 0.7;
    //j.playerStat.move[2].power = 2.5;

    //j.playerStat.move[3].accuracy = 1;
    //j.playerStat.move[1].power = -(10/100) ;



    int lenght_max_room =  18;
    int width_max_room = 9;

    



    int tot_room = ((rand()%100)%10)+20;
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

    for(int i = 0; i<tot_room; i++){
        room[i].nbdoor = NULL;
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
    
    createRoom(room+place ,width_max_room, lenght_max_room, tot_room ,-1 ,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length);

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

    int timer = 900;

    int t = (time(NULL));

    int stop = 0;

    int te = time(NULL);

    Time tim;

    while(exitCond != 1){


        te = time(NULL);
        tim = createTime(timer - (te-t-stop));    
        mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)-13, "匚ㄖ丂爪丨匚  ㄚㄖ几ᗪ乇尺\n");
        mvwprintw(stdscr, winposy-1, winposx+1, "CURRENT SEED : %d", seed);
        mvwprintw(stdscr, winlength+winposy+1, winposx+1, "TIME : %d min, %d sec", tim.min, tim.sec);

    /*
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
        
        mvwprintw(stdscr, 23,  winwidth+winposx+2, "tot room : %d", tot_room+1);
        mvwprintw(stdscr, 24,  winwidth+winposx+2, "tot door : %d", tot_door);
        mvwprintw(stdscr, 25,  winwidth+winposx+2, "nb door : %d", nb_door);

        mvwprintw(stdscr, 26,  winwidth+winposx+2, "event 1 x : %d", (room+place)->event[0].placex);
        mvwprintw(stdscr, 27,  winwidth+winposx+2, "event 1 y : %d", (room+place)->event[0].placey);
        mvwprintw(stdscr, 28,  winwidth+winposx+2, "event 2 x : %d", (room+place)->event[1].placex);
        mvwprintw(stdscr, 29,  winwidth+winposx+2, "event 2 y : %d", (room+place)->event[1].placey);
        mvwprintw(stdscr, 30,  winwidth+winposx+2, "count : %d", count);
    */

        


        box(win, 0,0);

        printmap(win, winlength, winwidth, map, j.posy, j.posx, size_map_width, size_map_length);

        
        
        wrefresh(win);
        wrefresh(stdscr);


        
        ch = getch();

        if(ch == KEY_DOWN && map[(size_map_width/2) + j.posy + 1][(size_map_length/2) + j.posx] != '_' && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] != 'p'){
            j.posy++;


            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'p'){
                if(doyouwantchangeroom(winwidth, winlength, win, &stop) == 1){
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

                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,0,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posy -= 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 2, room[place], j.posx, j.posy, winwidth, winlength);
                    }
                    
                }
                j.posy += 2;
                }
                else{
                    j.posy--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'M'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 0) == 1){
                    m = initMob(j);
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    }
                    j.posy--;
                }
                else{
                    j.posy--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'I'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 1) == 1){
                    
                    it = initItem();
                    printItem(winwidth, winlength, win, &stop, it);

                    countinv = 0;
                    isininv = FALSE;

                    for(int i = 0; i<10; i++){
                        if(j.inventory[i].type == -1  && isininv == FALSE){
                            for(int l = 0; l < 4; l++){
                                j.inventory[i].type = it.type;
                                j.inventory[i].buff = it.buff;
                                strcpy(j.inventory[i].name, it.name);
                                j.inventory[i].typebuff = it.typebuff;
                            }
                            isininv = TRUE;
                        }
                        else{
                            countinv++;
                        }
                    }


                    map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    j.posy--;
                }
                else{
                    j.posy--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    j.posy--;
                }
                else{
                    j.posy--;
                }
            }

        }
        



        if(ch == KEY_UP && map[(size_map_width/2) + j.posy - 1][(size_map_length/2) + j.posx] != '_' && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] != 'p'){
            j.posy--;
            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'p'){
                if(doyouwantchangeroom(winwidth, winlength, win, &stop) == 1){
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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,2,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posy += 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 0, room[place], j.posx, j.posy, winwidth, winlength);
                    }
                    
                }
                j.posy -= 2;
                }
                else{
                    j.posy++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'M'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 0) == 1){
                    m = initMob(j);
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    }
                    j.posy++;
                }
                else{
                    j.posy++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'I'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 1) == 1){
                    it = initItem();
                    printItem(winwidth, winlength, win, &stop, it);

                    countinv = 0;
                    isininv = FALSE;

                    for(int i = 0; i<10; i++){
                        if(j.inventory[i].type == -1  && isininv == FALSE){
                            for(int l = 0; l < 4; l++){
                                j.inventory[i].type = it.type;
                                j.inventory[i].buff = it.buff;
                                strcpy(j.inventory[i].name, it.name);
                                j.inventory[i].typebuff = it.typebuff;
                            }
                            isininv = TRUE;
                        }
                        else{
                            countinv++;
                        }
                    }

                    map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    j.posy++;
                }
                else{
                    j.posy++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    j.posy++;
                }
                else{
                    j.posy++;
                }
            }

        }

        if(ch == KEY_LEFT && map[(size_map_width/2) + j.posy ][(size_map_length/2) + j.posx - 1] != '|' && map[(size_map_width/2) + j.posy ][(size_map_length/2) + j.posx] != 'p'){
            j.posx--;
            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'p'){
                if(doyouwantchangeroom(winwidth, winlength, win, &stop) == 1){
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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,1,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posx+=3;

                    }
                    else{
                      placeRoom(map, size_map_width, size_map_length, 3, room[place], j.posx, j.posy, winwidth, winlength);  
                    }
                    
                }
                j.posx -= 2;
                }
                else{
                    j.posx++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'M'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 0) == 1){
                    m = initMob(j);
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    }
                    j.posx++;
                }
                else{
                    j.posx++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'I'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 1) == 1){
                    it = initItem();
                    printItem(winwidth, winlength, win, &stop, it);

                    countinv = 0;
                    isininv = FALSE;

                    for(int i = 0; i<10; i++){
                        if(j.inventory[i].type == -1  && isininv == FALSE){
                            for(int l = 0; l < 4; l++){
                                j.inventory[i].type = it.type;
                                j.inventory[i].buff = it.buff;
                                strcpy(j.inventory[i].name, it.name);
                                j.inventory[i].typebuff = it.typebuff;
                            }
                            isininv = TRUE;
                        }
                        else{
                            countinv++;
                        }
                    }

                    map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    j.posx++;
                }
                else{
                    j.posx++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    j.posx++;
                }
                else{
                    j.posx++;
                }
            }

        }


        if(ch == KEY_RIGHT && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx + 1] != '|' && map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] != 'p'){
            j.posx++;
            if (map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'p'){
                if(doyouwantchangeroom(winwidth, winlength, win, &stop) == 1){
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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room, 3, nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posx -= 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 1, room[place], j.posx, j.posy, winwidth, winlength);
                    }
                    
                }
                j.posx += 2;
                }
                else{
                    j.posx--;
                }
                
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'M'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 0) == 1){
                    m = initMob(j);
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    }
                    
                    j.posx--;
                }
                else{
                    j.posx--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'I'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 1) == 1){
                    it = initItem();
                    printItem(winwidth, winlength, win, &stop, it);

                    countinv = 0;
                    isininv = FALSE;

                    for(int i = 0; i<10; i++){
                        if(j.inventory[i].type == -1  && isininv == FALSE){
                            for(int l = 0; l < 4; l++){
                                j.inventory[i].type = it.type;
                                j.inventory[i].buff = it.buff;
                                strcpy(j.inventory[i].name, it.name);
                                j.inventory[i].typebuff = it.typebuff;
                            }
                            isininv = TRUE;
                        }
                        else{
                            countinv++;
                        }
                    }

                    map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                    j.posx--;
                }
                else{
                    j.posx--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    j.posx--;
                }
                else{
                    j.posx--;
                }
            }

        }


        if(ch == 'i'){
            openinventory(winwidth, winlength, win ,j.inventory, &j,&stop, &equipedsword, &equipedshild);
        }

        if(ch == 27){//pause menu


            wclear(win);
            exitCond = pauseMenu(winwidth, winlength, winposy, winposx , win, &stop);
        }


        

        if(j.live == 0){

            j.posx = 0;

            j.posy = 0;

            place = 0;

            newXp = j.playerStat.exp/2;
            
            initPlayer(&j,j.inventory);

            j.playerStat.exp = newXp;
            
            player_lvl_up(&j);

        }

        if(timer - (te-t-stop) <= 0){
            clear();
            box(win, 0, 0);
            wrefresh(win);
            mvwprintw(win, winlength/2, (winwidth/2)-12, "<┘ TO LEAVE | NO MORE TIME !");
            wrefresh(win);
            ch = 0;

            while (ch != ENTER)
            {
                ch = getch();
            }
            
            
            exitCond = 1;
        }

     


        wclear(win);
        wclear(stdscr);


        

    }

    if(timer - (te-t-stop) <= 0){
        //NO SAVE
    }


    quitAnim(win, winlength, winwidth);

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


    while(posy != y+(beg+(3*space)) || chr != ENTER ){
        
        wclear(win);

        mvwprintw(win, posy, posx, "-->");
        
        printMenu(win, x, y, beg, space);

        box(win, 0,0);
        
        wrefresh(win);

        chr = getch();

        

        if (chr == ENTER && posy == y+beg){
            quitAnim(win, winlength, winwidth);
            startagame(win, winposx, winposy, winlength, winwidth);
            startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
        }
        if (chr == ENTER && posy == y+(beg+(space))){
            quitAnim(win, winlength, winwidth);
            loadSave(win);
        }
        if (chr == ENTER && posy == y+(beg+(space*2))){
            quitAnim(win, winlength, winwidth);
            setting(win);
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
    int winlength = 30;//LINES - 3;
    int winwidth = 100;//COLS - 6;



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
