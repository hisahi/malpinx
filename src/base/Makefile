default: all
.PHONY: all clean

DEPS = ../includes/base/sdl2.hh

vbase.o: $(VBACKEND)/vbase.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
abase.o: $(ABACKEND)/abase.cc $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: vbase.o abase.o
clean:
	$(RM) *.o
