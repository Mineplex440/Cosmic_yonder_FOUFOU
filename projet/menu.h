#if !defined(__MENU__)

	#define __MENU__
	#define NB_MOVE 4
	#define ENTER 10
	#include <ncurses.h>
	
	void printMenu(WINDOW *win,int x, int y, int beg, int space);
	void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space);
	void quitAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space);
	void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy);
	
	
#endif
