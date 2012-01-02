#include <vector>
#include <lilv/lilv.h>
#include "plugin.h"

class LV2Plugin : public Plugin
{
public:
	LV2Plugin (std::string const &);
	~LV2Plugin ();

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
	LilvInstance* _instance;
	LV2_Feature** _features;
	LilvPlugin const * _plugin;
	int _num_ports;
	float* _controls;
	float* _defaults;
	float** _buffers;

	std::vector<int> _audio_inputs;
	std::vector<int> _audio_outputs;
	std::vector<int> _control_inputs;
};
