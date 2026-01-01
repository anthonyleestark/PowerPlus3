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
	FlagManager flagManager_;

	// Title and caption
	String  templateName_;
	String	windowCaption_;
	String	messageCaption_;

	// App language function
	LANGTABLE_PTR appLanguagePtr_;
	UINT_PTR	  currentSetLanguage_;
	UINT_PTR	  currentDisplayLanguage_;

	// Logging pointer
	SLogging* appEventLogPtr_;

	// App special flags
	bool changeFlag_;
	bool isReadOnlyMode_;
	bool isForceClose_;

public:
	// Instance functions
	virtual BOOL InitInstance();
	virtual int  ExitInstance();
	virtual int	 PreExitInstance();

	virtual BOOL PreTranslateMessage(MSG* messagePtr);

	// App language management
	virtual bool initAppLanguage(void);
	virtual bool reloadAppLanguage(unsigned currentLanguage = NULL);
	LANGTABLE_PTR getAppLanguage(void) {
		VERIFY(appLanguagePtr_ != NULL);
		if (appLanguagePtr_ == NULL) return NULL;
		return appLanguagePtr_;
	}
	UINT_PTR getAppLanguageOption(bool currentDisplayLang = false) const {
		if (currentDisplayLang == true) return currentDisplayLanguage_;
		return currentSetLanguage_;
	}
	void setAppLanguageOption(unsigned language) {
		currentSetLanguage_ = language;
	}

	// Application name get/set functions
	bool setAppName(unsigned resourceStringId);
	const wchar_t* getAppName(void) const {
		return m_pszAppName;
	}
	void getAppName(String& appName) const {
		appName = m_pszAppName;
	}
	void setAppName(const wchar_t* appName)	{
		free((void*)m_pszAppName);
		m_pszAppName = _tcsdup(appName);
	}

	// Application window caption get/set functions
	void setAppWindowCaption(const wchar_t* windowCaption, bool showProdVersion = false, bool fullVersion = false);
	bool setAppWindowCaption(unsigned resourceStringId, bool showProdVersion = false, bool fullVersion = false);

	const wchar_t* getAppWindowCaption(void) const {
		return windowCaption_.getString();
	}
	void getAppWindowCaption(String& windowCaption) const {
		windowCaption = windowCaption_;
	}
	void registerMessageBoxCaption(const wchar_t* caption) {
		messageCaption_ = caption;
	}

	// MessageBox functions
	void registerMessageBoxCaption(unsigned captionId);
	virtual int DoMessageBox(const wchar_t* prompt, unsigned type, unsigned nIDPrompt);
	int displayMessageBox(unsigned promptId, unsigned captionId = NULL, unsigned style = NULL);
	int displayMessageBox(const wchar_t* prompt, const wchar_t* caption = NULL, unsigned style = NULL);
	void getRegisterdMsgBoxCaption(String& regMsgBoxCaption) const {
		regMsgBoxCaption = messageCaption_;
	}

	// Logging functions
	virtual void initAppEventLog(void);
	void outputEventLog(USHORT eventId, const wchar_t* description = NULL, LOGDETAILINFO* detailInfoPtr = NULL);
	SLogging* getAppEventLog(void) {
		VERIFY(appEventLogPtr_ != NULL);
		return appEventLogPtr_;
	}

	// Flag management functions
	virtual int  getFlagValue(AppFlagID flagId) const;
	virtual void setFlagValue(AppFlagID flagId, int value);
	FlagManager& getAppFlagManager(void) {
		return flagManager_;
	}
	const FlagManager& getAppFlagManager(void) const {
		return flagManager_;
	}

	// Directly access flag values
	bool getChangeFlagValue(void) const {
		return changeFlag_;
	}
	void setChangeFlagValue(bool value) {
		changeFlag_ = value;
	}
	virtual bool checkDataChangeState(void) {
		return true;
	}
	virtual bool checkSettingChangeState(void) {
		return true;
	}
	bool getReadOnlyMode(void) const {
		return isReadOnlyMode_;
	}
	void setReadOnlyMode(bool value) {
		isReadOnlyMode_ = value;
	}
	bool isForceClosingByRequest(void) const {
		return isForceClose_;
	}

	// Request processing functions
	virtual LRESULT requestCloseDialog(unsigned dialogId);
	virtual LRESULT requestCloseDialog(HWND dialogWndHandle);
	virtual void postErrorMessage(DWORD errorCode, LPARAM lParam = NULL) {
		PostMessage(NULL, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, lParam);
	};
	virtual void postErrorMessage(HWND receivedWndHandle, DWORD errorCode, LPARAM lParam = NULL) {
		PostMessage(receivedWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, lParam);
	};
	virtual void postErrorMessage(CWnd* receivedWndPtr, DWORD errorCode, LPARAM lParam = NULL) {
		receivedWndPtr->PostMessage(SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, lParam);
	};
};

