INC_DIR :=include/
LIB_DIR := lib/
SRC_DIR := src/
CC :=g++
CPPFLAGS :=-I$(INC_DIR) -L$(LIB_DIR)
CXXFLAGS :=-Wall -std=c++17
LDLIBS :=-lraylib -lopengl32 -lgdi32 -lwinmm

.PHONY: all clean

all:
	$(CC) $(SRC_DIR) -o chippy $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS)

clean:
	del *.o *.exe /s
	@echo Cleaning done