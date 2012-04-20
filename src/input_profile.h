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

