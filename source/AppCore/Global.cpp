
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

#include "AppCore/Global.h"

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
DateTime Global::g_stAppLaunchTime;

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

// Global Power Reminder message style
RmdMsgStyleSet g_rmdMsgStyleSet;

/*-----------------------------------------------------------------------------------------------------------*/


// END_OF_FILE
