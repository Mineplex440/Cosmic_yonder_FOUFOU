#if !defined(__CREATE__)

	#define __CREATE__
	#define NB_MOVE 4
	#define ENTER 10
	#include <ncurses.h>
	
	void initPlayer(Player * p,Item * inven);
	Item initItem();
	Mob initMob(Player p);
	void player_lvl_up(Player * p, int maxlvl);
	void invIsFull(int winwidth, int winlength, WINDOW * win, int * stop);
	int giveSeed(WINDOW * win, int winwidth, int winlength);
	void giveName(WINDOW * win, char ch[], int winlength, int winwidth);
	char* createName(WINDOW * win, int winwidth, int winlength);
	Time createTime(int time);
	void phantomishere(int winwidth, int winlength, WINDOW * win, int * stop);
	
	
#endif