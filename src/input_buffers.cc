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

#include <cmath>
#include <iostream>
#include "input_buffers.h"
#include "main.h"

void
buffer_impulse (float* b, int N)
{
	b[0] = 1;
	
	for (int i = 1; i < N; ++i) {
		b[i] = 0;
	}
}

void
buffer_constant (float* b, int N, float v)
{
	for (int i = 0; i < N; ++i) {
		b[i] = v;
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

void
buffer_sine (float* b, int N, int f)
{
	for (int i = 0; i < N; ++i) {
		b[i] = sin (2 * M_PI * f * i / sampling_rate);
		std::cout << b[i] << "\n";
	}
}
