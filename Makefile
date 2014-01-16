CC = gcc -Wall -ansi
SOURCE = Tetris.cpp FreeType.cpp
FREETYPE_INCLUDE = /usr/include/freetype2
FLAGS = -lGL -lGLU -lSDL_mixer -lfreetype `sdl-config --cflags --libs`
BINARY = tetris

all:
	$(CC) $(SOURCE) -o $(BINARY) $(FLAGS) -I$(FREETYPE_INCLUDE)

clean:
	@echo Cleaning up...
	@rm $(BINARY)
	@echo Done.
