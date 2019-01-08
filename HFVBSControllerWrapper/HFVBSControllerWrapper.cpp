#include "stdafx.h"
#include "HFVBSControllerWrapper.h"
#include "../HFVBSController/fingopayFVBS.h"
#include <msclr\marshal_cppstd.h>

using namespace HFVBSControllerWrapper;
using namespace msclr::interop;
using namespace System;

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
void HFVBSControllerWrapper::HFVBSControlerWrapperClass::Shutdown()
{
	// do something
	std::printf("Shutdown");
	//call native void method 
	fingopayapp->Terminate();
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

System::String^ HFVBSControllerWrapper::HFVBSControlerWrapperClass::Enroll()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->Enroll();
	//printf(std::string<static_cast>(result));
	//BYTE *resul = new BYTE[235];
	/*System::String^ managed = "test";
	std::string unmanaged = msclr::interop::marshal_as<std::string>(managed);
	std::printf("returned: %s", result );
	*///return ()->managed;
	auto str = gcnew String(marshal_as<String^>(result.c_str()));
	//return marshal_as<System::String>(str);
	Console::WriteLine("HERE   " + str);
	return str;
}
System::String^ HFVBSControllerWrapper::HFVBSControlerWrapperClass::EnrollAndVerify()
{
	ControllerResult cr(false, "No Result", 200);
	auto result = fingopayapp->EnrollAndVerify();
	//printf(std::string<static_cast>(result));
	//BYTE *resul = new BYTE[235];
	/*System::String^ managed = "test";
	std::string unmanaged = msclr::interop::marshal_as<std::string>(managed);
	std::printf("returned: %s", result );
	*///return ()->managed;
	auto str = gcnew String(marshal_as<String^>(result.c_str()));
	//return marshal_as<System::String>(str);
	Console::WriteLine("HERE   " + str);
	return str;
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