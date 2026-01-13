/**
 * @file		AppBase.h
 * @brief		Base header file which includes all common and shared dependencies
 * @author		AnthonyLeeStark
 * @date		2025.06.04
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

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


// Check for C++ version
#ifdef __cplusplus
	#if defined(_MSVC_LANG) && _MSVC_LANG > __cplusplus
		#define _STL_LANG _MSVC_LANG
	#else
		#define _STL_LANG __cplusplus
	#endif
#else
	#define _STL_LANG 0L
#endif


// Check if it is C++ 17
#ifndef _HAS_CXX17
	#if _STL_LANG > 201402L
		#define _HAS_CXX17 1
	#else
		#define _HAS_CXX17 0
	#endif
#endif


// Check if it is C++ 20
#ifndef _HAS_CXX20
	#if _HAS_CXX17 && _STL_LANG > 201703L
		#define _HAS_CXX20 1
	#else
		#define _HAS_CXX20 0
	#endif
#endif


// Check if it is C++ 23
#ifndef _HAS_CXX23
	#if _HAS_CXX20 && _STL_LANG > 202002L
		#define _HAS_CXX23 1
	#else
		#define _HAS_CXX23 0
	#endif
#endif


// Check if it is C++ 26
#ifndef _HAS_CXX26
	#if _HAS_CXX23 && _STL_LANG > 202302L
		#define _HAS_CXX26 1
	#else
		#define _HAS_CXX26 0
	#endif
#endif

#undef _STL_LANG


// C++ 20 must imply C++ 17
#if _HAS_CXX20 && !_HAS_CXX17
	#error _HAS_CXX20 must imply _HAS_CXX17.
#endif

// C++ 23 must imply C++ 20
#if _HAS_CXX23 && !_HAS_CXX20
	#error _HAS_CXX23 must imply _HAS_CXX20.
#endif


// C++ 26 must imply C++ 23
#if _HAS_CXX26 && !_HAS_CXX23
	#error _HAS_CXX26 must imply _HAS_CXX23.
#endif


// Include application base definitions
#include "AppMacros.h"
#include "AppConstants.h"
#include "BaseTypes.h"


// Define global typenames for basic data types
using int8			= char;
using uint8			= unsigned char;
using int16			= short;
using uint16		= unsigned short;
using int32			= int;
using uint32		= unsigned int;
using int64			= long long;
using uint64		= unsigned long long;

using int8_array	= typename std::vector<int8>;
using uint8_array	= typename std::vector<uint8>;
using int16_array	= typename std::vector<int16>;
using uint16_array	= typename std::vector<uint16>;
using int32_array	= typename std::vector<int32>;
using uint32_array	= typename std::vector<uint32>;
using int64_array	= typename std::vector<int64>;
using uint64_array	= typename std::vector<uint64>;

using StringArray	= typename std::vector<String>;
