
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SWinApp.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for application (header file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.11.19:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"
#include "AppCore/Logging.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define custom message for dialogs
//
//////////////////////////////////////////////////////////////////////////

#define SCM_APPLICATION			(WM_USER+1000)

#define SCM_CHANGE_LANGUAGE		(SCM_APPLICATION+1)
#define SCM_FORCE_EXITAPP		(SCM_APPLICATION+2)


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SWinApp
//  Description: Custom base class for application
//  Base class:	 CWinAppEx
//
//////////////////////////////////////////////////////////////////////////

class SWinApp : public CWinAppEx
{
	DECLARE_DYNAMIC(SWinApp)

public:
	enum ExitCode {
		Error = -1,							// Exit because of error occurs
		NormalExit = 0,						// Normal exitting (no reason)
		PressExitButton,					// Exit by pressing [Exit] button
		FromNotifyIcon,						// Exit by selecting "Exit App" from notify menu selection
		RestartApp,							// Exit triggerred by Restart function
		FromDebugCommand,					// Exit by debug command
	};

public:
	// Construction
	SWinApp();								// constructor
	SWinApp(const wchar_t* templateName);	// custom constructor
	virtual ~SWinApp();						// destructor

protected:
	DECLARE_MESSAGE_MAP()

protected:
	// Application flag manager
	FlagManager m_flagManager;

	// Title and caption
	String  m_strTemplateName;
	String	m_strWindowCaption;
	String	m_strMessageCaption;

	// App language function
	LANGTABLE_PTR m_pAppLangPtr;
	UINT_PTR	  m_nCurSetLang;
	UINT_PTR	  m_nCurDispLang;

	// Logging pointer
	SLogging* m_pAppEventLog;

	// App special flags
	BOOL m_bChangeFlag;
	BOOL m_bReadOnlyMode;
	BOOL m_bForceClose;

public:
	// Instance functions
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	virtual int	 PreExitInstance();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// App language management
	virtual BOOL InitAppLanguage(void);
	virtual BOOL ReloadAppLanguage(UINT nCurLanguage = NULL);
	virtual LANGTABLE_PTR GetAppLanguage(void);
	virtual UINT_PTR GetAppLanguageOption(BOOL bCurDispLang = FALSE) const;
	virtual void SetAppLanguageOption(UINT nSetLanguage);

	// Application name get/set functions
	virtual const wchar_t* GetAppName(void) const;
	virtual void GetAppName(String& appName) const;
	virtual void SetAppName(const wchar_t* appName);
	virtual BOOL SetAppName(UINT nResourceStringID);

	// Application window caption get/set functions
	virtual const wchar_t* GetAppWindowCaption(void) const;
	virtual void GetAppWindowCaption(String& windowCaption) const;
	virtual void SetAppWindowCaption(const wchar_t* windowCaption, BOOL bShowProdVersion = FALSE, BOOL bFullVersion = FALSE);
	virtual BOOL SetAppWindowCaption(UINT nResourceStringID, BOOL bShowProdVersion = FALSE, BOOL bFullVersion = FALSE);

	// MessageBox functions
	virtual void RegisterMessageBoxCaption(UINT nCaptionID);
	virtual void RegisterMessageBoxCaption(const wchar_t* caption);
	virtual void GetRegisterdMsgBoxCaption(String& regMsgBoxCaption) const;
	virtual int DoMessageBox(const wchar_t* prompt, UINT nType, UINT nIDPrompt);
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, UINT nStyle = NULL);

	// Logging functions
	virtual void InitAppEventLog(void);
	virtual SLogging* GetAppEventLog(void);
	virtual void OutputEventLog(USHORT usEvent, const wchar_t* description = NULL, LOGDETAILINFO* pDetailInfo = NULL);

	// Flag management functions
	virtual int  GetFlagValue(AppFlagID eFlagID) const;
	virtual void SetFlagValue(AppFlagID eFlagID, int nValue);
	virtual FlagManager& GetAppFlagManager(void);
	virtual const FlagManager& GetAppFlagManager(void) const;

	// Directly access flag values
	virtual BOOL GetChangeFlagValue(void) const;
	virtual void SetChangeFlagValue(BOOL bChangeFlag);
	virtual BOOL CheckDataChangeState(void);
	virtual BOOL CheckSettingChangeState(void);
	virtual BOOL GetReadOnlyMode(void) const;
	virtual void SetReadOnlyMode(BOOL bReadOnly);
	virtual BOOL IsForceClosingByRequest(void) const;

	// Request processing functions
	virtual LRESULT RequestCloseDialog(UINT nDialogID);
	virtual LRESULT RequestCloseDialog(HWND hDialogWnd);
	virtual void PostErrorMessage(DWORD dwErrorCode, LPARAM lParam = NULL);
	virtual void PostErrorMessage(HWND hRcvWnd, DWORD dwErrorCode, LPARAM lParam = NULL);
	virtual void PostErrorMessage(CWnd* pRcvWnd, DWORD dwErrorCode, LPARAM lParam = NULL);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _SWINAPP_ENABLE_INLINES
	#include "Framework.inl"
	#ifdef _SWINAPP_INLINE_INCLUDED
		#pragma message("-- Framework inline library included (SWinApp.h)")
	#else
		#pragma error("-- Linking error in SWinApp.h: Unable to link to inline header!")
	#endif
	#undef _SWINAPP_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in SWinApp.h: Inline is not enabled!")
#endif
