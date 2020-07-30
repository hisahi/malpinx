default: all
.PHONY: clean

OBJS = colmath.o config.o gamedata.o layer.o \
	logic.o m_logo.o m_title.o malpinx.o render.o sprite.o

DEPS = ../includes/abase.hh ../includes/cfg.hh ../includes/config.hh \
 	   ../includes/defs.hh ../includes/formats.hh ../includes/gamedata.hh \
 	   ../includes/backend.hh ../includes/layer.hh ../includes/logic.hh \
 	   ../includes/malpinx.hh ../includes/maths.hh ../includes/modes.hh \
 	   ../includes/render.hh ../includes/sprite.hh ../includes/vbase.hh

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
