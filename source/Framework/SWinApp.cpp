
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		SWinApp.h
//		Project name:	Stupid Framework
//		Description:	Custom base class for application (source file)
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.11.19:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Framework\SWinApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for SWinApp
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(SWinApp, CWinAppEx)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SWinApp
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SWinApp::SWinApp() : CWinAppEx()
{
	// Title and caption
	m_strTemplateName = STRING_EMPTY;
	m_strWindowCaption = STRING_EMPTY;
	m_strMessageCaption = STRING_EMPTY;

	// App language function
	m_pAppLangPtr = NULL;
	m_nCurSetLang = INT_NULL;
	m_nCurDispLang = INT_NULL;

	// Logging pointer
	m_pAppEventLog = NULL;

	// App special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bForceClose = FALSE;
}

SWinApp::SWinApp(LPCTSTR lpszTemplateName) : CWinAppEx()
{
	// Title and caption
	m_strTemplateName = lpszTemplateName;
	m_strWindowCaption = STRING_EMPTY;
	m_strMessageCaption = STRING_EMPTY;

	// App language function
	m_pAppLangPtr = NULL;
	m_nCurSetLang = INT_NULL;
	m_nCurDispLang = INT_NULL;

	// Logging pointer
	m_pAppEventLog = NULL;

	// App special flags
	m_bReadOnlyMode = FALSE;
	m_bChangeFlag = FALSE;
	m_bForceClose = FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SWinApp
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SWinApp::~SWinApp()
{
	// Delete logging pointer
	if (m_pAppEventLog != NULL) {
		delete m_pAppEventLog;
		m_pAppEventLog = NULL;
	}

	// Destroy resource ID map
	DESTROY_RESOURCEID_MAP()
}

//////////////////////////////////////////////////////////////////////////
//
//	SWinApp application message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(SWinApp, CWinAppEx)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitInstance
//	Description:	Default method for application instance initialization
//  Arguments:		None
//  Return value:	Default
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::InitInstance()
{
	return CWinAppEx::InitInstance();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExitInstance
//	Description:	Default method for application instance exiting
//  Arguments:		None
//  Return value:	Default
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::ExitInstance()
{
	return CWinAppEx::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreExitInstance
//	Description:	Execute some actions before exiting app instance
//  Arguments:		None
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::PreExitInstance()
{
	// TODO: Override this function for custom actions

	return RESULT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreTranslateMessage
//	Description:	Default method for pre-translate message
//  Arguments:		pMsg - Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::PreTranslateMessage(MSG* pMsg)
{
	// Default
	return CWinAppEx::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppLanguage
//	Description:	Initialize application language package pointer
//  Arguments:		None
//  Return value:	BOOL - Result of initialization
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::InitAppLanguage(void)
{
	// Update current language setting
	UINT nCurLanguage = GetAppLanguageOption();
	m_nCurDispLang = nCurLanguage;

	// Load language package
	m_pAppLangPtr = LoadLanguageTable(nCurLanguage);

	// Check validity after loading
	if (m_pAppLangPtr == NULL) {
		TRACE_ERROR("Error: Language pointer acquiring failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReloadAppLanguage
//	Description:	Reload application language package pointer
//  Arguments:		nCurLanguage - Current language ID
//  Return value:	BOOL - Result of reloading process
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::ReloadAppLanguage(UINT nCurLanguage /* = NULL */)
{
	// If language is not specified
	if (nCurLanguage == NULL) {
		// Load current language option
		nCurLanguage = GetAppLanguageOption();
	}

	// If language changed
	if (m_nCurDispLang != nCurLanguage) {
		// Output event log
		CString strEventDescription;
		CString strOldLang = GetLanguageName(m_nCurDispLang);
		CString strNewLang = GetLanguageName(nCurLanguage);
		strEventDescription.Format(STRING_TEXTCHANGEFORMAT, strOldLang, strNewLang);
		OutputEventLog(LOG_EVENT_CHANGE_LANGUAGE, strEventDescription);
	}

	// Update current displaying language
	m_nCurDispLang = nCurLanguage;

	// Reload language package
	m_pAppLangPtr = LoadLanguageTable(nCurLanguage);

	// Check validity
	if (m_pAppLangPtr == NULL)
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppLanguage
//	Description:	Get application current language package pointer
//  Arguments:		None
//  Return value:	LANGTABLE_PTR - Language package pointer
//
//////////////////////////////////////////////////////////////////////////

LANGTABLE_PTR SWinApp::GetAppLanguage(void)
{
	// Check validity
	VERIFY(m_pAppLangPtr != NULL);
	if (m_pAppLangPtr == NULL)
		return NULL;

	return m_pAppLangPtr;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppLanguageOption
//	Description:	Get application current language option
//  Arguments:		bCurDispLang - Flag to get current displaying language
//  Return value:	UINT - Language ID
//
//////////////////////////////////////////////////////////////////////////

UINT_PTR SWinApp::GetAppLanguageOption(BOOL bCurDispLang /* = FALSE */) const
{
	// Return currently displaying language
	if (bCurDispLang == TRUE)
		return m_nCurDispLang;

	// Return current setting language
	return m_nCurSetLang;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppLanguageOption
//	Description:	Set application current language option
//  Arguments:		nSetLanguage - Language setting
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::SetAppLanguageOption(UINT nSetLanguage)
{
	m_nCurSetLang = nSetLanguage;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppName
//	Description:	Get application name
//  Arguments:		None
//  Return value:	LPCTSTR - App name string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR SWinApp::GetAppName(void) const
{
	return m_pszAppName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppName
//	Description:	Get application name
//  Arguments:		strAppName - Application name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::GetAppName(CString& strAppName) const
{
	strAppName = m_pszAppName;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppName
//	Description:	Set application name
//  Arguments:		lpszAppName - Application name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::SetAppName(LPCTSTR lpszAppName)
{
	// First, free default allocated string
	free((void*)m_pszAppName);

	// Assign new application name string
	m_pszAppName = _tcsdup(lpszAppName);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppName
//	Description:	Set application name by resource string ID
//  Arguments:		nResourceStringID - Resource string ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::SetAppName(UINT nResourceStringID)
{
	// Load resource string
	CString strTemp;
	BOOL bRet = strTemp.LoadString(nResourceStringID);
	ASSERT(!strTemp.IsEmpty());
	if ((bRet == TRUE) && (!strTemp.IsEmpty())) {
		// Set app name
		SetAppName(strTemp);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppWindowCaption
//	Description:	Get application window common caption
//  Arguments:		None
//  Return value:	LPCTSTR - App caption string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR SWinApp::GetAppWindowCaption(void) const
{
	return m_strWindowCaption.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppWindowCaption
//	Description:	Get application window common caption
//  Arguments:		strAppWndTitle - Application window caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::GetAppWindowCaption(CString& strWindowCaption) const
{
	strWindowCaption = m_strWindowCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppWindowCaption
//	Description:	Set application window common caption
//  Arguments:		lpszWindowCaption - Application window caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::SetAppWindowCaption(LPCTSTR lpszWindowCaption, BOOL bShowProdVersion /* = FALSE */, BOOL bFullVersion /* = FALSE */)
{
	// Set caption
	m_strWindowCaption = lpszWindowCaption;

	// Show product version
	if (bShowProdVersion == TRUE) {
		CString strTemp;
		strTemp.Format(_T(" %s"), GetProductVersion(bFullVersion));
		m_strWindowCaption.Append(strTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppWindowCaption
//	Description:	Set application window common caption by resource string ID
//  Arguments:		nResourceStringID - Resource string ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::SetAppWindowCaption(UINT nResourceStringID, BOOL bShowProdVersion /* = FALSE */, BOOL bFullVersion /* = FALSE */)
{
	CString strWindowCaption;
	BOOL bRet = strWindowCaption.LoadString(nResourceStringID);
	ASSERT(!strWindowCaption.IsEmpty());
	if ((bRet == TRUE) && (!strWindowCaption.IsEmpty())) {
		// Set app window caption 
		SetAppWindowCaption(strWindowCaption, bShowProdVersion, bFullVersion);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterMessageBoxCaption
//	Description:	Register default caption for message box-es
//  Arguments:		nCaptionID  - ID of message caption string
//					lpszCaption	- Message caption string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::RegisterMessageBoxCaption(UINT nCaptionID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = this->GetAppLanguage();
	CString strCaption = STRING_EMPTY;
	if (nCaptionID != NULL) {

		// Get language string caption
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(strTemp)) {
			// Set caption string
			strCaption = strTemp;
		}
	}

	// If caption is empty
	if (strCaption.IsEmpty()) {
		// Use default app window caption
		strCaption = this->GetAppWindowCaption();
	}

	// Register message box caption
	RegisterMessageBoxCaption(strCaption);
}

void SWinApp::RegisterMessageBoxCaption(LPCTSTR lpszCaption)
{
	// Register message box caption
	m_strMessageCaption = lpszCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetRegisterdMsgBoxCaption
//	Description:	Get registered message box caption string
//  Arguments:		strRegMsgBoxCap - Registered message box caption
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::GetRegisterdMsgBoxCaption(CString& strRegMsgBoxCap) const
{
	strRegMsgBoxCap = m_strMessageCaption;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoMessageBox
//	Description:	Override this function to customize application-wide 
//					processing of AfxMessageBox calls
//  Arguments:		lpszPrompt  - Message box text
//					nType		- Message box style
//					nIDPrompt	- An index to a Help context string.
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt)
{
	// Message caption
	CString strMsgCaption;
	strMsgCaption.Empty();

	// If application message box caption is registered
	if (!m_strMessageCaption.IsEmpty()) {
		// Use registered message box caption
		strMsgCaption = m_strMessageCaption;
	}
	// Otherwise,
	else {
		// Use app window caption
		strMsgCaption = this->GetAppWindowCaption();
	}

	// If message caption is empty (not registered)
	// or the global application window title is not set
	if (strMsgCaption.IsEmpty()) {
		// Use the default AfxMessageBox
		return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
	}
	else {
		// Use the MessageBox function, which we can specify the caption with
		nType |= MB_SYSTEMMODAL;							// Show message box as Top-most
		HWND hMainWnd = GET_HANDLE_MAINWND();				// Get main window handle
		return MessageBox(hMainWnd, lpszPrompt, strMsgCaption, nType);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string ID
//  Arguments:		nPromptID  - ID of prompt message string
//					nCaptionID - ID of message caption string
//					nStyle	   - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::DisplayMessageBox(UINT nPromptID, UINT nCaptionID /* = NULL */, UINT nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = this->GetAppLanguage();

	CString strMsg = GetLanguageString(pAppLang, nPromptID);
	CString strCaption = this->GetAppWindowCaption();
	if (nCaptionID != NULL) {
		// Get language string caption
		CString strTemp = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(strTemp))
			strCaption = strTemp;
	}
	else {
		// Using registered message box caption
		if (!m_strMessageCaption.IsEmpty()) {
			strCaption = m_strMessageCaption;
		}
	}

	// Display message box
	int nResult = DisplayMessageBox(strMsg, strCaption, nStyle);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayMessageBox
//	Description:	Display message box using language string
//  Arguments:		lpszPromptID  - Message string
//					lpszCaptionID - Message caption string
//					nStyle		  - Message box style
//  Return value:	int	- Result of message box
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::DisplayMessageBox(LPCTSTR lpszPrompt, LPCTSTR lpszCaption /* = NULL */, UINT nStyle /* = NULL */)
{
	// Set default style
	if (nStyle == NULL) {
		// Information message with [OK] button
		nStyle = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	CString strCaption(lpszCaption);
	if (strCaption.IsEmpty()) {
		// If application message box caption is registered
		if (!m_strMessageCaption.IsEmpty()) {
			// Use registered message box caption
			strCaption = m_strMessageCaption;
		}
		// Otherwise,
		else {
			// Use application window caption
			strCaption = this->GetAppWindowCaption();
		}
	}
	
	// Display message box
	nStyle |= MB_SYSTEMMODAL;
	return ::MessageBox(this->GetMainWnd()->GetSafeHwnd(), lpszPrompt, strCaption, nStyle);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppEventLog
//	Description:	Initialize application event logging data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::InitAppEventLog(void)
{
	// Initialization
	if (m_pAppEventLog == NULL) {
		m_pAppEventLog = new SLogging(LOGTYPE_APP_EVENT);
	}

	// Check validity after allocating
	if (m_pAppEventLog == NULL) {
		TRACE_ERROR("Error: AppEventLog initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set properties
	m_pAppEventLog->Init();
	m_pAppEventLog->SetWriteMode(LOG_WRITEMODE_ONCALL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppEventLog
//	Description:	Get application event logging pointer
//  Arguments:		None
//  Return value:	SLogging - Application event logging pointer
//
//////////////////////////////////////////////////////////////////////////

SLogging* SWinApp::GetAppEventLog(void)
{
	// Check validity
	VERIFY(m_pAppEventLog != NULL);
	return m_pAppEventLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputEventLog
//	Description:	Output application event log
//  Arguments:		usEvent			- Event ID
//					lpszDescription - Additional description
//					pDetailInfo		- Log detail info (array pointer)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::OutputEventLog(USHORT usEvent, LPCTSTR lpszDescription /* = NULL */, LOGDETAILINFO* pDetailInfo /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemAppEvent;
	logItemAppEvent.usCategory = usEvent;
	logItemAppEvent.stTime = GetCurSysTime();
	logItemAppEvent.dwProcessID = GetCurrentProcessId();
	if (lpszDescription != NULL) {
		// Include event description
		logItemAppEvent.strLogString = lpszDescription;
	}
	if (pDetailInfo != NULL) {
		// Include event detail info data
		for (int nIndex = 0; nIndex < pDetailInfo->GetSize(); nIndex++) {
			logItemAppEvent.AddDetail(pDetailInfo->GetAt(nIndex));
		}
	}

	// Output app event log
	if (SLogging* ptrAppEventLog = GetAppEventLog()) {
		ptrAppEventLog->OutputItem(logItemAppEvent);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppOption
//	Description:	Return option value by ID
//  Arguments:		eAppOptionID - ID of specific option
//					bTemp		 - Temp value or saved value (saved value by default)
//  Return value:	int - Option value
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp /* = FALSE */) const
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetFlagValue
//	Description:	Return flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//  Return value:	int - Flag value
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::GetFlagValue(APPFLAGID eFlagID) const
{
	int nValue = INT_INVALID;

	switch (eFlagID)
	{
	case FLAGID_CHANGE_FLAG:					// Data/setting change flag
		nValue = m_bChangeFlag;
		break;
	case FLAGID_READ_ONLY_MODE:					// Read-only mode
		nValue = m_bReadOnlyMode;
		break;
	case FLAGID_FORCE_CLOSING:					// Force closing by request
		nValue = m_bForceClose;
		break;
	}

	return nValue;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetFlagValue
//	Description:	Update flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//					nValue  - Value to set
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::SetFlagValue(APPFLAGID eFlagID, int nValue)
{
	// Check value validity
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	case FLAGID_CHANGE_FLAG:					// Data/setting change flag
		m_bChangeFlag = nValue;
		break;
	case FLAGID_READ_ONLY_MODE:					// Read-only mode
		m_bReadOnlyMode = nValue;
		break;
	case FLAGID_FORCE_CLOSING:					// Force closing by request
		m_bForceClose = nValue;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetChangeFlagValue
//	Description:	Get/set change flag value
//  Arguments:		bChangeFlag - Change flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SWinApp::GetChangeFlagValue(void) const
{
	return m_bChangeFlag;
}

void SWinApp::SetChangeFlagValue(BOOL bChangeFlag)
{
	m_bChangeFlag = bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckDataChangeState
//	Description:	Check if data changed
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::CheckDataChangeState(void)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckSettingChangeState
//	Description:	Check if settings changed
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL SWinApp::CheckSettingChangeState(void)
{
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetReadOnlyMode
//	Description:	Get/set read-only mode
//  Arguments:		bReadOnly - Read-only mode flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SWinApp::GetReadOnlyMode(void) const
{
	return m_bReadOnlyMode;
}

void SWinApp::SetReadOnlyMode(BOOL bReadOnly)
{
	m_bReadOnlyMode = bReadOnly;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsForceClosingByRequest
//	Description:	Check application force closing flag
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SWinApp::IsForceClosingByRequest(void) const
{
	return m_bForceClose;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestCloseDialog
//	Description:	Request current dialog to close
//  Arguments:		nDialogID  - Dialog ID
//					hDialogWnd - Dialog window handle
//  Return value:	LRESULT (0:Success, else:Failed)
//
//////////////////////////////////////////////////////////////////////////

LRESULT SWinApp::RequestCloseDialog(UINT nDialogID)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}

LRESULT SWinApp::RequestCloseDialog(HWND hDialogWnd)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PostErrorMessage
//	Description:	Post error message to specified window
//  Arguments:		hRcvWnd		- Receive window handle
//					pRcvWnd		- Receive window pointer
//					dwErrorCode	- Error code (as First param)
//					lParam		- Additional param (Second param)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SWinApp::PostErrorMessage(DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	PostMessage(NULL, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}

void SWinApp::PostErrorMessage(HWND hRcvWnd, DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	PostMessage(hRcvWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}

void SWinApp::PostErrorMessage(CWnd* pRcvWnd, DWORD dwErrorCode, LPARAM lParam /* = NULL */)
{
	pRcvWnd->PostMessage(SM_APP_ERROR_MESSAGE, (WPARAM)dwErrorCode, lParam);
}
