#include "stdafx.h"
#include "HFVBSControlerWrapper.h"


using namespace CPPCLIWrapper;

HFVBSControlerWrapper::HFVBSControlerWrapper():
	fingopayapp(new sthaler::fingopayFVBSApp)
{

}

void HFVBSControlerWrapper::SayThis(System::String^ phrase)
{
	// do something
	std::printf("Saythis");
	//hw->SayThis(marshal::to<wchar_t*>(phrase))
}

void HFVBSControlerWrapper::Startup()
{
	// do something
	std::printf("Startup");
	fingopayapp->Startup();
}

//HFVBSControlerWrapper::~HFVBSControlerWrapper()
//{
//	delete fingopayapp;
//}
