#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <xmmintrin.h>
#include "ladspa_plugin.h"
#include "log.h"
#include "input_buffers.h"

using namespace std;

int
main (int argc, char* argv[])
{
	int c;

	string plugin;
	bool detect_denormals = false;

	if (argc == 1) {
		cerr << argv[0] << ": usage: " << argv[0] << " [-d] -p <plugin.so>\n";
		exit (EXIT_FAILURE);
	}
	
	while ((c = getopt (argc, argv, "dp:")) != -1) {
		switch (c) {
		case 'd':
			detect_denormals = true;
			break;
		case 'p':
			plugin = optarg;
			break;
		}
	}

	if (detect_denormals) {
		log ("Turning on denormal detection.");
		int mxcsr = _mm_getcsr ();
		mxcsr &= ~(_MM_FLUSH_ZERO_ON | 0x8000);
		mxcsr &= ~_MM_MASK_DENORM;
		_mm_setcsr (mxcsr);
	}

	Plugin* p = new LadspaPlugin (plugin, 0);

	int N = 1024;

	p->instantiate (44100);
	p->activate ();
	p->prepare (N);

	int const bufs = p->input_buffers ();
	
	for (int i = 0; i < bufs; ++i) {
		buffer_silent (p->input_buffer(i), N);
	}
	p->run (N);

	for (int i = 0; i < bufs; ++i) {
		buffer_step_up (p->input_buffer(i), N);
	}
	p->run (N);

	for (int i = 0; i < bufs; ++i) {
		buffer_step_down (p->input_buffer(i), N);
	}
	p->run (N);
	
	for (int i = 0; i < bufs; ++i) {
		buffer_close_to_denormal (p->input_buffer(i), N);
	}
	p->run (N);
	
	p->deactivate ();

	return 0;
}
