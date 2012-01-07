/*
    Copyright (C) 2012 Carl Hetherington <cth@carlh.net>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

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
	std::string name () const;
	
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
