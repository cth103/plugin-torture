#include <sstream>
#include <fstream>
#include <cmath>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "log.h"
#include "plugin.h"
#include "input_profile.h"

using namespace std;

InputSpecification::InputSpecification (int n)
	: _input (n)
{

}

DiscreteInputSpecification::DiscreteInputSpecification (int n, vector<float> const & v)
	: InputSpecification (n)
{
	_values = v;
}

int
DiscreteInputSpecification::values () const
{
	return _values.size ();
}

float
DiscreteInputSpecification::value (int i) const
{
	return _values[i];
};

RangeInputSpecification::RangeInputSpecification (int n, vector<float> const & v)
	: InputSpecification (n)
{
	if (v.size() != 3) {
		throw runtime_error ("Badly formed range input specification");
	}

	_from = v[0];
	_step = v[1];
	_to = v[2];
}

int
RangeInputSpecification::values () const
{
	return ceil ((_to - _from) / _step);
}

float
RangeInputSpecification::value (int i) const
{
	return _from + i * _step;
}

InputProfile::InputProfile (string const & file)
{
	ifstream f (file.c_str ());
	
	while (f.good ()) {
		string line;
		getline (f, line);

		if (!line.empty() && line[0] == '#') {
			continue;
		}

		vector<string> bits;
		boost::split (bits, line, boost::is_any_of (" "));

		if (bits.size() < 2) {
			continue;
		}

		int const n = atoi (bits[0].c_str ());
		string const t = bits[1];

		vector<float> float_bits;
		for (int i = 2; i < int (bits.size()); ++i) {
			float_bits.push_back (atof (bits[i].c_str ()));
		}

		if (t == "range") {
			_specs.push_back (new RangeInputSpecification (n, float_bits));
		} else if (t == "values") {
			_specs.push_back (new DiscreteInputSpecification (n, float_bits));
		} else {
			stringstream s;
			s << "Unknown specification type " << t << " in input profile.";
			throw runtime_error (s.str ());
		}
	}

	if (_specs.empty ()) {
		stringstream s;
		s << "Could not read specs from profile " << file;
		throw runtime_error (s.str ());
	}
}

void
InputProfile::begin_iteration ()
{
	int N = _specs.size ();
	_iteration.resize (N);

	for (int i = 0; i < N; ++i) {
		_iteration[i] = 0;
	}
}

void
InputProfile::setup (Plugin* p) const
{
	int N = _specs.size ();

	for (int i = 0; i < N; ++i) {
		int const input = _specs[i]->input ();
		float const value = _specs[i]->value (_iteration[i]);
		p->set_control_input (input, value);
		stringstream s;
		s << "\t " << input << " " << p->control_input_name (input) << " => " << value;
		log (s.str ());
	}
}

bool
InputProfile::step ()
{
	int i = 0;
	while (1) {
		_iteration[i]++;
		if (_iteration[i] != _specs[i]->values()) {
			break;
		}

		_iteration[i] = 0;
		++i;

		if (i == int (_specs.size())) {
			return false;
		}
	}

	return true;
}
