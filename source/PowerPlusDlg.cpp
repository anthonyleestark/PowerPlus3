
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
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"

#include "Dialogs\AboutDlg.h"
#include "Dialogs\HelpDlg.h"
#include "Dialogs\MultiScheduleDlg.h"
#include "Dialogs\LogViewerDlg.h"
#include "Dialogs\HotkeySetDlg.h"
#include "Dialogs\PwrReminderDlg.h"
#include "Dialogs\ReminderMsgDlg.h"
#include "Dialogs\DebugTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;

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

	// Initialize boolean variables for checkboxes
	m_bRMBShowMenu = FALSE;
	m_bShowDlgAtStartup = FALSE;
	m_bStartupEnabled = FALSE;
	m_bConfirmAction = FALSE;
	m_bSaveHistoryLog = FALSE;
	m_bSaveAppEventLog = FALSE;
	m_bRunAsAdmin = FALSE;
	m_bShowErrorMsg = FALSE;
	m_bNotifySchedule = FALSE;
	m_bAllowCancelSchedule = FALSE;
	m_bEnableBackgroundHotkey = FALSE;
	m_bEnablePowerReminder = FALSE;

	// Initialize notify icon variables
	m_hNotifyIcon = NULL;
	m_pNotifyIconData = NULL;
	m_bNotifyIconShowed = FALSE;
	m_pNotifyMenu = NULL;

	// Init member variables for resizing dialog
	m_bDlgExpanded = TRUE;
	m_pDialogSize = NULL;

	// Init hotkey register data
	m_bHotkeyRegistered = FALSE;
	m_arrCurRegHKeyList.RemoveAll();

	// Init Power++ runtime queue data
	m_arrRuntimeQueue.RemoveAll();

	// Init other flags
	m_bRestartAsAdminFlag = FALSE;
	m_nPwrBroadcastSkipCount = 0;
	m_bWTSSessionNotifyRegistered = FALSE;

	// Init child dialogs
	m_pAboutDlg = NULL;
	m_pHelpDlg = NULL;
	m_pLogViewerDlg = NULL;
	m_pMultiScheduleDlg = NULL;
	m_pHotkeySetDlg = NULL;
	m_pPwrReminderDlg = NULL;
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

	// Clean Power++ runtime queue data
	m_arrRuntimeQueue.RemoveAll();
	m_arrRuntimeQueue.FreeExtra();

	// Kill timers
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Unregister for session state change notifications
	RegisterSessionNotification(MODE_DISABLE);
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
	DDX_Check(pDX,	IDC_SAVEHISTORYLOG_CHK,		m_bSaveHistoryLog);
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
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	INT_PTR - Number of controls added to management
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CPowerPlusDlg::RegisterDialogManagement(void)
{
	INT_PTR nRet = SDialog::RegisterDialogManagement();
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogManagement
//	Description:	Update dialog control management
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UnregisterDialogManagement
//	Description:	Unregister dialog control management
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::UnregisterDialogManagement(void)
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

//////////////////////////////////////////////////////////////////////////
//
//	CPowerPlusDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

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
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Setup system menu
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenuFormat;
		BOOL bNameValid = strAboutMenuFormat.LoadString(IDS_APP_SYSMENU_ABOUT);
		ASSERT(bNameValid);
		if (!strAboutMenuFormat.IsEmpty())
		{
			// Add product version number
			CString strAboutMenu;
			strAboutMenu.Format(strAboutMenuFormat, GetProductVersion(FALSE));

			// Add menu item
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
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
	UpdateDialogData(FALSE);

	// Create notify icon
	if (!CreateNotifyIcon()) {

		// Exit if failed to create notify icon
		TRACE("Failed to create notify icon!!!");
		ExitApp(EXITCODE_ERROR);
		return FALSE;
	}

	// Update dialog control management
	UpdateDialogManagement();

	// Initialize dialog state as collapsed
	ExpandDialog(FALSE);

	// Initialize Power++ runtime queue
	UpdateActionScheduleQueue(MODE_INIT);
	UpdatePwrReminderSnooze(MODE_INIT);

	// Initialize background hotkeys if enabled
	SetupBackgroundHotkey(MODE_INIT);

	// Register for session state change notifications
	RegisterSessionNotification(MODE_INIT);

	// Execute Power Reminder at startup
	ExecutePowerReminder(PREVT_AT_APPSTARTUP);

	// Execute Power Reminder after power action awake
	if (GetPwrActionFlag() == FLAG_ON) {
		ExecutePowerReminder(PREVT_AT_PWRACTIONWAKE);
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
		ExecutePowerReminder(PREVT_AT_SYSWAKEUP);
		SetSystemSuspendFlag(FLAG_OFF);					// Reset flag
		SetSessionEndFlag(FLAG_OFF);					// Reset flag
		if (pApp != NULL) {
			// Save flag value update
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreDestroyDialog
//	Description:	Pre-destroy dialog and exit application
//  Arguments:		None
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::PreDestroyDialog()
{
	// Request closing all child dialog if opening
	LRESULT resCloseReq;

	// About dialog
	if (m_pAboutDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pAboutDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}
	// Help dialog
	if (m_pHelpDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHelpDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}
	// LogViewer dialog
	if (m_pLogViewerDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pLogViewerDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}
	// Multi schedule dialog
	if (m_pMultiScheduleDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pMultiScheduleDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}
	// HotkeySet dialog
	if (m_pHotkeySetDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHotkeySetDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}
	// Power Reminder dialog
	if (m_pPwrReminderDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pPwrReminderDlg->RequestCloseDialog();
		if (resCloseReq != RESULT_SUCCESS)
			return resCloseReq;
	}

	// Can not destroy if Power Reminder messages are currently displaying
	CUIntArray arrPwrDispItemList;
	if (GetPwrReminderDispList(arrPwrDispItemList) > 0) {
		// Display notify message
		DisplayMessageBox(MSGBOX_OTHER_PREDESTROY_REMINDERDISP, MSGBOX_PWRREMINDER_CAPTION);
		return RESULT_FAILED;
	}

	// Destroy components
	RemoveNotifyIcon();
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Execute Power Reminder before exitting
	ExecutePowerReminder(PREVT_AT_APPEXIT);

	// Destroy background hotkeys if enabled
	SetupBackgroundHotkey(MODE_DISABLE);

	// Unregister for session state change notifications
	RegisterSessionNotification(MODE_DISABLE);

	return SDialog::PreDestroyDialog();
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
	SetupBackgroundHotkey(MODE_DISABLE);

	// Unregister for session state change notifications
	RegisterSessionNotification(MODE_DISABLE);

	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

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
	return STATIC_CAST(HCURSOR, m_hDefaultIcon);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_APPLY_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_RELOAD_BTN);

	// Message caption
	UINT nCaptionID = MSGBOX_CONFIG_RELOAD_CAPTION;

	// Check for setting changed
	m_bChangeFlag = CheckSettingChangeState();
	if (m_bChangeFlag == TRUE) {
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXIT_BTN);

	// Destroy dialog and exit
	ExitApp(EXITCODE_EXITBUTTON);
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
		ShowDialog(this, FALSE);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXPAND_BTN);

	// Expand/collapse dialog
	BOOL bCurState = GetFlagValue(FLAGID_DLG_EXPANDED);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_ABOUT_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_DEFAULT_BTN);

	// Set default config
	SetDefaultConfig();
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
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LMBACTION_LIST);
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
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_MMBACTION_LIST);
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
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_RMBACTION_LIST);

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

	// Save app event log if enabled
	OutputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LANGUAGE_LIST);

	// Reload app language
	UINT nCurLanguage = GetAppOption(OPTIONID_LANGUAGE_ID, TRUE);
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage(nCurLanguage);

	// Check for settings change
	m_bChangeFlag = CheckSettingChangeState();

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
//  Arguments:		nChkBoxID - ID of clicked checkbox
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OnCheckboxClicked(UINT nChkBoxID)
{
	// Get clicked checkbox control
	CButton* pChkCtrl = (CButton*)GetDlgItem(nChkBoxID);
	if (pChkCtrl == NULL) return;

	// Update checkbox state
	UpdateDialogData(TRUE);
	int nState = pChkCtrl->GetCheck();

	// Update setting change flag
	m_bChangeFlag = CheckSettingChangeState();

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_BACKUPCFG_BTN);

	// Backup configuration
	BOOL bRet = FALSE;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		bRet = pApp->BackupRegistryAppData();
	}

	// Backup failed
	if (bRet != TRUE) {
		// Show error message if enabled
		ShowErrorMessage(APP_ERROR_BACKUP_REG_FAILED);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_LOGVIEWER_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_SCHEDULE_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_BTN);

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWACTIONLOG_BTN);

	// View action log file
	OpenTextFileToView(FILENAME_HISTORY_LOG, FILEEXT_LOGFILE, SUBFOLDER_LOG);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWBAKCFG_BTN);

	// View backup config file
	OpenTextFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
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
		BOOL bPwrReminderActive = GetAppOption(OPTIONID_ENABLE_PWRREMINDER);
		if (bPwrReminderActive == TRUE) {
			// Execute Power reminder
			ExecutePowerReminder(PREVT_AT_SETTIME);
		}
	}

	// Timer ID: Event skip counter
	else if (nIDEvent == TIMERID_STD_EVENTSKIPCOUNTER) {
		// Process Power Broadcast event skip counter
		int nCounter = GetFlagValue(FLAGID_PWRBROADCAST_SKIP_COUNT);
		if (nCounter > 0) {
			// Count down (decrease value by 1)
			SetFlagValue(FLAGID_PWRBROADCAST_SKIP_COUNT, --nCounter);
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

	// Default
	return SDialog::OnChildDialogDestroy(wParam, lParam);
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
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		ScheduleData* pschData = pApp->GetAppScheduleData();
		if (pschData != NULL) {
			m_schScheduleData.Copy(*pschData);
			UpdateActionScheduleQueue(MODE_UPDATE);
		}
	}

	// Default: Always success
	return LRESULT(RESULT_SUCCESS);
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
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		HotkeySetData* phksData = pApp->GetAppHotkeySetData();
		if (phksData != NULL) {
			m_hksHotkeySetData.Copy(*phksData);
			SetupBackgroundHotkey(MODE_UPDATE);
		}
	}

	// Default: Always success
	return LRESULT(RESULT_SUCCESS);
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
	// Update data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		PwrReminderData* ppwrData = pApp->GetAppPwrReminderData();
		if (ppwrData != NULL) {
			m_prdReminderData.Copy(*ppwrData);
			UpdatePwrReminderSnooze(MODE_UPDATE);
		}
	}

	// Default: Always success
	return LRESULT(RESULT_SUCCESS);
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
		OutputDebugLog(STRING_NULL);
		return LRESULT(RESULT_FAILED);
	}

	// Get debug command string
	CString strDebugCommand(LPARAM_TO_STRING(lParam));

	// Process debug command
	DWORD dwErrorCode = APP_ERROR_DBG_SUCCESS;
	if (!ProcessDebugCommand(strDebugCommand, dwErrorCode)) {

		// Reply failed message
		if (dwErrorCode == APP_ERROR_DBG_INVALID_COMMAND) {
			// Error: Invalid command
			OutputDebugLog(_T("Invalid command!!!"));
		}
		else {
			// Reply corresponding error code
			OutputDebugLogFormat(_T("Failed! (Error code: 0x%X)"), dwErrorCode);
		}

		// Result: Failed
		return LRESULT(RESULT_FAILED);
	}

	// Notify app class about debug command execution
	WPARAM wAppNotiParam = MAKE_WPARAM_STRING(strDebugCommand);
	LPARAM lAppNotiParam = MAKE_LPARAM_STRING(strDebugCommand);
	::PostMessage(NULL, SM_APP_DEBUGCMD_EXEC, wAppNotiParam, lAppNotiParam);

	// Default: Success
	return LRESULT(RESULT_SUCCESS);
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
	// Get flag value
	BOOL bShowFlag = TRUE;
	if (wParam != NULL) {
		bShowFlag = (BOOL)wParam;
	}

	// Show/hide dialog
	ShowDialog(this, bShowFlag);

	// Default: Always success
	return LRESULT(RESULT_SUCCESS);
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
	// Get error code value
	DWORD dwErrorCode = NULL;
	if (wParam != NULL) {
		dwErrorCode = (DWORD)wParam;
	}

	// Show error message
	ShowErrorMessage(dwErrorCode);

	// Default: Always success
	return LRESULT(RESULT_SUCCESS);
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
	if (GetFlagValue(FLAGID_PWRBROADCAST_SKIP_COUNT) > 0) {
		TRACE("Power Broadcast Event will be skipped!!!");
		return LRESULT(RESULT_FAILED);
	}

	// Get event ID from param
	ULONG ulEvent = ULONG(wParam);
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Process system resume/wakeup events
	if ((ulEvent == PBT_APMRESUMESUSPEND) || (ulEvent == PBT_APMRESUMEAUTOMATIC)) {

		// Trigger skip event counter
		// Temporarily skip processing PowerBroadcastEvent in 3 seconds
		SetFlagValue(FLAGID_PWRBROADCAST_SKIP_COUNT, 3);

		// If Power action flag is triggered, 
		// handle it like a wakeup event after power action
		if (GetPwrActionFlag() == FLAG_ON) {
			// Execute Power Reminder after power action awake
			ExecutePowerReminder(PREVT_AT_PWRACTIONWAKE);
			SetPwrActionFlag(FLAG_OFF);		// Reset flag
		}
		// Otherwise,
		// handle it like regular system wakeup event
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
	return LRESULT(RESULT_SUCCESS);
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
	if (pApp == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return LRESULT(RESULT_FAILED);
	}

	/*---------- Process querry ending session event ----------*/

	// Turn on session ending flag
	SetSessionEndFlag(FLAG_ON);
	pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);

	// Save last session ending time
	SYSTEMTIME stCurSysTime = GetCurSysTime();
	pApp->SaveLastSysEventTime(SYSEVT_SESSIONEND, stCurSysTime);

	// Save action history if remaining unsaved
	SaveHistoryInfoData();

	/*---------------------------------------------------------*/

	// Default: Success
	return LRESULT(RESULT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnWTSSessionChange
//	Description:	Handle session state change notifications
//	Arguments:		wParam - First param
//					lParam - Second param
//  Return value:	LRESULT
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPowerPlusDlg::OnWTSSessionChange(WPARAM wParam, LPARAM lParam)
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
	return LRESULT(RESULT_SUCCESS);
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
		ExecuteAction(APP_MACRO_ACTION_SCHEDULE, GetAppOption(OPTIONID_SCHEDULE_ACTION));
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
		OpenTextFileToView(FILENAME_HISTORY_LOG, FILEEXT_LOGFILE, SUBFOLDER_LOG);
		break;
	case IDM_NOTIFY_BACKUP_CONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_BACKUP_CONFIG);
		((CPowerPlusApp*)AfxGetApp())->BackupRegistryAppData();
		break;
	case IDM_NOTIFY_VIEW_BAKCONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_BAKCONFIG);
		OpenTextFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
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
		RequestRestartApp(IDM_NOTIFY_RESTART_APP, FALSE);
		break;
	case IDM_NOTIFY_RESTART_ASADMIN:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_ASADMIN);
		RequestRestartApp(IDM_NOTIFY_RESTART_ASADMIN, TRUE);
		break;
	case IDM_NOTIFY_SHOW_WINDOW:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_SHOW_WINDOW);
		ExpandDialog(FALSE);
		PostMessage(SM_WND_SHOWDIALOG, TRUE);
		break;
	case IDM_NOTIFY_EXIT_APP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_EXIT_APP);
		ExitApp(EXITCODE_NOTIFYMENU);
		break;
	default:
		break;
	}

	// Default
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
	case SM_APP_LOCKSTATE_HOTKEY:
		ProcessLockStateHotkey((DWORD)wParam);
		break;
	case WM_POWERBROADCAST:
		OnPowerBroadcastEvent(wParam, NULL);
		break;
	case WM_WTSSESSION_CHANGE:
		OnWTSSessionChange(wParam, lParam);
		break;
	}

	// Default
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
	BOOL bCurState = GetFlagValue(FLAGID_DLG_EXPANDED);
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
	OutputEventLog(nEventID, this->GetCaption());

	// Update flag
	BOOL bNewState = !bCurState;
	SetFlagValue(FLAGID_DLG_EXPANDED, bNewState);
}


//////////////////////////////////////////////////////////////////////////
// Notify icon functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CreateNotifyIcon
//	Description:	Setup and create notify icon
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::CreateNotifyIcon(void)
{
	// If notify icon is showed, do nothing
	if (GetFlagValue(FLAGID_NOTIFY_ICON_SHOWED)) {
		TRACE("Notify icon is already showed!!!");
		return TRUE;
	}

	// Init notify icon
	if (m_pNotifyIconData == NULL) {
		m_pNotifyIconData = new NOTIFYICONDATA;
		if (m_pNotifyIconData == NULL) {
			// Initialization failed
			TRACE_ERROR("Error: Notify icon initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
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
	BOOL bRetCreate = FALSE;
	for (int nRetry = 0; nRetry < MAX_RETRY_TIMES; nRetry++) {
		bRetCreate = Shell_NotifyIcon(NIM_ADD, m_pNotifyIconData);
		if (bRetCreate != FALSE) break;
	}

	// Failed to create notify icon
	if (bRetCreate != TRUE) {
		// Failed to create notify icon
		TRACE_ERROR("Error: Failed to create notify icon!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Update flag
	SetFlagValue(FLAGID_NOTIFY_ICON_SHOWED, bRetCreate);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowNotifyMenu
//	Description:	Show notify menu
//  Arguments:		None
//	Return value:	BOOL - Show menu successfully or failed
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ShowNotifyMenu(void)
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
		return FALSE;
	}

	// Setup menu properties
	SetMenuItemText(m_pNotifyMenu);
	UpdateMenuItemState(m_pNotifyMenu);
	SetMenuDefaultItem(m_pNotifyMenu->m_hMenu, IDM_NOTIFY_SHOW_WINDOW, NULL);
	this->SetForegroundWindow();

	// Show notify menu
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	UINT nFlags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
	BOOL bResult = m_pNotifyMenu->TrackPopupMenu(nFlags, ptCursor.x, ptCursor.y, (CWnd*)this, NULL);

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveNotifyIcon
//	Description:	Remove notify icon when exiting
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RemoveNotifyIcon(void)
{
	// If notify icon is not showed, do nothing
	if (!GetFlagValue(FLAGID_NOTIFY_ICON_SHOWED)) {
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
	SetFlagValue(FLAGID_NOTIFY_ICON_SHOWED, FALSE);
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetAppOption
//	Description:	Return app option by ID
//  Arguments:		eAppOptionID - ID of specific app option
//					bTemp		 - Temp value or saved value (saved value by default)
//  Return value:	int - App option value
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp /* = FALSE */) const
{
	int nResult = INT_INVALID;
	int nTempResult = INT_INVALID;

	switch (eAppOptionID)
	{
	case OPTIONID_LMB_ACTION:
		nResult = m_cfgAppConfig.nLMBAction;
		nTempResult = m_cfgTempConfig.nLMBAction;
		break;
	case OPTIONID_MMB_ACTION:
		nResult = m_cfgAppConfig.nMMBAction;
		nTempResult = m_cfgTempConfig.nMMBAction;
		break;
	case OPTIONID_RMB_ACTION:
		nResult = m_cfgAppConfig.nRMBAction;
		nTempResult = m_cfgTempConfig.nRMBAction;
		break;
	case OPTIONID_RMB_SHOW_MENU:
		nResult = m_cfgAppConfig.bRMBShowMenu;
		nTempResult = m_cfgTempConfig.bRMBShowMenu;
		break;
	case OPTIONID_LANGUAGE_ID:
		nResult = m_cfgAppConfig.nLanguageID;
		nTempResult = m_cfgTempConfig.nLanguageID;
		break;
	case OPTIONID_SHOW_DLG_AT_STARTUP:
		nResult = m_cfgAppConfig.bShowDlgAtStartup;
		nTempResult = m_cfgTempConfig.bShowDlgAtStartup;
		break;
	case OPTIONID_STARTUP_ENABLE:
		nResult = m_cfgAppConfig.bStartupEnabled;
		nTempResult = m_cfgTempConfig.bStartupEnabled;
		break;
	case OPTIONID_CONFIRM_ACTION:
		nResult = m_cfgAppConfig.bConfirmAction;
		nTempResult = m_cfgTempConfig.bConfirmAction;
		break;
	case OPTIONID_SAVE_HISTORY_LOG:
		nResult = m_cfgAppConfig.bSaveHistoryLog;
		nTempResult = m_cfgTempConfig.bSaveHistoryLog;
		break;
	case OPTIONID_SAVE_APP_EVENT_LOG:
		nResult = m_cfgAppConfig.bSaveAppEventLog;
		nTempResult = m_cfgTempConfig.bSaveAppEventLog;
		break;
	case OPTIONID_RUN_AS_ADMIN:
		nResult = m_cfgAppConfig.bRunAsAdmin;
		nTempResult = m_cfgTempConfig.bRunAsAdmin;
		break;
	case OPTIONID_SHOW_ERROR_MSG:
		nResult = m_cfgAppConfig.bShowErrorMsg;
		nTempResult = m_cfgTempConfig.bShowErrorMsg;
		break;
	case OPTIONID_NOTIFY_SCHEDULE:
		nResult = m_cfgAppConfig.bNotifySchedule;
		nTempResult = m_cfgTempConfig.bNotifySchedule;
		break;
	case OPTIONID_ALLOW_CANCEL_SCHEDULE:
		nResult = m_cfgAppConfig.bAllowCancelSchedule;
		nTempResult = m_cfgTempConfig.bAllowCancelSchedule;
		break;
	case OPTIONID_ENABLE_HOTKEYSET:
		nResult = m_cfgAppConfig.bEnableBackgroundHotkey;
		nTempResult = m_cfgTempConfig.bEnableBackgroundHotkey;
		break;
	case OPTIONID_LOCK_STATE_HOTKEY:
		nResult = m_cfgAppConfig.bLockStateHotkey;
		nTempResult = m_cfgTempConfig.bLockStateHotkey;
		break;
	case OPTIONID_ENABLE_PWRREMINDER:
		nResult = m_cfgAppConfig.bEnablePowerReminder;
		nTempResult = m_cfgTempConfig.bEnablePowerReminder;
		break;
	case OPTIONID_SCHEDULE_ACTIVE:
		nResult = m_schScheduleData.GetDefaultItem().IsEnabled();
		nTempResult = nResult;		// No temp data
		break;
	case OPTIONID_SCHEDULE_ACTION:
		nResult = m_schScheduleData.GetDefaultItem().GetAction();
		nTempResult = nResult;		// No temp data
		break;
	case OPTIONID_SCHEDULE_REPEAT:
		nResult = m_schScheduleData.GetDefaultItem().IsRepeatEnabled();
		nTempResult = nResult;		// No temp data
		break;
	default:
		// Get dialog-base-class option value
		nResult = SDialog::GetAppOption(eAppOptionID, FALSE);
		nTempResult = SDialog::GetAppOption(eAppOptionID, TRUE);
		break;
	}

	// Return temp data if required and the result is valid
	if ((bTemp == TRUE) && (nTempResult != INT_INVALID))
		return nTempResult;

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDialogData
//	Description:	Update data values for dialog items
//  Arguments:		bSaveAndValidate - Same as default MFC UpdateData function
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateDialogData(BOOL bSaveAndValidate /* = TRUE */)
{
	UINT nCmbSel = 0;

	if (bSaveAndValidate == FALSE) {

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
		nCmbSel = (m_bRMBShowMenu == TRUE) ? APP_ACTION_SHOWMENU : m_cfgTempConfig.nRMBAction;
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
		UpdateData(FALSE);

	/*------------------------------------------------------------------------------------------*/

	}
	else {

	/*----------------------------<Bind dialog items data to config>----------------------------*/

		// Update dialog items
		UpdateData(TRUE);

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
		nCmbSel = (m_cfgTempConfig.bRMBShowMenu == TRUE) ? nCmbSel : m_cmbRMBAction.GetCurSel();
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckSettingChangeState
//	Description:	Check setting variables changing state
//  Arguments:		None
//  Return value:	BOOL - Change flag
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::CheckSettingChangeState(void)
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetFlagValue
//	Description:	Return flag value by ID
//  Arguments:		eFlagID - ID of specific flag
//  Return value:	int - Flag value
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::GetFlagValue(APPFLAGID eFlagID) const
{
	int nValue = INT_INVALID;

	switch (eFlagID)
	{
	case FLAGID_CHANGE_FLAG:					// Data/setting change flag
		nValue = m_bChangeFlag;
		break;
	case FLAGID_DLG_EXPANDED:					// Dialog expanded/collapsed
		nValue = m_bDlgExpanded;
		break;
	case FLAGID_NOTIFY_ICON_SHOWED:				// Notify icon showing flag
		nValue = m_bNotifyIconShowed;
		break;
	case FLAGID_HOTKEY_REGISTERED:				// Hotkey registered
		nValue = m_bHotkeyRegistered;
		break;
	case FLAGID_RESTART_AS_ADMIN:				// Restart as admin flag
		nValue = m_bRestartAsAdminFlag;
		break;
	case FLAGID_PWRBROADCAST_SKIP_COUNT:		// Power Broadcase event skip counter
		nValue = m_nPwrBroadcastSkipCount;
		break;
	case FLAGID_WTSSESSIONNOTIFY_REG:			// WTS Session Change State Notification registered
		nValue = m_bWTSSessionNotifyRegistered;
		break;
	default:
		// Get dialog-base-class flag value
		nValue = SDialog::GetFlagValue(eFlagID);
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
	if (nValue == INT_INVALID)
		return;

	switch (eFlagID)
	{
	case FLAGID_CHANGE_FLAG:					// Data/setting change flag
		m_bChangeFlag = nValue;
		break;
	case FLAGID_DLG_EXPANDED:					// Dialog expanded/collapsed
		m_bDlgExpanded = nValue;
		break;
	case FLAGID_NOTIFY_ICON_SHOWED:				// Notify icon showing flag
		m_bNotifyIconShowed = nValue;
		break;
	case FLAGID_HOTKEY_REGISTERED:				// Hotkey registered
		m_bHotkeyRegistered = nValue;
		break;
	case FLAGID_RESTART_AS_ADMIN:				// Restart as admin flag
		m_bRestartAsAdminFlag = nValue;
		break;
	case FLAGID_PWRBROADCAST_SKIP_COUNT:		// Power Broadcase event skip counter
		m_nPwrBroadcastSkipCount = nValue;
		break;
	case FLAGID_WTSSESSIONNOTIFY_REG:			// WTS Session Change State Notification registered
		m_bWTSSessionNotifyRegistered = nValue;
		break;
	default:
		// Set dialog-base-class flag value
		SDialog::SetFlagValue(eFlagID, nValue);
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Dialog setup functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetupLanguage(void)
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
			int nState = GetFlagValue(FLAGID_DLG_EXPANDED);
			if (nState == TRUE)	nID = IDC_COLLAPSE_BTN;
			else nID = IDC_EXPAND_BTN;
			SetControlText(pWndChild, nID, pAppLang);
		}	break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Default
	SDialog::SetupLanguage();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupComboBox
//	Description:	Setup data for combo-boxes
//  Arguments:		nComboID	- ID of combo box
//					ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
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
//  Arguments:		bEnable - Enable or disable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::EnableLogViewer(BOOL bEnable)
{
	// Prefer using app data option than temp config option
	BOOL bCheck = bEnable;
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		bCheck = pApp->GetAppOption(OPTIONID_SAVE_APP_EVENT_LOG);
	}
	if (bCheck == FALSE) {
		EnableItem(IDC_LOGVIEWER_BTN, bCheck);
		return;
	}

	// Update dialog data
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
	BOOL bCheck = GetAppOption(OPTIONID_RUN_AS_ADMIN);

	// If option is currently ON, do not update flag
	if (bCheck == TRUE)	return;

	// Update flag
	SetFlagValue(FLAGID_RESTART_AS_ADMIN, bFlag);
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
	if (pMenu == NULL) {
		// Trace error
		TRACE_ERROR("Error: Menu does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Main menu
	for (int nItem = 0; nItem < pMenu->GetMenuItemCount(); nItem++) {
		BOOL bShowItem = TRUE;
		UINT nID = pMenu->GetMenuItemID(nItem);
		switch (nID)
		{
		case IDM_NOTIFY_OPENDLG_LOGVIEWER:
			bShowItem = GetAppOption(OPTIONID_SAVE_APP_EVENT_LOG);
			if (bShowItem == TRUE)
				bShowItem = GetAppOption(OPTIONID_SAVE_APP_EVENT_LOG, TRUE);
			break;
		case IDM_NOTIFY_OPENDLG_HOTKEYSET:
			bShowItem = GetAppOption(OPTIONID_ENABLE_HOTKEYSET, TRUE);
			break;
		case IDM_NOTIFY_OPENDLG_PWRREMINDER:
			bShowItem = GetAppOption(OPTIONID_ENABLE_PWRREMINDER, TRUE);
			break;
		default:
			continue;
			break;
		}

		// Disable item
		if (bShowItem == FALSE) {
			pMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
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
			bShowItem = GetAppOption(OPTIONID_SCHEDULE_ACTIVE);
			bShowItem &= (GetAppOption(OPTIONID_SCHEDULE_ACTION) != APP_ACTION_NOTHING);
			break;
		default:
			continue;
			break;
		}

		// Disable item
		if (bShowItem == FALSE) {
			pSubMenu->EnableMenuItem(nItem, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
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
	if (pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return STRING_NULL;
	}

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
	if (pNotifyIconData == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strTipText;
	CString strFormat;
	CStringArray arrTipText;

	// Load language strings
	strFormat = GetLanguageString(pAppLang, NOTIFY_TIP_TEMPLATE);
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nLMBAction)));
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nMMBAction)));
	arrTipText.Add(GetLanguageString(pAppLang, GetPairedID(IDTable::NotifyTip, m_cfgAppConfig.nRMBAction)));

	// Format notify tip text
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

	CString strBalloonTip;
	CString strFormat;
	CString strBalloonText;

	// Load language strings
	strFormat = GetLanguageString(pAppLang, BALLOON_TIP_TEMPLATE);
	strBalloonText = GetLanguageString(pAppLang, GetPairedID(IDTable::BalloonTip, nScheduleAction));

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
	UINT nActionNameResID = 0;
	UINT nActionNameLangID = 0;
	UINT nMessage = 0;

	// Output debug log
	OutputDebugLogFormat(_T("Execute action: Type=0x%04X, Param=0x%04X"), nActionType, (UINT)wParam);

	// Get action ID
	switch (nActionType)
	{
	case APP_MACRO_LEFT_MOUSE:
		// Get action ID: Left mouse
		nActionID = GetAppOption(OPTIONID_LMB_ACTION);
		break;
	case APP_MACRO_MIDDLE_MOUSE:
		// Get action ID: Middle mouse
		nActionID = GetAppOption(OPTIONID_MMB_ACTION);
		break;
	case APP_MACRO_ACTION_SCHEDULE:
		// Get action ID from param
		if (wParam == NULL) {
			// Trace error
			TRACE_ERROR("Error: Invalid parameter!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
		nActionID = DEFAULT_CAST(UINT, wParam);
		break;
	case APP_MACRO_RIGHT_MOUSE:
		// If right mouse action is set to show notify menu
		if ((GetAppOption(OPTIONID_RMB_ACTION) == APP_ACTION_SHOWMENU) ||
			(GetAppOption(OPTIONID_RMB_SHOW_MENU) == TRUE))
			return ShowNotifyMenu();
		// Otherwise, get action ID: Right mouse
		nActionID = GetAppOption(OPTIONID_RMB_ACTION);
		break;
	case APP_MACRO_ACTION_MENU:
	case APP_MACRO_ACTION_HOTKEY:
		// Get action ID from param
		if (wParam == NULL) {
			// Trace error
			TRACE_ERROR("Error: Invalid parameter!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return FALSE;
		}
		nActionID = DEFAULT_CAST(UINT, wParam);
		break;
	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Get action info
	switch (nActionID)
	{
	case APP_ACTION_NOTHING:
		// Do nothing (litterally)
		// Pretend that the action completes successfully
		TRACE("[ExecuteAction] Do nothing -> Successfully");
		return TRUE;
		break;
	case APP_ACTION_DISPLAYOFF:
		// Turn off display
		nAction = APP_ACTIONTYPE_MONITOR;
		nMessage = APP_MESSAGE_DISPLAYOFF;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_DISPLAYOFF;
		nActionNameLangID = ACTION_NAME_DISPLAYOFF;
		break;
	case APP_ACTION_SLEEP:
		// Sleep
		nAction = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SLEEP;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SLEEP;
		nActionNameLangID = ACTION_NAME_SLEEP;
		break;
	case APP_ACTION_SHUTDOWN:
		// Shutdown
		nAction = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SHUTDOWN;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SHUTDOWN;
		nActionNameLangID = ACTION_NAME_SHUTDOWN;
		break;
	case APP_ACTION_RESTART:
		// Restart
		nAction = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_REBOOT;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_RESTART;
		nActionNameLangID = ACTION_NAME_RESTART;
		break;
	case APP_ACTION_SIGNOUT:
		// Sign out
		nAction = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_SIGNOUT;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SIGNOUT;
		nActionNameLangID = ACTION_NAME_SIGNOUT;
		break;
	case APP_ACTION_HIBERNATE:
		// Hibernate
		nAction = APP_ACTIONTYPE_POWER;
		nMessage = APP_MESSAGE_HIBERNATE;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_HIBERNATE;
		nActionNameLangID = ACTION_NAME_HIBERNATE;
		break;
	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Action result info
	BOOL bResult = FALSE;
	DWORD dwErrorCode = APP_ERROR_SUCCESS;

	// Confirm before executing action
	if (ConfirmActionExec(nActionType, nActionID) == IDYES) {

		// Execute Power Reminder before doing action
		ExecutePowerReminder(PREVT_AT_BFRPWRACTION);

		// Check if DummyTest mode is enabled or not
		BOOL bDummyTestMode = GetDummyTestMode();

		if (bDummyTestMode != TRUE) {
			// Normal mode
			bResult = ExecutePowerAction(nAction, nMessage, dwErrorCode);
		}
		else {
			// DummyTest mode
			bResult = ExecutePowerActionDummy(nAction, nMessage, dwErrorCode);
		}

		// Save Power Action trace flag
		SetPwrActionFlag((bResult == TRUE) ? FLAG_ON : FLAG_OFF);
		CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
		if (pApp != NULL) {
			pApp->SaveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}

		// Collect power action history info
		InitPwrActionHistoryInfo(nActionNameResID, bResult, dwErrorCode);
		SaveHistoryInfoData();

		// Show error message
		ShowErrorMessage(dwErrorCode);
	}
	else {
		// Power action canceled --> Output event log
		CString strPwrActionName;
		LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
		if (pAppLang != NULL) {
			strPwrActionName = GetLanguageString(pAppLang, nActionNameLangID);
		}
		OutputEventLog(LOG_EVENT_CANCEL_PWRACTION, strPwrActionName);
	}

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ApplySettings
//	Description:	Apply changes and minimize window to tray
//  Arguments:		bMinimize - Minimize to tray after applying changes
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
			DWORD dwErr = APP_ERROR_SAVE_CFG_FAILED;
			ShowErrorMessage(dwErr);
		}
	}

	// Restart as admin privileges if triggered
	BOOL bRestartTrigger = GetFlagValue(FLAGID_RESTART_AS_ADMIN);
	if (bRestartTrigger == TRUE) {
		RequestRestartApp(IDC_APPLY_BTN, TRUE);
	}

	// Update background hotkeys if enabled
	SetupBackgroundHotkey(MODE_UPDATE);

	// Disable Power Reminder snooze queue if feature's disabled
	if (GetAppOption(OPTIONID_ENABLE_PWRREMINDER) == NULL) {
		UpdatePwrReminderSnooze(MODE_DISABLE);
	}

	// Update notify icon tip text
	UpdateNotifyIcon();

	// Change display status
	if (bMinimize == TRUE) {
		// Minimize to tray (hide dialog)
		ShowDialog(this, FALSE);
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

void CPowerPlusDlg::ReloadSettings(void)
{
	// Reload app data
	GetAppData();

	// Reload app language
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();

	// Reupdate dialog items
	SetupLanguage();
	UpdateDialogData(FALSE);

	// Reset data change flag
	SetFlagValue(FLAGID_CHANGE_FLAG, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetDefaultConfig
//	Description:	Set default config data and re-update display
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetDefaultConfig(void)
{
	// Set default options
	m_cfgTempConfig.SetDefaultData();

	// Reload app language & reset language display
	((CPowerPlusApp*)AfxGetApp())->ReloadAppLanguage();
	SetupLanguage();

	// Reupdate dialog items
	UpdateDialogData(FALSE);
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
	this->SetWindowText(STRING_NULL);

	// Exit the current application instance
	ExitApp(EXITCODE_RESTARTAPP);

	// Restart immediately
	RunApp(GetApplicationPath(TRUE), bRestartAsAdmin);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ExitApp
//	Description:	Safely trigger exitting the application
//  Arguments:		nExitCode - Application exit code (use for PostQuitMessage)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ExitApp(int nExitCode)
{
	// Request closing opening dialogs and do clean-up
	if (PreDestroyDialog() != RESULT_SUCCESS) {
		TRACE("Request close application denied!!!");
		return;
	}

	// Termination
	// Trigger exitting by posting quit message
	PostQuitMessage(nExitCode);
}


//////////////////////////////////////////////////////////////////////////
// Dialog and window functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowDialog
//	Description:	Show/hide dialog out of/into system tray
//  Arguments:		pWnd	  - Pointer of the dialog to show/hide
//					bShowFlag - Flag to show/hide dialog
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::ShowDialog(CWnd* pWnd, BOOL bShowFlag /* = TRUE */)
{
	// Get show/hide flag
	int nCmdShow = (bShowFlag == TRUE) ? SW_SHOW : SW_HIDE;

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
	UINT nEventID = (bShowFlag == TRUE) ? LOG_EVENT_DLG_SHOWED : LOG_EVENT_DLG_HIDDEN;
	OutputEventLog(nEventID, ((SDialog*)pWnd)->GetCaption());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenChildDialogEx
//	Description:	Open child dialog with corresponding ID
//  Arguments:		nDialogID - Child dialog ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OpenChildDialogEx(UINT nDialogID)
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
			ShowDialog(m_pAboutDlg, TRUE);
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
			ShowDialog(m_pHelpDlg, TRUE);
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
			ShowDialog(m_pMultiScheduleDlg, TRUE);
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
			ShowDialog(m_pLogViewerDlg, TRUE);
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
			ShowDialog(m_pHotkeySetDlg, TRUE);
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
			ShowDialog(m_pPwrReminderDlg, TRUE);
		}
	}
	// DebugTest dialog
	else if (nDialogID == IDD_DEBUGTEST_DLG) {
		// Get app DebugTest dialog
		SDialog* pDialog = pApp->GetDebugTestDlg();
		if (pDialog == NULL) {

			// Initialize dialog
			BOOL bRetInit = pApp->InitDebugTestDlg();
			if (bRetInit != FALSE) {

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
				ShowDialog(pDialog, TRUE);
			}
			else {
				// Set parent window
				pDialog->SetParentWnd(this);

				// Open in modeless mode
				pDialog->Create(nDialogID, this);
				pDialog->ShowWindow(SW_SHOW);
			}
		}
	}

	// Base class processing
	SDialog::OpenChildDialogEx(nDialogID);
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
		CWnd* pDialogWnd = FromHandle(hDialogWnd);
		ShowDialog(pDialogWnd, TRUE);
		return;
	}

	// Dialog initialization
	SDialog* pDialog = NULL;
	BOOL bSetReadOnly = FALSE;
	CWnd* pParentWnd = NULL;
	UINT nEntryMsgID = INT_NULL;
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
			nOpenMode = MODE_OPENDLG_MODELESS;
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
		if (bSetReadOnly == TRUE) {
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OpenTextFileToView
//	Description:	Open text file to view using Notepad
//  Arguments:		lpszFileName - File name/path
//					lpszExtension - File extension
//					lpszSubDir	- Sub-directory name
//  Return value:	BOOL - Result of file opening
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::OpenTextFileToView(LPCTSTR lpszFileName, LPCTSTR lpszExtension, LPCTSTR lpszSubDir /* = STRING_EMPTY */)
{
	// Get file name
	VERIFY(lpszFileName != NULL);
	CString strFilePath = (CString)lpszFileName + lpszExtension;
	
	// If sub-directory name is not empty
	if (IS_NOT_EMPTY_STRING(lpszSubDir)) {
		// Format file path with sub-directory
		CString strFolderPath = GetSubFolderPath(lpszSubDir);
		MakeFilePath(strFilePath, strFolderPath, lpszFileName, lpszExtension);
	}

	// Standard file view
	return FileViewStd(FILETYPE_TEXT, strFilePath);
}


//////////////////////////////////////////////////////////////////////////
// Action Schedule feature functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessActionSchedule
//	Description:	Process Action schedule function
//  Arguments:		None
//  Return value:	BOOL - Schedule processing result
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessActionSchedule(void)
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
		if (schDefaultItem.IsEnabled() == FALSE) {
			// Do not process
			bSkipProcess = TRUE;
		}

		// If repeat option is ON and is set as active in current day of week
		if ((schDefaultItem.IsRepeatEnabled() == TRUE) && (!schDefaultItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek))) {
			// Do not process
			bSkipProcess = TRUE;
		}

		// Process default schedule
		if (bSkipProcess != TRUE) {

			// Check for time matching and trigger schedule notifying if enabled
			if (GetAppOption(OPTIONID_NOTIFY_SCHEDULE) == TRUE) {
				BOOL bTriggerNotify = CheckTimeMatch(stCurrentTime, schDefaultItem.GetTime(), -30);
				if (bTriggerNotify == TRUE) {
					// Do notify schedule (and check for trigger reupdate)
					int nRetNotify = NotifySchedule(&schDefaultItem, bTriggerReupdate);
					bResult = FALSE;
				}
			}

			// Check for time matching and trigger the scheduled action
			BOOL bTriggerAction = CheckTimeMatch(stCurrentTime, schDefaultItem.GetTime());
			if (bTriggerAction == TRUE) {
				
				// Check if item is marked as skipped
				BOOL bSkipFlag = GetActionScheduleSkipStatus(schDefaultItem.GetItemID());
				if (bSkipFlag != TRUE) {

					// Output event log: Schedule executed
					OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schDefaultItem);

					// Save history info data
					InitScheduleHistoryInfo(schDefaultItem);
					SaveHistoryInfoData();

					// Execute schedule action
					bResult = ExecuteAction(APP_MACRO_ACTION_SCHEDULE, schDefaultItem.GetAction());

					// If "Repeat" option is not ON,
					// --> Disable schedule item after done
					if (schDefaultItem.IsRepeatEnabled() == FALSE) {
						schDefaultItem.EnableItem(FALSE);
						bTriggerReupdate |= TRUE;
					}
				}
				else {
					// Process failed
					bResult = FALSE;
				}

				// Set item as no longer skipped
				SetActionScheduleSkip(schDefaultItem, FLAG_OFF);
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
		if ((schExtraItem.IsRepeatEnabled() == TRUE) && (!schExtraItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek)))
			continue;

		// Check for time matching and trigger schedule notifying if enabled
		if (GetAppOption(OPTIONID_NOTIFY_SCHEDULE) == TRUE) {
			BOOL bTriggerNotify = CheckTimeMatch(stCurrentTime, schExtraItem.GetTime(), -30);
			if (bTriggerNotify == TRUE) {
				// Do notify schedule (and check for trigger reupdate)
				int nRetNotify = NotifySchedule(&schExtraItem, bTriggerReupdate);
				bResult = FALSE;
				continue;
			}
		}

		// Check for time matching and trigger the scheduled action
		BOOL bTriggerAction = CheckTimeMatch(stCurrentTime, schExtraItem.GetTime());
		if (bTriggerAction == TRUE) {

			// Check if item is marked as skipped
			BOOL bSkipFlag = GetActionScheduleSkipStatus(schExtraItem.GetItemID());
			if (bSkipFlag != TRUE) {

				// Output event log: Schedule executed
				OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schExtraItem);

				// Save history info data
				InitScheduleHistoryInfo(schExtraItem);
				SaveHistoryInfoData();

				// Execute schedule action
				bResult = ExecuteAction(APP_MACRO_ACTION_SCHEDULE, schExtraItem.GetAction());

				// If "Repeat" option is not ON,
				// --> Disable schedule item after done
				if (schExtraItem.IsRepeatEnabled() == FALSE) {
					schExtraItem.EnableItem(FALSE);
					bTriggerReupdate |= TRUE;
				}
			}
			else {
				// Process failed
				bResult = FALSE;
			}

			// Set item as no longer skipped
			SetActionScheduleSkip(schExtraItem, FLAG_OFF);
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
//	Function name:	SetActionScheduleSkip
//	Description:	Setup Action Schedule item skip mode
//  Arguments:		schItem	  - Action schedule item
//					nSkipFlag - Skip flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetActionScheduleSkip(const SCHEDULEITEM& schItem, int nSkipFlag)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty()) return;

	// Find if item skip mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == schItem.GetItemID()) {

			// Update item skip mode data
			pwrRuntimeItem.SetSkipFlag(nSkipFlag);
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(FID_SCHEDULEITEM);
	pwrRuntimeItem.SetItemID(schItem.GetItemID());
	pwrRuntimeItem.SetSkipFlag(nSkipFlag);

	// Add item to runtime queue
	m_arrRuntimeQueue.Add(pwrRuntimeItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetActionScheduleSnooze
//	Description:	Setup Action Schedule item snooze mode
//  Arguments:		schItem		- Action schedule item
//					nSnoozeFlag - Snooze flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetActionScheduleSnooze(const SCHEDULEITEM& schItem, int nSnoozeFlag)
{
	// If item is empty, do nothing
	if (schItem.IsEmpty()) return;

	// If snoozing option is not available
	if (schItem.IsAllowSnoozing() != TRUE) {
		// Turn off flag
		nSnoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int nInterval = DEF_REPEATSET_DEFAULT_SNOOZE;

	// Find if item skip mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == schItem.GetItemID()) {

			// Update item skip mode data
			pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);

			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.SetTime(GetCurSysTime());
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(FID_SCHEDULEITEM);
	pwrRuntimeItem.SetItemID(schItem.GetItemID());
	pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);
	if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.SetTime(GetCurSysTime());
		pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
	}

	// Add item to runtime queue
	m_arrRuntimeQueue.Add(pwrRuntimeItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateActionScheduleQueue
//	Description:	Update Action Schedule runtime queue data
//  Arguments:		nMode - Update mode flag (INIT, UPDATE or DISABLE)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::UpdateActionScheduleQueue(int nMode)
{
	if (nMode == MODE_INIT) {

		// Initialize Action Schedule runtime queue
		// Set all items as not skipped
		SetActionScheduleSkip(m_schScheduleData.GetDefaultItem(), FLAG_OFF);
		for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
			SetActionScheduleSkip(m_schScheduleData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == MODE_UPDATE) {

		// Update skip queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.GetSize() - 1); nQueueIdx >= 0; nQueueIdx--) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nQueueIdx);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

			// Item found flag
			BOOL bItemFound = FALSE;

			// Check if item ID is default schedule item
			const SCHEDULEITEM& schDefaultItem = m_schScheduleData.GetDefaultItem();
			if (schDefaultItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

				// If item's snoozing mode is no longer available
				if (!schDefaultItem.IsAllowSnoozing()) {
					// Disable snooze mode
					pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
				}
				
				// Mark as found
				bItemFound = TRUE;
				break;
			}

			// Search for item ID in Action Schedule extra item data
			for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
				const SCHEDULEITEM& schItem = m_schScheduleData.GetItemAt(nItemIdx);
				if (schItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

					// If item's snoozing mode is no longer available
					if (!schItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
					}

					// Mark as found
					bItemFound = TRUE;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (bItemFound == FALSE) {
				// Remove item from runtime queue
				m_arrRuntimeQueue.RemoveAt(nQueueIdx);
			}
		}

		// Free extra memory
		m_arrRuntimeQueue.FreeExtra();
	}
	else if (nMode == MODE_DISABLE) {

		// Disable skip mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

			// Update item skip mode data
			pwrRuntimeItem.SetSkipFlag(FLAG_OFF);

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActionScheduleSkipStatus
//	Description:	Get Action Schedule item skip flag status
//  Arguments:		nItemID	- Action Schedule item ID
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::GetActionScheduleSkipStatus(UINT nItemID)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.IsEmpty())
		return FALSE;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		const PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get skip enable flag
			if (pwrRuntimeItem.GetSkipFlag() == FLAG_ON)
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetActionScheduleSnoozeStatus
//	Description:	Get Action Schedule item snooze trigger status
//  Arguments:		nItemID	   - Action Schedule item ID
//					curSysTime - Current system time
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::GetActionScheduleSnoozeStatus(UINT nItemID, SYSTEMTIME& curSysTime)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.IsEmpty())
		return FALSE;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		const PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not an Action Schedule item
		if (pwrRuntimeItem.GetCategory() != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_OFF)
				return FALSE;

			// Check for next snooze time matching
			if (CheckTimeMatch(curSysTime, pwrRuntimeItem.GetTime()))
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// HotkeySet feature functions

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
	// Get option and flag values
	BOOL bHKSEnable = GetAppOption(OPTIONID_ENABLE_HOTKEYSET);
	BOOL bHKRegisterFlag = GetFlagValue(FLAGID_HOTKEY_REGISTERED);

	// If background hotkey feature is disabled and no hotkey registered, do nothing
	if ((bHKSEnable == FALSE) &&											// HotkeySet option OFF
		(bHKRegisterFlag == FALSE) &&										// Hotkey registered flag OFF
		(m_arrCurRegHKeyList.IsEmpty())) {									// Registered hotkey list empty
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

	if ((nMode == MODE_DISABLE) || (nMode == MODE_UPDATE)) {

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
						SetFlagValue(FLAGID_HOTKEY_REGISTERED, FALSE);		// Reset hotkey registered flag
						m_arrCurRegHKeyList.RemoveAll();					// Cleanup registered hotkey list
						m_arrCurRegHKeyList.FreeExtra();
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

	if ((nMode == MODE_INIT) || (nMode == MODE_UPDATE)) {

		// If feature not enabled, do nothing
		if (bHKSEnable == FALSE) {
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
		SetFlagValue(FLAGID_HOTKEY_REGISTERED, FALSE);
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
			UINT nHKActionID = hksItem.GetActionID();

			// Get keycode
			DWORD dwCtrlKeyCode, dwFuncKeyCode;
			hksItem.GetKeyCode(dwCtrlKeyCode, dwFuncKeyCode);
			if ((dwCtrlKeyCode == 0) || (dwFuncKeyCode == 0))
				continue;

			dwCtrlKeyCode |= MOD_NOREPEAT;

			// Debug log format
			strLogTemp.Format(_T("ActionID=%d, CtrlKey=%d, FuncKey=%d"), nHKActionID, dwCtrlKeyCode, dwFuncKeyCode);

			// Get enable/disable status
			BOOL bEnabled = hksItem.IsEnabled();

			// Skip registering item if disabled
			if (bEnabled == FALSE) {
				OutputDebugLogFormat(_T("Skip registering hotkey (disabled): %s"), strLogTemp);
				continue;
			}

			// Register hotkey item
			BOOL bRet = RegisterHotKey(hWnd, nHKActionID, dwCtrlKeyCode, dwFuncKeyCode);

			// Trigger flag
			bRegistered |= bRet;
			SetFlagValue(FLAGID_HOTKEY_REGISTERED, bRegistered);

			if (bRet == TRUE) {
				// Register successfully
				OutputDebugLogFormat(_T("Registered hotkey: %s"), strLogTemp);
				m_arrCurRegHKeyList.Add(nHKActionID);						// Update registered hotkey list
			}
			else {
				// Register failed
				dwErrorCode = GetLastError();

				// Output debug log
				OutputDebugLogFormat(_T("Register hotkey failed: %s"), strLogTemp);

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
	if (GetAppOption(OPTIONID_ENABLE_HOTKEYSET) == FALSE) {
		TRACE("Background hotkey option is not enabled!!!");
		return FALSE;
	}

	// Output debug log
	OutputDebugLogFormat(_T("Process Hotkey: HKeyID=%d"), nHotkeyID);

	// Get HotkeySet item by ID
	HOTKEYSETITEM hksItem;
	for (int nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM& hksTemp = m_hksHotkeySetData.GetItemAt(nIndex);
		if (hksTemp.GetActionID() == nHotkeyID) {
			hksItem.Copy(hksTemp);
			break;
		}
	}

	// Check item validity
	if (hksItem.IsEmpty())
		return FALSE;

	// Output hotkey event log
	CString strHotkeyDescription;
	hksItem.PrintKeyStrokes(strHotkeyDescription);
	OutputEventLog(LOG_EVENT_EXEC_HOTKEY, strHotkeyDescription);

	// Get Power action ID by HotkeyID
	UINT nActionID = NULL;
	switch (nHotkeyID)
	{
	case HKID_DISPLAYOFF:
		nActionID = APP_ACTION_DISPLAYOFF;
		break;
	case HKID_SLEEP:
		nActionID = APP_ACTION_SLEEP;
		break;
	case HKID_SHUTDOWN:
		nActionID = APP_ACTION_SHUTDOWN;
		break;
	case HKID_RESTART:
		nActionID = APP_ACTION_RESTART;
		break;
	case HKID_SIGNOUT:
		nActionID = APP_ACTION_SIGNOUT;
		break;
	case HKID_HIBERNATE:
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
		return FALSE;
	}

	// Save history info data
	InitHotkeyHistoryInfo(nHotkeyID);
	SaveHistoryInfoData();
	
	// Execute hotkeyset action
	WPARAM wParam = (WPARAM)nActionID;
	BOOL bResult = ExecuteAction(APP_MACRO_ACTION_HOTKEY, wParam);
	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterSessionNotification
//	Description:	Register/unregister to receive session state change notification
//  Arguments:		nMode - Mode of setup (INIT, UPDATE or DISABLE)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

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

	if ((nMode == MODE_DISABLE) || (nMode == MODE_UPDATE)) {

		// Only unregister if the flag is not OFF
		if (GetFlagValue(FLAGID_WTSSESSIONNOTIFY_REG)) {

			if (WTSUnRegisterSessionNotification(hCurWnd)) {
				// Mark flag as OFF
				SetFlagValue(FLAGID_WTSSESSIONNOTIFY_REG, FALSE);
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

	if ((nMode == MODE_INIT) || (nMode == MODE_UPDATE)) {

		// Only register if the flag is not ON
		if (!GetFlagValue(FLAGID_WTSSESSIONNOTIFY_REG)) {

			if (WTSRegisterSessionNotification(hCurWnd, NOTIFY_FOR_THIS_SESSION)) {
				// Mark flag as ON
				SetFlagValue(FLAGID_WTSSESSIONNOTIFY_REG, TRUE);
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ProcessLockStateHotkey
//	Description:	Process when registered hotkey is pressed in lock state
//					(this function will convert key-hooked param sent from the
//					application class into corresponding registered hotkey ID)
//  Arguments:		dwHKeyParam - Hotkey parameters (sent from app class)
//  Return value:	BOOL - Hotkey processing result
//	Notes:			If the hotkey ID is found and is registered, the hotkey will
//					be processed and executed using the base ProcessHotkey function
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::ProcessLockStateHotkey(DWORD dwHKeyParam)
{
	// Only process if both options are enabled
	if ((GetAppOption(OPTIONID_ENABLE_HOTKEYSET) != TRUE) ||
		(GetAppOption(OPTIONID_LOCK_STATE_HOTKEY) != TRUE)) {
		TRACE("[LockState Hotkey] Either required options are not enabled!!!");
		return FALSE;
	}

	// Convert hotkey param back into keycode
	WORD wControlKey  = LOWORD(dwHKeyParam);
	WORD wFunctionKey = HIWORD(dwHKeyParam);

	// Look for corresponding HotkeyID in HotkeySet data
	UINT nHKActionID = INT_NULL;
	DWORD dwCtrlKeyTemp, dwFuncKeyTemp;
	for (INT_PTR nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		const HOTKEYSETITEM& hksItem = m_hksHotkeySetData.GetItemAt(nIndex);
		hksItem.GetKeyCode(dwCtrlKeyTemp, dwFuncKeyTemp);
		if ((hksItem.IsEnabled() == TRUE) &&		// HotkeySet item is enabled
			(dwCtrlKeyTemp == wControlKey) &&		// Control key is matching
			(dwFuncKeyTemp == wFunctionKey)) {		// Function key is matching
			nHKActionID = hksItem.GetActionID();
			break;
		}
	}

	// Do not process if HotkeyID is NOT available
	if (nHKActionID == INT_NULL) {
		TRACE("[LockState Hotkey] Hotkey not found!!!");
		return FALSE;
	}

	// Output debug log
	OutputDebugLogFormat(_T("[LockState Hotkey] HotkeyID found: HKeyID=0x%04X (%d)"), nHKActionID, nHKActionID);
		
	// Check if HotkeyID is registered
	if (GetFlagValue(FLAGID_HOTKEY_REGISTERED) != TRUE) {
		// Trace error
		TRACE_ERROR("[LockState Hotkey] No hotkey registered!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Only process if HotkeyID is registered
	for (INT_PTR nIndex = 0; nIndex < m_arrCurRegHKeyList.GetSize(); nIndex++) {
		UINT nRegHKeyID = m_arrCurRegHKeyList.GetAt(nIndex);
		if (nRegHKeyID == nHKActionID) {
			// Process Hotkey by ID
			return ProcessHotkey(nHKActionID);
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
// Power Reminder feature functions

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
	if (GetAppOption(OPTIONID_ENABLE_PWRREMINDER) == FALSE) {
		TRACE("Power Reminder option is not enabled!!!");
		return FALSE;
	}

	// If there's no item, do nothing
	int nItemNum = m_prdReminderData.GetItemNum();
	if (nItemNum <= 0) {
		TRACE("There is no Power Reminder items!!!");
		return FALSE;
	}

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
		if (pwrCurItem.GetEventID() != nExecEventID)
			continue;

		// If item is not enabled, skip this item
		if (pwrCurItem.IsEnabled() == FALSE)
			continue;

		// Process item
		PWRREMINDERITEM pwrDispItem;
		switch (nExecEventID)
		{
		case PREVT_AT_SETTIME:
			// If item is set to repeat but not set active in current day of week
			if ((pwrCurItem.IsRepeatEnabled() == TRUE) && (!pwrCurItem.IsDayActive((DAYOFWEEK)curSysTime.wDayOfWeek)))
				continue;
			// If set time matching or snooze time is triggered
			if ((CheckTimeMatch(curSysTime, pwrCurItem.GetTime())) ||
				(GetPwrReminderSnoozeStatus(pwrCurItem.GetItemID(), curSysTime))) {
				// Prepare to display
				pwrDispItem.Copy(pwrCurItem);
				SetPwrReminderSnooze(pwrCurItem, FLAG_OFF);
			}
			else continue;
			break;
		case PREVT_AT_SYSWAKEUP:
			// If System suspend flag and Session ending flag are both OFF, do not display
			if ((GetSystemSuspendFlag() == FLAG_OFF) && (GetSessionEndFlag() == FLAG_OFF)) continue;
			// Otherwise, just prepare to display
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
			if (pwrDispItem.IsRepeatEnabled() == FALSE) {
				bTriggerReupdate |= TRUE;
				pwrCurItem.EnableItem(FALSE);
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

int CPowerPlusDlg::DisplayPwrReminder(const PWRREMINDERITEM& pwrDispItem)
{
	// Check message content validity
	CString strMsgContent = pwrDispItem.GetMessage();
	if ((strMsgContent.IsEmpty()) ||
		(IS_NULL_STRING(strMsgContent))) {
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
	if (pwrDispItem.GetMessageStyle() == PRSTYLE_MSGBOX) {

		CString strCaption = GetLanguageString(pAppLang, IDD_PWRREMINDER_DLG);
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		nRespond = DisplayMessageBox(strMsgContent, strCaption, dwMsgStyle);
	}
	// Style: Dialog
	else if (pwrDispItem.GetMessageStyle() == PRSTYLE_DIALOG) {

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
		BYTE byIconPlacement = GetReminderMsgIconPlacement();

		// Message auto-close interval
		int nTimeout = GetReminderMsgTimeout();

		// Allow snooze mode
		BOOL bAllowSnooze = pwrDispItem.IsAllowSnoozing();

		// Set allow snooze mode
		pMsgDlg->SetAllowSnoozeMode(bAllowSnooze);

		// Set properties
		pMsgDlg->SetCaptionFromLanguage(IDD_PWRREMINDER_DLG);
		pMsgDlg->SetDispMessage(strMsgContent);
		pMsgDlg->SetBkgrdColor(clrMsgBkgrd);
		pMsgDlg->SetTextColor(clrMsgText);
		pMsgDlg->SetMsgFont(strFontName, nFontSize);
		pMsgDlg->SetMsgIcon(nIconID, nIconSize);
		pMsgDlg->SetMsgIconPlacement(byIconPlacement);
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

	// Update item runtime data flag
	SetPwrReminderSnooze(pwrDispItem, nRetFlag);
	SetPwrReminderDispFlag(pwrDispItem, FLAG_OFF);

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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetPwrReminderSnooze
//	Description:	Setup Power Reminder item snooze mode
//  Arguments:		pwrItem		- Power Reminder item
//					nSnoozeFlag - Snooze flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetPwrReminderSnooze(const PWRREMINDERITEM& pwrItem, int nSnoozeFlag)
{
	// If item is empty, do nothing
	if (pwrItem.IsEmpty()) return;

	// If item event ID is not "At set time", do nothing
	if (pwrItem.GetEventID() != PREVT_AT_SETTIME)
		return;

	// If snoozing option is not available
	if (pwrItem.IsAllowSnoozing() != TRUE) {
		// Turn off flag
		nSnoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int nInterval = pwrItem.GetSnoozeInterval();

	// Find if item snooze mode is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == pwrItem.GetItemID()) {

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);

			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.SetTime(GetCurSysTime());
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(FID_PWRREMINDERITEM);
	pwrRuntimeItem.SetItemID(pwrItem.GetItemID());
	pwrRuntimeItem.SetSnoozeFlag(nSnoozeFlag);
	if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.SetTime(GetCurSysTime());
		pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
	}

	// Add item to runtime queue
	m_arrRuntimeQueue.Add(pwrRuntimeItem);
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
	if (nMode == MODE_INIT) {

		// Initialize Power Reminder runtime snooze queue
		// Set all items as not snoozed
		for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
			SetPwrReminderSnooze(m_prdReminderData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == MODE_UPDATE) {

		// Update snooze queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.GetSize() - 1); nQueueIdx >= 0; nQueueIdx--) {
			
			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nQueueIdx);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

			// Search for item ID in Power Reminder data
			BOOL bItemFound = FALSE;
			for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
				const PWRREMINDERITEM& pwrItem = m_prdReminderData.GetItemAt(nItemIdx);
				if (pwrItem.GetItemID() == pwrRuntimeItem.GetItemID()) {

					// If item's snoozing mode is no longer available
					if (!pwrItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
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
				m_arrRuntimeQueue.RemoveAt(nQueueIdx);
			}
		}

		// Free extra memory
		m_arrRuntimeQueue.FreeExtra();
	}
	else if (nMode == MODE_DISABLE) {

		// Disable snooze mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

			// Update item snooze mode data
			pwrRuntimeItem.SetSnoozeFlag(FLAG_OFF);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPwrReminderSnoozeStatus
//	Description:	Get Power Reminder item snooze trigger status
//  Arguments:		nItemID	   - Power Reminder item ID
//					curSysTime - Current system time
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::GetPwrReminderSnoozeStatus(UINT nItemID, SYSTEMTIME& curSysTime)
{
	// If runtime queue data is empty, do not trigger
	if (m_arrRuntimeQueue.IsEmpty())
		return FALSE;

	// Find for item ID
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		const PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.GetSnoozeFlag() == FLAG_OFF)
				return FALSE;
			
			// Check for next snooze time matching
			if (CheckTimeMatch(curSysTime, pwrRuntimeItem.GetTime()))
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetPwrReminderDispFlag
//	Description:	Set Power Reminder item runtime displaying flag
//  Arguments:		pwrItem	  - Power Reminder item
//					nDispFlag - Display flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SetPwrReminderDispFlag(const PWRREMINDERITEM& pwrItem, int nDispFlag)
{
	// If item is empty, do nothing
	if (pwrItem.IsEmpty()) return;

	// Find if item runtime data is already setup
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.GetItemID() == pwrItem.GetItemID()) {
			// Update item displaying flag
			pwrRuntimeItem.SetDisplayFlag(nDispFlag);
			return;
		}
	}

	// Prepare item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.SetCategory(FID_PWRREMINDERITEM);
	pwrRuntimeItem.SetItemID(pwrItem.GetItemID());
	pwrRuntimeItem.SetDisplayFlag(nDispFlag);

	// Add item to runtime queue
	m_arrRuntimeQueue.Add(pwrRuntimeItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetPwrReminderDispList
//	Description:	Get Power Reminder runtime displaying item list
//  Arguments:		arrPwrDispList - Power Reminder displaying item list
//  Return value:	INT_PTR - Number of displaying items
//
//////////////////////////////////////////////////////////////////////////

INT_PTR CPowerPlusDlg::GetPwrReminderDispList(CUIntArray& arrPwrDispList)
{
	// Reset output data list
	arrPwrDispList.RemoveAll();
	arrPwrDispList.FreeExtra();

	// If runtime queue data is empty, return no item
	if (m_arrRuntimeQueue.IsEmpty())
		return INT_NULL;

	// Get runtime displaying item ID list
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		const PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.GetCategory() != FID_PWRREMINDERITEM) continue;

		// If item displaying flag is marked as ON
		if (pwrRuntimeItem.GetDisplayFlag() == FLAG_ON) {
			// Add item ID into output data list
			arrPwrDispList.Add(pwrRuntimeItem.GetItemID());
		}
	}

	// Return number of displaying items
	return arrPwrDispList.GetSize();
}


//////////////////////////////////////////////////////////////////////////
// History and logging functions

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputScheduleEventLog
//	Description:	Output action schedule event log
//  Arguments:		usEvent	- Event ID
//					schItem - Schedule item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OutputScheduleEventLog(USHORT usEvent, const SCHEDULEITEM& schItem)
{
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Schedule action name
	int nActionNameID = GetPairedID(IDTable::ActionName, schItem.GetAction());
	CString strActionName = GetLanguageString(pAppLang, nActionNameID);

	// Detail info
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_CONTENTID;
	logDetail.uiDetailInfo = schItem.GetItemID();
	LOGDETAILINFO logDetailInfo;
	logDetailInfo.Add(logDetail);

	// Output event log
	OutputEventLog(usEvent, strActionName, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputPwrReminderEventLog
//	Description:	Output Power Reminder event log
//  Arguments:		usEvent	- Event ID
//					pwrItem - Power Reminder item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::OutputPwrReminderEventLog(USHORT usEvent, const PWRREMINDERITEM& pwrItem)
{
	// Message content
	CString strMsgContent = pwrItem.GetMessage();

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Item ID
	LOGDETAIL logDetail;
	logDetail.usCategory = EVENTLOG_DETAIL_CONTENTID;
	logDetail.uiDetailInfo = pwrItem.GetItemID();
	logDetailInfo.Add(logDetail);

	// Output event log
	OutputEventLog(usEvent, strMsgContent, &logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitPwrActionHistoryInfo
//	Description:	Initialize Power Action history info data
//  Arguments:		nActionID	- Action macro ID
//					bResult		- Result of execution
//					dwErrorCode	- Error code
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::InitPwrActionHistoryInfo(UINT nActionID, BOOL bResult, DWORD dwErrorCode)
{
	// Initialize action history info data to save logs
	m_hidHistoryInfoData.Init(HISTORYLOG_CATE_PWRACTION);
	m_hidHistoryInfoData.m_nActionNameID = nActionID;

	// Collect action result
	m_hidHistoryInfoData.m_bActionResult = bResult;
	m_hidHistoryInfoData.m_dwErrorCode = dwErrorCode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitScheduleHistoryInfo
//	Description:	Initialize schedule history info data
//  Arguments:		schItem - Schedule item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::InitScheduleHistoryInfo(const SCHEDULEITEM& schItem)
{
	// Check item validity
	if (schItem.IsEmpty())
		return;

	// Get schedule action name ID
	UINT nActionNameID = NULL;
	switch (schItem.GetAction())
	{
	case APP_ACTION_NOTHING:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_DONOTHING;
		break;
	case APP_ACTION_DISPLAYOFF:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_DISPLAYOFF;
		break;
	case APP_ACTION_SLEEP:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SLEEP;
		break;
	case APP_ACTION_SHUTDOWN:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SHUTDOWN;
		break;
	case APP_ACTION_RESTART:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_RESTART;
		break;
	case APP_ACTION_SIGNOUT:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SIGNOUT;
		break;
	case APP_ACTION_HIBERNATE:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_HIBERNATE;
		break;
	}

	// Initialize schedule history info data
	m_hidHistoryInfoData.Init(HISTORYLOG_CATE_SCHEDULE);
	m_hidHistoryInfoData.m_nItemID = schItem.GetItemID();
	m_hidHistoryInfoData.m_nActionNameID = nActionNameID;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitHotkeyHistoryInfo
//	Description:	Initialize hotkeyset history info data
//  Arguments:		nHKID - Hotkey action ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::InitHotkeyHistoryInfo(UINT nHKID)
{
	// Get HotkeySet item by ID
	HOTKEYSETITEM hksItem;
	for (int nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM& hksTemp = m_hksHotkeySetData.GetItemAt(nIndex);
		if (hksTemp.GetActionID() == nHKID) {
			hksItem.Copy(hksTemp);
			break;
		}
	}

	// Check item validity
	if (hksItem.IsEmpty())
		return;

	// Get hotkey action name ID
	UINT nActionNameID = NULL;
	switch (hksItem.GetActionID())
	{
	case HKID_DISPLAYOFF:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_DISPLAYOFF;
		break;
	case HKID_SLEEP:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SLEEP;
		break;
	case HKID_SHUTDOWN:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SHUTDOWN;
		break;
	case HKID_RESTART:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_RESTART;
		break;
	case HKID_SIGNOUT:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SIGNOUT;
		break;
	case HKID_HIBERNATE:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_HIBERNATE;
		break;
	default:
		break;
	}

	// Initialize hotkey action history info
	m_hidHistoryInfoData.Init(HISTORYLOG_CATE_HOTKEYSET);
	m_hidHistoryInfoData.m_nActionNameID = nActionNameID;
	hksItem.PrintKeyStrokes(m_hidHistoryInfoData.m_strDescription);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	InitPwrReminderHistoryInfo
//	Description:	Initialize reminder displaying history info data
//  Arguments:		pwrItem - Power Reminder item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::InitPwrReminderHistoryInfo(const PWRREMINDERITEM& pwrItem)
{
	// Check item validity
	if (pwrItem.IsEmpty()) return;

	// Initialize history info data
	m_hidHistoryInfoData.Init(HISTORYLOG_CATE_PWRREMINDER);
	m_hidHistoryInfoData.m_nItemID = pwrItem.GetItemID();
	m_hidHistoryInfoData.m_strDescription.Format(STRING_QUOTEFORMAT, pwrItem.GetMessage());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveHistoryInfoData
//	Description:	Save history info data log file
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::SaveHistoryInfoData(void)
{
	// If history info data is empty (not yet initialized), do nothing
	if (m_hidHistoryInfoData.m_bInitState == FALSE)
		return;

	// Prepare common history log info
	LOGITEM actionLogItem;
	actionLogItem.stTime = m_hidHistoryInfoData.m_stTimestamp;

	// Get current process ID
	actionLogItem.dwProcessID = GetCurrentProcessId();

	// Get action name string
	CString strActionName = STRING_NULL;
	LoadResourceString(strActionName, m_hidHistoryInfoData.m_nActionNameID);

	// In case of power action history
	CString strActionResult = STRING_NULL;
	if (m_hidHistoryInfoData.m_nCategoryID == HISTORYLOG_CATE_PWRACTION) {

		// Get action result string
		if (m_hidHistoryInfoData.m_bActionResult == TRUE) {
			// Action succeed
			LoadResourceString(strActionResult, IDS_HISTORYLOG_RESULT_SUCCESS);
		}
		else {
			// Action failed
			CString strTemp;
			if (LoadResourceString(strTemp, IDS_HISTORYLOG_RESULT_FAILED_ERRCODE)) {
				// Attach error code
				strActionResult.Format(strTemp, m_hidHistoryInfoData.m_dwErrorCode);
			}
		}
	}

	// Prepare log string by category ID
	CString strTemplate = STRING_NULL;
	CString strLogString = STRING_NULL;
	switch (m_hidHistoryInfoData.m_nCategoryID)
	{
	case HISTORYLOG_CATE_PWRACTION:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_PWRACTION;
		strTemplate.LoadString(IDS_HISTORYLOG_PWRACTION_TEMPLATE);
		strLogString.Format(strTemplate, strActionName, strActionResult);
		actionLogItem.strLogString = strLogString;
		break;
	case HISTORYLOG_CATE_SCHEDULE:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_SCHEDULE;
		strTemplate.LoadString(IDS_HISTORYLOG_SCHEDULE_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.m_nItemID, strActionName);
		actionLogItem.strLogString = strLogString;
		break;
	case HISTORYLOG_CATE_HOTKEYSET:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_HOTKEY;
		strTemplate.LoadString(IDS_HISTORYLOG_HOTKEYSET_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.m_strDescription, strActionName);
		actionLogItem.strLogString = strLogString;
		break;
	case HISTORYLOG_CATE_PWRREMINDER:
		actionLogItem.usCategory = LOG_HISTORY_DISP_PWRREMINDER;
		strTemplate.LoadString(IDS_HISTORYLOG_PWRREMINDER_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.m_nItemID, m_hidHistoryInfoData.m_strDescription);
		actionLogItem.strLogString = strLogString;
		break;
	default:
		break;
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

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ConfirmActionExec
//	Description:	Show confirmation message before executing action
//  Arguments:		nActionType - Type of action
//					nActionID	- ID of action
//  Return value:	int	- Result of confirmation message
//
//////////////////////////////////////////////////////////////////////////

int CPowerPlusDlg::ConfirmActionExec(UINT nActionType, UINT nActionID)
{
	// If "Confirm action" option is not enabled, return YES
	if (GetAppOption(OPTIONID_CONFIRM_ACTION) == FALSE) {
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
	UINT nMsgStringID = GetPairedID(IDTable::ActionMessage, nActionID);
	int nResult = DisplayMessageBox(nMsgStringID, (UINT)NULL, MB_YESNO | MB_ICONQUESTION);

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
	// Do not notify if schedule action is "Do nothing"
	if (pschItem->GetAction() == APP_ACTION_NOTHING) {
		TRACE("Do not notify when schedule action is DO NOTHING");
		return INT_INVALID;
	}

	// Get action info
	UINT nActionStringID = GetPairedID(IDTable::ScheduleNotifyMessage, pschItem->GetAction());

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Format message
	CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CAPTION);
	CString strMsgScheduleAction = GetLanguageString(pAppLang, nActionStringID);
	CString strMsgTemplate = GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_NOTIFY);
	
	CString strMsgContent;
	strMsgContent.Format(strMsgTemplate, strMsgScheduleAction);

	// Allow cancelling schedule when notify
	BOOL bAllowCancel = GetAppOption(OPTIONID_ALLOW_CANCEL_SCHEDULE);
	if (bAllowCancel == TRUE)
	{
		// Update message content
		strMsgContent += GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_ALLOWCANCEL);
		int nRespond = DisplayMessageBox(strMsgContent, strMsgCaption, MB_OKCANCEL | MB_ICONINFORMATION);
		if (nRespond == IDCANCEL)
		{
			// Set item skip flag
			SetActionScheduleSkip(*pschItem, FLAG_ON);

			// If "Repeat" option is not ON,
			// --> Disable schedule item after canceling
			if (pschItem->IsRepeatEnabled() == FALSE) {
				// Deactivate schedule
				pschItem->EnableItem(FALSE);
				bReupdate = TRUE;
			}

			// Output event log: Schedule canceled
			OutputScheduleEventLog(LOG_EVENT_CANCEL_SCHEDULE, *pschItem);

			// Display notify message
			DisplayMessageBox(MSGBOX_PROCESSSCHEDULE_CANCELED, MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONINFORMATION);
		}

		return nRespond;
	}

	// Show message
	return DisplayMessageBox(strMsgContent, strMsgCaption, MB_OK | MB_ICONINFORMATION);
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
	if (GetAppOption(OPTIONID_SHOW_ERROR_MSG) == FALSE) {
		TRACE("Show error message option is not enabled!!!");
		return;
	}

	// Get window handle and language ID
	HWND hWnd = this->GetSafeHwnd();
	UINT nCurLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguageOption();

	// Show error message
	AppCore::ShowErrorMessage(hWnd, nCurLang, dwError);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestRestartApp
//	Description:	Request to restart the application
//	Arguments:		uiCmdSenderID	- ID of command which sends request
//					bRestartAsAdmin - Restart as admin???
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RequestRestartApp(UINT uiCmdSenderID, BOOL bRestartAsAdmin)
{
	// Init request data
	RESTARTREQ reqRestart;
	ZeroMemory(&reqRestart, sizeof(RESTARTREQ));

	// Request from [Apply] button
	if (uiCmdSenderID == IDC_APPLY_BTN) {
		BOOL bRestartTrigger = GetFlagValue(FLAGID_RESTART_AS_ADMIN);
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
//	Description:	Request to restart the application as admin
//	Arguments:		reqRestart - Request to restart
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPowerPlusDlg::RequestRestartAsAdmin(RESTARTREQ reqRestart)
{
	// If there's no request, do nothing
	if (reqRestart.bRequest == FALSE) {
		TRACE("There is no restart request!!!");
		return;
	}

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	CString strRequestMsg = GetLanguageString(pAppLang, MSGBOX_OTHER_REQUEST_RESTARTASADMIN);
	CString strCaption = ((CPowerPlusApp*)AfxGetApp())->GetAppWindowCaption();
	CString strMsgFormat = strRequestMsg;

	// Check if the application is currently running as admin
	BOOL bIsAdmin = FALSE;
	if (reqRestart.bAdminCheck == TRUE) {
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
			SetFlagValue(FLAGID_RESTART_AS_ADMIN, FALSE);
		}
	}
}

