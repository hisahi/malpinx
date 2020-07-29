.PHONY: clean

default: malpinx

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) *.o
