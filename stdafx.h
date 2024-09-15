
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		stdafx.h
//		Description:	Include file for standard system include files, 
//						or project specific include files that are used frequently, but are changed infrequently
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Disable warnings
#pragma once
__pragma(warning(disable:6387))
__pragma(warning(disable:6284))
__pragma(warning(disable:6031))
__pragma(warning(disable:4267))
__pragma(warning(disable:4244))
__pragma(warning(disable:26454))
__pragma(warning(disable:26495))
__pragma(warning(disable:4996))
__pragma(warning(disable:6386))
__pragma(warning(disable:4805))


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN							// Exclude rarely-used stuff from Windows headers
#endif

// Include windows.h
#ifndef WINDOWS_H_INCLUDED
#define WINDOWS_H_INCLUDED

#include "windows.h"
#include <powerbase.h>
#include "PowrProf.h"
#pragma comment (lib, "PowrProf.lib")

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// Turns off MFC's hiding of some common 
// and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>								// MFC core and standard components
#include <afxext.h>								// MFC extensions

#include <afxdisp.h>							// MFC Automation classes

#include "afxdialogex.h"						// CDialogEx class
#include "afxdtctl.h"
#include "afxcmn.h"

// Resource IDs
#ifndef _RESOURCE_LIB
#define _RESOURCE_LIB
#define _RESOURCE2_NOTINCLUDED_

#include "resource.h"							// Default resource header
#ifdef _RESOURCE2_NOTINCLUDED_
#include "resource2.h"							// Custom additional resource header
#undef _RESOURCE2_NOTINCLUDED_
#endif

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>							// MFC support for Internet Explorer 4 Common Controls
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>								// MFC support for Windows Common Controls
#endif

#include <afxcontrolbars.h>						// MFC support for ribbons and control bars

#ifndef _CPP_BASIC_LIB							// C++ basic libraries for input/output or etc.
#define _CPP_BASIC_LIB

// Include basic C++ standard libraries
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iterator>
#include <stdlib.h>
#include <errno.h>
#include <algorithm>

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _ADVANCED_LIB
#define _ADVANCED_LIB

// Include more advanced libraries
#include "mmsystem.h"
#include "uxtheme.h"

#include <dwmapi.h>
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif