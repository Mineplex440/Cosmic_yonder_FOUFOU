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


void initPlayer(Player * p,Item * inven){ 
    /*Function which initializes the player at the start of a game, it takes two arguments a player and item pointer*/

	p->playerStat.tot_pv = 50;
    p->playerStat.pv = p->playerStat.tot_pv;
	p->playerStat.atck = 5;
	p->playerStat.def = 3;
	p->playerStat.level = 0;
	p->playerStat.dodge = 8;
	p->playerStat.exp = 0;
	p->playerStat.speed = 2; // Initialization of the player stat
	p->live = 1;

    for(int i = 0; i<10; i++){  //Inventory initialization 
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


Item initItem(){ 
    /*This function randomly generate item and return it*/

    Item i;
    int decider = (rand()%500)%101;
    if ((decider >= 0) && (decider <=4)){
        strcpy(i.name, "LightSaber"); 
        i.type = 3;
        i.buff = 35;
    }
    else if((decider >= 5) && (decider <=30)){
        strcpy(i.name, "Pistol");
        i.type = 0;
        i.buff = 10;
    }
    else if((decider >= 31) && (decider <=60)){
        strcpy(i.name, "Shield");
        i.type = 1;
        i.buff = 15;
    }
    else{
        strcpy(i.name, "Potion");
        i.type = 2;
        decider = (rand()%100)%4;
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


Mob initMob(Player p){ 
    /*This function generates a mob modulated at the current level of the player passed in parameter*/
	Mob m;
	int lvl = p.playerStat.level;
	m.pv = 35 + (12*lvl);
	m.atck = ((rand()%100)%3)+4;
	m.def = ((rand()%100)%2)+1;
	m.dodge = 8;
	m.speed = ((rand()%100)%4)+1;

	for(int i=1;i<=lvl;i++){ // Loop who calibrate the mob to the player lvl
		m.atck += ((rand()%100)%3)+2;
		m.def += (rand()%100)%3;
		if((p.playerStat.level)%3 == 0){
		     m.dodge --;
		     m.speed ++;
		}
	}
	return m;
}

void player_lvl_up(Player * p, int maxlvl){
    /*Function which manages the player's level up and his improvement,
     it takes two arguments, a player pointer and the maximum level that we can reach.*/

	int lvl = p->playerStat.exp/100; 

    if(lvl <= maxlvl){
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

	

}


int giveSeed(WINDOW * win, int winwidth, int winlength){
    /*This function asks the user to enter a seed between 1 and 999999 and return it
    She take 3 arguments a window her width and length*/

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
    /*This function asks the user to enter their name/nickname and returns it as a pointer
    She take 4 arguments a window her width and length and a character tab*/

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
        wgetnstr(win, ch, 35); //Display window

    }

    noecho(); 
}




char* createName(WINDOW * win, int winwidth, int winlength){
    /*This function allows you to create a nickname and returns a character pointer and if the  player presses r when confirming, he can return a nickname*/

    char ch[36];

    int c = 0;
     
    wclear(win);

    giveName(win, ch, winlength, winwidth);


    do{ //Confirm the name

        wclear(win);
        mvwprintw(win, (winlength/2)-1, (winwidth/2)-25, "[r] TO RESET |");
        mvwprintw(win, (winlength/2), (winwidth/2)-25, "<┘ TO CONFIRM |");
        wprintw(win, " YOU CONFIRM THE PSEUDO : %s ?", ch); //Display window

        wrefresh(win);


        c = getch();
        

        if(c == 'r'){ // reset of the name if ‘r’ get pressed
            
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
    /*Function which will divide the time into minutes and seconds by taking the total time in    seconds as a parameter*/	

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
//Create the window inside of the main one to display the presence of the phantom
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

void invIsFull(int winwidth, int winlength, WINDOW * win, int * stop){
//Create the window inside of the main one to alert the player that his inventory is full
    WINDOW * inv = NULL;

    inv = subwin(win, 9, 60, (winlength/2)-2, (winwidth/2)-30);

    if(inv == NULL){
        printf("Error with invIsFull");
        exit(14);
    }

    wclear(inv);

    int t = time(NULL);

    mvwprintw(inv, 4, 5, "Your inventory is full !");

    box(inv, 0, 0);

    wrefresh(inv);

    sleep(1);

    int te = time(NULL);

    *stop += (te-t);


}