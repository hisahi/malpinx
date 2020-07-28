CXX=g++
LD=g++
CXXFLAGS=-std=c++11 -I../includes
LDFLAGS=-lSDL2
RM=rm -f
BACKEND=sdl2
export

SUBDIRS=base main

.PHONY: all clean $(SUBDIRS)
all clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

default: all
