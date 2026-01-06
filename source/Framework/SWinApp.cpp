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
	templateName_ = Constant::String::Empty;
	windowCaption_ = Constant::String::Empty;
	messageCaption_ = Constant::String::Empty;

	// App language function
	appLanguagePtr_ = NULL;
	currentSetLanguage_ = INT_NULL;
	currentDisplayLanguage_ = INT_NULL;

	// Logging pointer
	appEventLogPtr_ = NULL;

	// App special flags
	isReadOnlyMode_ = false;
	changeFlag_ = false;
	isForceClose_ = false;
}

SWinApp::SWinApp(const wchar_t* templateName) : CWinAppEx()
{
	// Title and caption
	templateName_ = templateName;
	windowCaption_ = Constant::String::Empty;
	messageCaption_ = Constant::String::Empty;

	// App language function
	appLanguagePtr_ = NULL;
	currentSetLanguage_ = INT_NULL;
	currentDisplayLanguage_ = INT_NULL;

	// Logging pointer
	appEventLogPtr_ = NULL;

	// App special flags
	isReadOnlyMode_ = false;
	changeFlag_ = false;
	isForceClose_ = false;
}

/**
 * @brief	Destructor
 */
SWinApp::~SWinApp()
{
	// Delete logging pointer
	if (appEventLogPtr_ != NULL) {
		delete appEventLogPtr_;
		appEventLogPtr_ = NULL;
	}

	// Destroy resource ID map
	DESTROY_RESOURCEID_MAP()
}


// SWinApp application message map
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
 * @param	messagePtr - Default
 * @return	None
 */
BOOL SWinApp::PreTranslateMessage(MSG* messagePtr)
{
	// Default
	return CWinAppEx::PreTranslateMessage(messagePtr);
}

/**
 * @brief	Initialize application language package pointer
 * @param	None
 * @return	bool - Result of initialization
 */
bool SWinApp::initAppLanguage(void)
{
	// Update current language setting
	unsigned currentLanguage = getAppLanguageOption();
	currentDisplayLanguage_ = currentLanguage;

	// Load language package
	appLanguagePtr_ = loadLanguageTable(currentLanguage);

	// Check validity after loading
	if (appLanguagePtr_ == NULL) {
		TRACE_ERROR("Error: Language pointer acquiring failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	return true;
}

/**
 * @brief	Reload application language package pointer
 * @param	currentLanguage - Current language ID
 * @return	bool - Result of reloading process
 */
bool SWinApp::reloadAppLanguage(unsigned currentLanguage /* = NULL */)
{
	// If language is not specified
	if (currentLanguage == NULL) {
		// Load current language option
		currentLanguage = getAppLanguageOption();
	}

	// If language changed
	if (currentDisplayLanguage_ != currentLanguage) {
		// Output event log
		const wchar_t* oldLangName = getLanguageName(currentDisplayLanguage_);
		const wchar_t* newLangName = getLanguageName(currentLanguage);
		String eventDescription = StringUtils::stringFormat(L"%s -> %s", oldLangName, newLangName);
		outputEventLog(LOG_EVENT_CHANGE_LANGUAGE, eventDescription);
	}

	// Update current displaying language
	currentDisplayLanguage_ = currentLanguage;

	// Reload language package
	appLanguagePtr_ = loadLanguageTable(currentLanguage);

	// Check validity
	if (appLanguagePtr_ == NULL)
		return false;

	return true;
}

/**
 * @brief	Set application name by resource string ID
 * @param	resourceStringId - Resource string ID
 * @return	true/false
 */
bool SWinApp::setAppName(unsigned resourceStringId)
{
	// Load resource string
	String tempString = StringUtils::loadResourceString(resourceStringId);
	ASSERT(!tempString.isEmpty());
	if (!tempString.isEmpty()) {
		// Set app name
		setAppName(tempString);
	}

	return (!tempString.isEmpty());
}

/**
 * @brief	Set application window common caption
 * @param	windowCaption - Application window caption
 * @return	None
 */
void SWinApp::setAppWindowCaption(const wchar_t* windowCaption, bool showProdVersion /* = false */, bool fullVersion /* = false */)
{
	// Set caption
	windowCaption_ = windowCaption;

	// Show product version
	if (showProdVersion == true) {
		String tempString = StringUtils::stringFormat(_T(" %s"), StringUtils::getProductVersion(fullVersion).getString());
		windowCaption_.append(tempString);
	}
}

/**
 * @brief	Set application window common caption by resource string ID
 * @param	resourceStringId - Resource string ID
 * @return	true/false
 */
bool SWinApp::setAppWindowCaption(unsigned resourceStringId, bool showProdVersion /* = false */, bool fullVersion /* = false */)
{
	// Load resource string
	String tempWindowCaption = StringUtils::loadResourceString(resourceStringId);
	ASSERT(!tempWindowCaption.isEmpty());
	if (!tempWindowCaption.isEmpty()) {
		// Set app window caption 
		setAppWindowCaption(tempWindowCaption, showProdVersion, fullVersion);
	}

	return (!tempWindowCaption.isEmpty());
}

/**
 * @brief	Register default caption for message box-es
 * @param	captionId  - ID of message caption string
 * @param	lpszCaption	- Message caption string
 * @return	None
 */
void SWinApp::registerMessageBoxCaption(unsigned captionId)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = this->getAppLanguage();
	String messageCaption = Constant::String::Empty;
	if (captionId != NULL) {

		// Get language string caption
		String langCaption = getLanguageString(languageTablePtr, captionId);
		if (IS_NOT_NULL_STRING(langCaption)) {
			// Set caption string
			messageCaption = langCaption;
		}
	}

	// If caption is empty
	if (messageCaption.isEmpty()) {
		// Use default app window caption
		messageCaption = this->getAppWindowCaption();
	}

	// Register message box caption
	registerMessageBoxCaption(messageCaption);
}

/**
 * @brief	Override this function to customize application-wide
			processing of AfxMessageBox calls
 * @param	prompt		- Message box text
 * @param	type		- Message box style
 * @param	nIDPrompt	- An index to a Help context string.
 * @return	int	- Result of message box
 */
int SWinApp::DoMessageBox(const wchar_t* prompt, unsigned type, unsigned nIDPrompt)
{
	// Message caption
	String messageCaption;

	// If application message box caption is registered
	if (!messageCaption_.isEmpty()) {
		// Use registered message box caption
		messageCaption = messageCaption_;
	}
	// Otherwise,
	else {
		// Use app window caption
		messageCaption = this->getAppWindowCaption();
	}

	// If message caption is empty (not registered)
	// or the global application window title is not set
	if (messageCaption.isEmpty()) {
		// Use the default AfxMessageBox
		return CWinApp::DoMessageBox(prompt, type, nIDPrompt);
	}
	else {
		// Use the MessageBox function, which we can specify the caption with
		type |= MB_SYSTEMMODAL;							// Show message box as Top-most
		HWND mainWndHandle = GET_HANDLE_MAINWND();			// Get main window handle
		return MessageBox(mainWndHandle, prompt, messageCaption, type);
	}
}

/**
 * @brief	Display message box using language string ID
 * @param	promptId  - ID of prompt message string
 * @param	captionId - ID of message caption string
 * @param	style	   - Message box style
 * @return	int	- Result of message box
 */
int SWinApp::displayMessageBox(unsigned promptId, unsigned captionId /* = NULL */, unsigned style /* = NULL */)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = this->getAppLanguage();

	String messagePrompt = getLanguageString(languageTablePtr, promptId);
	String messageCaption = this->getAppWindowCaption();
	if (captionId != NULL) {
		// Get language string caption
		String langCaption = getLanguageString(languageTablePtr, captionId);
		if (IS_NOT_NULL_STRING(langCaption))
			messageCaption = langCaption;
	}
	else {
		// Using registered message box caption
		if (!messageCaption_.isEmpty()) {
			messageCaption = messageCaption_;
		}
	}

	// Display message box
	int result = displayMessageBox(messagePrompt, messageCaption, style);

	return result;
}

/**
 * @brief	Display message box using language string
 * @param	prompt  - Message string
 * @param	caption - Message caption string
 * @param	style	- Message box style
 * @return	int	- Result of message box
 */
int SWinApp::displayMessageBox(const wchar_t* prompt, const wchar_t* caption /* = NULL */, unsigned style /* = NULL */)
{
	// Set default style
	if (style == NULL) {
		// Information message with [OK] button
		style = MB_OK | MB_ICONINFORMATION;
	}

	// If caption is not set
	String messageCaption(caption);
	if (messageCaption.isEmpty()) {
		// If application message box caption is registered
		if (!messageCaption_.isEmpty()) {
			// Use registered message box caption
			messageCaption = messageCaption_;
		}
		// Otherwise,
		else {
			// Use application window caption
			messageCaption = this->getAppWindowCaption();
		}
	}
	
	// Display message box
	style |= MB_SYSTEMMODAL;
	return ::MessageBox(this->GetMainWnd()->GetSafeHwnd(), prompt, messageCaption, style);
}

/**
 * @brief	Initialize application event logging data
 * @param	None
 * @return	None
 */
void SWinApp::initAppEventLog(void)
{
	// Initialization
	if (appEventLogPtr_ == NULL) {
		appEventLogPtr_ = new Logger(LOGTYPE_APP_EVENT);
	}

	// Check validity after allocating
	if (appEventLogPtr_ == NULL) {
		TRACE_ERROR("Error: AppEventLog initialization failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set properties
	appEventLogPtr_->init();
	appEventLogPtr_->setWriteMode(WriteOnCall);
}

/**
 * @brief	Output application event log
 * @param	eventId		- Event ID
 * @param	description - Additional description
 * @param	pDetailInfo	- Log detail info (array pointer)
 * @return	None
 */
void SWinApp::outputEventLog(uint16 eventId, const wchar_t* description /* = NULL */, LOGDETAILINFO* pDetailInfo /* = NULL */)
{
	// Prepare event log info
	LOGITEM logItemAppEvent;
	logItemAppEvent.setCategory(eventId);
	logItemAppEvent.setTime(DateTimeUtils::getCurrentDateTime());
	logItemAppEvent.setProcessId();
	if (description) {
		// Include event description
		logItemAppEvent.setLogString(description);
	}
	if (pDetailInfo != NULL) {
		// Include event detail info data
		for (int index = 0; index < pDetailInfo->size(); index++) {
			logItemAppEvent.addDetail(pDetailInfo->at(index));
		}
	}

	// Output app event log
	if (Logger* appEventLoggerPtr = getAppEventLog()) {
		appEventLoggerPtr->outputItem(logItemAppEvent);
	}
}

/**
 * @brief	Return flag value by ID
 * @param	flagId - ID of specific flag
 * @return	int - Flag value
 */
int SWinApp::getFlagValue(AppFlagID flagId) const
{
	int value = FLAG_OFF;

	switch (flagId)
	{
	// Application-base flags
	case AppFlagID::appDataChanged:
	case AppFlagID::appReadOnlyMode:
	case AppFlagID::appForceClosing:
	case AppFlagID::appExitCode:
		value = flagManager_.getFlagValue(flagId);
		break;

	default:
		// Request the flag value from global flag manager
		value = getGlobalFlagManager().getFlagValue(flagId);
		break;
	}

	return value;
}

/**
 * @brief	Update flag value by ID
 * @param	flagId - ID of specific flag
 * @param	value  - Value to set
 * @return	None
 */
void SWinApp::setFlagValue(AppFlagID flagId, int value)
{
	// Check value validity
	if (value == INT_INVALID)
		return;

	switch (flagId)
	{
	// Application-base flags
	case AppFlagID::appDataChanged:
	case AppFlagID::appReadOnlyMode:
	case AppFlagID::appForceClosing:
	case AppFlagID::appExitCode:
		flagManager_.setFlagValue(flagId, value);
		break;

	default:
		// Let the global flag manager do its job
		getGlobalFlagManager().setFlagValue(flagId, value);
		break;
	}
}

/**
 * @brief	Request current dialog to close
 * @param	dialogId  - Dialog ID
 * @param	dialogWndHandle - Dialog window handle
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT SWinApp::requestCloseDialog(unsigned /*dialogId*/)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}

LRESULT SWinApp::requestCloseDialog(HWND /*dialogWndHandle*/)
{
	// Request denied
	return LRESULT(1);	// ERROR_FAILED
}
