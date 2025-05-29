
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

#ifndef _GLOBAL_H_INCLUDED
#define _GLOBAL_H_INCLUDED

#include "stdafx.h"
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
	extern SYSTEMTIME g_stAppLaunchTime;
	extern UINT	g_uiAppLaunchTimeCounter;
};

// Access application launch-time data
static inline const SYSTEMTIME GetAppLaunchTime(void);
static inline void SetAppLaunchTime(SYSTEMTIME stTime);

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

// Define default value
#define DEFAULT_MSG_BKGRDCLR		COLOR_PINK					// Default background color: Bright pink
#define DEFAULT_MSG_TEXTCLR			COLOR_RED					// Default text color: Red
#define DEFAULT_MSG_FONTNAME		_T("Arial")					// Default font name: Arial
#define DEFAULT_MSG_FONTSIZE		20							// Default font size: 20pt
#define DEFAULT_MSG_TIMEOUT			0							// Default time-out: None
#define DEFAULT_MSG_ICONID			IDI_MSGICON_INFORMATION		// Default icon ID: MB_ICONINFORMATION
#define DEFAULT_MSG_ICONSIZE		50							// Default icon size: 50x50px
#define DEFAULT_MSG_ICONPLACEMENT	IconOnTheTop				// Default icon position: On top
#define DEFAULT_MSG_DISPLAYPOS		AtCenter					// Default display position: Center screen
#define DEFAULT_MSG_HMARGIN			50							// Default horizontal margin: 50px
#define DEFAULT_MSG_VMARGIN			50							// Default vertical margin: 50px
#define DEFAULT_PWRRMD_SNOOZETIME	600							// Default snooze time: 10 minutes

namespace Global {
	extern COLORREF g_colorMsgBkgrd;
	extern COLORREF g_colorMsgText;
	extern CString	g_strMsgFontName;
	extern UINT		g_uiMsgFontSize;
	extern UINT		g_uiRmdMsgTimeout;
	extern UINT		g_uiRmdMsgIconID;
	extern INT		g_nRmdMsgIconSize;
	extern BYTE		g_byRmdMsgIconPlacement;
	extern BYTE		g_byRmdMsgDispPos;
	extern UINT		g_uiRmdMsgHMargin;
	extern UINT		g_uiRmdMsgVMargin;
	extern UINT		g_uiRmdMsgSnoozeInterval;
};

// Background color
static inline const COLORREF GetReminderMsgBkgrdColor(void);
static inline void SetReminderMsgBkgrdColor(COLORREF colorValue);

// Text color
static inline const COLORREF GetReminderMsgTextColor(void);
static inline void SetReminderMsgTextColor(COLORREF colorValue);

// Font name
static inline BOOL GetReminderMsgFontName(CString& strFontName);
static inline void SetReminderMsgFontName(LPCTSTR lpszValue);

// Font size
static inline const UINT GetReminderMsgFontSize(void);
static inline void SetReminderMsgFontSize(UINT uiValue);

// Timeout (auto-close) interval
static inline const UINT GetReminderMsgTimeout(void);
static inline void SetReminderMsgTimeout(UINT uiValue);

// Define Message icon IDs
#define IDI_MSGICON_NOICON				0
#define IDI_MSGICON_APPLICATION     32512
#define IDI_MSGICON_HAND            32513
#define IDI_MSGICON_QUESTION        32514
#define IDI_MSGICON_EXCLAMATION     32515
#define IDI_MSGICON_ASTERISK        32516
#if(WINVER >= 0x0400)
#define IDI_MSGICON_WINLOGO         32517
#endif /* WINVER >= 0x0400 */
#if(WINVER >= 0x0600)
#define IDI_MSGICON_SHIELD          32518
#endif /* WINVER >= 0x0600 */
#if(WINVER >= 0x0400)
#define IDI_MSGICON_WARNING			IDI_MSGICON_EXCLAMATION
#define IDI_MSGICON_ERROR			IDI_MSGICON_HAND
#define IDI_MSGICON_INFORMATION		IDI_MSGICON_ASTERISK
#endif /* WINVER >= 0x0400 */

// Message icon ID
static inline const UINT GetReminderMsgIconID(void);
static inline void SetReminderMsgIconID(UINT uiValue);

// Message icon size
static inline const INT GetReminderMsgIconSize(void);
static inline void SetReminderMsgIconSize(int nValue);

// Message icon placement
enum MsgIconPlacement {
	IconOnTheTop = 0,
	IconOnTheLeft,
};
static inline const BYTE GetReminderMsgIconPlacement(void);
static inline void SetReminderMsgIconPlacement(BYTE byValue);

// Message display position
enum MsgDisplayPosition {
	AtCenter = 0,
	OnTopLeft,
	OnTopRight,
	OnBottomLeft,
	OnBottomRight,
};
static inline const BYTE GetReminderMsgDispPosition(void);
static inline void SetReminderMsgDispPosition(BYTE byValue);

// Display area horizontal margin
static inline const UINT GetReminderMsgHMargin(void);
static inline void SetReminderMsgHMargin(UINT uiValue);

// Display area vertical margin
static inline const UINT GetReminderMsgVMargin(void);
static inline void SetReminderMsgVMargin(UINT uiValue);

// Message snooze interval
static inline const UINT GetReminderMsgSnoozeInterval(void);
static inline void SetReminderMsgSnoozeInterval(UINT uiValue);

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Include inline file for global inline functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _GLOBAL_ENABLE_INLINES
	#include "Global.inl"
	#ifdef _GLOBAL_INL_INCLUDED
		#pragma message("-- Global inline library included")
	#else
		#pragma error("-- Linking error in Global.h: Unable to link to inline header!")
	#endif
	#undef _GLOBAL_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in Global.h: Inline is not enabled!")
#endif

#endif	// ifndef _GLOBAL_H_INCLUDED
