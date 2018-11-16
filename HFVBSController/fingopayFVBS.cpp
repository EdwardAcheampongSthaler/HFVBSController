
// STH.FingopayFVBS.cpp : Defines the class behaviors for the application.
//
#pragma once

#include "stdafx.h"
#include "fingopayFVBS.h"
#include <iostream>




//#define BSP_DEFAULT_UUID	( "85b448bf-36a6-4843-b34b-bcdfef45d1d7" ) 
#define BSP_DEFAULT_UUID	( "e41a9357-67df-44d5-80e3-64935d21b642" )  

using namespace std;
using namespace sthaler;

fingopayFVBSApp::fingopayFVBSApp()
{
	//constructor

		// BioAPI_Init
	m_BioAPIVersion = 0x20;

	// BioAPI_EnumBSPs
	m_BSPSchemaList = NULL;
	m_NumBSP = 0;

	// BioAPI_BSPLoad
	m_EventHandler = NULL;
	m_EventHandlerCxt = NULL;

	// BioAPI_QueryUnits
	m_UnitSchemaList = NULL;
	m_NumUnits = 0;
}

void sthaler::fingopayFVBSApp::Test()
{
	cout << "This is a test app";
}
string fingopayFVBSApp::Initialise()
{
	cout << "Initialise" << std::endl;
	return "Initialise";
}
string fingopayFVBSApp::Terminate()
{
	cout << "Terminate" << std::endl;
	return "Terminate";
}
string fingopayFVBSApp::ControlBiometricDevice()
{
	 cout << "ControlBiometricDevice" << std::endl;
	 return "ControlBiometricDevice";
}
string fingopayFVBSApp::Identify()
{
	cout << "Identify" << std::endl;
	return "Identify";
}
string sthaler::fingopayFVBSApp::Verify()
{
	cout << "Verify" << std::endl;
	return "Verify";
}
string sthaler::fingopayFVBSApp::Capture()
{
	cout << "Capture" << std::endl;
	return "Capture";
}
//void sthaler::fingopayFVBSApp::Cancel() 
//{
//	cout << "Cancel process" << endl;
//}
string sthaler::fingopayFVBSApp::Enroll()
{
	cout << "Enroll" << endl;
	return "Enroll";
}
string sthaler::fingopayFVBSApp::Reset()
{
	cout << "Reset" << endl;
	return "Reset";
}
void sthaler::fingopayFVBSApp::Startup()
{
	//bioaoi

	BioAPI_RETURN rc = fingopayFVBSApp::initFrameWork(BSP_DEFAULT_UUID);
}

BioAPI_RETURN BioAPI sthaler::fingopayFVBSApp::initFrameWork(const char * sBSPUuid)
{

	BioAPI_RETURN ret = BioAPI_OK;

	char msg[256];
	BioAPI_RETURN rc;

	char sBSPStr[50] = { 0 };
	char sLoadedBSP[50] = { 0 };

	//-------- BioAPI_Init --------//	
	rc = BioAPI_Init(m_BioAPIVersion);

	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_Init encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}

	//-------- BioAPI_GetFrameworkInfo --------//
	rc = BioAPI_GetFrameworkInfo(&m_FrameworkSchema);

	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_GetFrameworkInfo encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}

	//-------- BioAPI_EnumBSPs --------//
	rc = BioAPI_EnumBSPs(&m_BSPSchemaList, &m_NumBSP);
	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_EnumBSPs encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}


	int size = sizeof(sLoadedBSP);
	//strncpy(sLoadedBSP, sBSPUuid, size);
	errno_t err = strncpy_s(sLoadedBSP, size,sBSPUuid, _TRUNCATE);
	if (err == STRUNCATE)
		printf("truncation occurred!\n");
	printf("'%s'\n", sLoadedBSP);
	//sLoadedBSP[size - 1] = '\0';

	//UpdateData(TRUE);

	//m_xcCmbBSPUuid.ResetContent();

	for (uint32_t i = 0; i < m_NumBSP; i++) {

		sprintf_s(sBSPStr, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			m_BSPSchemaList[i].BSPUuid[0],
			m_BSPSchemaList[i].BSPUuid[1],
			m_BSPSchemaList[i].BSPUuid[2],
			m_BSPSchemaList[i].BSPUuid[3],
			m_BSPSchemaList[i].BSPUuid[4],
			m_BSPSchemaList[i].BSPUuid[5],
			m_BSPSchemaList[i].BSPUuid[6],
			m_BSPSchemaList[i].BSPUuid[7],
			m_BSPSchemaList[i].BSPUuid[8],
			m_BSPSchemaList[i].BSPUuid[9],
			m_BSPSchemaList[i].BSPUuid[10],
			m_BSPSchemaList[i].BSPUuid[11],
			m_BSPSchemaList[i].BSPUuid[12],
			m_BSPSchemaList[i].BSPUuid[13],
			m_BSPSchemaList[i].BSPUuid[14],
			m_BSPSchemaList[i].BSPUuid[15]
		);


		if (strcmp(sBSPStr, sLoadedBSP) == 0) {

			m_xvCmbBSPUuid = i;
			m_BSP_Uuid = &m_BSPSchemaList[i].BSPUuid;

		}

		m_xcCmbBSPUuid = sBSPStr;
		cout << m_xcCmbBSPUuid;
	}

	//UpdateData(FALSE);

	//-------- BioAPI_BSPLoad --------//
	rc = BioAPI_BSPLoad(m_BSP_Uuid, m_EventHandler, m_EventHandlerCxt);
	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_BSPLoad encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}

	//-------- BioAPI_QueryUnits --------//
	rc = BioAPI_QueryUnits(m_BSP_Uuid, &m_UnitSchemaList, &m_NumUnits);
	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_QueryUnits encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}


	if (m_UnitSchemaList == NULL) {
		sprintf_s(msg, "Unable to find the unit. The process is terminated.\r\n");
		cout << msg;
		return -99;
	}

	//-------- BioAPI_BSPAttach --------//
	BioAPI_UNIT_LIST_ELEMENT UnitList[1];
	UnitList[0].UnitCategory = m_UnitSchemaList[0].UnitCategory;
	UnitList[0].UnitId = m_UnitSchemaList[0].UnitId;

	rc = BioAPI_BSPAttach(m_BSP_Uuid,
		m_BioAPIVersion,
		UnitList,
		1,
		&m_BSPHandle);
	if (rc != BioAPI_OK) {
		sprintf_s(msg, "BioAPI_BSPAttach encountered an error. (0x%08x)\r\n", rc);
		cout << msg;
		return -99;
	}

	return ret;
}
BioAPI_RETURN sthaler::fingopayFVBSApp::terminateFrameWork(void)
{

	freeUnitSchemaList(&m_UnitSchemaList, &m_NumUnits);

	//-------- BioAPI_BSPDetach --------//
	BioAPI_RETURN rc = BioAPI_BSPDetach(m_BSPHandle);
	if (rc != BioAPI_OK) {
		// nop
	}

	//-------- BioAPI_BSPUnload --------//
	rc = BioAPI_BSPUnload(m_BSP_Uuid, m_EventHandler, m_EventHandlerCxt);
	if (rc != BioAPI_OK) {
		// nop
	}


	freeBSPSchemaList(&m_BSPSchemaList, &m_NumBSP);

	//-------- BioAPI_Terminate --------//
	rc = BioAPI_Terminate();
	if (rc != BioAPI_OK) {
		// nop
	}

	return rc;
}
void fingopayFVBSApp::freeBSPSchemaList(BioAPI_BSP_SCHEMA **ppBSPSchemaList, uint32_t *pNumBSP)
{
	BioAPI_BSP_SCHEMA *pBSPSchemaList = *ppBSPSchemaList;
	uint32_t nNumBSP = *pNumBSP;

	if (pBSPSchemaList == NULL) {
		return;
	}

	for (uint32_t i = 0; i < nNumBSP; i++)
	{
		BioAPI_Free(pBSPSchemaList[i].Path);
		BioAPI_Free(pBSPSchemaList[i].BSPSupportedFormats);
	}

	BioAPI_Free(pBSPSchemaList);
	*ppBSPSchemaList = NULL;

	*pNumBSP = 0;
}
void fingopayFVBSApp::freeUnitSchemaList(BioAPI_UNIT_SCHEMA **ppUnitSchemaList, uint32_t *pNumUnits)
{
	BioAPI_UNIT_SCHEMA *pUnitSchemaList = *ppUnitSchemaList;
	uint32_t nNumUnits = *pNumUnits;

	if (pUnitSchemaList == NULL) {
		return;
	}

	for (uint32_t i = 0; i < nNumUnits; i++)
	{
		BioAPI_Free(pUnitSchemaList[i].UnitProperty.Data);
	}

	BioAPI_Free(pUnitSchemaList);
	*ppUnitSchemaList = NULL;

	*pNumUnits = 0;
}

sthaler::fingopayFVBSApp::~fingopayFVBSApp()
{
	//destructor
	BioAPI_RETURN rc = terminateFrameWork();
}

