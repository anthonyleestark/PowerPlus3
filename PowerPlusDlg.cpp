
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

	// Initialize boolean variables for checkboxes
	m_bShowDlgAtStartup = FALSE;
	m_bStartupEnabled = FALSE;
	m_bConfirmAction = FALSE;
	m_bSaveHistoryLog = FALSE;
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

	// Clean Power++ runtime queue data
	m_arrRuntimeQueue.RemoveAll();
	m_arrRuntimeQueue.FreeExtra();

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
		TRCLOG("Error: Register dialog management failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_MAIN_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_LEFTMOUSE_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_LMBACTION_LIST, CTRL_TYPE_COMBOBOX);
		nRet = pCtrlMan->AddControl(IDC_MIDMOUSE_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_MMBACTION_LIST, CTRL_TYPE_COMBOBOX);
		nRet = pCtrlMan->AddControl(IDC_RIGHTMOUSE_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_RMBACTION_LIST, CTRL_TYPE_COMBOBOX);
		nRet = pCtrlMan->AddControl(IDC_ENABLERMBMENU_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_APPLY_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_RELOAD_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_EXIT_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_EXPAND_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_COLLAPSE_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_SYSTEM_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_SHOWATSTARTUP_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_ENABLEAUTOSTART_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_CONFIRMACTION_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_SAVEHISTORYLOG_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_SAVEAPPEVENTLOG_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_RUNASADMIN_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_SHOWERROR_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_SCHEDNOTIFY_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_SCHEDALLOWCANCEL_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_ENBBKGRDHOTKEYS_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_ENBPWRREMINDER_CHK, CTRL_TYPE_CHECKBOX);
		nRet = pCtrlMan->AddControl(IDC_LANGUAGE_TITLE, CTRL_TYPE_STATIC);
		nRet = pCtrlMan->AddControl(IDC_LANGUAGE_LIST, CTRL_TYPE_COMBOBOX);
		nRet = pCtrlMan->AddControl(IDC_VIEWACTIONLOG_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_BACKUPCFG_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_SCHEDULE_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_LOGVIEWER_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_PWRREMINDER_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_HELP_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_ABOUT_BTN, CTRL_TYPE_BUTTON);
		nRet = pCtrlMan->AddControl(IDC_DEFAULT_BTN, CTRL_TYPE_BUTTON);
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
	IDMAP_ADD(IDC_SAVEHISTORYLOG_CHK,			"SaveAppHistoryCheck")
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
	// First, init with base class method
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

	// Set main window app title
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp != NULL) {
		SetDialogCaption(pApp->GetAppWindowTitle());
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

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, GetDialogCaption());

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

	// Update dialog control management
	UpdateDialogManagement();

	// Initialize dialog state as collapsed
	ExpandDialog(FALSE);

	// Initialize Power++ runtime queue
	UpdateActionScheduleQueue(DEF_MODE_INIT);
	UpdatePwrReminderSnooze(DEF_MODE_INIT);

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
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}
	// Help dialog
	if (m_pHelpDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHelpDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}
	// LogViewer dialog
	if (m_pLogViewerDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pLogViewerDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}
	// Multi schedule dialog
	if (m_pMultiScheduleDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pMultiScheduleDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}
	// HotkeySet dialog
	if (m_pHotkeySetDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pHotkeySetDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}
	// Power Reminder dialog
	if (m_pPwrReminderDlg != NULL) {
		// Request close dialog
		resCloseReq = m_pPwrReminderDlg->RequestCloseDialog();
		if (resCloseReq != DEF_RESULT_SUCCESS)
			return resCloseReq;
	}

	// Can not destroy if Power Reminder messages are currently displaying
	CUIntArray arrPwrDispItemList;
	if (GetPwrReminderDispList(arrPwrDispItemList) > 0) {
		// Display notify message
		DisplayMessageBox(MSGBOX_OTHER_PREDESTROY_REMINDERDISP, MSGBOX_PWRREMINDER_CAPTION);
		return DEF_RESULT_FAILED;
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

	return DEF_RESULT_SUCCESS;
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
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, GetDialogCaption());

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
	int nRetPreDestroy = PreDestroyDialog();
	if (nRetPreDestroy != DEF_RESULT_SUCCESS)
		return;

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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXPAND_BTN);

	// Expand/collapse dialog
	BOOL bCurState = GetFlagValue(FLAGID_DLGEXPANDED);
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
	UINT nCurLanguage = GetAppOption(OPTIONID_LANGUAGEID, TRUE);
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
	OpenFileToView(FILENAME_HISTORY_LOG, SUBFOLDER_LOG);
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
	OpenFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
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
			UpdateActionScheduleQueue(DEF_MODE_UPDATE);
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
	DWORD dwErrorCode = DEF_DBG_ERROR_SUCCESS;
	if (!ProcessDebugCommand((LPCTSTR)lParam, dwErrorCode)) {

		// Reply failed message
		if (dwErrorCode == DEF_DBG_ERROR_INVALID_COMMAND) {
			// Error: Invalid command
			OutputDebugLog(_T("Invalid command!!!"));
		}
		else {
			// Reply corresponding error code
			OutputDebugLogFormat(_T("Failed! (Error code: 0x%X)"), dwErrorCode);
		}

		return LRESULT(DEF_RESULT_FAILED);
	}

	// Notify app class about debug command execution
	::PostMessage(NULL, SM_APP_DEBUGCMDEXEC, wParam, lParam);

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
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}

		// Save action history if remaining unsaved
		SaveHistoryInfoData();
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
	SaveHistoryInfoData();

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
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_DISPLAYOFF);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_DISPLAYOFF);
		break;
	case IDM_NOTIFY_ACTION_SLEEP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SLEEP);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SLEEP);
		break;
	case IDM_NOTIFY_ACTION_SHUTDOWN:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SHUTDOWN);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SHUTDOWN);
		break;
	case IDM_NOTIFY_ACTION_RESTART:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_RESTART);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_RESTART);
		break;
	case IDM_NOTIFY_ACTION_SIGNOUT:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SIGNOUT);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_SIGNOUT);
		break;
	case IDM_NOTIFY_ACTION_HIBERNATE:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_HIBERNATE);
		ExecuteAction(DEF_APP_MACRO_ACTION_MENU, DEF_APP_ACTION_HIBERNATE);
		break;
	case IDM_NOTIFY_ACTION_SCHEDULE:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SCHEDULE);
		ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE, GetAppOption(OPTIONID_SCHEDULEACTION));
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
		OpenFileToView(FILENAME_HISTORY_LOG, FILEEXT_LOGFILE, SUBFOLDER_LOG);
		break;
	case IDM_NOTIFY_BACKUP_CONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_BACKUP_CONFIG);
		((CPowerPlusApp*)AfxGetApp())->BackupRegistryAppData();
		break;
	case IDM_NOTIFY_VIEW_BAKCONFIG:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_BAKCONFIG);
		OpenFileToView(FILENAME_BAKCONFIG, FILEEXT_REGFILE);
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
		RequestRestart(IDM_NOTIFY_RESTART_APP, FALSE);
		break;
	case IDM_NOTIFY_RESTART_ASADMIN:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_ASADMIN);
		RequestRestart(IDM_NOTIFY_RESTART_ASADMIN, TRUE);
		break;
	case IDM_NOTIFY_SHOW_WINDOW:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_SHOW_WINDOW);
		ExpandDialog(FALSE);
		PostMessage(SM_WND_SHOWDIALOG, TRUE);
		break;
	case IDM_NOTIFY_EXIT_APP:
		OutputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_EXIT_APP);
		if (PreDestroyDialog() == DEF_RESULT_SUCCESS) {
			PostMessage(WM_DESTROY);
			EndDialog(IDCANCEL);
		} break;
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
			OutputEventLog(LOG_EVENT_TRAYICO_LMBCLICKED);
			ExecuteAction(DEF_APP_MACRO_LEFT_MOUSE);
			break;
		case WM_MBUTTONDOWN:
			OutputEventLog(LOG_EVENT_TRAYICO_MMBCLICKED);
			ExecuteAction(DEF_APP_MACRO_MIDDLE_MOUSE);
			break;
		case WM_RBUTTONUP:
			OutputEventLog(LOG_EVENT_TRAYICO_RMBCLICKED);
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
	OutputEventLog(nEventID, GetDialogCaption());

	// Update flag
	BOOL bNewState = !bCurState;
	SetFlagValue(FLAGID_DLGEXPANDED, bNewState);
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

void CPowerPlusDlg::SetNotifyIcon(void)
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
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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
		TRCLOG("Error: Show notify menu failed!!!");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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
	if (m_pNotifyIconData == NULL)
		return;

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

int CPowerPlusDlg::GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp /* = FALSE */) const
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
	case OPTIONID_SAVEHISTORYLOG:
		nResult = m_cfgAppConfig.bSaveHistoryLog;
		nTempResult = m_cfgTempConfig.bSaveHistoryLog;
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
	case OPTIONID_SCHEDULEACTIVE:
		nResult = m_schScheduleData.GetDefaultItem().bEnable;
		nTempResult = nResult;		// No temp data
		break;
	case OPTIONID_SCHEDULEACTION:
		nResult = m_schScheduleData.GetDefaultItem().nAction;
		nTempResult = nResult;		// No temp data
		break;
	case OPTIONID_SCHEDULEREPEAT:
		nResult = m_schScheduleData.GetDefaultItem().IsRepeatEnable();
		nTempResult = nResult;		// No temp data
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

		// Update data for dialog control management
		UpdateDialogManagement();

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
	if (nValue == DEF_INTEGER_INVALID)
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
	if (pApp != NULL) {
		bCheck = pApp->GetAppOption(OPTIONID_SAVEAPPEVENTLOG);
	}
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
			bShowItem = GetAppOption(OPTIONID_SCHEDULEACTIVE);
			bShowItem &= (GetAppOption(OPTIONID_SCHEDULEACTION) != DEF_APP_ACTION_NOTHING);
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
	UINT nActionNameResID = 0;
	UINT nActionNameLangID = 0;
	UINT nMessage = 0;

	// Output debug log
	OutputDebugLogFormat(_T("Execute action: Type = %d, Param = %d"), nActionType, (UINT)wParam);

	// Get action ID
	switch (nActionType)
	{
	case DEF_APP_MACRO_LEFT_MOUSE:
		// Get action ID: Left mouse
		nActionID = GetAppOption(OPTIONID_LMBACTION);
		break;
	case DEF_APP_MACRO_MIDDLE_MOUSE:
		// Get action ID: Middle mouse
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
		// Otherwise, get action ID: Right mouse
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
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_DISPLAYOFF;
		nActionNameLangID = ACTION_NAME_DISPLAYOFF;
		break;
	case DEF_APP_ACTION_SLEEP:
		// Sleep
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SLEEP;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SLEEP;
		nActionNameLangID = ACTION_NAME_SLEEP;
		break;
	case DEF_APP_ACTION_SHUTDOWN:
		// Shutdown
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SHUTDOWN;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SHUTDOWN;
		nActionNameLangID = ACTION_NAME_SHUTDOWN;
		break;
	case DEF_APP_ACTION_RESTART:
		// Restart
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_REBOOT;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_RESTART;
		nActionNameLangID = ACTION_NAME_RESTART;
		break;
	case DEF_APP_ACTION_SIGNOUT:
		// Sign out
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_SIGNOUT;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_SIGNOUT;
		nActionNameLangID = ACTION_NAME_SIGNOUT;
		break;
	case DEF_APP_ACTION_HIBERNATE:
		// Hibernate
		nAction = DEF_APP_ACTIONTYPE_POWER;
		nMessage = DEF_APP_MESSAGE_HIBERNATE;
		nActionNameResID = IDS_HISTORYLOG_PWRACTION_HIBERNATE;
		nActionNameLangID = ACTION_NAME_HIBERNATE;
		break;
	default:
		return FALSE;
	}

	// Initialize action history info data to save logs
	m_hidHistoryInfoData.Init(HSTRCATE_PWRACTION);
	m_hidHistoryInfoData.nActionNameID = nActionNameResID;

	BOOL bResult = FALSE;
	DWORD dwErrorCode = DEF_APP_ERROR_SUCCESS;
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
			pApp->SaveGlobalVars(DEF_GLBVAR_CATE_APPFLAG);
		}

		// Collect action result
		m_hidHistoryInfoData.bActionResult = bResult;
		m_hidHistoryInfoData.dwErrorCode = dwErrorCode;

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
	m_bChangeFlag = FALSE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	ShowDialog
//	Description:	Show/hide dialog out of/into system tray
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
	OutputEventLog(nEventID, ((SDialog*)pDialogWnd)->GetDialogCaption());
}

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
	if (pWnd == NULL) return;

	// Show/hide dialog
	pWnd->ShowWindow(nCmdShow);
	if (nCmdShow == SW_SHOW) {
		pWnd->BringWindowToTop();
	}

	// Save app event log if enabled
	UINT nEventID = (bShowFlag == TRUE) ? LOG_EVENT_DLG_SHOWED : LOG_EVENT_DLG_HIDDEN;
	OutputEventLog(nEventID, ((SDialog*)pWnd)->GetDialogCaption());
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
	// Get app pointer
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
				ShowDialog(pDialog->GetSafeHwnd(), TRUE);
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
//					lpszExtension - File extension
//					lpszSubDir	- Sub-directory name
//  Return value:	BOOL - Result of file opening
//
//////////////////////////////////////////////////////////////////////////

BOOL CPowerPlusDlg::OpenFileToView(LPCTSTR lpszFileName, LPCTSTR lpszExtension, LPCTSTR lpszSubDir /* = DEF_STRING_EMPTY */)
{
	// Get file name
	VERIFY(lpszFileName != NULL);
	CString strFilePath = (CString)lpszFileName + lpszExtension;
	
	// If sub-directory name is not empty
	if (_tcscmp(lpszSubDir, DEF_STRING_EMPTY)) {
		// Format file path with sub-directory
		MakeFilePath(strFilePath, lpszSubDir, lpszFileName, lpszExtension);
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
		if (schDefaultItem.bEnable == FALSE) {
			// Do not process
			bSkipProcess = TRUE;
		}

		// If repeat option is ON and is set as active in current day of week
		if ((schDefaultItem.IsRepeatEnable() == TRUE) && (!schDefaultItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek))) {
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
				
				// Check if item is marked as skipped
				BOOL bSkipFlag = GetActionScheduleSkipStatus(schDefaultItem.nItemID);
				if (bSkipFlag != TRUE) {

					// Output event log: Schedule executed
					OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schDefaultItem);

					// Save history info data
					InitScheduleHistoryInfo(schDefaultItem);
					SaveHistoryInfoData();

					// Execute schedule action
					bResult = ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE, schDefaultItem.nAction);

					// If "Repeat" option is not ON,
					// --> Disable schedule item after done
					if (schDefaultItem.IsRepeatEnable() == FALSE) {
						schDefaultItem.SetActiveState(FALSE);
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
		if ((schExtraItem.IsRepeatEnable() == TRUE) && (!schExtraItem.IsDayActive((DAYOFWEEK)stCurrentTime.wDayOfWeek)))
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

			// Check if item is marked as skipped
			BOOL bSkipFlag = GetActionScheduleSkipStatus(schExtraItem.nItemID);
			if (bSkipFlag != TRUE) {

				// Output event log: Schedule executed
				OutputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, schExtraItem);

				// Save history info data
				InitScheduleHistoryInfo(schExtraItem);
				SaveHistoryInfoData();

				// Execute schedule action
				bResult = ExecuteAction(DEF_APP_MACRO_ACTION_SCHEDULE, schExtraItem.nAction);

				// If "Repeat" option is not ON,
				// --> Disable schedule item after done
				if (schExtraItem.IsRepeatEnable() == FALSE) {
					schExtraItem.SetActiveState(FALSE);
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
		if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == schItem.nItemID) {

			// Update item skip mode data
			pwrRuntimeItem.nSkipFlag = nSkipFlag;
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.nCategory = FID_SCHEDULEITEM;
	pwrRuntimeItem.nItemID = schItem.nItemID;
	pwrRuntimeItem.nSkipFlag = nSkipFlag;

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
		if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == schItem.nItemID) {

			// Update item skip mode data
			pwrRuntimeItem.nSnoozeFlag = nSnoozeFlag;

			if (pwrRuntimeItem.nSnoozeFlag == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.stNextSnoozeTime = GetCurSysTime();
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.nCategory = FID_SCHEDULEITEM;
	pwrRuntimeItem.nItemID = schItem.nItemID;
	pwrRuntimeItem.nSnoozeFlag = nSnoozeFlag;
	if (pwrRuntimeItem.nSnoozeFlag == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.stNextSnoozeTime = GetCurSysTime();
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
	if (nMode == DEF_MODE_INIT) {

		// Initialize Action Schedule runtime queue
		// Set all items as not skipped
		SetActionScheduleSkip(m_schScheduleData.GetDefaultItem(), FLAG_OFF);
		for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
			SetActionScheduleSkip(m_schScheduleData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == DEF_MODE_UPDATE) {

		// Update skip queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.GetSize() - 1); nQueueIdx >= 0; nQueueIdx--) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nQueueIdx);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

			// Item found flag
			BOOL bItemFound = FALSE;

			// Check if item ID is default schedule item
			const SCHEDULEITEM& schDefaultItem = m_schScheduleData.GetDefaultItem();
			if (schDefaultItem.nItemID == pwrRuntimeItem.nItemID) {

				// If item's snoozing mode is no longer available
				if (!schDefaultItem.IsAllowSnoozing()) {
					// Disable snooze mode
					pwrRuntimeItem.nSnoozeFlag = FLAG_OFF;
				}
				
				// Mark as found
				bItemFound = TRUE;
				break;
			}

			// Search for item ID in Action Schedule extra item data
			for (int nItemIdx = 0; nItemIdx < m_schScheduleData.GetExtraItemNum(); nItemIdx++) {
				const SCHEDULEITEM& schItem = m_schScheduleData.GetItemAt(nItemIdx);
				if (schItem.nItemID == pwrRuntimeItem.nItemID) {

					// If item's snoozing mode is no longer available
					if (!schItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.nSnoozeFlag = FLAG_OFF;
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
	else if (nMode == DEF_MODE_DISABLE) {

		// Disable skip mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

			// Skip if it's not an Action Schedule item
			if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

			// Update item skip mode data
			pwrRuntimeItem.nSkipFlag = FLAG_OFF;

			// Update item snooze mode data
			pwrRuntimeItem.nSnoozeFlag = FLAG_OFF;
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
		if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == nItemID) {

			// Get skip enable flag
			if (pwrRuntimeItem.nSkipFlag == FLAG_ON)
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
		if (pwrRuntimeItem.nCategory != FID_SCHEDULEITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.nSnoozeFlag == FLAG_OFF)
				return FALSE;

			// Check for next snooze time matching
			if (CheckTimeMatch(curSysTime, pwrRuntimeItem.stNextSnoozeTime))
				return TRUE;

			return FALSE;
		}
	}

	return FALSE;
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
					TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
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
	if (GetAppOption(OPTIONID_ENABLEHOTKEYSET) == FALSE)
		return FALSE;

	// Output debug log
	OutputDebugLogFormat(_T("Hotkey pressed: ID = %d"), nHotkeyID);

	// Get HotkeySet item by ID
	HOTKEYSETITEM hksItem;
	for (int nIndex = 0; nIndex < m_hksHotkeySetData.GetItemNum(); nIndex++) {
		HOTKEYSETITEM& hksTemp = m_hksHotkeySetData.GetItemAt(nIndex);
		if (hksTemp.nHKActionID == nHotkeyID) {
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

	// If Power action ID is invalid, do nothing
	if (nActionID == NULL)
		return FALSE;

	// Save history info data
	InitHotkeyHistoryInfo(nHotkeyID);
	SaveHistoryInfoData();
	
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

int CPowerPlusDlg::DisplayPwrReminder(const PWRREMINDERITEM& pwrDispItem)
{
	// Check message content validity
	CString strMsgContent = pwrDispItem.strMessage;
	if ((strMsgContent.IsEmpty()) ||
		(strMsgContent == DEF_STRING_NULL)) {
		// Invalid message content
		return DEF_INTEGER_INVALID;
	}

	// Output debug log
	OutputDebugLogFormat(_T("Display reminder: ItemID = %d"), pwrDispItem.nItemID);

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	int nRetFlag = FLAG_OFF;
	int nRespond = DEF_INTEGER_NULL;

	// Update item runtime displaying flag
	SetPwrReminderDispFlag(pwrDispItem, FLAG_ON);

	// Output event log: Display Power Reminder
	OutputPwrReminderEventLog(LOG_EVENT_DISP_PWRREMINDER, pwrDispItem);

	// Save history info data
	InitPwrReminderHistoryInfo(pwrDispItem);
	SaveHistoryInfoData();

	// Style: MessageBox
	if (pwrDispItem.dwMsgStyle == PRSTYLE_MSGBOX) {
		CString strCaption = GetLanguageString(pAppLang, IDD_PWRREMINDER_DLG);
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		nRespond = DisplayMessageBox(strMsgContent, strCaption, dwMsgStyle);
	}
	// Style: Dialog
	else if (pwrDispItem.dwMsgStyle == PRSTYLE_DIALOG) {
		// Init reminder message dialog
		CReminderMsgDlg* pMsgDlg = new CReminderMsgDlg(this);
		if (pMsgDlg == NULL) {
			// Trace error
			TRCLOG("Error: Reminder message dialog initialization failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			// Update item runtime displaying flag
			SetPwrReminderDispFlag(pwrDispItem, FLAG_OFF);
			return DEF_INTEGER_INVALID;
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
		BYTE byIconPos = GetReminderMsgIconPosition();

		// Message auto-close interval
		int nTimeout = GetReminderMsgTimeout();

		// Allow snooze mode
		BOOL bAllowSnooze = pwrDispItem.IsAllowSnoozing();

		// Set allow snooze mode
		pMsgDlg->SetAllowSnoozeMode(bAllowSnooze);

		// Set properties
		pMsgDlg->SetLangDialogCaption(IDD_PWRREMINDER_DLG);
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
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == pwrItem.nItemID) {

			// Update item snooze mode data
			pwrRuntimeItem.nSnoozeFlag = nSnoozeFlag;

			if (pwrRuntimeItem.nSnoozeFlag == FLAG_ON) {
				// Calculate next snooze trigger time
				pwrRuntimeItem.stNextSnoozeTime = GetCurSysTime();
				pwrRuntimeItem.CalcNextSnoozeTime(nInterval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.nCategory = FID_PWRREMINDERITEM;
	pwrRuntimeItem.nItemID = pwrItem.nItemID;
	pwrRuntimeItem.nSnoozeFlag = nSnoozeFlag;
	if (pwrRuntimeItem.nSnoozeFlag == FLAG_ON) {
		// Calculate next snooze trigger time
		pwrRuntimeItem.stNextSnoozeTime = GetCurSysTime();
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
	if (nMode == DEF_MODE_INIT) {

		// Initialize Power Reminder runtime snooze queue
		// Set all items as not snoozed
		for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
			SetPwrReminderSnooze(m_prdReminderData.GetItemAt(nItemIdx), FLAG_OFF);
		}
	}
	else if (nMode == DEF_MODE_UPDATE) {

		// Update snooze queue items
		for (int nQueueIdx = (m_arrRuntimeQueue.GetSize() - 1); nQueueIdx >= 0; nQueueIdx--) {
			
			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nQueueIdx);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

			// Search for item ID in Power Reminder data
			BOOL bItemFound = FALSE;
			for (int nItemIdx = 0; nItemIdx < m_prdReminderData.GetItemNum(); nItemIdx++) {
				const PWRREMINDERITEM& pwrItem = m_prdReminderData.GetItemAt(nItemIdx);
				if (pwrItem.nItemID == pwrRuntimeItem.nItemID) {

					// If item's snoozing mode is no longer available
					if (!pwrItem.IsAllowSnoozing()) {
						// Disable snooze mode
						pwrRuntimeItem.nSnoozeFlag = FLAG_OFF;
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
	else if (nMode == DEF_MODE_DISABLE) {

		// Disable snooze mode for all items in queue
		for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

			// Get runtime item from queue
			PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

			// Skip if it's not Power Reminder item
			if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

			// Update item snooze mode data
			pwrRuntimeItem.nSnoozeFlag = FLAG_OFF;
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
		if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == nItemID) {

			// Get snooze enable flag
			if (pwrRuntimeItem.nSnoozeFlag == FLAG_OFF)
				return FALSE;
			
			// Check for next snooze time matching
			if (CheckTimeMatch(curSysTime, pwrRuntimeItem.stNextSnoozeTime))
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
		if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

		// If item ID is found
		if (pwrRuntimeItem.nItemID == pwrItem.nItemID) {
			// Update item displaying flag
			pwrRuntimeItem.nDisplayFlag = nDispFlag;
			return;
		}
	}

	// Prepare item info to add
	PWRRUNTIMEITEM pwrRuntimeItem;
	pwrRuntimeItem.nCategory = FID_PWRREMINDERITEM;
	pwrRuntimeItem.nItemID = pwrItem.nItemID;
	pwrRuntimeItem.nDisplayFlag = nDispFlag;

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
		return DEF_INTEGER_NULL;

	// Get runtime displaying item ID list
	for (int nIndex = 0; nIndex < m_arrRuntimeQueue.GetSize(); nIndex++) {

		// Get runtime item from queue
		const PWRRUNTIMEITEM& pwrRuntimeItem = m_arrRuntimeQueue.GetAt(nIndex);

		// Skip if it's not Power Reminder item
		if (pwrRuntimeItem.nCategory != FID_PWRREMINDERITEM) continue;

		// If item displaying flag is marked as ON
		if (pwrRuntimeItem.nDisplayFlag == FLAG_ON) {
			// Add item ID into output data list
			arrPwrDispList.Add(pwrRuntimeItem.nItemID);
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
	int nActionNameID = GetPairedID(idplActionName, schItem.nAction);
	CString strActionName = GetLanguageString(pAppLang, nActionNameID);

	// Detail info
	LOGDETAIL logDetail;
	logDetail.byCategory = LOG_DETAIL_CONTENTID;
	logDetail.uiDetailInfo = schItem.nItemID;
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
	CString strMsgContent = pwrItem.strMessage;

	// Detail info
	LOGDETAILINFO logDetailInfo;

	// Item ID
	LOGDETAIL logDetail;
	logDetail.byCategory = LOG_DETAIL_CONTENTID;
	logDetail.uiDetailInfo = pwrItem.nItemID;
	logDetailInfo.Add(logDetail);

	// Output event log
	OutputEventLog(usEvent, strMsgContent, &logDetailInfo);
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
	switch (schItem.nAction)
	{
	case DEF_APP_ACTION_NOTHING:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_DONOTHING;
		break;
	case DEF_APP_ACTION_DISPLAYOFF:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_DISPLAYOFF;
		break;
	case DEF_APP_ACTION_SLEEP:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SLEEP;
		break;
	case DEF_APP_ACTION_SHUTDOWN:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SHUTDOWN;
		break;
	case DEF_APP_ACTION_RESTART:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_RESTART;
		break;
	case DEF_APP_ACTION_SIGNOUT:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_SIGNOUT;
		break;
	case DEF_APP_ACTION_HIBERNATE:
		nActionNameID = IDS_HISTORYLOG_PWRACTION_HIBERNATE;
		break;
	}

	// Initialize schedule history info data
	m_hidHistoryInfoData.Init(HSTRCATE_SCHEDULE);
	m_hidHistoryInfoData.nItemID = schItem.nItemID;
	m_hidHistoryInfoData.nActionNameID = nActionNameID;
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
		if (hksTemp.nHKActionID == nHKID) {
			hksItem.Copy(hksTemp);
			break;
		}
	}

	// Check item validity
	if (hksItem.IsEmpty())
		return;

	// Get hotkey action name ID
	UINT nActionNameID = NULL;
	switch (hksItem.nHKActionID)
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
	m_hidHistoryInfoData.Init(HSTRCATE_HOTKEYSET);
	m_hidHistoryInfoData.nActionNameID = nActionNameID;
	hksItem.PrintKeyStrokes(m_hidHistoryInfoData.strDescription);
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
	m_hidHistoryInfoData.Init(HSTRCATE_PWRREMINDER);
	m_hidHistoryInfoData.nItemID = pwrItem.nItemID;
	m_hidHistoryInfoData.strDescription.Format(DEF_STRING_QUOTEFORMAT, pwrItem.strMessage);
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
	if (m_hidHistoryInfoData.bInitState == FALSE)
		return;

	// Prepare common history log info
	LOGITEM actionLogItem;
	actionLogItem.stTime = m_hidHistoryInfoData.stTimestamp;

	// Get action name string
	CString strActionName = DEF_STRING_NULL;
	LoadResourceString(strActionName, m_hidHistoryInfoData.nActionNameID);

	// In case of power action history
	CString strActionResult = DEF_STRING_NULL;
	if (m_hidHistoryInfoData.nCategoryID == HSTRCATE_PWRACTION) {

		// Get action result string
		if (m_hidHistoryInfoData.bActionResult == TRUE) {
			// Action succeed
			LoadResourceString(strActionResult, IDS_HISTORYLOG_RESULT_SUCCESS);
		}
		else {
			// Action failed
			CString strTemp;
			if (LoadResourceString(strTemp, IDS_HISTORYLOG_RESULT_FAILED_ERRCODE)) {
				// Attach error code
				strActionResult.Format(strTemp, m_hidHistoryInfoData.dwErrorCode);
			}
		}
	}

	// Prepare log string by category ID
	CString strTemplate = DEF_STRING_NULL;
	CString strLogString = DEF_STRING_NULL;
	switch (m_hidHistoryInfoData.nCategoryID)
	{
	case HSTRCATE_PWRACTION:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_PWRACTION;
		strTemplate.LoadString(IDS_HISTORYLOG_PWRACTION_TEMPLATE);
		strLogString.Format(strTemplate, strActionName, strActionResult);
		actionLogItem.strLogString = strLogString;
		break;
	case HSTRCATE_SCHEDULE:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_SCHEDULE;
		strTemplate.LoadString(IDS_HISTORYLOG_SCHEDULE_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.nItemID, strActionName);
		actionLogItem.strLogString = strLogString;
		break;
	case HSTRCATE_HOTKEYSET:
		actionLogItem.usCategory = LOG_HISTORY_EXEC_HOTKEY;
		strTemplate.LoadString(IDS_HISTORYLOG_HOTKEYSET_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.strDescription, strActionName);
		actionLogItem.strLogString = strLogString;
		break;
	case HSTRCATE_PWRREMINDER:
		actionLogItem.usCategory = LOG_HISTORY_DISP_PWRREMINDER;
		strTemplate.LoadString(IDS_HISTORYLOG_PWRREMINDER_TEMPLATE);
		strLogString.Format(strTemplate, m_hidHistoryInfoData.nItemID, m_hidHistoryInfoData.strDescription);
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
	// Do not notify if schedule action is "Do nothing"
	if (pschItem->nAction == DEF_APP_ACTION_NOTHING)
		return DEF_INTEGER_INVALID;

	// Get action info
	UINT nActionStringID = GetPairedID(idplSchedNotifyMsg, pschItem->nAction);

	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Format message
	CString strMsgCaption = GetLanguageString(pAppLang, MSGBOX_MULTISCHEDULE_CAPTION);
	CString strMsgScheduleAction = GetLanguageString(pAppLang, nActionStringID);
	CString strMsgTemplate = GetLanguageString(pAppLang, MSGBOX_PROCESSSCHEDULE_NOTIFY);
	
	CString strMsgContent;
	strMsgContent.Format(strMsgTemplate, strMsgScheduleAction);

	// Allow cancelling schedule when notify
	BOOL bAllowCancel = GetAppOption(OPTIONID_ALLOWCANCELSCHED);
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
			if (pschItem->IsRepeatEnable() == FALSE) {
				// Deactivate schedule
				pschItem->SetActiveState(FALSE);
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

