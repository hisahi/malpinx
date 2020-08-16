default: all
.PHONY: clean

OBJS = config.o gamedata.o color.o image.o layer.o sprite.o songs.o sfx.o \
	strutil.o fix.o input.o m_logo.o m_title.o m_game.o player.o tiled.o \
	stage.o object.o explode.o powerup.o scores.o bullet.o enemy.o \
	enemy/enemy01.o enemy/enemy02.o enemy/enemy03.o enemy/enemy04.o \
	enemy/enemy05.o enemy/enemy06.o enemy/boss1a.o \
	fonts.o script.o logic.o render.o malpinx.o
HDIR = ../includes
INCLUDES = $(HDIR)/cfg.hh $(HDIR)/config.hh $(HDIR)/backend.hh $(HDIR)/defs.hh \
	    $(HDIR)/formats.hh $(HDIR)/vbase.hh $(HDIR)/abase.hh $(HDIR)/input.hh \
		$(HDIR)/layer.hh $(HDIR)/logic.hh $(HDIR)/malpinx.hh $(HDIR)/maths.hh \
		$(HDIR)/modes.hh $(HDIR)/render.hh $(HDIR)/sprite.hh $(HDIR)/color.hh \
		$(HDIR)/songs.hh $(HDIR)/image.hh $(HDIR)/gamedata.hh $(HDIR)/fonts.hh \
		$(HDIR)/fix.hh $(HDIR)/explode.hh $(HDIR)/m_game.hh $(HDIR)/player.hh \
		$(HDIR)/fixrng.hh $(HDIR)/scores.hh \
		$(HDIR)/sfx.hh $(HDIR)/bullet.hh $(HDIR)/powerup.hh $(HDIR)/enemy.hh \
		$(HDIR)/object.hh $(HDIR)/strutil.hh $(HDIR)/tiled.hh $(HDIR)/stage.hh
DEPS = $(INCLUDES)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o enemy/*.o
