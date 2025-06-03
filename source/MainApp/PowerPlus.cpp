
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


#include "stdafx.h"

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
	SetAppLaunchTime(GetCurSysTime());

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
	SetRegistryKey(IDS_APP_REGISTRY_PROFILENAME);

	// Update application profile info data
	UpdateAppProfileInfo();

	// Create neccessary sub-folders
	CreateDirectory(GetSubFolderPath(SUBFOLDER_LOG), NULL);

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

ULONG CPowerPlusApp::DeviceNotifyCallbackRoutine(PVOID pContext, ULONG ulType, PVOID pSetting)
{
	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) 
		return ULONG(RESULT_FAILED);

	// Get current system time
	SYSTEMTIME stCurSysTime = GetCurSysTime();

	// Processing
	switch (ulType)
	{
	case PBT_APMSUSPEND:					// System suspend event
		// Save last system suspend time
		pApp->SaveLastSysEventTime(SystemEventID::SystemSuspend, stCurSysTime);
		break;

	case PBT_APMRESUMESUSPEND:				// System resume from suspend event
	case PBT_APMRESUMEAUTOMATIC:			// System automatic resume event
		// Save last system wakeup time
		pApp->SaveLastSysEventTime(SystemEventID::SystemWakeUp, stCurSysTime);
		break;

	default:
		break;
	}

	// Default return
	return ULONG(RESULT_SUCCESS);		// ERROR_SUCCESS
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
	CString strTemp = STRING_EMPTY;
	SYSTEMTIME stTimeTemp = SYSTEMTIME_ZERO;

	// Check data validity first
	if (!DataSerializeCheck(MODE_LOAD))
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
		nConfigRet += GetConfig(IDS_REGKEY_CFG_ACTIONLMB,			(int&)pcfgTempData->nLMBAction);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_ACTIONMMB,			(int&)pcfgTempData->nMMBAction);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_ACTIONRMB,			(int&)pcfgTempData->nRMBAction);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_RMBSHOWMENU,			pcfgTempData->bRMBShowMenu);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_LANGUAGEID,			(int&)pcfgTempData->nLanguageID);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SHOWATSTARTUP,		pcfgTempData->bShowDlgAtStartup);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_STARTUPENABLE,		pcfgTempData->bStartupEnabled);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_CONFIRMACTION,		pcfgTempData->bConfirmAction);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SAVEACTIONLOG,		pcfgTempData->bSaveHistoryLog);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SAVEAPPEVENTLOG,		pcfgTempData->bSaveAppEventLog);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_RUNASADMIN,			pcfgTempData->bRunAsAdmin);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SHOWERROR,			pcfgTempData->bShowErrorMsg);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SCHEDULENOTIFY,		pcfgTempData->bNotifySchedule);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_SCHEDALLOWCANCEL,	pcfgTempData->bAllowCancelSchedule);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS,		pcfgTempData->bEnableBackgroundHotkey);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_LOCKSTATEHOTKEY,		pcfgTempData->bLockStateHotkey);
		nConfigRet += GetConfig(IDS_REGKEY_CFG_ENBPWRREMINDER,		pcfgTempData->bEnablePowerReminder);

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
		SCHEDULEITEM schDefaultTemp(ScheduleData::defaultItemID);
		{
			// Read default schedule item
			int nDefSchedRet = INT_NULL;

			// Enable state
			nDefSchedRet += GetDefaultSchedule(IDS_REGKEY_SCHEDULE_ENABLE, nDataTemp);
			schDefaultTemp.EnableItem(nDataTemp);

			// Action ID
			nDefSchedRet += GetDefaultSchedule(IDS_REGKEY_SCHEDULE_ACTION, nDataTemp);
			schDefaultTemp.SetAction(nDataTemp);

			// Repeat enable state
			nDefSchedRet += GetDefaultSchedule(IDS_REGKEY_SCHEDULE_REPEAT, nDataTemp);
			schDefaultTemp.EnableRepeat(nDataTemp);

			// Repeat days
			nDefSchedRet += GetDefaultSchedule(IDS_REGKEY_SCHEDULE_REPEATDAYS, nDataTemp);
			schDefaultTemp.SetActiveDays(BYTE(nDataTemp));

			// Time value
			nDefSchedRet += GetDefaultSchedule(IDS_REGKEY_SCHEDULE_TIMEVALUE, nTimeTemp);
			if (nTimeTemp != INT_INVALID) {

				// Convert time value and set time
				stTimeTemp.wHour = GET_REGTIME_HOUR(nTimeTemp);
				stTimeTemp.wMinute = GET_REGTIME_MINUTE(nTimeTemp);
				schDefaultTemp.SetTime(stTimeTemp);

				// Reset temp data
				stTimeTemp = SYSTEMTIME_ZERO;
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
				SCHEDULEITEM& schDefaultItem = pschTempData->GetDefaultItem();
				schDefaultItem.Copy(schDefaultTemp);
			}
		}

		// Load number of extra items
		int nExtraItemNum = 0;
		bResult &= GetScheduleExtraItemNum(IDS_REGKEY_SCHEDULE_ITEMNUM, nExtraItemNum);
		if (bResult != FALSE) {

			// Read each extra item data
			for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {

				// Initialize temp item
				SCHEDULEITEM schExtraTemp;

				// Read extra item
				int nSchedRet = INT_NULL;

				// Enable state
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ENABLE, nDataTemp);
				schExtraTemp.EnableItem(nDataTemp);

				// Item ID
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ITEMID, nDataTemp);
				schExtraTemp.SetItemID(nDataTemp);

				// Action ID
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ACTION, nDataTemp);
				schExtraTemp.SetAction(nDataTemp);

				// Repeat enable state
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_REPEAT, nDataTemp);
				schExtraTemp.EnableRepeat(nDataTemp);

				// Repeat days
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_REPEATDAYS, nDataTemp);
				schExtraTemp.SetActiveDays(BYTE(nDataTemp));

				// Time value
				nSchedRet += GetScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_TIMEVALUE, nTimeTemp);
				if (nTimeTemp != INT_INVALID) {

					// Convert time value and set time
					stTimeTemp.wHour = GET_REGTIME_HOUR(nTimeTemp);
					stTimeTemp.wMinute = GET_REGTIME_MINUTE(nTimeTemp);
					schExtraTemp.SetTime(stTimeTemp);

					// Reset temp data
					stTimeTemp = SYSTEMTIME_ZERO;
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
		bResult &= GetHotkeyItemNum(IDS_REGKEY_HKEYSET_ITEMNUM, nItemNum);
		if (nItemNum > phksTempData->GetItemNum()) {
			// Limit the hotkeyset data item number
			nItemNum = phksTempData->GetItemNum();
		}

		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Initialize temp item
			HOTKEYSETITEM hksTemp;
			ZeroMemory(&hksTemp, sizeof(HOTKEYSETITEM));

			// Read item data
			int nItemRet = INT_NULL;

			// Enable state
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ENABLE, nDataTemp);
			hksTemp.EnableItem(nDataTemp);

			// Action ID
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ACTIONID, nDataTemp);
			hksTemp.SetActionID(nDataTemp);

			// Keycode
			int nCtrlKeyTemp, nFuncKeyTemp;
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_CTRLKEY, nCtrlKeyTemp);
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_FUNCKEY, nFuncKeyTemp);
			hksTemp.SetKeyCode(nCtrlKeyTemp, nFuncKeyTemp);

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
		bResult &= GetPwrReminderItemNum(IDS_REGKEY_PWRRMD_ITEMNUM, nItemNum);
		if (bResult != FALSE) {

			// Initialize temp data
			ppwrTempData->Init();

			// Read each item data
			for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

				// Initialize temp item
				PWRREMINDERITEM pwrTemp;

				// Read item data
				int nItemRet = INT_NULL;

				// Item ID
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ITEMID, nDataTemp);
				pwrTemp.SetItemID(nDataTemp);

				// Enable state
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ENABLE, nDataTemp);
				pwrTemp.EnableItem(nDataTemp);

				// Message content
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTRING, strTemp);
				pwrTemp.SetMessage(strTemp);

				// Event ID
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_EVENTID, nDataTemp);
				pwrTemp.SetEventID(nDataTemp);

				// Message style
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTYLE, nDataTemp);
				pwrTemp.SetMessageStyle(nDataTemp);

				// Repeat enable state
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEAT, nDataTemp);
				pwrTemp.EnableRepeat(nDataTemp);

				// Allow snoozing
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ALLOWSNOOZE, nDataTemp);
				pwrTemp.EnableSnoozing(nDataTemp);

				// Snooze interval
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_SNOOZEINTERVAL, nDataTemp);
				pwrTemp.SetSnoozeInterval(nDataTemp);

				// Repeat days
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEATDAYS, nDataTemp);
				pwrTemp.SetActiveDays(nDataTemp);

				// Time value
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_TIMEVALUE,	nTimeTemp);
				if (nTimeTemp != INT_INVALID) {

					// Convert time value and set time
					stTimeTemp.wHour = GET_REGTIME_HOUR(nTimeTemp);
					stTimeTemp.wMinute = GET_REGTIME_MINUTE(nTimeTemp);
					pwrTemp.SetTime(stTimeTemp);

					// Reset temp data
					stTimeTemp = SYSTEMTIME_ZERO;
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
	if (!DataSerializeCheck(MODE_SAVE, dwDataType))
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
		CONFIGDATA cfgConfigTemp;
		if (m_pcfgAppConfig != NULL) {
			m_pcfgAppConfig->GetData(cfgConfigTemp);
		}

		// Save registry data
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONLMB,		cfgConfigTemp.nLMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONMMB,		cfgConfigTemp.nMMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONRMB,		cfgConfigTemp.nRMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_RMBSHOWMENU,		cfgConfigTemp.bRMBShowMenu);
		bResult &= WriteConfig(IDS_REGKEY_CFG_LANGUAGEID,		cfgConfigTemp.nLanguageID);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SHOWATSTARTUP,	cfgConfigTemp.bShowDlgAtStartup);
		bResult &= WriteConfig(IDS_REGKEY_CFG_STARTUPENABLE,	cfgConfigTemp.bStartupEnabled);
		bResult &= WriteConfig(IDS_REGKEY_CFG_CONFIRMACTION,	cfgConfigTemp.bConfirmAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SAVEACTIONLOG,	cfgConfigTemp.bSaveHistoryLog);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SAVEAPPEVENTLOG,	cfgConfigTemp.bSaveAppEventLog);
		bResult &= WriteConfig(IDS_REGKEY_CFG_RUNASADMIN,		cfgConfigTemp.bRunAsAdmin);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SHOWERROR,		cfgConfigTemp.bShowErrorMsg);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SCHEDULENOTIFY,	cfgConfigTemp.bNotifySchedule);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SCHEDALLOWCANCEL, cfgConfigTemp.bAllowCancelSchedule);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS,	cfgConfigTemp.bEnableBackgroundHotkey);
		bResult &= WriteConfig(IDS_REGKEY_CFG_LOCKSTATEHOTKEY,	cfgConfigTemp.bLockStateHotkey);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ENBPWRREMINDER,	cfgConfigTemp.bEnablePowerReminder);

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
		SCHEDULEITEM schTempDefault = m_pschScheduleData->GetDefaultItem();
		{
			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(schTempDefault.GetTime());

			// Save registry data
			bResult &= WriteDefaultSchedule(IDS_REGKEY_SCHEDULE_ENABLE,		schTempDefault.IsEnabled());
			bResult &= WriteDefaultSchedule(IDS_REGKEY_SCHEDULE_ACTION,		schTempDefault.GetAction());
			bResult &= WriteDefaultSchedule(IDS_REGKEY_SCHEDULE_REPEAT,		schTempDefault.IsRepeatEnabled());
			bResult &= WriteDefaultSchedule(IDS_REGKEY_SCHEDULE_REPEATDAYS, schTempDefault.GetActiveDays());
			bResult &= WriteDefaultSchedule(IDS_REGKEY_SCHEDULE_TIMEVALUE,  nTimeTemp);

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
		bResult &= WriteScheduleExtraItemNum(IDS_REGKEY_SCHEDULE_ITEMNUM, nExtraItemNum);
		for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {

			// Get schedule extra item
			SCHEDULEITEM schTempExtra = m_pschScheduleData->GetItemAt(nExtraIndex);

			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(schTempExtra.GetTime());

			// Save registry data
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ENABLE,		schTempExtra.IsEnabled());
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ITEMID,		schTempExtra.GetItemID());
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_ACTION,		schTempExtra.GetAction());
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_REPEAT,		schTempExtra.IsRepeatEnabled());
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_REPEATDAYS,  schTempExtra.GetActiveDays());
			bResult &= WriteScheduleExtra(nExtraIndex, IDS_REGKEY_SCHEDULE_TIMEVALUE,	nTimeTemp);

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
		bResult &= WriteHotkeyItemNum(IDS_REGKEY_HKEYSET_ITEMNUM, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Get HotkeySet item
			HOTKEYSETITEM hksTemp = m_phksHotkeySetData->GetItemAt(nIndex);

			// Get keycode
			DWORD dwCtrlKeyTemp, dwFuncKeyTemp;
			hksTemp.GetKeyCode(dwCtrlKeyTemp, dwFuncKeyTemp);

			// Write item data
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ENABLE,	hksTemp.IsEnabled());
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ACTIONID,	hksTemp.GetActionID());
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_CTRLKEY,	dwCtrlKeyTemp);
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_FUNCKEY,	dwFuncKeyTemp);
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

		// Save registry data
		int nItemNum = m_ppwrReminderData->GetItemNum();
		bResult &= WritePwrReminderItemNum(IDS_REGKEY_PWRRMD_ITEMNUM, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Get Power Reminder item
			PWRREMINDERITEM pwrTemp = m_ppwrReminderData->GetItemAt(nIndex);

			// Convert time data
			nTimeTemp = FORMAT_REG_TIME(pwrTemp.GetTime());

			// Write item data
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ITEMID,			pwrTemp.GetItemID());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ENABLE,			pwrTemp.IsEnabled());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTRING,		pwrTemp.GetMessage());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_EVENTID,			pwrTemp.GetEventID());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_TIMEVALUE,		nTimeTemp);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTYLE,			pwrTemp.GetMessageStyle());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEAT,			pwrTemp.IsRepeatEnabled());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ALLOWSNOOZE,		pwrTemp.IsAllowSnoozing());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_SNOOZEINTERVAL,	pwrTemp.GetSnoozeInterval());
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEATDAYS,		pwrTemp.GetActiveDays());
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
	return SConfigBackup::AutoRegistryExport();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateAppProfileInfo
//	Description:	Load and update application profile info data
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::UpdateAppProfileInfo(void)
{
	BOOL bRet = FALSE;

	int nValue = (int)0;						// Integer type
	UINT uiValue = (UINT)0;						// Unsigned integer value
	CString strValue = STRING_EMPTY;			// String value

	/*------------------------<Application launch-time counter>--------------------------*/

	// Load info from registry
	if (GetProfileInfo(IDS_REGKEY_PROFILE_LAUNCHCOUNTER, nValue)) {
		SetAppLaunchTimeCounter(nValue);
		bRet = TRUE;
	}

	// Update and overwrite data
	UpdateAppLaunchTimeCounter();
	uiValue = GetAppLaunchTimeCounter();
	if (!WriteProfileInfo(IDS_REGKEY_PROFILE_LAUNCHCOUNTER, uiValue)) {
		bRet = FALSE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Application launch-time>-----------------------------*/

	// Format launch-time
	CString strDateTimeFormat;
	SYSTEMTIME stLaunchTime = GetAppLaunchTime();
	UINT nTimePeriod = (stLaunchTime.wHour < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	CString strTimePeriodFormat = GetLanguageString(LoadLanguageTable(NULL), nTimePeriod);
	strDateTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stLaunchTime.wYear, stLaunchTime.wMonth, stLaunchTime.wDay,
		stLaunchTime.wHour, stLaunchTime.wMinute, stLaunchTime.wSecond, stLaunchTime.wMilliseconds, strTimePeriodFormat);

	// Store launch-time info data
	if (!WriteProfileInfo(IDS_REGKEY_PROFILE_LAUNCHTIME, strDateTimeFormat)) {
		bRet = FALSE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*------------------------<Application directory/file info>--------------------------*/

	// Directory path (not including the executable file name)
	strValue = GetApplicationPath(FALSE);
	if (!strValue.IsEmpty()) {
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_DIRECTORY, strValue)) {
			bRet = FALSE;
		}
	}

	// Executable file name
	strValue = GetApplicationPath(TRUE);
	if (!strValue.IsEmpty()) {
		CString strFileName = PathFindFileName(strValue);
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_FILENAME, strFileName)) {
			bRet = FALSE;
		}
	}

	// Product version (full version)
	strValue = GetProductVersion(TRUE);
	if (!strValue.IsEmpty()) {
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_PRODUCTVERSION, strValue)) {
			bRet = FALSE;
		}
	}

	/*-----------------------------------------------------------------------------------*/

	/*--------------------------<Device, system and user info>---------------------------*/

	// Device name
	BOOL bRetGetInfo = GetDeviceName(strValue);
	if ((bRetGetInfo != FALSE) && (!strValue.IsEmpty())) {
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_DEVICENAME, strValue)) {
			bRet = FALSE;
		}
	}

	// User name
	bRetGetInfo = GetCurrentUserName(strValue);
	if ((bRetGetInfo != FALSE) && (!strValue.IsEmpty())) {
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_USERNAME, strValue)) {
			bRet = FALSE;
		}
	}

	// Operating system info
	OSVERSIONINFOEX osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
		// Operating system version
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_OSVERSION, osvi.dwMajorVersion)) {
			bRet = FALSE;
		}
		// Build number
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_OSBUILDNUMBER, osvi.dwBuildNumber)) {
			bRet = FALSE;
		}
		// Platform ID
		if (!WriteProfileInfo(IDS_REGKEY_PROFILE_OSPLATFORMID, osvi.dwPlatformId)) {
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

	int nGlbValue = (int)0;						// Integer type
	BOOL bGlbValue = (BOOL)0;					// Boolean type
	BYTE byGlbValue = (BYTE)0;					// Byte value
	UINT uiGlbValue = (UINT)0;					// Unsigned integer value
	DWORD dwGlbValue = (DWORD)0;				// D-WORD value
	CString strGlbValue = STRING_EMPTY;			// String value

	// Subsection name (string ID)
	UINT nSubSection = 0;

	/*------------------------<Load debugging/testing variables>-------------------------*/

	// Subsection: DebugTest
	nSubSection = IDS_REGSECTION_GLBDATA_DEBUGTEST;

	// DummyTest mode
	if (GetGlobalData(nSubSection, IDS_REGKEY_DBTEST_DUMMYTEST, nGlbValue)) {
		SetDummyTestMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug mode
	if (GetGlobalData(nSubSection, IDS_REGKEY_DBTEST_DEBUGMODE, nGlbValue)) {
		SetDebugMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug log output target
	if (GetGlobalData(nSubSection, IDS_REGKEY_DBTEST_DBLOGOUTPUT, nGlbValue)) {
		SetDebugOutputTarget(nGlbValue);
		bRet |= TRUE;
	}
	// Test feature enable
	if (GetGlobalData(nSubSection, IDS_REGKEY_DBTEST_TESTFEATURE, nGlbValue)) {
		SetTestFeatureEnable(nGlbValue);
		bRet |= TRUE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Load app special flags>------------------------------*/

	// Subsection: AppFlags
	nSubSection = IDS_REGSECTION_GLBDATA_APPFLAGS;

	// Power action trace flag
	if (GetGlobalData(nSubSection, IDS_REGKEY_APPFLAG_PWRACTIONFLG, nGlbValue)) {
		SetPwrActionFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// System suspended trace flag
	if (GetGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SYSTEMSUSPENDFLG, nGlbValue)) {
		SetSystemSuspendFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// Session ending trace flag
	if (GetGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SESSIONENDFLAG, nGlbValue)) {
		SetSessionEndFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// Previously safe termination trace flag
	if (GetGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SAFETERMINATION, nGlbValue)) {
		SetSafeTerminationFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	/*-----------------------------------------------------------------------------------*/

	/*-------------------------<Load special feature variables>--------------------------*/

	// Subsection: Features
	nSubSection = IDS_REGSECTION_GLBDATA_FEATURES;

	// Reminder message background color
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_BKGRDCLR, nGlbValue)) {
		SetReminderMsgBkgrdColor((DWORD)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message text color
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_TXTCLR, nGlbValue)) {
		SetReminderMsgTextColor((DWORD)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message font name
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_FONTNAME, strGlbValue)) {
		SetReminderMsgFontName(strGlbValue);
		bRet |= TRUE;
	}
	// Reminder message font size
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_FONTSIZE, nGlbValue)) {
		SetReminderMsgFontSize((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message auto-close interval
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_TIMEOUT, nGlbValue)) {
		SetReminderMsgTimeout((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon ID
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONID, nGlbValue)) {
		SetReminderMsgIconID(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon size
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONSIZE, nGlbValue)) {
		SetReminderMsgIconSize(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon position
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONPOS, nGlbValue)) {
		SetReminderMsgIconPosition(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message display position
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_MSGDISPPOS, nGlbValue)) {
		SetReminderMsgDispPosition(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message display area horizontal margin
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_HMARGIN, nGlbValue)) {
		SetReminderMsgHMargin((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message display area vertical margin
	if (GetGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_VMARGIN, nGlbValue)) {
		SetReminderMsgVMargin((UINT)nGlbValue);
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

	int nGlbValue = (int)0;						// Integer type
	BOOL bGlbValue = (BOOL)0;					// Boolean type
	BYTE byGlbValue = (BYTE)0;					// Byte value
	UINT uiGlbValue = (UINT)0;					// Unsigned integer value
	DWORD dwGlbValue = (DWORD)0;				// D-WORD value
	CString strGlbValue = STRING_EMPTY;			// String value

	// Subsection name (string ID)
	UINT nSubSection = 0;

	/*------------------------<Save debugging/testing variables>-------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBDATA_CATE_DEBUGTEST)) {
		// Subsection: DebugTest
		nSubSection = IDS_REGSECTION_GLBDATA_DEBUGTEST;

		// DummyTest mode
		nGlbValue = GetDummyTestMode();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_DBTEST_DUMMYTEST, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug mode
		nGlbValue = GetDebugMode();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_DBTEST_DEBUGMODE, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug log output target
		nGlbValue = GetDebugOutputTarget();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_DBTEST_DBLOGOUTPUT, nGlbValue)) {
			bRet = FALSE;
		}
		// Test feature enable
		nGlbValue = GetTestFeatureEnable();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_DBTEST_TESTFEATURE, nGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	/*---------------------------------<Save app flags>----------------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBDATA_CATE_APPFLAGS)) {
		// Subsection: AppFlags
		nSubSection = IDS_REGSECTION_GLBDATA_APPFLAGS;

		// Power action trace flag
		byGlbValue = GetPwrActionFlag();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_APPFLAG_PWRACTIONFLG, byGlbValue)) {
			bRet = FALSE;
		}

		// System suspended trace flag
		byGlbValue = GetSystemSuspendFlag();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SYSTEMSUSPENDFLG, byGlbValue)) {
			bRet = FALSE;
		}

		// Session ending trace flag
		byGlbValue = GetSessionEndFlag();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SESSIONENDFLAG, byGlbValue)) {
			bRet = FALSE;
		}

		// Previously safe termination trace flag
		byGlbValue = GetSafeTerminationFlag();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_APPFLAG_SAFETERMINATION, byGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Save special variables>------------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBDATA_CATE_FEATURES)) {
		// Subsection: Features
		nSubSection = IDS_REGSECTION_GLBDATA_FEATURES;

		// Reminder message background color
		dwGlbValue = GetReminderMsgBkgrdColor();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_BKGRDCLR, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message text color
		dwGlbValue = GetReminderMsgTextColor();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_TXTCLR, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message font name
		bGlbValue = GetReminderMsgFontName(strGlbValue);
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_FONTNAME, strGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message font size
		dwGlbValue = GetReminderMsgFontSize();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_FONTSIZE, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message auto-close interval
		dwGlbValue = GetReminderMsgTimeout();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_TIMEOUT, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon ID
		dwGlbValue = GetReminderMsgIconID();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONID, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon size
		dwGlbValue = GetReminderMsgIconSize();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONSIZE, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon position
		dwGlbValue = GetReminderMsgIconPosition();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_ICONPOS, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message display position
		dwGlbValue = GetReminderMsgDispPosition();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_MSGDISPPOS, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message display area horizontal margin
		dwGlbValue = GetReminderMsgHMargin();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_HMARGIN, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message display area vertical margin
		dwGlbValue = GetReminderMsgVMargin();
		if (!WriteGlobalData(nSubSection, IDS_REGKEY_FEATURE_RMDMSG_VMARGIN, dwGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	return bRet;
}

#ifdef _CONFIG_FILE_TEST_ACTIVE
//////////////////////////////////////////////////////////////////////////
// File data serialization functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitFileData
//	Description:	Initialize file app data structure
//  Arguments:		None
//  Return value:	None
//  Notes:			Test function, temporarily disabled
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::InitFileData()
{
	INISection sectionConfig;
	sectionConfig.SetName(IDS_REGSECTION_CONFIG);
	sectionConfig.AddKey(IDS_REGKEY_CFG_ACTIONLMB);
	sectionConfig.AddKey(IDS_REGKEY_CFG_ACTIONMMB);
	sectionConfig.AddKey(IDS_REGKEY_CFG_ACTIONRMB);
	sectionConfig.AddKey(IDS_REGKEY_CFG_RMBSHOWMENU);
	sectionConfig.AddKey(IDS_REGKEY_CFG_LANGUAGEID);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SHOWATSTARTUP);
	sectionConfig.AddKey(IDS_REGKEY_CFG_STARTUPENABLE);
	sectionConfig.AddKey(IDS_REGKEY_CFG_CONFIRMACTION);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SAVEACTIONLOG);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SAVEAPPEVENTLOG);
	sectionConfig.AddKey(IDS_REGKEY_CFG_RUNASADMIN);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SHOWERROR);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SCHEDULENOTIFY);
	sectionConfig.AddKey(IDS_REGKEY_CFG_SCHEDALLOWCANCEL);
	sectionConfig.AddKey(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS);
	sectionConfig.AddKey(IDS_REGKEY_CFG_ENBPWRREMINDER);

	INISection sectionSchedule;
	sectionSchedule.SetName(IDS_REGSECTION_SCHEDULE);
	sectionSchedule.AddKey(IDS_REGKEY_SCHED_ENABLE);
	sectionSchedule.AddKey(IDS_REGKEY_SCHED_ACTION);
	sectionSchedule.AddKey(IDS_REGKEY_SCHED_REPEAT);
	sectionSchedule.AddKey(IDS_REGKEY_SCHED_TIMEVALUE);

	INISection sectionHotkeySet;
	sectionHotkeySet.SetName(IDS_REGSECTION_HOTKEYSET);
	sectionHotkeySet.AddKey(IDS_REGKEY_HKEYSET_ITEMNUM);

	CArray<INISection, INISection> arrSectionHKItem;
	arrSectionHKItem.RemoveAll();
	for (int nIndex = 0; nIndex < m_phksHotkeySetData->nItemNum; nIndex++) {
		INISection sectionHKItem;
		CString strFullName = INIFile::MakeSubSectionName(sectionHotkeySet, IDS_REGSECTION_HKEYITEMID, nIndex);
		sectionHKItem.SetName(strFullName);
		sectionHKItem.AddKey(IDS_REGKEY_HKEYSET_ENABLE);
		sectionHKItem.AddKey(IDS_REGKEY_HKEYSET_ACTIONID);
		sectionHKItem.AddKey(IDS_REGKEY_HKEYSET_CTRLKEY);
		sectionHKItem.AddKey(IDS_REGKEY_HKEYSET_FUNCKEY);
		
		arrSectionHKItem.Add(sectionHKItem);
	}

	// Init config file data
	m_fileConfigData.SetName(FILE_APP_CONFIG);
	m_fileConfigData.SetSection(sectionConfig);
	m_fileConfigData.SetSection(sectionSchedule);
	m_fileConfigData.SetSection(sectionHotkeySet);

	for (int nIndex = 0; nIndex < arrSectionHKItem.GetSize(); nIndex++) {
		m_fileConfigData.SetSection(arrSectionHKItem.GetAt(nIndex));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadFileAppData
//	Description:	Load file app data
//  Arguments:		None
//  Return value:	None
//  Notes:			Test function, temporarily disabled
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::LoadFileAppData()
{
	INISection& sectionConfig = m_fileConfigData.GetSectionByName(IDS_REGSECTION_CONFIG);
	INISection& sectionSchedule = m_fileConfigData.GetSectionByName(IDS_REGSECTION_SCHEDULE);

	int nTemp = DEF_INTEGER_INVALID;

	/*---------- Load app config data ----------*/

	// Left mouse button action
	m_pcfgAppConfig->nLMBAction = DEF_APP_ACTION_DISPLAYOFF;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_ACTIONLMB);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->nLMBAction = nTemp;
	}
	// Middle mouse button action
	m_pcfgAppConfig->nMMBAction = DEF_APP_ACTION_SLEEP;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_ACTIONMMB);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->nMMBAction = nTemp;
	}
	// Right mouse button action
	m_pcfgAppConfig->nRMBAction = DEF_APP_ACTION_SHOWMENU;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_ACTIONRMB);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->nLMBAction = nTemp;
	}
	// Right mouse button: Only show menu
	m_pcfgAppConfig->bRMBShowMenu = TRUE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_RMBSHOWMENU);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bRMBShowMenu = nTemp;
	}
	// Language setting
	m_pcfgAppConfig->nLanguageID = APP_LANGUAGE_ENGLISH;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_LANGUAGEID);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->nLanguageID = nTemp;
	}
	// Show dialog at startup
	m_pcfgAppConfig->bShowDlgAtStartup = TRUE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SHOWATSTARTUP);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bShowDlgAtStartup = nTemp;
	}
	// Startup with Windows
	m_pcfgAppConfig->bStartupEnabled = TRUE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_STARTUPENABLE);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bStartupEnabled = nTemp;
	}
	// Show confirm message before doing action
	m_pcfgAppConfig->bConfirmAction = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_CONFIRMACTION);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bConfirmAction = nTemp;
	}
	// Save action log
	m_pcfgAppConfig->bSaveActionLog = TRUE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SAVEACTIONLOG);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bSaveActionLog = nTemp;
	}
	// Save app log
	m_pcfgAppConfig->bSaveAppEventLog = TRUE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SAVEAPPEVENTLOG);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bSaveAppEventLog = nTemp;
	}
	// Run with admin privileges
	m_pcfgAppConfig->bRunAsAdmin = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_RUNASADMIN);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bRunAsAdmin = nTemp;
	}
	// Show action error message
	m_pcfgAppConfig->bShowErrorMsg = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SHOWERROR);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bShowErrorMsg = nTemp;
	}
	// Show notify tip for schedule action
	m_pcfgAppConfig->bNotifySchedule = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SCHEDULENOTIFY);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bNotifySchedule = nTemp;
	}
	// Allow canceling schedule when notify
	m_pcfgAppConfig->bAllowCancelSchedule = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_SCHEDALLOWCANCEL);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bAllowCancelSchedule = nTemp;
	}
	// Enable background action hotkeys
	m_pcfgAppConfig->bEnableBackgroundHotkey = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bEnableBackgroundHotkey = nTemp;
	}
	// Enable Power Peminder feature
	m_pcfgAppConfig->bEnablePowerReminder = FALSE;
	nTemp = sectionConfig.GetKeyValueInt(IDS_REGKEY_CFG_ENBPWRREMINDER);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pcfgAppConfig->bEnablePowerReminder = nTemp;
	}

	/*---------- Load app schedule data ----------*/

	// Enable/disable status
	m_pschSheduleData->bEnable = FALSE;
	nTemp = sectionSchedule.GetKeyValueInt(IDS_REGKEY_SCHED_ENABLE);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pschSheduleData->bEnable = nTemp;
	}
	// Repeat daily
	m_pschSheduleData->bRepeat = FALSE;
	nTemp = sectionSchedule.GetKeyValueInt(IDS_REGKEY_SCHED_REPEAT);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pschSheduleData->bRepeat = nTemp;
	}
	// Schedule action
	m_pschSheduleData->nAction = DEF_APP_ACTION_NOTHING;
	nTemp = sectionSchedule.GetKeyValueInt(IDS_REGKEY_SCHED_ACTION);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pschSheduleData->nAction = nTemp;
	}
	// Schedule time
	m_pschSheduleData->stTime = GetCurSysTime();
	nTemp = sectionSchedule.GetKeyValueInt(IDS_REGKEY_SCHED_TIMEVALUE);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_pschSheduleData->stTime.wHour = WORD(nTemp / 100);	// Hour
		m_pschSheduleData->stTime.wMinute = WORD(nTemp % 100);	// Minute
	}

	/*---------- Load app hotkeyset data ----------*/

	INISection& sectionHotkeySet = m_fileConfigData.GetSectionByName(IDS_REGSECTION_HOTKEYSET);

	// Number of hotkeyset items
	int nItemNum = DEF_INTEGER_INVALID;
	nTemp = sectionHotkeySet.GetKeyValueInt(IDS_REGKEY_HKEYSET_ITEMNUM);
	if (nTemp > DEF_INTEGER_INVALID) {
		m_phksHotkeySetData->nItemNum = nTemp;
		nItemNum = m_phksHotkeySetData->nItemNum;
	}
	// HotkeySet list
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		INISection& sectionHKItem = m_fileConfigData.GetSubSectionByName(sectionHotkeySet, IDS_REGSECTION_HKEYITEMID, nIndex);
		HOTKEYSETITEM hksTemp = { 0 };
		// Hotkey enabled / disabled
		nTemp = sectionHKItem.GetKeyValueInt(IDS_REGKEY_HKEYSET_ENABLE);
		if (nTemp > DEF_INTEGER_INVALID) {
			hksTemp.bEnable = nTemp;
		}
		// Hotkey action ID
		nTemp = sectionHKItem.GetKeyValueInt(IDS_REGKEY_HKEYSET_ACTIONID);
		if (nTemp > DEF_INTEGER_INVALID) {
			hksTemp.nHKActionID = nTemp;
		}
		// Control Keycode #1
		nTemp = sectionHKItem.GetKeyValueInt(IDS_REGKEY_HKEYSET_CTRLKEY);
		if (nTemp > DEF_INTEGER_INVALID) {
			hksTemp.dwCtrlKeyCode = nTemp;
		}
		// Function Keycode #2
		nTemp = sectionHKItem.GetKeyValueInt(IDS_REGKEY_HKEYSET_FUNCKEY);
		if (nTemp > DEF_INTEGER_INVALID) {
			hksTemp.dwFuncKeyCode = nTemp;
		}

		// Update data
		m_phksHotkeySetData->Update(hksTemp);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetFileAppData
//	Description:	Set file app data (to write)
//  Arguments:		None
//  Return value:	None
//  Notes:			Test function, temporarily disabled
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetFileAppData()
{
	int nTemp = DEF_INTEGER_INVALID;

	INISection& sectionConfig = m_fileConfigData.GetSectionByName(IDS_REGSECTION_CONFIG);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_ACTIONLMB, m_pcfgAppConfig->nLMBAction);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_ACTIONMMB, m_pcfgAppConfig->nMMBAction);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_ACTIONRMB, m_pcfgAppConfig->nRMBAction);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_LANGUAGEID, m_pcfgAppConfig->nLanguageID);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_RMBSHOWMENU, m_pcfgAppConfig->bRMBShowMenu);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SHOWATSTARTUP, m_pcfgAppConfig->bShowDlgAtStartup);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_STARTUPENABLE, m_pcfgAppConfig->bStartupEnabled);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_CONFIRMACTION, m_pcfgAppConfig->bConfirmAction);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SAVEACTIONLOG, m_pcfgAppConfig->bSaveActionLog);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SAVEAPPEVENTLOG, m_pcfgAppConfig->bSaveAppEventLog);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_RUNASADMIN, m_pcfgAppConfig->bRunAsAdmin);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SHOWERROR, m_pcfgAppConfig->bShowErrorMsg);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SCHEDULENOTIFY, m_pcfgAppConfig->bNotifySchedule);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_SCHEDALLOWCANCEL, m_pcfgAppConfig->bAllowCancelSchedule);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS, m_pcfgAppConfig->bEnableBackgroundHotkey);
	sectionConfig.SetKeyValue(IDS_REGKEY_CFG_ENBPWRREMINDER, m_pcfgAppConfig->bEnablePowerReminder);

	INISection& sectionSchedule = m_fileConfigData.GetSectionByName(IDS_REGSECTION_SCHEDULE);
	sectionSchedule.SetKeyValue(IDS_REGKEY_SCHED_ENABLE, m_pschSheduleData->bEnable);
	sectionSchedule.SetKeyValue(IDS_REGKEY_SCHED_ACTION, m_pschSheduleData->nAction);
	sectionSchedule.SetKeyValue(IDS_REGKEY_SCHED_REPEAT, m_pschSheduleData->bRepeat);
	nTemp = int((m_pschSheduleData->stTime.wHour * 100) + m_pschSheduleData->stTime.wMinute);
	sectionSchedule.SetKeyValue(IDS_REGKEY_SCHED_TIMEVALUE, nTemp);

	INISection& sectionHotkeySet = m_fileConfigData.GetSectionByName(IDS_REGSECTION_HOTKEYSET);
	sectionHotkeySet.SetKeyValue(IDS_REGKEY_HKEYSET_ITEMNUM, m_phksHotkeySetData->nItemNum);

	for (int nIndex = 0; nIndex < m_phksHotkeySetData->nItemNum; nIndex++) {
		INISection& sectionHKItem = m_fileConfigData.GetSubSectionByName(sectionHotkeySet, IDS_REGSECTION_HKEYITEMID, nIndex);
		sectionHKItem.SetKeyValue(IDS_REGKEY_HKEYSET_ENABLE, m_phksHotkeySetData->GetItemAt(nIndex).bEnable);
		sectionHKItem.SetKeyValue(IDS_REGKEY_HKEYSET_ACTIONID, m_phksHotkeySetData->GetItemAt(nIndex).nHKActionID);
		sectionHKItem.SetKeyValue(IDS_REGKEY_HKEYSET_CTRLKEY, m_phksHotkeySetData->GetItemAt(nIndex).dwCtrlKeyCode);
		sectionHKItem.SetKeyValue(IDS_REGKEY_HKEYSET_FUNCKEY, m_phksHotkeySetData->GetItemAt(nIndex).dwFuncKeyCode);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReadFile
//	Description:	Read file app data
//  Arguments:		None
//  Return value:	BOOL - Result of file reading process
//  Notes:			Test function, temporarily disabled
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::ReadFile()
{
	SIniHandler hIniHandler;
	BOOL bRet = hIniHandler.ReadFile(m_fileConfigData);
	if (bRet == FALSE) {
		TraceLog("Read config file failed");
		return bRet;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteFile
//	Description:	Write file app data
//  Arguments:		None
//  Return value:	Result of file writing process
//  Notes:			Test function, temporarily disabled
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::WriteFile()
{
	SIniHandler hIniHandler;
	BOOL bRet = hIniHandler.WriteFile(m_fileConfigData);
	if (bRet == FALSE) {
		TraceLog("Write config file failed");
		return bRet;
	}

	return bRet;
}
#endif

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

#ifdef _LEGACY_FEATURE
	// Output data change log
	OutputDataChangeLog(BakData);
#endif
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

#ifdef _LEGACY_FEATURE
	// Output data change log
	OutputDataChangeLog(BakData);
#endif
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

#ifdef _LEGACY_FEATURE
	// Output data change log
	OutputDataChangeLog(BakData);
#endif
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

#ifdef _LEGACY_FEATURE
	// Output data change log
	OutputDataChangeLog(BakData);
#endif
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

#ifdef _LEGACY_FEATURE
//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output change log of config data
//  Arguments:		BakData - Backed up config data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputDataChangeLog(CONFIGDATA& BakData)
{
	// Get current data
	CONFIGDATA CurData;
	CurData.Copy(*GetAppConfigData());

	BYTE byDataType;
	BYTE byDataCate = DATACATE_APPCONFIG;
	DWORD dwCtrlID = DEF_INTEGER_NULL;
	SLogging* ptrLog = GetAppEventLog();
	if (ptrLog == NULL) return;

	// Compare data and output change log
	if (CurData.nLMBAction != BakData.nLMBAction) {
		byDataType = DATATYPE_OPTION_VALUE;
		dwCtrlID = IDC_LMBACTION_LIST;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nLMBAction, CurData.nLMBAction, 0);
	}
	if (CurData.nMMBAction != BakData.nMMBAction) {
		byDataType = DATATYPE_OPTION_VALUE;
		dwCtrlID = IDC_MMBACTION_LIST;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nMMBAction, CurData.nMMBAction, 0);
	}
	if (CurData.nRMBAction != BakData.nRMBAction) {
		byDataType = DATATYPE_OPTION_VALUE;
		dwCtrlID = IDC_RMBACTION_LIST;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nRMBAction, CurData.nRMBAction, 0);
	}
	if (CurData.bRMBShowMenu != BakData.bRMBShowMenu) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_ENABLERMBMENU_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bRMBShowMenu, CurData.bRMBShowMenu, 0);
	}
	if (CurData.nLanguageID != BakData.nLanguageID) {
		byDataType = DATATYPE_OPTION_VALUE;
		dwCtrlID = IDC_LANGUAGE_LIST;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nLanguageID, CurData.nLanguageID, 0);
	}
	if (CurData.bShowDlgAtStartup != BakData.bShowDlgAtStartup) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SHOWATSTARTUP_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bShowDlgAtStartup, CurData.bShowDlgAtStartup, 0);
	}
	if (CurData.bStartupEnabled != BakData.bStartupEnabled) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_ENABLEAUTOSTART_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bStartupEnabled, CurData.bStartupEnabled, 0);
	}
	if (CurData.bConfirmAction != BakData.bConfirmAction) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_CONFIRMACTION_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bConfirmAction, CurData.bConfirmAction, 0);
	}
	if (CurData.bSaveActionLog != BakData.bSaveActionLog) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SAVEACTIONLOG_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bSaveActionLog, CurData.bSaveActionLog, 0);
	}
	if (CurData.bSaveAppEventLog != BakData.bSaveAppEventLog) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SAVEAPPEVENTLOG_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bSaveAppEventLog, CurData.bSaveAppEventLog, 0);
	}
	if (CurData.bRunAsAdmin != BakData.bRunAsAdmin) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_RUNASADMIN_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bRunAsAdmin, CurData.bRunAsAdmin, 0);
	}
	if (CurData.bShowErrorMsg != BakData.bShowErrorMsg) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SHOWERROR_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bShowErrorMsg, CurData.bShowErrorMsg, 0);
	}
	if (CurData.bNotifySchedule != BakData.bNotifySchedule) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SCHEDNOTIFY_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bNotifySchedule, CurData.bNotifySchedule, 0);
	}
	if (CurData.bAllowCancelSchedule != BakData.bAllowCancelSchedule) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SCHEDALLOWCANCEL_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bAllowCancelSchedule, CurData.bAllowCancelSchedule, 0);
	}
	if (CurData.bEnableBackgroundHotkey != BakData.bEnableBackgroundHotkey) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_ENBBKGRDHOTKEYS_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bEnableBackgroundHotkey, CurData.bEnableBackgroundHotkey, 0);
	}
	if (CurData.bEnablePowerReminder != BakData.bEnablePowerReminder) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_ENBPWRREMINDER_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bEnablePowerReminder, CurData.bEnablePowerReminder, 0);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output change log of schedule data
//  Arguments:		BakData - Backed up schedule data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputDataChangeLog(SCHEDULEDATA& BakData)
{
	// Get current data
	SCHEDULEDATA CurData;
	CurData.Copy(*GetAppScheduleData());

	BYTE byDataType;
	BYTE byDataCate = DATACATE_SCHEDULE;
	DWORD dwCtrlID = DEF_INTEGER_NULL;
	SLogging* ptrLog = GetAppEventLog();
	if (ptrLog == NULL) return;

#ifdef _TEST
	// Compare data and output change log
	if (CurData.bEnable != BakData.bEnable) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SCHEDULE_ENABLE_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bEnable, CurData.bEnable, 0);
	}
	if (CurData.bRepeat != BakData.bRepeat) {
		byDataType = DATATYPE_YESNO_VALUE;
		dwCtrlID = IDC_SCHEDULE_REPEATDAILY_CHK;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.bRepeat, CurData.bRepeat, 0);
	}
	if (CurData.nAction != BakData.nAction) {
		byDataType = DATATYPE_OPTION_VALUE;
		dwCtrlID = IDC_SCHEDULE_ACTION_LIST;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nAction, CurData.nAction, 0);
	}
	if ((CurData.stTime.wHour != BakData.stTime.wHour) ||
		(CurData.stTime.wMinute != BakData.stTime.wMinute)) {
		byDataType = DATATYPE_TIME_VALUE;
		dwCtrlID = IDC_SCHEDULE_TIME_EDITBOX;
		DWORD dwPreTime, dwAftTime;
		Time2SpinPos(BakData.stTime, (int&)dwPreTime);
		Time2SpinPos(CurData.stTime, (int&)dwAftTime);
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, dwPreTime, dwAftTime, 0);
	}
#endif
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output change log of HotkeySet data
//  Arguments:		BakData - Backed up HotkeySet data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputDataChangeLog(HOTKEYSETDATA& BakData)
{
	// Get current data
	HOTKEYSETDATA CurData;
	CurData.Copy(*GetAppHotkeySetData());

	BYTE byDataType;
	BYTE byDataCate = DATACATE_HOTKEYSET;
	DWORD dwCtrlID = DEF_INTEGER_NULL;
	CString strFlagName = DEF_STRING_EMPTY;
	SLogging* ptrLog = GetAppEventLog();
	if (ptrLog == NULL) return;

	// Compare data and output change log
	if (CurData.GetItemNum() != BakData.GetItemNum()) {
		// Number of items changed
		byDataType = (CurData.GetItemNum() > BakData.GetItemNum()) ? DATATYPE_ITEMNUM_ADD : DATATYPE_ITEMNUM_REMOVE;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.GetItemNum(), CurData.GetItemNum(), 0);
	}
	int nItemNum = (CurData.GetItemNum() > BakData.GetItemNum()) ? (CurData.GetItemNum()) : (BakData.GetItemNum());
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (nIndex >= CurData.GetItemNum()) {
			// Item removed
			continue;
		}
		else if (nIndex >= BakData.GetItemNum()) {
			// Item added
			continue;
		}

		// Get data
		HOTKEYSETITEM CurItem = CurData.GetItemAt(nIndex);
		HOTKEYSETITEM BakItem = BakData.GetItemAt(nIndex);

		// Output item detail change log
		if (CurItem.nHKActionID != BakItem.nHKActionID) {
			byDataType = DATATYPE_OPTION_VALUE;
			strFlagName.Format(IDS_REGSECTION_HKEYITEMID, nIndex);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakItem.nHKActionID, CurItem.nHKActionID, strFlagName);
		}
		if (CurItem.bEnable != BakItem.bEnable) {
			byDataType = DATATYPE_YESNO_VALUE;
			strFlagName.Format(IDS_REGSECTION_HKEYITEMID, nIndex);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakItem.bEnable, CurItem.bEnable, strFlagName);
		}
		if ((CurItem.dwCtrlKeyCode != BakItem.dwCtrlKeyCode) ||
			(CurItem.dwFuncKeyCode != BakItem.dwFuncKeyCode)) {
			byDataType = DATATYPE_SPEC_VALUE;
			strFlagName.Format(IDS_REGSECTION_HKEYITEMID, nIndex);
			CString strPreKeyStrokes = DEF_STRING_EMPTY;
			if (BakItem.dwCtrlKeyCode & MOD_CONTROL)	strPreKeyStrokes += _T("Ctrl + ");
			if (BakItem.dwCtrlKeyCode & MOD_ALT)		strPreKeyStrokes += _T("Alt + ");
			if (BakItem.dwCtrlKeyCode & MOD_WIN)		strPreKeyStrokes += _T("Win + ");
			strPreKeyStrokes += GetPairedString(strplFuncKeyList, BakItem.dwFuncKeyCode);
			CString strAftKeyStrokes = DEF_STRING_EMPTY;
			if (CurItem.dwCtrlKeyCode & MOD_CONTROL)	strAftKeyStrokes += _T("Ctrl + ");
			if (CurItem.dwCtrlKeyCode & MOD_ALT)		strAftKeyStrokes += _T("Alt + ");
			if (CurItem.dwCtrlKeyCode & MOD_WIN)		strAftKeyStrokes += _T("Win + ");
			strAftKeyStrokes += GetPairedString(strplFuncKeyList, CurItem.dwFuncKeyCode);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, strPreKeyStrokes, strAftKeyStrokes, strFlagName);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputDataChangeLog
//	Description:	Output change log of Power Reminder data
//  Arguments:		BakData - Backed up Power Reminder data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputDataChangeLog(PWRREMINDERDATA& BakData)
{
	// Get current data
	PWRREMINDERDATA CurData;
	CurData.Copy(*GetAppPwrReminderData());

	BYTE byDataType;
	BYTE byDataCate = DATACATE_PWRREMINDER;
	DWORD dwCtrlID = DEF_INTEGER_NULL;
	CString strFlagName = DEF_STRING_EMPTY;
	SLogging* ptrLog = GetAppEventLog();
	if (ptrLog == NULL) return;

	// Compare data and output change log
	if (CurData.GetItemNum() != BakData.GetItemNum()) {
		// Number of items changed
		byDataType = (CurData.GetItemNum() > BakData.GetItemNum()) ? DATATYPE_ITEMNUM_ADD : DATATYPE_ITEMNUM_REMOVE;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.GetItemNum(), CurData.GetItemNum(), 0);
	}
	int nItemNum = (CurData.GetItemNum() > BakData.GetItemNum()) ? (CurData.GetItemNum()) : (BakData.GetItemNum());
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (nIndex >= CurData.GetItemNum()) {
			// Item removed
			continue;
		}
		else if (nIndex >= BakData.GetItemNum()) {
			// Item added
			continue;
		}

		// Get data
		PWRREMINDERITEM CurItem = CurData.GetItemAt(nIndex);
		PWRREMINDERITEM BakItem = BakData.GetItemAt(nIndex);

		// Output item detail change log
		if (CurItem.bEnable != BakItem.bEnable) {
			byDataType = DATATYPE_YESNO_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakItem.bEnable, CurItem.bEnable, strFlagName);
		}
		if (CurItem.strMessage != BakItem.strMessage) {
			byDataType = DATATYPE_STRING_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakItem.strMessage, CurItem.strMessage, strFlagName);
		}
		if (CurItem.nEventID != BakItem.nEventID) {
			byDataType = DATATYPE_OPTION_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			int nTemp = GetPairedID(idplPwrReminderEvt, CurItem.nEventID);
			CString strCurEvent = GetLanguageString(GetAppLanguage(), nTemp);
			if (CurItem.nEventID == PREVT_AT_SETTIME) {
				// Format time string
				CString strFormat = strCurEvent;
				strCurEvent = FormatDispTime(GetAppLanguage(), strFormat, CurItem.stTime);
			}
			nTemp = GetPairedID(idplPwrReminderEvt, BakItem.nEventID);
			CString strBakEvent = GetLanguageString(GetAppLanguage(), nTemp);
			if (BakItem.nEventID == PREVT_AT_SETTIME) {
				// Format time string
				CString strFormat = strBakEvent;
				strBakEvent = FormatDispTime(GetAppLanguage(), strFormat, BakItem.stTime);
			}
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, strBakEvent, strCurEvent, strFlagName);
		}
		if (CurItem.dwMsgStyle != BakItem.dwMsgStyle) {
			byDataType = DATATYPE_OPTION_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			int nTemp = GetPairedID(idplPwrReminderStyle, CurItem.dwMsgStyle);
			CString strCurStyle = GetLanguageString(GetAppLanguage(), nTemp);
			nTemp = GetPairedID(idplPwrReminderStyle, BakItem.dwMsgStyle);
			CString strBakStyle = GetLanguageString(GetAppLanguage(), nTemp);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, strBakStyle, strCurStyle, strFlagName);
		}
		if (CurItem.IsRepeatEnable() != BakItem.IsRepeatEnable()) {
			byDataType = DATATYPE_YESNO_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakItem.IsRepeatEnable(), CurItem.IsRepeatEnable(), strFlagName);
		}
	}
}
#endif

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
	CString strTrcTitle = STRING_EMPTY;
	CString strTrcLogFormat = STRING_EMPTY;
	LPCTSTR lpszDataNull = _T("Data pointer is NULL");
	LPCTSTR lpszReadFailed = _T("Registry data is unreadable or invalid");
	LPCTSTR lpszWriteFailed = _T("Unable to write registry data");

	// Serialization trace skip flag
	// Note: If the application is launching for the 1st time, do not trace data loading error
	BOOL bSkipFlag = FALSE;
	
	switch (wErrCode)
	{
	case APP_ERROR_LOAD_CFG_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Load config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_LOAD_CFG_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		strTrcTitle = _T("Load config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;

	case APP_ERROR_LOAD_SCHED_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Load schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_LOAD_SCHED_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		strTrcTitle = _T("Load schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;

	case APP_ERROR_LOAD_HKEYSET_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Load hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_LOAD_HKEYSET_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		strTrcTitle = _T("Load hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;

	case APP_ERROR_LOAD_PWRRMD_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Load reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_LOAD_PWRRMD_FAILED:
		bSkipFlag = IsAppFirstLaunch();
		strTrcTitle = _T("Load reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;

	case APP_ERROR_SAVE_CFG_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_SAVE_CFG_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;

	case APP_ERROR_SAVE_SCHED_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_SAVE_SCHED_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;

	case APP_ERROR_SAVE_HKEYSET_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_SAVE_HKEYSET_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;

	case APP_ERROR_SAVE_PWRRMD_INVALID:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;

	case APP_ERROR_SAVE_PWRRMD_FAILED:
		bSkipFlag = FALSE;							// Do not skip
		strTrcTitle = _T("Save reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;
	}

	// If skip flag is triggered
	if (bSkipFlag != FALSE)
		return;

	// Output trace error log
	if (!strTrcLogFormat.IsEmpty()) {
		TRACE_ERROR(strTrcLogFormat);
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
		if (bySerializeMode == MODE_LOAD) {
			wLoadRet = APP_ERROR_LOAD_CFG_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == MODE_SAVE) && ((nSaveFlag & APPDATA_CONFIG) != 0)) {
			wSaveRet = APP_ERROR_SAVE_CFG_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate schedule data
	if (GetAppScheduleData() == NULL) {
		if (bySerializeMode == MODE_LOAD) {
			wLoadRet = APP_ERROR_LOAD_SCHED_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == MODE_SAVE) && ((nSaveFlag & APPDATA_SCHEDULE) != 0)) {
			wSaveRet = APP_ERROR_SAVE_SCHED_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate HotkeySet data
	if (GetAppHotkeySetData() == NULL) {
		if (bySerializeMode == MODE_LOAD) {
			wLoadRet = APP_ERROR_LOAD_HKEYSET_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == MODE_SAVE) && ((nSaveFlag & APPDATA_HOTKEYSET) != 0)) {
			wSaveRet = APP_ERROR_SAVE_HKEYSET_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate Power Reminder data
	if (GetAppPwrReminderData() == NULL) {
		if (bySerializeMode == MODE_LOAD) {
			wLoadRet = APP_ERROR_LOAD_PWRRMD_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == MODE_SAVE) && ((nSaveFlag & APPDATA_PWRREMINDER) != 0)) {
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
	OSVERSIONINFOEX oviOSVersion;
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
	CString strAutoStartRegPath;
	strAutoStartRegPath.LoadString(IDS_REGSECTION_AUTOSTART);
	lRes = RegCreateKeyEx(hRootKey, strAutoStartRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key creation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return 0;
	}

	// Enable auto-start
	if (bEnable == TRUE) {

		CString strExecCommand;

		if (bRunAsAdmin == TRUE) {
			// Register to run as admin
			strExecCommand.Format(COMMAND_REGISTER_RUNASADMIN, REG_AFX_PROJECTNAME, GetApplicationPath(TRUE));
			WinExec(MAKEANSI(strExecCommand.GetString()), SW_HIDE);
		}
		else {
			// Unregister to run as admin
			strExecCommand.Format(COMMAND_UNREGISTER_RUNASADMIN, REG_AFX_PROJECTNAME);
			WinExec(MAKEANSI(strExecCommand.GetString()), SW_HIDE);
		}

		// Debug log
		CString strALSLog;
		strALSLog.Format(_T("[ALS] => EnableAutoStart: Cmd=(%s)"), strExecCommand);
		OutputDebugString(strALSLog);

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
	CString strAutoStartRegPath;
	strAutoStartRegPath.LoadString(IDS_REGSECTION_AUTOSTART);
	lRes = RegOpenKeyEx(hRootKey, strAutoStartRegPath, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey);
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

BOOL CPowerPlusApp::GetLastSysEventTime(BYTE byEventType, SYSTEMTIME& timeSysEvent)
{
	long lRes;
	HKEY hRegOpenKey;
	BOOL bRet = TRUE;

	// Initialize registry info data
	REGISTRYINFO regInfoLastSysEvt;
	regInfoLastSysEvt.SetRootKeyName(IDS_APP_REGISTRY_HKEY);
	regInfoLastSysEvt.SetSubkeyPath(IDS_APP_REGISTRY_SUBKEYPATH);
	regInfoLastSysEvt.SetProfileName(IDS_APP_REGISTRY_PROFILENAME);
	regInfoLastSysEvt.SetAppName(IDS_APP_REGISTRY_APPNAME);
	regInfoLastSysEvt.SetSectionName(IDS_REGSECTION_GLBDATA);

	// Set subsection
	regInfoLastSysEvt.SetSectionName(IDS_REGSECTION_GLBDATA_TRACKING);

	// Get registry path
	CString strLastSysEvtRegPath = MakeRegistryPath(regInfoLastSysEvt, RegistryPathType::includingSectionName, FALSE);

	// Open registry key
	lRes = RegOpenKeyEx(HKEY_CURRENT_USER, strLastSysEvtRegPath, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hRegOpenKey);

	// Registry key open failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key open failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get key name
	CString strKeyName;
	if (byEventType == SystemEventID::SystemSuspend) {
		// Last system suspend
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSYSSUSPEND);
	}
	else if (byEventType == SystemEventID::SystemWakeUp) {
		// Last system wakeup
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSYSWAKEUP);
	}
	else if (byEventType == SystemEventID::SessionEnded) {
		// Last app/system session ending
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSESSIONEND);
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
	lRes = RegQueryValueEx(hRegOpenKey, strKeyName, 0, &dwType, (LPBYTE)tcBuffer, &dwBufferSize);
	
	// Get registry key value failed
	if (lRes != ERROR_SUCCESS) {
		// Trace error
		TRACE_ERROR("Error: Registry key query value failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

		// Close key and exit
		RegCloseKey(hRegOpenKey);
		return FALSE;
	}

	// Extract time data from string
	TCHAR tcTimePeriod[5];
	CString strDateTimeFormat;
	strDateTimeFormat.LoadString(IDS_FORMAT_FULLDATETIME);
	_stscanf_s(tcBuffer, strDateTimeFormat, &timeSysEvent.wYear, &timeSysEvent.wMonth, &timeSysEvent.wDay,
		&timeSysEvent.wHour, &timeSysEvent.wMinute, &timeSysEvent.wSecond, &timeSysEvent.wMilliseconds, tcTimePeriod, (unsigned)_countof(tcTimePeriod));

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

BOOL CPowerPlusApp::SaveLastSysEventTime(BYTE byEventType, const SYSTEMTIME& timeSysEvent)
{
	long lRes;
	HKEY hRegOpenKey;
	DWORD dwState;
	BOOL bRet = TRUE;

	// Initialize registry info data
	REGISTRYINFO regInfoLastSysEvt;
	regInfoLastSysEvt.SetRootKeyName(IDS_APP_REGISTRY_HKEY);
	regInfoLastSysEvt.SetSubkeyPath(IDS_APP_REGISTRY_SUBKEYPATH);
	regInfoLastSysEvt.SetProfileName(IDS_APP_REGISTRY_PROFILENAME);
	regInfoLastSysEvt.SetAppName(IDS_APP_REGISTRY_APPNAME);
	regInfoLastSysEvt.SetSectionName(IDS_REGSECTION_GLBDATA);

	// Set subsection
	regInfoLastSysEvt.SetSectionName(IDS_REGSECTION_GLBDATA_TRACKING);

	// Get registry path
	CString strLastSysEvtRegPath = MakeRegistryPath(regInfoLastSysEvt, RegistryPathType::includingSectionName, FALSE);

	// Create registry key (open if key exists)
	lRes = RegCreateKeyEx(HKEY_CURRENT_USER, strLastSysEvtRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hRegOpenKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRACE_ERROR("Error: Registry key creation failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get key name
	CString strKeyName;
	if (byEventType == SystemEventID::SystemSuspend) {
		// Last system suspend
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSYSSUSPEND);
	}
	else if (byEventType == SystemEventID::SystemWakeUp) {
		// Last system wakeup
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSYSWAKEUP);
	}
	else if (byEventType == SystemEventID::SessionEnded) {
		// Last app/system session ending
		strKeyName.LoadString(IDS_REGKEY_TRACKING_LASTSESSIONEND);
	}
	else {
		// Close key and exit
		RegCloseKey(hRegOpenKey);
		return FALSE;
	}

	// Format date/time
	CString strDateTimeFormat;
	UINT nTimePeriod = (timeSysEvent.wHour < 12) ? FORMAT_TIMEPERIOD_ANTE_MERIDIEM : FORMAT_TIMEPERIOD_POST_MERIDIEM;
	CString strTimePeriodFormat = GetLanguageString(GetAppLanguage(), nTimePeriod);
	strDateTimeFormat.Format(IDS_FORMAT_FULLDATETIME, timeSysEvent.wYear, timeSysEvent.wMonth, timeSysEvent.wDay,
		timeSysEvent.wHour, timeSysEvent.wMinute, timeSysEvent.wSecond, timeSysEvent.wMilliseconds, strTimePeriodFormat);

	// Save registry value
	DWORD dwDataSize = (strDateTimeFormat.GetLength() + 1) * sizeof(TCHAR);
	TCHAR* pszData = new TCHAR[dwDataSize];
	_tcscpy_s(pszData, dwDataSize, strDateTimeFormat);
	lRes = RegSetValueEx(hRegOpenKey, strKeyName, 0, REG_SZ, (LPBYTE)pszData, dwDataSize);
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

void CPowerPlusApp::OnExecuteDebugCommand(WPARAM wParam, LPARAM lParam)
{
	// If debug command is empty, do nothing
	CString strDebugCommand(LPARAM_TO_STRING(lParam));
	if (strDebugCommand.IsEmpty())
		return;

	// Format debug command
	strDebugCommand.MakeLower();

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
	CString strDescription;
	strDescription.Format(_T("Error code: 0x%04X"), dwErrCode);

	// Event log detail info
	LOGDETAILINFO logDetailInfo;
	{
		// Error code ID
		logDetailInfo.AddDetail(EventDetail::EventError, dwErrCode);

		// Message content detail info
		logDetailInfo.AddDetail(EventDetail::MessageText, LPARAM_TO_STRING(lParam));
	}

	// Output event log
	OutputEventLog(LOG_EVENT_ERROR_MESSAGE, strDescription, &logDetailInfo);
}

