default: all
.PHONY: all clean

OBJS = cfg.o stream.o txp.o cfp.o tip.o sxp.o
DEPS = ../includes/formats.hh

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
