#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <xmmintrin.h>
#include <getopt.h>
#include <sstream>
#include <list>
#include "ladspa_plugin.h"
#include "lv2_plugin.h"
#include "log.h"
#include "input_buffers.h"
#include "tests.h"

using namespace std;

int
main (int argc, char* argv[])
{
	list<Test*> tests;
	tests.push_back (new ImpulseAndWait);

	string plugin;
	bool detect_denormals = false;

	enum Type {
		LADSPA,
		LV2
	};

	Type type = LADSPA;
	
	if (argc == 1) {
		cerr << argv[0] << ": usage: " << argv[0] << " [-d] -p <plugin.so>\n";
		exit (EXIT_FAILURE);
	}

	while (1) {
		
		static struct option long_options[] = {
			{ "denormals", no_argument, 0, 'd' },
			{ "ladspa", no_argument, 0, 's' },
			{ "lv2", no_argument, 0, 'l' },
			{ "plugin", required_argument, 0, 'p'},
			{ 0, 0, 0, 0 }
		};

		int i;
		int c = getopt_long (argc, argv, "dslp:", long_options, &i);
		if (c == -1) {
			break;
		}

		switch (c) {
		case 'd':
			detect_denormals = true;
			break;
		case 's':
			type = LADSPA;
			break;
		case 'l':
			type = LV2;
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


	Plugin* p = 0;
	
	switch (type) {
	case LADSPA:
		p = new LadspaPlugin (plugin, 0);
		break;
	case LV2:
		p = new LV2Plugin (plugin);
		break;
	}

	int N = 1024;

	p->instantiate (44100);
	p->activate ();
	p->prepare (N);

	int const control_inputs = p->control_inputs ();
	log ("Inputs:");
	for (int i = 0; i < control_inputs; ++i) {
		stringstream s;
		s << "\t" << i << " " << p->control_input_name (i) << " [default " << p->get_control_input (i) << "]";
		log (s.str ());
	}

	for (list<Test*>::iterator i = tests.begin(); i != tests.end(); ++i) {
		log ((*i)->name ());
		(*i)->run (p, N);
	}
	
	p->deactivate ();
	delete p;

	return 0;
}
