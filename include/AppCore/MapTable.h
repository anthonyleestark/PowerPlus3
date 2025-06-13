/**
 * @file		MapTable.h
 * @brief		Define and implement data map table, macros and methods for map table processing
 * @author		AnthonyLeeStark
 * @date		2025.04.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore.h"
#include "Global.h"


namespace MapTable
{
	// ID pair entry
	struct IDPAIR
	{
		unsigned first;											// First pair ID
		unsigned second;										// Second pair ID
	};

	// ID map table
	using IDMAPTABLE = const IDPAIR[];
	using IDMAPTABLE_REF = const IDPAIR*;

	// Initialize ID map table
	#define INITIALIZE_IDMAPTABLE(tableName) \
		static IDMAPTABLE tableName = {
	#define END_IDMAPTABLE() \
		,INFINITE,INFINITE};

	// String map table
	using STRINGTABLE = const LANGTEXT[];
	using STRINGTABLE_REF = const LANGTEXT*;

	// Initialize string map table
	#define INITIALIZE_STRINGMAPTABLE(tableName) \
		static STRINGTABLE tableName = {
	#define END_STRINGMAPTABLE() \
		,INFINITE, _T("")};

	// Maximum table size
	#define MAX_TABLESIZE	1000

	// Get map table size
	#define TABLE_SIZE(tableName) \
		(INT(sizeof(tableName) / sizeof(tableName[0]) - 1))

	// Initialize template table
	#define INITIALIZE_TABLE(elementType, tableName) \
		static const elementType tableName[] = {
	#define END_TABLE() \
		};

	// Hotkey info description
	struct HOTKEYINFO
	{
		DWORD	dwModifiers;								// Modifier keys
		DWORD	dwVirtualKey;								// Virtual key code
		UINT	nHotkeyDescription;							// Hotkey description (string ID)
	};

	// Action definition/combination table
	struct ACTIONDEF
	{
		UINT nActionDefID;									// Action ID
		UINT nActionNameID;									// Action Name ID
		UINT nActionMsgID;									// Action message ID
		UINT nSchedNotifyID;								// Schedule notify message ID
		UINT nNotifyTipID;									// Notify file tip ID
		UINT nBalloonTipID;									// Balloon tip ID
	};

	
	// Static data tables for application
	namespace IDTable
	{
		// Using for pairing action macro IDs and action name IDs
		INITIALIZE_IDMAPTABLE(ActionName)
		/*------Action ID------------------------------Action Name ID------------------*/
			APP_ACTION_NOTHING,					ACTION_NAME_NOTHING,
			APP_ACTION_DISPLAYOFF,				ACTION_NAME_DISPLAYOFF,
			APP_ACTION_SLEEP,					ACTION_NAME_SLEEP,
			APP_ACTION_SHUTDOWN,				ACTION_NAME_SHUTDOWN,
			APP_ACTION_RESTART,					ACTION_NAME_RESTART,
			APP_ACTION_SIGNOUT,					ACTION_NAME_SIGNOUT,
			APP_ACTION_HIBERNATE,				ACTION_NAME_HIBERNATE,
			APP_ACTION_SHOWMENU,				ACTION_NAME_SHOWMENU
		/*-----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing action macro IDs and action message IDs
		INITIALIZE_IDMAPTABLE(ActionMessage)
		/*------Action ID-------------------------------Message String ID-------------*/
			APP_ACTION_DISPLAYOFF,				MESSAGE_ACTION_DISPLAYOFF,
			APP_ACTION_SLEEP,					MESSAGE_ACTION_SLEEP,
			APP_ACTION_SHUTDOWN,				MESSAGE_ACTION_SHUTDOWN,
			APP_ACTION_RESTART,					MESSAGE_ACTION_RESTART,
			APP_ACTION_SIGNOUT,					MESSAGE_ACTION_SIGNOUT,
			APP_ACTION_HIBERNATE,				MESSAGE_ACTION_HIBERNATE
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing action macro IDs and string IDs for notify icon tip text
		INITIALIZE_IDMAPTABLE(NotifyTip)
		/*------Action ID------------------------------Notify String ID---------------*/
			APP_ACTION_DISPLAYOFF,				NOTIFY_TIP_DISPLAYOFF,
			APP_ACTION_SLEEP,					NOTIFY_TIP_SLEEP,
			APP_ACTION_SHUTDOWN,				NOTIFY_TIP_SHUTDOWN,
			APP_ACTION_RESTART,					NOTIFY_TIP_RESTART,
			APP_ACTION_SIGNOUT,					NOTIFY_TIP_SIGNOUT,
			APP_ACTION_HIBERNATE,				NOTIFY_TIP_HIBERNATE,
			APP_ACTION_SHOWMENU,				NOTIFY_TIP_SHOWMENU
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		
		// Using for pairing action macro IDs and string IDs for notify icon balloon tip text
		INITIALIZE_IDMAPTABLE(BalloonTip)
		/*------Action ID------------------------------Balloon String ID--------------*/
			APP_ACTION_DISPLAYOFF,				BALLOON_TIP_DISPLAYOFF,
			APP_ACTION_SLEEP,					BALLOON_TIP_SLEEP,
			APP_ACTION_SHUTDOWN,				BALLOON_TIP_SHUTDOWN,
			APP_ACTION_RESTART,					BALLOON_TIP_RESTART,
			APP_ACTION_SIGNOUT,					BALLOON_TIP_SIGNOUT,
			APP_ACTION_HIBERNATE,				BALLOON_TIP_HIBERNATE
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		
		// Using for pairing application-defined error codes and system-defined error codes
		INITIALIZE_IDMAPTABLE(ErrorCode)
		/*------Action ID--------------------------------Action Name ID---------------*/
			APP_ERROR_SUCCESS,					ERROR_SUCCESS,
			APP_ERROR_INVALID_FUNCTION,			ERROR_INVALID_FUNCTION,
			APP_ERROR_FILE_NOT_FOUND,			ERROR_FILE_NOT_FOUND,
			APP_ERROR_PATH_NOT_FOUND,			ERROR_PATH_NOT_FOUND,
			APP_ERROR_ACCESS_DENIED,			ERROR_ACCESS_DENIED,
			APP_ERROR_INVALID_HANDLE,			ERROR_INVALID_HANDLE,
			APP_ERROR_INVALID_DATA,				ERROR_INVALID_DATA,
			APP_ERROR_NO_MORE_FILES,			ERROR_NO_MORE_FILES,
			APP_ERROR_FILE_EXISTS,				ERROR_FILE_EXISTS,
			APP_ERROR_CANNOT_MAKE,				ERROR_CANNOT_MAKE,
			APP_ERROR_INVALID_PARAMETER,		ERROR_INVALID_PARAMETER,
			APP_ERROR_OPEN_FAILED,				ERROR_OPEN_FAILED,
			APP_ERROR_BUFFER_OVERFLOW,			ERROR_BUFFER_OVERFLOW,
			APP_ERROR_INVALID_NAME,				ERROR_INVALID_NAME,
			APP_ERROR_DIR_NOT_EMPTY,			ERROR_DIR_NOT_EMPTY,
			APP_ERROR_FAIL_SHUTDOWN,			ERROR_FAIL_SHUTDOWN,
			APP_ERROR_FAIL_RESTART,				ERROR_FAIL_RESTART,
			APP_ERROR_INVALID_ADDRESS,			ERROR_INVALID_ADDRESS,
			APP_ERROR_APP_INIT_FAILURE,			ERROR_APP_INIT_FAILURE,
			APP_ERROR_CANNOT_LOAD_REGISTRY,		ERROR_CANNOT_LOAD_REGISTRY_FILE,
			APP_ERROR_REGISTRY_QUOTA_LIMIT,		ERROR_REGISTRY_QUOTA_LIMIT,
			APP_ERROR_SYSTEM_SHUTDOWN,			ERROR_SYSTEM_SHUTDOWN,
			APP_ERROR_HIBERNATED,				ERROR_HIBERNATED,
			APP_ERROR_RESUME_HIBERNATION,		ERROR_RESUME_HIBERNATION,
			APP_ERROR_WAKE_SYSTEM,				ERROR_WAKE_SYSTEM
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		
		// Using for pairing application-defined error codes and error message string IDs
		INITIALIZE_IDMAPTABLE(ErrorMessage)
		/*------Action ID-----------------------------Error message string ID---------*/
			APP_ERROR_SUCCESS,					INT_NULL,
			APP_ERROR_FAILED,					MSGBOX_ERROR_FAILED,
			APP_ERROR_WRONG_ARGUMENT,			MSGBOX_ERROR_WRONG_ARGUMENT,
			APP_ERROR_INVALID_FUNCTION,			MSGBOX_ERROR_INVALID_FUNCTION,
			APP_ERROR_FILE_NOT_FOUND,			MSGBOX_ERROR_FILE_NOT_FOUND,
			APP_ERROR_PATH_NOT_FOUND,			MSGBOX_ERROR_PATH_NOT_FOUND,
			APP_ERROR_ACCESS_DENIED,			MSGBOX_ERROR_ACCESS_DENIED,
			APP_ERROR_INVALID_HANDLE,			MSGBOX_ERROR_INVALID_HANDLE,
			APP_ERROR_INVALID_DATA,				MSGBOX_ERROR_INVALID_DATA,
			APP_ERROR_NO_MORE_FILES,			MSGBOX_ERROR_NO_MORE_FILES,
			APP_ERROR_FILE_EXISTS,				MSGBOX_ERROR_FILE_EXISTS,
			APP_ERROR_CANNOT_MAKE,				MSGBOX_ERROR_CANNOT_MAKE,
			APP_ERROR_INVALID_PARAMETER,		MSGBOX_ERROR_INVALID_PARAMETER,
			APP_ERROR_OPEN_FAILED,				MSGBOX_ERROR_OPEN_FAILED,
			APP_ERROR_BUFFER_OVERFLOW,			MSGBOX_ERROR_BUFFER_OVERFLOW,
			APP_ERROR_INVALID_NAME,				MSGBOX_ERROR_INVALID_NAME,
			APP_ERROR_DIR_NOT_EMPTY,			MSGBOX_ERROR_DIR_NOT_EMPTY,
			APP_ERROR_FAIL_SHUTDOWN,			MSGBOX_ERROR_FAIL_SHUTDOWN,
			APP_ERROR_FAIL_RESTART,				MSGBOX_ERROR_FAIL_RESTART,
			APP_ERROR_INVALID_ADDRESS,			MSGBOX_ERROR_INVALID_ADDRESS,
			APP_ERROR_APP_INIT_FAILURE,			MSGBOX_ERROR_APP_INIT_FAILURE,
			APP_ERROR_CANNOT_LOAD_REGISTRY,		MSGBOX_ERROR_CANNOT_LOAD_REGISTRY,
			APP_ERROR_REGISTRY_QUOTA_LIMIT,		MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT,
			APP_ERROR_SYSTEM_SHUTDOWN,			MSGBOX_ERROR_SYSTEM_SHUTDOWN,
			APP_ERROR_HIBERNATED,				MSGBOX_ERROR_HIBERNATED,
			APP_ERROR_RESUME_HIBERNATION,		MSGBOX_ERROR_RESUME_HIBERNATION,
			APP_ERROR_BACKUP_REG_FAILED,		MSGBOX_ERROR_BACKUP_REG_FAILED,
			APP_ERROR_LOAD_CFG_INVALID,			MSGBOX_ERROR_LOAD_CFG_FAILED,
			APP_ERROR_LOAD_CFG_FAILED,			MSGBOX_ERROR_LOAD_CFG_FAILED,
			APP_ERROR_SAVE_CFG_INVALID,			MSGBOX_ERROR_SAVE_CFG_FAILED,
			APP_ERROR_SAVE_CFG_FAILED,			MSGBOX_ERROR_SAVE_CFG_FAILED,
			APP_ERROR_LOAD_SCHED_INVALID,		MSGBOX_ERROR_LOAD_SCHED_FAILED,
			APP_ERROR_LOAD_SCHED_FAILED,		MSGBOX_ERROR_LOAD_SCHED_FAILED,
			APP_ERROR_SAVE_SCHED_INVALID,		MSGBOX_ERROR_SAVE_SCHED_FAILED,
			APP_ERROR_SAVE_SCHED_FAILED,		MSGBOX_ERROR_SAVE_SCHED_FAILED,
			APP_ERROR_LOAD_HKEYSET_INVALID,		MSGBOX_ERROR_LOAD_HKEYSET_FAILED,
			APP_ERROR_LOAD_HKEYSET_FAILED,		MSGBOX_ERROR_LOAD_HKEYSET_FAILED,
			APP_ERROR_SAVE_HKEYSET_INVALID,		MSGBOX_ERROR_SAVE_HKEYSET_FAILED,
			APP_ERROR_SAVE_HKEYSET_FAILED,		MSGBOX_ERROR_SAVE_HKEYSET_FAILED,
			APP_ERROR_LOAD_PWRRMD_INVALID,		MSGBOX_ERROR_LOAD_PWRRMD_FAILED,
			APP_ERROR_LOAD_PWRRMD_FAILED,		MSGBOX_ERROR_LOAD_PWRRMD_FAILED,
			APP_ERROR_SAVE_PWRRMD_INVALID,		MSGBOX_ERROR_SAVE_PWRRMD_FAILED,
			APP_ERROR_SAVE_PWRRMD_FAILED,		MSGBOX_ERROR_SAVE_PWRRMD_FAILED,
			APP_ERROR_WRITE_LOG_FAILED,			MSGBOX_ERROR_WRITE_LOG_FAILED,
			APP_ERROR_OUTPUT_LOG_FAILED,		MSGBOX_ERROR_OUTPUT_LOG_FAILED,
			APP_ERROR_UNKNOWN,					MSGBOX_ERROR_UNKNOWN
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing action macro IDs and message box string IDs for Notify Schedule function
		INITIALIZE_IDMAPTABLE(ScheduleNotifyMessage)
		/*------Action ID---------------------------------Message String ID-----------*/
			APP_ACTION_DISPLAYOFF,				MESSAGE_SCHEDNOTIFY_DISPLAYOFF,
			APP_ACTION_SLEEP,					MESSAGE_SCHEDNOTIFY_SLEEP,
			APP_ACTION_SHUTDOWN,				MESSAGE_SCHEDNOTIFY_SHUTDOWN,
			APP_ACTION_RESTART,					MESSAGE_SCHEDNOTIFY_RESTART,
			APP_ACTION_SIGNOUT,					MESSAGE_SCHEDNOTIFY_SIGNOUT,
			APP_ACTION_HIBERNATE,				MESSAGE_SCHEDNOTIFY_HIBERNATE
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing Hotkey ID and action macro IDs
		INITIALIZE_IDMAPTABLE(HKActionID)
		/*--HotKey Action ID-----------------------------Action ID--------------------*/
			HotkeyID::displayOff,				APP_ACTION_DISPLAYOFF,
			HotkeyID::sleep,					APP_ACTION_SLEEP,
			HotkeyID::shutdown,					APP_ACTION_SHUTDOWN,
			HotkeyID::restart,					APP_ACTION_RESTART,
			HotkeyID::signOut,					APP_ACTION_SIGNOUT,
			HotkeyID::hibernate,				APP_ACTION_HIBERNATE
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing Power Reminder event IDs and string IDs 
		// which will display in Power Reminder data table
		INITIALIZE_IDMAPTABLE(PwrReminderEvent)
		/*-----Event ID--------------------------------Event String ID----------------*/
			PwrReminderEvent::atSetTime,		PWRRMD_EVENT_AT_SETTIME,
			PwrReminderEvent::atAppStartup,		PWRRMD_EVENT_AT_APPSTARTUP,
			PwrReminderEvent::atSysWakeUp,		PWRRMD_EVENT_AT_SYSWAKEUP,
			PwrReminderEvent::beforePwrAction,	PWRRMD_EVENT_AT_BFRPWRACTION,
			PwrReminderEvent::wakeAfterAction,	PWRRMD_EVENT_AT_PWRACTIONWAKE,
			PwrReminderEvent::atAppExit,		PWRRMD_EVENT_AT_APPEXIT
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing Power Reminder style IDs and string IDs 
		// which will display in Power Reminder data table
		INITIALIZE_IDMAPTABLE(PwrReminderStyle)
		/*----Style ID---------------------------------Style String ID----------------*/
			PwrReminderStyle::messageBox,		PWRRMD_STYLE_MESSAGEBOX,
			PwrReminderStyle::dialogBox,		PWRRMD_STYLE_DIALOG
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()


		// Using for pairing day-of-week macro IDs and title IDs
		INITIALIZE_IDMAPTABLE(DayOfWeek)
		/*--Day ID-----------------------------------Day title string ID--------------*/
			DayOfWeek::Monday,					DAYOFWEEK_TITLE_MONDAY,
			DayOfWeek::Tuesday,					DAYOFWEEK_TITLE_TUESDAY,
			DayOfWeek::Wednesday,				DAYOFWEEK_TITLE_WEDNESDAY,
			DayOfWeek::Thursday,				DAYOFWEEK_TITLE_THURSDAY,
			DayOfWeek::Friday,					DAYOFWEEK_TITLE_FRIDAY,
			DayOfWeek::Saturday,				DAYOFWEEK_TITLE_SATURDAY,
			DayOfWeek::Sunday,					DAYOFWEEK_TITLE_SUNDAY
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()
	};

	namespace StringTable
	{
		// Using for pairing function key macros and key names
		INITIALIZE_STRINGMAPTABLE(FunctionKeys)
		/*----------------------------------------------------------------------------*/
			{ VK_F1,  _T("F1") },	{ VK_F2,  _T("F2") },	{ VK_F3,  _T("F3") },
			{ VK_F4,  _T("F4") },	{ VK_F5,  _T("F5") },	{ VK_F6,  _T("F6") },
			{ VK_F7,  _T("F7") },	{ VK_F8,  _T("F8") },	{ VK_F9,  _T("F9") },
			{ VK_F10, _T("F10") },	{ VK_F11, _T("F11") },	{ VK_F12, _T("F12") }
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()


		// Using for pairing color macro IDs and color names
		INITIALIZE_STRINGMAPTABLE(ColorName)
		/*-------Color ID----------------------------Color name-----------------------*/
			Color::Red,							_T("Red"),
			Color::Green,						_T("Green"),
			Color::Yellow,						_T("Yellow"),
			Color::Blue,						_T("Blue"),
			Color::White,						_T("White"),
			Color::Black,						_T("Black"),
			Color::Pink,						_T("Pink"),
			Color::Sakura_Pink,					_T("Sakura pink"),
			Color::Orange,						_T("Orange"),
			Color::Ultramarine_Blue,			_T("Ultramarine blue"),
			Color::Indigo,						_T("Indigo"),
			Color::Violet,						_T("Violet"),
			Color::Purple,						_T("Purple"),
			Color::Scarlet,						_T("Scarlet"),
			Color::Jade,						_T("Jade"),
			Color::Emerald,						_T("Emerald"),
			Color::Gray,						_T("Gray"),
			Color::Dark_Gray,					_T("Dark gray"),
			Color::Bright_Gray,					_T("Bright gray")
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()


		// Using for pairing message icon IDs and icon names
		INITIALIZE_STRINGMAPTABLE(MsgIconName)
		/*--------Icon ID---------------------------Icon name-------------------------*/
			SystemIcon::Application,			_T("App"),
			SystemIcon::Hand,					_T("Hand"),
			SystemIcon::Question,				_T("Question"),
			SystemIcon::Exclamation,			_T("Exclamation"),
			SystemIcon::Asterisk,				_T("Asterisk"),
			SystemIcon::Warning,				_T("Warning"),
			SystemIcon::Error,					_T("Error"),
			SystemIcon::Information,			_T("Information")
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()
	};

	namespace OtherTable
	{
		// Using to define list of existed system-defined keystrokes 
		INITIALIZE_TABLE(HOTKEYINFO, ExistedSysHotkeyList)
		/*----Control Key----------------Function Key-----Hotkey description----------*/
			MOD_ALT,						VK_F4,		HKEYSET_EXISTED_ALT_F4,
			MOD_ALT,						VK_F8,		HKEYSET_EXISTED_ALT_F8,
			MOD_CONTROL,					VK_F3,		HKEYSET_EXISTED_CTRL_F3,
			MOD_CONTROL,					VK_F4,		HKEYSET_EXISTED_CTRL_F4,
			MOD_CONTROL,					VK_F5,		HKEYSET_EXISTED_CTRL_F5,
			MOD_CONTROL | MOD_WIN,			VK_F4,		HKEYSET_EXISTED_CTRL_WIN_F4,
			INT_NULL,						INT_NULL,	INT_NULL,
		/*-----------------------------------------------------------------------------*/
		END_TABLE()
	};

	namespace StringTable
	{
		// Using for pairing log key IDs and output texts when writing log files
		INITIALIZE_STRINGMAPTABLE(LogKey)
		/*----Base log key ID--------------------Output Text-------------------------*/
			BaseLog::Time,						_T("time"),
			BaseLog::PID,						_T("pid"),
			BaseLog::LogCategory,				_T("category"),
			BaseLog::Description,				_T("description"),
			BaseLog::Details,					_T("details"),
			BaseLog::DetailNumeric,				_T("value"),
			BaseLog::DetailString,				_T("text"),
		/*---------------------------------------------------------------------------*/

		/*--Event log detail category ID---------Output Text-------------------------*/
			EventDetail::ResourceID,			_T("resourceid"),
			EventDetail::NameID,				_T("nameid"),
			EventDetail::DialogCaption,			_T("dialogcaption"),
			EventDetail::ControlCaption,		_T("ctrlcaption"),
			EventDetail::CheckState,			_T("checkstate"),
			EventDetail::Selection,				_T("selection"),
			EventDetail::DataValue,				_T("datavalue"),
			EventDetail::DataChangeState,		_T("datachange"),
			EventDetail::ContentID,				_T("contentid"),
			EventDetail::MessageText,			_T("messagetext"),
			EventDetail::EventError,			_T("errorcode"),
		/*---------------------------------------------------------------------------*/

		/*-Action history log detail ID----------Output Text-------------------------*/
			HistoryDetail::Category,			_T("category"),
			HistoryDetail::Action,				_T("action"),
			HistoryDetail::Keystrokes,			_T("keystrokes"),
			HistoryDetail::ItemID,				_T("itemid"),
			HistoryDetail::Message,				_T("message"),
			HistoryDetail::Result,				_T("result"),
			HistoryDetail::ActionError,			_T("errorcode")
		/*---------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()


		// Using for pairing log value IDs and output texts when writing log files
		INITIALIZE_STRINGMAPTABLE(LogValue)
		/*--Action history category ID-----------Output Text-------------------------*/
			HistoryCategory::PowerAction,		_T("Power Action"),
			HistoryCategory::ScheduleAction,	_T("Schedule"),
			HistoryCategory::HotkeySet,			_T("HotkeySet"),
			HistoryCategory::PowerReminder,		_T("Power Reminder"),
		/*---------------------------------------------------------------------------*/

		/*---Action history action ID------------Output Text-------------------------*/
			HistoryAction::DoNothing,			_T("Do nothing"),
			HistoryAction::DisplayOff,			_T("Turn off display"),
			HistoryAction::SleepMode,			_T("Sleep"),
			HistoryAction::Shutdown,			_T("Shutdown"),
			HistoryAction::Restart,				_T("Restart"),
			HistoryAction::SignOut,				_T("Sign out"),
			HistoryAction::Hibernate,			_T("Hibernate"),
		/*---------------------------------------------------------------------------*/

		/*---Action history result ID------------Output Text-------------------------*/
			HistoryResult::SuccessNoError,		_T("Success"),
			HistoryResult::FailedUnknown,		_T("Failed (Unknown error)"),
			HistoryResult::FailedWithErrorCode,	_T("Failed (With determined error)")
		/*---------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()
	};


	//	Define methods for processing data map tables
	UINT GetPairedID(IDMAPTABLE_REF pIDTableRef, UINT nID, BOOL bReverse = FALSE);
	UINT GetStringID(STRINGTABLE_REF pStringTableRef, const wchar_t* input);
	const wchar_t* GetString(STRINGTABLE_REF pStringTableRef, UINT nID);
};
