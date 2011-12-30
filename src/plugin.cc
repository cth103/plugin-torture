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
