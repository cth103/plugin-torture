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

#include "lv2_plugin.h"
#include "uri_map.h"
#include <lilv/lilv.h>
#include <boost/algorithm/string.hpp>
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

class LV2World
{
public:
	LV2World ()
	{
		world = lilv_world_new ();
		lilv_world_load_all (world);
		input_class = lilv_new_uri (world, LILV_URI_INPUT_PORT);
		output_class = lilv_new_uri (world, LILV_URI_OUTPUT_PORT);
		control_class = lilv_new_uri (world, LILV_URI_CONTROL_PORT);
		audio_class = lilv_new_uri (world, LILV_URI_AUDIO_PORT);
		srate = lilv_new_uri (world, LILV_NS_LV2 "sampleRate");
	}

	~LV2World ()
	{
		lilv_node_free (input_class);
		lilv_node_free (output_class);
		lilv_node_free (control_class);
		lilv_node_free (audio_class);
	}

	LilvWorld* world;
	LilvNode* input_class;
	LilvNode* output_class;
	LilvNode* audio_class;
	LilvNode* control_class;
	LilvNode* srate;
};



static LV2World world;

LV2Plugin::LV2Plugin (string const & filename)
	: _plugin (0)
	, _uri_map (URIMap::instance ())
{
	_features = (LV2_Feature**) calloc (4, sizeof (LV2_Feature*));
	_features[0] = _uri_map.uri_map_feature ();
	_features[1] = _uri_map.urid_map_feature ();
	_features[2] = _uri_map.urid_unmap_feature ();
	_features[3] = 0;

	LilvPlugins const * plugins = lilv_world_get_all_plugins (world.world);

	LILV_FOREACH(plugins, i, plugins) {
		const LilvPlugin* p = lilv_plugins_get(plugins, i);
		LilvNodes const * uris = lilv_plugin_get_data_uris (p);
		LILV_FOREACH (nodes, j, uris) {
			string path = lilv_uri_to_path (lilv_node_as_string (lilv_nodes_get (uris, j)));

			while (path.find ("//") != string::npos) {
				boost::replace_all (path, "//", "/");
			}

			if (path == filename) {
				_plugin = p;
			}
		}
	}

	if (_plugin == 0) {
		stringstream s;
		s << "Could not find LV2 plugin " << filename;
		throw runtime_error (s.str ());
	}

	_num_ports = lilv_plugin_get_num_ports (_plugin);

	for (int i = 0; i < _num_ports; ++i) {
		LilvPort const * p = lilv_plugin_get_port_by_index (_plugin, i);
		if (lilv_port_is_a (_plugin, p, world.control_class)) {
			if (lilv_port_is_a (_plugin, p, world.input_class)) {
				_control_inputs.push_back (i);
			}
		} else if (lilv_port_is_a (_plugin, p, world.audio_class)) {
			if (lilv_port_is_a (_plugin, p, world.input_class)) {
				_audio_inputs.push_back (i);
			} else if (lilv_port_is_a (_plugin, p, world.output_class)) {
				_audio_outputs.push_back (i);
			}
		}
	}
}

LV2Plugin::~LV2Plugin ()
{
}

void
LV2Plugin::instantiate (int Fs)
{
	Plugin::instantiate (Fs);

	_instance = lilv_plugin_instantiate (_plugin, Fs, _features);
	if (_instance == 0) {
		throw runtime_error ("Failed to instantiate LV2 plugin");
	}
}

void
LV2Plugin::activate ()
{
	lilv_instance_activate (_instance);
}

void
LV2Plugin::deactivate ()
{
	lilv_instance_deactivate (_instance);
}

void
LV2Plugin::prepare (int buffer_size)
{
	_controls = new float[_num_ports];
	_defaults = new float[_num_ports];
	_buffers = (float **) calloc (_num_ports, sizeof (float *));

	for (int i = 0; i < _num_ports; ++i) {

		LilvPort const * p = lilv_plugin_get_port_by_index (_plugin, i);

		if (lilv_port_is_a (_plugin, p, world.audio_class)) {

			_buffers[i] = (float *) calloc (buffer_size, sizeof (float));
			lilv_instance_connect_port (_instance, i, _buffers[i]);

		} else if (lilv_port_is_a (_plugin, p, world.control_class)) {

			LilvNode* def;
			lilv_port_get_range (_plugin, p, &def, NULL, NULL);
			_defaults[i] = def ? lilv_node_as_float (def) : 0.0f;
			if (lilv_port_has_property (_plugin, p, world.srate)) {
				_defaults[i] *= _sampling_rate;
			}
			lilv_node_free (def);

			_controls[i] = _defaults[i];
			lilv_instance_connect_port (_instance, i, &_controls[i]);
		}
	}
}

void
LV2Plugin::run (int N)
{
	lilv_instance_run (_instance, N);
}

int
LV2Plugin::audio_inputs () const
{
	return _audio_inputs.size ();
}

int
LV2Plugin::audio_outputs () const
{
	return _audio_outputs.size ();
}

float *
LV2Plugin::input_buffer (int n) const
{
	assert (n < audio_inputs ());
	return _buffers[_audio_inputs[n]];
}

float *
LV2Plugin::output_buffer (int n) const
{
	assert (n < audio_outputs ());
	return _buffers[_audio_outputs[n]];
}

int
LV2Plugin::control_inputs () const
{
	return _control_inputs.size ();
}

string
LV2Plugin::control_input_name (int n) const
{
	assert (n < control_inputs ());
	return lilv_node_as_string (lilv_port_get_name (_plugin, lilv_plugin_get_port_by_index (_plugin, _control_inputs[n])));

}

void
LV2Plugin::set_control_input (int n, float v)
{
	assert (n < control_inputs ());
	_controls[_control_inputs[n]] = v;
}

float
LV2Plugin::get_control_input (int n) const
{
	assert (n < control_inputs ());
	return _controls[_control_inputs[n]];
}

string
LV2Plugin::name () const
{
	return lilv_node_as_string (lilv_plugin_get_name (_plugin));
}
