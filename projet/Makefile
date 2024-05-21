all : exe

room.o : room.c 
	gcc -o room.o -c room.c -lncursesw

func.o : func.c
	gcc -o func.o -c func.c -lncursesw

game.o : game.c room.h func.h
	gcc -o game.o -c game.c -lncursesw
	
menu.o : menu.c 
	gcc -o menu.o -c menu.c -lncursesw
	
create.o : create.c func.h
	gcc -o create.o -c create.c -lncursesw
	
main.o : main.c game.h
	gcc -o main.o -c main.c -lncursesw
	
exe : main.o room.o game.o menu.o func.o create.o
	gcc -o exe main.o game.o menu.o room.o func.o create.o -lncursesw
	
clean :
	rm -f *.o
	rm -f exe
