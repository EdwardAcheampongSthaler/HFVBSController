#pragma once

#include "../HFVBSController/fingopayFVBS.h"

using namespace System;
using namespace sthaler;

namespace NativeWrapper {
	public ref class DeviceControllerWrapperClass
	{
	public:
		DeviceControllerWrapperClass();
		~DeviceControllerWrapperClass();
		void SayThis(System::String^ phrase);
		void Startup();
	private:
		fingopayFVBSApp *fingopayapp;
	};
}
