CXX=g++
LD=g++
RM=rm -f
CXXFLAGS=-g -std=c++14 -I../includes
LDFLAGS=
CXXFLAGS := $(CXXFLAGS) `sdl2-config --cflags`
LDLIBS=`sdl2-config --libs` -lSDL2_mixer
# video backend
VBACKEND=sdl2
# input backend
IBACKEND=sdl2
# audio backend
ABACKEND=sdlmixer
export

SUBDIRS=base formats main
OBJS = base/vbase.o base/ibase.o base/abase.o formats/cfg.o \
		formats/stream.o formats/txp.o formats/cfp.o formats/tip.o \
		formats/sxp.o \
		main/logic.o main/color.o main/config.o main/gamedata.o \
		main/layer.o main/sprite.o main/render.o main/m_logo.o \
		main/songs.o main/image.o main/fonts.o main/sfx.o main/input.o \
		main/m_title.o main/malpinx.o

default: all

.PHONY: all clean malpinx_clean $(SUBDIRS)
all: $(SUBDIRS) malpinx
clean: $(SUBDIRS)
	$(RM) malpinx

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

malpinx: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
