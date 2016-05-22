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

#include <vector>

class InputSpecification
{
public:
	InputSpecification (int);

	int input () const {
		return _input;
	}

	virtual int values () const = 0;
	virtual float value (int) const = 0;

private:
	int _input;
};

class DiscreteInputSpecification : public InputSpecification
{
public:
	DiscreteInputSpecification (int, std::vector<float> const &);

	int values () const;
	float value (int) const;

private:
	std::vector<float> _values;
};

class RangeInputSpecification : public InputSpecification
{
public:
	RangeInputSpecification (int, std::vector<float> const &);

	int values () const;
	float value (int) const;

private:
	float _from;
	float _step;
	float _to;
};

class InputProfile
{
public:
	InputProfile (std::string const &);

	void begin_iteration ();
	void setup (Plugin* p) const;
	bool step ();

private:
	std::vector<InputSpecification*> _specs;
	std::vector<int> _iteration;
};

