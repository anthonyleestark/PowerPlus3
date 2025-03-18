
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

#ifndef _STUPIDFRAMEWORK_SWINAPP_H_INCLUDED
#define _STUPIDFRAMEWORK_SWINAPP_H_INCLUDED

#include "stdafx.h"

#include "Core.h"
#include "IDMapping.h"
#include "Logging.h"


//////////////////////////////////////////////////////////////////////////
//
//	Define custom message for dialogs
//
//////////////////////////////////////////////////////////////////////////

#define SCM_APPLICATION			(WM_USER+1000)

#define SCM_CHANGE_LANGUAGE		(SCM_APPLICATION+1)
#define SCM_FORCE_EXITAPP		(SCM_APPLICATION+2)


/////////////////////////////////////////////////////////////////
//
//	Class name:	 SWinApp
//  Description: Custom base class for application
//  Base class:	 CWinAppEx
//
/////////////////////////////////////////////////////////////////

class SWinApp : public CWinAppEx
{
	DECLARE_DYNAMIC(SWinApp)

	// Construction
public:
	SWinApp();								// constructor
	SWinApp(LPCTSTR lpszTemplateName);		// custom constructor
	virtual ~SWinApp();						// destructor

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_APP_IDMAP()

protected:
	// Title and caption
	CString m_strTemplateName;
	CString	m_strWindowCaption;
	CString	m_strMessageCaption;

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
	virtual LPCTSTR GetAppName(void) const;
	virtual void GetAppName(CString& strAppName) const;
	virtual void SetAppName(LPCTSTR lpszAppName);
	virtual BOOL SetAppName(UINT nResourceStringID);

	// Application window caption get/set functions
	virtual LPCTSTR GetAppWindowCaption(void) const;
	virtual void GetAppWindowCaption(CString& strWindowCaption) const;
	virtual void SetAppWindowCaption(LPCTSTR lpszWindowCaption);
	virtual BOOL SetAppWindowCaption(UINT nResourceStringID);
	virtual BOOL SetAppWindowCaption(UINT nResourceStringID, LPCTSTR lpszProductVersion);

	// MessageBox functions
	virtual void RegisterMessageBoxCaption(UINT nCaptionID);
	virtual void RegisterMessageBoxCaption(LPCTSTR lpszCaption);
	virtual void GetRegisterdMsgBoxCaption(CString& strRegMsgBoxCap) const;
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	virtual int DisplayMessageBox(UINT nPromptID, UINT nCaptionID = NULL, UINT nStyle = NULL);
	virtual int DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption = NULL, UINT nStyle = NULL);

	// Logging functions
	virtual void InitAppEventLog(void);
	virtual SLogging* GetAppEventLog(void);
	virtual void OutputEventLog(USHORT usEvent, LPCTSTR lpszDescription = NULL, LOGDETAILINFO* pDetailInfo = NULL);

	// Data processing and flags get/set functions
	virtual int  GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp = FALSE) const;
	virtual int  GetFlagValue(APPFLAGID eFlagID) const;
	virtual void SetFlagValue(APPFLAGID eFlagID, int nValue);
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

#endif	// ifndef _STUPIDFRAMEWORK_SWINAPP_H_INCLUDED
