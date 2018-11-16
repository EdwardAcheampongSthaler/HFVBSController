#include "stdafx.h"
#include "HFVBSControllerWrapper.h"
#include "../HFVBSController/fingopayFVBS.h"

using namespace HFVBSControllerWrapper;

HFVBSControllerWrapper::HFVBSControlerWrapperClass::HFVBSControlerWrapperClass()
{
	fingopayapp = new fingopayFVBSApp();
}


//HFVBSControlerWrapperClass::HFVBSControlerWrapperClass() :
//	fingopayapp(new sthaler::fingopayFVBSApp)
//{
//
//}

HFVBSControllerWrapper::HFVBSControlerWrapperClass::~HFVBSControlerWrapperClass()
{
	delete fingopayapp;
}

void HFVBSControllerWrapper::HFVBSControlerWrapperClass::SayThis(System::String^ phrase)
{
	// do something
	std::printf("Saythis");
	//hw->SayThis(marshal::to<wchar_t*>(phrase))
}

void HFVBSControllerWrapper::HFVBSControlerWrapperClass::Startup()
{
	// do something
	std::printf("Startup");
	//call native void method 
	fingopayapp -> Startup();
}

void HFVBSControllerWrapper::HFVBSControlerWrapperClass::Test() {
	std::printf("Firing Test!");
	fingopayapp->Test();
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::Initialise()
{
	ControllerResult cr(false, "", 200);
	auto result = fingopayapp->Initialise();
	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::Enroll()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->Enroll();
	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::Verify()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->Verify();
	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::ControlBiometricDevice()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->ControlBiometricDevice();
	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::Identify()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->Identify();

	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::CancelOperations()
{
	ControllerResult cr(false, "No Result", 200);
	fingopayapp->Cancel();
	return 0;
}

int HFVBSControllerWrapper::HFVBSControlerWrapperClass::Terminate()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->Terminate();
	return 0;
}


//ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::Initialise()
//{ 
//	ControllerResult cr(false,"",200);
//	return cr;
//}
//
//ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::Enroll() 
//{
//
//}
//
//const ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::Verify()
//{
//
//}
//
//const ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::ControlBiometricDevice()
//{
//
//}
//
//const ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::Identify() 
//{
//	ControllerResult cr(false, "No Result", 200);
// return 
//}
//
//const ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::CancelOperations()
//{
//
//}
//
//const ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::Terminate()
//{
//	ControllerResult cr(false,"",200);
//	return cr;
//}
//
//ControllerResult HFVBSControllerWrapper::HFVBSControlerWrapperClass::ResetBiometricDevice() 
//{
//
//}