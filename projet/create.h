#if !defined(__CREATE__)

	#define __CREATE__
	#define NB_MOVE 4
	#define ENTER 10
	#include <ncurses.h>
	
    int giveSeed(WINDOW * win, int winwidth, int winlength);
	void giveName(WINDOW * win, char ch[], int winlength, int winwidth);
	char* createName(WINDOW * win, int winwidth, int winlength);
	Time createTime(int time);
	
	
#endif