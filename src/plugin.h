#ifndef PLUGIN_TORTURE_PLUGIN_H
#define PLUGIN_TORTURE_PLUGIN_H

#include <string>

class Plugin
{
public:
	Plugin ();
	virtual ~Plugin () {}

	virtual void instantiate (int);
	virtual void activate () = 0;
	virtual void prepare (int) = 0;
	virtual void run (int) = 0;
	virtual void deactivate () = 0;

	virtual int audio_inputs () const = 0;
	virtual int audio_outputs () const = 0;
	virtual float* input_buffer (int) const = 0;
	virtual float* output_buffer (int) const = 0;
	virtual int control_inputs () const = 0;
	virtual std::string control_input_name (int) const = 0;
	virtual void set_control_input (int, float) = 0;
	virtual float get_control_input (int) const = 0;

	bool output_has_denormals (int) const;

protected:
	int _sampling_rate;
};

#endif
