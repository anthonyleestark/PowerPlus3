
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.h
//		Description:	Define global data and objects which will be used everywhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.01.27:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define categories of global data variables
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEF_GLBDATA_CATE_NONE			0x00			// No category (main profile section)
#define DEF_GLBDATA_CATE_DEBUGTEST		0x01			// Debugging/Testing variables
#define DEF_GLBDATA_CATE_APPFLAGS		0x02			// App special flags
#define DEF_GLBDATA_CATE_FEATURES		0x03			// Special feature variables
#define DEF_GLBDATA_CATE_TRACKING		0x04			// Tracking data variables
#define DEF_GLBDATA_CATE_OTHERS			0x05			// Other special variables


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for application profile management
// These variables are not being grouped in any category and are stored in the main profile section
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------Launch-info data----------------------------------------------*/

namespace Global {
	extern DateTime g_stAppLaunchTime;
	extern UINT	g_uiAppLaunchTimeCounter;
};

// Access application launch-time data
static inline const DateTime& GetAppLaunchTime(void);
static inline void SetAppLaunchTime(const DateTime& stTime);

// Access application launch-time counter data
static inline const UINT GetAppLaunchTimeCounter(void);
static inline const BOOL IsAppFirstLaunch(void);
static inline void SetAppLaunchTimeCounter(UINT uiValue);
static inline void UpdateAppLaunchTimeCounter(void);

/*-----------------------------------------------------------------------------------------------------------*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global data, objects, variables and methods which are used during the application's runtime.
// These variables are not grouped into any specific category and are serialized independently.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Application global/shared flag manager
extern FlagManager g_sharedFlagManager;
extern inline FlagManager& GetGlobalFlagManager(void);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for debugging/testing
// These special variables can only be changed by using debug commands
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------Debug/Test flags---------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST			FALSE
#define DEFAULT_DEBUGMODE			FALSE
#define DEFAULT_DEBUGOUTPUT			DefaultOutput
#define DEFAULT_TESTFEATURE			FALSE

// Dummy test mode flag
static inline const BOOL GetDummyTestMode(void);
static inline void SetDummyTestMode(BOOL bValue);

// Debug mode flag
static inline const BOOL GetDebugMode(void);
static inline void SetDebugMode(BOOL bValue);

// Debug log output target flag
enum DebugOutput {
	DefaultOutput = 0,
	DebugInfoFile,
	DebugTestTool,
};
static inline const INT GetDebugOutputTarget(void);
static inline void SetDebugOutputTarget(int nValue);

// Test feature enable flag
static inline const BOOL GetTestFeatureEnable(void);
static inline void SetTestFeatureEnable(BOOL bValue);

/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------Debug/Test special variables---------------------------------------*/

namespace Global {
	extern CFile* g_pFileLogTraceError;
	extern CFile* g_pFileLogTraceDebug;
	extern CFile* g_pFileLogDebugInfo;
	extern CFileException* g_pExcLogTraceError;
	extern CFileException* g_pExcLogTraceDebug;
	extern CFileException* g_pExcLogDebugInfo;
};

// Trace error log file pointer
static inline CFile* GetTraceErrorLogFile(void);
static BOOL InitTraceErrorLogFile(void);
inline void ReleaseTraceErrorLogFile(void);

//Trace debug info log file pointer
static inline CFile* GetTraceDebugLogFile(void);
static BOOL InitTraceDebugLogFile(void);
inline void ReleaseTraceDebugLogFile(void);

// Debug info output log file pointer
static inline CFile* GetDebugInfoLogFile(void);
static BOOL InitDebugInfoLogFile(void);
inline void ReleaseDebugInfoLogFile(void);

// Trace error log file exception pointer
static inline CFileException* GetTraceErrorException(void);

// Trace debug info log file exception pointer
static inline CFileException* GetTraceDebugException(void);

// Debug info output log file exception pointer
static inline CFileException* GetDebugInfoException(void);

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for other special purposes,
// These special variables can only be changed by using debug commands
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*---------------------------------------------App tracing flags---------------------------------------------*/

// Power action trace flag
static inline const BYTE GetPwrActionFlag(void);
static inline void SetPwrActionFlag(BYTE byValue);

// System suspended trace flag
static inline const BYTE GetSystemSuspendFlag(void);
static inline void SetSystemSuspendFlag(BYTE byValue);

// Session ended trace flag
static inline const BYTE GetSessionEndFlag(void);
static inline void SetSessionEndFlag(BYTE byValue);

// Previously safe termination trace flag
static inline const BYTE GetSafeTerminationFlag(void);
static inline void SetSafeTerminationFlag(BYTE byValue);

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Memory-only flags---------------------------------------------*/

// Session lock trace flag
static inline const BYTE GetSessionLockFlag(void);
static inline void SetSessionLockFlag(BYTE byValue);

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Global Power Reminder message style
extern RmdMsgStyleSet g_rmdMsgStyleSet;

// Message style
static inline const RmdMsgStyleSet& GetReminderMessageStyle(void);
static inline void SetReminderMessageStyle(const RmdMsgStyleSet& rmdMsgStyle);

// Background color
static inline const COLORREF GetReminderMsgBkgrdColor(void);
static inline void SetReminderMsgBkgrdColor(COLORREF colorValue);

// Text color
static inline const COLORREF GetReminderMsgTextColor(void);
static inline void SetReminderMsgTextColor(COLORREF colorValue);

// Font name
static inline BOOL GetReminderMsgFontName(String& strFontName);
static inline void SetReminderMsgFontName(const wchar_t* lpszValue);

// Font size
static inline const UINT GetReminderMsgFontSize(void);
static inline void SetReminderMsgFontSize(UINT uiValue);

// Timeout (auto-close) interval
static inline const UINT GetReminderMsgTimeout(void);
static inline void SetReminderMsgTimeout(UINT uiValue);

// Message icon ID
static inline const UINT GetReminderMsgIconID(void);
static inline void SetReminderMsgIconID(UINT uiValue);

// Message icon size
static inline const INT GetReminderMsgIconSize(void);
static inline void SetReminderMsgIconSize(int nValue);

// Message icon position
static inline const BYTE GetReminderMsgIconPosition(void);
static inline void SetReminderMsgIconPosition(BYTE byValue);

// Message display position
static inline const BYTE GetReminderMsgDispPosition(void);
static inline void SetReminderMsgDispPosition(BYTE byValue);

// Display area horizontal margin
static inline const UINT GetReminderMsgHMargin(void);
static inline void SetReminderMsgHMargin(UINT uiValue);

// Display area vertical margin
static inline const UINT GetReminderMsgVMargin(void);
static inline void SetReminderMsgVMargin(UINT uiValue);

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Include inline file for global inline functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _GLOBAL_ENABLE_INLINES
	#include "Global.inl"
	#ifdef _GLOBAL_INLINE_INCLUDED
		#pragma message("-- Global inline library included")
	#else
		#pragma error("-- Linking error in Global.h: Unable to link to inline header!")
	#endif
	#undef _GLOBAL_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in Global.h: Inline is not enabled!")
#endif
