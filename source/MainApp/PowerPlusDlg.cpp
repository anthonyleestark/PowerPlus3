/**
 * @file		PowerPlusDlg.cpp
 * @brief		Source file for main window
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/AboutDlg.h"
#include "Dialogs/HelpDlg.h"
#include "Dialogs/MultiScheduleDlg.h"
#include "Dialogs/LogViewerDlg.h"
#include "Dialogs/HotkeySetDlg.h"
#include "Dialogs/PwrReminderDlg.h"
#include "Dialogs/ReminderMsgDlg.h"
#include "Dialogs/DebugTestDlg.h"
#include "Dialogs/DebugTestV2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;


// TaskbarCreated message constant
const unsigned WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));


// Implement methods for CPowerPlusDlg
IMPLEMENT_DYNAMIC(CPowerPlusDlg, SDialog)


/**
 * @brief	Constructor
 */
CPowerPlusDlg::CPowerPlusDlg(CWnd* pParent /*=NULL*/)
	: SDialog(IDD_POWERPLUS_DIALOG, pParent)
{
	// Initialize member variables
	m_hDefaultIcon = NULL;

	// Initialize boolean variables for checkboxes
	m_bRMBShowMenu = false;
	m_bShowDlgAtStartup = false;
	m_bStartupEnabled = false;
	m_bConfirmAction = false;
	m_bSaveHistoryLog = false;
	m_bSaveAppEventLog = false;
	m_bRunAsAdmin = false;
	m_bShowErrorMsg = false;
	m_bNotifySchedule = false;
	m_bAllowCancelSchedule = false;
	m_bEnableBackgroundHotkey = false;
	m_bEnablePowerReminder = false;

	// Initialize notify icon variables
	m_hNotifyIcon = NULL;
	m_pNotifyIconData = NULL;
	m_pNotifyMenu = NULL;

	// Init member variables for resizing dialog
	m_pDialogSize = NULL;

	// Init hotkey register data
	m_arrCurRegHKeyList.clear();

	// Init Power++ runtime queue data
	m_arrRuntimeQueue.clear();

	// Init child dialogs
	m_pAboutDlg = NULL;
	m_pHelpDlg = NULL;
	m_pLogViewerDlg = NULL;
	m_pMultiScheduleDlg = NULL;
	m_pHotkeySetDlg = NULL;
	m_pPwrReminderDlg = NULL;
}


/**
 * @brief	Destructor
 */
CPowerPlusDlg::~CPowerPlusDlg()
{
	// Delete member pointers
	if (m_pNotifyIconData != NULL) {
		delete m_pNotifyIconData;
		m_pNotifyIconData = NULL;
	}

	if (m_pDialogSize != NULL) {
		delete m_pDialogSize;
		m_pDialogSize = NULL;
	}

	// Destroy child dialogs
	if (m_pAboutDlg != NULL) {
		// Destroy dialog
		delete m_pAboutDlg;
		m_pAboutDlg = NULL;
	}

	if (m_pHelpDlg != NULL) {
		// Destroy dialog
		delete m_pHelpDlg;
		m_pHelpDlg = NULL;
	}

	if (m_pMultiScheduleDlg != NULL) {
		// Destroy dialog
		delete m_pMultiScheduleDlg;
		m_pMultiScheduleDlg = NULL;
	}

	if (m_pLogViewerDlg != NULL) {
		// Destroy dialog
		delete m_pLogViewerDlg;
		m_pLogViewerDlg = NULL;
	}

	if (m_pHotkeySetDlg != NULL) {
		// Destroy dialog
		delete m_pHotkeySetDlg;
		m_pHotkeySetDlg = NULL;
	}

	if (m_pPwrReminderDlg != NULL) {
		// Destroy dialog
		delete m_pPwrReminderDlg;
		m_pPwrReminderDlg = NULL;
	}

	// Clear Action Schedule data
	m_schScheduleData.DeleteAll();
	
	// Clear HotkeySet data
	m_hksHotkeySetData.DeleteAll();

	// Clear Power Reminder data
	m_prdReminderData.DeleteAll();

	// Clear registered hotkey list
	m_arrCurRegHKeyList.clear();

	// Clean Power++ runtime queue data
	m_arrRuntimeQueue.clear();

	// Kill timers
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Unregister for session state change notifications
	RegisterSessionNotification(Mode::Disable);
}


/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CPowerPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LMBACTION_LIST,		m_cmbLMBAction);
	DDX_Control(pDX, IDC_MMBACTION_LIST,		m_cmbMMBAction);
	DDX_Control(pDX, IDC_RMBACTION_LIST,		m_cmbRMBAction);
	DDX_Control(pDX, IDC_LANGUAGE_LIST,			m_cmbLanguages);
	DDX_Check(pDX,	IDC_ENABLERMBMENU_CHK,		m_bRMBShowMenu);
	DDX_Check(pDX,	IDC_SHOWATSTARTUP_CHK,		m_bShowDlgAtStartup);
	DDX_Check(pDX,	IDC_ENABLEAUTOSTART_CHK,	m_bStartupEnabled);
	DDX_Check(pDX,	IDC_CONFIRMACTION_CHK,		m_bConfirmAction);
	DDX_Check(pDX,	IDC_SAVEHISTORYLOG_CHK,		m_bSaveHistoryLog);
	DDX_Check(pDX,	IDC_SAVEAPPEVENTLOG_CHK,	m_bSaveAppEventLog);
	DDX_Check(pDX,	IDC_RUNASADMIN_CHK,			m_bRunAsAdmin);
	DDX_Check(pDX,	IDC_SHOWERROR_CHK,			m_bShowErrorMsg);
	DDX_Check(pDX,	IDC_SCHEDNOTIFY_CHK,		m_bNotifySchedule);
	DDX_Check(pDX,	IDC_SCHEDALLOWCANCEL_CHK,	m_bAllowCancelSchedule);
	DDX_Check(pDX,	IDC_ENBBKGRDHOTKEYS_CHK,	m_bEnableBackgroundHotkey);
	DDX_Check(pDX,	IDC_ENBPWRREMINDER_CHK,		m_bEnablePowerReminder);
}


/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CPowerPlusDlg::RegisterDialogManagement(void)
{
	size_t nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_MAIN_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_LEFTMOUSE_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_LMBACTION_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_MIDMOUSE_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_MMBACTION_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_RIGHTMOUSE_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_RMBACTION_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_ENABLERMBMENU_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_APPLY_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_RELOAD_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_EXIT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_EXPAND_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_COLLAPSE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_SYSTEM_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_SHOWATSTARTUP_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_ENABLEAUTOSTART_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_CONFIRMACTION_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_SAVEHISTORYLOG_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_SAVEAPPEVENTLOG_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_RUNASADMIN_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_SHOWERROR_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_SCHEDNOTIFY_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_SCHEDALLOWCANCEL_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_ENBBKGRDHOTKEYS_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_ENBPWRREMINDER_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_LANGUAGE_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_LANGUAGE_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_VIEWACTIONLOG_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_BACKUPCFG_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_SCHEDULE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_LOGVIEWER_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HELP_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_ABOUT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_DEFAULT_BTN, Button);
	}

	return nRet;
}


/**
 * @brief	Update dialog control management
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::UpdateDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();
	if (pCtrlMan == NULL) return;

	// Update control info
	pCtrlMan->SetBuddy(IDC_LMBACTION_LIST, IDC_LEFTMOUSE_TITLE);
	pCtrlMan->SetBuddy(IDC_MMBACTION_LIST, IDC_MIDMOUSE_TITLE);
	pCtrlMan->SetBuddy(IDC_RMBACTION_LIST, IDC_RIGHTMOUSE_TITLE);
	pCtrlMan->SetBuddy(IDC_LANGUAGE_LIST, IDC_LANGUAGE_TITLE);

	// Update control data
	pCtrlMan->UpdateData(NULL);

	// Default
	SDialog::UpdateDialogManagement();
}


/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CPowerPlusDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_MAIN_TITLE);
		pCtrlMan->RemoveControl(IDC_LEFTMOUSE_TITLE);
		pCtrlMan->RemoveControl(IDC_LMBACTION_LIST);
		pCtrlMan->RemoveControl(IDC_MIDMOUSE_TITLE);
		pCtrlMan->RemoveControl(IDC_MMBACTION_LIST);
		pCtrlMan->RemoveControl(IDC_RIGHTMOUSE_TITLE);
		pCtrlMan->RemoveControl(IDC_RMBACTION_LIST);
		pCtrlMan->RemoveControl(IDC_ENABLERMBMENU_CHK);
		pCtrlMan->RemoveControl(IDC_APPLY_BTN);
		pCtrlMan->RemoveControl(IDC_RELOAD_BTN);
		pCtrlMan->RemoveControl(IDC_EXIT_BTN);
		pCtrlMan->RemoveControl(IDC_EXPAND_BTN);
		pCtrlMan->RemoveControl(IDC_COLLAPSE_BTN);
		pCtrlMan->RemoveControl(IDC_SYSTEM_TITLE);
		pCtrlMan->RemoveControl(IDC_SHOWATSTARTUP_CHK);
		pCtrlMan->RemoveControl(IDC_ENABLEAUTOSTART_CHK);
		pCtrlMan->RemoveControl(IDC_CONFIRMACTION_CHK);
		pCtrlMan->RemoveControl(IDC_SAVEHISTORYLOG_CHK);
		pCtrlMan->RemoveControl(IDC_SAVEAPPEVENTLOG_CHK);
		pCtrlMan->RemoveControl(IDC_RUNASADMIN_CHK);
		pCtrlMan->RemoveControl(IDC_SHOWERROR_CHK);
		pCtrlMan->RemoveControl(IDC_SCHEDNOTIFY_CHK);
		pCtrlMan->RemoveControl(IDC_SCHEDALLOWCANCEL_CHK);
		pCtrlMan->RemoveControl(IDC_ENBBKGRDHOTKEYS_CHK);
		pCtrlMan->RemoveControl(IDC_ENBPWRREMINDER_CHK);
		pCtrlMan->RemoveControl(IDC_LANGUAGE_TITLE);
		pCtrlMan->RemoveControl(IDC_LANGUAGE_LIST);
		pCtrlMan->RemoveControl(IDC_VIEWACTIONLOG_BTN);
		pCtrlMan->RemoveControl(IDC_BACKUPCFG_BTN);
		pCtrlMan->RemoveControl(IDC_SCHEDULE_BTN);
		pCtrlMan->RemoveControl(IDC_LOGVIEWER_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_BTN);
		pCtrlMan->RemoveControl(IDC_PWRREMINDER_BTN);
		pCtrlMan->RemoveControl(IDC_HELP_BTN);
		pCtrlMan->RemoveControl(IDC_ABOUT_BTN);
		pCtrlMan->RemoveControl(IDC_DEFAULT_BTN);
	}

	return SDialog::UnregisterDialogManagement();
}


// CPowerPlusDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CPowerPlusDlg)
	// Dialog controls
	ON_ID_DIALOG(IDD_POWERPLUS_DIALOG,			"AppMainDlg")
	ON_ID_CONTROL(IDC_MAIN_TITLE,				"MainSettingTitle")
	ON_ID_CONTROL(IDC_LMBACTION_LIST,			"LMBActionList")
	ON_ID_CONTROL(IDC_MMBACTION_LIST,			"MMBActionList")
	ON_ID_CONTROL(IDC_RMBACTION_LIST,			"RMBActionList")
	ON_ID_CONTROL(IDC_ENABLERMBMENU_CHK,		"EnableRMBCheck")
	ON_ID_CONTROL(IDC_APPLY_BTN,				"ApplyButton")
	ON_ID_CONTROL(IDC_RELOAD_BTN,				"ReloadButton")
	ON_ID_CONTROL(IDC_EXIT_BTN,					"ExitButton")
	ON_ID_CONTROL(IDC_EXPAND_BTN,				"ExpandButton")
	ON_ID_CONTROL(IDC_COLLAPSE_BTN,				"CollapseButton")
	ON_ID_CONTROL(IDC_SHOWATSTARTUP_CHK,		"ShowDlgAtStartupCheck")
	ON_ID_CONTROL(IDC_ENABLEAUTOSTART_CHK,		"RunAtStartupCheck")
	ON_ID_CONTROL(IDC_CONFIRMACTION_CHK,		"ConfirmActionCheck")
	ON_ID_CONTROL(IDC_SAVEHISTORYLOG_CHK,		"SaveAppHistoryCheck")
	ON_ID_CONTROL(IDC_SAVEAPPEVENTLOG_CHK,		"SaveAppEventLogCheck")
	ON_ID_CONTROL(IDC_RUNASADMIN_CHK,			"RunAsAdminCheck")
	ON_ID_CONTROL(IDC_SHOWERROR_CHK,			"ShowErrMsgCheck")
	ON_ID_CONTROL(IDC_SCHEDNOTIFY_CHK,			"ShowSchedNotifyCheck")
	ON_ID_CONTROL(IDC_SCHEDALLOWCANCEL_CHK,		"AllowCancelSchedCheck")
	ON_ID_CONTROL(IDC_ENBBKGRDHOTKEYS_CHK,		"EnableBkgrdHotkeysCheck")
	ON_ID_CONTROL(IDC_ENBPWRREMINDER_CHK,		"EnablePowerReminderCheck")
	ON_ID_CONTROL(IDC_LANGUAGE_LIST,			"AppLanguageList")
	ON_ID_CONTROL(IDC_VIEWACTIONLOG_BTN,		"ViewActionLogButton")
	ON_ID_CONTROL(IDC_BACKUPCFG_BTN,			"BackupConfigButton")
	ON_ID_CONTROL(IDC_SCHEDULE_BTN,				"ScheduleButton")
	ON_ID_CONTROL(IDC_LOGVIEWER_BTN,			"LogViewerButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_BTN,			"HotkeySetButton")
	ON_ID_CONTROL(IDC_PWRREMINDER_BTN,			"PowerReminderButton")
	ON_ID_CONTROL(IDC_HELP_BTN,					"HelpButton")
	ON_ID_CONTROL(IDC_ABOUT_BTN,				"AboutButton")
	ON_ID_CONTROL(IDC_DEFAULT_BTN,				"DefaultButton")
	ON_ID_CONTROL(IDC_LEFTMOUSE_TITLE,			"LMBTitle")
	ON_ID_CONTROL(IDC_MIDMOUSE_TITLE,			"MMBTitle")
	ON_ID_CONTROL(IDC_RIGHTMOUSE_TITLE,			"RMBTitle")
	ON_ID_CONTROL(IDC_SYSTEM_TITLE,				"AdvSettingsTitle")
	ON_ID_CONTROL(IDC_LANGUAGE_TITLE,			"AppLanguageTitle")

	// Menu items
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_ABOUT,		"Menu.AboutDlg")
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_HELP,			"Menu.HelpDlg")
	ON_ID_MENU(IDM_NOTIFY_VIEW_ACTIONLOG,		"Menu.ViewActionLog")
	ON_ID_MENU(IDM_NOTIFY_BACKUP_CONFIG,		"Menu.BackupConfig")
	ON_ID_MENU(IDM_NOTIFY_VIEW_BAKCONFIG,		"Menu.ViewBakConfig")
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_LOGVIEWER,	"Menu.LogViewerDlg")
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_SCHEDULE,		"Menu.ScheduleDlg")
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_HOTKEYSET,	"Menu.HotkeySetDlg")
	ON_ID_MENU(IDM_NOTIFY_OPENDLG_PWRREMINDER,	"Menu.PowerReminderDlg")
	ON_ID_MENU(IDM_NOTIFY_ACTION_DISPLAYOFF,	"Menu.Action.DisplayOff")
	ON_ID_MENU(IDM_NOTIFY_ACTION_SLEEP,			"Menu.Action.Sleep")
	ON_ID_MENU(IDM_NOTIFY_ACTION_SHUTDOWN,		"Menu.Action.Shutdown")
	ON_ID_MENU(IDM_NOTIFY_ACTION_RESTART,		"Menu.Action.Restart")
	ON_ID_MENU(IDM_NOTIFY_ACTION_SIGNOUT,		"Menu.Action.SignOut")
	ON_ID_MENU(IDM_NOTIFY_ACTION_HIBERNATE,		"Menu.Action.Hibernate")
	ON_ID_MENU(IDM_NOTIFY_ACTION_SCHEDULE,		"Menu.Action.Schedule")
	ON_ID_MENU(IDM_NOTIFY_RESTART_APP,			"Menu.RestartApp")
	ON_ID_MENU(IDM_NOTIFY_RESTART_ASADMIN,		"Menu.RestartAsAdmin")
	ON_ID_MENU(IDM_NOTIFY_SHOW_WINDOW,			"Menu.ShowWindow")
	ON_ID_MENU(IDM_NOTIFY_EXIT_APP,				"Menu.ExitApp")
END_RESOURCEID_MAP()


// CPowerPlusDlg dialog message map
BEGIN_MESSAGE_MAP(CPowerPlusDlg, SDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_APPLY_BTN,				&CPowerPlusDlg::OnApply)
	ON_BN_CLICKED(IDC_RELOAD_BTN,				&CPowerPlusDlg::OnReload)
	ON_BN_CLICKED(IDC_EXIT_BTN,					&CPowerPlusDlg::OnExit)
	ON_BN_CLICKED(IDC_EXPAND_BTN,				&CPowerPlusDlg::OnExpand)
	ON_BN_CLICKED(IDC_ABOUT_BTN,				&CPowerPlusDlg::OnAbout)
	ON_BN_CLICKED(IDC_HELP_BTN,					&CPowerPlusDlg::OnHelp)
	ON_BN_CLICKED(IDC_DEFAULT_BTN,				&CPowerPlusDlg::OnDefault)
	ON_BN_CLICKED(IDC_BACKUPCFG_BTN,			&CPowerPlusDlg::OnBackupConfig)
	ON_BN_CLICKED(IDC_LOGVIEWER_BTN,			&CPowerPlusDlg::OnLogViewer)
	ON_BN_CLICKED(IDC_SCHEDULE_BTN,				&CPowerPlusDlg::OnSchedule)
	ON_BN_CLICKED(IDC_HOTKEYSET_BTN,			&CPowerPlusDlg::OnHotkeySet)
	ON_BN_CLICKED(IDC_PWRREMINDER_BTN,			&CPowerPlusDlg::OnPowerReminder)
	ON_CBN_SELCHANGE(IDC_LANGUAGE_LIST,			&CPowerPlusDlg::OnChangeLanguage)
	ON_CBN_SELCHANGE(IDC_LMBACTION_LIST,		&CPowerPlusDlg::OnChangeLMBAction)
	ON_CBN_SELCHANGE(IDC_MMBACTION_LIST,		&CPowerPlusDlg::OnChangeMMBAction)
	ON_CBN_SELCHANGE(IDC_RMBACTION_LIST,		&CPowerPlusDlg::OnChangeRMBAction)
	ON_BN_CLICKED(IDC_ENABLERMBMENU_CHK,		&CPowerPlusDlg::OnEnableRightMouseMenu)
	ON_BN_CLICKED(IDC_VIEWACTIONLOG_BTN,		&CPowerPlusDlg::OnViewActionLog)
	ON_MESSAGE(SCM_NOTIFY_DIALOG_DESTROY,		&CPowerPlusDlg::OnChildDialogDestroy)
	ON_MESSAGE(SM_APP_UPDATE_SCHEDULEDATA,		&CPowerPlusDlg::OnUpdateScheduleData)
	ON_MESSAGE(SM_APP_UPDATE_HOTKEYSETDATA,		&CPowerPlusDlg::OnUpdateHotkeySetData)
	ON_MESSAGE(SM_APP_UPDATE_PWRREMINDERDATA,	&CPowerPlusDlg::OnUpdatePwrReminderData)
	ON_MESSAGE(SM_APP_DEBUG_COMMAND,			&CPowerPlusDlg::OnProcessDebugCommand)
	ON_MESSAGE(SM_WND_SHOWDIALOG,				&CPowerPlusDlg::OnShowDialog)
	ON_MESSAGE(SM_APP_ERROR_MESSAGE,			&CPowerPlusDlg::OnShowErrorMessage)
	ON_COMMAND_RANGE(IDC_SHOWATSTARTUP_CHK, IDC_ENBPWRREMINDER_CHK, &CPowerPlusDlg::OnCheckboxClicked)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
// CPowerPlusDlg message handlers


/**
 * @brief	Initialize main dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default result
 */
BOOL CPowerPlusDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Prevent flickering on startup
	this->ShowWindow(SW_HIDE);

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Setup system menu
	CMenu* pSysMenu = GetSystemMenu(false);
	if (pSysMenu != NULL)
	{
		String aboutMenuFormat = StringUtils::LoadResourceString(IDS_APP_SYSMENU_ABOUT);
		if (!aboutMenuFormat.IsEmpty())
		{
			// Add product version number
			String aboutMenuTitle;
			aboutMenuTitle.Format(aboutMenuFormat, StringUtils::GetProductVersion(false).GetString());

			// Add menu item
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, aboutMenuTitle);
		}
	}

	// Set application's main window caption
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		this->SetCaption(pApp->GetAppWindowCaption());
	}

	// Load app default icon
	SetIcon(IDI_ICON_APP);

	// Set icon
	SetIcon(m_hDefaultIcon, true);		// Set big icon
	SetIcon(m_hDefaultIcon, false);		// Set small icon

	// Do not exit using Esc button
	SetUseEscape(false);

	// Do not apply settings with Enter button
	SetUseEnter(false);

	// Set app features standard timers
	SetTimer(TIMERID_STD_ACTIONSCHEDULE, 1000, NULL);
	SetTimer(TIMERID_STD_POWERREMINDER, 1000, NULL);
	SetTimer(TIMERID_STD_EVENTSKIPCOUNTER, 1000, NULL);

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

	// First, init all default data
	m_cfgAppConfig.SetDefaultData();
	m_cfgTempConfig.SetDefaultData();
	m_schScheduleData.SetDefaultData();
	m_hksHotkeySetData.SetDefaultData();
	m_prdReminderData.SetDefaultData();

	// Load data
	GetAppData(APPDATA_ALL);

	// Setup main dialog
	SetupLanguage();
	UpdateDialogData(false);

	// Create notify icon
	if (!CreateNotifyIcon()) {

		// Exit if failed to create notify icon
		TRACE("Failed to create notify icon!!!");
		ExitApp(ExitCode::Error);
		return false;
	}

	// Update dialog control management
	UpdateDialogManagement();

	// Initialize dialog state as collapsed
	SetFlagValue(AppFlagID::dialogExpanded, true);
	ExpandDialog(false);

	// Initialize Power++ runtime queue
	UpdateActionScheduleQueue(Mode::Init);
	UpdatePwrReminderSnooze(Mode::Init);

	// Initialize background hotkeys if enabled
	SetupBackgroundHotkey(Mode::Init);

	// Register for session state change notifications
	RegisterSessionNotification(Mode::Init);

	// Execute Power Reminder at startup
	ExecutePowerReminder(PwrReminderEvent::atAppStartup);

	// Execute Power Reminder after power action awake
	if (GetPwrActionFlag() == FLAG_ON) {
		ExecutePowerReminder(PwrReminderEvent::wakeAfterAction);
		SetPwrActionFlag(FLAG_OFF);						// Reset flag
		SetSystemSuspendFlag(FLAG_OFF);					// Reset flag
		SetSessionEndFlag(FLAG_OFF);					// Reset flag
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
	}

	// Execute Power Reminder after system awake (or after session ending)
	if ((GetSystemSuspendFlag() == FLAG_ON) || (GetSessionEndFlag() == FLAG_ON)) {
		ExecutePowerReminder(PwrReminderEvent::atSysWakeUp);
		SetSystemSuspendFlag(FLAG_OFF);					// Reset flag
		SetSessionEndFlag(FLAG_OFF);					// Reset flag
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
	}

	return true;
}


/**
 * @brief	Pre-destroy dialog and exit application
 * @param	None
 * @return	int
 */
int CPowerPlusDlg::PreDestroyDialog()
{
	// Request closing all child dialog if opening
	LRESULT resCloseReq;

	// About dialog
	if (m_pAboutDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pAboutDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Help dialog
	if (m_pHelpDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHelpDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// LogViewer dialog
	if (m_pLogViewerDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pLogViewerDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Multi schedule dialog
	if (m_pMultiScheduleDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pMultiScheduleDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// HotkeySet dialog
	if (m_pHotkeySetDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHotkeySetDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Power Reminder dialog
	if (m_pPwrReminderDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pPwrReminderDlg->RequestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}

	// Can not destroy if Power Reminder messages are currently displaying
	UIntArray arrPwrDispItemList;
	if (GetPwrReminderDispList(arrPwrDispItemList) > 0) {
		// Display notify message
		DisplayMessageBox(MSGBOX_OTHER_PREDESTROY_REMINDERDISP, MSGBOX_PWRREMINDER_CAPTION);
		return Result::Failure;
	}

	// Destroy components
	RemoveNotifyIcon();
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Execute Power Reminder before exitting
	ExecutePowerReminder(PwrReminderEvent::atAppExit);

	// Destroy background hotkeys if enabled
	SetupBackgroundHotkey(Mode::Disable);

	// Unregister for session state change notifications
	RegisterSessionNotification(Mode::Disable);

	return SDialog::PreDestroyDialog();
}


/**
 * @brief	Destroy dialog and quit
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnDestroy()
{
	// Destroy background hotkeys if enabled
	SetupBackgroundHotkey(Mode::Disable);

	// Unregister for session state change notifications
	RegisterSessionNotification(Mode::Disable);

	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}


/**
 * @brief	OnSysCommand function
 * @param	Default
 * @return	None
 */
void CPowerPlusDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		// Open About dialog
		OpenChildDialogEx(IDD_ABOUT_DLG);
	}
	else {
		// Execute default syscommand
		SDialog::OnSysCommand(nID, lParam);
	}
}


/**
 * @brief	OnPaint function
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnPaint()
{
	if (IsIconic()) {
		// Device context for painting
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hDefaultIcon);
	}
	else {
		// Call base painting method
		SDialog::OnPaint();
	}
}


/**
 * @brief	OnQueryDragIcon function
 * @param	None
 * @return	HCURSOR - Default
 */
HCURSOR CPowerPlusDlg::OnQueryDragIcon()
{
	return STATIC_CAST(HCURSOR, m_hDefaultIcon);
}

//////////////////////////////////////////////////////////////////////////
// Message processing functions


/**
 * @brief	Handle click event for [Apply] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnApply()
{
	// Save app event logs if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_APPLY_BTN);

	// Apply settings and hide dialog
	ApplySettings(true);
}


/**
 * @brief	Handle click event for [Reload] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnReload()
{
	// Save app event logs if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_RELOAD_BTN);

	// Message caption
	unsigned nCaptionID = MSGBOX_CONFIG_RELOAD_CAPTION;

	// Check for setting changed
	bool bIsChanged = CheckSettingChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == true) {
		int nRet = DisplayMessageBox(MSGBOX_CONFIG_CHANGED_CONTENT, nCaptionID, MB_YESNO | MB_ICONINFORMATION);
		if (nRet == IDYES) {
			// Reload all settings
			ReloadSettings();
		}
	}
	else {
		// There's nothing changed
		DisplayMessageBox(MSGBOX_CONFIG_NOTCHANGED_CONTENT, nCaptionID, MB_OK | MB_ICONINFORMATION);
	}
}


/**
 * @brief	Handle click event for [Exit] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnExit()
{
	// Save app event logs if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXIT_BTN);

	// Destroy dialog and exit
	ExitApp(ExitCode::PressExitButton);
}


/**
 * @brief	Handle click event for [X] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnClose()
{
	// Check for setting changed
	bool bIsChanged = CheckSettingChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == true) {
		// Apply settings and hide dialog
		ApplySettings(true);
	}
	else {
		// Only hide the dialog
		ShowDialog(this, false);
	}
}


/**
 * @brief	Handle click event for [Expand/Collapse] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnExpand()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXPAND_BTN);

	// Expand/collapse dialog
	bool bCurState = GetFlagValue(AppFlagID::dialogExpanded);
	ExpandDialog(!bCurState);
	UpdateDialogData(false);
}


/**
 * @brief	Show about dialog when pressing [About] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnAbout()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_ABOUT_BTN);

	// Open About dialog
	OpenChildDialogEx(IDD_ABOUT_DLG);
}


/**
 * @brief	Show help dialog when pressing [Help] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnHelp()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_BTN);

	// Open Help dialog
	OpenChildDialogEx(IDD_HELP_DLG);
}


/**
 * @brief	Handle click event for [Default] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnDefault()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_DEFAULT_BTN);

	// Set default config
	SetDefaultConfig();
}


/**
 * @brief	Change action for left mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeLMBAction()
{
	// Update data
	UpdateDialogData(true);
	m_cmbLMBAction.GetCurSel();

	// Check for settings change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckSettingChangeState());

	// Save app event log if enabled
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LMBACTION_LIST);
}


/**
 * @brief	Change action for middle mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeMMBAction()
{
	// Update data
	UpdateDialogData(true);
	m_cmbMMBAction.GetCurSel();

	// Check for settings change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckSettingChangeState());

	// Save app event log if enabled
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_MMBACTION_LIST);
}


/**
 * @brief	Change action for right mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeRMBAction()
{
	// Update data
	UpdateDialogData(true);
	m_cmbRMBAction.GetCurSel();

	// Check for settings change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckSettingChangeState());

	// Save app event log if enabled
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_RMBACTION_LIST);

	// Refresh dialog item state
	m_cmbRMBAction.EnableWindow(!m_bRMBShowMenu);
	UpdateDialogData(false);
}


/**
 * @brief	Update dialog items when changing language
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeLanguage()
{
	// Update data
	UpdateDialogData(true);
	m_cmbLanguages.GetCurSel();

	// Save app event log if enabled
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LANGUAGE_LIST);

	// Reload app language
	unsigned nCurLanguage = GetAppOption(AppOptionID::languageID, true);
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage(nCurLanguage);

	// Check for settings change
	SetFlagValue(AppFlagID::dialogDataChanged, CheckSettingChangeState());

	// Refresh dialog display
	SetupLanguage();
	UpdateDialogData(false);
}


/**
 * @brief	Handle click event for "Right mouse action" checkbox
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnEnableRightMouseMenu()
{
	// Using custom checkbox clicked event handler
	OnCheckboxClicked(IDC_ENABLERMBMENU_CHK);
}


/**
 * @brief	Handle click event on checkbox-es
 * @param	nChkBoxID - ID of clicked checkbox
 * @return	None
 */
void CPowerPlusDlg::OnCheckboxClicked(UINT nChkBoxID)
{
	// Get clicked checkbox control
	CButton* pChkCtrl = (CButton*)GetDlgItem(nChkBoxID);
	if (pChkCtrl == NULL) return;

	// Update checkbox state
	UpdateDialogData(true);
	int nState = pChkCtrl->GetCheck();

	// Update setting change flag
	SetFlagValue(AppFlagID::dialogDataChanged, CheckSettingChangeState());

	// Save app event log if enabled
	OutputCheckBoxLog(LOG_EVENT_CHK_CLICKED, nChkBoxID);

	// Do custom checkbox actions
	switch (nChkBoxID)
	{
	case IDC_ENABLERMBMENU_CHK:
		EnableRightMouseMenu(nState);
		break;
	case IDC_SAVEAPPEVENTLOG_CHK:
		EnableLogViewer(nState);
		break;
	case IDC_ENBBKGRDHOTKEYS_CHK:
		EnableBackgroundHotkey(nState);
		break;
	case IDC_ENBPWRREMINDER_CHK:
		EnablePowerReminder(nState);
		break;
	case IDC_RUNASADMIN_CHK:
		UpdateRestartAsAdminFlag(nState);
		break;
	default:
		break;
	}
}


/**
 * @brief	Handle click event for [Backup Configuration] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnBackupConfig()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_BACKUPCFG_BTN);

	// Backup configuration
	bool bRet = false;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		bRet = pApp->BackupRegistryAppData();
	}

	// Backup failed
	if (bRet != true) {
		// Show error message if enabled
		ShowErrorMessage(APP_ERROR_BACKUP_REG_FAILED);
	}
}


/**
 * @brief	Handle click event for [Logviewer] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnLogViewer()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_LOGVIEWER_BTN);

	// Open LogViewer dialog
	OpenChildDialogEx(IDD_LOGVIEWER_DLG);
}


/**
 * @brief	Handle click event for [Schedule] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnSchedule()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_SCHEDULE_BTN);

	// Open Schedule dialog
	OpenChildDialogEx(IDD_MULTISCHEDULE_DLG);
}


/**
 * @brief	Handle click event for [HotkeySet] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnHotkeySet()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_BTN);

	// Open HotkeySet dialog
	OpenChildDialogEx(IDD_HOTKEYSET_DLG);
}


/**
 * @brief	Handle click event for [Power Reminder] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnPowerReminder()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_BTN);

	// Open PowerReminder dialog
	OpenChildDialogEx(IDD_PWRREMINDER_DLG);
}


/**
 * @brief	Open action log file with Notepad
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnViewActionLog()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWACTIONLOG_BTN);

	// View action log file
	OpenTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
}


/**
 * @brief	Open backed-up configuration file with Notepad
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnViewBackupConfig()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWBAKCFG_BTN);

	// View backup config file
	OpenTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
}


/**
 * @brief	OnTimer function
 * @param	nIDEvent - Time event ID
 * @return	None
 */
void CPowerPlusDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Timer ID: Action Schedule
	if (nIDEvent == TIMERID_STD_ACTIONSCHEDULE) {
		// Process Action schedule
		ProcessActionSchedule();
	}

	// Timer ID: Power Reminder
	else if (nIDEvent == TIMERID_STD_POWERREMINDER) {
		// Process Power Reminder at set time event
		bool bPwrReminderActive = GetAppOption(AppOptionID::pwrReminderEnabled);
		if (bPwrReminderActive == true) {
			// Execute Power reminder
			ExecutePowerReminder(PwrReminderEvent::atSetTime);
		}
	}

	// Timer ID: Event skip counter
	else if (nIDEvent == TIMERID_STD_EVENTSKIPCOUNTER) {
		// Process Power Broadcast event skip counter
		int nCounter = GetFlagValue(AppFlagID::pwrBroadcastSkipCount);
		if (nCounter > 0) {
			// Count down (decrease value by 1)
			SetFlagValue(AppFlagID::pwrBroadcastSkipCount, --nCounter);
		}
	}

	// Default
	SDialog::OnTimer(nIDEvent);
}


/**
 * @brief	Handle event when a child dialog is closed/destroyed
 * @param	wParam - Child dialog ID
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnChildDialogDestroy(WPARAM wParam, LPARAM lParam)
{
	// Get dialog ID
	unsigned nDialogID = (unsigned)wParam;

	// About dialog
	if (nDialogID == IDD_ABOUT_DLG) {
		if (m_pAboutDlg != NULL) {
			// Delete dialog
			delete m_pAboutDlg;
			m_pAboutDlg = NULL;
		}
	}
	// Help dialog
	else if (nDialogID == IDD_HELP_DLG) {
		if (m_pHelpDlg != NULL) {
			// Delete dialog
			delete m_pHelpDlg;
			m_pHelpDlg = NULL;
		}
	}
	// Multi schedule dialog
	else if (nDialogID == IDD_MULTISCHEDULE_DLG) {
		if (m_pMultiScheduleDlg != NULL) {
			// Delete dialog
			delete m_pMultiScheduleDlg;
			m_pMultiScheduleDlg = NULL;
		}
	}
	// LogViewer dialog
	else if (nDialogID == IDD_LOGVIEWER_DLG) {
		if (m_pLogViewerDlg != NULL) {
			// Delete dialog
			delete m_pLogViewerDlg;
			m_pLogViewerDlg = NULL;
		}
	}
	// HotkeySet dialog
	else if (nDialogID == IDD_HOTKEYSET_DLG) {
		if (m_pHotkeySetDlg != NULL) {
			// Delete dialog
			delete m_pHotkeySetDlg;
			m_pHotkeySetDlg = NULL;
		}
	}
	// Power Reminder dialog
	else if (nDialogID == IDD_PWRREMINDER_DLG) {
		if (m_pPwrReminderDlg != NULL) {
			// Delete dialog
			delete m_pPwrReminderDlg;
			m_pPwrReminderDlg = NULL;
		}
	}
	// DebugTest dialog
	else if (nDialogID == IDD_DEBUGTEST_DLG) {
		// Temporarily do nothing
		// DebugTest dialog will be destroyed in app class
	}

	// Default
	return SDialog::OnChildDialogDestroy(wParam, lParam);
}


/**
 * @brief	Update schedule data if changed
 * @param	wParam - Not used
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnUpdateScheduleData(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		ScheduleData* pschData = pApp->GetAppScheduleData();
		if (pschData != NULL) {
			m_schScheduleData.Copy(*pschData);
			UpdateActionScheduleQueue(Mode::Update);
		}
	}

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Update HotkeySet data if changed
 * @param	wParam - Not used
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnUpdateHotkeySetData(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		HotkeySetData* phksData = pApp->GetAppHotkeySetData();
		if (phksData != NULL) {
			m_hksHotkeySetData.Copy(*phksData);
			SetupBackgroundHotkey(Mode::Update);
		}
	}

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Update Power Reminder data if changed
 * @param	wParam - Not used
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnUpdatePwrReminderData(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		PwrReminderData* ppwrData = pApp->GetAppPwrReminderData();
		if (ppwrData != NULL) {
			m_prdReminderData.Copy(*ppwrData);
			UpdatePwrReminderSnooze(Mode::Update);
		}
	}

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle debug command message
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnProcessDebugCommand(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((lParam == NULL) || ((wParam == NULL) && (lParam == NULL))) {
		OutputDebugLog(Constant::String::Null);
		return LRESULT(Result::Failure);
	}

	// Get debug command string
	String debugCommand(LPARAM_TO_STRING(lParam));

	// Process debug command
	DWORD dwErrorCode = APP_ERROR_DBG_SUCCESS;
	if (!ProcessDebugCommand(debugCommand, dwErrorCode)) {

		// Reply failed message
		if (dwErrorCode == APP_ERROR_DBG_INVALID_COMMAND) {

			// Error: Invalid command
			OutputDebugLog(_T("Invalid command!!!"));
		}
		else if (dwErrorCode == APP_ERROR_DBG_TOKENIZATION_FAILED) {

			// Trace error
			TRACE_ERROR(_T("Error: Debug command tokenization failed!!!"));

			// Get command character list
			String commandCharList;
			StringUtils::PrintCharList(debugCommand, commandCharList);

			// Output debug info (to file)
			String debugLog = StringUtils::StringFormat(_T("Failed debug command: %s"), commandCharList.GetString());
			OutputDebugLog(debugLog, DebugInfoFile);
		}
		else {
			// Reply corresponding error code
			OutputDebugLogFormat(_T("Failed! (Error code: 0x%X)"), dwErrorCode);
		}

		// Result: Failed
		return LRESULT(Result::Failure);
	}

	// Notify app class about debug command execution
	WPARAM wAppNotiParam = MAKE_WPARAM_STRING(debugCommand);
	LPARAM lAppNotiParam = MAKE_LPARAM_STRING(debugCommand);
	::PostMessage(NULL, SM_APP_DEBUGCMD_EXEC, wAppNotiParam, lAppNotiParam);

	// Default: Success
	return LRESULT(Result::Success);
}


/**
 * @brief	Show/hide dialog when receiving message
 * @param	wParam - Show/hide flag
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnShowDialog(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get flag value
	bool bShowFlag = true;
	if (wParam != NULL) {
		bShowFlag = static_cast<bool>(wParam);
	}

	// Show/hide dialog
	ShowDialog(this, bShowFlag);

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Show error when receiving message
 * @param	wParam - Error code
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnShowErrorMessage(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get error code value
	DWORD dwErrorCode = NULL;
	if (wParam != NULL) {
		dwErrorCode = static_cast<DWORD>(wParam);
	}

	// Show error message
	ShowErrorMessage(dwErrorCode);

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle power broadcast event
	Arguments:		wParam - Event ID
					lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnPowerBroadcastEvent(WPARAM wParam, LPARAM /*lParam*/)
{
	// Check if event skip counter is triggered
	if (GetFlagValue(AppFlagID::pwrBroadcastSkipCount) > 0) {
		TRACE("Power Broadcast Event will be skipped!!!");
		return LRESULT(Result::Failure);
	}

	// Get event ID from param
	ULONG ulEvent = static_cast<ULONG>(wParam);
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Process system resume/wakeup events
	if ((ulEvent == PBT_APMRESUMESUSPEND) || (ulEvent == PBT_APMRESUMEAUTOMATIC)) {

		// Trigger skip event counter
		// Temporarily skip processing PowerBroadcastEvent in 3 seconds
		SetFlagValue(AppFlagID::pwrBroadcastSkipCount, 3);

		// If Power action flag is triggered, 
		// handle it like a wakeup event after power action
		if (GetPwrActionFlag() == FLAG_ON) {
			// Execute Power Reminder after power action awake
			ExecutePowerReminder(PwrReminderEvent::wakeAfterAction);
			SetPwrActionFlag(FLAG_OFF);		// Reset flag
		}
		// Otherwise,
		// handle it like regular system wakeup event
		else {
			// Execute Power Reminder at system awaken event
			ExecutePowerReminder(PwrReminderEvent::atSysWakeUp);
		}

		// Reset session ending flag
		SetSessionEndFlag(FLAG_OFF);

		// Reset system suspended flag
		SetSystemSuspendFlag(FLAG_OFF);
		if (pApp != NULL) {
			// Save flag value updates
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
		
		// Output action history after waken up
		// Notes: This is only backup solution to prevent history data lost
		// If it had already been saved, it will be skipped automatically
		SaveHistoryInfoData();
	}
	// Process system suspend event
	else if (ulEvent == PBT_APMSUSPEND) {

		// Turn on system suspended flag
		SetSystemSuspendFlag(FLAG_ON);
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}

		// Save action history if remaining unsaved
		SaveHistoryInfoData();
	}

	// Default: Success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle querry ending session event
	Arguments:		wParam - Not used
					lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnQuerryEndSession(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return LRESULT(Result::Failure);
	}

	/*---------- Process querry ending session event ----------*/
	// Turn on session ending flag
	SetSessionEndFlag(FLAG_ON);
	pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);

	// Save last session ending time
	DateTime curSysDateTime = DateTimeUtils::GetCurrentDateTime();
	pApp->SaveLastSysEventTime(SystemEventID::SessionEnded, curSysDateTime);

	// Save action history if remaining unsaved
	SaveHistoryInfoData();

	/*---------------------------------------------------------*/
	// Default: Success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle session state change notifications
	Arguments:		wParam - First param
					lParam - Second param
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnWTSSessionChange(WPARAM wParam, LPARAM /*lParam*/)
{
	// Process status code
	switch (wParam)
	{
	case WTS_SESSION_LOCK:
		// Screen locked
		SetSessionLockFlag(FLAG_ON);
		OutputDebugLog(_T("The screen is LOCKED!!!"));
		break;

	case WTS_SESSION_UNLOCK:
		// Screen unlocked
		SetSessionLockFlag(FLAG_OFF);
		OutputDebugLog(_T("The screen is UNLOCKED!!!"));
		break;

	default:
		// Trace other notification codes
		OutputDebugLogFormat(_T("WTS session notification: Code=0x%02X"), wParam);
		break;
	}

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle app command messages (WM_COMMAND)
	Arguments:		wParam - First param (HIWORD)
					lParam - Second param (LOWORD)
 * @return	BOOL
 */
BOOL CPowerPlusDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process commands
	switch (LOWORD(wParam))
	{


/*********************************************************************/
	/*																	 */
	/*				Handle commands for Action menu items				 */
	/*																	 */


/*********************************************************************/
	case IDM_NOTIFY_ACTION_DISPLAYOFF:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_DISPLAYOFF);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_DISPLAYOFF);
		break;
	case IDM_NOTIFY_ACTION_SLEEP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SLEEP);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_SLEEP);
		break;
	case IDM_NOTIFY_ACTION_SHUTDOWN:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SHUTDOWN);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_SHUTDOWN);
		break;
	case IDM_NOTIFY_ACTION_RESTART:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_RESTART);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_RESTART);
		break;
	case IDM_NOTIFY_ACTION_SIGNOUT:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SIGNOUT);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_SIGNOUT);
		break;
	case IDM_NOTIFY_ACTION_HIBERNATE:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_HIBERNATE);
		ExecuteAction(APP_MACRO_ACTION_MENU, APP_ACTION_HIBERNATE);
		break;
	case IDM_NOTIFY_ACTION_SCHEDULE:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SCHEDULE);
		ExecuteAction(APP_MACRO_ACTION_SCHEDULE, GetAppOption(AppOptionID::defaultScheduleActionID));
		break;


/*********************************************************************/
	/*																	 */
	/*				Handle commands for other menu items				 */
	/*																	 */


/*********************************************************************/
	case IDM_NOTIFY_OPENDLG_ABOUT:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_ABOUT);
		OpenChildDialogEx(IDD_ABOUT_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HELP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_HELP);
		OpenChildDialogEx(IDD_HELP_DLG);
		break;
	case IDM_NOTIFY_VIEW_ACTIONLOG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_ACTIONLOG);
		OpenTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
		break;
	case IDM_NOTIFY_BACKUP_CONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_BACKUP_CONFIG);
		((CPowerPlusApp*)AfxGetApp())->BackupRegistryAppData();
		break;
	case IDM_NOTIFY_VIEW_BAKCONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_BAKCONFIG);
		OpenTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
		break;
	case IDM_NOTIFY_OPENDLG_LOGVIEWER:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_LOGVIEWER);
		OpenChildDialogEx(IDD_LOGVIEWER_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_SCHEDULE:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_SCHEDULE);
		OpenChildDialogEx(IDD_MULTISCHEDULE_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HOTKEYSET:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_HOTKEYSET);
		OpenChildDialogEx(IDD_HOTKEYSET_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_PWRREMINDER:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_PWRREMINDER);
		OpenChildDialogEx(IDD_PWRREMINDER_DLG);
		break;
	case IDM_NOTIFY_RESTART_APP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_APP);
		RequestRestartApp(IDM_NOTIFY_RESTART_APP, false);
		break;
	case IDM_NOTIFY_RESTART_ASADMIN:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_ASADMIN);
		RequestRestartApp(IDM_NOTIFY_RESTART_ASADMIN, true);
		break;
	case IDM_NOTIFY_SHOW_WINDOW:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_SHOW_WINDOW);
		ExpandDialog(false);
		PostMessage(SM_WND_SHOWDIALOG, true);
		break;
	case IDM_NOTIFY_EXIT_APP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_EXIT_APP);
		ExitApp(ExitCode::FromNotifyIcon);
		break;
	default:
		break;
	}

	// Default
	return CWnd::OnCommand(wParam, lParam);
}


/**
 * @brief	Handle app window messages
 * @param	message - Message ID
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case SM_APP_TRAYICON:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			OutputEventLog(LOG_EVENT_TRAYICO_LMBCLICKED);
			ExecuteAction(APP_MACRO_LEFT_MOUSE);
			break;
		case WM_MBUTTONDOWN:
			OutputEventLog(LOG_EVENT_TRAYICO_MMBCLICKED);
			ExecuteAction(APP_MACRO_MIDDLE_MOUSE);
			break;
		case WM_RBUTTONUP:
			OutputEventLog(LOG_EVENT_TRAYICO_RMBCLICKED);
			ExecuteAction(APP_MACRO_RIGHT_MOUSE);
			break;
		}
		return true;

	case SM_WND_DEBUGTEST:
		OpenChildDialogEx(IDD_DEBUGTEST_DLG);
		break;
	case WM_QUERYENDSESSION:
		OnQuerryEndSession(NULL, NULL);
		break;
	case WM_HOTKEY:
		ProcessHotkey(static_cast<unsigned>(wParam));
		break;
	case SM_APP_LOCKSTATE_HOTKEY:
		ProcessLockStateHotkey(static_cast<DWORD>(wParam));
		break;
	case WM_POWERBROADCAST:
		OnPowerBroadcastEvent(wParam, NULL);
		break;
	case WM_WTSSESSION_CHANGE:
		OnWTSSessionChange(wParam, lParam);
		break;
	}

	// Special messages
	if (message == WM_TASKBARCREATED) {
		// Re-create notify icon
		CreateNotifyIcon();
		return true;
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
// Member functions using for resizing dialog


/**
 * @brief	Expand/collapse dialog
 * @param	bExpand - Flag to expand or collapse
 * @return	None
 */
void CPowerPlusDlg::ExpandDialog(bool bExpand)
{
	// If new state is the same as current state, do nothing
	bool bCurState = GetFlagValue(AppFlagID::dialogExpanded);
	if (bExpand == bCurState) {
		TRACE("State doesn't change, do not process!!!");
		return;
	}

	CRect rcWnd, rcFrameWnd, rcChild, rcIntersection;
	CWnd* pWndChild = NULL;
	CWnd* pWndFrame = NULL;

	// Get [Expand/Collapse] button
	CWnd* pButton = GetDlgItem(IDC_EXPAND_BTN);
	if (pButton == NULL) {
		TRACE_ERROR("Error: [Expand/Collapse] button not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get separator frame
	pWndFrame = GetDlgItem(IDC_FRAMEWND);
	if (pWndFrame == NULL) {
		TRACE_ERROR("Error: Frame not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}
	
	// Get separator frame rect
	pWndFrame->GetWindowRect(&rcFrameWnd);

	// Update dialog items state when expanding/collapsing
	for (pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		pWndChild->GetWindowRect(&rcChild);
		if (rcChild.bottom > rcFrameWnd.top)
		{
			switch (pWndChild->GetDlgCtrlID())
			{
			case IDC_SYSTEM_TITLE:
			case IDC_SHOWATSTARTUP_CHK:
			case IDC_LANGUAGE_TITLE:
			case IDC_LANGUAGE_LIST:
				// Show/hide these items
				pWndChild->ShowWindow(bExpand);
				break;

			case IDC_HELP_BTN:
			case IDC_ABOUT_BTN:
			case IDC_DEFAULT_BTN:
				{
					// Move buttons
					CRect rcChildNew, rcChildOld;
					this->GetDlgItem(pWndChild->GetDlgCtrlID())->GetWindowRect(&rcChildOld);
					this->ScreenToClient(&rcChildOld);
					if (!bExpand) {
						CRect rcCollapseBtn;
						GetDlgItem(IDC_COLLAPSE_BTNPOS)->GetWindowRect(&rcCollapseBtn);
						this->ScreenToClient(&rcCollapseBtn);
						rcChildNew = rcChildOld;
						rcChildNew.top = rcCollapseBtn.top;
						rcChildNew.bottom = rcCollapseBtn.bottom;
					}
					else {
						CRect rcExpandBtn;
						GetDlgItem(IDC_EXPAND_BTNPOS)->GetWindowRect(&rcExpandBtn);
						this->ScreenToClient(&rcExpandBtn);
						rcChildNew = rcChildOld;
						rcChildNew.top = rcExpandBtn.top;
						rcChildNew.bottom = rcExpandBtn.bottom;
					}
					pWndChild->MoveWindow(&rcChildNew);
				}
				break;

			default:
				pWndChild->EnableWindow(bExpand);
				break;
			}
		}
	}

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Initialize dialog size pointer
	VERIFY_INITIALIZATION(m_pDialogSize, Size);

	// Expand/collapse dialog
	if (!bExpand) {

		// Calculate dialog size
		GetWindowRect(&rcWnd);
		m_pDialogSize->_width = rcWnd.right - rcWnd.left;
		m_pDialogSize->_height = rcWnd.bottom - rcWnd.top;

		// Resize dialog
		SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcFrameWnd.bottom - rcWnd.top, SWP_NOMOVE | SWP_NOZORDER);
		
		// Change [Expand/Collapse] button title
		SetControlText(pButton, IDC_EXPAND_BTN, pAppLang);
	}
	else {
		// Resize dialog
		SetWindowPos(NULL, 0, 0, m_pDialogSize->Width(), m_pDialogSize->Height(), SWP_NOMOVE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Change [Expand/Collapse] button title
		SetControlText(pButton, IDC_COLLAPSE_BTN, pAppLang);
	}

	// Save app event log if enabled
	unsigned nEventID = bExpand ? LOG_EVENT_DLG_EXPANDED : LOG_EVENT_DLG_COLLAPSED;
	OutputEventLog(nEventID, this->GetCaption());

	// Update flag
	bool bNewState = !bCurState;
	SetFlagValue(AppFlagID::dialogExpanded, bNewState);
}


//////////////////////////////////////////////////////////////////////////
// Notify icon functions


/**
 * @brief	Setup and create notify icon
 * @param	None
 * @return	true/false
 */
bool CPowerPlusDlg::CreateNotifyIcon(void)
{
	// If notify icon is showed, re-create it
	if (GetFlagValue(AppFlagID::notifyIconShowed)) {
		TRACE("Notify icon is showed, now it will be removed and re-created!!!");
		RemoveNotifyIcon();
	}

	// Init notify icon
	if (m_pNotifyIconData == NULL) {
		m_pNotifyIconData = new NOTIFYICONDATA;
		if (m_pNotifyIconData == NULL) {
			// Initialization failed
			TRACE_ERROR("Error: Notify icon initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Setup notify icon properties
	m_pNotifyIconData->hWnd = this->GetSafeHwnd();
	m_pNotifyIconData->cbSize = sizeof(NOTIFYICONDATA);
	m_pNotifyIconData->hIcon = m_hDefaultIcon;
	m_pNotifyIconData->uCallbackMessage = SM_APP_TRAYICON;
	m_pNotifyIconData->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_pNotifyIconData->uID = 500;
	SetNotifyTipText(m_pNotifyIconData);

	// Create and show notify icon
	bool bRetCreate = false;
	for (int nRetry = 0; nRetry < Constant::Max::RetryTime; nRetry++) {
		bRetCreate = Shell_NotifyIcon(NIM_ADD, m_pNotifyIconData);
		if (bRetCreate != false) break;
	}

	// Failed to create notify icon
	if (bRetCreate != true) {
		// Failed to create notify icon
		TRACE_ERROR("Error: Failed to create notify icon!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Update flag
	SetFlagValue(AppFlagID::notifyIconShowed, bRetCreate);

	return true;
}


/**
 * @brief	Show notify menu
 * @param	None
 * @param	Return value:	bool - Show menu successfully or failed
 */
bool CPowerPlusDlg::ShowNotifyMenu(void)
{
	// Reset notify menu
	if (m_pNotifyMenu != NULL) {
		m_pNotifyMenu->DestroyMenu();
	}

	// Prepare notify menu
	CMenu menuNotify;
	menuNotify.LoadMenu(IDR_MENU_NOTIFY_DEFAULT);
	m_pNotifyMenu = menuNotify.GetSubMenu(0);
	if (m_pNotifyMenu == NULL) {
		// Trace error
		TRACE_ERROR("Error: Show notify menu failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Setup menu properties
	SetMenuItemText(m_pNotifyMenu);
	UpdateMenuItemState(m_pNotifyMenu);
	SetMenuDefaultItem(m_pNotifyMenu->m_hMenu, IDM_NOTIFY_SHOW_WINDOW, NULL);
	this->SetForegroundWindow();

	// Show notify menu
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	unsigned nFlags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
	bool bResult = m_pNotifyMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y, (CWnd*)this, NULL);

	return bResult;
}


/**
 * @brief	Update notify icon changes
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::UpdateNotifyIcon(void)
{
	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Update notify icon tip text
	SetNotifyTipText(m_pNotifyIconData);
	Shell_NotifyIcon(NIM_MODIFY, m_pNotifyIconData);
}


/**
 * @brief	Remove notify icon when exiting
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::RemoveNotifyIcon(void)
{
	// If notify icon is not showed, do nothing
	if (!GetFlagValue(AppFlagID::notifyIconShowed)) {
		TRACE("Notify icon is not showed!!!");
		return;
	}

	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Remove notify icon
	m_pNotifyIconData->hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, m_pNotifyIconData);

	// Update flag
	SetFlagValue(AppFlagID::notifyIconShowed, false);
}


//////////////////////////////////////////////////////////////////////////
// Data processing functions


/**
 * @brief	Get app data
 * @param	dwDataType - App data type to get
 * @return	None
 */
void CPowerPlusDlg::GetAppData(unsigned dwDataType /* = APPDATA_ALL */)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get config data
	if ((dwDataType & APPDATA_CONFIG) != 0) {
		PCONFIGDATA pcfgData = pApp->GetAppConfigData();
		if (pcfgData != NULL) {
			m_cfgAppConfig.Copy(*pcfgData);
			m_cfgTempConfig.Copy(m_cfgAppConfig);
		}
	}

	// Get schedule data
	if ((dwDataType & APPDATA_SCHEDULE) != 0) {
		ScheduleData* pschData = pApp->GetAppScheduleData();
		if (pschData != NULL) {
			m_schScheduleData.Copy(*pschData);
		}
	}

	// Get HotkeySet data
	if ((dwDataType & APPDATA_HOTKEYSET) != 0) {
		HotkeySetData* phksData = pApp->GetAppHotkeySetData();
		if (phksData != NULL) {
			m_hksHotkeySetData.Copy(*phksData);
		}
	}

	// Get Power Reminder data
	if ((dwDataType & APPDATA_PWRREMINDER) != 0) {
		PwrReminderData* ppwrData = pApp->GetAppPwrReminderData();
		if (ppwrData != NULL) {
			m_prdReminderData.Copy(*ppwrData);
		}
	}
}


/**
 * @brief	Return app option by ID
 * @param	eAppOptionID - ID of specific app option
 * @param	bTemp		 - Temp value or saved value (saved value by default)
 * @return	int - App option value
 */
int CPowerPlusDlg::GetAppOption(AppOptionID eAppOptionID, bool bTemp /* = false */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	switch (eAppOptionID)
	{
	case AppOptionID::curDispLanguage:
		nResult = ((SWinApp*)AfxGetApp())->GetAppLanguageOption(true);
		nTempResult = nResult;		// No temp data
		break;
	case AppOptionID::defaultScheduleActiveState:
		nResult = m_schScheduleData.GetDefaultItem().IsEnabled();
		nTempResult = nResult;		// No temp data
		break;
	case AppOptionID::defaultScheduleActionID:
		nResult = m_schScheduleData.GetDefaultItem().GetAction();
		nTempResult = nResult;		// No temp data
		break;
	case AppOptionID::defaultScheduleRepeat:
		nResult = m_schScheduleData.GetDefaultItem().IsRepeatEnabled();
		nTempResult = nResult;		// No temp data
		break;
	default:
		nResult = m_cfgAppConfig.GetAppOption(eAppOptionID);
		nTempResult = m_cfgTempConfig.GetAppOption(eAppOptionID);
		break;
	}

	// Return temp data if required and the result is valid
	if ((bTemp == true) && (nTempResult != INT_INVALID))
		return nTempResult;

	return nResult;
}


/**
 * @brief	Update data values for dialog items
 * @param	bSaveAndValidate - Same as default MFC UpdateData function
 * @return	None
 */
void CPowerPlusDlg::UpdateDialogData(bool bSaveAndValidate /* = true */)
{
	unsigned nCmbSel = 0;

	if (bSaveAndValidate == false) {

	/*----------------------------<Bind config data to dialog items>----------------------------*/
		// Left mouse button action combo-box
		nCmbSel = m_cfgTempConfig.nLMBAction;
		m_cmbLMBAction.SetCurSel(Opt2Sel(APP_ACTION, nCmbSel));

		// Middle mouse button action combo-box
		nCmbSel = m_cfgTempConfig.nMMBAction;
		m_cmbMMBAction.SetCurSel(Opt2Sel(APP_ACTION, nCmbSel));

		// Right mouse button show menu checkbox
		m_bRMBShowMenu = m_cfgTempConfig.bRMBShowMenu;

		// Right mouse button action combo-box
		nCmbSel = (m_bRMBShowMenu == true) ? APP_ACTION_SHOWMENU : m_cfgTempConfig.nRMBAction;
		m_cmbRMBAction.SetCurSel(Opt2Sel(APP_ACTION, nCmbSel));
		EnableItem(IDC_RMBACTION_LIST, !m_bRMBShowMenu);

		// Update other checkbox-es
		m_bShowDlgAtStartup = m_cfgTempConfig.bShowDlgAtStartup;
		m_bStartupEnabled = m_cfgTempConfig.bStartupEnabled;
		m_bConfirmAction = m_cfgTempConfig.bConfirmAction;
		m_bSaveHistoryLog = m_cfgTempConfig.bSaveHistoryLog;
		m_bSaveAppEventLog = m_cfgTempConfig.bSaveAppEventLog;
		m_bRunAsAdmin = m_cfgTempConfig.bRunAsAdmin;
		m_bShowErrorMsg = m_cfgTempConfig.bShowErrorMsg;
		m_bNotifySchedule = m_cfgTempConfig.bNotifySchedule;
		m_bAllowCancelSchedule = m_cfgTempConfig.bAllowCancelSchedule;
		m_bEnableBackgroundHotkey = m_cfgTempConfig.bEnableBackgroundHotkey;
		m_bEnablePowerReminder = m_cfgTempConfig.bEnablePowerReminder;

		// Language list combo-box
		nCmbSel = Opt2Sel(APP_LANGUAGE, m_cfgTempConfig.nLanguageID);
		m_cmbLanguages.SetCurSel(nCmbSel);

		// Update buttons
		EnableItem(IDC_LOGVIEWER_BTN, m_bSaveAppEventLog);
		EnableItem(IDC_HOTKEYSET_BTN, m_bEnableBackgroundHotkey);
		EnableItem(IDC_PWRREMINDER_BTN, m_bEnablePowerReminder);

		// Update dialog items
		UpdateData(false);

	/*------------------------------------------------------------------------------------------*/
	}
	else {

	/*----------------------------<Bind dialog items data to config>----------------------------*/
		// Update dialog items
		UpdateData(true);

		// Left mouse button action combo-box
		nCmbSel = m_cmbLMBAction.GetCurSel();
		m_cfgTempConfig.nLMBAction = Sel2Opt(APP_ACTION, nCmbSel);

		// Middle mouse button action combo-box
		nCmbSel = m_cmbMMBAction.GetCurSel();
		m_cfgTempConfig.nMMBAction = Sel2Opt(APP_ACTION, nCmbSel);

		// Right mouse button show menu checkbox
		m_cfgTempConfig.bRMBShowMenu = m_bRMBShowMenu;

		// Right mouse button action combo-box
		nCmbSel = Opt2Sel(APP_ACTION, APP_ACTION_SHOWMENU);
		nCmbSel = (m_cfgTempConfig.bRMBShowMenu == true) ? nCmbSel : m_cmbRMBAction.GetCurSel();
		m_cfgTempConfig.nRMBAction = Sel2Opt(APP_ACTION, nCmbSel);

		// Update other checkbox-es
		m_cfgTempConfig.bShowDlgAtStartup = m_bShowDlgAtStartup;
		m_cfgTempConfig.bStartupEnabled = m_bStartupEnabled;
		m_cfgTempConfig.bConfirmAction = m_bConfirmAction;
		m_cfgTempConfig.bSaveHistoryLog = m_bSaveHistoryLog;
		m_cfgTempConfig.bSaveAppEventLog = m_bSaveAppEventLog;
		m_cfgTempConfig.bRunAsAdmin = m_bRunAsAdmin;
		m_cfgTempConfig.bShowErrorMsg = m_bShowErrorMsg;
		m_cfgTempConfig.bNotifySchedule = m_bNotifySchedule;
		m_cfgTempConfig.bAllowCancelSchedule = m_bAllowCancelSchedule;
		m_cfgTempConfig.bEnableBackgroundHotkey = m_bEnableBackgroundHotkey;
		m_cfgTempConfig.bEnablePowerReminder = m_bEnablePowerReminder;

		// Language list combo-box
		nCmbSel = m_cmbLanguages.GetCurSel();
		m_cfgTempConfig.nLanguageID = Sel2Opt(APP_LANGUAGE, nCmbSel);

	/*------------------------------------------------------------------------------------------*/
	}

	// Update base dialog data
	SDialog::UpdateDialogData(bSaveAndValidate);
}


/**
 * @brief	Check setting variables changing state
 * @param	None
 * @return	bool - Change flag
 */
bool CPowerPlusDlg::CheckSettingChangeState(void)
{
	bool bChangeFlag = false;

	bChangeFlag |= (m_cfgTempConfig.nLMBAction != m_cfgAppConfig.nLMBAction);
	bChangeFlag |= (m_cfgTempConfig.nMMBAction != m_cfgAppConfig.nMMBAction);
	bChangeFlag |= (m_cfgTempConfig.nRMBAction != m_cfgAppConfig.nRMBAction);
	bChangeFlag |= (m_cfgTempConfig.bRMBShowMenu != m_cfgAppConfig.bRMBShowMenu);
	bChangeFlag |= (m_cfgTempConfig.nLanguageID != m_cfgAppConfig.nLanguageID);
	bChangeFlag |= (m_cfgTempConfig.bShowDlgAtStartup != m_cfgAppConfig.bShowDlgAtStartup);
	bChangeFlag |= (m_cfgTempConfig.bStartupEnabled != m_cfgAppConfig.bStartupEnabled);
	bChangeFlag |= (m_cfgTempConfig.bConfirmAction != m_cfgAppConfig.bConfirmAction);
	bChangeFlag |= (m_cfgTempConfig.bSaveHistoryLog != m_cfgAppConfig.bSaveHistoryLog);
	bChangeFlag |= (m_cfgTempConfig.bSaveAppEventLog != m_cfgAppConfig.bSaveAppEventLog);
	bChangeFlag |= (m_cfgTempConfig.bRunAsAdmin != m_cfgAppConfig.bRunAsAdmin);
	bChangeFlag |= (m_cfgTempConfig.bShowErrorMsg != m_cfgAppConfig.bShowErrorMsg);
	bChangeFlag |= (m_cfgTempConfig.bNotifySchedule != m_cfgAppConfig.bNotifySchedule);
	bChangeFlag |= (m_cfgTempConfig.bAllowCancelSchedule != m_cfgAppConfig.bAllowCancelSchedule);
	bChangeFlag |= (m_cfgTempConfig.bEnableBackgroundHotkey != m_cfgAppConfig.bEnableBackgroundHotkey);
	bChangeFlag |= (m_cfgTempConfig.bLockStateHotkey != m_cfgAppConfig.bLockStateHotkey);
	bChangeFlag |= (m_cfgTempConfig.bEnablePowerReminder != m_cfgAppConfig.bEnablePowerReminder);

	return bChangeFlag;
}


/**
 * @brief	Return flag value by ID
 * @param	eFlagID - ID of specific flag
 * @return	int - Flag value
 */
int CPowerPlusDlg::GetFlagValue(AppFlagID eFlagID) const
{
	int nValue = FLAG_OFF;

	switch (eFlagID)
	{
	// Application main window runtime flags
	case AppFlagID::notifyIconShowed:
	case AppFlagID::hotkeyRegistered:
	case AppFlagID::restartAsAdmin:
	case AppFlagID::pwrBroadcastSkipCount:
	case AppFlagID::wtsSessionNotifyRegistered:
		nValue = GetAppFlagManager().GetFlagValue(eFlagID);
		break;

	default:
		// Get dialog-base flag value and others
		nValue = SDialog::GetFlagValue(eFlagID);
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
void CPowerPlusDlg::SetFlagValue(AppFlagID eFlagID, int nValue)
{
	// Check value validity
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	// Application main window runtime flags
	case AppFlagID::notifyIconShowed:
	case AppFlagID::hotkeyRegistered:
	case AppFlagID::restartAsAdmin:
	case AppFlagID::pwrBroadcastSkipCount:
	case AppFlagID::wtsSessionNotifyRegistered:
		GetAppFlagManager().SetFlagValue(eFlagID, nValue);
		break;

	default:
		// Set dialog-base-class flag value
		SDialog::SetFlagValue(eFlagID, nValue);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Dialog setup functions


/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::SetupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_FRAMEWND:
		case IDC_COLLAPSE_BTNPOS:
		case IDC_EXPAND_BTNPOS:
			// Skip these items
			break;

		case IDC_LMBACTION_LIST:
		case IDC_MMBACTION_LIST:
		case IDC_RMBACTION_LIST:
		case IDC_LANGUAGE_LIST:
			SetupComboBox(nID, pAppLang);
			break;

		case IDC_EXPAND_BTN:
		{
			// Check dialog current state
			int nState = GetFlagValue(AppFlagID::dialogExpanded);
			if (nState == true)	nID = IDC_COLLAPSE_BTN;
			else nID = IDC_EXPAND_BTN;
			SetControlText(pWndChild, nID, pAppLang);
		} break;

		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Default
	SDialog::SetupLanguage();
}


/**
 * @brief	Setup data for combo-boxes
 * @param	nComboID	- ID of combo box
 * @param	ptrLanguage - Language package pointer
 * @return	None
 */
void CPowerPlusDlg::SetupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage)
{
	switch (nComboID)
	{
	case IDC_LMBACTION_LIST:
		// Left mouse button action list
		m_cmbLMBAction.ResetContent();
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SLEEP));			// Sleep
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbLMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;

	case IDC_MMBACTION_LIST:
		// Middle mouse button action list
		m_cmbMMBAction.ResetContent();
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SLEEP));			// Sleep
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbMMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;

	case IDC_RMBACTION_LIST:
		// Right mouse button action list
		m_cmbRMBAction.ResetContent();
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SLEEP));			// Sleep
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		m_cmbRMBAction.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHOWMENU));			// Just show menu
		break;

	case IDC_LANGUAGE_LIST:
		// App language list
		m_cmbLanguages.ResetContent();
		m_cmbLanguages.AddString(GetLanguageString(ptrLanguage, COMBOBOX_LANGUAGE_ENGLISH));		// English
		m_cmbLanguages.AddString(GetLanguageString(ptrLanguage, COMBOBOX_LANGUAGE_VIETNAMESE));		// Vietnamese
		m_cmbLanguages.AddString(GetLanguageString(ptrLanguage, COMBOBOX_LANGUAGE_SIMPCHINESE));	// Chinese
		break;
	}

	// Default
	SDialog::SetupComboBox(nComboID, ptrLanguage);
}


//////////////////////////////////////////////////////////////////////////
// Item state/checkbox update functions


/**
 * @brief	Enable/disable right mouse action combo-box
 * @param	bEnable - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::EnableRightMouseMenu(bool /*bEnable*/)
{
	UpdateDialogData(false);
}


/**
 * @brief	Enable/disable LogViewer function
 * @param	bEnable - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::EnableLogViewer(bool bEnable)
{
	// Prefer using app data option than temp config option
	bool bCheck = bEnable;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		bCheck = pApp->GetAppOption(AppOptionID::saveAppEventLog);
	}
	if (bCheck == false) {
		EnableItem(IDC_LOGVIEWER_BTN, bCheck);
		return;
	}

	// Update dialog data
	UpdateDialogData(false);
}


/**
 * @brief	Enable/disable HotKeySet button
 * @param	bEnable - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::EnableBackgroundHotkey(bool /*bEnable*/)
{
	UpdateDialogData(false);
}


/**
 * @brief	Enable/disable Power Reminder button
 * @param	bEnable - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::EnablePowerReminder(bool /*bEnable*/)
{
	UpdateDialogData(false);
}


/**
 * @brief	Update restart as admin flag
 * @param	bFlag - Update flag
 * @return	None
 */
void CPowerPlusDlg::UpdateRestartAsAdminFlag(bool bFlag)
{
	// Check current set app data option
	bool bCheck = GetAppOption(AppOptionID::runAsAdmin);

	// If option is currently ON, do not update flag
	if (bCheck == true)	return;

	// Update flag
	SetFlagValue(AppFlagID::restartAsAdmin, bFlag);
}


//////////////////////////////////////////////////////////////////////////
// Component update functions


/**
 * @brief	Setup language for menu items
 * @param	pMenu - Menu pointer
 * @return	None
 */
void CPowerPlusDlg::SetMenuItemText(CMenu* pMenu)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup language for entry menu
	for (int nItem = 0; nItem < pMenu->GetMenuItemCount(); nItem++) {
		unsigned nID = pMenu->GetMenuItemID(nItem);
		if (nID == 0) continue;
		// "Actions" child pop-up menu title
		if (nItem == IDM_NOTIFY_ACTIONS_TITLE) {
			pMenu->ModifyMenu(nItem, MF_BYPOSITION | MF_STRING, NULL, GetLanguageString(pAppLang, nItem));
			continue;
		}

		// Set menu item title
		pMenu->ModifyMenu(nItem, MF_BYPOSITION | MF_STRING, nID, GetLanguageString(pAppLang, nID));
	}

	// Setup language for sub menu
	CMenu* pSubMenu = pMenu->GetSubMenu(IDM_NOTIFY_ACTIONS_TITLE);
	if (pSubMenu == NULL) return;
	for (int nItem = 0; nItem < pSubMenu->GetMenuItemCount(); nItem++) {
		unsigned nID = pSubMenu->GetMenuItemID(nItem);
		if (nID == 0) continue;

		// Set menu item title
		pSubMenu->ModifyMenu(nItem, MF_BYPOSITION | MF_STRING, nID, GetLanguageString(pAppLang, nID));
	}
}


/**
 * @brief	Update state for menu items
 * @param	pMenu - Menu pointer
 * @return	None
 */
void CPowerPlusDlg::UpdateMenuItemState(CMenu* pMenu)
{
	// Check validity
	if (pMenu == NULL) {
		// Trace error
		TRACE_ERROR("Error: Menu does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Main menu
	for (int nItem = 0; nItem < pMenu->GetMenuItemCount(); nItem++) {
		bool bShowItem = true;
		unsigned nID = pMenu->GetMenuItemID(nItem);
		switch (nID)
		{
		case IDM_NOTIFY_OPENDLG_LOGVIEWER:
			bShowItem = GetAppOption(AppOptionID::saveAppEventLog);
			if (bShowItem == true)
				bShowItem = GetAppOption(AppOptionID::saveAppEventLog, true);
			break;

		case IDM_NOTIFY_OPENDLG_HOTKEYSET:
			bShowItem = GetAppOption(AppOptionID::backgroundHotkeyEnabled, true);
			break;

		case IDM_NOTIFY_OPENDLG_PWRREMINDER:
			bShowItem = GetAppOption(AppOptionID::pwrReminderEnabled, true);
			break;

		default:
			continue;
		}

		// Disable item
		if (bShowItem == false) {
			pMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}

	// Sub-menu: Actions
	CMenu* pSubMenu = pMenu->GetSubMenu(IDM_NOTIFY_ACTIONS_TITLE);
	if (pSubMenu == NULL) return;
	for (int nItem = 0; nItem < pSubMenu->GetMenuItemCount(); nItem++) {
		bool bShowItem = true;
		unsigned nID = pSubMenu->GetMenuItemID(nItem);
		switch (nID)
		{
		case IDM_NOTIFY_ACTION_SCHEDULE:
			bShowItem = GetAppOption(AppOptionID::defaultScheduleActiveState);
			bShowItem &= (GetAppOption(AppOptionID::defaultScheduleActionID) != APP_ACTION_NOTHING);
			break;

		default:
			continue;
		}

		// Disable item
		if (bShowItem == false) {
			pSubMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
}


/**
 * @brief	Get notify tip text
 * @param	pNotifyIconData - Notify icon data pointer
 * @return	const wchar_t*
 */
const wchar_t* CPowerPlusDlg::GetNotifyTipText(PNOTIFYICONDATA pNotifyIconData)
{
	// Check validity
	if (pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Null;
	}

	return pNotifyIconData->szTip;
}


/**
 * @brief	Set notify tip text
 * @param	pNotifyIconData - Notify icon data pointer
 * @return	None
 */
void CPowerPlusDlg::SetNotifyTipText(PNOTIFYICONDATA pNotifyIconData)
{
	// Check validity
	if (pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	StringArray arrTipText;
	arrTipText.reserve(3);

	// Load language strings
	String formatString = GetLanguageString(pAppLang, NOTIFY_TIP_TEMPLATE);
	arrTipText.push_back(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nLMBAction)));
	arrTipText.push_back(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nMMBAction)));
	arrTipText.push_back(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nRMBAction)));

	// Format notify tip text
	String notifyTipText = StringUtils::StringFormat(formatString, arrTipText.at(0).GetString(), arrTipText.at(1).GetString(), arrTipText.at(2).GetString());

	// Set notify tip text
	StrCpyW(pNotifyIconData->szTip, notifyTipText.GetString());
}


/**
 * @brief	Show balloon tip to notify schedule
 * @param	nCurLanguage - Current language ID
 * @param	nScheduleAction - Upcoming schedule action
 * @param	nSecondLeft  - Number of second left
 * @return	None
 */
void CPowerPlusDlg::SetBalloonTipText(unsigned /*nCurLanguage*/, unsigned nScheduleAction, unsigned nSecondLeft)
{
	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Setup balloon tip properties
	m_pNotifyIconData->uFlags = NIF_INFO;
	m_pNotifyIconData->dwInfoFlags = NIIF_INFO;
	m_pNotifyIconData->uTimeout = 300;

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Load language strings
	const wchar_t* formatString = GetLanguageString(pAppLang, BALLOON_TIP_TEMPLATE);
	const wchar_t* balloonText = GetLanguageString(pAppLang, GetPairedID(IDTable::BalloonTip, nScheduleAction));

	String balloonTipText = StringUtils::StringFormat(formatString, balloonText, nSecondLeft);

	// Set balloon tip text
	StrCpy(m_pNotifyIconData->szInfoTitle, (wchar_t*)(const wchar_t*)GetLanguageString(pAppLang, IDC_APPNAME_LABEL));
	StrCpy(m_pNotifyIconData->szInfo, (wchar_t*)balloonTipText.GetString());
	Shell_NotifyIcon(NIM_MODIFY, m_pNotifyIconData);
}


//////////////////////////////////////////////////////////////////////////
// Core functions


/**
 * @brief	Execute action as config/schedule/menu selection
 * @param	nActionMacro - Action macro
 * @param	wParam		 - First param (HIWORD)
 * @param	lParam		 - Second param (LOWORD)
 * @return	bool - Result of action execution
 */
bool CPowerPlusDlg::ExecuteAction(unsigned nActionMacro, WPARAM wParam /* = NULL */, LPARAM /* lParam = NULL */)
{
	unsigned nActionType = 0;
	unsigned nActionID = 0;
	unsigned nHistoryActionID = 0;
	unsigned nActionNameLangID = 0;
	unsigned nMessage = 0;

	// Output debug log
	OutputDebugLogFormat(_T("Execute action: Type=0x%04X, Param=0x%04X"), nActionMacro, (unsigned)wParam);

	// Get action ID
	switch (nActionMacro)
	{
	case APP_MACRO_LEFT_MOUSE:
		// Get action ID: Left mouse
		nActionID = GetAppOption(AppOptionID::leftMouseAction);
		break;

	case APP_MACRO_MIDDLE_MOUSE:
		// Get action ID: Middle mouse
		nActionID = GetAppOption(AppOptionID::middleMouseAction);
		break;

	case APP_MACRO_ACTION_SCHEDULE:
		// Get action ID from param
		if (wParam == NULL) {
			// Trace error
			TRACE_ERROR("Error: Invalid parameter!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
		nActionID = DEFAULT_CAST(unsigned, wParam);
		break;

	case APP_MACRO_RIGHT_MOUSE:
		// If right mouse action is set to show notify menu
		if ((GetAppOption(AppOptionID::rightMouseAction) == APP_ACTION_SHOWMENU) ||
			(GetAppOption(AppOptionID::rightMouseShowMenu) == true))
			return ShowNotifyMenu();

		// Otherwise, get action ID: Right mouse
		nActionID = GetAppOption(AppOptionID::rightMouseAction);
		break;

	case APP_MACRO_ACTION_MENU:
	case APP_MACRO_ACTION_HOTKEY:
		// Get action ID from param
		if (wParam == NULL) {
			// Trace error
			TRACE_ERROR("Error: Invalid parameter!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
		nActionID = DEFAULT_CAST(unsigned, wParam);
		break;

	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get action info
	switch (nActionID)
	{
	case APP_ACTION_NOTHING:
		// Do nothing (litterally)
		// Pretend that the action completes successfully
		TRACE("[ExecuteAction] Do nothing -> Successfully");
		return true;

	case APP_ACTION_DISPLAYOFF:
		// Turn off display
		nActionType = APP_ACTIONTYPE_MONITOR;
		nMessage = APP_MESSAGE_DISPLAYOFF;
		nHistoryActionID = HistoryAction::DisplayOff;
		nActionNameLangID = ACTION_NAME_DISPLAYOFF;
		break;

	case APP_ACTION_SLEEP:
		// Sleep
		nActionType = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SLEEP;
		nHistoryActionID = HistoryAction::SleepMode;
		nActionNameLangID = ACTION_NAME_SLEEP;
		break;

	case APP_ACTION_SHUTDOWN:
		// Shutdown
		nActionType = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SHUTDOWN;
		nHistoryActionID = HistoryAction::Shutdown;
		nActionNameLangID = ACTION_NAME_SHUTDOWN;
		break;

	case APP_ACTION_RESTART:
		// Restart
		nActionType = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_REBOOT;
		nHistoryActionID = HistoryAction::Restart;
		nActionNameLangID = ACTION_NAME_RESTART;
		break;

	case APP_ACTION_SIGNOUT:
		// Sign out
		nActionType = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SIGNOUT;
		nHistoryActionID = HistoryAction::SignOut;
		nActionNameLangID = ACTION_NAME_SIGNOUT;
		break;

	case APP_ACTION_HIBERNATE:
		// Hibernate
		nActionType = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_HIBERNATE;
		nHistoryActionID = HistoryAction::Hibernate;
		nActionNameLangID = ACTION_NAME_HIBERNATE;
		break;

	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Action result info
	bool bResult = false;
	DWORD dwErrorCode = APP_ERROR_SUCCESS;

	// Confirm before executing action
	if (ConfirmActionExec(nActionMacro, nActionID) == IDYES) {

		// Execute Power Reminder before doing action
		ExecutePowerReminder(PwrReminderEvent::beforePwrAction);

		// Check if DummyTest mode is enabled or not
		bool bDummyTestMode = GetDummyTestMode();

		if (bDummyTestMode != true) {
			// Normal mode
			bResult = ExecutePowerAction(nActionType, nMessage, dwErrorCode);
		}
		else {
			// DummyTest mode
			bResult = ExecutePowerActionDummy(nActionType, nMessage, dwErrorCode);
		}

		// Save Power Action trace flag
		SetPwrActionFlag((bResult == true) ? FLAG_ON : FLAG_OFF);
		CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
		if (pApp != NULL) {
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}

		// Collect power action history info
		InitPwrActionHistoryInfo(nHistoryActionID, bResult, dwErrorCode);
		SaveHistoryInfoData();

		// Show error message
		ShowErrorMessage(dwErrorCode);
	}
	else {
		// Power action canceled --> Output event log
		String pwrActionNameString;
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (pAppLang != NULL) {
			pwrActionNameString = GetLanguageString(pAppLang, nActionNameLangID);
		}
		OutputEventLog(LOG_EVENT_CANCEL_PWRACTION, pwrActionNameString);
	}

	return bResult;
}


/**
 * @brief	Apply changes and minimize window to tray
 * @param	bMinimize - Minimize to tray after applying changes
 * @return	None
 */
void CPowerPlusDlg::ApplySettings(bool bMinimize)
{
	// Update data
	UpdateDialogData(true);

	// Copy configurations and save
	m_cfgAppConfig.Copy(m_cfgTempConfig);
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		pApp->SetAppConfigData(&m_cfgAppConfig);
		bool bRet = pApp->SaveRegistryAppData();
		if (bRet == false) {
			// Show error message
			DWORD dwErr = APP_ERROR_SAVE_CFG_FAILED;
			ShowErrorMessage(dwErr);
		}
	}

	// Restart as admin privileges if triggered
	bool bRestartTrigger = GetFlagValue(AppFlagID::restartAsAdmin);
	if (bRestartTrigger == true) {
		RequestRestartApp(IDC_APPLY_BTN, true);
	}

	// Update background hotkeys if enabled
	SetupBackgroundHotkey(Mode::Update);

	// Disable Power Reminder snooze queue if feature's disabled
	if (GetAppOption(AppOptionID::pwrReminderEnabled) == NULL) {
		UpdatePwrReminderSnooze(Mode::Disable);
	}

	// Update notify icon tip text
	UpdateNotifyIcon();

	// Change display status
	if (bMinimize == true) {
		// Minimize to tray (hide dialog)
		ShowDialog(this, false);
	}
}


/**
 * @brief	Abort all changes and reload settings
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::ReloadSettings(void)
{
	// Reload app data
	GetAppData();

	// Reload app language
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();

	// Reupdate dialog items
	SetupLanguage();
	UpdateDialogData(false);

	// Reset data change flag
	SetFlagValue(AppFlagID::dialogDataChanged, false);
}


/**
 * @brief	Set default config data and re-update display
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::SetDefaultConfig(void)
{
	// Set default options
	m_cfgTempConfig.SetDefaultData();

	// Reload app language & reset language display
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();
	SetupLanguage();

	// Reupdate dialog items
	UpdateDialogData(false);
}


/**
 * @brief	Restart application with/without admin privileges
 * @param	bRestartAsAdmin - Restart with/without admin privileges
 * @return	None
 */
void CPowerPlusDlg::RestartApp(bool bRestartAsAdmin)
{
	// Remove window title to prevent from startup checking
	this->SetWindowText(Constant::String::Null);

	// Exit the current application instance
	ExitApp(ExitCode::RestartApp);

	// Restart immediately
	RunApp(StringUtils::GetApplicationPath(true), bRestartAsAdmin);
}


/**
 * @brief	Safely trigger exitting the application
 * @param	nExitCode - Application exit code (use for PostQuitMessage)
 * @return	None
 */
void CPowerPlusDlg::ExitApp(int nExitCode)
{
	// Request closing opening dialogs and do clean-up
	if (PreDestroyDialog() != Result::Success) {
		TRACE("Request close application denied!!!");
		return;
	}

	// Tell the application by updating flag value
	SetFlagValue(AppFlagID::appExitCode, nExitCode);

	// Termination
	// Trigger exitting by posting quit message
	PostQuitMessage(nExitCode);
}


//////////////////////////////////////////////////////////////////////////
// Dialog and window functions


/**
 * @brief	Show/hide dialog out of/into system tray
 * @param	pWnd	  - Pointer of the dialog to show/hide
 * @param	bShowFlag - Flag to show/hide dialog
 * @return	None
 */
void CPowerPlusDlg::ShowDialog(CWnd* pWnd, bool bShowFlag /* = true */)
{
	// Get show/hide flag
	int nCmdShow = (bShowFlag == true) ? SW_SHOW : SW_HIDE;

	// Check dialog pointer validity
	if (pWnd == NULL) {
		// Trace error
		TRACE_ERROR("Error: Window does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Show/hide dialog
	pWnd->ShowWindow(nCmdShow);
	if (nCmdShow == SW_SHOW) {
		pWnd->BringWindowToTop();
	}

	// Save app event log if enabled
	unsigned nEventID = (bShowFlag == true) ? LOG_EVENT_DLG_SHOWED : LOG_EVENT_DLG_HIDDEN;
	OutputEventLog(nEventID, ((SDialog*)pWnd)->GetCaption());
}


/**
 * @brief	Open child dialog with corresponding ID
 * @param	nDialogID - Child dialog ID
 * @return	None
 */
void CPowerPlusDlg::OpenChildDialogEx(unsigned nDialogID)
{
	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// About dialog
	if (nDialogID == IDD_ABOUT_DLG) {
		if (m_pAboutDlg == NULL) {
			// Initialize dialog
			m_pAboutDlg = new CAboutDlg();
			m_pAboutDlg->SetParentWnd(this);
			m_pAboutDlg->DoModal();
		}
		else {
			// Show dialog
			m_pAboutDlg->SetParentWnd(this);
			ShowDialog(m_pAboutDlg, true);
		}
	}
	// Help dialog
	else if (nDialogID == IDD_HELP_DLG) {
		if (m_pHelpDlg == NULL) {
			// Initialize dialog
			m_pHelpDlg = new CHelpDlg();
			m_pHelpDlg->SetParentWnd(this);
			m_pHelpDlg->DoModal();
		}
		else {
			// Show dialog
			m_pHelpDlg->SetParentWnd(this);
			ShowDialog(m_pHelpDlg, true);
		}
	}
	// Multi schedule dialog
	else if (nDialogID == IDD_MULTISCHEDULE_DLG) {
		if (m_pMultiScheduleDlg == NULL) {
			// Initialize dialog
			m_pMultiScheduleDlg = new CMultiScheduleDlg();
			m_pMultiScheduleDlg->SetParentWnd(this);
			m_pMultiScheduleDlg->DoModal();
		}
		else {
			// Show dialog
			m_pMultiScheduleDlg->SetParentWnd(this);
			ShowDialog(m_pMultiScheduleDlg, true);
		}
	}
	// LogViewer dialog
	else if (nDialogID == IDD_LOGVIEWER_DLG) {
		if (m_pLogViewerDlg == NULL) {
			// Initialize dialog
			m_pLogViewerDlg = new CLogViewerDlg();
			m_pLogViewerDlg->SetParentWnd(this);
			m_pLogViewerDlg->DoModal();
		}
		else {
			// Show dialog
			m_pLogViewerDlg->SetParentWnd(this);
			ShowDialog(m_pLogViewerDlg, true);
		}
	}
	// HotkeySet dialog
	else if (nDialogID == IDD_HOTKEYSET_DLG) {
		if (m_pHotkeySetDlg == NULL) {
			// Initialize dialog
			m_pHotkeySetDlg = new CHotkeySetDlg();
			m_pHotkeySetDlg->SetParentWnd(this);
			m_pHotkeySetDlg->DoModal();
		}
		else {
			// Show dialog
			m_pHotkeySetDlg->SetParentWnd(this);
			ShowDialog(m_pHotkeySetDlg, true);
		}
	}
	// Power Reminder dialog
	else if (nDialogID == IDD_PWRREMINDER_DLG) {
		if (m_pPwrReminderDlg == NULL) {
			// Initialize dialog
			m_pPwrReminderDlg = new CPwrReminderDlg();
			m_pPwrReminderDlg->SetParentWnd(this);
			m_pPwrReminderDlg->DoModal();
		}
		else {
			// Show dialog
			m_pPwrReminderDlg->SetParentWnd(this);
			ShowDialog(m_pPwrReminderDlg, true);
		}
	}
	// DebugTest dialog
	else if (nDialogID == IDD_DEBUGTEST_DLG) {
		// Get app DebugTest dialog
		SDialog* pDialog = pApp->GetDebugTestDlg();
		if (pDialog == NULL) {

			// Initialize dialog
			bool bRetInit = pApp->InitDebugTestDlg();
			if (bRetInit != false) {

				// Re-acquire DebugTest dialog pointer
				pDialog = pApp->GetDebugTestDlg();
				if (pDialog != NULL) {
					// Set parent window
					pDialog->SetParentWnd(this);

					// Open in modeless mode
					pDialog->Create(nDialogID, this);
					pDialog->ShowWindow(SW_SHOW);
				}
			}
		}
		else {
			// If DebugTest dialog is already created
			if (::IsWindow(pDialog->GetSafeHwnd())) {
				// Show dialog
				pDialog->SetParentWnd(this);
				ShowDialog(pDialog, true);
			}
			else {
				// Set parent window
				pDialog->SetParentWnd(this);

				// Open in modeless mode
				pDialog->Create(nDialogID, this);
				pDialog->ShowWindow(SW_SHOW);
			}
		}

		// Bring to top (by default)
		pDialog->PostMessage(SM_WND_SHOWDIALOG, true);
	}

	// Base class processing
	SDialog::OpenChildDialogEx(nDialogID);
}


/**
 * @brief	Open dialog with corresponding ID
 * @param	nDialogID	  - Dialog ID
 * @param	bReadOnlyMode - Read-only mode flag
 * @param	nOpenMode	  - Open mode: Modal or modeless
 * @return	None
 * @note:	Base function (no longer used)
 */
void CPowerPlusDlg::OpenDialogBase(unsigned nDialogID, bool bReadOnlyMode /* = false */, int nOpenMode /* = DEF_MODE_OPENDLG_MODAL */)
{
	// Check if there is any other instance of dialog currently running,
	HWND hDialogWnd = NULL;
	if (nDialogID == IDD_DEBUGTEST_DLG) {
		// Find DebugTest dialog
		hDialogWnd = FindDebugTestDlg();
	}
	else {
		// Find dialog by title
		LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		String dialogTitle = GetLanguageString(pLang, nDialogID);
		hDialogWnd = ::FindWindow(NULL, dialogTitle);
	}

	// If yes, only bring that instance up to top instead of starting a new one
	if (hDialogWnd != NULL) {
		CWnd* pDialogWnd = FromHandle(hDialogWnd);
		ShowDialog(pDialogWnd, true);
		return;
	}

	// Dialog initialization
	SDialog* pDialog = NULL;
	bool bSetReadOnly = false;
	CWnd* pParentWnd = NULL;
	unsigned nEntryMsgID = INT_NULL;
	{
		switch (nDialogID)
		{
		case IDD_ABOUT_DLG:
			// About dialog
			pDialog = new CAboutDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_HELP_DLG:
			// Help dialog
			pDialog = new CHelpDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_MULTISCHEDULE_DLG:
			// Multi schedule dialog
			pDialog = new CMultiScheduleDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_LOGVIEWER_DLG:
			// LogViewer dialog
			pDialog = new CLogViewerDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_HOTKEYSET_DLG:
			// HotkeySet dialog
			pDialog = new CHotkeySetDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_PWRREMINDER_DLG:
			// Power Reminder dialog
			pDialog = new CPwrReminderDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			break;

		case IDD_DEBUGTEST_DLG:
			// DebugTest dialog
			pDialog = new CDebugTestDlg;
			bSetReadOnly = false;
			pParentWnd = this;
			nOpenMode = MODE_OPENDLG_MODELESS;
			break;

		default:
			break;
		}

		// Show entry message before displaying dialog
		if (nEntryMsgID != INT_NULL) {
			DisplayMessageBox(nEntryMsgID);
		}

		// Check validity
		if (pDialog == NULL)
			return;

		// Set read-only mode
		if (bSetReadOnly == true) {
			pDialog->SetReadOnlyMode(bReadOnlyMode);
		}

		// Open dialog
		if (nOpenMode == MODE_OPENDLG_MODAL) {
			// Modal dialog
			pDialog->SetParentWnd(pParentWnd);
			pDialog->DoModal();
		}
		else if (nOpenMode == MODE_OPENDLG_MODELESS) {
			// Modeless dialog
			pDialog->Create(nDialogID, pParentWnd);
			pDialog->ShowWindow(SW_SHOW);
			pDialog->RunModalLoop();
		}
	}

	// Delete dialog pointer when done
	if (pDialog != NULL) {
		delete pDialog;
		pDialog = NULL;
	}
}


/**
 * @brief	Open text file to view using Notepad
 * @param	fileName  - File name/path
 * @param	extension - File extension
 * @param	subDir	  - Sub-directory name
 * @return	bool - Result of file opening
 */
bool CPowerPlusDlg::OpenTextFileToView(const wchar_t* fileName, const wchar_t* extension, const wchar_t* subDir /* = Constant::String::Empty */)
{
	// Get file name
	VERIFY(fileName != NULL);
	String filePath(fileName);
	filePath.Append(extension);
	
	// If sub-directory name is not empty
	if (IS_NOT_EMPTY_STRING(subDir)) {
		// Format file path with sub-directory
		String folderPath = StringUtils::GetSubFolderPath(subDir);
		filePath = StringUtils::MakeFilePath(folderPath, fileName, extension);
	}

	// Standard file view
	return FileViewStd(FILETYPE_TEXT, filePath);
}


//////////////////////////////////////////////////////////////////////////
// Action Schedule feature functions


/**
 * @brief	Process Action schedule function
 * @param	None
 * @return	bool - Schedule processing result
 */
bool CPowerPlusDlg::ProcessActionSchedule(void)
{
	bool bResult = false;

	// Get current time
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

	// Flag that trigger to reupdate schedule data
	bool bTriggerReupdate = false;

	// Get default schedule item
	ScheduleItem& schDefaultItem = m_schScheduleData.GetDefaultItem();
	{
		// Flag to skip processing schedule item
		bool bSkipProcess = false;

		// If item is not enabled
		if (schDefaultItem.IsEnabled() == false) {
			// Do not process
			bSkipProcess = true;
		}

		// If repeat option is ON and is set as active in current day of week
		if ((schDefaultItem.IsRepeatEnabled() == true) && (!schDefaultItem.IsDayActive((DayOfWeek)currentDateTime.DayOfWeek()))) {
			// Do not process
			bSkipProcess = true;
		}

		// Process default schedule
		if (bSkipProcess != true) {

			// Check for time matching and trigger schedule notifying if enabled
			if (GetAppOption(AppOptionID::notifySchedule) == true) {
				bool bTriggerNotify = ClockTimeUtils::IsMatching(currentDateTime.GetClockTime(), schDefaultItem.GetTime(), -30);
				if (bTriggerNotify == true) {
					// Do notify schedule (and check for trigger reupdate)
					NotifySchedule(&schDefaultItem, bTriggerReupdate);
					bResult = false;
				}
			}

			// Check for time matching and trigger the scheduled action
			bool bTriggerAction = ClockTimeUtils::IsMatching(currentDateTime.GetClockTime(), schDefaultItem.GetTime());
			if (bTriggerAction == true) {
				
				// Check if item is marked as skipped
				bool bSkipFlag = GetActionScheduleSkipStatus(schDefaultItem.GetItemID());
				if (bSkipFlag != true) {

					// Output event log: Schedule executed
					OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schDefaultItem);

					// Save history info data
					InitScheduleHistoryInfo(schDefaultItem);
					SaveHistoryInfoData();

					// Execute schedule action
					bResult = ExecuteAction(APP_MACRO_ACTION_SCHEDULE, schDefaultItem.GetAction());

					// If "Repeat" option is not ON,
					// --> Disable schedule item after done
					if (schDefaultItem.IsRepeatEnabled() == false) {
						schDefaultItem.EnableItem(false);
						bTriggerReupdate |= true;
					}
				}
				else {
					// Process failed
					bResult = false;
				}

				// Set item as no longer skipped
				SetActionScheduleSkip(schDefaultItem, FLAG_OFF);
			}
		}
		else {
			// Process failed
			bResult = false;
		}
	}

	// Loop through each extra item and process
	for (int nExtraIndex = 0; nExtraIndex < m_schScheduleData.GetExtraItemNum(); nExtraIndex++) {

		// Get schedule item
		ScheduleItem& schExtraItem = m_schScheduleData.GetItemAt(nExtraIndex);

		// Do not process if repeat option is ON but is not set as active in current day of week
		if ((schExtraItem.IsRepeatEnabled() == true) && (!schExtraItem.IsDayActive((DayOfWeek)currentDateTime.DayOfWeek())))
			continue;

		// Check for time matching and trigger schedule notifying if enabled
		if (GetAppOption(AppOptionID::notifySchedule) == true) {
			bool bTriggerNotify = ClockTimeUtils::IsMatching(currentDateTime.GetClockTime(), schExtraItem.GetTime(), -30);
			if (bTriggerNotify == true) {
				// Do notify schedule (and check for trigger reupdate)
				NotifySchedule(&schExtraItem, bTriggerReupdate);
				bResult = false;
				continue;
			}
		}

		// Check for time matching and trigger the scheduled action
		bool bTriggerAction = ClockTimeUtils::IsMatching(currentDateTime.GetClockTime(), schExtraItem.GetTime());
		if (bTriggerAction == true) {

			// Check if item is marked as skipped
			bool bSkipFlag = GetActionScheduleSkipStatus(schExtraItem.GetItemID());
			if (bSkipFlag != true) {

				// Output event log: Schedule executed
				OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schExtraItem);

				// Save history info data
				InitScheduleHistoryInfo(schExtraItem);
				SaveHistoryInfoData();

				// Execute schedule action
				bResult = ExecuteAction(APP_MACRO_ACTION_SCHEDULE, schExtraItem.GetAction());

				// If "Repeat" option is not ON,
				// --> Disable schedule item after done
				if (schExtraItem.IsRepeatEnabled() == false) {
					schExtraItem.EnableItem(false);
					bTriggerReupdate |= true;
				}
			}
			else {
				// Process failed
				bResult = false;
			}

			// Set item as no longer skipped
			SetActionScheduleSkip(schExtraItem, FLAG_OFF);
		}
	}

	// Reupdate flag is triggered
	if (bTriggerReupdate == true) {
		// Reupdate schedule data
		ReupdateActionScheduleData();
		bResult = true;
	}

	return bResult;
}


/**
 * @brief	Reupdate Action Schedule data
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::ReupdateActionScheduleData(void)
{
	// Disable Action schedule items
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		// Update schedule data
		pApp->SetAppScheduleData(&m_schScheduleData);
		pApp->SaveRegistryAppData(APPDATA_SCHEDULE);
	}

	// Trigger reupdate schedule data
	PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);
}


/**
 * @brief	Setup Action Schedule item skip mode
 * @param	schItem	  - Action schedule item
 * @param	nSkipFlag - Skip flag
 * @return	None
 */
void CPowerPlusDlg::SetActionScheduleSkip(const ScheduleItem& schItem, int nSkipFlag)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty()) return;

	// Find if item skip mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == schItem.GetItemID()) {

			// Update item skip mode data
			pwrRuntimeItem.SetSkipFlag(nSkipFlag);
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(PwrFeatureID::schedule);
	pwrRuntimeItem.SetItemID(schItem.GetItemID());
	pwrRuntimeItem.SetSkipFlag(nSkipFlag);

	// Add item to runtime queue
	m_arrRuntimeQueue.push_back(pwrRuntimeItem);
}


/**
 * @brief	Setup Action Schedule item snooze mode
 * @param	schItem		- Action schedule item
 * @param	nSnoozeFlag - Snooze flag
 * @return	None
 */
void CPowerPlusDlg::SetActionScheduleSnooze(const ScheduleItem& schItem, int nSnoozeFlag)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty()) return;

	// If snoozing option is not available
	if (schItem.IsAllowSnoozing() != true) {
		// Turn off flag
		nSnoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int nInterval = PwrRepeatSet::defaultSnoozeInterval;

	// Find if item skip mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == schItem.GetItemID()) {

			// Update item skip mode data
			pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);

			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.SetTime(ClockTimeUtils::GetCurrentClockTime());
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(PwrFeatureID::schedule);
	pwrRuntimeItem.SetItemID(schItem.GetItemID());
	pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);
	if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.SetTime(ClockTimeUtils::GetCurrentClockTime());
		pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
	}

	// Add item to runtime queue
	m_arrRuntimeQueue.push_back(pwrRuntimeItem);
}


/**
 * @brief	Update Action Schedule runtime queue data
 * @param	nMode - Update mode flag (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::UpdateActionScheduleQueue(int nMode)
{
	if (nMode == Mode::Init) {

		// Initialize Action Schedule runtime queue
		// Set all items as not skipped
		SetActionScheduleSkip(m_schScheduleData.GetDefaultItem(), FLAG_OFF);
		for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
			SetActionScheduleSkip(m_schScheduleData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == Mode::Update) {

		// Update skip queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.size() - 1); nQueueIdx >= 0; nQueueIdx--) {

			// Get runtime item from queue
			PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nQueueIdx);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

			// Item found flag
			bool bItemFound = false;

			// Check if item ID is default schedule item
			const ScheduleItem& schDefaultItem = m_schScheduleData.GetDefaultItem();
			if (schDefaultItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

				// If item's snoozing mode is no longer available
				if (!schDefaultItem.IsAllowSnoozing()) {
					// Disable snooze mode
					pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
				}
				
				// Mark as found
				bItemFound = true;
				break;
			}

			// Search for item ID in Action Schedule extra item data
			for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
				const ScheduleItem& schItem = m_schScheduleData.GetItemAt(nItemIdx);
				if (schItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

					// If item's snoozing mode is no longer available
					if (!schItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
					}

					// Mark as found
					bItemFound = true;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (bItemFound == false) {
				// Remove item from runtime queue
				m_arrRuntimeQueue.erase(m_arrRuntimeQueue.begin() + nQueueIdx);
			}
		}
	}
	else if (nMode == Mode::Disable) {

		// Disable skip mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

			// Get runtime item from queue
			PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

			// Update item skip mode data
			pwrRuntimeItem.SetSkipFlag(FLAG_OFF);

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
		}
	}
}


/**
 * @brief	Get Action Schedule item skip flag status
 * @param	nItemID	- Action Schedule item ID
 * @return	true/false
 */
bool CPowerPlusDlg::GetActionScheduleSkipStatus(unsigned nItemID)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.empty())
		return false;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		const PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get skip enable flag
			if (pwrRuntimeItem.GetSkipFlag() == FLAG_ON)
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Get Action Schedule item snooze trigger status
 * @param	nItemID	   - Action Schedule item ID
 * @param	curSysTime - Current system time
 * @return	true/false
 */
bool CPowerPlusDlg::GetActionScheduleSnoozeStatus(unsigned nItemID, const ClockTime& currentTime)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.empty())
		return false;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		const PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_OFF)
				return false;

			// Check for next snooze time matching
			if (ClockTimeUtils::IsMatching(currentTime, pwrRuntimeItem.GetTime()))
				return true;

			return false;
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// HotkeySet feature functions


/**
 * @brief	Setup background hotkey if enabled
 * @param	nMode - Mode of setup (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::SetupBackgroundHotkey(int nMode)
{
	// Get option and flag values
	bool bHKSEnable = GetAppOption(AppOptionID::backgroundHotkeyEnabled);
	bool bHKRegisterFlag = GetFlagValue(AppFlagID::hotkeyRegistered);

	// If background hotkey feature is disabled and no hotkey registered, do nothing
	if ((bHKSEnable == false) &&											// HotkeySet option OFF
		(bHKRegisterFlag == false) &&										// Hotkey registered flag OFF
		(m_arrCurRegHKeyList.empty())) {									// Registered hotkey list empty
		TRACE("Background hotkey setup will be skipped!!!");
		return;
	}

	// Get window handle
	HWND hWnd = this->GetSafeHwnd();
	DWORD dwErrorCode;


/*********************************************************************/
	/*																	 */
	/*	         Unregister already registered HotkeySet items			 */
	/*	             Applied for modes: DISABLE or UPDATE                */
	/*																	 */


/*********************************************************************/
	if ((nMode == Mode::Disable) || (nMode == Mode::Update)) {

		if ((bHKRegisterFlag == true) &&									// Hotkey registered flag ON
			(!m_arrCurRegHKeyList.empty())) {								// Registered hotkey list is NOT empty

			// Unregister currently registered hotkeys
			int nRegItemNum = m_arrCurRegHKeyList.size();
			for (int nIndex = (nRegItemNum - 1); nIndex >= 0; nIndex--) {
				unsigned nHKID = m_arrCurRegHKeyList.at(nIndex);
				bool bRet = UnregisterHotKey(hWnd, nHKID);
				if (bRet == true) {
					// Unregister successfully
					OutputDebugLogFormat(_T("Unregistered hotkey: %d"), nHKID);
					m_arrCurRegHKeyList.erase(m_arrCurRegHKeyList.begin() + nIndex);
					if (nIndex == 0) {										// Last item unregistered
						SetFlagValue(AppFlagID::hotkeyRegistered, false);	// Reset hotkey registered flag
						m_arrCurRegHKeyList.clear();						// Cleanup registered hotkey list
					}
				}
				else {
					// Unregister failed
					dwErrorCode = GetLastError();

					// Output debug log
					OutputDebugLogFormat(_T("Unregister hotkey failed: %d"), nHKID);

					// Trace error
					TRACE_FORMAT("Error: Hotkey unregister failed!!! (Code=0x%X)", dwErrorCode);
					TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

					// Show error message
					ShowErrorMessage(dwErrorCode);
					DisplayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);
				}
			}
		}
	}


/*********************************************************************/
	/*																	 */
	/*				      Register HotkeySet items					     */
	/*				   Applied for modes: INIT or UPDATE		         */
	/*																	 */


/*********************************************************************/
	if ((nMode == Mode::Init) || (nMode == Mode::Update)) {

		// If feature not enabled, do nothing
		if (bHKSEnable == false) {
			TRACE("Background hotkey setup will be skipped!!!");
			return;
		}

		// If there's no item, do nothing
		int nItemNum = m_hksHotkeySetData.GetItemNum();
		if (nItemNum <= 0) {
			TRACE("There is not Hotkey item!!!");
			return;
		}

		// Reset flag and re-initialize registered hotkey list
		SetFlagValue(AppFlagID::hotkeyRegistered, false);
		m_arrCurRegHKeyList.clear();
		m_arrCurRegHKeyList.reserve(nItemNum);

		// Debug log
		String logTempString;

		// Register each HotkeySet item
		bool bRegistered = false;
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

			// Get hotkey item
			HotkeySetItem hksItem = m_hksHotkeySetData.GetItemAt(nIndex);

			// If item is empty, skip this item
			if (hksItem.IsEmpty()) 
				continue;

			// Get hotkey action ID
			unsigned nHKActionID = hksItem.GetActionID();

			// Get keycode
			DWORD dwModifiers, dwVirtualKey;
			hksItem.GetKeyCode(dwModifiers, dwVirtualKey);
			if ((dwModifiers == 0) || (dwVirtualKey == 0))
				continue;

			// No yeild keyboard auto-repeat
			dwModifiers |= MOD_NOREPEAT;

			// Debug log format
			logTempString.Format(_T("ActionID=%d, Modifiers=%d, VirtualKey=%d"), nHKActionID, dwModifiers, dwVirtualKey);

			// Get enable/disable status
			bool bEnabled = hksItem.IsEnabled();

			// Skip registering item if disabled
			if (bEnabled == false) {
				OutputDebugLogFormat(_T("Skip registering hotkey (disabled): %s"), logTempString.GetString());
				continue;
			}

			// Register hotkey item
			bool bRet = RegisterHotKey(hWnd, nHKActionID, dwModifiers, dwVirtualKey);

			// Trigger flag
			bRegistered |= bRet;
			SetFlagValue(AppFlagID::hotkeyRegistered, bRegistered);

			if (bRet == true) {
				// Register successfully
				OutputDebugLogFormat(_T("Registered hotkey: %s"), logTempString.GetString());
				m_arrCurRegHKeyList.push_back(nHKActionID);						// Update registered hotkey list
			}
			else {
				// Register failed
				dwErrorCode = GetLastError();

				// Output debug log
				OutputDebugLogFormat(_T("Register hotkey failed: %s"), logTempString.GetString());

				// Trace error
				TRACE_FORMAT("Error: Hotkey register failed!!! (Code=0x%X)", dwErrorCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				ShowErrorMessage(dwErrorCode);
				DisplayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);
			}
		}
	}
}


/**
 * @brief	Process when registered hotkey is pressed
 * @param	nHotkeyID - ID of pressed hotkey
 * @return	bool - Hotkey processing result
 */
bool CPowerPlusDlg::ProcessHotkey(int nHotkeyID)
{
	// If "Background hotkey" option is not enabled, do nothing
	if (GetAppOption(AppOptionID::backgroundHotkeyEnabled) == false) {
		TRACE("Background hotkey option is not enabled!!!");
		return false;
	}

	// Output debug log
	OutputDebugLogFormat(_T("Process Hotkey: HKeyID=%d"), nHotkeyID);

	// Get HotkeySet item by ID
	HotkeySetItem hksItem;
	for (int nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		HotkeySetItem& hksTemp = m_hksHotkeySetData.GetItemAt(nIndex);
		if (hksTemp.GetActionID() == static_cast<unsigned>(nHotkeyID)) {
			hksItem.Copy(hksTemp);
			break;
		}
	}

	// Check item validity
	if (hksItem.IsEmpty())
		return false;

	// Output hotkey event log
	String hotkeyDescription;
	hksItem.PrintKeyStrokes(hotkeyDescription);
	OutputEventLog(LOG_EVENT_EXEC_HOTKEY, hotkeyDescription);

	// Get Power action ID by HotkeyID
	unsigned nActionID = NULL;
	switch (nHotkeyID)
	{
	case HotkeyID::displayOff:
		nActionID = APP_ACTION_DISPLAYOFF;
		break;
	case HotkeyID::sleep:
		nActionID = APP_ACTION_SLEEP;
		break;
	case HotkeyID::shutdown:
		nActionID = APP_ACTION_SHUTDOWN;
		break;
	case HotkeyID::restart:
		nActionID = APP_ACTION_RESTART;
		break;
	case HotkeyID::signOut:
		nActionID = APP_ACTION_SIGNOUT;
		break;
	case HotkeyID::hibernate:
		nActionID = APP_ACTION_HIBERNATE;
		break;

	default:
		TRACE("Wrong argument!!!");
		break;
	}

	// If Power action ID is invalid, do nothing
	if (nActionID == NULL) {
		// Trace error
		TRACE_ERROR("Error: Invalid Power action ID!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Save history info data
	InitHotkeyHistoryInfo(nHotkeyID);
	SaveHistoryInfoData();
	
	// Execute hotkeyset action
	WPARAM wParam = static_cast<WPARAM>(nActionID);
	bool bResult = ExecuteAction(APP_MACRO_ACTION_HOTKEY, wParam);
	return bResult;
}


/**
 * @brief	Register/unregister to receive session state change notification
 * @param	nMode - Mode of setup (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::RegisterSessionNotification(int nMode)
{
	DWORD dwError;
	HWND hCurWnd = this->GetSafeHwnd();


/*********************************************************************/
	/*																	 */
	/*		  Unregister for session state change notifications			 */
	/*	            Applied for modes: DISABLE or UPDATE		         */
	/*																	 */


/*********************************************************************/
	if ((nMode == Mode::Disable) || (nMode == Mode::Update)) {

		// Only unregister if the flag is not OFF
		if (GetFlagValue(AppFlagID::wtsSessionNotifyRegistered)) {

			if (WTSUnRegisterSessionNotification(hCurWnd)) {
				// Mark flag as OFF
				SetFlagValue(AppFlagID::wtsSessionNotifyRegistered, false);
			}
			else {
				// Unregister failed
				dwError = GetLastError();

				// Trace error
				TRACE_FORMAT("Error: Unregister session notification failed!!! (Code=%d)", dwError);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				ShowErrorMessage(dwError);
			}
		}
	}


/*********************************************************************/
	/*																	 */
	/*			Register for session state change notifications			 */
	/*				   Applied for modes: INIT or UPDATE		         */
	/*																	 */


/*********************************************************************/
	if ((nMode == Mode::Init) || (nMode == Mode::Update)) {

		// Only register if the flag is not ON
		if (!GetFlagValue(AppFlagID::wtsSessionNotifyRegistered)) {

			if (WTSRegisterSessionNotification(hCurWnd, NOTIFY_FOR_THIS_SESSION)) {
				// Mark flag as ON
				SetFlagValue(AppFlagID::wtsSessionNotifyRegistered, true);
			}
			else {
				// Register failed
				dwError = GetLastError();

				// Trace error
				TRACE_FORMAT("Error: Register session notification failed!!! (Code=%d)", dwError);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				ShowErrorMessage(dwError);
			}
		}
	}
}


/**
 * @brief	Process when registered hotkey is pressed in lock state
					(this function will convert key-hooked param sent from the
					application class into corresponding registered hotkey ID)
 * @param	dwHKeyParam - Hotkey parameters (sent from app class)
 * @return	bool - Hotkey processing result
 * @note	If the hotkey ID is found and is registered, the hotkey will be processed and executed using the base ProcessHotkey function
 */
bool CPowerPlusDlg::ProcessLockStateHotkey(DWORD dwHKeyParam)
{
	// Only process if both options are enabled
	if ((GetAppOption(AppOptionID::backgroundHotkeyEnabled) != true) ||
		(GetAppOption(AppOptionID::lockStateHotkeyEnabled) != true)) {
		TRACE("[LockState Hotkey] Either required options are not enabled!!!");
		return false;
	}

	// Convert hotkey param back into keycode
	WORD wModifiers  = LOWORD(dwHKeyParam);
	WORD wVirtualKey = HIWORD(dwHKeyParam);

	// Look for corresponding HotkeyID in HotkeySet data
	unsigned nHKActionID = INT_NULL;
	for (size_t nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		const HotkeySetItem& hksItem = m_hksHotkeySetData.GetItemAt(nIndex);
		if ((hksItem.IsEnabled() == true) &&						// HotkeySet item is enabled
			(hksItem.CompareKeycode(wModifiers, wVirtualKey))) {	// Keycode is matching
			nHKActionID = hksItem.GetActionID();
			break;
		}
	}

	// Do not process if HotkeyID is NOT available
	if (nHKActionID == INT_NULL) {
		TRACE("[LockState Hotkey] Hotkey not found!!!");
		return false;
	}

	// Output debug log
	OutputDebugLogFormat(_T("[LockState Hotkey] HotkeyID found: HKeyID=0x%04X (%d)"), nHKActionID, nHKActionID);
		
	// Check if HotkeyID is registered
	if (GetFlagValue(AppFlagID::hotkeyRegistered) != true) {
		// Trace error
		TRACE_ERROR("[LockState Hotkey] No hotkey registered!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Only process if HotkeyID is registered
	for (size_t nIndex = 0; nIndex < m_arrCurRegHKeyList.size(); nIndex++) {
		unsigned nRegHKeyID = m_arrCurRegHKeyList.at(nIndex);
		if (nRegHKeyID == nHKActionID) {
			// Process Hotkey by ID
			return ProcessHotkey(nHKActionID);
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Power Reminder feature functions


/**
 * @brief	Excute Power Reminder action
 * @param	nExecEventID - Execute event ID
 * @return	true/false
 */
bool CPowerPlusDlg::ExecutePowerReminder(unsigned nExecEventID)
{
	// If "Power Reminder" option is not enabled, do nothing
	if (GetAppOption(AppOptionID::pwrReminderEnabled) == false) {
		TRACE("Power Reminder option is not enabled!!!");
		return false;
	}

	// If there's no item, do nothing
	int nItemNum = m_prdReminderData.GetItemNum();
	if (nItemNum <= 0) {
		TRACE("There is no Power Reminder items!!!");
		return false;
	}

	// Get current time
	DateTime currentDateTime;
	ClockTime currentClockTime;
	if (nExecEventID == PwrReminderEvent::atSetTime) {
		currentDateTime = DateTimeUtils::GetCurrentDateTime();
		currentClockTime = currentDateTime.GetClockTime();
	}

	// Flag that trigger to reupdate Power Reminder data
	bool bTriggerReupdate = false;

	// Search all items and process reminder
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		PwrReminderItem& pwrCurItem = m_prdReminderData.GetItemAt(nIndex);

		// If item is empty, skip this item
		if (pwrCurItem.IsEmpty())
			continue;

		// If event ID is not matching, skip this item
		if (pwrCurItem.GetEventID() != nExecEventID)
			continue;

		// If item is not enabled, skip this item
		if (pwrCurItem.IsEnabled() == false)
			continue;

		// Process item
		PwrReminderItem pwrDispItem;
		switch (nExecEventID)
		{
		case PwrReminderEvent::atSetTime:
			// If item is set to repeat but not set active in current day of week
			if ((pwrCurItem.IsRepeatEnabled() == true) && (!pwrCurItem.IsDayActive((DayOfWeek)currentDateTime.DayOfWeek())))
				continue;

			// If set time matching or snooze time is triggered
			if ((ClockTimeUtils::IsMatching(currentClockTime, pwrCurItem.GetTime())) ||
				(GetPwrReminderSnoozeStatus(pwrCurItem.GetItemID(), currentClockTime))) {
				// Prepare to display
				pwrDispItem.Copy(pwrCurItem);
				SetPwrReminderSnooze(pwrCurItem, FLAG_OFF);
			}
			else continue;
			break;

		case PwrReminderEvent::atSysWakeUp:
			// If System suspend flag and Session ending flag are both OFF, do not display
			if ((GetSystemSuspendFlag() == FLAG_OFF) && (GetSessionEndFlag() == FLAG_OFF)) continue;
			// Otherwise, just prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;

		case PwrReminderEvent::wakeAfterAction:
			// If Power Action flag is OFF, do not display
			if (GetPwrActionFlag() == FLAG_OFF) continue;
			// Otherwise, prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;

		case PwrReminderEvent::atAppStartup:
		case PwrReminderEvent::beforePwrAction:
		case PwrReminderEvent::atAppExit:
			// Just prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;

		default:
			continue;
		}

		// Display reminder
		if (!pwrDispItem.IsEmpty() && !GetPwrReminderDispFlag(pwrDispItem)) {

			// Display reminder item
			DisplayPwrReminder(pwrDispItem);

			// If repeat daily option is not enabled
			// --> Disable reminder item after displaying
			if (pwrDispItem.IsRepeatEnabled() == false) {
				bTriggerReupdate |= true;
				pwrCurItem.EnableItem(false);
			}
		}
	}

	// Reupdate flag is triggered
	if (bTriggerReupdate == true) {
		// Reupdate Power Reminder data
		ReupdatePwrReminderData();
	}

	return true;
}


/**
 * @brief	Display Power Reminder item
 * @param	pwrDispItem - Item to display
 * @return	int
 */
int CPowerPlusDlg::DisplayPwrReminder(const PwrReminderItem& pwrDispItem)
{
	// Check message content validity
	String messageContent = pwrDispItem.GetMessage();
	if ((messageContent.IsEmpty()) || (IS_NULL_STRING(messageContent))) {
		// Invalid message content
		TRACE("Invalid message content!!!");
		return INT_INVALID;
	}

	// Output debug log
	OutputDebugLogFormat(_T("Display reminder: ItemID=%d"), pwrDispItem.GetItemID());

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	int nRetFlag = FLAG_OFF;
	int nRespond = INT_NULL;

	// Update item runtime displaying flag
	SetPwrReminderDispFlag(pwrDispItem, FLAG_ON);

	// Output event log: Display Power Reminder
	OutputPwrReminderEventLog(LOG_EVENT_DISP_PWRREMINDER, pwrDispItem);

	// Save history info data
	InitPwrReminderHistoryInfo(pwrDispItem);
	SaveHistoryInfoData();

	// Style: MessageBox
	if (pwrDispItem.GetMessageStyle() == PwrReminderStyle::messageBox) {

		const wchar_t* messageCaption = GetLanguageString(pAppLang, IDD_PWRREMINDER_DLG);
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		nRespond = DisplayMessageBox(messageContent, messageCaption, dwMsgStyle);
	}
	// Style: Dialog
	else if (pwrDispItem.GetMessageStyle() == PwrReminderStyle::dialogBox) {

		// Init reminder message dialog
		CReminderMsgDlg* pMsgDlg = new CReminderMsgDlg(this);
		if (pMsgDlg == NULL) {

			// Trace error
			TRACE_ERROR("Error: Reminder message dialog initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Update item runtime displaying flag
			SetPwrReminderDispFlag(pwrDispItem, FLAG_OFF);
			return INT_INVALID;
		}

		// Message style
		RmdMsgStyleSet rmdMessageStyle = m_prdReminderData.GetCommonStyle();
		if (pwrDispItem.IsCustomStyleEnabled())
			rmdMessageStyle = pwrDispItem.GetMessageStyleData();

		// Message auto-close interval
		int nTimeout = rmdMessageStyle.GetTimeout();

		// Allow snooze mode
		bool bAllowSnooze = pwrDispItem.IsAllowSnoozing();

		// Set allow snooze mode
		pMsgDlg->SetAllowSnoozeMode(bAllowSnooze);

		// Set properties
		pMsgDlg->SetCaptionFromLanguage(IDD_PWRREMINDER_DLG);
		pMsgDlg->SetDispMessage(messageContent);
		pMsgDlg->SetMessageStyle(rmdMessageStyle);
		pMsgDlg->SetAutoCloseInterval(nTimeout);

		// Set notify state flags
		pMsgDlg->SetTopMost(true);
		pMsgDlg->SetInitSound(true);

		// Display message
		nRespond = pMsgDlg->DoModal();

		// Get returned flag
		pMsgDlg->GetSnoozeTriggerFlag(nRetFlag);

		delete pMsgDlg;
	}

	// Update item runtime data flag
	SetPwrReminderSnooze(pwrDispItem, nRetFlag);
	SetPwrReminderDispFlag(pwrDispItem, FLAG_OFF);

	return nRespond;
}


/**
 * @brief	Reupdate Power Reminder data
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::ReupdatePwrReminderData(void)
{
	// Disable Power Reminder items
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		// Update Power Reminder data
		pApp->SetAppPwrReminderData(&m_prdReminderData);
		pApp->SaveRegistryAppData(APPDATA_PWRREMINDER);
	}

	// Trigger reupdate Power Reminder data
	PostMessage(SM_APP_UPDATE_PWRREMINDERDATA, NULL, NULL);
}


/**
 * @brief	Setup Power Reminder item snooze mode
 * @param	pwrItem		- Power Reminder item
 * @param	nSnoozeFlag - Snooze flag
 * @return	None
 */
void CPowerPlusDlg::SetPwrReminderSnooze(const PwrReminderItem& pwrItem, int nSnoozeFlag)
{
	// If item is empty, do nothing
	if (pwrItem.IsEmpty()) return;

	// If item event ID is not "At set time", do nothing
	if (pwrItem.GetEventID() != PwrReminderEvent::atSetTime)
		return;

	// If snoozing option is not available
	if (pwrItem.IsAllowSnoozing() != true) {
		// Turn off flag
		nSnoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int nInterval = pwrItem.GetSnoozeInterval();

	// Find if item snooze mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == pwrItem.GetItemID()) {

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);

			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.SetTime(ClockTimeUtils::GetCurrentClockTime());
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(PwrFeatureID::pwrReminder);
	pwrRuntimeItem.SetItemID(pwrItem.GetItemID());
	pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);
	if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.SetTime(ClockTimeUtils::GetCurrentClockTime());
		pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
	}

	// Add item to runtime queue
	m_arrRuntimeQueue.push_back(pwrRuntimeItem);
}


/**
 * @brief	Update Power Reminder snooze queue data
 * @param	nMode - Update mode flag (UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::UpdatePwrReminderSnooze(int nMode)
{
	if (nMode == Mode::Init) {

		// Initialize Power Reminder runtime snooze queue
		// Set all items as not snoozed
		for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
			SetPwrReminderSnooze(m_prdReminderData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == Mode::Update) {

		// Update snooze queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.size() - 1); nQueueIdx >= 0; nQueueIdx--) {
			
			// Get runtime item from queue
			PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nQueueIdx);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

			// Search for item ID in Power Reminder data
			bool bItemFound = false;
			for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
				const PwrReminderItem& pwrItem = m_prdReminderData.GetItemAt(nItemIdx);
				if (pwrItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

					// If item's snoozing mode is no longer available
					if (!pwrItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
					}

					// Mark as found
					bItemFound = true;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (bItemFound == false) {
				// Remove item from snooze queue
				m_arrRuntimeQueue.erase(m_arrRuntimeQueue.begin() + nQueueIdx);
			}
		}
	}
	else if (nMode == Mode::Disable) {

		// Disable snooze mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

			// Get runtime item from queue
			PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
		}
	}
}


/**
 * @brief	Get Power Reminder item snooze trigger status
 * @param	nItemID	   - Power Reminder item ID
 * @param	curSysTime - Current system time
 * @return	true/false
 */
bool CPowerPlusDlg::GetPwrReminderSnoozeStatus(unsigned nItemID, const ClockTime& currentTime)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.empty())
		return false;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		const PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_OFF)
				return false;
			
			// Check for next snooze time matching
			if (ClockTimeUtils::IsMatching(currentTime, pwrRuntimeItem.GetTime()))
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Get Power Reminder item runtime displaying flag
 * @param	pwrItem	- Power Reminder item
 * @return	true/false
 */
bool CPowerPlusDlg::GetPwrReminderDispFlag(const PwrReminderItem& pwrItem)
{
	// If item is empty, it can not be displayed
	if (pwrItem.IsEmpty()) return false;

	// Find if item runtime data is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == pwrItem.GetItemID()) {

			// Get snooze enable flag
			if (pwrRuntimeItem.GetDisplayFlag() == FLAG_ON)
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Set Power Reminder item runtime displaying flag
 * @param	pwrItem	  - Power Reminder item
 * @param	nDispFlag - Display flag
 * @return	None
 */
void CPowerPlusDlg::SetPwrReminderDispFlag(const PwrReminderItem& pwrItem, int nDispFlag)
{
	// If item is empty, do nothing
	if (pwrItem.IsEmpty()) return;

	// Find if item runtime data is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == pwrItem.GetItemID()) {
			// Update item displaying flag
			pwrRuntimeItem.SetDisplayFlag(nDispFlag);
			return;
		}
	}

	// Prepare item info to add
	PwrRuntimeItem pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(PwrFeatureID::pwrReminder);
	pwrRuntimeItem.SetItemID(pwrItem.GetItemID());
	pwrRuntimeItem.SetDisplayFlag(nDispFlag);

	// Add item to runtime queue
	m_arrRuntimeQueue.push_back(pwrRuntimeItem);
}


/**
 * @brief	Get Power Reminder runtime displaying item list
 * @param	arrPwrDispList - Power Reminder displaying item list
 * @return	size_t - Number of displaying items
 */
size_t CPowerPlusDlg::GetPwrReminderDispList(UIntArray& arrPwrDispList)
{
	// Reset output data list
	arrPwrDispList.clear();

	// If runtime queue data is empty, return no item
	if (m_arrRuntimeQueue.empty())
		return INT_NULL;

	// Get runtime displaying item ID list
	arrPwrDispList.reserve(m_arrRuntimeQueue.size());
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.size(); nIndex++) {

		// Get runtime item from queue
		const PwrRuntimeItem& pwrRuntimeItem = m_arrRuntimeQueue.at(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != PwrFeatureID::pwrReminder) continue;

		// If item displaying flag is marked as ON
		if (pwrRuntimeItem.GetDisplayFlag() == FLAG_ON) {
			// Add item ID into output data list
			arrPwrDispList.push_back(pwrRuntimeItem.GetItemID());
		}
	}

	// Return number of displaying items
	arrPwrDispList.shrink_to_fit();
	return arrPwrDispList.size();
}


//////////////////////////////////////////////////////////////////////////
// History and logging functions


/**
 * @brief	Output action schedule event log
 * @param	usEvent	- Event ID
 * @param	schItem - Schedule item
 * @return	None
 */
void CPowerPlusDlg::OutputScheduleEventLog(USHORT usEvent, const ScheduleItem& schItem)
{
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Schedule action name
	int nActionNameID = GetPairedID(IDTable::ActionName, schItem.GetAction());
	const wchar_t* actionNameString = GetLanguageString(pAppLang, nActionNameID);

	// Detail info
	LOGDETAILINFO logDetailInfo;
	logDetailInfo.AddDetail(EventDetail::ContentID, schItem.GetItemID());

	// Output event log
	OutputEventLog(usEvent, actionNameString, &logDetailInfo);
}


/**
 * @brief	Output Power Reminder event log
 * @param	usEvent	- Event ID
 * @param	pwrItem - Power Reminder item
 * @return	None
 */
void CPowerPlusDlg::OutputPwrReminderEventLog(USHORT usEvent, const PwrReminderItem& pwrItem)
{
	// Message content
	const wchar_t* messageContent = pwrItem.GetMessage();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	logDetailInfo.AddDetail(EventDetail::ContentID, pwrItem.GetItemID());

	// Output event log
	OutputEventLog(usEvent, messageContent, &logDetailInfo);
}


/**
 * @brief	Initialize Power Action history info data
 * @param	nActionID	- History action ID
 * @param	bResult		- Result of execution
 * @param	dwErrorCode	- Error code
 * @return	None
 */
void CPowerPlusDlg::InitPwrActionHistoryInfo(unsigned nActionID, bool bResult, DWORD dwErrorCode)
{
	// Initialize action history info data to save logs
	m_hidHistoryInfoData.Init(PowerAction);
	m_hidHistoryInfoData.SetActionID(nActionID);

	// Collect action result
	m_hidHistoryInfoData.SetResult(bResult);
	m_hidHistoryInfoData.SetErrorCode(dwErrorCode);
}


/**
 * @brief	Initialize schedule history info data
 * @param	schItem - Schedule item
 * @return	None
 */
void CPowerPlusDlg::InitScheduleHistoryInfo(const ScheduleItem& schItem)
{
	// Check item validity
	if (schItem.IsEmpty())
		return;

	// Get schedule action name ID
	unsigned nActionID = NULL;
	switch (schItem.GetAction())
	{
	case APP_ACTION_NOTHING:
		nActionID = HistoryAction::DoNothing;
		break;
	case APP_ACTION_DISPLAYOFF:
		nActionID = HistoryAction::DisplayOff;
		break;
	case APP_ACTION_SLEEP:
		nActionID = HistoryAction::SleepMode;
		break;
	case APP_ACTION_SHUTDOWN:
		nActionID = HistoryAction::Shutdown;
		break;
	case APP_ACTION_RESTART:
		nActionID = HistoryAction::Restart;
		break;
	case APP_ACTION_SIGNOUT:
		nActionID = HistoryAction::SignOut;
		break;
	case APP_ACTION_HIBERNATE:
		nActionID = HistoryAction::Hibernate;
		break;
	}

	// Initialize schedule history info data
	m_hidHistoryInfoData.Init(ScheduleAction);
	m_hidHistoryInfoData.SetItemID(schItem.GetItemID());
	m_hidHistoryInfoData.SetActionID(nActionID);
}


/**
 * @brief	Initialize hotkeyset history info data
 * @param	nHKID - Hotkey action ID
 * @return	None
 */
void CPowerPlusDlg::InitHotkeyHistoryInfo(unsigned nHKID)
{
	// Get HotkeySet item by ID
	HotkeySetItem hksItem;
	for (int nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		HotkeySetItem& hksTemp = m_hksHotkeySetData.GetItemAt(nIndex);
		if (hksTemp.GetActionID() == nHKID) {
			hksItem.Copy(hksTemp);
			break;
		}
	}

	// Check item validity
	if (hksItem.IsEmpty())
		return;

	// Get hotkey action name ID
	unsigned nActionID = NULL;
	switch (hksItem.GetActionID())
	{
	case HotkeyID::displayOff:
		nActionID = HistoryAction::DisplayOff;
		break;
	case HotkeyID::sleep:
		nActionID = HistoryAction::SleepMode;
		break;
	case HotkeyID::shutdown:
		nActionID = HistoryAction::Shutdown;
		break;
	case HotkeyID::restart:
		nActionID = HistoryAction::Restart;
		break;
	case HotkeyID::signOut:
		nActionID = HistoryAction::SignOut;
		break;
	case HotkeyID::hibernate:
		nActionID = HistoryAction::Hibernate;
		break;
	default:
		break;
	}

	// Initialize hotkey action history info
	m_hidHistoryInfoData.Init(HotkeySet);
	m_hidHistoryInfoData.SetActionID(nActionID);
	String keyStrokesString;
	hksItem.PrintKeyStrokes(keyStrokesString);
	m_hidHistoryInfoData.SetDescription(keyStrokesString);
}


/**
 * @brief	Initialize reminder displaying history info data
 * @param	pwrItem - Power Reminder item
 * @return	None
 */
void CPowerPlusDlg::InitPwrReminderHistoryInfo(const PwrReminderItem& pwrItem)
{
	// Check item validity
	if (pwrItem.IsEmpty()) return;

	// Initialize history info data
	m_hidHistoryInfoData.Init(PowerReminder);
	m_hidHistoryInfoData.SetItemID(pwrItem.GetItemID());
	m_hidHistoryInfoData.SetDescription(pwrItem.GetMessage());
}


/**
 * @brief	Save history info data log file
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::SaveHistoryInfoData(void)
{
	// If history info data is empty (not yet initialized), do nothing
	if (!m_hidHistoryInfoData.IsInit())
		return;

	// Prepare common history log info
	LOGITEM actionLogItem;
	actionLogItem.SetTime(m_hidHistoryInfoData.GetTime());

	// Get current process ID
	actionLogItem.SetProcessID();

	// Attach history detail info by category ID
	switch (m_hidHistoryInfoData.GetCategoryID())
	{
	case HistoryCategory::PowerAction:
		actionLogItem.SetCategory(LOG_HISTORY_EXEC_PWRACTION);
		actionLogItem.AddDetail(HistoryDetail::Action, m_hidHistoryInfoData.GetActionID(), LogDetailFlag::LookUp_Dict);
		break;

	case HistoryCategory::ScheduleAction:
		actionLogItem.SetCategory(LOG_HISTORY_EXEC_SCHEDULE);
		actionLogItem.AddDetail(HistoryDetail::ItemID, m_hidHistoryInfoData.GetItemID());
		actionLogItem.AddDetail(HistoryDetail::Action, m_hidHistoryInfoData.GetActionID(), LogDetailFlag::LookUp_Dict);
		break;

	case HistoryCategory::HotkeySet:
		actionLogItem.SetCategory(LOG_HISTORY_EXEC_HOTKEY);
		actionLogItem.AddDetail(HistoryDetail::Action, m_hidHistoryInfoData.GetActionID(), LogDetailFlag::LookUp_Dict);
		actionLogItem.AddDetail(HistoryDetail::Keystrokes, m_hidHistoryInfoData.GetDescription());
		break;

	case HistoryCategory::PowerReminder:
		actionLogItem.SetCategory(LOG_HISTORY_DISP_PWRREMINDER);
		actionLogItem.AddDetail(HistoryDetail::ItemID, m_hidHistoryInfoData.GetItemID());
		actionLogItem.AddDetail(HistoryDetail::Message, m_hidHistoryInfoData.GetDescription());
		break;

	default:
		break;
	}

	// Attach history action result detail info
	if (m_hidHistoryInfoData.IsSuccess() || m_hidHistoryInfoData.GetErrorCode() == APP_ERROR_SUCCESS) {
		actionLogItem.AddDetail(HistoryDetail::Result, HistoryResult::SuccessNoError, LogDetailFlag::LookUp_Dict);
	}
	else {
		if (m_hidHistoryInfoData.GetErrorCode() == APP_ERROR_UNKNOWN) {

			// If error code is NULL or unknown, set as failed with unknown reason
			actionLogItem.AddDetail(HistoryDetail::Result, HistoryResult::FailedUnknown, LogDetailFlag::LookUp_Dict);
		}
		else {
			// If error code is available, set as failed with error code
			actionLogItem.AddDetail(HistoryDetail::Result, HistoryResult::FailedWithErrorCode, LogDetailFlag::LookUp_Dict);

			// Attach error code detail info
			actionLogItem.AddDetail(HistoryDetail::ActionError, m_hidHistoryInfoData.GetErrorCode());
		}
	}

	// Output action history log if enabled
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		pApp->OutputAppHistoryLog(actionLogItem);
	}

	// Empty history data after done output
	m_hidHistoryInfoData.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
// Notification and error message functions


/**
 * @brief	Show confirmation message before executing action
 * @param	nActionType - Type of action
 * @param	nActionID	- ID of action
 * @return	int	- Result of confirmation message
 */
int CPowerPlusDlg::ConfirmActionExec(unsigned nActionType, unsigned nActionID)
{
	// If "Confirm action" option is not enabled, return YES
	if (GetAppOption(AppOptionID::confirmBeforeExecuting) == false) {
		TRACE("Confirm action option is not enabled, always allow execution!!!");
		return IDYES;
	}

	// Allowed action IDs --> Bypass confirmation
	if (nActionID == APP_ACTION_DISPLAYOFF) {
		TRACE("Always allow executing monitor power action!!!");
		return IDYES;
	}

	// Allowed action types --> Bypass confirmation
	if ((nActionType == APP_MACRO_ACTION_SCHEDULE) ||
		(nActionType == APP_MACRO_ACTION_HOTKEY) ||
		(nActionType == APP_MACRO_ACTION_MENU)) {
		TRACE("Bypassing confirmation for these power actions!!!");
		return IDYES;
	}

	// Display confirmation message
	unsigned nMsgStringID = GetPairedID(IDTable::ActionMessage, nActionID);
	int nResult = DisplayMessageBox(nMsgStringID, (unsigned)NULL, MB_YESNO | MB_ICONQUESTION);

	return nResult;
}


/**
 * @brief	Notify schedule if enabled
 * @param	pschItem  - Schedule item to notify (pointer)
 * @param	bReupdate - Trigger reupdate flag (out)
 * @return	int - Result of notify message
 */
int CPowerPlusDlg::NotifySchedule(PScheduleItem pschItem, bool& bReupdate)
{
	// Do not notify if schedule action is "Do nothing"
	if (pschItem->GetAction() == APP_ACTION_NOTHING) {
		TRACE("Do not notify when schedule action is DO NOTHING");
		return INT_INVALID;
	}

	// Get action info
	unsigned nActionStringID = GetPairedID(IDTable::ScheduleNotifyMessage, pschItem->GetAction());

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Format message
	const wchar_t* messageCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CAPTION);
	const wchar_t* messageScheduleAction = GetLanguageString(pAppLang, nActionStringID);
	const wchar_t* messageTemplate = GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_NOTIFY);
	
	String messageContent = StringUtils::StringFormat(messageTemplate, messageScheduleAction);

	// Allow cancelling schedule when notify
	bool bAllowCancel = GetAppOption(AppOptionID::allowCancelingSchedule);
	if (bAllowCancel == true)
	{
		// Update message content
		messageContent += GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_ALLOWCANCEL);
		int nRespond = DisplayMessageBox(messageContent, messageCaption, MB_OKCANCEL | MB_ICONINFORMATION);
		if (nRespond == IDCANCEL)
		{
			// Set item skip flag
			SetActionScheduleSkip(*pschItem, FLAG_ON);

			// If "Repeat" option is not ON,
			// --> Disable schedule item after canceling
			if (pschItem->IsRepeatEnabled() == false) {
				// Deactivate schedule
				pschItem->EnableItem(false);
				bReupdate = true;
			}

			// Output event log: Schedule canceled
			OutputScheduleEventLog(LOG_EVENT_CANCEL_SCHEDULE, *pschItem);

			// Display notify message
			DisplayMessageBox(MSGBOX_PROCESSSCHEDULE_CANCELED, MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONINFORMATION);
		}

		return nRespond;
	}

	// Show message
	return DisplayMessageBox(messageContent, messageCaption, MB_OK | MB_ICONINFORMATION);
}


/**
 * @brief	Show error message if enabled
 * @param	dwError - Error code
 * @return	None
 */
void CPowerPlusDlg::ShowErrorMessage(DWORD dwError)
{
	// If option is not enabled, do nothing
	if (GetAppOption(AppOptionID::showErrorMessage) == false) {
		TRACE("Show error message option is not enabled!!!");
		return;
	}

	// Get window handle and language ID
	HWND hWnd = this->GetSafeHwnd();
	unsigned nCurLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguageOption();

	// Show error message
	AppCore::ShowErrorMessage(hWnd, nCurLang, dwError);
}


/**
 * @brief	Request to restart the application
	Arguments:		uiCmdSenderID	- ID of command which sends request
					bRestartAsAdmin - Restart as admin???
 * @return	None
 */
void CPowerPlusDlg::RequestRestartApp(unsigned uiCmdSenderID, bool bRestartAsAdmin)
{
	// Init request data
	RESTARTREQ reqRestart;
	ZeroMemory(&reqRestart, sizeof(RESTARTREQ));

	// Request from [Apply] button
	if (uiCmdSenderID == IDC_APPLY_BTN) {
		bool bRestartTrigger = GetFlagValue(AppFlagID::restartAsAdmin);
		reqRestart.bRequest = bRestartTrigger;
		reqRestart.bAdminCheck = true;
		reqRestart.bNotAdminShowMsg = true;
		reqRestart.bIsAdminDoNothing = true;
		reqRestart.bShowMsgWhenDeny = true;
		reqRestart.bResetFlag = true;
	}
	// Request from [Restart with admin priviledges] menu
	else if (uiCmdSenderID == IDM_NOTIFY_RESTART_ASADMIN) {
		reqRestart.bRequest = true;
		reqRestart.bAdminCheck = true;
		reqRestart.bNotAdminShowMsg = false;
		reqRestart.bIsAdminDoNothing = true;
		reqRestart.bShowMsgWhenDeny = false;
		reqRestart.bResetFlag = false;
	}
	// Request from [Restart app] menu or debug command
	else if ((uiCmdSenderID == IDM_NOTIFY_RESTART_APP) ||
			 (uiCmdSenderID == IDD_DEBUGTEST_DLG)) {
		// Restart directly without request
		RestartApp(bRestartAsAdmin);
		return;
	}
	else {
		// Invalid command --> Do not request
		return;
	}

	// Send restart request
	if (bRestartAsAdmin) {
		// Request restart as admin
		RequestRestartAsAdmin(reqRestart);
	}
	else {
		// Restart directly
		RestartApp(false);
	}
}


/**
 * @brief	Request to restart the application as admin
	Arguments:		reqRestart - Request to restart
 * @return	None
 */
void CPowerPlusDlg::RequestRestartAsAdmin(RESTARTREQ reqRestart)
{
	// If there's no request, do nothing
	if (reqRestart.bRequest == false) {
		TRACE("There is no restart request!!!");
		return;
	}

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	const wchar_t* requestMessage = GetLanguageString(pAppLang, MSGBOX_OTHER_REQUEST_RESTARTASADMIN);
	const wchar_t* messageCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowCaption();
	String messageFormatString = requestMessage;

	// Check if the application is currently running as admin
	bool bIsAdmin = false;
	if (reqRestart.bAdminCheck == true) {
		bIsAdmin = IsUserAnAdmin();
		if (bIsAdmin == true) {
			// If already running as admin, do nothing
			if (reqRestart.bIsAdminDoNothing == true) {
				if (reqRestart.bShowMsgWhenDeny == false) {
					DisplayMessageBox(MSGBOX_OTHER_ALREADY_ADMIN, (unsigned)NULL, MB_OK | MB_ICONINFORMATION);
				}
				return;
			}
		}
		else if (bIsAdmin == false) {
			// Show "not admin" message
			if (reqRestart.bNotAdminShowMsg == true) {
				const wchar_t* notAdminMsg = GetLanguageString(pAppLang, MSGBOX_OTHER_NOTRUNASADMIN);
				messageFormatString.Format(_T("%s\n%s"), notAdminMsg, requestMessage);
			}
		}
	}
	
	// Display request message
	int nRet = DisplayMessageBox(messageFormatString, messageCaption, MB_YESNO | MB_ICONQUESTION);
	if (nRet == IDYES) {
		// Restart as admin
		RestartApp(true);
	}
	else if (nRet == IDNO) {
		// Run as admin next time
		if (reqRestart.bShowMsgWhenDeny == true) {
			// Show message when denied
			DisplayMessageBox(MSGBOX_OTHER_RUNASADMIN_NEXTTIME, (unsigned)NULL, MB_OK | MB_ICONINFORMATION);
		}

		if (reqRestart.bResetFlag == true) {
			// Reset flag
			SetFlagValue(AppFlagID::restartAsAdmin, false);
		}
	}
}

