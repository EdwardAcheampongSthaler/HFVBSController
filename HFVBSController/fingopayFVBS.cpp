
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
string sthaler::fingopayFVBSApp::Verify(BioAPI_BIR_HANDLE processedTemplateBIRHandle)
{
	char msg[256];
	BioAPI_RETURN rc;

	cout << "Verify Initiated" << endl;

	BioAPI_BIR_HANDLE CapturedBIRHandle;
	BioAPI_BIR_HANDLE CapturedProcessedBIRHandle;
	BioAPI_BIR_PURPOSE Purpose = BioAPI_PURPOSE_VERIFY;
	BioAPI_BIR_SUBTYPE Subtype = BioAPI_NO_SUBTYPE_AVAILABLE;
	int32_t Timeout = 10000;


	//-------- BioAPI_Capture for Verification --------//
	rc = BioAPI_Capture(m_BSPHandle, Purpose, Subtype, NULL, &CapturedBIRHandle, Timeout, NULL);

	if (rc != BioAPI_OK) {
		// print error
	}

	BioAPI_INPUT_BIR CapturedBIRInputData;

	CapturedBIRInputData.Form = BioAPI_BIR_HANDLE_INPUT;
	CapturedBIRInputData.InputBIR.BIRinBSP = &CapturedBIRHandle;
	// process the BIR before obtaining the header and processing.
	rc = BioAPI_Process(m_BSPHandle, &CapturedBIRInputData, NULL, &CapturedProcessedBIRHandle);
	if (rc != BioAPI_OK) {
		// print error
	}
	BioAPI_BIR_HEADER capturedHeader;
	rc = BioAPI_GetHeaderFromHandle(m_BSPHandle, CapturedProcessedBIRHandle, &capturedHeader);
	if (rc != BioAPI_OK) {
		sprintf(msg, "BioAPI_GetHeaderFromHandle encountered an error. (0x%08x)\r\n");
		printf(msg);
		return "BioAPI_BIR_HEADER";
	}

	BioAPI_BIR captureBir;
	captureBir.BiometricData.Data = NULL;
	captureBir.SecurityBlock.Data = NULL;

	//-------- BioAPI_GetBIRFromHandle --------//
	rc = BioAPI_GetBIRFromHandle(m_BSPHandle, CapturedProcessedBIRHandle, &captureBir);

	if (rc != BioAPI_OK) {
		sprintf(msg, "BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		printf(msg);
		return msg;
	}


	// create template for enrollment and for verification
	string verify = CreateFvtTemplateFromBIR(captureBir, capturedHeader);

	cout << "----Verifcation Template----" << endl;

	cout << verify << endl;

	return verify;
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
	BioAPI_BIR_HANDLE ProcessedBIRHandle;
	BioAPI_BIR_PURPOSE Purpose = BioAPI_PURPOSE_ENROLL;
	BioAPI_BIR_SUBTYPE Subtype = BioAPI_NO_SUBTYPE_AVAILABLE;
	int32_t Timeout = 10000;

	//-------- BioAPI_Enroll --------//
	rc = BioAPI_Enroll(m_BSPHandle, Purpose, Subtype, NULL, NULL, &TemplateBIRHandle, NULL, Timeout, NULL, NULL);

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

	// Custom - get he header from the handle.
	BioAPI_BIR_HEADER Header;

	rc = BioAPI_GetHeaderFromHandle(m_BSPHandle, TemplateBIRHandle, &Header);
	if (rc != BioAPI_OK) {
		cout << "BioAPI_GetHeaderFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetHeaderFromHandle encountered an error. (0x%08x)\r\n");
		return "BioAPI_BIR_HEADER";
	}

	BioAPI_BIR BIR;
	BIR.BiometricData.Data = NULL;
	BIR.SecurityBlock.Data = NULL;

	//-------- BioAPI_GetBIRFromHandle --------//
	rc = BioAPI_GetBIRFromHandle(m_BSPHandle, TemplateBIRHandle, &BIR);
	cout << &BIR << endl;

	if (rc != BioAPI_OK) {
		sprintf(msg,"BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		printf(msg);
		return msg;
	}
	cout << "Obtained BIR From Handle: memory location: " << &BIR << endl;
	cout << &BIR << endl;


	// create template
	int offset = 0;
	int numOfBytesToAdd = 0;
	//Index			Offset		Description
	//0				4			total number of bytes to follow (56 + bioLen + secLen + 4 if secLen is not zero) written as a 4-byte integer (iLen).
    uint32_t bytesToFollow = (56 + BIR.BiometricData.Length + (BIR.SecurityBlock.Length != 0 ? 4 + BIR.SecurityBlock.Length : 0));
	//BYTE *fvtTemplate = new BYTE[bytesToFollow+4];
	unsigned char *fvtTemplate = new unsigned char[bytesToFollow+4];
	//memcpy(&fvtTemplate[0], &bytesToFollow, sizeof(uint32_t)); // 4bytes
	//cout << &bytesToFollow << endl;
	memcpy(&fvtTemplate[0], &bytesToFollow, sizeof(uint32_t)); // 4bytes
	memcpy(&fvtTemplate[4], &BIR.Header.HeaderVersion, 1); // 1 bytes
	memcpy(&fvtTemplate[5], &BIR.Header.Type, 1); // 1 bytes
	memcpy(&fvtTemplate[6], &BIR.Header.Format.FormatOwner, 2); // 2 bytes
	memcpy(&fvtTemplate[8], &BIR.Header.Format.FormatType, 2); // 4 bytes
	memcpy(&fvtTemplate[10], &BIR.Header.Quality, 1); // 1 bytes
	memcpy(&fvtTemplate[11], &BIR.Header.Purpose, 1); // 1 bytes
	memcpy(&fvtTemplate[12], &BIR.Header.FactorsMask, 4); // 4 bytes
	memcpy(&fvtTemplate[16], &BIR.Header.ProductID.ProductOwner, 2); // 4 bytes
	memcpy(&fvtTemplate[18], &BIR.Header.ProductID.ProductType, 2); // 4 bytes
	memcpy(&fvtTemplate[20], &BIR.Header.CreationDTG.Date.Year, 2); // 7 bytes
	memcpy(&fvtTemplate[22], &BIR.Header.CreationDTG.Date.Month, 1); // 7 bytes
	memcpy(&fvtTemplate[23], &BIR.Header.CreationDTG.Date.Day, 1); // 7 bytes
	memcpy(&fvtTemplate[24], &BIR.Header.CreationDTG.Time.Hour, 1); // 7 bytes
	memcpy(&fvtTemplate[25], &BIR.Header.CreationDTG.Time.Minute, 1); // 7 bytes
	memcpy(&fvtTemplate[26], &BIR.Header.CreationDTG.Time.Second, 1); // 7 bytes
	
	memcpy(&fvtTemplate[27], &Header.Subtype, 1);
	// get last bytes individually.
	//28           24 (4+4+16)           memcpy the last 24 bytes of BIR header (from processedBIR.ExpirationDate)
	memcpy(&fvtTemplate[28], &BIR.Header.ExpirationDate.Year, 2); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[30], &BIR.Header.ExpirationDate.Month, 1); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[31], &BIR.Header.ExpirationDate.Day, 1); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[32], &BIR.Header.SBFormat.SecurityFormatOwner, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[34], &BIR.Header.SBFormat.SecurityFormatType, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[36], &BIR.Header.Index, 16); //from 36 onwards copy the data into ur template 
	memcpy(&fvtTemplate[52], &BIR.BiometricData.Length, 4); // 4 bytes
	//56           bioLen   The encrypted biometric data from the BIR
	//memset(&fvtTemplate[56], 0, BIR.BiometricData.Length);
	memcpy(&fvtTemplate[56], BIR.BiometricData.Data, BIR.BiometricData.Length);// pointer data

	int secLen = (BIR.SecurityBlock.Length > 0) ? BIR.SecurityBlock.Length + 4 : 0;
	//if secLen is not zero
	if (secLen != 0)
	{
		numOfBytesToAdd = 4;

		//56+bioLen, 4      secLen (written as an integer)
		memcpy(&fvtTemplate[60 + BIR.BiometricData.Length], &BIR.SecurityBlock.Length, 4);
		numOfBytesToAdd = BIR.SecurityBlock.Length;

		//…            secLen The security data from the BIR.
		memcpy(&fvtTemplate[60 + BIR.BiometricData.Length + 4], BIR.SecurityBlock.Data, BIR.SecurityBlock.Length);
	}
	memcpy(&fvtTemplate[56 + BIR.BiometricData.Length], &BIR.SecurityBlock.Length, 4);

	int totalsize = 56 + BIR.BiometricData.Length + 4 + secLen;
	cout << "Total size : " << totalsize << endl;

	std::string templateOutput((char*)fvtTemplate, totalsize);

	string base64myoutput = base64_encode(reinterpret_cast<const unsigned char*>(templateOutput.c_str()), templateOutput.length());
	utilities helper;
	unsigned char* base64newarray = helper.spc_base64_encode(fvtTemplate, 588, false);

	//fvtTemplate = NULL;
	delete[] fvtTemplate;
	//printf("Template generation successful");
	cout << "Template generation successful" << endl;
	cout << "####################" << endl;
	cout << templateOutput << endl;
	cout << "####################" << endl;
	cout << base64myoutput << endl;
	cout << "####################" << endl;


	// return
	return base64myoutput;
}

string sthaler::fingopayFVBSApp::EnrollAndVerify() 
{
	char msg[256];
	BioAPI_RETURN rc;

	cout << "Enroll And Verify Initiated" << endl;

	BioAPI_BIR_HANDLE TemplateBIRHandle;
	BioAPI_BIR_HANDLE ProcessedBIRHandle;
	BioAPI_BIR_PURPOSE Purpose = BioAPI_PURPOSE_ENROLL;
	BioAPI_BIR_SUBTYPE Subtype = BioAPI_NO_SUBTYPE_AVAILABLE;
	int32_t Timeout = 10000;

	//-------- BioAPI_Enroll --------//
	rc = BioAPI_Enroll(m_BSPHandle, Purpose, Subtype, NULL, NULL, &TemplateBIRHandle, NULL, Timeout, NULL, NULL);
	if (rc != BioAPI_OK) {

		printf("an encountered an error. (0x%08x)\r\n", rc);
		if (rc == 0x01000118) {
			//cout << "Processing cancelled by the user." << endl;
			printf("Processing cancelled by the user.(0x%08x)\r\n", rc);
		}
		else {
			printf("BioAPI_Enroll encountered an error. (0x%08x)\r\n", rc);
		}
		return "Errored out";
	}

	// Custom - get he header from the handle.
	BioAPI_BIR_HEADER Header;

	rc = BioAPI_GetHeaderFromHandle(m_BSPHandle, TemplateBIRHandle, &Header);
	if (rc != BioAPI_OK) {
		cout << "BioAPI_GetHeaderFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetHeaderFromHandle encountered an error. (0x%08x)\r\n");
		return "BioAPI_BIR_HEADER";
	}

	BioAPI_BIR BIR;
	BIR.BiometricData.Data = NULL;
	BIR.SecurityBlock.Data = NULL;

	//-------- BioAPI_GetBIRFromHandle --------//
	rc = BioAPI_GetBIRFromHandle(m_BSPHandle, TemplateBIRHandle, &BIR);

	if (rc != BioAPI_OK) {
		sprintf(msg, "BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		printf(msg);
		return msg;
	}
	cout << "Obtained BIR From Handle: memory location: " << &BIR << endl;

	// Now verify the BIR by performing a verification 
	//BioAPI_BOOL Result;
	//BioAPI_FMR FMRAchieved;

	//BioAPI_FMR MaxFMRRequested = DEFAULT_MAX_FMR_REQUESTED;
	//BioAPI_INPUT_BIR ReferenceTemplateInputData;
	//ReferenceTemplateInputData.Form = BioAPI_FULLBIR_INPUT;
	//ReferenceTemplateInputData.InputBIR.BIR = &BIR;  // << the BIR

	////-------- BioAPI_Verify --------//
	//rc = BioAPI_Verify(m_BSPHandle, MaxFMRRequested,
	//	&ReferenceTemplateInputData, BioAPI_NO_SUBTYPE_AVAILABLE, NULL,
	//	&Result, &FMRAchieved, NULL, 10000, NULL);

	//if (rc != BioAPI_OK) {

	//	if (rc == 0x01000118) {
	//		utilities::printfmsg("Processing cancelled by the user.");
	//	}
	//	else {
	//		utilities::printfmsg("BioAPI_Verify encountered an error. (0x%08x)\r\n");
	//	}
	//	return "";
	//}

	BioAPI_BIR_HANDLE CapturedBIRHandle;
	BioAPI_BIR_HANDLE CapturedProcessedBIRHandle;
	Purpose = BioAPI_PURPOSE_VERIFY;
	Subtype = BioAPI_NO_SUBTYPE_AVAILABLE;

	//-------- BioAPI_Capture for Verification --------//
	rc = BioAPI_Capture(m_BSPHandle, Purpose, Subtype, NULL, &CapturedBIRHandle,  Timeout, NULL);

	if (rc != BioAPI_OK) {
		// print error
	}

	BioAPI_INPUT_BIR CapturedBIRInputData;

	CapturedBIRInputData.Form = BioAPI_BIR_HANDLE_INPUT;
	CapturedBIRInputData.InputBIR.BIRinBSP = &CapturedBIRHandle;
	// process the BIR before obtaining the header and processing.
	rc = BioAPI_Process(m_BSPHandle, &CapturedBIRInputData, NULL, &CapturedProcessedBIRHandle);
	if (rc != BioAPI_OK) {
		// print error
	}
	BioAPI_BIR_HEADER capturedHeader;
	rc = BioAPI_GetHeaderFromHandle(m_BSPHandle, CapturedProcessedBIRHandle, &capturedHeader);
	if (rc != BioAPI_OK) {
		cout << "BioAPI_GetHeaderFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetHeaderFromHandle encountered an error. (0x%08x)\r\n");
		return "BioAPI_BIR_HEADER";
	}

	BioAPI_BIR captureBir;
	captureBir.BiometricData.Data = NULL;
	captureBir.SecurityBlock.Data = NULL;

	//-------- BioAPI_GetBIRFromHandle --------//
	rc = BioAPI_GetBIRFromHandle(m_BSPHandle, CapturedProcessedBIRHandle, &captureBir);

	if (rc != BioAPI_OK) {
		sprintf(msg, "BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		printf(msg);
		return msg;
	}


	// create template for enrollment and for verification
	string enrollment = CreateFvtTemplateFromBIR(BIR, Header);
	string verify = CreateFvtTemplateFromBIR(captureBir, capturedHeader);
	cout << enrollment << endl;

	cout << "----" << endl;

	cout << verify << endl;

	return "Verify Ended";
}

string sthaler::fingopayFVBSApp::CreateFvtTemplateFromBIR(BioAPI_BIR bir, BioAPI_BIR_HEADER header)
{
	// create template
	int offset = 0;
	int numOfBytesToAdd = 0;
	//Index			Offset		Description
	//0				4			total number of bytes to follow (56 + bioLen + secLen + 4 if secLen is not zero) written as a 4-byte integer (iLen).
	uint32_t bytesToFollow = (56 + bir.BiometricData.Length + (bir.SecurityBlock.Length != 0 ? 4 + bir.SecurityBlock.Length : 0));
	//BYTE *fvtTemplate = new BYTE[bytesToFollow+4];
	unsigned char *fvtTemplate = new unsigned char[bytesToFollow + 4];
	//memcpy(&fvtTemplate[0], &bytesToFollow, sizeof(uint32_t)); // 4bytes
	//cout << &bytesToFollow << endl;
	memcpy(&fvtTemplate[0], &bytesToFollow, sizeof(uint32_t)); // 4bytes
	memcpy(&fvtTemplate[4], &bir.Header.HeaderVersion, 1); // 1 bytes
	memcpy(&fvtTemplate[5], &bir.Header.Type, 1); // 1 bytes
	memcpy(&fvtTemplate[6], &bir.Header.Format.FormatOwner, 2); // 2 bytes
	memcpy(&fvtTemplate[8], &bir.Header.Format.FormatType, 2); // 4 bytes
	memcpy(&fvtTemplate[10], &bir.Header.Quality, 1); // 1 bytes
	memcpy(&fvtTemplate[11], &bir.Header.Purpose, 1); // 1 bytes
	memcpy(&fvtTemplate[12], &bir.Header.FactorsMask, 4); // 4 bytes
	memcpy(&fvtTemplate[16], &bir.Header.ProductID.ProductOwner, 2); // 4 bytes
	memcpy(&fvtTemplate[18], &bir.Header.ProductID.ProductType, 2); // 4 bytes
	memcpy(&fvtTemplate[20], &bir.Header.CreationDTG.Date.Year, 2); // 7 bytes
	memcpy(&fvtTemplate[22], &bir.Header.CreationDTG.Date.Month, 1); // 7 bytes
	memcpy(&fvtTemplate[23], &bir.Header.CreationDTG.Date.Day, 1); // 7 bytes
	memcpy(&fvtTemplate[24], &bir.Header.CreationDTG.Time.Hour, 1); // 7 bytes
	memcpy(&fvtTemplate[25], &bir.Header.CreationDTG.Time.Minute, 1); // 7 bytes
	memcpy(&fvtTemplate[26], &bir.Header.CreationDTG.Time.Second, 1); // 7 bytes

	memcpy(&fvtTemplate[27], &header.Subtype, 1);
	// get last bytes individually.
	//28           24 (4+4+16)           memcpy the last 24 bytes of BIR header (from processedBIR.ExpirationDate)
	memcpy(&fvtTemplate[28], &bir.Header.ExpirationDate.Year, 2); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[30], &bir.Header.ExpirationDate.Month, 1); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[31], &bir.Header.ExpirationDate.Day, 1); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[32], &bir.Header.SBFormat.SecurityFormatOwner, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[34], &bir.Header.SBFormat.SecurityFormatType, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[36], &bir.Header.Index, 16); //from 36 onwards copy the data into ur template 
	memcpy(&fvtTemplate[52], &bir.BiometricData.Length, 4); // 4 bytes
	//56           bioLen   The encrypted biometric data from the BIR
	//memset(&fvtTemplate[56], 0, BIR.BiometricData.Length);
	memcpy(&fvtTemplate[56], bir.BiometricData.Data, bir.BiometricData.Length);// pointer data

	int secLen = (bir.SecurityBlock.Length > 0) ? bir.SecurityBlock.Length + 4 : 0;
	//if secLen is not zero
	if (secLen != 0)
	{
		numOfBytesToAdd = 4;

		//56+bioLen, 4      secLen (written as an integer)
		memcpy(&fvtTemplate[60 + bir.BiometricData.Length], &bir.SecurityBlock.Length, 4);
		numOfBytesToAdd = bir.SecurityBlock.Length;

		//…            secLen The security data from the BIR.
		memcpy(&fvtTemplate[60 + bir.BiometricData.Length + 4], bir.SecurityBlock.Data, bir.SecurityBlock.Length);
	}
	memcpy(&fvtTemplate[56 + bir.BiometricData.Length], &bir.SecurityBlock.Length, 4);

	int totalsize = 56 + bir.BiometricData.Length + 4 + secLen;
	cout << "Total size : " << totalsize << endl;

	std::string templateOutput((char*)fvtTemplate, totalsize);

	string base64myoutput = base64_encode(reinterpret_cast<const unsigned char*>(templateOutput.c_str()), templateOutput.length());
	utilities helper;
	unsigned char* base64newarray = helper.spc_base64_encode(fvtTemplate, 588, false);

	//fvtTemplate = NULL;
	delete[] fvtTemplate;
	//printf("Template generation successful");
	cout << "Template generation successful" << endl;
	cout << "####################" << endl;
	cout << templateOutput << endl;
	cout << "####################" << endl;
	cout << base64myoutput << endl;
	cout << "####################" << endl;


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

