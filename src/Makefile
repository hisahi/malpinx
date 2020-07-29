CXX=g++
LD=g++
RM=rm -f
CXXFLAGS=-std=c++11 -I../includes
LDFLAGS=
CXXFLAGS := $(CXXFLAGS) `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs` -lSDL2_mixer
VBACKEND=sdl2
ABACKEND=sdlmixer
export

SUBDIRS=base cfg formats main
OBJS = base/vbase.o base/abase.o \
		formats/txp.o formats/cfp.o formats/dfp.o \
		main/logic.o main/colmath.o main/config.o main/gamedata.o \
		main/layer.o main/sprite.o main/render.o \
		main/m_logo.o main/malpinx.o

default: all

.PHONY: all clean malpinx_clean $(SUBDIRS)
all: $(SUBDIRS) malpinx
clean: $(SUBDIRS)
	$(RM) malpinx

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

malpinx: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
