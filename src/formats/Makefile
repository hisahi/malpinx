.PHONY: all clean

default: all

%.o: $(VBACKEND)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: txp.o cfp.o dfp.o
clean:
	$(RM) *.o
