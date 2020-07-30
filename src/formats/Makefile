default: all
.PHONY: all clean

OBJS = stream.o txp.o cfp.o
DEPS = ../includes/formats.hh

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o
