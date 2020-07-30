default: all
.PHONY: all clean

OBJS = cfg.o
DEPS = ../includes/cfg.hh

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
