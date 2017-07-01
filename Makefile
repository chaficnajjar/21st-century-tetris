BINARY 			:= tetris
SRCS			:= $(wildcard src/*.cc)
OBJS			:= $(SRCS:.cc=.o)

DEBUG			:= -g

SDL_INCLUDE		:= `sdl2-config --cflags` -IirrKlang-64bit-1.5.0/include -I.
SDL_LIB			:= `sdl2-config --libs` -lSDL2_ttf -lSDL2_image ./irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so

CPPFLAGS		+= $(SDL_INCLUDE)
CXXFLAGS		+= $(DEBUG) -Wall -std=c++0x
LDFLAGS			+= $(SDL_LIB)

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BINARY) $(LDFLAGS)

.depend: $(SRCS)
	@- $(RM) .depend
	@- $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $^ | sed -E 's|^([^ ])|src/\1|' > .depend;

-include .depend

clean:
	@- $(RM) $(BINARY)
	@- $(RM) $(OBJS)
	@- $(RM) .depend
