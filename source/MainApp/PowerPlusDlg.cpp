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

// Special contants
const unsigned IDM_ABOUTBOX = 0x0010;
const unsigned WM_TASKBARCREATED = RegisterWindowMessage(_T("TaskbarCreated"));


// Implement methods for CPowerPlusDlg
IMPLEMENT_DYNAMIC(CPowerPlusDlg, SDialog)


/**
 * @brief	Constructor
 */
CPowerPlusDlg::CPowerPlusDlg(CWnd* parentWnd /*=NULL*/)
	: SDialog(IDD_POWERPLUS_DIALOG, parentWnd)
{
	// Initialize member variables
	defaultIcon_ = NULL;

	// Initialize boolean variables for checkboxes
	rightMouseShowMenu_ = false;
	showDialogAtStartup_ = false;
	isStartupEnabled_ = false;
	confirmBeforeAction_ = false;
	saveHistoryLog_ = false;
	saveAppEventLog_ = false;
	isRunAsAdmin_ = false;
	showErrorMsg_ = false;
	notifySchedule_ = false;
	allowCancelSchedule_ = false;
	enableBackgroundHotkey_ = false;
	enablePowerReminder_ = false;

	// Initialize notify icon variables
	notifyIconHandle_ = NULL;
	notifyIconDataPtr_ = NULL;
	notifyMenuPtr_ = NULL;

	// Init member variables for resizing dialog
	dialogSize_ = NULL;

	// Init hotkey register data
	currentRegHotkeyList_.clear();

	// Init Power++ runtime queue data
	runtimeQueue_.clear();

	// Init child dialogs
	aboutDlgPtr_ = NULL;
	helpDlgPtr_ = NULL;
	logViewerDlgPtr_ = NULL;
	multiScheduleDlgPtr_ = NULL;
	hotkeySetDlgPtr_ = NULL;
	pwrReminderDlgPtr_ = NULL;
}


/**
 * @brief	Destructor
 */
CPowerPlusDlg::~CPowerPlusDlg()
{
	// Delete member pointers
	if (notifyIconDataPtr_ != NULL) {
		delete notifyIconDataPtr_;
		notifyIconDataPtr_ = NULL;
	}

	if (dialogSize_ != NULL) {
		delete dialogSize_;
		dialogSize_ = NULL;
	}

	// Destroy child dialogs
	if (aboutDlgPtr_ != NULL) {
		// Destroy dialog
		delete aboutDlgPtr_;
		aboutDlgPtr_ = NULL;
	}

	if (helpDlgPtr_ != NULL) {
		// Destroy dialog
		delete helpDlgPtr_;
		helpDlgPtr_ = NULL;
	}

	if (multiScheduleDlgPtr_ != NULL) {
		// Destroy dialog
		delete multiScheduleDlgPtr_;
		multiScheduleDlgPtr_ = NULL;
	}

	if (logViewerDlgPtr_ != NULL) {
		// Destroy dialog
		delete logViewerDlgPtr_;
		logViewerDlgPtr_ = NULL;
	}

	if (hotkeySetDlgPtr_ != NULL) {
		// Destroy dialog
		delete hotkeySetDlgPtr_;
		hotkeySetDlgPtr_ = NULL;
	}

	if (pwrReminderDlgPtr_ != NULL) {
		// Destroy dialog
		delete pwrReminderDlgPtr_;
		pwrReminderDlgPtr_ = NULL;
	}

	// Clear Action Schedule data
	scheduleData_.deleteAll();
	
	// Clear HotkeySet data
	hotkeySetData_.deleteAll();

	// Clear Power Reminder data
	reminderData_.deleteAll();

	// Clear registered hotkey list
	currentRegHotkeyList_.clear();

	// Clean Power++ runtime queue data
	runtimeQueue_.clear();

	// Kill timers
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Unregister for session state change notifications
	registerSessionNotification(Mode::Disable);
}


/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CPowerPlusDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LMBACTION_LIST,		leftMouseActionCombo_);
	DDX_Control(pDX, IDC_MMBACTION_LIST,		middleMouseActionCombo_);
	DDX_Control(pDX, IDC_RMBACTION_LIST,		rightMouseActionCombo_);
	DDX_Control(pDX, IDC_LANGUAGE_LIST,			languageCombo_);
	DDX_Check(pDX,	IDC_ENABLERMBMENU_CHK,		rightMouseShowMenu_);
	DDX_Check(pDX,	IDC_SHOWATSTARTUP_CHK,		showDialogAtStartup_);
	DDX_Check(pDX,	IDC_ENABLEAUTOSTART_CHK,	isStartupEnabled_);
	DDX_Check(pDX,	IDC_CONFIRMACTION_CHK,		confirmBeforeAction_);
	DDX_Check(pDX,	IDC_SAVEHISTORYLOG_CHK,		saveHistoryLog_);
	DDX_Check(pDX,	IDC_SAVEAPPEVENTLOG_CHK,	saveAppEventLog_);
	DDX_Check(pDX,	IDC_RUNASADMIN_CHK,			isRunAsAdmin_);
	DDX_Check(pDX,	IDC_SHOWERROR_CHK,			showErrorMsg_);
	DDX_Check(pDX,	IDC_SCHEDNOTIFY_CHK,		notifySchedule_);
	DDX_Check(pDX,	IDC_SCHEDALLOWCANCEL_CHK,	allowCancelSchedule_);
	DDX_Check(pDX,	IDC_ENBBKGRDHOTKEYS_CHK,	enableBackgroundHotkey_);
	DDX_Check(pDX,	IDC_ENBPWRREMINDER_CHK,		enablePowerReminder_);
}


/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CPowerPlusDlg::registerDialogManagement(void)
{
	size_t returnValue = SDialog::registerDialogManagement();
	if (returnValue != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnValue;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		returnValue = pCtrlMan->addControl(IDC_MAIN_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_LEFTMOUSE_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_LMBACTION_LIST, Combo_Box);
		returnValue = pCtrlMan->addControl(IDC_MIDMOUSE_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_MMBACTION_LIST, Combo_Box);
		returnValue = pCtrlMan->addControl(IDC_RIGHTMOUSE_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_RMBACTION_LIST, Combo_Box);
		returnValue = pCtrlMan->addControl(IDC_ENABLERMBMENU_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_APPLY_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_RELOAD_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_EXIT_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_EXPAND_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_COLLAPSE_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_SYSTEM_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_SHOWATSTARTUP_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_ENABLEAUTOSTART_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_CONFIRMACTION_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_SAVEHISTORYLOG_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_SAVEAPPEVENTLOG_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_RUNASADMIN_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_SHOWERROR_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_SCHEDNOTIFY_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_SCHEDALLOWCANCEL_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_ENBBKGRDHOTKEYS_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_ENBPWRREMINDER_CHK, Check_Box);
		returnValue = pCtrlMan->addControl(IDC_LANGUAGE_TITLE, Static_Text);
		returnValue = pCtrlMan->addControl(IDC_LANGUAGE_LIST, Combo_Box);
		returnValue = pCtrlMan->addControl(IDC_VIEWACTIONLOG_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_BACKUPCFG_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_SCHEDULE_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_LOGVIEWER_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_HOTKEYSET_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_PWRREMINDER_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_HELP_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_ABOUT_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_DEFAULT_BTN, Button);
	}

	return returnValue;
}


/**
 * @brief	Update dialog control management
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::updateDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();
	if (pCtrlMan == NULL) return;

	// Update control info
	pCtrlMan->setBuddy(IDC_LMBACTION_LIST, IDC_LEFTMOUSE_TITLE);
	pCtrlMan->setBuddy(IDC_MMBACTION_LIST, IDC_MIDMOUSE_TITLE);
	pCtrlMan->setBuddy(IDC_RMBACTION_LIST, IDC_RIGHTMOUSE_TITLE);
	pCtrlMan->setBuddy(IDC_LANGUAGE_LIST, IDC_LANGUAGE_TITLE);

	// Update control data
	pCtrlMan->updateData(NULL);

	// Default
	SDialog::updateDialogManagement();
}


/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CPowerPlusDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->removeControl(IDC_MAIN_TITLE);
		pCtrlMan->removeControl(IDC_LEFTMOUSE_TITLE);
		pCtrlMan->removeControl(IDC_LMBACTION_LIST);
		pCtrlMan->removeControl(IDC_MIDMOUSE_TITLE);
		pCtrlMan->removeControl(IDC_MMBACTION_LIST);
		pCtrlMan->removeControl(IDC_RIGHTMOUSE_TITLE);
		pCtrlMan->removeControl(IDC_RMBACTION_LIST);
		pCtrlMan->removeControl(IDC_ENABLERMBMENU_CHK);
		pCtrlMan->removeControl(IDC_APPLY_BTN);
		pCtrlMan->removeControl(IDC_RELOAD_BTN);
		pCtrlMan->removeControl(IDC_EXIT_BTN);
		pCtrlMan->removeControl(IDC_EXPAND_BTN);
		pCtrlMan->removeControl(IDC_COLLAPSE_BTN);
		pCtrlMan->removeControl(IDC_SYSTEM_TITLE);
		pCtrlMan->removeControl(IDC_SHOWATSTARTUP_CHK);
		pCtrlMan->removeControl(IDC_ENABLEAUTOSTART_CHK);
		pCtrlMan->removeControl(IDC_CONFIRMACTION_CHK);
		pCtrlMan->removeControl(IDC_SAVEHISTORYLOG_CHK);
		pCtrlMan->removeControl(IDC_SAVEAPPEVENTLOG_CHK);
		pCtrlMan->removeControl(IDC_RUNASADMIN_CHK);
		pCtrlMan->removeControl(IDC_SHOWERROR_CHK);
		pCtrlMan->removeControl(IDC_SCHEDNOTIFY_CHK);
		pCtrlMan->removeControl(IDC_SCHEDALLOWCANCEL_CHK);
		pCtrlMan->removeControl(IDC_ENBBKGRDHOTKEYS_CHK);
		pCtrlMan->removeControl(IDC_ENBPWRREMINDER_CHK);
		pCtrlMan->removeControl(IDC_LANGUAGE_TITLE);
		pCtrlMan->removeControl(IDC_LANGUAGE_LIST);
		pCtrlMan->removeControl(IDC_VIEWACTIONLOG_BTN);
		pCtrlMan->removeControl(IDC_BACKUPCFG_BTN);
		pCtrlMan->removeControl(IDC_SCHEDULE_BTN);
		pCtrlMan->removeControl(IDC_LOGVIEWER_BTN);
		pCtrlMan->removeControl(IDC_HOTKEYSET_BTN);
		pCtrlMan->removeControl(IDC_PWRREMINDER_BTN);
		pCtrlMan->removeControl(IDC_HELP_BTN);
		pCtrlMan->removeControl(IDC_ABOUT_BTN);
		pCtrlMan->removeControl(IDC_DEFAULT_BTN);
	}

	return SDialog::unregisterDialogManagement();
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
	ON_ID_CONTROL(IDC_ENBPWRREMINDER_CHK,		"enablePowerReminderCheck")
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
	ON_ID_MENU(IDM_NOTIFY_RESTART_APP,			"Menu.restartApp")
	ON_ID_MENU(IDM_NOTIFY_RESTART_ASADMIN,		"Menu.RestartAsAdmin")
	ON_ID_MENU(IDM_NOTIFY_SHOW_WINDOW,			"Menu.ShowWindow")
	ON_ID_MENU(IDM_NOTIFY_EXIT_APP,				"Menu.exitApp")
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
		String aboutMenuFormat = StringUtils::loadResourceString(IDS_APP_SYSMENU_ABOUT);
		if (!aboutMenuFormat.isEmpty())
		{
			// Add product version number
			String aboutMenuTitle;
			aboutMenuTitle.format(aboutMenuFormat, StringUtils::getProductVersion(false).getString());

			// Add menu item
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, aboutMenuTitle);
		}
	}

	// Set application's main window caption
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		this->setCaption(theAppPtr->getAppWindowCaption());
	}

	// Load app default icon
	setIcon(IDI_ICON_APP);

	// Set icon
	setIcon(defaultIcon_, true);		// Set big icon
	setIcon(defaultIcon_, false);		// Set small icon

	// Do not exit using Esc button
	setUseEscape(false);

	// Do not apply settings with Enter button
	setUseEnter(false);

	// Set app features standard timers
	SetTimer(TIMERID_STD_ACTIONSCHEDULE, 1000, NULL);
	SetTimer(TIMERID_STD_POWERREMINDER, 1000, NULL);
	SetTimer(TIMERID_STD_EVENTSKIPCOUNTER, 1000, NULL);

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	// First, init all default data
	appConfigData_.setDefaultData();
	tempConfigData_.setDefaultData();
	scheduleData_.setDefaultData();
	hotkeySetData_.setDefaultData();
	reminderData_.setDefaultData();

	// Load data
	getAppData(APPDATA_ALL);

	// Setup main dialog
	setupLanguage();
	updateDialogData(false);

	// Create notify icon
	if (!createNotifyIcon()) {

		// Exit if failed to create notify icon
		TRACE("Failed to create notify icon!!!");
		exitApp(ExitCode::Error);
		return false;
	}

	// Update dialog control management
	updateDialogManagement();

	// Initialize dialog state as collapsed
	setFlagValue(AppFlagID::dialogExpanded, true);
	expandDialog(false);

	// Initialize Power++ runtime queue
	updateActionScheduleQueue(Mode::Init);
	updatePwrReminderSnooze(Mode::Init);

	// Initialize background hotkeys if enabled
	setupBackgroundHotkey(Mode::Init);

	// Register for session state change notifications
	registerSessionNotification(Mode::Init);

	// Execute Power Reminder at startup
	executePowerReminder(PwrReminderEvent::atAppStartup);

	// Execute Power Reminder after power action awake
	if (getPwrActionFlag() == FLAG_ON) {
		executePowerReminder(PwrReminderEvent::wakeAfterAction);
		setPwrActionFlag(FLAG_OFF);						// Reset flag
		setSystemSuspendFlag(FLAG_OFF);					// Reset flag
		setSessionEndFlag(FLAG_OFF);					// Reset flag
		if (theAppPtr != NULL) {
			// Save flag value update
			theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
	}

	// Execute Power Reminder after system awake (or after session ending)
	if ((getSystemSuspendFlag() == FLAG_ON) || (getSessionEndFlag() == FLAG_ON)) {
		executePowerReminder(PwrReminderEvent::atSysWakeUp);
		setSystemSuspendFlag(FLAG_OFF);					// Reset flag
		setSessionEndFlag(FLAG_OFF);					// Reset flag
		if (theAppPtr != NULL) {
			// Save flag value update
			theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
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
	if (aboutDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = aboutDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Help dialog
	if (helpDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = helpDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// LogViewer dialog
	if (logViewerDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = logViewerDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Multi schedule dialog
	if (multiScheduleDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = multiScheduleDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// HotkeySet dialog
	if (hotkeySetDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = hotkeySetDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}
	// Power Reminder dialog
	if (pwrReminderDlgPtr_ != NULL) {
		// Request close dialog
		resCloseReq = pwrReminderDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}

	// Can not destroy if Power Reminder messages are currently displaying
	uint32_array arrPwrDispItemList;
	if (getPwrReminderDispList(arrPwrDispItemList) > 0) {
		// Display notify message
		displayMessageBox(MSGBOX_OTHER_PREDESTROY_REMINDERDISP, MSGBOX_PWRREMINDER_CAPTION);
		return Result::Failure;
	}

	// Destroy components
	removeNotifyIcon();
	KillTimer(TIMERID_STD_ACTIONSCHEDULE);
	KillTimer(TIMERID_STD_POWERREMINDER);
	KillTimer(TIMERID_STD_EVENTSKIPCOUNTER);

	// Execute Power Reminder before exitting
	executePowerReminder(PwrReminderEvent::atAppExit);

	// Destroy background hotkeys if enabled
	setupBackgroundHotkey(Mode::Disable);

	// Unregister for session state change notifications
	registerSessionNotification(Mode::Disable);

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
	setupBackgroundHotkey(Mode::Disable);

	// Unregister for session state change notifications
	registerSessionNotification(Mode::Disable);

	// Save app event log if enabled
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Destroy dialog
	SDialog::OnDestroy();
}


/**
 * @brief	OnSysCommand function
 * @param	Default
 * @return	None
 */
void CPowerPlusDlg::OnSysCommand(UINT id, LPARAM lParam)
{
	if ((id & 0xFFF0) == IDM_ABOUTBOX) {
		// Open About dialog
		openChildDialogEx(IDD_ABOUT_DLG);
	}
	else {
		// Execute default syscommand
		SDialog::OnSysCommand(id, lParam);
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
		dc.DrawIcon(x, y, defaultIcon_);
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
	return STATIC_CAST(HCURSOR, defaultIcon_);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_APPLY_BTN);

	// Apply settings and hide dialog
	applySettings(true);
}


/**
 * @brief	Handle click event for [Reload] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnReload()
{
	// Save app event logs if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_RELOAD_BTN);

	// Message caption
	unsigned captionId = MSGBOX_CONFIG_RELOAD_CAPTION;

	// Check for setting changed
	bool isChanged = checkSettingChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		int returnValue = displayMessageBox(MSGBOX_CONFIG_CHANGED_CONTENT, captionId, MB_YESNO | MB_ICONINFORMATION);
		if (returnValue == IDYES) {
			// Reload all settings
			reloadSettings();
		}
	}
	else {
		// There's nothing changed
		displayMessageBox(MSGBOX_CONFIG_NOTCHANGED_CONTENT, captionId, MB_OK | MB_ICONINFORMATION);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXIT_BTN);

	// Destroy dialog and exit
	exitApp(ExitCode::PressExitButton);
}


/**
 * @brief	Handle click event for [X] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnClose()
{
	// Check for setting changed
	bool isChanged = checkSettingChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		// Apply settings and hide dialog
		applySettings(true);
	}
	else {
		// Only hide the dialog
		showDialog(this, false);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_EXPAND_BTN);

	// Expand/collapse dialog
	bool currentState = getFlagValue(AppFlagID::dialogExpanded);
	expandDialog(!currentState);
	updateDialogData(false);
}


/**
 * @brief	Show about dialog when pressing [About] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnAbout()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_ABOUT_BTN);

	// Open About dialog
	openChildDialogEx(IDD_ABOUT_DLG);
}


/**
 * @brief	Show help dialog when pressing [Help] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnHelp()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HELP_BTN);

	// Open Help dialog
	openChildDialogEx(IDD_HELP_DLG);
}


/**
 * @brief	Handle click event for [Default] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnDefault()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_DEFAULT_BTN);

	// Set default config
	setDefaultConfig();
}


/**
 * @brief	Change action for left mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeLMBAction()
{
	// Update data
	updateDialogData(true);
	leftMouseActionCombo_.GetCurSel();

	// Check for settings change
	setFlagValue(AppFlagID::dialogDataChanged, checkSettingChangeState());

	// Save app event log if enabled
	outputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LMBACTION_LIST);
}


/**
 * @brief	Change action for middle mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeMMBAction()
{
	// Update data
	updateDialogData(true);
	middleMouseActionCombo_.GetCurSel();

	// Check for settings change
	setFlagValue(AppFlagID::dialogDataChanged, checkSettingChangeState());

	// Save app event log if enabled
	outputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_MMBACTION_LIST);
}


/**
 * @brief	Change action for right mouse button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeRMBAction()
{
	// Update data
	updateDialogData(true);
	rightMouseActionCombo_.GetCurSel();

	// Check for settings change
	setFlagValue(AppFlagID::dialogDataChanged, checkSettingChangeState());

	// Save app event log if enabled
	outputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_RMBACTION_LIST);

	// Refresh dialog item state
	rightMouseActionCombo_.EnableWindow(!rightMouseShowMenu_);
	updateDialogData(false);
}


/**
 * @brief	Update dialog items when changing language
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnChangeLanguage()
{
	// Update data
	updateDialogData(true);
	languageCombo_.GetCurSel();

	// Save app event log if enabled
	outputComboBoxLog(LOG_EVENT_CMB_SELCHANGE, IDC_LANGUAGE_LIST);

	// Reload app language
	unsigned nCurLanguage = getAppOption(AppOptionID::languageID, true);
	((CPowerPlusApp*)AfxGetApp())->reloadAppLanguage(nCurLanguage);

	// Check for settings change
	setFlagValue(AppFlagID::dialogDataChanged, checkSettingChangeState());

	// Refresh dialog display
	setupLanguage();
	updateDialogData(false);
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
 * @param	checkboxId - ID of clicked checkbox
 * @return	None
 */
void CPowerPlusDlg::OnCheckboxClicked(UINT checkboxId)
{
	// Get clicked checkbox control
	CButton* checkboxPtr = (CButton*)GetDlgItem(checkboxId);
	if (checkboxPtr == NULL) return;

	// Update checkbox state
	updateDialogData(true);
	int state = checkboxPtr->GetCheck();

	// Update setting change flag
	setFlagValue(AppFlagID::dialogDataChanged, checkSettingChangeState());

	// Save app event log if enabled
	outputCheckBoxLog(LOG_EVENT_CHK_CLICKED, checkboxId);

	// Do custom checkbox actions
	switch (checkboxId)
	{
	case IDC_ENABLERMBMENU_CHK:
		enableRightMouseMenu(state);
		break;
	case IDC_SAVEAPPEVENTLOG_CHK:
		enableLogViewer(state);
		break;
	case IDC_ENBBKGRDHOTKEYS_CHK:
		enableBackgroundHotkey(state);
		break;
	case IDC_ENBPWRREMINDER_CHK:
		enablePowerReminder(state);
		break;
	case IDC_RUNASADMIN_CHK:
		updateRestartAsAdminFlag(state);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_BACKUPCFG_BTN);

	// Backup configuration
	bool returnFlag = false;
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		returnFlag = theAppPtr->backupRegistryAppData();
	}

	// Backup failed
	if (returnFlag != true) {
		// Show error message if enabled
		showErrorMessage(APP_ERROR_BACKUP_REG_FAILED);
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
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_LOGVIEWER_BTN);

	// Open LogViewer dialog
	openChildDialogEx(IDD_LOGVIEWER_DLG);
}


/**
 * @brief	Handle click event for [Schedule] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnSchedule()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_SCHEDULE_BTN);

	// Open Schedule dialog
	openChildDialogEx(IDD_MULTISCHEDULE_DLG);
}


/**
 * @brief	Handle click event for [HotkeySet] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnHotkeySet()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_BTN);

	// Open HotkeySet dialog
	openChildDialogEx(IDD_HOTKEYSET_DLG);
}


/**
 * @brief	Handle click event for [Power Reminder] button
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnPowerReminder()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_PWRREMINDER_BTN);

	// Open PowerReminder dialog
	openChildDialogEx(IDD_PWRREMINDER_DLG);
}


/**
 * @brief	Open action log file with Notepad
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnViewActionLog()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWACTIONLOG_BTN);

	// View action log file
	openTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
}


/**
 * @brief	Open backed-up configuration file with Notepad
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::OnViewBackupConfig()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_VIEWBAKCFG_BTN);

	// View backup config file
	openTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
}


/**
 * @brief	OnTimer function
 * @param	eventId - Time event ID
 * @return	None
 */
void CPowerPlusDlg::OnTimer(UINT_PTR eventId)
{
	// Timer ID: Action Schedule
	if (eventId == TIMERID_STD_ACTIONSCHEDULE) {
		// Process Action schedule
		processActionSchedule();
	}

	// Timer ID: Power Reminder
	else if (eventId == TIMERID_STD_POWERREMINDER) {
		// Process Power Reminder at set time event
		bool isPwrReminderActive = getAppOption(AppOptionID::enablePowerReminder);
		if (isPwrReminderActive == true) {
			// Execute Power reminder
			executePowerReminder(PwrReminderEvent::atSetTime);
		}
	}

	// Timer ID: Event skip counter
	else if (eventId == TIMERID_STD_EVENTSKIPCOUNTER) {
		// Process Power Broadcast event skip counter
		int count = getFlagValue(AppFlagID::pwrBroadcastSkipCount);
		if (count > 0) {
			// Count down (decrease value by 1)
			setFlagValue(AppFlagID::pwrBroadcastSkipCount, --count);
		}
	}

	// Default
	SDialog::OnTimer(eventId);
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
	unsigned dialogId = (unsigned)wParam;

	// About dialog
	if (dialogId == IDD_ABOUT_DLG) {
		if (aboutDlgPtr_ != NULL) {
			// Delete dialog
			delete aboutDlgPtr_;
			aboutDlgPtr_ = NULL;
		}
	}
	// Help dialog
	else if (dialogId == IDD_HELP_DLG) {
		if (helpDlgPtr_ != NULL) {
			// Delete dialog
			delete helpDlgPtr_;
			helpDlgPtr_ = NULL;
		}
	}
	// Multi schedule dialog
	else if (dialogId == IDD_MULTISCHEDULE_DLG) {
		if (multiScheduleDlgPtr_ != NULL) {
			// Delete dialog
			delete multiScheduleDlgPtr_;
			multiScheduleDlgPtr_ = NULL;
		}
	}
	// LogViewer dialog
	else if (dialogId == IDD_LOGVIEWER_DLG) {
		if (logViewerDlgPtr_ != NULL) {
			// Delete dialog
			delete logViewerDlgPtr_;
			logViewerDlgPtr_ = NULL;
		}
	}
	// HotkeySet dialog
	else if (dialogId == IDD_HOTKEYSET_DLG) {
		if (hotkeySetDlgPtr_ != NULL) {
			// Delete dialog
			delete hotkeySetDlgPtr_;
			hotkeySetDlgPtr_ = NULL;
		}
	}
	// Power Reminder dialog
	else if (dialogId == IDD_PWRREMINDER_DLG) {
		if (pwrReminderDlgPtr_ != NULL) {
			// Delete dialog
			delete pwrReminderDlgPtr_;
			pwrReminderDlgPtr_ = NULL;
		}
	}
	// DebugTest dialog
	else if (dialogId == IDD_DEBUGTEST_DLG) {
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
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		ScheduleData* pschData = theAppPtr->getAppScheduleData();
		if (pschData != NULL) {
			scheduleData_.copy(*pschData);
			updateActionScheduleQueue(Mode::Update);
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
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		HotkeySetData* hotkeySetDataPtr = theAppPtr->getAppHotkeySetData();
		if (hotkeySetDataPtr != NULL) {
			hotkeySetData_.copy(*hotkeySetDataPtr);
			setupBackgroundHotkey(Mode::Update);
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
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		PwrReminderData* reminderDataPtr = theAppPtr->getAppPwrReminderData();
		if (reminderDataPtr != NULL) {
			reminderData_.copy(*reminderDataPtr);
			updatePwrReminderSnooze(Mode::Update);
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
		outputDebugLog(Constant::String::Null);
		return LRESULT(Result::Failure);
	}

	// Get debug command string
	String debugCommand(LPARAM_TO_STRING(lParam));

	// Process debug command
	DWORD errorCode = APP_ERROR_DBG_SUCCESS;
	if (!processDebugCommand(debugCommand, errorCode)) {

		// Reply failed message
		if (errorCode == APP_ERROR_DBG_INVALID_COMMAND) {

			// Error: Invalid command
			outputDebugLog(_T("Invalid command!!!"));
		}
		else if (errorCode == APP_ERROR_DBG_TOKENIZATION_FAILED) {

			// Trace error
			TRACE_ERROR(_T("Error: Debug command tokenization failed!!!"));

			// Get command character list
			String commandCharList;
			StringUtils::printCharList(debugCommand, commandCharList);

			// Output debug info (to file)
			String debugLog = StringUtils::stringFormat(_T("Failed debug command: %s"), commandCharList.getString());
			outputDebugLog(debugLog, DebugInfoFile);
		}
		else {
			// Reply corresponding error code
			outputDebugLogFormat(_T("Failed! (Error code: 0x%X)"), errorCode);
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
	bool showFlag = true;
	if (wParam != NULL) {
		showFlag = static_cast<bool>(wParam);
	}

	// Show/hide dialog
	showDialog(this, showFlag);

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
	DWORD errorCode = NULL;
	if (wParam != NULL) {
		errorCode = static_cast<DWORD>(wParam);
	}

	// Show error message
	showErrorMessage(errorCode);

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle power broadcast event
 * @param	wParam - Event ID
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnPowerBroadcastEvent(WPARAM wParam, LPARAM /*lParam*/)
{
	// Check if event skip counter is triggered
	if (getFlagValue(AppFlagID::pwrBroadcastSkipCount) > 0) {
		TRACE("Power Broadcast Event will be skipped!!!");
		return LRESULT(Result::Failure);
	}

	// Get event ID from param
	ULONG eventId = static_cast<ULONG>(wParam);
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Process system resume/wakeup events
	if ((eventId == PBT_APMRESUMESUSPEND) || (eventId == PBT_APMRESUMEAUTOMATIC)) {

		// Trigger skip event counter
		// Temporarily skip processing PowerBroadcastEvent in 3 seconds
		setFlagValue(AppFlagID::pwrBroadcastSkipCount, 3);

		// If Power action flag is triggered, 
		// handle it like a wakeup event after power action
		if (getPwrActionFlag() == FLAG_ON) {
			// Execute Power Reminder after power action awake
			executePowerReminder(PwrReminderEvent::wakeAfterAction);
			setPwrActionFlag(FLAG_OFF);		// Reset flag
		}
		// Otherwise,
		// handle it like regular system wakeup event
		else {
			// Execute Power Reminder at system awaken event
			executePowerReminder(PwrReminderEvent::atSysWakeUp);
		}

		// Reset session ending flag
		setSessionEndFlag(FLAG_OFF);

		// Reset system suspended flag
		setSystemSuspendFlag(FLAG_OFF);
		if (theAppPtr != NULL) {
			// Save flag value updates
			theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}
		
		// Output action history after waken up
		// Notes: This is only backup solution to prevent history data lost
		// If it had already been saved, it will be skipped automatically
		saveHistoryInfoData();
	}
	// Process system suspend event
	else if (eventId == PBT_APMSUSPEND) {

		// Turn on system suspended flag
		setSystemSuspendFlag(FLAG_ON);
		if (theAppPtr != NULL) {
			// Save flag value update
			theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}

		// Save action history if remaining unsaved
		saveHistoryInfoData();
	}

	// Default: Success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle querry ending session event
 * @param	wParam - Not used
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnQuerryEndSession(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) {
		TRACE_ERROR("Error: Get application pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return LRESULT(Result::Failure);
	}

	/*---------- Process querry ending session event ----------*/

	// Turn on session ending flag
	setSessionEndFlag(FLAG_ON);
	theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);

	// Save last session ending time
	DateTime curSysDateTime = DateTimeUtils::getCurrentDateTime();
	theAppPtr->saveLastSysEventTime(SystemEventID::SessionEnded, curSysDateTime);

	// Save action history if remaining unsaved
	saveHistoryInfoData();

	/*---------------------------------------------------------*/

	// Default: Success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle session state change notifications
 * @param	wParam - First param
 * @param	lParam - Second param
 * @return	LRESULT
 */
LRESULT CPowerPlusDlg::OnWTSSessionChange(WPARAM wParam, LPARAM /*lParam*/)
{
	// Process status code
	switch (wParam)
	{
	case WTS_SESSION_LOCK:
		// Screen locked
		setSessionLockFlag(FLAG_ON);
		outputDebugLog(_T("The screen is LOCKED!!!"));
		break;

	case WTS_SESSION_UNLOCK:
		// Screen unlocked
		setSessionLockFlag(FLAG_OFF);
		outputDebugLog(_T("The screen is UNLOCKED!!!"));
		break;

	default:
		// Trace other notification codes
		outputDebugLogFormat(_T("WTS session notification: Code=0x%02X"), wParam);
		break;
	}

	// Default: Always success
	return LRESULT(Result::Success);
}


/**
 * @brief	Handle app command messages (WM_COMMAND)
 * @param	wParam - First param (HIWORD)
 * @param	lParam - Second param (LOWORD)
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
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_DISPLAYOFF);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_DISPLAYOFF);
		break;
	case IDM_NOTIFY_ACTION_SLEEP:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SLEEP);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_SLEEP);
		break;
	case IDM_NOTIFY_ACTION_SHUTDOWN:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SHUTDOWN);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_SHUTDOWN);
		break;
	case IDM_NOTIFY_ACTION_RESTART:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_RESTART);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_RESTART);
		break;
	case IDM_NOTIFY_ACTION_SIGNOUT:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SIGNOUT);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_SIGNOUT);
		break;
	case IDM_NOTIFY_ACTION_HIBERNATE:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_HIBERNATE);
		executeAction(APP_MACRO_ACTION_MENU, APP_ACTION_HIBERNATE);
		break;
	case IDM_NOTIFY_ACTION_SCHEDULE:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_ACTION_SCHEDULE);
		executeAction(APP_MACRO_ACTION_SCHEDULE, getAppOption(AppOptionID::defaultScheduleActionID));
		break;

	/*********************************************************************/
	/*																	 */
	/*				Handle commands for other menu items				 */
	/*																	 */
	/*********************************************************************/

	case IDM_NOTIFY_OPENDLG_ABOUT:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_ABOUT);
		openChildDialogEx(IDD_ABOUT_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HELP:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_HELP);
		openChildDialogEx(IDD_HELP_DLG);
		break;
	case IDM_NOTIFY_VIEW_ACTIONLOG:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_ACTIONLOG);
		openTextFileToView(Constant::File::Name::AppHistory, Constant::File::Extension::Log, Constant::Folder::Log);
		break;
	case IDM_NOTIFY_BACKUP_CONFIG:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_BACKUP_CONFIG);
		((CPowerPlusApp*)AfxGetApp())->backupRegistryAppData();
		break;
	case IDM_NOTIFY_VIEW_BAKCONFIG:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_VIEW_BAKCONFIG);
		openTextFileToView(Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
		break;
	case IDM_NOTIFY_OPENDLG_LOGVIEWER:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_LOGVIEWER);
		openChildDialogEx(IDD_LOGVIEWER_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_SCHEDULE:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_SCHEDULE);
		openChildDialogEx(IDD_MULTISCHEDULE_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_HOTKEYSET:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_HOTKEYSET);
		openChildDialogEx(IDD_HOTKEYSET_DLG);
		break;
	case IDM_NOTIFY_OPENDLG_PWRREMINDER:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_OPENDLG_PWRREMINDER);
		openChildDialogEx(IDD_PWRREMINDER_DLG);
		break;
	case IDM_NOTIFY_RESTART_APP:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_APP);
		requestRestartApp(IDM_NOTIFY_RESTART_APP, false);
		break;
	case IDM_NOTIFY_RESTART_ASADMIN:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_RESTART_ASADMIN);
		requestRestartApp(IDM_NOTIFY_RESTART_ASADMIN, true);
		break;
	case IDM_NOTIFY_SHOW_WINDOW:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_SHOW_WINDOW);
		expandDialog(false);
		PostMessage(SM_WND_SHOWDIALOG, true);
		break;
	case IDM_NOTIFY_EXIT_APP:
		outputMenuLog(LOG_EVENT_MENU_SELECTED, IDM_NOTIFY_EXIT_APP);
		exitApp(ExitCode::FromNotifyIcon);
		break;
	default:
		break;
	}

	// Default
	return SDialog::OnCommand(wParam, lParam);
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
		{
			switch (lParam)
			{
				case WM_LBUTTONDOWN:
					outputEventLog(LOG_EVENT_TRAYICO_LMBCLICKED);
					executeAction(APP_MACRO_LEFT_MOUSE);
					break;
				case WM_MBUTTONDOWN:
					outputEventLog(LOG_EVENT_TRAYICO_MMBCLICKED);
					executeAction(APP_MACRO_MIDDLE_MOUSE);
					break;
				case WM_RBUTTONUP:
					outputEventLog(LOG_EVENT_TRAYICO_RMBCLICKED);
					executeAction(APP_MACRO_RIGHT_MOUSE);
					break;
			}
			return true;
		}

		case SM_APP_SHOW_REMINDER_BALLOON_TIP:
		{
			if (lParam == NULL)
				return false;

			// Get application language package
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

			// Prepare balloon tip content
			String balloonTitle = getLanguageString(languageTablePtr, IDD_PWRREMINDER_DLG);
			String balloonInfoContent = LPARAM_TO_STRING(lParam);

			// Show tray icon balloon tip
			setBalloonTipText(balloonTitle, balloonInfoContent);

			return true;
		}

		case SM_WND_DEBUGTEST:
			openChildDialogEx(IDD_DEBUGTEST_DLG);
			break;
		case WM_QUERYENDSESSION:
			OnQuerryEndSession(NULL, NULL);
			break;
		case WM_HOTKEY:
			processHotkey(static_cast<unsigned>(wParam));
			break;
		case SM_APP_LOCKSTATE_HOTKEY:
			processLockStateHotkey(static_cast<DWORD>(wParam));
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
		createNotifyIcon();
		return true;
	}

	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}


//////////////////////////////////////////////////////////////////////////
// Member functions using for resizing dialog


/**
 * @brief	Expand/collapse dialog
 * @param	isExpandOrCollapse - Flag to expand or collapse
 * @return	None
 */
void CPowerPlusDlg::expandDialog(bool isExpandOrCollapse)
{
	// If new state is the same as current state, do nothing
	bool currentState = getFlagValue(AppFlagID::dialogExpanded);
	if (isExpandOrCollapse == currentState) {
		TRACE("State doesn't change, do not process!!!");
		return;
	}

	CRect windowRect, frameWndRect, childWndRect;
	CWnd* childWndPtr = NULL;
	CWnd* frameWndPtr = NULL;

	// Get [Expand/Collapse] button
	CWnd* buttonPtr = GetDlgItem(IDC_EXPAND_BTN);
	if (buttonPtr == NULL) {
		TRACE_ERROR("Error: [Expand/Collapse] button not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get separator frame
	frameWndPtr = GetDlgItem(IDC_FRAMEWND);
	if (frameWndPtr == NULL) {
		TRACE_ERROR("Error: Frame not found!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}
	
	// Get separator frame rect
	frameWndPtr->GetWindowRect(&frameWndRect);

	// Update dialog items state when expanding/collapsing
	for (childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		childWndPtr->GetWindowRect(&childWndRect);
		if (childWndRect.bottom > frameWndRect.top)
		{
			switch (childWndPtr->GetDlgCtrlID())
			{
			case IDC_SYSTEM_TITLE:
			case IDC_SHOWATSTARTUP_CHK:
			case IDC_LANGUAGE_TITLE:
			case IDC_LANGUAGE_LIST:
				// Show/hide these items
				childWndPtr->ShowWindow(isExpandOrCollapse);
				break;

			case IDC_HELP_BTN:
			case IDC_ABOUT_BTN:
			case IDC_DEFAULT_BTN:
				{
					// Move buttons
					CRect newChildRect, oldChildRect;
					this->GetDlgItem(childWndPtr->GetDlgCtrlID())->GetWindowRect(&oldChildRect);
					this->ScreenToClient(&oldChildRect);
					if (!isExpandOrCollapse) {
						CRect collapseButtonRect;
						GetDlgItem(IDC_COLLAPSE_BTNPOS)->GetWindowRect(&collapseButtonRect);
						this->ScreenToClient(&collapseButtonRect);
						newChildRect = oldChildRect;
						newChildRect.top = collapseButtonRect.top;
						newChildRect.bottom = collapseButtonRect.bottom;
					}
					else {
						CRect expandButtonRect;
						GetDlgItem(IDC_EXPAND_BTNPOS)->GetWindowRect(&expandButtonRect);
						this->ScreenToClient(&expandButtonRect);
						newChildRect = oldChildRect;
						newChildRect.top = expandButtonRect.top;
						newChildRect.bottom = expandButtonRect.bottom;
					}
					childWndPtr->MoveWindow(&newChildRect);
				}
				break;

			default:
				childWndPtr->EnableWindow(isExpandOrCollapse);
				break;
			}
		}
	}

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Initialize dialog size pointer
	VERIFY_INITIALIZATION(dialogSize_, Size);

	// Expand/collapse dialog
	if (!isExpandOrCollapse) {

		// Calculate dialog size
		GetWindowRect(&windowRect);
		dialogSize_->_width = windowRect.right - windowRect.left;
		dialogSize_->_height = windowRect.bottom - windowRect.top;

		// Resize dialog
		SetWindowPos(NULL, 0, 0, windowRect.right - windowRect.left, frameWndRect.bottom - windowRect.top, SWP_NOMOVE | SWP_NOZORDER);
		
		// Change [Expand/Collapse] button title
		setControlText(buttonPtr, IDC_EXPAND_BTN, languageTablePtr);
	}
	else {
		// Resize dialog
		SetWindowPos(NULL, 0, 0, dialogSize_->width(), dialogSize_->height(), SWP_NOMOVE | SWP_NOZORDER);

		// Make sure that the entire dialog box is visible on the screen
		SendMessage(DM_REPOSITION, 0, 0);

		// Change [Expand/Collapse] button title
		setControlText(buttonPtr, IDC_COLLAPSE_BTN, languageTablePtr);
	}

	// Save app event log if enabled
	unsigned eventId = isExpandOrCollapse ? LOG_EVENT_DLG_EXPANDED : LOG_EVENT_DLG_COLLAPSED;
	outputEventLog(eventId, this->getCaption());

	// Update flag
	bool newState = !currentState;
	setFlagValue(AppFlagID::dialogExpanded, newState);
}


//////////////////////////////////////////////////////////////////////////
// Notify icon functions


/**
 * @brief	Setup and create notify icon
 * @param	None
 * @return	true/false
 */
bool CPowerPlusDlg::createNotifyIcon(void)
{
	// If notify icon is showed, re-create it
	if (getFlagValue(AppFlagID::notifyIconShowed)) {
		TRACE("Notify icon is showed, now it will be removed and re-created!!!");
		removeNotifyIcon();
	}

	// Init notify icon
	if (notifyIconDataPtr_ == NULL) {
		notifyIconDataPtr_ = new NOTIFYICONDATA;
		if (notifyIconDataPtr_ == NULL) {
			// Initialization failed
			TRACE_ERROR("Error: Notify icon initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
	}

	// Setup notify icon properties
	notifyIconDataPtr_->hWnd = this->GetSafeHwnd();
	notifyIconDataPtr_->cbSize = sizeof(NOTIFYICONDATA);
	notifyIconDataPtr_->hIcon = defaultIcon_;
	notifyIconDataPtr_->uCallbackMessage = SM_APP_TRAYICON;
	notifyIconDataPtr_->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconDataPtr_->uID = 500;
	setNotifyTipText(notifyIconDataPtr_);

	// Create and show notify icon
	bool creationRet = false;
	for (int retryCount = 0; retryCount < Constant::Max::RetryTime; retryCount++) {
		creationRet = Shell_NotifyIcon(NIM_ADD, notifyIconDataPtr_);
		if (creationRet != false) break;
	}

	// Failed to create notify icon
	if (creationRet != true) {
		// Failed to create notify icon
		TRACE_ERROR("Error: Failed to create notify icon!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Update flag
	setFlagValue(AppFlagID::notifyIconShowed, creationRet);

	return true;
}


/**
 * @brief	Show notify menu
 * @param	None
 * @param	Return value:	bool - Show menu successfully or failed
 */
bool CPowerPlusDlg::showNotifyMenu(void)
{
	// Reset notify menu
	if (notifyMenuPtr_ != NULL) {
		notifyMenuPtr_->DestroyMenu();
	}

	// Prepare notify menu
	CMenu menuNotify;
	menuNotify.LoadMenu(IDR_MENU_NOTIFY_DEFAULT);
	notifyMenuPtr_ = menuNotify.GetSubMenu(0);
	if (notifyMenuPtr_ == NULL) {
		// Trace error
		TRACE_ERROR("Error: Show notify menu failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Setup menu properties
	setMenuItemText(notifyMenuPtr_);
	updateMenuItemState(notifyMenuPtr_);
	SetMenuDefaultItem(notifyMenuPtr_->m_hMenu, IDM_NOTIFY_SHOW_WINDOW, NULL);
	this->SetForegroundWindow();

	// Show notify menu
	POINT cursorPoint;
	GetCursorPos(&cursorPoint);
	unsigned flags = TPM_RIGHTALIGN | TPM_BOTTOMALIGN;
	bool result = notifyMenuPtr_->TrackPopupMenu(flags, cursorPoint.x, cursorPoint.y, (CWnd*)this, NULL);

	return result;
}


/**
 * @brief	Update notify icon changes
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::updateNotifyIcon(void)
{
	// If notify icon doesn't exist, do nothing
	if (notifyIconDataPtr_ == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Update notify icon tip text
	setNotifyTipText(notifyIconDataPtr_);
	Shell_NotifyIcon(NIM_MODIFY, notifyIconDataPtr_);
}


/**
 * @brief	Remove notify icon when exiting
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::removeNotifyIcon(void)
{
	// If notify icon is not showed, do nothing
	if (!getFlagValue(AppFlagID::notifyIconShowed)) {
		TRACE("Notify icon is not showed!!!");
		return;
	}

	// If notify icon doesn't exist, do nothing
	if (notifyIconDataPtr_ == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Remove notify icon
	notifyIconDataPtr_->hIcon = NULL;
	Shell_NotifyIcon(NIM_DELETE, notifyIconDataPtr_);

	// Update flag
	setFlagValue(AppFlagID::notifyIconShowed, false);
}


//////////////////////////////////////////////////////////////////////////
// Data processing functions


/**
 * @brief	Get app data
 * @param	dataType - App data type to get
 * @return	None
 */
void CPowerPlusDlg::getAppData(unsigned dataType /* = APPDATA_ALL */)
{
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Get config data
	if ((dataType & APPDATA_CONFIG) != 0) {
		ConfigData* pcfgData = theAppPtr->getAppConfigData();
		if (pcfgData != NULL) {
			appConfigData_.copy(*pcfgData);
			tempConfigData_.copy(appConfigData_);
		}
	}

	// Get schedule data
	if ((dataType & APPDATA_SCHEDULE) != 0) {
		ScheduleData* pschData = theAppPtr->getAppScheduleData();
		if (pschData != NULL) {
			scheduleData_.copy(*pschData);
		}
	}

	// Get HotkeySet data
	if ((dataType & APPDATA_HOTKEYSET) != 0) {
		HotkeySetData* hotkeySetDataPtr = theAppPtr->getAppHotkeySetData();
		if (hotkeySetDataPtr != NULL) {
			hotkeySetData_.copy(*hotkeySetDataPtr);
		}
	}

	// Get Power Reminder data
	if ((dataType & APPDATA_PWRREMINDER) != 0) {
		PwrReminderData* reminderDataPtr = theAppPtr->getAppPwrReminderData();
		if (reminderDataPtr != NULL) {
			reminderData_.copy(*reminderDataPtr);
		}
	}
}


/**
 * @brief	Return app option by ID
 * @param	optionId - ID of specific app option
 * @param	isTemp		 - Temp value or saved value (saved value by default)
 * @return	int - App option value
 */
int CPowerPlusDlg::getAppOption(AppOptionID optionId, bool isTemp /* = false */) const
{
	int result = Constant::kInvalidInteger;
	int tempResult = Constant::kInvalidInteger;

	switch (optionId)
	{
	case AppOptionID::curDispLanguage:
		result = ((SWinApp*)AfxGetApp())->getAppLanguageOption(true);
		tempResult = result;		// No temp data
		break;
	case AppOptionID::defaultScheduleActiveState:
		result = scheduleData_.getDefaultItem().isEnabled();
		tempResult = result;		// No temp data
		break;
	case AppOptionID::defaultScheduleActionID:
		result = scheduleData_.getDefaultItem().getAction();
		tempResult = result;		// No temp data
		break;
	case AppOptionID::defaultScheduleRepeat:
		result = scheduleData_.getDefaultItem().isRepeatEnabled();
		tempResult = result;		// No temp data
		break;
	default:
		result = appConfigData_.getAppOption(optionId);
		tempResult = tempConfigData_.getAppOption(optionId);
		break;
	}

	// Return temp data if required and the result is valid
	if ((isTemp == true) && (tempResult != Constant::kInvalidInteger))
		return tempResult;

	return result;
}


/**
 * @brief	Update data values for dialog items
 * @param	saveAndValidate - Same as default MFC UpdateData function
 * @return	None
 */
void CPowerPlusDlg::updateDialogData(bool saveAndValidate /* = true */)
{
	unsigned comboSelection = 0;

	if (saveAndValidate == false) {

	/*----------------------------<Bind config data to dialog items>----------------------------*/

		// Left mouse button action combo-box
		comboSelection = tempConfigData_.leftMouseAction;
		leftMouseActionCombo_.SetCurSel(AppCore::opt2Sel(APP_ACTION, comboSelection));

		// Middle mouse button action combo-box
		comboSelection = tempConfigData_.middleMouseAction;
		middleMouseActionCombo_.SetCurSel(AppCore::opt2Sel(APP_ACTION, comboSelection));

		// Right mouse button show menu checkbox
		rightMouseShowMenu_ = tempConfigData_.rightMouseShowMenu;

		// Right mouse button action combo-box
		comboSelection = (rightMouseShowMenu_ == true) ? APP_ACTION_SHOWMENU : tempConfigData_.rightMouseAction;
		rightMouseActionCombo_.SetCurSel(AppCore::opt2Sel(APP_ACTION, comboSelection));
		enableItem(IDC_RMBACTION_LIST, !rightMouseShowMenu_);

		// Update other checkbox-es
		showDialogAtStartup_ = tempConfigData_.showDialogAtStartup;
		isStartupEnabled_ = tempConfigData_.enableAutoStart;
		confirmBeforeAction_ = tempConfigData_.actionConfirmation;
		saveHistoryLog_ = tempConfigData_.saveActionHistory;
		saveAppEventLog_ = tempConfigData_.saveAppEventLog;
		isRunAsAdmin_ = tempConfigData_.runAsAdmin;
		showErrorMsg_ = tempConfigData_.showErrorMessage;
		notifySchedule_ = tempConfigData_.scheduleNotification;
		allowCancelSchedule_ = tempConfigData_.allowScheduleCancellation;
		enableBackgroundHotkey_ = tempConfigData_.enableBackgroundHotkey;
		enablePowerReminder_ = tempConfigData_.enablePowerReminder;

		// Language list combo-box
		comboSelection = AppCore::opt2Sel(APP_LANGUAGE, tempConfigData_.languageID);
		languageCombo_.SetCurSel(comboSelection);

		// Update buttons
		enableItem(IDC_LOGVIEWER_BTN, saveAppEventLog_);
		enableItem(IDC_HOTKEYSET_BTN, enableBackgroundHotkey_);
		enableItem(IDC_PWRREMINDER_BTN, enablePowerReminder_);

		// Update dialog items
		UpdateData(false);

	/*------------------------------------------------------------------------------------------*/
	}
	else {
	/*----------------------------<Bind dialog items data to config>----------------------------*/

		// Update dialog items
		UpdateData(true);

		// Left mouse button action combo-box
		comboSelection = leftMouseActionCombo_.GetCurSel();
		tempConfigData_.leftMouseAction = AppCore::sel2Opt(APP_ACTION, comboSelection);

		// Middle mouse button action combo-box
		comboSelection = middleMouseActionCombo_.GetCurSel();
		tempConfigData_.middleMouseAction = AppCore::sel2Opt(APP_ACTION, comboSelection);

		// Right mouse button show menu checkbox
		tempConfigData_.rightMouseShowMenu = rightMouseShowMenu_;

		// Right mouse button action combo-box
		comboSelection = AppCore::opt2Sel(APP_ACTION, APP_ACTION_SHOWMENU);
		comboSelection = (tempConfigData_.rightMouseShowMenu == true) ? comboSelection : rightMouseActionCombo_.GetCurSel();
		tempConfigData_.rightMouseAction = AppCore::sel2Opt(APP_ACTION, comboSelection);

		// Update other checkbox-es
		tempConfigData_.showDialogAtStartup = showDialogAtStartup_;
		tempConfigData_.enableAutoStart = isStartupEnabled_;
		tempConfigData_.actionConfirmation = confirmBeforeAction_;
		tempConfigData_.saveActionHistory = saveHistoryLog_;
		tempConfigData_.saveAppEventLog = saveAppEventLog_;
		tempConfigData_.runAsAdmin = isRunAsAdmin_;
		tempConfigData_.showErrorMessage = showErrorMsg_;
		tempConfigData_.scheduleNotification = notifySchedule_;
		tempConfigData_.allowScheduleCancellation = allowCancelSchedule_;
		tempConfigData_.enableBackgroundHotkey = enableBackgroundHotkey_;
		tempConfigData_.enablePowerReminder = enablePowerReminder_;

		// Language list combo-box
		comboSelection = languageCombo_.GetCurSel();
		tempConfigData_.languageID = AppCore::sel2Opt(APP_LANGUAGE, comboSelection);

	/*------------------------------------------------------------------------------------------*/
	}

	// Update base dialog data
	SDialog::updateDialogData(saveAndValidate);
}


/**
 * @brief	Check setting variables changing state
 * @param	None
 * @return	bool - Change flag
 */
bool CPowerPlusDlg::checkSettingChangeState(void)
{
	bool changeFlag = false;

	changeFlag |= (tempConfigData_.leftMouseAction != appConfigData_.leftMouseAction);
	changeFlag |= (tempConfigData_.middleMouseAction != appConfigData_.middleMouseAction);
	changeFlag |= (tempConfigData_.rightMouseAction != appConfigData_.rightMouseAction);
	changeFlag |= (tempConfigData_.rightMouseShowMenu != appConfigData_.rightMouseShowMenu);
	changeFlag |= (tempConfigData_.languageID != appConfigData_.languageID);
	changeFlag |= (tempConfigData_.showDialogAtStartup != appConfigData_.showDialogAtStartup);
	changeFlag |= (tempConfigData_.enableAutoStart != appConfigData_.enableAutoStart);
	changeFlag |= (tempConfigData_.actionConfirmation != appConfigData_.actionConfirmation);
	changeFlag |= (tempConfigData_.saveActionHistory != appConfigData_.saveActionHistory);
	changeFlag |= (tempConfigData_.saveAppEventLog != appConfigData_.saveAppEventLog);
	changeFlag |= (tempConfigData_.runAsAdmin != appConfigData_.runAsAdmin);
	changeFlag |= (tempConfigData_.showErrorMessage != appConfigData_.showErrorMessage);
	changeFlag |= (tempConfigData_.scheduleNotification != appConfigData_.scheduleNotification);
	changeFlag |= (tempConfigData_.allowScheduleCancellation != appConfigData_.allowScheduleCancellation);
	changeFlag |= (tempConfigData_.enableBackgroundHotkey != appConfigData_.enableBackgroundHotkey);
	changeFlag |= (tempConfigData_.allowLockscreenHotkey != appConfigData_.allowLockscreenHotkey);
	changeFlag |= (tempConfigData_.enablePowerReminder != appConfigData_.enablePowerReminder);

	return changeFlag;
}


/**
 * @brief	Return flag value by ID
 * @param	flagId - ID of specific flag
 * @return	int - Flag value
 */
int CPowerPlusDlg::getFlagValue(AppFlagID flagId) const
{
	int value = FLAG_OFF;

	switch (flagId)
	{
	// Application main window runtime flags
	case AppFlagID::notifyIconShowed:
	case AppFlagID::hotkeyRegistered:
	case AppFlagID::restartAsAdmin:
	case AppFlagID::pwrBroadcastSkipCount:
	case AppFlagID::wtsSessionNotifyRegistered:
		value = getAppFlagManager().getFlagValue(flagId);
		break;

	default:
		// Get dialog-base flag value and others
		value = SDialog::getFlagValue(flagId);
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
void CPowerPlusDlg::setFlagValue(AppFlagID flagId, int value)
{
	// Check value validity
	if (value == Constant::kInvalidInteger)
		return;

	switch (flagId)
	{
	// Application main window runtime flags
	case AppFlagID::notifyIconShowed:
	case AppFlagID::hotkeyRegistered:
	case AppFlagID::restartAsAdmin:
	case AppFlagID::pwrBroadcastSkipCount:
	case AppFlagID::wtsSessionNotifyRegistered:
		getAppFlagManager().setFlagValue(flagId, value);
		break;

	default:
		// Set dialog-base-class flag value
		SDialog::setFlagValue(flagId, value);
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
void CPowerPlusDlg::setupLanguage(void)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
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
			setupComboBox(id, languageTablePtr);
			break;

		case IDC_EXPAND_BTN:
		{
			// Check dialog current state
			int state = getFlagValue(AppFlagID::dialogExpanded);
			if (state == true)	id = IDC_COLLAPSE_BTN;
			else id = IDC_EXPAND_BTN;
			setControlText(childWndPtr, id, languageTablePtr);
		} break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Default
	SDialog::setupLanguage();
}


/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CPowerPlusDlg::setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr)
{
	switch (comboId)
	{
	case IDC_LMBACTION_LIST:
		// Left mouse button action list
		leftMouseActionCombo_.ResetContent();
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SLEEP));			// Sleep
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_RESTART));			// Restart
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		leftMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;

	case IDC_MMBACTION_LIST:
		// Middle mouse button action list
		middleMouseActionCombo_.ResetContent();
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SLEEP));			// Sleep
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_RESTART));			// Restart
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		middleMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;

	case IDC_RMBACTION_LIST:
		// Right mouse button action list
		rightMouseActionCombo_.ResetContent();
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SLEEP));			// Sleep
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHUTDOWN));			// Shutdown
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_RESTART));			// Restart
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SIGNOUT));			// Sign out
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		rightMouseActionCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHOWMENU));			// Just show menu
		break;

	case IDC_LANGUAGE_LIST:
		// App language list
		languageCombo_.ResetContent();
		languageCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_LANGUAGE_ENGLISH));		// English
		languageCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_LANGUAGE_VIETNAMESE));		// Vietnamese
		languageCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_LANGUAGE_SIMPCHINESE));	// Chinese
		break;
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}


//////////////////////////////////////////////////////////////////////////
// Item state/checkbox update functions


/**
 * @brief	Enable/disable right mouse action combo-box
 * @param	isEnabled - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::enableRightMouseMenu(bool /*isEnabled*/)
{
	updateDialogData(false);
}


/**
 * @brief	Enable/disable LogViewer function
 * @param	isEnabled - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::enableLogViewer(bool isEnabled)
{
	// Prefer using app data option than temp config option
	bool isChecked = isEnabled;
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		isChecked = theAppPtr->getAppOption(AppOptionID::saveAppEventLog);
	}
	if (isChecked == false) {
		enableItem(IDC_LOGVIEWER_BTN, isChecked);
		return;
	}

	// Update dialog data
	updateDialogData(false);
}


/**
 * @brief	Enable/disable HotKeySet button
 * @param	isEnabled - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::enableBackgroundHotkey(bool /*isEnabled*/)
{
	updateDialogData(false);
}


/**
 * @brief	Enable/disable Power Reminder button
 * @param	isEnabled - Enable or disable
 * @return	None
 */
void CPowerPlusDlg::enablePowerReminder(bool /*isEnabled*/)
{
	updateDialogData(false);
}


/**
 * @brief	Update restart as admin flag
 * @param	flag - Update flag
 * @return	None
 */
void CPowerPlusDlg::updateRestartAsAdminFlag(bool flag)
{
	// Check current set app data option
	bool isChecked = getAppOption(AppOptionID::runAsAdmin);

	// If option is currently ON, do not update flag
	if (isChecked == true)	return;

	// Update flag
	setFlagValue(AppFlagID::restartAsAdmin, flag);
}


//////////////////////////////////////////////////////////////////////////
// Component update functions


/**
 * @brief	Setup language for menu items
 * @param	menuPtr - Menu pointer
 * @return	None
 */
void CPowerPlusDlg::setMenuItemText(CMenu* menuPtr)
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup language for entry menu
	for (int item = 0; item < menuPtr->GetMenuItemCount(); item++) {
		unsigned id = menuPtr->GetMenuItemID(item);
		if (id == 0) continue;
		// "Actions" child pop-up menu title
		if (item == IDM_NOTIFY_ACTIONS_TITLE) {
			menuPtr->ModifyMenu(item, MF_BYPOSITION | MF_STRING, NULL, getLanguageString(languageTablePtr, item));
			continue;
		}

		// Set menu item title
		menuPtr->ModifyMenu(item, MF_BYPOSITION | MF_STRING, id, getLanguageString(languageTablePtr, id));
	}

	// Setup language for sub menu
	CMenu* subMenuPtr = menuPtr->GetSubMenu(IDM_NOTIFY_ACTIONS_TITLE);
	if (subMenuPtr == NULL) return;
	for (int item = 0; item < subMenuPtr->GetMenuItemCount(); item++) {
		unsigned id = subMenuPtr->GetMenuItemID(item);
		if (id == 0) continue;

		// Set menu item title
		subMenuPtr->ModifyMenu(item, MF_BYPOSITION | MF_STRING, id, getLanguageString(languageTablePtr, id));
	}
}


/**
 * @brief	Update state for menu items
 * @param	menuPtr - Menu pointer
 * @return	None
 */
void CPowerPlusDlg::updateMenuItemState(CMenu* menuPtr)
{
	// Check validity
	if (menuPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Menu does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Main menu
	for (int item = 0; item < menuPtr->GetMenuItemCount(); item++) {
		bool isItemShowed = true;
		unsigned id = menuPtr->GetMenuItemID(item);
		switch (id)
		{
		case IDM_NOTIFY_OPENDLG_LOGVIEWER:
			isItemShowed = getAppOption(AppOptionID::saveAppEventLog);
			if (isItemShowed == true)
				isItemShowed = getAppOption(AppOptionID::saveAppEventLog, true);
			break;

		case IDM_NOTIFY_OPENDLG_HOTKEYSET:
			isItemShowed = getAppOption(AppOptionID::enableBackgroundHotkey, true);
			break;

		case IDM_NOTIFY_OPENDLG_PWRREMINDER:
			isItemShowed = getAppOption(AppOptionID::enablePowerReminder, true);
			break;

		default:
			continue;
		}

		// Disable item
		if (isItemShowed == false) {
			menuPtr->EnableMenuItem(item, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}

	// Sub-menu: Actions
	CMenu* subMenuPtr = menuPtr->GetSubMenu(IDM_NOTIFY_ACTIONS_TITLE);
	if (subMenuPtr == NULL) return;
	for (int item = 0; item < subMenuPtr->GetMenuItemCount(); item++) {
		bool isItemShowed = true;
		unsigned id = subMenuPtr->GetMenuItemID(item);
		switch (id)
		{
		case IDM_NOTIFY_ACTION_SCHEDULE:
			isItemShowed = getAppOption(AppOptionID::defaultScheduleActiveState);
			isItemShowed &= (getAppOption(AppOptionID::defaultScheduleActionID) != APP_ACTION_NOTHING);
			break;

		default:
			continue;
		}

		// Disable item
		if (isItemShowed == false) {
			subMenuPtr->EnableMenuItem(item, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
		}
	}
}


/**
 * @brief	Get notify tip text
 * @param	notifyIconDataPtr - Notify icon data pointer
 * @return	const wchar_t*
 */
const wchar_t* CPowerPlusDlg::getNotifyTipText(PNOTIFYICONDATA notifyIconDataPtr)
{
	// Check validity
	if (notifyIconDataPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return Constant::String::Null;
	}

	return notifyIconDataPtr->szTip;
}


/**
 * @brief	Set notify tip text
 * @param	notifyIconDataPtr - Notify icon data pointer
 * @return	None
 */
void CPowerPlusDlg::setNotifyTipText(PNOTIFYICONDATA notifyIconDataPtr)
{
	// Check validity
	if (notifyIconDataPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	StringArray arrTipText;
	arrTipText.reserve(3);

	// Load language strings
	String formatString = getLanguageString(languageTablePtr, NOTIFY_TIP_TEMPLATE);
	arrTipText.push_back(getLanguageString(languageTablePtr, getPairedID(IDTable::NotifyTip, appConfigData_.leftMouseAction)));
	arrTipText.push_back(getLanguageString(languageTablePtr, getPairedID(IDTable::NotifyTip, appConfigData_.middleMouseAction)));
	arrTipText.push_back(getLanguageString(languageTablePtr, getPairedID(IDTable::NotifyTip, appConfigData_.rightMouseAction)));

	// Format notify tip text
	String notifyTipText = StringUtils::stringFormat(formatString, arrTipText.at(0).getString(), arrTipText.at(1).getString(), arrTipText.at(2).getString());

	// Set notify tip text
	StrCpyW(notifyIconDataPtr->szTip, notifyTipText.getString());
}


/**
 * @brief	Show tray icon balloon tip
 * @param	balloonTitle	   - Balloon tip title
 * @param	balloonInfoContent - Balloon tip content
 * @return	None
 */
void CPowerPlusDlg::setBalloonTipText(const wchar_t* balloonTitle, const wchar_t* balloonInfoContent)
{
	// If notify icon doesn't exist, do nothing
	if (notifyIconDataPtr_ == NULL) {
		// Trace error
		TRACE_ERROR("Error: Notify icon does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Setup balloon tip properties
	notifyIconDataPtr_->uFlags = NIF_INFO;
	notifyIconDataPtr_->dwInfoFlags = NIIF_INFO;
	notifyIconDataPtr_->uTimeout = 10000;

	// Set balloon tip text
	wcscpy_s(notifyIconDataPtr_->szInfoTitle, balloonTitle);
	wcscpy_s(notifyIconDataPtr_->szInfo, balloonInfoContent);

	// Show the balloon tip
	Shell_NotifyIcon(NIM_MODIFY, notifyIconDataPtr_);
}


//////////////////////////////////////////////////////////////////////////
// Core functions


/**
 * @brief	Execute action as config/schedule/menu selection
 * @param	actionMacro - Action macro
 * @param	wParam		 - First param (HIWORD)
 * @param	lParam		 - Second param (LOWORD)
 * @return	bool - Result of action execution
 */
bool CPowerPlusDlg::executeAction(unsigned actionMacro, WPARAM wParam /* = NULL */, LPARAM /* lParam = NULL */)
{
	unsigned actionType = 0;
	unsigned actionId = 0;
	unsigned historyActionId = 0;
	unsigned actionNameLangId = 0;
	unsigned message = 0;

	// Output debug log
	outputDebugLogFormat(_T("Execute action: Type=0x%04X, Param=0x%04X"), actionMacro, (unsigned)wParam);

	// Get action ID
	switch (actionMacro)
	{
	case APP_MACRO_LEFT_MOUSE:
		// Get action ID: Left mouse
		actionId = getAppOption(AppOptionID::leftMouseAction);
		break;

	case APP_MACRO_MIDDLE_MOUSE:
		// Get action ID: Middle mouse
		actionId = getAppOption(AppOptionID::middleMouseAction);
		break;

	case APP_MACRO_ACTION_SCHEDULE:
		// Get action ID from param
		if (wParam == NULL) {
			// Trace error
			TRACE_ERROR("Error: Invalid parameter!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return false;
		}
		actionId = DEFAULT_CAST(unsigned, wParam);
		break;

	case APP_MACRO_RIGHT_MOUSE:
		// If right mouse action is set to show notify menu
		if ((getAppOption(AppOptionID::rightMouseAction) == APP_ACTION_SHOWMENU) ||
			(getAppOption(AppOptionID::rightMouseShowMenu) == true))
			return showNotifyMenu();

		// Otherwise, get action ID: Right mouse
		actionId = getAppOption(AppOptionID::rightMouseAction);
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
		actionId = DEFAULT_CAST(unsigned, wParam);
		break;

	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Get action info
	switch (actionId)
	{
	case APP_ACTION_NOTHING:
		// Do nothing (litterally)
		// Pretend that the action completes successfully
		TRACE("[executeAction] Do nothing -> Successfully");
		return true;

	case APP_ACTION_DISPLAYOFF:
		// Turn off display
		actionType = APP_ACTIONTYPE_MONITOR;
		message = APP_MESSAGE_DISPLAYOFF;
		historyActionId = HistoryAction::DisplayOff;
		actionNameLangId = ACTION_NAME_DISPLAYOFF;
		break;

	case APP_ACTION_SLEEP:
		// Sleep
		actionType = APP_ACTIONTYPE_POWER;
		message = APP_MESSAGE_SLEEP;
		historyActionId = HistoryAction::SleepMode;
		actionNameLangId = ACTION_NAME_SLEEP;
		break;

	case APP_ACTION_SHUTDOWN:
		// Shutdown
		actionType = APP_ACTIONTYPE_POWER;
		message = APP_MESSAGE_SHUTDOWN;
		historyActionId = HistoryAction::Shutdown;
		actionNameLangId = ACTION_NAME_SHUTDOWN;
		break;

	case APP_ACTION_RESTART:
		// Restart
		actionType = APP_ACTIONTYPE_POWER;
		message = APP_MESSAGE_REBOOT;
		historyActionId = HistoryAction::Restart;
		actionNameLangId = ACTION_NAME_RESTART;
		break;

	case APP_ACTION_SIGNOUT:
		// Sign out
		actionType = APP_ACTIONTYPE_POWER;
		message = APP_MESSAGE_SIGNOUT;
		historyActionId = HistoryAction::SignOut;
		actionNameLangId = ACTION_NAME_SIGNOUT;
		break;

	case APP_ACTION_HIBERNATE:
		// Hibernate
		actionType = APP_ACTIONTYPE_POWER;
		message = APP_MESSAGE_HIBERNATE;
		historyActionId = HistoryAction::Hibernate;
		actionNameLangId = ACTION_NAME_HIBERNATE;
		break;

	default:
		// Trace error
		TRACE_ERROR("Error: Wrong argument!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Action result info
	bool result = false;
	DWORD errorCode = APP_ERROR_SUCCESS;

	// Confirm before executing action
	if (confirmActionExec(actionMacro, actionId) == IDYES) {

		// Execute Power Reminder before doing action
		executePowerReminder(PwrReminderEvent::beforePwrAction);

		// Check if DummyTest mode is enabled or not
		bool dummyTestMode = getDummyTestMode();

		if (dummyTestMode != true) {
			// Normal mode
			result = AppCore::executePowerAction(actionType, message, errorCode);
		}
		else {
			// DummyTest mode
			result = AppCore::executePowerActionDummy(actionType, message, errorCode);
		}

		// Save Power Action trace flag
		setPwrActionFlag((result == true) ? FLAG_ON : FLAG_OFF);
		CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
		if (theAppPtr != NULL) {
			theAppPtr->saveGlobalData(DEF_GLBDATA_CATE_APPFLAGS);
		}

		// Collect power action history info
		initPwrActionHistoryInfo(historyActionId, result, errorCode);
		saveHistoryInfoData();

		// Show error message
		showErrorMessage(errorCode);
	}
	else {
		// Power action canceled --> Output event log
		String pwrActionNameString;
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		if (languageTablePtr != NULL) {
			pwrActionNameString = getLanguageString(languageTablePtr, actionNameLangId);
		}
		outputEventLog(LOG_EVENT_CANCEL_PWRACTION, pwrActionNameString);
	}

	return result;
}


/**
 * @brief	Apply changes and minimize window to tray
 * @param	isMinimized - Minimize to tray after applying changes
 * @return	None
 */
void CPowerPlusDlg::applySettings(bool isMinimized)
{
	// Update data
	updateDialogData(true);

	// Copy configurations and save
	appConfigData_.copy(tempConfigData_);
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		theAppPtr->setAppConfigData(&appConfigData_);
		bool returnFlag = theAppPtr->saveRegistryAppData();
		if (returnFlag == false) {
			// Show error message
			DWORD dwErr = APP_ERROR_SAVE_CFG_FAILED;
			showErrorMessage(dwErr);
		}
	}

	// Restart as admin privileges if triggered
	bool isRestartTriggered = getFlagValue(AppFlagID::restartAsAdmin);
	if (isRestartTriggered == true) {
		requestRestartApp(IDC_APPLY_BTN, true);
	}

	// Update background hotkeys if enabled
	setupBackgroundHotkey(Mode::Update);

	// Disable Power Reminder snooze queue if feature's disabled
	if (getAppOption(AppOptionID::enablePowerReminder) == NULL) {
		updatePwrReminderSnooze(Mode::Disable);
	}

	// Update notify icon tip text
	updateNotifyIcon();

	// Change display status
	if (isMinimized == true) {
		// Minimize to tray (hide dialog)
		showDialog(this, false);
	}
}


/**
 * @brief	Abort all changes and reload settings
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::reloadSettings(void)
{
	// Reload app data
	getAppData();

	// Reload app language
	((CPowerPlusApp*)AfxGetApp())->reloadAppLanguage();

	// Reupdate dialog items
	setupLanguage();
	updateDialogData(false);

	// Reset data change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);
}


/**
 * @brief	Set default config data and re-update display
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::setDefaultConfig(void)
{
	// Set default options
	tempConfigData_.setDefaultData();

	// Reload app language & reset language display
	((CPowerPlusApp*)AfxGetApp())->reloadAppLanguage();
	setupLanguage();

	// Reupdate dialog items
	updateDialogData(false);
}


/**
 * @brief	Restart application with/without admin privileges
 * @param	restartAsAdmin - Restart with/without admin privileges
 * @return	None
 */
void CPowerPlusDlg::restartApp(bool restartAsAdmin)
{
	// Remove window title to prevent from startup checking
	this->SetWindowText(Constant::String::Null);

	// Exit the current application instance
	exitApp(ExitCode::RestartApp);

	// Restart immediately
	AppCore::runApp(StringUtils::getApplicationPath(true), restartAsAdmin);
}


/**
 * @brief	Safely trigger exitting the application
 * @param	exitCode - Application exit code (use for PostQuitMessage)
 * @return	None
 */
void CPowerPlusDlg::exitApp(int exitCode)
{
	// Request closing opening dialogs and do clean-up
	if (PreDestroyDialog() != Result::Success) {
		TRACE("Request close application denied!!!");
		return;
	}

	// Tell the application by updating flag value
	setFlagValue(AppFlagID::appExitCode, exitCode);

	// Termination
	// Trigger exitting by posting quit message
	PostQuitMessage(exitCode);
}


//////////////////////////////////////////////////////////////////////////
// Dialog and window functions


/**
 * @brief	Show/hide dialog out of/into system tray
 * @param	windowPtr	  - Pointer of the dialog to show/hide
 * @param	showFlag - Flag to show/hide dialog
 * @return	None
 */
void CPowerPlusDlg::showDialog(CWnd* windowPtr, bool showFlag /* = true */)
{
	// Get show/hide flag
	int showCommand = (showFlag == true) ? SW_SHOW : SW_HIDE;

	// Check dialog pointer validity
	if (windowPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Window does not exist!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Show/hide dialog
	windowPtr->ShowWindow(showCommand);
	if (showCommand == SW_SHOW) {
		windowPtr->BringWindowToTop();
	}

	// Save app event log if enabled
	unsigned eventId = (showFlag == true) ? LOG_EVENT_DLG_SHOWED : LOG_EVENT_DLG_HIDDEN;
	outputEventLog(eventId, ((SDialog*)windowPtr)->getCaption());
}


/**
 * @brief	Open child dialog with corresponding ID
 * @param	dialogId - Child dialog ID
 * @return	None
 */
void CPowerPlusDlg::openChildDialogEx(unsigned dialogId)
{
	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) {
		// Trace error
		TRACE_ERROR("Error: Get app pointer failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// About dialog
	if (dialogId == IDD_ABOUT_DLG) {
		if (aboutDlgPtr_ == NULL) {
			// Initialize dialog
			aboutDlgPtr_ = new CAboutDlg();
			aboutDlgPtr_->setParent(this);
			aboutDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			aboutDlgPtr_->setParent(this);
			showDialog(aboutDlgPtr_, true);
		}
	}
	// Help dialog
	else if (dialogId == IDD_HELP_DLG) {
		if (helpDlgPtr_ == NULL) {
			// Initialize dialog
			helpDlgPtr_ = new CHelpDlg();
			helpDlgPtr_->setParent(this);
			helpDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			helpDlgPtr_->setParent(this);
			showDialog(helpDlgPtr_, true);
		}
	}
	// Multi schedule dialog
	else if (dialogId == IDD_MULTISCHEDULE_DLG) {
		if (multiScheduleDlgPtr_ == NULL) {
			// Initialize dialog
			multiScheduleDlgPtr_ = new CMultiScheduleDlg();
			multiScheduleDlgPtr_->setParent(this);
			multiScheduleDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			multiScheduleDlgPtr_->setParent(this);
			showDialog(multiScheduleDlgPtr_, true);
		}
	}
	// LogViewer dialog
	else if (dialogId == IDD_LOGVIEWER_DLG) {
		if (logViewerDlgPtr_ == NULL) {
			// Initialize dialog
			logViewerDlgPtr_ = new CLogViewerDlg();
			logViewerDlgPtr_->setParent(this);
			logViewerDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			logViewerDlgPtr_->setParent(this);
			showDialog(logViewerDlgPtr_, true);
		}
	}
	// HotkeySet dialog
	else if (dialogId == IDD_HOTKEYSET_DLG) {
		if (hotkeySetDlgPtr_ == NULL) {
			// Initialize dialog
			hotkeySetDlgPtr_ = new CHotkeySetDlg();
			hotkeySetDlgPtr_->setParent(this);
			hotkeySetDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			hotkeySetDlgPtr_->setParent(this);
			showDialog(hotkeySetDlgPtr_, true);
		}
	}
	// Power Reminder dialog
	else if (dialogId == IDD_PWRREMINDER_DLG) {
		if (pwrReminderDlgPtr_ == NULL) {
			// Initialize dialog
			pwrReminderDlgPtr_ = new CPwrReminderDlg();
			pwrReminderDlgPtr_->setParent(this);
			pwrReminderDlgPtr_->DoModal();
		}
		else {
			// Show dialog
			pwrReminderDlgPtr_->setParent(this);
			showDialog(pwrReminderDlgPtr_, true);
		}
	}
	// DebugTest dialog
	else if (dialogId == IDD_DEBUGTEST_DLG) {
		// Get app DebugTest dialog
		SDialog* dialogPtr = theAppPtr->getDebugTestDlg();
		if (dialogPtr == NULL) {

			// Initialize dialog
			bool bRetInit = theAppPtr->initDebugTestDlg();
			if (bRetInit != false) {

				// Re-acquire DebugTest dialog pointer
				dialogPtr = theAppPtr->getDebugTestDlg();
				if (dialogPtr != NULL) {
					// Set parent window
					dialogPtr->setParent(this);

					// Open in modeless mode
					dialogPtr->Create(dialogId, this);
					dialogPtr->ShowWindow(SW_SHOW);
				}
			}
		}
		else {
			// If DebugTest dialog is already created
			if (::IsWindow(dialogPtr->GetSafeHwnd())) {
				// Show dialog
				dialogPtr->setParent(this);
				showDialog(dialogPtr, true);
			}
			else {
				// Set parent window
				dialogPtr->setParent(this);

				// Open in modeless mode
				dialogPtr->Create(dialogId, this);
				dialogPtr->ShowWindow(SW_SHOW);
			}
		}

		// Bring to top (by default)
		dialogPtr->PostMessage(SM_WND_SHOWDIALOG, true);
	}

	// Base class processing
	SDialog::openChildDialogEx(dialogId);
}


/**
 * @brief	Open dialog with corresponding ID
 * @param	dialogId	  - Dialog ID
 * @param	readOnlyMode - Read-only mode flag
 * @param	openMode	  - Open mode: Modal or modeless
 * @return	None
 * @note:	Base function (no longer used)
 */
void CPowerPlusDlg::openDialogBase(unsigned dialogId, bool readOnlyMode /* = false */, int openMode /* = DEF_MODE_OPENDLG_MODAL */)
{
	// Check if there is any other instance of dialog currently running,
	HWND dialogWndHandle = NULL;
	if (dialogId == IDD_DEBUGTEST_DLG) {
		// Find DebugTest dialog
		dialogWndHandle = AppCore::findDebugTestDlg();
	}
	else {
		// Find dialog by title
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		String dialogTitle = getLanguageString(languageTablePtr, dialogId);
		dialogWndHandle = ::FindWindow(NULL, dialogTitle);
	}

	// If yes, only bring that instance up to top instead of starting a new one
	if (dialogWndHandle != NULL) {
		CWnd* pDialogWnd = FromHandle(dialogWndHandle);
		showDialog(pDialogWnd, true);
		return;
	}

	// Dialog initialization
	SDialog* dialogPtr = NULL;
	bool isReadOnlySet = false;
	CWnd* parentWnd = NULL;
	unsigned entryMessageId = Constant::kNullInteger;
	{
		switch (dialogId)
		{
		case IDD_ABOUT_DLG:
			// About dialog
			dialogPtr = new CAboutDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_HELP_DLG:
			// Help dialog
			dialogPtr = new CHelpDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_MULTISCHEDULE_DLG:
			// Multi schedule dialog
			dialogPtr = new CMultiScheduleDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_LOGVIEWER_DLG:
			// LogViewer dialog
			dialogPtr = new CLogViewerDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_HOTKEYSET_DLG:
			// HotkeySet dialog
			dialogPtr = new CHotkeySetDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_PWRREMINDER_DLG:
			// Power Reminder dialog
			dialogPtr = new CPwrReminderDlg;
			isReadOnlySet = false;
			parentWnd = this;
			break;

		case IDD_DEBUGTEST_DLG:
			// DebugTest dialog
			dialogPtr = new CDebugTestDlg;
			isReadOnlySet = false;
			parentWnd = this;
			openMode = MODE_OPENDLG_MODELESS;
			break;

		default:
			break;
		}

		// Show entry message before displaying dialog
		if (entryMessageId != Constant::kNullInteger) {
			displayMessageBox(entryMessageId);
		}

		// Check validity
		if (dialogPtr == NULL)
			return;

		// Set read-only mode
		if (isReadOnlySet == true) {
			dialogPtr->setReadOnlyMode(readOnlyMode);
		}

		// Open dialog
		if (openMode == MODE_OPENDLG_MODAL) {
			// Modal dialog
			dialogPtr->setParent(parentWnd);
			dialogPtr->DoModal();
		}
		else if (openMode == MODE_OPENDLG_MODELESS) {
			// Modeless dialog
			dialogPtr->Create(dialogId, parentWnd);
			dialogPtr->ShowWindow(SW_SHOW);
			dialogPtr->RunModalLoop();
		}
	}

	// Delete dialog pointer when done
	if (dialogPtr != NULL) {
		delete dialogPtr;
		dialogPtr = NULL;
	}
}


/**
 * @brief	Open text file to view using Notepad
 * @param	fileName  - File name/path
 * @param	extension - File extension
 * @param	subDir	  - Sub-directory name
 * @return	bool - Result of file opening
 */
bool CPowerPlusDlg::openTextFileToView(const wchar_t* fileName, const wchar_t* extension, const wchar_t* subDir /* = Constant::String::Empty */)
{
	// Get file name
	VERIFY(fileName != NULL);
	String filePath(fileName);
	filePath.append(extension);
	
	// If sub-directory name is not empty
	if (IS_NOT_EMPTY_STRING(subDir)) {
		// Format file path with sub-directory
		String folderPath = StringUtils::getSubFolderPath(subDir);
		filePath = StringUtils::makeFilePath(folderPath, fileName, extension);
	}

	// Standard file view
	return AppCore::fileViewStd(FILETYPE_TEXT, filePath);
}


//////////////////////////////////////////////////////////////////////////
// Action Schedule feature functions


/**
 * @brief	Process Action schedule function
 * @param	None
 * @return	bool - Schedule processing result
 */
bool CPowerPlusDlg::processActionSchedule(void)
{
	bool result = false;

	// Get current time
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Flag that trigger to reupdate schedule data
	bool isReupdateTriggered = false;

	// Get default schedule item
	ScheduleItem& defaultScheduleItem = scheduleData_.getDefaultItem();
	{
		// Flag to skip processing schedule item
		bool skipProcessFlag = false;

		// If item is not enabled
		if (defaultScheduleItem.isEnabled() == false) {
			// Do not process
			skipProcessFlag = true;
		}

		// If repeat option is ON and is set as active in current day of week
		if ((defaultScheduleItem.isRepeatEnabled() == true) && (!defaultScheduleItem.isDayActive((DayOfWeek)currentDateTime.dayOfWeek()))) {
			// Do not process
			skipProcessFlag = true;
		}

		// Process default schedule
		if (skipProcessFlag != true) {

			// Check for time matching and trigger schedule notifying if enabled
			if (getAppOption(AppOptionID::scheduleNotification) == true) {
				bool notifyTriggerFlag = ClockTimeUtils::isMatching(currentDateTime.getClockTime(), defaultScheduleItem.getTime(), -30);
				if (notifyTriggerFlag == true) {
					// Do notify schedule (and check for trigger reupdate)
					notifySchedule(&defaultScheduleItem, isReupdateTriggered);
					result = false;
				}
			}

			// Check for time matching and trigger the scheduled action
			bool actionTriggerFlag = ClockTimeUtils::isMatching(currentDateTime.getClockTime(), defaultScheduleItem.getTime());
			if (actionTriggerFlag == true) {
				
				// Check if item is marked as skipped
				bool skipFlag = getActionScheduleSkipStatus(defaultScheduleItem.getItemId());
				if (skipFlag != true) {

					// Output event log: Schedule executed
					outputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, defaultScheduleItem);

					// Save history info data
					initScheduleHistoryInfo(defaultScheduleItem);
					saveHistoryInfoData();

					// Execute schedule action
					result = executeAction(APP_MACRO_ACTION_SCHEDULE, defaultScheduleItem.getAction());

					// If "Repeat" option is not ON,
					// --> Disable schedule item after done
					if (defaultScheduleItem.isRepeatEnabled() == false) {
						defaultScheduleItem.enableItem(false);
						isReupdateTriggered |= true;
					}
				}
				else {
					// Process failed
					result = false;
				}

				// Set item as no longer skipped
				setActionScheduleSkip(defaultScheduleItem, FLAG_OFF);
			}
		}
		else {
			// Process failed
			result = false;
		}
	}

	// Loop through each extra item and process
	for (int extraIndex = 0; extraIndex < scheduleData_.getExtraItemNum(); extraIndex++) {

		// Get schedule item
		ScheduleItem& extraScheduleItem = scheduleData_.getItemAt(extraIndex);

		// Do not process if repeat option is ON but is not set as active in current day of week
		if ((extraScheduleItem.isRepeatEnabled() == true) && (!extraScheduleItem.isDayActive((DayOfWeek)currentDateTime.dayOfWeek())))
			continue;

		// Check for time matching and trigger schedule notifying if enabled
		if (getAppOption(AppOptionID::scheduleNotification) == true) {
			bool notifyTriggerFlag = ClockTimeUtils::isMatching(currentDateTime.getClockTime(), extraScheduleItem.getTime(), -30);
			if (notifyTriggerFlag == true) {
				// Do notify schedule (and check for trigger reupdate)
				notifySchedule(&extraScheduleItem, isReupdateTriggered);
				result = false;
				continue;
			}
		}

		// Check for time matching and trigger the scheduled action
		bool actionTriggerFlag = ClockTimeUtils::isMatching(currentDateTime.getClockTime(), extraScheduleItem.getTime());
		if (actionTriggerFlag == true) {

			// Check if item is marked as skipped
			bool skipFlag = getActionScheduleSkipStatus(extraScheduleItem.getItemId());
			if (skipFlag != true) {

				// Output event log: Schedule executed
				outputScheduleEventLog(LOG_EVENT_EXEC_SCHEDULE, extraScheduleItem);

				// Save history info data
				initScheduleHistoryInfo(extraScheduleItem);
				saveHistoryInfoData();

				// Execute schedule action
				result = executeAction(APP_MACRO_ACTION_SCHEDULE, extraScheduleItem.getAction());

				// If "Repeat" option is not ON,
				// --> Disable schedule item after done
				if (extraScheduleItem.isRepeatEnabled() == false) {
					extraScheduleItem.enableItem(false);
					isReupdateTriggered |= true;
				}
			}
			else {
				// Process failed
				result = false;
			}

			// Set item as no longer skipped
			setActionScheduleSkip(extraScheduleItem, FLAG_OFF);
		}
	}

	// Reupdate flag is triggered
	if (isReupdateTriggered == true) {
		// Reupdate schedule data
		reupdateActionScheduleData();
		result = true;
	}

	return result;
}


/**
 * @brief	Reupdate Action Schedule data
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::reupdateActionScheduleData(void)
{
	// Disable Action schedule items
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		// Update schedule data
		theAppPtr->setAppScheduleData(&scheduleData_);
		theAppPtr->saveRegistryAppData(APPDATA_SCHEDULE);
	}

	// Trigger reupdate schedule data
	PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);
}


/**
 * @brief	Setup Action Schedule item skip mode
 * @param	scheduleItem	  - Action schedule item
 * @param	skipFlag - Skip flag
 * @return	None
 */
void CPowerPlusDlg::setActionScheduleSkip(const ScheduleItem& scheduleItem, int skipFlag)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty()) return;

	// Find if item skip mode is already setup
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not an Action Schedule item
		if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == scheduleItem.getItemId()) {

			// Update item skip mode data
			runtimeItem.setSkipFlag(skipFlag);
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem runtimeItem;
	runtimeItem.setCategory(PwrFeatureID::schedule);
	runtimeItem.setItemId(scheduleItem.getItemId());
	runtimeItem.setSkipFlag(skipFlag);

	// Add item to runtime queue
	runtimeQueue_.push_back(runtimeItem);
}


/**
 * @brief	Setup Action Schedule item snooze mode
 * @param	scheduleItem		- Action schedule item
 * @param	snoozeFlag - Snooze flag
 * @return	None
 */
void CPowerPlusDlg::setActionScheduleSnooze(const ScheduleItem& scheduleItem, int snoozeFlag)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty()) return;

	// If snoozing option is not available
	if (scheduleItem.isAllowSnoozing() != true) {
		// Turn off flag
		snoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int interval = PwrRepeatSet::kDefaultSnoozeInterval;

	// Find if item skip mode is already setup
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not an Action Schedule item
		if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == scheduleItem.getItemId()) {

			// Update item skip mode data
			runtimeItem.setSnoozeFlag(snoozeFlag);

			if (runtimeItem.getSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				runtimeItem.setTime(ClockTimeUtils::getCurrentClockTime());
				runtimeItem.calcNextSnoozeTime(interval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem runtimeItem;
	runtimeItem.setCategory(PwrFeatureID::schedule);
	runtimeItem.setItemId(scheduleItem.getItemId());
	runtimeItem.setSnoozeFlag(snoozeFlag);
	if (runtimeItem.getSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		runtimeItem.setTime(ClockTimeUtils::getCurrentClockTime());
		runtimeItem.calcNextSnoozeTime(interval);
	}

	// Add item to runtime queue
	runtimeQueue_.push_back(runtimeItem);
}


/**
 * @brief	Update Action Schedule runtime queue data
 * @param	mode - Update mode flag (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::updateActionScheduleQueue(int mode)
{
	if (mode == Mode::Init) {

		// Initialize Action Schedule runtime queue
		// Set all items as not skipped
		setActionScheduleSkip(scheduleData_.getDefaultItem(), FLAG_OFF);
		for (int itemIndex = 0; itemIndex < scheduleData_.getExtraItemNum(); itemIndex++) {
			setActionScheduleSkip(scheduleData_.getItemAt(itemIndex), FLAG_OFF);
		}
	}
	else if (mode == Mode::Update) {

		// Update skip queue items
		for (int queueIndex = (runtimeQueue_.size() - 1); queueIndex >= 0; queueIndex--) {

			// Get runtime item from queue
			PwrRuntimeItem& runtimeItem = runtimeQueue_.at(queueIndex);

			// Skip if it's not an Action Schedule item
			if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

			// Item found flag
			bool isItemFound = false;

			// Check if item ID is default schedule item
			const ScheduleItem& defaultScheduleItem = scheduleData_.getDefaultItem();
			if (defaultScheduleItem.getItemId() == runtimeItem.getItemId()) {

				// If item's snoozing mode is no longer available
				if (!defaultScheduleItem.isAllowSnoozing()) {
					// Disable snooze mode
					runtimeItem.setSnoozeFlag(FLAG_OFF);
				}
				
				// Mark as found
				isItemFound = true;
				break;
			}

			// Search for item ID in Action Schedule extra item data
			for (int itemIndex = 0; itemIndex < scheduleData_.getExtraItemNum(); itemIndex++) {
				const ScheduleItem& scheduleItem = scheduleData_.getItemAt(itemIndex);
				if (scheduleItem.getItemId() == runtimeItem.getItemId()) {

					// If item's snoozing mode is no longer available
					if (!scheduleItem.isAllowSnoozing()) {
						// Disable snooze mode
						runtimeItem.setSnoozeFlag(FLAG_OFF);
					}

					// Mark as found
					isItemFound = true;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (isItemFound == false) {
				// Remove item from runtime queue
				runtimeQueue_.erase(runtimeQueue_.begin() + queueIndex);
			}
		}
	}
	else if (mode == Mode::Disable) {

		// Disable skip mode for all items in queue
		for (int index = 0; index < runtimeQueue_.size(); index++) {

			// Get runtime item from queue
			PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

			// Skip if it's not an Action Schedule item
			if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

			// Update item skip mode data
			runtimeItem.setSkipFlag(FLAG_OFF);

			// Update item snooze mode data
			runtimeItem.setSnoozeFlag(FLAG_OFF);
		}
	}
}


/**
 * @brief	Get Action Schedule item skip flag status
 * @param	itemId	- Action Schedule item ID
 * @return	true/false
 */
bool CPowerPlusDlg::getActionScheduleSkipStatus(unsigned itemId)
{
	// If runtime queue data is empty, do not trigger
	if (runtimeQueue_.empty())
		return false;

	// Find for item ID
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		const PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not an Action Schedule item
		if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == itemId) {

			// Get skip enable flag
			if (runtimeItem.getSkipFlag() == FLAG_ON)
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Get Action Schedule item snooze trigger status
 * @param	itemId	   - Action Schedule item ID
 * @param	curSysTime - Current system time
 * @return	true/false
 */
bool CPowerPlusDlg::getActionScheduleSnoozeStatus(unsigned itemId, const ClockTime& currentTime)
{
	// If runtime queue data is empty, do not trigger
	if (runtimeQueue_.empty())
		return false;

	// Find for item ID
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		const PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not an Action Schedule item
		if (runtimeItem.getCategory() != PwrFeatureID::schedule) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == itemId) {

			// Get snooze enable flag
			if (runtimeItem.getSnoozeFlag() == FLAG_OFF)
				return false;

			// Check for next snooze time matching
			if (ClockTimeUtils::isMatching(currentTime, runtimeItem.getTime()))
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
 * @param	mode - Mode of setup (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::setupBackgroundHotkey(int mode)
{
	// Get option and flag values
	bool isHotkeySetEnabled = getAppOption(AppOptionID::enableBackgroundHotkey);
	bool isHotkeyRegistered = getFlagValue(AppFlagID::hotkeyRegistered);

	// If background hotkey feature is disabled and no hotkey registered, do nothing
	if ((isHotkeySetEnabled == false) &&											// HotkeySet option OFF
		(isHotkeyRegistered == false) &&										// Hotkey registered flag OFF
		(currentRegHotkeyList_.empty())) {									// Registered hotkey list empty
		TRACE("Background hotkey setup will be skipped!!!");
		return;
	}

	// Get window handle
	HWND windowHandle = this->GetSafeHwnd();
	DWORD errorCode;

	/*********************************************************************/
	/*																	 */
	/*	         Unregister already registered HotkeySet items			 */
	/*	             Applied for modes: DISABLE or UPDATE                */
	/*																	 */
	/*********************************************************************/

	if ((mode == Mode::Disable) || (mode == Mode::Update)) {

		if ((isHotkeyRegistered == true) &&									// Hotkey registered flag ON
			(!currentRegHotkeyList_.empty())) {								// Registered hotkey list is NOT empty

			// Unregister currently registered hotkeys
			int regItemNum = currentRegHotkeyList_.size();
			for (int index = (regItemNum - 1); index >= 0; index--) {
				unsigned hotkeyId = currentRegHotkeyList_.at(index);
				bool returnFlag = UnregisterHotKey(windowHandle, hotkeyId);
				if (returnFlag == true) {
					// Unregister successfully
					outputDebugLogFormat(_T("Unregistered hotkey: %d"), hotkeyId);
					currentRegHotkeyList_.erase(currentRegHotkeyList_.begin() + index);
					if (index == 0) {										// Last item unregistered
						setFlagValue(AppFlagID::hotkeyRegistered, false);	// Reset hotkey registered flag
						currentRegHotkeyList_.clear();						// Cleanup registered hotkey list
					}
				}
				else {
					// Unregister failed
					errorCode = GetLastError();

					// Output debug log
					outputDebugLogFormat(_T("Unregister hotkey failed: %d"), hotkeyId);

					// Trace error
					TRACE_FORMAT("Error: Hotkey unregister failed!!! (Code=0x%X)", errorCode);
					TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

					// Show error message
					showErrorMessage(errorCode);
					displayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);
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

	if ((mode == Mode::Init) || (mode == Mode::Update)) {

		// If feature not enabled, do nothing
		if (isHotkeySetEnabled == false) {
			TRACE("Background hotkey setup will be skipped!!!");
			return;
		}

		// If there's no item, do nothing
		int itemNum = hotkeySetData_.getItemNum();
		if (itemNum <= 0) {
			TRACE("There is not Hotkey item!!!");
			return;
		}

		// Reset flag and re-initialize registered hotkey list
		setFlagValue(AppFlagID::hotkeyRegistered, false);
		currentRegHotkeyList_.clear();
		currentRegHotkeyList_.reserve(itemNum);

		// Debug log
		String tempLogString;

		// Register each HotkeySet item
		bool isRegistered = false;
		for (int index = 0; index < itemNum; index++) {

			// Get hotkey item
			HotkeySetItem hotkeyItem = hotkeySetData_.getItemAt(index);

			// If item is empty, skip this item
			if (hotkeyItem.isEmpty()) 
				continue;

			// Get hotkey action ID
			unsigned hotkeyActionId = hotkeyItem.getActionId();

			// Get keycode
			DWORD modifiers, virtualKey;
			hotkeyItem.getKeyCode(modifiers, virtualKey);
			if ((modifiers == 0) || (virtualKey == 0))
				continue;

			// No yeild keyboard auto-repeat
			modifiers |= MOD_NOREPEAT;

			// Debug log format
			tempLogString.format(_T("ActionID=%d, Modifiers=%d, VirtualKey=%d"), hotkeyActionId, modifiers, virtualKey);

			// Get enable/disable status
			bool isEnabled = hotkeyItem.isEnabled();

			// Skip registering item if disabled
			if (isEnabled == false) {
				outputDebugLogFormat(_T("Skip registering hotkey (disabled): %s"), tempLogString.getString());
				continue;
			}

			// Register hotkey item
			bool returnFlag = RegisterHotKey(windowHandle, hotkeyActionId, modifiers, virtualKey);

			// Trigger flag
			isRegistered |= returnFlag;
			setFlagValue(AppFlagID::hotkeyRegistered, isRegistered);

			if (returnFlag == true) {
				// Register successfully
				outputDebugLogFormat(_T("Registered hotkey: %s"), tempLogString.getString());
				currentRegHotkeyList_.push_back(hotkeyActionId);						// Update registered hotkey list
			}
			else {
				// Register failed
				errorCode = GetLastError();

				// Output debug log
				outputDebugLogFormat(_T("Register hotkey failed: %s"), tempLogString.getString());

				// Trace error
				TRACE_FORMAT("Error: Hotkey register failed!!! (Code=0x%X)", errorCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				showErrorMessage(errorCode);
				displayMessageBox(MSGBOX_HOTKEYSET_UNREG_FAILED, MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);
			}
		}
	}
}


/**
 * @brief	Process when registered hotkey is pressed
 * @param	hotkeyId - ID of pressed hotkey
 * @return	bool - Hotkey processing result
 */
bool CPowerPlusDlg::processHotkey(int hotkeyId)
{
	// If "Background hotkey" option is not enabled, do nothing
	if (getAppOption(AppOptionID::enableBackgroundHotkey) == false) {
		TRACE("Background hotkey option is not enabled!!!");
		return false;
	}

	// Output debug log
	outputDebugLogFormat(_T("Process Hotkey: HKeyID=%d"), hotkeyId);

	// Get HotkeySet item by ID
	HotkeySetItem hotkeyItem;
	for (int index = 0; index < hotkeySetData_.getItemNum(); index++) {
		HotkeySetItem& tempHotkeyItem = hotkeySetData_.getItemAt(index);
		if (tempHotkeyItem.getActionId() == static_cast<unsigned>(hotkeyId)) {
			hotkeyItem.copy(tempHotkeyItem);
			break;
		}
	}

	// Check item validity
	if (hotkeyItem.isEmpty())
		return false;

	// Output hotkey event log
	String hotkeyDescription;
	hotkeyItem.printKeyStrokes(hotkeyDescription);
	outputEventLog(LOG_EVENT_EXEC_HOTKEY, hotkeyDescription);

	// Get Power action ID by HotkeyID
	unsigned actionId = NULL;
	switch (hotkeyId)
	{
	case HotkeyID::displayOff:
		actionId = APP_ACTION_DISPLAYOFF;
		break;
	case HotkeyID::sleep:
		actionId = APP_ACTION_SLEEP;
		break;
	case HotkeyID::shutdown:
		actionId = APP_ACTION_SHUTDOWN;
		break;
	case HotkeyID::restart:
		actionId = APP_ACTION_RESTART;
		break;
	case HotkeyID::signOut:
		actionId = APP_ACTION_SIGNOUT;
		break;
	case HotkeyID::hibernate:
		actionId = APP_ACTION_HIBERNATE;
		break;

	default:
		TRACE("Wrong argument!!!");
		break;
	}

	// If Power action ID is invalid, do nothing
	if (actionId == NULL) {
		// Trace error
		TRACE_ERROR("Error: Invalid Power action ID!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Save history info data
	initHotkeyHistoryInfo(hotkeyId);
	saveHistoryInfoData();
	
	// Execute hotkeyset action
	WPARAM wParam = static_cast<WPARAM>(actionId);
	bool result = executeAction(APP_MACRO_ACTION_HOTKEY, wParam);
	return result;
}


/**
 * @brief	Register/unregister to receive session state change notification
 * @param	mode - Mode of setup (INIT, UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::registerSessionNotification(int mode)
{
	DWORD errorCode;
	HWND currentWndHandle = this->GetSafeHwnd();

	/*********************************************************************/
	/*																	 */
	/*		  Unregister for session state change notifications			 */
	/*	            Applied for modes: DISABLE or UPDATE		         */
	/*																	 */
	/*********************************************************************/

	if ((mode == Mode::Disable) || (mode == Mode::Update)) {

		// Only unregister if the flag is not OFF
		if (getFlagValue(AppFlagID::wtsSessionNotifyRegistered)) {

			if (WTSUnRegisterSessionNotification(currentWndHandle)) {
				// Mark flag as OFF
				setFlagValue(AppFlagID::wtsSessionNotifyRegistered, false);
			}
			else {
				// Unregister failed
				errorCode = GetLastError();

				// Trace error
				TRACE_FORMAT("Error: Unregister session notification failed!!! (Code=%d)", errorCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				showErrorMessage(errorCode);
			}
		}
	}

	/*********************************************************************/
	/*																	 */
	/*			Register for session state change notifications			 */
	/*				   Applied for modes: INIT or UPDATE		         */
	/*																	 */
	/*********************************************************************/

	if ((mode == Mode::Init) || (mode == Mode::Update)) {

		// Only register if the flag is not ON
		if (!getFlagValue(AppFlagID::wtsSessionNotifyRegistered)) {

			if (WTSRegisterSessionNotification(currentWndHandle, NOTIFY_FOR_THIS_SESSION)) {
				// Mark flag as ON
				setFlagValue(AppFlagID::wtsSessionNotifyRegistered, true);
			}
			else {
				// Register failed
				errorCode = GetLastError();

				// Trace error
				TRACE_FORMAT("Error: Register session notification failed!!! (Code=%d)", errorCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				showErrorMessage(errorCode);
			}
		}
	}
}


/**
 * @brief	Process when registered hotkey is pressed in lock state
					(this function will convert key-hooked param sent from the
					application class into corresponding registered hotkey ID)
 * @param	hotkeyParam - Hotkey parameters (sent from app class)
 * @return	bool - Hotkey processing result
 * @note	If the hotkey ID is found and is registered, the hotkey will be processed and executed using the base processHotkey function
 */
bool CPowerPlusDlg::processLockStateHotkey(DWORD hotkeyParam)
{
	// Only process if both options are enabled
	if ((getAppOption(AppOptionID::enableBackgroundHotkey) != true) ||
		(getAppOption(AppOptionID::allowLockscreenHotkey) != true)) {
		TRACE("[LockState Hotkey] Either required options are not enabled!!!");
		return false;
	}

	// Convert hotkey param back into keycode
	WORD modifiers  = LOWORD(hotkeyParam);
	WORD virtualKey = HIWORD(hotkeyParam);

	// Look for corresponding HotkeyID in HotkeySet data
	unsigned hotkeyActionId = Constant::kNullInteger;
	for (size_t index = 0; index < hotkeySetData_.getItemNum(); index++) {
		const HotkeySetItem& hotkeyItem = hotkeySetData_.getItemAt(index);
		if ((hotkeyItem.isEnabled() == true) &&						// HotkeySet item is enabled
			(hotkeyItem.compareKeycode(modifiers, virtualKey))) {	// Keycode is matching
			hotkeyActionId = hotkeyItem.getActionId();
			break;
		}
	}

	// Do not process if HotkeyID is NOT available
	if (hotkeyActionId == Constant::kNullInteger) {
		TRACE("[LockState Hotkey] Hotkey not found!!!");
		return false;
	}

	// Output debug log
	outputDebugLogFormat(_T("[LockState Hotkey] HotkeyID found: HKeyID=0x%04X (%d)"), hotkeyActionId, hotkeyActionId);
		
	// Check if HotkeyID is registered
	if (getFlagValue(AppFlagID::hotkeyRegistered) != true) {
		// Trace error
		TRACE_ERROR("[LockState Hotkey] No hotkey registered!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Only process if HotkeyID is registered
	for (size_t index = 0; index < currentRegHotkeyList_.size(); index++) {
		unsigned regHotkeyId = currentRegHotkeyList_.at(index);
		if (regHotkeyId == hotkeyActionId) {
			// Process Hotkey by ID
			return processHotkey(hotkeyActionId);
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// Power Reminder feature functions


/**
 * @brief	Excute Power Reminder action
 * @param	eventId - Execute event ID
 * @return	true/false
 */
bool CPowerPlusDlg::executePowerReminder(unsigned eventId)
{
	// If "Power Reminder" option is not enabled, do nothing
	if (getAppOption(AppOptionID::enablePowerReminder) == false) {
		TRACE("Power Reminder option is not enabled!!!");
		return false;
	}

	// If there's no item, do nothing
	int itemNum = reminderData_.getItemNum();
	if (itemNum <= 0) {
		TRACE("There is no Power Reminder items!!!");
		return false;
	}

	// Get current time
	DateTime currentDateTime;
	ClockTime currentClockTime;
	if (eventId == PwrReminderEvent::atSetTime) {
		currentDateTime = DateTimeUtils::getCurrentDateTime();
		currentClockTime = currentDateTime.getClockTime();
	}

	// Flag that trigger to reupdate Power Reminder data
	bool isReupdateTriggered = false;

	// Search all items and process reminder
	for (int index = 0; index < itemNum; index++) {
		PwrReminderItem& currentItem = reminderData_.getItemAt(index);

		// If item is empty, skip this item
		if (currentItem.isEmpty())
			continue;

		// If event ID is not matching, skip this item
		if (currentItem.getEventId() != eventId)
			continue;

		// If item is not enabled, skip this item
		if (currentItem.isEnabled() == false)
			continue;

		// Process item
		PwrReminderItem displayItem;
		switch (eventId)
		{
		case PwrReminderEvent::atSetTime:
			// If item is set to repeat but not set active in current day of week
			if ((currentItem.isRepeatEnabled() == true) && (!currentItem.isDayActive((DayOfWeek)currentDateTime.dayOfWeek())))
				continue;

			// If set time matching or snooze time is triggered
			if ((ClockTimeUtils::isMatching(currentClockTime, currentItem.getTime())) ||
				(getPwrReminderSnoozeStatus(currentItem.getItemId(), currentClockTime))) {
				// Prepare to display
				displayItem.copy(currentItem);
				setPwrReminderSnooze(currentItem, FLAG_OFF);
			}
			else continue;
			break;

		case PwrReminderEvent::atSysWakeUp:
			// If System suspend flag and Session ending flag are both OFF, do not display
			if ((getSystemSuspendFlag() == FLAG_OFF) && (getSessionEndFlag() == FLAG_OFF)) continue;
			// Otherwise, just prepare to display
			displayItem.copy(currentItem);
			break;

		case PwrReminderEvent::wakeAfterAction:
			// If Power Action flag is OFF, do not display
			if (getPwrActionFlag() == FLAG_OFF) continue;
			// Otherwise, prepare to display
			displayItem.copy(currentItem);
			break;

		case PwrReminderEvent::atAppStartup:
		case PwrReminderEvent::beforePwrAction:
		case PwrReminderEvent::atAppExit:
			// Just prepare to display
			displayItem.copy(currentItem);
			break;

		default:
			continue;
		}

		// Display reminder
		if (!displayItem.isEmpty() && !getPwrReminderDispFlag(displayItem)) {

			// Display reminder item
			displayPwrReminder(displayItem);

			// If repeat daily option is not enabled
			// --> Disable reminder item after displaying
			if (displayItem.isRepeatEnabled() == false) {
				isReupdateTriggered |= true;
				currentItem.enableItem(false);
			}
		}
	}

	// Reupdate flag is triggered
	if (isReupdateTriggered == true) {
		// Reupdate Power Reminder data
		reupdatePwrReminderData();
	}

	return true;
}


/**
 * @brief	Display Power Reminder item
 * @param	displayItem - Item to display
 * @return	int
 */
int CPowerPlusDlg::displayPwrReminder(const PwrReminderItem& displayItem)
{
	// Check message content validity
	String messageContent = displayItem.getMessage();
	if ((messageContent.isEmpty()) || (IS_NULL_STRING(messageContent))) {
		// Invalid message content
		TRACE("Invalid message content!!!");
		return Constant::kInvalidInteger;
	}

	// Output debug log
	outputDebugLogFormat(_T("Display reminder: ItemID=%d"), displayItem.getItemId());

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	int returnFlag = FLAG_OFF;
	int respond = Constant::kNullInteger;

	// Update item runtime displaying flag
	setPwrReminderDispFlag(displayItem, FLAG_ON);

	// Output event log: Display Power Reminder
	outputPwrReminderEventLog(LOG_EVENT_DISP_PWRREMINDER, displayItem);

	// Save history info data
	initPwrReminderHistoryInfo(displayItem);
	saveHistoryInfoData();

	// Style: MessageBox
	if (displayItem.getMessageStyle() == PwrReminderStyle::messageBox) {

		const wchar_t* messageCaption = getLanguageString(languageTablePtr, IDD_PWRREMINDER_DLG);
		DWORD messageStyle = MB_OK | MB_ICONINFORMATION;
		respond = displayMessageBox(messageContent, messageCaption, messageStyle);
	}
	// Style: Dialog
	else if (displayItem.getMessageStyle() == PwrReminderStyle::dialogBox) {

		// Init reminder message dialog
		CReminderMsgDlg* messageDialogPtr = new CReminderMsgDlg(this);
		if (messageDialogPtr == NULL) {

			// Trace error
			TRACE_ERROR("Error: Reminder message dialog initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Update item runtime displaying flag
			setPwrReminderDispFlag(displayItem, FLAG_OFF);
			return Constant::kInvalidInteger;
		}

		// Message style
		RmdMsgStyleSet msgStyleSet = reminderData_.getCommonStyle();
		if (displayItem.isCustomStyleEnabled())
			msgStyleSet = displayItem.getMessageStyleData();

		// Message auto-close interval
		int timeoutValue = msgStyleSet.getTimeout();

		// Allow snooze mode
		bool isSnoozingAllowed = displayItem.isAllowSnoozing();

		// Set allow snooze mode
		messageDialogPtr->setAllowSnoozeMode(isSnoozingAllowed);

		// Set properties
		messageDialogPtr->setCaptionFromLanguage(IDD_PWRREMINDER_DLG);
		messageDialogPtr->setDispMessage(messageContent);
		messageDialogPtr->setMessageStyle(msgStyleSet);
		messageDialogPtr->setAutoCloseInterval(timeoutValue);

		// Set notify state flags
		messageDialogPtr->setTopMost(true);
		messageDialogPtr->setInitSound(true);

		// Display message
		respond = messageDialogPtr->DoModal();

		// Get returned flag
		messageDialogPtr->getSnoozeTriggerFlag(returnFlag);

		delete messageDialogPtr;
	}

	// Update item runtime data flag
	setPwrReminderSnooze(displayItem, returnFlag);
	setPwrReminderDispFlag(displayItem, FLAG_OFF);

	return respond;
}


/**
 * @brief	Reupdate Power Reminder data
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::reupdatePwrReminderData(void)
{
	// Disable Power Reminder items
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		// Update Power Reminder data
		theAppPtr->setAppPwrReminderData(&reminderData_);
		theAppPtr->saveRegistryAppData(APPDATA_PWRREMINDER);
	}

	// Trigger reupdate Power Reminder data
	PostMessage(SM_APP_UPDATE_PWRREMINDERDATA, NULL, NULL);
}


/**
 * @brief	Setup Power Reminder item snooze mode
 * @param	reminderItem		- Power Reminder item
 * @param	snoozeFlag - Snooze flag
 * @return	None
 */
void CPowerPlusDlg::setPwrReminderSnooze(const PwrReminderItem& reminderItem, int snoozeFlag)
{
	// If item is empty, do nothing
	if (reminderItem.isEmpty()) return;

	// If item event ID is not "At set time", do nothing
	if (reminderItem.getEventId() != PwrReminderEvent::atSetTime)
		return;

	// If snoozing option is not available
	if (reminderItem.isAllowSnoozing() != true) {
		// Turn off flag
		snoozeFlag = FLAG_OFF;
	}

	// Snooze interval
	int interval = reminderItem.getSnoozeInterval();

	// Find if item snooze mode is already setup
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not Power Reminder item
		if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == reminderItem.getItemId()) {

			// Update item snooze mode data
			runtimeItem.setSnoozeFlag(snoozeFlag);

			if (runtimeItem.getSnoozeFlag() == FLAG_ON) {
				// Calculate next snooze trigger time
				runtimeItem.setTime(ClockTimeUtils::getCurrentClockTime());
				runtimeItem.calcNextSnoozeTime(interval);
			}
			return;
		}
	}

	// Prepare runtime item info to add
	PwrRuntimeItem runtimeItem;
	runtimeItem.setCategory(PwrFeatureID::pwrReminder);
	runtimeItem.setItemId(reminderItem.getItemId());
	runtimeItem.setSnoozeFlag(snoozeFlag);
	if (runtimeItem.getSnoozeFlag() == FLAG_ON) {
		// Calculate next snooze trigger time
		runtimeItem.setTime(ClockTimeUtils::getCurrentClockTime());
		runtimeItem.calcNextSnoozeTime(interval);
	}

	// Add item to runtime queue
	runtimeQueue_.push_back(runtimeItem);
}


/**
 * @brief	Update Power Reminder snooze queue data
 * @param	mode - Update mode flag (UPDATE or DISABLE)
 * @return	None
 */
void CPowerPlusDlg::updatePwrReminderSnooze(int mode)
{
	if (mode == Mode::Init) {

		// Initialize Power Reminder runtime snooze queue
		// Set all items as not snoozed
		for (int itemIndex = 0; itemIndex < reminderData_.getItemNum(); itemIndex++) {
			setPwrReminderSnooze(reminderData_.getItemAt(itemIndex), FLAG_OFF);
		}
	}
	else if (mode == Mode::Update) {

		// Update snooze queue items
		for (int queueIndex = (runtimeQueue_.size() - 1); queueIndex >= 0; queueIndex--) {
			
			// Get runtime item from queue
			PwrRuntimeItem& runtimeItem = runtimeQueue_.at(queueIndex);

			// Skip if it's not Power Reminder item
			if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

			// Search for item ID in Power Reminder data
			bool isItemFound = false;
			for (int itemIndex = 0; itemIndex < reminderData_.getItemNum(); itemIndex++) {
				const PwrReminderItem& reminderItem = reminderData_.getItemAt(itemIndex);
				if (reminderItem.getItemId() == runtimeItem.getItemId()) {

					// If item's snoozing mode is no longer available
					if (!reminderItem.isAllowSnoozing()) {
						// Disable snooze mode
						runtimeItem.setSnoozeFlag(FLAG_OFF);
					}

					// Mark as found
					isItemFound = true;
					break;
				}
			}

			// If item data is not found,
			// which means item maybe removed and no longer exists
			if (isItemFound == false) {
				// Remove item from snooze queue
				runtimeQueue_.erase(runtimeQueue_.begin() + queueIndex);
			}
		}
	}
	else if (mode == Mode::Disable) {

		// Disable snooze mode for all items in queue
		for (int index = 0; index < runtimeQueue_.size(); index++) {

			// Get runtime item from queue
			PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

			// Skip if it's not Power Reminder item
			if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

			// Update item snooze mode data
			runtimeItem.setSnoozeFlag(FLAG_OFF);
		}
	}
}


/**
 * @brief	Get Power Reminder item snooze trigger status
 * @param	itemId	   - Power Reminder item ID
 * @param	curSysTime - Current system time
 * @return	true/false
 */
bool CPowerPlusDlg::getPwrReminderSnoozeStatus(unsigned itemId, const ClockTime& currentTime)
{
	// If runtime queue data is empty, do not trigger
	if (runtimeQueue_.empty())
		return false;

	// Find for item ID
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		const PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not Power Reminder item
		if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == itemId) {

			// Get snooze enable flag
			if (runtimeItem.getSnoozeFlag() == FLAG_OFF)
				return false;
			
			// Check for next snooze time matching
			if (ClockTimeUtils::isMatching(currentTime, runtimeItem.getTime()))
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Get Power Reminder item runtime displaying flag
 * @param	reminderItem	- Power Reminder item
 * @return	true/false
 */
bool CPowerPlusDlg::getPwrReminderDispFlag(const PwrReminderItem& reminderItem)
{
	// If item is empty, it can not be displayed
	if (reminderItem.isEmpty()) return false;

	// Find if item runtime data is already setup
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not Power Reminder item
		if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == reminderItem.getItemId()) {

			// Get snooze enable flag
			if (runtimeItem.getDisplayFlag() == FLAG_ON)
				return true;

			return false;
		}
	}

	return false;
}


/**
 * @brief	Set Power Reminder item runtime displaying flag
 * @param	reminderItem	  - Power Reminder item
 * @param	displayFlag - Display flag
 * @return	None
 */
void CPowerPlusDlg::setPwrReminderDispFlag(const PwrReminderItem& reminderItem, int displayFlag)
{
	// If item is empty, do nothing
	if (reminderItem.isEmpty()) return;

	// Find if item runtime data is already setup
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not Power Reminder item
		if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

		// If item ID is found
		if (runtimeItem.getItemId() == reminderItem.getItemId()) {
			// Update item displaying flag
			runtimeItem.setDisplayFlag(displayFlag);
			return;
		}
	}

	// Prepare item info to add
	PwrRuntimeItem runtimeItem;
	runtimeItem.setCategory(PwrFeatureID::pwrReminder);
	runtimeItem.setItemId(reminderItem.getItemId());
	runtimeItem.setDisplayFlag(displayFlag);

	// Add item to runtime queue
	runtimeQueue_.push_back(runtimeItem);
}


/**
 * @brief	Get Power Reminder runtime displaying item list
 * @param	displayItemList - Power Reminder displaying item list
 * @return	size_t - Number of displaying items
 */
size_t CPowerPlusDlg::getPwrReminderDispList(uint32_array& displayItemList)
{
	// Reset output data list
	displayItemList.clear();

	// If runtime queue data is empty, return no item
	if (runtimeQueue_.empty())
		return Constant::kNullInteger;

	// Get runtime displaying item ID list
	displayItemList.reserve(runtimeQueue_.size());
	for (int index = 0; index < runtimeQueue_.size(); index++) {

		// Get runtime item from queue
		const PwrRuntimeItem& runtimeItem = runtimeQueue_.at(index);

		// Skip if it's not Power Reminder item
		if (runtimeItem.getCategory() != PwrFeatureID::pwrReminder) continue;

		// If item displaying flag is marked as ON
		if (runtimeItem.getDisplayFlag() == FLAG_ON) {
			// Add item ID into output data list
			displayItemList.push_back(runtimeItem.getItemId());
		}
	}

	// Return number of displaying items
	displayItemList.shrink_to_fit();
	return displayItemList.size();
}


//////////////////////////////////////////////////////////////////////////
// History and logging functions


/**
 * @brief	Output action schedule event log
 * @param	eventId	- Event ID
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CPowerPlusDlg::outputScheduleEventLog(uint16 eventId, const ScheduleItem& scheduleItem)
{
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Schedule action name
	int actionNameId = getPairedID(IDTable::ActionName, scheduleItem.getAction());
	const wchar_t* actionNameString = getLanguageString(languageTablePtr, actionNameId);

	// Detail info
	LOGDETAILINFO logDetailInfo;
	logDetailInfo.addDetail(EventDetail::ContentID, scheduleItem.getItemId());

	// Output event log
	outputEventLog(eventId, actionNameString, &logDetailInfo);
}


/**
 * @brief	Output Power Reminder event log
 * @param	eventId	- Event ID
 * @param	reminderItem - Power Reminder item
 * @return	None
 */
void CPowerPlusDlg::outputPwrReminderEventLog(uint16 eventId, const PwrReminderItem& reminderItem)
{
	// Message content
	const wchar_t* messageContent = reminderItem.getMessage();

	// Detail info
	LOGDETAILINFO logDetailInfo;
	logDetailInfo.addDetail(EventDetail::ContentID, reminderItem.getItemId());

	// Output event log
	outputEventLog(eventId, messageContent, &logDetailInfo);
}


/**
 * @brief	Initialize Power Action history info data
 * @param	actionId	- History action ID
 * @param	result		- Result of execution
 * @param	errorCode	- Error code
 * @return	None
 */
void CPowerPlusDlg::initPwrActionHistoryInfo(unsigned actionId, bool result, DWORD errorCode)
{
	// Initialize action history info data to save logs
	historyInfoData_.init(PowerAction);
	historyInfoData_.setActionId(actionId);

	// Collect action result
	historyInfoData_.setResult(result);
	historyInfoData_.setErrorCode(errorCode);
}


/**
 * @brief	Initialize schedule history info data
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CPowerPlusDlg::initScheduleHistoryInfo(const ScheduleItem& scheduleItem)
{
	// Check item validity
	if (scheduleItem.isEmpty())
		return;

	// Get schedule action name ID
	unsigned actionId = NULL;
	switch (scheduleItem.getAction())
	{
	case APP_ACTION_NOTHING:
		actionId = HistoryAction::DoNothing;
		break;
	case APP_ACTION_DISPLAYOFF:
		actionId = HistoryAction::DisplayOff;
		break;
	case APP_ACTION_SLEEP:
		actionId = HistoryAction::SleepMode;
		break;
	case APP_ACTION_SHUTDOWN:
		actionId = HistoryAction::Shutdown;
		break;
	case APP_ACTION_RESTART:
		actionId = HistoryAction::Restart;
		break;
	case APP_ACTION_SIGNOUT:
		actionId = HistoryAction::SignOut;
		break;
	case APP_ACTION_HIBERNATE:
		actionId = HistoryAction::Hibernate;
		break;
	}

	// Initialize schedule history info data
	historyInfoData_.init(ScheduleAction);
	historyInfoData_.setItemId(scheduleItem.getItemId());
	historyInfoData_.setActionId(actionId);
}


/**
 * @brief	Initialize hotkeyset history info data
 * @param	hotkeyId - Hotkey action ID
 * @return	None
 */
void CPowerPlusDlg::initHotkeyHistoryInfo(unsigned hotkeyId)
{
	// Get HotkeySet item by ID
	HotkeySetItem hotkeyItem;
	for (int index = 0; index < hotkeySetData_.getItemNum(); index++) {
		HotkeySetItem& tempHotkeyItem = hotkeySetData_.getItemAt(index);
		if (tempHotkeyItem.getActionId() == hotkeyId) {
			hotkeyItem.copy(tempHotkeyItem);
			break;
		}
	}

	// Check item validity
	if (hotkeyItem.isEmpty())
		return;

	// Get hotkey action name ID
	unsigned actionId = NULL;
	switch (hotkeyItem.getActionId())
	{
	case HotkeyID::displayOff:
		actionId = HistoryAction::DisplayOff;
		break;
	case HotkeyID::sleep:
		actionId = HistoryAction::SleepMode;
		break;
	case HotkeyID::shutdown:
		actionId = HistoryAction::Shutdown;
		break;
	case HotkeyID::restart:
		actionId = HistoryAction::Restart;
		break;
	case HotkeyID::signOut:
		actionId = HistoryAction::SignOut;
		break;
	case HotkeyID::hibernate:
		actionId = HistoryAction::Hibernate;
		break;
	default:
		break;
	}

	// Initialize hotkey action history info
	historyInfoData_.init(HotkeySet);
	historyInfoData_.setActionId(actionId);
	String keyStrokesString;
	hotkeyItem.printKeyStrokes(keyStrokesString);
	historyInfoData_.setDescription(keyStrokesString);
}


/**
 * @brief	Initialize reminder displaying history info data
 * @param	reminderItem - Power Reminder item
 * @return	None
 */
void CPowerPlusDlg::initPwrReminderHistoryInfo(const PwrReminderItem& reminderItem)
{
	// Check item validity
	if (reminderItem.isEmpty()) return;

	// Initialize history info data
	historyInfoData_.init(PowerReminder);
	historyInfoData_.setItemId(reminderItem.getItemId());
	historyInfoData_.setDescription(reminderItem.getMessage());
}


/**
 * @brief	Save history info data log file
 * @param	None
 * @return	None
 */
void CPowerPlusDlg::saveHistoryInfoData(void)
{
	// If history info data is empty (not yet initialized), do nothing
	if (!historyInfoData_.isInit())
		return;

	// Prepare common history log info
	LOGITEM actionLogItem;
	actionLogItem.setTime(historyInfoData_.getTime());

	// Get current process ID
	actionLogItem.setProcessId();

	// Attach history detail info by category ID
	switch (historyInfoData_.getCategoryId())
	{
	case HistoryCategory::PowerAction:
		actionLogItem.setCategory(LOG_HISTORY_EXEC_PWRACTION);
		actionLogItem.addDetail(HistoryDetail::Action, historyInfoData_.getActionId(), LogDetailFlag::LookUp_Dict);
		break;

	case HistoryCategory::ScheduleAction:
		actionLogItem.setCategory(LOG_HISTORY_EXEC_SCHEDULE);
		actionLogItem.addDetail(HistoryDetail::ItemID, historyInfoData_.getItemId());
		actionLogItem.addDetail(HistoryDetail::Action, historyInfoData_.getActionId(), LogDetailFlag::LookUp_Dict);
		break;

	case HistoryCategory::HotkeySet:
		actionLogItem.setCategory(LOG_HISTORY_EXEC_HOTKEY);
		actionLogItem.addDetail(HistoryDetail::Action, historyInfoData_.getActionId(), LogDetailFlag::LookUp_Dict);
		actionLogItem.addDetail(HistoryDetail::Keystrokes, historyInfoData_.getDescription());
		break;

	case HistoryCategory::PowerReminder:
		actionLogItem.setCategory(LOG_HISTORY_DISP_PWRREMINDER);
		actionLogItem.addDetail(HistoryDetail::ItemID, historyInfoData_.getItemId());
		actionLogItem.addDetail(HistoryDetail::Message, historyInfoData_.getDescription());
		break;

	default:
		break;
	}

	// Attach history action result detail info
	if (historyInfoData_.isSuccess() || historyInfoData_.getErrorCode() == APP_ERROR_SUCCESS) {
		actionLogItem.addDetail(HistoryDetail::Result, HistoryResult::SuccessNoError, LogDetailFlag::LookUp_Dict);
	}
	else {
		if (historyInfoData_.getErrorCode() == APP_ERROR_UNKNOWN) {

			// If error code is NULL or unknown, set as failed with unknown reason
			actionLogItem.addDetail(HistoryDetail::Result, HistoryResult::FailedUnknown, LogDetailFlag::LookUp_Dict);
		}
		else {
			// If error code is available, set as failed with error code
			actionLogItem.addDetail(HistoryDetail::Result, HistoryResult::FailedWithErrorCode, LogDetailFlag::LookUp_Dict);

			// Attach error code detail info
			actionLogItem.addDetail(HistoryDetail::ActionError, historyInfoData_.getErrorCode());
		}
	}

	// Output action history log if enabled
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr != NULL) {
		theAppPtr->outputAppHistoryLog(actionLogItem);
	}

	// Empty history data after done output
	historyInfoData_.removeAll();
}


//////////////////////////////////////////////////////////////////////////
// Notification and error message functions


/**
 * @brief	Show confirmation message before executing action
 * @param	actionType - Type of action
 * @param	actionId	- ID of action
 * @return	int	- Result of confirmation message
 */
int CPowerPlusDlg::confirmActionExec(unsigned actionType, unsigned actionId)
{
	// If "Confirm action" option is not enabled, return YES
	if (getAppOption(AppOptionID::actionConfirmation) == false) {
		TRACE("Confirm action option is not enabled, always allow execution!!!");
		return IDYES;
	}

	// Allowed action IDs --> Bypass confirmation
	if (actionId == APP_ACTION_DISPLAYOFF) {
		TRACE("Always allow executing monitor power action!!!");
		return IDYES;
	}

	// Allowed action types --> Bypass confirmation
	if ((actionType == APP_MACRO_ACTION_SCHEDULE) ||
		(actionType == APP_MACRO_ACTION_HOTKEY) ||
		(actionType == APP_MACRO_ACTION_MENU)) {
		TRACE("Bypassing confirmation for these power actions!!!");
		return IDYES;
	}

	// Display confirmation message
	unsigned messageStringId = getPairedID(IDTable::ActionMessage, actionId);
	int result = displayMessageBox(messageStringId, (unsigned)NULL, MB_YESNO | MB_ICONQUESTION);

	return result;
}


/**
 * @brief	Notify schedule if enabled
 * @param	scheduleItemPtr  - Schedule item to notify (pointer)
 * @param	update - Trigger reupdate flag (out)
 * @return	int - Result of notify message
 */
int CPowerPlusDlg::notifySchedule(PScheduleItem scheduleItemPtr, bool& update)
{
	// Do not notify if schedule action is "Do nothing"
	if (scheduleItemPtr->getAction() == APP_ACTION_NOTHING) {
		TRACE("Do not notify when schedule action is DO NOTHING");
		return Constant::kInvalidInteger;
	}

	// Get action info
	unsigned actionStringId = getPairedID(IDTable::ScheduleNotifyMessage, scheduleItemPtr->getAction());

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Format message
	const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CAPTION);
	const wchar_t* messageScheduleAction = getLanguageString(languageTablePtr, actionStringId);
	const wchar_t* messageTemplate = getLanguageString(languageTablePtr, MSGBOX_PROCESSSCHEDULE_NOTIFY);
	
	String messageContent = StringUtils::stringFormat(messageTemplate, messageScheduleAction);

	// Allow cancelling schedule when notify
	bool isCancelAllowed = getAppOption(AppOptionID::allowScheduleCancellation);
	if (isCancelAllowed == true)
	{
		// Update message content
		messageContent += getLanguageString(languageTablePtr, MSGBOX_PROCESSSCHEDULE_ALLOWCANCEL);
		int respond = displayMessageBox(messageContent, messageCaption, MB_OKCANCEL | MB_ICONINFORMATION);
		if (respond == IDCANCEL)
		{
			// Set item skip flag
			setActionScheduleSkip(*scheduleItemPtr, FLAG_ON);

			// If "Repeat" option is not ON,
			// --> Disable schedule item after canceling
			if (scheduleItemPtr->isRepeatEnabled() == false) {
				// Deactivate schedule
				scheduleItemPtr->enableItem(false);
				update = true;
			}

			// Output event log: Schedule canceled
			outputScheduleEventLog(LOG_EVENT_CANCEL_SCHEDULE, *scheduleItemPtr);

			// Display notify message
			displayMessageBox(MSGBOX_PROCESSSCHEDULE_CANCELED, MSGBOX_MULTISCHEDULE_CAPTION, MB_OK | MB_ICONINFORMATION);
		}

		return respond;
	}

	// Show message
	return displayMessageBox(messageContent, messageCaption, MB_OK | MB_ICONINFORMATION);
}


/**
 * @brief	Show error message if enabled
 * @param	errorCode - Error code
 * @return	None
 */
void CPowerPlusDlg::showErrorMessage(DWORD errorCode)
{
	// If option is not enabled, do nothing
	if (getAppOption(AppOptionID::showErrorMessage) == false) {
		TRACE("Show error message option is not enabled!!!");
		return;
	}

	// Get window handle and language ID
	HWND windowHandle = this->GetSafeHwnd();
	unsigned currentLangOption = ((CPowerPlusApp*)AfxGetApp())->getAppLanguageOption();

	// Show error message
	AppCore::showErrorMessage(windowHandle, currentLangOption, errorCode);
}


/**
 * @brief	Request to restart the application
 * @param	commandSenderId	- ID of command which sends request
 * @param	restartAsAdmin - Restart as admin???
 * @return	None
 */
void CPowerPlusDlg::requestRestartApp(unsigned commandSenderId, bool restartAsAdmin)
{
	// Init request data
	RESTARTREQ restartRequest;
	ZeroMemory(&restartRequest, sizeof(RESTARTREQ));

	// Request from [Apply] button
	if (commandSenderId == IDC_APPLY_BTN) {
		bool isRestartTriggered = getFlagValue(AppFlagID::restartAsAdmin);
		restartRequest.request = isRestartTriggered;
		restartRequest.adminCheck = true;
		restartRequest.showMsgIfNotAdmin = true;
		restartRequest.doNothingIfAdmin = true;
		restartRequest.showMsgWhenDeny = true;
		restartRequest.resetFlag = true;
	}
	// Request from [Restart with admin priviledges] menu
	else if (commandSenderId == IDM_NOTIFY_RESTART_ASADMIN) {
		restartRequest.request = true;
		restartRequest.adminCheck = true;
		restartRequest.showMsgIfNotAdmin = false;
		restartRequest.doNothingIfAdmin = true;
		restartRequest.showMsgWhenDeny = false;
		restartRequest.resetFlag = false;
	}
	// Request from [Restart app] menu or debug command
	else if ((commandSenderId == IDM_NOTIFY_RESTART_APP) ||
			 (commandSenderId == IDD_DEBUGTEST_DLG)) {
		// Restart directly without request
		restartApp(restartAsAdmin);
		return;
	}
	else {
		// Invalid command --> Do not request
		return;
	}

	// Send restart request
	if (restartAsAdmin) {
		// Request restart as admin
		requestRestartAsAdmin(restartRequest);
	}
	else {
		// Restart directly
		restartApp(false);
	}
}


/**
 * @brief	Request to restart the application as admin
 * @param	restartRequest - Request to restart
 * @return	None
 */
void CPowerPlusDlg::requestRestartAsAdmin(RESTARTREQ restartRequest)
{
	// If there's no request, do nothing
	if (restartRequest.request == false) {
		TRACE("There is no restart request!!!");
		return;
	}

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	const wchar_t* requestMessage = getLanguageString(languageTablePtr, MSGBOX_OTHER_REQUEST_RESTARTASADMIN);
	const wchar_t* messageCaption = ((CPowerPlusApp*)AfxGetApp())->getAppWindowCaption();
	String messageFormatString = requestMessage;

	// Check if the application is currently running as admin
	bool isAdmin = false;
	if (restartRequest.adminCheck == true) {
		isAdmin = IsUserAnAdmin();
		if (isAdmin == true) {
			// If already running as admin, do nothing
			if (restartRequest.doNothingIfAdmin == true) {
				if (restartRequest.showMsgWhenDeny == false) {
					displayMessageBox(MSGBOX_OTHER_ALREADY_ADMIN, (unsigned)NULL, MB_OK | MB_ICONINFORMATION);
				}
				return;
			}
		}
		else if (isAdmin == false) {
			// Show "not admin" message
			if (restartRequest.showMsgIfNotAdmin == true) {
				const wchar_t* notAdminMsg = getLanguageString(languageTablePtr, MSGBOX_OTHER_NOTRUNASADMIN);
				messageFormatString.format(_T("%s\n%s"), notAdminMsg, requestMessage);
			}
		}
	}
	
	// Display request message
	int returnValue = displayMessageBox(messageFormatString, messageCaption, MB_YESNO | MB_ICONQUESTION);
	if (returnValue == IDYES) {
		// Restart as admin
		restartApp(true);
	}
	else if (returnValue == IDNO) {
		// Run as admin next time
		if (restartRequest.showMsgWhenDeny == true) {
			// Show message when denied
			displayMessageBox(MSGBOX_OTHER_RUNASADMIN_NEXTTIME, (unsigned)NULL, MB_OK | MB_ICONINFORMATION);
		}

		if (restartRequest.resetFlag == true) {
			// Reset flag
			setFlagValue(AppFlagID::restartAsAdmin, false);
		}
	}
}

