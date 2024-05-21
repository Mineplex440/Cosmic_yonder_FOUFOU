game : main.o player.o
	gcc -o game main.o player.o -lncursesw

main.o : main.c
	gcc -o main.o -c main.c -lncursesw

player.o : player.c
	gcc -o player.o -c player.c -lncursesw
