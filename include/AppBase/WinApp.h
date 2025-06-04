
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		WinApp.h
//		Description:	Include necessary and headers and dependencies for basic Windows application
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//						<4> 2025.06.04:		Rename from stdafx.h
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _WINAPP_LIB_INCLUDED
	#define _WINAPP_LIB_INCLUDED
#endif


// Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN
#endif

// Include windows.h
#ifndef _WINDOWS_H_INCLUDED
	#define _WINDOWS_H_INCLUDED
	#include "windows.h"
	#include <winver.h>
#endif


// Include Power functions
#ifndef _POWER_FEATURE_INCLUDED
	#define _POWER_FEATURE_INCLUDED
	#include <powerbase.h>
	#include "PowrProf.h"
	#pragma comment (lib, "PowrProf.lib")
#endif


// Support register session state change notification
#ifndef _SUPPORT_SESSION_NOTIFY
	#define _SUPPORT_SESSION_NOTIFY
	#include "wtsapi32.h"
	#pragma comment (lib, "Wtsapi32.lib")
#endif


// Include MFC base libraries
#ifndef _MFC_BASE_LIB_INCLUDED
	#define _MFC_BASE_LIB_INCLUDED

	// some CString constructors will be explicit
	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

	// Turns off MFC's hiding of some common 
	// and often safely ignored warning messages
	#define _AFX_ALL_WARNINGS

	// MFC core and standard components
	#include <afxwin.h>

	// MFC extensions
	#include <afxext.h>

	// MFC Automation classes
	#include <afxdisp.h>

	// CDialogEx class
	#include "afxdialogex.h"
	#include "afxdtctl.h"
	#include "afxcmn.h"

	// MFC support for Internet Explorer 4 Common Controls
	#ifndef _AFX_NO_OLE_SUPPORT
		#include <afxdtctl.h>
	#endif

	// MFC support for Windows Common Controls
	#ifndef _AFX_NO_AFXCMN_SUPPORT
		#include <afxcmn.h>
	#endif

	// MFC support for ribbons and control bars
	#include <afxcontrolbars.h>

#endif


// Include advanced libraries
#ifndef _ADVANCED_LIB_INCLUDED
	#define _ADVANCED_LIB_INCLUDED
	#include "mmsystem.h"
	#include "uxtheme.h"

	#include <cfgmgr32.h>
	#pragma comment(lib, "Cfgmgr32.lib")

	#include <setupapi.h>
	#pragma comment(lib, "setupapi.lib")

	#include <comdef.h>
	#include <Wbemidl.h>
	#pragma comment(lib, "wbemuuid.lib")

	#include <dwmapi.h>
	#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
		#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
	#endif
#endif


// Support Visual Styles
#ifndef _SUPPORT_VISUAL_STYLES
	#define _SUPPORT_VISUAL_STYLES
	#include "commctrl.h"
	#pragma comment (lib, "Comctl32.lib")
	#ifdef _UNICODE
		#if defined _M_IX86
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#elif defined _M_X64
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#else
			#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
		#endif
	#endif
#endif
