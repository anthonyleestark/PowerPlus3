
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		MapTable.h
//		Description:	Define and implement data map table, macros and methods for map table processing
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.04.03:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MAPTABLE_H_INCLUDED
#define	_MAPTABLE_H_INCLUDED

#include "stdafx.h"
#include "Core.h"

namespace MapTable
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Define data types and macros for data map table initialization and process
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// ID map table
	using IDMAPTABLE = const IDPAIR[];
	using IDMAPTABLE_REF = const IDPAIR*;

	// Initialize ID map table
	#define INITIALIZE_IDMAPTABLE(tableName) \
		static IDMAPTABLE tableName = {
	#define END_IDMAPTABLE() \
		,INT_NULL,INT_NULL};

	// String map table
	using STRINGTABLE = const LANGTEXT[];
	using STRINGTABLE_REF = const LANGTEXT*;

	// Initialize string map table
	#define INITIALIZE_STRINGMAPTABLE(tableName) \
		static STRINGTABLE tableName = {
	#define END_STRINGMAPTABLE() \
		, INT_NULL, STRING_EMPTY, };

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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Define global static data tables for program, these data will be used elsewhere in the program
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace IDTable
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ActionName
		//  Description:	Using for pairing action macro IDs and action name IDs
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ActionMessage
		//  Description:	Using for pairing action macro IDs and action message IDs
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		NotifyTip
		//  Description:	Using for pairing action macro IDs and string IDs for
		//					notify icon tip text
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		BalloonTip
		//  Description:	Using for pairing action macro IDs and string IDs for
		//					notify icon balloon tip text
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ErrorCode
		//  Description:	Using for pairing application-defined error codes and
		//					system-defined error codes
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ErrorMessage
		//  Description:	Using for pairing application-defined error codes and
		//					error message string IDs
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ScheduleNotifyMessage
		//  Description:	Using for pairing action macro IDs and message box
		//					string IDs for Notify Schedule function
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

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

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		HKActionID
		//  Description:	Using for pairing Hotkey ID and action macro IDs
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_IDMAPTABLE(HKActionID)
		/*--HotKey Action ID-----------------------------Action ID--------------------*/
			HKID_DISPLAYOFF,					APP_ACTION_DISPLAYOFF,
			HKID_SLEEP,							APP_ACTION_SLEEP,
			HKID_SHUTDOWN,						APP_ACTION_SHUTDOWN,
			HKID_RESTART,						APP_ACTION_RESTART,
			HKID_SIGNOUT,						APP_ACTION_SIGNOUT,
			HKID_HIBERNATE,						APP_ACTION_HIBERNATE
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		PwrReminderEvent
		//  Description:	Using for pairing Power Reminder event IDs and string IDs 
		//					which will display in Power Reminder data table
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_IDMAPTABLE(PwrReminderEvent)
		/*-----Event ID--------------------------------Event String ID----------------*/
			PREVT_AT_SETTIME,					PWRRMD_EVENT_AT_SETTIME,
			PREVT_AT_APPSTARTUP,				PWRRMD_EVENT_AT_APPSTARTUP,
			PREVT_AT_SYSWAKEUP,					PWRRMD_EVENT_AT_SYSWAKEUP,
			PREVT_AT_BFRPWRACTION,				PWRRMD_EVENT_AT_BFRPWRACTION,
			PREVT_AT_PWRACTIONWAKE,				PWRRMD_EVENT_AT_PWRACTIONWAKE,
			PREVT_AT_APPEXIT,					PWRRMD_EVENT_AT_APPEXIT
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		PwrReminderStyle
		//  Description:	Using for pairing Power Reminder style IDs and string IDs 
		//					which will display in Power Reminder data table
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_IDMAPTABLE(PwrReminderStyle)
		/*----Style ID---------------------------------Style String ID----------------*/
			PRSTYLE_MSGBOX,						PWRRMD_STYLE_MESSAGEBOX,
			PRSTYLE_DIALOG,						PWRRMD_STYLE_DIALOG
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		DayOfWeek
		//  Description:	Using for pairing day-of-week macro IDs and title IDs
		//  Table type:		IDMAPTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_IDMAPTABLE(DayOfWeek)
		/*--Day ID-----------------------------------Day title string ID--------------*/
			MONDAY,								DAYOFWEEK_TITLE_MONDAY,
			TUESDAY,							DAYOFWEEK_TITLE_TUESDAY,
			WEDNESDAY,							DAYOFWEEK_TITLE_WEDNESDAY,
			THURSDAY,							DAYOFWEEK_TITLE_THURSDAY,
			FRIDAY,								DAYOFWEEK_TITLE_FRIDAY,
			SATURDAY,							DAYOFWEEK_TITLE_SATURDAY,
			SUNDAY,								DAYOFWEEK_TITLE_SUNDAY
		/*----------------------------------------------------------------------------*/
		END_IDMAPTABLE()
	};

	namespace StringTable
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		FuncKeyList
		//  Description:	Using for pairing function key macros and key names
		//  Table type:		STRINGTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_STRINGMAPTABLE(FuncKeyList)
		/*----------------------------------------------------------------------------*/
			{ VK_F1,  _T("F1") },	{ VK_F2,  _T("F2") },	{ VK_F3,  _T("F3") },
			{ VK_F4,  _T("F4") },	{ VK_F5,  _T("F5") },	{ VK_F6,  _T("F6") },
			{ VK_F7,  _T("F7") },	{ VK_F8,  _T("F8") },	{ VK_F9,  _T("F9") },
			{ VK_F10, _T("F10") },	{ VK_F11, _T("F11") },	{ VK_F12, _T("F12") }
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()

		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ColorName
		//  Description:	Using for pairing color macro IDs and color names
		//  Table type:		STRINGTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_STRINGMAPTABLE(ColorName)
		/*-------Color ID----------------------------Color name-----------------------*/
			COLOR_RED,							_T("Red"),
			COLOR_GREEN,						_T("Green"),
			COLOR_YELLOW,						_T("Yellow"),
			COLOR_BLUE,							_T("Blue"),
			COLOR_WHITE,						_T("White"),
			COLOR_BLACK,						_T("Black"),
			COLOR_PINK,							_T("Pink"),
			COLOR_SAKURA_PINK,					_T("Sakura pink"),
			COLOR_ORANGE,						_T("Orange"),
			COLOR_UMARINE_BLUE,					_T("Ultramarine blue"),
			COLOR_INDIGO,						_T("Indigo"),
			COLOR_VIOLET,						_T("Violet"),
			COLOR_PURPLE,						_T("Purple"),
			COLOR_SCARLET,						_T("Scarlet"),
			COLOR_JADE,							_T("Jade"),
			COLOR_EMERALD,						_T("Emerald"),
			COLOR_GRAY,							_T("Gray"),
			COLOR_DARK_GRAY,					_T("Dark gray"),
			COLOR_BRIGHT_GRAY,					_T("Bright gray")
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()


		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		MsgIconName
		//  Description:	Using for pairing message icon IDs and icon names
		//  Table type:		STRINGTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_STRINGMAPTABLE(MsgIconName)
		/*--------Icon ID---------------------------Icon name-------------------------*/
			IDI_MSGICON_APPLICATION,			_T("App"),
			IDI_MSGICON_HAND,					_T("Hand"),
			IDI_MSGICON_QUESTION,				_T("Question"),
			IDI_MSGICON_EXCLAMATION,			_T("Exclamation"),
			IDI_MSGICON_ASTERISK,				_T("Asterisk"),
			IDI_MSGICON_WARNING,				_T("Warning"),
			IDI_MSGICON_ERROR,					_T("Error"),
			IDI_MSGICON_INFORMATION,			_T("Information")
		/*----------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()
	};

	namespace OtherTable
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		ExistedSysHotkeyList
		//  Description:	Using to define list of existed system-defined keystrokes 
		//  Table type:		HOTKEYINFO Table
		//
		//////////////////////////////////////////////////////////////////////////

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


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Define static data tables for logging, these data will be used for logging feature only
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	namespace StringTable
	{
		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		LogInfoIDTitle
		//  Description:	Using for pairing logging info IDs and the titles when
		//					writing log files
		//  Table type:		STRINGTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_STRINGMAPTABLE(LogInfoIDTitle)
		/*----Base log info IDs------------------------Log Info Titles---------------*/
			BASELOG_INFO_TIME,					_T("time"),
			BASELOG_INFO_PID,					_T("pid"),
			BASELOG_INFO_CATEGORY,				_T("category"),
			BASELOG_INFO_DESCRIPTION,			_T("description"),
			BASELOG_INFO_DETAILS,				_T("details"),
		/*---------------------------------------------------------------------------*/

		/*----Base log info IDs------------------------Log Info Titles---------------*/
			EVENTLOG_DETAIL_RESOURCEID,			_T("resourceid"),
			EVENTLOG_DETAIL_NAMEID,				_T("nameid"),
			EVENTLOG_DETAIL_DIALOGCAPTION,		_T("dialogcaption"),
			EVENTLOG_DETAIL_CTRLCAPTION,		_T("ctrlcaption"),
			EVENTLOG_DETAIL_CHKSTATE,			_T("checkstate"),
			EVENTLOG_DETAIL_SELECTION,			_T("selection"),
			EVENTLOG_DETAIL_DATAVALUE,			_T("datavalue"),
			EVENTLOG_DETAIL_DATACHANGE,			_T("datachange"),
			EVENTLOG_DETAIL_CONTENTID,			_T("contentid"),
			EVENTLOG_DETAIL_MESSAGETEXT,		_T("messagetext"),
			EVENTLOG_DETAIL_ERRORCODE,			_T("errorcode"),
		/*---------------------------------------------------------------------------*/

		/*----History log info IDs---------------------Log Info Titles---------------*/
			HISTORYLOG_DETAIL_CATEGORY,			_T("category"),
			HISTORYLOG_DETAIL_ACTION,			_T("action"),
			HISTORYLOG_DETAIL_KEYSTROKES,		_T("keystrokes"),
			HISTORYLOG_DETAIL_ITEMID,			_T("itemid"),
			HISTORYLOG_DETAIL_MESSAGE,			_T("message"),
			HISTORYLOG_DETAIL_RESULT,			_T("result"),
			HISTORYLOG_DETAIL_ERRORCODE,		_T("errorcode")
		/*---------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()


		//////////////////////////////////////////////////////////////////////////
		//
		//	Table name:		HistoryLogDetail
		//  Description:	Using for pairing history log detail info IDs and the
		//					titles when writing log files
		//  Table type:		STRINGTABLE
		//
		//////////////////////////////////////////////////////////////////////////

		INITIALIZE_STRINGMAPTABLE(HistoryLogDetail)
		/*---History category IDs-----------------------Log Info Titles--------------*/
			HISTORYLOG_CATE_PWRACTION,			_T("Power Action"),
			HISTORYLOG_CATE_SCHEDULE,			_T("Schedule"),
			HISTORYLOG_CATE_HOTKEYSET,			_T("HotkeySet"),
			HISTORYLOG_CATE_PWRREMINDER,		_T("Power Reminder"),
		/*---------------------------------------------------------------------------*/

		/*---History log action IDs---------------------Log Info Titles--------------*/
			HISTORYLOG_ACTION_NOTHING,			_T("Do nothing"),
			HISTORYLOG_ACTION_DISPLAYOFF,		_T("Turn off display"),
			HISTORYLOG_ACTION_SLEEP,			_T("Sleep"),
			HISTORYLOG_ACTION_SHUTDOWN,			_T("Shutdown"),
			HISTORYLOG_ACTION_RESTART,			_T("Restart"),
			HISTORYLOG_ACTION_SIGNOUT,			_T("Sign out"),
			HISTORYLOG_ACTION_HIBERNATE,		_T("Hibernate"),
		/*---------------------------------------------------------------------------*/

		/*---History log action IDs---------------------Log Info Titles--------------*/
			HISTORYLOG_RESULT_SUCCESS,			_T("Success"),
			HISTORYLOG_RESULT_FAILED_UNKNOWN,	_T("Failed (Unknown error)"),
			HISTORYLOG_RESULT_FAILED_ERRCODE,	_T("Failed (Error code: 0x%08X)")
		/*---------------------------------------------------------------------------*/
		END_STRINGMAPTABLE()
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Define methods for processing data map tables
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UINT	GetPairedID(IDMAPTABLE_REF pIDTableRef, UINT nID, BOOL bReverse = FALSE);
	UINT	GetStringID(STRINGTABLE_REF pStringTableRef, LPCTSTR lpszInput);
	LPCTSTR	GetString(STRINGTABLE_REF pStringTableRef, UINT nID, LPTSTR pszResult = NULL);
};

#endif		// ifndef _MAPTABLE_H_INCLUDED
