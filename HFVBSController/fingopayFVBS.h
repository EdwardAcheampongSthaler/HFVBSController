
// STH.FingopayFVBS.h : main header file for the STH.FingopayFVBS application
//
#pragma once
#include <string>
#include "../Include/bioapi.h"
#include "../Include/hi_bioapi.h"
#include "define.h"
#include "utilities.h"

using namespace std;


namespace sthaler {

	class fingopayFVBSApp
	{
	public:
		// constructor & destructor
		fingopayFVBSApp();
	    ~fingopayFVBSApp();

		// v3 sthaler Operations Contract
		string Initialise(); // calls init_framework, loads BSP, Queries Units available - and Attaches a BSP to a session.
		string Terminate(); // in order detaches BSP, Unloads BSP, Terminates Framework, and removes and memory allocation to pointers/variables used
		
		string ControlBiometricDevice(); // Control commands
		string Enroll();
		string EnrollAndVerify();
		string Verify(BioAPI_BIR_HANDLE processedTemplateBIRHandle); // 1-1 Match
		string Identify();  // 1-Many Match
		string Capture(); // captures a BIR
		void Cancel();


		//// bio api special members + commands
		bool  m_bHiColorIcons;
		int m_nAppLook;

		void Startup();
		void Test();
	private:

		string CreateFvtTemplateFromBIR(BioAPI_BIR BIR, BioAPI_BIR_HEADER Header);

		BioAPI_VERSION m_BioAPIVersion;
		BioAPI_FRAMEWORK_SCHEMA m_FrameworkSchema;
		BioAPI_BSP_SCHEMA *m_BSPSchemaList;
		uint32_t m_NumBSP;
		uint32_t m_NumUnits;
		BioAPI_EventHandler m_EventHandler;
		HWND m_EventHandlerCxt;
		BioAPI_UNIT_SCHEMA *m_UnitSchemaList;
		BioAPI_HANDLE m_BSPHandle;

		BIRArray m_BIRList;
		Data_InfoArray m_DataInfoList;
		BioAPI_UUID* m_BSP_Uuid;

		string m_xcCmbBSPUuid;
		int m_xvCmbBSPUuid;

		void freeBSPSchemaList(BioAPI_BSP_SCHEMA **, uint32_t *);
		void freeUnitSchemaList(BioAPI_UNIT_SCHEMA **ppUnitSchemaList, uint32_t *pNumUnits);

		BioAPI_RETURN BioAPI initFrameWork(const char *);
		BioAPI_UNIT_ID getUnitID(void);
		BioAPI_RETURN terminateFrameWork(void);
	};

}