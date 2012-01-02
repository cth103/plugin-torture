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
