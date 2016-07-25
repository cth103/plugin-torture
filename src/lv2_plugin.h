/*
    Copyright (C) 2012-2016 Carl Hetherington <cth@carlh.net>

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

#include <vector>
#include <lilv/lilv.h>
#include "plugin.h"

class URIMap;

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
	std::string name () const;

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

	URIMap & _uri_map;
};
