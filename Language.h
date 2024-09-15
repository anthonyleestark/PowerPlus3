
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Language.h
//		Description:	Define all language strings to be used in program
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.01.27:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LANGUAGE_H_INCLUDED
#define _LANGUAGE_H_INCLUDED

#include "stdafx.h"

#ifdef _RESOURCE2_NOTINCLUDED_
#pragma message("--Language: Resource2.h not included")
#endif

//////////////////////////////////////////////////
// Implementation

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LANGTEXT
//  Description:	Store language text item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagLANGTEXT
{
	DWORD	dwLangStringID;
	LPCTSTR lpszLangString;
} LANGTEXT, *PLANGTEXT;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LANGTABLE/LANGTABLE_PTR
//  Description:	Language package data
//  Derivered from: C++ vector type
//
//////////////////////////////////////////////////////////////////////////

typedef const std::vector<LANGTEXT> LANGTABLE, *LANGTABLE_PTR;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define global language packages for program, these packages will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
//	Language: English (United States)
//
//////////////////////////////////////////////////////////////////////////

static LANGTABLE langtable_en_US
{
/*--------ActionName ID-----------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ ACTION_NAME_DISPLAYOFF,								_T("Turn off display") },
	{ ACTION_NAME_SLEEP,									_T("Sleep") },
	{ ACTION_NAME_SHUTDOWN,									_T("Shut down") },
	{ ACTION_NAME_RESTART,									_T("Restart") },
	{ ACTION_NAME_SIGNOUT,									_T("Sign out") },
	{ ACTION_NAME_HIBERNATE,								_T("Hibernate") },
	{ ACTION_NAME_SHOWMENU,									_T("Show notify menu") },

/*--------Action Message----------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ MESSAGE_ACTION_DISPLAYOFF,							_T("Are you sure you want to turn off display?") },
	{ MESSAGE_ACTION_SLEEP,									_T("Are you sure you want to put your computer into sleep mode?") },
	{ MESSAGE_ACTION_SHUTDOWN,								_T("Are you really sure you want to shut down your computer?") },
	{ MESSAGE_ACTION_RESTART,								_T("Are you sure you really want to restart your computer?") },
	{ MESSAGE_ACTION_SIGNOUT,								_T("Are you sure you want to sign out your current account?") },
	{ MESSAGE_ACTION_HIBERNATE,								_T("Are you sure you want to put your computer into hibernation mode?") },

/*---------Control ID--------------------------------------------------------------------Language String--------------------------------------------------------------*/
	{ IDC_MAIN_TITLE,										_T("Main Settings") },
	{ IDC_LEFTMOUSE_TITLE,									_T("Left Mouse Button") },
	{ IDC_MIDMOUSE_TITLE,									_T("Middle Mouse Button") },
	{ IDC_RIGHTMOUSE_TITLE,									_T("Right Mouse Button") },
	{ IDC_ENABLERMBMENU_CHK,								_T("Right mouse button: Just show menu") },
	{ IDC_APPLY_BTN,		 								_T("Apply") },
	{ IDC_RELOAD_BTN,				 						_T("Reload") },
	{ IDC_EXIT_BTN,											_T("Exit") },
	{ IDC_EXPAND_BTN, 										_T("Expand >>") },
	{ IDC_COLLAPSE_BTN,										_T("<< Collapse") },
	{ IDC_HELP_BTN,											_T("Help") },
	{ IDC_ABOUT_BTN,				 						_T("About") },
	{ IDC_DEFAULT_BTN,										_T("Default") },
	{ IDC_SYSTEM_TITLE, 									_T("Advanced Options") },
	{ IDC_SHOWATSTARTUP_CHK,								_T("Show this dialog box at startup") },
	{ IDC_ENABLEAUTOSTART_CHK,						 		_T("Startup with Windows") },
	{ IDC_CONFIRMACTION_CHK,								_T("Confirm before executing action") },
	{ IDC_SAVEACTIONLOG_CHK,								_T("Save the log of actions") },
	{ IDC_SAVEAPPEVENTLOG_CHK,								_T("Save the log of app events") },
	{ IDC_RUNASADMIN_CHK,									_T("Run with admin privileges") },
	{ IDC_SHOWERROR_CHK,									_T("Show error message on screen") },
	{ IDC_SCHEDNOTIFY_CHK, 									_T("Notify before doing scheduled action") },
	{ IDC_SCHEDALLOWCANCEL_CHK,				 				_T("Allow canceling schedule when notify") },
	{ IDC_ENBBKGRDHOTKEYS_CHK,								_T("Enable background action hotkeys") },
	{ IDC_ENBPWRREMINDER_CHK,								_T("Enable Power Reminder feature") },
	{ IDC_LANGUAGE_TITLE,									_T("Language") },
	{ IDC_VIEWACTIONLOG_BTN,								_T("View the log of actions") },
	{ IDC_BACKUPCFG_BTN,									_T("Backup settings") },
	{ IDC_SCHEDULE_BTN,										_T("Action Schedule") },
	{ IDC_LOGVIEWER_BTN,									_T("Logviewer") },
	{ IDC_HOTKEYSET_BTN,									_T("Hotkeys Set") },
	{ IDC_PWRREMINDER_BTN,									_T("Power Reminder") },

/*--------Menu Item ID--------------------------------------------------------------------Language String-------------------------------------------------------------*/
	{ IDM_OPENDLG_ABOUT,									_T("About") },
	{ IDM_OPENDLG_HELP,										_T("Help") },
	{ IDM_VIEW_ACTIONLOG,									_T("View action log") },
	{ IDM_BACKUP_CONFIG,									_T("Backup configurations") },
	{ IDM_VIEW_BAKCONFIG,									_T("View backup configurations") },
	{ IDM_OPENDLG_LOGVIEWER,								_T("LogViewer") },
	{ IDM_OPENDLG_SCHEDULE,									_T("Schedule") },
	{ IDM_OPENDLG_HOTKEYSET,								_T("Hotkey Set") },
	{ IDM_OPENDLG_PWRREMINDER,								_T("Power Reminder") },
	{ IDM_ACTIONS_TITLE,									_T("Actions") },
	{ IDM_RESTART_APP,										_T("Restart application") },
	{ IDM_RESTART_ASADMIN,									_T("Restart with admin privileges") },
	{ IDM_SHOW_WINDOW,										_T("Main Window") },
	{ IDM_EXIT_APP,											_T("Exit") },

	{ IDM_ACTION_DISPLAYOFF,								_T("Turn off display") },
	{ IDM_ACTION_SLEEP,										_T("Sleep") },
	{ IDM_ACTION_SHUTDOWN,									_T("Shut down") },
	{ IDM_ACTION_RESTART,									_T("Restart") },
	{ IDM_ACTION_SIGNOUT,									_T("Sign out") },
	{ IDM_ACTION_HIBERNATE,									_T("Hibernate") },
	{ IDM_ACTION_SCHEDULE,									_T("Action Schedule") },

/*-------Combo-box Option ID---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ COMBOBOX_ACTION_DISPLAYOFF,							_T("Turn off display") },
	{ COMBOBOX_ACTION_SLEEP,								_T("Sleep") },
	{ COMBOBOX_ACTION_SHUTDOWN,								_T("Shut down") },
	{ COMBOBOX_ACTION_RESTART,								_T("Restart") },
	{ COMBOBOX_ACTION_SIGNOUT,								_T("Sign out") },
	{ COMBOBOX_ACTION_HIBERNATE,							_T("Hibernate") },
	{ COMBOBOX_ACTION_SHOWMENU,								_T("Just show the menu") },
	{ COMBOBOX_LANGUAGE_ENGLISH,							_T("English") },
	{ COMBOBOX_LANGUAGE_VIETNAMESE,							_T("Vietnamese") },
	{ COMBOBOX_LANGUAGE_SIMPCHINESE,						_T("Chinese") },

/*------Notify String ID-------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ NOTIFY_TIP_TEMPLATE,						 			_T("Click left mouse here to %s.\n")
															_T("Click middle mouse here to %s.\n")
															_T("Click right mouse to %s.") },
	{ NOTIFY_TIP_DISPLAYOFF,								_T("turn off display") },
	{ NOTIFY_TIP_SLEEP,  									_T("sleep") },
	{ NOTIFY_TIP_SHUTDOWN,						 			_T("shut down") },
	{ NOTIFY_TIP_RESTART,			 						_T("restart") },
	{ NOTIFY_TIP_SIGNOUT,  									_T("sign out") },
	{ NOTIFY_TIP_HIBERNATE,  								_T("hibernate") },
	{ NOTIFY_TIP_SHOWMENU, 									_T("show menu") },

/*------Balloon String ID-------------------------------------------------------------------Language String-----------------------------------------------------------*/
	{ BALLOON_TIP_TEMPLATE,							 		_T("Scheduled action is imminent.\n%s in %d second(s).") },
	{ BALLOON_TIP_DISPLAYOFF, 								_T("The screen will turn off") },
	{ BALLOON_TIP_SLEEP,									_T("The computer will sleep") },
	{ BALLOON_TIP_SHUTDOWN,					  				_T("The computer will shut down") },
	{ BALLOON_TIP_RESTART, 									_T("The computer will restart") },
	{ BALLOON_TIP_SIGNOUT,  								_T("The current account will sign out") },
	{ BALLOON_TIP_HIBERNATE,  								_T("The computer will hibernate") },

/*-----About Dialog Item--------------------------------------------------------------------Language String-----------------------------------------------------------*/
	{ IDD_ABOUT_DLG,										_T("About Power++") },
	{ IDC_APPNAME_LABEL,								  	_T("Power++ 3.1 (v3.1.1.0)") },
	{ IDC_COPYRIGHT_LABEL, 							 		_T("Copyright (C) 2017 - 2024 AnthonyLeeStark.") },
	{ IDC_AUTH_LABEL,  										_T("Developed by: Anthony Lee Stark (@AnthonyLeeStark)") },
	{ IDC_DEVPROFILE_LINK,	 								_T("<a>View Developer's Facebook Profile</a>") },
	{ IDC_APPINFO_LABEL,						 			_T("Power++ - Quick power action utility\n")
															_T("Doing power action and setting up power schedule simplier.\n\n")
															_T("Developer's Profile:\n")
															_T("Name: AnthonyLeeStark\n")
															_T("Email: anthonyleestark.official@gmail.com\n")
															_T("Facebook: https://www.facebook.com/anthonyleestark \n")
															_T("Twitter: https://twitter.com/AnthonyLeeStark \n\n")
															_T("Power++ v3.1 is an utility which allow you to do some power actions quickly and to set up schedule to excute those actions. ")
															_T("This tool helps you turn off screen, turn PC to sleep, shutdown, reboot, hibernate, or sign off from current account just by one-click. ")
															_T("It also helps you setup alarms to do those actions automatically or setup hotkeys for each of them. ") },
	{ IDC_ABOUT_CLOSE_BTN,	 								_T("Close") },

/*-----Help Dialog Item--------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_HELP_DLG,											_T("Help") },
	{ IDC_HELP_CLOSE_BTN,									_T("Close") },
	{ BTN_HELPDLG_VIEWMODE_HELPFILE,						_T("View Help") },
	{ BTN_HELPDLG_VIEWMODE_CHANGELOG,						_T("View Changelog") },
	{ ERROR_HELPDLG_NOHELPFILE,								_T("Can not open help file!\r\nThe file maybe corrupted or doesn't exist.") },
	{ ERROR_HELPDLG_NOCHANGELOGFILE,						_T("Can not open changelog file!\r\nThe file maybe corrupted or doesn't exist.") },

/*------Schedule Dialog Item---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_SCHEDULE_DLG,  									_T("Schedule") },
	{ IDC_SCHEDULE_ENABLE_CHK,								_T("Active schedule action") },
	{ IDC_SCHEDULE_ACTION_LABEL,							_T("Action") },
	{ IDC_SCHEDULE_TIME_LABEL,  							_T("Time") },
	{ IDC_SCHEDULE_REPEATDAILY_CHK,							_T("Repeat schedule daily") },
	{ IDC_SCHEDULE_APPLY_BTN,		 						_T("Save") },
	{ IDC_SCHEDULE_CANCEL_BTN, 								_T("Close") },

/*-----LogViewer Dialog Item---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_LOGVIEWER_DLG,									_T("LogViewer") },
	{ IDC_LOGVIEWER_DETAILS_LABEL,							_T("Detail") },
	{ IDC_LOGVIEWER_DATETIME_LABEL,							_T("Date/Time") },
	{ IDC_LOGVIEWER_CONTENT_LABEL,							_T("Content") },

/*------HotkeySet Dialog Item--------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_HOTKEYSET_DLG,									_T("Hotkey Set") },
	{ IDC_HOTKEYSET_ADD_BTN,								_T("Add") },
	{ IDC_HOTKEYSET_REMOVE_BTN,								_T("Remove") },
	{ IDC_HOTKEYSET_REMOVEALL_BTN,							_T("Remove All") },
	{ IDC_HOTKEYSET_EXPORT_BTN,								_T("Export") },
	{ BTN_HKSETDLG_CHECKALL,								_T("Check All") },
	{ BTN_HKSETDLG_UNCHECKALL,								_T("Uncheck All") },
	{ IDC_HOTKEYSET_ACTION_TITLE,							_T("Action") },
	{ IDC_HOTKEYSET_CONTROLKEY_STATIC,						_T("Control Key") },
	{ IDC_HOTKEYSET_CTRLKEY_CHK,							_T("CTRL") },
	{ IDC_HOTKEYSET_ALTKEY_CHK,								_T("ALT") },
	{ IDC_HOTKEYSET_WINKEY_CHK,								_T("WIN") },
	{ IDC_HOTKEYSET_FUNCKEY_TITLE,							_T("Function Key") },
	{ IDC_HOTKEYSET_APPLY_BTN,								_T("Apply") },
	{ IDC_HOTKEYSET_CANCEL_BTN,								_T("Cancel") },
	{ LVCOLUMN_EMPTY_TITLE,									_T("") },
	{ LVCOLUMN_HOTKEYSET_ACTION,							_T("Action") },
	{ LVCOLUMN_HOTKEYSET_KEYSTROKES,						_T("Keystrokes") },

/*------Power Reminder Dialog Item---------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_PWRREMINDER_DLG,									_T("Power Reminder") },
	{ IDC_PWRREMINDER_ADD_BTN,								_T("Add") },
	{ IDC_PWRREMINDER_EDIT_BTN,								_T("Edit") },
	{ IDC_PWRREMINDER_REMOVE_BTN,							_T("Remove") },
	{ IDC_PWRREMINDER_REMOVEALL_BTN,						_T("Remove All") },
	{ IDC_PWRREMINDER_CHECKALL_BTN,							_T("Check All") },
	{ IDC_PWRREMINDER_UNCHECKALL_BTN,						_T("Uncheck All") },
	{ IDC_PWRREMINDER_PREVIEW_BTN,							_T("Preview") },
	{ IDC_PWRREMINDER_APPLY_BTN,							_T("Apply") },
	{ IDC_PWRREMINDER_CANCEL_BTN,							_T("Cancel") },
	{ IDC_PWRREMINDER_MSGSTRING_TITLE,						_T("Message Content") },
	{ IDC_PWRREMINDER_EVENT_TITLE,							_T("Event") },
	{ IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,					_T("At the specific set time") },
	{ IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,				_T("At the moment of this application startup") },
	{ IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,				_T("At the moment of the system wakeup (required \"Startup with Windows\")") },
	{ IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,			_T("Before this application executes any power action") },
	{ IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,			_T("After system waking up from any power action") },
	{ IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,				_T("At the moment right before this application exits") },
	{ IDC_PWRREMINDER_MSGSTYLE_TITLE,						_T("Message Style") },
	{ IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,				_T("Message Box style") },
	{ IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,				_T("Dialog Box style") },
	{ GRIDCOLUMN_PWRREMINDER_INDEX,							_T("No.") },
	{ GRIDCOLUMN_PWRREMINDER_STATE,							_T("Enable") },
	{ GRIDCOLUMN_PWRREMINDER_ITEMID,						_T("Item ID") },
	{ GRIDCOLUMN_PWRREMINDER_MESSAGE,						_T("Message Content") },
	{ GRIDCOLUMN_PWRREMINDER_EVENTID,						_T("Event") },
	{ GRIDCOLUMN_PWRREMINDER_STYLE,							_T("Style") },
	{ GRIDCOLUMN_PWRREMINDER_REPEAT,						_T("Repeat") },
	{ PWRRMD_EVENT_AT_SETTIME,								_T("At %02d:%02d %2s") },
	{ PWRRMD_EVENT_AT_APPSTARTUP,							_T("At app startup") },
	{ PWRRMD_EVENT_AT_SYSWAKEUP,							_T("At system wake") },
	{ PWRRMD_EVENT_AT_BFRPWRACTION,							_T("Before power action") },
	{ PWRRMD_EVENT_AT_PWRACTIONWAKE,						_T("Wake after action") },
	{ PWRRMD_EVENT_AT_APPEXIT,								_T("Before app exit") },
	{ PWRRMD_STYLE_MESSAGEBOX,								_T("Message Box") },
	{ PWRRMD_STYLE_DIALOG,									_T("Dialog Box") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MESSAGE_SCHEDNOTIFY_DISPLAYOFF,						_T("computer will turn off display") },
	{ MESSAGE_SCHEDNOTIFY_SLEEP,							_T("computer will be put into sleep mode") },
	{ MESSAGE_SCHEDNOTIFY_SHUTDOWN,							_T("computer will shut down") },
	{ MESSAGE_SCHEDNOTIFY_RESTART,							_T("computer will restart") },
	{ MESSAGE_SCHEDNOTIFY_SIGNOUT,							_T("account will be signed out") },
	{ MESSAGE_SCHEDNOTIFY_HIBERNATE,						_T("computer will be put into hibernation mode") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MSGBOX_CONFIG_RELOAD_CAPTION,							_T("Reload Settings") },
	{ MSGBOX_CONFIG_CHANGED_CONTENT,						_T("Some options had been changed.\nDo you want to abort all changes and reload the settings?") },
	{ MSGBOX_CONFIG_NOTCHANGED_CONTENT,						_T("All settings are up to date. There is nothing to reload.") },
	{ MSGBOX_SCHEDULE_CHANGED_CAPTION,						_T("Save options") },
	{ MSGBOX_SCHEDULE_CHANGED_CONTENT,						_T("Some options had been changed.\nDo you want to save them?") },
	{ MSGBOX_SCHEDULE_NOTIFY,								_T("Your %s in 30 seconds") },
	{ MSGBOX_SCHEDULE_ALLOWCANCEL,							_T("\nYou can press [Cancel] to cancel the schedule action") },
	{ MSGBOX_SCHEDULE_CANCELED,								_T("Schedule has been canceled successfully.") },
	{ MSGBOX_HOTKEYSET_CAPTION,								_T("Hotkey Set") },
	{ MSGBOX_HOTKEYSET_REG_FAILED,							_T("Failed to register background hotkey.") },
	{ MSGBOX_HOTKEYSET_UNREG_FAILED,						_T("Failed to unregister background hotkey.") },
	{ MSGBOX_HOTKEYSET_CHANGED_CONTENT,						_T("Some data had been changed.\nDo you want to save them?") },
	{ MSGBOX_HOTKEYSET_REMOVE_ITEM,							_T("Do you want to remove this HotkeySet item?") },
	{ MSGBOX_HOTKEYSET_REMOVEALL_ITEMS,						_T("Do you want to remove all HotkeySet items?") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID,				_T("HotkeySet is invalid.\nAction cannot be empty or invalid.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY,					_T("HotkeySet is invalid.\nSelect at least one control key.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY,					_T("HotkeySet is invalid.\nFunction key option cannot be empty or invalid.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_EMPTY,					_T("HotkeySet is invalid.\nData cannot empty or invalid.") },
	{ MSGBOX_HOTKEYSET_EXISTED_HOTKEY,						_T("System key shortcut already existed.\n%s\nPlease select other hotkey options.") },
	{ MSGBOX_PWRREMINDER_CAPTION,							_T("Power Reminder") },
	{ MSGBOX_PWRREMINDER_DISPLAY_FAILED,					_T("Something is wrong when displaying reminder content.") },
	{ MSGBOX_PWRREMINDER_CHANGED_CONTENT,					_T("Some data had been changed.\nDo you want to save them?") },
	{ MSGBOX_PWRREMINDER_REMOVE_ITEM,						_T("Do you want to remove this reminder item?") },
	{ MSGBOX_PWRREMINDER_REMOVEALL_ITEMS,					_T("Do you want to remove all reminder items?") },
	{ MSGBOX_PWRREMINDER_CONFIRM_EXITMODE,					_T("Are you sure you want to abort all current changes?") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID,				_T("Reminder data is invalid.\nItem ID is empty or invalid.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY,			_T("Reminder data is invalid.\nMessage content can not be empty.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT,	_T("Reminder data is invalid.\nMessage content can not contain more than 50 characters.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_INVALIDCHAR,	_T("Reminder data is invalid.\nMessage content contains one or more invalid characters") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID,				_T("Reminder data is invalid.\nEvent setting is empty or invalid.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_TIMEVALUE,				_T("Reminder data is invalid.\nTime setting is empty or invalid.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID,				_T("Reminder data is invalid.\nMessage style setting is empty or invalid.") },
	{ MSGBOX_OTHER_NOTRUNASADMIN,							_T("The application is not currently running with admin privileges.") },
	{ MSGBOX_OTHER_REQUEST_RESTARTASADMIN,					_T("Do you want to restart the application with admin privileges?") },
	{ MSGBOX_OTHER_RUNASADMIN_NEXTTIME,						_T("The application will run with admin privileges in the next startup.") },
	{ MSGBOX_OTHER_ALREADY_ADMIN,							_T("The application is currently running with admin privileges.") },
	{ MSGBOX_OTHER_FUNCTION_NOT_AVAILABLE,					_T("This function is currently not available.") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MSGBOX_ERROR_CAPTION,									_T("Error") },
	{ MSGBOX_ERROR_FAILED,									_T("Execute action failed!") },
	{ MSGBOX_ERROR_WRONG_ARGUMENT, 							_T("Wrong argument of function!") },
	{ MSGBOX_ERROR_INVALID_FUNCTION, 						_T("Incorrect function.") },
	{ MSGBOX_ERROR_FILE_NOT_FOUND, 							_T("The system cannot find the file specified.") },
	{ MSGBOX_ERROR_PATH_NOT_FOUND, 							_T("The system cannot find the path specified.") },
	{ MSGBOX_ERROR_ACCESS_DENIED, 							_T("Access is denied.") },
	{ MSGBOX_ERROR_INVALID_HANDLE, 							_T("The handle is invalid.") },
	{ MSGBOX_ERROR_INVALID_DATA, 							_T("The data is invalid.") },
	{ MSGBOX_ERROR_NO_MORE_FILES, 							_T("There are no more files.") },
	{ MSGBOX_ERROR_FILE_EXISTS, 							_T("The file exists.") },
	{ MSGBOX_ERROR_CANNOT_MAKE, 							_T("The directory or file cannot be created.") },
	{ MSGBOX_ERROR_INVALID_PARAMETER, 						_T("The parameter is incorrect.") },
	{ MSGBOX_ERROR_OPEN_FAILED, 							_T("The system cannot open the device or file specified.") },
	{ MSGBOX_ERROR_BUFFER_OVERFLOW, 						_T("The file name is too long.") },
	{ MSGBOX_ERROR_INVALID_NAME, 							_T("The filename, directory name, or volume label syntax is incorrect.") },
	{ MSGBOX_ERROR_DIR_NOT_EMPTY, 							_T("The directory is not empty.") },
	{ MSGBOX_ERROR_FAIL_SHUTDOWN, 							_T("The shutdown operation failed.") },
	{ MSGBOX_ERROR_FAIL_RESTART, 							_T("The restart operation failed.") },
	{ MSGBOX_ERROR_INVALID_ADDRESS, 						_T("Attempt to access invalid address.") },
	{ MSGBOX_ERROR_APP_INIT_FAILURE, 						_T("The application was unable to start correctly. Click OK to close the application.") },
	{ MSGBOX_ERROR_CANNOT_LOAD_REGISTRY,					_T("The registry cannot load. It is corrupt, absent, or not writable.") },
	{ MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT, 					_T("The system has reached the maximum size allowed for the system part of the registry. Additional storage requests will be ignored.") },
	{ MSGBOX_ERROR_SYSTEM_SHUTDOWN,							_T("The system is in the process of shutting down.") },
	{ MSGBOX_ERROR_HIBERNATED, 								_T("The system was put into hibernation.") },
	{ MSGBOX_ERROR_RESUME_HIBERNATION, 						_T("The system was resumed from hibernation.") },
	{ MSGBOX_ERROR_WAKE_SYSTEM, 							_T("The system has awoken.") },
	{ MSGBOX_ERROR_BACKUP_REG_FAILED, 						_T("Backup data failed!") },
	{ MSGBOX_ERROR_LOAD_CFG_FAILED, 						_T("Load configuration data failed") },
	{ MSGBOX_ERROR_SAVE_CFG_FAILED, 						_T("Save configuration data failed") },
	{ MSGBOX_ERROR_LOAD_SCHED_FAILED, 						_T("Load schedule data failed") },
	{ MSGBOX_ERROR_SAVE_SCHED_FAILED, 						_T("Save schedule data failed") },
	{ MSGBOX_ERROR_LOAD_HKEYSET_FAILED, 					_T("Load hotkeyset data failed") },
	{ MSGBOX_ERROR_SAVE_HKEYSET_FAILED, 					_T("Save hotkeyset data failed") },
	{ MSGBOX_ERROR_LOAD_PWRRMD_FAILED, 						_T("Load reminder data failed") },
	{ MSGBOX_ERROR_SAVE_PWRRMD_FAILED, 						_T("Save reminder data failed") },
	{ MSGBOX_ERROR_WRITE_LOG_FAILED,						_T("Write log failed") },
	{ MSGBOX_ERROR_OUTPUT_LOG_FAILED,						_T("Output log failed") },
	{ MSGBOX_ERROR_UNKNOWN, 								_T("Unknown error.") },

/*--------String ID---------------------------------------------------------------Language String---------------------------------------------------------------------*/
	{ FORMAT_TIME_BEFOREMIDDAY,								_T("AM") },
	{ FORMAT_TIME_AFTERMIDDAY,								_T("PM") },
	{ HKEYSET_EXISTED_ALT_F4,								_T("Close active window") },
	{ HKEYSET_EXISTED_ALT_F8,								_T("Reveals typed password in Sign-in screen") },
	{ HKEYSET_EXISTED_CTRL_F3,								_T("Start search") },
	{ HKEYSET_EXISTED_CTRL_F4,								_T("Close the active document") },
	{ HKEYSET_EXISTED_CTRL_F5,								_T("Refresh current window") },
	{ HKEYSET_EXISTED_CTRL_WIN_F4,							_T("Close active virtual desktop") },

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
// 
//	Language: Vietnamese (Vietnam)
//
//////////////////////////////////////////////////////////////////////////

static LANGTABLE langtable_vi_VN
{
/*--------ActionName ID-----------------------------------------------------Language String---------------------------------------------------------------------------*/
	{ ACTION_NAME_DISPLAYOFF,								_T("Tắt màn hình") },
	{ ACTION_NAME_SLEEP,									_T("Ngủ") },
	{ ACTION_NAME_SHUTDOWN,									_T("Tắt máy") },
	{ ACTION_NAME_RESTART,									_T("Khởi động lại") },
	{ ACTION_NAME_SIGNOUT,									_T("Đăng xuất") },
	{ ACTION_NAME_HIBERNATE,								_T("Ngủ đông") },
	{ ACTION_NAME_SHOWMENU,									_T("Hiển thị menu") },

/*--------Action Message---------------------------------------------------------------Language String----------------------------------------------------------------*/
	{ MESSAGE_ACTION_DISPLAYOFF,							_T("Bạn có chắc chắn muốn tắt màn hình?") },
	{ MESSAGE_ACTION_SLEEP,									_T("Bạn có chắc chắn muốn chuyển máy sang chế độ ngủ?") },
	{ MESSAGE_ACTION_SHUTDOWN,								_T("Bạn có chắc chắn muốn tắt máy?") },
	{ MESSAGE_ACTION_RESTART,								_T("Bạn có chắc chắn muốn khởi động lại máy?") },
	{ MESSAGE_ACTION_SIGNOUT,								_T("Bạn có chắc chắn muốn đăng xuất khỏi tài khoản hiện tại?") },
	{ MESSAGE_ACTION_HIBERNATE,								_T("Bạn có chắc chắn muốn chuyển máy sang chế độ ngủ đông?") },

/*---------Control ID------------------------------------------------------------------Language String----------------------------------------------------------------*/
	{ IDC_MAIN_TITLE,										_T("Thiết lập chính") },
	{ IDC_LEFTMOUSE_TITLE,									_T("Nút chuột trái") },
	{ IDC_MIDMOUSE_TITLE,									_T("Nút chuột giữa") },
	{ IDC_RIGHTMOUSE_TITLE,									_T("Nút chuột phải") },
	{ IDC_ENABLERMBMENU_CHK,							 	_T("Chuột phải: Chỉ hiển thị menu") },
	{ IDC_APPLY_BTN, 										_T("Lưu") },
	{ IDC_RELOAD_BTN, 										_T("Tải lại") },
	{ IDC_EXIT_BTN,											_T("Kết thúc") },
	{ IDC_EXPAND_BTN,	 									_T("Mở rộng >>") },
	{ IDC_COLLAPSE_BTN,										_T("<< Thu nhỏ") },
	{ IDC_HELP_BTN, 										_T("Trợ giúp") },
	{ IDC_ABOUT_BTN, 										_T("Giới thiệu") },
	{ IDC_DEFAULT_BTN,										_T("Mặc định") },
	{ IDC_SYSTEM_TITLE,	 									_T("Thiết lập Nâng cao") },
	{ IDC_SHOWATSTARTUP_CHK,								_T("Hiển thị hộp thoại này khi khởi động") },
	{ IDC_ENABLEAUTOSTART_CHK, 								_T("Khởi động cùng Windows") },
	{ IDC_CONFIRMACTION_CHK,								_T("Hỏi trước khi thực hiện") },
	{ IDC_SAVEACTIONLOG_CHK,								_T("Lưu nhật ký hành động") },
	{ IDC_SAVEAPPEVENTLOG_CHK,								_T("Lưu nhật ký ứng dụng") },
	{ IDC_RUNASADMIN_CHK,									_T("Chạy với quyền admin") },
	{ IDC_SHOWERROR_CHK,							 		_T("Hiển thị thông báo lỗi trên màn hình") },
	{ IDC_SCHEDNOTIFY_CHK, 									_T("Nhắc nhở trước khi thực hiện lịch") },
	{ IDC_SCHEDALLOWCANCEL_CHK,								_T("Cho phép huỷ hẹn giờ khi có nhắc nhở") },
	{ IDC_ENBBKGRDHOTKEYS_CHK,								_T("Kích hoạt phím tắt hành động trong nền") },
	{ IDC_ENBPWRREMINDER_CHK,								_T("Kích hoạt chức năng Power Reminder") },
	{ IDC_LANGUAGE_TITLE, 									_T("Ngôn ngữ") },
	{ IDC_VIEWACTIONLOG_BTN,								_T("Xem nhật ký hoạt động") },
	{ IDC_BACKUPCFG_BTN, 									_T("Sao lưu thiết lập") },
	{ IDC_SCHEDULE_BTN, 									_T("Hẹn giờ") },
	{ IDC_LOGVIEWER_BTN, 									_T("Logviewer") },
	{ IDC_HOTKEYSET_BTN,									_T("Thiết lập phím tắt") },
	{ IDC_PWRREMINDER_BTN,									_T("Power Reminder") },

/*--------Menu Item ID----------------------------------------------------------------Language String-----------------------------------------------------------------*/
	{ IDM_OPENDLG_ABOUT,									_T("Giới thiệu") },
	{ IDM_OPENDLG_HELP,										_T("Trợ giúp") },
	{ IDM_VIEW_ACTIONLOG,									_T("Xem nhật ký hoạt động") },
	{ IDM_BACKUP_CONFIG,									_T("Sao lưu thiết lập") },
	{ IDM_VIEW_BAKCONFIG,									_T("Xem file sao lưu thiết lập") },
	{ IDM_OPENDLG_LOGVIEWER,								_T("LogViewer") },
	{ IDM_OPENDLG_SCHEDULE,									_T("Hẹn giờ") },
	{ IDM_OPENDLG_HOTKEYSET,								_T("Thiết lập phím tắt") },
	{ IDM_OPENDLG_PWRREMINDER,								_T("Power Reminder") },
	{ IDM_ACTIONS_TITLE,									_T("Hành động") },
	{ IDM_RESTART_APP,										_T("Khởi động lại ứng dụng") },
	{ IDM_RESTART_ASADMIN,									_T("Khởi động lại bằng quyền admin") },
	{ IDM_SHOW_WINDOW,										_T("Hiện cửa sổ chính") },
	{ IDM_EXIT_APP,											_T("Thoát") },

	{ IDM_ACTION_DISPLAYOFF,								_T("Tắt màn hình") },
	{ IDM_ACTION_SLEEP,										_T("Ngủ") },
	{ IDM_ACTION_SHUTDOWN,									_T("Tắt máy") },
	{ IDM_ACTION_RESTART,									_T("Khởi động lại") },
	{ IDM_ACTION_SIGNOUT,									_T("Đăng xuất") },
	{ IDM_ACTION_HIBERNATE,									_T("Ngủ đông") },
	{ IDM_ACTION_SCHEDULE,									_T("Hành động hẹn giờ") },

/*-------Combo-box Option ID-----------------------------------------------------------Language String----------------------------------------------------------------*/
	{ COMBOBOX_ACTION_DISPLAYOFF,							_T("Tắt màn hình") },
	{ COMBOBOX_ACTION_SLEEP,								_T("Ngủ") },
	{ COMBOBOX_ACTION_SHUTDOWN,								_T("Tắt máy") },
	{ COMBOBOX_ACTION_RESTART,								_T("Khởi động lại") },
	{ COMBOBOX_ACTION_SIGNOUT,								_T("Đăng xuất") },
	{ COMBOBOX_ACTION_HIBERNATE,							_T("Ngủ đông") },
	{ COMBOBOX_ACTION_SHOWMENU,								_T("Chỉ hiển thị menu") },
	{ COMBOBOX_LANGUAGE_ENGLISH,							_T("Tiếng Anh") },
	{ COMBOBOX_LANGUAGE_VIETNAMESE,							_T("Tiếng Việt") },
	{ COMBOBOX_LANGUAGE_SIMPCHINESE,						_T("Tiếng Trung") },

/*------Notify String ID---------------------------------------------------------------Language String----------------------------------------------------------------*/
	{ NOTIFY_TIP_TEMPLATE,									_T("Bấm chuột trái vào đây để %s.\n")
															_T("Bấm chuột giữa để %s.\n")
															_T("Bấm chuột phải để %s.") },
	{ NOTIFY_TIP_DISPLAYOFF,								_T("tắt màn hình") },
	{ NOTIFY_TIP_SLEEP,  									_T("ngủ") },
	{ NOTIFY_TIP_SHUTDOWN,  								_T("tắt máy") },
	{ NOTIFY_TIP_RESTART,  									_T("khởi động lại") },
	{ NOTIFY_TIP_SIGNOUT,  									_T("đăng xuất") },
	{ NOTIFY_TIP_HIBERNATE,  								_T("ngủ đông") },
	{ NOTIFY_TIP_SHOWMENU, 									_T("hiển thị menu") },

/*------Balloon String ID--------------------------------------------------------------Language String----------------------------------------------------------------*/
	{ BALLOON_TIP_TEMPLATE,									_T("Hành động sắp diễn ra.\n%s trong %d giây nữa.") },
	{ BALLOON_TIP_DISPLAYOFF, 								_T("Màn hình sẽ tắt") },
	{ BALLOON_TIP_SLEEP,									_T("Máy tính sẽ chuyển sang chế độ ngủ") },
	{ BALLOON_TIP_SHUTDOWN,									_T("Máy tính sẽ tắt") },
	{ BALLOON_TIP_RESTART, 									_T("Máy tính sẽ khởi động lại") },
	{ BALLOON_TIP_SIGNOUT,  								_T("Tài khoản hiện tại sẽ đăng xuất") },
	{ BALLOON_TIP_HIBERNATE,							 	_T("Máy tính sẽ chuyển sang chế độ ngủ đông") },

/*-----About Dialog Item---------------------------------------------------------------Language String----------------------------------------------------------------*/
	{ IDD_ABOUT_DLG, 										_T("Về Power++") },
	{ IDC_APPNAME_LABEL,									_T("Power++ 3.1 (v3.1.1.0)") },
	{ IDC_COPYRIGHT_LABEL, 									_T("Bản quyền thuộc (C) AnthonyLeeStark 2017 - 2024.") },
	{ IDC_AUTH_LABEL,  										_T("Phát triển bởi: Anthony Lee Stark (@AnthonyLeeStark)") },
	{ IDC_DEVPROFILE_LINK,									_T("<a>Xem trang Facebook cá nhân của tác giả</a>") },
	{ IDC_APPINFO_LABEL,									_T("Power++ - Tiện ích tắt máy tính nhanh\n")
															_T("Tắt máy và hẹn giờ tắt nhanh hơn và đơn giản hơn.\n\n")
															_T("Thông tin người phát triển:\n")
															_T("Tên: Anthony Lee Stark\n")
															_T("Email: anthonyleestark.official@gmail.com\n")
															_T("Facebook: https://www.facebook.com/anthonyleestark \n")
															_T("Twitter: https://twitter.com/AnthonyLeeStark \n\n")
															_T("Power++ v3.1 là một tiện ích cho phép người dùng thực hiện các hành động nguồn điện và hẹn giờ một cách nhanh chóng. ")
															_T("Nó giúp bạn tắt màn hình, chuyển máy tính sang chế độ ngủ và ngủ đông, tắt máy, khởi động lại và đăng xuất chỉ bằng một cú nhấp chuột. ")
															_T("Nó cũng giúp bạn hẹn giờ để thực hiện những hành động này một cách tự động hoặc thiết lập phím tắt cho từng hành động.") },
	{ IDC_ABOUT_CLOSE_BTN,	 								_T("Đóng") },

/*-----Help Dialog Item-----------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ IDD_HELP_DLG,											_T("Trợ giúp") },
	{ IDC_HELP_CLOSE_BTN,									_T("Đóng") },
	{ BTN_HELPDLG_VIEWMODE_HELPFILE,						_T("Xem Trợ giúp") },
	{ BTN_HELPDLG_VIEWMODE_CHANGELOG,						_T("Xem Changelog") },
	{ ERROR_HELPDLG_NOHELPFILE,								_T("Không thể mở file trợ giúp!\r\nFile có thể bị lỗi hoặc không tồn tại.") },
	{ ERROR_HELPDLG_NOCHANGELOGFILE,						_T("Không thể mở file changelog!\r\nFile có thể bị lỗi hoặc không tồn tại.") },

/*------Schedule Dialog Item------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ IDD_SCHEDULE_DLG,  									_T("Hẹn giờ") },
	{ IDC_SCHEDULE_ENABLE_CHK,								_T("Kích hoạt chức năng lịch hoạt động") },
	{ IDC_SCHEDULE_ACTION_LABEL,							_T("Hành động") },
	{ IDC_SCHEDULE_TIME_LABEL,  							_T("Thời gian") },
	{ IDC_SCHEDULE_REPEATDAILY_CHK,							_T("Lặp lại hành động hàng ngày") },
	{ IDC_SCHEDULE_APPLY_BTN,  								_T("Lưu") },
	{ IDC_SCHEDULE_CANCEL_BTN, 								_T("Đóng") },

/*-----LogViewer Dialog Item------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ IDD_LOGVIEWER_DLG,									_T("LogViewer") },
	{ IDC_LOGVIEWER_DETAILS_LABEL,							_T("Chi tiết") },
	{ IDC_LOGVIEWER_DATETIME_LABEL,							_T("Ngày/Giờ") },
	{ IDC_LOGVIEWER_CONTENT_LABEL,							_T("Nội dung") },

/*------HotkeySet Dialog Item-----------------------------------------------------------Language String---------------------------------------------------------------*/
	{ IDD_HOTKEYSET_DLG,									_T("Thiết lập phím tắt") },
	{ IDC_HOTKEYSET_ADD_BTN,								_T("Thêm") },
	{ IDC_HOTKEYSET_REMOVE_BTN,								_T("Xoá") },
	{ IDC_HOTKEYSET_REMOVEALL_BTN,							_T("Xoá hết") },
	{ IDC_HOTKEYSET_EXPORT_BTN,								_T("Trích xuất") },
	{ BTN_HKSETDLG_CHECKALL,								_T("Chọn hết") },
	{ BTN_HKSETDLG_UNCHECKALL,								_T("Bỏ chọn hết") },
	{ IDC_HOTKEYSET_ACTION_TITLE,							_T("Hành động") },
	{ IDC_HOTKEYSET_CONTROLKEY_STATIC,						_T("Phím điều khiển") },
	{ IDC_HOTKEYSET_CTRLKEY_CHK,							_T("CTRL") },
	{ IDC_HOTKEYSET_ALTKEY_CHK,								_T("ALT") },
	{ IDC_HOTKEYSET_WINKEY_CHK,								_T("WIN") },
	{ IDC_HOTKEYSET_FUNCKEY_TITLE,							_T("Phím chức năng") },
	{ IDC_HOTKEYSET_APPLY_BTN,								_T("Lưu") },
	{ IDC_HOTKEYSET_CANCEL_BTN,								_T("Huỷ bỏ") },
	{ LVCOLUMN_EMPTY_TITLE,									_T("") },
	{ LVCOLUMN_HOTKEYSET_ACTION,							_T("Hành động") },
	{ LVCOLUMN_HOTKEYSET_KEYSTROKES,						_T("Phím tắt") },

/*------Power Reminder Dialog Item---------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_PWRREMINDER_DLG,									_T("Power Reminder") },
	{ IDC_PWRREMINDER_ADD_BTN,								_T("Thêm") },
	{ IDC_PWRREMINDER_ADD_BTN,								_T("Sửa") },
	{ IDC_PWRREMINDER_REMOVE_BTN,							_T("Xoá") },
	{ IDC_PWRREMINDER_REMOVEALL_BTN,						_T("Xoá hết") },
	{ IDC_PWRREMINDER_CHECKALL_BTN,							_T("Chọn hết") },
	{ IDC_PWRREMINDER_UNCHECKALL_BTN,						_T("Bỏ chọn hết") },
	{ IDC_PWRREMINDER_PREVIEW_BTN,							_T("Xem trước") },
	{ IDC_PWRREMINDER_APPLY_BTN,							_T("Lưu") },
	{ IDC_PWRREMINDER_CANCEL_BTN,							_T("Huỷ bỏ") },
	{ IDC_PWRREMINDER_MSGSTRING_TITLE,						_T("Nội dung Thông điệp") },
	{ IDC_PWRREMINDER_EVENT_TITLE,							_T("Sự kiện") },
	{ IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,					_T("Vào thời gian được thiết lập cụ thể") },
	{ IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,				_T("Vào thời điểm khi ứng dụng này được khởi chạy") },
	{ IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,				_T("Vào thời điểm hệ thống khởi động (yêu cầu thiết lập \"Khởi động cùng Windows\")") },
	{ IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,			_T("Trước khi ứng dụng thực hiện một hành động nguồn điện bất kỳ") },
	{ IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,			_T("Vào thời điểm hệ thống thức/khởi động sau khi thực hiện hành động nguồn điện") },
	{ IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,				_T("Vào thời điểm trước khi chuẩn bị thoát khỏi ứng dụng") },
	{ IDC_PWRREMINDER_MSGSTYLE_TITLE,						_T("Kiểu thông điệp") },
	{ IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,				_T("Dạng Hộp thông báo") },
	{ IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,				_T("Dạng Hộp thoại") },
	{ GRIDCOLUMN_PWRREMINDER_INDEX,							_T("Số") },
	{ GRIDCOLUMN_PWRREMINDER_STATE,							_T("K.hoạt") },
	{ GRIDCOLUMN_PWRREMINDER_ITEMID,						_T("Mã đ.danh") },
	{ GRIDCOLUMN_PWRREMINDER_MESSAGE,						_T("Nội dung Thông điệp") },
	{ GRIDCOLUMN_PWRREMINDER_EVENTID,						_T("Sự kiện") },
	{ GRIDCOLUMN_PWRREMINDER_STYLE,							_T("Kiểu") },
	{ GRIDCOLUMN_PWRREMINDER_REPEAT,						_T("Lặp lại") },
	{ PWRRMD_EVENT_AT_SETTIME,								_T("Vào %02d:%02d %2s") },
	{ PWRRMD_EVENT_AT_APPSTARTUP,							_T("Khi ứ.dụng k.động") },
	{ PWRRMD_EVENT_AT_SYSWAKEUP,							_T("Khi h.thống thức") },
	{ PWRRMD_EVENT_AT_BFRPWRACTION,							_T("Trước khi th.hiện h.động") },
	{ PWRRMD_EVENT_AT_PWRACTIONWAKE,						_T("Khi thức sau h.động") },
	{ PWRRMD_EVENT_AT_APPEXIT,								_T("Khi thoát ứ.dụng") },
	{ PWRRMD_STYLE_MESSAGEBOX,								_T("Hộp thông báo") },
	{ PWRRMD_STYLE_DIALOG,									_T("Hộp thoại") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MESSAGE_SCHEDNOTIFY_DISPLAYOFF,						_T("Máy tính của bạn sẽ tắt màn hình") },
	{ MESSAGE_SCHEDNOTIFY_SLEEP,							_T("Máy tính của bạn sẽ chuyển sang chế độ ngủ") },
	{ MESSAGE_SCHEDNOTIFY_SHUTDOWN,							_T("Máy tính của bạn sẽ tắt nguồn") },
	{ MESSAGE_SCHEDNOTIFY_RESTART,							_T("Máy tính của bạn sẽ khởi động lại") },
	{ MESSAGE_SCHEDNOTIFY_SIGNOUT,							_T("Tài khoản của bạn sẽ bị đăng xuất") },
	{ MESSAGE_SCHEDNOTIFY_HIBERNATE,						_T("Máy tính của bạn sẽ chuyển sang chế độ ngủ đông") },

/*----------MessageBox ID---------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ MSGBOX_CONFIG_RELOAD_CAPTION,							_T("Tải lại thiết lập") },
	{ MSGBOX_CONFIG_CHANGED_CONTENT,						_T("Một vài thiết lập đã bị thay đổi.\nBạn có chắc muốn bỏ qua tất cả thay đổi và tải lại thiết lập không?") },
	{ MSGBOX_CONFIG_NOTCHANGED_CONTENT,						_T("Tất cả thiết lập đều không thay đổi. Không có gì để tải lại.") },
	{ MSGBOX_SCHEDULE_CHANGED_CAPTION,						_T("Lưu thiết lập") },
	{ MSGBOX_SCHEDULE_CHANGED_CONTENT,						_T("Một số thiết lập đã bị thay đổi.\nBạn có muốn lưu chúng lại không?") },
	{ MSGBOX_SCHEDULE_NOTIFY,								_T("%s sau 30 giây nữa") },
	{ MSGBOX_SCHEDULE_ALLOWCANCEL,							_T("\nBạn có thể bấm [Cancel] để huỷ hẹn giờ") },
	{ MSGBOX_SCHEDULE_CANCELED,								_T("Đã huỷ hẹn giờ thành công.") },
	{ MSGBOX_HOTKEYSET_CAPTION,								_T("Thiết lập phím tắt") },
	{ MSGBOX_HOTKEYSET_REG_FAILED,							_T("Đăng ký phím tắt trong nền thất bại.") },
	{ MSGBOX_HOTKEYSET_UNREG_FAILED,						_T("Huỷ đăng ký phím tắt trong nền thất bại.") },
	{ MSGBOX_HOTKEYSET_CHANGED_CONTENT,						_T("Một vài dữ liệu đã bị thay đổi.\nBạn có muốn lưu chúng lại không?") },
	{ MSGBOX_HOTKEYSET_REMOVE_ITEM,							_T("Bạn có muốn xoá mục thiết lập phím tắt này không?") },
	{ MSGBOX_HOTKEYSET_REMOVEALL_ITEMS,						_T("Bạn có muốn xoá tất cả mục thiết lập phím tắt không?") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID,				_T("Thiết lập phím tắt không hợp lệ.\nHành động không thể để trống hoặc nằm ngoài phạm vi cho phép.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY,					_T("Thiết lập phím tắt không hợp lệ.\nVui lòng chọn ít nhất một phím điều khiển.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY,					_T("Thiết lập phím tắt không hợp lệ.\nTuỳ chọn phím chức năng không thể để trống hoặc nằm ngoài phạm vi cho phép.") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_EMPTY,					_T("Thiết lập phím tắt không hợp lệ.\nDữ liệu không thể để trống hoặc nằm ngoài phạm vi cho phép.") },
	{ MSGBOX_HOTKEYSET_EXISTED_HOTKEY,						_T("Phím tắt hệ thống đã tồn tại.\n%s\nVui lòng chọn lại thiết lập phím tắt khác.") },
	{ MSGBOX_PWRREMINDER_CAPTION,							_T("Power Reminder") },
	{ MSGBOX_PWRREMINDER_DISPLAY_FAILED,					_T("Có lỗi xảy ra khi hiển thị nội dung nhắc nhở.") },
	{ MSGBOX_PWRREMINDER_CHANGED_CONTENT,					_T("Một vài dữ liệu đã bị thay đổi.\nBạn có muốn lưu chúng lại không?") },
	{ MSGBOX_PWRREMINDER_REMOVE_ITEM,						_T("Bạn có muốn xoá mục nhắc nhở này không?") },
	{ MSGBOX_PWRREMINDER_REMOVEALL_ITEMS,					_T("Bạn có muốn xoá tất cả mục nhắc nhở không?") },
	{ MSGBOX_PWRREMINDER_CONFIRM_EXITMODE,					_T("Bạn có chắc muốn huỷ bỏ tất cả các thay đổi?") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID,				_T("Thiết lập không hợp lệ.\nMã định danh nằm ngoài phạm vi cho phép.") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY,			_T("Thiết lập không hợp lệ.\nNội dung thông điệp không được để trống") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT,	_T("Thiết lập không hợp lệ.\nNội dung thông điệp không được vượt quá 50 ký tự") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_INVALIDCHAR,	_T("Thiết lập không hợp lệ.\nNội dung thông điệp chứa ký tự không hợp lệ") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID,				_T("Thiết lập không hợp lệ.\nThiết lập sự kiện nằm ngoài phạm vi cho phép") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_TIMEVALUE,				_T("Thiết lập không hợp lệ.\nThiết lập thời gian nằm ngoài phạm vi cho phép") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID,				_T("Thiết lập không hợp lệ.\nThiếp lập kiểu thông điệp nằm ngoài phạm vi cho phép") },
	{ MSGBOX_OTHER_NOTRUNASADMIN,							_T("Ứng dụng hiện đang không được chạy dưới quyền admin.") },
	{ MSGBOX_OTHER_REQUEST_RESTARTASADMIN,					_T("Bạn có muốn khởi động lại ứng dụng với quyền admin không?") },
	{ MSGBOX_OTHER_RUNASADMIN_NEXTTIME,						_T("Ứng dụng sẽ được chạy dưới quyền admin ở lần khởi động tiếp theo.") },
	{ MSGBOX_OTHER_ALREADY_ADMIN,							_T("Ứng dụng hiện đang được chạy dưới quyền admin.") },
	{ MSGBOX_OTHER_FUNCTION_NOT_AVAILABLE,					_T("Tính năng hiện tại chưa được hỗ trợ") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MSGBOX_ERROR_CAPTION,									_T("Lỗi") },
	{ MSGBOX_ERROR_FAILED,									_T("Thực thi hành động thất bại!") },
	{ MSGBOX_ERROR_WRONG_ARGUMENT, 							_T("Tham số của phương thức không đúng!") },
	{ MSGBOX_ERROR_INVALID_FUNCTION, 						_T("Phương thức không hợp lệ.") },
	{ MSGBOX_ERROR_FILE_NOT_FOUND, 							_T("Hệ thống không tìm thấy file được chỉ định.") },
	{ MSGBOX_ERROR_PATH_NOT_FOUND, 							_T("Hệ thống không tìm thấy đường dẫn được chỉ định.") },
	{ MSGBOX_ERROR_ACCESS_DENIED, 							_T("Truy cập bị từ chối.") },
	{ MSGBOX_ERROR_INVALID_HANDLE, 							_T("The handle is invalid.") },
	{ MSGBOX_ERROR_INVALID_DATA, 							_T("Dữ liệu không hợp lệ.") },
	{ MSGBOX_ERROR_NO_MORE_FILES, 							_T("File không tồn tại.") },
	{ MSGBOX_ERROR_FILE_EXISTS, 							_T("File đã tồn tại.") },
	{ MSGBOX_ERROR_CANNOT_MAKE, 							_T("Không thể tạo file hoặc thư mục.") },
	{ MSGBOX_ERROR_INVALID_PARAMETER, 						_T("Tham số không hợp lệ.") },
	{ MSGBOX_ERROR_OPEN_FAILED, 							_T("Hệ thống không thể mở thiết bị hoặc file được chỉ định.") },
	{ MSGBOX_ERROR_BUFFER_OVERFLOW, 						_T("Tên file quá dài.") },
	{ MSGBOX_ERROR_INVALID_NAME, 							_T("Tên file, thư mục, hoặc ổ đĩa không hợp lệ.") },
	{ MSGBOX_ERROR_DIR_NOT_EMPTY, 							_T("Thư mục không rỗng.") },
	{ MSGBOX_ERROR_FAIL_SHUTDOWN, 							_T("Tắt máy tính thất bại.") },
	{ MSGBOX_ERROR_FAIL_RESTART, 							_T("Khởi động lại thất bại.") },
	{ MSGBOX_ERROR_INVALID_ADDRESS, 						_T("Đang cố truy cập vào địa chỉ không hợp lệ.") },
	{ MSGBOX_ERROR_APP_INIT_FAILURE, 						_T("Ứng dụng không thể khởi động một cách chính xác. Bấm OK để đóng ứng dụng.") },
	{ MSGBOX_ERROR_CANNOT_LOAD_REGISTRY,					_T("Không thể tải registry. Registry có thể bị hỏng, thiếu hoặc không thể ghi.") },
	{ MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT,					_T("Hệ thống đã đạt giới hạn kích thước cho phép của phân vùng registry. Yêu cầu lưu thêm sẽ bị từ chối.") },
	{ MSGBOX_ERROR_SYSTEM_SHUTDOWN, 						_T("Hệ thống đang tiến hành chuyển sang trạng thái tắt.") },
	{ MSGBOX_ERROR_HIBERNATED, 								_T("Hệ thống đã rơi vào chế độ ngủ đông.") },
	{ MSGBOX_ERROR_RESUME_HIBERNATION, 						_T("Hệ thống đã thức khỏi chế độ ngủ đông.") },
	{ MSGBOX_ERROR_WAKE_SYSTEM, 							_T("Hệ thống đã bị đánh thức.") },
	{ MSGBOX_ERROR_BACKUP_REG_FAILED, 						_T("Sao lưu dữ liệu thất bại!") },
	{ MSGBOX_ERROR_LOAD_CFG_FAILED, 						_T("Tải dữ liệu thiết lập cấu hình thất bại!") },
	{ MSGBOX_ERROR_SAVE_CFG_FAILED, 						_T("Lưu dữ liệu thiết lập cấu hình thất bại!") },
	{ MSGBOX_ERROR_LOAD_SCHED_FAILED, 						_T("Tải dữ liệu thiết lập hẹn giờ thất bại!") },
	{ MSGBOX_ERROR_SAVE_SCHED_FAILED, 						_T("Lưu dữ liệu thiết lập hẹn giờ thất bại!") },
	{ MSGBOX_ERROR_LOAD_HKEYSET_FAILED, 					_T("Tải dữ liệu thiết lập phím tắt thất bại!") },
	{ MSGBOX_ERROR_SAVE_HKEYSET_FAILED, 					_T("Lưu dữ liệu thiết lập phím tắt thất bại!") },
	{ MSGBOX_ERROR_LOAD_PWRRMD_FAILED, 						_T("Tải dữ liệu nội dung nhắc nhở thất bại!") },
	{ MSGBOX_ERROR_SAVE_PWRRMD_FAILED, 						_T("Lưu dữ liệu nội dung nhắc nhở thất bại!") },
	{ MSGBOX_ERROR_WRITE_LOG_FAILED,						_T("Ghi log thất bại") },
	{ MSGBOX_ERROR_OUTPUT_LOG_FAILED,						_T("Xuất log thất bại") },
	{ MSGBOX_ERROR_UNKNOWN, 								_T("Lỗi không xác định.") },

/*--------String ID---------------------------------------------------------------Language String---------------------------------------------------------------------*/
	{ FORMAT_TIME_BEFOREMIDDAY,								_T("SA") },
	{ FORMAT_TIME_AFTERMIDDAY,								_T("CH") },
	{ HKEYSET_EXISTED_ALT_F4,								_T("Đóng cửa sổ hiện tại") },
	{ HKEYSET_EXISTED_ALT_F8,								_T("Hiển thị mật khẩu đã nhập trong màn hình Đăng nhập") },
	{ HKEYSET_EXISTED_CTRL_F3,								_T("Bắt đầu tìm kiếm") },
	{ HKEYSET_EXISTED_CTRL_F4,								_T("Đóng tài liệu hiện tại") },
	{ HKEYSET_EXISTED_CTRL_F5,								_T("Làm mới cửa sổ hiện tại") },
	{ HKEYSET_EXISTED_CTRL_WIN_F4,							_T("Đóng màn hình nền ảo hiện tại") },

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
};

////////////////////////////////////////////////////////
// 
//	Language: Simplified Chinese (China mainland)
//
////////////////////////////////////////////////////////

static LANGTABLE langtable_zh_CH
{
/*--------ActionName ID-----------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ ACTION_NAME_DISPLAYOFF,								_T("关闭显示器") },
	{ ACTION_NAME_SLEEP,									_T("睡眠") },
	{ ACTION_NAME_SHUTDOWN,									_T("关闭") },
	{ ACTION_NAME_RESTART,									_T("重启") },
	{ ACTION_NAME_SIGNOUT,									_T("登出") },
	{ ACTION_NAME_HIBERNATE,								_T("休眠") },
	{ ACTION_NAME_SHOWMENU,									_T("显示通知托盘菜单") },

/*--------Action Message----------------------------------------------------------------Language String---------------------------------------------------------------*/
	{ MESSAGE_ACTION_DISPLAYOFF,							_T("您确定要关闭显示器吗？") },
	{ MESSAGE_ACTION_SLEEP,									_T("您确定要将电脑进入睡眠模式吗？") },
	{ MESSAGE_ACTION_SHUTDOWN,								_T("您确定要将电脑关闭吗？") },
	{ MESSAGE_ACTION_RESTART,								_T("您确定要重新启动计算机吗？") },
	{ MESSAGE_ACTION_SIGNOUT,								_T("您确定要退出当前帐户吗？") },
	{ MESSAGE_ACTION_HIBERNATE,								_T("您确定要将电脑进入休眠模式吗？") },

/*---------Control ID--------------------------------------------------------------------Language String--------------------------------------------------------------*/
	{ IDC_MAIN_TITLE,										_T("主要设置") },
	{ IDC_LEFTMOUSE_TITLE,									_T("鼠标左键") },
	{ IDC_MIDMOUSE_TITLE,									_T("鼠标中键") },
	{ IDC_RIGHTMOUSE_TITLE,									_T("鼠标右键") },
	{ IDC_ENABLERMBMENU_CHK,								_T("鼠标右键：仅显示菜单") },
	{ IDC_APPLY_BTN,		 								_T("应用") },
	{ IDC_RELOAD_BTN,				 						_T("重新加载") },
	{ IDC_EXIT_BTN,											_T("退出") },
	{ IDC_EXPAND_BTN, 										_T("扩大 >>") },
	{ IDC_COLLAPSE_BTN,										_T("<< 缩小") },
	{ IDC_HELP_BTN,											_T("帮助") },
	{ IDC_ABOUT_BTN,				 						_T("关于") },
	{ IDC_DEFAULT_BTN,										_T("默认") },
	{ IDC_SYSTEM_TITLE, 									_T("高级设置") },
	{ IDC_SHOWATSTARTUP_CHK,								_T("启动时显示此对话框") },
	{ IDC_ENABLEAUTOSTART_CHK,						 		_T("随Windows启动") },
	{ IDC_CONFIRMACTION_CHK,								_T("执行动作前确认") },
	{ IDC_SAVEACTIONLOG_CHK,								_T("保存操作日志") },
	{ IDC_SAVEAPPEVENTLOG_CHK,								_T("保存应用程序事件日志") },
	{ IDC_RUNASADMIN_CHK,									_T("以管理员权限运行") },
	{ IDC_SHOWERROR_CHK,									_T("在屏幕上显示错误信息") },
	{ IDC_SCHEDNOTIFY_CHK, 									_T("执行预定操作前通知") },
	{ IDC_SCHEDALLOWCANCEL_CHK,				 				_T("允许在收到通知时取消计划") },
	{ IDC_ENBBKGRDHOTKEYS_CHK,								_T("启用后台操作热键") },
	{ IDC_ENBPWRREMINDER_CHK,								_T("启用Power Reminder功能") },
	{ IDC_LANGUAGE_TITLE,									_T("语言") },
	{ IDC_VIEWACTIONLOG_BTN,								_T("查看操作日志") },
	{ IDC_BACKUPCFG_BTN,									_T("备份设置") },
	{ IDC_SCHEDULE_BTN,										_T("行动计划") },
	{ IDC_LOGVIEWER_BTN,									_T("日志查看器") },
	{ IDC_HOTKEYSET_BTN,									_T("热键设置") },
	{ IDC_PWRREMINDER_BTN,									_T("Power Reminder") },

/*--------Menu Item ID--------------------------------------------------------------------Language String-------------------------------------------------------------*/
	{ IDM_OPENDLG_ABOUT,									_T("关于") },
	{ IDM_OPENDLG_HELP,										_T("帮助") },
	{ IDM_VIEW_ACTIONLOG,									_T("查看操作日志") },
	{ IDM_BACKUP_CONFIG,									_T("备份设置") },
	{ IDM_VIEW_BAKCONFIG,									_T("查看备份设置") },
	{ IDM_OPENDLG_LOGVIEWER,								_T("日志查看器") },
	{ IDM_OPENDLG_SCHEDULE,									_T("行动计划") },
	{ IDM_OPENDLG_HOTKEYSET,								_T("热键设置") },
	{ IDM_OPENDLG_PWRREMINDER,								_T("Power Reminder") },
	{ IDM_ACTIONS_TITLE,									_T("操作") },
	{ IDM_RESTART_APP,										_T("重启应用程序") },
	{ IDM_RESTART_ASADMIN,									_T("以管理员权限重新启动") },
	{ IDM_SHOW_WINDOW,										_T("主窗口") },
	{ IDM_EXIT_APP,											_T("退出") },

	{ IDM_ACTION_DISPLAYOFF,								_T("关闭显示器") },
	{ IDM_ACTION_SLEEP,										_T("睡眠") },
	{ IDM_ACTION_SHUTDOWN,									_T("关闭") },
	{ IDM_ACTION_RESTART,									_T("重启") },
	{ IDM_ACTION_SIGNOUT,									_T("登出") },
	{ IDM_ACTION_HIBERNATE,									_T("休眠") },
	{ IDM_ACTION_SCHEDULE,									_T("行动计划") },

/*-------Combo-box Option ID---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ COMBOBOX_ACTION_DISPLAYOFF,							_T("关闭显示器") },
	{ COMBOBOX_ACTION_SLEEP,								_T("睡眠") },
	{ COMBOBOX_ACTION_SHUTDOWN,								_T("关闭") },
	{ COMBOBOX_ACTION_RESTART,								_T("重启") },
	{ COMBOBOX_ACTION_SIGNOUT,								_T("登出") },
	{ COMBOBOX_ACTION_HIBERNATE,							_T("休眠") },
	{ COMBOBOX_ACTION_SHOWMENU,								_T("仅显示菜单") },
	{ COMBOBOX_LANGUAGE_ENGLISH,							_T("英文") },
	{ COMBOBOX_LANGUAGE_VIETNAMESE,							_T("越语") },
	{ COMBOBOX_LANGUAGE_SIMPCHINESE,						_T("中文") },

/*------Notify String ID-------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ NOTIFY_TIP_TEMPLATE,						 			_T("单击此处鼠标左键就%s.\n")
															_T("单击此处鼠标中键就%s.\n")
															_T("单击此处鼠标右键就%s.") },
	{ NOTIFY_TIP_DISPLAYOFF,								_T("关闭显示器") },
	{ NOTIFY_TIP_SLEEP,  									_T("睡眠") },
	{ NOTIFY_TIP_SHUTDOWN,						 			_T("关闭") },
	{ NOTIFY_TIP_RESTART,			 						_T("重启") },
	{ NOTIFY_TIP_SIGNOUT,  									_T("登出") },
	{ NOTIFY_TIP_HIBERNATE,  								_T("休眠") },
	{ NOTIFY_TIP_SHOWMENU, 									_T("显示菜单") },

/*------Balloon String ID-------------------------------------------------------------------Language String-----------------------------------------------------------*/
	{ BALLOON_TIP_TEMPLATE,							 		_T("预定的行动即将开始。\n%s在%d秒后.") },
	{ BALLOON_TIP_DISPLAYOFF, 								_T("屏幕将关闭") },
	{ BALLOON_TIP_SLEEP,									_T("电脑将进入睡眠状态") },
	{ BALLOON_TIP_SHUTDOWN,					  				_T("电脑将关闭") },
	{ BALLOON_TIP_RESTART, 									_T("电脑将重新启动") },
	{ BALLOON_TIP_SIGNOUT,  								_T("当前帐户将退出") },
	{ BALLOON_TIP_HIBERNATE,  								_T("电脑将进入休眠状态") },

/*-----About Dialog Item--------------------------------------------------------------------Language String-----------------------------------------------------------*/
	{ IDD_ABOUT_DLG,										_T("关于Power++") },
	{ IDC_APPNAME_LABEL,								  	_T("Power++ 3.1 (v3.1.1.0)") },
	{ IDC_COPYRIGHT_LABEL, 							 		_T("版权(C)属于2017 - 2024 AnthonyLeeStark.") },
	{ IDC_AUTH_LABEL,  										_T("开发者：Anthony Lee Stark (@AnthonyLeeStark)") },
	{ IDC_DEVPROFILE_LINK,	 								_T("<a>查看开发者的Facebook个人资料</a>") },
	{ IDC_APPINFO_LABEL,						 			_T("Power++ - 快速电源行动实用程序\n")
															_T("执行电源操作和设置电源计划更简单。\n\n")
															_T("开发者简介：\n")
															_T("姓名：AnthonyLeeStark\n")
															_T("电子邮件：anthonyleestark.official@gmail.com\n")
															_T("Facebook： https://www.facebook.com/anthonyleestark \n")
															_T("Twitter: https://twitter.com/AnthonyLeeStark \n\n")
															_T("Power++ v3.1 是一个实用程序，它允许您快速执行一些电源操作并设置执行这些操作的计划。")
															_T("此软件可帮助您只需单击一下即可关闭屏幕、使 PC 进入睡眠状态、关机、重启、休眠或退出当前帐户。")
															_T("它还可以帮助您设置闹钟来自动执行这些操作或为每个操作设置热键。") },
	{ IDC_ABOUT_CLOSE_BTN,	 								_T("关闭") },

/*-----Help Dialog Item--------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_HELP_DLG,											_T("帮助") },
	{ IDC_HELP_CLOSE_BTN,									_T("关闭") },
	{ BTN_HELPDLG_VIEWMODE_HELPFILE,						_T("查看帮助") },
	{ BTN_HELPDLG_VIEWMODE_CHANGELOG,						_T("查看变更日志") },
	{ ERROR_HELPDLG_NOHELPFILE,								_T("无法打开帮助文件！\r\n该文件可能已损坏或不存在。") },
	{ ERROR_HELPDLG_NOCHANGELOGFILE,						_T("无法打开变更日志文件！\r\n该文件可能已损坏或不存在。") },

/*------Schedule Dialog Item---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_SCHEDULE_DLG,  									_T("行动计划") },
	{ IDC_SCHEDULE_ENABLE_CHK,								_T("启用行动计划") },
	{ IDC_SCHEDULE_ACTION_LABEL,							_T("行动") },
	{ IDC_SCHEDULE_TIME_LABEL,  							_T("时间") },
	{ IDC_SCHEDULE_REPEATDAILY_CHK,							_T("每天重复计划") },
	{ IDC_SCHEDULE_APPLY_BTN,		 						_T("应用") },
	{ IDC_SCHEDULE_CANCEL_BTN, 								_T("关闭") },

/*-----LogViewer Dialog Item---------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_LOGVIEWER_DLG,									_T("日志查看器") },
	{ IDC_LOGVIEWER_DETAILS_LABEL,							_T("细节") },
	{ IDC_LOGVIEWER_DATETIME_LABEL,							_T("日期/时间") },
	{ IDC_LOGVIEWER_CONTENT_LABEL,							_T("内容") },

/*------HotkeySet Dialog Item--------------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_HOTKEYSET_DLG,									_T("热键设置") },
	{ IDC_HOTKEYSET_ADD_BTN,								_T("添加") },
	{ IDC_HOTKEYSET_REMOVE_BTN,								_T("删除") },
	{ IDC_HOTKEYSET_REMOVEALL_BTN,							_T("全部删除") },
	{ IDC_HOTKEYSET_EXPORT_BTN,								_T("导出") },
	{ BTN_HKSETDLG_CHECKALL,								_T("全部选中") },
	{ BTN_HKSETDLG_UNCHECKALL,								_T("全部取消选中") },
	{ IDC_HOTKEYSET_ACTION_TITLE,							_T("行动") },
	{ IDC_HOTKEYSET_CONTROLKEY_STATIC,						_T("控制键") },
	{ IDC_HOTKEYSET_CTRLKEY_CHK,							_T("CTRL") },
	{ IDC_HOTKEYSET_ALTKEY_CHK,								_T("ALT") },
	{ IDC_HOTKEYSET_WINKEY_CHK,								_T("WIN") },
	{ IDC_HOTKEYSET_FUNCKEY_TITLE,							_T("功能键") },
	{ IDC_HOTKEYSET_APPLY_BTN,								_T("应用") },
	{ IDC_HOTKEYSET_CANCEL_BTN,								_T("取消") },
	{ LVCOLUMN_EMPTY_TITLE,									_T("") },
	{ LVCOLUMN_HOTKEYSET_ACTION,							_T("行动") },
	{ LVCOLUMN_HOTKEYSET_KEYSTROKES,						_T("按键") },

/*------Power Reminder Dialog Item---------------------------------------------------------Language String------------------------------------------------------------*/
	{ IDD_PWRREMINDER_DLG,									_T("Power Reminder") },
	{ IDC_PWRREMINDER_ADD_BTN,								_T("添加") },
	{ IDC_PWRREMINDER_EDIT_BTN,								_T("编辑") },
	{ IDC_PWRREMINDER_REMOVE_BTN,							_T("删除") },
	{ IDC_PWRREMINDER_REMOVEALL_BTN,						_T("全部删除") },
	{ IDC_PWRREMINDER_CHECKALL_BTN,							_T("全部选中") },
	{ IDC_PWRREMINDER_UNCHECKALL_BTN,						_T("全部取消选中") },
	{ IDC_PWRREMINDER_PREVIEW_BTN,							_T("预览") },
	{ IDC_PWRREMINDER_APPLY_BTN,							_T("应用") },
	{ IDC_PWRREMINDER_CANCEL_BTN,							_T("取消") },
	{ IDC_PWRREMINDER_MSGSTRING_TITLE,						_T("消息内容") },
	{ IDC_PWRREMINDER_EVENT_TITLE,							_T("事件") },
	{ IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,					_T("在特定的时间") },
	{ IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,				_T("在此软件程序启动的时间") },
	{ IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,				_T("在系统唤醒的时间 (要求 \"随Windows启动\")") },
	{ IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,			_T("在此软件程序执行任何电源操作之前") },
	{ IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,			_T("在系统从任何电源操作唤醒之后") },
	{ IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,				_T("在此软件程序退出之前") },
	{ IDC_PWRREMINDER_MSGSTYLE_TITLE,						_T("消息样式") },
	{ IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,				_T("消息框样式") },
	{ IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,				_T("对话框样式") },
	{ GRIDCOLUMN_PWRREMINDER_INDEX,							_T("号") },
	{ GRIDCOLUMN_PWRREMINDER_STATE,							_T("启用") },
	{ GRIDCOLUMN_PWRREMINDER_ITEMID,						_T("项目ID") },
	{ GRIDCOLUMN_PWRREMINDER_MESSAGE,						_T("消息内容") },
	{ GRIDCOLUMN_PWRREMINDER_EVENTID,						_T("事件") },
	{ GRIDCOLUMN_PWRREMINDER_STYLE,							_T("样式") },
	{ GRIDCOLUMN_PWRREMINDER_REPEAT,						_T("重复") },
	{ PWRRMD_EVENT_AT_SETTIME,								_T("在 %02d:%02d %2s") },
	{ PWRRMD_EVENT_AT_APPSTARTUP,							_T("在软件启动时") },
	{ PWRRMD_EVENT_AT_SYSWAKEUP,							_T("在系统唤醒时") },
	{ PWRRMD_EVENT_AT_BFRPWRACTION,							_T("在执行操作前") },
	{ PWRRMD_EVENT_AT_PWRACTIONWAKE,						_T("在行动后唤醒时") },
	{ PWRRMD_EVENT_AT_APPEXIT,								_T("在软件退出前") },
	{ PWRRMD_STYLE_MESSAGEBOX,								_T("消息框") },
	{ PWRRMD_STYLE_DIALOG,									_T("对话框") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MESSAGE_SCHEDNOTIFY_DISPLAYOFF,						_T("电脑将关闭显示器") },
	{ MESSAGE_SCHEDNOTIFY_SLEEP,							_T("电脑将进入睡眠模式") },
	{ MESSAGE_SCHEDNOTIFY_SHUTDOWN,							_T("电脑将关闭") },
	{ MESSAGE_SCHEDNOTIFY_RESTART,							_T("电脑将重新启动") },
	{ MESSAGE_SCHEDNOTIFY_SIGNOUT,							_T("帐户将退出") },
	{ MESSAGE_SCHEDNOTIFY_HIBERNATE,						_T("将进入休眠模式") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MSGBOX_CONFIG_RELOAD_CAPTION,							_T("重新加载设置") },
	{ MSGBOX_CONFIG_CHANGED_CONTENT,						_T("一些选项已经改变。\n您是否要中止所有更改并重新加载设置？") },
	{ MSGBOX_CONFIG_NOTCHANGED_CONTENT,						_T("所有设置均已更新。无需重新加载。") },
	{ MSGBOX_SCHEDULE_CHANGED_CAPTION,						_T("保存选项") },
	{ MSGBOX_SCHEDULE_CHANGED_CONTENT,						_T("一些选项已经改变。\n你想拯救它们吗？") },
	{ MSGBOX_SCHEDULE_NOTIFY,								_T("您的%s在30秒后") },
	{ MSGBOX_SCHEDULE_ALLOWCANCEL,							_T("\n您可以按[取消]取消计划操作") },
	{ MSGBOX_SCHEDULE_CANCELED,								_T("计划已成功取消。") },
	{ MSGBOX_HOTKEYSET_CAPTION,								_T("Hotkey Set") },
	{ MSGBOX_HOTKEYSET_REG_FAILED,							_T("无法注册后台热键。") },
	{ MSGBOX_HOTKEYSET_UNREG_FAILED,						_T("无法取消注册后台热键。") },
	{ MSGBOX_HOTKEYSET_CHANGED_CONTENT,						_T("部分数据已被更改。\n你想拯救它们吗？") },
	{ MSGBOX_HOTKEYSET_REMOVE_ITEM,							_T("您是否要删除所有此热键项？") },
	{ MSGBOX_HOTKEYSET_REMOVEALL_ITEMS,						_T("您是否要删除所有热键项吗？") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID,				_T("HotkeySet 无效。\n操作不能为空或者无效。") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY,					_T("HotkeySet 无效。\n请选择至少一个控制键。") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY,					_T("HotkeySet 无效。\n功能键选项不能为空或者无效。") },
	{ MSGBOX_HOTKEYSET_INVALIDITEM_EMPTY,					_T("HotkeySet 无效。\n数据不能为空或者无效。") },
	{ MSGBOX_HOTKEYSET_EXISTED_HOTKEY,						_T("系统键快捷键已存在。\n%s\n请选择其他热键选项。") },
	{ MSGBOX_PWRREMINDER_CAPTION,							_T("Power Reminder") },
	{ MSGBOX_PWRREMINDER_DISPLAY_FAILED,					_T("提醒内容显示中发生些错吴的。") },
	{ MSGBOX_PWRREMINDER_CHANGED_CONTENT,					_T("部分数据已被更改。\n你想拯救它们吗？") },
	{ MSGBOX_PWRREMINDER_REMOVE_ITEM,						_T("您是否要删除此提醒项？") },
	{ MSGBOX_PWRREMINDER_REMOVEALL_ITEMS,					_T("您是否要删除所有提醒事项吗？") },
	{ MSGBOX_PWRREMINDER_CONFIRM_EXITMODE,					_T("您确实要中止所有当前更改吗？") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID,				_T("提醒数据无效。\n产品 ID 为空或无效。") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY,			_T("提醒数据无效。\n留言内容不能为空。") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT,	_T("提醒数据无效。\n留言内容不能超过50个字符。") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_INVALIDCHAR,	_T("提醒数据无效。\n邮件内容包含一个或多个无效字符") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID,				_T("提醒数据无效。\n事件设置为空或者无效。") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_TIMEVALUE,				_T("提醒数据无效。\n时间设置为空或者无效。") },
	{ MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID,				_T("提醒数据无效。\n消息样式设置为空或者无效。") },
	{ MSGBOX_OTHER_NOTRUNASADMIN,							_T("该应用程序当前未以管理员权限运行。") },
	{ MSGBOX_OTHER_REQUEST_RESTARTASADMIN,					_T("您是否要以管理员权限重新启动该应用程序？") },
	{ MSGBOX_OTHER_RUNASADMIN_NEXTTIME,						_T("该应用程序将在下次启动时以管理员权限运行。") },
	{ MSGBOX_OTHER_ALREADY_ADMIN,							_T("该应用程序当前以管理员权限运行。") },
	{ MSGBOX_OTHER_FUNCTION_NOT_AVAILABLE,					_T("此功能目前不可用。") },

/*----------MessageBox ID------------------------------------------------------------------Language String------------------------------------------------------------*/
	{ MSGBOX_ERROR_CAPTION,									_T("错误") },
	{ MSGBOX_ERROR_FAILED,									_T("执行动作失败！") },
	{ MSGBOX_ERROR_WRONG_ARGUMENT, 							_T("函数参数错误！") },
	{ MSGBOX_ERROR_INVALID_FUNCTION, 						_T("功能不正确。") },
	{ MSGBOX_ERROR_FILE_NOT_FOUND, 							_T("系统找不到指定的文件。") },
	{ MSGBOX_ERROR_PATH_NOT_FOUND, 							_T("系统找不到指定的路径。") },
	{ MSGBOX_ERROR_ACCESS_DENIED, 							_T("拒绝访问。") },
	{ MSGBOX_ERROR_INVALID_HANDLE, 							_T("句柄无效。") },
	{ MSGBOX_ERROR_INVALID_DATA, 							_T("数据无效。") },
	{ MSGBOX_ERROR_NO_MORE_FILES, 							_T("没有其他文件。") },
	{ MSGBOX_ERROR_FILE_EXISTS, 							_T("文件已经存在。") },
	{ MSGBOX_ERROR_CANNOT_MAKE, 							_T("无法创建目录或文件。") },
	{ MSGBOX_ERROR_INVALID_PARAMETER, 						_T("参数不正确。") },
	{ MSGBOX_ERROR_OPEN_FAILED, 							_T("系统无法打开指定的设备或文件。") },
	{ MSGBOX_ERROR_BUFFER_OVERFLOW, 						_T("文件名太长。") },
	{ MSGBOX_ERROR_INVALID_NAME, 							_T("文件名、目录名或卷标语法不正确。") },
	{ MSGBOX_ERROR_DIR_NOT_EMPTY, 							_T("目录不是空的") },
	{ MSGBOX_ERROR_FAIL_SHUTDOWN, 							_T("关机操作失败。") },
	{ MSGBOX_ERROR_FAIL_RESTART, 							_T("重启操作失败。") },
	{ MSGBOX_ERROR_INVALID_ADDRESS, 						_T("尝试访问无效地址。") },
	{ MSGBOX_ERROR_APP_INIT_FAILURE, 						_T("应用程序无法正确启动。单击“确定”关闭该应用程序。") },
	{ MSGBOX_ERROR_CANNOT_LOAD_REGISTRY,					_T("无法加载注册表。注册表已损坏、缺失或不可写入。") },
	{ MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT, 					_T("系统已达到注册表系统部分允许的最大大小。额外的存储请求将被忽略。") },
	{ MSGBOX_ERROR_SYSTEM_SHUTDOWN,							_T("系统正在关闭。") },
	{ MSGBOX_ERROR_HIBERNATED, 								_T("系统进入休眠状态。") },
	{ MSGBOX_ERROR_RESUME_HIBERNATION, 						_T("系统已从休眠状态恢复。") },
	{ MSGBOX_ERROR_WAKE_SYSTEM, 							_T("系统已唤醒。") },
	{ MSGBOX_ERROR_BACKUP_REG_FAILED, 						_T("备份数据失败！") },
	{ MSGBOX_ERROR_LOAD_CFG_FAILED, 						_T("加载配置数据失败") },
	{ MSGBOX_ERROR_SAVE_CFG_FAILED, 						_T("保存配置数据失败") },
	{ MSGBOX_ERROR_LOAD_SCHED_FAILED, 						_T("加载计划数据失败") },
	{ MSGBOX_ERROR_SAVE_SCHED_FAILED, 						_T("保存计划数据失败") },
	{ MSGBOX_ERROR_LOAD_HKEYSET_FAILED, 					_T("加载热键集数据失败") },
	{ MSGBOX_ERROR_SAVE_HKEYSET_FAILED, 					_T("保存热键集数据失败") },
	{ MSGBOX_ERROR_LOAD_PWRRMD_FAILED, 						_T("加载提醒数据失败") },
	{ MSGBOX_ERROR_SAVE_PWRRMD_FAILED, 						_T("保存提醒数据失败") },
	{ MSGBOX_ERROR_WRITE_LOG_FAILED,						_T("写入日志失败") },
	{ MSGBOX_ERROR_OUTPUT_LOG_FAILED,						_T("输出日志失败") },
	{ MSGBOX_ERROR_UNKNOWN, 								_T("未知错误。") },

/*--------String ID---------------------------------------------------------------Language String---------------------------------------------------------------------*/
	{ FORMAT_TIME_BEFOREMIDDAY,								_T("午前") },
	{ FORMAT_TIME_AFTERMIDDAY,								_T("下午") },
	{ HKEYSET_EXISTED_ALT_F4,								_T("关闭活动窗口") },
	{ HKEYSET_EXISTED_ALT_F8,								_T("在登录屏幕中显示输入的密码") },
	{ HKEYSET_EXISTED_CTRL_F3,								_T("开始搜索") },
	{ HKEYSET_EXISTED_CTRL_F4,								_T("关闭活动文档") },
	{ HKEYSET_EXISTED_CTRL_F5,								_T("刷新当前窗口") },
	{ HKEYSET_EXISTED_CTRL_WIN_F4,							_T("关闭活动虚拟桌面") },

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
};

#endif // ifndef _LANGUAGE_H_INCLUDED
