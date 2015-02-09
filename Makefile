BINARY 			:= tetris
SRCS			:= $(wildcard src/*.cpp)
OBJS			:= $(SRCS:.cpp=.o)

DEBUG			:= -g

SDL_INCLUDE		:= `sdl2-config --cflags` -IirrKlang-1.4.0/include
SDL_LIB			:= `sdl2-config --libs` -lSDL2_ttf -lSDL2_image ./irrKlang-1.4.0/lib/libIrrKlang.so

CPPFLAGS		+= $(SDL_INCLUDE) 
CXXFLAGS		+= $(DEBUG) -Wall -std=c++0x
LDFLAGS			+= $(SDL_LIB)

.PHONY: all clean

all: $(BINARY)

$(BINARY): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BINARY) $(LDFLAGS)

.depend: $(SRCS)
	@- $(RM) .depend
	@- $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $^ | sed -r 's|^([^ ])|src/\1|' > .depend;

-include .depend

clean:
	@- $(RM) $(BINARY)
	@- $(RM) $(OBJS)
	@- $(RM) .depend
