
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Global.h
//		Description:	Define global objects which will be used everywhere in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.01.27:		Create new
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

/*-------------------------------------------Debug/Test variables-------------------------------------------*/

// Define default value
#define DEFAULT_DUMMYTEST		FALSE
#define DEFAULT_DEBUGMODE		FALSE
#define DEFAULT_DEBUGLOGSTYLE	DBLOG_OUTPUTDBSTRING

// Dummy test mode
extern BOOL g_bDummyTest;
static inline BOOL GetDummyTestMode() { return g_bDummyTest; }
static inline void SetDummyTestMode(BOOL bValue) { g_bDummyTest = bValue; }

// Debug mode
extern BOOL g_bDebugMode;
static inline BOOL GetDebugMode() {	return g_bDebugMode; }
static inline void SetDebugMode(BOOL bValue) { g_bDebugMode = bValue; }

// Debug log style
enum eDEBUGLOGSTYLE {
	DBLOG_OUTPUTDBSTRING = 0,
	DBLOG_OUTPUTTOFILE,
	DBLOG_OUTPUTTODBTOOL,
};
extern int g_nDebugLogStyle;
static inline BOOL GetDebugLogStyle() { return g_nDebugLogStyle; }
static inline void SetDebugLogStyle(int nValue) { g_nDebugLogStyle = nValue; }

/*-----------------------------------------------------------------------------------------------------------*/


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Define global variables and methods which are used for other special purposes,
// These special variables can only be changed by using debug commands
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*-------------------------------------------------App flags-------------------------------------------------*/

// Power reminder action trace flag
extern BYTE g_byPwrActionFlag;
static inline BYTE GetPwrActionFlag() { return g_byPwrActionFlag; }
static inline void SetPwrActionFlag(BYTE byValue) { g_byPwrActionFlag = byValue; }

/*-----------------------------------------------------------------------------------------------------------*/

/*---------------------------------------Power reminder message styles---------------------------------------*/

// Define default value
#define DEFAULT_MSGBKGRDCLR		RGB(230,190,200)
#define DEFAULT_MSGTEXTCLR		RGB(255,0,0)
#define DEFAULT_MSGFONTNAME		_T("Arial")
#define DEFAULT_MSGFONTSIZE		20
#define DEFAULT_MSGTIMEOUT		0
#define DEFAULT_MSGICONID		IDI_MSGICON_INFORMATION
#define DEFAULT_MSGICONSIZE		50
#define DEFAULT_MSGICONPOS		MSGICONPOS_ONTOP
#define DEFAULT_MSGHMARGIN		50
#define DEFAULT_MSGVMARGIN		50

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

// Display area horizontal margin
extern UINT g_uiRmdMsgHMargin;
static inline UINT GetReminderMsgHMargin() { return g_uiRmdMsgHMargin; }
static inline void SetReminderMsgHMargin(UINT uiValue) { g_uiRmdMsgHMargin = uiValue; }

// Display area vertical margin
extern UINT g_uiRmdMsgVMargin;
static inline UINT GetReminderMsgVMargin() { return g_uiRmdMsgVMargin; }
static inline void SetReminderMsgVMargin(UINT uiValue) { g_uiRmdMsgVMargin = uiValue; }

/*-----------------------------------------------------------------------------------------------------------*/


#endif	// ifndef _GLOBAL_H_INCLUDED
