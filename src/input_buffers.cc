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

void
buffer_silent (float* b, int N)
{
	for (int i = 0; i < N; ++i) {
		b[i] = 0;
	}
}

void
buffer_impulse (float* b, int N)
{
	b[0] = 1;
	
	for (int i = 1; i < N; ++i) {
		b[i] = 0;
	}
}

void
buffer_flt_min (float* b, int N)
{
	for (int i = 0; i < N; ++i) {
		b[i] = FLT_MIN;
	}
}

void
buffer_denormal (float* b, int N)
{
	for (int i = 0; i < N; ++i) {
		b[i] = 1e-38;
	}
}

void
buffer_step_up (float* b, int N)
{
	int const M = N / 2;

	for (int i = 0; i < M; ++i) {
		b[i] = 0;
	}

	for (int i = M; i < N; ++i) {
		b[i] = 1;
	}
}

void
buffer_step_down (float* b, int N)
{
	int const M = N / 2;

	for (int i = 0; i < M; ++i) {
		b[i] = 1;
	}

	for (int i = M; i < N; ++i) {
		b[i] = 0;
	}
}
