
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Core.h
//		Description:	Define and implement core data types, macros and methods which will be used elsewhere in program
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

#ifndef _CORE_H_INCLUDED
#define	_CORE_H_INCLUDED

#include "stdafx.h"

#include "Global.h"
#include "Language.h"
#include "GridCtrl.h"
#include "GridCellCheck.h"

/////////////////////////////////////

//// Declarations

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define macros for program, these macros will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Define languages
#define APP_LANGUAGE							0x01
#define APP_LANGUAGE_ENGLISH					((APP_LANGUAGE << 8) + 0x01)
#define APP_LANGUAGE_VIETNAMESE					((APP_LANGUAGE << 8) + 0x02)
#define APP_LANGUAGE_SIMPCHINESE				((APP_LANGUAGE << 8) + 0x03)

// Define app actions
#define DEF_APP_ACTION							0x02
#define DEF_APP_ACTION_NOTHING					((DEF_APP_ACTION << 8) + 0x00)
#define DEF_APP_ACTION_DISPLAYOFF				((DEF_APP_ACTION << 8) + 0x01)
#define DEF_APP_ACTION_SLEEP					((DEF_APP_ACTION << 8) + 0x02)
#define DEF_APP_ACTION_SHUTDOWN					((DEF_APP_ACTION << 8) + 0x03)
#define DEF_APP_ACTION_RESTART					((DEF_APP_ACTION << 8) + 0x04)
#define DEF_APP_ACTION_SIGNOUT					((DEF_APP_ACTION << 8) + 0x05)
#define DEF_APP_ACTION_HIBERNATE				((DEF_APP_ACTION << 8) + 0x06)
#define DEF_APP_ACTION_SHOWMENU					((DEF_APP_ACTION << 8) + 0x07)
#define DEF_APP_ACTION_SHOWWINDOW				((DEF_APP_ACTION << 8) + 0x08)

// Define app action messages
#define DEF_APP_MESSAGE							0x03
#define DEF_APP_MESSAGE_DISPLAYOFF				SC_MONITORPOWER
#define DEF_APP_MESSAGE_SLEEP					((DEF_APP_MESSAGE << 8) + 0x01)
#define DEF_APP_MESSAGE_HIBERNATE				((DEF_APP_MESSAGE << 8) + 0x02)
#define DEF_APP_MESSAGE_SHUTDOWN				(EWX_SHUTDOWN | EWX_HYBRID_SHUTDOWN)
#define DEF_APP_MESSAGE_POWEROFF				EWX_POWEROFF
#define DEF_APP_MESSAGE_REBOOT					EWX_REBOOT
#define DEF_APP_MESSAGE_SIGNOUT					EWX_LOGOFF

// Define action type constants
#define DEF_APP_ACTIONTYPE						0x04
#define DEF_APP_ACTIONTYPE_MONITOR				((DEF_APP_ACTIONTYPE << 8) + 0x01)
#define DEF_APP_ACTIONTYPE_POWER				((DEF_APP_ACTIONTYPE << 8) + 0x02)

// Define app error codes
#define DEF_APP_ERROR							0x05
#define DEF_APP_ERROR_SUCCESS					((DEF_APP_ERROR << 8) + 0xE000)
#define DEF_APP_ERROR_FAILED					((DEF_APP_ERROR << 8) + 0xE001)
#define DEF_APP_ERROR_WRONG_ARGUMENT			((DEF_APP_ERROR << 8) + 0xE002)
#define DEF_APP_ERROR_INVALID_FUNCTION			((DEF_APP_ERROR << 8) + 0xE003)
#define DEF_APP_ERROR_FILE_NOT_FOUND			((DEF_APP_ERROR << 8) + 0xE004)
#define DEF_APP_ERROR_PATH_NOT_FOUND			((DEF_APP_ERROR << 8) + 0xE005)
#define DEF_APP_ERROR_ACCESS_DENIED				((DEF_APP_ERROR << 8) + 0xE006)
#define DEF_APP_ERROR_INVALID_HANDLE			((DEF_APP_ERROR << 8) + 0xE007)
#define DEF_APP_ERROR_INVALID_DATA				((DEF_APP_ERROR << 8) + 0xE008)
#define DEF_APP_ERROR_NO_MORE_FILES				((DEF_APP_ERROR << 8) + 0xE009)
#define DEF_APP_ERROR_FILE_EXISTS				((DEF_APP_ERROR << 8) + 0xE00A)
#define DEF_APP_ERROR_CANNOT_MAKE				((DEF_APP_ERROR << 8) + 0xE00B)
#define DEF_APP_ERROR_INVALID_PARAMETER			((DEF_APP_ERROR << 8) + 0xE00C)
#define DEF_APP_ERROR_OPEN_FAILED				((DEF_APP_ERROR << 8) + 0xE00D)
#define DEF_APP_ERROR_BUFFER_OVERFLOW			((DEF_APP_ERROR << 8) + 0xE00E)
#define DEF_APP_ERROR_INVALID_NAME				((DEF_APP_ERROR << 8) + 0xE00F)
#define DEF_APP_ERROR_DIR_NOT_EMPTY				((DEF_APP_ERROR << 8) + 0xE010)
#define DEF_APP_ERROR_FAIL_SHUTDOWN				((DEF_APP_ERROR << 8) + 0xE011)
#define DEF_APP_ERROR_FAIL_RESTART				((DEF_APP_ERROR << 8) + 0xE012)
#define DEF_APP_ERROR_INVALID_ADDRESS			((DEF_APP_ERROR << 8) + 0xE013)
#define DEF_APP_ERROR_APP_INIT_FAILURE			((DEF_APP_ERROR << 8) + 0xE014)
#define DEF_APP_ERROR_CANNOT_LOAD_REGISTRY		((DEF_APP_ERROR << 8) + 0xE015)
#define DEF_APP_ERROR_REGISTRY_QUOTA_LIMIT		((DEF_APP_ERROR << 8) + 0xE016)
#define DEF_APP_ERROR_SYSTEM_SHUTDOWN			((DEF_APP_ERROR << 8) + 0xE017)
#define DEF_APP_ERROR_HIBERNATED				((DEF_APP_ERROR << 8) + 0xE018)
#define DEF_APP_ERROR_RESUME_HIBERNATION		((DEF_APP_ERROR << 8) + 0xE019)
#define DEF_APP_ERROR_WAKE_SYSTEM				((DEF_APP_ERROR << 8) + 0xE01A)
#define DEF_APP_ERROR_BACKUP_REG_FAILED			((DEF_APP_ERROR << 8) + 0xE01B)
#define DEF_APP_ERROR_LOAD_CFG_INVALID			((DEF_APP_ERROR << 8) + 0xE01C)
#define DEF_APP_ERROR_LOAD_CFG_FAILED			((DEF_APP_ERROR << 8) + 0xE01D)
#define DEF_APP_ERROR_LOAD_SCHED_INVALID		((DEF_APP_ERROR << 8) + 0xE01E)
#define DEF_APP_ERROR_LOAD_SCHED_FAILED			((DEF_APP_ERROR << 8) + 0xE01F)
#define DEF_APP_ERROR_LOAD_HKEYSET_INVALID		((DEF_APP_ERROR << 8) + 0xE020)
#define DEF_APP_ERROR_LOAD_HKEYSET_FAILED		((DEF_APP_ERROR << 8) + 0xE021)
#define DEF_APP_ERROR_LOAD_PWRRMD_INVALID		((DEF_APP_ERROR << 8) + 0xE022)
#define DEF_APP_ERROR_LOAD_PWRRMD_FAILED		((DEF_APP_ERROR << 8) + 0xE023)
#define DEF_APP_ERROR_SAVE_CFG_INVALID			((DEF_APP_ERROR << 8) + 0xE024)
#define DEF_APP_ERROR_SAVE_CFG_FAILED			((DEF_APP_ERROR << 8) + 0xE025)
#define DEF_APP_ERROR_SAVE_SCHED_INVALID		((DEF_APP_ERROR << 8) + 0xE026)
#define DEF_APP_ERROR_SAVE_SCHED_FAILED			((DEF_APP_ERROR << 8) + 0xE027)
#define DEF_APP_ERROR_SAVE_HKEYSET_INVALID		((DEF_APP_ERROR << 8) + 0xE028)
#define DEF_APP_ERROR_SAVE_HKEYSET_FAILED		((DEF_APP_ERROR << 8) + 0xE029)
#define DEF_APP_ERROR_SAVE_PWRRMD_INVALID		((DEF_APP_ERROR << 8) + 0xE02A)
#define DEF_APP_ERROR_SAVE_PWRRMD_FAILED		((DEF_APP_ERROR << 8) + 0xE02B)
#define DEF_APP_ERROR_WRITE_LOG_FAILED			((DEF_APP_ERROR << 8) + 0xE02C)
#define DEF_APP_ERROR_OUTPUT_LOG_FAILED			((DEF_APP_ERROR << 8) + 0xE02D)
#define DEF_APP_ERROR_FUNC_GETAPP_FAILED		((DEF_APP_ERROR << 8) + 0xE02E)
#define DEF_APP_ERROR_ALLOC_FAILED_POINTER		((DEF_APP_ERROR << 8) + 0xE02F)
#define DEF_APP_ERROR_ALLOC_FAILED_BUFFER		((DEF_APP_ERROR << 8) + 0xE030)
#define DEF_APP_ERROR_DEBUG_COMMAND				((DEF_APP_ERROR << 8) + 0xE031)
#define DEF_APP_ERROR_UNKNOWN					((DEF_APP_ERROR << 8) + 0xE0FF)

// Define debug command error codes
#define DEF_DBG_ERROR_SUCCESS					((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x00)
#define DEF_DBG_ERROR_EMPTY						((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x01)
#define DEF_DBG_ERROR_GETAPP_FAILED				((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x02)
#define DEF_DBG_ERROR_CPYBUFF_ALLOC_FAILED		((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x03)
#define DEF_DBG_ERROR_RETBUFF_ALLOC_FAILED		((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x04)
#define DEF_DBG_ERROR_TOKENIZATION_FAILED		((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x05)
#define DEF_DBG_ERROR_INVALID_COMMAND			((DEF_APP_ERROR_DEBUG_COMMAND << 8) + 0x06)

// Define function results
#define DEF_RESULT_SUCCESS						0x0000
#define DEF_RESULT_FAILED						0xFFFF

// Define menu item types
#define MENU_ITEM								0x06
#define MENU_ITEM_OPTION						((MENU_ITEM << 8) + 0x0001)
#define MENU_ITEM_SEPERATOR_ITEM				((MENU_ITEM << 8) + 0x0002)
#define	MENU_ITEM_POPUP_CHILD_MENU				((MENU_ITEM << 8) + 0x0003)

// Define menu actions
#define MENU_ACTION								0x07
#define MENU_ACTION_HELP						((MENU_ACTION << 8) + 0x01)
#define MENU_ACTION_ABOUT						((MENU_ACTION << 8) + 0x02)
#define MENU_ACTION_SHOW_WINDOW					((MENU_ACTION << 8) + 0x03)
#define MENU_ACTION_EXIT						((MENU_ACTION << 8) + 0x04)
#define MENU_ACTION_SELECT_VIETNAMESE			((MENU_ACTION << 8) + 0x05)
#define MENU_ACTION_SELECT_ENGLISH				((MENU_ACTION << 8) + 0x06)
#define MENU_ACTION_VIEW_LOG					((MENU_ACTION << 8) + 0x07)
#define MENU_ACTION_DEFAULT						((MENU_ACTION << 8) + 0x08)
#define MENU_ACTION_ADJUST_MENU					((MENU_ACTION << 8) + 0x09)
#define MENU_ACTION_SHOW_WHEN_START				((MENU_ACTION << 8) + 0x0A)
#define MENU_ACTION_STARTUP						((MENU_ACTION << 8) + 0x0B)
#define MENU_ACTION_ASK_BEFORE					((MENU_ACTION << 8) + 0x0C)
#define MENU_ACTION_SAVE_LOGS					((MENU_ACTION << 8) + 0x0D)
#define MENU_ACTION_SHOW_ERROR_MSG				((MENU_ACTION << 8) + 0x0E)

// Define types of sound
#define DEF_APP_SOUND							0x08
#define DEF_APP_SOUND_ERROR						((DEF_APP_SOUND << 8) + 0x0001)
#define DEF_APP_SOUND_SUCCESS					((DEF_APP_SOUND << 8) + 0x0002)

// Macros definition
#define DEF_APP_MACRO							0x09
#define DEF_APP_MACRO_LEFT_MOUSE				((DEF_APP_MACRO << 8) + 0x0001)
#define DEF_APP_MACRO_MIDDLE_MOUSE				((DEF_APP_MACRO << 8) + 0x0002)
#define DEF_APP_MACRO_RIGHT_MOUSE				((DEF_APP_MACRO << 8) + 0x0003)
#define DEF_APP_MACRO_ACTION_MENU				((DEF_APP_MACRO << 8) + 0x0004)
#define DEF_APP_MACRO_ACTION_SCHEDULE			((DEF_APP_MACRO << 8) + 0x0005)
#define DEF_APP_MACRO_ACTION_HOTKEY				((DEF_APP_MACRO << 8) + 0x0006)

// Define registry informations
#define REG_DEFINE_NONE							0x0a
#define REG_HKEY_CURRENT_USER					HKEY_CURRENT_USER
#define REG_HKEY_LOCAL_MACHINE					HKEY_LOCAL_MACHINE
#define REG_TYPE								((REG_DEFINE_NONE << 4) + 0x001)
#define REG_VALUE_STRING						((REG_TYPE << 4) + 0x0001)
#define REG_VALUE_DWORD							((REG_TYPE << 4) + 0x0002)

// Define custom user-defined messages
#define SM_APP_MESSAGE							(WM_USER + 100)
#define SM_WND_MESSAGE							(WM_USER + 200)

// Define application custom messages
#define SM_APP_TRAYICON							(SM_APP_MESSAGE + 1)
#define SM_APP_UPDATE_CONFIGDATA				(SM_APP_MESSAGE + 2)
#define SM_APP_UPDATE_SCHEDULEDATA				(SM_APP_MESSAGE + 3)
#define SM_APP_UPDATE_HOTKEYSETDATA				(SM_APP_MESSAGE + 4)
#define SM_APP_UPDATE_PWRREMINDERDATA			(SM_APP_MESSAGE + 5)
#define SM_APP_ERROR_MESSAGE					(SM_APP_MESSAGE + 6)
#define SM_APP_SHOW_ERROR_MSG					(SM_APP_MESSAGE + 7)
#define SM_APP_DEBUGCOMMAND						(SM_APP_MESSAGE + 8)
#define SM_APP_DEBUGCMDEXEC						(SM_APP_MESSAGE + 9)
#define SM_APP_DEBUGOUTPUT						(SM_APP_MESSAGE + 10)

// Define window custom messages
#define SM_WND_SHOWDIALOG						(SM_WND_MESSAGE + 1)
#define SM_WND_DEBUGTEST						(SM_WND_MESSAGE + 2)
#define SM_WND_DEBUGVIEWCLRSCR					(SM_WND_MESSAGE + 3)
#define SM_WND_DEBUGOUTPUTDISP					(SM_WND_MESSAGE + 4)

// Define app data types
#define APPDATA_CONFIG							0x00001001L
#define APPDATA_SCHEDULE						0x00001002L
#define APPDATA_HOTKEYSET						0x00001004L
#define APPDATA_PWRREMINDER						0x00001008L
#define APPDATA_ALL								(APPDATA_CONFIG|APPDATA_SCHEDULE|APPDATA_HOTKEYSET|APPDATA_PWRREMINDER)

// Define modes
#define DEF_MODE_INIT							0x000					// Mode: Initialize
#define DEF_MODE_VIEW							0x001					// Mode: View
#define DEF_MODE_ADD							0x002					// Mode: Add
#define DEF_MODE_UPDATE							0x003					// Mode: Update
#define DEF_MODE_LOAD							0x004					// Mode: Load
#define DEF_MODE_SAVE							0x005					// Mode: Save
#define DEF_MODE_DISABLE						0xFFF					// Mode: Disable
#define DEF_MODE_HELPVIEW_HELPFILE				0x0a1					// Mode: View help file
#define DEF_MODE_HELPVIEW_CHANGELOG				0x0a2					// Mode: View changelog file
#define DEF_MODE_BAKREGDATA_MANUAL				0x0a3					// Mode: Registry manually backup
#define DEF_MODE_BAKREGDATA_AUTO				0x0a4					// Mode: Registry auto backup
#define DEF_MODE_OPENDLG_MODAL					0x0a5					// Modal dialog
#define DEF_MODE_OPENDLG_MODELESS				0x0a6					// Modeless dialog

// Define colors
#define DEF_COLOR_RED							RGB(255,0,0)			// Color: Red
#define DEF_COLOR_GREEN							RGB(0,255,0)			// Color: Green
#define DEF_COLOR_YELLOW						RGB(255,255,0)			// Color: Yellow
#define DEF_COLOR_BLUE							RGB(0,0,255)			// Color: Blue
#define DEF_COLOR_WHITE							RGB(255,255,255)		// Color: White
#define DEF_COLOR_BLACK							RGB(0,0,0)				// Color: Black
#define DEF_COLOR_PINK							RGB(230,190,200)		// Color: Pink
#define DEF_COLOR_SAKURA_PINK					RGB(250,224,216)		// Color: Sakura pink
#define DEF_COLOR_ORANGE						RGB(255,127,40)			// Color: Orange
#define DEF_COLOR_UMARINE_BLUE					RGB(63,72,204)			// Color: Ultramarine blue
#define DEF_COLOR_INDIGO						RGB(75,0,130)			// Color: Indigo
#define DEF_COLOR_VIOLET						RGB(238,130,238)		// Color: Violet
#define DEF_COLOR_PURPLE						RGB(115,43,245)			// Color: Purple
#define DEF_COLOR_SCARLET						RGB(255,36,0)			// Color: Scarlet
#define DEF_COLOR_JADE							RGB(0,168,207)			// Color: Jade
#define DEF_COLOR_EMERALD						RGB(80,200,120)			// Color: Emerald
#define DEF_COLOR_GRAY							RGB(200,200,200)		// Color: Gray
#define DEF_COLOR_DARK_GRAY						RGB(160,160,160)		// Color: Dark gray
#define DEF_COLOR_BRIGHT_GRAY					RGB(240,240,240)		// Color: Bright gray

// Define log types
#define LOGTYPE_NONE							0x00					// Not defined
#define LOGTYPE_APP_EVENT						(LOGTYPE_NONE + 0x01)	// App event log
#define LOGTYPE_HISTORY_LOG						(LOGTYPE_NONE + 0x02)	// Action log/history
#define LOGTYPE_TRACE_ERROR						(LOGTYPE_NONE + 0x03)	// Trace error log
#define LOGTYPE_TRACE_DEBUG						(LOGTYPE_NONE + 0x04)	// Trace debug log
#define LOGTYPE_DEBUG_INFO						(LOGTYPE_NONE + 0x05)	// Debug info output log

// Define subdirectory names
#define SUBFOLDER_LOG							_T(".\\Log")
#define SUBFOLDER_HELP							_T(".\\Help")
#define SUBFOLDER_BACKUP						_T(".\\Backup")
#define SUBFOLDER_TEMP							_T(".\\Temp")

// Define file names
#define FILENAME_APPEXEFILE						_T("PowerPlus3")
#define FILENAME_APPCONFIG						_T("Config")
#define FILENAME_BAKCONFIG						_T("BakConfig")
#define FILENAME_APPEVENT_LOG					_T("AppEventLog_%d%02d%02d")
#define FILENAME_HISTORY_LOG					_T("AppHistory")
#define FILENAME_TRACE_ERROR_LOG				_T("TraceError")
#define FILENAME_TRACE_DEBUG_LOG				_T("TraceDebug")
#define FILENAME_DEBUG_INFO_LOG					_T("DebugInfo")
#define FILENAME_HELP_ENG						_T("English")
#define FILENAME_HELP_VIE						_T("Vietnamese")
#define FILENAME_HELP_CHS						_T("Chinese")
#define FILENAME_CHANGELOG_ENG					_T("Change_log.en")
#define FILENAME_CHANGELOG_VIE					_T("Change_log.vi")
#define FILENAME_CHANGELOG_CHS					_T("Change_log.ch")

// Define file extensions
#define FILEEXT_EXEFILE							_T(".exe")					// EXE file
#define FILEEXT_INIFILE							_T(".ini")					// INI file
#define FILEEXT_REGFILE							_T(".reg")					// Registry file
#define FILEEXT_LOGFILE							_T(".log")					// Log file
#define FILEEXT_BAKFILE							_T(".bak")					// Backup file extension
#define FILEEXT_BAKLOGFILE						_T("_%02d.log.bak")			// Backup log file extension
#define FILEEXT_HELPFILE						_T(".hlps")					// Help file

// Define timer IDs
#define TIMERID_DEFAULT							0x0100
#define TIMERID_STD_ACTIONSCHEDULE				(TIMERID_DEFAULT + 1)
#define TIMERID_STD_POWERREMINDER				(TIMERID_DEFAULT + 2)
#define TIMERID_STD_EVENTSKIPCOUNTER			(TIMERID_DEFAULT + 3)
#define TIMERID_RMDMSG_AUTOCLOSE				(TIMERID_DEFAULT + 4)

// Other special definitions
#define REG_AFX_PROJECTNAME						_T("PowerPlus3")
#define REG_STARTUP_VALUENAME					REG_AFX_PROJECTNAME
#define APP_NOTEPAD_PATH						_T("C:\\Windows\\notepad.exe")
#define APP_SYSTEMCMD_PATH						_T("C:\\Windows\\System32\\cmd.exe")

// Define macros
#define TRCFMT									TraceLogFormat
#define TRCLOG(logString)						TraceLog(logString)
#define TRCDBG(func,file,line)					TraceDebugInfo(func,file,line)
#define MAKEANSI(string)						CW2A(string).m_psz
#define MAKEUNICODE(string)						CA2W(string).m_psz
#define RESOURCESTRING(resourceid)				LoadResourceString(resourceid)
#define __FILENAME__							(strrchr("\\" __FILE__, '\\') + 1)
#define FORMAT_REG_TIME(systemtime)				(INT((systemtime.wHour * 100) + systemtime.wMinute))
#define GET_REGTIME_HOUR(time)					(WORD(time / 100))
#define GET_REGTIME_MINUTE(time)				(WORD(time % 100))

// Define properties for RepeatSet data
#define DEF_REPEATSET_MIN_SNOOZE				60							// Min snooze interval: 1 minutes
#define DEF_REPEATSET_DEFAULT_SNOOZE			600							// Default snooze interval: 10 minutes
#define DEF_REPEATSET_MAX_SNOOZE				1800						// Max snooze interval: 30 minutes
#define DEF_REPEATSET_DEFAULT_ACTIVEDAYS		0b01111111					// Default repeat: All days of weekss

// Define properties for Action Schedule function
#define DEF_SCHEDULE_DEFAULT_ITEMNUM			1							// Default item number: 1
#define DEF_SCHEDULE_MAX_ITEMNUM				100							// Max item number: 100
#define DEF_SCHEDULE_DEFAULT_ITEMID				0x00						// Default item ID: 0
#define DEF_SCHEDULE_MIN_ITEMID					10000						// Min item ID: 10000
#define DEF_SCHEDULE_MAX_ITEMID					19999						// Max item ID: 19999
#define DEF_SCHEDULE_INIT_ACTION				DEF_APP_ACTION_DISPLAYOFF	// Init action (for new item): Turn off display

#define DEF_SCHEDULE_ERROR						3240L
#define DEF_SCHEDULE_ERROR_SUCCESS				(DEF_SCHEDULE_ERROR + 1)	// Success (no error)
#define DEF_SCHEDULE_ERROR_ISDEFAULT			(DEF_SCHEDULE_ERROR + 2)	// Item is default (can not remove/delete)
#define DEF_SCHEDULE_ERROR_EMPTY				(DEF_SCHEDULE_ERROR + 3)	// Schedule data is empty
#define DEF_SCHEDULE_ERROR_MAXITEM				(DEF_SCHEDULE_ERROR + 4)	// Schedule data item number reaches maximum limit
#define DEF_SCHEDULE_ERROR_DUPLICATE			(DEF_SCHEDULE_ERROR + 5)	// Item data is duplicated (can not add)
#define DEF_SCHEDULE_ERROR_DUPLICATETIME		(DEF_SCHEDULE_ERROR + 6)	// Item time value is duplicated (can not add)

// Define properties for Power Reminder function
#define DEF_PWRREMINDER_MAX_ITEMNUM				100							// Max item number: 100
#define DEF_PWRREMINDER_MIN_ITEMID				10000						// Min item ID: 10000
#define DEF_PWRREMINDER_MAX_ITEMID				19999						// Max item ID: 19999
#define DEF_PWRREMINDER_PREVIEW_TIMEOUT			10							// Default time-out for preview: 10s

// Define special strings and numbers
#define DEF_STRING_EMPTY						_T("")						// Empty string
#define DEF_STRING_NEWLINE						_T("\n")					// New line string
#define DEF_STRING_NEWLINEWRET					_T("\r\n")					// New line string (with 'return' character)
#define DEF_STRING_NULL							_T("#NULL")					// Null/invalid string
#define DEF_STRING_QUOTEFORMAT					_T("\"%s\"")				// Quote string template format
#define DEF_STRING_TEXTCHANGEFORMAT				_T("%s -> %s")				// Text change format string template
#define DEF_STRING_INTCHANGEFORMAT				_T("%d -> %d")				// Integer number change format string template
#define DEF_STRING_FLOATCHANGEFORMAT			_T("%f -> %f")				// Float number change format string template
#define DEF_SYMBOL_DOT							_T(".")						// Dot (.) symbol
#define DEF_SYMBOL_COMMA						_T(",")						// Comma (,) symbol
#define DEF_SYMBOL_PATHSEPARATOR				_T("\\")					// File/folder path separator
#define DEF_SYMBOL_OUTPUTSIGN					_T(">> ")					// Output sign
#define DEF_SYMBOL_INPUTSIGN					_T(" <<")					// Input sign
#define DEF_SYMBOL_ANTEMERIDIEM					_T("AM")					// Ante meridiem (AM)
#define DEF_SYMBOL_POSTMERIDIEM					_T("PM")					// Post meridiem (PM)
#define DEF_CHAR_DOT							_T('.')						// Dot (.) character
#define DEF_CHAR_COMMA							_T(',')						// Comma (,) character
#define DEF_CHAR_QUOTAMARK						_T('\"')					// 'Quotation mark' character
#define DEF_CHAR_RETURN							_T('\r')					// 'Return' character
#define DEF_CHAR_ENDLINE						_T('\n')					// 'Endline' character
#define DEF_CHAR_NEWLINE						_T('\r\n')					// 'New line' character
#define DEF_CHAR_ENDSTRING						_T('\0')					// Null-termination (end of string)
#define DEF_BAKFILE_MAXNUM						100							// Maximum backup file number: 100

#define DEF_INTEGER_INVALID						-1							// Invalid integer number (equals -1)
#define DEF_INTEGER_NULL						0							// Null integer number (equals 0)
#define DEF_INTEGER_INFINITE					0							// Infinite (0 means "limitation not set")
#define DEF_BOOLVAL_CHECK						-1							// Checked
#define DEF_BOOLVAL_UNCHECK						0							// Unchecked
#define DEF_NUM_DAYSOFWEEK						7							// Number of days of week: 7 days
#define DEF_TOKEN_MAXCOUNT						50							// Max token number: 50
#define DEF_BUFF_MAXLENGTH						512							// Max buffer length: 512 characters
#define DEF_STRING_MAXLENGTH					256							// Max string length: 256 characters

#define DEF_WINVER_NONE							0x00
#define DEF_WINVER_WIN10						(DEF_WINVER_NONE+1)			// Windows 10
#define DEF_WINVER_WIN11						(DEF_WINVER_NONE+2)			// Windows 11
#define DEF_WINVER_BUILDNUMVER11				21996						// Build number: 21996

#define DEF_OFFSET_VSCRLBRWIDTH					3							// Offset = 3px
#define DEF_OFFSET_LISTCTRL						5							// Offset = 5px
#define DEF_OFFSET_LISTCTRL_WIN10				16							// Offset (on Windows 10) = 16px
#define DEF_OFFSET_LISTCOLWIDTH_WIN10			10							// Offset (on Windows 10) = 10px
#define DEF_LISTCTRL_HEADERHEIGHT				27							// Header height = 27px
#define DEF_LISTCTRL_ROWHEIGHT					18							// Row height = 18px
#define DEF_GRIDCTRL_ROWHEADER					0							// Row header index
#define DEF_GRIDCTRL_HEADERHEIGHT				25							// Header height = 25px
#define DEF_GRIDCTRL_ROWHEIGHT					23							// Row height = 23px
#define DEF_GRIDCTRL_HEADERHEIGHTEX				28							// Header height (extra) = 28px
#define DEF_GRIDCTRL_ROWHEIGHTEX				25							// Row height (extra) = 25px
#define DEF_GRIDCELL_LEFTMARGIN					3							// Grid cell left margin = 3px

#define DEF_SPINCTRL_TIMEMINPOS					0							// As 00:00
#define DEF_SPINCTRL_TIMEMAXPOS					1439						// As 23:59
#define DEF_SPINCTRL_SNOOZEMINPOS				1							// Min snooze time: 1 minutes
#define DEF_SPINCTRL_SNOOZEMAXPOS				30							// Max snooze time: 30 minutes
#define DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH	30							// Max length: 30 characters
#define DEF_LOGDISP_STRING_MAXLENGTH			20							// Max length: 20 characters
#define DEF_LOGFILE_MAXLENGTH					1048576						// Max file size: 1MB
#define DEF_WAITMESSAGE_TIMEOUT					30000						// Wait message timeout (tick-counts): 30s


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum datas for program, these datas will be used elsewhere in the programs
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Flag value
typedef enum eFLAG {
	FLAG_OFF = 0,					// Flag OFF
	FLAG_ON	 = 1,					// Flag ON
} FLAG;

// Return flag value
typedef enum eRETFLAG {
	RETFLAG_INVALID = -1,			// Return flag: Invalid
	RETFLAG_OK,						// Return flag: OK
	RETFLAG_CANCEL,					// Return flag: Cancel
	RETFLAG_UPDATE,					// Return flag: Update
	RETFLAG_CLOSE,					// Return flag: Close
} RETFLAG;

// Days of week
typedef enum eDAYOFWEEK {
	SUNDAY,							// Sunday
	MONDAY,							// Monday
	TUESDAY,						// Tuesday
	WEDNESDAY,						// Wednesday
	THURSDAY,						// Thursday
	FRIDAY,							// Friday
	SATURDAY,						// Saturday
} DAYOFWEEK;

// System events
typedef enum eSYSTEMEVENTID {
	SYSEVT_SUSPEND = 0,				// System suspend event
	SYSEVT_WAKEUP,					// System wakeup event
	SYSEVT_SESSIONEND,				// Session end event
} SYSTEMEVENTID;

// App option IDs
typedef enum eAPPOPTIONID {
	OPTIONID_LMBACTION = 0,			// Left mouse button action
	OPTIONID_MMBACTION,				// Middle mouse button action
	OPTIONID_RMBACTION,				// Right mouse button action
	OPTIONID_RMBSHOWMENU,			// Right mouse button: Only show menu
	OPTIONID_LANGUAGEID,			// Language setting
	OPTIONID_CURDISPLANGUAGE,		// Currently displaying language
	OPTIONID_SHOWDLGATSTARTUP,		// Show dialog at startup
	OPTIONID_STARTUPENABLE,			// Startup with Windows
	OPTIONID_CONFIRMACTION,			// Show confirm message before doing action
	OPTIONID_SAVEAPPEVENTLOG,		// Save app event log
	OPTIONID_SAVEHISTORYLOG,		// Save action history log
	OPTIONID_RUNASADMIN,			// Run with admin privileges
	OPTIONID_SHOWERRORMSG,			// Show action error message
	OPTIONID_NOTIFYSCHEDULE,		// Show notify tip for schedule action
	OPTIONID_ALLOWCANCELSCHED,		// Allow canceling schedule when notify
	OPTIONID_ENABLEHOTKEYSET,		// Enable background action hotkeys
	OPTIONID_ENABLEPWRREMINDER,		// Enable Power Peminder feature
	OPTIONID_SCHEDULEACTIVE,		// Default schedule active state
	OPTIONID_SCHEDULEACTION,		// Default schedule action ID
	OPTIONID_SCHEDULEREPEAT			// Default schedule repeat option
} APPOPTIONID;

// App flag IDs
typedef enum eAPPFLAGID {
	FLAGID_CHANGEFLAG = 0,			// Data/setting change flag
	FLAGID_READONLYMODE,			// Read-only mode
	FLAGID_LOCKSTATE,				// Lock state
	FLAGID_FORCECLOSING,			// Force closing by request
	FLAGID_DLGEXPANDED,				// Dialog expanded/collapsed
	FLAGID_NOTIFYICONSHOWED,		// Notify icon showing flag
	FLAGID_HOTKEYREGISTERED,		// Hotkey registered
	FLAGID_RESTARTASADMIN,			// Restart as admin flag
	FLAGID_PWRBROADCASTSKIPCOUNT,	// Power Broadcase event skip counter
} APPFLAGID;

// App feature item category IDs
typedef enum eFEATUREITEMCATEID {
	FID_POWERACTION = 0,			// Power action
	FID_SCHEDULEITEM,				// Action schedule item
	FID_HOTKEYITEM,					// HotkeySet item
	FID_PWRREMINDERITEM,			// Power Reminder item
} FEATUREITEMCATEID;

// Hotkey IDs
typedef enum eHOTKEYID {
	HKID_DISPLAYOFF = 0x1a01,		// Turn off display
	HKID_SLEEP,						// Sleep
	HKID_SHUTDOWN,					// Shut down
	HKID_RESTART,					// Restart
	HKID_SIGNOUT,					// Sign out
	HKID_HIBERNATE,					// Hibernate
} HOTKEYID;

// Power Reminder event IDs
typedef enum ePWRREMINDEREVENT {
	PREVT_AT_SETTIME = 0x1b01,		// At set time
	PREVT_AT_APPSTARTUP,			// At app startup
	PREVT_AT_SYSWAKEUP,				// At system wake
	PREVT_AT_BFRPWRACTION,			// Before power action
	PREVT_AT_PWRACTIONWAKE,			// Wake after action
	PREVT_AT_APPEXIT,				// Before app exit
} PWRREMINDEREVENT;

// Power Reminder message styles
typedef enum ePWRREMIDERSTYLE {
	PRSTYLE_MSGBOX = 0x1c01,		// Message Box
	PRSTYLE_DIALOG,					// Dialog Box
} PWRREMINDERSTYLE;

// Action history category IDs
typedef enum eHISTORYCATEGORY {
	HSTRCATE_PWRACTION = 0x1c02,	// Power action
	HSTRCATE_SCHEDULE,				// Schedule
	HSTRCATE_HOTKEYSET,				// HotkeySet
	HSTRCATE_PWRREMINDER,			// Power Reminder
} HISTORYCATEGORY;

// List view column size units
typedef enum eLVCOLSIZEUNIT {
	COLSIZE_PIXEL = 0,				// Pixel
	COLSIZE_PERCENT,				// Percent (of frame width)
} LVCOLSIZEUNIT;

// Grid table column style
typedef enum eGRIDCOLSTYLE {
	COLSTYLE_FIXED = 0,				// Fixed cell
	COLSTYLE_CHECKBOX,				// Checkbox cell
	COLSTYLE_NORMAL,				// Normal cell
} GRIDCOLSTYLE;

// LogViewer table column IDs
typedef enum eLOGVWERTABLECOLID {
	SCHCOL_ID_DATETIME = 0,			// Date/Time
	SCHCOL_ID_CATEGORY,				// Event ID (category ID)
	SCHCOL_ID_DESCRIPTION,			// Additional description
} LOGVWERTABLECOLID;

// Schedule data table column IDs
typedef enum eSCHTABLECOLID {
	SCHCOL_ID_INDEX = 0,			// Index
	SCHCOL_ID_STATE,				// Enable/active state
	SCHCOL_ID_ACTIONID,				// Action ID
	SCHCOL_ID_TIMEVALUE,			// Time value
	SCHCOL_ID_REPEAT,				// Repeat daily
} SCHTABLECOLID;

// HotkeySet table column IDs
typedef enum eHKEYTABLECOLID {
	HKSCOL_ID_STATE = 0,			// Enable state
	HKSCOL_ID_HKACTIONID,			// Hotkey action ID
	HKSCOL_ID_KEYSTROKES,			// Keystrokes
} HKEYTABLECOLID;

// Power Reminder table column IDs
typedef enum ePWRTABLECOLID {
	PWRCOL_ID_INDEX = 0,			// Index
	PWRCOL_ID_STATE,				// Enable state
	PWRCOL_ID_ITEMID,				// Item ID
	PWRCOL_ID_MESSAGE,				// Message content
	PWRCOL_ID_EVENTID,				// Event ID
	PWRCOL_ID_STYLE,				// Reminder style
	PWRCOL_ID_REPEAT				// Repeat daily
} PWRTABLECOLID;

// Registry path type
typedef enum eREGPATHTYPE {
	REGPATH_FULL = 0,				// Full path
	REGPATH_ROOTKEY,				// Root key only
	REGPATH_SUBPATH,				// Including sub-key path
	REGPATH_PROFILEKEY,				// Including profile key name
	REGPATH_APPNAME,				// Including app name
	REGPATH_SECTIONNAME,			// Including section name
	REGPATH_KEYNAME,				// Including key name
} REGPATHTYPE;

// Registry value type
typedef enum eREGVALUETYPE {
	REGTYPE_NONE = 0,				// Undefined type
	REGTYPE_STRING,					// String value
	REGTYPE_DWORD32,				// DWORD (32-bit) value
	REGTYPE_QWORD64,				// QWORD (64-bit) value
	REGTYPE_MULTISTRING,			// Multi-string value
} REGVALUETYPE;

// String validation error codes
typedef enum eSTRVALIDERR {
	STRVAL_ERR_NORMAL = 0,			// Normal/valid string
	STRVAL_ERR_EMPTY,				// Empty string
	STRVAL_ERR_OVERSIZE,			// Character number overlimit
	STRVAL_ERR_INVALIDCHAR,			// Contains invalid character(s)
	STRVAL_ERR_UNKNOWN,				// Unknown
} STRVALIDERR;

// Substring types
typedef enum eSUBSTRINGTYPE {
	SUBSTR_LEFT = 0,				// Left substring
	SUBSTR_MID,						// Middle substring
	SUBSTR_RIGHT,					// Right substring
} SUBSTRINGTYPE;

// File types (view file mode)
typedef enum eFILETYPE {
	FILETYPE_TXT = 0,				// Text files
	FILETYPE_IMG,					// Image files
} FILETYPE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for program, these data types will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	CONFIGDATA
//  Description:	Store app settings and configurations
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagCONFIGDATA
{
	// Main settings
	UINT	nLMBAction;												// Left mouse button action
	UINT	nMMBAction;												// Middle mouse button action
	UINT	nRMBAction;												// Right mouse button action
	BOOL	bRMBShowMenu;											// Right mouse button: Only show menu

	// Display setting
	UINT	nLanguageID;											// Language setting

	// System settings
	BOOL	bShowDlgAtStartup;										// Show dialog at startup
	BOOL	bStartupEnabled;										// Startup with Windows
	BOOL	bConfirmAction;											// Show confirm message before executing action
	BOOL	bSaveHistoryLog;										// Save app history log
	BOOL	bSaveAppEventLog;										// Save app event log
	BOOL	bRunAsAdmin;											// Run with admin privileges
	BOOL	bShowErrorMsg;											// Show action error message
	BOOL	bNotifySchedule;										// Show notify tip for schedule action
	BOOL	bAllowCancelSchedule;									// Allow canceling schedule when notify
	BOOL	bEnableBackgroundHotkey;								// Enable background action hotkeys
	BOOL	bEnablePowerReminder;									// Enable Power Peminder feature

	// Member functions
	void Copy(const tagCONFIGDATA&);								// Copy data
	BOOL Compare(const tagCONFIGDATA&) const;						// Compare data
} CONFIGDATA, *PCONFIGDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRREPEATSET
//  Description:	Store data of a Power++ item repeat set
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagPWRREPEATSET
{
	// Member variables
	BOOL		bRepeat;											// Repeat daily
	BOOL		bAllowSnooze;										// Allow snoozing mode
	INT			nSnoozeInterval;									// Snooze interval
	BYTE		byRepeatDays;										// Days of week (for repeating)

	// Constructor
	tagPWRREPEATSET();												// Default constructor
	tagPWRREPEATSET(const tagPWRREPEATSET&);						// Copy constructor

	// Operator
	tagPWRREPEATSET& operator=(const tagPWRREPEATSET&);				// Copy assignment operator

	// Member functions
	void Copy(const tagPWRREPEATSET&);								// Copy data
	BOOL Compare(const tagPWRREPEATSET&) const;						// Compare data
	BOOL IsDayActive(DAYOFWEEK dayOfWeek) const;					// Check if day of week is active
} PWRREPEATSET, *PPWRREPEATSET;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	SCHEDULEITEM
//  Description:	Store schedule item settings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagSCHEDULEITEM
{
	// Member variables
	UINT			nItemID;										// Item ID
	BOOL			bEnable;										// Enable/disable state
	UINT			nAction;										// Schedule action
	SYSTEMTIME		stTime;											// Schedule time
	PWRREPEATSET	rpsRepeatSet;									// Repeat set data

	// Constructor
	tagSCHEDULEITEM();												// Default constructor
	tagSCHEDULEITEM(UINT nItemID);									// Overloaded constructor
	tagSCHEDULEITEM(const tagSCHEDULEITEM&);						// Copy constructor

	// Operator
	tagSCHEDULEITEM& operator=(const tagSCHEDULEITEM&);				// Copy assignment operator

	// Member functions
	void Copy(const tagSCHEDULEITEM&);								// Copy data
	BOOL Compare(const tagSCHEDULEITEM&) const;						// Compare items
	void SetActiveState(BOOL);										// Set item active state
	BOOL IsEmpty(void) const;										// Check if item data is empty
	BOOL IsRepeatEnable(void) const;								// Check if repeat is enabled
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	BOOL IsDayActive(DAYOFWEEK) const;								// Check if day of week is active
	BYTE GetActiveDays(void) const;									// Get repeat active days
	void Print(CString& strOutput);									// Print item data
} SCHEDULEITEM, *PSCHEDULEITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	SCHEDULEITEMLIST
//  Description:	Store list of Action Schedule items
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<SCHEDULEITEM, SCHEDULEITEM> SCHEDULEITEMLIST;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	SCHEDULEDATA
//  Description:	Store app Action Schedule data settings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagSCHEDULEDATA
{
	// Member variables
	SCHEDULEITEM		schDefaultItem;								// Default schedule item
	SCHEDULEITEMLIST	arrSchedExtraItemList;						// List of extra schedule items

	// Constructor
	tagSCHEDULEDATA();												// Default constructor
	tagSCHEDULEDATA(const tagSCHEDULEDATA&);						// Copy constructor

	// Operator
	tagSCHEDULEDATA& operator=(const tagSCHEDULEDATA&);				// Copy assignment operator

	// Member functions
	void Init();													// Init data
	void Copy(const tagSCHEDULEDATA&);								// Copy data
	DWORD Add(const SCHEDULEITEM&);									// Add item
	DWORD Update(const SCHEDULEITEM&);								// Update item
	const SCHEDULEITEM& GetDefaultItem(void) const;					// Get default item (constant)
	SCHEDULEITEM& GetDefaultItem(void);								// Get default item
	const SCHEDULEITEM& GetItemAt(int) const;						// Get item at index (constant)
	SCHEDULEITEM& GetItemAt(int);									// Get item at index
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust();													// Adjust data validity
	UINT GetNextID();												// Get next item ID (to add new item)
	INT_PTR GetExtraItemNum(void) const;							// Get number of extra items
	BOOL IsDefaultEmpty(void) const;								// Check if default item is empty
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsExtraEmpty(void) const;									// Check if extra data is empty
	BOOL IsAllEmpty(void) const;									// Check if all items are empty
	void Delete(int);												// Delete item at index
	void DeleteExtra(void);											// Delete all extra items
	void DeleteAll(void);											// Delete all data
} SCHEDULEDATA, *PSCHEDULEDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	HOTKEYSETITEM
//  Description:	Store data of a Hotkeyset item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagHOTKEYSETITEM
{
	// Member variables
	BOOL	bEnable;												// Hotkey enabled/disabled
	UINT	nHKActionID;											// Hotkey action ID
	DWORD	dwCtrlKeyCode;											// Control Keycode #1
	DWORD	dwFuncKeyCode;											// Function Keycode #2

	// Constructor
	tagHOTKEYSETITEM();												// Default constructor
	tagHOTKEYSETITEM(UINT nHKActionID);								// Overloaded constructor
	tagHOTKEYSETITEM(const tagHOTKEYSETITEM&);						// Copy constructor

	// Operator
	tagHOTKEYSETITEM& operator=(const tagHOTKEYSETITEM&);			// Copy assignment operator

	// Member functions
	void Copy(const tagHOTKEYSETITEM&);								// Copy item
	BOOL IsEmpty(void) const;										// Check if item is empty
	BOOL Compare(const tagHOTKEYSETITEM&) const;					// Compare items
	void Print(CString& strOutput);									// Print item data
	void PrintKeyStrokes(CString& strOutput);						// Print item keystrokes
} HOTKEYSETITEM, *PHOTKEYSETITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	HOTKEYSETITEMLIST
//  Description:	Store list of Hotkeyset items
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<HOTKEYSETITEM, HOTKEYSETITEM> HOTKEYSETITEMLIST;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	HOTKEYSETDATA
//  Description:	Store app Hotkeyset data settings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagHOTKEYSETDATA
{
	// Member variables
	HOTKEYSETITEMLIST arrHotkeySetList;								// Pointer to HotkeySet list

	// Constructor
	tagHOTKEYSETDATA();												// Default constructor
	tagHOTKEYSETDATA(const tagHOTKEYSETDATA&);						// Copy constructor

	// Operator
	tagHOTKEYSETDATA& operator=(const tagHOTKEYSETDATA&);			// Copy assignment operator

	// Member functions
	void Init();													// Init data
	void Copy(const tagHOTKEYSETDATA&);								// Copy data
	void Add(const HOTKEYSETITEM&);									// Add item
	void Update(const HOTKEYSETITEM&);								// Update item
	const HOTKEYSETITEM& GetItemAt(int) const;						// Get item at index (const)
	HOTKEYSETITEM& GetItemAt(int);									// Get item at index
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust();													// Adjust data validity
	INT_PTR GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty() const;										// Check if all items are empty
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
	void PrintKeyStrokes(UINT nHKID, CString& strOutput);			// Print item keystrokes by ID
} HOTKEYSETDATA, *PHOTKEYSETDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	RMDMSGSTYLESET
//  Description:	Store data of a Reminder message style set
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagRMDMSGSTYLESET
{
	// Member variables
	COLORREF	colorBkgrd;											// Background color
	COLORREF	colorText;											// Text color
	CString		strFontName;										// Font name
	UINT		uiFontSize;											// Font size
	UINT		uiTimeout;											// Timeout (auto-close) interval
	UINT		uiIconID;											// Message icon ID
	INT			nIconSize;											// Message icon size
	BYTE		byIconPos;											// Message icon position
	BYTE		byDisplayPos;										// Message display position
	UINT		uiHMargin;											// Display area horizontal margin
	UINT		uiVMargin;											// Display area vertical margin

	// Constructor
	tagRMDMSGSTYLESET();											// Default constructor
	tagRMDMSGSTYLESET(const tagRMDMSGSTYLESET&);					// Copy constructor

	// Operator
	tagRMDMSGSTYLESET& operator=(const tagRMDMSGSTYLESET&);			// Copy assignment operator

	// Member functions
	void Copy(const tagRMDMSGSTYLESET&);							// Copy data
	BOOL Compare(const tagRMDMSGSTYLESET&) const;					// Compare data
} RMDMSGSTYLESET, *PRMDMSGSTYLESET;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRREMINDERITEM
//  Description:	Store data of a Power Reminder item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagPWRREMINDERITEM
{
	// Member variables
	BOOL			bEnable;										// Enable state
	UINT			nItemID;										// Item ID
	CString			strMessage;										// Message content
	UINT			nEventID;										// Event ID
	SYSTEMTIME		stTime;											// Event time
	DWORD			dwMsgStyle;										// Reminder style
	PWRREPEATSET	rpsRepeatSet;									// Repeat set data
	BOOL			bUseCustomStyle;								// Use message custom style
	RMDMSGSTYLESET	rmsMsgStyleSet;									// Reminder message style set

	// Constructor
	tagPWRREMINDERITEM();											// Default constructor
	tagPWRREMINDERITEM(const tagPWRREMINDERITEM&);					// Copy constructor

	// Operator
	tagPWRREMINDERITEM& operator=(const tagPWRREMINDERITEM&);		// Copy assignment operator

	// Member functions
	void Copy(const tagPWRREMINDERITEM&);							// Copy item
	BOOL IsEmpty() const;											// Check if item is empty
	BOOL Compare(const tagPWRREMINDERITEM&) const;					// Compare items
	void SetEnableState(BOOL);										// Set item enable state
	BOOL IsRepeatEnable(void) const;								// Check if item repeat mode is enabled
	BOOL IsDayActive(DAYOFWEEK dayOfWeek) const;					// Check if day of week is active
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	BYTE GetActiveDays(void) const;									// Get repeat active days
	void Print(CString& strOutput);									// Print item data
} PWRREMINDERITEM, *PPWRREMINDERITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRREMINDERITEMLIST
//  Description:	Store list of Power Reminder items
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<PWRREMINDERITEM, PWRREMINDERITEM> PWRREMINDERITEMLIST;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRREMINDERDATA
//  Description:	Store app Power Reminder data settings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagPWRREMINDERDATA
{
	// Member variables
	PWRREMINDERITEMLIST	arrRmdItemList;								// List of reminder items
	RMDMSGSTYLESET		rmdCommonStyle;								// Common message style set

	// Constructor
	tagPWRREMINDERDATA();											// Default constructor
	tagPWRREMINDERDATA(const tagPWRREMINDERDATA&);					// Copy constructor

	// Operator
	tagPWRREMINDERDATA& operator=(const tagPWRREMINDERDATA&);		// Copy assignment operator

	// Member functions
	void Init();													// Init data
	void Copy(const tagPWRREMINDERDATA&);							// Copy data
	void Add(const PWRREMINDERITEM&);								// Add item
	void Update(const PWRREMINDERITEM&);							// Update item
	const PWRREMINDERITEM& GetItemAt(int) const;					// Get item at index (constant)
	PWRREMINDERITEM& GetItemAt(int);								// Get item at index
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust();													// Adjust data validity
	UINT GetNextID();												// Get next item ID (to add new item)
	INT_PTR GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty() const;										// Check if all items are empty
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
} PWRREMINDERDATA, *PPWRREMINDERDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRRUNTIMEITEM
//  Description:	Store data of a runtime info item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagPWRRUNTIMEITEM 
{
	// Member variables
	INT			nCategory;											// Item category
	UINT		nItemID;											// Power Reminder item ID
	INT			nDisplayFlag;										// Item displaying flag
	INT			nSkipFlag;											// Item skip flag
	INT			nSnoozeFlag;										// Item snooze trigger flag
	SYSTEMTIME	stNextSnoozeTime;									// Next snooze trigger time

	// Constructor
	tagPWRRUNTIMEITEM();											// Default constructor
	tagPWRRUNTIMEITEM(const tagPWRRUNTIMEITEM&);					// Copy constructor

	// Operator
	tagPWRRUNTIMEITEM& operator=(const tagPWRRUNTIMEITEM&);			// Copy assignment operator

	// Member functions
	void Copy(const tagPWRRUNTIMEITEM&);							// Copy data
	void CalcNextSnoozeTime(int nInterval);							// Calculate next snooze time
} PWRRUNTIMEITEM, *PPWRRUNTIMEITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PWRRUNTIMEQUEUE
//  Description:	Store list of Power++ runtime item queue
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<PWRRUNTIMEITEM, PWRRUNTIMEITEM> PWRRUNTIMEQUEUE;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	HISTORYINFODATA
//  Description:	Store app action history info data
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagHISTORYINFODATA
{
	// Member variables
	BOOL		bInitState;										// Init state flag
	UINT		nCategoryID;									// Category ID
	SYSTEMTIME	stTimestamp;									// Timestamp of history
	UINT		nItemID;										// Item ID
	UINT		nActionNameID;									// Name of action (string ID)
	BOOL		bActionResult;									// Action result
	DWORD		dwErrorCode;									// Returned error code
	CString		strDescription;									// History description (attached info)

	// Constructor
	tagHISTORYINFODATA();										// Default constructor
	tagHISTORYINFODATA(const tagHISTORYINFODATA&);				// Copy constructor

	// Operator
	tagHISTORYINFODATA& operator=(const tagHISTORYINFODATA&);	// Copy assignment operator

	// Member functions
	void Copy(const tagHISTORYINFODATA&);						// Copy data
	void Init(UINT nCategoryID);								// Initialization
	void RemoveAll(void);										// Remove all data
} HISTORYINFODATA, *PHISTORYINFODATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	USERMENUITEM
//  Description:	Store data of a user custom menu item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagUSERMENUITEM
{
	UINT	nItemID;										// Menu item ID
	UINT	nItemType;										// Menu item type
	LPTSTR	lpszItemCaption;								// Menu item text
	UINT	nParentID;										// Item parent ID
	LPTSTR	lpszParentCaption;								// Item parent caption
} USERMENUITEM, *PUSERMENUITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	USERMENU
//  Description:	Store data of a user custom menu
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagUSERMENU
{
	UINT nItemCount;										// Number of menu items
	PUSERMENUITEM pMenuItemList;							// User menu pointer
} USERMENU, *PUSERMENU;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	REGISTRYVALUE
//  Description:	Store data values of a registry key 
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagREGISTRYVALUE 
{
	// Member data values
	CString			strValue;								// String value
	DWORD			dwValue;								// DWORD (32-bit) value
	ULONGLONG		ullValue;								// QWORD (64-bit) value
	CStringArray	astrStringValue;						// Multi-string value

	// Construction functions
	tagREGISTRYVALUE();										// Default constructor
	tagREGISTRYVALUE(const tagREGISTRYVALUE&);				// Copy constructor	
	~tagREGISTRYVALUE();									// Default destructor

	// Operator
	tagREGISTRYVALUE& operator=(const tagREGISTRYVALUE&);	// Copy assignment operator

	// Member functions
	void Copy(const tagREGISTRYVALUE&);						// Copy data
	void RemoveAll(void);									// Remove all data
} REGISTRYVALUE, *PREGISTRYVALUE;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	REGISTRYKEY
//  Description:	Store data of a registry key info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagREGISTRYKEY
{
	// Member variables
	CString			strKeyName;								// Key name (string)
	UINT			nValueType;								// Value type
	REGISTRYVALUE	regValue;								// Registry value data

	// Constructor
	tagREGISTRYKEY();										// Default constructor
	tagREGISTRYKEY(const tagREGISTRYKEY&);					// Copy constructor

	// Operator
	tagREGISTRYKEY& operator=(const tagREGISTRYKEY&);		// Copy assignment operator

	// Member functions
	void Copy(const tagREGISTRYKEY&);						// Copy data
	void RemoveAll(void);									// Remove all data

	// Get/set properties
	void GetKeyName(CString&) const;						// Get key name
	LPCTSTR GetKeyName(void) const;							// Get key name
	void SetKeyName(UINT);									// Set key name
	void SetKeyName(LPCTSTR);								// Set key name
	UINT GetValueType(void) const;							// Get value type
	void SetValueType(UINT);								// Set value type

	// Get/set value
	void GetStringValue(CString&) const;					// Get String value
	LPCTSTR GetStringValue(void) const;						// Get String value
	void SetStringValue(LPCTSTR);							// Set String value
	DWORD GetDWordValue(void) const;						// Get DWORD (32-bit) value
	void SetDWordValue(DWORD);								// Set DWORD (32-bit) value
	QWORD GetQWordValue(void) const;						// Get QWORD (64-bit) value
	void SetQWordValue(ULONGLONG);							// Set QWORD (64-bit) value
	void GetMultiStringValue(CStringArray&) const;			// Get Multi-string value
	void SetMultiStringValue(CStringArray&);				// Set Multi-string value
} REGISTRYKEY, *PREGISTRYKEY;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	REGISTRYINFO
//  Description:	Store data of a registry key info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagREGISTRYINFO
{
	// Member variables
	HKEY			hRootKey;								// Root key (HKEY)
	CString			strRootKey;								// Root key (string)
	CStringArray	astrSubkeyPath;							// Subkey path (string array)
	CString			strProfileName;							// Profile key name (string)
	CString			strAppName;								// App name (string)
	CStringArray	astrSectionArray;						// Section array (string)
	REGISTRYKEY		regKeyInfo;								// Registry key info

	// Constructor
	tagREGISTRYINFO();										// Default constructor
	tagREGISTRYINFO(const tagREGISTRYINFO&);				// Copy constructor

	// Operator
	tagREGISTRYINFO& operator=(const tagREGISTRYINFO&);		// Copy assignment operator

	// Member functions
	void Copy(const tagREGISTRYINFO&);						// Copy data
	void RemoveAll(void);									// Remove all data

	// Set properties
	void SetRootKeyName(UINT);								// Set root key name (resource ID)
	void SetSubkeyPath(UINT);								// Set Subkey path (resource ID)
	void SetSubkeyPath(CStringArray&);						// Set Subkey path (string array)
	void SetProfileName(UINT);								// Set Profile key name (resource ID)
	void SetAppName(UINT);									// Set App name (resource ID)
	void SetSectionName(UINT);								// Set Section array (resource ID)
	void SetSectionName(CStringArray&);						// Set Section array (string array)
} REGISTRYINFO, *PREGISTRYINFO;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LVCOLUMNFORMAT
//  Description:	Store list control/view column format info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagLVCOLUMNFORMAT
{
	INT		nColumnTitleID;									// Column title ID
	INT		nColumnSize;									// Column size
	INT		nColumnSizeUnit;								// Column size unit
} LVCOLUMNFORMAT, *PLVCOLUMNFORMAT;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	GRIDCTRLCOLFORMAT
//  Description:	Store grid control/view column format info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagGRIDCTRLCOLFMT 
{
	INT		nColID;											// Column ID
	UINT	nHeaderTitleID;									// Header title string ID
	INT		nWidth;											// Column width
	UINT	nColStyle;										// Column style
	BOOL	bCenter;										// Align center
} GRIDCTRLCOLFORMAT, *PGRIDCTRLCOLFORMAT;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	HOTKEYLIST
//  Description:	Store hotkey info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagHOTKEYINFO
{
	DWORD	dwCtrlKeyCode;									// Control Keycode #1
	DWORD	dwFuncKeyCode;									// Function Keycode #2
	UINT	nHotkeyDescription;								// Hotkey description (string ID)
} HOTKEYINFO, *PHOTKEYINFO;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	IDPAIR
//  Description:	Using for ID pair list
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagIDPAIR
{
	UINT nFirstID;											// First pair ID
	UINT nSecondID;											// Second pair ID
} IDPAIR, *PIDPAIR;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	ACTIONDEF
//  Description:	Using for action definition/combination table
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagACTIONDEF
{
	UINT nActionDefID;										// Action ID
	UINT nActionNameID;										// Action Name ID
	UINT nActionMsgID;										// Action message ID
	UINT nSchedNotifyID;									// Schedule notify message ID
	UINT nNotifyTipID;										// Notify file tip ID
	UINT nBalloonTipID;										// Balloon tip ID
} ACTIONDEF, *PACTIONDEF;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	IDPAIRLIST
//					STRINGPAIRLIST
//					ACTIONDEFCOMBTABLE
//  Description:	Data pair list
//  Derivered from: C++ vector type
//
//////////////////////////////////////////////////////////////////////////

typedef std::vector<IDPAIR>		IDPAIRLIST;
typedef std::vector<LANGTEXT>	STRINGPAIRLIST;
typedef std::vector<ACTIONDEF>	ACTIONDEFTABLE;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	BUFFER
//  Description:	Using for string buffer processing (debug command, etc)
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagBUFFER 
{
	INT		nLength;										// Buffer length
	TCHAR	tcToken[DEF_BUFF_MAXLENGTH];					// Buffer token
} BUFFER, *PBUFFER;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	RESTARTREQ
//  Description:	Using for request to restart app as admin
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagRESTARTREQ 
{
	BOOL bRequest;											// Request to restart
	BOOL bAdminCheck;										// Check if already running as admin
	BOOL bNotAdminShowMsg;									// If not admin, not show check message
	BOOL bIsAdminDoNothing;									// If already running as admin, do nothing
	BOOL bShowMsgWhenDeny;									// Show message when denied
	BOOL bResetFlag;										// Reset flag when denied
} RESTARTREQ, *PRESTARTREQ;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	PERFORMANCECOUNTER
//  Description:	Using for querrying performance counter of functions
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagPERFORMANCECOUNTER
{
	// Member variables
	LARGE_INTEGER startTime;								// Start time
	LARGE_INTEGER endTime;									// End time
	LARGE_INTEGER frequency;								// Performance frequency

	// Constructor
	tagPERFORMANCECOUNTER();								// Default constructor

	// Member functions
	void Start(void);										// Start performance counter
	void Stop(void);										// Stop performance counter
	double GetElapsedTime(BOOL) const;						// Get function execution elapsed time
} PERFORMANCECOUNTER, *PPERFORMANCECOUNTER;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define global static variables for program, these variables will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static IDPAIRLIST idplErrorCode
{
/*----------Action ID----------------------------Action Name ID---------------------*/
	{ DEF_APP_ERROR_SUCCESS,				ERROR_SUCCESS						},
	{ DEF_APP_ERROR_INVALID_FUNCTION,		ERROR_INVALID_FUNCTION				},
	{ DEF_APP_ERROR_FILE_NOT_FOUND,			ERROR_FILE_NOT_FOUND				},
	{ DEF_APP_ERROR_PATH_NOT_FOUND,			ERROR_PATH_NOT_FOUND				},
	{ DEF_APP_ERROR_ACCESS_DENIED,			ERROR_ACCESS_DENIED					},
	{ DEF_APP_ERROR_INVALID_HANDLE,			ERROR_INVALID_HANDLE				},
	{ DEF_APP_ERROR_INVALID_DATA,			ERROR_INVALID_DATA					},
	{ DEF_APP_ERROR_NO_MORE_FILES,			ERROR_NO_MORE_FILES					},
	{ DEF_APP_ERROR_FILE_EXISTS,			ERROR_FILE_EXISTS					},
	{ DEF_APP_ERROR_CANNOT_MAKE,			ERROR_CANNOT_MAKE					},
	{ DEF_APP_ERROR_INVALID_PARAMETER,		ERROR_INVALID_PARAMETER				},
	{ DEF_APP_ERROR_OPEN_FAILED,			ERROR_OPEN_FAILED					},
	{ DEF_APP_ERROR_BUFFER_OVERFLOW,		ERROR_BUFFER_OVERFLOW				},
	{ DEF_APP_ERROR_INVALID_NAME,			ERROR_INVALID_NAME					},
	{ DEF_APP_ERROR_DIR_NOT_EMPTY,			ERROR_DIR_NOT_EMPTY					},
	{ DEF_APP_ERROR_FAIL_SHUTDOWN,			ERROR_FAIL_SHUTDOWN					},
	{ DEF_APP_ERROR_FAIL_RESTART,			ERROR_FAIL_RESTART					},
	{ DEF_APP_ERROR_INVALID_ADDRESS,		ERROR_INVALID_ADDRESS				},
	{ DEF_APP_ERROR_APP_INIT_FAILURE,		ERROR_APP_INIT_FAILURE				},
	{ DEF_APP_ERROR_CANNOT_LOAD_REGISTRY,	ERROR_CANNOT_LOAD_REGISTRY_FILE		},
	{ DEF_APP_ERROR_REGISTRY_QUOTA_LIMIT,	ERROR_REGISTRY_QUOTA_LIMIT			},
	{ DEF_APP_ERROR_SYSTEM_SHUTDOWN,		ERROR_SYSTEM_SHUTDOWN				},
	{ DEF_APP_ERROR_HIBERNATED,				ERROR_HIBERNATED					},
	{ DEF_APP_ERROR_RESUME_HIBERNATION,		ERROR_RESUME_HIBERNATION			},
	{ DEF_APP_ERROR_WAKE_SYSTEM,			ERROR_WAKE_SYSTEM					},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplActionName
{
/*----------Action ID----------------------------Action Name ID---------------------*/
	{ DEF_APP_ACTION_NOTHING,				ACTION_NAME_NOTHING					},
	{ DEF_APP_ACTION_DISPLAYOFF,			ACTION_NAME_DISPLAYOFF				},
	{ DEF_APP_ACTION_SLEEP,					ACTION_NAME_SLEEP					},
	{ DEF_APP_ACTION_SHUTDOWN,				ACTION_NAME_SHUTDOWN				},
	{ DEF_APP_ACTION_RESTART,				ACTION_NAME_RESTART					},
	{ DEF_APP_ACTION_SIGNOUT,				ACTION_NAME_SIGNOUT					},
	{ DEF_APP_ACTION_HIBERNATE,				ACTION_NAME_HIBERNATE				},
	{ DEF_APP_ACTION_SHOWMENU,				ACTION_NAME_SHOWMENU				},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplActionMsg
{
/*----------Action ID---------------------------Message String ID-------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,			MESSAGE_ACTION_DISPLAYOFF			},
	{ DEF_APP_ACTION_SLEEP,					MESSAGE_ACTION_SLEEP				},
	{ DEF_APP_ACTION_SHUTDOWN,				MESSAGE_ACTION_SHUTDOWN				},
	{ DEF_APP_ACTION_RESTART,				MESSAGE_ACTION_RESTART				},
	{ DEF_APP_ACTION_SIGNOUT,				MESSAGE_ACTION_SIGNOUT				},
	{ DEF_APP_ACTION_HIBERNATE,				MESSAGE_ACTION_HIBERNATE			},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplSchedNotifyMsg
{
/*---------Action ID------------------------------Message String ID-----------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,			MESSAGE_SCHEDNOTIFY_DISPLAYOFF		},
	{ DEF_APP_ACTION_SLEEP,					MESSAGE_SCHEDNOTIFY_SLEEP			},
	{ DEF_APP_ACTION_SHUTDOWN,				MESSAGE_SCHEDNOTIFY_SHUTDOWN		},
	{ DEF_APP_ACTION_RESTART,				MESSAGE_SCHEDNOTIFY_RESTART			},
	{ DEF_APP_ACTION_SIGNOUT,				MESSAGE_SCHEDNOTIFY_SIGNOUT			},
	{ DEF_APP_ACTION_HIBERNATE,				MESSAGE_SCHEDNOTIFY_HIBERNATE		},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplNotifyTip
{
/*---------Action ID---------------------------Notify String ID---------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,			NOTIFY_TIP_DISPLAYOFF				},
	{ DEF_APP_ACTION_SLEEP,					NOTIFY_TIP_SLEEP					},
	{ DEF_APP_ACTION_SHUTDOWN,				NOTIFY_TIP_SHUTDOWN					},
	{ DEF_APP_ACTION_RESTART,				NOTIFY_TIP_RESTART					},
	{ DEF_APP_ACTION_SIGNOUT,				NOTIFY_TIP_SIGNOUT					},
	{ DEF_APP_ACTION_HIBERNATE,				NOTIFY_TIP_HIBERNATE				},
	{ DEF_APP_ACTION_SHOWMENU,				NOTIFY_TIP_SHOWMENU					},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplBalloonTip
{
/*---------Action ID---------------------------Balloon String ID--------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,			BALLOON_TIP_DISPLAYOFF				},
	{ DEF_APP_ACTION_SLEEP,					BALLOON_TIP_SLEEP					},
	{ DEF_APP_ACTION_SHUTDOWN,				BALLOON_TIP_SHUTDOWN				},
	{ DEF_APP_ACTION_RESTART,				BALLOON_TIP_RESTART					},
	{ DEF_APP_ACTION_SIGNOUT,				BALLOON_TIP_SIGNOUT					},
	{ DEF_APP_ACTION_HIBERNATE,				BALLOON_TIP_HIBERNATE				},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplHKActionID
{
/*----HotKey Action ID----------------------------Action ID-------------------------*/
	{ HKID_DISPLAYOFF,						DEF_APP_ACTION_DISPLAYOFF			},
	{ HKID_SLEEP,							DEF_APP_ACTION_SLEEP				},
	{ HKID_SHUTDOWN,						DEF_APP_ACTION_SHUTDOWN				},
	{ HKID_RESTART,							DEF_APP_ACTION_RESTART				},
	{ HKID_SIGNOUT,							DEF_APP_ACTION_SIGNOUT				},
	{ HKID_HIBERNATE,						DEF_APP_ACTION_HIBERNATE			},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplPwrReminderEvt
{
/*--------Event ID-----------------------------Event String ID----------------------*/
	{ PREVT_AT_SETTIME,						PWRRMD_EVENT_AT_SETTIME				},
	{ PREVT_AT_APPSTARTUP,					PWRRMD_EVENT_AT_APPSTARTUP			},
	{ PREVT_AT_SYSWAKEUP,					PWRRMD_EVENT_AT_SYSWAKEUP			},
	{ PREVT_AT_BFRPWRACTION,				PWRRMD_EVENT_AT_BFRPWRACTION		},
	{ PREVT_AT_PWRACTIONWAKE,				PWRRMD_EVENT_AT_PWRACTIONWAKE		},
	{ PREVT_AT_APPEXIT,						PWRRMD_EVENT_AT_APPEXIT				},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplPwrReminderStyle
{
/*-----Style ID--------------------------------Style String ID----------------------*/
	{ PRSTYLE_MSGBOX,						PWRRMD_STYLE_MESSAGEBOX				},
	{ PRSTYLE_DIALOG,						PWRRMD_STYLE_DIALOG					},
/*----------------------------------------------------------------------------------*/
};

static IDPAIRLIST idplDayOfWeek
{
/*----Day ID---------------------------------Day title string ID--------------------*/
	{ MONDAY,								DAYOFWEEK_TITLE_MONDAY				},
	{ TUESDAY,								DAYOFWEEK_TITLE_TUESDAY				},
	{ WEDNESDAY,							DAYOFWEEK_TITLE_WEDNESDAY			},
	{ THURSDAY,								DAYOFWEEK_TITLE_THURSDAY			},
	{ FRIDAY,								DAYOFWEEK_TITLE_FRIDAY				},
	{ SATURDAY,								DAYOFWEEK_TITLE_SATURDAY			},
	{ SUNDAY,								DAYOFWEEK_TITLE_SUNDAY				},
/*----------------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplFuncKeyList 
{
/*----------------------------------------------------------------------------------*/
	{ VK_F1,  _T("F1")	},		{ VK_F2,  _T("F2")  },		{ VK_F3,  _T("F3")	}, 
	{ VK_F4,  _T("F4")	},		{ VK_F5,  _T("F5")  },		{ VK_F6,  _T("F6")	}, 
	{ VK_F7,  _T("F7")  },		{ VK_F8,  _T("F8")  },		{ VK_F9,  _T("F9")	}, 
	{ VK_F10, _T("F10") },		{ VK_F11, _T("F11") },		{ VK_F12, _T("F12") },
/*----------------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplColorName
{
/*-------Color ID----------------------------Color name-----------------------------*/
	{ DEF_COLOR_RED,						_T("Red")							},
	{ DEF_COLOR_GREEN,						_T("Green")							},
	{ DEF_COLOR_YELLOW,						_T("Yellow")						},
	{ DEF_COLOR_BLUE,						_T("Blue")							},
	{ DEF_COLOR_WHITE,						_T("White")							},
	{ DEF_COLOR_BLACK,						_T("Black")							},
	{ DEF_COLOR_PINK,						_T("Pink")							},
	{ DEF_COLOR_SAKURA_PINK,				_T("Sakura pink")					},
	{ DEF_COLOR_ORANGE,						_T("Orange")						},
	{ DEF_COLOR_UMARINE_BLUE,				_T("Ultramarine blue")				},
	{ DEF_COLOR_INDIGO,						_T("Indigo")						},
	{ DEF_COLOR_VIOLET,						_T("Violet")						},
	{ DEF_COLOR_PURPLE,						_T("Purple")						},
	{ DEF_COLOR_SCARLET,					_T("Scarlet")						},
	{ DEF_COLOR_JADE,						_T("Jade")							},
	{ DEF_COLOR_EMERALD,					_T("Emerald")						},
	{ DEF_COLOR_GRAY,						_T("Gray")							},
	{ DEF_COLOR_DARK_GRAY,					_T("Dark gray")						},
	{ DEF_COLOR_BRIGHT_GRAY,				_T("Bright gray")					},
/*----------------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplMsgIconName
{
/*--------Icon ID---------------------------Icon name-------------------------------*/
	{ IDI_MSGICON_APPLICATION,				_T("App")							},
	{ IDI_MSGICON_HAND,						_T("Hand")							},
	{ IDI_MSGICON_QUESTION,					_T("Question")						},
	{ IDI_MSGICON_EXCLAMATION,				_T("Exclamation")					},
	{ IDI_MSGICON_ASTERISK,					_T("Asterisk")						},
	{ IDI_MSGICON_WARNING,					_T("Warning")						},
	{ IDI_MSGICON_ERROR,					_T("Error")							},
	{ IDI_MSGICON_INFORMATION,				_T("Information")					},
/*----------------------------------------------------------------------------------*/
};

static const HOTKEYINFO hklExistedSysHotkeyList[] =
{
/*------Control Key--------------Function Key---------Hotkey description------------*/
	{ MOD_ALT,						VK_F4,			HKEYSET_EXISTED_ALT_F4		},
	{ MOD_ALT,						VK_F8,			HKEYSET_EXISTED_ALT_F8		},
	{ MOD_CONTROL,					VK_F3,			HKEYSET_EXISTED_CTRL_F3		},
	{ MOD_CONTROL,					VK_F4,			HKEYSET_EXISTED_CTRL_F4		},
	{ MOD_CONTROL,					VK_F5,			HKEYSET_EXISTED_CTRL_F5		},
	{ MOD_CONTROL | MOD_WIN,		VK_F4,			HKEYSET_EXISTED_CTRL_WIN_F4 },
/*----------------------------------------------------------------------------------*/
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program, these functions will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace PairFuncs
{
	UINT		  GetPairedID(IDPAIRLIST& idplRef, UINT nID, BOOL bReverse = FALSE);
	UINT		  GetStringID(STRINGPAIRLIST& strplRef, LPCTSTR lpszInput);
	LPCTSTR		  GetPairedString(STRINGPAIRLIST& strplRef, UINT nID, LPTSTR pszResult = NULL);
	LPCSTR		  GetLanguageName(UINT nCurLanguage, LPSTR pszResult = NULL);
	LANGTABLE_PTR LoadLanguageTable(UINT nCurLanguage, LPSTR pszRetLangName = NULL, int* pnSize = NULL);
	LPCTSTR		  GetLanguageString(LANGTABLE_PTR ptLanguage, UINT nID, LPTSTR pszResult = NULL);
};

namespace CoreFuncs
{
	// Power action execution functions (main core)
	BOOL ExecutePowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode);
	BOOL ExecutePowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode);

	// Default data initialization
	void SetDefaultData(PCONFIGDATA pcfgData);
	void SetDefaultData(PSCHEDULEDATA pschData);
	void SetDefaultData(PHOTKEYSETDATA phksData);
	void SetDefaultData(PPWRREMINDERDATA ppwrData);

	// Trace logging functions
	void TraceLog(LPCSTR lpszTraceLogA);
	void TraceLog(LPCTSTR lpszTraceLogW);
	void TraceLogFormat(LPCSTR lpszTraceLogFormat, ...);
	void TraceLogFormat(LPCTSTR lpszTraceLogFormat, ...);
	void TraceDebugInfo(LPCSTR lpszFuncName, LPCSTR lpszFileName, int nLineIndex);

	// Debug logging functions
	void OutputDebugLog(LPCTSTR lpszDebugLog, int nForceStyle = -1);
	void OutputDebugLogFormat(LPCTSTR lpszDebugLogFormat, ...);

	// Trace/debug file logging functions
	BOOL BackupOldLogFile(CString& strFilePath, LPCTSTR lpszLogFileName);
	void WriteTraceErrorLogFile(LPCTSTR lpszLogStringW);
	void WriteTraceDebugLogFile(LPCTSTR lpszLogStringW);
	void WriteDebugInfoLogFile(LPCTSTR lpszLogStringW);
	void WriteTraceNDebugLogFileBase(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW);

	// Message functions
	LRESULT	WaitMessage(UINT nMsg, int nTimeout = DEF_WAITMESSAGE_TIMEOUT);
	void ShowErrorMessage(HWND hWnd, UINT nLanguageID, DWORD dwErrCode, LPARAM lParam = NULL);
	int  DisplayMessageBox(HWND hWnd, LPCTSTR strPrompt, LPCTSTR strCaption, UINT nStyle);

	// Data converting functions
	UINT Sel2Opt(UINT nOptionMacro, UINT nSelection);
	UINT Opt2Sel(UINT nOptionMacro, UINT nCurOption);
	BOOL Text2Time(SYSTEMTIME& stTime, CString strText);
	BOOL Text2TimeBase(SYSTEMTIME& stTime, CString strText);
	void SpinPos2Time(SYSTEMTIME& stTime, int nPos);
	void Time2SpinPos(SYSTEMTIME stTime, int& nPos);

	// Data/control/window processing functions
	int	 GetListCurSel(CListCtrl& pListCtrl);
	HWND FindDebugTestDlg(void);
	void SetFixedCellStyle(CGridCtrl* pGridCtrl, int nRow, int nCol);
	void DrawGridTableRow(CGridCtrl* pGridCtrl, int nRow, int nRowNum, int nColNum, GRIDCTRLCOLFORMAT* apGrdColFormat);

	// Time data processing functions
	SYSTEMTIME	GetCurSysTime(void);
	void		CalcTimeOffset(SYSTEMTIME& stTime, int nOffset);
	BOOL		CheckTimeMatch(SYSTEMTIME timeDest, SYSTEMTIME timePar, int nOffset = 0);
	CString		FormatDispTime(LANGTABLE_PTR pLang, UINT nFormatID, SYSTEMTIME timeVal);
	CString		FormatDispTime(LANGTABLE_PTR pLang, LPCTSTR lpszFormatString, SYSTEMTIME timeVal);

	// String processing functions
	LPCTSTR LoadResourceString(UINT nResStringID);
	BOOL	LoadResourceString(CString& strResult, UINT nResStringID);
	int		GetTokenList(LPTSTR lpszBuff, BUFFER* retBuff, LPCTSTR lpszKeyChars);
	void	UpperEachWord(CString& strInput, BOOL bTrim);
	BOOL	MakeFilePath(CString& strOutput, LPCTSTR lpszDirectory, LPCTSTR lpszFileName, LPCTSTR lpszExtension);
	BOOL	StringValidate(LPCTSTR lpszSrc, DWORD& dwError);

	BOOL	SubString(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd, UINT nSubStringType);
	BOOL	Left(LPCTSTR lpszSrc, CString& strDest, TCHAR tcEnd);
	BOOL	Mid(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd);
	BOOL	Right(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart);

	// Additional functions
	LPCTSTR GetAppPath(void);
	CString	GetProductVersion(BOOL bFullVersion);
	BOOL	GetProductVersion(CString& strFullVersion, CString& strShortVersion);
	BOOL	AddRegistryKey(const REGISTRYINFO& regInfo);
	LPCTSTR MakeRegistryPath(const REGISTRYINFO& regInfo, UINT nRegPathType = REGPATH_FULL, BOOL bIncRootKey = TRUE);

	void	PlaySound(BOOL bSoundEnable, UINT nTypeOfSound);
	BOOL	FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath);
	BOOL	OpenWebURL(LPCTSTR lpszWebUrl);

	LRESULT RunApp(LPCTSTR lpszAppPath, BOOL bRunAsAdmin = FALSE, BOOL bShowFlag = TRUE);
	LRESULT ExecuteCommand(LPCTSTR lpszCmd, BOOL bRunAsAdmin = TRUE, BOOL bShowFlag = TRUE);
	BOOL	CreateAppProcess(LPCWSTR lpszAppPath, LPWSTR lpszCmdLine, UINT nStyle, DWORD& dwErrorCode);

	BOOL	SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode);
	void	DrawButton(CButton*& pButton, UINT nIconID, LPCTSTR lpszButtonTitle = DEF_STRING_EMPTY);
	BOOL	EnumFontNames(std::vector<std::wstring>& fontNames);
	BOOL	ValidateFontName(LPCTSTR lpszFontName);
};

#endif	// ifndef _CORE_H_INCLUDED