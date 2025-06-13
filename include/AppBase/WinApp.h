/**
 * @file		WinApp.h
 * @brief		Include necessary and headers and dependencies for basic Windows application
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#ifndef _WINAPP_INCLUDED
	#define _WINAPP_INCLUDED
#endif


// Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
	#define VC_EXTRALEAN
#endif


// Include Windows APIs
#ifndef _WINDOWS_
	#ifndef _WINSOCK2API_
		#include <winsock2.h>
		#define _WINSOCK2API_INCLUDED
	#endif
	#include <windows.h>
	#define _WINDOWSAPI_INCLUDED
#endif


// Include Power functions
#ifndef _POWRPROF_H_
	#include "powrprof.h"
	#pragma comment (lib, "powrprof.lib")
	#define _POWERBASEAPI_INCLUDED
#endif


// Support register session state change notification
#ifndef _INC_WTSAPI
	#include "wtsapi32.h"
	#pragma comment (lib, "wtsapi32.lib")
	#define _WTS_API_INCLUDED
#endif


// Include MFC base libraries
#ifndef _MFC_BASE_INCLUDED

	// some CString constructors will be explicit
	#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

	// Turns off MFC's hiding of some common 
	// and often safely ignored warning messages
	#define _AFX_ALL_WARNINGS

	// Warning: MFC requires Winsock2 included
	#if !defined _WINSOCK2API_ || !defined _WINSOCK2API_INCLUDED
		#error "Include <winsock2.h> before <windows.h>"
	#endif

	// MFC core and standard components
	#include <afxwin.h>

	// MFC extensions
	#include <afxext.h>

	// MFC Automation classes
	#include <afxdisp.h>

	// CDialogEx class
	#include <afxdialogex.h>
	#include <afxdtctl.h>
	#include <afxcmn.h>

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

	#define _MFC_BASE_INCLUDED
#endif


// Include addtional dependencies
#ifndef _ADDITIONAL_DEPENDENCIES_INCLUDED
	#define _ADDITIONAL_DEPENDENCIES_INCLUDED

	#include "mmsystem.h"
	#include "uxtheme.h"

	#include <cfgmgr32.h>
	#pragma comment(lib, "cfgmgr32.lib")

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
	#pragma comment (lib, "comctl32.lib")
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
