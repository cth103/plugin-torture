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

