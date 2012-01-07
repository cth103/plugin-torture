/*
    Copyright (C) 2012 Carl Hetherington <cth@carlh.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>
#include <xmmintrin.h>
#include <getopt.h>
#include <sstream>
#include <list>
#include <signal.h>
#include "ladspa_plugin.h"
#include "lv2_plugin.h"
#include "log.h"
#include "input_buffers.h"
#include "tests.h"

using namespace std;

static bool abort_on_sigfpe = false;

void
fp_exception_handler (int)
{
	warning ("FP exception");
	if (abort_on_sigfpe) {
		abort ();
	}

	/* return code 2: SIGFPE was raised (probably due to a denormal) */
	exit (2);
}

int
main (int argc, char* argv[])
{
	signal (SIGFPE, fp_exception_handler);

	/* Make a list of tests */
	list<Test*> tests;
	tests.push_back (new Silence);
	tests.push_back (new Impulse);
	tests.push_back (new Pulse);
	tests.push_back (new ArdourDCBias);
	tests.push_back (new FltMin);
	tests.push_back (new Denormals);

	/* Parse the command line */
	
	string plugin;
	bool evil = false;
	bool detect_denormals = false;
	int ladspa_index = 0;

	enum Type {
		LADSPA,
		LV2
	};

	Type type = LADSPA;
	
	if (argc == 1) {
		cerr << argv[0] << ": usage: " << argv[0] << " [-e] [-d] [-a] [-s|--ladspa] [-i,--index <n>] [-l,--lv2] -p <plugin.{so,ttl}>\n"
		     << "\t-e run particularly evil tests\n"
		     << "\t-d set CPU to raise SIGFPE on encountering a denormal, and catch it\n"
		     << "\t-a abort on SIGFPE; otherwise return with exit code 2\n"
		     << "\t-s|--ladspa plugin is LADSPA (specify the .so)\n"
		     << "\t-i|--index index of plugin in LADSPA .so (defaults to 0)\n"
		     << "\t-l|--lv2 plugin is LV2 (specify the .ttl, must be on LV2_PATH)\n"
		     << "\t-p <plugin.{so,ttl}> plugin to torture\n";
		exit (EXIT_FAILURE);
	}

	while (1) {
		
		static struct option long_options[] = {
			{ "evil", no_argument, 0, 'e' },
			{ "denormals", no_argument, 0, 'd' },
			{ "abort", no_argument, 0, 'a' },
			{ "ladspa", no_argument, 0, 's' },
			{ "index", required_argument, 0, 'i'},
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
		case 'e':
			evil = true;
			break;
		case 'd':
			detect_denormals = true;
			break;
		case 'a':
			abort_on_sigfpe = true;
			break;
		case 's':
			type = LADSPA;
			break;
		case 'i':
			ladspa_index = atoi (optarg);
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
		p = new LadspaPlugin (plugin, ladspa_index);
		break;
	case LV2:
		p = new LV2Plugin (plugin);
		break;
	}

	{
		stringstream s;
		s << "Running `" << p->name() << "' (" << plugin << ")";
		if (type == LADSPA) {
			s << " index " << ladspa_index;
		}
		log (s.str ());
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
		if (evil || !(*i)->evil ()) {
			log ((*i)->name ());
			(*i)->run (p, N);
		}
	}
	
	p->deactivate ();
	delete p;

	return 0;
}
