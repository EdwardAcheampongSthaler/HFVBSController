#include "stdafx.h"
#include "utilities.h"
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>

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
		for (int i = 0; i < byteCount; i++) {}
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
	static char b64table[64] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'};
	/* Accepts a binary buffer with an associated size.
	 * Returns a base64 encoded, NULL-terminated string.
	 */
	unsigned char* utilities::spc_base64_encode(unsigned char *input, size_t len, int wrap) {
		unsigned char *output, *p;
		size_t        i = 0, mod = len % 3, toalloc;

		toalloc = (len / 3) * 4 + (3 - mod) % 3 + 1;
		if (wrap) {
			toalloc += len / 57;
			if (len % 57) toalloc++;
		}

		p = output = (unsigned char *)malloc(((len / 3) + (mod ? 1 : 0)) * 4 + 1);
		if (!p) return 0;

		while (i < len - mod) {
			*p++ = b64table[input[i++] >> 2];
			*p++ = b64table[((input[i - 1] << 4) | (input[i] >> 4)) & 0x3f];
			*p++ = b64table[((input[i] << 2) | (input[i + 1] >> 6)) & 0x3f];
			*p++ = b64table[input[i + 1] & 0x3f];
			i += 2;
			if (wrap && !(i % 57)) *p++ = '\n';
		}
		if (!mod) {
			if (wrap && i % 57) *p++ = '\n';
			*p = 0;
			return output;
		}
		else {
			*p++ = b64table[input[i++] >> 2];
			*p++ = b64table[((input[i - 1] << 4) | (input[i] >> 4)) & 0x3f];
			if (mod == 1) {
				*p++ = '=';
				*p++ = '=';
				if (wrap) *p++ = '\n';
				*p = 0;
				return output;
			}
			else {
				*p++ = b64table[(input[i] << 2) & 0x3f];
				*p++ = '=';
				if (wrap) *p++ = '\n';
				*p = 0;
				return output;
			}
		}
	}
}