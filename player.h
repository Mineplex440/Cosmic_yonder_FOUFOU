#if !defined(__PLAYER__H__)

	#define __PLAYER__H__
	#define NB_MOVE 4
	#define ENTER 10
	#include <ncurses.h>
	
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
	
	int isInt(char * ch);
	int giveSeed(WINDOW * win, int winwidth, int winlength);
	void giveName(WINDOW * win, char ch[], int winlength, int winwidth);
	char* createName(WINDOW * win, int winwidth, int winlength);
	Time createTime(int time);
	void iscolide(char ** map, int map_width, int map_length, Room * actual_room,int posx, int posy, int pressed);
	Door * placeNbDoor(int realtot_door, int pressed, int previous_room, int length, int width, int tot_door, int nb_door, int * count);
	Room createRoom(int width_max_room, int lenght_max_room,int realtot_door, int pressed, int nb_door, int tot_door, int nb_room, int previous_room,int * count, int posx, int posy, char ** map, int map_width, int map_length);
	void printmap(WINDOW * win, int winlength, int winwidth, char ** map, int posy, int posx, int mapwidth, int maplength);
	void placeRoom(char ** map, int map_width, int map_length, int pressed, Room actual_room, int posx, int posy, int winwidth, int winlength);
	void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth);
	void loadSave(WINDOW * win);
	void setting(WINDOW * win);
	void printMenu(WINDOW *win,int x, int y, int beg, int space);
	void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space);
	void quitAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space);
	void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy);
	
	
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
