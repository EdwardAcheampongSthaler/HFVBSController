// TestConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "C:\Users\Ed\source\repos\HFVBSController\HFVBSController\fingopayFVBS.h"


 sthaler::fingopayFVBSApp controller = sthaler::fingopayFVBSApp();
//
int main()
{
 
	////std::cout << "Hello World!\n"; 
	//controller.Startup();
	controller.Initialise();
	//controller.Verify(); -- fix array
	//controller.Enroll();

	controller.EnrollAndVerify();

	//controller.ControlBiometricDevice(); // Control commands
	//controller.Identify();  // 1-Many Match
	//controller.Verify(); // 1-1 Match
	//controller.Capture(); // captures a BIR
	//controller.Cancel();
	//controller.Enroll();
	controller.Terminate();
	//// Call a Initialise and Terminate Function - with BioAPI headers

	
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
