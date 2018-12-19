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
		unsigned char* spc_base64_encode(unsigned char *input, size_t len, int wrap);	

		// Plugin commands
	};

}