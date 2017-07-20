
CC          = gcc
CLINKER     = gcc
CCC         = g++
CCLINKER    = $(CCC)
INCLUDE_DIR = 
CFLAGS  = $(INCLUDE_DIR)
CCFLAGS = $(CFLAGS)
OPTFLAGS    = -g
LIB_PATH    =
LIB_LIST    = -lm
LIBS = $(LIB_PATH) $(LIB_LIST)

objects = Interface.o Sort.o Utilities.o main.o

default: sorttest

sorttest: $(objects)
	g++ -g -o sorttest -pthread $(objects)

Interface.o : Interface.cpp Interface.h Sort.h Utilities.h
	g++ -c Interface.cpp -o Interface.o

Sort.o : Sort.cpp Sort.h Utilities.h
	g++ -c Sort.cpp -o Sort.o

Utilities.o : Utilities.cpp Utilities.h
	g++ -c Utilities.cpp -o Utilities.o
    
main.o : main.cpp Sort.h Interface.h
	g++ -c main.cpp -o main.o


clean :
	rm *.o
	rm sorttest
