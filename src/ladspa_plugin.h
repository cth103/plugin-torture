#include <string>
#include <vector>
#include <ladspa.h>
#include "plugin.h"

class LadspaPlugin : public Plugin
{
public:
	LadspaPlugin (std::string const &, int);
	~LadspaPlugin ();

	void instantiate (int);
	void activate ();
	void prepare (int);
	void run (int);
	void deactivate ();

	int audio_inputs () const;
	int audio_outputs () const;
	float* input_buffer (int) const;
	float* output_buffer (int) const;
	int control_inputs () const;
	std::string control_input_name (int) const;
	void set_control_input (int, float);
	float get_control_input (int) const;
	
private:
	void unprepare ();

	void* _library;
	const LADSPA_Descriptor * _descriptor;
	int _num_ports;
	LADSPA_Handle _handle;
	LADSPA_Data* _controls;
	LADSPA_Data** _buffers;

	std::vector<int> _audio_inputs;
	std::vector<int> _audio_outputs;
	std::vector<int> _control_inputs;
};
