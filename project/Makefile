
CC=g++
CFLAGS=-std=c++11 -c -Wall -pedantic -g
OBJS=main.o components/fetch.o components/decoder.o structures/instruction.o structures/ins_table.o 

default: $(OBJS)
	$(CC) -o main $(OBJS)
clean: 
	find . -type f -name "*.o" -delete
	rm main
%.o:	%.cpp
	$(CC) $(CFLAGS) $*.cpp -o $*.o