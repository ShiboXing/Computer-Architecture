
CC=g++
CFLAGS=-std=c++14 -c -Wall -pedantic -g
OBJS= \
main.o \
fetch.o \
structures/instruction.o \
structures/res_record.o \
components/decoder.o \
components/ins_table.o \
components/res_station.o \
components/CDB.o \
components/ROB.o \
components/BTB.o \
components/back_writer.o

default: $(OBJS)
	$(CC) -o main $(OBJS)
run:  		
	./main 4 4 16 4 test_files/ins2.dat
clean: 
	find . -type f -name "*.o" -delete
	find . -type f -name "*.h.gch" -delete
	rm main
%.o:	%.cpp
	$(CC) $(CFLAGS) -o $*.o $*.cpp