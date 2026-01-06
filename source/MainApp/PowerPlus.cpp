/**
 * @file		PowerPlus.cpp
 * @brief		Source file for app class
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/DebugTestDlg.h"
#include "Dialogs/DebugTestV2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Language;
using namespace AppCore;
using namespace AppRegistry;


// Application message map
BEGIN_MESSAGE_MAP(CPowerPlusApp, SWinApp)
	ON_COMMAND(ID_HELP,	&SWinApp::OnHelp)
END_MESSAGE_MAP()


/**
 * @brief	Constructor
 */
CPowerPlusApp::CPowerPlusApp() : SWinApp()
{
	// RestartManagerSupport
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// Init application data pointers
	appConfigDataPtr_ = NULL;
	scheduleDataPtr_ = NULL;
	hotkeySetDataPtr_ = NULL;
	reminderDataPtr_ = NULL;

	// Init logging pointers
	appHistoryLogPtr_ = NULL;

	// Hook procedure handle
	appKeyboardHookHandler_ = NULL;

	// Init DebugTest dialog
	debugTestDlgPtr_ = NULL;
}

/**
 * @brief	Destructor
 */
CPowerPlusApp::~CPowerPlusApp()
{
	// Delete app data pointers
	if (appConfigDataPtr_ != NULL) {
		delete appConfigDataPtr_;
		appConfigDataPtr_ = NULL;
	}

	if (scheduleDataPtr_ != NULL) {
		scheduleDataPtr_->deleteAll();
		delete scheduleDataPtr_;
		scheduleDataPtr_ = NULL;
	}

	if (hotkeySetDataPtr_ != NULL) {
		hotkeySetDataPtr_->deleteAll();
		delete hotkeySetDataPtr_;
		hotkeySetDataPtr_ = NULL;
	}

	if (reminderDataPtr_ != NULL) {
		reminderDataPtr_->deleteAll();
		delete reminderDataPtr_;
		reminderDataPtr_ = NULL;
	}

	// Delete log data pointers
	if (appHistoryLogPtr_ != NULL) {
		delete appHistoryLogPtr_;
		appHistoryLogPtr_ = NULL;
	}

	// Destroy DebugTest dialog
	destroyDebugTestDlg();
}


// The unique global application object
CPowerPlusApp theApp;


//////////////////////////////////////////////////////////////////////////
// Instance functions

/**
 * @brief	Initialize app instance
 * @param	None
 * @return	BOOL - Default
 */
BOOL CPowerPlusApp::InitInstance()
{
	DWORD errorCode;

	// Set application launch time
	setAppLaunchTime(DateTimeUtils::getCurrentDateTime());

	// Set application window caption (with product version number)
	if (!setAppWindowCaption(IDS_APP_WINDOW_CAPTION, true)) {

		// Set title string failed
		TRACE_ERROR("Error: Set app window caption failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		postErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return false;
	}

	// Check if there is any other instance currently running
	// If yes, bring that instance to top and exit current instance
	if (HWND previousWndHandle = FindWindow(NULL, getAppWindowCaption())) {
		PostMessage(previousWndHandle, SM_WND_SHOWDIALOG, true, (LPARAM)0);
		BringWindowToTop(previousWndHandle);
		SetForegroundWindow(previousWndHandle);
		return false;
	}

	// Enable support Visual Styles
#ifdef _SUPPORT_VISUAL_STYLES
	// Init common controls
	InitCommonControls();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles. Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
#endif

	// Init instance
	if (!SWinApp::InitInstance()) {

		// Trace log
		TRACE_ERROR("Error: Init instance failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		postErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return false;
	}

	// Initialize DebugTest dialog
	initDebugTestDlg();

	// Check CTRL key press state and open DebugTest dialog
	if (IS_PRESSED(VK_CONTROL)) {
		HWND debugTestDlgHandle = debugTestDlgPtr_->GetSafeHwnd();
		PostMessage(debugTestDlgHandle, SM_WND_SHOWDIALOG, true, NULL);
	}

	// Setup registry key info
	SetRegistryKey(AppProfile::CompanyName);

	// Update application profile info data
	updateAppLaunchTimeProfileInfo();

	// Create neccessary sub-folders
	String subDirectory = StringUtils::getSubFolderPath(Constant::Folder::Log);
	CreateDirectory(subDirectory, NULL);

	// Setup low-level keyboard hook
	appKeyboardHookHandler_ = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	// Initialize app data
	if (!initAppData()) {

		// Trace log
		TRACE_ERROR("Error: Init app data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		postErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return false;
	}
	
	// Initialize default data
	appConfigDataPtr_->setDefaultData();
	scheduleDataPtr_->setDefaultData();
	hotkeySetDataPtr_->setDefaultData();
	reminderDataPtr_->setDefaultData();

	// Load registry app data
	if (!loadRegistryAppData()) {
		// Trace log
		TRACE_ERROR("Error: Load registry app data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}

	// Initialize application language
	setAppLanguageOption(getAppOption(AppOptionID::languageID));
	if (!initAppLanguage()) {

		// Trace log
		TRACE_ERROR("Error: Init app language failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		postErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return false;
	}
	
	// Initialize log objects
	initAppEventLog();
	initAppHistoryLog();

	// Output event log: InitInstance
	outputEventLog(LOG_EVENT_INIT_INSTANCE);

	// Register to system wakeup event notifications
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS paramDevNotifySubs;
	paramDevNotifySubs.Callback = DeviceNotifyCallbackRoutine;
	paramDevNotifySubs.Context = NULL;

	HPOWERNOTIFY hPowerNotify = NULL;
	errorCode = PowerRegisterSuspendResumeNotification(DEVICE_NOTIFY_CALLBACK, &paramDevNotifySubs, &hPowerNotify);

	if (errorCode != ERROR_SUCCESS) {
		// Handle error and show message
		TRACE_FORMAT("Error: Power event notification register failed!!! (Code: 0x%08X)", errorCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		postErrorMessage(errorCode);
	}

	/************************************************************************************/
	/*																					*/
	/*							BEGIN MAIN DIALOG MODAL LOOP							*/
	/*																					*/
	/************************************************************************************/

	// Initialize main dialog
	CPowerPlusDlg* mainDialogPtr = new CPowerPlusDlg;
	if (mainDialogPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Main dialog pointer allocation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		postErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return false;
	}

	// Set application main window pointer
	m_pMainWnd = mainDialogPtr;

	// Show/hide main dialog at startup
	if (getAppOption(AppOptionID::showDialogAtStartup) == false) {

		// Hide dialog
		mainDialogPtr->Create(IDD_POWERPLUS_DIALOG, NULL);
		mainDialogPtr->ShowWindow(SW_HIDE);

		// Set parent window for DebugTest dialog if available
		if (debugTestDlgPtr_ != NULL) {
			debugTestDlgPtr_->setParent(mainDialogPtr);
		}

		// Notification sound
		MessageBeep(0xFFFFFFFF);

		// Run modal loop
		mainDialogPtr->RunModalLoop();
	}
	else {

		// Set parent window for DebugTest dialog if available
		if (debugTestDlgPtr_ != NULL) {
			debugTestDlgPtr_->setParent(mainDialogPtr);
		}

		// Show dialog in modal state
		mainDialogPtr->DoModal();

		// Bring dialog window to top
		mainDialogPtr->SendMessage(SM_WND_SHOWDIALOG, true, NULL);
		mainDialogPtr->BringWindowToTop();
		mainDialogPtr->SetForegroundWindow();
	}

	/************************************************************************************/
	/*																					*/
	/*							 END MAIN DIALOG MODAL LOOP								*/
	/*																					*/
	/************************************************************************************/

	// Unregister to system wakeup event notifications
	if (hPowerNotify != NULL) {
		errorCode = PowerUnregisterSuspendResumeNotification(hPowerNotify);
		if (errorCode != ERROR_SUCCESS) {
			// Handle error and show message
			TRACE_FORMAT("Error: Power event notification unregister failed!!! (Code: 0x%08X)", errorCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			postErrorMessage(errorCode);
		}
	}

	// Destroy DebugTest dialog
	destroyDebugTestDlg();

	// Delete the main dialog pointer
	if (mainDialogPtr != NULL) {
		delete mainDialogPtr;
		mainDialogPtr = NULL;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	return false;
}

/**
 * @brief	Exit app instance
 * @param	None
 * @return	int - Default
 */
int CPowerPlusApp::ExitInstance()
{
	// Output event log: ExitInstance
	outputEventLog(LOG_EVENT_EXIT_INSTANCE);

	// Write application event logging data to file if enabled
	if (getAppOption(AppOptionID::saveAppEventLog) == true) {
		getAppEventLog()->write();
	}

	// Write action history logging data to file if enabled
	if (getAppOption(AppOptionID::saveActionHistory) == true) {
		getAppHistoryLog()->write();
	}

	// Close DebugTest dialog
	destroyDebugTestDlg();

	// Find if the DebugTest dialog is still running
	HWND debugTestDlgHandle = AppCore::findDebugTestDlg();
	if (debugTestDlgHandle != NULL) {
		// Destroy dialog
		::DestroyWindow(debugTestDlgHandle);
	}

	// Unhook keyboard
	UnhookWindowsHookEx(appKeyboardHookHandler_);

	// Mark as safe-termination
	setSafeTerminationFlag(FLAG_ON);
	saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);

	// Default
	return SWinApp::ExitInstance();
}

/**
 * @brief	Low-level keyboard hook process
 * @param	Default
 * @return	LRESULT - Default
 */
LRESULT WINAPI CPowerPlusApp::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Get low-level keyboard hook info param
	PKBDLLHOOKSTRUCT kbHookStructPtr = (PKBDLLHOOKSTRUCT)lParam;
	if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && nCode == HC_ACTION) {
		DWORD keyCode = kbHookStructPtr->vkCode;		// Get keycode
		DWORD keyFlags = kbHookStructPtr->flags;		// Get keyflags

		/*********************************************************************/
		/*																	 */
		/*			  Keyboard process in screen unlock state			     */
		/*																	 */
		/*********************************************************************/

		if (getSessionLockFlag() == FLAG_OFF) {
			// Process when Alt & Backspace keys are pressed
			if ((keyCode == VK_BACK) && (keyFlags & LLKHF_ALTDOWN)) {

				// Keystroke pressed: "Alt + Win + Backspace"
				if (IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)) {
					// Show DebugTest dialog
					PostMessage(GET_HANDLE_MAINWND(), SM_WND_DEBUGTEST, NULL, NULL);
				}
				// Keystroke pressed: "Alt + Shift + Backspace"
				else if (IS_PRESSED(VK_LSHIFT) || IS_PRESSED(VK_RSHIFT)) {
					// Show main dialog window
					PostMessage(GET_HANDLE_MAINWND(), SM_WND_SHOWDIALOG, true, NULL);
				}
			}
		}

		/*********************************************************************/
		/*																	 */
		/*				Keyboard process in screen lock state			     */
		/*																	 */
		/*********************************************************************/

		else if (getSessionLockFlag() == FLAG_ON) {
			// Process hotkey when the screen is locked
			if (((keyCode >= VK_F1) && (keyCode <= VK_F12)) &&					// Only process if a function key (F1 -> F12) and
				((keyFlags & LLKHF_ALTDOWN) ||									// either at least one of these control keys: Alt key (left or right)
				(IS_PRESSED(VK_LCONTROL) || IS_PRESSED(VK_RCONTROL)) ||				// or Ctrl key (left or right)
				(IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)))) {					// or Windows key (left or right) is pressed

				// Get app pointer
				CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();

				// Only process if both options are enabled
				if ((theAppPtr != NULL) &&
					(theAppPtr->getAppOption(AppOptionID::enableBackgroundHotkey) == true) &&		// Enable background action hotkeys
					(theAppPtr->getAppOption(AppOptionID::allowLockscreenHotkey) == true)) {		// Allow background hotkeys on lockscreen

					// Keycode param
					DWORD hotkeyParam = NULL;
					{
						// Modifier keys
						WORD modifiers = NULL;
						modifiers |= ((IS_PRESSED(VK_LCONTROL) || IS_PRESSED(VK_RCONTROL)) ? MOD_CONTROL : NULL);		// Is Ctrl key pressed???
						modifiers |= ((keyFlags & LLKHF_ALTDOWN) ? MOD_ALT : 0);										// Is Alt key pressed???
						modifiers |= ((IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)) ? MOD_WIN : NULL);					// Is Windows key pressed???

						// Virtual key code
						WORD virtualKey = keyCode;

						// Make keycode param (combine modifiers and virtual key code)
						hotkeyParam = MAKELONG(modifiers, virtualKey);

						// Output debug log
						outputDebugLogFormat(_T("Lockstate Hotkey pressed: Modifiers=0x%04X, VirtualKey=0x%04X"), modifiers, virtualKey);
					}

					// Post message to main dialog window
					PostMessage(GET_HANDLE_MAINWND(), SM_APP_LOCKSTATE_HOTKEY, hotkeyParam, NULL);
				}
			}
		}
	}

	// Call next hook
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

/**
 * @brief	Device notify callback routine (callback function)
 * @param	Default (see MSDN "DEVICE_NOTIFY_CALLBACK_ROUTINE")
 * @return	ULONG - Default
 */
ULONG CPowerPlusApp::DeviceNotifyCallbackRoutine(PVOID /*pContext*/, ULONG ulType, PVOID /*pSetting*/)
{
	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) 
		return ULONG(Result::Failure);

	// Get current date/time
	DateTime curDateTime = DateTimeUtils::getCurrentDateTime();

	// Processing
	switch (ulType)
	{
	case PBT_APMSUSPEND:					// System suspend event
		// Save last system suspend time
		theAppPtr->saveLastSysEventTime(SystemEventID::SystemSuspend, curDateTime);
		break;

	case PBT_APMRESUMESUSPEND:				// System resume from suspend event
	case PBT_APMRESUMEAUTOMATIC:			// System automatic resume event
		// Save last system wakeup time
		theAppPtr->saveLastSysEventTime(SystemEventID::SystemWakeUp, curDateTime);
		break;

	default:
		break;
	}

	// Default return
	return ULONG(Result::Success);		// ERROR_SUCCESS
}

/**
 * @brief	Process app message filter
 * @param	code - Default
 * @param	msg - Default
 * @return	BOOL - Default
 */
BOOL CPowerPlusApp::ProcessMessageFilter(int nCode, LPMSG lpMsg)
{
	// Default process
	if (SWinApp::ProcessMessageFilter(nCode, lpMsg))
		return true;

	// Pre-translate message if there's no window handler
	if (lpMsg->hwnd == NULL)
		return PreTranslateMessage(lpMsg);

	return false;
}

/**
 * @brief	Pre-translate app messages
 * @param	messagePtr - Default
 * @return	BOOL - Default
 */
BOOL CPowerPlusApp::PreTranslateMessage(MSG* messagePtr)
{
	// "Show error message" message
	if (messagePtr->message == SM_APP_ERROR_MESSAGE) {
		// Get window handle and error code
		HWND receivedWndHandle = messagePtr->hwnd;
		DWORD errorCode = (DWORD)(messagePtr->wParam);

		// If the message window handle is invalid (HWND is NULL), 
		// and the main window has not been initialized, or the app language has not been loaded,
		// handle message and show error messagebox here
		if (((receivedWndHandle == NULL) && (this->GetMainWnd() == NULL)) || (this->getAppLanguage() == NULL)) {
			AppCore::showErrorMessage(NULL, NULL, errorCode);
			return true;
		}

		// If the main window is available, 
		// let it handle the error message on its own
		else if (this->GetMainWnd() != NULL) {
			HWND mainWndHandle = GET_HANDLE_MAINWND();
			SendMessage(mainWndHandle, messagePtr->message, messagePtr->wParam, messagePtr->lParam);
			return true;
		}
	}

	// "Error message showed" message
	else if (messagePtr->message == SM_APP_SHOW_ERROR_MSG) {
		// Only process if the message window handle is invalid (HWND is NULL)
		HWND receivedWndHandle = messagePtr->hwnd;
		if (receivedWndHandle == NULL) {
			OnShowErrorMessage(messagePtr->wParam, messagePtr->lParam);
			return true;
		}
	}

	// "Debug command execution" message
	else if (messagePtr->message == SM_APP_DEBUGCMD_EXEC) {
		// Only process if the message window handle is invalid (HWND is NULL)
		HWND receivedWndHandle = messagePtr->hwnd;
		if (receivedWndHandle == NULL) {
			OnExecuteDebugCommand(messagePtr->wParam, messagePtr->lParam);
			return true;
		}
	}

	// Default
	return SWinApp::PreTranslateMessage(messagePtr);
}


//////////////////////////////////////////////////////////////////////////
// App data serialization functions

/**
 * @brief	Initialize app data
 * @param	None
 * @return	bool - Result of initialization
 */
bool CPowerPlusApp::initAppData()
{
	// Initialize app config data
	if (appConfigDataPtr_ == NULL) {
		appConfigDataPtr_ = new ConfigData;
	}
	// Initialize schedule data
	if (scheduleDataPtr_ == NULL) {
		scheduleDataPtr_ = new ScheduleData;
	}
	// Initialize HotkeySet data
	if (hotkeySetDataPtr_ == NULL) {
		hotkeySetDataPtr_ = new HotkeySetData;
	}
	// Initialize Power Reminder data
	if (reminderDataPtr_ == NULL) {
		reminderDataPtr_ = new PwrReminderData;
	}

	// Check data validity
	bool result = true;

	// Check app config data
	if (appConfigDataPtr_ == NULL) {
		result = false;
		TRACE_ERROR("Error: App config data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check schedule data
	if (scheduleDataPtr_ == NULL) {
		result = false;
		TRACE_ERROR("Error: Schedule data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check HotkeySet data
	if (hotkeySetDataPtr_ == NULL) {
		result = false;
		TRACE_ERROR("Error: HotkeySet data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check Power Reminder data
	if (reminderDataPtr_ == NULL) {
		result = false;
		TRACE_ERROR("Error: Power Reminder data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}

	return result;
}

/**
 * @brief	Load app data from registry
 * @param	None
 * @return	bool - Result of loading process
 */
bool CPowerPlusApp::loadRegistryAppData()
{
	bool result = true;
	bool finalResult = true;
	WORD loadingResult = APP_ERROR_SUCCESS;

	int tempDate = INT_INVALID;
	int tempTime = INT_INVALID;
	String tempString = Constant::String::Empty;
	ClockTime clockTimeTemp;

	// Check data validity first
	if (!dataSerializeCheck(Mode::Load))
		return false;

	// Create temporary data
	ConfigData* tempConfigDataPtr = new ConfigData;
	ScheduleData* tempScheduleDataPtr = new ScheduleData;
	HotkeySetData* tempHotkeyDataPtr = new HotkeySetData;
	PwrReminderData* tempReminderDataPtr = new PwrReminderData;


	/***********************************************************************************************/
	/*																							   */
	/*									 Load configuration info								   */
	/*																							   */
	/***********************************************************************************************/

	if (tempConfigDataPtr != NULL) {

		// Read configuration data
		int configResult = INT_NULL;
		configResult += getConfig(Key::ConfigData::LMBAction,				(int&)tempConfigDataPtr->leftMouseAction);
		configResult += getConfig(Key::ConfigData::MMBAction,				(int&)tempConfigDataPtr->middleMouseAction);
		configResult += getConfig(Key::ConfigData::RMBAction,				(int&)tempConfigDataPtr->rightMouseAction);
		configResult += getConfig(Key::ConfigData::RMBShowMenu,				tempConfigDataPtr->rightMouseShowMenu);
		configResult += getConfig(Key::ConfigData::LanguageID,				(int&)tempConfigDataPtr->languageID);
		configResult += getConfig(Key::ConfigData::ShowDlgAtStartup,		tempConfigDataPtr->showDialogAtStartup);
		configResult += getConfig(Key::ConfigData::StartupEnabled,			tempConfigDataPtr->enableAutoStart);
		configResult += getConfig(Key::ConfigData::ConfirmAction,			tempConfigDataPtr->actionConfirmation);
		configResult += getConfig(Key::ConfigData::SaveHistoryLog,			tempConfigDataPtr->saveActionHistory);
		configResult += getConfig(Key::ConfigData::SaveAppEventLog,			tempConfigDataPtr->saveAppEventLog);
		configResult += getConfig(Key::ConfigData::RunAsAdmin,				tempConfigDataPtr->runAsAdmin);
		configResult += getConfig(Key::ConfigData::ShowErrorMsg,			tempConfigDataPtr->showErrorMessage);
		configResult += getConfig(Key::ConfigData::NotifySchedule,			tempConfigDataPtr->scheduleNotification);
		configResult += getConfig(Key::ConfigData::AllowCancelSchedule,		tempConfigDataPtr->allowScheduleCancellation);
		configResult += getConfig(Key::ConfigData::EnableBackgroundHotkey,	tempConfigDataPtr->enableBackgroundHotkey);
		configResult += getConfig(Key::ConfigData::LockStateHotkey,			tempConfigDataPtr->allowLockscreenHotkey);
		configResult += getConfig(Key::ConfigData::EnablePowerReminder,		tempConfigDataPtr->enablePowerReminder);

		// Mark data as reading failed
		// only if all values were read unsuccessfully
		result = (configResult != INT_NULL);
	}

	// Trace error
	if (result == false) {
		loadingResult = APP_ERROR_LOAD_CFG_FAILED;
		traceSerializeData(loadingResult);
		finalResult = false;	// Set final result
		result = true;			// Reset flag
	}
	else {
		// Copy temporary data
		if (tempConfigDataPtr != NULL) {
			appConfigDataPtr_->copy(*tempConfigDataPtr);
			result = true;		// Reset flag
		}
	}

	// Delete temporary data
	if (tempConfigDataPtr != NULL) {
		delete tempConfigDataPtr;
		tempConfigDataPtr = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*										Load schedule info									   */
	/*																							   */
	/***********************************************************************************************/

	if (tempScheduleDataPtr != NULL) {

		// Initialize temp data
		tempScheduleDataPtr->init();

		// Initialize default item
		ScheduleItem tempDefaultItem(ScheduleData::kDefaultItemID);
		{
			// Read default schedule item
			int defaultScheduleResult = INT_NULL;

			// Enable state
			defaultScheduleResult += getDefaultSchedule(Key::ScheduleItem::IsEnabled, tempDate);
			tempDefaultItem.enableItem(tempDate);

			// Action ID
			defaultScheduleResult += getDefaultSchedule(Key::ScheduleItem::ActionID, tempDate);
			tempDefaultItem.setAction(tempDate);

			// Repeat enable state
			defaultScheduleResult += getDefaultSchedule(Key::PwrRepeatSet::IsRepeated, tempDate);
			tempDefaultItem.enableRepeat(tempDate);

			// Repeat days
			defaultScheduleResult += getDefaultSchedule(Key::PwrRepeatSet::RepeatDays, tempDate);
			tempDefaultItem.setActiveDays(BYTE(tempDate));

			// Time value
			defaultScheduleResult += getDefaultSchedule(Key::ScheduleItem::Time, tempTime);
			if (tempTime != INT_INVALID) {

				// Convert time value and set time
				clockTimeTemp.setHour(GET_REGTIME_HOUR(tempTime));
				clockTimeTemp.setMinute(GET_REGTIME_MINUTE(tempTime));
				tempDefaultItem.setTime(clockTimeTemp);

				// Reset temp data
				clockTimeTemp = ClockTime();
				tempTime = INT_INVALID;
			}

			// Mark data as reading failed
			// only if all values were read unsuccessfully
			result = (defaultScheduleResult != INT_NULL);

			// Trace error
			if (result == false) {
				loadingResult = APP_ERROR_LOAD_SCHED_FAILED;
				traceSerializeData(loadingResult);
				finalResult = false;	// Set final result
				result = true;			// Reset flag
			}
			else {
				// Update default item data
				ScheduleItem& defaultScheduleItem = tempScheduleDataPtr->getDefaultItem();
				defaultScheduleItem.copy(tempDefaultItem);
			}
		}

		// Load number of extra items
		int extraItemNum = 0;
		result &= getScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, extraItemNum);
		if (result != false) {

			// Read each extra item data
			for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {

				// Initialize temp item
				ScheduleItem tempExtraItem;

				// Read extra item
				int scheduleItemResult = INT_NULL;

				// Enable state
				scheduleItemResult += getScheduleExtra(extraIndex, Key::ScheduleItem::IsEnabled, tempDate);
				tempExtraItem.enableItem(tempDate);

				// Item ID
				scheduleItemResult += getScheduleExtra(extraIndex, Key::ScheduleItem::ItemID, tempDate);
				tempExtraItem.setItemId(tempDate);

				// Action ID
				scheduleItemResult += getScheduleExtra(extraIndex, Key::ScheduleItem::ActionID, tempDate);
				tempExtraItem.setAction(tempDate);

				// Repeat enable state
				scheduleItemResult += getScheduleExtra(extraIndex, Key::PwrRepeatSet::IsRepeated, tempDate);
				tempExtraItem.enableRepeat(tempDate);

				// Repeat days
				scheduleItemResult += getScheduleExtra(extraIndex, Key::PwrRepeatSet::RepeatDays, tempDate);
				tempExtraItem.setActiveDays(BYTE(tempDate));

				// Time value
				scheduleItemResult += getScheduleExtra(extraIndex, Key::ScheduleItem::Time, tempTime);
				if (tempTime != INT_INVALID) {

					// Convert time value and set time
					clockTimeTemp.setHour(GET_REGTIME_HOUR(tempTime));
					clockTimeTemp.setMinute(GET_REGTIME_MINUTE(tempTime));
					tempExtraItem.setTime(clockTimeTemp);

					// Reset temp data
					clockTimeTemp = ClockTime();
					tempTime = INT_INVALID;
				}

				// Mark data as reading failed
				// only if all values were read unsuccessfully
				result = (scheduleItemResult != INT_NULL);

				// Trace error
				if (result == false) {
					loadingResult = APP_ERROR_LOAD_SCHED_FAILED;
					traceSerializeData(loadingResult);
					finalResult = false;	// Set final result
					result = true;			// Reset flag
				}

				// Update item data
				tempScheduleDataPtr->update(tempExtraItem);
			}
		}
	}

	// Trace error
	if (result == false) {
		loadingResult = APP_ERROR_LOAD_SCHED_FAILED;
		traceSerializeData(loadingResult);
		finalResult = false;	// Set final result
		result = true;			// Reset flag
	}
	else {
		// Copy temporary data
		if (tempScheduleDataPtr != NULL) {
			scheduleDataPtr_->copy(*tempScheduleDataPtr);
			result = true;		// Reset flag
		}
	}

	// Delete temporary data
	if (tempScheduleDataPtr != NULL) {
		delete tempScheduleDataPtr;
		tempScheduleDataPtr = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*									  Load HotkeySet info								       */
	/*																							   */
	/***********************************************************************************************/

	// Load HotkeySet data
	int itemNum = 0;
	if (tempHotkeyDataPtr != NULL) {

		// Copy data
		tempHotkeyDataPtr->copy(*hotkeySetDataPtr_);

		// Load number of items
		result &= getHotkeyItemNum(Key::HotkeySetData::ItemNum, itemNum);
		if (itemNum > tempHotkeyDataPtr->getItemNum()) {
			// Limit the hotkeyset data item number
			itemNum = tempHotkeyDataPtr->getItemNum();
		}

		for (int index = 0; index < itemNum; index++) {

			// Initialize temp item
			HotkeySetItem tempHotkeyItem;
			ZeroMemory(&tempHotkeyItem, sizeof(HotkeySetItem));

			// Read item data
			int itemResult = INT_NULL;

			// Enable state
			itemResult += getHotkeySet(index, Key::HotkeySetItem::IsEnabled, tempDate);
			tempHotkeyItem.enableItem(tempDate);

			// Action ID
			itemResult += getHotkeySet(index, Key::HotkeySetItem::HKActionID, tempDate);
			tempHotkeyItem.setActionId(tempDate);

			// Keycode
			int nModifiersTemp, nVirtKeyTemp;
			itemResult += getHotkeySet(index, Key::HotkeySetItem::Modifiers, nModifiersTemp);
			itemResult += getHotkeySet(index, Key::HotkeySetItem::VirtualKey, nVirtKeyTemp);
			tempHotkeyItem.setKeyCode(nModifiersTemp, nVirtKeyTemp);

			// Mark the item as reading failed
			// only if all values were read unsuccessfully
			result = (itemResult != INT_NULL);

			// Trace error
			if (result == false) {
				loadingResult = APP_ERROR_LOAD_HKEYSET_FAILED;
				traceSerializeData(loadingResult);
				finalResult = false;	// Set final result
				result = true;			// Reset flag
				continue;
			}

			// Update item data
			tempHotkeyDataPtr->update(tempHotkeyItem);
		}
	}

	// Trace error
	if (result == false) {
		loadingResult = APP_ERROR_LOAD_HKEYSET_FAILED;
		traceSerializeData(loadingResult);
		finalResult = false;	// Set final result
		result = true;			// Reset flag
	}
	else {
		// Copy temporary data
		if (tempHotkeyDataPtr != NULL) {
			hotkeySetDataPtr_->copy(*tempHotkeyDataPtr);
			result = true;		// Reset flag
		}
	}

	// Delete temporary data
	if (tempHotkeyDataPtr != NULL) {
		tempHotkeyDataPtr->deleteAll();
		delete tempHotkeyDataPtr;
		tempHotkeyDataPtr = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Load Power Reminder info								   */
	/*																							   */
	/***********************************************************************************************/

	// Load Power Reminder data
	itemNum = 0;
	if (tempReminderDataPtr != NULL) {
		result &= getPwrReminderItemNum(Key::PwrReminderData::ItemNum, itemNum);
		if (result != false) {

			// Initialize temp data
			tempReminderDataPtr->init();

			// Initialize Power Reminder common style data
			RmdMsgStyleSet rmdCommonStyleTemp;
			{
				// Read Power Reminder common style data
				int nPwrRmdCommonStyleRet = INT_NULL;

				// Background color
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::BkgrdColor, tempDate);
				rmdCommonStyleTemp.setBkgrdColor((COLORREF)tempDate);

				// Text color
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::TextColor, tempDate);
				rmdCommonStyleTemp.setTextColor((COLORREF)tempDate);

				// Font name
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontName, tempString);
				rmdCommonStyleTemp.setFontName(tempString);

				// Font size
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontSize, tempDate);
				rmdCommonStyleTemp.setFontSize(tempDate);

				// Timeout (auto-close) interval
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::Timeout, tempDate);
				rmdCommonStyleTemp.setTimeout(tempDate);

				// Message icon ID
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconID, tempDate);
				rmdCommonStyleTemp.setIconId(tempDate);

				// Message icon size
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconSize, tempDate);
				rmdCommonStyleTemp.setIconSize(tempDate);

				// Message icon position
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconPosition, tempDate);
				rmdCommonStyleTemp.setIconPosition(tempDate);

				// Message display position
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::DisplayPosition, tempDate);
				rmdCommonStyleTemp.setDisplayPosition(tempDate);

				// Display area horizontal margin
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::HorizontalMargin, tempDate);
				rmdCommonStyleTemp.setHorizontalMargin(tempDate);

				// Display area vertical margin
				nPwrRmdCommonStyleRet += getPwrReminderCommonStyle(Key::PwrReminderMsgStyle::VerticalMargin, tempDate);
				rmdCommonStyleTemp.setVerticalMargin(tempDate);

				// Mark data as reading failed
				// only if all values were read unsuccessfully
				result = (nPwrRmdCommonStyleRet != INT_NULL);

				// Trace error
				if (result == false) {
					loadingResult = APP_ERROR_LOAD_PWRRMD_FAILED;
					traceSerializeData(loadingResult);
					finalResult = false;	// Set final result
					result = true;			// Reset flag
				}
				else {
					// Update Power Reminder common style data
					RmdMsgStyleSet& rmdCommonStyle = tempReminderDataPtr->getCommonStyle();
					rmdCommonStyle.copy(rmdCommonStyleTemp);
				}
			}

			// Read each item data
			for (int index = 0; index < itemNum; index++) {

				// Initialize temp item
				PwrReminderItem tempItem;

				// Read item data
				int itemResult = INT_NULL;

				// Item ID
				itemResult += getPwrReminder(index, Key::PwrReminderItem::ItemID, tempDate);
				tempItem.setItemId(tempDate);

				// Enable state
				itemResult += getPwrReminder(index, Key::PwrReminderItem::IsEnabled, tempDate);
				tempItem.enableItem(tempDate);

				// Message content
				itemResult += getPwrReminder(index, Key::PwrReminderItem::Message, tempString);
				tempItem.setMessage(tempString);

				// Event ID
				itemResult += getPwrReminder(index, Key::PwrReminderItem::EventID, tempDate);
				tempItem.setEventId(tempDate);

				// Message style
				itemResult += getPwrReminder(index, Key::PwrReminderItem::MsgStyle, tempDate);
				tempItem.setMessageStyle(tempDate);

				// Repeat enable state
				itemResult += getPwrReminder(index, Key::PwrRepeatSet::IsRepeated, tempDate);
				tempItem.enableRepeat(tempDate);

				// Allow snoozing
				itemResult += getPwrReminder(index, Key::PwrRepeatSet::AllowSnooze, tempDate);
				tempItem.enableSnoozing(tempDate);

				// Snooze interval
				itemResult += getPwrReminder(index, Key::PwrRepeatSet::SnoozeInterval, tempDate);
				tempItem.setSnoozeInterval(tempDate);

				// Repeat days
				itemResult += getPwrReminder(index, Key::PwrRepeatSet::RepeatDays, tempDate);
				tempItem.setActiveDays(tempDate);

				// Time value
				itemResult += getPwrReminder(index, Key::PwrReminderItem::Time, tempTime);
				if (tempTime != INT_INVALID) {

					// Convert time value and set time
					clockTimeTemp.setHour(GET_REGTIME_HOUR(tempTime));
					clockTimeTemp.setMinute(GET_REGTIME_MINUTE(tempTime));
					tempItem.setTime(clockTimeTemp);

					// Reset temp data
					clockTimeTemp = ClockTime();
					tempTime = INT_INVALID;
				}

				// Mark the item as reading failed
				// only if all values were read unsuccessfully
				result = (itemResult != INT_NULL);

				// Trace error
				if (result == false) {
					loadingResult = APP_ERROR_LOAD_PWRRMD_FAILED;
					traceSerializeData(loadingResult);
					finalResult = false;	// Set final result
					result = true;			// Reset flag
					continue;
				}

				// Update item data
				tempReminderDataPtr->update(tempItem);
			}
		}
	}

	// Trace error
	if (result == false) {
		loadingResult = APP_ERROR_LOAD_PWRRMD_FAILED;
		traceSerializeData(loadingResult);
		finalResult = false;	// Set final result
		result = true;			// Reset flag
	}
	else {
		// Copy temporary data
		if (tempReminderDataPtr != NULL) {
			reminderDataPtr_->copy(*tempReminderDataPtr);
			reminderDataPtr_->adjust();
			result = true;		// Reset flag
		}
	}

	// Delete temporary data
	if (tempReminderDataPtr != NULL) {
		tempReminderDataPtr->deleteAll();
		delete tempReminderDataPtr;
		tempReminderDataPtr = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*										Load other data										   */
	/*																							   */
	/***********************************************************************************************/

	// Load global data values
	loadGlobalData();
	
	return finalResult;
}

/**
 * @brief	Save app data to registry
 * @param	dataType - App data type to save
 * @return	bool - Result of saving process
 */
bool CPowerPlusApp::saveRegistryAppData(DWORD dataType /* = APPDATA_ALL */)
{
	bool result = true;
	bool finalResult = true;
	WORD savingResult = APP_ERROR_SUCCESS;
	int tempTime = INT_INVALID;

	// Check data validity first
	if (!dataSerializeCheck(Mode::Save, dataType))
		return false;

	/***********************************************************************************************/
	/*																							   */
	/*									Save configuration info									   */
	/*																							   */
	/***********************************************************************************************/

	// Save configuration data
	if ((dataType & APPDATA_CONFIG) != 0) {

		// Delete old data before writing
		deleteConfigSection();

		// Get a copy of config data
		ConfigData tempConfigData{};
		if (appConfigDataPtr_ != NULL) {
			tempConfigData.copy(*appConfigDataPtr_);
		}

		// Save registry data
		result &= writeConfig(Key::ConfigData::LMBAction,				tempConfigData.leftMouseAction);
		result &= writeConfig(Key::ConfigData::MMBAction,				tempConfigData.middleMouseAction);
		result &= writeConfig(Key::ConfigData::RMBAction,				tempConfigData.rightMouseAction);
		result &= writeConfig(Key::ConfigData::RMBShowMenu,				tempConfigData.rightMouseShowMenu);
		result &= writeConfig(Key::ConfigData::LanguageID,				tempConfigData.languageID);
		result &= writeConfig(Key::ConfigData::ShowDlgAtStartup,		tempConfigData.showDialogAtStartup);
		result &= writeConfig(Key::ConfigData::StartupEnabled,			tempConfigData.enableAutoStart);
		result &= writeConfig(Key::ConfigData::ConfirmAction,			tempConfigData.actionConfirmation);
		result &= writeConfig(Key::ConfigData::SaveHistoryLog,			tempConfigData.saveActionHistory);
		result &= writeConfig(Key::ConfigData::SaveAppEventLog,			tempConfigData.saveAppEventLog);
		result &= writeConfig(Key::ConfigData::RunAsAdmin,				tempConfigData.runAsAdmin);
		result &= writeConfig(Key::ConfigData::ShowErrorMsg,			tempConfigData.showErrorMessage);
		result &= writeConfig(Key::ConfigData::NotifySchedule,			tempConfigData.scheduleNotification);
		result &= writeConfig(Key::ConfigData::AllowCancelSchedule,		tempConfigData.allowScheduleCancellation);
		result &= writeConfig(Key::ConfigData::EnableBackgroundHotkey,	tempConfigData.enableBackgroundHotkey);
		result &= writeConfig(Key::ConfigData::LockStateHotkey,			tempConfigData.allowLockscreenHotkey);
		result &= writeConfig(Key::ConfigData::EnablePowerReminder,		tempConfigData.enablePowerReminder);

		// Trace error
		if (result == false) {
			savingResult = APP_ERROR_SAVE_CFG_FAILED;
			traceSerializeData(savingResult);
			finalResult = false; // Set final result
			result = true; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Save schedule info										   */
	/*																							   */
	/***********************************************************************************************/

	// Save schedule data
	if ((dataType & APPDATA_SCHEDULE) != 0) {

		// Delete old data before writing
		deleteScheduleSection();

		// Save default schedule item
		ScheduleItem tempDefaultItem = scheduleDataPtr_->getDefaultItem();
		{
			// Convert time data
			tempTime = FORMAT_REG_TIME(tempDefaultItem.getTime());

			// Save registry data
			result &= writeDefaultSchedule(Key::ScheduleItem::IsEnabled,	tempDefaultItem.isEnabled());
			result &= writeDefaultSchedule(Key::ScheduleItem::ActionID,		tempDefaultItem.getAction());
			result &= writeDefaultSchedule(Key::PwrRepeatSet::IsRepeated,	tempDefaultItem.isRepeatEnabled());
			result &= writeDefaultSchedule(Key::PwrRepeatSet::RepeatDays,	tempDefaultItem.getActiveDays());
			result &= writeDefaultSchedule(Key::ScheduleItem::Time,			tempTime);

			// Trace error
			if (result == false) {
				savingResult = APP_ERROR_SAVE_SCHED_FAILED;
				traceSerializeData(savingResult);
				finalResult = false; // Set final result
				result = true; // Reset flag
			}
		}

		// Save schedule extra data
		int extraItemNum = scheduleDataPtr_->getExtraItemNum();
		result &= writeScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, extraItemNum);
		for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {

			// Get schedule extra item
			ScheduleItem tempExtraItem = scheduleDataPtr_->getItemAt(extraIndex);

			// Convert time data
			tempTime = FORMAT_REG_TIME(tempExtraItem.getTime());

			// Save registry data
			result &= writeScheduleExtra(extraIndex, Key::ScheduleItem::IsEnabled,	tempExtraItem.isEnabled());
			result &= writeScheduleExtra(extraIndex, Key::ScheduleItem::ItemID,		tempExtraItem.getItemId());
			result &= writeScheduleExtra(extraIndex, Key::ScheduleItem::ActionID,	tempExtraItem.getAction());
			result &= writeScheduleExtra(extraIndex, Key::PwrRepeatSet::IsRepeated,	tempExtraItem.isRepeatEnabled());
			result &= writeScheduleExtra(extraIndex, Key::PwrRepeatSet::RepeatDays,	tempExtraItem.getActiveDays());
			result &= writeScheduleExtra(extraIndex, Key::ScheduleItem::Time,		tempTime);

			// Trace error
			if (result == false) {
				savingResult = APP_ERROR_SAVE_SCHED_FAILED;
				traceSerializeData(savingResult);
				finalResult = false; // Set final result
				result = true; // Reset flag
			}
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*								 Save auto-start status info								   */
	/*																							   */
	/***********************************************************************************************/

	// Save auto-start status info
	if ((dataType & APPDATA_CONFIG) != 0) {

		bool isAutoStartEnabled = appConfigDataPtr_->enableAutoStart;
		bool isRunAsAdmin = appConfigDataPtr_->runAsAdmin;
		int autoStartEnabled = enableAutoStart(isAutoStartEnabled, isRunAsAdmin);
		result &= autoStartEnabled ? true : false;

		// Trace error
		if (result == false) {
			savingResult = APP_ERROR_SAVE_CFG_FAILED;
			traceSerializeData(savingResult);
			finalResult = false; // Set final result
			result = true; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Save HotkeySet info									   */
	/*																							   */
	/***********************************************************************************************/

	// Save HotkeySet data
	if ((dataType & APPDATA_HOTKEYSET) != 0) {

		// Delete old data before writing
		deleteHotkeySetSection();

		// Save registry data
		int itemNum = hotkeySetDataPtr_->getItemNum();
		result &= writeHotkeyItemNum(Key::HotkeySetData::ItemNum, itemNum);
		for (int index = 0; index < itemNum; index++) {

			// Get HotkeySet item
			HotkeySetItem tempHotkeyItem = hotkeySetDataPtr_->getItemAt(index);

			// Get keycode
			DWORD tempModifiers, tempVirtualKey;
			tempHotkeyItem.getKeyCode(tempModifiers, tempVirtualKey);

			// Write item data
			result &= writeHotkeySet(index, Key::HotkeySetItem::IsEnabled,	tempHotkeyItem.isEnabled());
			result &= writeHotkeySet(index, Key::HotkeySetItem::HKActionID,	tempHotkeyItem.getActionId());
			result &= writeHotkeySet(index, Key::HotkeySetItem::Modifiers,	tempModifiers);
			result &= writeHotkeySet(index, Key::HotkeySetItem::VirtualKey,	tempVirtualKey);
		}

		// Trace error
		if (result == false) {
			savingResult = APP_ERROR_SAVE_HKEYSET_FAILED;
			traceSerializeData(savingResult);
			finalResult = false; // Set final result
			result = true; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									Save Power Reminder info								   */
	/*																							   */
	/***********************************************************************************************/

	// Save Power Reminder data
	if ((dataType & APPDATA_PWRREMINDER) != 0) {

		// Delete old data before writing
		deletePwrReminderSection();

		// Save Power Reminder common style data
		RmdMsgStyleSet& tempCommonStyle = reminderDataPtr_->getCommonStyle();
		{
			// Save registry data
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::BkgrdColor,		tempCommonStyle.getBkgrdColor());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::TextColor,			tempCommonStyle.getTextColor());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontName,			tempCommonStyle.getFontName());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontSize,			tempCommonStyle.getFontSize());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::Timeout,			tempCommonStyle.getTimeout());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconID,			tempCommonStyle.getIconId());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconSize,	 		tempCommonStyle.getIconSize());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconPosition,		tempCommonStyle.getIconPosition());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::DisplayPosition,	tempCommonStyle.getDisplayPosition());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::HorizontalMargin,	tempCommonStyle.getHorizontalMargin());
			result &= writePwrReminderCommonStyle(Key::PwrReminderMsgStyle::VerticalMargin,	tempCommonStyle.getVerticalMargin());

			// Trace error
			if (result == false) {
				savingResult = APP_ERROR_SAVE_PWRRMD_FAILED;
				traceSerializeData(savingResult);
				finalResult = false; // Set final result
				result = true; // Reset flag
			}
		}

		// Save registry data
		int itemNum = reminderDataPtr_->getItemNum();
		result &= writePwrReminderItemNum(Key::PwrReminderData::ItemNum, itemNum);
		for (int index = 0; index < itemNum; index++) {

			// Get Power Reminder item
			PwrReminderItem tempItem = reminderDataPtr_->getItemAt(index);

			// Convert time data
			tempTime = FORMAT_REG_TIME(tempItem.getTime());

			// Write item data
			result &= writePwrReminder(index, Key::PwrReminderItem::ItemID,		tempItem.getItemId());
			result &= writePwrReminder(index, Key::PwrReminderItem::IsEnabled,	tempItem.isEnabled());
			result &= writePwrReminder(index, Key::PwrReminderItem::Message,		tempItem.getMessage());
			result &= writePwrReminder(index, Key::PwrReminderItem::EventID,		tempItem.getEventId());
			result &= writePwrReminder(index, Key::PwrReminderItem::Time,			tempTime);
			result &= writePwrReminder(index, Key::PwrReminderItem::MsgStyle,		tempItem.getMessageStyle());
			result &= writePwrReminder(index, Key::PwrRepeatSet::IsRepeated,		tempItem.isRepeatEnabled());
			result &= writePwrReminder(index, Key::PwrRepeatSet::AllowSnooze,		tempItem.isAllowSnoozing());
			result &= writePwrReminder(index, Key::PwrRepeatSet::SnoozeInterval,	tempItem.getSnoozeInterval());
			result &= writePwrReminder(index, Key::PwrRepeatSet::RepeatDays,		tempItem.getActiveDays());
		}

		// Trace error
		if (result == false) {
			savingResult = APP_ERROR_SAVE_PWRRMD_FAILED;
			traceSerializeData(savingResult);
			finalResult = false; // Set final result
			result = true; // Reset flag
		}
	}

	return finalResult;
}

/**
 * @brief	Backup app data to file
 * @param	None
 * @return	bool - Result of backing up process
 */
bool CPowerPlusApp::backupRegistryAppData()
{
	// Auto backup using Registry Export
	return BackupSystem::RegistryExport();
}

/**
 * @brief	Load and update application launch-time profile info data
 * @param	None
 * @return	bool - Result of loading process
 */
bool CPowerPlusApp::updateAppLaunchTimeProfileInfo(void)
{
	bool returnFlag = false;

	int value = (int)0;								// Integer type
	unsigned valueUnsigned = (unsigned)0;			// Unsigned integer value
	String valueString = Constant::String::Empty;	// String value

	/*------------------------<Application launch-time counter>--------------------------*/

	// Load info from registry
	if (getProfileInfo(AppProfile::LaunchInfo::LaunchCounter, value)) {
		setAppLaunchTimeCounter(value);
		returnFlag = true;
	}

	// Update and overwrite data
	updateAppLaunchTimeCounter();
	valueUnsigned = getAppLaunchTimeCounter();
	if (!writeProfileInfo(AppProfile::LaunchInfo::LaunchCounter, valueUnsigned)) {
		returnFlag = false;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Application launch-time>-----------------------------*/

	// Format launch-time
	DateTime dateTimeAppLaunch = getAppLaunchTime();
	unsigned timePeriod = (dateTimeAppLaunch.hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = getLanguageString(loadLanguageTable(NULL), timePeriod);
	const wchar_t* timeFormatString = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	valueString = StringUtils::stringFormat(timeFormatString, dateTimeAppLaunch.year(), dateTimeAppLaunch.month(), dateTimeAppLaunch.day(),
		dateTimeAppLaunch.hour(), dateTimeAppLaunch.minute(), dateTimeAppLaunch.second(), dateTimeAppLaunch.millisecond(), timePeriodFormat);

	// Store launch-time info data
	if (!writeProfileInfo(AppProfile::LaunchInfo::LaunchTime, valueString)) {
		returnFlag = false;
	}

	/*-----------------------------------------------------------------------------------*/

	/*------------------------<Application directory/file info>--------------------------*/

	// Directory path (not including the executable file name)
	valueString = StringUtils::getApplicationPath(false);
	if (!valueString.isEmpty()) {
		if (!writeProfileInfo(AppProfile::LaunchInfo::Directory, valueString)) {
			returnFlag = false;
		}
	}

	// Executable file name
	valueString = StringUtils::getApplicationPath(true);
	if (!valueString.isEmpty()) {
		String execFileName = PathFindFileName(valueString);
		if (!writeProfileInfo(AppProfile::LaunchInfo::FileName, execFileName)) {
			returnFlag = false;
		}
	}

	// Product version (full version)
	valueString = StringUtils::getProductVersion(true);
	if (!valueString.isEmpty()) {
		if (!writeProfileInfo(AppProfile::LaunchInfo::ProductVersion, valueString)) {
			returnFlag = false;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	/*--------------------------<Device, system and user info>---------------------------*/

	// Device name
	bool resultGetInfo = StringUtils::getDeviceName(valueString);
	if ((resultGetInfo != false) && (!valueString.isEmpty())) {
		if (!writeProfileInfo(AppProfile::LaunchInfo::DeviceName, valueString)) {
			returnFlag = false;
		}
	}

	// User name
	resultGetInfo = StringUtils::getCurrentUserName(valueString);
	if ((resultGetInfo != false) && (!valueString.isEmpty())) {
		if (!writeProfileInfo(AppProfile::LaunchInfo::UserName, valueString)) {
			returnFlag = false;
		}
	}

	// Operating system info
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
		// Operating system version
		if (!writeProfileInfo(AppProfile::LaunchInfo::OSVersion, osvi.dwMajorVersion)) {
			returnFlag = false;
		}
		// Build number
		if (!writeProfileInfo(AppProfile::LaunchInfo::OSBuildNumber, osvi.dwBuildNumber)) {
			returnFlag = false;
		}
		// Platform ID
		if (!writeProfileInfo(AppProfile::LaunchInfo::OSPlatformID, osvi.dwPlatformId)) {
			returnFlag = false;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	return returnFlag;
}

/**
 * @brief	Load global data values from registry
 * @param	None
 * @return	bool - Result of loading process
 */
bool CPowerPlusApp::loadGlobalData(void)
{
	bool returnFlag = false;

	int globalValue = (int)0;									// Integer type

	// Subsection name
	String subSectionName = Constant::String::Empty;

	/*------------------------<Load debugging/testing variables>-------------------------*/

	// Subsection: DebugTest
	subSectionName = Section::GlobalData::DebugTest;

	// DummyTest mode
	if (getGlobalData(subSectionName, Key::GlobalData::DebugTest::DummyTest, globalValue)) {
		setDummyTestMode(globalValue);
		returnFlag |= true;
	}
	// Debug mode
	if (getGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugMode, globalValue)) {
		setDebugMode(globalValue);
		returnFlag |= true;
	}
	// Debug log output target
	if (getGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugOutput, globalValue)) {
		setDebugOutputTarget(globalValue);
		returnFlag |= true;
	}
	// Test feature enable
	if (getGlobalData(subSectionName, Key::GlobalData::DebugTest::TestFeature, globalValue)) {
		setTestFeatureEnable(globalValue);
		returnFlag |= true;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Load app special flags>------------------------------*/

	// Subsection: AppFlags
	subSectionName = Section::GlobalData::AppFlag;

	// Power action trace flag
	if (getGlobalData(subSectionName, Key::GlobalData::AppFlag::PwrActionFlag, globalValue)) {
		setPwrActionFlag((BYTE)globalValue);
		returnFlag |= true;
	}

	// System suspended trace flag
	if (getGlobalData(subSectionName, Key::GlobalData::AppFlag::SystemSuspendFlag, globalValue)) {
		setSystemSuspendFlag((BYTE)globalValue);
		returnFlag |= true;
	}

	// Session ending trace flag
	if (getGlobalData(subSectionName, Key::GlobalData::AppFlag::SessionEndFlag, globalValue)) {
		setSessionEndFlag((BYTE)globalValue);
		returnFlag |= true;
	}

	// Previously safe termination trace flag
	if (getGlobalData(subSectionName, Key::GlobalData::AppFlag::SafeTermination, globalValue)) {
		setSafeTerminationFlag((BYTE)globalValue);
		returnFlag |= true;
	}

	/*-----------------------------------------------------------------------------------*/

	return returnFlag;
}

/**
 * @brief	Save global data values to registry
 * @param	categoryId - Category ID
 * @return	bool - Result of loading process
 */
bool CPowerPlusApp::saveGlobalData(BYTE categoryId /* = 0xFF */)
{
	bool returnFlag = true;

	int globalValue = (int)0;									// Integer type
	BYTE globalValueByte = (BYTE)0;								// Byte value
	String globalValueString = Constant::String::Empty;			// String value

	// Subsection name
	String subSectionName = Constant::String::Empty;

	/*------------------------<Save debugging/testing variables>-------------------------*/

	if ((categoryId == 0xFF) || (categoryId == DEF_GLBDATA_CATE_DEBUGTEST)) {
		// Subsection: DebugTest
		subSectionName = Section::GlobalData::DebugTest;

		// DummyTest mode
		globalValue = getDummyTestMode();
		if (!writeGlobalData(subSectionName, Key::GlobalData::DebugTest::DummyTest, globalValue)) {
			returnFlag = false;
		}
		// Debug mode
		globalValue = getDebugMode();
		if (!writeGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugMode, globalValue)) {
			returnFlag = false;
		}
		// Debug log output target
		globalValue = getDebugOutputTarget();
		if (!writeGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugOutput, globalValue)) {
			returnFlag = false;
		}
		// Test feature enable
		globalValue = getTestFeatureEnable();
		if (!writeGlobalData(subSectionName, Key::GlobalData::DebugTest::TestFeature, globalValue)) {
			returnFlag = false;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	/*---------------------------------<Save app flags>----------------------------------*/

	if ((categoryId == 0xFF) || (categoryId == DEF_GLBDATA_CATE_APPFLAGS)) {
		// Subsection: AppFlags
		subSectionName = Section::GlobalData::AppFlag;

		// Power action trace flag
		globalValueByte = getPwrActionFlag();
		if (!writeGlobalData(subSectionName, Key::GlobalData::AppFlag::PwrActionFlag, globalValueByte)) {
			returnFlag = false;
		}

		// System suspended trace flag
		globalValueByte = getSystemSuspendFlag();
		if (!writeGlobalData(subSectionName, Key::GlobalData::AppFlag::SystemSuspendFlag, globalValueByte)) {
			returnFlag = false;
		}

		// Session ending trace flag
		globalValueByte = getSessionEndFlag();
		if (!writeGlobalData(subSectionName, Key::GlobalData::AppFlag::SessionEndFlag, globalValueByte)) {
			returnFlag = false;
		}

		// Previously safe termination trace flag
		globalValueByte = getSafeTerminationFlag();
		if (!writeGlobalData(subSectionName, Key::GlobalData::AppFlag::SafeTermination, globalValueByte)) {
			returnFlag = false;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	return returnFlag;
}

//////////////////////////////////////////////////////////////////////////
// App data processing functions

/**
 * @brief	Get app configuration data
 * @param	None
 * @return	ConfigData* - Config data pointer
 */
ConfigData* CPowerPlusApp::getAppConfigData()
{
	// Check validity
	ASSERT(appConfigDataPtr_ != NULL);
	return appConfigDataPtr_;
}

/**
 * @brief	Set app configuration data
 * @param	data - Config data pointer
 * @return	None
 */
void CPowerPlusApp::setAppConfigData(ConfigData* data)
{
	// Check data validity
	ASSERT(data != NULL);
	if (data == NULL) return;

	// Backup data
	ConfigData dataBackup;
	dataBackup.copy(*getAppConfigData());

	// Copy value of data pointer
	getAppConfigData()->copy(*data);
}

/**
 * @brief	Get app schedule data
 * @param	None
 * @return	ScheduleData* - Schedule data pointer
 */
ScheduleData* CPowerPlusApp::getAppScheduleData()
{
	// Check validity
	ASSERT(scheduleDataPtr_ != NULL);
	return scheduleDataPtr_;
}

/**
 * @brief	Set app schedule data
 * @param	data - Schedule data pointer
 * @return	None
 */
void CPowerPlusApp::setAppScheduleData(ScheduleData* data)
{
	// Check data validity
	ASSERT(data != NULL);
	if (data == NULL) return;

	// Backup data
	ScheduleData dataBackup;
	dataBackup.copy(*getAppScheduleData());

	// Copy value of data pointer
	getAppScheduleData()->copy(*data);
}

/**
 * @brief	Get app hotkeyset data
 * @param	None
 * @return	HotkeySetData* - HotkeySet data pointer
 */
HotkeySetData* CPowerPlusApp::getAppHotkeySetData()
{
	// Check validity
	ASSERT(hotkeySetDataPtr_ != NULL);
	return hotkeySetDataPtr_;
}

/**
 * @brief	Set app hotkeyset data
 * @param	data - HotkeySet data pointer
 * @return	None
 */
void CPowerPlusApp::setAppHotkeySetData(HotkeySetData* data)
{
	// Check data validity
	ASSERT(data != NULL);
	if (data == NULL) return;

	// Backup data
	HotkeySetData dataBackup;
	dataBackup.copy(*getAppHotkeySetData());

	// Copy value of data pointer
	getAppHotkeySetData()->copy(*data);
}

/**
 * @brief	Get app Power Reminder data
 * @param	None
 * @return	PwrReminderData* - Power Reminder data pointer
 */
PwrReminderData* CPowerPlusApp::getAppPwrReminderData()
{
	// Check validity
	ASSERT(reminderDataPtr_ != NULL);
	return reminderDataPtr_;
}

/**
 * @brief	Set app Power Reminder data
 * @param	data - Power Reminder data pointer
 * @return	None
 */
void CPowerPlusApp::setAppPwrReminderData(PwrReminderData* data)
{
	// Check data validity
	ASSERT(data != NULL);
	if (data == NULL) return;

	// Backup data
	PwrReminderData dataBackup;
	dataBackup.copy(*getAppPwrReminderData());

	// Copy value of data pointer
	getAppPwrReminderData()->copy(*data);
}


//////////////////////////////////////////////////////////////////////////
// Data options and flags get/set functions

/**
 * @brief	Return option value by ID
 * @param	optionId - ID of specific option
 * @return	int - Option value
 */
int CPowerPlusApp::getAppOption(AppOptionID optionId) const
{
	int result = INT_INVALID;

	switch (optionId)
	{
	case AppOptionID::curDispLanguage:
		result = SWinApp::getAppLanguageOption(true);
		break;
	case AppOptionID::defaultScheduleActiveState:
		result = scheduleDataPtr_->getDefaultItem().isEnabled();
		break;
	case AppOptionID::defaultScheduleActionID:
		result = scheduleDataPtr_->getDefaultItem().getAction();
		break;
	case AppOptionID::defaultScheduleRepeat:
		result = scheduleDataPtr_->getDefaultItem().isRepeatEnabled();
		break;
	default:
		result = appConfigDataPtr_->getAppOption(optionId);
		break;
	}

	return result;
}

/**
 * @brief	Initialize action history log data
 * @param	None
 * @return	None
 */
void CPowerPlusApp::initAppHistoryLog()
{
	// Initialization
	if (appHistoryLogPtr_ == NULL) {
		appHistoryLogPtr_ = new Logger(LOGTYPE_HISTORY_LOG);
	}
	
	// Check validity after allocating
	if (appHistoryLogPtr_ == NULL) {
		TRACE_ERROR("Error: AppHistoryLog initialization failed");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set properties
	appHistoryLogPtr_->init();
	appHistoryLogPtr_->setWriteMode(WriteInstantly);
}

/**
 * @brief	Get app action history log pointer
 * @param	None
 * @return	Logger - Action history log pointer
 */
Logger* CPowerPlusApp::getAppHistoryLog()
{
	// Check validity
	VERIFY(appHistoryLogPtr_ != NULL);
	return appHistoryLogPtr_;
}

/**
 * @brief	Output a log item to action history log
 * @param	logItem - Log item data
 * @return	None
 */
void CPowerPlusApp::outputAppHistoryLog(LOGITEM logItem)
{
	// Get app history logging pointer
	Logger* appHistoryLoggerPtr = getAppHistoryLog();
	
	// Only output log if option is ON
	if ((appHistoryLoggerPtr != NULL) && (getAppOption(AppOptionID::saveActionHistory) != false)) {
		appHistoryLoggerPtr->outputItem(logItem);
	}
}

/**
 * @brief	Output trace log of data serialization
 * @param	errorCode - Error code
 * @return	None
 */
void CPowerPlusApp::traceSerializeData(WORD errorCode)
{
	String traceMessageTitle = Constant::String::Empty;
	String traceLogFormat = Constant::String::Empty;
	const wchar_t* _dataNullString = _T("Data pointer is NULL");
	const wchar_t* _readFailedString = _T("Registry data is unreadable or invalid");
	const wchar_t* _writeFailedString = _T("Unable to write registry data");

	// Serialization trace skip flag
	// Note: If the application is launching for the 1st time, do not trace data loading error
	bool skipFlag = false;
	
	switch (errorCode)
	{
	case APP_ERROR_LOAD_CFG_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Load config failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_CFG_FAILED:
		skipFlag = isAppFirstLaunch();
		traceMessageTitle = _T("Load config failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_SCHED_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Load schedule failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_SCHED_FAILED:
		skipFlag = isAppFirstLaunch();
		traceMessageTitle = _T("Load schedule failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_HKEYSET_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Load hotkeyset failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_HKEYSET_FAILED:
		skipFlag = isAppFirstLaunch();
		traceMessageTitle = _T("Load hotkeyset failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_PWRRMD_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Load reminder failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_PWRRMD_FAILED:
		skipFlag = isAppFirstLaunch();
		traceMessageTitle = _T("Load reminder failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _readFailedString);
		break;

	case APP_ERROR_SAVE_CFG_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save config failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_CFG_FAILED:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save config failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_SCHED_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save schedule failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_SCHED_FAILED:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save schedule failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_HKEYSET_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save hotkeyset failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_HKEYSET_FAILED:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save hotkeyset failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_PWRRMD_INVALID:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save reminder failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_PWRRMD_FAILED:
		skipFlag = false;							// Do not skip
		traceMessageTitle = _T("Save reminder failed");
		traceLogFormat.format(_T("%s: %s"), traceMessageTitle.getString(), _writeFailedString);
		break;
	}

	// If skip flag is triggered
	if (skipFlag != false)
		return;

	// Output trace error log
	if (!traceLogFormat.isEmpty()) {
		TRACE_ERROR(traceLogFormat);
	}

	// Show error message
	unsigned message = SM_APP_ERROR_MESSAGE;
	WPARAM wParam = (WPARAM)errorCode;
	CWnd* mainWndPtr = this->GetMainWnd();
	if (mainWndPtr != NULL) {
		// Post message to main window
		// Message will be handled by main window message map handler
		HWND windowHandle = mainWndPtr->GetSafeHwnd();
		PostMessage(windowHandle, message, wParam, NULL);
	}
	else {
		// Post message without passing window handler
		// Message will be handled by ProcessMessageFilter() and PreTranslateMessage()
		PostMessage(NULL, message, wParam, NULL);
	}
}

/**
 * @brief	Check validity of data for serialization
 * @param	serializeMode - Data serializing mode (load/save)
 * @param	saveFlag - Data saving type flag
 * @return	true/false
 */
bool CPowerPlusApp::dataSerializeCheck(BYTE serializeMode, int saveFlag /* = APPDATA_ALL */)
{
	bool result = true;
	WORD loadingResult = APP_ERROR_SUCCESS;
	WORD savingResult = APP_ERROR_SUCCESS;

	// Validate app config data
	if (getAppConfigData() == NULL) {
		if (serializeMode == Mode::Load) {
			loadingResult = APP_ERROR_LOAD_CFG_INVALID;
			traceSerializeData(loadingResult);
		}
		else if ((serializeMode == Mode::Save) && ((saveFlag & APPDATA_CONFIG) != 0)) {
			savingResult = APP_ERROR_SAVE_CFG_INVALID;
			traceSerializeData(savingResult);
		}
		result = false;
	}
	// Validate schedule data
	if (getAppScheduleData() == NULL) {
		if (serializeMode == Mode::Load) {
			loadingResult = APP_ERROR_LOAD_SCHED_INVALID;
			traceSerializeData(loadingResult);
		}
		else if ((serializeMode == Mode::Save) && ((saveFlag & APPDATA_SCHEDULE) != 0)) {
			savingResult = APP_ERROR_SAVE_SCHED_INVALID;
			traceSerializeData(savingResult);
		}
		result = false;
	}
	// Validate HotkeySet data
	if (getAppHotkeySetData() == NULL) {
		if (serializeMode == Mode::Load) {
			loadingResult = APP_ERROR_LOAD_HKEYSET_INVALID;
			traceSerializeData(loadingResult);
		}
		else if ((serializeMode == Mode::Save) && ((saveFlag & APPDATA_HOTKEYSET) != 0)) {
			savingResult = APP_ERROR_SAVE_HKEYSET_INVALID;
			traceSerializeData(savingResult);
		}
		result = false;
	}
	// Validate Power Reminder data
	if (getAppPwrReminderData() == NULL) {
		if (serializeMode == Mode::Load) {
			loadingResult = APP_ERROR_LOAD_PWRRMD_INVALID;
			traceSerializeData(loadingResult);
		}
		else if ((serializeMode == Mode::Save) && ((saveFlag & APPDATA_PWRREMINDER) != 0)) {
			savingResult = APP_ERROR_SAVE_PWRRMD_INVALID;
			traceSerializeData(savingResult);
		}
		result = false;
	}

	return result;
}


//////////////////////////////////////////////////////////////////////////
// DebugTest dialog function

/**
 * @brief	Initialize app DebugTest dialog pointer
 * @param	None
 * @return	true/false
 */
bool CPowerPlusApp::initDebugTestDlg(void)
{
	// Initialize dialog
	debugTestDlgPtr_ = new CDebugTestDlg();

	// Check initialization validity
	if (debugTestDlgPtr_ == NULL) {
		TRACE_ERROR("Error: DebugTest dialog init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	else {
		debugTestDlgPtr_->Create(IDD_DEBUGTEST_DLG, NULL);
		debugTestDlgPtr_->ShowWindow(SW_HIDE);
	}

	return (debugTestDlgPtr_ != NULL);
}

/**
 * @brief	Get app DebugTest dialog pointer
 * @param	None
 * @return	SDialog*
 */
SDialog* CPowerPlusApp::getDebugTestDlg(void)
{
	return debugTestDlgPtr_;
}

/**
 * @brief	Destroy app DebugTest dialog pointer
 * @param	None
 * @return	None
 */
void CPowerPlusApp::destroyDebugTestDlg(void)
{
	// Destroy DebugTest dialog
	if (debugTestDlgPtr_ != NULL) {
		if (::IsWindow(debugTestDlgPtr_->GetSafeHwnd())) {
			debugTestDlgPtr_->DestroyWindow();
		}
		delete debugTestDlgPtr_;
		debugTestDlgPtr_ = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// Registry functions

/**
 * @brief	Get registry root key for auto-start function
 * @param	hAutoStartRootKey - Returned root key handle (ref-value)
 * @return	None
 */
void CPowerPlusApp::getAutoStartRegistryRootKey(HKEY& hAutoStartRootKey)
{
	// Init info data
	OSVERSIONINFOEX oviOSVersion{};
	oviOSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	// Get Window OS version
	if (RtlGetVersion != NULL)
		RtlGetVersion(&oviOSVersion);

	// Get root directory
	if (oviOSVersion.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		// HKEY_CURRENT_USER
		hAutoStartRootKey = HKEY_CURRENT_USER;
	}
	else {
		// HKEY_LOCAL_MACHINE
		hAutoStartRootKey = HKEY_LOCAL_MACHINE;
	}
}

/**
 * @brief	Enable/disable start-up with Windows function
 * @param	isEnabled	 - Auto startup option
 * @param	isRunAsAdmin - Run as admin option
 * @return	int - Result of registry writing process
 */
int CPowerPlusApp::enableAutoStart(bool isEnabled, bool isRunAsAdmin)
{
	long result;
	HKEY hRootKey, hKey;

	DWORD state;
	TCHAR path[MAX_PATH];

	int returnValue;

	// Get root key
	getAutoStartRegistryRootKey(hRootKey);

	// Create registry key
	result = RegCreateKeyEx(hRootKey, Registry::Path::AutoStart, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &state);

	// Registry key creation failed
	if (result != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key creation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return 0;
	}

	// Enable auto-start
	if (isEnabled == true) {

		String execCommand;

		if (isRunAsAdmin == true) {
			// Register to run as admin
			execCommand.format(Constant::Command::RunAsAdmin::Register, AppProfile::ProjectName, StringUtils::getApplicationPath(true).getString());
			WinExec(MAKEANSI(execCommand), SW_HIDE);
		}
		else {
			// Unregister to run as admin
			execCommand.format(Constant::Command::RunAsAdmin::Unregister, AppProfile::ProjectName);
			WinExec(MAKEANSI(execCommand), SW_HIDE);
		}

		// Register to run at startup
		GetModuleFileName(NULL, path, sizeof(path) / sizeof(TCHAR));
		result = RegSetValueEx(hKey, AppProfile::ProjectName, 0, REG_SZ, (LPBYTE)path, (_tcsclen(path) + 1) * sizeof(TCHAR));
		returnValue = (result == ERROR_SUCCESS);
	}
	else {
		// Unregister to run at startup
		RegDeleteValue(hKey, AppProfile::ProjectName);
		result = RegQueryValueEx(hKey, AppProfile::ProjectName, 0, NULL, NULL, NULL);
		returnValue = (result != ERROR_SUCCESS);
	}

	// Close key
	RegCloseKey(hKey);
	return returnValue;
}

/**
 * @brief	Check if startup with Windows is enabled or not
 * @param	None
 * @return	int - Result of querrying process
 */
int CPowerPlusApp::getAutoStartRegisterStatus(void)
{
	long result;
	HKEY hRootKey, hKey;

	// Get root directory
	getAutoStartRegistryRootKey(hRootKey);

	// Open registry key
	result = RegOpenKeyEx(hRootKey, Registry::Path::AutoStart, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey);
	if (result != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key open failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return 0;
	}

	// Get registry key value
	result = RegQueryValueEx(hKey, AppProfile::ProjectName, 0, NULL, NULL, NULL);

	// Close key
	RegCloseKey(hKey);
	return (result == ERROR_SUCCESS);
}

/**
 * @brief	Get last system event time
 * @param	eventType  - Event type (suspend/wakeup)
 * @param	timeSysEvent - Event time value
 * @return	true/false
 */
bool CPowerPlusApp::getLastSysEventTime(BYTE eventType, DateTime& timeSysEvent)
{
	// Get key name
	String keyName;
	switch (eventType)
	{
	case SystemEventID::SystemSuspend:
		keyName = Key::SystemEventTracking::LastSysSuspend;
		break;

	case SystemEventID::SystemWakeUp:
		keyName = Key::SystemEventTracking::LastSysWakeup;
		break;

	case SystemEventID::SessionEnded:
		keyName = Key::SystemEventTracking::LastSessionEnd;
		break;

	default:
		// Invalid argument
		TRACE_ERROR("Error: Invalid argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get registry data
	String sysEventTrackingInfo;
	if (!getSysEventTracking(keyName, sysEventTrackingInfo)) {

		// Get system event tracking data failed
		TRACE_ERROR("Error: Get system event tracking data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Extract time data from result string
	wchar_t timePeriod[5] = {0};
	int year, month, day, hour, minute, second, millisecs;
	int returnValue = swscanf_s(sysEventTrackingInfo.getString(), L"%d/%d/%d %d:%d:%d.%d %ls", &year, &month, &day,
		&hour, &minute, &second, &millisecs, timePeriod, static_cast<unsigned int>(_countof(timePeriod)));

	if (returnValue != 8) {
		// Extract system event tracking time failed
		TRACE_ERROR("Error: Get system event tracking time failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Return time data
	timeSysEvent = DateTime(year, static_cast<unsigned int>(month), static_cast<unsigned int>(day), hour, minute, second, millisecs);

	return true;
}

/**
 * @brief	Save last system event time
 * @param	eventType  - Event type (suspend/wakeup)
 * @param	timeSysEvent - Event time value
 * @return	true/false
 */
bool CPowerPlusApp::saveLastSysEventTime(BYTE eventType, const DateTime& timeSysEvent)
{
	// Get key name
	String keyName;
	switch (eventType)
	{
	case SystemEventID::SystemSuspend:
		keyName = Key::SystemEventTracking::LastSysSuspend;
		break;

	case SystemEventID::SystemWakeUp:
		keyName = Key::SystemEventTracking::LastSysWakeup;
		break;

	case SystemEventID::SessionEnded:
		keyName = Key::SystemEventTracking::LastSessionEnd;
		break;

	default:
		// Invalid argument
		TRACE_ERROR("Error: Invalid argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Format date/time
	unsigned timePeriod = (timeSysEvent.hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = getLanguageString(getAppLanguage(), timePeriod);
	const wchar_t* timeFormatString = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String dateTimeFormat = StringUtils::stringFormat(timeFormatString,
													timeSysEvent.year(),
													timeSysEvent.month(),
													timeSysEvent.day(),
													timeSysEvent.hour(),
													timeSysEvent.minute(),
													timeSysEvent.second(),
													timeSysEvent.millisecond(),
													timePeriodFormat);

	// Save registry data
	if (!writeSysEventTracking(keyName, dateTimeFormat)) {

		// Save system event tracking data failed
		TRACE_ERROR("Error: Save system event tracking data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	return true;
}

/**
 * @brief	Handle event when a debug command is executed
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	None
 */
void CPowerPlusApp::OnExecuteDebugCommand(WPARAM /*wParam*/, LPARAM lParam)
{
	// If debug command is empty, do nothing
	String debugCommand(LPARAM_TO_STRING(lParam));
	if (debugCommand.isEmpty())
		return;

	// Format debug command
	debugCommand.toLower();

	// Output event log
	outputEventLog(LOG_EVENT_EXEC_DEBUGCMD, debugCommand);
}

/**
 * @brief	Handle event when an error message is displayed
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	None
 */
void CPowerPlusApp::OnShowErrorMessage(WPARAM wParam, LPARAM lParam)
{
	// Error code
	DWORD errorCode = DWORD(wParam);
	String description = StringUtils::stringFormat(_T("Error code: 0x%04X"), errorCode);

	// Event log detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Error code ID
		logDetailInfo.addDetail(EventDetail::EventError, errorCode);

		// Message content detail info
		logDetailInfo.addDetail(EventDetail::MessageText, LPARAM_TO_STRING(lParam));
	}

	// Output event log
	outputEventLog(LOG_EVENT_ERROR_MESSAGE, description, &logDetailInfo);
}

