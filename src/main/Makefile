default: all
.PHONY: clean

OBJS = config.o gamedata.o color.o image.o layer.o sprite.o songs.o sfx.o \
	strutil.o fix.o input.o m_logo.o m_title.o m_game.o player.o tiled.o \
	stage.o object.o explode.o powerup.o \
	bullet.o fonts.o logic.o render.o malpinx.o
HDIR = ../includes
INCLUDES = $(HDIR)/cfg.hh $(HDIR)/config.hh $(HDIR)/backend.hh $(HDIR)/defs.hh \
	    $(HDIR)/formats.hh $(HDIR)/vbase.hh $(HDIR)/abase.hh $(HDIR)/input.hh \
		$(HDIR)/layer.hh $(HDIR)/logic.hh $(HDIR)/malpinx.hh $(HDIR)/maths.hh \
		$(HDIR)/modes.hh $(HDIR)/render.hh $(HDIR)/sprite.hh $(HDIR)/color.hh \
		$(HDIR)/songs.hh $(HDIR)/image.hh $(HDIR)/gamedata.hh $(HDIR)/fonts.hh \
		$(HDIR)/sfx.hh $(HDIR)/fix.hh $(HDIR)/m_game.hh $(HDIR)/player.hh \
		$(HDIR)/explode.hh $(HDIR)/bullet.hh $(HDIR)/powerup.hh \
		$(HDIR)/object.hh $(HDIR)/strutil.hh $(HDIR)/tiled.hh $(HDIR)/stage.hh
DEPS = $(INCLUDES)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
