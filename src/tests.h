#include <string>

class Plugin;

class Test
{
public:
	virtual ~Test () {}

	virtual std::string name () const = 0;
	virtual void run (Plugin *, int) = 0;

protected:
	void check_for_output_denormals (Plugin *, int);
};

class ImpulseAndWait : public Test
{
public:
	std::string name () const {
		return "impulse-and-wait";
	}
	
	void run (Plugin *, int);
};
