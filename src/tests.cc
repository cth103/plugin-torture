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
