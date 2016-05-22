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

#include <float.h>
#include "input_buffers.h"
#include "tests.h"
#include "log.h"
#include "plugin.h"

void
Test::check_for_output_denormals (Plugin* p, int buffer_size)
{
	if (p->output_has_denormals (buffer_size)) {
		warning ("Output buffer contains denormals");
	}
}

void
Test::wait (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < 256; ++i) {
		for (int j = 0; j < ins; ++j) {
			buffer_constant (p->input_buffer (j), N, 0);
		}

		p->run (N);
		check_for_output_denormals (p, N);
	}
}


void
Silence::run (Plugin* p, int N)
{
	wait (p, N);
}

void
Impulse::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_impulse (p->input_buffer (i), N);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}

void
Pulse::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_step_down (p->input_buffer (i), N);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}

void
ArdourDCBias::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_constant (p->input_buffer (i), N, 1e-27);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}

void
FltMin::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_constant (p->input_buffer (i), N, FLT_MIN);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}

void
Denormals::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_constant (p->input_buffer (i), N, 1e-38);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}

void
Sine::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_sine (p->input_buffer (i), N, 440);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	wait (p, N);
}
