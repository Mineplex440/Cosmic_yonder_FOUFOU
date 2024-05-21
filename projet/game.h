#if !defined(__GAME__)

	#define __GAME__
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
	
	void iscolide(char ** map, int map_width, int map_length, Room * actual_room,int posx, int posy, int pressed);
	void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth);
	void loadSave(WINDOW * win);
	void setting(WINDOW * win);
	
	
	
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
