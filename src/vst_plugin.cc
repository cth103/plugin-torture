#include "vst_plugin.h"

using namespace std;

VSTPlugin::VSTPlugin (string const & filename)
{
	_library = dlopen (filename.c_str(), RTLD_LOCAL | RTLD_LAZY);
	if (_library == 0) {
		stringstream s;
		s << "Could not load VST plugin " << filename;
		throw runtime_error (s.str ());
	}

	if (_main_entry = (main_entry_t) dlsym (_library, "main") == 0) {
		stringstream s;
		s << "Could not find VST entry point in " << filename;
		throw runtime_error (s.str ());
	}
}

VSTPlugin::~VSTPlugin ()
{
	dlclose (_library);
}

void
VSTPlugin::instantiate (int Fs)
{
	Plugin::instantiate (Fs);

	_plugin = _main_entry (vst_callback);
	if (_plugin == 0) {
		throw runtime_error ("Could not instantiate VST plugin");
	}

	if (_plugin->magic != kEffectMagic ){
		throw runtime_error ("VST plugin magic word not found");
	}

	_plugin->dispatcher (_plugin, effOpen, 0, 0, 0, 0);
}

void
VSTPlugin::activate ()
{
}

void
VSTPlugin::deactivate ()
{
}

void
VSTPlugin::unprepare ()
{
}

void
VSTPlugin::prepare (int buffer_size)
{
}

void
VSTPlugin::run (int N)
{
}

int
VSTPlugin::input_buffers () const
{
}


float *
VSTPlugin::input_buffer (int n) const
{
	return 0;
}
