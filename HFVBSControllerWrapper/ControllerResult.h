#pragma once

#include <string>
using namespace std;
namespace HFVBSControllerWrapper {


	public ref class ControllerResult
	{
		// TODO: Add your methods for this class here.
	public:

		ControllerResult(bool success, const string& message,  int statusCode)
			:m_success(success), m_message(message), m_statusCode(statusCode)
		{}
		~ControllerResult();

		int m_statusCode;
		const string& m_message;
		bool m_success;
	};
}