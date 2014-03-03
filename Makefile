GCC = g++
SOURCE = src/tetris.cpp
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_ttf -Wl,-rpath=/usr/local/lib `sdl2-config --cflags --libs`
SDL_INCLUDE = -I/usr/local/include
GCCFLAGS = -Wall -ansi -std=c++0x $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
BINARY = tetris

all: 
	$(GCC) $(SOURCE) $(GCCFLAGS) $(LDFLAGS) -o $(BINARY)

clean:
	rm *.o && rm $(BINARY)
