
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
		cout << "BioAPI_GetBIRFromHandle encountered an error" << endl;
		utilities::printfmsg("BioAPI_GetBIRFromHandle encountered an error. (0x%08x)\r\n");
		return "BIO API";
	}
	cout << "Obtained BIR From Handle: memory lock: " << &BIR << endl;

	// create template
	int offset = 0;
	int numOfBytesToAdd = 0;
	//Index			Offset		Description
	//0				4			total number of bytes to follow (56 + bioLen + secLen + 4 if secLen is not zero) written as a 4-byte integer (iLen).

	uint32_t bytesToFollow = (56 + BIR.BiometricData.Length + (BIR.SecurityBlock.Length != 0 ? 4 + BIR.SecurityBlock.Length : 0));
	BYTE *fvtTemplate = new BYTE[bytesToFollow+4];

	memcpy(&fvtTemplate[0], &bytesToFollow, numOfBytesToAdd = sizeof(uint32_t)); // 4bytes
	cout << &bytesToFollow << endl;

	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << fvtTemplate[i] << endl;

	}
	offset += numOfBytesToAdd;

	//4            23           memcpy the first 23 bytes from processedBIR.Header
	//memcpy(&fvtTemplate[4], &BIR.Header, 23); // 23 bytes

////	memcpy(&fvtTemplate[4], &BIR.Header, 12); // 23 bytes
////	numOfBytesToAdd = 12;
////	for (int i = offset; i < offset + numOfBytesToAdd; i++)
////	{
////		cout << i << ": " << fvtTemplate[i] << endl;
////
////	}
////	offset += numOfBytesToAdd;
//////	
////	numOfBytesToAdd = 4;
////	memcpy(&fvtTemplate[16], &BIR.Header.ProductID, 4); 
////	for (int i = offset; i < offset + numOfBytesToAdd; i++)
////	{
////		cout << i << ": " << fvtTemplate[i] << endl;
////
////	}
////	offset += numOfBytesToAdd;
////
////	memcpy(&fvtTemplate[20], &BIR.Header.CreationDTG.Date.Year, sizeof(BIR.Header.CreationDTG.Date.Year));
////	memcpy(&fvtTemplate[22], &BIR.Header.CreationDTG.Date.Month, 1);// 23 bytes
////	memcpy(&fvtTemplate[23], &BIR.Header.CreationDTG.Date.Day, 1);// 23 bytes
////	numOfBytesToAdd = 4;
////	for (int i = offset; i < offset + numOfBytesToAdd; i++)
////	{
////		cout << i << ": " << fvtTemplate[i] << endl;
////
////	}
////	offset += numOfBytesToAdd;
////
////	memcpy(&fvtTemplate[24], &BIR.Header.CreationDTG.Time, 3); // 23 bytes
////	numOfBytesToAdd = 3;
////	for (int i = offset; i < offset + numOfBytesToAdd; i++)
////	{
////		cout << i << ": " << fvtTemplate[i] << endl;
////
////	}
////	offset += numOfBytesToAdd;
////	numOfBytesToAdd = 1;

//TETST
	//memcpy(&fvtTemplate[4], &(BIR.Header), 52); // 48 bytes
	//cout << "Size:" << sizeof(Header) << " vs " << sizeof(BIR.Header) << endl;
	//numOfBytesToAdd = 48;

	//for (int i = offset; i < offset + numOfBytesToAdd; i++)
	//{
	//	cout << i << ": " << &fvtTemplate[i] << endl;

	//}
	//offset += numOfBytesToAdd;
//TESET
	//	// START PAST
	/////GOOOD
//	memcpy(&fvtTemplate[4], &BIR.Header, 23); // 23 bytes


	memcpy(&fvtTemplate[4], &BIR.Header.HeaderVersion, 1); // 1 bytes
	memcpy(&fvtTemplate[5], &BIR.Header.Type, 1); // 1 bytes
	//memcpy(&fvtTemplate[6], &BIR.Header.Format, 4); // 4 bytes
	memcpy(&fvtTemplate[6], &BIR.Header.Format.FormatOwner, 2); // 2 bytes
	memcpy(&fvtTemplate[8], &BIR.Header.Format.FormatType, 2); // 2 bytes

	memcpy(&fvtTemplate[10], &BIR.Header.Quality, 1); // 1 bytes
	memcpy(&fvtTemplate[11], &BIR.Header.Purpose, 1); // 1 bytes
	memcpy(&fvtTemplate[12], &BIR.Header.FactorsMask, 4); // 4 bytes
	//memcpy(&fvtTemplate[16], &BIR.Header.ProductID, 4); // 4 bytes
	memcpy(&fvtTemplate[16], &BIR.Header.ProductID.ProductOwner, 2); // 4 bytes
	memcpy(&fvtTemplate[18], &BIR.Header.ProductID.ProductType, 2); // 4 bytes
	//memcpy(&fvtTemplate[20], &BIR.Header.CreationDTG, 7); // 7 bytes
	memcpy(&fvtTemplate[20], &BIR.Header.CreationDTG.Date.Year, 2); // 7 bytes
	memcpy(&fvtTemplate[22], &BIR.Header.CreationDTG.Date.Month, 1); // 7 bytes
	memcpy(&fvtTemplate[23], &BIR.Header.CreationDTG.Date.Day, 1); // 7 bytes
	
	memcpy(&fvtTemplate[24], &BIR.Header.CreationDTG.Time.Hour, 1); // 7 bytes
	memcpy(&fvtTemplate[25], &BIR.Header.CreationDTG.Time.Minute, 1); // 7 bytes
	memcpy(&fvtTemplate[26], &BIR.Header.CreationDTG.Time.Second, 1); // 7 bytes
	


	cout << "Size:" << sizeof(Header) << " vs " << sizeof(BIR.Header) << endl;
	cout << "Size:" << sizeof(Header.HeaderVersion) << " vs " << sizeof(BIR.Header.HeaderVersion) << endl;
	cout << "Size:" << sizeof(Header.Type) << " vs " << sizeof(BIR.Header.Type) << endl;
	cout << "Size:" << sizeof(Header.Format) << " vs " << sizeof(BIR.Header.Format) << endl;
	cout << "Size:" << sizeof(Header.Quality) << " vs " << sizeof(BIR.Header.Quality) << endl;
	cout << "Size:" << sizeof(Header.Purpose) << " vs " << sizeof(BIR.Header.Purpose) << endl;
	cout << "Size:" << sizeof(Header.FactorsMask) << " vs " << sizeof(BIR.Header.FactorsMask) << endl;
	cout << "Size:" << sizeof(Header.ProductID) << " vs " << sizeof(BIR.Header.ProductID) << endl;
	cout << "Size:" << sizeof(Header.CreationDTG) << " vs " << sizeof(BIR.Header.CreationDTG) << endl;
	cout << "Size:" << sizeof(Header.Subtype) << " vs " << sizeof(BIR.Header.Subtype) << endl;
	cout << "Size:" << sizeof(Header.ExpirationDate) << " vs " << sizeof(BIR.Header.ExpirationDate) << endl;
	cout << "Size:" << sizeof(Header.SBFormat) << " vs " << sizeof(BIR.Header.SBFormat) << endl;
	cout << "Size:" << sizeof(Header.Index) << " vs " << sizeof(BIR.Header.Index) << endl;
	numOfBytesToAdd = 23;

	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << fvtTemplate[i] << endl;

	}
	offset += numOfBytesToAdd;
	//
	numOfBytesToAdd = 1;
	////GOOD
		//27           1             copy 1 byte from processedBIR.Subtype
	memcpy(&fvtTemplate[27], &BIR.Header.Subtype, numOfBytesToAdd);

	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << &fvtTemplate[i] << endl;
	}
	offset += numOfBytesToAdd;

	// get last bytes individually.
	//28           24 (4+4+16)           memcpy the last 24 bytes of BIR header (from processedBIR.ExpirationDate)

	numOfBytesToAdd = 24;
	//memcpy(&fvtTemplate[28], &BIR.Header.ExpirationDate, 4); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[28], &BIR.Header.ExpirationDate.Year, 2); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[30], &BIR.Header.ExpirationDate.Month, 1); //from 28 onwards copy the data into ur template 
	memcpy(&fvtTemplate[31], &BIR.Header.ExpirationDate.Day, 1); //from 28 onwards copy the data into ur template 
	//memcpy(&fvtTemplate[32], &BIR.Header.SBFormat, 4); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[32], &BIR.Header.SBFormat.SecurityFormatOwner, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[34], &BIR.Header.SBFormat.SecurityFormatType, 2); //from 32 onwards copy the data into ur template 
	memcpy(&fvtTemplate[36], &BIR.Header.Index, 16); //from 36 onwards copy the data into ur template 


	// GOOD
	//numOfBytesToAdd = 24;
	////28           24           memcpy the last 24 bytes of BIR header (from processedBIR.ExpirationDate)
	//int sizeofHeader = sizeof(BIR.Header); // should be 52
	//BYTE *temp = new BYTE[sizeofHeader];
	//memcpy(&temp[0], &BIR.Header, sizeofHeader); // copy the entire header into a temp array
	//memcpy(&fvtTemplate[28], &temp[25], numOfBytesToAdd); //from 25 onwards copy the data into ur template 
	// // delete temp
	//delete[] temp;
// GOOD
	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << &fvtTemplate[i] << endl;
	}
	offset += numOfBytesToAdd;
	//
	numOfBytesToAdd = 4;
	// END PAST
		//52           4             bioLen (written as an integer)

	memcpy(&fvtTemplate[52], &BIR.BiometricData.Length, 4); // 4 bytes

	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << &fvtTemplate[i] << endl;
	}
	offset += numOfBytesToAdd;

	numOfBytesToAdd = BIR.BiometricData.Length;
	//56           bioLen   The encrypted biometric data from the BIR
	memcpy(&fvtTemplate[56], BIR.BiometricData.Data, BIR.BiometricData.Length);

	for (int i = offset; i < offset + numOfBytesToAdd; i++)
	{
		cout << i << ": " << &fvtTemplate[i] << endl;
	}
	offset += numOfBytesToAdd;

	//
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
	numOfBytesToAdd = BIR.SecurityBlock.Length;

	int totalsize = 56 + BIR.BiometricData.Length + 4 + secLen;

	cout << "Total size : " << totalsize << endl;

	std::string myoutput((char*)fvtTemplate, totalsize);
	//cout << myoutput << endl;
	std::string newString;
	newString.resize(totalsize);
	memcpy((char*)newString.data(), &fvtTemplate[0], totalsize);
	//cout << newString << endl;

	string base64myoutput = base64_encode(reinterpret_cast<const unsigned char*>(myoutput.c_str()), myoutput.length());
	string base64newString = base64_encode(reinterpret_cast<const unsigned char*>(newString.c_str()), newString.length());

	delete[] fvtTemplate;
	//printf("Template generation successful");
	cout << "Template generation successful" << endl;
	cout << "####################" << endl;
	cout << myoutput << endl;
	cout << "####################" << endl;
	cout << base64myoutput << endl;
	cout << "####################" << endl;
	cout << newString << endl;
	cout << "####################" << endl;
	cout << base64newString << endl;
	cout << "####################" << endl;

	// deconstruct
	string mybstemplate = "SAIAACAUNwAEAWQDAAAAQDcABAHiBwwHDDkuAAAAAAA3AAQBAAAAAAAAAAAAAAAAAAAAABACAACoWvrsaweLrFNkBCx8bVYqDqLLt8ESyB1PxD0YxZYaXeH2l1nMW1qnoRaJxTMBpNpRcZz+gZuWH8Ay+9TN240ZrWZvXoGIDRJdFXQbt2xeAl+YZ1fpOe/EM1bC1gc5GLLZ0cSiiPH7nc+KPOFXBGAGZypEVsJB7kFPv2e+eshe+1M8H4EwaQAQKzGM1ep0AxGcD4ceqtgfc18n3HZrHnyrdExZdxFyA9LuzB8LdMEAJqV7Tq2Yzn8ppJP8a8bF+P3OVxkbXvel9cIWa+Vh+xUFFSxoOI/v4HwG/dQq5GwCdt1bVrDB3dwOuqfT8j1hC/Cpacfndlo/s1ZUwj0ObY0U3Bzhmg97XFaAnWRZRPezqiei4PKsRBy0un8m8naS2C2CNJ5JZrYi7mVhPUJAcHiJAUV1/7fH1Fc1OzaFppn91SRqvxfW5jAQAqP14F99bm40g9QsIZTXrZDlhhFhFMqzGB4QJ3UTcMYl5iI6srqjR1rS8WheALm4JTj4ZSDPfGibCvlHb0JzjcuiQdGqd1mOYtaWsGG4y9mkE3tCxY7r+TpQRUUgdV61AQcFvsU5KxnFTds+Owoo0K7QR9SALpYogKQIdY70k2daFDO11E9QxR+GXnfjvzpb+Ob4wIKp0d51ipq9DGnBHGPSWcfJ8O5nm7DZ0OY258LakUTsPLyr8XmBweLeRRlAsbrU3Cs8SgQAAAAA";
	string goodtemplate = "SAIAACAUNwAEAWQDAAAAQDcABAHiBwwFCTgOAAAAAAA3AAQBAAAAAAAAAAAAAAAAAAAAABACAADYHHsU0LNlIjxD9RX9+V5m5W6w9zEx6rbN9FlDCfPmHlY6fgxKeWN8y4ahDVphtkhPaf2nVMbUGwDsA6sn/BAULwTwbg9+4d0OGRXSknSA+wLi4kfkPjChEOCqBHQMYcxU84bts1iSII0A8o6YVleC0HB13gNuJSJ3sMEjn7jn0GFuFMxlfxUlZ4El7nGbxZ/3hRcNDLe/OGW1s6KJKcq+jw2CiHT7imdZaCcYxeC0eGqzwfIol1rZewD0sTGUjS6/UEgcF8cBcSAH1Bw0nchIssUvp52mHKS1eeuCller4CsyexDH6dXJZtu7JSZSmMWm3q81nnKQ1E1/vE3AbW0ON97Am+6g0hZe9lIsE1Q39OyLYJhZYBAHNAxDxNy/ROQwhDt/m026QjcUT00difPp/4y0W8NJ4+3013FQKbym2bwu8TQL1oErTMzb4CvCKAkGH3JXeuEem86gEVhQQ//xlryBwcILZeUeeXEy4EU9+AvbIhpsN5tKEyZnIhguSIkVg7sigxdZay7YauKjMKcAKRbp5996n7/pXswm/J/j+BUcO7662NqiIxJN+boQkWWxBg1JkSXrOlv4srAZziNvLV8ftSAcfiNBJH0G6F05oACagVfrDU8IsLbmK+c4SqNUwI+LpHIC2E3xgVkG7l4SmiscQkM4y9gVUmp6GwSY3VZLM8nXVV7R3PZqysBfdC0AAAAA"; 
		//"SAIAACAUNwAEAWQDAAAAQDcABAHiBwwFCTgOAAAAAAA3AAQBAAAAAAAAAAAAAAAAAAAAABACAADYHHsU0LNlIjxD9RX9+V5m5W6w9zEx6rbN9FlDCfPmHlY6fgxKeWN8y4ahDVphtkhPaf2nVMbUGwDsA6sn / BAULwTwbg9 + 4d0OGRXSknSA + wLi4kfkPjChEOCqBHQMYcxU84bts1iSII0A8o6YVleC0HB13gNuJSJ3sMEjn7jn0GFuFMxlfxUlZ4El7nGbxZ / 3hRcNDLe / OGW1s6KJKcq + jw2CiHT7imdZaCcYxeC0eGqzwfIol1rZewD0sTGUjS6 / UEgcF8cBcSAH1Bw0nchIssUvp52mHKS1eeuCller4CsyexDH6dXJZtu7JSZSmMWm3q81nnKQ1E1 / vE3AbW0ON97Am + 6g0hZe9lIsE1Q39OyLYJhZYBAHNAxDxNy / ROQwhDt / m026QjcUT00difPp / 4y0W8NJ4 + 3013FQKbym2bwu8TQL1oErTMzb4CvCKAkGH3JXeuEem86gEVhQQ//xlryBwcILZeUeeXEy4EU9+AvbIhpsN5tKEyZnIhguSIkVg7sigxdZay7YauKjMKcAKRbp5996n7/pXswm/J/j+BUcO7662NqiIxJN+boQkWWxBg1JkSXrOlv4srAZziNvLV8ftSAcfiNBJH0G6F05oACagVfrDU8IsLbmK+c4SqNUwI+LpHIC2E3xgVkG7l4SmiscQkM4y9gVUmp6GwSY3VZLM8nXVV7R3PZqysBfdC0AAAAA";
	string newone =		  "SAIAACAUNwAEAWQDAAAAQDcABAHiBwwHDAYFAAAAAAA3AAQBAAAAAAAAAAAAAAAAAAAAABACAAAuAvwIYuRcXw3qhY9EzbKczjsvQmQn6tRiJ0cRqABz0hAYxQZmQoHywubZj/PYF2jWAdLoDXPBuIhi37mec9WuZ0AIn4Xh7QePcGL97P5UDE7UHoRwLcmrsPzZR6ah1RM/Mxah9VHIQ3EvM2PTTvLEhQq5UBFjCff6bkHNJiSEcW8BwFYXBonALgMNqCgR+npNvpbEmznXbyFcsWvaqU2PFpGaLRwZuJKWxPMzgMhrVWrn71NcO139qUm+06E1vd2yvxeqLjHpos1oA57+QdSZPqkw3U4IiUg/jvYpKutmXZpwgWQU7ekx4Cf7vPQxNlic3g3F5PinIa4rtpiIcD/OA3Ok5crdCiVuZWenvIcDHLH6pgmHnBaqBa4wn6unSLP34th9VM6Q21tDkRHnig2DW5XUpJS2NLbF6tKM+f5zSszDtzXPBdURB+8A1d1MLMFXUxuRrI1xUbgGRPhAgvXHFqRBIMeBK9dOq/CXQZ+dxQOA3vHPlD++vkWqzO9cvLtMqtqYdHGLE+EyRDM/qbCo5t03n+bssdTwa9EPEO++j9LKraV8hAvzCr45rfD8v7GU/oB+OjYc0jLu4vwWY7XLhnbOfr4TrPdYD+Nvi5+XSzi9czx7n1Uu0OulNxFTGcIcTQhJFsmoHxa84zE3xfVnndM6DKst2Z8Zk9xNt4AehmgTA/LizFDOSNzpLWrPnsoAAAAA";
	string decoded = base64_decode(goodtemplate);
	INT64 sizeOfDecoded = decoded.length();
	const char *deconstructed = decoded.c_str();
	BYTE *reconstitutedTemplate = new BYTE[sizeOfDecoded];

	memcpy(&reconstitutedTemplate[0], &deconstructed[0], 4); // 4bytes
	memcpy(&reconstitutedTemplate[4], &deconstructed[4], 1); // 1 bytes
	memcpy(&reconstitutedTemplate[5], &deconstructed[5], 1); // 1 bytes
	memcpy(&reconstitutedTemplate[6], &deconstructed[6], 2); // 4 bytes
	memcpy(&reconstitutedTemplate[8], &deconstructed[8], 2); // 4 bytes
	memcpy(&reconstitutedTemplate[10], &deconstructed[10], 1); // 1 bytes
	memcpy(&reconstitutedTemplate[11], &deconstructed[11], 1); // 1 bytes
	memcpy(&reconstitutedTemplate[12], &deconstructed[12], 4); // 4 bytes
	memcpy(&reconstitutedTemplate[16], &deconstructed[16], 2); // 4 bytes
	memcpy(&reconstitutedTemplate[18], &deconstructed[18], 2); // 4 bytes
	memcpy(&reconstitutedTemplate[20], &deconstructed[20], 2); // 7 bytes
	memcpy(&reconstitutedTemplate[22], &deconstructed[22], 1); // 7 bytes
	memcpy(&reconstitutedTemplate[23], &deconstructed[23], 1); // 7 bytes
	memcpy(&reconstitutedTemplate[24], &deconstructed[24], 1); // 7 bytes
	memcpy(&reconstitutedTemplate[25], &deconstructed[25], 1); // 7 bytes
	memcpy(&reconstitutedTemplate[26], &deconstructed[26], 1); // 7 bytes
	memcpy(&reconstitutedTemplate[27], &deconstructed[27], 1);
	memcpy(&reconstitutedTemplate[28], &deconstructed[28], 2); //from 28 onwards copy the data into ur template 
	memcpy(&reconstitutedTemplate[30], &deconstructed[30], 1); //from 28 onwards copy the data into ur template 
	memcpy(&reconstitutedTemplate[31], &deconstructed[31], 1); //from 28 onwards copy the data into ur template 
	memcpy(&reconstitutedTemplate[32], &deconstructed[32], 2); //from 32 onwards copy the data into ur template 
	memcpy(&reconstitutedTemplate[34], &deconstructed[34], 2); //from 32 onwards copy the data into ur template 
	memcpy(&reconstitutedTemplate[36], &deconstructed[36], 16); //from 36 onwards copy the data into ur template 

	for (int i = 0; i < 4 ; i++)
	{
		cout << i << ": " << 
			uint32_t((unsigned char)(deconstructed[0]) << 24 |
			(unsigned char)(deconstructed[1]) << 16 |
			(unsigned char)(deconstructed[2]) << 8 |
			(unsigned char)(deconstructed[3]))
		<< endl;
	}
	cout << "1byte: " << int((unsigned char)(deconstructed[4])) << endl ;
	cout << "1byte: " << int((unsigned char)(deconstructed[5])) << endl;

	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[6]) << 8 |
				(unsigned char)(deconstructed[7]))
			<< endl;
	}
	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[8]) << 8 |
			(unsigned char)(deconstructed[9]))
			<< endl;
	}
	cout << "1byte: " << uint8_t((unsigned char)(deconstructed[10])) << endl;
	cout << "1byte: " << uint8_t((unsigned char)(deconstructed[11])) << endl;


	for (int i = 0; i < 4; i++)
	{
		cout << i << ": " <<
			uint32_t((unsigned char)(deconstructed[12]) << 24 |
			(unsigned char)(deconstructed[13]) << 16 |
				(unsigned char)(deconstructed[14]) << 8 |
				(unsigned char)(deconstructed[15]))
			<< endl;
	}


	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[16]) << 8 |
			(unsigned char)(deconstructed[17]))
			<< endl;
	}
	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[18]) << 8 |
			(unsigned char)(deconstructed[19]))
			<< endl;
	}
	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			int((unsigned char)(deconstructed[20]) << 8 |
			(unsigned char)(deconstructed[21]))
			<< endl;
	}
	cout << "1byte: " << uint8_t((deconstructed[22])) << endl;
	cout << "1byte: " << uint8_t((deconstructed[23])) << endl;
	cout << "1byte: " << uint8_t((deconstructed[24])) << endl;
	cout << "1byte: " << uint8_t((deconstructed[25])) << endl;
	cout << "1byte: " << uint8_t((deconstructed[26])) << endl;
	cout << "1byte: " << uint8_t((deconstructed[27])) << endl;

	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[28]) << 8 |
			(unsigned char)(deconstructed[29]))
			<< endl;
	}
	cout << "1byte: " << int((unsigned char)(deconstructed[30])) << endl;
	cout << "1byte: " << int((unsigned char)(deconstructed[31])) << endl;

	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[32]) << 8 |
			(unsigned char)(deconstructed[33]))
			<< endl;
	}

	for (int i = 0; i < 2; i++)
	{
		cout << i << "2byte: " <<
			uint16_t((unsigned char)(deconstructed[34]) << 8 |
			(unsigned char)(deconstructed[35]))
			<< endl;
	}


	for (int i = 36; i < 52; i++)
	{
		cout << "index byte: " << int((unsigned char)(deconstructed[i])) << endl;
	}

	deconstructed = NULL;
	delete[] reconstitutedTemplate;
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

