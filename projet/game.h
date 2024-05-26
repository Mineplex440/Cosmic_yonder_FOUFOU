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
		//int nbmove;
		float power;
		float accuracy;
	}Spell;

	typedef struct{
		float pv;
		int tot_pv;
		float exp;
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
	
	
	
	
	
	int doyouwantchangeroom(int winwidth, int winlength, WINDOW * win, int * stop);
	int doyouwantfight(int winwidth, int winlength, WINDOW * win, int * stop, int type_event);
	int fight(Mob mob, Player * player, WINDOW * win, int winlength, int winwidth, int * stop, int * equipedsword, int * equipedshield, int maxlvl);
	void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth, int maxroomwidth, int maxroomlength, int minroom, int isloaded, char * name );
	void savefile(Player p, int countinv, int seed, int size_map_width, int size_map_length, char** map, int equipedshild, int equipedsword, int place, int place_before, int isininv, int nbtask, int taskeffectued, int mobkilled, int lootpicked, int lenght_max_room, int width_max_room, int tot_room, int count, int tot_door, Room * room, int timer, int count_room, int nb_room);
	int Inittask(WINDOW * win, int winwidth, int winlength, int taskeffectued, Player play);
	int minigame5(WINDOW * win, int winwidth, int winlength);
	int minigame4(WINDOW * win, int winwidth, int winlength);
	int minigame3(WINDOW * win, int winwidth, int winlength);
	int minigame2(WINDOW * win, int winwidth, int winlength);
	int minigame1(WINDOW * win, int winwidth, int winlength);
	
	
	
	
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
