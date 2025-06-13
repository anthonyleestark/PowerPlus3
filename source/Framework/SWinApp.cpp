/**
 * @file		SWinApp.h
 * @brief		Custom base class for application (source file)
 * @author		AnthonyLeeStark
 * @date		2024.11.19
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "Framework/SWinApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;


// Implement methods for SWinApp
IMPLEMENT_DYNAMIC(SWinApp, CWinAppEx)


/**
 * @brief	Constructor
 */
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

SWinApp::SWinApp(const wchar_t* templateName) : CWinAppEx()
{
	// Title and caption
	m_strTemplateName = templateName;
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

/**
 * @brief	Destructor
 */
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


/**
 * @brief	Default method for application instance initialization
 * @param	None
 * @return	Default
 */
BOOL SWinApp::InitInstance()
{
	return CWinAppEx::InitInstance();
}

/**
 * @brief	Default method for application instance exiting
 * @param	None
 * @return	Default
 */
int SWinApp::ExitInstance()
{
	return CWinAppEx::ExitInstance();
}

/**
 * @brief	Execute some actions before exiting app instance
 * @param	None
 * @return	int
 */
int SWinApp::PreExitInstance()
{
	// TODO: Override this function for custom actions

	return Result::Success;
}

/**
 * @brief	Default method for pre-translate message
 * @param	pMsg - Default
 * @return	None
 */
BOOL SWinApp::PreTranslateMessage(MSG* pMsg)
{
	// Default
	return CWinAppEx::PreTranslateMessage(pMsg);
}

/**
 * @brief	Initialize application language package pointer
 * @param	None
 * @return	BOOL - Result of initialization
 */
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

/**
 * @brief	Reload application language package pointer
 * @param	nCurLanguage - Current language ID
 * @return	BOOL - Result of reloading process
 */
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
		const wchar_t* oldLangName = GetLanguageName(m_nCurDispLang);
		const wchar_t* newLangName = GetLanguageName(nCurLanguage);
		String eventDescription = StringUtils::StringFormat(L"%s -> %s", oldLangName, newLangName);
		OutputEventLog(LOG_EVENT_CHANGE_LANGUAGE, eventDescription);
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

/**
 * @brief	Set application name by resource string ID
 * @param	nResourceStringID - Resource string ID
 * @return	TRUE/FALSE
 */
BOOL SWinApp::SetAppName(UINT nResourceStringID)
{
	// Load resource string
	String tempString = StringUtils::LoadResourceString(nResourceStringID);
	ASSERT(!tempString.IsEmpty());
	if (!tempString.IsEmpty()) {
		// Set app name
		SetAppName(tempString);
	}

	return (!tempString.IsEmpty());
}

/**
 * @brief	Set application window common caption
 * @param	windowCaption - Application window caption
 * @return	None
 */
void SWinApp::SetAppWindowCaption(const wchar_t* windowCaption, BOOL bShowProdVersion /* = FALSE */, BOOL bFullVersion /* = FALSE */)
{
	// Set caption
	m_strWindowCaption = windowCaption;

	// Show product version
	if (bShowProdVersion == TRUE) {
		String tempString = StringUtils::StringFormat(_T(" %s"), StringUtils::GetProductVersion(bFullVersion).GetString());
		m_strWindowCaption.Append(tempString);
	}
}

/**
 * @brief	Set application window common caption by resource string ID
 * @param	nResourceStringID - Resource string ID
 * @return	TRUE/FALSE
 */
BOOL SWinApp::SetAppWindowCaption(UINT nResourceStringID, BOOL bShowProdVersion /* = FALSE */, BOOL bFullVersion /* = FALSE */)
{
	// Load resource string
	String tempWindowCaption = StringUtils::LoadResourceString(nResourceStringID);
	ASSERT(!tempWindowCaption.IsEmpty());
	if (!tempWindowCaption.IsEmpty()) {
		// Set app window caption 
		SetAppWindowCaption(tempWindowCaption, bShowProdVersion, bFullVersion);
	}

	return (!tempWindowCaption.IsEmpty());
}

/**
 * @brief	Register default caption for message box-es
 * @param	nCaptionID  - ID of message caption string
 * @param	lpszCaption	- Message caption string
 * @return	None
 */
void SWinApp::RegisterMessageBoxCaption(UINT nCaptionID)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = this->GetAppLanguage();
	String messageCaption = Constant::String::Empty;
	if (nCaptionID != NULL) {

		// Get language string caption
		String langCaption = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(langCaption)) {
			// Set caption string
			messageCaption = langCaption;
		}
	}

	// If caption is empty
	if (messageCaption.IsEmpty()) {
		// Use default app window caption
		messageCaption = this->GetAppWindowCaption();
	}

	// Register message box caption
	RegisterMessageBoxCaption(messageCaption);
}

/**
 * @brief	Override this function to customize application-wide
					processing of AfxMessageBox calls
 * @param	prompt		- Message box text
 * @param	nType		- Message box style
 * @param	nIDPrompt	- An index to a Help context string.
 * @return	int	- Result of message box
 */
int SWinApp::DoMessageBox(const wchar_t* prompt, UINT nType, UINT nIDPrompt)
{
	// Message caption
	String messageCaption;

	// If application message box caption is registered
	if (!m_strMessageCaption.IsEmpty()) {
		// Use registered message box caption
		messageCaption = m_strMessageCaption;
	}
	// Otherwise,
	else {
		// Use app window caption
		messageCaption = this->GetAppWindowCaption();
	}

	// If message caption is empty (not registered)
	// or the global application window title is not set
	if (messageCaption.IsEmpty()) {
		// Use the default AfxMessageBox
		return CWinApp::DoMessageBox(prompt, nType, nIDPrompt);
	}
	else {
		// Use the MessageBox function, which we can specify the caption with
		nType |= MB_SYSTEMMODAL;							// Show message box as Top-most
		HWND hMainWnd = GET_HANDLE_MAINWND();				// Get main window handle
		return MessageBox(hMainWnd, prompt, messageCaption, nType);
	}
}

/**
 * @brief	Display message box using language string ID
 * @param	nPromptID  - ID of prompt message string
 * @param	nCaptionID - ID of message caption string
 * @param	nStyle	   - Message box style
 * @return	int	- Result of message box
 */
int SWinApp::DisplayMessageBox(UINT nPromptID, UINT nCaptionID /* = NULL */, UINT nStyle /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = this->GetAppLanguage();

	String messagePrompt = GetLanguageString(pAppLang, nPromptID);
	String messageCaption = this->GetAppWindowCaption();
	if (nCaptionID != NULL) {
		// Get language string caption
		String langCaption = GetLanguageString(pAppLang, nCaptionID);
		if (IS_NOT_NULL_STRING(langCaption))
			messageCaption = langCaption;
	}
	else {
		// Using registered message box caption
		if (!m_strMessageCaption.IsEmpty()) {
			messageCaption = m_strMessageCaption;
		}
	}

	// Display message box
	int nResult = DisplayMessageBox(messagePrompt, messageCaption, nStyle);

	return nResult;
}

/**
 * @brief	Display message box using language string
 * @param	prompt  - Message string
 * @param	caption - Message caption string
 * @param	nStyle	- Message box style
 * @return	int	- Result of message box
 */
int SWinApp::DisplayMessageBox(const wchar_t* prompt, const wchar_t* caption /* = NULL */, UINT nStyle /* = NULL */)
{
	// Set default style
	if (nStyle == NULL) {
		// Information message with [OK] button
		nStyle = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	String messageCaption(caption);
	if (messageCaption.IsEmpty()) {
		// If application message box caption is registered
		if (!m_strMessageCaption.IsEmpty()) {
			// Use registered message box caption
			messageCaption = m_strMessageCaption;
		}
		// Otherwise,
		else {
			// Use application window caption
			messageCaption = this->GetAppWindowCaption();
		}
	}
	
	// Display message box
	nStyle |= MB_SYSTEMMODAL;
	return ::MessageBox(this->GetMainWnd()->GetSafeHwnd(), prompt, messageCaption, nStyle);
}

/**
 * @brief	Initialize application event logging data
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Output application event log
 * @param	usEvent		- Event ID
 * @param	description - Additional description
 * @param	pDetailInfo	- Log detail info (array pointer)
 * @return	None
 */
void SWinApp::OutputEventLog(USHORT usEvent, const wchar_t* description /* = NULL */, LOGDETAILINFO* pDetailInfo /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemAppEvent;
	logItemAppEvent.SetCategory(usEvent);
	logItemAppEvent.SetTime(DateTimeUtils::GetCurrentDateTime());
	logItemAppEvent.SetProcessID();
	if (description) {
		// Include event description
		logItemAppEvent.SetLogString(description);
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

/**
 * @brief	Return flag value by ID
 * @param	eFlagID - ID of specific flag
 * @return	int - Flag value
 */
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

/**
 * @brief	Update flag value by ID
 * @param	eFlagID - ID of specific flag
 * @param	nValue  - Value to set
 * @return	None
 */
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

/**
 * @brief	Request current dialog to close
 * @param	nDialogID  - Dialog ID
 * @param	hDialogWnd - Dialog window handle
 * @return	LRESULT (0:Success, else:Failed)
 */
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
