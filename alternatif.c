#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <string.h>

#define ENTER 10

typedef struct{
    int hours;
    int min;
    int sec;
}Time; // Time structure which will serve in the game window

typedef struct{
    int posx;
    int posy;
    char * name;
    int pv;
    int exp;
    int level;
    char ** inventory;
}Player; // Playable player structure

typedef struct{
    int verif;
    int wall;
    int pos;
}Door; // Door structure which will appear in romm

typedef struct{
    int length;
    int width;
    int nbevent;
    Door * nbdoor;
    char **room;
}Room; // Room structure which well serve for each rooms int the game



int isInt(char * ch){ 
	/* Function which verify if the input is between the char '0' and '9' then she return 0 (used in the function giveSeed). She only take a char pointer in argument */
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
	/* Function which ask the player to input the seed he want. The function take three arguments : 
	wINDOW pointer, the width and lenght of the window (integer)*/

    char cseed[7]; // The seed is included between a string of 1 to 6 characters
    int seed = -1; // Default value

    echo(); // prevents keyboard input from being placed on the window
    //nodelay(stdscr, FALSE);

    mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");        
    mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE THE SEED : "); // Menu display
    wrefresh(win);
    wgetnstr(win, cseed, 6); // Input of the char cseed with a maximum of 6 characters
    wclear(win); // removeable


    while(isInt(cseed)!=1 || atoi(cseed) > 999999 || atoi(cseed) <= 0){
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE A CORRECT SEED, BETWEEN 1 AND 999999 : "); // Menu display
        usleep(40000);
        wrefresh(win); 
        wgetnstr(win, cseed, 6);
        wclear(win); // this line and wrefresh, manage the refresh of the screen
    }

    noecho(); // Stop the echo
    //nodelay(stdscr, TRUE);

    seed = atoi(cseed); // convert the cseed into a integer variable

    return seed;
}

















char* createName(WINDOW * win, int winwidth, int winlength){
	/*Function which ask the player which name he want to input for his save. 
	The function take three arguments : wINDOW pointer, the width and lenght 
	of the window (integer).*/
	
	
    char ch[36]; // 36 character string for the save's name

    int c = 0;

    
    //nodelay(stdscr, FALSE);
    wclear(win); //Clear the window to prepare new display 

    echo();
    

    mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
    mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE YOUR NAME : ");// Menu display
    usleep(40000);
    wrefresh(win);
    wgetnstr(win, ch, 35); // Name entry

    noecho();    

    while(ch[0] == '\0'){ // While no valid name has been input the loop ask again

    echo();
    mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
    mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE A CORRECT NAME : ");// Menu display
    usleep(40000);
    wrefresh(win);
    wgetnstr(win, ch, 35);
    noecho(); 

    }

    do{ 

        wclear(win);
        mvwprintw(win, (winlength/2)-1, (winwidth/2)-20, "[r] TO RESET |");
        mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
        mvwprintw(win, (winlength/2), (winwidth/2)-5, " YOU CONFIRM THE PSEUDO : %s ?", ch);// Menu display
        usleep(40000);

        wrefresh(win);

        c = getch(); // Waits for the player to enter an input to continue
        usleep(40000);

        if(c == 'r'){ // If the player press r restarts the name entry procedure

            echo();
            wclear(win);
            mvwprintw(win, (winlength/2), (winwidth/2)-20, "<┘ TO CONFIRM |");
            mvwprintw(win, (winlength/2), (winwidth/2)-5, " GIVE YOUR NAME : ");
            usleep(40000);
            wrefresh(win);
            usleep(40000);
            wgetnstr(win, ch, 99);
            noecho();
        }

    }while(c != 10 || ch[0] == '\0'); // We keep looping if the input c insn't the char \n or ch[0] == \0

    int namesize = strlen(ch); // Creating a variable which contain the name size

    char * name = NULL;

    name = malloc((namesize+1)*sizeof(char)); // dynamic allocation of the name pointer which will be returned

    if(name == NULL){
        printf("problem with the allocation of the name\n");
        exit(2);
    }

    for(int i = 0; i<namesize+1; i++){ // Filling the name pointer due to the ch 
        name[i] = ch[i];
    }

    wclear(win);
    //nodelay(stdscr, TRUE);

    return name;

}














Time createTime(int time){
	/*Function which will manage the playing time by returning it. She take one argument a interger 'time' which is considered in second*/
	
    Time tim; // Creation of a variable Time (struct imported from the time.h library) tim

    if(time >= 60){ // this part of the code will take care of dividing the seconds into minutes and hours
        tim.sec = time%60;
        time = time / 60;

        if(time >= 60){
            tim.min = time % 60;
            tim.hours = time /60;
        }
        else{
            tim.hours = 0;
            tim.min = time;
        }
    }
    else{
        tim.hours = 0;
        tim.min = 0;
        tim.sec = time;
    }

    return tim;
}








Door placeNbDoor(){ 
	/**/
    Door d;
    return d;
}






Room createRoom(int nb_porte){
	/*Function which randomly create room and return it, she take one integer argument which is the number of door in that room*/
	
    Room law; // delaration of the room

    law.length = (rand()%7)+3; //random pick of the room's length
    law.width = (rand()%7)+3;  //random pick of the room's width

    law.nbdoor = NULL; 

    law.nbdoor = malloc(sizeof(Door)*nb_porte); // allocates the necessary space for the doors in memory

    if(law.nbdoor == NULL){ // Memory assertion 
        printf("Error with the door allocation");
        exit(3);
    }

    for(int i = 0; i<nb_porte; i++){ // This part manage de random placement of the door in the room
        law.nbdoor[i] = placeNbDoor();
    }

    law.nbevent = (rand()%4)+1; // Random pick of the number of event that are in the room

    law.room = NULL;




}










void startagame(WINDOW * win, int winposx, int winposy, int winlength, int winwidth){
    /*Function which manage de startting game procedure, she takes 5 argument, the window, an integer position x and an integer position y
    , an integer window length and another integer window width*/

    wclear(win);
    

    int ch = 0;

    int seed = giveSeed(win, winwidth, winlength); // Collecting the seed

    srand(seed); // initialization of the seed

    Player j; // player declaration

    j.name = createName(win, winwidth, winlength); // Player creation 





	
    int t = (time(NULL));

    int te = time(NULL);

    Time tim;




    while(ch != 10){


        te = time(NULL);
        tim = createTime(te-t); // The soustraction between t an te manage to have a time which start to 0 seconde    
        mvwprintw(stdscr, winposy-1, winposx+(winwidth/2)-13, "匚ㄖ丂爪丨匚  ㄚㄖ几ᗪ乇尺\n");
        mvwprintw(stdscr, winposy-1, winposx+1, "CURRENT SEED : %d", seed);
        mvwprintw(stdscr, winlength+winposy+1, winposx+1, "TIME : %d h, %d min, %d sec", tim.hours, tim.min, tim.sec); // Game window display 


        box(win, 0,0); // This function draws the window border

        // PLACE ROOM...
	usleep(40000);
        wrefresh(win);


        

        
        ch = getch(); // If the player press enter then the loop stop
        usleep(40000);




        wclear(win);
        wclear(stdscr);


        

    }




}














void loadSave(WINDOW * win){
	/**/
    wclear(win);
}






















void setting(WINDOW * win){
	/**/
    wclear(win);
}

















void printMenu(WINDOW *win,int x, int y, int beg, int space){
	/*This funciton manage the menu display, she take 5 arguments, the window, two integer x and y, 
	an integer beg, an integer space which is the space between two line in the menu */

    mvwprintw(win, y, x+2, "匚ㄖ丂爪丨匚\n");

    mvwprintw(win, y+1, x+2, "ㄚㄖ几ᗪ乇尺\n");

    mvwprintw(win, y+beg, x,"PLAY\n");

    mvwprintw(win, y+(beg+(space)), x,"SAVES\n");

    mvwprintw(win, y+(beg+(space*2)),x,"SETTINGS\n");

    mvwprintw(win, y+(beg+(space*3)), x,"QUIT\n"); // Menu diplay managed by the arguments
}





















void startAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space){
	/*This funciton manage the start animation, she take 9 arguments, the window, the window length and widht
	 which are integers, two integers posx and posy here the coordonate of the arrow, two integer x and y, 
	an integer beg, an integer space which is the space between two line in the menu */

    for(int i = 0; i<winlength; i++){
    
    mvwprintw(win, posy, posx, "-->");
        
    printMenu(win, x, y, beg, space);

    box(win, 0,0);

    for(int j = 1; j<winlength-i-1; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-"); // This double loop manage the star animation curtain up and down
        }
    }
	usleep(30000);
    wrefresh(win);
    usleep(30000);
    wclear(win);
    }
    
}

void quitAnim(WINDOW * win, int winlength, int winwidth, int posx, int posy, int x, int y, int beg, int space){
	/*This funciton manage the quit animation, she take 9 arguments, the window, the window length and widht
	 which are integers, two integers posx and posy here the coordonate of the arrow, two integer x and y, 
	an integer beg, an integer space which is the space between two line in the menu */


    for(int i = 0; i<winlength; i++){
    
    mvwprintw(win, posy, posx, "-->");
        
    printMenu(win, x, y, beg, space);

    box(win, 0,0);

    for(int j = 1; j<i; j++){
        for(int l = 1; l<winwidth-1;l++){
            mvwprintw(win, j, l,"-"); // This double loop manage the star animation curtain down and up
        }
    }
    usleep(30000);
    wrefresh(win);
    usleep(30000);
    wclear(win);
    }
}





















void showMenu(WINDOW *win, int winlength, int winwidth, int winposx, int winposy){
	/*This function will manage the menu display while the game is running,
	she has 5 argumments, the window, the windows length and width which are two 
	integers, two intergers posx and posy*/
    

    int chr = 0;

    

    int x = (winwidth/2)-10;
    int y = 2;
    
    
    int space = 4;
    int beg = y+3; 


    int posx, posy;
    posx = (winwidth/3)-5;
    posy = beg+y; // Declaration of variable to manage the display 
    
    startAnim(win, winlength, winwidth, posx, posy, x, y, beg, space); // Initialazing the game start process

    wclear(win);

    while(posy != y+(beg+(3*space)) || chr != ENTER ){ // Starting the game by entering the menu we can exit this loop by pressing enter at the y+(beg+(3*space)) (Quit) position

        mvwprintw(win, posy, posx, "-->");
        
        printMenu(win, x, y, beg, space);

        
        box(win, 0,0);
        usleep(40000);
        wrefresh(win);

        chr = getch();
        usleep(40000);

        wclear(win);
        
	/*This part of the code manage the movement and the entry in the different part of the menu*/
	
        if (chr == ENTER && posy == y+beg){ // Start
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            startagame(win, winposx, winposy, winlength, winwidth);
        }
        if (chr == ENTER && posy == y+(beg+(space))){ // Save
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            loadSave(win);
        }
        if (chr == ENTER && posy == y+(beg+(space*2))){ // Setting
            quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);
            setting(win);
        }


        if(chr == KEY_UP && posy > y+beg){ // moving the arrow up
            posy-= space;
        }
        if (chr == KEY_DOWN && posy < y+(beg+(space*3))){ // moving the arrow down
            posy+= space;
        } 


    }

    quitAnim(win, winlength, winwidth, posx, posy, x, y, beg, space);

}


//test

int main(){

    int ch = 0; 
    setlocale(LC_CTYPE,""); // Allow the print to use new font character
    
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();


    int winposx = 5;
    int winposy = 2;
    int winlength = 30;
    int winwidth = 100; // window variable declarationS



    WINDOW * win = NULL;

    win = subwin(stdscr,winlength, winwidth, winposy, winposx); Creation of the window 

    if (win == NULL){
        printf("allocation problem, try to modify the size of your terminal\n");
        exit(1);
    }


    showMenu(win, winlength, winwidth, winposx, winposy); // Starting the game


    
    endwin();

    return 0;
}








