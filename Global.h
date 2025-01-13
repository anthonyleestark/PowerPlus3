
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
//	Define categories of global variables
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEF_GLBVAR_CATE_DEBUGTEST	0x01		// Debugging/Testing variables
#define DEF_GLBVAR_CATE_APPFLAG		0x02		// App special flags
#define DEF_GLBVAR_CATE_SPECIAL		0x03		// Special variables


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for debugging/testing
// These special variables can only be changed by using debug commands
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------Debug/Test flags---------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST		FALSE
#define DEFAULT_DEBUGMODE		FALSE
#define DEFAULT_DEBUGOUTPUT		DBOUT_DEFAULT

// Dummy test mode flag
extern BOOL g_bDummyTest;
static inline BOOL GetDummyTestMode() { return g_bDummyTest; }
static inline void SetDummyTestMode(BOOL bValue) { g_bDummyTest = bValue; }

// Debug mode flag
extern BOOL g_bDebugMode;
static inline BOOL GetDebugMode() {	return g_bDebugMode; }
static inline void SetDebugMode(BOOL bValue) { g_bDebugMode = bValue; }

// Debug log output target flag
enum eDEBUGOUTPUT {
	DBOUT_DEFAULT = 0,
	DBOUT_DEBUGINFOFILE,
	DBOUT_DEBUGTESTTOOL,
};
extern int g_nDebugOutputTarget;
static inline BOOL GetDebugOutputTarget() { return g_nDebugOutputTarget; }
static inline void SetDebugOutputTarget(int nValue) { g_nDebugOutputTarget = nValue; }

/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------Debug/Test special variables---------------------------------------*/

// Trace error log file pointer
extern CFile* g_pFileLogTraceError;
static inline CFile* GetTraceErrorLogFile() { return g_pFileLogTraceError; }
static BOOL InitTraceErrorLogFile(void);
inline void ReleaseTraceErrorLogFile(void);

// Trace debug info log file pointer
extern CFile* g_pFileLogTraceDebug;
static inline CFile* GetTraceDebugLogFile() { return g_pFileLogTraceDebug; }
static BOOL InitTraceDebugLogFile(void);
inline void ReleaseTraceDebugLogFile(void);

// Debug info output log file pointer
extern CFile* g_pFileLogDebugInfo;
static inline CFile* GetDebugInfoLogFile() { return g_pFileLogDebugInfo; }
static BOOL InitDebugInfoLogFile(void);
inline void ReleaseDebugInfoLogFile(void);

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for other special purposes,
// These special variables can only be changed by using debug commands
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------App flags-------------------------------------------------*/

// Power action trace flag
extern BYTE g_byPwrActionFlag;
static inline BYTE GetPwrActionFlag() { return g_byPwrActionFlag; }
static inline void SetPwrActionFlag(BYTE byValue) { g_byPwrActionFlag = byValue; }

// System suspended trace flag
extern BYTE g_bySystemSuspendFlag;
static inline BYTE GetSystemSuspendFlag() { return g_bySystemSuspendFlag; }
static inline void SetSystemSuspendFlag(BYTE byValue) { g_bySystemSuspendFlag = byValue; }

// Session ending trace flag
extern BYTE g_bySessionEndFlag;
static inline BYTE GetSessionEndFlag() { return g_bySessionEndFlag; }
static inline void SetSessionEndFlag(BYTE byValue) { g_bySessionEndFlag = byValue; }

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Define default value
#define DEFAULT_MSGBKGRDCLR		RGB(230,190,200)			// Default background color: Bright pink
#define DEFAULT_MSGTEXTCLR		RGB(255,0,0)				// Default text color: Red
#define DEFAULT_MSGFONTNAME		_T("Arial")					// Default font name: Arial
#define DEFAULT_MSGFONTSIZE		20							// Default font size: 20pt
#define DEFAULT_MSGTIMEOUT		0							// Default time-out: None
#define DEFAULT_MSGICONID		IDI_MSGICON_INFORMATION		// Default icon ID: MB_ICONINFORMATION
#define DEFAULT_MSGICONSIZE		50							// Default icon size: 50x50px
#define DEFAULT_MSGICONPOS		MSGICONPOS_ONTOP			// Default icon position: On top
#define DEFAULT_MSGDISPPOS		MSGDISPPOS_CENTER			// Default display position: Center screen
#define DEFAULT_MSGHMARGIN		50							// Default horizontal margin: 50px
#define DEFAULT_MSGVMARGIN		50							// Default vertical margin: 50px
#define DEFAULT_SNOOZETIME		600							// Default snooze time: 10 minutes

// Background color
extern COLORREF g_colorMsgBkgrd;
static inline COLORREF GetReminderMsgBkgrdColor() { return g_colorMsgBkgrd; }
static inline void SetReminderMsgBkgrdColor(COLORREF colorValue) { g_colorMsgBkgrd = colorValue; }

// Text color
extern COLORREF g_colorMsgText;
static inline COLORREF GetReminderMsgTextColor() { return g_colorMsgText; }
static inline void SetReminderMsgTextColor(COLORREF colorValue) { g_colorMsgText = colorValue; }

// Font name
extern CString g_strMsgFontName;
static inline BOOL GetReminderMsgFontName(CString& strFontName) { strFontName = g_strMsgFontName; return (!strFontName.IsEmpty()); }
static inline void SetReminderMsgFontName(LPCTSTR lpszValue) { if (_tcscmp(lpszValue, _T(""))) g_strMsgFontName = lpszValue; }

// Font size
extern UINT	g_uiMsgFontSize;
static inline UINT GetReminderMsgFontSize() { return g_uiMsgFontSize; }
static inline void SetReminderMsgFontSize(UINT uiValue) { g_uiMsgFontSize = uiValue; }

// Timeout (auto-close) interval
extern UINT g_uiRmdMsgTimeout;
static inline UINT GetReminderMsgTimeout() { return g_uiRmdMsgTimeout; }
static inline void SetReminderMsgTimeout(UINT uiValue) { g_uiRmdMsgTimeout = uiValue; }

// Message icon ID
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

extern UINT g_uiRmdMsgIconID;
static inline UINT GetReminderMsgIconID() { return g_uiRmdMsgIconID; }
static inline void SetReminderMsgIconID(UINT uiValue) { g_uiRmdMsgIconID = uiValue; };

// Message icon size
extern int g_nRmdMsgIconSize;
static inline int GetReminderMsgIconSize() { return g_nRmdMsgIconSize; }
static inline void SetReminderMsgIconSize(int nValue) { g_nRmdMsgIconSize = nValue; };

// Message icon position
enum eMSGICONPOSITION {
	MSGICONPOS_ONTOP = 0,
	MSGICONPOS_ONLEFT,
};
extern BYTE g_byRmdMsgIconPos;
static inline BYTE GetReminderMsgIconPosition() { return g_byRmdMsgIconPos; }
static inline void SetReminderMsgIconPosition(BYTE byValue) { g_byRmdMsgIconPos = byValue; };

// Message display position
enum eMSGDISPPOSITION {
	MSGDISPPOS_CENTER = 0,
	MSGDISPPOS_TOPLEFT,
	MSGDISPPOS_TOPRIGHT,
	MSGDISPPOS_BOTTOMLEFT,
	MSGDISPPOS_BOTTOMRIGHT,
};
extern BYTE g_byRmdMsgDispPos;
static inline BYTE GetReminderMsgDispPosition() { return g_byRmdMsgDispPos; }
static inline void SetReminderMsgDispPosition(BYTE byValue) { g_byRmdMsgDispPos = byValue; };

// Display area horizontal margin
extern UINT g_uiRmdMsgHMargin;
static inline UINT GetReminderMsgHMargin() { return g_uiRmdMsgHMargin; }
static inline void SetReminderMsgHMargin(UINT uiValue) { g_uiRmdMsgHMargin = uiValue; }

// Display area vertical margin
extern UINT g_uiRmdMsgVMargin;
static inline UINT GetReminderMsgVMargin() { return g_uiRmdMsgVMargin; }
static inline void SetReminderMsgVMargin(UINT uiValue) { g_uiRmdMsgVMargin = uiValue; }

// Message snooze interval
extern UINT g_uiRmdMsgSnoozeInterval;
static inline UINT GetReminderMsgSnoozeInterval() { return g_uiRmdMsgSnoozeInterval; }
static inline void SetReminderMsgSnoozeInterval(UINT uiValue) { g_uiRmdMsgSnoozeInterval = uiValue; }

/*-----------------------------------------------------------------------------------------------------------*/


#endif	// ifndef _GLOBAL_H_INCLUDED
