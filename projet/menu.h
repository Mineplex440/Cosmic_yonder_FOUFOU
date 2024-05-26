#if !defined(__MENU__)

	#define __MENU__
	#define NB_MOVE 4
	#define ENTER 10
	#include <ncurses.h>

	void quitAnim(WINDOW * win, int winlength, int winwidth);
	int pauseMenu(int winwidth, int winlength, int winposx, int winposy,WINDOW * win, int * stop);
	void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy, int * maxroomwidth, int * maxroomlength, int * minroom);
	void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space);
	void printMenu(WINDOW *win,int x, int y, int beg, int space);
	void setting(WINDOW * win, int winwidth, int winlendth ,int * maxroomwidth, int * maxroomlength, int * minroom);
	void printinv(int width, int length, WINDOW * win, WINDOW * showstat,Item * inventory, int y, int x, int * equipedsword, int * equipedshield);
	void interactwithobject(WINDOW * win, int * equipedsword, int * equipedshield, int y, int x, Player * play, Item * inventory);
	void openinventory(int winwidth, int winlength, WINDOW * win, Item * inventory, Player * pla, int * stop, int * equipedsword, int * equipedshield);
	void printItem(int winwidth, int winlength, WINDOW * win, int * stop, Item i);
	
	
#endif
