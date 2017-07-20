CC          = gcc
CLINKER     = gcc
CCC         = g++
CCLINKER    = $(CCC)
INCLUDE_DIR = 
LIB_LIST    = -lm
CFLAGS  = $(INCLUDE_DIR)
CCFLAGS = $(CFLAGS)
OPTFLAGS    = -g
LIB_PATH    =
LIBS = $(LIB_PATH) $(LIB_LIST)

objects = graph.o Utilities.o main.o

default: graph

graph: $(objects) 
	$(CCLINKER) $(OPTFLAGS) -o graph $(objects) $(LIBS)

graph.o : graph.cc graph.h
	$(CCC) $(CCFLAGS) $(INCLUDE_DIRS) -c graph.cc

Utilities.o : Utilities.cpp Utilities.h
	$(CCC) $(CCFLAGS) $(INCLUDE_DIRS) -c Utilities.cpp
    
wstring.o : wstring.cc
	$(CCC) $(CCFLAGS) $(INCLUDE_DIRS) -c wstring.cc

main.o : main.cc graph.h
	$(CCC) $(CCFLAGS) $(INCLUDE_DIRS) -c main.cc

clean :
	rm *.o
	rm graph



