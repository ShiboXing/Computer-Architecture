
CC=g++
CFLAGS=-std=c++11 -c -Wall -pedantic -g

default: main.o fetch.o
	$(CC) -o main main.o fetch.o
clean: 
	rm *.o main
%.o:	%.cpp
	$(CC) $(CFLAGS) $*.cpp	

