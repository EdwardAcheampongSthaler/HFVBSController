#include "stdafx.h"
#include "utilities.h"
#include <string>
#include <iostream>

using namespace std;

namespace core {
	utilities::utilities()
	{
	}


	utilities::~utilities()
	{
	}

	void utilities::printfmsg(string msg)
	{
		cout << msg << endl;
	}
}