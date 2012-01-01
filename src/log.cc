#include <iostream>
#include "log.h"

using namespace std;

void
log (string const & s)
{
	cout << "LOG: " << s << "\n";
}

void
warning (string const & s)
{
	cout << "WARNING: " << s << "\n";
}
