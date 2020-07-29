.PHONY: all clean

default: all

vbase.o: $(VBACKEND)/vbase.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<
abase.o: $(ABACKEND)/abase.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: vbase.o abase.o
clean:
	$(RM) *.o
