
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.h
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

#ifndef _GLOBAL_H_INCLUDED
#define _GLOBAL_H_INCLUDED

#include "stdafx.h"


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
static inline const SYSTEMTIME GetAppLaunchTime(void) { return Global::g_stAppLaunchTime; }
static inline void SetAppLaunchTime(SYSTEMTIME stTime) { Global::g_stAppLaunchTime = stTime; }

// Access application launch-time counter data
static inline const UINT GetAppLaunchTimeCounter(void) { return Global::g_uiAppLaunchTimeCounter; }
static inline const BOOL IsAppFirstLaunch(void) { return (Global::g_uiAppLaunchTimeCounter <= 1); }
static inline void SetAppLaunchTimeCounter(UINT uiValue) { Global::g_uiAppLaunchTimeCounter = uiValue; }
static inline void UpdateAppLaunchTimeCounter(void) { ++Global::g_uiAppLaunchTimeCounter; }

/*-----------------------------------------------------------------------------------------------------------*/


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

namespace Global {
	extern BOOL	 g_bDummyTest;
	extern BOOL	 g_bDebugMode;
	extern INT	 g_nDebugOutputTarget;
	extern BOOL  g_bTestFeature;
};

// Dummy test mode flag
static inline const BOOL GetDummyTestMode(void) { return Global::g_bDummyTest; }
static inline void SetDummyTestMode(BOOL bValue) { Global::g_bDummyTest = bValue; }

// Debug mode flag
static inline const BOOL GetDebugMode(void) { return Global::g_bDebugMode; }
static inline void SetDebugMode(BOOL bValue) { Global::g_bDebugMode = bValue; }

// Debug log output target flag
enum DebugOutput {
	DefaultOutput = 0,
	DebugInfoFile,
	DebugTestTool,
};
static inline const INT GetDebugOutputTarget(void) { return Global::g_nDebugOutputTarget; }
static inline void SetDebugOutputTarget(int nValue) { Global::g_nDebugOutputTarget = nValue; }

// Test feature enable flag
static inline const BOOL GetTestFeatureEnable(void) { return Global::g_bTestFeature; }
static inline void SetTestFeatureEnable(BOOL bValue) { Global::g_bTestFeature = bValue; }

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
static inline CFile* GetTraceErrorLogFile(void) { return Global::g_pFileLogTraceError; }
static BOOL InitTraceErrorLogFile(void);
inline void ReleaseTraceErrorLogFile(void);

//Trace debug info log file pointer
static inline CFile* GetTraceDebugLogFile(void) { return Global::g_pFileLogTraceDebug; }
static BOOL InitTraceDebugLogFile(void);
inline void ReleaseTraceDebugLogFile(void);

// Debug info output log file pointer
static inline CFile* GetDebugInfoLogFile(void) { return Global::g_pFileLogDebugInfo; }
static BOOL InitDebugInfoLogFile(void);
inline void ReleaseDebugInfoLogFile(void);

// Trace error log file exception pointer
static inline CFileException* GetTraceErrorException(void) { return Global::g_pExcLogTraceError; }

// Trace debug info log file exception pointer
static inline CFileException* GetTraceDebugException(void) { return Global::g_pExcLogTraceDebug; }

// Debug info output log file exception pointer
static inline CFileException* GetDebugInfoException(void) { return Global::g_pExcLogDebugInfo; }

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for other special purposes,
// These special variables can only be changed by using debug commands
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------App flags-------------------------------------------------*/

namespace Global {
	extern BYTE g_byPwrActionFlag;
	extern BYTE g_bySystemSuspendFlag;
	extern BYTE g_bySessionEndFlag;
};

// Power action trace flag
static inline const BYTE GetPwrActionFlag(void) { return Global::g_byPwrActionFlag; }
static inline void SetPwrActionFlag(BYTE byValue) { Global::g_byPwrActionFlag = byValue; }

// System suspended trace flag
static inline const BYTE GetSystemSuspendFlag(void) { return Global::g_bySystemSuspendFlag; }
static inline void SetSystemSuspendFlag(BYTE byValue) { Global::g_bySystemSuspendFlag = byValue; }

// Session ended trace flag
static inline const BYTE GetSessionEndFlag(void) { return Global::g_bySessionEndFlag; }
static inline void SetSessionEndFlag(BYTE byValue) { Global::g_bySessionEndFlag = byValue; }

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------Memory-only flags---------------------------------------------*/

namespace Global {
	extern BYTE g_bySessionLockFlag;
};

// Session lock trace flag
static inline const BYTE GetSessionLockFlag(void) { return Global::g_bySessionLockFlag; }
static inline void SetSessionLockFlag(BYTE byValue) { Global::g_bySessionLockFlag = byValue; }

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Define default value
#define DEFAULT_MSG_BKGRDCLR		RGB(230,190,200)			// Default background color: Bright pink
#define DEFAULT_MSG_TEXTCLR			RGB(255,0,0)				// Default text color: Red
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
static inline const COLORREF GetReminderMsgBkgrdColor(void) { return Global::g_colorMsgBkgrd; }
static inline void SetReminderMsgBkgrdColor(COLORREF colorValue) { Global::g_colorMsgBkgrd = colorValue; }

// Text color
static inline const COLORREF GetReminderMsgTextColor(void) { return Global::g_colorMsgText; }
static inline void SetReminderMsgTextColor(COLORREF colorValue) { Global::g_colorMsgText = colorValue; }

// Font name
static inline BOOL GetReminderMsgFontName(CString& strFontName) { strFontName = Global::g_strMsgFontName; return (!strFontName.IsEmpty()); }
static inline void SetReminderMsgFontName(LPCTSTR lpszValue) { if (_tcscmp(lpszValue, _T(""))) Global::g_strMsgFontName = lpszValue; }

// Font size
static inline const UINT GetReminderMsgFontSize(void) { return Global::g_uiMsgFontSize; }
static inline void SetReminderMsgFontSize(UINT uiValue) { Global::g_uiMsgFontSize = uiValue; }

// Timeout (auto-close) interval
static inline const UINT GetReminderMsgTimeout(void) { return Global::g_uiRmdMsgTimeout; }
static inline void SetReminderMsgTimeout(UINT uiValue) { Global::g_uiRmdMsgTimeout = uiValue; }

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
static inline const UINT GetReminderMsgIconID(void) { return Global::g_uiRmdMsgIconID; }
static inline void SetReminderMsgIconID(UINT uiValue) { Global::g_uiRmdMsgIconID = uiValue; };

// Message icon size
static inline const INT GetReminderMsgIconSize(void) { return Global::g_nRmdMsgIconSize; }
static inline void SetReminderMsgIconSize(int nValue) { Global::g_nRmdMsgIconSize = nValue; };

// Message icon placement
enum MsgIconPlacement {
	IconOnTheTop = 0,
	IconOnTheLeft,
};
static inline const BYTE GetReminderMsgIconPlacement(void) { return Global::g_byRmdMsgIconPlacement; }
static inline void SetReminderMsgIconPlacement(BYTE byValue) { Global::g_byRmdMsgIconPlacement = byValue; };

// Message display position
enum MsgDisplayPosition {
	AtCenter = 0,
	OnTopLeft,
	OnTopRight,
	OnBottomLeft,
	OnBottomRight,
};
static inline const BYTE GetReminderMsgDispPosition(void) { return Global::g_byRmdMsgDispPos; }
static inline void SetReminderMsgDispPosition(BYTE byValue) { Global::g_byRmdMsgDispPos = byValue; };

// Display area horizontal margin
static inline const UINT GetReminderMsgHMargin(void) { return Global::g_uiRmdMsgHMargin; }
static inline void SetReminderMsgHMargin(UINT uiValue) { Global::g_uiRmdMsgHMargin = uiValue; }

// Display area vertical margin
static inline const UINT GetReminderMsgVMargin(void) { return Global::g_uiRmdMsgVMargin; }
static inline void SetReminderMsgVMargin(UINT uiValue) { Global::g_uiRmdMsgVMargin = uiValue; }

// Message snooze interval
static inline const UINT GetReminderMsgSnoozeInterval(void) { return Global::g_uiRmdMsgSnoozeInterval; }
static inline void SetReminderMsgSnoozeInterval(UINT uiValue) { Global::g_uiRmdMsgSnoozeInterval = uiValue; }

/*-----------------------------------------------------------------------------------------------------------*/


#endif	// ifndef _GLOBAL_H_INCLUDED
