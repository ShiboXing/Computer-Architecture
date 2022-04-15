
CC=g++
CFLAGS=-std=c++11 -c -Wall -pedantic -g
OBJS= \
main.o \
structures/instruction.o \
structures/res_record.o \
components/fetch.o \
components/decoder.o \
components/ins_table.o \
components/res_station.o \
components/CDB.o \
components/ROB.o\
components/back_writer.o

default: $(OBJS)
	$(CC) -o main $(OBJS)
clean: 
	find . -type f -name "*.o" -delete
	find . -type f -name "*.h.gch" -delete
	rm main
%.o:	%.cpp
	$(CC) $(CFLAGS) -o $*.o $*.cpp