EXEC = ./bin/cache
CC = g++
#CPPFLAGS = -g -Wall -std=c++14 
INCLUDE = -I./include
CPPFLAGS = -g -Wall -std=c++14 $(INCLUDE)
LIBS = -lpthread
OBJS = obj/Main.o obj/HandleThreads.o obj/Utilities.o obj/Readers.o obj/Writers.o

SRC_DIR := ./src
OBJ_DIR := ./obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

###################################
#default all : exec
#exec: $(EXEC)
###################################

$(EXEC): $(OBJ_FILES)
	$(CC) $(CPPFLAGS) -o $@ $^ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

#$(EXEC#): $(OBJS)
#$(CC) $(CPPFLAGS) -o $@ $(OBJS) $(LIBS)
#Main.o : Main.h 
#	$(CC) $(CPPFLAGS) -c src/Main.cpp $(LIBS)
#HandleThreads.o : HandleThreads.h 
#
#	$(CC) -c src/HandleThreads.cpp 
#Utilities.o : Utilities.h
#	$(CC) -c src/Utilities.cpp 
clean:
	rm -f $(EXEC) $(OBJS)

