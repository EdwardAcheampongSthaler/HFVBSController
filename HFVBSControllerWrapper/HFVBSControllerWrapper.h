#pragma once
#include "../HFVBSController/fingopayFVBS.h"

#include "ControllerResult.h"
using namespace sthaler;
using namespace System;

namespace HFVBSControllerWrapper {
    public ref class HFVBSControlerWrapperClass
	{
		// TODO: Add your methods for this class here.
	public:
		HFVBSControlerWrapperClass();
		~HFVBSControlerWrapperClass();
		void SayThis(System::String^ phrase);
		void Startup();
		void Test();

		// Standard Functions v3
		int Initialise();
		int Enroll();
		int Verify();
		int ControlBiometricDevice();
		int Identify();
		int CancelOperations();
		int Terminate();
		//const ControllerResult Initialise();
		//const ControllerResult Enroll();
		//const ControllerResult Verify();
		//const ControllerResult ControlBiometricDevice();
		//const ControllerResult Identify();
		//const ControllerResult CancelOperations();
		//const ControllerResult Terminate();
		//const ControllerResult ResetBiometricDevice();
	private:
		fingopayFVBSApp* fingopayapp;
	};

}

