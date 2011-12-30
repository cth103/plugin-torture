OBJS :=	src/ladspa_plugin.o src/main.o src/plugin.o src/log.o

CXXFLAGS = -g -msse -mfpmath=sse

plugin-torture:	$(OBJS)
		g++ $(CXXFLAGS) -o $@ $(OBJS) -ldl

%.o:	%.cc
	gcc $(CXXFLAGS) -g -Wall -c -o $@ $<

ladspa_plugin.o:	ladspa_plugin.h plugin.h
main.o:			ladspa_plugin.h plugin.h
log.o:			log.h

clean:;	rm -f src/*.o plugin-torture
