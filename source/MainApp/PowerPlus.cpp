
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlus.cpp
//		Description:	Source file for app class
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<1> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CPowerPlus
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPowerPlusApp, SWinApp)
	ON_COMMAND(ID_HELP,	&SWinApp::OnHelp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CPowerPlusApp
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusApp::CPowerPlusApp() : SWinApp()
{
	// RestartManagerSupport
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// Init application data pointers
	m_pcfgAppConfig = NULL;
	m_pschScheduleData = NULL;
	m_phksHotkeySetData = NULL;
	m_ppwrReminderData = NULL;

	// Init logging pointers
	m_pAppHistoryLog = NULL;

	// Hook procedure handle
	m_hAppKeyboardHook = NULL;

	// Init DebugTest dialog
	m_pDebugTestDlg = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CPowerPlusApp
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusApp::~CPowerPlusApp()
{
	// Delete app data pointers
	if (m_pcfgAppConfig != NULL) {
		delete m_pcfgAppConfig;
		m_pcfgAppConfig = NULL;
	}

	if (m_pschScheduleData != NULL) {
		m_pschScheduleData->DeleteAll();
		delete m_pschScheduleData;
		m_pschScheduleData = NULL;
	}

	if (m_phksHotkeySetData != NULL) {
		m_phksHotkeySetData->DeleteAll();
		delete m_phksHotkeySetData;
		m_phksHotkeySetData = NULL;
	}

	if (m_ppwrReminderData != NULL) {
		m_ppwrReminderData->DeleteAll();
		delete m_ppwrReminderData;
		m_ppwrReminderData = NULL;
	}

	// Delete log data pointers
	if (m_pAppHistoryLog != NULL) {
		delete m_pAppHistoryLog;
		m_pAppHistoryLog = NULL;
	}

	// Destroy DebugTest dialog
	DestroyDebugTestDlg();

	// Release trace/debug log files
	ReleaseTraceErrorLogFile();
	ReleaseTraceDebugLogFile();
	ReleaseDebugInfoLogFile();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Object name:	theApp
//	Description:	Unique app object
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusApp theApp;


//////////////////////////////////////////////////////////////////////////
// Instance functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitInstance
//	Description:	Initialize app instance
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::InitInstance()
{
	DWORD dwErrorCode;

	// Set application launch time
	SetAppLaunchTime(DateTimeUtils::GetCurrentDateTime());

	// Set application window caption (with product version number)
	if (!SetAppWindowCaption(IDS_APP_WINDOW_CAPTION, TRUE)) {

		// Set title string failed
		TRACE_ERROR("Error: Set app window caption failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		PostErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return FALSE;
	}

	// Check if there is any other instance currently running
	// If yes, bring that instance to top and exit current instance
	if (HWND hPrevWnd = FindWindow(NULL, GetAppWindowCaption())) {
		PostMessage(hPrevWnd, SM_WND_SHOWDIALOG, TRUE, (LPARAM)0);
		BringWindowToTop(hPrevWnd);
		SetForegroundWindow(hPrevWnd);
		return FALSE;
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
		PostErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return FALSE;
	}

	// Initialize DebugTest dialog
	InitDebugTestDlg();

	// Check CTRL key press state and open DebugTest dialog
	if (IS_PRESSED(VK_CONTROL)) {
		HWND hDebugTestDlg = m_pDebugTestDlg->GetSafeHwnd();
		PostMessage(hDebugTestDlg, SM_WND_SHOWDIALOG, TRUE, NULL);
	}

	// Setup registry key info
	SetRegistryKey(AppProfile::CompanyName);

	// Update application profile info data
	UpdateAppLaunchTimeProfileInfo();

	// Create neccessary sub-folders
	String subDirectory = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);
	CreateDirectory(subDirectory, NULL);

	// Setup low-level keyboard hook
	m_hAppKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	// Initialize app data
	if (!InitAppData()) {

		// Trace log
		TRACE_ERROR("Error: Init app data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		PostErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return FALSE;
	}
	
	// Initialize default data
	m_pcfgAppConfig->SetDefaultData();
	m_pschScheduleData->SetDefaultData();
	m_phksHotkeySetData->SetDefaultData();
	m_ppwrReminderData->SetDefaultData();

	// Load registry app data
	if (!LoadRegistryAppData()) {
		// Trace log
		TRACE_ERROR("Error: Load registry app data failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}

	// Initialize application language
	SetAppLanguageOption(GetAppOption(AppOptionID::languageID));
	if (!InitAppLanguage()) {

		// Trace log
		TRACE_ERROR("Error: Init app language failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		PostErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return FALSE;
	}
	
	// Initialize log objects
	InitAppEventLog();
	InitAppHistoryLog();

	// Output event log: InitInstance
	OutputEventLog(LOG_EVENT_INIT_INSTANCE);

	// Register to system wakeup event notifications
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS paramDevNotifySubs;
	paramDevNotifySubs.Callback = DeviceNotifyCallbackRoutine;
	paramDevNotifySubs.Context = NULL;

	HPOWERNOTIFY hPowerNotify = NULL;
	dwErrorCode = PowerRegisterSuspendResumeNotification(DEVICE_NOTIFY_CALLBACK, &paramDevNotifySubs, &hPowerNotify);

	if (dwErrorCode != ERROR_SUCCESS) {
		// Handle error and show message
		TRACE_FORMAT("Error: Power event notification register failed!!! (Code: 0x%08X)", dwErrorCode);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		PostErrorMessage(dwErrorCode);
	}

	/************************************************************************************/
	/*																					*/
	/*							BEGIN MAIN DIALOG MODAL LOOP							*/
	/*																					*/
	/************************************************************************************/
	
	// Initialize main dialog
	CPowerPlusDlg* pMainDlg = new CPowerPlusDlg;
	if (pMainDlg == NULL) {
		// Trace error
		TRACE_ERROR("Error: Main dialog pointer allocation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Show error message
		PostErrorMessage(APP_ERROR_APP_INIT_FAILURE);
		return FALSE;
	}

	// Set application main window pointer
	m_pMainWnd = pMainDlg;

	// Show/hide main dialog at startup
	if (GetAppOption(AppOptionID::showDlgAtStartup) == FALSE) {

		// Hide dialog
		pMainDlg->Create(IDD_POWERPLUS_DIALOG, NULL);
		pMainDlg->ShowWindow(SW_HIDE);

		// Set parent window for DebugTest dialog if available
		if (m_pDebugTestDlg != NULL) {
			m_pDebugTestDlg->SetParentWnd(pMainDlg);
		}

		// Notification sound
		MessageBeep(0xFFFFFFFF);

		// Run modal loop
		pMainDlg->RunModalLoop();
	}
	else {

		// Set parent window for DebugTest dialog if available
		if (m_pDebugTestDlg != NULL) {
			m_pDebugTestDlg->SetParentWnd(pMainDlg);
		}

		// Show dialog in modal state
		pMainDlg->DoModal();

		// Bring dialog window to top
		pMainDlg->SendMessage(SM_WND_SHOWDIALOG, TRUE, NULL);
		pMainDlg->BringWindowToTop();
		pMainDlg->SetForegroundWindow();
	}

	/************************************************************************************/
	/*																					*/
	/*							 END MAIN DIALOG MODAL LOOP								*/
	/*																					*/
	/************************************************************************************/

	// Unregister to system wakeup event notifications
	if (hPowerNotify != NULL) {
		dwErrorCode = PowerUnregisterSuspendResumeNotification(hPowerNotify);
		if (dwErrorCode != ERROR_SUCCESS) {
			// Handle error and show message
			TRACE_FORMAT("Error: Power event notification unregister failed!!! (Code: 0x%08X)", dwErrorCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			PostErrorMessage(dwErrorCode);
		}
	}

	// Destroy DebugTest dialog
	DestroyDebugTestDlg();

	// Delete the main dialog pointer
	if (pMainDlg != NULL) {
		delete pMainDlg;
		pMainDlg = NULL;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExitInstance
//	Description:	Exit app instance
//  Arguments:		None
//  Return value:	int - Default
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusApp::ExitInstance()
{
	// Output event log: ExitInstance
	OutputEventLog(LOG_EVENT_EXIT_INSTANCE);

	// Write application event logging data to file if enabled
	if (GetAppOption(AppOptionID::saveAppEventLog) == TRUE) {
		GetAppEventLog()->Write();
	}

	// Write action history logging data to file if enabled
	if (GetAppOption(AppOptionID::saveAppHistoryLog) == TRUE) {
		GetAppHistoryLog()->Write();
	}

	// Close DebugTest dialog
	DestroyDebugTestDlg();

	// Find if the DebugTest dialog is still running
	HWND hDebugTestWnd = FindDebugTestDlg();
	if (hDebugTestWnd != NULL) {
		// Destroy dialog
		::DestroyWindow(hDebugTestWnd);
	}

	// Unhook keyboard
	UnhookWindowsHookEx(m_hAppKeyboardHook);

	// Mark as safe-termination
	SetSafeTerminationFlag(FLAG_ON);
	SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);

	// Default
	return SWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	KeyboardProc
//	Description:	Low-level keyboard hook process
//  Arguments:		Default
//  Return value:	LRESULT - Default
//
//////////////////////////////////////////////////////////////////////////

LRESULT WINAPI CPowerPlusApp::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Get low-level keyboard hook info param
	PKBDLLHOOKSTRUCT hHookKeyInfo = (PKBDLLHOOKSTRUCT)lParam;
	if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) && nCode == HC_ACTION) {
		DWORD dwKeyCode = hHookKeyInfo->vkCode;		// Get keycode
		DWORD dwKeyFlags = hHookKeyInfo->flags;		// Get keyflags

		/*********************************************************************/
		/*																	 */
		/*			  Keyboard process in screen unlock state			     */
		/*																	 */
		/*********************************************************************/

		if (GetSessionLockFlag() == FLAG_OFF) {
			// Process when Alt & Backspace keys are pressed
			if ((dwKeyCode == VK_BACK) && (dwKeyFlags & LLKHF_ALTDOWN)) {

				// Keystroke pressed: "Alt + Win + Backspace"
				if (IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)) {
					// Show DebugTest dialog
					PostMessage(GET_HANDLE_MAINWND(), SM_WND_DEBUGTEST, NULL, NULL);
				}
				// Keystroke pressed: "Alt + Shift + Backspace"
				else if (IS_PRESSED(VK_LSHIFT) || IS_PRESSED(VK_RSHIFT)) {
					// Show main dialog window
					PostMessage(GET_HANDLE_MAINWND(), SM_WND_SHOWDIALOG, TRUE, NULL);
				}
			}
		}

		/*********************************************************************/
		/*																	 */
		/*				Keyboard process in screen lock state			     */
		/*																	 */
		/*********************************************************************/

		else if (GetSessionLockFlag() == FLAG_ON) {
			// Process hotkey when the screen is locked
			if (((dwKeyCode >= VK_F1) && (dwKeyCode <= VK_F12)) &&					// Only process if a function key (F1 -> F12) and
				((dwKeyFlags & LLKHF_ALTDOWN) ||									// either at least one of these control keys: Alt key (left or right)
				(IS_PRESSED(VK_LCONTROL) || IS_PRESSED(VK_RCONTROL)) ||				// or Ctrl key (left or right)
				(IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)))) {					// or Windows key (left or right) is pressed

				// Get app pointer
				CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();

				// Only process if both options are enabled
				if ((pApp != NULL) &&
					(pApp->GetAppOption(AppOptionID::backgroundHotkeyEnabled) == TRUE) &&		// Enable background action hotkeys
					(pApp->GetAppOption(AppOptionID::lockStateHotkeyEnabled) == TRUE)) {		// Allow background hotkeys on lockscreen

					// Keycode param
					DWORD dwHKeyParam = NULL;
					{
						// Modifier keys
						WORD wModifiers = NULL;
						wModifiers |= ((IS_PRESSED(VK_LCONTROL) || IS_PRESSED(VK_RCONTROL)) ? MOD_CONTROL : NULL);		// Is Ctrl key pressed???
						wModifiers |= ((dwKeyFlags & LLKHF_ALTDOWN) ? MOD_ALT : 0);										// Is Alt key pressed???
						wModifiers |= ((IS_PRESSED(VK_LWIN) || IS_PRESSED(VK_RWIN)) ? MOD_WIN : NULL);					// Is Windows key pressed???

						// Virtual key code
						WORD wVirtualKey = dwKeyCode;

						// Make keycode param (combine modifiers and virtual key code)
						dwHKeyParam = MAKELONG(wModifiers, wVirtualKey);

						// Output debug log
						OutputDebugLogFormat(_T("Lockstate Hotkey pressed: Modifiers=0x%04X, VirtualKey=0x%04X"), wModifiers, wVirtualKey);
					}

					// Post message to main dialog window
					PostMessage(GET_HANDLE_MAINWND(), SM_APP_LOCKSTATE_HOTKEY, dwHKeyParam, NULL);
				}
			}
		}
	}

	// Call next hook
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeviceNotifyCallbackRoutine
//	Description:	Device notify callback routine (callback function)
//  Arguments:		Default (see MSDN "DEVICE_NOTIFY_CALLBACK_ROUTINE")
//  Return value:	ULONG - Default
//
//////////////////////////////////////////////////////////////////////////

ULONG CPowerPlusApp::DeviceNotifyCallbackRoutine(PVOID /*pContext*/, ULONG ulType, PVOID /*pSetting*/)
{
	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) 
		return ULONG(Result::Failure);

	// Get current date/time
	DateTime curDateTime = DateTimeUtils::GetCurrentDateTime();

	// Processing
	switch (ulType)
	{
	case PBT_APMSUSPEND:					// System suspend event
		// Save last system suspend time
		pApp->SaveLastSysEventTime(SystemEventID::SystemSuspend, curDateTime);
		break;

	case PBT_APMRESUMESUSPEND:				// System resume from suspend event
	case PBT_APMRESUMEAUTOMATIC:			// System automatic resume event
		// Save last system wakeup time
		pApp->SaveLastSysEventTime(SystemEventID::SystemWakeUp, curDateTime);
		break;

	default:
		break;
	}

	// Default return
	return ULONG(Result::Success);		// ERROR_SUCCESS
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessMessageFilter
//	Description:	Process app message filter
//  Arguments:		code - Default
//					msg - Default
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::ProcessMessageFilter(int nCode, LPMSG lpMsg)
{
	// Default process
	if (SWinApp::ProcessMessageFilter(nCode, lpMsg))
		return TRUE;

	// Pre-translate message if there's no window handler
	if (lpMsg->hwnd == NULL)
		return PreTranslateMessage(lpMsg);

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreTranslateMessage
//	Description:	Pre-translate app messages
//  Arguments:		pMsg - Default
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::PreTranslateMessage(MSG* pMsg)
{
	// "Show error message" message
	if (pMsg->message == SM_APP_ERROR_MESSAGE) {
		// Get window handle and error code
		HWND hRcvWnd = pMsg->hwnd;
		DWORD dwErrCode = (DWORD)(pMsg->wParam);

		// If the message window handle is invalid (HWND is NULL), 
		// and the main window has not been initialized, or the app language has not been loaded,
		// handle message and show error messagebox here
		if (((hRcvWnd == NULL) && (this->GetMainWnd() == NULL)) || (this->GetAppLanguage() == NULL)) {
			ShowErrorMessage(NULL, NULL, dwErrCode);
			return TRUE;
		}

		// If the main window is available, 
		// let it handle the error message on its own
		else if (this->GetMainWnd() != NULL) {
			HWND hMainWnd = GET_HANDLE_MAINWND();
			SendMessage(hMainWnd, pMsg->message, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	// "Error message showed" message
	else if (pMsg->message == SM_APP_SHOW_ERROR_MSG) {
		// Only process if the message window handle is invalid (HWND is NULL)
		HWND hRcvWnd = pMsg->hwnd;
		if (hRcvWnd == NULL) {
			OnShowErrorMessage(pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	// "Debug command execution" message
	else if (pMsg->message == SM_APP_DEBUGCMD_EXEC) {
		// Only process if the message window handle is invalid (HWND is NULL)
		HWND hRcvWnd = pMsg->hwnd;
		if (hRcvWnd == NULL) {
			OnExecuteDebugCommand(pMsg->wParam, pMsg->lParam);
			return TRUE;
		}
	}

	// Default
	return SWinApp::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////////////////////////////
// App data serialization functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppData
//	Description:	Initialize app data
//  Arguments:		None
//  Return value:	BOOL - Result of initialization
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::InitAppData()
{
	// Initialize app config data
	if (m_pcfgAppConfig == NULL) {
		m_pcfgAppConfig = new ConfigData;
	}
	// Initialize schedule data
	if (m_pschScheduleData == NULL) {
		m_pschScheduleData = new ScheduleData;
	}
	// Initialize HotkeySet data
	if (m_phksHotkeySetData == NULL) {
		m_phksHotkeySetData = new HotkeySetData;
	}
	// Initialize Power Reminder data
	if (m_ppwrReminderData == NULL) {
		m_ppwrReminderData = new PwrReminderData;
	}

	// Check data validity
	BOOL bResult = TRUE;

	// Check app config data
	if (m_pcfgAppConfig == NULL) {
		bResult = FALSE;
		TRACE_ERROR("Error: App config data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check schedule data
	if (m_pschScheduleData == NULL) {
		bResult = FALSE;
		TRACE_ERROR("Error: Schedule data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check HotkeySet data
	if (m_phksHotkeySetData == NULL) {
		bResult = FALSE;
		TRACE_ERROR("Error: HotkeySet data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	// Check Power Reminder data
	if (m_ppwrReminderData == NULL) {
		bResult = FALSE;
		TRACE_ERROR("Error: Power Reminder data init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadRegistryAppData
//	Description:	Load app data from registry
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::LoadRegistryAppData()
{
	BOOL bResult = TRUE;
	BOOL bFinalResult = TRUE;
	WORD wLoadRet = APP_ERROR_SUCCESS;

	int nDataTemp = INT_INVALID;
	int nTimeTemp = INT_INVALID;
	String tempString = Constant::String::Empty;
	ClockTime clockTimeTemp;

	// Check data validity first
	if (!DataSerializeCheck(Mode::Load))
		return FALSE;

	// Create temporary data
	PCONFIGDATA pcfgTempData = new CONFIGDATA;
	ScheduleData* pschTempData = new ScheduleData;
	HotkeySetData* phksTempData = new HotkeySetData;
	PwrReminderData* ppwrTempData = new PwrReminderData;


	/***********************************************************************************************/
	/*																							   */
	/*									 Load configuration info								   */
	/*																							   */
	/***********************************************************************************************/

	if (pcfgTempData != NULL) {

		// Read configuration data
		int nConfigRet = INT_NULL;
		nConfigRet += GetConfig(Key::ConfigData::LMBAction,					(int&)pcfgTempData->nLMBAction);
		nConfigRet += GetConfig(Key::ConfigData::MMBAction,					(int&)pcfgTempData->nMMBAction);
		nConfigRet += GetConfig(Key::ConfigData::RMBAction,					(int&)pcfgTempData->nRMBAction);
		nConfigRet += GetConfig(Key::ConfigData::RMBShowMenu,				pcfgTempData->bRMBShowMenu);
		nConfigRet += GetConfig(Key::ConfigData::LanguageID,				(int&)pcfgTempData->nLanguageID);
		nConfigRet += GetConfig(Key::ConfigData::ShowDlgAtStartup,			pcfgTempData->bShowDlgAtStartup);
		nConfigRet += GetConfig(Key::ConfigData::StartupEnabled,			pcfgTempData->bStartupEnabled);
		nConfigRet += GetConfig(Key::ConfigData::ConfirmAction,				pcfgTempData->bConfirmAction);
		nConfigRet += GetConfig(Key::ConfigData::SaveHistoryLog,			pcfgTempData->bSaveHistoryLog);
		nConfigRet += GetConfig(Key::ConfigData::SaveAppEventLog,			pcfgTempData->bSaveAppEventLog);
		nConfigRet += GetConfig(Key::ConfigData::RunAsAdmin,				pcfgTempData->bRunAsAdmin);
		nConfigRet += GetConfig(Key::ConfigData::ShowErrorMsg,				pcfgTempData->bShowErrorMsg);
		nConfigRet += GetConfig(Key::ConfigData::NotifySchedule,			pcfgTempData->bNotifySchedule);
		nConfigRet += GetConfig(Key::ConfigData::AllowCancelSchedule,		pcfgTempData->bAllowCancelSchedule);
		nConfigRet += GetConfig(Key::ConfigData::EnableBackgroundHotkey,	pcfgTempData->bEnableBackgroundHotkey);
		nConfigRet += GetConfig(Key::ConfigData::LockStateHotkey,			pcfgTempData->bLockStateHotkey);
		nConfigRet += GetConfig(Key::ConfigData::EnablePowerReminder,		pcfgTempData->bEnablePowerReminder);

		// Mark data as reading failed
		// only if all values were read unsuccessfully
		bResult = (nConfigRet != INT_NULL);
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = APP_ERROR_LOAD_CFG_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE;	// Set final result
		bResult = TRUE;			// Reset flag
	}
	else {
		// Copy temporary data
		if (pcfgTempData != NULL) {
			m_pcfgAppConfig->Copy(*pcfgTempData);
			bResult = TRUE;		// Reset flag
		}
	}

	// Delete temporary data
	if (pcfgTempData != NULL) {
		delete pcfgTempData;
		pcfgTempData = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*										Load schedule info									   */
	/*																							   */
	/***********************************************************************************************/

	if (pschTempData != NULL) {

		// Initialize temp data
		pschTempData->Init();

		// Initialize default item
		ScheduleItem schDefaultTemp(ScheduleData::defaultItemID);
		{
			// Read default schedule item
			int nDefSchedRet = INT_NULL;

			// Enable state
			nDefSchedRet += GetDefaultSchedule(Key::ScheduleItem::IsEnabled, nDataTemp);
			schDefaultTemp.EnableItem(nDataTemp);

			// Action ID
			nDefSchedRet += GetDefaultSchedule(Key::ScheduleItem::ActionID, nDataTemp);
			schDefaultTemp.SetAction(nDataTemp);

			// Repeat enable state
			nDefSchedRet += GetDefaultSchedule(Key::PwrRepeatSet::IsRepeated, nDataTemp);
			schDefaultTemp.EnableRepeat(nDataTemp);

			// Repeat days
			nDefSchedRet += GetDefaultSchedule(Key::PwrRepeatSet::RepeatDays, nDataTemp);
			schDefaultTemp.SetActiveDays(BYTE(nDataTemp));

			// Time value
			nDefSchedRet += GetDefaultSchedule(Key::ScheduleItem::Time, nTimeTemp);
			if (nTimeTemp != INT_INVALID) {

				// Convert time value and set time
				clockTimeTemp.SetHour(GET_REGTIME_HOUR(nTimeTemp));
				clockTimeTemp.SetMinute(GET_REGTIME_MINUTE(nTimeTemp));
				schDefaultTemp.SetTime(clockTimeTemp);

				// Reset temp data
				clockTimeTemp = ClockTime();
				nTimeTemp = INT_INVALID;
			}

			// Mark data as reading failed
			// only if all values were read unsuccessfully
			bResult = (nDefSchedRet != INT_NULL);

			// Trace error
			if (bResult == FALSE) {
				wLoadRet = APP_ERROR_LOAD_SCHED_FAILED;
				TraceSerializeData(wLoadRet);
				bFinalResult = FALSE;	// Set final result
				bResult = TRUE;			// Reset flag
			}
			else {
				// Update default item data
				ScheduleItem& schDefaultItem = pschTempData->GetDefaultItem();
				schDefaultItem.Copy(schDefaultTemp);
			}
		}

		// Load number of extra items
		int nExtraItemNum = 0;
		bResult &= GetScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, nExtraItemNum);
		if (bResult != FALSE) {

			// Read each extra item data
			for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {

				// Initialize temp item
				ScheduleItem schExtraTemp;

				// Read extra item
				int nSchedRet = INT_NULL;

				// Enable state
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::ScheduleItem::IsEnabled, nDataTemp);
				schExtraTemp.EnableItem(nDataTemp);

				// Item ID
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::ScheduleItem::ItemID, nDataTemp);
				schExtraTemp.SetItemID(nDataTemp);

				// Action ID
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::ScheduleItem::ActionID, nDataTemp);
				schExtraTemp.SetAction(nDataTemp);

				// Repeat enable state
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::PwrRepeatSet::IsRepeated, nDataTemp);
				schExtraTemp.EnableRepeat(nDataTemp);

				// Repeat days
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::PwrRepeatSet::RepeatDays, nDataTemp);
				schExtraTemp.SetActiveDays(BYTE(nDataTemp));

				// Time value
				nSchedRet += GetScheduleExtra(nExtraIndex, Key::ScheduleItem::Time, nTimeTemp);
				if (nTimeTemp != INT_INVALID) {

					// Convert time value and set time
					clockTimeTemp.SetHour(GET_REGTIME_HOUR(nTimeTemp));
					clockTimeTemp.SetMinute(GET_REGTIME_MINUTE(nTimeTemp));
					schExtraTemp.SetTime(clockTimeTemp);

					// Reset temp data
					clockTimeTemp = ClockTime();
					nTimeTemp = INT_INVALID;
				}

				// Mark data as reading failed
				// only if all values were read unsuccessfully
				bResult = (nSchedRet != INT_NULL);

				// Trace error
				if (bResult == FALSE) {
					wLoadRet = APP_ERROR_LOAD_SCHED_FAILED;
					TraceSerializeData(wLoadRet);
					bFinalResult = FALSE;	// Set final result
					bResult = TRUE;			// Reset flag
				}

				// Update item data
				pschTempData->Update(schExtraTemp);
			}
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = APP_ERROR_LOAD_SCHED_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE;	// Set final result
		bResult = TRUE;			// Reset flag
	}
	else {
		// Copy temporary data
		if (pschTempData != NULL) {
			m_pschScheduleData->Copy(*pschTempData);
			bResult = TRUE;		// Reset flag
		}
	}

	// Delete temporary data
	if (pschTempData != NULL) {
		delete pschTempData;
		pschTempData = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*									  Load HotkeySet info								       */
	/*																							   */
	/***********************************************************************************************/

	// Load HotkeySet data
	int nItemNum = 0;
	if (phksTempData != NULL) {

		// Copy data
		phksTempData->Copy(*m_phksHotkeySetData);

		// Load number of items
		bResult &= GetHotkeyItemNum(Key::HotkeySetData::ItemNum, nItemNum);
		if (nItemNum > phksTempData->GetItemNum()) {
			// Limit the hotkeyset data item number
			nItemNum = phksTempData->GetItemNum();
		}

		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Initialize temp item
			HotkeySetItem hksTemp;
			ZeroMemory(&hksTemp, sizeof(HotkeySetItem));

			// Read item data
			int nItemRet = INT_NULL;

			// Enable state
			nItemRet += GetHotkeySet(nIndex, Key::HotkeySetItem::IsEnabled, nDataTemp);
			hksTemp.EnableItem(nDataTemp);

			// Action ID
			nItemRet += GetHotkeySet(nIndex, Key::HotkeySetItem::HKActionID, nDataTemp);
			hksTemp.SetActionID(nDataTemp);

			// Keycode
			int nModifiersTemp, nVirtKeyTemp;
			nItemRet += GetHotkeySet(nIndex, Key::HotkeySetItem::Modifiers, nModifiersTemp);
			nItemRet += GetHotkeySet(nIndex, Key::HotkeySetItem::VirtualKey, nVirtKeyTemp);
			hksTemp.SetKeyCode(nModifiersTemp, nVirtKeyTemp);

			// Mark the item as reading failed
			// only if all values were read unsuccessfully
			bResult = (nItemRet != INT_NULL);

			// Trace error
			if (bResult == FALSE) {
				wLoadRet = APP_ERROR_LOAD_HKEYSET_FAILED;
				TraceSerializeData(wLoadRet);
				bFinalResult = FALSE;	// Set final result
				bResult = TRUE;			// Reset flag
				continue;
			}

			// Update item data
			phksTempData->Update(hksTemp);
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = APP_ERROR_LOAD_HKEYSET_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE;	// Set final result
		bResult = TRUE;			// Reset flag
	}
	else {
		// Copy temporary data
		if (phksTempData != NULL) {
			m_phksHotkeySetData->Copy(*phksTempData);
			bResult = TRUE;		// Reset flag
		}
	}

	// Delete temporary data
	if (phksTempData != NULL) {
		phksTempData->DeleteAll();
		delete phksTempData;
		phksTempData = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Load Power Reminder info								   */
	/*																							   */
	/***********************************************************************************************/

	// Load Power Reminder data
	nItemNum = 0;
	if (ppwrTempData != NULL) {
		bResult &= GetPwrReminderItemNum(Key::PwrReminderData::ItemNum, nItemNum);
		if (bResult != FALSE) {

			// Initialize temp data
			ppwrTempData->Init();

			// Initialize Power Reminder common style data
			RmdMsgStyleSet rmdCommonStyleTemp;
			{
				// Read Power Reminder common style data
				int nPwrRmdCommonStyleRet = INT_NULL;

				// Background color
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::BkgrdColor, nDataTemp);
				rmdCommonStyleTemp.SetBkgrdColor((COLORREF)nDataTemp);

				// Text color
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::TextColor, nDataTemp);
				rmdCommonStyleTemp.SetTextColor((COLORREF)nDataTemp);

				// Font name
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontName, tempString);
				rmdCommonStyleTemp.SetFontName(tempString);

				// Font size
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontSize, nDataTemp);
				rmdCommonStyleTemp.SetFontSize(nDataTemp);

				// Timeout (auto-close) interval
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::Timeout, nDataTemp);
				rmdCommonStyleTemp.SetTimeout(nDataTemp);

				// Message icon ID
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconID, nDataTemp);
				rmdCommonStyleTemp.SetIconID(nDataTemp);

				// Message icon size
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconSize, nDataTemp);
				rmdCommonStyleTemp.SetIconSize(nDataTemp);

				// Message icon position
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconPosition, nDataTemp);
				rmdCommonStyleTemp.SetIconPosition(nDataTemp);

				// Message display position
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::DisplayPosition, nDataTemp);
				rmdCommonStyleTemp.SetDisplayPosition(nDataTemp);

				// Display area horizontal margin
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::HorizontalMargin, nDataTemp);
				rmdCommonStyleTemp.SetHorizontalMargin(nDataTemp);

				// Display area vertical margin
				nPwrRmdCommonStyleRet += GetPwrReminderCommonStyle(Key::PwrReminderMsgStyle::VerticalMargin, nDataTemp);
				rmdCommonStyleTemp.SetVerticalMargin(nDataTemp);

				// Mark data as reading failed
				// only if all values were read unsuccessfully
				bResult = (nPwrRmdCommonStyleRet != INT_NULL);

				// Trace error
				if (bResult == FALSE) {
					wLoadRet = APP_ERROR_LOAD_PWRRMD_FAILED;
					TraceSerializeData(wLoadRet);
					bFinalResult = FALSE;	// Set final result
					bResult = TRUE;			// Reset flag
				}
				else {
					// Update Power Reminder common style data
					RmdMsgStyleSet& rmdCommonStyle = ppwrTempData->GetCommonStyle();
					rmdCommonStyle.Copy(rmdCommonStyleTemp);
				}
			}

			// Read each item data
			for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

				// Initialize temp item
				PwrReminderItem pwrTemp;

				// Read item data
				int nItemRet = INT_NULL;

				// Item ID
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::ItemID, nDataTemp);
				pwrTemp.SetItemID(nDataTemp);

				// Enable state
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::IsEnabled, nDataTemp);
				pwrTemp.EnableItem(nDataTemp);

				// Message content
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::Message, tempString);
				pwrTemp.SetMessage(tempString);

				// Event ID
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::EventID, nDataTemp);
				pwrTemp.SetEventID(nDataTemp);

				// Message style
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::MsgStyle, nDataTemp);
				pwrTemp.SetMessageStyle(nDataTemp);

				// Repeat enable state
				nItemRet += GetPwrReminder(nIndex, Key::PwrRepeatSet::IsRepeated, nDataTemp);
				pwrTemp.EnableRepeat(nDataTemp);

				// Allow snoozing
				nItemRet += GetPwrReminder(nIndex, Key::PwrRepeatSet::AllowSnooze, nDataTemp);
				pwrTemp.EnableSnoozing(nDataTemp);

				// Snooze interval
				nItemRet += GetPwrReminder(nIndex, Key::PwrRepeatSet::SnoozeInterval, nDataTemp);
				pwrTemp.SetSnoozeInterval(nDataTemp);

				// Repeat days
				nItemRet += GetPwrReminder(nIndex, Key::PwrRepeatSet::RepeatDays, nDataTemp);
				pwrTemp.SetActiveDays(nDataTemp);

				// Time value
				nItemRet += GetPwrReminder(nIndex, Key::PwrReminderItem::Time, nTimeTemp);
				if (nTimeTemp != INT_INVALID) {

					// Convert time value and set time
					clockTimeTemp.SetHour(GET_REGTIME_HOUR(nTimeTemp));
					clockTimeTemp.SetMinute(GET_REGTIME_MINUTE(nTimeTemp));
					pwrTemp.SetTime(clockTimeTemp);

					// Reset temp data
					clockTimeTemp = ClockTime();
					nTimeTemp = INT_INVALID;
				}

				// Mark the item as reading failed
				// only if all values were read unsuccessfully
				bResult = (nItemRet != INT_NULL);

				// Trace error
				if (bResult == FALSE) {
					wLoadRet = APP_ERROR_LOAD_PWRRMD_FAILED;
					TraceSerializeData(wLoadRet);
					bFinalResult = FALSE;	// Set final result
					bResult = TRUE;			// Reset flag
					continue;
				}

				// Update item data
				ppwrTempData->Update(pwrTemp);
			}
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = APP_ERROR_LOAD_PWRRMD_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE;	// Set final result
		bResult = TRUE;			// Reset flag
	}
	else {
		// Copy temporary data
		if (ppwrTempData != NULL) {
			m_ppwrReminderData->Copy(*ppwrTempData);
			m_ppwrReminderData->Adjust();
			bResult = TRUE;		// Reset flag
		}
	}

	// Delete temporary data
	if (ppwrTempData != NULL) {
		ppwrTempData->DeleteAll();
		delete ppwrTempData;
		ppwrTempData = NULL;
	}

	/***********************************************************************************************/
	/*																							   */
	/*										Load other data										   */
	/*																							   */
	/***********************************************************************************************/
	
	// Load global data values
	LoadGlobalData();
	
	return bFinalResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveRegistryAppData
//	Description:	Save app data to registry
//  Arguments:		dwDataType - App data type to save
//  Return value:	BOOL - Result of saving process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::SaveRegistryAppData(DWORD dwDataType /* = APPDATA_ALL */)
{
	BOOL bResult = TRUE;
	BOOL bFinalResult = TRUE;
	WORD wSaveRet = APP_ERROR_SUCCESS;
	int nTimeTemp = INT_INVALID;

	// Check data validity first
	if (!DataSerializeCheck(Mode::Save, dwDataType))
		return FALSE;

	/***********************************************************************************************/
	/*																							   */
	/*									Save configuration info									   */
	/*																							   */
	/***********************************************************************************************/

	// Save configuration data
	if ((dwDataType & APPDATA_CONFIG) != 0) {

		// Delete old data before writing
		DeleteConfigSection();

		// Get a copy of config data
		CONFIGDATA cfgConfigTemp{};
		if (m_pcfgAppConfig != NULL) {
			m_pcfgAppConfig->GetData(cfgConfigTemp);
		}

		// Save registry data
		bResult &= WriteConfig(Key::ConfigData::LMBAction,				cfgConfigTemp.nLMBAction);
		bResult &= WriteConfig(Key::ConfigData::MMBAction,				cfgConfigTemp.nMMBAction);
		bResult &= WriteConfig(Key::ConfigData::RMBAction,				cfgConfigTemp.nRMBAction);
		bResult &= WriteConfig(Key::ConfigData::RMBShowMenu,			cfgConfigTemp.bRMBShowMenu);
		bResult &= WriteConfig(Key::ConfigData::LanguageID,				cfgConfigTemp.nLanguageID);
		bResult &= WriteConfig(Key::ConfigData::ShowDlgAtStartup,		cfgConfigTemp.bShowDlgAtStartup);
		bResult &= WriteConfig(Key::ConfigData::StartupEnabled,			cfgConfigTemp.bStartupEnabled);
		bResult &= WriteConfig(Key::ConfigData::ConfirmAction,			cfgConfigTemp.bConfirmAction);
		bResult &= WriteConfig(Key::ConfigData::SaveHistoryLog,			cfgConfigTemp.bSaveHistoryLog);
		bResult &= WriteConfig(Key::ConfigData::SaveAppEventLog,		cfgConfigTemp.bSaveAppEventLog);
		bResult &= WriteConfig(Key::ConfigData::RunAsAdmin,				cfgConfigTemp.bRunAsAdmin);
		bResult &= WriteConfig(Key::ConfigData::ShowErrorMsg,			cfgConfigTemp.bShowErrorMsg);
		bResult &= WriteConfig(Key::ConfigData::NotifySchedule,			cfgConfigTemp.bNotifySchedule);
		bResult &= WriteConfig(Key::ConfigData::AllowCancelSchedule,	cfgConfigTemp.bAllowCancelSchedule);
		bResult &= WriteConfig(Key::ConfigData::EnableBackgroundHotkey,	cfgConfigTemp.bEnableBackgroundHotkey);
		bResult &= WriteConfig(Key::ConfigData::LockStateHotkey,		cfgConfigTemp.bLockStateHotkey);
		bResult &= WriteConfig(Key::ConfigData::EnablePowerReminder,	cfgConfigTemp.bEnablePowerReminder);

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = APP_ERROR_SAVE_CFG_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Save schedule info										   */
	/*																							   */
	/***********************************************************************************************/

	// Save schedule data
	if ((dwDataType & APPDATA_SCHEDULE) != 0) {

		// Delete old data before writing
		DeleteScheduleSection();

		// Save default schedule item
		ScheduleItem schTempDefault = m_pschScheduleData->GetDefaultItem();
		{
			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(schTempDefault.GetTime());

			// Save registry data
			bResult &= WriteDefaultSchedule(Key::ScheduleItem::IsEnabled,		schTempDefault.IsEnabled());
			bResult &= WriteDefaultSchedule(Key::ScheduleItem::ActionID,		schTempDefault.GetAction());
			bResult &= WriteDefaultSchedule(Key::PwrRepeatSet::IsRepeated,		schTempDefault.IsRepeatEnabled());
			bResult &= WriteDefaultSchedule(Key::PwrRepeatSet::RepeatDays,		schTempDefault.GetActiveDays());
			bResult &= WriteDefaultSchedule(Key::ScheduleItem::Time,			nTimeTemp);

			// Trace error
			if (bResult == FALSE) {
				wSaveRet = APP_ERROR_SAVE_SCHED_FAILED;
				TraceSerializeData(wSaveRet);
				bFinalResult = FALSE; // Set final result
				bResult = TRUE; // Reset flag
			}
		}

		// Save schedule extra data
		int nExtraItemNum = m_pschScheduleData->GetExtraItemNum();
		bResult &= WriteScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, nExtraItemNum);
		for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {

			// Get schedule extra item
			ScheduleItem schTempExtra = m_pschScheduleData->GetItemAt(nExtraIndex);

			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(schTempExtra.GetTime());

			// Save registry data
			bResult &= WriteScheduleExtra(nExtraIndex, Key::ScheduleItem::IsEnabled,	schTempExtra.IsEnabled());
			bResult &= WriteScheduleExtra(nExtraIndex, Key::ScheduleItem::ItemID,		schTempExtra.GetItemID());
			bResult &= WriteScheduleExtra(nExtraIndex, Key::ScheduleItem::ActionID,		schTempExtra.GetAction());
			bResult &= WriteScheduleExtra(nExtraIndex, Key::PwrRepeatSet::IsRepeated,	schTempExtra.IsRepeatEnabled());
			bResult &= WriteScheduleExtra(nExtraIndex, Key::PwrRepeatSet::RepeatDays,	schTempExtra.GetActiveDays());
			bResult &= WriteScheduleExtra(nExtraIndex, Key::ScheduleItem::Time,			nTimeTemp);

			// Trace error
			if (bResult == FALSE) {
				wSaveRet = APP_ERROR_SAVE_SCHED_FAILED;
				TraceSerializeData(wSaveRet);
				bFinalResult = FALSE; // Set final result
				bResult = TRUE; // Reset flag
			}
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*								 Save auto-start status info								   */
	/*																							   */
	/***********************************************************************************************/

	// Save auto-start status info
	if ((dwDataType & APPDATA_CONFIG) != 0) {

		BOOL bStartupEnabled = m_pcfgAppConfig->bStartupEnabled;
		BOOL bRunAsAdmin = m_pcfgAppConfig->bRunAsAdmin;
		int nRetAutoStartEnabled = EnableAutoStart(bStartupEnabled, bRunAsAdmin);
		bResult &= nRetAutoStartEnabled ? TRUE : FALSE;

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = APP_ERROR_SAVE_CFG_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									 Save HotkeySet info									   */
	/*																							   */
	/***********************************************************************************************/

	// Save HotkeySet data
	if ((dwDataType & APPDATA_HOTKEYSET) != 0) {

		// Delete old data before writing
		DeleteHotkeySetSection();

		// Save registry data
		int nItemNum = m_phksHotkeySetData->GetItemNum();
		bResult &= WriteHotkeyItemNum(Key::HotkeySetData::ItemNum, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Get HotkeySet item
			HotkeySetItem hksTemp = m_phksHotkeySetData->GetItemAt(nIndex);

			// Get keycode
			DWORD dwModifiersTemp, dwVirtKeyTemp;
			hksTemp.GetKeyCode(dwModifiersTemp, dwVirtKeyTemp);

			// Write item data
			bResult &= WriteHotkeySet(nIndex, Key::HotkeySetItem::IsEnabled,	hksTemp.IsEnabled());
			bResult &= WriteHotkeySet(nIndex, Key::HotkeySetItem::HKActionID,	hksTemp.GetActionID());
			bResult &= WriteHotkeySet(nIndex, Key::HotkeySetItem::Modifiers,	dwModifiersTemp);
			bResult &= WriteHotkeySet(nIndex, Key::HotkeySetItem::VirtualKey,	dwVirtKeyTemp);
		}

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = APP_ERROR_SAVE_HKEYSET_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***********************************************************************************************/
	/*																							   */
	/*									Save Power Reminder info								   */
	/*																							   */
	/***********************************************************************************************/

	// Save Power Reminder data
	if ((dwDataType & APPDATA_PWRREMINDER) != 0) {

		// Delete old data before writing
		DeletePwrReminderSection();

		// Save Power Reminder common style data
		RmdMsgStyleSet& rmdTempCommonStyle = m_ppwrReminderData->GetCommonStyle();
		{
			// Save registry data
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::BkgrdColor,		rmdTempCommonStyle.GetBkgrdColor());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::TextColor,			rmdTempCommonStyle.GetTextColor());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontName,			rmdTempCommonStyle.GetFontName());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::FontSize,			rmdTempCommonStyle.GetFontSize());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::Timeout,			rmdTempCommonStyle.GetTimeout());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconID,			rmdTempCommonStyle.GetIconID());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconSize,	 		rmdTempCommonStyle.GetIconSize());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::IconPosition,		rmdTempCommonStyle.GetIconPosition());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::DisplayPosition,	rmdTempCommonStyle.GetDisplayPosition());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::HorizontalMargin,	rmdTempCommonStyle.GetHorizontalMargin());
			bResult &= WritePwrReminderCommonStyle(Key::PwrReminderMsgStyle::VerticalMargin,	rmdTempCommonStyle.GetVerticalMargin());

			// Trace error
			if (bResult == FALSE) {
				wSaveRet = APP_ERROR_SAVE_PWRRMD_FAILED;
				TraceSerializeData(wSaveRet);
				bFinalResult = FALSE; // Set final result
				bResult = TRUE; // Reset flag
			}
		}

		// Save registry data
		int nItemNum = m_ppwrReminderData->GetItemNum();
		bResult &= WritePwrReminderItemNum(Key::PwrReminderData::ItemNum, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Get Power Reminder item
			PwrReminderItem pwrTemp = m_ppwrReminderData->GetItemAt(nIndex);

			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(pwrTemp.GetTime());

			// Write item data
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::ItemID,		pwrTemp.GetItemID());
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::IsEnabled,	pwrTemp.IsEnabled());
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::Message,		pwrTemp.GetMessage());
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::EventID,		pwrTemp.GetEventID());
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::Time,			nTimeTemp);
			bResult &= WritePwrReminder(nIndex, Key::PwrReminderItem::MsgStyle,		pwrTemp.GetMessageStyle());
			bResult &= WritePwrReminder(nIndex, Key::PwrRepeatSet::IsRepeated,		pwrTemp.IsRepeatEnabled());
			bResult &= WritePwrReminder(nIndex, Key::PwrRepeatSet::AllowSnooze,		pwrTemp.IsAllowSnoozing());
			bResult &= WritePwrReminder(nIndex, Key::PwrRepeatSet::SnoozeInterval,	pwrTemp.GetSnoozeInterval());
			bResult &= WritePwrReminder(nIndex, Key::PwrRepeatSet::RepeatDays,		pwrTemp.GetActiveDays());
		}

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = APP_ERROR_SAVE_PWRRMD_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	return bFinalResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	BackupRegistryAppData
//	Description:	Backup app data to file
//  Arguments:		None
//  Return value:	BOOL - Result of backing up process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::BackupRegistryAppData()
{
	// Auto backup using Registry Export
	return BackupSystem::RegistryExport();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateAppLaunchTimeProfileInfo
//	Description:	Load and update application launch-time profile info data
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::UpdateAppLaunchTimeProfileInfo(void)
{
	BOOL bRet = FALSE;

	int nValue = (int)0;						// Integer type
	UINT uiValue = (UINT)0;						// Unsigned integer value
	String strValue = Constant::String::Empty;	// String value

	/*------------------------<Application launch-time counter>--------------------------*/

	// Load info from registry
	if (GetProfileInfo(AppProfile::LaunchInfo::LaunchCounter, nValue)) {
		SetAppLaunchTimeCounter(nValue);
		bRet = TRUE;
	}

	// Update and overwrite data
	UpdateAppLaunchTimeCounter();
	uiValue = GetAppLaunchTimeCounter();
	if (!WriteProfileInfo(AppProfile::LaunchInfo::LaunchCounter, uiValue)) {
		bRet = FALSE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Application launch-time>-----------------------------*/

	// Format launch-time
	DateTime dateTimeAppLaunch = GetAppLaunchTime();
	UINT nTimePeriod = (dateTimeAppLaunch.Hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = GetLanguageString(LoadLanguageTable(NULL), nTimePeriod);
	const wchar_t* timeFormatString = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	strValue = StringUtils::StringFormat(timeFormatString, dateTimeAppLaunch.Year(), dateTimeAppLaunch.Month(), dateTimeAppLaunch.Day(),
		dateTimeAppLaunch.Hour(), dateTimeAppLaunch.Minute(), dateTimeAppLaunch.Second(), dateTimeAppLaunch.Millisecond(), timePeriodFormat);

	// Store launch-time info data
	if (!WriteProfileInfo(AppProfile::LaunchInfo::LaunchTime, strValue)) {
		bRet = FALSE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*------------------------<Application directory/file info>--------------------------*/

	// Directory path (not including the executable file name)
	strValue = StringUtils::GetApplicationPath(false);
	if (!strValue.IsEmpty()) {
		if (!WriteProfileInfo(AppProfile::LaunchInfo::Directory, strValue)) {
			bRet = FALSE;
		}
	}

	// Executable file name
	strValue = StringUtils::GetApplicationPath(true);
	if (!strValue.IsEmpty()) {
		String execFileName = PathFindFileName(strValue);
		if (!WriteProfileInfo(AppProfile::LaunchInfo::FileName, execFileName)) {
			bRet = FALSE;
		}
	}

	// Product version (full version)
	strValue = StringUtils::GetProductVersion(true);
	if (!strValue.IsEmpty()) {
		if (!WriteProfileInfo(AppProfile::LaunchInfo::ProductVersion, strValue)) {
			bRet = FALSE;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	/*--------------------------<Device, system and user info>---------------------------*/

	// Device name
	BOOL bRetGetInfo = StringUtils::GetDeviceName(strValue);
	if ((bRetGetInfo != FALSE) && (!strValue.IsEmpty())) {
		if (!WriteProfileInfo(AppProfile::LaunchInfo::DeviceName, strValue)) {
			bRet = FALSE;
		}
	}

	// User name
	bRetGetInfo = StringUtils::GetCurrentUserName(strValue);
	if ((bRetGetInfo != FALSE) && (!strValue.IsEmpty())) {
		if (!WriteProfileInfo(AppProfile::LaunchInfo::UserName, strValue)) {
			bRet = FALSE;
		}
	}

	// Operating system info
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
		// Operating system version
		if (!WriteProfileInfo(AppProfile::LaunchInfo::OSVersion, osvi.dwMajorVersion)) {
			bRet = FALSE;
		}
		// Build number
		if (!WriteProfileInfo(AppProfile::LaunchInfo::OSBuildNumber, osvi.dwBuildNumber)) {
			bRet = FALSE;
		}
		// Platform ID
		if (!WriteProfileInfo(AppProfile::LaunchInfo::OSPlatformID, osvi.dwPlatformId)) {
			bRet = FALSE;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadGlobalData
//	Description:	Load global data values from registry
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::LoadGlobalData(void)
{
	BOOL bRet = FALSE;

	int nGlbValue = (int)0;									// Integer type

	// Subsection name
	String subSectionName = Constant::String::Empty;

	/*------------------------<Load debugging/testing variables>-------------------------*/

	// Subsection: DebugTest
	subSectionName = Section::GlobalData::DebugTest;

	// DummyTest mode
	if (GetGlobalData(subSectionName, Key::GlobalData::DebugTest::DummyTest, nGlbValue)) {
		SetDummyTestMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug mode
	if (GetGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugMode, nGlbValue)) {
		SetDebugMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug log output target
	if (GetGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugOutput, nGlbValue)) {
		SetDebugOutputTarget(nGlbValue);
		bRet |= TRUE;
	}
	// Test feature enable
	if (GetGlobalData(subSectionName, Key::GlobalData::DebugTest::TestFeature, nGlbValue)) {
		SetTestFeatureEnable(nGlbValue);
		bRet |= TRUE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Load app special flags>------------------------------*/

	// Subsection: AppFlags
	subSectionName = Section::GlobalData::AppFlag;

	// Power action trace flag
	if (GetGlobalData(subSectionName, Key::GlobalData::AppFlag::PwrActionFlag, nGlbValue)) {
		SetPwrActionFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// System suspended trace flag
	if (GetGlobalData(subSectionName, Key::GlobalData::AppFlag::SystemSuspendFlag, nGlbValue)) {
		SetSystemSuspendFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// Session ending trace flag
	if (GetGlobalData(subSectionName, Key::GlobalData::AppFlag::SessionEndFlag, nGlbValue)) {
		SetSessionEndFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// Previously safe termination trace flag
	if (GetGlobalData(subSectionName, Key::GlobalData::AppFlag::SafeTermination, nGlbValue)) {
		SetSafeTerminationFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	/*-----------------------------------------------------------------------------------*/

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveGlobalData
//	Description:	Save global data values to registry
//  Arguments:		byCateID - Category ID
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::SaveGlobalData(BYTE byCateID /* = 0xFF */)
{
	BOOL bRet = TRUE;

	int nGlbValue = (int)0;									// Integer type
	BYTE byGlbValue = (BYTE)0;								// Byte value
	String strGlbValue = Constant::String::Empty;			// String value

	// Subsection name
	String subSectionName = Constant::String::Empty;

	/*------------------------<Save debugging/testing variables>-------------------------*/

	if ((byCateID == 0xFF) || (byCateID == DEF_GLBDATA_CATE_DEBUGTEST)) {
		// Subsection: DebugTest
		subSectionName = Section::GlobalData::DebugTest;

		// DummyTest mode
		nGlbValue = GetDummyTestMode();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::DebugTest::DummyTest, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug mode
		nGlbValue = GetDebugMode();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugMode, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug log output target
		nGlbValue = GetDebugOutputTarget();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::DebugTest::DebugOutput, nGlbValue)) {
			bRet = FALSE;
		}
		// Test feature enable
		nGlbValue = GetTestFeatureEnable();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::DebugTest::TestFeature, nGlbValue)) {
			bRet = FALSE;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	/*---------------------------------<Save app flags>----------------------------------*/

	if ((byCateID == 0xFF) || (byCateID == DEF_GLBDATA_CATE_APPFLAGS)) {
		// Subsection: AppFlags
		subSectionName = Section::GlobalData::AppFlag;

		// Power action trace flag
		byGlbValue = GetPwrActionFlag();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::AppFlag::PwrActionFlag, byGlbValue)) {
			bRet = FALSE;
		}

		// System suspended trace flag
		byGlbValue = GetSystemSuspendFlag();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::AppFlag::SystemSuspendFlag, byGlbValue)) {
			bRet = FALSE;
		}

		// Session ending trace flag
		byGlbValue = GetSessionEndFlag();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::AppFlag::SessionEndFlag, byGlbValue)) {
			bRet = FALSE;
		}

		// Previously safe termination trace flag
		byGlbValue = GetSafeTerminationFlag();
		if (!WriteGlobalData(subSectionName, Key::GlobalData::AppFlag::SafeTermination, byGlbValue)) {
			bRet = FALSE;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// App data processing functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppConfigData
//	Description:	Get app configuration data
//  Arguments:		None
//  Return value:	ConfigData* - Config data pointer
//
//////////////////////////////////////////////////////////////////////////

ConfigData* CPowerPlusApp::GetAppConfigData()
{
	// Check validity
	ASSERT(m_pcfgAppConfig != NULL);
	return m_pcfgAppConfig;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppConfigData
//	Description:	Set app configuration data
//  Arguments:		pcfgData - Config data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppConfigData(ConfigData* pcfgData)
{
	// Check data validity
	ASSERT(pcfgData != NULL);
	if (pcfgData == NULL) return;

	// Backup data
	CONFIGDATA BakData;
	GetAppConfigData()->GetData(BakData);

	// Copy value of data pointer
	GetAppConfigData()->Copy(*pcfgData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppScheduleData
//	Description:	Get app schedule data
//  Arguments:		None
//  Return value:	ScheduleData* - Schedule data pointer
//
//////////////////////////////////////////////////////////////////////////

ScheduleData* CPowerPlusApp::GetAppScheduleData()
{
	// Check validity
	ASSERT(m_pschSheduleData != NULL);
	return m_pschScheduleData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppScheduleData
//	Description:	Set app schedule data
//  Arguments:		pschData - Schedule data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppScheduleData(ScheduleData* pschData)
{
	// Check data validity
	ASSERT(pschData != NULL);
	if (pschData == NULL) return;

	// Backup data
	ScheduleData BakData;
	BakData.Copy(*GetAppScheduleData());

	// Copy value of data pointer
	GetAppScheduleData()->Copy(*pschData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppHotkeySetData
//	Description:	Get app hotkeyset data
//  Arguments:		None
//  Return value:	HotkeySetData* - HotkeySet data pointer
//
//////////////////////////////////////////////////////////////////////////

HotkeySetData* CPowerPlusApp::GetAppHotkeySetData()
{
	// Check validity
	ASSERT(m_phksHotkeySetData != NULL);
	return m_phksHotkeySetData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppHotkeySetData
//	Description:	Set app hotkeyset data
//  Arguments:		phksData - HotkeySet data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppHotkeySetData(HotkeySetData* phksData)
{
	// Check data validity
	ASSERT(phksData != NULL);
	if (phksData == NULL) return;

	// Backup data
	HotkeySetData BakData;
	BakData.Copy(*GetAppHotkeySetData());

	// Copy value of data pointer
	GetAppHotkeySetData()->Copy(*phksData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppPwrReminderData
//	Description:	Get app Power Reminder data
//  Arguments:		None
//  Return value:	PwrReminderData* - Power Reminder data pointer
//
//////////////////////////////////////////////////////////////////////////

PwrReminderData* CPowerPlusApp::GetAppPwrReminderData()
{
	// Check validity
	ASSERT(m_ppwrReminderData != NULL);
	return m_ppwrReminderData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppPwrReminderData
//	Description:	Set app Power Reminder data
//  Arguments:		ppwrData - Power Reminder data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppPwrReminderData(PwrReminderData* ppwrData)
{
	// Check data validity
	ASSERT(ppwrData != NULL);
	if (ppwrData == NULL) return;

	// Backup data
	PwrReminderData BakData;
	BakData.Copy(*GetAppPwrReminderData());

	// Copy value of data pointer
	GetAppPwrReminderData()->Copy(*ppwrData);
}


//////////////////////////////////////////////////////////////////////////
// Data options and flags get/set functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppOption
//	Description:	Return option value by ID
//  Arguments:		eAppOptionID - ID of specific option
//  Return value:	int - Option value
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusApp::GetAppOption(AppOptionID eAppOptionID) const
{
	int nResult = INT_INVALID;

	switch (eAppOptionID)
	{
	case AppOptionID::curDispLanguage:
		nResult = SWinApp::GetAppLanguageOption(TRUE);
		break;
	case AppOptionID::defaultScheduleActiveState:
		nResult = m_pschScheduleData->GetDefaultItem().IsEnabled();
		break;
	case AppOptionID::defaultScheduleActionID:
		nResult = m_pschScheduleData->GetDefaultItem().GetAction();
		break;
	case AppOptionID::defaultScheduleRepeat:
		nResult = m_pschScheduleData->GetDefaultItem().IsRepeatEnabled();
		break;
	default:
		nResult = m_pcfgAppConfig->GetAppOption(eAppOptionID);
		break;
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppHistoryLog
//	Description:	Initialize action history log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::InitAppHistoryLog()
{
	// Initialization
	if (m_pAppHistoryLog == NULL) {
		m_pAppHistoryLog = new SLogging(LOGTYPE_HISTORY_LOG);
	}
	
	// Check validity after allocating
	if (m_pAppHistoryLog == NULL) {
		TRACE_ERROR("Error: AppHistoryLog initialization failed");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Set properties
	m_pAppHistoryLog->Init();
	m_pAppHistoryLog->SetWriteMode(WriteInstantly);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppHistoryLog
//	Description:	Get app action history log pointer
//  Arguments:		None
//  Return value:	SLogging - Action history log pointer
//
//////////////////////////////////////////////////////////////////////////

SLogging* CPowerPlusApp::GetAppHistoryLog()
{
	// Check validity
	VERIFY(m_pAppHistoryLog != NULL);
	return m_pAppHistoryLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputAppHistoryLog
//	Description:	Output a log item to action history log
//  Arguments:		logItem - Log item data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputAppHistoryLog(LOGITEM logItem)
{
	// Get app history logging pointer
	SLogging* ptrAppHistoryLog = GetAppHistoryLog();
	
	// Only output log if option is ON
	if ((ptrAppHistoryLog != NULL) && (GetAppOption(AppOptionID::saveAppHistoryLog) != FALSE)) {
		ptrAppHistoryLog->OutputItem(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	TraceSerializeData
//	Description:	Output trace log of data serialization
//  Arguments:		wErrCode - Error code
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::TraceSerializeData(WORD wErrCode)
{
	String traceMessageTitle = Constant::String::Empty;
	String traceLogFormat = Constant::String::Empty;
	const wchar_t* _dataNullString = _T("Data pointer is NULL");
	const wchar_t* _readFailedString = _T("Registry data is unreadable or invalid");
	const wchar_t* _writeFailedString = _T("Unable to write registry data");

	// Serialization trace skip flag
	// Note: If the application is launching for the 1st time, do not trace data loading error
	BOOL bSkipFlag = FALSE;
	
	switch (wErrCode)
	{
	case APP_ERROR_LOAD_CFG_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Load config failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_CFG_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		traceMessageTitle = _T("Load config failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_SCHED_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Load schedule failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_SCHED_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		traceMessageTitle = _T("Load schedule failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_HKEYSET_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Load hotkeyset failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_HKEYSET_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		traceMessageTitle = _T("Load hotkeyset failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _readFailedString);
		break;

	case APP_ERROR_LOAD_PWRRMD_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Load reminder failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_LOAD_PWRRMD_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		traceMessageTitle = _T("Load reminder failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _readFailedString);
		break;

	case APP_ERROR_SAVE_CFG_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save config failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_CFG_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save config failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_SCHED_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save schedule failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_SCHED_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save schedule failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_HKEYSET_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save hotkeyset failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_HKEYSET_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save hotkeyset failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _writeFailedString);
		break;

	case APP_ERROR_SAVE_PWRRMD_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save reminder failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _dataNullString);
		break;

	case APP_ERROR_SAVE_PWRRMD_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		traceMessageTitle = _T("Save reminder failed");
		traceLogFormat.Format(_T("%s: %s"), traceMessageTitle.GetString(), _writeFailedString);
		break;
	}

	// If skip flag is triggered
	if (bSkipFlag != FALSE)
		return;

	// Output trace error log
	if (!traceLogFormat.IsEmpty()) {
		TRACE_ERROR(traceLogFormat);
	}

	// Show error message
	UINT nMsg = SM_APP_ERROR_MESSAGE;
	WPARAM wParam = (WPARAM)wErrCode;
	CWnd* pMainWnd = this->GetMainWnd();
	if (pMainWnd != NULL) {
		// Post message to main window
		// Message will be handled by main window message map handler
		HWND hWnd = pMainWnd->GetSafeHwnd();
		PostMessage(hWnd, nMsg, wParam, NULL);
	}
	else {
		// Post message without passing window handler
		// Message will be handled by ProcessMessageFilter() and PreTranslateMessage()
		PostMessage(NULL, nMsg, wParam, NULL);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DataSerializeCheck
//	Description:	Check validity of data for serialization
//  Arguments:		bySerializeMode - Data serializing mode (load/save)
//					nSaveFlag		- Data saving type flag
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::DataSerializeCheck(BYTE bySerializeMode, int nSaveFlag /* = APPDATA_ALL */)
{
	BOOL bResult = TRUE;
	WORD wLoadRet = APP_ERROR_SUCCESS;
	WORD wSaveRet = APP_ERROR_SUCCESS;

	// Validate app config data
	if (GetAppConfigData() == NULL) {
		if (bySerializeMode == Mode::Load) {
			wLoadRet = APP_ERROR_LOAD_CFG_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == Mode::Save) && ((nSaveFlag & APPDATA_CONFIG) != 0)) {
			wSaveRet = APP_ERROR_SAVE_CFG_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate schedule data
	if (GetAppScheduleData() == NULL) {
		if (bySerializeMode == Mode::Load) {
			wLoadRet = APP_ERROR_LOAD_SCHED_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == Mode::Save) && ((nSaveFlag & APPDATA_SCHEDULE) != 0)) {
			wSaveRet = APP_ERROR_SAVE_SCHED_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate HotkeySet data
	if (GetAppHotkeySetData() == NULL) {
		if (bySerializeMode == Mode::Load) {
			wLoadRet = APP_ERROR_LOAD_HKEYSET_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == Mode::Save) && ((nSaveFlag & APPDATA_HOTKEYSET) != 0)) {
			wSaveRet = APP_ERROR_SAVE_HKEYSET_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate Power Reminder data
	if (GetAppPwrReminderData() == NULL) {
		if (bySerializeMode == Mode::Load) {
			wLoadRet = APP_ERROR_LOAD_PWRRMD_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == Mode::Save) && ((nSaveFlag & APPDATA_PWRREMINDER) != 0)) {
			wSaveRet = APP_ERROR_SAVE_PWRRMD_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}

	return bResult;
}


//////////////////////////////////////////////////////////////////////////
// DebugTest dialog function

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitDebugTestDlg
//	Description:	Initialize app DebugTest dialog pointer
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::InitDebugTestDlg(void)
{
	// Initialize dialog
	m_pDebugTestDlg = new CDebugTestDlg();

	// Check initialization validity
	if (m_pDebugTestDlg == NULL) {
		TRACE_ERROR("Error: DebugTest dialog init failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
	}
	else {
		m_pDebugTestDlg->Create(IDD_DEBUGTEST_DLG, NULL);
		m_pDebugTestDlg->ShowWindow(SW_HIDE);
	}

	return (m_pDebugTestDlg != NULL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetDebugTestDlg
//	Description:	Get app DebugTest dialog pointer
//  Arguments:		None
//  Return value:	SDialog*
//
//////////////////////////////////////////////////////////////////////////

SDialog* CPowerPlusApp::GetDebugTestDlg(void)
{
	return m_pDebugTestDlg;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DestroyDebugTestDlg
//	Description:	Destroy app DebugTest dialog pointer
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::DestroyDebugTestDlg(void)
{
	// Destroy DebugTest dialog
	if (m_pDebugTestDlg != NULL) {
		if (::IsWindow(m_pDebugTestDlg->GetSafeHwnd())) {
			m_pDebugTestDlg->DestroyWindow();
		}
		delete m_pDebugTestDlg;
		m_pDebugTestDlg = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// Registry functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAutoStartRegistryRootKey
//	Description:	Get registry root key for auto-start function
//  Arguments:		hAutoStartRootKey - Returned root key handle (ref-value)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::GetAutoStartRegistryRootKey(HKEY& hAutoStartRootKey)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableAutoStart
//	Description:	Enable/disable start-up with Windows function
//  Arguments:		bEnable		- Auto startup option
//					bRunAsAdmin - Run as admin option
//  Return value:	int - Result of registry writing process
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusApp::EnableAutoStart(BOOL bEnable, BOOL bRunAsAdmin)
{
	long lRes;
	HKEY hRootKey, hKey;
	DWORD dwState;
	TCHAR tcPath[MAX_PATH];
	int nRet;

	// Get root key
	GetAutoStartRegistryRootKey(hRootKey);

	// Create registry key
	lRes = RegCreateKeyEx(hRootKey, Section::AutoStart, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key creation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return 0;
	}

	// Enable auto-start
	if (bEnable == TRUE) {

		String execCommand;

		if (bRunAsAdmin == TRUE) {
			// Register to run as admin
			execCommand.Format(Constant::Command::RunAsAdmin::Register, REG_AFX_PROJECTNAME, StringUtils::GetApplicationPath(true).GetString());
			WinExec(MAKEANSI(execCommand), SW_HIDE);
		}
		else {
			// Unregister to run as admin
			execCommand.Format(Constant::Command::RunAsAdmin::Unregister, REG_AFX_PROJECTNAME);
			WinExec(MAKEANSI(execCommand), SW_HIDE);
		}

		// Register to run at startup
		GetModuleFileName(NULL, tcPath, sizeof(tcPath) / sizeof(TCHAR));
		lRes = RegSetValueEx(hKey, REG_STARTUP_VALUENAME, 0, REG_SZ, (LPBYTE)tcPath, (_tcsclen(tcPath) + 1) * sizeof(TCHAR));
		nRet = (lRes == ERROR_SUCCESS);
	}
	else {
		// Unregister to run at startup
		RegDeleteValue(hKey, REG_STARTUP_VALUENAME);
		lRes = RegQueryValueEx(hKey, REG_STARTUP_VALUENAME, 0, NULL, NULL, NULL);
		nRet = (lRes != ERROR_SUCCESS);
	}

	// Close key
	RegCloseKey(hKey);
	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAutoStartRegisterStatus
//	Description:	Check if startup with Windows is enabled or not
//  Arguments:		None
//  Return value:	int - Result of querrying process
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusApp::GetAutoStartRegisterStatus(void)
{
	long lRes;
	HKEY hRootKey, hKey;

	// Get root directory
	GetAutoStartRegistryRootKey(hRootKey);

	// Open registry key
	lRes = RegOpenKeyEx(hRootKey, Section::AutoStart, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey);
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key open failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return 0;
	}

	// Get registry key value
	lRes = RegQueryValueEx(hKey, REG_STARTUP_VALUENAME, 0, NULL, NULL, NULL);

	// Close key
	RegCloseKey(hKey);
	return (lRes == ERROR_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLastSysEventTime
//	Description:	Get last system event time
//  Arguments:		byEventType  - Event type (suspend/wakeup)
//					timeSysEvent - Event time value
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::GetLastSysEventTime(BYTE byEventType, DateTime& timeSysEvent)
{
	long lRes;
	HKEY hRegOpenKey;

	// Initialize registry info data
	RegistryInfo regInfoLastSysEvt;
	regInfoLastSysEvt.SetRootKeyName(AppProfile::Registry::RootKey);
	regInfoLastSysEvt.SetSubkeyPath(AppProfile::Registry::SubKeys);
	regInfoLastSysEvt.SetCompanyName(AppProfile::Registry::CompanyName);
	regInfoLastSysEvt.SetProductName(AppProfile::Registry::ProductID);
	regInfoLastSysEvt.SetSectionName(Section::GlobalData);

	// Set subsection
	regInfoLastSysEvt.SetSectionName(Section::GlobalData::Tracking);

	// Get registry path
	String lastSysEvtRegPath = MakeRegistryPath(regInfoLastSysEvt, RegistryPathType::includingSectionName, FALSE);

	// Open registry key
	lRes = RegOpenKeyEx(HKEY_CURRENT_USER, lastSysEvtRegPath, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hRegOpenKey);

	// Registry key open failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key open failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get key name
	String keyName;
	if (byEventType == SystemEventID::SystemSuspend) {
		// Last system suspend
		keyName = Key::GlobalData::Tracking::LastSysSuspend;
	}
	else if (byEventType == SystemEventID::SystemWakeUp) {
		// Last system wakeup
		keyName = Key::GlobalData::Tracking::LastSysWakeup;
	}
	else if (byEventType == SystemEventID::SessionEnded) {
		// Last app/system session ending
		keyName = Key::GlobalData::Tracking::LastSessionEnd;
	}
	else {
		// Close key and exit
		RegCloseKey(hRegOpenKey);
		return FALSE;
	}

	// Get registry key value
	DWORD dwType = REG_SZ;
	TCHAR tcBuffer[MAX_PATH];
	DWORD dwBufferSize = sizeof(tcBuffer);
	lRes = RegQueryValueEx(hRegOpenKey, keyName, 0, &dwType, (LPBYTE)tcBuffer, &dwBufferSize);
	
	// Get registry key value failed
	if (lRes != ERROR_SUCCESS) {
		// Trace error
		TRACE_ERROR("Error: Registry key query value failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Close key and exit
		RegCloseKey(hRegOpenKey);
		return FALSE;
	}

	SYSTEMTIME sysTimeTemp = DateTimeUtils::ToSystemTime(timeSysEvent);

	// Extract time data from string
	TCHAR tcTimePeriod[5];
	String dateTimeFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	_stscanf_s(tcBuffer, dateTimeFormat, &sysTimeTemp.wYear, &sysTimeTemp.wMonth, &sysTimeTemp.wDay,
		&sysTimeTemp.wHour, &sysTimeTemp.wMinute, &sysTimeTemp.wSecond, &sysTimeTemp.wMilliseconds, tcTimePeriod, (unsigned)_countof(tcTimePeriod));

	timeSysEvent = DateTimeUtils::FromSystemTime(sysTimeTemp);

	// Close key
	RegCloseKey(hRegOpenKey);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveLastSysEventTime
//	Description:	Save last system event time
//  Arguments:		byEventType  - Event type (suspend/wakeup)
//					timeSysEvent - Event time value
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::SaveLastSysEventTime(BYTE byEventType, const DateTime& timeSysEvent)
{
	long lRes;
	HKEY hRegOpenKey;
	DWORD dwState;
	BOOL bRet = TRUE;

	// Initialize registry info data
	RegistryInfo regInfoLastSysEvt;
	regInfoLastSysEvt.SetRootKeyName(AppProfile::Registry::RootKey);
	regInfoLastSysEvt.SetSubkeyPath(AppProfile::Registry::SubKeys);
	regInfoLastSysEvt.SetCompanyName(AppProfile::Registry::CompanyName);
	regInfoLastSysEvt.SetProductName(AppProfile::Registry::ProductID);
	regInfoLastSysEvt.SetSectionName(Section::GlobalData);

	// Set subsection
	regInfoLastSysEvt.SetSectionName(Section::GlobalData::Tracking);

	// Get registry path
	String lastSysEvtRegPath = MakeRegistryPath(regInfoLastSysEvt, RegistryPathType::includingSectionName, FALSE);

	// Create registry key (open if key exists)
	lRes = RegCreateKeyEx(HKEY_CURRENT_USER, lastSysEvtRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hRegOpenKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key creation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get key name
	String keyName;
	if (byEventType == SystemEventID::SystemSuspend) {
		// Last system suspend
		keyName = Key::GlobalData::Tracking::LastSysSuspend;
	}
	else if (byEventType == SystemEventID::SystemWakeUp) {
		// Last system wakeup
		keyName = Key::GlobalData::Tracking::LastSysWakeup;
	}
	else if (byEventType == SystemEventID::SessionEnded) {
		// Last app/system session ending
		keyName = Key::GlobalData::Tracking::LastSessionEnd;
	}
	else {
		// Close key and exit
		RegCloseKey(hRegOpenKey);
		return FALSE;
	}

	// Format date/time
	UINT nTimePeriod = (timeSysEvent.Hour() < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	const wchar_t* timePeriodFormat = GetLanguageString(GetAppLanguage(), nTimePeriod);
	const wchar_t* timeFormatString = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String dateTimeFormat = StringUtils::StringFormat(timeFormatString, timeSysEvent.Year(), timeSysEvent.Month(), timeSysEvent.Day(),
		timeSysEvent.Hour(), timeSysEvent.Minute(), timeSysEvent.Second(), timeSysEvent.Millisecond(), timePeriodFormat);

	// Save registry value
	DWORD dwDataSize = (dateTimeFormat.GetLength() + 1) * sizeof(TCHAR);
	TCHAR* pszData = new TCHAR[dwDataSize];
	_tcscpy_s(pszData, dwDataSize, dateTimeFormat);
	lRes = RegSetValueEx(hRegOpenKey, keyName, 0, REG_SZ, (LPBYTE)pszData, dwDataSize);
	delete[] pszData;

	// Close key
	RegCloseKey(hRegOpenKey);

	bRet = (lRes == ERROR_SUCCESS);
	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnExecuteDebugCommand
//	Description:	Handle event when a debug command is executed
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OnExecuteDebugCommand(WPARAM /*wParam*/, LPARAM lParam)
{
	// If debug command is empty, do nothing
	String strDebugCommand(LPARAM_TO_STRING(lParam));
	if (strDebugCommand.IsEmpty())
		return;

	// Format debug command
	strDebugCommand.ToLower();

	// Output event log
	OutputEventLog(LOG_EVENT_EXEC_DEBUGCMD, strDebugCommand);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnShowErrorMessage
//	Description:	Handle event when an error message is displayed
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OnShowErrorMessage(WPARAM wParam, LPARAM lParam)
{
	// Error code
	DWORD dwErrCode = DWORD(wParam);
	String description = StringUtils::StringFormat(_T("Error code: 0x%04X"), dwErrCode);

	// Event log detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Error code ID
		logDetailInfo.AddDetail(EventDetail::EventError, dwErrCode);

		// Message content detail info
		logDetailInfo.AddDetail(EventDetail::MessageText, LPARAM_TO_STRING(lParam));
	}

	// Output event log
	OutputEventLog(LOG_EVENT_ERROR_MESSAGE, description, &logDetailInfo);
}

