
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AppBase.h
//		Description:	Base header file which includes all common and shared dependencies
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.06.04:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _APP_BASE_INCLUDED
#define _APP_BASE_INCLUDED
#endif


// Disable warnings
__pragma(warning(disable:4005))
__pragma(warning(disable:4244))
__pragma(warning(disable:4267))
__pragma(warning(disable:4805))
__pragma(warning(disable:4996))
__pragma(warning(disable:6031))
__pragma(warning(disable:6284))
__pragma(warning(disable:6386))
__pragma(warning(disable:6387))
__pragma(warning(disable:6388))
__pragma(warning(disable:26454))
__pragma(warning(disable:26495))
__pragma(warning(disable:28159))


// Include dependencies
#include "AppCommon.h"
#include "WinApp.h"


// Resource headers
#ifndef _RESOURCE_INCLUDED

	#include "Resource.h"
	#define _RESOURCE_INCLUDED

	#include "Resource2.h"
	#define _RESOURCE2_INCLUDED
#endif


// Re-check dependencies included
#ifndef _CPP_STANDARD_INCLUDED
	#error("(X) Compilation Error: C++ standard libraries are missing or not included!")
#endif
#ifndef _WINAPP_INCLUDED
	#error("(X) Compilation Error: Windows SDK libraries are missing or not included!")
#endif


// Include application base definitions
#include "AppMacros.h"
#include "AppConstants.h"


// Define global typenames for basic data types
using IntArray = typename std::vector<int>;
using UIntArray = typename std::vector<unsigned int>;
using LongArray = typename std::vector<long long>;
using ULongArray = typename std::vector<unsigned long long>;
using StringArray = typename std::vector<CString>;
