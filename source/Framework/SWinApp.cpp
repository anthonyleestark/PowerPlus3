
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

#include "Framework/SWinApp.h"

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
	m_strTemplateName = Constant::String::Empty;
	m_strWindowCaption = Constant::String::Empty;
	m_strMessageCaption = Constant::String::Empty;

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
	m_strWindowCaption = Constant::String::Empty;
	m_strMessageCaption = Constant::String::Empty;

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
		const wchar_t* lpszOldLang = GetLanguageName(m_nCurDispLang);
		const wchar_t* lpszNewLang = GetLanguageName(nCurLanguage);
		strEventDescription.Format(L"%s -> %s", lpszOldLang, lpszNewLang);
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
		strTemp.Format(_T(" %s"), GetProductVersion(bFullVersion).GetString());
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
	CString strCaption = Constant::String::Empty;
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
	m_pAppEventLog->SetWriteMode(WriteOnCall);
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
	logItemAppEvent.SetCategory(usEvent);
	logItemAppEvent.SetTime(GetCurSysTime());
	logItemAppEvent.SetProcessID();
	if (lpszDescription != NULL) {
		// Include event description
		logItemAppEvent.SetLogString(lpszDescription);
	}
	if (pDetailInfo != NULL) {
		// Include event detail info data
		for (int nIndex = 0; nIndex < pDetailInfo->size(); nIndex++) {
			logItemAppEvent.AddDetail(pDetailInfo->at(nIndex));
		}
	}

	// Output app event log
	if (SLogging* ptrAppEventLog = GetAppEventLog()) {
		ptrAppEventLog->OutputItem(logItemAppEvent);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetFlagValue
//	Description:	Return flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//  Return value:	int - Flag value
//
//////////////////////////////////////////////////////////////////////////

int SWinApp::GetFlagValue(AppFlagID eFlagID) const
{
	int nValue = FLAG_OFF;

	switch (eFlagID)
	{
	// Application-base flags
	case AppFlagID::appDataChanged:
	case AppFlagID::appReadOnlyMode:
	case AppFlagID::appForceClosing:
	case AppFlagID::appExitCode:
		nValue = m_flagManager.GetFlagValue(eFlagID);
		break;

	default:
		// Request the flag value from global flag manager
		nValue = GetGlobalFlagManager().GetFlagValue(eFlagID);
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

void SWinApp::SetFlagValue(AppFlagID eFlagID, int nValue)
{
	// Check value validity
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	// Application-base flags
	case AppFlagID::appDataChanged:
	case AppFlagID::appReadOnlyMode:
	case AppFlagID::appForceClosing:
	case AppFlagID::appExitCode:
		m_flagManager.SetFlagValue(eFlagID, nValue);
		break;

	default:
		// Let the global flag manager do its job
		GetGlobalFlagManager().SetFlagValue(eFlagID, nValue);
		break;
	}
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

LRESULT SWinApp::RequestCloseDialog(UINT /*nDialogID*/)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}

LRESULT SWinApp::RequestCloseDialog(HWND /*hDialogWnd*/)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}
