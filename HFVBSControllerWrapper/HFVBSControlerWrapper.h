#pragma once

#include "../HFVBSController/fingopayFVBS.h"
using namespace sthaler;

namespace CPPCLIWrapper {
	ref class HFVBSControlerWrapper
	{
	public:
		HFVBSControlerWrapper();
		//~HFVBSControlerWrapper();
		void SayThis(System::String^ phrase);
		void Startup();
	private:
		fingopayFVBSApp *fingopayapp;
	};

}