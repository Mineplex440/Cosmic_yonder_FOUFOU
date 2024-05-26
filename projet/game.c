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


int doyouwantfight(int winwidth, int winlength, WINDOW * win, int * stop, int type_event){
	 /*Function which will ask the player if he want to fight or pick up an item or start a task, She take 5 argument the width and length of a WINDOW win, and an int * stop who keep the time spend on the display, int type event which is the type of the event faced (mob, item…) */  
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

int doyouwantchangeroom(int winwidth, int winlength, WINDOW * win, int * stop){

   /*Function which will ask the player when entering a door whether or not he wants to go  through the door
    She take 4 argument the width and length of a WINDOW win, and an int * stop who keep the time spend on the display */    

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

int fight(Mob mob, Player * player, WINDOW * win, int winlength, int winwidth, int * stop, int * equipedsword, int * equipedshield, int maxlvl){
	/*Function which will manage a fight between a mob and the player and return an       integer The function take 9 arguments : the mob and Player *  player, with the window and his length and witdth, also a int * stop which count the amount of time spend in the fight, two variables to know if a sword and   a shield are equiped, and an int maxlvl */

    clear();
    refresh();

	

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
	
	while(ex != 1 && ex != 2 ){ // fight loop
	
	
		wclear(win_fight);
		
		//usleep(20000);
	
		box(win_fight, 0,0);	
		box(win_a, 0, 0);
		box(win_s, 0, 0);
        mvwprintw(win_fight, (win_fl/2)+1, (win_fw/2)-5, "☃");
        mvwprintw(win_fight, (win_fl/2)-4, (win_fw/2)+15, "M");
		mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
		mvwprintw(win_fight, 1, 2, "PV %.0f/%d", player->playerStat.pv, player->playerStat.tot_pv );
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
				if((((rand()%100)%10)+1)%mob.dodge==0 ){ // Chance of miss the attack
				
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster dodge the attack !"); 
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def_value = mob.def;
					def = (rand()%100)%def_value;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You attack and inflict him %d damage !", player->playerStat.atck-def);//display of the fight information
					mob.pv -= player->playerStat.atck-def;
					wrefresh(win_fight);
                    for(int i = 0; i<3; i++){
                        mvwprintw(win_fight, (win_fl/2)+0-(i*2), (win_fw/2)-4+(7*i),"----");
                        wrefresh(win_fight);
                        usleep(50000);
                    }
					sleep(1);		
				}
				
				
				wclear(win_fight);
		
				//usleep(20000);
			
				box(win_fight, 0,0);	
				box(win_a, 0, 0);
				box(win_s, 0, 0);
                mvwprintw(win_fight, (win_fl/2)+1, (win_fw/2)-5, "☃");
                mvwprintw(win_fight, (win_fl/2)-4, (win_fw/2)+15, "M");
				mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
				mvwprintw(win_fight, 1, (win_fw)-15, "Mob pv %d/%d", mob.pv, pvmob );
				mvwprintw(win_fight, 1, 2, "PV %.0f/%d", player->playerStat.pv, player->playerStat.tot_pv );
				mvwprintw(win_fight, (win_fl/2)+4, (win_fw/6)-3, "attack");
				mvwprintw(win_fight, (win_fl/2)+7, (win_fw/6)-3, "escape");
				mvwprintw(win_fight, (win_fl/2)+10, (win_fw/6)-3, "bag");
				mvwprintw(win_fight, posy, (win_fw /6)-7 , "-->"); // Display setup
						
				if(mob.pv>0){
					mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Monster turn ! ");
					wrefresh(win_fight);				
					sleep(1);
					

					if(((rand()%100)%10)%player->playerStat.dodge == 0){
					
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
						wrefresh(win_fight);
						sleep(1);
					}
					else{
						def = (rand()%100)%mob.def;
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
						player->playerStat.pv -= mob.atck-def;
						wrefresh(win_fight);
                        for(int i = 0; i<3; i++){
                            mvwprintw(win_fight, (win_fl/2)-3+(i*2), (win_fw/2)+10-(7*i),"----");
                            wrefresh(win_fight);
                            usleep(50000);
                        }
						sleep(1);				
					}
				}
				
				
			}
			else{ // Monster is faster case
			
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Monster turn ! ");
				wrefresh(win_fight);			
				sleep(1);
					
				if(((rand()%100)%10)%player->playerStat.dodge == 0){
					
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def = (rand()%100)%mob.def;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
					player->playerStat.pv -= mob.atck-def;
					wrefresh(win_fight);
                    for(int i = 0; i<3; i++){
                        mvwprintw(win_fight, (win_fl/2)-3+(i*2), (win_fw/2)+10-(7*i),"----");
                        wrefresh(win_fight);
                        usleep(50000);
                    }
					sleep(1);				
				}
				
				
				
				wclear(win_fight);
	
				//usleep(20000);
			
				box(win_fight, 0,0);	
				box(win_a, 0, 0);
				box(win_s, 0, 0);
                mvwprintw(win_fight, (win_fl/2)+1, (win_fw/2)-5, "☃");
                mvwprintw(win_fight, (win_fl/2)-4, (win_fw/2)+15, "M");
				mvwprintw(win_fight, 2, 2,"lvl %d", player->playerStat.level );
				mvwprintw(win_fight, 1, (win_fw)-15, "Mob pv %d/%d", mob.pv, pvmob );
				mvwprintw(win_fight, 1, 2, "PV %.0f/%d", player->playerStat.pv, player->playerStat.tot_pv );
				mvwprintw(win_fight, (win_fl/2)+4, (win_fw/6)-3, "attack");
				mvwprintw(win_fight, (win_fl/2)+7, (win_fw/6)-3, "escape");
				mvwprintw(win_fight, (win_fl/2)+10, (win_fw/6)-3, "bag");
				mvwprintw(win_fight, posy, (win_fw /6)-7 , "-->"); // Display setup
				
				
				
				mvwprintw(win_fight, (win_fl/2)+9, (win_fw/6)+6, "Your turn !");
				wrefresh(win_fight);
				sleep(1);
				
				if(player->playerStat.pv>0){
					if((((rand()%100)%10)+1)%mob.dodge==0 ){ 
					
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster dodge the attack !");
						wrefresh(win_fight);
						sleep(1);
					}
					else{
						def = (rand()%100)%mob.def;
						mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You attack and inflict him %d damage !", player->playerStat.atck-def);//display of the fight information
						mob.pv -= player->playerStat.atck-def;
						wrefresh(win_fight);
                        for(int i = 0; i<3; i++){
                            mvwprintw(win_fight, (win_fl/2)+0-(i*2), (win_fw/2)-4+(7*i),"----");
                            wrefresh(win_fight);
                            usleep(50000);
                        }
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
			
			if((((rand()%100)%10)+1)%(player->playerStat.dodge-4)==0 ){ // Chance of succesfully escape
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
				if(((rand()%100)%10)%player->playerStat.dodge == 0){
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "You have dodge the attack !");
					wrefresh(win_fight);
					sleep(1);
				}
				else{
					def = (rand()%100)%mob.def;
					mvwprintw(win_fight, (win_fl/2)+11, (win_fw/6)+6, "The monster inflict you %d damage !", mob.atck-def);
					player->playerStat.pv -= mob.atck-def;
					wrefresh(win_fight);
                    for(int i = 0; i<3; i++){
                        mvwprintw(win_fight, (win_fl/2)-3+(i*2), (win_fw/2)+10-(7*i),"----");
                        wrefresh(win_fight);
                        usleep(50000);
                    }
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
			reward += (((rand()%100)%30)+30)+reward;
			player->playerStat.exp +=reward;
			mvwprintw(win_fight, (win_fl/2)+2, (win_fw/2)-4, "You earn +%d exp !", reward); // Victory rewards
            wrefresh(win_fight);
			sleep(1);

            if( (player->playerStat.exp/100) > maxlvl){
                mvwprintw(win_fight, (win_fl/2)+4, (win_fw/2)-4, "You are max level !"); //check lvl max
                wrefresh(win_fight);
                sleep(2);
            }
            else{
                player_lvl_up(player, maxlvl); // level up check
            }
			
			
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



int minigame1(WINDOW * win, int winwidth, int winlength){
    /*Function which is the first task of the game, its a simple function were the player copy a 
    sentence displayed on the screen. The function return 1 in case of a success or 0 if the player loose, she take 3 argument a window, his length and width*/

    char c[20] = "VOID"; // Char tab were the player will write the message

    int ch = 0;

    int ex = 0;

    wclear(win);

    mvwprintw(win, winwidth/2, winlength/3, "TASK 1");

    box(win, 0, 0);

    wrefresh(win);

    sleep(1);

    echo();

    wclear(win);

    mvwprintw(win, winwidth/2-2, winlength/3, "%d try left", 3-ex);

    mvwprintw(win, winwidth/2, winlength/3, "YOU NEED TO REBOOT THE SYSTEM");

    mvwprintw(win, (winwidth/2)+2, winlength/10, "<┘ TO CONFIRM | GIVE THE ORDER '/REB00T_SYS.exe' : ");

    box(win, 0, 0);

    wrefresh(win);

    wgetnstr(win, c, 19);



        

    while(strcmp(c, "/REB00T_SYS.exe") != 0 && ex < 3){ // While the answer isn't correct or the the nmber of try is <3 the user can re try to write the line 

        ex++;

        wclear(win);

        mvwprintw(win, winwidth/2-2, winlength/3, "%d try left", 3-ex);

        mvwprintw(win, winwidth/2, winlength/3, "YOU NEED TO REBOOT THE SYSTEM");

        mvwprintw(win, (winwidth/2)+2, winlength/10, "<┘ TO CONFIRM | GIVE THE ORDER '/REB00T_SYS.exe' : ");

        box(win, 0, 0);

        wrefresh(win);

        wgetnstr(win, c, 35);

    }

    noecho();

    if(ex == 3){ // this part manage the return in case of win or loose
        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU LOOSE");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);
        
        return 0;
    }

    else{

        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU WIN");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);

        return 1;
    }


        



}

int minigame2(WINDOW * win, int winwidth, int winlength){
     /*Function which is the second task of the game, its a simple function were the player move an arrow
    until she reach the right position. The function return 1 in case of a win or 0 if the player loose, she take 3 argument a window, his length and width*/

    int ch = 0;

    int vartuch = 0;

    int ex = 0;

    int wherestop = ((rand()%100)%41)+10; // Initialization of the position of the cursor

    wclear(win);

    mvwprintw(win, winwidth/2, winlength/3, "TASK 2");

    box(win, 0, 0);

    wrefresh(win);

    sleep(1);

    while (ex < 3)
    {
        wclear(win);
        mvwprintw(win, winwidth/2-2, winlength/3, "%d try remain", 3-ex);

        mvwprintw(win, winwidth/2, winlength/3, "BE PRECISE");

        for(int i = 0; i<50; i++){
            mvwprintw(win, (winwidth/2)+2, winlength/5+i, "-");
        }

        mvwprintw(win, (winwidth/2)+1, winlength/5+wherestop, "*");

        mvwprintw(win, (winwidth/2)+3, winlength/5+vartuch, "^");
        mvwprintw(win, (winwidth/2)+4, winlength/5+vartuch, "|"); // Window of the minigame display

        box(win, 0, 0);

        wrefresh(win);

        ch = getch();


        while (ch == KEY_RIGHT && vartuch < 50)  // The player can only go on the right direction and must continue to press the key right to continue the loop
        {
            

            vartuch++;


            wclear(win);
            mvwprintw(win, winwidth/2-2, winlength/3, "%d try remain", 3-ex);

            mvwprintw(win, winwidth/2, winlength/3, "BE PRECISE");

            for(int i = 0; i<50; i++){
                mvwprintw(win, (winwidth/2)+2, winlength/5+i, "-");
            }

            mvwprintw(win, (winwidth/2)+1, winlength/5+wherestop, "*");

            mvwprintw(win, (winwidth/2)+3, winlength/5+vartuch, "^");
            mvwprintw(win, (winwidth/2)+4, winlength/5+vartuch, "|"); // Window of the minigame display and manage the player arrow position

            box(win, 0, 0);
            
            ch = getch();

            wrefresh(win);
        }
        

        if(vartuch > wherestop){
            vartuch = 0;
            ex++;
        }

        if(vartuch == wherestop){
            ex = 4;
        }


    }

    if(ex == 4){ // Winning condition 
        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU WIN");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);

        return 1;
    }

    else{
        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU LOOSE");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);

        return 0;
    }
    
}

int minigame3(WINDOW * win, int winwidth, int winlength){
    /*Function which is the third task of the game, its a simple function simulate a plus or less game.
    The function return 1 in case of a win or 0 if the player loose , she take 3 argument a window, his length and width*/

    int ch = 0;

    char guess[4] = "NULL"; // variable who keep the player answer 

    int ex = 0;

    int mysterynb = (rand()%98)+1; //initialization of the mysterious number

    wclear(win);

    mvwprintw(win, winwidth/2, winlength/3, "TASK 3");

    box(win, 0, 0);

    wrefresh(win);

    sleep(1);

    echo();

    while(ex < 10){ // Loop which manage the plus or less algorithm


        wclear(win);

        mvwprintw(win, winwidth/2-2, winlength/3, "%d try remain", 10-ex);

        mvwprintw(win, winwidth/2, winlength/4, "GUESS THE MYSTERY NOMBER : ");

        box(win, 0, 0);

        wgetnstr(win, guess, 2);

        wrefresh(win);

        while (isInt(guess) != 1 && ex < 10){

            ex++;

            wclear(win);

            mvwprintw(win, winwidth/2-2, winlength/3, "%d try remain", 10-ex);

            mvwprintw(win, winwidth/2, winlength/4, "ARE YOU SERIOUS : ");

            box(win, 0, 0);

            wgetnstr(win, guess, 2);

            wrefresh(win);
            
        }

        if(atoi(guess) < mysterynb){

            wclear(win);

            mvwprintw(win, winwidth/2, winlength/3, "ITS BIGER");

            box(win, 0, 0);

            wrefresh(win);

            sleep(1);

            ex++;
        }

        else if(atoi(guess) > mysterynb){

            wclear(win);

            mvwprintw(win, winwidth/2, winlength/3, "ITS SMALLER");

            box(win, 0, 0);

            wrefresh(win);

            sleep(1);

            ex++;
        }

        else{

            wclear(win);

            mvwprintw(win, winwidth/2, winlength/4, "ITS THE RIGHT NUMBER !");

            box(win, 0, 0);

            wrefresh(win);

            sleep(1);

            ex = 12;

        }

    }
    
    noecho();

    if(ex == 12){
        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU WIN");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);

        return 1;
    }

    else{
        wclear(win);

        mvwprintw(win, winwidth/2, winlength/3, "YOU LOOSE");

        box(win, 0, 0);

        wrefresh(win);

        sleep(1);

        return 0;
    }

}

int minigame4(WINDOW * win, int winwidth, int winlength){

    /*Function which is the fourth task of the game, its a simple function who return 1 after execution, she take 3 argument a window, his length and width*/

    int ch = 0;

    int ex = 0;

    wclear(win);

    int load = 0;
    
    mvwprintw(win, winwidth/2, winlength/3, "TASK 4");
    
    box(win, 0, 0);
    wrefresh(win);
    sleep(1);
    
    for(int i=0;i<5;i++){ 


    wclear(win);
    box(win, 0, 0);
    mvwprintw(win, winwidth/2, (winlength/5), "DOWNLOADING THE STATION CRITIC FILES : %d/100 ", load);
    wrefresh(win);
    sleep(1);
    load+=25;
    }
    
    wclear(win);
    box(win, 0, 0);
    
    mvwprintw(win, (winwidth/2), winlength/3, "FILES SUCCESFULLY DOWNLOADED !");
   
    wrefresh(win);

    sleep(1);

    return 1;
    
}

int minigame5(WINDOW * win, int winwidth, int winlength){
	/*Function which is the fifth task of the game, it's a function which return 1 if the player
	successfully enter the good answer else 0, she take 3 argument a window, his length and width*/
    int ch = 0;

    int ex = 1; // Success condition
    
    int posx; //Positon x of the operation that will be print on the window
    
    int posy; //Position y of the operation that will be print on the window
    
    int a; // First value of the operation that will printed
    int b; // Second value of the operation that will printed

    int random_t; // Random amount of time when waiting for the next operation

    char result[3]; // Player answer variable
    int iresult; // Player answer converted to int	

    int ms_time = 800000; // Time in milliseconds where the operation will be displayed
    wclear(win);

    box(win, 0, 0);

    mvwprintw(win, winwidth/2, winlength/3, "TASK 5");    

    wrefresh(win);

    sleep(1);
    
    wclear(win);

    box(win, 0, 0);
   

    for(int i=0;i<3;i++){
    
    	ms_time -= 100000; // Next rouned the player will have less time to see the operation
    
    	random_t = ((rand()%100)%3)+1;
    
    	posx = ((rand()%100)%(winlength-10))+1;
    
    	posy = ((rand()%100)%(winwidth-2))+1;
    	
    	a = ((rand()%100)%11)+1;
    
    	b = ((rand()%100)%11)+1; //random drawing of values
    
    	wclear(win);

    	box(win, 0, 0);
    
        mvwprintw(win, winwidth/2, winlength/3, "Round %d", i+1); 

        wrefresh(win); 
        
        sleep(1);

        wclear(win); 

        box(win, 0, 0);
   
   	    mvwprintw(win, (winwidth/2)-1, winlength/3, "get ready !");   
   	
   	    wrefresh(win); 
   	
   	    sleep(1);
   	
   	    wclear(win);

    	box(win, 0, 0);
   	
   	    sleep(random_t);
   	
   	    mvwprintw(win, posy, posx, "%dx%d=?", a, b);  // display of the operation at the coordinate draw randomly
   	
   	    wrefresh(win); 
   	
   	    usleep(ms_time);
   	
   	    wclear(win);

    	box(win, 0, 0);
    	
    	echo();
    	
    	mvwprintw(win, winwidth/2, winlength/3, "Enter the result : "); 

        wrefresh(win);  
    	
    	wgetnstr(win, result, 3);
    	
    	noecho();
    	
        iresult = atoi(result);
        
        if(iresult == a*b){ // If the player input is correct the task continue
        
        	wclear(win);

    		box(win, 0, 0);
        
        	mvwprintw(win, winwidth/2, winlength/3, "Round success !");  
        	
        	wrefresh(win); 
        	
        	sleep(1); 
        
        }
        else{
        
        	wclear(win);

    		box(win, 0, 0);
        
        	mvwprintw(win, winwidth/2, winlength/3, "TASK FAILED !");  
        	
        	wrefresh(win); 
        	
        	sleep(1); 
        	
        	return 0;
        }
    	
    } 
    
    wclear(win);

    box(win, 0, 0);
       
    mvwprintw(win, winwidth/2, winlength/3, "TASK 5 COMPLETE !");  
        	
    wrefresh(win); 
        	
    sleep(1); 

    return 1;
	
}



int Inittask(WINDOW * win, int winwidth, int winlength, int taskeffectued, Player play){
    /*This function will manage the launch of the different tasks that the player will launch.
    If the task is successful the function return 1 else 0.
    She takes 5 argument a, a window her length and width and the the task identifier and the player*/

    int ex = -1;

    WINDOW * task = NULL;

    task = subwin(win, winlength/2 , winwidth-20, winlength/3, winwidth/8);

    if(task == NULL){
        printf("Error with task");
        exit(21);
    }

    if(taskeffectued == 0){ // This part manage which task will be lunched Depending on the task, enter parameters
        ex = (minigame1(task, winlength/2, winwidth-20));
    }

    if(play.playerStat.level >= 1){

        if(taskeffectued == 1){
            ex = (minigame2(task, winlength/2, winwidth-20));
        }

        if(taskeffectued == 2){
            ex = minigame3(task,  winlength/2, winwidth-20);
        }
    }
    else if(taskeffectued >= 1 && taskeffectued <= 2){ // Some task need the player to be at least lvl x

        wclear(task);

        mvwprintw(task, winlength/4, (winwidth-20)/3, "YOU NEED TO BE LVL 1");

        box(task, 0, 0);

        wrefresh(task);

        sleep(1);

        return 0;

    }

    if(play.playerStat.level >= 3){

        if(taskeffectued == 3){

            ex =(minigame4(task, winlength/2, winwidth-20));
        
        }

        if(taskeffectued == 4){

            ex = (minigame5(task, winwidth/2, winwidth-20));
        
        }
    }
    else if(taskeffectued >= 3 && taskeffectued <= 4){

        wclear(task);

        mvwprintw(task, winlength/4, (winwidth-20)/3, "YOU NEED TO BE LVL 3");

        box(task, 0, 0);

        wrefresh(task);

        sleep(1);

        return 0;

    }

    

    

    if(ex == 1){ //This part manage the return depending of win or loose during the minigame
        return 1;
    }
    else{
        return 0;
    }

    wclear(win);

    wrefresh(win);

}

void savefile(Player p, int countinv, int seed, int size_map_width, int size_map_length, char** map, int equipedshild, int equipedsword, int place, int place_before, int isininv, int nbtask, int taskeffectued, int mobkilled, int lootpicked, int lenght_max_room, int width_max_room, int tot_room, int count, int tot_door, Room * room, int timer, int count_room, int nb_room){
    FILE *save = fopen(p.name, "w");


    if (save == NULL) {
        printf("Not possible to open the save.\n");
        return;
    }

    // Saving data of the game in a file
    fprintf(save, "%d\n", countinv);
    fprintf(save, "%d\n", p.posy);
    fprintf(save, "%d\n", p.posx);
    fprintf(save, "%s\n", p.name);
    fprintf(save, "%d\n", p.live);
    fprintf(save, "%d\n", p.playerStat.tot_pv);
    fprintf(save, "%f\n", p.playerStat.pv);
    fprintf(save, "%d\n", p.playerStat.atck);
    fprintf(save, "%d\n", p.playerStat.def);
    fprintf(save, "%d\n", p.playerStat.level);
    fprintf(save, "%d\n", p.playerStat.dodge);
    fprintf(save, "%f\n", p.playerStat.exp);
    fprintf(save, "%d\n", p.playerStat.speed);
    fprintf(save, "%d\n", equipedshild);
    fprintf(save, "%d\n", equipedsword);
    fprintf(save, "%d\n", place);
    fprintf(save, "%d\n", place_before);
    fprintf(save, "%d\n", isininv);
    fprintf(save, "%d\n", nbtask);
    fprintf(save, "%d\n", taskeffectued);
    fprintf(save, "%d\n", mobkilled);
    fprintf(save, "%d\n", lootpicked);
    fprintf(save, "%d\n", lenght_max_room);
    fprintf(save, "%d\n", width_max_room);
    fprintf(save, "%d\n", tot_room);
    fprintf(save, "%d\n", tot_door);
    fprintf(save, "%d\n", count);
    fprintf(save, "%d\n", timer);
    fprintf(save, "%d\n", count_room);
    fprintf(save, "%d\n", nb_room);


    

    for (int i = 0; i < tot_room+1; ++i) {

        if(room[i].nb != -1 ){


        fprintf(save, "%d ", room[i].nb);
        fprintf(save, "%d ", room[i].length);
        fprintf(save, "%d ", room[i].width);
        fprintf(save, "%d ", room[i].nbevent);
        for (int j = 0; j < room[i].nbevent; ++j) {
            fprintf(save, "%d ", room[i].event[j].placex);
            fprintf(save, "%d ", room[i].event[j].placey);
            fprintf(save, "%d ", room[i].event[j].typeEvent);
        }
        for (int j = 0; j < 4; ++j) {
            fprintf(save, "%d ", room[i].nbdoor[j].howmuchroom);
            fprintf(save, "%d ", room[i].nbdoor[j].remote);
            fprintf(save, "%d ", room[i].nbdoor[j].wall);
            fprintf(save, "%d", room[i].nbdoor[j].pos);
        }
        fprintf(save, "\n");
        for (int j = 0; j < room[i].width+2; ++j) {
            for (int p = 0; p < room[i].length+2; ++p) {
                fprintf(save, "%d ", room[i].room[j][p]);
            }
            fprintf(save, "\n");
        }
        fprintf(save, "\n");
        }
    }


    // Saving the inventory
    for (int i = 0; i < 10; ++i) {
        fprintf(save, "%s ", p.inventory[i].name);
        fprintf(save, "%d ", p.inventory[i].type);
        fprintf(save, "%d ", p.inventory[i].typebuff);
        fprintf(save, "%d ", p.inventory[i].buff);
    }
    fprintf(save, "\n");

    
    fprintf(save, "%d\n", seed);
    fprintf(save, "%d\n", size_map_width);
    fprintf(save, "%d\n", size_map_length);

    // Saving the map
    for (int i = 0; i < size_map_width+1; ++i) {
        for (int j = 0; j < size_map_length+1; ++j) {
            fprintf(save, "%d ", map[i][j]);
        }
        fprintf(save, "\n");
    }



    fclose(save);
}


void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth, int maxroomwidth, int maxroomlength, int minroom, int isloaded, char * name ){
/*Main fonction of the game start each game initialize each value*/


    wclear(win);

    int exitCond = -1;

    int null = 0;

    int seed;
    

    int ch = 0;

    FILE * save = NULL;

    if(isloaded == 1){
        save = fopen(name, "r");

        if (save == NULL) {
            printf("Not possible to open the save.\n");
            return;
        }
    }

    if(isloaded != 1){
        int seed = giveSeed(win, winwidth, winlength);
        srand(seed);
    }

    



    

    int newXp = -1;
    
    Mob m;
    Item it;

    
    int countinv = 0;//How much place in the inventory remain


    Player j;

    initPlayer(&j ,j.inventory);


    int equipedsword = -1;
    int equipedshild = -1;

    int place = 0;
    int place_before = -1;


    int isininv = FALSE; //For place the item in the inventory

    int nbtask = 0;
    int taskeffectued = 0;

    int mobkilled = 0;

    int lootpicked = 0;

    int lenght_max_room =  maxroomlength;
    int width_max_room = maxroomwidth;

    int tot_room = ((rand()%100)%10)+minroom;
    int tot_door = tot_room -1;


    int count = 0;

    int timer = 900;

    int countroom = 0;

    int nb_door = 4;

    if(isloaded == 1){
        fscanf(save, "%d", &countinv);
        fscanf(save, "%d", &j.posy);
        fscanf(save, "%d", &j.posx);
        fscanf(save, "%s", j.name);
        fscanf(save, "%d", &j.live);
        fscanf(save, "%d", &j.playerStat.tot_pv);
        fscanf(save, "%f", &j.playerStat.pv);
        fscanf(save, "%d", &j.playerStat.atck);
        fscanf(save, "%d", &j.playerStat.def);
        fscanf(save, "%d", &j.playerStat.level);
        fscanf(save, "%d", &j.playerStat.dodge);
        fscanf(save, "%f", &j.playerStat.exp);
        fscanf(save, "%d", &j.playerStat.speed);
        fscanf(save, "%d", &equipedshild);
        fscanf(save, "%d", &equipedsword);
        fscanf(save, "%d", &place);
        fscanf(save, "%d", &place_before);
        fscanf(save, "%d", &isininv);
        fscanf(save, "%d", &nbtask);
        fscanf(save, "%d", &taskeffectued);
        fscanf(save, "%d", &mobkilled);
        fscanf(save, "%d", &lootpicked);
        fscanf(save, "%d", &lenght_max_room);
        fscanf(save, "%d", &width_max_room);
        fscanf(save, "%d", &tot_room);
        fscanf(save, "%d", &tot_door);
        tot_room++;
        fscanf(save, "%d", &count);
        fscanf(save, "%d", &timer);
        fscanf(save, "%d", &countroom);
        fscanf(save, "%d", &nb_door);
    }



    Room * room = NULL;

    room = calloc(tot_room, sizeof(Room));

    if(room == NULL){
        printf("Error with the allocation of the room");
        exit(6);
    }

    if(isloaded == 1){

        int loadroom = 0;

        for(int i = 0; i < countroom ; i++){

            fscanf(save, "%d", &loadroom);
            room[loadroom].nb = loadroom;
            fscanf(save, "%d", &room[loadroom].length);
            fscanf(save, "%d", &room[loadroom].width);
            fscanf(save, "%d", &room[loadroom].nbevent);

            for(int j = 0; j<room[loadroom].nbevent ;j++){
                fscanf(save, "%d", &room[loadroom].event[j].placex);
                fscanf(save, "%d", &room[loadroom].event[j].placey);
                fscanf(save, "%d", &room[loadroom].event[j].typeEvent);
            }

            for(int j = 0; j<4; j++){

                fscanf(save, "%d", &room[loadroom].nbdoor[j].howmuchroom);
                fscanf(save, "%d", &room[loadroom].nbdoor[j].remote);
                fscanf(save, "%d", &room[loadroom].nbdoor[j].wall);
                fscanf(save, "%d", &room[loadroom].nbdoor[j].pos);

            }

            int temp = 0;

            for (int j = 0; j < room[i].width+2; ++j) {
                for (int p = 0; p < room[i].length+2; ++p) {
                    fscanf(save, "%d", &temp);
                    room[i].room[j][p] = temp;
                }
                fscanf(save, "%d", &null);
            }

            fscanf(save, "%d", &null);

        }

        for(int i = 0; i<10; i++){

            fscanf(save, "%s", j.inventory[i].name);
            fscanf(save, "%d", &j.inventory[i].type);
            fscanf(save, "%d", &j.inventory[i].typebuff);
            fscanf(save, "%d", &j.inventory[i].buff);

        }
        fscanf(save, "%d", &null);

    }
    
    
    
    int size_map_width = (((tot_room+2) * width_max_room) * 2);
    int size_map_length = ((tot_room+2)*lenght_max_room)*2;

    if(isloaded == 1){
        fscanf(save, "%d", &seed);
        srand(seed);
        fscanf(save, "%d", &size_map_width);
        fscanf(save, "%d", &size_map_length);
    }




    char ** map = NULL;

    map = calloc(size_map_width+1 ,sizeof(char*));

    if(map == NULL){
        printf("Error with the allocation of the map");
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

    if(isloaded == 1){
        int temp = 0;
        for (int i = 0; i < size_map_width+1; ++i) {
            for (int j = 0; j < size_map_length+1; ++j) {
                fscanf(save, "%d", &temp);
                map[i][j] = temp;
            }
            fscanf(save, "%d", &null);
        }
    }
    


    float dividepv = j.playerStat.pv/(j.playerStat.tot_pv); 

    int maxlvl = 20;

    

  
    
    if(isloaded != 1){
        
        j.name = createName(win, winwidth, winlength);

        for(int i = 0; i<10; i++){
            j.inventory[i].type = -1;
            j.inventory[i].name[0] = '\0';
            j.inventory[i].buff = -1;
            j.inventory[i].typebuff = -1;
        }



    

        for(int i = 0; i<tot_room; i++){
            room[i].nb = -1;
            room[i].nbdoor = NULL;
        }

    
     

    
        
        j.posx = 0;
        j.posy = 0;
    }
    
    tot_room -= 1;

    


    


    
    
    if(isloaded != 1){
        createRoom(room+place ,width_max_room, lenght_max_room, tot_room ,-1 ,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length, &nbtask);
        countroom++;
    }






    placeRoom(map, size_map_width, size_map_length, -1, room[place], j.posx, j.posy, winwidth, winlength);

    /*Function which manage the placement and size of room*/

    

    int t = (time(NULL));

    int stop = 0;

    int te = time(NULL);


    Time tim;

    while(exitCond == -1){




        te = time(NULL);



        tim = createTime(timer - (te - t - stop));


        mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)-13, "匚ㄖ丂爪丨匚  ㄚㄖ几ᗪ乇尺\n");
        mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)+40, "Task : %d/5", taskeffectued);
        mvwprintw(stdscr, winposy-1, winposx+1, "CURRENT SEED : %d", seed);
        mvwprintw(stdscr, winlength+winposy+1, winposx+1, "TIME : %d min, %d sec", tim.min, tim.sec);
        mvwprintw(stdscr, winlength+winposy+1, winposx+30, "HP : %.0f/%d", j.playerStat.pv, j.playerStat.tot_pv);

        dividepv = j.playerStat.pv/j.playerStat.tot_pv; 

        mvwprintw(stdscr, winlength+winposy+1, winposx+43,"[");

        for(int i = 0; i < 10 ; i++){
            if(i < ((dividepv)*10)){
                mvwprintw(stdscr, winlength+winposy+1, winposx+44+i,"♥");
            }
            else{
                mvwprintw(stdscr, winlength+winposy+1, winposx+44+i,"-");
            }
            
        }

        printw("]");


        mvwprintw(stdscr, winlength+winposy+1, winposx+winwidth-35, "LVL : %d ", j.playerStat.level);

        mvwprintw(stdscr, winlength+winposy+1, winposx+winwidth-25,"[");

        for(int i = 0; i < 10 ; i++){
            if(i < (((j.playerStat.exp - (j.playerStat.level * 100)) / 100) *10)){
                mvwprintw(stdscr, winlength+winposy+1, winposx+winwidth-24+i,"💡");
            }
            else{
                mvwprintw(stdscr, winlength+winposy+1, winposx+winwidth-24+i,"-");
            }
            
        }

        printw("]");


        mvwprintw(stdscr, 2,  winwidth+winposx+2, "name : %s", j.name);
        mvwprintw(stdscr, 4, winwidth+winposx+2, "← TO GO LEFT");
        mvwprintw(stdscr, 6,  winwidth+winposx+2, "→ TO GO RIGHT");
        mvwprintw(stdscr, 8,  winwidth+winposx+2, "↑ TO GO UP");
        mvwprintw(stdscr, 10,  winwidth+winposx+2, "↓ TO GO DOWN");

        mvwprintw(stdscr, 14,  winwidth+winposx+2, "'i' TO OPEN AND CLOSE");
        mvwprintw(stdscr, 15,  winwidth+winposx+2, "THE INVENTORY");

        mvwprintw(stdscr, 17,  winwidth+winposx+2, "'Escape' TO OPEN AND CLOSE");
        mvwprintw(stdscr, 18,  winwidth+winposx+2, "THE PAUSE MENU");

        mvwprintw(stdscr, 22,  winwidth+winposx+2, "<┘ TO INTERACT");

        


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
                if(room[place].nbdoor == NULL){       

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
                    else if (tot_door == 0) {
                        nb_door = 0;
                    }

                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,0,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length, &nbtask) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        room[place].nb = -1;
                        place = place_before;
                        j.posy -= 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 2, room[place], j.posx, j.posy, winwidth, winlength);
                        countroom++;
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
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild, maxlvl) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        mobkilled++;
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

                    if(countinv >= 10){
                        invIsFull(winwidth, winlength, win, &stop);
                        j.posy--;
                    }

                    else{
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        lootpicked++;
                        j.posy--;
                    }


                    
                }
                else{
                    j.posy--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    if(Inittask(win, winwidth, winlength, taskeffectued, j) == 1){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        taskeffectued++;
                    }
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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,2,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length, &nbtask) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posy += 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 0, room[place], j.posx, j.posy, winwidth, winlength);
                        countroom++;
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
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild, maxlvl) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        mobkilled++;
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

                    if(countinv >= 10){
                        invIsFull(winwidth, winlength, win, &stop);
                        lootpicked++;
                        j.posy++;
                    }

                    else{
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        j.posy++;
                    }

                    
                }
                else{
                    j.posy++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){

                    if(Inittask(win, winwidth, winlength, taskeffectued, j) == 1){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        taskeffectued++;
                    }

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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room,1,nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length, &nbtask) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posx+=3;

                    }
                    else{
                      placeRoom(map, size_map_width, size_map_length, 3, room[place], j.posx, j.posy, winwidth, winlength);  
                      countroom++;
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
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild, maxlvl) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        mobkilled++;
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

                    if(countinv >= 10){
                        invIsFull(winwidth, winlength, win, &stop);
                        lootpicked++;
                        j.posx++;
                    }

                    else{
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        j.posx++;
                    }

                    
                }
                else{
                    j.posx++;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    if(Inittask(win, winwidth, winlength, taskeffectued, j) == 1){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        taskeffectued++;
                    }
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
                    if(createRoom(room+place,width_max_room, lenght_max_room, tot_room, 3, nb_door, tot_door, place, place_before, &count, j.posx, j.posy, map, size_map_width, size_map_length, &nbtask) == 0){
                        phantomishere(winwidth, winlength, win, &stop);
                        place = place_before;
                        j.posx -= 3;

                    }
                    else{
                        placeRoom(map, size_map_width, size_map_length, 1, room[place], j.posx, j.posy, winwidth, winlength);
                        countroom++;
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
                    if(fight(m, &j, win, winlength, winwidth, &stop, &equipedsword, &equipedshild, maxlvl) == 2){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        mobkilled++;
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

                    if(countinv >= 10){
                        invIsFull(winwidth, winlength, win, &stop);
                        j.posx--;
                    }

                    else{
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        lootpicked++;
                        j.posx--;
                    }

                    
                }
                else{
                    j.posx--;
                }
            }

            if(map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] == 'T'){
                if(doyouwantfight(winwidth, winlength, win, &stop, 2) == 1){
                    if(Inittask(win, winwidth, winlength, taskeffectued, j) == 1){
                        map[(size_map_width/2) + j.posy][(size_map_length/2) + j.posx] = 0;
                        taskeffectued++;
                    }
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
            
            player_lvl_up(&j, maxlvl);

        }


        if(timer - (te - t - stop) <= 0){
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
            
            
            exitCond = 2;
        }

        if(taskeffectued == 5 || mobkilled == 10 || lootpicked == 15){

            clear();
            box(win, 0, 0);
            wrefresh(win);
            mvwprintw(win, winlength/2, (winwidth/2)-12, "<┘ TO LEAVE | YOU WIN !");
            wrefresh(win);

            ch = 0;

            while (ch != ENTER)
            {
                ch = getch();
            }
            
            
            exitCond = 2;

        }

     


        wclear(win);
        wclear(stdscr);


        

    }

    if(exitCond != 2){
       //savefile(j, countinv, seed, size_map_width, size_map_length, map, equipedshild, equipedsword, place, place_before, isininv, nbtask, taskeffectued, mobkilled, lootpicked, lenght_max_room, width_max_room, tot_room, count, tot_door, room, timer-(te-t-stop), countroom, nb_door);
    }


    quitAnim(win, winlength, winwidth);

}