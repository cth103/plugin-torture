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

#include <math.h>
#include "plugin.h"

Plugin::Plugin ()
	: _sampling_rate (0)
{

}

void
Plugin::instantiate (int Fs)
{
	_sampling_rate = Fs;
}

bool
Plugin::output_has_denormals (int buffer_size) const
{
	int const N = audio_outputs ();
	for (int i = 0; i < N; ++i) {
		float* b = output_buffer (i);
		for (int j = 0; j < buffer_size; ++j) {
			if (fpclassify (b[j]) == FP_SUBNORMAL) {
				return true;
			}
		}
	}

	return false;
}

