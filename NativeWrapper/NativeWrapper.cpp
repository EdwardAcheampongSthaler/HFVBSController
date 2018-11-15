#include "stdafx.h"

#include "NativeWrapper.h"
using namespace NativeWrapper;
DeviceControllerWrapperClass::DeviceControllerWrapperClass() :
	fingopayapp(new sthaler::fingopayFVBSApp)
{

}

void DeviceControllerWrapperClass::SayThis(System::String^ phrase)
{
	// do something
	std::printf("Saythis");
	//hw->SayThis(marshal::to<wchar_t*>(phrase))
}

void DeviceControllerWrapperClass::Startup()
{
	// do something
	std::printf("Startup");
	fingopayapp->Startup();
}

DeviceControllerWrapperClass::~DeviceControllerWrapperClass()
{
	// do something
	delete fingopayapp;
}