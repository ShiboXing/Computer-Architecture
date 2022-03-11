
CC=g++
CFLAGS=-std=c++20 -c -Wall -pedantic -g
OBJS=main.o components/fetch.o structures/rename_map.o structures/ins_table.o structures/instruction.o

default: $(OBJS)
	$(CC) -o main main.o fetch.o rename_map.o ins_table.o
clean: 
	rm *.o main
%.o:	%.cpp
	$(CC) $(CFLAGS) $*.cpp	
