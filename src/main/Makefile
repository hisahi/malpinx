default: all
.PHONY: clean

OBJS = config.o gamedata.o \
	color.o image.o layer.o sprite.o \
	m_logo.o m_title.o \
	logic.o render.o songs.o \
	malpinx.o

DEPS = ../includes/abase.hh ../includes/cfg.hh ../includes/config.hh \
 	   ../includes/defs.hh ../includes/formats.hh ../includes/gamedata.hh \
 	   ../includes/backend.hh ../includes/layer.hh ../includes/logic.hh \
 	   ../includes/malpinx.hh ../includes/maths.hh ../includes/modes.hh \
 	   ../includes/render.hh ../includes/sprite.hh ../includes/vbase.hh \
	   ../includes/songs.hh ../includes/image.hh ../includes/color.hh

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
