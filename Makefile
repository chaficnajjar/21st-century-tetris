SHELL 		:= /bin/bash	# Use bash instead of sh

BINARY 		:= tetris
SRCS		:= $(wildcard src/*.cpp)
OBJS		:= ${SRCS:.cpp=.o}

DEBUG		:= -g

CPPFLAGS	:= `sdl2-config --cflags` -IirrKlang-1.4.0/include
CXXFLAGS	:= -Wall -std=c++0x
LDFLAGS		:= `sdl2-config --libs` -lSDL2 -lSDL2_ttf -lSDL2_image ./irrKlang-1.4.0/lib/libIrrKlang.so

all: $(BINARY)

$(BINARY): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BINARY) $(LDFLAGS)

clean:
	$(RM) src/*.o $(BINARY)
