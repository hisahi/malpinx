.PHONY: all clean

OBJS = vbase.o abase.o

%.o: $(BACKEND)/%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(OBJS)
clean:
	$(RM) *.o

default: all
