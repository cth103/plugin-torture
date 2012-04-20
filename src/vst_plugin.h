#include <string>
#include <ladspa.h>
#include "plugin.h"

class VSTPlugin : public Plugin
{
public:
	VSTPlugin (std::string const &, int);
	~VSTPlugin ();

	void instantiate (int);
	void activate ();
	void prepare (int);
	void run (int);
	void deactivate ();

	float* input_buffer (int) const;
	int input_buffers () const;
	
private:
	void* _dll;
	char* _name;
	char* _nameptr;
	typedef AEffect * (* main_entry_t) (audioMasterCallback);
	main_entry_t _main_entry
	int _plugincnt;
};
