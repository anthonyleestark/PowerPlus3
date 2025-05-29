
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.cpp
//		Description:	Define global objects which will be used everywhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.01.27:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppCore\Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global variables which are used for application profile management
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------Launch-info data----------------------------------------------*/

// Application launch-time
SYSTEMTIME Global::g_stAppLaunchTime = {0};

// Application launch-time counter
UINT Global::g_uiAppLaunchTimeCounter = 0;

/*-----------------------------------------------------------------------------------------------------------*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global data, objects, variables and methods which are used during the application's runtime
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Application global/shared flag manager
FlagManager g_sharedFlagManager;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global variables which are used for debugging/testing
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------Debug/Test special variables---------------------------------------*/

// Trace error log file pointer
CFile* Global::g_pFileLogTraceError = NULL;

// Trace error log file pointer
CFile* Global::g_pFileLogTraceDebug = NULL;

// Debug info log file pointer
CFile* Global::g_pFileLogDebugInfo = NULL;

// Trace error log file exception pointer
CFileException* Global::g_pExcLogTraceError = NULL;

// Trace debug info log file exception pointer
CFileException* Global::g_pExcLogTraceDebug = NULL;

// Debug info output log file exception pointer
CFileException* Global::g_pExcLogDebugInfo = NULL;

/*-----------------------------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global variables which are used for other special purposes
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Background color
COLORREF Global::g_colorMsgBkgrd = DEFAULT_MSG_BKGRDCLR;

// Text color
COLORREF Global::g_colorMsgText = DEFAULT_MSG_TEXTCLR;

// Font name
CString	 Global::g_strMsgFontName = DEFAULT_MSG_FONTNAME;

// Font size
UINT	 Global::g_uiMsgFontSize = DEFAULT_MSG_FONTSIZE;

// Timeout (auto-close) interval
UINT	 Global::g_uiRmdMsgTimeout = DEFAULT_MSG_TIMEOUT;

// Message icon ID
UINT	 Global::g_uiRmdMsgIconID = DEFAULT_MSG_ICONID;

// Message icon size
INT		 Global::g_nRmdMsgIconSize = DEFAULT_MSG_ICONSIZE;

// Message icon placement
BYTE	 Global::g_byRmdMsgIconPlacement = DEFAULT_MSG_ICONPLACEMENT;

// Message display position
BYTE	 Global::g_byRmdMsgDispPos = DEFAULT_MSG_DISPLAYPOS;

// Display area horizontal margin
UINT	 Global::g_uiRmdMsgHMargin = DEFAULT_MSG_HMARGIN;

// Display area vertical margin
UINT	 Global::g_uiRmdMsgVMargin = DEFAULT_MSG_VMARGIN;

// Message snooze interval
UINT	 Global::g_uiRmdMsgSnoozeInterval = DEFAULT_PWRRMD_SNOOZETIME;

/*-----------------------------------------------------------------------------------------------------------*/


// END_OF_FILE
