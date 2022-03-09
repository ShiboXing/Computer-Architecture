
CC=g++
CFLAGS=-std=c++20 -c -Wall -pedantic -g

default: main.o components/fetch.o structures/rename_map.o structures/ins_table.o
	$(CC) -o main main.o fetch.o rename_map.o ins_table.o
clean: 
	rm *.o main
%.o:	%.cpp
	$(CC) $(CFLAGS) $*.cpp	

