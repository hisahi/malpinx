CXX=g++
LD=g++
RM=rm -f
CXXFLAGS=-std=c++17 -I../includes -g3 -O0
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
OBJS = base/vbase.o base/ibase.o base/abase.o formats/cfg.o formats/stream.o \
		formats/txp.o formats/cfp.o formats/tip.o formats/tlp.o formats/slp.o \
		formats/sxp.o formats/hsc.o main/color.o \
		main/gamedata.o main/layer.o main/logic.o \
		main/fix.o main/image.o main/config.o main/strutil.o main/render.o \
		main/m_logo.o main/songs.o main/explode.o main/sprite.o main/fonts.o \
		main/powerup.o main/input.o main/enemy.o main/script.o main/scores.o \
		main/tiled.o main/stage.o main/object.o main/bullet.o main/sfx.o \
		main/enemy/enemy01.o main/enemy/enemy02.o main/enemy/enemy03.o \
		main/enemy/enemy04.o main/enemy/enemy05.o main/enemy/enemy06.o \
		main/enemy/boss1a.o \
		main/m_title.o main/m_game.o main/player.o main/malpinx.o

default: all

.PHONY: all clean malpinx_clean $(SUBDIRS)
all: $(SUBDIRS) malpinx
clean: $(SUBDIRS)
	$(RM) malpinx

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

malpinx: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)
