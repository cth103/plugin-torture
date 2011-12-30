#include <string>
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

	float* input_buffer (int) const;
	int input_buffers () const;
	
private:
	void unprepare ();

	void* _library;
	const LADSPA_Descriptor * _descriptor;
	int _num_ports;
	LADSPA_Handle _handle;
	LADSPA_Data* _controls;
	LADSPA_Data** _buffers;
};
