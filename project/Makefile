
CC=g++
CFLAGS=-std=c++11 -c -Wall -pedantic -g
OBJS=main.o components/fetch.o structures/rename_map.o structures/instruction.o structures/ins_table.o structures/instruction.o

default: $(OBJS)
	$(CC) -o main main.o fetch.o rename_map.o ins_table.o instruction.o
debug: $(OBJS)
	#
clean: 
	rm *.o main
%.o:	%.cpp
	$(CC) $(CFLAGS) $*.cpp	
