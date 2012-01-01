OBJS :=	src/ladspa_plugin.o src/main.o src/plugin.o src/log.o src/input_buffers.o

CXXFLAGS = -g -msse -mfpmath=sse

plugin-torture:	$(OBJS)
		g++ $(CXXFLAGS) -o $@ $(OBJS) -ldl

%.o:	%.cc
	gcc $(CXXFLAGS) -g -Wall -c -o $@ $<

ladspa_plugin.o:	ladspa_plugin.h plugin.h
main.o:			ladspa_plugin.h plugin.h input_buffers.h
log.o:			log.h
input_buffers.o:	input_buffers.h

clean:;	rm -f src/*.o plugin-torture
