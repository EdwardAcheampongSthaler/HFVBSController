#ifndef _DEFINE_H_INCLUDED
#define _DEFINE_H_INCLUDED

#include <vector>
#include "../Include/bioapi_type.h"

typedef std::vector<std::string> StrArray;

typedef struct bir_handle_info {
	BioAPI_BIR_HANDLE Handle;
	BioAPI_BIR_PURPOSE Purpose;
	BioAPI_BIR_DATA_TYPE Type;
} BIR_HANDLE_INFO;

typedef std::vector<BIR_HANDLE_INFO> BIRHandleArray;

typedef std::vector<BioAPI_BIR> BIRArray;

typedef struct data_info {
	std::string GName;
	std::string DName;
} DATA_INFO;

typedef std::vector<DATA_INFO> Data_InfoArray;

// Title
#define TITLE "Controller Services"	

#endif // _DEFINE_H_INCLUDED


