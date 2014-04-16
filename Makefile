# Use bash not sh
SHELL := /bin/bash

GCC = g++
SOURCE = src/main.cpp src/board.cpp src/tetromino.cpp src/introstate.cpp src/menustate.cpp src/playstate.cpp src/game_engine.cpp src/utilities.cpp
FLAGS = -Wall -g -std=c++0x -lSDL2 -lSDL2_ttf -lSDL2_image `sdl2-config --cflags --libs` -I"irrKlang-1.4.0/include" ./irrKlang-1.4.0/lib/libIrrKlang.so -pthread
BINARY = tetris

all: 
	@if [ "$(shell uname -s)" = "Darwin" ]; then \
		brew install sdl2; \
		brew install sdl2_ttf; \
		brew install sdl2_mixer; \
	fi
	$(GCC) $(SOURCE) $(FLAGS) -o $(BINARY)

clean:
	@rm $(BINARY)
