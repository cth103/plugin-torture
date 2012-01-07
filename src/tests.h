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

class Plugin;

class Test
{
public:
	virtual ~Test () {}

	virtual std::string name () const = 0;
	virtual void run (Plugin *, int) = 0;
	virtual bool evil () const {
		return false;
	}

protected:
	void check_for_output_denormals (Plugin *, int);
	void wait (Plugin *, int);
};

class Silence : public Test
{
public:
	std::string name () const {
		return "silence";
	}
	
	void run (Plugin *, int);
};

class Impulse : public Test
{
public:
	std::string name () const {
		return "impulse";
	}
	
	void run (Plugin *, int);
};

class Pulse : public Test
{
public:
	std::string name () const {
		return "pulse";
	}
	
	void run (Plugin *, int);
};

class FltMin : public Test
{
public:
	std::string name () const {
		return "FLT_MIN";
	}

	void run (Plugin *, int);

	bool evil () const {
		return true;
	}
};

class ArdourDCBias : public Test
{
public:
	std::string name () const {
		return "ardour-dc-bias";
	}

	void run (Plugin *, int);
};

class Denormals : public Test
{
public:
	std::string name () const {
		return "denormals";
	}

	void run (Plugin *, int);

	bool evil () const {
		return true;
	}
};
	
class Sine : public Test
{
public:
	std::string name () const {
		return "sine";
	}

	void run (Plugin *, int);
};
