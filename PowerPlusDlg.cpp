
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlusDlg.cpp
//		Description:	Source file for main window
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"

#include "AboutDlg.h"
#include "MultiScheduleDlg.h"
#include "LogViewerDlg.h"
#include "HotkeySetDlg.h"
#include "PwrReminderDlg.h"
#include "ReminderMsgDlg.h"
#include "DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;

//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CPowerPlusDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPowerPlusDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CPowerPlusDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CPowerPlusDlg::CPowerPlusDlg(CWnd* pParent /*=NULL*/)
	: SDialog(IDD_POWERPLUS_DIALOG, pParent)
{
	// Initialize member variables
	m_hDefaultIcon = NULL;

	// Init app data variables
	ZeroMemory(&m_cfgAppConfig, sizeof(CONFIGDATA));
	ZeroMemory(&m_cfgTempConfig, sizeof(CONFIGDATA));
	ZeroMemory(&m_schScheduleData, sizeof(SCHEDULEDATA));
	ZeroMemory(&m_hksHotkeySetData, sizeof(HOTKEYSETDATA));
	ZeroMemory(&m_prdReminderData, sizeof(PWRREMINDERDATA));
	ZeroMemory(&m_actActionData, sizeof(ACTIONDATA));

	// Initialize boolean variables for checkboxes
	m_bShowDlgAtStartup = FALSE;
	m_bStartupEnabled = FALSE;
	m_bConfirmAction = FALSE;
	m_bSaveActionLog = FALSE;
	m_bSaveAppEventLog = FALSE;
	m_bRunAsAdmin = FALSE;
	m_bShowErrorMsg = FALSE;
	m_bRMBShowMenu = FALSE;
	m_bNotifySchedule = FALSE;
	m_bAllowCancelSchedule = FALSE;
	m_bEnableBackgroundHotkey = FALSE;
	m_bEnablePowerReminder = FALSE;

	// Initialize notify icon variables
	m_hNotifyIcon = NULL;
	m_pNotifyIconData = NULL;
	m_bNotifyIconShowed = FALSE;

	// Init member variables for resizing dialog
	m_bDlgExpanded = TRUE;
	m_pDialogSize = NULL;

	// Init hotkey register data
	m_bHotkeyRegistered = FALSE;
	m_arrCurRegHKeyList.RemoveAll();

	// Init Power Reminder advanced data
	m_arrRmdAdvList.RemoveAll();

	// Init other flags
	m_bRestartAsAdminFlag = FALSE;
	m_nPwrBroadcastSkipCount = 0;

	// Init child dialogs
	m_pAboutDlg = NULL;
	m_pHelpDlg = NULL;
	m_pLogViewerDlg = NULL;
	m_pMultiScheduleDlg = NULL;
	m_pHotkeySetDlg = NULL;
	m_pPwrReminderDlg = NULL;

	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CPowerPlusDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

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

	// Clear registered hotkey list
	m_arrCurRegHKeyList.RemoveAll();
	m_arrCurRegHKeyList.FreeExtra();

	// Clear Power Reminder data
	m_prdReminderData.DeleteAll();

	// Clean Power Reminder advanced data
	m_arrRmdAdvList.RemoveAll();
	m_arrRmdAdvList.FreeExtra();

	// Kill timers
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

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
	DDX_Check(pDX,	IDC_SAVEACTIONLOG_CHK,		m_bSaveActionLog);
	DDX_Check(pDX,	IDC_SAVEAPPEVENTLOG_CHK,	m_bSaveAppEventLog);
	DDX_Check(pDX,	IDC_RUNASADMIN_CHK,			m_bRunAsAdmin);
	DDX_Check(pDX,	IDC_SHOWERROR_CHK,			m_bShowErrorMsg);
	DDX_Check(pDX,	IDC_SCHEDNOTIFY_CHK,		m_bNotifySchedule);
	DDX_Check(pDX,	IDC_SCHEDALLOWCANCEL_CHK,	m_bAllowCancelSchedule);
	DDX_Check(pDX,	IDC_ENBBKGRDHOTKEYS_CHK,	m_bEnableBackgroundHotkey);
	DDX_Check(pDX,	IDC_ENBPWRREMINDER_CHK,		m_bEnablePowerReminder);
}

//////////////////////////////////////////////////////////////////////////
//
//	CPowerPlusDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_ID_MAPPING(CPowerPlusDlg)
	// Dialog controls
	IDMAP_ADD(IDD_POWERPLUS_DIALOG,				"AppMainDlg")
	IDMAP_ADD(IDC_MAIN_TITLE,					"MainSettingTitle")
	IDMAP_ADD(IDC_LMBACTION_LIST,				"LMBActionList")
	IDMAP_ADD(IDC_MMBACTION_LIST,				"MMBActionList")
	IDMAP_ADD(IDC_RMBACTION_LIST,				"RMBActionList")
	IDMAP_ADD(IDC_ENABLERMBMENU_CHK,			"EnableRMBCheck")
	IDMAP_ADD(IDC_APPLY_BTN,					"ApplyButton")
	IDMAP_ADD(IDC_RELOAD_BTN,					"ReloadButton")
	IDMAP_ADD(IDC_EXIT_BTN,						"ExitButton")
	IDMAP_ADD(IDC_EXPAND_BTN,					"ExpandButton")
	IDMAP_ADD(IDC_COLLAPSE_BTN,					"CollapseButton")
	IDMAP_ADD(IDC_SHOWATSTARTUP_CHK,			"ShowDlgAtStartupCheck")
	IDMAP_ADD(IDC_ENABLEAUTOSTART_CHK,			"RunAtStartupCheck")
	IDMAP_ADD(IDC_CONFIRMACTION_CHK,			"ConfirmActionCheck")
	IDMAP_ADD(IDC_SAVEACTIONLOG_CHK,			"SaveActionLogCheck")
	IDMAP_ADD(IDC_SAVEAPPEVENTLOG_CHK,			"SaveAppEventLogCheck")
	IDMAP_ADD(IDC_RUNASADMIN_CHK,				"RunAsAdminCheck")
	IDMAP_ADD(IDC_SHOWERROR_CHK,				"ShowErrMsgCheck")
	IDMAP_ADD(IDC_SCHEDNOTIFY_CHK,				"ShowSchedNotifyCheck")
	IDMAP_ADD(IDC_SCHEDALLOWCANCEL_CHK,			"AllowCancelSchedCheck")
	IDMAP_ADD(IDC_ENBBKGRDHOTKEYS_CHK,			"EnableBkgrdHotkeysCheck")
	IDMAP_ADD(IDC_ENBPWRREMINDER_CHK,			"EnablePowerReminderCheck")
	IDMAP_ADD(IDC_LANGUAGE_LIST,				"AppLanguageList")
	IDMAP_ADD(IDC_VIEWACTIONLOG_BTN,			"ViewActionLogButton")
	IDMAP_ADD(IDC_BACKUPCFG_BTN,				"BackupConfigButton")
	IDMAP_ADD(IDC_SCHEDULE_BTN,					"ScheduleButton")
	IDMAP_ADD(IDC_LOGVIEWER_BTN,				"LogViewerButton")
	IDMAP_ADD(IDC_HOTKEYSET_BTN,				"HotkeySetButton")
	IDMAP_ADD(IDC_PWRREMINDER_BTN,				"PowerReminderButton")
	IDMAP_ADD(IDC_HELP_BTN,						"HelpButton")
	IDMAP_ADD(IDC_ABOUT_BTN,					"AboutButton")
	IDMAP_ADD(IDC_DEFAULT_BTN,					"DefaultButton")
	IDMAP_ADD(IDC_LEFTMOUSE_TITLE,				"LMBTitle")
	IDMAP_ADD(IDC_MIDMOUSE_TITLE,				"MMBTitle")
	IDMAP_ADD(IDC_RIGHTMOUSE_TITLE,				"RMBTitle")
	IDMAP_ADD(IDC_SYSTEM_TITLE,					"AdvSettingsTitle")
	IDMAP_ADD(IDC_LANGUAGE_TITLE,				"AppLanguageTitle")

	// Menu items
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_ABOUT,			"Menu.AboutDlg")
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_HELP,			"Menu.HelpDlg")
	IDMAP_ADD(IDM_NOTIFY_VIEW_ACTIONLOG,		"Menu.ViewActionLog")
	IDMAP_ADD(IDM_NOTIFY_BACKUP_CONFIG,			"Menu.BackupConfig")
	IDMAP_ADD(IDM_NOTIFY_VIEW_BAKCONFIG,		"Menu.ViewBakConfig")
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_LOGVIEWER,		"Menu.LogViewerDlg")
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_SCHEDULE,		"Menu.ScheduleDlg")
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_HOTKEYSET,		"Menu.HotkeySetDlg")
	IDMAP_ADD(IDM_NOTIFY_OPENDLG_PWRREMINDER,	"Menu.PowerReminderDlg")
	IDMAP_ADD(IDM_NOTIFY_ACTION_DISPLAYOFF,		"Menu.Action.DisplayOff")
	IDMAP_ADD(IDM_NOTIFY_ACTION_SLEEP,			"Menu.Action.Sleep")
	IDMAP_ADD(IDM_NOTIFY_ACTION_SHUTDOWN,		"Menu.Action.Shutdown")
	IDMAP_ADD(IDM_NOTIFY_ACTION_RESTART,		"Menu.Action.Restart")
	IDMAP_ADD(IDM_NOTIFY_ACTION_SIGNOUT,		"Menu.Action.SignOut")
	IDMAP_ADD(IDM_NOTIFY_ACTION_HIBERNATE,		"Menu.Action.Hibernate")
	IDMAP_ADD(IDM_NOTIFY_ACTION_SCHEDULE,		"Menu.Action.Schedule")
	IDMAP_ADD(IDM_NOTIFY_RESTART_APP,			"Menu.RestartApp")
	IDMAP_ADD(IDM_NOTIFY_RESTART_ASADMIN,		"Menu.RestartAsAdmin")
	IDMAP_ADD(IDM_NOTIFY_SHOW_WINDOW,			"Menu.ShowWindow")
	IDMAP_ADD(IDM_NOTIFY_EXIT_APP,				"Menu.ExitApp")
END_ID_MAPPING()

//////////////////////////////////////////////////////////////////////////
//
//	CPowerPlusDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

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
	ON_MESSAGE(SCM_DLGDESTROY_NOTIFY,			&CPowerPlusDlg::OnChildDialogDestroy)
	ON_MESSAGE(SM_APP_UPDATE_SCHEDULEDATA,		&CPowerPlusDlg::OnUpdateScheduleData)
	ON_MESSAGE(SM_APP_UPDATE_HOTKEYSETDATA,		&CPowerPlusDlg::OnUpdateHotkeySetData)
	ON_MESSAGE(SM_APP_UPDATE_PWRREMINDERDATA,	&CPowerPlusDlg::OnUpdatePwrReminderData)
	ON_MESSAGE(SM_APP_DEBUGCOMMAND,				&CPowerPlusDlg::OnProcessDebugCommand)
	ON_MESSAGE(SM_WND_SHOWDIALOG,				&CPowerPlusDlg::OnShowDialog)
	ON_MESSAGE(SM_APP_SHOW_ERROR_MSG,			&CPowerPlusDlg::OnShowErrorMessage)
	ON_COMMAND_RANGE(IDC_SHOWATSTARTUP_CHK, IDC_ENBPWRREMINDER_CHK, &CPowerPlusDlg::OnCheckboxClicked)
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementation

// CPowerPlusDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize main dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default result
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::OnInitDialog()
{
	// First, init with base class method
	SDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Setup system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_TEXT_EN_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set main window app title
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		SetDlgTitle(pApp->GetAppWindowTitle());
	}

	// Load app default icon
	SetIcon(IDI_ICON_DEFAULT);

	// Set icon
	SetIcon(m_hDefaultIcon, TRUE);		// Set big icon
	SetIcon(m_hDefaultIcon, FALSE);		// Set small icon

	// Do not exit using Esc button
	SetUseEscape(FALSE);

	// Do not apply settings with Enter button
	SetUseEnter(FALSE);

	// Set app features standard timers
	SetTimer(TIMERID_STD_ACTIONSCHEDULE, 1000, NULL);
	SetTimer(TIMERID_STD_POWERREMINDER, 1000, NULL);
	SetTimer(TIMERID_STD_EVENTSKIPCOUNTER, 1000, NULL);

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_STARTUP);

	// First, init all default data
	SetDefaultData(&m_cfgAppConfig);
	SetDefaultData(&m_cfgTempConfig);
	SetDefaultData(&m_schScheduleData);
	SetDefaultData(&m_hksHotkeySetData);
	SetDefaultData(&m_prdReminderData);

	// Load data
	GetAppData(APPDATA_ALL);

	// Setup main dialog
	SetupLanguage();
	UpdateDialogData(FALSE);
	SetNotifyIcon();

	// Initialize dialog state as collapsed
	ExpandDialog(FALSE);

	// Initialize background hotkeys if enabled
	SetupBackgroundHotkey(DEF_MODE_INIT);

	// Execute Power Reminder at startup
	ExecutePowerReminder(PREVT_AT_APPSTARTUP);

	// Execute Power Reminder after power action awake
	if (GetPwrActionFlag() == FLAG_ON) {
		ExecutePowerReminder(PREVT_AT_PWRACTIONWAKE);
		SetPwrActionFlag(FLAG_OFF);			// Reset flag
		SetSystemSuspendFlag(FLAG_OFF);		// Reset flag
		SetSessionEndFlag(FLAG_OFF);		// Reset flag
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}
	}

	// Execute Power Reminder after system awake (or after session ending)
	if ((GetSystemSuspendFlag() == FLAG_ON) || (GetSessionEndFlag() == FLAG_ON)) {
		ExecutePowerReminder(PREVT_AT_SYSWAKEUP);
		SetSystemSuspendFlag(FLAG_OFF);		// Reset flag
		SetSessionEndFlag(FLAG_OFF);		// Reset flag
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreDestroyDialog
//	Description:	Pre-destroy dialog and exit application
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::PreDestroyDialog()
{
	// Request closing all child dialog if opening
	LRESULT resCloseReq;

	// About dialog
	if (m_pAboutDlg != NULL) {
		// Request close dialog
		 resCloseReq = m_pAboutDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}
	// Help dialog
	if (m_pHelpDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHelpDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}
	// LogViewer dialog
	if (m_pLogViewerDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pLogViewerDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}
	// Multi schedule dialog
	if (m_pMultiScheduleDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pMultiScheduleDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}
	// HotkeySet dialog
	if (m_pHotkeySetDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHotkeySetDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}
	// Power Reminder dialog
	if (m_pPwrReminderDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pPwrReminderDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return;
	}

	// Destroy components
	RemoveNotifyIcon();
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Execute Power Reminder before exitting
	ExecutePowerReminder(PREVT_AT_APPEXIT);

	// Destroy background hotkeys if enabled
	SetupBackgroundHotkey(DEF_MODE_DISABLE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Destroy dialog and quit
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnDestroy()
{
	// Destroy background hotkeys if enabled
	SetupBackgroundHotkey(DEF_MODE_DISABLE);

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_DESTROYED);

	// Destroy dialog
	SDialog::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSysCommand
//	Description:	OnSysCommand function
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPaint
//	Description:	OnPaint function
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnQueryDragIcon
//	Description:	OnQueryDragIcon function
//  Arguments:		None
//  Return value:	HCURSOR - Default
//
//////////////////////////////////////////////////////////////////////////

HCURSOR CPowerPlusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hDefaultIcon);
}

//////////////////////////////////////////////////////////////////////////
// Message processing functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnApply
//	Description:	Handle click event for [Apply] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnApply()
{
	// Save app event logs if enabled
	OutputButtonLog(GetDialogID(), IDC_APPLY_BTN);

	// Apply settings and hide dialog
	ApplySettings(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnReload
//	Description:	Handle click event for [Reload] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnReload()
{
	// Save app event logs if enabled
	OutputButtonLog(GetDialogID(), IDC_RELOAD_BTN);

	// Message caption
	UINT nCaptionID = MSGBOX_CONFIG_RELOAD_CAPTION;

	// Check for setting changed
	m_bChangeFlag = CheckSettingChangeState();
	if (m_bChangeFlag == TRUE) {
		int nRet = DisplayMessageBox(MSGBOX_CONFIG_CHANGED_CONTENT, nCaptionID, MB_YESNO | MB_ICONINFORMATION);
		if (nRet == IDYES) {
			// Reload all settings
			ReloadSettings();
			OutputAppEventLog("Settings reloaded");
		}
	}
	else {
		// There's nothing changed
		DisplayMessageBox(MSGBOX_CONFIG_NOTCHANGED_CONTENT, nCaptionID, MB_OK | MB_ICONINFORMATION);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnExit
//	Description:	Handle click event for [Exit] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnExit()
{
	// Save app event logs if enabled
	OutputButtonLog(GetDialogID(), IDC_EXIT_BTN);

	// Destroy dialog and exit
	PreDestroyDialog();

	// Termination
	PostMessage(WM_DESTROY);
	EndDialog(IDCANCEL);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClose
//	Description:	Handle click event for [X] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnClose()
{
	// Check for setting changed
	m_bChangeFlag = CheckSettingChangeState();
	if (m_bChangeFlag == TRUE) {
		// Apply settings and hide dialog
		ApplySettings(TRUE);
	}
	else {
		// Only hide the dialog
		HWND hWnd = this->GetSafeHwnd();
		ShowDialog(hWnd, FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnExpand
//	Description:	Handle click event for [Expand/Collapse] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnExpand()
{
	// Save app event log if enabled
	BOOL bCurState = GetFlagValue(FLAGID_DLGEXPANDED);
	UINT nButtonID = bCurState ? IDC_COLLAPSE_BTN : IDC_EXPAND_BTN;
	OutputButtonLog(GetDialogID(), nButtonID);

	// Expand/collapse dialog
	ExpandDialog(!bCurState);
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnAbout
//	Description:	Show about dialog when pressing [About] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnAbout()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_ABOUT_BTN);

	// Open About dialog
	OpenChildDialogEx(IDD_ABOUT_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnHelp
//	Description:	Show help dialog when pressing [Help] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnHelp()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HELP_BTN);

	// Open Help dialog
	OpenChildDialogEx(IDD_HELP_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDefault
//	Description:	Handle click event for [Default] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnDefault()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_DEFAULT_BTN);

	// Set default options
	SetDefaultData(&m_cfgTempConfig);

	// Reload app language & reset language display
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();
	SetupLanguage();

	// Reupdate dialog items
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeLMBAction
//	Description:	Change action for left mouse button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnChangeLMBAction()
{
	// Update data
	UpdateDialogData(TRUE);
	int nCurSel = m_cmbLMBAction.GetCurSel();

	// Check for settings change
	m_bChangeFlag = CheckSettingChangeState();

	// Save app event log if enabled
	OutputComboLog(GetDialogID(), IDC_LMBACTION_LIST, nCurSel);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeMMBAction
//	Description:	Change action for middle mouse button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnChangeMMBAction()
{
	// Update data
	UpdateDialogData(TRUE);
	int nCurSel = m_cmbMMBAction.GetCurSel();

	// Check for settings change
	m_bChangeFlag = CheckSettingChangeState();

	// Save app event log if enabled
	OutputComboLog(GetDialogID(), IDC_MMBACTION_LIST, nCurSel);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeRMBAction
//	Description:	Change action for right mouse button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnChangeRMBAction()
{
	// Update data
	UpdateDialogData(TRUE);
	int nCurSel = m_cmbRMBAction.GetCurSel();

	// Check for settings change
	m_bChangeFlag = CheckSettingChangeState();

	// Save app event log if enabled
	OutputComboLog(GetDialogID(), IDC_RMBACTION_LIST, nCurSel);

	// Refresh dialog item state
	m_cmbRMBAction.EnableWindow(!m_bRMBShowMenu);
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChangeLanguage
//	Description:	Update dialog items when changing language
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnChangeLanguage()
{
	// Update data
	UpdateDialogData(TRUE);
	int nCurSel = m_cmbLanguages.GetCurSel();

	// Reload app language
	UINT nCurLanguage = GetAppOption(OPTIONID_LANGUAGEID, TRUE);
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage(nCurLanguage);

	// Check for settings change
	m_bChangeFlag = CheckSettingChangeState();

	// Save app event log if enabled
	OutputComboLog(GetDialogID(), IDC_LANGUAGE_LIST, nCurSel);

	// Refresh dialog display
	SetupLanguage();
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnEnableRightMouseMenu
//	Description:	Handle click event for "Right mouse action" checkbox
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnEnableRightMouseMenu()
{
	// Using custom checkbox clicked event handler
	OnCheckboxClicked(IDC_ENABLERMBMENU_CHK);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCheckboxClicked
//	Description:	Handle click event on checkbox-es
//  Arguments:		nID - ID of clicked checkbox
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnCheckboxClicked(UINT nID)
{
	// Get clicked checkbox control
	CButton* pChkCtrl = (CButton*)GetDlgItem(nID);
	if (pChkCtrl == NULL) return;

	// Update checkbox state
	UpdateDialogData(TRUE);
	int nState = pChkCtrl->GetCheck();

	// Update setting change flag
	m_bChangeFlag = CheckSettingChangeState();

	// Save app event log if enabled
	UINT nStateMsg = (nState == 1) ? LOG_EVENT_CHKBOX_CHECKED : LOG_EVENT_CHKBOX_UNCHECKED;
	OutputCheckboxLog(GetDialogID(), nID, nStateMsg);

	// Do custom checkbox actions
	switch (nID)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnBackupConfig
//	Description:	Handle click event for [Backup Configuration] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnBackupConfig()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_BACKUPCFG_BTN);

	// Backup configuration
	BOOL bRet = ((CPowerPlusApp*)AfxGetApp())->BackupRegistryAppData();

	// Show error message if enabled
	if (bRet == FALSE) {
		ShowErrorMessage(DEF_APP_ERROR_BACKUP_REG_FAILED);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnLogViewer
//	Description:	Handle click event for [Logviewer] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnLogViewer()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_LOGVIEWER_BTN);

	// Open LogViewer dialog
	OpenChildDialogEx(IDD_LOGVIEWER_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSchedule
//	Description:	Handle click event for [Schedule] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnSchedule()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_SCHEDULE_BTN);

	// Open Schedule dialog
	OpenChildDialogEx(IDD_MULTISCHEDULE_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnHotkeySet
//	Description:	Handle click event for [HotkeySet] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnHotkeySet()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_BTN);

	// Open HotkeySet dialog
	OpenChildDialogEx(IDD_HOTKEYSET_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPowerReminder
//	Description:	Handle click event for [Power Reminder] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnPowerReminder()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_BTN);

	// Open PowerReminder dialog
	OpenChildDialogEx(IDD_PWRREMINDER_DLG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnViewActionLog
//	Description:	Open action log file with Notepad
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnViewActionLog()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_VIEWACTIONLOG_BTN);

	// View action log file
	OpenFileToView(FILE_ACTION_LOG, DIR_SUBDIR_LOG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnViewBackupConfig
//	Description:	Open backed-up configuration file with Notepad
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnViewBackupConfig()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_VIEWBAKCFG_BTN);

	// View backup config file
	OpenFileToView(FILE_BAK_CONFIG);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimer
//	Description:	OnTimer function
//  Arguments:		nIDEvent - Time event ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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
		BOOL bPwrReminderActive = GetAppOption(OPTIONID_ENABLEPWRREMINDER);
		if (bPwrReminderActive == TRUE) {
			// Execute Power reminder
			ExecutePowerReminder(PREVT_AT_SETTIME);
		}
	}

	// Timer ID: Event skip counter
	else if (nIDEvent == TIMERID_STD_EVENTSKIPCOUNTER) {
		// Process Power Broadcast event skip counter
		int nCounter = GetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT);
		if (nCounter > 0) {
			// Count down (decrease value by 1)
			SetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT, --nCounter);
		}
	}

	// Default
	SDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnChildDialogDestroy
//	Description:	Handle event when a child dialog is closed/destroyed
//  Arguments:		wParam - Child dialog ID
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnChildDialogDestroy(WPARAM wParam, LPARAM lParam)
{
	// Get dialog ID
	UINT nDialogID = (UINT)wParam;

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

	return LRESULT(DEF_RESULT_SUCCESS);	// ERROR_SUCCESS
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUpdateScheduleData
//	Description:	Update schedule data if changed
//  Arguments:		wParam - Not used
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnUpdateScheduleData(WPARAM wParam, LPARAM lParam)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		PSCHEDULEDATA pschData = pApp->GetAppScheduleData();
		if (pschData != NULL) {
			m_schScheduleData.Copy(*pschData);
		}
	}

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUpdateHotkeySetData
//	Description:	Update HotkeySet data if changed
//  Arguments:		wParam - Not used
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnUpdateHotkeySetData(WPARAM wParam, LPARAM lParam)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		PHOTKEYSETDATA phksData = pApp->GetAppHotkeySetData();
		if (phksData != NULL) {
			m_hksHotkeySetData.Copy(*phksData);
			SetupBackgroundHotkey(DEF_MODE_UPDATE);
		}
	}

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUpdatePwrReminderData
//	Description:	Update Power Reminder data if changed
//  Arguments:		wParam - Not used
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnUpdatePwrReminderData(WPARAM wParam, LPARAM lParam)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		PPWRREMINDERDATA ppwrData = pApp->GetAppPwrReminderData();
		if (ppwrData != NULL) {
			m_prdReminderData.Copy(*ppwrData);
			UpdatePwrReminderSnooze(DEF_MODE_UPDATE);
		}
	}

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnProcessDebugCommand
//	Description:	Handle debug command message
//  Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnProcessDebugCommand(WPARAM wParam, LPARAM lParam)
{
	// Check argument validity
	if ((lParam == NULL) || ((wParam == NULL) && (lParam == NULL))) {
		OutputDebugLog(DEF_STRING_NULL);
		return LRESULT(DEF_RESULT_FAILED);
	}

	// Process debug command
	DWORD dwErrorCode = DEF_APP_ERROR_SUCCESS;
	if (!ProcessDebugCommand((LPCTSTR)lParam, dwErrorCode)) {
		OutputDebugLogFormat(_T("Failed! (Error code: 0x%02X)"), dwErrorCode);
		return LRESULT(DEF_RESULT_FAILED);
	}

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnShowDialog
//	Description:	Show/hide dialog when receiving message
//  Arguments:		wParam - Show/hide flag
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnShowDialog(WPARAM wParam, LPARAM lParam)
{
	BOOL bShowFlag = TRUE;
	if (wParam != NULL) {
		// Get flag value
		bShowFlag = (BOOL)wParam;
	}

	// Show/hide dialog
	HWND hWnd = this->GetSafeHwnd();
	ShowDialog(hWnd, bShowFlag);

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnShowErrorMessage
//	Description:	Show error when receiving message
//  Arguments:		wParam - Error code
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnShowErrorMessage(WPARAM wParam, LPARAM lParam)
{
	DWORD dwErrorCode = NULL;
	if (wParam != NULL) {
		// Get error code value
		dwErrorCode = (DWORD)wParam;
	}

	// Show error message
	ShowErrorMessage(dwErrorCode);

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPowerBroadcastEvent
//	Description:	Handle power broadcast event
//	Arguments:		wParam - Event ID
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnPowerBroadcastEvent(WPARAM wParam, LPARAM lParam)
{
	// Check if event skip counter is triggered
	if (GetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT) > 0)
		return LRESULT(DEF_RESULT_FAILED);

	// Get event ID from param
	ULONG ulEvent = ULONG(wParam);
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();

	// Process system resume/wakeup events
	if ((ulEvent == PBT_APMRESUMESUSPEND) || (ulEvent == PBT_APMRESUMEAUTOMATIC)) {

		// Trigger skip event counter
		// Temporarily skip processing PowerBroadcastEvent in 3 seconds
		SetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT, 3);

		// If Power action flag is triggered
		// handle it like a wakeup event after power action
		if (GetPwrActionFlag() == FLAG_ON) {
			// Execute Power Reminder after power action awake
			ExecutePowerReminder(PREVT_AT_PWRACTIONWAKE);
			SetPwrActionFlag(FLAG_OFF);		// Reset flag
		}
		// Otherwise, handle it like regular system wakeup event
		else {
			// Execute Power Reminder at system awaken event
			ExecutePowerReminder(PREVT_AT_SYSWAKEUP);
		}

		// Reset session ending flag
		SetSessionEndFlag(FLAG_OFF);

		// Reset system suspended flag
		SetSystemSuspendFlag(FLAG_OFF);
		if (pApp != NULL) {
			// Save flag value updates
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}
		
		// Output action history after waken up
		SaveActionHistory();
	}
	// Process system suspend event
	else if (ulEvent == PBT_APMSUSPEND) {
		// Turn on system suspended flag
		SetSystemSuspendFlag(FLAG_ON);
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}
	}

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnQuerryEndSession
//	Description:	Handle querry ending session event
//	Arguments:		wParam - Not used
//					lParam - Not used
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnQuerryEndSession(WPARAM wParam, LPARAM lParam)
{
	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) 
		return LRESULT(DEF_RESULT_FAILED);

	/*---------- Process querry ending session event ----------*/

	// Turn on session ending flag
	SetSessionEndFlag(FLAG_ON);
	pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);

	// Save last session ending time
	SYSTEMTIME stCurSysTime = GetCurSysTime();
	pApp->SaveLastSysEventTime(SYSEVT_SESSIONEND, stCurSysTime);

	// Save action history if remaining unsaved
	SaveActionHistory();

	/*---------------------------------------------------------*/

	return LRESULT(DEF_RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCommand
//	Description:	Handle app command messages (WM_COMMAND)
//	Arguments:		wParam - First param (HIWORD)
//					lParam - Second param (LOWORD)
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

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
		OutputMenuLog(IDM_NOTIFY_ACTION_DISPLAYOFF);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_DISPLAYOFF);
		break;
	case IDM_NOTIFY_ACTION_SLEEP:
		OutputMenuLog(IDM_NOTIFY_ACTION_SLEEP);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SLEEP);
		break;
	case IDM_NOTIFY_ACTION_SHUTDOWN:
		OutputMenuLog(IDM_NOTIFY_ACTION_SHUTDOWN);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SHUTDOWN);
		break;
	case IDM_NOTIFY_ACTION_RESTART:
		OutputMenuLog(IDM_NOTIFY_ACTION_RESTART);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_RESTART);
		break;
	case IDM_NOTIFY_ACTION_SIGNOUT:
		OutputMenuLog(IDM_NOTIFY_ACTION_SIGNOUT);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SIGNOUT);
		break;
	case IDM_NOTIFY_ACTION_HIBERNATE:
		OutputMenuLog(IDM_NOTIFY_ACTION_HIBERNATE);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_HIBERNATE);
		break;
	case IDM_NOTIFY_ACTION_SCHEDULE:
		OutputMenuLog(IDM_NOTIFY_ACTION_SCHEDULE);
		ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE);
		break;

	/*********************************************************************/
	/*																	 */
	/*				Handle commands for other menu items				 */
	/*																	 */
	/*********************************************************************/

	case IDM_NOTIFY_OPENDLG_ABOUT:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_ABOUT);
		OpenChildDialogEx(IDD_ABOUT_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HELP:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_HELP);
		OpenChildDialogEx(IDD_HELP_DLG);
		break;
	case IDM_NOTIFY_VIEW_ACTIONLOG:
		OutputMenuLog(IDM_NOTIFY_VIEW_ACTIONLOG);
		OpenFileToView(FILE_ACTION_LOG, DIR_SUBDIR_LOG);
		break;
	case IDM_NOTIFY_BACKUP_CONFIG:
		OutputMenuLog(IDM_NOTIFY_BACKUP_CONFIG);
		((CPowerPlusApp*)AfxGetApp())->BackupRegistryAppData();
		break;
	case IDM_NOTIFY_VIEW_BAKCONFIG:
		OutputMenuLog(IDM_NOTIFY_VIEW_BAKCONFIG);
		OpenFileToView(FILE_BAK_CONFIG);
		break;
	case IDM_NOTIFY_OPENDLG_LOGVIEWER:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_LOGVIEWER);
		OpenChildDialogEx(IDD_LOGVIEWER_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_SCHEDULE:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_SCHEDULE);
		OpenChildDialogEx(IDD_MULTISCHEDULE_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HOTKEYSET:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_HOTKEYSET);
		OpenChildDialogEx(IDD_HOTKEYSET_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_PWRREMINDER:
		OutputMenuLog(IDM_NOTIFY_OPENDLG_PWRREMINDER);
		OpenChildDialogEx(IDD_PWRREMINDER_DLG);
		break;
	case IDM_NOTIFY_RESTART_APP:
		OutputMenuLog(IDM_NOTIFY_RESTART_APP);
		RequestRestart(IDM_NOTIFY_RESTART_APP, FALSE);
		break;
	case IDM_NOTIFY_RESTART_ASADMIN:
		OutputMenuLog(IDM_NOTIFY_RESTART_ASADMIN);
		RequestRestart(IDM_NOTIFY_RESTART_ASADMIN, TRUE);
		break;
	case IDM_NOTIFY_SHOW_WINDOW:
		OutputMenuLog(IDM_NOTIFY_SHOW_WINDOW);
		ExpandDialog(FALSE);
		PostMessage(SM_WND_SHOWDIALOG, TRUE);
		break;
	case IDM_NOTIFY_EXIT_APP:
		OutputMenuLog(IDM_NOTIFY_EXIT_APP);
		PreDestroyDialog();
		PostMessage(WM_DESTROY);
		EndDialog(IDCANCEL);
		break;
	default:
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WindowProc
//	Description:	Handle app window messages
//  Arguments:		message - Message ID
//					wParam - First param (HIWORD)
//					lParam - Second param (LOWORD)
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case SM_APP_TRAYICON:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			OutputNotifyIconLog(LOG_EVENT_NOTIFY_LMBCLICKED);
			ExecuteAction(DEF_APP_MACRO_LEFT_MOUSE);
			break;
		case WM_MBUTTONDOWN:
			OutputNotifyIconLog(LOG_EVENT_NOTIFY_MMBCLICKED);
			ExecuteAction(DEF_APP_MACRO_MIDDLE_MOUSE);
			//PostMessage(WM_COMMAND, IDM_SHOW_WINDOW, NULL);
			break;
		case WM_RBUTTONUP:
			OutputNotifyIconLog(LOG_EVENT_NOTIFY_RMBCLICKED);
			ExecuteAction(DEF_APP_MACRO_RIGHT_MOUSE);
			break;
		}
		return 0;
		break;
	case SM_WND_DEBUGTEST:
		OpenChildDialogEx(IDD_DEBUGTEST_DLG);
		break;
	case WM_QUERYENDSESSION:
		OnQuerryEndSession(NULL, NULL);
		break;
	case WM_HOTKEY:
		ProcessHotkey((UINT)wParam);
		break;
	case WM_POWERBROADCAST:
		OnPowerBroadcastEvent(wParam, NULL);
		break;
	}

	return SDialog::WindowProc(message, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
// Member functions using for resizing dialog

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExpandDialog
//	Description:	Expand/collapse dialog
//  Arguments:		bExpand - Flag to expand or collapse
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ExpandDialog(BOOL bExpand)
{
	// If new state is the same as current state, do nothing
	BOOL bCurState = GetFlagValue(FLAGID_DLGEXPANDED);
	if (bExpand == bCurState)
		return;

	CRect rcWnd, rcFrameWnd, rcChild, rcIntersection;
	CWnd* pWndChild = NULL;
	CWnd* pWndFrame = NULL;

	// Get [Expand/Collapse] button
	CWnd* pButton = GetDlgItem(IDC_EXPAND_BTN);
	if (pButton == NULL)
		return;

	// Get separator frame
	pWndFrame = GetDlgItem(IDC_FRAMEWND);
	if (pWndFrame == NULL)
		return;
	
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

	if (!bExpand) {

		// Calculate dialog size
		GetWindowRect(&rcWnd);
		if (m_pDialogSize == NULL) {
			m_pDialogSize = new CSize;
			m_pDialogSize->cx = rcWnd.right - rcWnd.left;
			m_pDialogSize->cy = rcWnd.bottom - rcWnd.top;
		}

		// Resize dialog
		SetWindowPos(NULL, 0, 0, rcWnd.right - rcWnd.left, rcFrameWnd.bottom - rcWnd.top, SWP_NOMOVE | SWP_NOZORDER);
		
		// Change [Expand/Collapse] button title
		SetControlText(pButton, IDC_EXPAND_BTN, pAppLang);
	}
	else {
		// Resize dialog
		SetWindowPos(NULL, 0, 0, m_pDialogSize->cx, m_pDialogSize->cy, SWP_NOMOVE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Change [Expand/Collapse] button title
		SetControlText(pButton, IDC_COLLAPSE_BTN, pAppLang);
	}

	// Save app event log if enabled
	UINT nEventID = bExpand ? LOG_EVENT_DLG_EXPANDED : LOG_EVENT_DLG_COLLAPSED;
	OutputDialogLog(GetDialogID(), nEventID);

	// Update flag
	BOOL bNewState = !bCurState;
	SetFlagValue(FLAGID_DLGEXPANDED, bNewState);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	MoveControls
//	Description:	Move controls to specific position
//  Arguments:		arrCtrlIDs - Array of control IDs
//					nCount	   - Number of controls
//					nDir	   - Moving direction
//					nDistance  - Moving distance
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::MoveControls(const int* arrCtrlIDs, int nCount, int nDir, int nDistance)
{
	// Check data validity
	if (arrCtrlIDs == NULL)
		return;

	RECT rcControl;
	CWnd* pControl = NULL;
	int nNewX = 0, nNewY = 0;

	// Loop through each item and move
	for (int nID = 0; nID < nCount; nID++)
	{
		// Get dialog item
		pControl = GetDlgItem(arrCtrlIDs[nID]);
		if (pControl == NULL)
			continue;

		// Get item rect
		pControl->GetWindowRect(&rcControl);

		// Move horizontal direction
		if (nDir == DEF_APP_MACRO_DIR_HORZ) {
			nNewX = rcControl.left + nDistance;
			nNewY = rcControl.top;
			pControl->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}

		// Move vertical direction
		else if (nDir == DEF_APP_MACRO_DIR_VERT) {
			nNewX = rcControl.left;
			nNewY = rcControl.top + nDistance;
			pControl->SetWindowPos(NULL, nNewX, nNewY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Notify icon functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetNotifyIcon
//	Description:	Setup notify icon
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetNotifyIcon()
{
	// If notify icon is showed, do nothing
	if (GetFlagValue(FLAGID_NOTIFYICONSHOWED))
		return;

	// Init notify icon
	if (m_pNotifyIconData == NULL) {
		m_pNotifyIconData = new NOTIFYICONDATA;
		if (m_pNotifyIconData == NULL) {
			// Initialization failed
			TRCLOG("Error: Notify icon initialization failed.");
			TRCDBG(__FUNCTION__, __FILE__, __LINE__);
			return;
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

	// Show notify icon
	Shell_NotifyIcon(NIM_ADD, m_pNotifyIconData);

	// Update flag
	SetFlagValue(FLAGID_NOTIFYICONSHOWED, TRUE);

	// Save app event log if enabled
	OutputNotifyIconLog(LOG_EVENT_NOTIFY_CREATED);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowNotifyMenu
//	Description:	Show notify menu
//  Arguments:		None
//	Return value:	BOOL - Show menu successfully or failed
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ShowNotifyMenu()
{
	// Prepare menu
	CMenu menuNotify, *pMenu;
	menuNotify.LoadMenu(IDR_MENU_NOTIFY_DEFAULT);
	pMenu = menuNotify.GetSubMenu(0);
	if (pMenu == NULL) {
		// Trace error
		TRCLOG("Error: Show notify menu failed!!!");
		TRCDBG(__FUNCTION__, __FILE__, __LINE__);
		return FALSE;
	}

	// Setup menu properties
	SetMenuItemText(pMenu);
	UpdateMenuItemState(pMenu);
	SetMenuDefaultItem(pMenu->m_hMenu, IDM_NOTIFY_SHOW_WINDOW, NULL);
	this->SetForegroundWindow();

	// Show menu
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	UINT nFlags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
	BOOL bResult = pMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y, (CWnd*)this, NULL);

	// Save app event log if enabled
	OutputNotifyIconLog(LOG_EVENT_NOTIFY_SHOWMENU);

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateNotifyIcon
//	Description:	Update notify icon changes
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateNotifyIcon()
{
	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL)
		return;

	// Update notify icon tip text
	SetNotifyTipText(m_pNotifyIconData);
	Shell_NotifyIcon(NIM_MODIFY, m_pNotifyIconData);

	// Save app event log if enabled
	OutputNotifyIconLog(LOG_EVENT_NOTIFY_UPDATETIPTEXT);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveNotifyIcon
//	Description:	Remove notify icon when exiting
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RemoveNotifyIcon()
{
	// If notify icon is not showed, do nothing
	if (!GetFlagValue(FLAGID_NOTIFYICONSHOWED))
		return;

	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL)
		return;

	// Remove notify icon
	m_pNotifyIconData->hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, m_pNotifyIconData);

	// Update flag
	SetFlagValue(FLAGID_NOTIFYICONSHOWED, FALSE);

	// Save app event log if enabled
	OutputNotifyIconLog(LOG_EVENT_NOTIFY_REMOVED);
}


//////////////////////////////////////////////////////////////////////////
// Data processing functions
	
//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppData
//	Description:	Get app data
//  Arguments:		dwDataType - App data type to get
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::GetAppData(UINT dwDataType /* = APPDATA_ALL */)
{
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

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
		PSCHEDULEDATA pschData = pApp->GetAppScheduleData();
		if (pschData != NULL) {
			m_schScheduleData.Copy(*pschData);
		}
	}

	// Get HotkeySet data
	if ((dwDataType & APPDATA_HOTKEYSET) != 0) {
		PHOTKEYSETDATA phksData = pApp->GetAppHotkeySetData();
		if (phksData != NULL) {
			m_hksHotkeySetData.Copy(*phksData);
		}
	}

	// Get Power Reminder data
	if ((dwDataType & APPDATA_PWRREMINDER) != 0) {
		PPWRREMINDERDATA ppwrData = pApp->GetAppPwrReminderData();
		if (ppwrData != NULL) {
			m_prdReminderData.Copy(*ppwrData);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppOption
//	Description:	Return app option by ID
//  Arguments:		eAppOptionID - ID of specific app option
//					bTemp		 - Temp value or saved value (saved value by default)
//  Return value:	int - App option value
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp /* = FALSE */)
{
	int nResult = DEF_INTEGER_INVALID;
	int nTempResult = DEF_INTEGER_INVALID;

	switch (eAppOptionID)
	{
	case OPTIONID_LMBACTION:
		nResult = m_cfgAppConfig.nLMBAction;
		nTempResult = m_cfgTempConfig.nLMBAction;
		break;
	case OPTIONID_MMBACTION:
		nResult = m_cfgAppConfig.nMMBAction;
		nTempResult = m_cfgTempConfig.nMMBAction;
		break;
	case OPTIONID_RMBACTION:
		nResult = m_cfgAppConfig.nRMBAction;
		nTempResult = m_cfgTempConfig.nRMBAction;
		break;
	case OPTIONID_RMBSHOWMENU:
		nResult = m_cfgAppConfig.bRMBShowMenu;
		nTempResult = m_cfgTempConfig.bRMBShowMenu;
		break;
	case OPTIONID_LANGUAGEID:
		nResult = m_cfgAppConfig.nLanguageID;
		nTempResult = m_cfgTempConfig.nLanguageID;
		break;
	case OPTIONID_SHOWDLGATSTARTUP:
		nResult = m_cfgAppConfig.bShowDlgAtStartup;
		nTempResult = m_cfgTempConfig.bShowDlgAtStartup;
		break;
	case OPTIONID_STARTUPENABLE:
		nResult = m_cfgAppConfig.bStartupEnabled;
		nTempResult = m_cfgTempConfig.bStartupEnabled;
		break;
	case OPTIONID_CONFIRMACTION:
		nResult = m_cfgAppConfig.bConfirmAction;
		nTempResult = m_cfgTempConfig.bConfirmAction;
		break;
	case OPTIONID_SAVEACTIONLOG:
		nResult = m_cfgAppConfig.bSaveActionLog;
		nTempResult = m_cfgTempConfig.bSaveActionLog;
		break;
	case OPTIONID_SAVEAPPEVENTLOG:
		nResult = m_cfgAppConfig.bSaveAppEventLog;
		nTempResult = m_cfgTempConfig.bSaveAppEventLog;
		break;
	case OPTIONID_RUNASADMIN:
		nResult = m_cfgAppConfig.bRunAsAdmin;
		nTempResult = m_cfgTempConfig.bRunAsAdmin;
		break;
	case OPTIONID_SHOWERRORMSG:
		nResult = m_cfgAppConfig.bShowErrorMsg;
		nTempResult = m_cfgTempConfig.bShowErrorMsg;
		break;
	case OPTIONID_NOTIFYSCHEDULE:
		nResult = m_cfgAppConfig.bNotifySchedule;
		nTempResult = m_cfgTempConfig.bNotifySchedule;
		break;
	case OPTIONID_ALLOWCANCELSCHED:
		nResult = m_cfgAppConfig.bAllowCancelSchedule;
		nTempResult = m_cfgTempConfig.bAllowCancelSchedule;
		break;
	case OPTIONID_ENABLEHOTKEYSET:
		nResult = m_cfgAppConfig.bEnableBackgroundHotkey;
		nTempResult = m_cfgTempConfig.bEnableBackgroundHotkey;
		break;
	case OPTIONID_ENABLEPWRREMINDER:
		nResult = m_cfgAppConfig.bEnablePowerReminder;
		nTempResult = m_cfgTempConfig.bEnablePowerReminder;
		break;
	}

	// Return temp data if required and is valid
	if ((bTemp == TRUE) && (nTempResult != DEF_INTEGER_INVALID))
		return nTempResult;

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogData
//	Description:	Update data values for dialog items
//  Arguments:		bUpdate - TRUE/FALSE: Same as default MFC UpdateData function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateDialogData(BOOL bUpdate)
{
	UINT nCmbSel = 0;

	if (bUpdate == FALSE) {
	/*----------------------------<Bind config data to dialog items>----------------------------*/

		// Left mouse button action combo-box
		nCmbSel = m_cfgTempConfig.nLMBAction;
		m_cmbLMBAction.SetCurSel(Opt2Sel(DEF_APP_ACTION, nCmbSel));

		// Middle mouse button action combo-box
		nCmbSel = m_cfgTempConfig.nMMBAction;
		m_cmbMMBAction.SetCurSel(Opt2Sel(DEF_APP_ACTION, nCmbSel));

		// Right mouse button show menu checkbox
		m_bRMBShowMenu = m_cfgTempConfig.bRMBShowMenu;

		// Right mouse button action combo-box
		nCmbSel = (m_bRMBShowMenu == TRUE) ? DEF_APP_ACTION_SHOWMENU : m_cfgTempConfig.nRMBAction;
		m_cmbRMBAction.SetCurSel(Opt2Sel(DEF_APP_ACTION, nCmbSel));
		EnableControl(IDC_RMBACTION_LIST, !m_bRMBShowMenu);

		// Update other checkbox-es
		m_bShowDlgAtStartup = m_cfgTempConfig.bShowDlgAtStartup;
		m_bStartupEnabled = m_cfgTempConfig.bStartupEnabled;
		m_bConfirmAction = m_cfgTempConfig.bConfirmAction;
		m_bSaveActionLog = m_cfgTempConfig.bSaveActionLog;
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
		EnableControl(IDC_LOGVIEWER_BTN, m_bSaveAppEventLog);
		EnableControl(IDC_HOTKEYSET_BTN, m_bEnableBackgroundHotkey);
		EnableControl(IDC_PWRREMINDER_BTN, m_bEnablePowerReminder);

		// Update dialog items
		UpdateData(FALSE);

	/*------------------------------------------------------------------------------------------*/
	}
	else {
	/*----------------------------<Bind dialog items data to config>----------------------------*/

		// Update dialog items
		UpdateData(TRUE);

		// Left mouse button action combo-box
		nCmbSel = m_cmbLMBAction.GetCurSel();
		m_cfgTempConfig.nLMBAction = Sel2Opt(DEF_APP_ACTION, nCmbSel);

		// Middle mouse button action combo-box
		nCmbSel = m_cmbMMBAction.GetCurSel();
		m_cfgTempConfig.nMMBAction = Sel2Opt(DEF_APP_ACTION, nCmbSel);

		// Right mouse button show menu checkbox
		m_cfgTempConfig.bRMBShowMenu = m_bRMBShowMenu;

		// Right mouse button action combo-box
		nCmbSel = Opt2Sel(DEF_APP_ACTION, DEF_APP_ACTION_SHOWMENU);
		nCmbSel = (m_cfgTempConfig.bRMBShowMenu == TRUE) ? nCmbSel : m_cmbRMBAction.GetCurSel();
		m_cfgTempConfig.nRMBAction = Sel2Opt(DEF_APP_ACTION, nCmbSel);

		// Update other checkbox-es
		m_cfgTempConfig.bShowDlgAtStartup = m_bShowDlgAtStartup;
		m_cfgTempConfig.bStartupEnabled = m_bStartupEnabled;
		m_cfgTempConfig.bConfirmAction = m_bConfirmAction;
		m_cfgTempConfig.bSaveActionLog = m_bSaveActionLog;
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
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckSettingChangeState
//	Description:	Check setting variables changing state
//  Arguments:		None
//  Return value:	BOOL - Change flag
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::CheckSettingChangeState()
{
	BOOL bChangeFlag = FALSE;

	bChangeFlag |= (m_cfgTempConfig.nLMBAction != m_cfgAppConfig.nLMBAction);
	bChangeFlag |= (m_cfgTempConfig.nMMBAction != m_cfgAppConfig.nMMBAction);
	bChangeFlag |= (m_cfgTempConfig.nRMBAction != m_cfgAppConfig.nRMBAction);
	bChangeFlag |= (m_cfgTempConfig.bRMBShowMenu != m_cfgAppConfig.bRMBShowMenu);
	bChangeFlag |= (m_cfgTempConfig.nLanguageID != m_cfgAppConfig.nLanguageID);
	bChangeFlag |= (m_cfgTempConfig.bShowDlgAtStartup != m_cfgAppConfig.bShowDlgAtStartup);
	bChangeFlag |= (m_cfgTempConfig.bStartupEnabled != m_cfgAppConfig.bStartupEnabled);
	bChangeFlag |= (m_cfgTempConfig.bConfirmAction != m_cfgAppConfig.bConfirmAction);
	bChangeFlag |= (m_cfgTempConfig.bSaveActionLog != m_cfgAppConfig.bSaveActionLog);
	bChangeFlag |= (m_cfgTempConfig.bSaveAppEventLog != m_cfgAppConfig.bSaveAppEventLog);
	bChangeFlag |= (m_cfgTempConfig.bRunAsAdmin != m_cfgAppConfig.bRunAsAdmin);
	bChangeFlag |= (m_cfgTempConfig.bShowErrorMsg != m_cfgAppConfig.bShowErrorMsg);
	bChangeFlag |= (m_cfgTempConfig.bNotifySchedule != m_cfgAppConfig.bNotifySchedule);
	bChangeFlag |= (m_cfgTempConfig.bAllowCancelSchedule != m_cfgAppConfig.bAllowCancelSchedule);
	bChangeFlag |= (m_cfgTempConfig.bEnableBackgroundHotkey != m_cfgAppConfig.bEnableBackgroundHotkey);
	bChangeFlag |= (m_cfgTempConfig.bEnablePowerReminder != m_cfgAppConfig.bEnablePowerReminder);

	return bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetFlagValue
//	Description:	Return flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//  Return value:	int - Flag value
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::GetFlagValue(APPFLAGID eFlagID)
{
	int nValue = DEF_INTEGER_INVALID;

	switch (eFlagID)
	{
	case FLAGID_CHANGEFLAG:
		nValue = m_bChangeFlag;
		break;
	case FLAGID_DLGEXPANDED:
		nValue = m_bDlgExpanded;
		break;
	case FLAGID_NOTIFYICONSHOWED:
		nValue = m_bNotifyIconShowed;
		break;
	case FLAGID_HOTKEYREGISTERED:
		nValue = m_bHotkeyRegistered;
		break;
	case FLAGID_RESTARTASADMIN:
		nValue = m_bRestartAsAdminFlag;
		break;
	case FLAGID_PWRBROADCASTSKIPCOUNT:
		nValue = m_nPwrBroadcastSkipCount;
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

void CPowerPlusDlg::SetFlagValue(APPFLAGID eFlagID, int nValue)
{
	// Check value validity
	if ((nValue == DEF_INTEGER_INVALID) &&
		((nValue != TRUE) && (nValue != FALSE)))
		return;

	switch (eFlagID)
	{
	case FLAGID_CHANGEFLAG:
		m_bChangeFlag = nValue;
		break;
	case FLAGID_DLGEXPANDED:
		m_bDlgExpanded = nValue;
		break;
	case FLAGID_NOTIFYICONSHOWED:
		m_bNotifyIconShowed = nValue;
		break;
	case FLAGID_HOTKEYREGISTERED:
		m_bHotkeyRegistered = nValue;
		break;
	case FLAGID_RESTARTASADMIN:
		m_bRestartAsAdminFlag = nValue;
		break;
	case FLAGID_PWRBROADCASTSKIPCOUNT:
		m_nPwrBroadcastSkipCount = nValue;
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Setup dialog functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		UINT nID = pWndChild->GetDlgCtrlID();

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
			int nState = GetFlagValue(FLAGID_DLGEXPANDED);
			if (nState == TRUE)	nID = IDC_COLLAPSE_BTN;
			else nID = IDC_EXPAND_BTN;
			SetControlText(pWndChild, nID, pAppLang);
		}	break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupComboBox
//	Description:	Setup data for combo-boxes
//  Arguments:		nComboID - ID of combo box
//					pLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR pLanguage)
{
	switch (nComboID)
	{
	case IDC_LMBACTION_LIST:
		// Left mouse button action list
		m_cmbLMBAction.ResetContent();
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_DISPLAYOFF));			// Turn off display
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SLEEP));				// Sleep
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbLMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_HIBERNATE));			// Hibernate
		break;

	case IDC_MMBACTION_LIST:
		// Middle mouse button action list
		m_cmbMMBAction.ResetContent();
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_DISPLAYOFF));			// Turn off display
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SLEEP));				// Sleep
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbMMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_HIBERNATE));			// Hibernate
		break;

	case IDC_RMBACTION_LIST:
		// Right mouse button action list
		m_cmbRMBAction.ResetContent();
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_DISPLAYOFF));			// Turn off display
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SLEEP));				// Sleep
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_HIBERNATE));			// Hibernate
		m_cmbRMBAction.AddString(GetLanguageString(pLanguage, COMBOBOX_ACTION_SHOWMENU));			// Just show menu
		break;

	case IDC_LANGUAGE_LIST:
		// App language list
		m_cmbLanguages.ResetContent();
		m_cmbLanguages.AddString(GetLanguageString(pLanguage, COMBOBOX_LANGUAGE_ENGLISH));			// English
		m_cmbLanguages.AddString(GetLanguageString(pLanguage, COMBOBOX_LANGUAGE_VIETNAMESE));		// Vietnamese
		m_cmbLanguages.AddString(GetLanguageString(pLanguage, COMBOBOX_LANGUAGE_SIMPCHINESE));		// Chinese
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Item state update functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableRightMouseMenu
//	Description:	Enable/disable right mouse action combo-box
//  Arguments:		bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnableRightMouseMenu(BOOL bEnable)
{
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableLogViewer
//	Description:	Enable/disable LogViewer function
//  Arguments:		bEnable - Enable to disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnableLogViewer(BOOL bEnable)
{
	// Prefer using app data option than temp config option
	BOOL bCheck = bEnable;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL)
		bCheck = pApp->GetAppEventLogOption();
	if (bCheck == FALSE) {
		EnableControl(IDC_LOGVIEWER_BTN, bCheck);
		return;
	}

	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableBackgroundHotkey
//	Description:	Enable/disable HotKeySet button
//  Arguments:		bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnableBackgroundHotkey(BOOL bEnable)
{
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnablePowerReminder
//	Description:	Enable/disable Power Reminder button
//  Arguments:		bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnablePowerReminder(BOOL bEnable)
{
	UpdateDialogData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateRestartAsAdminFlag
//	Description:	Update restart as admin flag
//  Arguments:		bFlag - Update flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateRestartAsAdminFlag(BOOL bFlag)
{
	// Check current set app data option
	BOOL bCheck = GetAppOption(OPTIONID_RUNASADMIN);

	// If option is currently ON, do not update flag
	if (bCheck == TRUE)	return;

	// Update flag
	SetFlagValue(FLAGID_RESTARTASADMIN, bFlag);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	EnableControl
//	Description:	Enable/disable control with corresponding ID
//  Arguments:		nCtrlID - Control ID
//					bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnableControl(UINT nCtrlID, BOOL bEnable)
{
	// If new control state is the same as current state, do nothing
	CWnd* pWndCtrl = GetDlgItem(nCtrlID);
	if (pWndCtrl == NULL) return;
	if (pWndCtrl->IsWindowEnabled() == bEnable)
		return;

	// Save app event log if enabled
	UINT nEventID;
	switch (nCtrlID)
	{
	// Buttons
	case IDC_APPLY_BTN:
	case IDC_RELOAD_BTN:
	case IDC_EXIT_BTN:
	case IDC_EXPAND_BTN:
	case IDC_ABOUT_BTN:
	case IDC_DEFAULT_BTN:
	case IDC_HELP_BTN:
	case IDC_VIEWACTIONLOG_BTN:
	case IDC_BACKUPCFG_BTN:
	case IDC_SCHEDULE_BTN:
	case IDC_LOGVIEWER_BTN:
	case IDC_HOTKEYSET_BTN:
	case IDC_PWRREMINDER_BTN:
		nEventID = (bEnable == TRUE) ? LOG_EVENT_BUTTON_ENABLED : LOG_EVENT_BUTTON_DISABLED;
		OutputButtonLog(GetDialogID(), nCtrlID, nEventID);
		break;

	// Combo-boxes
	case IDC_LMBACTION_LIST:
	case IDC_MMBACTION_LIST:
	case IDC_RMBACTION_LIST:
	case IDC_LANGUAGE_LIST:
		nEventID = (bEnable == TRUE) ? LOG_EVENT_COMBO_ENABLED : LOG_EVENT_COMBO_DISABLED;
		OutputComboLog(GetDialogID(), nCtrlID, 0, nEventID);
		break;

	// Checkboxes
	case IDC_ENABLERMBMENU_CHK:
	case IDC_SHOWATSTARTUP_CHK:
	case IDC_ENABLEAUTOSTART_CHK:
	case IDC_CONFIRMACTION_CHK:
	case IDC_SAVEACTIONLOG_CHK:
	case IDC_SAVEAPPEVENTLOG_CHK:
	case IDC_RUNASADMIN_CHK:
	case IDC_SHOWERROR_CHK:
	case IDC_SCHEDNOTIFY_CHK:
	case IDC_SCHEDALLOWCANCEL_CHK:
	case IDC_ENBBKGRDHOTKEYS_CHK:
	case IDC_ENBPWRREMINDER_CHK:
		nEventID = (bEnable == TRUE) ? LOG_EVENT_CHKBOX_ENABLED : LOG_EVENT_CHKBOX_DISABLED;
		OutputCheckboxLog(GetDialogID(), nCtrlID, nEventID);
		break;

	default:
		break;
	}

	// Enable/disable control
	pWndCtrl->EnableWindow(bEnable);
}


//////////////////////////////////////////////////////////////////////////
// Component update functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetMenuItemText
//	Description:	Setup language for menu items
//  Arguments:		pMenu - Menu pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetMenuItemText(CMenu* pMenu)
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup language for entry menu
	for (int nItem = 0; nItem < pMenu->GetMenuItemCount(); nItem++) {
		UINT nID = pMenu->GetMenuItemID(nItem);
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
		UINT nID = pSubMenu->GetMenuItemID(nItem);
		if (nID == 0) continue;

		// Set menu item title
		pSubMenu->ModifyMenu(nItem, MF_BYPOSITION | MF_STRING, nID, GetLanguageString(pAppLang, nID));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateMenuItemState
//	Description:	Update state for menu items
//  Arguments:		pMenu - Menu pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateMenuItemState(CMenu* pMenu)
{
	// Check validity
	if (pMenu == NULL)
		return;

	// Main menu
	for (int nItem = 0; nItem < pMenu->GetMenuItemCount(); nItem++) {
		BOOL bShowItem = TRUE;
		UINT nID = pMenu->GetMenuItemID(nItem);
		switch (nID)
		{
		case IDM_NOTIFY_OPENDLG_LOGVIEWER:
			bShowItem = GetAppOption(OPTIONID_SAVEAPPEVENTLOG);
			if (bShowItem == TRUE)
				bShowItem = GetAppOption(OPTIONID_SAVEAPPEVENTLOG, TRUE);
			break;
		case IDM_NOTIFY_OPENDLG_HOTKEYSET:
			bShowItem = GetAppOption(OPTIONID_ENABLEHOTKEYSET, TRUE);
			break;
		case IDM_NOTIFY_OPENDLG_PWRREMINDER:
			bShowItem = GetAppOption(OPTIONID_ENABLEPWRREMINDER, TRUE);
			break;
		default:
			continue;
			break;
		}

		// Disable item
		if (bShowItem == FALSE) {
			pMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			OutputMenuLog(nID, LOG_EVENT_MENU_DISABLED);
		}
	}

	// Sub-menu: Actions
	CMenu* pSubMenu = pMenu->GetSubMenu(IDM_NOTIFY_ACTIONS_TITLE);
	if (pSubMenu == NULL) return;
	for (int nItem = 0; nItem < pSubMenu->GetMenuItemCount(); nItem++) {
		BOOL bShowItem = TRUE;
		UINT nID = pSubMenu->GetMenuItemID(nItem);
		switch (nID)
		{
		case IDM_NOTIFY_ACTION_SCHEDULE:
			bShowItem = FALSE;
			break;
		default:
			continue;
			break;
		}

		// Disable item
		if (bShowItem == FALSE) {
			pSubMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
			OutputMenuLog(nID, LOG_EVENT_MENU_DISABLED);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetNotifyTipText
//	Description:	Get notify tip text
//  Arguments:		pNotifyIconData - Notify icon data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR CPowerPlusDlg::GetNotifyTipText(PNOTIFYICONDATA pNotifyIconData)
{
	// Check validity
	if (pNotifyIconData == NULL)
		return DEF_STRING_NULL;

	return pNotifyIconData->szTip;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetNotifyTipText
//	Description:	Set notify tip text
//  Arguments:		pNotifyIconData - Notify icon data pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetNotifyTipText(PNOTIFYICONDATA pNotifyIconData)
{
	// Check validity
	if (pNotifyIconData == NULL)
		return;

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strTipText;
	CString strFormat;
	CStringArray arrTipText;

	// Load language strings
	strFormat = GetLanguageString(pAppLang, NOTIFY_TIP_TEMPLATE);
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(idplNotifyTip, m_cfgAppConfig.nLMBAction)));
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(idplNotifyTip, m_cfgAppConfig.nMMBAction)));
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(idplNotifyTip, m_cfgAppConfig.nRMBAction)));

	strTipText.Format(strFormat, arrTipText[0], arrTipText[1], arrTipText[2]);

	// Set notify tip text
	StrCpyW(pNotifyIconData->szTip, strTipText.GetString());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetBallonTipText
//	Description:	Show balloon tip to notify schedule
//  Arguments:		nCurLanguage - Current language ID
//					nScheduleAction - Upcoming schedule action
//					nSecondLeft  - Number of second left
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetBalloonTipText(UINT nCurLanguage, UINT nScheduleAction, UINT nSecondLeft)
{
	// If notify icon doesn't exist, do nothing
	if (m_pNotifyIconData == NULL)
		return;

	// Setup balloon tip properties
	m_pNotifyIconData->uFlags = NIF_INFO;
	m_pNotifyIconData->dwInfoFlags = NIIF_INFO;
	m_pNotifyIconData->uTimeout = 300;

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strBalloonTip;
	CString strFormat;
	CString strBalloonText;

	// Load language strings
	strFormat = GetLanguageString(pAppLang, BALLOON_TIP_TEMPLATE);
	strBalloonText = GetLanguageString(pAppLang, GetPairedID(idplBalloonTip, nScheduleAction));

	strBalloonTip.Format(strFormat, strBalloonText, nSecondLeft);

	// Set balloon tip text
	StrCpy(m_pNotifyIconData->szInfoTitle, (PWSTR)(LPCTSTR)GetLanguageString(pAppLang, IDC_APPNAME_LABEL));
	StrCpy(m_pNotifyIconData->szInfo, (PWSTR)strBalloonTip.GetString());
	Shell_NotifyIcon(NIM_MODIFY, m_pNotifyIconData);
}


//////////////////////////////////////////////////////////////////////////
// Core functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecuteAction
//	Description:	Execute action as config/schedule/menu selection
//  Arguments:		nActionType - Action type
//					wParam		- First param (HIWORD)
//					lParam		- Second param (LOWORD)
//  Return value:	BOOL - Result of action execution
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ExecuteAction(UINT nActionType, WPARAM wParam /* = NULL */, LPARAM lParam /* = NULL */)
{
	UINT nAction = 0;
	UINT nActionID = 0;
	UINT nActionNameID = 0;
	UINT nMessage = 0;

	// Get action ID
	switch (nActionType)
	{
	case DEF_APP_MACRO_LEFT_MOUSE:
		// Get action ID
		nActionID = GetAppOption(OPTIONID_LMBACTION);
		break;
	case DEF_APP_MACRO_MIDDLE_MOUSE:
		// Get action ID
		nActionID = GetAppOption(OPTIONID_MMBACTION);
		break;
	case DEF_APP_MACRO_ACTION_SCHEDULE:
		// Get action ID from param
		if (wParam == NULL)	return FALSE;
		nActionID = (UINT)wParam;
		break;
	case DEF_APP_MACRO_RIGHT_MOUSE:
		// If right mouse action is set to show notify menu
		if ((GetAppOption(OPTIONID_RMBACTION) == DEF_APP_ACTION_SHOWMENU) ||
			(GetAppOption(OPTIONID_RMBSHOWMENU) == TRUE))
			return ShowNotifyMenu();
		nActionID = GetAppOption(OPTIONID_RMBACTION);
		break;
	case DEF_APP_MACRO_ACTION_MENU:
	case DEF_APP_MACRO_ACTION_HOTKEY:
		// Get action ID from param
		if (wParam == NULL)	return FALSE;
		nActionID = (UINT)wParam;
		break;
	default:
		return FALSE;
	}

	// Get action info
	switch (nActionID)
	{
	case DEF_APP_ACTION_DISPLAYOFF:
		// Turn off display
		nAction = DEF_APP_ACTIONTYPE_MONITOR;
		nMessage = DEF_APP_MESSAGE_DISPLAYOFF;
		nActionNameID = IDS_ACTIONLOG_ACTION_DISPLAYOFF;
		break;
	case DEF_APP_ACTION_SLEEP:
		// Sleep
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SLEEP;
		nActionNameID = IDS_ACTIONLOG_ACTION_SLEEP;
		break;
	case DEF_APP_ACTION_SHUTDOWN:
		// Shutdown
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SHUTDOWN;
		nActionNameID = IDS_ACTIONLOG_ACTION_SHUTDOWN;
		break;
	case DEF_APP_ACTION_RESTART:
		// Restart
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_REBOOT;
		nActionNameID = IDS_ACTIONLOG_ACTION_RESTART;
		break;
	case DEF_APP_ACTION_SIGNOUT:
		// Sign out
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SIGNOUT;
		nActionNameID = IDS_ACTIONLOG_ACTION_SIGNOUT;
		break;
	case DEF_APP_ACTION_HIBERNATE:
		// Hibernate
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_HIBERNATE;
		nActionNameID = IDS_ACTIONLOG_ACTION_HIBERNATE;
		break;
	default:
		return FALSE;
	}

	// Get action info to save logs
	m_actActionData.RemoveAll();
	m_actActionData.bInitState = FALSE;
	m_actActionData.nActionType = nMessage;
	m_actActionData.stActionTime = GetCurSysTime();
	m_actActionData.nActionNameID = nActionNameID;

	BOOL bResult = FALSE;
	DWORD dwError = DEF_APP_ERROR_SUCCESS;
	if (ConfirmAction(nActionType, nActionID) == IDYES) {

		// Execute Power Reminder before doing action
		ExecutePowerReminder(PREVT_AT_BFRPWRACTION);

		// Check if DummyTest mode is enabled or not
		BOOL bDummyTestMode = GetDummyTestMode();

		if (bDummyTestMode != TRUE) {
			// Normal mode
			bResult = ExecutePowerAction(nAction, nMessage, dwError);
		}
		else {
			// DummyTest mode
			bResult = ExecutePowerActionDummy(nAction, nMessage, dwError);
		}

		// Save Power Action trace flag
		SetPwrActionFlag((bResult == TRUE) ? FLAG_ON : FLAG_OFF);
		CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
		if (pApp != NULL) {
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}

		// Collect action result
		m_actActionData.bActionSucceed = bResult;
		m_actActionData.nErrorCode = dwError;

		// Show error message
		ShowErrorMessage(dwError);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ApplySettings
//	Description:	Apply changes and minimize window to tray
//  Arguments:		bMinimize - Minimize to tray
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ApplySettings(BOOL bMinimize)
{
	// Update data
	UpdateDialogData(TRUE);

	// Copy configurations and save
	m_cfgAppConfig.Copy(m_cfgTempConfig);
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		pApp->SetAppConfigData(&m_cfgAppConfig);
		BOOL bRet = pApp->SaveRegistryAppData();
		if (bRet == FALSE) {
			// Show error message
			DWORD dwErr = DEF_APP_ERROR_SAVE_CFG_FAILED;
			ShowErrorMessage(dwErr);
		}
	}

	// Restart as admin privileges if triggered
	BOOL bRestartTrigger = GetFlagValue(FLAGID_RESTARTASADMIN);
	if (bRestartTrigger == TRUE) {
		RequestRestart(IDC_APPLY_BTN, TRUE);
	}

	// Update background hotkeys if enabled
	SetupBackgroundHotkey(DEF_MODE_UPDATE);

	// Disable Power Reminder snooze queue if feature's disabled
	if (GetAppOption(OPTIONID_ENABLEPWRREMINDER) == NULL) {
		UpdatePwrReminderSnooze(DEF_MODE_DISABLE);
	}

	// Update notify icon tip text
	UpdateNotifyIcon();

	// Change display status
	if (bMinimize == TRUE) {
		// Minimize to tray (hide dialog)
		HWND hWnd = this->GetSafeHwnd();
		ShowDialog(hWnd, FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReloadSettings
//	Description:	Abort all changes and reload settings
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ReloadSettings()
{
	// Reload app data
	GetAppData();

	// Reload app language
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();

	// Reupdate dialog items
	SetupLanguage();
	UpdateDialogData(FALSE);

	// Reset data change flag
	m_bChangeFlag = FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowDialog
//	Description:	Show up dialog out of system tray
//  Arguments:		hWnd	  - Handle of the dialog to show/hide
//					bShowFlag - Flag to show/hide dialog
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ShowDialog(HWND hWnd, BOOL bShowFlag /* = TRUE */)
{
	// Get show/hide flag
	int nCmdShow = (bShowFlag == TRUE) ? SW_SHOW : SW_HIDE;

	// Get dialog pointer
	if (hWnd == NULL) return;
	CWnd* pDialogWnd = FromHandle(hWnd);
	if (pDialogWnd == NULL) return;

	// Show/hide dialog
	pDialogWnd->ShowWindow(nCmdShow);
	if (nCmdShow == SW_SHOW) {
		pDialogWnd->BringWindowToTop();
	}

	// Save app event log if enabled
	UINT nEventID = (bShowFlag == TRUE) ? LOG_EVENT_DLG_SHOWED : LOG_EVENT_DLG_HIDDEN;
	OutputDialogLog(dynamic_cast<SDialog*>(pDialogWnd)->GetDialogID(), nEventID);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenChildDialogEx
//	Description:	Open child dialog with corresponding ID
//  Arguments:		nDialogID	  - Dialog ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OpenChildDialogEx(UINT nDialogID)
{
	// Get app
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL)
		return;

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
			ShowDialog(m_pAboutDlg->GetSafeHwnd(), TRUE);
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
			ShowDialog(m_pHelpDlg->GetSafeHwnd(), TRUE);
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
			ShowDialog(m_pMultiScheduleDlg->GetSafeHwnd(), TRUE);
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
			ShowDialog(m_pLogViewerDlg->GetSafeHwnd(), TRUE);
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
			ShowDialog(m_pHotkeySetDlg->GetSafeHwnd(), TRUE);
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
			ShowDialog(m_pPwrReminderDlg->GetSafeHwnd(), TRUE);
		}
	}
	// DebugTest dialog
	else if (nDialogID == IDD_DEBUGTEST_DLG) {
		// Get app DebugTest dialog
		CDebugTestDlg* pDialog = pApp->GetDebugTestDlg();
		if (pDialog == NULL) {
			// Initialize dialog
			pDialog = new CDebugTestDlg();
			pDialog->SetParentWnd(this);

			// Open in modeless mode
			pDialog->Create(nDialogID, this);
			pDialog->ShowWindow(SW_SHOW);
		}
		else {
			// Show dialog
			pDialog->SetParentWnd(this);
			ShowDialog(pDialog->GetSafeHwnd(), TRUE);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenDialogBase
//	Description:	Open dialog with corresponding ID
//  Arguments:		nDialogID	  - Dialog ID
//					bReadOnlyMode - Read-only mode flag
//					nOpenMode	  - Open mode: Modal or modeless
//  Return value:	None
//	Note:			Base function (no longer used)
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OpenDialogBase(UINT nDialogID, BOOL bReadOnlyMode /* = FALSE */, int nOpenMode /* = DEF_MODE_OPENDLG_MODAL */)
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
		CString strDialogTitle = GetLanguageString(pLang, nDialogID);
		hDialogWnd = ::FindWindow(NULL, strDialogTitle);
	}

	// If yes, only bring that instance up to top instead of starting a new one
	if (hDialogWnd != NULL) {
		ShowDialog(hDialogWnd, TRUE);
		return;
	}

	// Dialog initialization
	SDialog* pDialog = NULL;
	BOOL bSetReadOnly = FALSE;
	CWnd* pParentWnd = NULL;
	UINT nEntryMsgID = DEF_INTEGER_NULL;
	{
		switch (nDialogID)
		{
		case IDD_ABOUT_DLG:
			// About dialog
			pDialog = new CAboutDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_HELP_DLG:
			// Help dialog
			pDialog = new CHelpDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_MULTISCHEDULE_DLG:
			// Multi schedule dialog
			pDialog = new CMultiScheduleDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_LOGVIEWER_DLG:
			// LogViewer dialog
			pDialog = new CLogViewerDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_HOTKEYSET_DLG:
			// HotkeySet dialog
			pDialog = new CHotkeySetDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_PWRREMINDER_DLG:
			// Power Reminder dialog
			pDialog = new CPwrReminderDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			break;
		case IDD_DEBUGTEST_DLG:
			// DebugTest dialog
			pDialog = new CDebugTestDlg;
			bSetReadOnly = FALSE;
			pParentWnd = this;
			nOpenMode = DEF_MODE_OPENDLG_MODELESS;
		default:
			break;
		}

		// Show entry message before displaying dialog
		if (nEntryMsgID != DEF_INTEGER_NULL) {
			DisplayMessageBox(nEntryMsgID, (LPCTSTR)NULL, MB_OK | MB_ICONINFORMATION);
		}

		// Check validity
		if (pDialog == NULL)
			return;

		// Set read-only mode
		if (bSetReadOnly == TRUE) {
			pDialog->SetReadOnlyMode(bReadOnlyMode);
		}

		// Open dialog
		if (nOpenMode == DEF_MODE_OPENDLG_MODAL) {
			// Modal dialog
			pDialog->SetParentWnd(pParentWnd);
			pDialog->DoModal();
		}
		else if (nOpenMode == DEF_MODE_OPENDLG_MODELESS) {
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenFileToView
//	Description:	Open file to view using Notepad
//  Arguments:		lpszFileName - File name/path
//					lpszSubDir	- Sub-directory name
//  Return value:	BOOL - Result of file opening
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::OpenFileToView(LPCTSTR lpszFileName, LPCTSTR lpszSubDir /* = DEF_STRING_EMPTY */)
{
	// Get file name
	VERIFY(lpszFileName != NULL);
	CString strFilePath = lpszFileName;
	
	// If sub-directory name is not empty
	if (_tcscmp(lpszSubDir, DEF_STRING_EMPTY)) {
		// Format file path with sub-directory
		strFilePath.Format(_T("%s\\%s"), lpszSubDir, lpszFileName);
	}

	return FileViewStd(FILETYPE_TXT, strFilePath);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RestartApp
//	Description:	Restart application with/without admin privileges
//  Arguments:		bRestartAsAdmin - Restart with/without admin privileges
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RestartApp(BOOL bRestartAsAdmin)
{
	// Remove window title to prevent from startup checking
	this->SetWindowText(DEF_STRING_NULL);

	// Restart immediately
	PostQuitMessage(0);
	RunApp(GetAppPath(), bRestartAsAdmin);
}


//////////////////////////////////////////////////////////////////////////
// Advanced features functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessActionSchedule
//	Description:	Process Action schedule function
//  Arguments:		None
//  Return value:	BOOL - Schedule processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessActionSchedule()
{
	BOOL bResult = FALSE;

	// Get current time
	SYSTEMTIME stCurrentTime;
	GetLocalTime(&stCurrentTime);

	// Flag that trigger to reupdate schedule data
	BOOL bTriggerReupdate = FALSE;

	// Get default schedule item
	SCHEDULEITEM& schDefaultItem = m_schScheduleData.GetDefaultItem();
	{
		// Flag to skip processing schedule item
		BOOL bSkipProcess = FALSE;

		// If item is not enabled
		if (schDefaultItem.bEnable == FALSE) {
			// Do not process
			bSkipProcess = TRUE;
		}

		// If repeat option is ON and is set as active in current day of week
		if ((schDefaultItem.bRepeat == TRUE) && (!schDefaultItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek))) {
			// Do not process
			bSkipProcess = TRUE;
		}

		// Process default schedule
		if (bSkipProcess != TRUE) {

			// Check for time matching and trigger schedule notifying if enabled
			if (GetAppOption(OPTIONID_NOTIFYSCHEDULE) == TRUE) {
				BOOL bTriggerNotify = CheckTimeMatch(stCurrentTime, schDefaultItem.stTime, -30);
				if (bTriggerNotify == TRUE) {
					// Do notify schedule (and check for trigger reupdate)
					int nRetNotify = NotifySchedule(&schDefaultItem, bTriggerReupdate);
					bResult = FALSE;
				}
			}

			// Check for time matching and trigger the scheduled action
			BOOL bTriggerAction = CheckTimeMatch(stCurrentTime, schDefaultItem.stTime);
			if (bTriggerAction == TRUE) {
				// Execute schedule action
				bResult = ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE, schDefaultItem.nAction);

				// If "Repeat" option is not ON,
				// --> Disable schedule item after done
				if (schDefaultItem.bRepeat == FALSE) {
					schDefaultItem.SetActiveState(FALSE);
					bTriggerReupdate |= TRUE;
				}
			}
		}
		else {
			// Process failed
			bResult = FALSE;
		}
	}

	// Loop through each extra item and process
	for (int nExtraIndex = 0; nExtraIndex < m_schScheduleData.GetExtraItemNum(); nExtraIndex++) {

		// Get schedule item
		SCHEDULEITEM& schExtraItem = m_schScheduleData.GetItemAt(nExtraIndex);

		// Do not process if repeat option is ON but is not set as active in current day of week
		if ((schExtraItem.bRepeat == TRUE) && (!schExtraItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek)))
			continue;

		// Check for time matching and trigger schedule notifying if enabled
		if (GetAppOption(OPTIONID_NOTIFYSCHEDULE) == TRUE) {
			BOOL bTriggerNotify = CheckTimeMatch(stCurrentTime, schExtraItem.stTime, -30);
			if (bTriggerNotify == TRUE) {
				// Do notify schedule (and check for trigger reupdate)
				int nRetNotify = NotifySchedule(&schExtraItem, bTriggerReupdate);
				bResult = FALSE;
				continue;
			}
		}

		// Check for time matching and trigger the scheduled action
		BOOL bTriggerAction = CheckTimeMatch(stCurrentTime, schExtraItem.stTime);
		if (bTriggerAction == TRUE) {
			// Execute schedule action
			bResult = ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE, schExtraItem.nAction);

			// If "Repeat" option is not ON,
			// --> Disable schedule item after done
			if (schExtraItem.bRepeat == FALSE) {
				schExtraItem.SetActiveState(FALSE);
				bTriggerReupdate |= TRUE;
			}
		}
	}

	// Reupdate flag is triggered
	if (bTriggerReupdate == TRUE) {
		// Reupdate schedule data
		ReupdateActionScheduleData();
		bResult = TRUE;
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReupdateActionScheduleData
//	Description:	Reupdate Action Schedule data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupBackgroundHotkey
//	Description:	Setup background hotkey if enabled
//  Arguments:		nMode - Mode of setup (INIT, UPDATE or DISABLE)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetupBackgroundHotkey(int nMode)
{
	// If background hotkey feature is disabled and no hotkey registered, do nothing
	BOOL bHKSEnable = GetAppOption(OPTIONID_ENABLEHOTKEYSET);
	BOOL bHKRegisterFlag = GetFlagValue(FLAGID_HOTKEYREGISTERED);

	if ((bHKSEnable == FALSE) &&											// HotkeySet option OFF
		(bHKRegisterFlag == FALSE) &&										// Hotkey registered flag OFF
		(m_arrCurRegHKeyList.IsEmpty())) {									// Registered hotkey list empty
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

	if ((nMode == DEF_MODE_DISABLE) || (nMode == DEF_MODE_UPDATE)) {
		if ((bHKRegisterFlag == TRUE) &&									// Hotkey registered flag ON
			(!m_arrCurRegHKeyList.IsEmpty())) {								// Registered hotkey list is NOT empty

			// Unregister currently registered hotkeys
			int nRegItemNum = m_arrCurRegHKeyList.GetSize();
			for (int nIndex = (nRegItemNum - 1); nIndex >= 0; nIndex--) {
				UINT nHKID = m_arrCurRegHKeyList.GetAt(nIndex);
				BOOL bRet = UnregisterHotKey(hWnd, nHKID);
				if (bRet == TRUE) {
					// Unregister successfully
					OutputDebugLogFormat(_T("Unregistered hotkey: %d"), nHKID);
					m_arrCurRegHKeyList.RemoveAt(nIndex);
					if (nIndex == 0) {										// Last item unregistered
						SetFlagValue(FLAGID_HOTKEYREGISTERED, FALSE);		// Reset hotkey registered flag
						m_arrCurRegHKeyList.RemoveAll();					// Cleanup registered hotkey list
						m_arrCurRegHKeyList.FreeExtra();
					}
				}
				else {
					// Unregister failed
					dwErrorCode = GetLastError();
					ShowErrorMessage(dwErrorCode);
					OutputDebugLogFormat(_T("Unregister hotkey failed: %d"), nHKID);
					DisplayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);

					// Trace error
					TRCLOG("Error: Hotkey unregister failed");
					TRCDBG(__FUNCTION__, __FILE__, __LINE__);
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

	if ((nMode == DEF_MODE_INIT) || (nMode == DEF_MODE_UPDATE)) {
		// If feature not enabled, do nothing
		if (bHKSEnable == FALSE)
			return;

		// If there's no item, do nothing
		int nItemNum = m_hksHotkeySetData.GetItemNum();
		if (nItemNum <= 0) return;

		// Reset flag and re-initialize registered hotkey list
		SetFlagValue(FLAGID_HOTKEYREGISTERED, FALSE);
		m_arrCurRegHKeyList.RemoveAll();
		m_arrCurRegHKeyList.FreeExtra();

		// Debug log
		CString strLogTemp;

		// Register each HotkeySet item
		BOOL bRegistered = FALSE;
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			// Get hotkey item
			HOTKEYSETITEM hksItem = m_hksHotkeySetData.GetItemAt(nIndex);

			// If item is empty, skip this item
			if (hksItem.IsEmpty()) 
				continue;

			// Get hotkey action ID
			UINT nHKActionID = hksItem.nHKActionID;

			// Get control keycode
			UINT nCtrlKeyCode = (UINT)hksItem.dwCtrlKeyCode;
			if (nCtrlKeyCode == 0)
				continue;

			nCtrlKeyCode |= MOD_NOREPEAT;

			// Get function keycode
			UINT nFuncKeyCode = (UINT)hksItem.dwFuncKeyCode;
			if (nFuncKeyCode == 0)
				continue;

			// Debug log format
			strLogTemp.Format(_T("ActionID=%d, CtrlKey=%d, FuncKey=%d"), nHKActionID, nCtrlKeyCode, nFuncKeyCode);

			// Get enable/disable status
			BOOL bEnable = hksItem.bEnable;

			// Skip registering item if disabled
			if (bEnable == FALSE) {
				OutputDebugLogFormat(_T("Skip registering hotkey (disabled): %s"), strLogTemp);
				continue;
			}

			// Register hotkey item
			BOOL bRet = RegisterHotKey(hWnd, nHKActionID, nCtrlKeyCode, nFuncKeyCode);
			bRegistered |= bRet;
			SetFlagValue(FLAGID_HOTKEYREGISTERED, bRegistered);

			if (bRet == TRUE) {
				// Register successfully
				OutputDebugLogFormat(_T("Registered hotkey: %s"), strLogTemp);
				m_arrCurRegHKeyList.Add(nHKActionID);						// Update registered hotkey list
			}
			else {
				// Register failed
				dwErrorCode = GetLastError();
				ShowErrorMessage(dwErrorCode);
				OutputDebugLogFormat(_T("Register hotkey failed: %s"), strLogTemp);
				DisplayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);

				// Trace error
				TRCLOG("Error: Hotkey register failed");
				TRCDBG(__FUNCTION__, __FILE__, __LINE__);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessHotkey
//	Description:	Process when registered hotkey is pressed
//  Arguments:		nHotkeyID - ID of pressed hotkey
//  Return value:	BOOL - Hotkey processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessHotkey(int nHotkeyID)
{
	// If "Background hotkey" option is not enabled, do nothing
	BOOL bHKSEnable = GetAppOption(OPTIONID_ENABLEHOTKEYSET);
	if (bHKSEnable == FALSE)
		return FALSE;

	// Output debug log
	OutputDebugLogFormat(_T("Hotkey pressed: ID = %d"), nHotkeyID);

	// Output app event log
	CStringA strEventLog;
	strEventLog.Format("Hotkey pressed: ID = %d", nHotkeyID);
	OutputAppEventLog(strEventLog);

	UINT nActionID = NULL;
	switch (nHotkeyID)
	{
	case HKID_DISPLAYOFF:
		nActionID = DEF_APP_ACTION_DISPLAYOFF;
		break;
	case HKID_SLEEP:
		nActionID = DEF_APP_ACTION_SLEEP;
		break;
	case HKID_SHUTDOWN:
		nActionID = DEF_APP_ACTION_SHUTDOWN;
		break;
	case HKID_RESTART:
		nActionID = DEF_APP_ACTION_RESTART;
		break;
	case HKID_SIGNOUT:
		nActionID = DEF_APP_ACTION_SIGNOUT;
		break;
	case HKID_HIBERNATE:
		nActionID = DEF_APP_ACTION_HIBERNATE;
		break;
	default:
		break;
	}

	// If invalid action ID, do nothing
	if (nActionID == NULL)
		return FALSE;
	
	// Execute hotkeyset action
	WPARAM wParam = (WPARAM)nActionID;
	BOOL bResult = ExecuteAction(DEF_APP_MACRO_ACTION_HOTKEY, wParam);
	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExecutePowerReminder
//	Description:	Excute Power Reminder action
//  Arguments:		nExecEventID - Execute event ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ExecutePowerReminder(UINT nExecEventID)
{
	// If "Power Reminder" option is not enabled, do nothing
	if (GetAppOption(OPTIONID_ENABLEPWRREMINDER) == FALSE)
		return FALSE;

	// If there's no item, do nothing
	int nItemNum = m_prdReminderData.GetItemNum();
	if (nItemNum <= 0) return FALSE;

	// Get current time
	SYSTEMTIME curSysTime;
	if (nExecEventID == PREVT_AT_SETTIME) {
		curSysTime = GetCurSysTime();
	}

	// Flag that trigger to reupdate Power Reminder data
	BOOL bTriggerReupdate = FALSE;

	// Search all items and process reminder
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		PWRREMINDERITEM& pwrCurItem = m_prdReminderData.GetItemAt(nIndex);

		// If item is empty, skip this item
		if (pwrCurItem.IsEmpty())
			continue;

		// If event ID is not matching, skip this item
		if (pwrCurItem.nEventID != nExecEventID)
			continue;

		// If item is not enabled, skip this item
		if (pwrCurItem.bEnable == FALSE)
			continue;

		// Process item
		PWRREMINDERITEM pwrDispItem;
		switch (nExecEventID)
		{
		case PREVT_AT_SETTIME:
			// If item is set to repeat but not set active in current day of week
			if ((pwrCurItem.IsRepeatEnable() == TRUE) && (!pwrCurItem.IsDayActive((DAYOFWEEK)curSysTime.wDayOfWeek)))
				continue;
			// If set time matching or snooze time is triggered
			if ((CheckTimeMatch(curSysTime, pwrCurItem.stTime)) ||
				(GetPwrReminderSnoozeStatus(pwrCurItem.nItemID, curSysTime))) {
				// Prepare to display
				pwrDispItem.Copy(pwrCurItem);
				SetPwrReminderSnooze(pwrCurItem, FLAG_OFF);
			}
			else continue;
			break;
		case PREVT_AT_SYSWAKEUP:
			// Just prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;
		case PREVT_AT_PWRACTIONWAKE:
			// If Power Action flag is OFF, do not display
			if (GetPwrActionFlag() == FLAG_OFF) continue;
			// Otherwise, prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;
		case PREVT_AT_APPSTARTUP:
		case PREVT_AT_BFRPWRACTION:
		case PREVT_AT_APPEXIT:
			// Just prepare to display
			pwrDispItem.Copy(pwrCurItem);
			break;
		default:
			continue;
			break;
		}

		// Display reminder
		if (!pwrDispItem.IsEmpty()) {

			// Display reminder item
			DisplayPwrReminder(pwrDispItem);

			// If repeat daily option is not enabled
			// --> Disable reminder item after displaying
			if (pwrDispItem.IsRepeatEnable() == FALSE) {
				bTriggerReupdate |= TRUE;
				pwrCurItem.SetEnableState(FALSE);
			}
		}
	}

	// Reupdate flag is triggered
	if (bTriggerReupdate == TRUE) {
		// Reupdate Power Reminder data
		ReupdatePwrReminderData();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayPwrReminder
//	Description:	Display Power Reminder item
//  Arguments:		pwrDispItem - Item to display
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::DisplayPwrReminder(PWRREMINDERITEM& pwrDispItem)
{
	// Check message content validity
	CString strMsgContent = pwrDispItem.strMessage;
	if ((strMsgContent.IsEmpty()) ||
		(strMsgContent == DEF_STRING_NULL)) {
		// Invalid message content
		return DEF_INTEGER_INVALID;
	}

	int nRetFlag = FLAG_OFF;
	int nRespond = DEF_INTEGER_NULL;

	// Style: MessageBox
	if (pwrDispItem.dwStyle == PRSTYLE_MSGBOX) {
		UINT nCaptionID = IDD_PWRREMINDER_DLG;
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		nRespond = DisplayMessageBox(strMsgContent, nCaptionID, dwMsgStyle);
		return nRespond;
	}
	// Style: Dialog
	else if (pwrDispItem.dwStyle == PRSTYLE_DIALOG) {
		// Init reminder message dialog
		CReminderMsgDlg* pMsgDlg = new CReminderMsgDlg(this);
		if (pMsgDlg == NULL) return DEF_INTEGER_INVALID;

		// Message color
		COLORREF clrMsgBkgrd = GetReminderMsgBkgrdColor();
		COLORREF clrMsgText = GetReminderMsgTextColor();

		// Message font info
		CString strFontName;
		BOOL bGetFontName = GetReminderMsgFontName(strFontName);
		int nFontSize = GetReminderMsgFontSize();

		// Message icon info
		int nIconID = GetReminderMsgIconID();
		int nIconSize = GetReminderMsgIconSize();
		BYTE byIconPos = GetReminderMsgIconPosition();

		// Message auto-close interval
		int nTimeout = GetReminderMsgTimeout();

		// Allow snooze mode
		BOOL bAllowSnooze = pwrDispItem.IsAllowSnoozing();

		// Set allow snooze mode
		pMsgDlg->SetAllowSnoozeMode(bAllowSnooze);

		// Set properties
		pMsgDlg->SetLangDlgTitle(IDD_PWRREMINDER_DLG);
		pMsgDlg->SetDispMessage(strMsgContent);
		pMsgDlg->SetBkgrdColor(clrMsgBkgrd);
		pMsgDlg->SetTextColor(clrMsgText);
		pMsgDlg->SetMsgFont(strFontName, nFontSize);
		pMsgDlg->SetMsgIcon(nIconID, nIconSize);
		pMsgDlg->SetMsgIconPosition(byIconPos);
		pMsgDlg->SetAutoCloseInterval(nTimeout);

		// Set notify state flags
		pMsgDlg->SetTopMost(TRUE);
		pMsgDlg->SetInitSound(TRUE);

		// Display message
		nRespond = pMsgDlg->DoModal();

		// Get returned flag
		pMsgDlg->GetSnoozeTriggerFlag(nRetFlag);

		delete pMsgDlg;
	}

	// Update data
	SetPwrReminderSnooze(pwrDispItem, nRetFlag);

	return nRespond;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ReupdatePwrReminderData
//	Description:	Reupdate Power Reminder data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ReupdatePwrReminderData()
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetPwrReminderSnooze
//	Description:	Setup Power Reminder item snooze mode
//  Arguments:		pwrItem		- Power Reminder item
//					nSnoozeFlag - Snooze flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetPwrReminderSnooze(PWRREMINDERITEM pwrItem, int nSnoozeFlag)
{
	// If item is empty, do nothing
	if (pwrItem.IsEmpty()) return;

	// If item event ID is not "At set time", do nothing
	if (pwrItem.nEventID != PREVT_AT_SETTIME)
		return;

	// If snoozing option is not available
	if (pwrItem.IsAllowSnoozing() != TRUE) {
		// Turn off flag
		nSnoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int nInterval = pwrItem.rpsRepeatSet.nSnoozeInterval;

	// Find if item snooze mode is already setup
	for (int nIndex = 0; nIndex < m_arrRmdAdvList.GetSize(); nIndex++) {
		PWRRMDITEMADVSPEC& pAdvItem = m_arrRmdAdvList.GetAt(nIndex);
		if (pAdvItem.nItemID == pwrItem.nItemID) {
			// Update item snooze mode data
			pAdvItem.nSnoozeFlag = nSnoozeFlag;
			if (pAdvItem.nSnoozeFlag == FLAG_ON) {
				// Calculate next snooze trigger time
				pAdvItem.stNextSnoozeTime = GetCurSysTime();
				pAdvItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare item info to add
	PWRRMDITEMADVSPEC pwrAdvItem;
	pwrAdvItem.nItemID = pwrItem.nItemID;
	pwrAdvItem.nSnoozeFlag = nSnoozeFlag;
	if (pwrAdvItem.nSnoozeFlag == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrAdvItem.stNextSnoozeTime = GetCurSysTime();
		pwrAdvItem.CalcNextSnoozeTime(nInterval);
	}

	// Add item
	m_arrRmdAdvList.Add(pwrAdvItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdatePwrReminderSnooze
//	Description:	Update Power Reminder snooze queue data
//  Arguments:		nMode - Update mode flag (UPDATE or DISABLE)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdatePwrReminderSnooze(int nMode)
{
	if (nMode == DEF_MODE_UPDATE) {
		// Update snooze queue items
		for (int nQueueIdx = (m_arrRmdAdvList.GetSize() - 1); nQueueIdx >= 0; nQueueIdx--) {
			// Get item in queue
			PWRRMDITEMADVSPEC& pAdvItem = m_arrRmdAdvList.GetAt(nQueueIdx);

			// Search for item ID in Power Reminder data
			BOOL bItemFound = FALSE;
			for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
				PWRREMINDERITEM& pwrItem = m_prdReminderData.GetItemAt(nItemIdx);
				if (pwrItem.nItemID == pAdvItem.nItemID) {
					// If item's snoozing mode is no longer available
					if (!pwrItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pAdvItem.nSnoozeFlag = FLAG_OFF;
					}

					// Mark as found
					bItemFound = TRUE;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (bItemFound == FALSE) {
				// Remove item from snooze queue
				m_arrRmdAdvList.RemoveAt(nQueueIdx);
			}
		}

		// Free extra memory
		m_arrRmdAdvList.FreeExtra();
	}
	else if (nMode == DEF_MODE_DISABLE) {
		// Disable snooze mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRmdAdvList.GetSize(); nIndex++) {
			// Update item snooze mode data
			PWRRMDITEMADVSPEC& pAdvItem = m_arrRmdAdvList.GetAt(nIndex);
			pAdvItem.nSnoozeFlag = FLAG_OFF;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPwrReminderSnoozeStatus
//	Description:	Get Power Reminder item snooze trigger status
//  Arguments:		nItemID	   - Power Reminder item ID
//					curSysTime - Current system time
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::GetPwrReminderSnoozeStatus(UINT nItemID, SYSTEMTIME& curSysTime)
{
	// If data list is empty, do not trigger
	if (m_arrRmdAdvList.IsEmpty())
		return FALSE;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRmdAdvList.GetSize(); nIndex++) {
		PWRRMDITEMADVSPEC& pAdvItem = m_arrRmdAdvList.GetAt(nIndex);
		if (pAdvItem.nItemID == nItemID) {
			// Get snooze enable flag
			if (pAdvItem.nSnoozeFlag == FLAG_OFF)
				return FALSE;
			
			// Check for next snooze time matching
			if (CheckTimeMatch(curSysTime, pAdvItem.stNextSnoozeTime))
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessDebugCommand
//	Description:	Process app debug commands
//  Arguments:		lpszCommand - Debug command string
//  Return value:	BOOL
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessDebugCommand(LPCTSTR lpszCommand, DWORD& dwErrorCode)
{
	// If debug command is empty, do nothing
	CString strDebugCommand = lpszCommand;
	if (strDebugCommand.IsEmpty()) {
		dwErrorCode = 0x01;
		return FALSE;
	}

	// Get app
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		dwErrorCode = 0x02;
		return FALSE;
	}

	// Format debug command
	strDebugCommand.MakeLower();

	// Debug log output result
	CString strOutputResult;

	// Invalid command flag
	BOOL bInvalidCmdFlag = FALSE;

	/*********************************************************************/
	/*																	 */
	/*					Process basic debug commands				     */
	/*																	 */
	/*********************************************************************/

	if (!_tcscmp(strDebugCommand, _T("exit"))) {
		// Exit app
		PreDestroyDialog();
		EndDialog(IDCANCEL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("taskkill"))) {
		// Exit app (terminate process)
		PostQuitMessage(0);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("reload"))) {
		// Reload settings
		ReloadSettings();
		strOutputResult.Format(_T("Settings reloaded"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("setdefault"))) {
		// Set default settings
		SetDefaultData(&m_cfgTempConfig);
		((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();
		SetupLanguage();
		UpdateDialogData(FALSE);
		strOutputResult.Format(_T("Reset default settings"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("restartapp"))) {
		// Restart app (non-admin)
		RequestRestart(IDD_DEBUGTEST_DLG, FALSE);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("restartadmin"))) {
		// Restart app (as admin)
		RequestRestart(IDD_DEBUGTEST_DLG, TRUE);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("logviewer"))) {
		// Open Logviewer dialog
		OpenChildDialogEx(IDD_LOGVIEWER_DLG);
		strOutputResult.Format(_T("Logviewer opened"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("hksupdate"))) {
		// Update HotkeySet settings
		PostMessage(SM_APP_UPDATE_HOTKEYSETDATA);
		strOutputResult.Format(_T("HotkeySet data updated"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("rmdupdate"))) {
		// Update Power Reminder data
		PostMessage(SM_APP_UPDATE_PWRREMINDERDATA);
		strOutputResult.Format(_T("Power Reminder data updated"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("bakconfig"))) {
		// Backup configuration
		SConfigBackup::AutoRegistryExport();
		strOutputResult.Format(_T("Config backed-up"));
		OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("clrscr"))) {
		// Clear debug view screen buffer
		HWND hDebugTestDlg = FindDebugTestDlg();
		if (hDebugTestDlg != NULL) {
			// Post clear screen message
			::PostMessage(hDebugTestDlg, SM_WND_DEBUGVIEWCLRSCR, NULL, NULL);
		}
		return TRUE;
	}
	else if (!_tcscmp(strDebugCommand, _T("close"))) {
		// Close DebugTest dialog if opening
		HWND hDebugTestWnd = FindDebugTestDlg();
		if (hDebugTestWnd != NULL) {
			// Destroy dialog
			::DestroyWindow(hDebugTestWnd);
		}
		return TRUE;
	}

	/*********************************************************************/
	/*																	 */
	/*			Process advanced/multi-token debug commands				 */
	/*																	 */
	/*********************************************************************/

	// Prepare buffer data
	LPTSTR lpszBuff = new TCHAR[DEF_BUFF_MAXLENGTH];
	// Allocation failed
	if (lpszBuff == NULL) {
		dwErrorCode = 0x03;
		return FALSE;
	}
	_tcscpy(lpszBuff, strDebugCommand.operator LPCTSTR());
	PBUFFER retBuff = new BUFFER[DEF_TOKEN_MAXCOUNT];
	// Allocation failed
	if (retBuff == NULL) {
		// Clean-up buffer data
		dwErrorCode = 0x04;
		delete[] lpszBuff;
		return FALSE;
	}

	// Make token list (break string buffer)
	int nCount = GetTokenList(lpszBuff, retBuff, _T(" =,;:-_"));

	// Validate number of tokens
	if (nCount <= 0) {
		// Clean-up buffer data
		dwErrorCode = 0x05;
		delete[] lpszBuff;
		delete[] retBuff;
		return FALSE;
	}

	int nTemp = DEF_INTEGER_INVALID;
	CString strTemp = DEF_STRING_EMPTY;

	// Process debug commands by tokens
	if (!_tcscmp(retBuff[0].tcToken, _T("test"))) {
		// Debug command token test
		if (nCount > 1) {
			// Print token list
			int nTokenCount = (nCount - 1);
			OutputDebugLogFormat(_T("Token number: %d"), nTokenCount);
			for (int nTokenIndex = 0; nTokenIndex < nTokenCount; nTokenIndex++) {
				strTemp = retBuff[nTokenIndex + 1].tcToken;
				OutputDebugLogFormat(_T("Token[%d]: %s"), nTokenIndex, strTemp);
			}
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("dmytest"))) {
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetDummyTestMode(TRUE);
			OutputDebugLog(_T("DummyTest ON"));
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetDummyTestMode(FALSE);
			OutputDebugLog(_T("DummyTest OFF"));
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
			SetDummyTestMode(DEFAULT_DUMMYTEST);
			OutputDebugLog(_T("DummyTest reset"));
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("dmyteststs"))) {
		// Get dummy test mode
		int nRet = GetDummyTestMode();
		OutputDebugLogFormat(_T("DummyTest status = %d"), nRet);
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debugmode"))) {
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetDebugMode(TRUE);
			OutputDebugLog(_T("DebugMode ON"));
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetDebugMode(FALSE);
			OutputDebugLog(_T("DebugMode OFF"));
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
			SetDebugMode(DEFAULT_DEBUGMODE);
			OutputDebugLog(_T("DebugMode reset"));
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debugmodests"))) {
		// Get debug mode
		int nRet = GetDebugMode();
		OutputDebugLogFormat(_T("DebugMode status = %d"), nRet);
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("debuglog"))) {
		if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("style")))) {
			if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("default")))) {
				SetDebugLogStyle(DBLOG_OUTPUTDBSTRING);
				OutputDebugLog(_T("Debug log style changed"));
			}
			else if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("tofile")))) {
				SetDebugLogStyle(DBLOG_OUTPUTTOFILE);
				OutputDebugLog(_T("Debug log style changed"));
			}
			else if ((nCount >= 3) && (!_tcscmp(retBuff[2].tcToken, _T("todbtool")))) {
				SetDebugLogStyle(DBLOG_OUTPUTTODBTOOL);
				OutputDebugLog(_T("Debug log style changed"));
			}
			else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("reset")))) {
				SetDebugLogStyle(DEFAULT_DEBUGLOGSTYLE);
				OutputDebugLog(_T("Debug log style reset"));
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount >= 2) && (!_tcscmp(retBuff[1].tcToken, _T("getstyle")))) {
			// Get debug log style
			int nRet = GetDebugLogStyle();
			OutputDebugLogFormat(_T("DebugLogStyle = %d"), nRet);
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("trclogtest"))) {
		if (nCount == 2) {
			int nVal = _tstoi(retBuff[1].tcToken);
			if (nVal >= 1) {
				for (int i = 0; i < nVal; i++) {
					// Write test trace log
					TraceLogFormat("TraceLog Test (%d)", i);
					Sleep(50); // wait for a blink
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("errtest"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("code")))) {
			// Test show error message
			int nErrCode = _tstoi(retBuff[2].tcToken);
			if (nErrCode >= 0) {
				if (GetAppOption(OPTIONID_SHOWERRORMSG) == FALSE) {
					OutputDebugLog(_T("Show error message OFF"));
				}
				else {
					// Show error message
					ShowErrorMessage(nErrCode);
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("appeventlog"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("clear")))) {
			// Clear all app event log data in memory
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->GetAppEventLog();
			if (pAppEventLog != NULL) {
				pAppEventLog->Init();
				OutputDebugLog(_T("App event log data cleared"));
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("write")))) {
			// Force writing app event log data from memory to file
			SLogging* pAppEventLog = ((CPowerPlusApp*)AfxGetApp())->GetAppEventLog();
			if (pAppEventLog != NULL) {
				BOOL bRet = pAppEventLog->WriteLog();
				if (bRet == TRUE) {
					// Write log succeeded
					OutputDebugLog(_T("App event log data written"));
				}
				else {
					// Write log failed
					OutputDebugLog(_T("Write app event log data failed"));
				}
			}
		}
		else if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("del")))) {
			// Delete app event log files
			CString strKeyName = retBuff[2].tcToken;;
			if (!_tcscmp(retBuff[2].tcToken, _T("all"))) {
				strKeyName = _T("AppEventLog");
			}
			CFileFind Finder;
			CString strFileName = (CString)DIR_SUBDIR_LOG + DEF_PATH_SEPARATOR + _T("*.log");
			BOOL bFindRet = Finder.FindFile(strFileName);
			if (!strKeyName.IsEmpty()) {
				int nDelFileCount = 0;
				while (bFindRet == TRUE) {
					bFindRet = Finder.FindNextFile();
					strFileName = Finder.GetFileName();
					if (strFileName.Find(strKeyName) != -1) {
						// Delete file
						CFile::Remove(Finder.GetFilePath());
						nDelFileCount++;	// Increase counter
					}
				}
				if (nDelFileCount > 0) {
					// Output number of deleted files
					strOutputResult.Format(_T("App event log file(s) deleted (Count=%d)"), nDelFileCount);
					OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("reminderdisp"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("id")))) {
			int nItemID = _tstoi(retBuff[2].tcToken);
			// Find and display reminder item by ID
			BOOL bFindRet = FALSE;
			PWRREMINDERITEM pwrTemp;
			int nItemNum = m_prdReminderData.GetItemNum();
			for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
				pwrTemp = m_prdReminderData.GetItemAt(nIndex);
				if (pwrTemp.nItemID == nItemID) {
					bFindRet = TRUE;
					DisplayPwrReminder(pwrTemp);
					break;
				}
			}
			if (bFindRet == FALSE) {
				// Item not found
				OutputDebugLog(_T("Reminder item not found"));
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("saveglobalvars"))) {
		if (nCount == 1) {
			// Save all global variables
			pApp->SaveGlobalVars(0xFF);
			strOutputResult.Format(_T("Global variables stored"));
			OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
		}
		else if (nCount == 2) {
			if (!_tcscmp(retBuff[1].tcToken, _T("all"))) {
				// Save all global variables
				pApp->SaveGlobalVars(0xFF);
				strOutputResult.Format(_T("Global variables stored"));
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("dbtest"))) {
				// Save debugging/testing config (global variables)
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_DEBUGTEST);
				strOutputResult.Format(_T("Debug/test config stored"));
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("appflags"))) {
				// Save app flags (global variables)
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
				strOutputResult.Format(_T("Global app flags stored"));
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
			}
			else if (!_tcscmp(retBuff[1].tcToken, _T("special"))) {
				// Save special variables (global variables)
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				strOutputResult.Format(_T("Global special variables stored"));
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("print"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("config")))) {
			// Print app config data
			PCONFIGDATA pcfgDataTemp = pApp->GetAppConfigData();
			if (pcfgDataTemp != NULL) {
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Format and print data
				CString strKeyName = DEF_STRING_EMPTY;
				CString strValue = DEF_STRING_EMPTY;
				// Left mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONLMB);
				int nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nLMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Middle mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONMMB);
				nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nMMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ACTIONRMB);
				nActionStringID = GetPairedID(idplActionName, pcfgDataTemp->nRMBAction);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, GetLanguageString(ptrLanguage, nActionStringID));
				// Right mouse button: Only show menu
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_RMBSHOWMENU);
				strValue = ((pcfgDataTemp->bRMBShowMenu) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Language setting
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_LANGUAGEID);
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, CA2W(GetLanguageName(pcfgDataTemp->nLanguageID)).m_psz);
				// Show dialog at startup
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SHOWATSTARTUP);
				strValue = ((pcfgDataTemp->bShowDlgAtStartup) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Startup with Windows
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_STARTUPENABLE);
				strValue = ((pcfgDataTemp->bStartupEnabled) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show confirm message before executing action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_CONFIRMACTION);
				strValue = ((pcfgDataTemp->bConfirmAction) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Save action log
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SAVEACTIONLOG);
				strValue = ((pcfgDataTemp->bSaveActionLog) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Save app event log
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SAVEAPPEVENTLOG);
				strValue = ((pcfgDataTemp->bSaveAppEventLog) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Run with admin privileges
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_RUNASADMIN);
				strValue = ((pcfgDataTemp->bRunAsAdmin) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show action error message
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SHOWERROR);
				strValue = ((pcfgDataTemp->bShowErrorMsg) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Show notify tip for schedule action
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SCHEDULENOTIFY);
				strValue = ((pcfgDataTemp->bNotifySchedule) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Allow canceling schedule when notify
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_SCHEDALLOWCANCEL);
				strValue = ((pcfgDataTemp->bAllowCancelSchedule) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Enable background action hotkeys
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ENBBKGRDHOTKEYS);
				strValue = ((pcfgDataTemp->bEnableBackgroundHotkey) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
				// Enable Power Peminder feature
				LoadResourceString(strKeyName, IDS_REGKEY_CFG_ENBPWRREMINDER);
				strValue = ((pcfgDataTemp->bEnablePowerReminder) ? _T("YES") : _T("NO"));
				OutputDebugLogFormat(_T("%s=%s"), strKeyName, strValue);
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("schedule")))) {
			// Print schedule data
			PSCHEDULEDATA pSchedDataTemp = pApp->GetAppScheduleData();
			if (pSchedDataTemp != NULL) {
				// Print default schedule
				CString strDefItemPrint;
				pSchedDataTemp->GetDefaultItem().Print(strDefItemPrint);
				strOutputResult.Format(_T("DefaultSchedule: %s"), strDefItemPrint);
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				// Print extra item number
				int nExtraItemNum = pSchedDataTemp->GetExtraItemNum();
				strOutputResult.Format(_T("ScheduleExtraData: ItemNum = %d"), nExtraItemNum);
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				// Print each item data
				for (int nExtraIndex = 0; nExtraIndex < nExtraItemNum; nExtraIndex++) {
					SCHEDULEITEM schExtraItem = pSchedDataTemp->GetItemAt(nExtraIndex);

					// Print item
					CString strItemPrint;
					schExtraItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nExtraIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				}
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hksetdata")))) {
			// Print HotkeySet data
			PHOTKEYSETDATA pHksDataTemp = pApp->GetAppHotkeySetData();
			if (pHksDataTemp != NULL) {
				// Print item number
				int nItemNum = pHksDataTemp->GetItemNum();
				strOutputResult.Format(_T("HotkeySetData: ItemNum = %d"), nItemNum);
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					HOTKEYSETITEM hksItem = pHksDataTemp->GetItemAt(nIndex);

					// Print item
					CString strItemPrint;
					hksItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				}
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("rmddata")))) {
			// Print Power Reminder data
			PPWRREMINDERDATA pRmdDataTemp = pApp->GetAppPwrReminderData();
			if (pRmdDataTemp != NULL) {
				// Print item number
				int nItemNum = pRmdDataTemp->GetItemNum();
				strOutputResult.Format(_T("PwrReminderData: ItemNum = %d"), nItemNum);
				OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				// Load app language package
				LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();
				// Print each item data
				for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
					PWRREMINDERITEM pwrItem = pRmdDataTemp->GetItemAt(nIndex);

					// Print item
					CString strItemPrint;
					pwrItem.Print(strItemPrint);
					strOutputResult.Format(_T("Index=%d, %s"), nIndex, strItemPrint);
					OutputDebugLog(strOutputResult, DBLOG_OUTPUTTODBTOOL);
				}
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgset"))) {
		if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			if (nCount == 3) {
				// Set message background color by name
				CString strColorName = retBuff[2].tcToken;
				DWORD dwRetColorID = GetStringID(strplColorName, strColorName);
				if (dwRetColorID != DEF_INTEGER_INVALID) {
					// Set background color
					SetReminderMsgBkgrdColor(dwRetColorID);
					pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
					OutputDebugLogFormat(_T("Message background color set: %s"), strColorName.MakeUpper());
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
				}
			}
			else if (((nCount > 3) && (nCount <= 6)) && (!_tcscmp(retBuff[2].tcToken, _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (nCount >= 4) ? _tstoi(retBuff[3].tcToken) : 0;	// Red
				int nGValue = (nCount >= 5) ? _tstoi(retBuff[4].tcToken) : 0;	// Green
				int nBValue = (nCount >= 6) ? _tstoi(retBuff[5].tcToken) : 0;	// Blue
				if (((nRValue < 0) || (nRValue > 255)) || ((nGValue < 0) || (nGValue > 255)) || ((nBValue < 0) || (nBValue > 255))) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
				}
				else {
					// Set background color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					SetReminderMsgBkgrdColor(clrRGB);
					pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
					OutputDebugLogFormat(_T("Message background color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			if (nCount == 3) {
				// Set message text color by name
				CString strColorName = retBuff[2].tcToken;
				DWORD dwRetColorID = GetStringID(strplColorName, strColorName);
				if (dwRetColorID != DEF_INTEGER_INVALID) {
					// Set text color
					SetReminderMsgTextColor(dwRetColorID);
					pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
					OutputDebugLogFormat(_T("Message text color set: %s"), strColorName.MakeUpper());
				}
				else {
					// Invalid command
					bInvalidCmdFlag = TRUE;
				}
			}
			else if (((nCount > 3) && (nCount <= 6)) && (!_tcscmp(retBuff[2].tcToken, _T("rgb")))) {
				// Set message background color by RGB value
				int nRValue = (nCount >= 4) ? _tstoi(retBuff[3].tcToken) : 0;	// Red
				int nGValue = (nCount >= 5) ? _tstoi(retBuff[4].tcToken) : 0;	// Green
				int nBValue = (nCount >= 6) ? _tstoi(retBuff[5].tcToken) : 0;	// Blue
				if (((nRValue < 0) || (nRValue > 255)) || ((nGValue < 0) || (nGValue > 255)) || ((nBValue < 0) || (nBValue > 255))) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 0 -> 255)"));
				}
				else {
					// Set text color
					COLORREF clrRGB = RGB(nRValue, nGValue, nBValue);
					SetReminderMsgTextColor(clrRGB);
					pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
					OutputDebugLogFormat(_T("Message text color set: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
				}
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Set reminder message font name
			CString strFontName = retBuff[2].tcToken;
			// Because the input debug command had been converted to lowercase,
			// it needs to be re-formatted by capitalizing first character of each word
			UpperEachWord(strFontName, TRUE);
			// Validate font name
			BOOL bRet = ValidateFontName(strFontName);
			if (bRet != TRUE) {
				// Invalid font name
				OutputDebugLog(_T("Invalid font name"));
			}
			else {
				// Set message font
				SetReminderMsgFontName(strFontName);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message font name set: %s"), strFontName);
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Set reminder message font size
			int nFontSize = _tstoi(retBuff[2].tcToken);
			if ((nFontSize < 10) || (nFontSize > 100)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 100)"));
			}
			else {
				// Set font size
				SetReminderMsgFontSize(nFontSize);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message font size set: %dpt"), nFontSize);
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Set reminder message auto-close interval (timeout)
			int nTimeout = _tstoi(retBuff[2].tcToken);
			if ((nTimeout < 10) || (nTimeout > 1800)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 1800)"));
			}
			else {
				// Set timeout
				SetReminderMsgTimeout(nTimeout);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message time-out set: %ds"), nTimeout);
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("notimeout")))) {
			// No reminder message timeout (default 0)
			SetReminderMsgTimeout(DEF_INTEGER_NULL);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message time-out disabled"));
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Set message icon ID by name
			CString strIconName = retBuff[2].tcToken;
			DWORD dwRetIconID = GetStringID(strplMsgIconName, strIconName);
			if (dwRetIconID != DEF_INTEGER_INVALID) {
				// Set icon ID
				SetReminderMsgIconID(dwRetIconID);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message icon ID set: %s (%d)"), strIconName.MakeUpper(), dwRetIconID);
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("noicon")))) {
			// No reminder message icon (default 0)
			SetReminderMsgIconID(IDI_MSGICON_NOICON);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message icon disabled"));
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Set reminder message icon size
			int nIconSize = _tstoi(retBuff[2].tcToken);
			if ((nIconSize < 30) || (nIconSize > 100)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 30 -> 100)"));
			}
			else {
				// Set icon size
				SetReminderMsgIconSize(nIconSize);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message icon size set: %dx%dpx"), nIconSize, nIconSize);
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Set reminder message icon position
			CString strIconPos = retBuff[2].tcToken;
			if (!_tcscmp(strIconPos, _T("left"))) {
				// Set icon position: Left
				SetReminderMsgIconPosition(MSGICONPOS_ONLEFT);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLog(_T("Message icon position set: Left"));
			}
			else if (!_tcscmp(strIconPos, _T("top"))) {
				// Set icon position: Top
				SetReminderMsgIconPosition(MSGICONPOS_ONTOP);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLog(_T("Message icon position set: Top"));
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Set reminder message horizontal margin
			int nHMargin = _tstoi(retBuff[2].tcToken);
			if ((nHMargin < 10) || (nHMargin > 120)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
			}
			else {
				// Set margin
				SetReminderMsgHMargin(nHMargin);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message horizontal margin set: %dpx"), nHMargin);
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
			// Set reminder message vertical margin
			int nVMargin = _tstoi(retBuff[2].tcToken);
			if ((nVMargin < 10) || (nVMargin > 120)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 10 -> 120)"));
			}
			else {
				// Set margin
				SetReminderMsgVMargin(nVMargin);
				pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
				OutputDebugLogFormat(_T("Message vertical margin set: %dpx"), nVMargin);
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgreset"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			// Reset message background color
			SetReminderMsgBkgrdColor(DEFAULT_MSGBKGRDCLR);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message background color reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			// Set message text color by name
			SetReminderMsgTextColor(DEFAULT_MSGTEXTCLR);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message text color reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Set reminder message font name
			SetReminderMsgFontName(DEFAULT_MSGFONTNAME);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message font name reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Set reminder message font size
			SetReminderMsgFontSize(DEFAULT_MSGFONTSIZE);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message font size reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Reset reminder message auto-close interval (time-out)
			SetReminderMsgTimeout(DEFAULT_MSGTIMEOUT);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message time-out reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Reset reminder message icon ID
			SetReminderMsgIconID(DEFAULT_MSGICONID);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message icon ID reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Reset reminder message icon size
			SetReminderMsgIconSize(DEFAULT_MSGICONSIZE);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message icon size reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Reset reminder message icon position
			SetReminderMsgIconPosition(DEFAULT_MSGICONPOS);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLog(_T("Message icon position reset"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Reset reminder message horizontal margin
			SetReminderMsgHMargin(DEFAULT_MSGHMARGIN);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message horizontal margin reset)"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
			// Reset reminder message vertical margin
			SetReminderMsgVMargin(DEFAULT_MSGVMARGIN);
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
			OutputDebugLogFormat(_T("Message vertical margin reset"));
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdmsgget"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("bkgclr")))) {
			// Get reminder message background color
			DWORD dwBkgrdColor = GetReminderMsgBkgrdColor();
			int nBValue = (dwBkgrdColor & 0x00FF0000) >> 16;	// Blue
			int nGValue = (dwBkgrdColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwBkgrdColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message background color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("txtclr")))) {
			// Get reminder message text color
			DWORD dwTextColor = GetReminderMsgTextColor();
			int nBValue = (dwTextColor & 0x00FF0000) >> 16;		// Blue
			int nGValue = (dwTextColor & 0x0000FF00) >> 8;		// Green
			int nRValue = (dwTextColor & 0x000000FF);			// Red
			OutputDebugLogFormat(_T("Message text color: RGB(%d,%d,%d)"), nRValue, nGValue, nBValue);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontname")))) {
			// Get reminder message font name
			CString strFontName;
			BOOL bRet = GetReminderMsgFontName(strFontName);
			OutputDebugLogFormat(_T("Message font name: %s"),strFontName);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontsize")))) {
			// Get reminder message font size
			int nFontSize = GetReminderMsgFontSize();
			OutputDebugLogFormat(_T("Message font size: %dpt"), nFontSize);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("timeout")))) {
			// Get reminder message auto-close interval (time-out)
			int nTimeout = GetReminderMsgTimeout();
			OutputDebugLogFormat(_T("Message time-out: %ds"), nTimeout);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconid")))) {
			// Get reminder message icon ID
			UINT nIconID = GetReminderMsgIconID();
			OutputDebugLogFormat(_T("Message icon ID: %d"), nIconID);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconsize")))) {
			// Get reminder message icon size
			int nIconSize = GetReminderMsgIconSize();
			OutputDebugLogFormat(_T("Message icon size: %dx%dpx"), nIconSize, nIconSize);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("iconpos")))) {
			// Get reminder message icon position
			BYTE byIconPosition = GetReminderMsgIconPosition();
			if (byIconPosition == MSGICONPOS_ONLEFT) {
				OutputDebugLog(_T("Message icon position: Left"));
			}
			else if (byIconPosition == MSGICONPOS_ONTOP) {
				OutputDebugLog(_T("Message icon position: Top"));
			}
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("hmargin")))) {
			// Get reminder message horizontal margin
			int nHMargin = GetReminderMsgHMargin();
			OutputDebugLogFormat(_T("Message horizontal margin: %dpx"), nHMargin);
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("vmargin")))) {
			// Get reminder message vertical margin
			int nVMargin = GetReminderMsgVMargin();
			OutputDebugLogFormat(_T("Message vertical margin: %dpx"), nVMargin);
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("rmdsnooze"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("prntqueue")))) {
			// Print Power Reminder snooze queue list
			if (m_arrRmdAdvList.IsEmpty()) {
				// Empty list
				OutputDebugLog(_T("Reminder snooze queue empty!"));
			}
			else {
				// Print each item
				PWRRMDITEMADVSPEC pwrAdvTemp;
				for (int nIndex = 0; nIndex < m_arrRmdAdvList.GetSize(); nIndex++) {
					pwrAdvTemp = m_arrRmdAdvList.GetAt(nIndex);
					OutputDebugLogFormat(_T("Item%03d: ID=%d, Snooze=%d, NextTrigger=%02d:%02d"), nIndex, pwrAdvTemp.nItemID,
						pwrAdvTemp.nSnoozeFlag, pwrAdvTemp.stNextSnoozeTime.wHour, pwrAdvTemp.stNextSnoozeTime.wMinute);
				}
			}
		}
		else if ((nCount >= 3) && (!_tcscmp(retBuff[1].tcToken, _T("interval")))) {
			if ((nCount == 4) && (!_tcscmp(retBuff[2].tcToken, _T("set")))) {
				// Set reminder message snooze interval
				int nInputInterval = _tstoi(retBuff[3].tcToken);
				if ((nInputInterval < 1) || (nInputInterval > 60)) {
					// Invalid argument
					OutputDebugLog(_T("Invalid value (Value range: 1 -> 60)"));
				}
				else {
					// Set snooze interval
					int nSnoozeInterval = nInputInterval * 60;
					SetReminderMsgSnoozeInterval(nSnoozeInterval);
					pApp->SaveGlobalVars(DEF_GLBVAR_CATE_SPECIAL);
					OutputDebugLogFormat(_T("Message snooze interval set: %ds"), nSnoozeInterval);
				}
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("get")))) {
				// Get reminder message snooze interval
				int nSnoozeInterval = GetReminderMsgSnoozeInterval();
				OutputDebugLogFormat(_T("Message snooze interval: %ds"), nSnoozeInterval);
			}
			else if ((nCount == 3) && (!_tcscmp(retBuff[2].tcToken, _T("reset")))) {
				// Reset reminder message snooze interval
				SetReminderMsgSnoozeInterval(DEFAULT_SNOOZETIME);
				OutputDebugLog(_T("Message snooze interval reset"));
			}
			else {
				// Invalid command
				bInvalidCmdFlag = TRUE;
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("upper"))) {
		if ((nCount > 2) && (!_tcscmp(retBuff[1].tcToken, _T("string")))) {
			// Upper each word
			CString strInput = retBuff[2].tcToken;
			UpperEachWord(strInput, TRUE);
			OutputDebugLogFormat(_T("Upper: %s"), strInput);
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("enum"))) {
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("fontnames")))) {
			// Enumerate all currently available fonts
			std::vector<std::wstring> fontNames;
			BOOL bRet = EnumFontNames(fontNames);
			if (bRet == FALSE) {
				// Enumerate fonts failed
				OutputDebugLog(_T("Enumerate fonts failed"));
			}
			else {
				// Print all font names
				BeginWaitCursor();
				{
					for (auto it = fontNames.begin(); it != fontNames.end(); it++) {
						OutputDebugLogFormat(_T("Font [%d]: %s"), (it - fontNames.begin()), (*it).c_str());
						Sleep(50);  // wait for a blink
					}
				}
				EndWaitCursor();
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("getlastsysevttime"))) {
		// Get last system event time
		SYSTEMTIME stTimeTemp;
		CString strDateTimeFormat;
		strDateTimeFormat.LoadString(IDS_FORMAT_FULLDATETIME);
		// Get last system suspend time
		if (pApp->GetLastSysEventTime(SYSEVT_SUSPEND, stTimeTemp)) {
			// Format date time
			CString strLogTemp;
			CString strMiddayFlag = (stTimeTemp.wHour >= 12) ? _T("PM") : _T("AM");
			strLogTemp.Format(strDateTimeFormat, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay,
				stTimeTemp.wHour, stTimeTemp.wMinute, stTimeTemp.wSecond, stTimeTemp.wMilliseconds, strMiddayFlag);
			strOutputResult.Format(_T("Last System Suspend: %s"), strLogTemp);
			OutputDebugLog(strOutputResult);
		}
		else {
			// Get last system suspend time failed
			OutputDebugLog(_T("Get last system suspend time failed"));
		}
		// Get last system wakeup time
		if (pApp->GetLastSysEventTime(SYSEVT_WAKEUP, stTimeTemp)) {
			// Format date time
			CString strLogTemp;
			CString strMiddayFlag = (stTimeTemp.wHour >= 12) ? _T("PM") : _T("AM");
			strLogTemp.Format(strDateTimeFormat, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay,
				stTimeTemp.wHour, stTimeTemp.wMinute, stTimeTemp.wSecond, stTimeTemp.wMilliseconds, strMiddayFlag);
			strOutputResult.Format(_T("Last System Wakeup: %s"), strLogTemp);
			OutputDebugLog(strOutputResult);
		}
		else {
			// Get last system wakeup time failed
			OutputDebugLog(_T("Get last system wakeup time failed"));
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("powerbroadcast"))) {
		if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("param")))) {
			// Simulate system power event (WM_POWERBROADCAST message)
			int nParam = _tstoi(retBuff[2].tcToken);
			if ((nParam == PBT_APMPOWERSTATUSCHANGE) ||
				(nParam == PBT_APMRESUMEAUTOMATIC) ||
				(nParam == PBT_APMRESUMESUSPEND) ||
				(nParam == PBT_APMSUSPEND) ||
				(nParam == PBT_POWERSETTINGCHANGE)) {
				// Post message
				PostMessage(WM_POWERBROADCAST, (WPARAM)nParam, NULL);
				OutputDebugLogFormat(_T("Message posted: Param=%d"), nParam);
			}
			else {
				// Invalid param
				OutputDebugLog(_T("Invalid parameter"));
			}
		}
		else if ((nCount == 3) && (!_tcscmp(retBuff[1].tcToken, _T("skip")))) {
			// Skip system power event (WM_POWERBROADCAST message)
			int nSkipCount = _tstoi(retBuff[2].tcToken);
			if ((nSkipCount <= 0) || (nSkipCount > 3600)) {
				// Invalid argument
				OutputDebugLog(_T("Invalid value (Value range: 1 -> 3600)"));
			}
			else {
				// Set skip counter
				SetFlagValue(FLAGID_PWRBROADCASTSKIPCOUNT, nSkipCount);
				OutputDebugLogFormat(_T("Skip PowerBroadcast event: Counter=%d"), nSkipCount);
			}
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else if (!_tcscmp(retBuff[0].tcToken, _T("pwractionflag"))) {
		// Turn ON/OFF power action flag
		if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("on")))) {
			SetPwrActionFlag(FLAG_ON);
			OutputDebugLog(_T("Power action flag: ON"));
		}
		else if ((nCount == 2) && (!_tcscmp(retBuff[1].tcToken, _T("off")))) {
			SetPwrActionFlag(FLAG_OFF);
			OutputDebugLog(_T("Power action flag: OFF"));
		}
		else {
			// Invalid command
			bInvalidCmdFlag = TRUE;
		}
	}
	else {
		// Invalid command
		bInvalidCmdFlag = TRUE;
	}

	// If command is invalid
	if (bInvalidCmdFlag == TRUE) {
		OutputDebugLog(_T("Invalid command"));
	}

	// Clean-up
	delete[] lpszBuff;
	delete[] retBuff;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
// Logging and message functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveActionHistory
//	Description:	Save action history log to log list data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SaveActionHistory(void)
{
	// If action info data is empty (init state), do nothing
	if (m_actActionData.bInitState == TRUE)
		return;

	// Prepare log info
	LOGITEM actionLogItem;
	actionLogItem.byType = 0;
	actionLogItem.stTime = m_actActionData.stActionTime;
	LoadResourceString(actionLogItem.strLogString, m_actActionData.nActionNameID);

	// Action result
	int nTemp;
	CString strTemp;
	switch (m_actActionData.nErrorCode)
	{
	case DEF_APP_ERROR_SUCCESS:
		strTemp.LoadString(IDS_ACTIONLOG_ERR_SUCCESS);
		actionLogItem.strDetails = strTemp;
		break;
	case DEF_APP_ERROR_FAILED:
	case DEF_APP_ERROR_UNKNOWN:
		strTemp.LoadString(IDS_ACTIONLOG_ERR_FAILED_UNKNOWN);
		actionLogItem.strDetails = strTemp;
		break;
	default:
		nTemp = m_actActionData.nErrorCode;
		strTemp.Format(IDS_ACTIONLOG_ERR_FAILED_ERRCODE, nTemp);
		actionLogItem.strDetails = strTemp;
		break;
	}

	// Output action log if enabled
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		BOOL bEnable = pApp->GetActionLogOption();
		if (bEnable != FALSE) {
			pApp->OutputActionLog(actionLogItem);
		}
	}

	// Empty action data after output
	m_actActionData.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ConfirmAction
//	Description:	Show confirmation message before doing action
//  Arguments:		nActionType - Type of action
//					nActionID	- ID of action
//  Return value:	int	- Result of confirmation message
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::ConfirmAction(UINT nActionType, UINT nActionID)
{
	// If "Confirm action" option is not enabled, return YES
	BOOL bConfirmAction = GetAppOption(OPTIONID_CONFIRMACTION);
	if (bConfirmAction == FALSE)
		return IDYES;

	// Allowed action ID --> Bypass confirmation
	if (nActionID == DEF_APP_ACTION_DISPLAYOFF)
		return IDYES;

	// Allowed action type --> Bypass confirmation
	if ((nActionType == DEF_APP_MACRO_ACTION_SCHEDULE) ||
		(nActionType == DEF_APP_MACRO_ACTION_HOTKEY) ||
		(nActionType == DEF_APP_MACRO_ACTION_MENU))
		return IDYES;

	// Display confirmation message
	UINT nStringID = GetPairedID(idplActionMsg, nActionID);
	int nResult = DisplayMessageBox(nStringID, (UINT)NULL, MB_YESNO | MB_ICONQUESTION);

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	NotifySchedule
//	Description:	Notify schedule if enabled
//  Arguments:		pschItem  - Schedule item to notify (pointer)
//					bReupdate - Trigger reupdate flag (out)
//  Return value:	int - Result of notify message
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::NotifySchedule(PSCHEDULEITEM pschItem, BOOL& bReupdate)
{
	// Get action info
	UINT nActionStringID = GetPairedID(idplSchedNotifyMsg, pschItem->nAction);

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Format message
	CString strCaption = GetLanguageString(pAppLang, IDD_MULTISCHEDULE_DLG);
	CString strAction = GetLanguageString(pAppLang, nActionStringID);
	CString strMsgTemp = GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_NOTIFY);
	
	CString strMsg;
	strMsg.Format(strMsgTemp, strAction);

	// Allow cancelling schedule when notify
	BOOL bAllowCancel = GetAppOption(OPTIONID_ALLOWCANCELSCHED);
	if (bAllowCancel == TRUE)
	{
		// Update message content
		strMsg += GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_ALLOWCANCEL);
		int nRespond = DisplayMessageBox(strMsg, strCaption, MB_OKCANCEL | MB_ICONINFORMATION);
		if (nRespond == IDCANCEL)
		{
			// If "Repeat" option is not ON,
			// --> Disable schedule item after canceling
			if (pschItem->bRepeat == FALSE) {
				// Deactivate schedule
				pschItem->SetActiveState(FALSE);
				bReupdate = TRUE;
			}

			// Output event log
			OutputAppEventLog("Schedule has been canceled");
			DisplayMessageBox(MSGBOX_PROCESSSCHEDULE_CANCELED, IDD_MULTISCHEDULE_DLG, MB_OK | MB_ICONINFORMATION);
			return nRespond;
		}
	}

	// Show message
	return DisplayMessageBox(strMsg, strCaption, MB_OK | MB_ICONINFORMATION);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowErrorMessage
//	Description:	Show error message if enabled
//  Arguments:		dwError - Error code
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ShowErrorMessage(DWORD dwError)
{
	// If option is not enabled, do nothing
	BOOL bShowErrMsg = GetAppOption(OPTIONID_SHOWERRORMSG);
	if (bShowErrMsg == FALSE)
		return;

	// Get window handle and language ID
	HWND hWnd = this->GetSafeHwnd();
	UINT nCurLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguageOption();

	// Show error message
	CoreFuncs::ShowErrorMessage(hWnd, nCurLang, dwError);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestRestart
//	Description:	Request to restart app
//	Arguments:		uiCmdSenderID	- ID of command which sends request
//					bRestartAsAdmin - Restart as admin???
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RequestRestart(UINT uiCmdSenderID, BOOL bRestartAsAdmin)
{
	// Init request data
	RESTARTREQ reqRestart;
	ZeroMemory(&reqRestart, sizeof(RESTARTREQ));

	// Request from [Apply] button
	if (uiCmdSenderID == IDC_APPLY_BTN) {
		BOOL bRestartTrigger = GetFlagValue(FLAGID_RESTARTASADMIN);
		reqRestart.bRequest = bRestartTrigger;
		reqRestart.bAdminCheck = TRUE;
		reqRestart.bNotAdminShowMsg = TRUE;
		reqRestart.bIsAdminDoNothing = TRUE;
		reqRestart.bShowMsgWhenDeny = TRUE;
		reqRestart.bResetFlag = TRUE;
	}
	// Request from [Restart with admin priviledges] menu
	else if (uiCmdSenderID == IDM_NOTIFY_RESTART_ASADMIN) {
		reqRestart.bRequest = TRUE;
		reqRestart.bAdminCheck = TRUE;
		reqRestart.bNotAdminShowMsg = FALSE;
		reqRestart.bIsAdminDoNothing = TRUE;
		reqRestart.bShowMsgWhenDeny = FALSE;
		reqRestart.bResetFlag = FALSE;
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
		RestartApp(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestRestartAsAdmin
//	Description:	Request to restart app as admin
//	Arguments:		reqRestart - Request to restart
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RequestRestartAsAdmin(RESTARTREQ reqRestart)
{
	// If there's no request, do nothing
	if (reqRestart.bRequest == FALSE)
		return;

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strRequestMsg = GetLanguageString(pAppLang, MSGBOX_OTHER_REQUEST_RESTARTASADMIN);
	CString strCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowTitle();
	CString strMsgFormat = strRequestMsg;

	BOOL bIsAdmin = FALSE;
	if (reqRestart.bAdminCheck == TRUE) {
		// Check if the app is currently running as admin
		bIsAdmin = IsUserAnAdmin();
		if (bIsAdmin == TRUE) {
			// If already running as admin, do nothing
			if (reqRestart.bIsAdminDoNothing == TRUE) {
				if (reqRestart.bShowMsgWhenDeny == FALSE) {
					DisplayMessageBox(MSGBOX_OTHER_ALREADY_ADMIN, (UINT)NULL, MB_OK | MB_ICONINFORMATION);
				}
				return;
			}
		}
		else if (bIsAdmin == FALSE) {
			// Show "not admin" message
			if (reqRestart.bNotAdminShowMsg == TRUE) {
				CString strNotAdmin = GetLanguageString(pAppLang, MSGBOX_OTHER_NOTRUNASADMIN);
				strMsgFormat.Format(_T("%s\n%s"), strNotAdmin, strRequestMsg);
			}
		}
	}
	
	// Display request message
	int nRet = DisplayMessageBox(strMsgFormat, strCaption, MB_YESNO | MB_ICONQUESTION);
	if (nRet == IDYES) {
		// Restart as admin
		RestartApp(TRUE);
	}
	else if (nRet == IDNO) {
		// Run as admin next time
		if (reqRestart.bShowMsgWhenDeny == TRUE) {
			// Show message when denied
			DisplayMessageBox(MSGBOX_OTHER_RUNASADMIN_NEXTTIME, (UINT)NULL, MB_OK | MB_ICONINFORMATION);
		}

		if (reqRestart.bResetFlag == TRUE) {
			// Reset flag
			SetFlagValue(FLAGID_RESTARTASADMIN, FALSE);
		}
	}
}

