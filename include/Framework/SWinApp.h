/**
 * @file		SWinApp.h
 * @brief		Custom base class for application (header file)
 * @author		AnthonyLeeStark
 * @date		2024.11.19
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/IDManager.h"
#include "AppCore/Logging.h"


// Define custom message for dialogs

#define SCM_APPLICATION			(WM_USER+1000)

#define SCM_CHANGE_LANGUAGE		(SCM_APPLICATION+1)
#define SCM_FORCE_EXITAPP		(SCM_APPLICATION+2)


// Custom base class for application
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
	bool m_bChangeFlag;
	bool m_bReadOnlyMode;
	bool m_bForceClose;

public:
	// Instance functions
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	virtual int	 PreExitInstance();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// App language management
	virtual bool InitAppLanguage(void);
	virtual bool ReloadAppLanguage(unsigned nCurLanguage = NULL);
	virtual LANGTABLE_PTR GetAppLanguage(void) {
		VERIFY(m_pAppLangPtr != NULL);
		if (m_pAppLangPtr == NULL) return NULL;
		return m_pAppLangPtr;
	};
	virtual UINT_PTR GetAppLanguageOption(bool bCurDispLang = false) const {
		if (bCurDispLang == true) return m_nCurDispLang;
		return m_nCurSetLang;
	};
	virtual void SetAppLanguageOption(unsigned nSetLanguage) {
		m_nCurSetLang = nSetLanguage;
	};

	// Application name get/set functions
	virtual bool SetAppName(unsigned nResourceStringID);
	virtual const wchar_t* GetAppName(void) const {
		return m_pszAppName;
	};
	virtual void GetAppName(String& appName) const {
		appName = m_pszAppName;
	};
	virtual void SetAppName(const wchar_t* appName)	{
		free((void*)m_pszAppName);
		m_pszAppName = _tcsdup(appName);
	};

	// Application window caption get/set functions
	virtual void SetAppWindowCaption(const wchar_t* windowCaption, bool bShowProdVersion = false, bool bFullVersion = false);
	virtual bool SetAppWindowCaption(unsigned nResourceStringID, bool bShowProdVersion = false, bool bFullVersion = false);

	virtual const wchar_t* GetAppWindowCaption(void) const {
		return m_strWindowCaption.GetString();
	};
	virtual void GetAppWindowCaption(String& windowCaption) const {
		windowCaption = m_strWindowCaption;
	};
	virtual void RegisterMessageBoxCaption(const wchar_t* caption) {
		m_strMessageCaption = caption;
	};

	// MessageBox functions
	virtual void RegisterMessageBoxCaption(unsigned nCaptionID);
	virtual int DoMessageBox(const wchar_t* prompt, unsigned nType, unsigned nIDPrompt);
	virtual int DisplayMessageBox(unsigned nPromptID, unsigned nCaptionID = NULL, unsigned nStyle = NULL);
	virtual int DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, unsigned nStyle = NULL);
	virtual void GetRegisterdMsgBoxCaption(String& regMsgBoxCaption) const {
		regMsgBoxCaption = m_strMessageCaption;
	};

	// Logging functions
	virtual void InitAppEventLog(void);
	virtual void OutputEventLog(USHORT usEvent, const wchar_t* description = NULL, LOGDETAILINFO* pDetailInfo = NULL);
	virtual SLogging* GetAppEventLog(void) {
		VERIFY(m_pAppEventLog != NULL);
		return m_pAppEventLog;
	};

	// Flag management functions
	virtual int  GetFlagValue(AppFlagID eFlagID) const;
	virtual void SetFlagValue(AppFlagID eFlagID, int nValue);
	virtual FlagManager& GetAppFlagManager(void) {
		return m_flagManager;
	};
	virtual const FlagManager& GetAppFlagManager(void) const {
		return m_flagManager;
	};

	// Directly access flag values
	virtual bool GetChangeFlagValue(void) const {
		return m_bChangeFlag;
	};
	virtual void SetChangeFlagValue(bool bChangeFlag) {
		m_bChangeFlag = bChangeFlag;
	};
	virtual bool CheckDataChangeState(void) {
		return true;
	};
	virtual bool CheckSettingChangeState(void) {
		return true;
	};
	virtual bool GetReadOnlyMode(void) const {
		return m_bReadOnlyMode;
	};
	virtual void SetReadOnlyMode(bool bReadOnly) {
		m_bReadOnlyMode = bReadOnly;
	};
	virtual bool IsForceClosingByRequest(void) const {
		return m_bForceClose;
	};

	// Request processing functions
	virtual LRESULT RequestCloseDialog(unsigned nDialogID);
	virtual LRESULT RequestCloseDialog(HWND hDialogWnd);
	virtual void PostErrorMessage(DWORD dwErrorCode, LPARAM lParam = NULL) {
		PostMessage(NULL, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
	};
	virtual void PostErrorMessage(HWND hRcvWnd, DWORD dwErrorCode, LPARAM lParam = NULL) {
		PostMessage(hRcvWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
	};
	virtual void PostErrorMessage(CWnd* pRcvWnd, DWORD dwErrorCode, LPARAM lParam = NULL) {
		pRcvWnd->PostMessage(SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
	};
};

