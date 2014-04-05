
GCC = g++
SOURCE = src/main.cpp src/tetris.cpp src/tetromino.cpp src/board.cpp
FLAGS = -Wall -g -std=c++0x -lSDL2 -lSDL2_ttf `sdl2-config --cflags --libs`
BINARY = tetris

all: 
	$(GCC) $(SOURCE) $(FLAGS) -o $(BINARY)

clean:
	@rm $(BINARY)
