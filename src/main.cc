#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <xmmintrin.h>
#include "ladspa_plugin.h"
#include "log.h"

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

	float y = 1e-56;
	printf ("%e\n", y);

	Plugin* p = new LadspaPlugin (plugin, 0);

	p->instantiate (44100);
	p->activate ();
	p->prepare (1024);

	int const N = p->input_buffers ();
	for (int i = 0; i < N; ++i) {
		float* b = p->input_buffer (i);
		for (int j = 0; j < 1024; ++j) {
			b[j] = 1e-25;
		}
	}
	
	p->run (1024);
	p->deactivate ();

	return 0;
}
