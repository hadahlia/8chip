INC_DIR :=include
LIB_DIR := lib
SRC_DIR := src
SRC := $(wildcard $(SRC_DIR)/*.cpp) 
CC :=g++
CPPFLAGS :=-I$(INC_DIR) -L$(LIB_DIR)
CXXFLAGS :=-Wall -std=c++17 
#-pedantic-errors
LDLIBS :=-lraylib -lopengl32 -lgdi32 -lwinmm

.PHONY: all clean

all:
	$(CC) $(SRC) -o chippy $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)

debug:
	$(CC) -g $(SRC) -o chippy $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	del *.o *.exe /s
	@echo Cleaning done