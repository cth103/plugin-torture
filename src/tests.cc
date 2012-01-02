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
ImpulseAndWait::run (Plugin* p, int N)
{
	int const ins = p->audio_inputs ();

	for (int i = 0; i < ins; ++i) {
		buffer_impulse (p->input_buffer (i), N);
	}

	p->run (N);
	check_for_output_denormals (p, N);

	for (int i = 0; i < 256; ++i) {
		for (int j = 0; j < ins; ++j) {
			buffer_silent (p->input_buffer (j), N);
		}

		p->run (N);
		check_for_output_denormals (p, N);
	}
}
