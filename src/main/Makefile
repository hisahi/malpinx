.PHONY: clean

OBJS = ../base/vbase.o ../base/abase.o \
		logic.o colmath.o render.o m_logo.o malpinx.o
DEPS = 

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

malpinx: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)
clean:
	$(RM) *.o malpinx

default: malpinx
