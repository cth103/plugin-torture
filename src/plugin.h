
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

	virtual int input_buffers () const = 0;
	virtual float * input_buffer (int) const = 0;

protected:
	int _sampling_rate;
};

