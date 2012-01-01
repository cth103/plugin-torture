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
