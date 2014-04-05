# Use bash not sh
SHELL := /bin/bash

GCC = g++
SOURCE = src/main.cpp src/tetris.cpp src/tetromino.cpp src/board.cpp
FLAGS = -Wall -g -std=c++0x -lSDL2 -lSDL2_ttf `sdl2-config --cflags --libs`
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
