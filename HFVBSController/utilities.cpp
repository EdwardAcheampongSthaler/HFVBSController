#include "stdafx.h"
#include "utilities.h"
#include <string>
#include <iostream>
#include <vector>

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

	int utilities::buffToInteger(char * buffer, int byteCount)
	{
		for(int i = 0; i< byteCount ; i++) { }
		int a = static_cast<int>(static_cast<unsigned char>(buffer[0]) << 24 |
			static_cast<unsigned char>(buffer[1]) << 16 |
			static_cast<unsigned char>(buffer[2]) << 8 |
			static_cast<unsigned char>(buffer[3]));
		return a;
	}

	vector<unsigned char> utilities::intToBytes(int paramInt)
	{
		vector<unsigned char> arrayOfByte(4);
		for (int i = 0; i < 4; i++)
			arrayOfByte[3 - i] = (paramInt >> (i * 8));
		return arrayOfByte;
	}
}