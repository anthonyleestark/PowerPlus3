
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlus.cpp
//		Description:	Source file for app class
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "DebugTestDlg.h"

using namespace PairFuncs;
using namespace CoreFuncs;
using namespace RegFuncs;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CPowerPlus
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPowerPlusApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CPowerPlusApp
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusApp::CPowerPlusApp()
{
	// RestartManagerSupport
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// Init app data pointers
	m_pcfgAppConfig = NULL;
	m_pschSheduleData = NULL;
	m_phksHotkeySetData = NULL;
	m_ppwrReminderData = NULL;

	// Init app language pointer
	m_pAppLang = NULL;

	// Init logging pointers
	m_pAppEventLog = NULL;
	m_pActionLog = NULL;

	// Hook procedure handle
	m_hAppKeyboardHook = NULL;

	// Init DebugTest dialog
	m_pDebugTestDlg = NULL;

	// Init other member variables
	m_strAppWndTitle.Empty();
	m_nCurDispLang = DEF_INTEGER_NULL;
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

	if (m_pschSheduleData != NULL) {
		delete m_pschSheduleData;
		m_pschSheduleData = NULL;
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
	if (m_pAppEventLog != NULL) {
		delete m_pAppEventLog;
		m_pAppEventLog = NULL;
	}

	if (m_pActionLog != NULL) {
		delete m_pActionLog;
		m_pActionLog = NULL;
	}

	// Destroy DebugTest dialog
	if (m_pDebugTestDlg != NULL) {
		// Destroy dialog
		m_pDebugTestDlg->DestroyWindow();
		delete m_pDebugTestDlg;
		m_pDebugTestDlg = NULL;
	}

	IDMAP_CLEAR()
	DESTROY_APP_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Object name:	theApp
//	Description:	Unique app object
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusApp theApp; 
INIT_APP_IDMAP(CPowerPlusApp)


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
	BOOL bRet = TRUE;
	DWORD dwErrorCode;

	// Set app window title
	CString strAppWindowTitle;
	bRet = strAppWindowTitle.LoadString(IDS_APP_WINDOW_TITLE);
	if ((bRet != FALSE) && (!strAppWindowTitle.IsEmpty())) {
		SetAppWindowTitle(strAppWindowTitle.GetString());
	}
	else {
		// Load title string failed
		TRCLOG("Error: Load app window title failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Show error message
		dwErrorCode = DEF_APP_ERROR_APP_INIT_FAILURE;
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrorCode, NULL);
		return bRet;
	}

	// Check if there is any other instance currently running
	// If yes, bring that instance to top and exit current instance
	if (HWND hPrevWnd = FindWindow(NULL, strAppWindowTitle)) {
		PostMessage(hPrevWnd, SM_WND_SHOWDIALOG, TRUE, (LPARAM)0);
		BringWindowToTop(hPrevWnd);
		SetForegroundWindow(hPrevWnd);
		return FALSE;
	}

	// Init instance
	bRet = CWinApp::InitInstance();
	if (bRet == FALSE) {
		// Trace log
		TRCLOG("Error: Init instance failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Show error message
		dwErrorCode = DEF_APP_ERROR_APP_INIT_FAILURE;
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrorCode, NULL);
		return FALSE;
	}

	// Check CTRL key press state and open DebugTest dialog
	if (0x8000 & ::GetKeyState(VK_CONTROL)) {
		m_pDebugTestDlg = new CDebugTestDlg();
		if (m_pDebugTestDlg != NULL) {
			// Parent is NULL because main window hasn't been initialized yet
			m_pDebugTestDlg->Create(IDD_DEBUGTEST_DLG, NULL);
			m_pDebugTestDlg->ShowWindow(SW_SHOW);
		}
	}

	// Set register key and create neccessary folders
	SetRegistryKey(IDS_APP_REGISTRY_MASTERKEY);
	CreateDirectory(DIR_SUBDIR_LOG, NULL);

	// Setup advanced functions
	m_hAppKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);

	// Initialize app data
	bRet = InitAppData();
	if (bRet == FALSE) {
		// Trace log
		TRCLOG("Error: Init app data failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Show error message
		dwErrorCode = DEF_APP_ERROR_APP_INIT_FAILURE;
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrorCode, NULL);
		return bRet;
	}
	
	// Initialize default data
	SetDefaultData(m_pcfgAppConfig);
	SetDefaultData(m_pschSheduleData);
	SetDefaultData(m_phksHotkeySetData);
	SetDefaultData(m_ppwrReminderData);

	// Load registry app data
	if (!LoadRegistryAppData()) {
		// Trace log
		TRCLOG("Error: Load registry app data failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
	}

	// Initialize app language
	bRet = InitAppLanguage();
	if (bRet == FALSE) {
		// Trace log
		TRCLOG("Error: Init app language failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Show error message
		dwErrorCode = DEF_APP_ERROR_APP_INIT_FAILURE;
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrorCode, NULL);
		return bRet;
	}
	
	// Initialize log objects
	InitAppEventLog();
	InitActionLog();

	if (GetAppEventLogOption() == TRUE) {
		GetAppEventLog()->OutputLogString("Init Instance");
	}

	// Register to system wakeup event notifications
	DEVICE_NOTIFY_SUBSCRIBE_PARAMETERS paramDevNotifySubs;
	paramDevNotifySubs.Callback = DeviceNotifyCallbackRoutine;
	paramDevNotifySubs.Context = NULL;

	HPOWERNOTIFY hPowerNotify = NULL;
	DWORD dwResult = PowerRegisterSuspendResumeNotification(DEVICE_NOTIFY_CALLBACK, &paramDevNotifySubs, &hPowerNotify);

	if (dwResult != ERROR_SUCCESS) {
		// Handle error and show message
		TRCLOG("Error: Power event notification register failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwResult, NULL);
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
		TRCLOG("Error: Main dialog pointer allocation failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Show error message
		dwErrorCode = DEF_APP_ERROR_APP_INIT_FAILURE;
		PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwErrorCode, NULL);
		return FALSE;
	}

	// Set app main window pointer
	m_pMainWnd = pMainDlg;

	// Show/hide main dialog at startup
	BOOL bShowDlgAtStartup = TRUE;
	GetConfig(IDS_REGKEY_CFG_SHOWATSTARTUP, bShowDlgAtStartup);

	if (bShowDlgAtStartup == FALSE) { 
		// Hide dialog
		pMainDlg->Create(IDD_POWERPLUS_DIALOG, NULL);
		pMainDlg->ShowWindow(SW_HIDE);
		// Set parent window for DebugTest dialog if available
		if (m_pDebugTestDlg != NULL) {
			m_pDebugTestDlg->SetParentWnd(pMainDlg);
		}
		// Notification sound
		MessageBeep(0xFFFFFFFF);
		pMainDlg->RunModalLoop();
	}
	else {
		// Set parent window for DebugTest dialog if available
		if (m_pDebugTestDlg != NULL) {
			m_pDebugTestDlg->SetParentWnd(pMainDlg);
		}
		// Show dialog (in modal state)
		pMainDlg->DoModal();
		PostMessage(pMainDlg->m_hWnd, SM_WND_SHOWDIALOG, TRUE, NULL);
	}

	/************************************************************************************/
	/*																					*/
	/*							 END MAIN DIALOG MODAL LOOP								*/
	/*																					*/
	/************************************************************************************/

	// Unregister to system wakeup event notifications
	if (hPowerNotify != NULL) {
		dwResult = PowerUnregisterSuspendResumeNotification(hPowerNotify);
		if (dwResult != ERROR_SUCCESS) {
			// Handle error and show message
			TRCLOG("Error: Power event notification unregister failed");
			TRCDBG(__FUNCTION__, __FILE__, __LINE__);
			PostMessage(NULL, SM_APP_SHOW_ERROR_MSG, (WPARAM)dwResult, NULL);
		}
	}

	// Destroy DebugTest dialog if opening
	if (m_pDebugTestDlg != NULL) {
		m_pDebugTestDlg->DestroyWindow();
		delete m_pDebugTestDlg;
		m_pDebugTestDlg = NULL;
	}

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
	// Write app logs to file if enabled
	if (GetAppEventLogOption() == TRUE) {
		GetAppEventLog()->OutputLogString("Exit Instance");
		GetAppEventLog()->WriteLog();
	}

	// Write action logs to file if enabled
	if (GetActionLogOption() == TRUE) {
		GetActionLog()->WriteLog();
	}

	// Close DebugTest dialog
	if (m_pDebugTestDlg != NULL) {
		// Destroy dialog
		m_pDebugTestDlg->DestroyWindow();
		delete m_pDebugTestDlg;
		m_pDebugTestDlg = NULL;
	}

	// Find if the DebugTest dialog is still running
	HWND hDebugTestWnd = FindDebugTestDlg();
	if (hDebugTestWnd != NULL) {
		// Destroy dialog
		::DestroyWindow(hDebugTestWnd);
	}

	// Unhook keyboard
	UnhookWindowsHookEx(m_hAppKeyboardHook);

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	KeyboardProc
//	Description:	Keyboard hook process
//  Arguments:		Default
//  Return value:	LRESULT - Default
//
//////////////////////////////////////////////////////////////////////////

LRESULT WINAPI CPowerPlusApp::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	// Get param
	PKBDLLHOOKSTRUCT hHookKeyInfo = (PKBDLLHOOKSTRUCT)lParam;
	if (((wParam == WM_KEYDOWN || (wParam == WM_SYSKEYDOWN))) && (nCode == HC_ACTION)) {
		DWORD dwKeyCode = hHookKeyInfo->vkCode;		// Get keycode
		DWORD dwKeyFlags = hHookKeyInfo->flags;		// Get keyflags
		// Process when Alt & Backspace keys are pressed
		if ((dwKeyCode == VK_BACK) && (dwKeyFlags & LLKHF_ALTDOWN)) {
			// Keystroke pressed: "Alt + Left-Win + Backspace"
			if (0x8000 & ::GetKeyState(VK_LWIN)) {
				// Show DebugTest dialog
				::PostMessage((AfxGetMainWnd()->m_hWnd), SM_WND_DEBUGTEST, NULL, NULL);
			}
			// Keystroke pressed: "Alt + Left-Shift + Backspace"
			else if (0x8000 & ::GetKeyState(VK_LSHIFT)) {
				// Show main window
				::PostMessage((AfxGetMainWnd()->m_hWnd), SM_WND_SHOWDIALOG, TRUE, NULL);
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
		return ULONG(DEF_RESULT_FAILED);

	// Get current system time
	SYSTEMTIME stCurSysTime = GetCurSysTime();

	// Processing
	switch (ulType)
	{
	case PBT_APMSUSPEND:					// System suspend event
		// Save last system suspend time
		pApp->SaveLastSysEventTime(SYSEVT_SUSPEND, stCurSysTime);
		break;
	case PBT_APMRESUMESUSPEND:				// System resume from suspend event
	case PBT_APMRESUMEAUTOMATIC:			// System automatic resume event
		// Save last system wakeup time
		pApp->SaveLastSysEventTime(SYSEVT_WAKEUP, stCurSysTime);
		break;
	default:
		break;
	}

	// Default return
	return ULONG(DEF_RESULT_SUCCESS);		// ERROR_SUCCESS
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
	if (CWinApp::ProcessMessageFilter(nCode, lpMsg))
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
	if (pMsg->message == SM_APP_SHOW_ERROR_MSG) {
		// Get window handle and error code
		HWND hWnd = pMsg->hwnd;
		DWORD dwErrCode = (DWORD)(pMsg->wParam);

		// If message window handle is invalid (HWND is NULL), 
		// or main window has not been initialized, or app language has not been loaded,
		// handle message and show error messagebox here
		if ((hWnd == NULL) || (this->GetMainWnd() == NULL) || (this->GetAppLanguage() == NULL)) {
			ShowErrorMessage(NULL, NULL, dwErrCode);
			return TRUE;
		}
	}

	// Default
	return CWinApp::PreTranslateMessage(pMsg);
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
		m_pcfgAppConfig = new CONFIGDATA;
	}
	// Initialize schedule data
	if (m_pschSheduleData == NULL) {
		m_pschSheduleData = new SCHEDULEDATA;
	}
	// Initialize HotkeySet data
	if (m_phksHotkeySetData == NULL) {
		m_phksHotkeySetData = new HOTKEYSETDATA;
	}
	// Initialize Power Reminder data
	if (m_ppwrReminderData == NULL) {
		m_ppwrReminderData = new PWRREMINDERDATA;
	}

	// Check data validity
	BOOL bResult = TRUE;

	// Check app config data
	if (m_pcfgAppConfig == NULL) {
		bResult = FALSE;
		TRCLOG("Error: App config data init failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
	}
	// Check schedule data
	if (m_pschSheduleData == NULL) {
		bResult = FALSE;
		TRCLOG("Error: Schedule data init failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
	}
	// Check HotkeySet data
	if (m_phksHotkeySetData == NULL) {
		bResult = FALSE;
		TRCLOG("Error: HotkeySet data init failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
	}
	// Check Power Reminder data
	if (m_ppwrReminderData == NULL) {
		bResult = FALSE;
		TRCLOG("Error: Power Reminder data init failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
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
	WORD wLoadRet = DEF_APP_ERROR_SUCCESS;
	int nTemp = DEF_INTEGER_INVALID;

	// Check data validity first
	if (!DataSerializeCheck(DEF_MODE_LOAD))
		return FALSE;

	// Create temporary data
	PCONFIGDATA pcfgTempData = new CONFIGDATA;
	PSCHEDULEDATA pschTempData = new SCHEDULEDATA;
	PHOTKEYSETDATA phksTempData = new HOTKEYSETDATA;
	PPWRREMINDERDATA ppwrTempData = new PWRREMINDERDATA;


	/***************************************************************/
	/*															   */
	/*					 Load configuration info				   */
	/*															   */
	/***************************************************************/

	// Load configuration data
	if (pcfgTempData != NULL) {
		bResult &= GetConfig(IDS_REGKEY_CFG_ACTIONLMB,			(int&)pcfgTempData->nLMBAction);
		bResult &= GetConfig(IDS_REGKEY_CFG_ACTIONMMB,			(int&)pcfgTempData->nMMBAction);
		bResult &= GetConfig(IDS_REGKEY_CFG_ACTIONRMB,			(int&)pcfgTempData->nRMBAction);
		bResult &= GetConfig(IDS_REGKEY_CFG_RMBSHOWMENU,		pcfgTempData->bRMBShowMenu);
		bResult &= GetConfig(IDS_REGKEY_CFG_LANGUAGEID,			(int&)pcfgTempData->nLanguageID);
		bResult &= GetConfig(IDS_REGKEY_CFG_SHOWATSTARTUP,		pcfgTempData->bShowDlgAtStartup);
		bResult &= GetConfig(IDS_REGKEY_CFG_STARTUPENABLE,		pcfgTempData->bStartupEnabled);
		bResult &= GetConfig(IDS_REGKEY_CFG_CONFIRMACTION,		pcfgTempData->bConfirmAction);
		bResult &= GetConfig(IDS_REGKEY_CFG_SAVEACTIONLOG,		pcfgTempData->bSaveActionLog);
		bResult &= GetConfig(IDS_REGKEY_CFG_SAVEAPPEVENTLOG,	pcfgTempData->bSaveAppEventLog);
		bResult &= GetConfig(IDS_REGKEY_CFG_RUNASADMIN,			pcfgTempData->bRunAsAdmin);
		bResult &= GetConfig(IDS_REGKEY_CFG_SHOWERROR,			pcfgTempData->bShowErrorMsg);
		bResult &= GetConfig(IDS_REGKEY_CFG_SCHEDULENOTIFY,		pcfgTempData->bNotifySchedule);
		bResult &= GetConfig(IDS_REGKEY_CFG_SCHEDALLOWCANCEL,	pcfgTempData->bAllowCancelSchedule);
		bResult &= GetConfig(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS,	pcfgTempData->bEnableBackgroundHotkey);
		bResult &= GetConfig(IDS_REGKEY_CFG_ENBPWRREMINDER,		pcfgTempData->bEnablePowerReminder);
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = DEF_APP_ERROR_LOAD_CFG_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE; // Set final result
		bResult = TRUE; // Reset flag
	}
	else {
		// Copy temporary data
		if (pcfgTempData != NULL) {
			m_pcfgAppConfig->Copy(*pcfgTempData);
			bResult = TRUE; // Reset flag
		}
	}

	// Delete temporary data
	if (pcfgTempData != NULL) {
		delete pcfgTempData;
		pcfgTempData = NULL;
	}

	/***************************************************************/
	/*															   */
	/*						Load schedule info				       */
	/*															   */
	/***************************************************************/

	// Load schedule data
	if (pschTempData != NULL) {
		bResult &= GetSchedule(IDS_REGKEY_SCHED_ENABLE, pschTempData->bEnable);
		bResult &= GetSchedule(IDS_REGKEY_SCHED_ACTION, (int&)pschTempData->nAction);
		bResult &= GetSchedule(IDS_REGKEY_SCHED_REPEAT, pschTempData->bRepeat);
		bResult &= GetSchedule(IDS_REGKEY_SCHED_TIME,	nTemp);
		if (nTemp != DEF_INTEGER_INVALID) {
			pschTempData->stTime.wHour = WORD(nTemp / 100);
			pschTempData->stTime.wMinute = WORD(nTemp % 100);
			nTemp = DEF_INTEGER_INVALID;
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = DEF_APP_ERROR_LOAD_SCHED_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE; // Set final result
		bResult = TRUE; // Reset flag
	}
	else {
		// Copy temporary data
		if (pschTempData != NULL) {
			m_pschSheduleData->Copy(*pschTempData);
			bResult = TRUE; // Reset flag
		}
	}

	// Delete temporary data
	if (pschTempData != NULL) {
		delete pschTempData;
		pschTempData = NULL;
	}

	/***************************************************************/
	/*															   */
	/*					  Load HotkeySet info				       */
	/*															   */
	/***************************************************************/

	// Load HotkeySet data
	int nItemNum = 0;
	if (phksTempData != NULL) {
		// Copy data
		phksTempData->Copy(*m_phksHotkeySetData);

		// Load number of items
		bResult &= GetHotkeyItemNum(IDS_REGKEY_HKEYSET_ITEMNUM, nItemNum);
		if (nItemNum > phksTempData->nItemNum) {
			// Limit the hotkeyset data item number
			nItemNum = phksTempData->nItemNum;
		}

		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			// Initialize temp item
			HOTKEYSETITEM hksTemp;
			ZeroMemory(&hksTemp, sizeof(HOTKEYSETITEM));

			// Read item data
			int nItemRet = DEF_INTEGER_NULL;
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ENABLE,		(int&)hksTemp.bEnable);
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ACTIONID,	(int&)hksTemp.nHKActionID);
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_CTRLKEY,	(int&)hksTemp.dwCtrlKeyCode);
			nItemRet += GetHotkeySet(nIndex, IDS_REGKEY_HKEYSET_FUNCKEY,	(int&)hksTemp.dwFuncKeyCode);

			// Mark the item as reading failed
			// only if all values were read unsuccessfully
			bResult = (nItemRet != DEF_INTEGER_NULL);

			// Trace error
			if (bResult == FALSE) {
				wLoadRet = DEF_APP_ERROR_LOAD_HKEYSET_FAILED;
				TraceSerializeData(wLoadRet);
				bFinalResult = FALSE; // Set final result
				bResult = TRUE; // Reset flag
				continue;
			}

			// Update item data
			phksTempData->Update(hksTemp);
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = DEF_APP_ERROR_LOAD_HKEYSET_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE; // Set final result
		bResult = TRUE; // Reset flag
	}
	else {
		// Copy temporary data
		if (phksTempData != NULL) {
			m_phksHotkeySetData->Copy(*phksTempData);
			bResult = TRUE; // Reset flag
		}
	}

	// Delete temporary data
	if (phksTempData != NULL) {
		phksTempData->DeleteAll();
		delete phksTempData;
		phksTempData = NULL;
	}

	/***************************************************************/
	/*															   */
	/*					 Load Power Reminder info				   */
	/*															   */
	/***************************************************************/

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
				int nItemRet = DEF_INTEGER_NULL;
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ITEMID,		 (int&)pwrTemp.nItemID);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ENABLE,		 (int&)pwrTemp.bEnable);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTRING,		 pwrTemp.strMessage);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_EVENTID,		 (int&)pwrTemp.nEventID);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTYLE,		 (int&)pwrTemp.dwStyle);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEAT,		 (int&)pwrTemp.rpsRepeatSet.bRepeat);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_ALLOWSNOOZE,	 (int&)pwrTemp.rpsRepeatSet.bAllowSnooze);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_SNOOZEINTERVAL, (int&)pwrTemp.rpsRepeatSet.nSnoozeInterval);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEATDAYS,	 (int&)pwrTemp.rpsRepeatSet.byRepeatDays);
				nItemRet += GetPwrReminder(nIndex, IDS_REGKEY_PWRRMD_TIME,			 nTemp);
				if (nTemp != DEF_INTEGER_INVALID) {
					pwrTemp.stTime.wHour = WORD(nTemp / 100);
					pwrTemp.stTime.wMinute = WORD(nTemp % 100);
					nTemp = DEF_INTEGER_INVALID;
				}

				// Mark the item as reading failed
				// only if all values were read unsuccessfully
				bResult = (nItemRet != DEF_INTEGER_NULL);

				// Trace error
				if (bResult == FALSE) {
					wLoadRet = DEF_APP_ERROR_LOAD_PWRRMD_FAILED;
					TraceSerializeData(wLoadRet);
					bFinalResult = FALSE; // Set final result
					bResult = TRUE; // Reset flag
					continue;
				}

				// Update item data
				ppwrTempData->Update(pwrTemp);
			}
		}
	}

	// Trace error
	if (bResult == FALSE) {
		wLoadRet = DEF_APP_ERROR_LOAD_PWRRMD_FAILED;
		TraceSerializeData(wLoadRet);
		bFinalResult = FALSE; // Set final result
		bResult = TRUE; // Reset flag
	}
	else {
		// Copy temporary data
		if (ppwrTempData != NULL) {
			m_ppwrReminderData->Copy(*ppwrTempData);
			m_ppwrReminderData->Adjust();
			bResult = TRUE; // Reset flag
		}
	}

	// Delete temporary data
	if (ppwrTempData != NULL) {
		ppwrTempData->DeleteAll();
		delete ppwrTempData;
		ppwrTempData = NULL;
	}

	/***************************************************************/
	/*															   */
	/*					Load global variable info				   */
	/*															   */
	/***************************************************************/
	
	// Load global variable values
	LoadGlobalVars();
	
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
	WORD wSaveRet = DEF_APP_ERROR_SUCCESS;
	int nTemp = DEF_INTEGER_INVALID;

	// Check data validity first
	if (!DataSerializeCheck(DEF_MODE_SAVE, dwDataType))
		return FALSE;

	/***************************************************************/
	/*															   */
	/*					Save configuration info					   */
	/*															   */
	/***************************************************************/

	// Save configuration data
	if ((dwDataType & APPDATA_CONFIG) != 0) {
		// Delete old data before writing
		DeleteConfigSection();

		// Save registry data
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONLMB,		m_pcfgAppConfig->nLMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONMMB,		m_pcfgAppConfig->nMMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ACTIONRMB,		m_pcfgAppConfig->nRMBAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_RMBSHOWMENU,		m_pcfgAppConfig->bRMBShowMenu);
		bResult &= WriteConfig(IDS_REGKEY_CFG_LANGUAGEID,		m_pcfgAppConfig->nLanguageID);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SHOWATSTARTUP,	m_pcfgAppConfig->bShowDlgAtStartup);
		bResult &= WriteConfig(IDS_REGKEY_CFG_STARTUPENABLE,	m_pcfgAppConfig->bStartupEnabled);
		bResult &= WriteConfig(IDS_REGKEY_CFG_CONFIRMACTION,	m_pcfgAppConfig->bConfirmAction);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SAVEACTIONLOG,	m_pcfgAppConfig->bSaveActionLog);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SAVEAPPEVENTLOG,	m_pcfgAppConfig->bSaveAppEventLog);
		bResult &= WriteConfig(IDS_REGKEY_CFG_RUNASADMIN,		m_pcfgAppConfig->bRunAsAdmin);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SHOWERROR,		m_pcfgAppConfig->bShowErrorMsg);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SCHEDULENOTIFY,	m_pcfgAppConfig->bNotifySchedule);
		bResult &= WriteConfig(IDS_REGKEY_CFG_SCHEDALLOWCANCEL, m_pcfgAppConfig->bAllowCancelSchedule);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ENBBKGRDHOTKEYS,	m_pcfgAppConfig->bEnableBackgroundHotkey);
		bResult &= WriteConfig(IDS_REGKEY_CFG_ENBPWRREMINDER,	m_pcfgAppConfig->bEnablePowerReminder);

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = DEF_APP_ERROR_SAVE_CFG_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***************************************************************/
	/*															   */
	/*					  Save schedule info					   */
	/*															   */
	/***************************************************************/

	// Save schedule data
	if ((dwDataType & APPDATA_SCHEDULE) != 0) {
		// Delete old data before writing
		DeleteScheduleSection();

		// Save registry data
		bResult &= WriteSchedule(IDS_REGKEY_SCHED_ENABLE,	m_pschSheduleData->bEnable);
		bResult &= WriteSchedule(IDS_REGKEY_SCHED_ACTION,	m_pschSheduleData->nAction);
		bResult &= WriteSchedule(IDS_REGKEY_SCHED_REPEAT,	m_pschSheduleData->bRepeat);
		nTemp = int((m_pschSheduleData->stTime.wHour * 100) + m_pschSheduleData->stTime.wMinute);
		bResult &= WriteSchedule(IDS_REGKEY_SCHED_TIME,		nTemp);

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = DEF_APP_ERROR_SAVE_SCHED_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***************************************************************/
	/*															   */
	/*				 Save auto-start status info				   */
	/*															   */
	/***************************************************************/

	// Save auto-start status info
	if ((dwDataType & APPDATA_CONFIG) != 0) {
		BOOL bStartupEnabled = m_pcfgAppConfig->bStartupEnabled;
		BOOL bRunAsAdmin = m_pcfgAppConfig->bRunAsAdmin;
		int nRetAutoStartEnabled = EnableAutoStart(bStartupEnabled, bRunAsAdmin);
		bResult &= nRetAutoStartEnabled ? TRUE : FALSE;

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = DEF_APP_ERROR_SAVE_CFG_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***************************************************************/
	/*															   */
	/*					 Save HotkeySet info					   */
	/*															   */
	/***************************************************************/

	// Save HotkeySet data
	if ((dwDataType & APPDATA_HOTKEYSET) != 0) {
		// Delete old data before writing
		DeleteHotkeySetSection();

		// Save registry data
		int nItemNum = m_phksHotkeySetData->nItemNum;
		bResult &= WriteHotkeyItemNum(IDS_REGKEY_HKEYSET_ITEMNUM, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			HOTKEYSETITEM hksTemp = m_phksHotkeySetData->GetItemAt(nIndex);
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ENABLE,	hksTemp.bEnable);
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_ACTIONID,	hksTemp.nHKActionID);
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_CTRLKEY,	hksTemp.dwCtrlKeyCode);
			bResult &= WriteHotkeySet(nIndex, IDS_REGKEY_HKEYSET_FUNCKEY,	hksTemp.dwFuncKeyCode);
		}

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = DEF_APP_ERROR_SAVE_HKEYSET_FAILED;
			TraceSerializeData(wSaveRet);
			bFinalResult = FALSE; // Set final result
			bResult = TRUE; // Reset flag
		}
	}

	/***************************************************************/
	/*															   */
	/*					Save Power Reminder info				   */
	/*															   */
	/***************************************************************/

	// Save Power Reminder data
	if ((dwDataType & APPDATA_PWRREMINDER) != 0) {
		// Delete old data before writing
		DeletePwrReminderSection();

		// Save registry data
		int nItemNum = m_ppwrReminderData->nItemNum;
		bResult &= WritePwrReminderItemNum(IDS_REGKEY_PWRRMD_ITEMNUM, nItemNum);
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			PWRREMINDERITEM pwrTemp = m_ppwrReminderData->GetItemAt(nIndex);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ITEMID,			pwrTemp.nItemID);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ENABLE,			pwrTemp.bEnable);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTRING,		pwrTemp.strMessage);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_EVENTID,			pwrTemp.nEventID);
			nTemp = int((pwrTemp.stTime.wHour * 100) + pwrTemp.stTime.wMinute);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_TIME, nTemp);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_MSGSTYLE,			pwrTemp.dwStyle);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEAT,			pwrTemp.rpsRepeatSet.bRepeat);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_ALLOWSNOOZE,		pwrTemp.rpsRepeatSet.bAllowSnooze);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_SNOOZEINTERVAL,	pwrTemp.rpsRepeatSet.nSnoozeInterval);
			bResult &= WritePwrReminder(nIndex, IDS_REGKEY_PWRRMD_REPEATDAYS,		pwrTemp.rpsRepeatSet.byRepeatDays);
		}

		// Trace error
		if (bResult == FALSE) {
			wSaveRet = DEF_APP_ERROR_SAVE_PWRRMD_FAILED;
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
	SConfigBackup::AutoRegistryExport();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadGlobalVars
//	Description:	Load global variable values from registry
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::LoadGlobalVars(void)
{
	BOOL bRet = FALSE;

	int nGlbValue = (int)0;						// Integer type
	BOOL bGlbValue = (BOOL)0;					// Boolean type
	BYTE byGlbValue = (BYTE)0;					// Byte value
	UINT uiGlbValue = (UINT)0;					// Unsigned integer value
	DWORD dwGlbValue = (DWORD)0;				// D-WORD value
	CString strGlbValue = DEF_STRING_EMPTY;		// String value

	// Subsection name (string ID)
	UINT nSubSection = 0;

	/*------------------------<Load debugging/testing variables>-------------------------*/
	// Subsection: DebugTest
	nSubSection = IDS_REGSECTION_GLBVAR_DEBUGTEST;

	// DummyTest mode
	if (GetGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DUMMYTEST, nGlbValue)) {
		SetDummyTestMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug mode
	if (GetGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DEBUGMODE, nGlbValue)) {
		SetDebugMode(nGlbValue);
		bRet |= TRUE;
	}
	// Debug log style
	if (GetGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DBLOGSTYLE, nGlbValue)) {
		SetDebugLogStyle(nGlbValue);
		bRet |= TRUE;
	}
	/*-----------------------------------------------------------------------------------*/

	/*---------------------------------<Load app flags>----------------------------------*/
	// Subsection: AppFlags
	nSubSection = IDS_REGSECTION_GLBVAR_APPFLAG;

	// Power action trace flag
	if (GetGlobalVar(nSubSection, IDS_REGKEY_APPFLAG_PWRACTIONFLG, nGlbValue)) {
		SetPwrActionFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}

	// System suspended trace flag
	if (GetGlobalVar(nSubSection, IDS_REGKEY_APPFLAG_SYSTEMSUSPENDFLG, nGlbValue)) {
		SetSystemSuspendFlag((BYTE)nGlbValue);
		bRet |= TRUE;
	}
	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Load special variables>------------------------------*/
	// Subsection: SpecialVars
	nSubSection = IDS_REGSECTION_GLBVAR_SPECVAR;

	// Reminder message background color
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_BKGRDCLR, nGlbValue)) {
		SetReminderMsgBkgrdColor((DWORD)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message text color
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_TXTCLR, nGlbValue)) {
		SetReminderMsgTextColor((DWORD)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message font name
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_FONTNAME, strGlbValue)) {
		SetReminderMsgFontName(strGlbValue);
		bRet |= TRUE;
	}
	// Reminder message font size
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_FONTSIZE, nGlbValue)) {
		SetReminderMsgFontSize((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message auto-close interval
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_TIMEOUT, nGlbValue)) {
		SetReminderMsgTimeout((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon ID
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONID, nGlbValue)) {
		SetReminderMsgIconID(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon size
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONSIZE, nGlbValue)) {
		SetReminderMsgIconSize(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message icon position
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONPOS, nGlbValue)) {
		SetReminderMsgIconPosition(nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message display area horizontal margin
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_HMARGIN, nGlbValue)) {
		SetReminderMsgHMargin((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message display area vertical margin
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_VMARGIN, nGlbValue)) {
		SetReminderMsgVMargin((UINT)nGlbValue);
		bRet |= TRUE;
	}
	// Reminder message snooze interval
	if (GetGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_SNOOZETIME, nGlbValue)) {
		SetReminderMsgSnoozeInterval((UINT)nGlbValue);
		bRet |= TRUE;
	}
	/*-----------------------------------------------------------------------------------*/

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveGlobalVars
//	Description:	Save global variable values to registry
//  Arguments:		byCateID - Category ID
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::SaveGlobalVars(BYTE byCateID /* = 0xFF */)
{
	BOOL bRet = TRUE;

	int nGlbValue = (int)0;						// Integer type
	BOOL bGlbValue = (BOOL)0;					// Boolean type
	BYTE byGlbValue = (BYTE)0;					// Byte value
	UINT uiGlbValue = (UINT)0;					// Unsigned integer value
	DWORD dwGlbValue = (DWORD)0;				// D-WORD value
	CString strGlbValue = DEF_STRING_EMPTY;		// String value

	// Subsection name (string ID)
	UINT nSubSection = 0;

	/*------------------------<Save debugging/testing variables>-------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBVAR_CATE_DEBUGTEST)) {
		// Subsection: DebugTest
		nSubSection = IDS_REGSECTION_GLBVAR_DEBUGTEST;

		// DummyTest mode
		nGlbValue = GetDummyTestMode();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DUMMYTEST, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug mode
		nGlbValue = GetDebugMode();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DEBUGMODE, nGlbValue)) {
			bRet = FALSE;
		}
		// Debug log style
		nGlbValue = GetDebugLogStyle();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_DBTESTVAR_DBLOGSTYLE, nGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	/*---------------------------------<Save app flags>----------------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBVAR_CATE_APPFLAG)) {
		// Subsection: AppFlags
		nSubSection = IDS_REGSECTION_GLBVAR_APPFLAG;

		// Power action trace flag
		byGlbValue = GetPwrActionFlag();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_APPFLAG_PWRACTIONFLG, byGlbValue)) {
			bRet = FALSE;
		}

		// System suspended trace flag
		byGlbValue = GetSystemSuspendFlag();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_APPFLAG_SYSTEMSUSPENDFLG, byGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	/*-----------------------------<Save special variables>------------------------------*/
	if ((byCateID == 0xFF) || (byCateID == DEF_GLBVAR_CATE_SPECIAL)) {
		// Subsection: SpecialVars
		nSubSection = IDS_REGSECTION_GLBVAR_SPECVAR;

		// Reminder message background color
		dwGlbValue = GetReminderMsgBkgrdColor();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_BKGRDCLR, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message text color
		dwGlbValue = GetReminderMsgTextColor();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_TXTCLR, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message font name
		bGlbValue = GetReminderMsgFontName(strGlbValue);
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_FONTNAME, strGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message font size
		dwGlbValue = GetReminderMsgFontSize();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_FONTSIZE, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message auto-close interval
		dwGlbValue = GetReminderMsgTimeout();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_TIMEOUT, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon ID
		dwGlbValue = GetReminderMsgIconID();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONID, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon size
		dwGlbValue = GetReminderMsgIconSize();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONSIZE, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message icon position
		dwGlbValue = GetReminderMsgIconPosition();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_ICONPOS, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message display area horizontal margin
		dwGlbValue = GetReminderMsgHMargin();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_HMARGIN, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message display area vertical margin
		dwGlbValue = GetReminderMsgVMargin();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_VMARGIN, dwGlbValue)) {
			bRet = FALSE;
		}
		// Reminder message snooze interval
		dwGlbValue = GetReminderMsgSnoozeInterval();
		if (!WriteGlobalVar(nSubSection, IDS_REGKEY_SPECVAR_RMDMSG_SNOOZETIME, dwGlbValue)) {
			bRet = FALSE;
		}
	}
	/*-----------------------------------------------------------------------------------*/

	return bRet;
}


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
	sectionSchedule.AddKey(IDS_REGKEY_SCHED_TIME);

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
	nTemp = sectionSchedule.GetKeyValueInt(IDS_REGKEY_SCHED_TIME);
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
	sectionSchedule.SetKeyValue(IDS_REGKEY_SCHED_TIME, nTemp);

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


//////////////////////////////////////////////////////////////////////////
// App data processing functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppConfigData
//	Description:	Get app configuration data
//  Arguments:		None
//  Return value:	PCONFIGDATA - Config data pointer
//
//////////////////////////////////////////////////////////////////////////

PCONFIGDATA CPowerPlusApp::GetAppConfigData()
{
	// Check validity
	ASSERT(m_pcfgAppConfig != NULL);
	return m_pcfgAppConfig;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppConfigData
//	Description:	Set app configuration data
//  Arguments:		PCONFIGDATA - Config data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppConfigData(PCONFIGDATA pcfgData)
{
	// Check data validity
	ASSERT(pcfgData != NULL);
	if (pcfgData == NULL) return;

	// Backup data
	CONFIGDATA BakData;
	BakData.Copy(*GetAppConfigData());

	// Copy value of data pointer
	GetAppConfigData()->Copy(*pcfgData);

	// Output data change log
	OutputDataChangeLog(BakData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppScheduleData
//	Description:	Get app schedule data
//  Arguments:		None
//  Return value:	PSCHEDULEDATA - Schedule data pointer
//
//////////////////////////////////////////////////////////////////////////

PSCHEDULEDATA CPowerPlusApp::GetAppScheduleData()
{
	// Check validity
	ASSERT(m_pschSheduleData != NULL);
	return m_pschSheduleData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppScheduleData
//	Description:	Set app schedule data
//  Arguments:		PSCHEDULEDATA - Schedule data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppScheduleData(PSCHEDULEDATA pschData)
{
	// Check data validity
	ASSERT(pschData != NULL);
	if (pschData == NULL) return;

	// Backup data
	SCHEDULEDATA BakData;
	BakData.Copy(*GetAppScheduleData());

	// Copy value of data pointer
	GetAppScheduleData()->Copy(*pschData);

	// Output data change log
	OutputDataChangeLog(BakData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppHotkeySetData
//	Description:	Get app hotkeyset data
//  Arguments:		None
//  Return value:	PHOTKEYSETDATA - HotkeySet data pointer
//
//////////////////////////////////////////////////////////////////////////

PHOTKEYSETDATA CPowerPlusApp::GetAppHotkeySetData()
{
	// Check validity
	ASSERT(m_phksHotkeySetData != NULL);
	return m_phksHotkeySetData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppHotkeySetData
//	Description:	Set app hotkeyset data
//  Arguments:		PHOTKEYSETDATA - HotkeySet data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppHotkeySetData(PHOTKEYSETDATA phksData)
{
	// Check data validity
	ASSERT(phksData != NULL);
	if (phksData == NULL) return;

	// Backup data
	HOTKEYSETDATA BakData;
	BakData.Copy(*GetAppHotkeySetData());

	// Copy value of data pointer
	GetAppHotkeySetData()->Copy(*phksData);

	// Output data change log
	OutputDataChangeLog(BakData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppPwrReminderData
//	Description:	Get app Power Reminder data
//  Arguments:		None
//  Return value:	PPWRREMINDERDATA - Power Reminder data pointer
//
//////////////////////////////////////////////////////////////////////////

PPWRREMINDERDATA CPowerPlusApp::GetAppPwrReminderData()
{
	// Check validity
	ASSERT(m_ppwrReminderData != NULL);
	return m_ppwrReminderData;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppPwrReminderData
//	Description:	Set app Power Reminder data
//  Arguments:		PPWRREMINDERDATA - Power Reminder data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppPwrReminderData(PPWRREMINDERDATA ppwrData)
{
	// Check data validity
	ASSERT(ppwrData != NULL);
	if (ppwrData == NULL) return;

	// Backup data
	PWRREMINDERDATA BakData;
	BakData.Copy(*GetAppPwrReminderData());

	// Copy value of data pointer
	GetAppPwrReminderData()->Copy(*ppwrData);

	// Output data change log
	OutputDataChangeLog(BakData);
}


//////////////////////////////////////////////////////////////////////////
// Language functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppLanguage
//	Description:	Initialize app language package pointer
//  Arguments:		None
//  Return value:	BOOL - Result of initialization
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::InitAppLanguage()
{
	// Update current language option from config
	UINT nCurLanguage = GetAppLanguageOption();
	m_nCurDispLang = nCurLanguage;

	// Load language package
	m_pAppLang = LoadLanguageTable(nCurLanguage);

	// Check validity after loading
	if (m_pAppLang == NULL) {
		TRCLOG("Error: Language pointer acquiring failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReloadAppLanguage
//	Description:	Reload app language package pointer
//  Arguments:		nCurLanguage - Current language ID
//  Return value:	BOOL - Result of reloading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::ReloadAppLanguage(UINT nCurLanguage /* = NULL */)
{
	// Load current language option from config if not specified
	if (nCurLanguage == NULL) {
		nCurLanguage = GetAppLanguageOption();
	}

	// Update current displaying language
	m_nCurDispLang = nCurLanguage;

	// Reload language package
	m_pAppLang = LoadLanguageTable(nCurLanguage);

	// Check validity
	if (m_pAppLang == NULL)
		return FALSE;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppLanguage
//	Description:	Get app current language package pointer
//  Arguments:		None
//  Return value:	LANGTABLE_PTR - Language package pointer
//
//////////////////////////////////////////////////////////////////////////

LANGTABLE_PTR CPowerPlusApp::GetAppLanguage()
{
	// Check validity
	VERIFY(m_pAppLang != NULL);
	if (m_pAppLang == NULL)
		return NULL;

	return m_pAppLang;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppLanguageOption
//	Description:	Get app current language option
//  Arguments:		bCurDispLang - Flag to get current displaying language
//  Return value:	UINT - Language ID
//
//////////////////////////////////////////////////////////////////////////

UINT CPowerPlusApp::GetAppLanguageOption(BOOL bCurDispLang /* = FALSE */)
{
	// Return currently displaying language
	if (bCurDispLang == TRUE)
		return m_nCurDispLang;

	// Config error
	VERIFY(m_pcfgAppConfig != NULL);
	if (m_pcfgAppConfig == NULL)
		return (UINT)DEF_INTEGER_INVALID;

	// Return config language option
	return m_pcfgAppConfig->nLanguageID;
}


//////////////////////////////////////////////////////////////////////////
// Logging functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppEventLogOption
//	Description:	Check if app event log option is enabled
//  Arguments:		None
//  Return value:	BOOL - Save app event log option
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::GetAppEventLogOption()
{
	// Check validity
	VERIFY(m_pcfgAppConfig != NULL);
	if (m_pcfgAppConfig == NULL)
		return FALSE;

	return m_pcfgAppConfig->bSaveAppEventLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitAppEventLog
//	Description:	Initialize app event log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::InitAppEventLog()
{
	// Initialization
	if (m_pAppEventLog == NULL) {
		m_pAppEventLog = new SLogging;
	}

	// Check validity after allocating
	if (m_pAppEventLog == NULL) {
		TRCLOG("Error: AppEventLog initialization failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Set properties
	m_pAppEventLog->Init();
	m_pAppEventLog->SetLogDataType(LOGTYPE_APPEVENT_LOG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppEventLog
//	Description:	Get app event log pointer
//  Arguments:		None
//  Return value:	SLogging - App event log pointer
//
//////////////////////////////////////////////////////////////////////////

SLogging* CPowerPlusApp::GetAppEventLog()
{
	// Check validity
	VERIFY(m_pAppEventLog != NULL);
	if ((m_pAppEventLog == NULL) || (GetAppEventLogOption() == FALSE))
		return NULL;

	return m_pAppEventLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActionLogOption
//	Description:	Check if action event log option is enabled
//  Arguments:		None
//  Return value:	BOOL - Save action log option
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::GetActionLogOption()
{
	// Check validity
	VERIFY(m_pcfgAppConfig != NULL);
	if (m_pcfgAppConfig == NULL)
		return FALSE;

	return m_pcfgAppConfig->bSaveActionLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitActionLog
//	Description:	Initialize action log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::InitActionLog()
{
	// Initialization
	if (m_pActionLog == NULL) {
		m_pActionLog = new SLogging;
	}
	
	// Check validity after allocating
	if (m_pActionLog == NULL) {
		TRCLOG("Error: ActionLog initialization failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return;
	}

	// Set properties
	m_pActionLog->Init();
	m_pActionLog->SetLogDataType(LOGTYPE_ACTION_LOG);
	m_pActionLog->SetLogWriteMode(LOG_WRITE_MODE_INSTANT);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActionLog
//	Description:	Get app action log pointer
//  Arguments:		None
//  Return value:	SLogging - Action log pointer
//
//////////////////////////////////////////////////////////////////////////

SLogging* CPowerPlusApp::GetActionLog()
{
	// Check validity
	VERIFY(m_pActionLog != NULL);
	if ((m_pActionLog == NULL) || (GetActionLogOption() == FALSE))
		return NULL;

	return m_pActionLog;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputActionLog
//	Description:	Output a log item to action history log
//  Arguments:		logItem - Log item data
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::OutputActionLog(LOGITEM logItem)
{
	if (m_pActionLog == NULL) return;
	m_pActionLog->OutputLog(logItem);
}

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
	if (CurData.nItemNum != BakData.nItemNum) {
		// Number of items changed
		byDataType = (CurData.nItemNum > BakData.nItemNum) ? DATATYPE_ITEMNUM_ADD : DATATYPE_ITEMNUM_REMOVE;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nItemNum, CurData.nItemNum, 0);
	}
	int nItemNum = (CurData.nItemNum > BakData.nItemNum) ? (CurData.nItemNum) : (BakData.nItemNum);
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (nIndex >= CurData.nItemNum) {
			// Item removed
			continue;
		}
		else if (nIndex >= BakData.nItemNum) {
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
	if (CurData.nItemNum != BakData.nItemNum) {
		// Number of items changed
		byDataType = (CurData.nItemNum > BakData.nItemNum) ? DATATYPE_ITEMNUM_ADD : DATATYPE_ITEMNUM_REMOVE;
		ptrLog->OutputDataChangeLog(byDataType, byDataCate, dwCtrlID, BakData.nItemNum, CurData.nItemNum, 0);
	}
	int nItemNum = (CurData.nItemNum > BakData.nItemNum) ? (CurData.nItemNum) : (BakData.nItemNum);
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		if (nIndex >= CurData.nItemNum) {
			// Item removed
			continue;
		}
		else if (nIndex >= BakData.nItemNum) {
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
		if (CurItem.dwStyle != BakItem.dwStyle) {
			byDataType = DATATYPE_OPTION_VALUE;
			strFlagName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);
			int nTemp = GetPairedID(idplPwrReminderStyle, CurItem.dwStyle);
			CString strCurStyle = GetLanguageString(GetAppLanguage(), nTemp);
			nTemp = GetPairedID(idplPwrReminderStyle, BakItem.dwStyle);
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
	CString strTrcTitle = DEF_STRING_EMPTY;
	CString strTrcLogFormat = DEF_STRING_EMPTY;
	LPCTSTR lpszDataNull = _T("Data is NULL");
	LPCTSTR lpszReadFailed = _T("Key value is unreadable or invalid");
	LPCTSTR lpszWriteFailed = _T("Unable to write registry value");
	
	switch (wErrCode)
	{
	case DEF_APP_ERROR_LOAD_CFG_INVALID:
		strTrcTitle = _T("Load config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_LOAD_CFG_FAILED:
		strTrcTitle = _T("Load config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;
	case DEF_APP_ERROR_LOAD_SCHED_INVALID:
		strTrcTitle = _T("Load schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_LOAD_SCHED_FAILED:
		strTrcTitle = _T("Load schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;
	case DEF_APP_ERROR_LOAD_HKEYSET_INVALID:
		strTrcTitle = _T("Load hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_LOAD_HKEYSET_FAILED:
		strTrcTitle = _T("Load hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;
	case DEF_APP_ERROR_LOAD_PWRRMD_INVALID:
		strTrcTitle = _T("Load reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_LOAD_PWRRMD_FAILED:
		strTrcTitle = _T("Load reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszReadFailed);
		break;
	case DEF_APP_ERROR_SAVE_CFG_INVALID:
		strTrcTitle = _T("Save config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_SAVE_CFG_FAILED:
		strTrcTitle = _T("Save config failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;
	case DEF_APP_ERROR_SAVE_SCHED_INVALID:
		strTrcTitle = _T("Save schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_SAVE_SCHED_FAILED:
		strTrcTitle = _T("Save schedule failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;
	case DEF_APP_ERROR_SAVE_HKEYSET_INVALID:
		strTrcTitle = _T("Save hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_SAVE_HKEYSET_FAILED:
		strTrcTitle = _T("Save hotkeyset failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;
	case DEF_APP_ERROR_SAVE_PWRRMD_INVALID:
		strTrcTitle = _T("Save reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszDataNull);
		break;
	case DEF_APP_ERROR_SAVE_PWRRMD_FAILED:
		strTrcTitle = _T("Save reminder failed");
		strTrcLogFormat.Format(_T("%s: %s"), strTrcTitle, lpszWriteFailed);
		break;
	}

	// Output trace log
	if (!strTrcLogFormat.IsEmpty()) {
		TRCLOG(strTrcLogFormat);
	}

	// Show error message
	UINT nMsg = SM_APP_SHOW_ERROR_MSG;
	WPARAM wParam = (WPARAM)wErrCode;
	CWnd* pMainWnd = this->GetMainWnd();
	if (pMainWnd != NULL) {
		// Post message to main window
		// Message will be handled by main window message map handler
		HWND hWnd = pMainWnd->m_hWnd;
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
	WORD wLoadRet = DEF_APP_ERROR_SUCCESS;
	WORD wSaveRet = DEF_APP_ERROR_SUCCESS;

	// Validate app config data
	if (GetAppConfigData() == NULL) {
		if (bySerializeMode == DEF_MODE_LOAD) {
			wLoadRet = DEF_APP_ERROR_LOAD_CFG_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == DEF_MODE_SAVE) && ((nSaveFlag & APPDATA_CONFIG) != 0)) {
			wSaveRet = DEF_APP_ERROR_SAVE_CFG_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate schedule data
	if (GetAppScheduleData() == NULL) {
		if (bySerializeMode == DEF_MODE_LOAD) {
			wLoadRet = DEF_APP_ERROR_LOAD_SCHED_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == DEF_MODE_SAVE) && ((nSaveFlag & APPDATA_SCHEDULE) != 0)) {
			wSaveRet = DEF_APP_ERROR_SAVE_SCHED_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate HotkeySet data
	if (GetAppHotkeySetData() == NULL) {
		if (bySerializeMode == DEF_MODE_LOAD) {
			wLoadRet = DEF_APP_ERROR_LOAD_HKEYSET_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == DEF_MODE_SAVE) && ((nSaveFlag & APPDATA_HOTKEYSET) != 0)) {
			wSaveRet = DEF_APP_ERROR_SAVE_HKEYSET_INVALID;
			TraceSerializeData(wSaveRet);
		}
		bResult = FALSE;
	}
	// Validate Power Reminder data
	if (GetAppPwrReminderData() == NULL) {
		if (bySerializeMode == DEF_MODE_LOAD) {
			wLoadRet = DEF_APP_ERROR_LOAD_PWRRMD_INVALID;
			TraceSerializeData(wLoadRet);
		}
		else if ((bySerializeMode == DEF_MODE_SAVE) && ((nSaveFlag & APPDATA_PWRREMINDER) != 0)) {
			wSaveRet = DEF_APP_ERROR_SAVE_PWRRMD_INVALID;
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
//	Function name:	GetDebugTestDlg
//	Description:	Get app DebugTest dialog pointer
//  Arguments:		None
//  Return value:	CDebugTestDlg*
//
//////////////////////////////////////////////////////////////////////////

CDebugTestDlg* CPowerPlusApp::GetDebugTestDlg(void)
{
	return m_pDebugTestDlg;
}


//////////////////////////////////////////////////////////////////////////
// Window title functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppWindowTitle
//	Description:	Get app window title
//  Arguments:		None
//  Return value:	LPCTSTR - App title string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR CPowerPlusApp::GetAppWindowTitle(void)
{
	return m_strAppWndTitle.GetString();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAppWindowTitle
//	Description:	Set app window title
//  Arguments:		lpszTitle - App title string
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::SetAppWindowTitle(LPCTSTR lpszTitle)
{
	VERIFY(lpszTitle != DEF_STRING_EMPTY);
	if (lpszTitle == DEF_STRING_EMPTY) return;
	m_strAppWndTitle = lpszTitle;
}


//////////////////////////////////////////////////////////////////////////
// Registry functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAutoStartRegRoot
//	Description:	Get registry root directory for auto-start function
//  Arguments:		hRootDir - Returned root directory handle (ref-value)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusApp::GetAutoStartRegRootDir(HKEY& hRootDir)
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
		hRootDir = HKEY_CURRENT_USER;
	}
	else {
		// HKEY_LOCAL_MACHINE
		hRootDir = HKEY_LOCAL_MACHINE;
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
	HKEY hRootDir, hKey;
	DWORD dwState;
	TCHAR tcPath[_MAX_PATH];
	int nRet;

	// Get root directory
	GetAutoStartRegRootDir(hRootDir);

	// Create registry key
	CString strAutoStartRegPath;
	strAutoStartRegPath.LoadString(IDS_REGSECTION_AUTOSTART);
	lRes = RegCreateKeyEx(hRootDir, strAutoStartRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRCLOG("Error: Registry key creation failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return 0;
	}

	// Enable auto-start
	if (bEnable == TRUE) {

		CString strAppPath;
		CString strCommand;

		if (bRunAsAdmin == TRUE) {
			// Register to run as admin
			strAppPath.Format(_T("\"%s\""), GetAppPath());
			strCommand.Format(IDS_COMMAND_REGISTER_RUNASADMIN, REG_AFX_PROJECTNAME, strAppPath);
			WinExec(CW2A(strCommand.GetString()).m_psz, SW_HIDE);
		}
		else {
			// Unregister to run as admin
			strCommand.Format(IDS_COMMAND_UNREGISTER_RUNASADMIN, REG_AFX_PROJECTNAME);
			WinExec(CW2A(strCommand.GetString()).m_psz, SW_HIDE);
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
//	Function name:	GetAutoStartStatus
//	Description:	Check if startup with Windows is enabled or not
//  Arguments:		None
//  Return value:	int - Result of querrying process
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusApp::GetAutoStartStatus(void)
{
	long lRes;
	HKEY hRootDir, hKey;

	// Get root directory
	GetAutoStartRegRootDir(hRootDir);

	// Open registry key
	CString strAutoStartRegPath;
	strAutoStartRegPath.LoadString(IDS_REGSECTION_AUTOSTART);
	lRes = RegOpenKeyEx(hRootDir, strAutoStartRegPath, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey);
	if (lRes != ERROR_SUCCESS) {
		TRCLOG("Error: Registry key open failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
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
//	Function name:	Get/SaveLastSysEventTime
//	Description:	Get/save last system event time
//  Arguments:		byEventType  - Event type (suspend/wakeup)
//					timeSysEvent - Event time value
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusApp::GetLastSysEventTime(BYTE byEventType, SYSTEMTIME& timeSysEvent)
{
	long lRes;
	HKEY hRootDir, hKey;
	BOOL bRet = TRUE;

	// Format registry path
	hRootDir = HKEY_CURRENT_USER;
	CString strRegPath;
	CString strSubPath, strMasterKey;
	CString strAppName, strSectionName;
	VERIFY(strSubPath.LoadString(IDS_APP_REGISTRY_SUBPATH));
	VERIFY(strMasterKey.LoadString(IDS_APP_REGISTRY_MASTERKEY));
	VERIFY(strAppName.LoadString(IDS_APP_REGISTRY_APPNAME));
	VERIFY(strSectionName.LoadString(IDS_REGSECTION_GLBVAR_OTHER));
	strRegPath.Format(_T("%s\\%s\\%s\\%s"), strSubPath, strMasterKey, strAppName, strSectionName);

	// Open registry key
	lRes = RegOpenKeyEx(hRootDir, strRegPath, 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey);

	// Registry key open failed
	if (lRes != ERROR_SUCCESS) {
		TRCLOG("Error: Registry key open failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	}

	// Get key name
	CString strKeyName;
	if (byEventType == SYSEVT_SUSPEND) {
		// Last system suspend
		strKeyName.LoadString(IDS_REGKEY_OTHER_LASTSYSSUSPEND);
	}
	else if (byEventType == SYSEVT_WAKEUP) {
		// Last system wakeup
		strKeyName.LoadString(IDS_REGKEY_OTHER_LASTSYSWAKEUP);
	}
	else {
		// Close key and exit
		RegCloseKey(hKey);
		return FALSE;
	}

	// Get registry key value
	DWORD dwType = REG_SZ;
	TCHAR tcBuffer[_MAX_PATH];
	DWORD dwBufferSize = sizeof(tcBuffer);
	lRes = RegQueryValueEx(hKey, strKeyName, 0, &dwType, (LPBYTE)tcBuffer, &dwBufferSize);
	
	// Get registry key value failed
	if (lRes != ERROR_SUCCESS) {
		// Trace error
		TRCLOG("Error: Registry key query value failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);

		// Close key and exit
		RegCloseKey(hKey);
		return FALSE;
	}

	// Extract time data from string
	TCHAR tcMiddayFlag[5];
	CString strDateTimeFormat;
	strDateTimeFormat.LoadString(IDS_FORMAT_FULLDATETIME_NOSPACE);
	_stscanf_s(tcBuffer, strDateTimeFormat, &timeSysEvent.wYear, &timeSysEvent.wMonth, &timeSysEvent.wDay,
		&timeSysEvent.wHour, &timeSysEvent.wMinute, &timeSysEvent.wSecond, &timeSysEvent.wMilliseconds, tcMiddayFlag, (unsigned)_countof(tcMiddayFlag));

	// Close key
	RegCloseKey(hKey);
	return TRUE;
}

BOOL CPowerPlusApp::SaveLastSysEventTime(BYTE byEventType, SYSTEMTIME timeSysEvent)
{
	long lRes;
	HKEY hRootDir, hKey;
	DWORD dwState;
	BOOL bRet = TRUE;

	// Format registry path
	hRootDir = HKEY_CURRENT_USER;
	CString strRegPath;
	CString strSubPath, strMasterKey;
	CString strAppName, strSectionName;
	VERIFY(strSubPath.LoadString(IDS_APP_REGISTRY_SUBPATH));
	VERIFY(strMasterKey.LoadString(IDS_APP_REGISTRY_MASTERKEY));
	VERIFY(strAppName.LoadString(IDS_APP_REGISTRY_APPNAME));
	VERIFY(strSectionName.LoadString(IDS_REGSECTION_GLBVAR_OTHER));
	strRegPath.Format(_T("%s\\%s\\%s\\%s"), strSubPath, strMasterKey, strAppName, strSectionName);

	// Create registry key (open if key exists)
	lRes = RegCreateKeyEx(hRootDir, strRegPath, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE | KEY_SET_VALUE, NULL, &hKey, &dwState);

	// Registry key creation failed
	if (lRes != ERROR_SUCCESS) {
		TRCLOG("Error: Registry key creation failed");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	}

	// Format time
	CString strDateTimeFormat;
	UINT nMiddayFlag = (timeSysEvent.wHour < 12) ? FORMAT_TIME_BEFOREMIDDAY : FORMAT_TIME_AFTERMIDDAY;
	CString strMiddayFlag = PairFuncs::GetLanguageString(GetAppLanguage(), nMiddayFlag);
	strDateTimeFormat.Format(IDS_FORMAT_FULLDATETIME_NOSPACE, timeSysEvent.wYear, timeSysEvent.wMonth, timeSysEvent.wDay,
		timeSysEvent.wHour, timeSysEvent.wMinute, timeSysEvent.wSecond, timeSysEvent.wMilliseconds, strMiddayFlag);

	// Get key name
	CString strKeyName;
	if (byEventType == SYSEVT_SUSPEND) {
		// Last system suspend
		strKeyName.LoadString(IDS_REGKEY_OTHER_LASTSYSSUSPEND);
	}
	else if (byEventType == SYSEVT_WAKEUP) {
		// Last system wakeup
		strKeyName.LoadString(IDS_REGKEY_OTHER_LASTSYSWAKEUP);
	}
	else {
		// Close key and exit
		RegCloseKey(hKey);
		return FALSE;
	}

	// Save registry value
	DWORD dwDataSize = (strDateTimeFormat.GetLength() + 1) * sizeof(TCHAR);
	TCHAR* pszData = new TCHAR[dwDataSize];
	_tcscpy_s(pszData, dwDataSize, strDateTimeFormat);
	lRes = RegSetValueEx(hKey, strKeyName, 0, REG_SZ, (LPBYTE)pszData, dwDataSize);
	delete[] pszData;

	// Close key
	RegCloseKey(hKey);
	bRet = (lRes == ERROR_SUCCESS);
	return bRet;
}
