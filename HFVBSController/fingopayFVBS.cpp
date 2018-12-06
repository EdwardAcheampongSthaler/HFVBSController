
// STH.FingopayFVBS.cpp : Defines the class behaviors for the application.
//
#pragma once

#include "stdafx.h"
#include "fingopayFVBS.h"
#include <iostream>
#include "utilities.h"
#include <vector>
#include "base64.h"


//#define BSP_DEFAULT_UUID	( "85b448bf-36a6-4843-b34b-bcdfef45d1d7" ) 
#define BSP_DEFAULT_UUID	( "e41a9357-67df-44d5-80e3-64935d21b642" )  

//using namespace std;
using namespace sthaler;
using namespace core;

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
	//bioaoi

	BioAPI_RETURN rc = fingopayFVBSApp::initFrameWork(BSP_DEFAULT_UUID);
	cout << "Initialise Finish" << std::endl;
	return "Initialise complete";
}
string fingopayFVBSApp::Terminate()
{
	cout << "Terminate" << std::endl;

	BioAPI_RETURN rc = terminateFrameWork();
	return "Terminate";
}
string fingopayFVBSApp::ControlBiometricDevice()
{
	cout << "ControlBiometricDevice" << endl;

	char msg[256];
	bool c_flg = false;
	BioAPI_DATA InputData, OutputData = { 0, NULL };

	BioAPI_UNIT_ID unit_id = getUnitID();

	int nInputData[1];

	if (c_flg) {
		nInputData[0] = 0;
	}

	else {
		nInputData[0] = 1;
	}
	InputData.Length = 1;
	InputData.Data = nInputData;

	//-------- BioAPI_ControlUnit--------//
	BioAPI_RETURN rc = BioAPI_ControlUnit(m_BSPHandle, unit_id, 5, &InputData, &OutputData);
	if (rc != BioAPI_OK) {
		utilities::printfmsg("BioAPI_ControlUnit encountered an error. (0x%08x)\r\n");
		return "";
	}
	else {
		return BioAPI_OK;
	}

	return "ControlBiometricDevice";
}
string fingopayFVBSApp::Identify()
{
	cout << "Identify Called" << endl;

	string GName;
	char msg[256];
	BioAPI_RETURN rc;

	if (rc != BioAPI_OK) {
		utilities::printfmsg("An error occurred in sound alert control.");
		return "";
	}
	int m_All_BirCnt = 1;
	int p_cnt = 0;

	for (unsigned int j = 0; j < m_All_BirCnt; j++) {

		if (m_DataInfoList[j].GName == GName) {
			p_cnt++;
		}
	}

	uint32_t NumberOfResults;
	BioAPI_CANDIDATE *Candidates = NULL;

	BioAPI_FMR MaxFMRRequested = DEFAULT_MAX_FMR_REQUESTED;
	BioAPI_IDENTIFY_POPULATION Population;

	Population.Type = BioAPI_ARRAY_TYPE;
	Population.BIRs.BIRArray = (BioAPI_BIR_ARRAY_POPULATION*)malloc(sizeof(BioAPI_BIR_ARRAY_POPULATION));
	Population.BIRs.BIRArray->NumberOfMembers = p_cnt;
	Population.BIRs.BIRArray->Members = (BioAPI_BIR*)malloc(sizeof(BioAPI_BIR) * p_cnt);
	int t = 0;
	for (unsigned int i = 0; i < m_All_BirCnt; i++) {

		if (m_DataInfoList[i].GName != GName) {
			continue;
		}
		Population.BIRs.BIRArray->Members[t].Header = m_BIRList[i].Header;
		Population.BIRs.BIRArray->Members[t].BiometricData.Length = m_BIRList[i].BiometricData.Length;
		if (Population.BIRs.BIRArray->Members[t].BiometricData.Length > 0) {
			Population.BIRs.BIRArray->Members[t].BiometricData.Data = malloc(Population.BIRs.BIRArray->Members[t].BiometricData.Length);
			memcpy(Population.BIRs.BIRArray->Members[t].BiometricData.Data, m_BIRList[i].BiometricData.Data, m_BIRList[i].BiometricData.Length);
		}
		Population.BIRs.BIRArray->Members[t].SecurityBlock.Length = 0;
		Population.BIRs.BIRArray->Members[t].SecurityBlock.Data = NULL;

		t++;
	}

	//-------- BioAPI_Identify --------//
	rc = BioAPI_Identify(m_BSPHandle, MaxFMRRequested,
		BioAPI_NO_SUBTYPE_AVAILABLE, &Population, p_cnt, BioAPI_FALSE, 0,
		&NumberOfResults, &Candidates, 10000, NULL);

	if (rc != BioAPI_OK) {

		if (rc == 0x01000118) {
			utilities::printfmsg("Processing cancelled by the user.");
		}
		else {
			utilities::printfmsg("BioAPI_Identify encountered an error. (0x%08x)\r\n");
		}
		return "";
	}

	utilities::printfmsg("Identification %s.\r\n\r\nMatched BIRs: %d" + NumberOfResults);
	utilities::printfmsg((Candidates != NULL) ? "successful" : "failed");


	for (uint32_t i = 0; i < NumberOfResults; i++)
	{
		BioAPI_Free(Candidates[i].BIR.BIRInArray);
	}

	BioAPI_Free(Candidates);
	Candidates = NULL;

	for (int i = 0; i < p_cnt; i++)
	{
		free(Population.BIRs.BIRArray->Members[i].BiometricData.Data);
	}
	free(Population.BIRs.BIRArray->Members);
	free(Population.BIRs.BIRArray);

	return "Identify Ended";
}
string sthaler::fingopayFVBSApp::Verify()
{
	cout << "Verify" << endl;


	char msg[256];
	BioAPI_RETURN rc;

	int m_xvSelectedIndex = 0;

	if (m_xvSelectedIndex == -1) {
		utilities::printfmsg("Select a template to match.");
		return "";
	}

	BioAPI_BOOL Result;
	BioAPI_FMR FMRAchieved;

	BioAPI_FMR MaxFMRRequested = DEFAULT_MAX_FMR_REQUESTED;
	BioAPI_INPUT_BIR ReferenceTemplateInputData;
	ReferenceTemplateInputData.Form = BioAPI_FULLBIR_INPUT;
	ReferenceTemplateInputData.InputBIR.BIR = &(m_BIRList[m_xvSelectedIndex]);

	//-------- BioAPI_Verify --------//
	rc = BioAPI_Verify(m_BSPHandle, MaxFMRRequested,
		&ReferenceTemplateInputData, BioAPI_NO_SUBTYPE_AVAILABLE, NULL,
		&Result, &FMRAchieved, NULL, 10000, NULL);

	if (rc != BioAPI_OK) {

		if (rc == 0x01000118) {
			utilities::printfmsg("Processing cancelled by the user.");
		}
		else {
			utilities::printfmsg("BioAPI_Verify encountered an error. (0x%08x)\r\n");
		}
		return "";
	}

	utilities::printfmsg("Authentication %s.");
	utilities::printfmsg((Result == BioAPI_FALSE) ? "failed" : "successful");


	return "Verify Ended";
}
string sthaler::fingopayFVBSApp::Capture()
{
	cout << "Capture" << std::endl;

	return "Capture";
}
void sthaler::fingopayFVBSApp::Cancel()
{
	cout << "Cancel process" << endl;
}
string sthaler::fingopayFVBSApp::Enroll()
{
	char msg[256];
	BioAPI_RETURN rc;


	cout << "Enroll Initiated" << endl;


	BioAPI_BIR_HANDLE TemplateBIRHandle;

	BioAPI_BIR_PURPOSE Purpose = BioAPI_PURPOSE_ENROLL;
	BioAPI_BIR_SUBTYPE Subtype = BioAPI_NO_SUBTYPE_AVAILABLE;
	int32_t Timeout = 10000;

	//-------- BioAPI_Enroll --------//
	rc = BioAPI_Enroll(m_BSPHandle, Purpose, Subtype, NULL, NULL, &TemplateBIRHandle, NULL, Timeout, NULL, NULL);
	cout << rc << endl;
	cout << &rc << endl;
	if (rc != BioAPI_OK) {

		printf("an encountered an error. (0x%08x)\r\n", rc);
		if (rc == 0x01000118) {
			//cout << "Processing cancelled by the user." << endl;
			printf("Processing cancelled by the user.(0x%08x)\r\n", rc);
		}
		else {
			cout << "BioAPI_Enroll encountered an error." << rc << endl;
			printf("BioAPI_Enroll encountered an error. (0x%08x)\r\n", rc);
		}
		return "Errored out";
	}
	cout << "Getting BIR From Handle" << endl;

	BioAPI_BIR BIR;
	BIR.BiometricData.Data = NULL;
	BIR.SecurityBlock.Data = NULL;


	// Custom - get he header from the handle.
	BioAPI_BIR_HEADER Header;
	rc = BioAPI_GetHeaderFromHandle(m_BSPHandle, TemplateBIRHandle, &Header);
	if (rc != BioAPI_OK) {
		cout << "BioAPI_GetHeaderFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetHeaderFromHandle encountered an error. (0x%08x)\r\n");
		return "BioAPI_BIR_HEADER";
	}

	//-------- BioAPI_GetBIRFromHandle --------//
	rc = BioAPI_GetBIRFromHandle(m_BSPHandle, TemplateBIRHandle, &BIR);
	cout << &BIR << endl;


	if (rc != BioAPI_OK) {
		cout << "BioAPI_GetBIRFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		return "BIO API";
	}
	cout << "Obtained BIR From Handle: memory lock: " << &BIR << endl;


	// get bir info 
	BIR_HANDLE_INFO BIRInfo;
	BIRInfo.Handle = TemplateBIRHandle;
	BIRInfo.Purpose = Header.Purpose;
	BIRInfo.Type = Header.Type;


	// create template
	int offset = 0;
	int numOfBytesToAdd = 0;
	//for (uint32_t i = 0; i < BIR.BiometricData.Length; i += offset)
	//{
		//Index			Offset		Description
		//0				4			total number of bytes to follow (56 + bioLen + secLen + 4 if secLen is not zero) written as a 4-byte integer (iLen).
		uint32_t part1 = (56 + BIR.BiometricData.Length + BIR.SecurityBlock.Length + (BIR.SecurityBlock.Length != 0 ? 4 : 0));
		BYTE *newtemplate = new BYTE[part1];
		//unsigned char *newtemplate = new unsigned char[part1];
		//memset(newtemplate, 0, part1);
		utilities myutilities = utilities();
		vector<BYTE> mybytes = myutilities.intToBytes(part1);
		//numOfBytesToAdd = sizeof(uint32_t);
		
		memcpy(&newtemplate[0], &part1, numOfBytesToAdd = sizeof(uint32_t)); // 4bytes
		cout << &part1 << endl;
		cout << &newtemplate << endl;
		
		for (int i = offset; i < offset + numOfBytesToAdd; i++) 
		{
			cout << i << ": " << &newtemplate[i] << endl;
			
		}
		offset += numOfBytesToAdd;
		
		//4            23           memcpy the first 23 bytes from processedBIR.Header
		memcpy(&newtemplate[4], &Header, 23); // 23 bytes
		cout << sizeof(&BIR.Header) << " : " << &Header << endl;
		cout << newtemplate << endl;
		numOfBytesToAdd = 23;
		//numOfBytesToAdd = 23;
	
		for (int i = offset; i < offset + numOfBytesToAdd; i++)
		{
			cout << i << ": " << &newtemplate[i] << endl;
			
		}
		offset += numOfBytesToAdd;
//
		numOfBytesToAdd = 1;

		//27           1             copy 1 byte from processedBIR.Subtype
		memcpy(&newtemplate[27], &BIR.Header.Subtype, numOfBytesToAdd);
		cout << newtemplate << endl;
		cout << sizeof(&BIR.Header.Subtype) << endl;


		for (int i = offset; i < offset + numOfBytesToAdd; i++)
		{
			cout << i << ": " << &newtemplate[i] << endl;			
		}
		offset += numOfBytesToAdd;
		cout << newtemplate << endl;

//
		numOfBytesToAdd = 24;
		//28           24           memcpy the last 24 bytes of BIR header (from processedBIR.ExpirationDate)
		int sizeofHeader = sizeof(BIR.Header); // should be 52
		BYTE *temp = new BYTE[sizeofHeader];
		memcpy(&temp[0], &BIR.Header, sizeofHeader); // copy the entire header into a temp array
		memcpy(&newtemplate[28], &temp[25], numOfBytesToAdd); //from 25 onwards copy the data into ur template 
		 // delete temp
		delete[] temp;

		for (int i = offset; i < offset + numOfBytesToAdd; i++)
		{
			cout << i << ": " << &newtemplate[i] << endl;
		}
		offset += numOfBytesToAdd;
//
		numOfBytesToAdd = 4;
		cout << newtemplate << endl;

		//52           4             bioLen (written as an integer)

		memcpy(&newtemplate[52], &BIR.BiometricData.Length, 4); // 4 bytes

		for (int i = offset; i < offset + numOfBytesToAdd; i++)
		{
			cout << i << ": " << &newtemplate[i] << endl;
		}
		offset += numOfBytesToAdd;
		cout << newtemplate << endl;

// 
		numOfBytesToAdd = BIR.BiometricData.Length;
		//56           bioLen   The encrypted biometric data from the BIR
		memcpy(&newtemplate[56], &BIR.BiometricData.Data, BIR.BiometricData.Length);

		for (int i = offset; i < offset + numOfBytesToAdd; i++)
		{
			cout << i << ": " << &newtemplate[i] << endl;
		}
		offset += numOfBytesToAdd;

//
		cout << newtemplate << endl;

		int secLen = (BIR.SecurityBlock.Length > 0) ? BIR.SecurityBlock.Length + 4 : 0;
		//if secLen is not zero
		if (BIR.SecurityBlock.Length != 0) 
		{
			numOfBytesToAdd = 4;

			//56+bioLen, 4      secLen (written as an integer)
			memcpy(&newtemplate[56+ BIR.BiometricData.Length], &BIR.SecurityBlock.Length, 4);
			numOfBytesToAdd = BIR.SecurityBlock.Length;

			//…            secLen The security data from the BIR.
			memcpy(&newtemplate[56 + BIR.BiometricData.Length + 4], &BIR.SecurityBlock.Data, BIR.SecurityBlock.Length);
		}
//	}
		int totalsize = 56 + BIR.BiometricData.Length + secLen;

		cout << "total size : " << totalsize << endl;

		std::string myoutput((char*)newtemplate, totalsize );
		cout << myoutput << endl;

		string base64myoutput = base64_encode(reinterpret_cast<const unsigned char*>(myoutput.c_str()), myoutput.length());

	////std::string data = malloc(BIR.BiometricData.Length);
	//const int size = sizeof(BIR.BiometricData.Data);
	//int length = BIR.BiometricData.Length;
	//cout << size << endl; // size of pointer is 8 
	//cout << length << endl; // size of BIR data
	//BYTE *bytes = new BYTE[BIR.BiometricData.Length];
	//unsigned char *uc = new unsigned char[length];
	//memcpy(uc, BIR.BiometricData.Data, BIR.BiometricData.Length);
	//std::string sa(reinterpret_cast<char const*>(uc));

	////memcpy(&bytes, &BIR.BiometricData.Data, sizeof(BIR.BiometricData.Data));
	//cout << sa << endl;

	////std::string newString;
	////newString.resize(length);
	////memcpy((char*)newString.data(), &bytes[0], length);
	////cout << newString << endl;
	//
	//const int edsize = sizeof(BIR.BiometricData.Data);

 //   char *data = new char[edsize];
	//char *cptr = (char *)(BIR.BiometricData.Data);
	//while (*cptr != '\0') {
	//	sprintf(data,"%c", *cptr);
	//	printf("%c", *cptr);
	//	cptr++;
	//}




	//// Capture
	//BioAPI_BIR_HANDLE CapturedBIRHandle;

	//rc = BioAPI_Capture(m_BSPHandle, BioAPI_PURPOSE_ENROLL, BioAPI_NO_SUBTYPE_AVAILABLE,
	//	NULL, &CapturedBIRHandle, Timeout, NULL);
	//if (rc != BioAPI_OK) {
	//	sprintf(msg, "BioAPI_Capture(CreateTemplate) failed.(0x%08x)", rc);
	//	cout << "BioAPI_Capture(CreateTemplate) failed.(0x%08x)" << msg << endl;
	//	return "";
	//}

	//BioAPI_INPUT_BIR CapturedBIRInputData;
	//BioAPI_BIR_HANDLE TemplateBIRHandle2;

	//CapturedBIRInputData.Form = BioAPI_BIR_HANDLE_INPUT;
	//CapturedBIRInputData.InputBIR.BIRinBSP = &CapturedBIRHandle;
	//rc = BioAPI_CreateTemplate(m_BSPHandle, &CapturedBIRInputData, NULL,
	//	NULL, &TemplateBIRHandle2, NULL, NULL);
	//if (rc != BioAPI_OK) {
	//	sprintf(msg, "BioAPI_CreateTemplate failed.(0x%08x)", rc);
	//	cout << "BioAPI_CreateTemplate failed.(0x%08x)" << msg << endl;

	//	return "";
	//}
	//BioAPI_BIR BIR2;
	//BIR2.BiometricData.Data = NULL;
	//BIR2.SecurityBlock.Data = NULL;
	//rc = BioAPI_GetBIRFromHandle(m_BSPHandle, TemplateBIRHandle2, &BIR2);
	//cout << &BIR2 << endl;
	//
	//const int size2 = sizeof(BIR2.BiometricData.Data);
	//int length2 = BIR2.BiometricData.Length;
	//cout << size2 << endl; // size of pointer is 8 
	//cout << length2 << endl; // size of BIR data
	//BYTE *bytes2 = new BYTE[BIR2.BiometricData.Length];
	//unsigned char *uc2 = new unsigned char[length2];
	//memcpy(bytes2, BIR2.BiometricData.Data, BIR2.BiometricData.Length);
	//memcpy(uc2, BIR2.BiometricData.Data, BIR2.BiometricData.Length);
	//std::string sa2(reinterpret_cast<char const*>(uc2));
	//cout << sa2 << endl;

	//delete bytes;
	//delete bytes2;
	//delete sp;

	printf("Enrolled successful.");
	cout << myoutput << endl;
	cout << base64myoutput << endl;

	// return
	return base64myoutput;

}
void sthaler::fingopayFVBSApp::Startup()
{
	//bioapi
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
		utilities::printfmsg("BioAPI_Init encountered an error. (0x%08x)\r\n");
		cout << msg;
		return -99;
	}

	//-------- BioAPI_GetFrameworkInfo --------//
	rc = BioAPI_GetFrameworkInfo(&m_FrameworkSchema);

	if (rc != BioAPI_OK) {
		utilities::printfmsg("BioAPI_GetFrameworkInfo encountered an error. (0x%08x)\r\n");
		cout << msg;
		return -99;
	}

	//-------- BioAPI_EnumBSPs --------//
	rc = BioAPI_EnumBSPs(&m_BSPSchemaList, &m_NumBSP);
	if (rc != BioAPI_OK) {
		utilities::printfmsg("BioAPI_EnumBSPs encountered an error. (0x%08x)\r\n");
		cout << msg;
		return -99;
	}


	int size = sizeof(sLoadedBSP);
	//strncpy(sLoadedBSP, sBSPUuid, size);
	errno_t err = strncpy_s(sLoadedBSP, size, sBSPUuid, _TRUNCATE);
	if (err == STRUNCATE)
		utilities::printfmsg("truncation occurred!\n");
	utilities::printfmsg(sLoadedBSP);
	//sLoadedBSP[size - 1] = '\0';

	//UpdateData(TRUE);

	//m_xcCmbBSPUuid.ResetContent();

	for (uint32_t i = 0; i < m_NumBSP; i++) {

		sprintf(sBSPStr, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
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
		utilities::printfmsg("BioAPI_BSPLoad encountered an error. (0x%08x)\r\n");
		cout << msg;
		return -99;
	}

	//-------- BioAPI_QueryUnits --------//
	rc = BioAPI_QueryUnits(m_BSP_Uuid, &m_UnitSchemaList, &m_NumUnits);
	if (rc != BioAPI_OK) {
		utilities::printfmsg("BioAPI_QueryUnits encountered an error. (0x%08x)\r\n");
		cout << msg;
		return -99;
	}


	if (m_UnitSchemaList == NULL) {
		utilities::printfmsg("Unable to find the unit. The process is terminated.\r\n");
		std::cout << msg;
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
		utilities::printfmsg("BioAPI_BSPAttach encountered an error. (0x%08x)\r\n");
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
BioAPI_UNIT_ID fingopayFVBSApp::getUnitID(void)
{
	return m_UnitSchemaList[0].UnitId;
}
sthaler::fingopayFVBSApp::~fingopayFVBSApp()
{
	//destructor
	BioAPI_RETURN rc = terminateFrameWork();
}

