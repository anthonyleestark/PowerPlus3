
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.cpp
//		Description:	Define global objects which will be used everywhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.01.27:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Global.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global variables which are used for debugging/testing
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------Debug/Test variables-------------------------------------------*/

// Dummy test mode
BOOL g_bDummyTest = DEFAULT_DUMMYTEST;

// Debug mode
BOOL g_bDebugMode = DEFAULT_DEBUGMODE;

// Debug log output target flag
int g_nDebugOutputTarget = DEFAULT_DEBUGOUTPUT;

/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------Debug/Test special variables---------------------------------------*/

// Trace error log file pointer
CFile* g_pFileLogTraceError = NULL;

// Trace error log file pointer
CFile* g_pFileLogTraceDebug = NULL;

// Debug info log file pointer
CFile* g_pFileLogDebugInfo = NULL;

/*-----------------------------------------------------------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Initialize global variables which are used for other special purposes
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------App flags-------------------------------------------------*/

// Power action trace flag
BYTE g_byPwrActionFlag = 0;

// System suspended trace flag
BYTE g_bySystemSuspendFlag = 0;

// Session ending trace flag
BYTE g_bySessionEndFlag = 0;

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Background color
COLORREF g_colorMsgBkgrd = DEFAULT_MSGBKGRDCLR;

// Text color
COLORREF g_colorMsgText = DEFAULT_MSGTEXTCLR;

// Font name
CString	 g_strMsgFontName = DEFAULT_MSGFONTNAME;

// Font size
UINT	 g_uiMsgFontSize = DEFAULT_MSGFONTSIZE;

// Timeout (auto-close) interval
UINT	 g_uiRmdMsgTimeout = DEFAULT_MSGTIMEOUT;

// Message icon ID
UINT	 g_uiRmdMsgIconID = DEFAULT_MSGICONID;

// Message icon size
int		 g_nRmdMsgIconSize = DEFAULT_MSGICONSIZE;

// Message icon position
BYTE	 g_byRmdMsgIconPos = DEFAULT_MSGICONPOS;

// Message display position
BYTE	 g_byRmdMsgDispPos = DEFAULT_MSGDISPPOS;

// Display area horizontal margin
UINT	 g_uiRmdMsgHMargin = DEFAULT_MSGHMARGIN;

// Display area vertical margin
UINT	 g_uiRmdMsgVMargin = DEFAULT_MSGVMARGIN;

// Message snooze interval
UINT	 g_uiRmdMsgSnoozeInterval = DEFAULT_SNOOZETIME;

/*-----------------------------------------------------------------------------------------------------------*/


// END_OF_FILE