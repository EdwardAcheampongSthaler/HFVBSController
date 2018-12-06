#pragma once
#include <string>
#include <vector>

using namespace std;
namespace core {
	class utilities
	{
	public:
		utilities();
		~utilities();

		static void printfmsg(string msg);
		int buffToInteger(char * buffer, int byteCount);
		vector<unsigned char> intToBytes(int paramInt);
	};

}