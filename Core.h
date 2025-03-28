
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
//						<4> 2024.12.18:		Update to version 3.2
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
//

#define APP_LANGUAGE								0x01
#define APP_LANGUAGE_ENGLISH						((APP_LANGUAGE << 8) + 0x01)
#define APP_LANGUAGE_VIETNAMESE						((APP_LANGUAGE << 8) + 0x02)
#define APP_LANGUAGE_SIMPCHINESE					((APP_LANGUAGE << 8) + 0x03)


// Define app actions
//

#define APP_ACTION									0x02
#define APP_ACTION_NOTHING							((APP_ACTION << 8) + 0x00)
#define APP_ACTION_DISPLAYOFF						((APP_ACTION << 8) + 0x01)
#define APP_ACTION_SLEEP							((APP_ACTION << 8) + 0x02)
#define APP_ACTION_SHUTDOWN							((APP_ACTION << 8) + 0x03)
#define APP_ACTION_RESTART							((APP_ACTION << 8) + 0x04)
#define APP_ACTION_SIGNOUT							((APP_ACTION << 8) + 0x05)
#define APP_ACTION_HIBERNATE						((APP_ACTION << 8) + 0x06)
#define APP_ACTION_SHOWMENU							((APP_ACTION << 8) + 0x07)
#define APP_ACTION_SHOWWINDOW						((APP_ACTION << 8) + 0x08)


// Define app action messages
//

#define APP_MESSAGE									0x03
#define APP_MESSAGE_DISPLAYOFF						SC_MONITORPOWER
#define APP_MESSAGE_SLEEP							((APP_MESSAGE << 8) + 0x01)
#define APP_MESSAGE_HIBERNATE						((APP_MESSAGE << 8) + 0x02)
#define APP_MESSAGE_SHUTDOWN						(EWX_SHUTDOWN | EWX_HYBRID_SHUTDOWN)
#define APP_MESSAGE_POWEROFF						EWX_POWEROFF
#define APP_MESSAGE_REBOOT							EWX_REBOOT
#define APP_MESSAGE_SIGNOUT							EWX_LOGOFF


// Define action type constants
//

#define APP_ACTIONTYPE								0x04
#define APP_ACTIONTYPE_MONITOR						((APP_ACTIONTYPE << 8) + 0x01)
#define APP_ACTIONTYPE_POWER						((APP_ACTIONTYPE << 8) + 0x02)


// Define app error codes
//

#define APP_ERROR									0x05
#define APP_ERROR_SUCCESS							((APP_ERROR << 8) + 0xE000)				// Code: 58624
#define APP_ERROR_FAILED							((APP_ERROR << 8) + 0xE001)				// Code: 58625
#define APP_ERROR_WRONG_ARGUMENT					((APP_ERROR << 8) + 0xE002)				// Code: 58626
#define APP_ERROR_INVALID_FUNCTION					((APP_ERROR << 8) + 0xE003)				// Code: 58627
#define APP_ERROR_FILE_NOT_FOUND					((APP_ERROR << 8) + 0xE004)				// Code: 58628
#define APP_ERROR_PATH_NOT_FOUND					((APP_ERROR << 8) + 0xE005)				// Code: 58629
#define APP_ERROR_ACCESS_DENIED						((APP_ERROR << 8) + 0xE006)				// Code: 58630
#define APP_ERROR_INVALID_HANDLE					((APP_ERROR << 8) + 0xE007)				// Code: 58631
#define APP_ERROR_INVALID_DATA						((APP_ERROR << 8) + 0xE008)				// Code: 58632
#define APP_ERROR_NO_MORE_FILES						((APP_ERROR << 8) + 0xE009)				// Code: 58633
#define APP_ERROR_FILE_EXISTS						((APP_ERROR << 8) + 0xE00A)				// Code: 58634
#define APP_ERROR_CANNOT_MAKE						((APP_ERROR << 8) + 0xE00B)				// Code: 58635
#define APP_ERROR_INVALID_PARAMETER					((APP_ERROR << 8) + 0xE00C)				// Code: 58636
#define APP_ERROR_OPEN_FAILED						((APP_ERROR << 8) + 0xE00D)				// Code: 58637
#define APP_ERROR_BUFFER_OVERFLOW					((APP_ERROR << 8) + 0xE00E)				// Code: 58638
#define APP_ERROR_INVALID_NAME						((APP_ERROR << 8) + 0xE00F)				// Code: 58639
#define APP_ERROR_DIR_NOT_EMPTY						((APP_ERROR << 8) + 0xE010)				// Code: 58640
#define APP_ERROR_FAIL_SHUTDOWN						((APP_ERROR << 8) + 0xE011)				// Code: 58641
#define APP_ERROR_FAIL_RESTART						((APP_ERROR << 8) + 0xE012)				// Code: 58642
#define APP_ERROR_INVALID_ADDRESS					((APP_ERROR << 8) + 0xE013)				// Code: 58643
#define APP_ERROR_APP_INIT_FAILURE					((APP_ERROR << 8) + 0xE014)				// Code: 58644
#define APP_ERROR_CANNOT_LOAD_REGISTRY				((APP_ERROR << 8) + 0xE015)				// Code: 58645
#define APP_ERROR_REGISTRY_QUOTA_LIMIT				((APP_ERROR << 8) + 0xE016)				// Code: 58646
#define APP_ERROR_SYSTEM_SHUTDOWN					((APP_ERROR << 8) + 0xE017)				// Code: 58647
#define APP_ERROR_HIBERNATED						((APP_ERROR << 8) + 0xE018)				// Code: 58648
#define APP_ERROR_RESUME_HIBERNATION				((APP_ERROR << 8) + 0xE019)				// Code: 58649
#define APP_ERROR_WAKE_SYSTEM						((APP_ERROR << 8) + 0xE01A)				// Code: 58650
#define APP_ERROR_BACKUP_REG_FAILED					((APP_ERROR << 8) + 0xE01B)				// Code: 58651
#define APP_ERROR_LOAD_CFG_INVALID					((APP_ERROR << 8) + 0xE01C)				// Code: 58652
#define APP_ERROR_LOAD_CFG_FAILED					((APP_ERROR << 8) + 0xE01D)				// Code: 58653
#define APP_ERROR_LOAD_SCHED_INVALID				((APP_ERROR << 8) + 0xE01E)				// Code: 58654
#define APP_ERROR_LOAD_SCHED_FAILED					((APP_ERROR << 8) + 0xE01F)				// Code: 58655
#define APP_ERROR_LOAD_HKEYSET_INVALID				((APP_ERROR << 8) + 0xE020)				// Code: 58656
#define APP_ERROR_LOAD_HKEYSET_FAILED				((APP_ERROR << 8) + 0xE021)				// Code: 58657
#define APP_ERROR_LOAD_PWRRMD_INVALID				((APP_ERROR << 8) + 0xE022)				// Code: 58658
#define APP_ERROR_LOAD_PWRRMD_FAILED				((APP_ERROR << 8) + 0xE023)				// Code: 58659
#define APP_ERROR_SAVE_CFG_INVALID					((APP_ERROR << 8) + 0xE024)				// Code: 58660
#define APP_ERROR_SAVE_CFG_FAILED					((APP_ERROR << 8) + 0xE025)				// Code: 58661
#define APP_ERROR_SAVE_SCHED_INVALID				((APP_ERROR << 8) + 0xE026)				// Code: 58662
#define APP_ERROR_SAVE_SCHED_FAILED					((APP_ERROR << 8) + 0xE027)				// Code: 58663
#define APP_ERROR_SAVE_HKEYSET_INVALID				((APP_ERROR << 8) + 0xE028)				// Code: 58664
#define APP_ERROR_SAVE_HKEYSET_FAILED				((APP_ERROR << 8) + 0xE029)				// Code: 58665
#define APP_ERROR_SAVE_PWRRMD_INVALID				((APP_ERROR << 8) + 0xE02A)				// Code: 58666
#define APP_ERROR_SAVE_PWRRMD_FAILED				((APP_ERROR << 8) + 0xE02B)				// Code: 58667
#define APP_ERROR_WRITE_LOG_FAILED					((APP_ERROR << 8) + 0xE02C)				// Code: 58668
#define APP_ERROR_OUTPUT_LOG_FAILED					((APP_ERROR << 8) + 0xE02D)				// Code: 58669
#define APP_ERROR_FUNC_GETAPP_FAILED				((APP_ERROR << 8) + 0xE02E)				// Code: 58670
#define APP_ERROR_ALLOC_FAILED_POINTER				((APP_ERROR << 8) + 0xE02F)				// Code: 58671
#define APP_ERROR_ALLOC_FAILED_BUFFER				((APP_ERROR << 8) + 0xE030)				// Code: 58672
#define APP_ERROR_DEBUG_COMMAND						((APP_ERROR << 8) + 0xE031)				// Code: 58673
#define APP_ERROR_UNKNOWN							((APP_ERROR << 8) + 0xE0FF)				// Unknown: 58897


// Define debug command error codes
//

#define APP_ERROR_DBG_SUCCESS						((APP_ERROR_DEBUG_COMMAND << 8) + 0x00)
#define APP_ERROR_DBG_EMPTY							((APP_ERROR_DEBUG_COMMAND << 8) + 0x01)
#define APP_ERROR_DBG_GETAPP_FAILED					((APP_ERROR_DEBUG_COMMAND << 8) + 0x02)
#define APP_ERROR_DBG_CPYBUFF_ALLOC_FAILED			((APP_ERROR_DEBUG_COMMAND << 8) + 0x03)
#define APP_ERROR_DBG_RETBUFF_ALLOC_FAILED			((APP_ERROR_DEBUG_COMMAND << 8) + 0x04)
#define APP_ERROR_DBG_TOKENIZATION_FAILED			((APP_ERROR_DEBUG_COMMAND << 8) + 0x05)
#define APP_ERROR_DBG_INVALID_COMMAND				((APP_ERROR_DEBUG_COMMAND << 8) + 0x06)


// Define function results
//

#define RESULT_SUCCESS								0x0000
#define RESULT_FAILED								0xFFFF


// Define menu item types
//

#define MENU_ITEM									0x06
#define MENU_ITEM_OPTION							((MENU_ITEM << 8) + 0x0001)
#define MENU_ITEM_SEPERATOR_ITEM					((MENU_ITEM << 8) + 0x0002)
#define	MENU_ITEM_POPUP_CHILD_MENU					((MENU_ITEM << 8) + 0x0003)


// Define menu actions
//

#define MENU_ACTION									0x07
#define MENU_ACTION_ABOUT							((MENU_ACTION << 8) + 0x01)
#define MENU_ACTION_HELP							((MENU_ACTION << 8) + 0x02)
#define MENU_ACTION_VIEW_HISTORYLOG					((MENU_ACTION << 8) + 0x03)
#define MENU_ACTION_BACKUP_CONFIG					((MENU_ACTION << 8) + 0x04)
#define MENU_ACTION_VIEW_BAKCONFIG					((MENU_ACTION << 8) + 0x05)
#define MENU_ACTION_OPENDLG_LOGVIEWER				((MENU_ACTION << 8) + 0x06)
#define MENU_ACTION_OPENDLG_SCHEDULE				((MENU_ACTION << 8) + 0x07)
#define MENU_ACTION_OPENDLG_HOTKEYSET				((MENU_ACTION << 8) + 0x08)
#define MENU_ACTION_OPENDLG_PWRREMINDER				((MENU_ACTION << 8) + 0x09)
#define MENU_ACTION_CFG_RELOAD						((MENU_ACTION << 8) + 0x0A)
#define MENU_ACTION_CFG_DEFAULT						((MENU_ACTION << 8) + 0x0B)
#define MENU_ACTION_CFG_ADJUST_MENU					((MENU_ACTION << 8) + 0x0C)
#define MENU_ACTION_CFG_RMB_SHOW_MENU				((MENU_ACTION << 8) + 0x0D)
#define MENU_ACTION_CFG_SHOW_AT_STARTUP				((MENU_ACTION << 8) + 0x0E)
#define MENU_ACTION_CFG_ENABLE_STARTUP				((MENU_ACTION << 8) + 0x0F)
#define MENU_ACTION_CFG_CONFIRM_ACTION				((MENU_ACTION << 8) + 0x10)
#define MENU_ACTION_CFG_SAVE_HISTORY_LOG			((MENU_ACTION << 8) + 0x11)
#define MENU_ACTION_CFG_SAVE_APPEVENT_LOG			((MENU_ACTION << 8) + 0x12)
#define MENU_ACTION_CFG_RUN_AS_ADMIN				((MENU_ACTION << 8) + 0x13)
#define MENU_ACTION_CFG_SHOW_ERROR_MESSAGE			((MENU_ACTION << 8) + 0x14)
#define MENU_ACTION_CFG_NOTIFY_SCHEDULE				((MENU_ACTION << 8) + 0x15)
#define MENU_ACTION_CFG_ALLOW_CANCEL_SCHEDULE		((MENU_ACTION << 8) + 0x16)
#define MENU_ACTION_CFG_ENABLE_HOTKEYS				((MENU_ACTION << 8) + 0x17)
#define MENU_ACTION_CFG_LOCKSTATE_HOTKEYS			((MENU_ACTION << 8) + 0x18)
#define MENU_ACTION_CFG_ENABLE_PWRREMINDER			((MENU_ACTION << 8) + 0x19)
#define MENU_ACTION_LANGUAGE_VIETNAMESE				((MENU_ACTION << 8) + 0x1A)
#define MENU_ACTION_LANGUAGE_ENGLISH				((MENU_ACTION << 8) + 0x1B)
#define MENU_ACTION_LANGUAGE_SIMPCHINESE			((MENU_ACTION << 8) + 0x1C)
#define MENU_ACTION_PWRACTION_DISPLAYOFF			((MENU_ACTION << 8) + 0x1D)
#define MENU_ACTION_PWRACTION_SLEEP					((MENU_ACTION << 8) + 0x1E)
#define MENU_ACTION_PWRACTION_SHUTDOWN				((MENU_ACTION << 8) + 0x1F)
#define MENU_ACTION_PWRACTION_RESTART				((MENU_ACTION << 8) + 0x20)
#define MENU_ACTION_PWRACTION_SIGNOUT				((MENU_ACTION << 8) + 0x21)
#define MENU_ACTION_PWRACTION_HIBERNATE				((MENU_ACTION << 8) + 0x22)
#define MENU_ACTION_PWRACTION_SCHEDULE				((MENU_ACTION << 8) + 0x23)
#define MENU_ACTION_RESTART_APP						((MENU_ACTION << 8) + 0x24)
#define MENU_ACTION_RESTART_AS_ADMIN				((MENU_ACTION << 8) + 0x25)
#define MENU_ACTION_SHOW_WINDOW						((MENU_ACTION << 8) + 0x26)
#define MENU_ACTION_EXIT							((MENU_ACTION << 8) + 0x27)


// Define types of sound
//

#define APP_SOUND									0x08
#define APP_SOUND_ERROR								((APP_SOUND << 8) + 0x0001)
#define APP_SOUND_SUCCESS							((APP_SOUND << 8) + 0x0002)


// Macros definition
//

#define APP_MACRO									0x09
#define APP_MACRO_LEFT_MOUSE						((APP_MACRO << 8) + 0x0001)
#define APP_MACRO_MIDDLE_MOUSE						((APP_MACRO << 8) + 0x0002)
#define APP_MACRO_RIGHT_MOUSE						((APP_MACRO << 8) + 0x0003)
#define APP_MACRO_ACTION_MENU						((APP_MACRO << 8) + 0x0004)
#define APP_MACRO_ACTION_SCHEDULE					((APP_MACRO << 8) + 0x0005)
#define APP_MACRO_ACTION_HOTKEY						((APP_MACRO << 8) + 0x0006)


// Define registry informations
//

#define REG_DEFINE_NONE								0x0a
#define REG_HKEY_CURRENT_USER						HKEY_CURRENT_USER
#define REG_HKEY_LOCAL_MACHINE						HKEY_LOCAL_MACHINE
#define REG_TYPE									((REG_DEFINE_NONE << 4) + 0x001)
#define REG_VALUE_STRING							((REG_TYPE << 4) + 0x0001)
#define REG_VALUE_DWORD								((REG_TYPE << 4) + 0x0002)


// Define custom user-defined messages
//

#define SM_APP_MESSAGE								(WM_USER + 100)
#define SM_WND_MESSAGE								(WM_USER + 200)


// Define application custom messages
//

#define SM_APP_TRAYICON								(SM_APP_MESSAGE + 1)
#define SM_APP_UPDATE_CONFIGDATA					(SM_APP_MESSAGE + 2)
#define SM_APP_UPDATE_SCHEDULEDATA					(SM_APP_MESSAGE + 3)
#define SM_APP_UPDATE_HOTKEYSETDATA					(SM_APP_MESSAGE + 4)
#define SM_APP_UPDATE_PWRREMINDERDATA				(SM_APP_MESSAGE + 5)
#define SM_APP_LOCKSTATE_HOTKEY						(SM_APP_MESSAGE + 6)
#define SM_APP_ERROR_MESSAGE						(SM_APP_MESSAGE + 7)
#define SM_APP_SHOW_ERROR_MSG						(SM_APP_MESSAGE + 8)
#define SM_APP_DEBUG_COMMAND						(SM_APP_MESSAGE + 9)
#define SM_APP_DEBUGCMD_EXEC						(SM_APP_MESSAGE + 10)
#define SM_APP_DEBUG_OUTPUT							(SM_APP_MESSAGE + 11)
#define SM_APP_DEBUGCMD_NOREPLY						(SM_APP_MESSAGE + 12)
	

// Define window custom messages
//

#define SM_WND_SHOWDIALOG							(SM_WND_MESSAGE + 1)
#define SM_WND_DEBUGTEST							(SM_WND_MESSAGE + 2)
#define SM_WND_DEBUGVIEW_CLRSCR						(SM_WND_MESSAGE + 3)
#define SM_WND_DEBUGOUTPUT_DISP						(SM_WND_MESSAGE + 4)


// Define app data types
//

#define APPDATA_CONFIG								0x00001001L
#define APPDATA_SCHEDULE							0x00001002L
#define APPDATA_HOTKEYSET							0x00001004L
#define APPDATA_PWRREMINDER							0x00001008L
#define APPDATA_ALL									(APPDATA_CONFIG|APPDATA_SCHEDULE|APPDATA_HOTKEYSET|APPDATA_PWRREMINDER)


// Define modes
//

#define MODE_INIT									0x000							// Mode: Initialize
#define MODE_VIEW									0x001							// Mode: View
#define MODE_ADD									0x002							// Mode: Add
#define MODE_UPDATE									0x003							// Mode: Update
#define MODE_LOAD									0x004							// Mode: Load
#define MODE_SAVE									0x005							// Mode: Save
#define MODE_DISABLE								0xFFF							// Mode: Disable
#define MODE_HELPVIEW_HELPFILE						0x0a1							// Mode: View help file
#define MODE_HELPVIEW_CHANGELOG						0x0a2							// Mode: View changelog file
#define MODE_BAKREGDATA_MANUAL						0x0a3							// Mode: Registry manually backup
#define MODE_BAKREGDATA_AUTO						0x0a4							// Mode: Registry auto backup
#define MODE_OPENDLG_MODAL							0x0a5							// Modal dialog
#define MODE_OPENDLG_MODELESS						0x0a6							// Modeless dialog


// Define colors
//

#define COLOR_RED									RGB(255,0,0)					// Color: Red
#define COLOR_GREEN									RGB(0,255,0)					// Color: Green
#define COLOR_YELLOW								RGB(255,255,0)					// Color: Yellow
#define COLOR_BLUE									RGB(0,0,255)					// Color: Blue
#define COLOR_WHITE									RGB(255,255,255)				// Color: White
#define COLOR_BLACK									RGB(0,0,0)						// Color: Black
#define COLOR_PINK									RGB(230,190,200)				// Color: Pink
#define COLOR_SAKURA_PINK							RGB(250,224,216)				// Color: Sakura pink
#define COLOR_ORANGE								RGB(255,127,40)					// Color: Orange
#define COLOR_UMARINE_BLUE							RGB(63,72,204)					// Color: Ultramarine blue
#define COLOR_INDIGO								RGB(75,0,130)					// Color: Indigo
#define COLOR_VIOLET								RGB(238,130,238)				// Color: Violet
#define COLOR_PURPLE								RGB(115,43,245)					// Color: Purple
#define COLOR_SCARLET								RGB(255,36,0)					// Color: Scarlet
#define COLOR_JADE									RGB(0,168,207)					// Color: Jade
#define COLOR_EMERALD								RGB(80,200,120)					// Color: Emerald
#define COLOR_GRAY									RGB(200,200,200)				// Color: Gray
#define COLOR_DARK_GRAY								RGB(160,160,160)				// Color: Dark gray
#define COLOR_BRIGHT_GRAY							RGB(240,240,240)				// Color: Bright gray


// Define log types
//

#define LOGTYPE_NONE								0x00							// Not defined
#define LOGTYPE_APP_EVENT							(LOGTYPE_NONE + 0x01)			// App event log
#define LOGTYPE_HISTORY_LOG							(LOGTYPE_NONE + 0x02)			// Action log/history
#define LOGTYPE_TRACE_ERROR							(LOGTYPE_NONE + 0x03)			// Trace error log
#define LOGTYPE_TRACE_DEBUG							(LOGTYPE_NONE + 0x04)			// Trace debug log
#define LOGTYPE_DEBUG_INFO							(LOGTYPE_NONE + 0x05)			// Debug info output log


// Define subdirectory names
//

#define SUBFOLDER_LOG								_T(".\\Log")
#define SUBFOLDER_HELP								_T(".\\Help")
#define SUBFOLDER_BACKUP							_T(".\\Backup")
#define SUBFOLDER_TEMP								_T(".\\Temp")


// Define file names
//

#define FILENAME_APPEXEFILE							_T("PowerPlus3")
#define FILENAME_APPCONFIG							_T("Config")
#define FILENAME_BAKCONFIG							_T("BakConfig")
#define FILENAME_APPEVENT_LOG						_T("AppEventLog_%d%02d%02d")
#define FILENAME_HISTORY_LOG						_T("AppHistory")
#define FILENAME_TRACE_ERROR_LOG					_T("TraceError")
#define FILENAME_TRACE_DEBUG_LOG					_T("TraceDebug")
#define FILENAME_DEBUG_INFO_LOG						_T("DebugInfo")
#define FILENAME_HELP_ENG							_T("English")
#define FILENAME_HELP_VIE							_T("Vietnamese")
#define FILENAME_HELP_CHS							_T("Chinese")
#define FILENAME_CHANGELOG_ENG						_T("Change_log.en")
#define FILENAME_CHANGELOG_VIE						_T("Change_log.vi")
#define FILENAME_CHANGELOG_CHS						_T("Change_log.ch")


// Define file extensions
//

#define FILEEXT_EXEFILE								_T(".exe")						// EXE file
#define FILEEXT_INIFILE								_T(".ini")						// INI file
#define FILEEXT_REGFILE								_T(".reg")						// Registry file
#define FILEEXT_LOGFILE								_T(".log")						// Log file
#define FILEEXT_BAKFILE								_T(".bak")						// Backup file extension
#define FILEEXT_BAKLOGFILE							_T("_%02d.log.bak")				// Backup log file extension
#define FILEEXT_HELPFILE							_T(".hlps")						// Help file


// Define timer IDs
//

#define TIMERID_DEFAULT								0x0100
#define TIMERID_STD_ACTIONSCHEDULE					(TIMERID_DEFAULT + 1)			// Timer ID for Action Schedule feature
#define TIMERID_STD_POWERREMINDER					(TIMERID_DEFAULT + 2)			// Timer ID for Power Reminder feature
#define TIMERID_STD_EVENTSKIPCOUNTER				(TIMERID_DEFAULT + 3)			// Timer ID for Event skip counter
#define TIMERID_RMDMSG_AUTOCLOSE					(TIMERID_DEFAULT + 4)			// Timer ID for Reminder message auto close feature


// Other special definitions
//

#define REG_AFX_PROJECTNAME							_T("PowerPlus3")
#define REG_STARTUP_VALUENAME						REG_AFX_PROJECTNAME
#define PATH_APP_NOTEPAD							_T("C:\\Windows\\notepad.exe")
#define PATH_APP_SYSTEMCMD							_T("C:\\Windows\\System32\\cmd.exe")


// Define special strings
//

#define STRING_EMPTY								_T("")							// Empty string
#define STRING_ENDLINE								_T("\n")						// "End line" string
#define STRING_NEWLINE								_T("\r\n")						// "New line" string (with 'return' character)
#define STRING_NULL									_T("#NULL")						// Null/invalid string
#define STRING_QUOTEFORMAT							_T("\"%s\"")					// Quote string template format
#define STRING_TEXTCHANGEFORMAT						_T("%s -> %s")					// Text change format string template
#define STRING_INTCHANGEFORMAT						_T("%d -> %d")					// Integer number change format string template
#define STRING_FLOATCHANGEFORMAT					_T("%f -> %f")					// Float number change format string template


// Define special symbols
//

#define SYMBOL_DOT									_T(".")							// Dot (.) symbol
#define SYMBOL_COMMA								_T(",")							// Comma (,) symbol
#define SYMBOL_COLON								_T(":")							// Colon (:) symbol
#define SYMBOL_DASH									_T("-")							// Dash (-) symbol
#define SYMBOL_UNDERSCORE							_T("_")							// Underscore (_) symbol
#define SYMBOL_BACKSLASH							_T("\\")						// Backslash (typically used as file/folder path separator)

#define SYMBOL_ARROW								_T("->")						// Arrow symbol
#define SYMBOL_ARROW_EQ								_T("=>")						// Arrow symbol (with equal sign)
#define SYMBOL_OUTPUT								_T(">> ")						// Output sign
#define SYMBOL_INPUT								_T(" <<")						// Input sign

#define SYMBOL_SPACE								_T(" ")							// Space
#define SYMBOL_DOUBLE_SPACE							_T("  ")						// Double spaces
#define SYMBOL_INDENT								_T("\t")						// Indentation (tab character)

#define SYMBOL_ANTE_MERIDIEM						_T("AM")						// Ante meridiem (AM)
#define SYMBOL_POST_MERIDIEM						_T("PM")						// Post meridiem (PM)

#define SYMBOL_CUR_FOLDER							_T(".\\")						// Current folder
#define SYMBOL_PRT_FOLDER							_T("..\\")						// Parent folder


// Define special characters
//

#define CHAR_SPACE									_T(' ')							// Space character
#define CHAR_DOT									_T('.')							// Dot (.) character
#define CHAR_COMMA									_T(',')							// Comma (,) character
#define CHAR_QUOTAMARK								_T('\"')						// 'Quotation mark' character
#define CHAR_TAB									_T('\t')						// Indentation (tab character)

#define CHAR_COLON									_T(':')							// Colon symbol
#define CHAR_DASH									_T('-')							// Dash symbol
#define CHAR_UNDERSCORE								_T('_')							// Underscore symbol
#define CHAR_BACKSLASH								_T('\\')						// Backslash (typically used as file/folder path separator)

#define CHAR_RETURN									_T('\r')						// 'Return' character
#define CHAR_ENDLINE								_T('\n')						// 'Endline' character
#define CHAR_NEWLINE								_T('\r\n')						// 'New line' character
#define CHAR_ENDSTRING								_T('\0')						// Null-termination (end of string)


// Define special numbers and numeric values
//

#define INT_INVALID									-1								// Invalid integer number (equals -1)
#define INT_NULL									0								// Null integer number (equals 0)
#define INT_INFINITE								INFINITE						// Infinite (no limitation)
#define FLOAT_INVALID								-1.0F							// Invalid float number (equals -1.0)
#define FLOAT_NULL									0.0F							// Null float number (equals 0)
#define STRUCT_ZERO									{0}								// Zero-initialized struct data
#define SYSTEMTIME_ZERO								STRUCT_ZERO						// Zero-initialized systemtime data


// Define special values
//

#define VALUE_TRUE									_T("Yes")						// Boolean value: True (Yes)
#define VALUE_FALSE									_T("No")						// Boolean value: False (No)
#define VALUE_UNKNOWN								_T("Unknown")					// Unknown value
#define VALUE_UNDEFINED								_T("Undefined")					// Undefined value


// Define special min/max values
//

#define MIN_SNOOZETIME								1								// Min snooze time: 1 minute
#define MIN_PASSWORD_LENGTH							6								// Min password length: 6 characters

#define MAX_SNOOZETIME								30								// Max snooze time: 30 minutes
#define MAX_DAYS_OF_WEEK							7								// Number of days of week: 7 days
#define MAX_TOKEN_COUNT								50								// Max token number: 50
#define MAX_BUFFER_LENGTH							512								// Max buffer length: 512 characters
#define MAX_STRING_LENGTH							1024							// Max string length: 2KB ~ 1024 characters
#define MAX_TEXT_LENGTH								2097152							// Max text length: 2MB ~ 2097152 characters
#define MAX_PASSWORD_LENGTH							30								// Max password length: 30 characters
#define MAX_DISP_LOGSTRING_LENGTH					20								// Max log string displaying length: 20 characters
#define MAX_BAKFILE_COUNT							100								// Maximum backup file number: 100
#define MAX_LOGFILE_SIZE							1048576							// Max file size: 1MB

#define TIMEOUT_WAIT_MESSAGE						30000							// Wait message timeout (tick-counts): 30s


// Define windows system OS info macros
//

#define WINDOWS_VERSION_NONE						0x00
#define WINDOWS_VERSION_UNKNOWN						WINDOWS_VERSION_NONE			// Unknown version
#define WINDOWS_VERSION_95							(WINDOWS_VERSION_NONE+1)		// Windows 95
#define WINDOWS_VERSION_NT							(WINDOWS_VERSION_NONE+2)		// Windows NT
#define WINDOWS_VERSION_98							(WINDOWS_VERSION_NONE+3)		// Windows 98
#define WINDOWS_VERSION_XP							(WINDOWS_VERSION_NONE+4)		// Windows XP
#define WINDOWS_VERSION_VISTA						(WINDOWS_VERSION_NONE+5)		// Windows Vista
#define WINDOWS_VERSION_7							(WINDOWS_VERSION_NONE+6)		// Windows 7
#define WINDOWS_VERSION_8							(WINDOWS_VERSION_NONE+7)		// Windows 8
#define WINDOWS_VERSION_10							(WINDOWS_VERSION_NONE+8)		// Windows 10
#define WINDOWS_VERSION_11							(WINDOWS_VERSION_NONE+9)		// Windows 11

#define OS_BUILDNUMBER_W7_EARLIEST					6469							// Windows 7 earliest build number: 7700
#define OS_BUILDNUMBER_W7_LATEST					7601							// Windows 7 earliest build number: 9600
#define OS_BUILDNUMBER_W8_EARLIEST					7700							// Windows 8/8.1 earliest build number: 7700
#define OS_BUILDNUMBER_W8_LATEST					9600							// Windows 8/8.1 latest build number: 9600
#define OS_BUILDNUMBER_W10_ORIGINAL					10240							// Windows 10 original release build number: 10240
#define OS_BUILDNUMBER_W11_EARLIEST					21996							// Windows 11 earliest build number: 21996


// Define specific/default values for controls or items
//

#define TIMESPIN_MIN								0								// Equals to 00:00
#define TIMESPIN_MAX								1439							// Equals to 23:59
#define TIMESPIN_DEFAULT							0								// Equals to 00:00

#define OFFSET_WIDTH_VSCRLBR						3								// Vertical scrollbar width offset = 3px
#define OFFSET_WIDTH_LISTCTRL						5								// List control width offset = 5px
#define OFFSET_WIDTH_LISTCTRL_WIN10					16								// List control width offset (on Windows 10) = 16px
#define OFFSET_HEIGHT_LISTCTRL						5								// List control height offset = 5px
#define OFFSET_HEIGHT_LISTCTRL_WIN10				16								// List control height offset (on Windows 10) = 16px
#define OFFSET_WIDTH_LISTCOL_WIN10					10								// List control column width offset (on Windows 10) = 10px

#define LISTCTRL_HEIGHT_HEADER						27								// List control header height = 27px
#define LISTCTRL_HEIGHT_ROW							18								// List control row height = 18px

#define GRIDCTRL_HEIGHT_HEADER						25								// Grid control header height = 25px
#define GRIDCTRL_HEIGHT_HEADER_EX					28								// Grid control header height (extra) = 28px
#define GRIDCTRL_HEIGHT_ROW							23								// Grid control row height = 23px
#define GRIDCTRL_HEIGHT_ROW_EX						25								// Grid control row height (extra) = 25px
#define GRIDCTRL_INDEX_HEADER_ROW					0								// Grid control header row index
#define GRIDCELL_MARGIN_LEFT						3								// Grid cell left margin = 3px


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define special properties and values for program features
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Define properties for RepeatSet data
//

#define DEF_REPEATSET_MIN_SNOOZE					60								// Min snooze interval: 1 minutes
#define DEF_REPEATSET_DEFAULT_SNOOZE				600								// Default snooze interval: 10 minutes
#define DEF_REPEATSET_MAX_SNOOZE					1800							// Max snooze interval: 30 minutes
#define DEF_REPEATSET_DEFAULT_ACTIVEDAYS			0b01111111						// Default repeat: All days of weekss


// Define properties for Action Schedule function
//

#define DEF_SCHEDULE_DEFAULT_ITEMNUM				1								// Default item number: 1
#define DEF_SCHEDULE_MAX_ITEMNUM					100								// Max item number: 100
#define DEF_SCHEDULE_DEFAULT_ITEMID					0x00							// Default item ID: 0
#define DEF_SCHEDULE_MIN_ITEMID						10000							// Min item ID: 10000
#define DEF_SCHEDULE_MAX_ITEMID						19999							// Max item ID: 19999
#define DEF_SCHEDULE_INIT_ACTION					APP_ACTION_DISPLAYOFF			// Init action (for new item): Turn off display

#define DEF_SCHEDULE_ERROR							3240L
#define DEF_SCHEDULE_ERROR_SUCCESS					(DEF_SCHEDULE_ERROR + 1)		// Success (no error)
#define DEF_SCHEDULE_ERROR_ISDEFAULT				(DEF_SCHEDULE_ERROR + 2)		// Item is default (can not remove/delete)
#define DEF_SCHEDULE_ERROR_EMPTY					(DEF_SCHEDULE_ERROR + 3)		// Schedule data is empty
#define DEF_SCHEDULE_ERROR_MAXITEM					(DEF_SCHEDULE_ERROR + 4)		// Schedule data item number reaches maximum limit
#define DEF_SCHEDULE_ERROR_DUPLICATE				(DEF_SCHEDULE_ERROR + 5)		// Item data is duplicated (can not add)
#define DEF_SCHEDULE_ERROR_DUPLICATETIME			(DEF_SCHEDULE_ERROR + 6)		// Item time value is duplicated (can not add)


// Define properties for Power Reminder function
//

#define DEF_PWRREMINDER_MAX_ITEMNUM					100								// Max item number: 100
#define DEF_PWRREMINDER_MIN_ITEMID					10000							// Min item ID: 10000
#define DEF_PWRREMINDER_MAX_ITEMID					19999							// Max item ID: 19999
#define DEF_PWRREMINDER_PREVIEW_TIMEOUT				10								// Default time-out for preview: 10s


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define special function-base, format and expression macros for program, 
//	these macros will be used elsewhere in the programs for special purposes and calculation
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Trace log functions
//

#define TRCFMT										TraceLogFormat
#define TRCLOG(logString)							TraceLog(logString)
#define TRCDBG(func,file,line)						TraceDebugInfo(func,file,line)


// String processing functions
//

#define IS_NOT_EMPTY_STRING(string)					(_tcscmp(string, STRING_EMPTY))
#define IS_EMPTY_STRING(string)						(!IS_NOT_EMPTY_STRING(string))
#define IS_NOT_NULL_STRING(string)					(_tcscmp(string, STRING_NULL))
#define IS_NULL_STRING(string)						(!IS_NOT_NULL_STRING(string))

#define MAKEANSI(string)							(CW2A(string).m_psz)
#define MAKEUNICODE(string)							(CA2W(string).m_psz)
#define RESOURCESTRING(resourceid)					LoadResourceString(resourceid)

#define MAKE_WPARAM_STRING(string)					((WPARAM)(_tcslen(string)))
#define MAKE_LPARAM(type, data)						(reinterpret_cast<LPARAM>((type)data))
#define MAKE_LPARAM_STRING(string)					MAKE_LPARAM(LPCTSTR, string)
#define LPARAM_STATIC_CAST(type, lParam)			(static_cast<type>(lParam))
#define LPARAM_REINTERPRET_CAST(type, lParam)		(reinterpret_cast<type>(lParam))
#define LPARAM_TO_STRING(lParam)					LPARAM_REINTERPRET_CAST(LPCTSTR, lParam)


// Special expressions
//

#define GET_HANDLE_MAINWND()						(AfxGetMainWnd()->GetSafeHwnd())
#define IS_PRESSED(keycode)							(0x8000 & ::GetKeyState(keycode))
#define __FILENAME__								(strrchr("\\" __FILE__, '\\') + 1)


// Time value processing expressions
//

#define FORMAT_REG_TIME(systime)					(INT((systime.wHour * 100) + systime.wMinute))
#define GET_REGTIME_HOUR(time)						(WORD(time / 100))
#define GET_REGTIME_MINUTE(time)					(WORD(time % 100))
#define TIME_TO_SECONDS(time)						(INT((time.wHour * 3600) + (time.wMinute * 60) + time.wSecond))
#define GET_HOUR(nTotalSecs)						(WORD(nTotalSecs / 3600))
#define GET_MINUTE(nTotalSecs)						(WORD((nTotalSecs % 3600) / 60))
#define GET_SECOND(nTotalSecs)						(WORD((nTotalSecs % 3600) % 60))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define commands for special functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Command runas flags
//

#define COMMAND_FLAG_RUNAS							_T("runas")
#define COMMAND_FLAG_OPEN							_T("open")


// Registry commands
//

#define COMMAND_REGISTRY_EXPORT						_T("\"reg.exe export \"%s\" \"%s\"\" /y")


// Enable/disable startup as admin
//

#define COMMAND_REGISTER_RUNASADMIN					_T("schtasks /create /sc onlogon /tn %s /rl highest /tr \"%s\" /f")
#define COMMAND_UNREGISTER_RUNASADMIN				_T("schtasks /delete /tn %s /f")


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum datas for program, these datas will be used elsewhere in the programs
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////// ********************
// 
// Flag values - use for numeric and boolean flag-base variables
//
//////////////////// ********************

typedef enum eFLAG {
	FLAG_OFF = 0,						// Flag OFF
	FLAG_ON	 = 1,						// Flag ON
} FLAG;


//////////////////// ********************
// 
// Application exit codes - use for PostQuitMessage function
//
//////////////////// ********************

typedef enum eAPPEXITCODE {
	EXITCODE_NORMAL = 0,				// Normal exitting (no reason)
	EXITCODE_EXITBUTTON,				// Exit by pressing [Exit] button
	EXITCODE_NOTIFYMENU,				// Exit by selecting "Exit App" from notify menu selection
	EXITCODE_RESTARTAPP,				// Exit triggerred by Restart function
	EXITCODE_DEBUGCOMMAND,				// Exit by debug command
} APPEXITCODE;


//////////////////// ********************
// 
// Return flag value - use when return a flag from a child dialog
//
//////////////////// ********************

typedef enum eRETFLAG {
	RETFLAG_INVALID = -1,				// Return flag: Invalid
	RETFLAG_OK,							// Return flag: OK
	RETFLAG_CANCEL,						// Return flag: Cancel
	RETFLAG_UPDATE,						// Return flag: Update
	RETFLAG_CLOSE,						// Return flag: Close
} RETFLAG;


//////////////////// ********************
// 
// Days of week - use in time/date calculation and display
//
//////////////////// ********************

typedef enum eDAYOFWEEK {
	SUNDAY,								// Sunday
	MONDAY,								// Monday
	TUESDAY,							// Tuesday
	WEDNESDAY,							// Wednesday
	THURSDAY,							// Thursday
	FRIDAY,								// Friday
	SATURDAY,							// Saturday
} DAYOFWEEK;


//////////////////// ********************
// 
// System events - use in system event time tracing and logging
//
//////////////////// ********************

typedef enum eSYSTEMEVENTID {
	SYSEVT_SUSPEND = 0,					// System suspend event
	SYSEVT_WAKEUP,						// System wakeup event
	SYSEVT_SESSIONEND,					// Session end event
} SYSTEMEVENTID;


//////////////////// ********************
// 
// App option IDs - use to get/set application data option values
//
//////////////////// ********************

typedef enum eAPPOPTIONID {
	OPTIONID_INVALID = -1,				// *** Invalid option ***
	OPTIONID_LMB_ACTION = 0,			// Left mouse button action
	OPTIONID_MMB_ACTION,				// Middle mouse button action
	OPTIONID_RMB_ACTION,				// Right mouse button action
	OPTIONID_RMB_SHOW_MENU,				// Right mouse button: Only show menu
	OPTIONID_LANGUAGE_ID,				// Language setting
	OPTIONID_CUR_DISP_LANGUAGE,			// Currently displaying language
	OPTIONID_SHOW_DLG_AT_STARTUP,		// Show dialog at startup
	OPTIONID_STARTUP_ENABLE,			// Startup with Windows
	OPTIONID_CONFIRM_ACTION,			// Show confirm message before doing action
	OPTIONID_SAVE_APP_EVENT_LOG,		// Save app event log
	OPTIONID_SAVE_HISTORY_LOG,			// Save action history log
	OPTIONID_RUN_AS_ADMIN,				// Run with admin privileges
	OPTIONID_SHOW_ERROR_MSG,			// Show action error message
	OPTIONID_NOTIFY_SCHEDULE,			// Show notify tip for schedule action
	OPTIONID_ALLOW_CANCEL_SCHEDULE,		// Allow canceling schedule when notify
	OPTIONID_ENABLE_HOTKEYSET,			// Enable background action hotkeys
	OPTIONID_LOCK_STATE_HOTKEY,			// Allow background hotkeys on lockscreen
	OPTIONID_ENABLE_PWRREMINDER,		// Enable Power Peminder feature
	OPTIONID_SCHEDULE_ACTIVE,			// Default schedule active state
	OPTIONID_SCHEDULE_ACTION,			// Default schedule action ID
	OPTIONID_SCHEDULE_REPEAT			// Default schedule repeat option
} APPOPTIONID;


//////////////////// ********************
// 
// App flag IDs - use to get/set application flag values
//
//////////////////// ********************

typedef enum eAPPFLAGID {
	FLAGID_INVALID = -1,				// *** Invalid flag ID ***
	FLAGID_CHANGE_FLAG = 0,				// Data/setting change flag
	FLAGID_READ_ONLY_MODE,				// Read-only mode
	FLAGID_LOCK_STATE,					// Lock state
	FLAGID_FORCE_CLOSING,				// Force closing by request
	FLAGID_USE_ESCAPE,					// Use Escape key
	FLAGID_USE_ENTER,					// Use Enter key
	FLAGID_BKGRDCLR_SET,				// Dialog background color is set
	FLAGID_TEXTCLR_SET,					// Dialog text color is set
	FLAGID_MIN_SIZE_SET,				// Dialog minimum size is set
	FLAGID_MAX_SIZE_SET,				// Dialog maximum size is set
	FLAGID_TOPMOST_SET,					// Dialog top-most position is set
	FLAGID_INIT_SOUND_SET,				// Dialog initialize sound is set
	FLAGID_DLG_EXPANDED,				// Dialog expanded/collapsed
	FLAGID_NOTIFY_ICON_SHOWED,			// Notify icon showing flag
	FLAGID_HOTKEY_REGISTERED,			// Hotkey registered
	FLAGID_RESTART_AS_ADMIN,			// Restart as admin flag
	FLAGID_PWRBROADCAST_SKIP_COUNT,		// Power Broadcase event skip counter
	FLAGID_WTSSESSIONNOTIFY_REG,		// WTS Session Change State Notification registered
} APPFLAGID;


//////////////////// ********************
// 
// App feature item category IDs
//
//////////////////// ********************

typedef enum eFEATUREITEMCATEID {
	FID_POWERACTION = 0,				// Power action
	FID_SCHEDULEITEM,					// Action schedule item
	FID_HOTKEYITEM,						// HotkeySet item
	FID_PWRREMINDERITEM,				// Power Reminder item
} FEATUREITEMCATEID;


//////////////////// ********************
// 
// Hotkey IDs - use for HotkeySet features
//
//////////////////// ********************

typedef enum eHOTKEYID {
	HKID_DISPLAYOFF = 0x1a01,			// Turn off display
	HKID_SLEEP,							// Sleep
	HKID_SHUTDOWN,						// Shut down
	HKID_RESTART,						// Restart
	HKID_SIGNOUT,						// Sign out
	HKID_HIBERNATE,						// Hibernate
} HOTKEYID;


//////////////////// ********************
// 
// Power Reminder event IDs - use for Power Reminder features
//
//////////////////// ********************

typedef enum ePWRREMINDEREVENT {
	PREVT_AT_SETTIME = 0x1b01,			// At set time
	PREVT_AT_APPSTARTUP,				// At app startup
	PREVT_AT_SYSWAKEUP,					// At system wake
	PREVT_AT_BFRPWRACTION,				// Before power action
	PREVT_AT_PWRACTIONWAKE,				// Wake after action
	PREVT_AT_APPEXIT,					// Before app exit
} PWRREMINDEREVENT;


//////////////////// ********************
// 
// Power Reminder message styles - use for Power Reminder features
//
//////////////////// ********************

typedef enum ePWRREMIDERSTYLE {
	PRSTYLE_MSGBOX = 0x1c01,			// Message Box
	PRSTYLE_DIALOG,						// Dialog Box
} PWRREMINDERSTYLE;


//////////////////// ********************
// 
// List view column size units
//
//////////////////// ********************

typedef enum eLVCOLSIZEUNIT {
	COLSIZE_PIXEL = 0,					// Pixel
	COLSIZE_PERCENT,					// Percent (of frame width)
} LVCOLSIZEUNIT;


//////////////////// ********************
// 
// Grid table column style
//
//////////////////// ********************

typedef enum eGRIDCOLSTYLE {
	COLSTYLE_FIXED = 0,					// Fixed cell
	COLSTYLE_CHECKBOX,					// Checkbox cell
	COLSTYLE_NORMAL,					// Normal cell
} GRIDCOLSTYLE;


//////////////////// ********************
// 
// LogViewer table column IDs
//
//////////////////// ********************

typedef enum eLOGVWERTABLECOLID {
	LGVCOL_ID_DATETIME = 0,				// Date/Time
	LGVCOL_ID_CATEGORY,					// Event ID (category ID)
	LGVCOL_ID_DESCRIPTION,				// Additional description
} LOGVWERTABLECOLID;


//////////////////// ********************
// 
// Schedule data table column IDs
//
//////////////////// ********************

typedef enum eSCHTABLECOLID {
	SCHCOL_ID_INDEX = 0,				// Index
	SCHCOL_ID_STATE,					// Enable/active state
	SCHCOL_ID_ACTIONID,					// Action ID
	SCHCOL_ID_TIMEVALUE,				// Time value
	SCHCOL_ID_REPEAT,					// Repeat daily
} SCHTABLECOLID;


//////////////////// ********************
// 
// HotkeySet table column IDs
//
//////////////////// ********************

typedef enum eHKEYTABLECOLID {
	HKSCOL_ID_STATE = 0,				// Enable state
	HKSCOL_ID_HKACTIONID,				// Hotkey action ID
	HKSCOL_ID_KEYSTROKES,				// Keystrokes
} HKEYTABLECOLID;


//////////////////// ********************
// 
// Power Reminder table column IDs
//
//////////////////// ********************

typedef enum ePWRTABLECOLID {
	PWRCOL_ID_INDEX = 0,				// Index
	PWRCOL_ID_STATE,					// Enable state
	PWRCOL_ID_ITEMID,					// Item ID
	PWRCOL_ID_MESSAGE,					// Message content
	PWRCOL_ID_EVENTID,					// Event ID
	PWRCOL_ID_STYLE,					// Reminder style
	PWRCOL_ID_REPEAT					// Repeat daily
} PWRTABLECOLID;


//////////////////// ********************
// 
// Registry path type
//
//////////////////// ********************

typedef enum eREGPATHTYPE {
	REGPATH_FULL = 0,					// Full path
	REGPATH_ROOTKEY,					// Root key only
	REGPATH_SUBPATH,					// Including sub-key path
	REGPATH_PROFILEKEY,					// Including profile key name
	REGPATH_APPNAME,					// Including app name
	REGPATH_SECTIONNAME,				// Including section name
	REGPATH_KEYNAME,					// Including key name
} REGPATHTYPE;


//////////////////// ********************
// 
// Registry value type
//
//////////////////// ********************

typedef enum eREGVALUETYPE {
	REGTYPE_NONE = 0,					// Undefined type
	REGTYPE_STRING,						// String value
	REGTYPE_DWORD32,					// DWORD (32-bit) value
	REGTYPE_QWORD64,					// QWORD (64-bit) value
	REGTYPE_MULTISTRING,				// Multi-string value
} REGVALUETYPE;


//////////////////// ********************
// 
// String validation error codes
//
//////////////////// ********************

typedef enum eSTRVALIDERR {
	STRVAL_ERR_NORMAL = 0,				// Normal/valid string
	STRVAL_ERR_EMPTY,					// Empty string
	STRVAL_ERR_OVERSIZE,				// Character number overlimit
	STRVAL_ERR_INVALIDCHAR,				// Contains invalid character(s)
	STRVAL_ERR_UNKNOWN,					// Unknown
} STRVALIDERR;


//////////////////// ********************
// 
// File types (view file mode)
//
//////////////////// ********************

typedef enum eFILETYPE {
	FILETYPE_TEXT = 0,					// Text files
	FILETYPE_IMAGE,						// Image files
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
	BOOL	bLockStateHotkey;										// Allow background hotkeys on lockscreen
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

using SCHEDULEITEMLIST = CArray<SCHEDULEITEM, SCHEDULEITEM>;

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

using HOTKEYSETITEMLIST = CArray<HOTKEYSETITEM, HOTKEYSETITEM>;

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

using PWRREMINDERITEMLIST = CArray<PWRREMINDERITEM, PWRREMINDERITEM>;

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
	QWORD			qwValue;								// QWORD (64-bit) value
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
	void SetQWordValue(QWORD);								// Set QWORD (64-bit) value
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
	// Member variables
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
	// Member variables
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
	// Member variables
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
	// Member variables
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
	// Member variables
	UINT nActionDefID;										// Action ID
	UINT nActionNameID;										// Action Name ID
	UINT nActionMsgID;										// Action message ID
	UINT nSchedNotifyID;									// Schedule notify message ID
	UINT nNotifyTipID;										// Notify file tip ID
	UINT nBalloonTipID;										// Balloon tip ID
} ACTIONDEF, *PACTIONDEF;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	IDMAPTABLE
//					ACTIONDEFCOMBTABLE
//  Description:	Data mapping table
//  Derivered from: C++ vector type
//
//////////////////////////////////////////////////////////////////////////

typedef std::vector<IDPAIR>		IDMAPTABLE;
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
	// Member variables
	INT		nLength;										// Buffer length
	TCHAR	tcToken[MAX_BUFFER_LENGTH];						// Buffer token
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
	// Member variables
	BOOL bRequest;											// Request to restart
	BOOL bAdminCheck;										// Check if already running as admin
	BOOL bNotAdminShowMsg;									// If not admin, not show check message
	BOOL bIsAdminDoNothing;									// If already running as admin, do nothing
	BOOL bShowMsgWhenDeny;									// Show message when denied
	BOOL bResetFlag;										// Reset flag when denied
} RESTARTREQ, *PRESTARTREQ;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	SUBSTRING
//  Description:	Using for getting substrings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagSUBSTRING
{
	// Member variables
	CString strLeft;										// Left part
	CString strMid;											// Middle part
	CString strRight;										// Right part

	// Constructor
	tagSUBSTRING();											// Default constructor
	tagSUBSTRING(const tagSUBSTRING&);						// Copy constructor

	// Operator
	tagSUBSTRING& operator=(const tagSUBSTRING&);			// Copy assignment operator

	// Member functions
	void Copy(const tagSUBSTRING&);							// Copy data
	void RemoveAll(void);									// Remove all data
	BOOL IsEmpty(void) const;								// Check if data is empty

	LPCTSTR	Left(void) const;								// Get left part
	LPCTSTR	Mid(void) const;								// Get middle part
	LPCTSTR	Right(void) const;								// Get right part
} SUBSTRING, *PSUBSTRING;

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
//	Define global static data tables for program, these data will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableActionName
//  Description:	Using for pairing action macro IDs and action name IDs
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableActionName
{
/*----------Action ID----------------------------Action Name ID---------------------*/
	{ APP_ACTION_NOTHING,					ACTION_NAME_NOTHING					},
	{ APP_ACTION_DISPLAYOFF,				ACTION_NAME_DISPLAYOFF				},
	{ APP_ACTION_SLEEP,						ACTION_NAME_SLEEP					},
	{ APP_ACTION_SHUTDOWN,					ACTION_NAME_SHUTDOWN				},
	{ APP_ACTION_RESTART,					ACTION_NAME_RESTART					},
	{ APP_ACTION_SIGNOUT,					ACTION_NAME_SIGNOUT					},
	{ APP_ACTION_HIBERNATE,					ACTION_NAME_HIBERNATE				},
	{ APP_ACTION_SHOWMENU,					ACTION_NAME_SHOWMENU				},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableActionMsg
//  Description:	Using for pairing action macro IDs and action message IDs
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableActionMsg
{
/*----------Action ID---------------------------Message String ID-------------------*/
	{ APP_ACTION_DISPLAYOFF,				MESSAGE_ACTION_DISPLAYOFF			},
	{ APP_ACTION_SLEEP,						MESSAGE_ACTION_SLEEP				},
	{ APP_ACTION_SHUTDOWN,					MESSAGE_ACTION_SHUTDOWN				},
	{ APP_ACTION_RESTART,					MESSAGE_ACTION_RESTART				},
	{ APP_ACTION_SIGNOUT,					MESSAGE_ACTION_SIGNOUT				},
	{ APP_ACTION_HIBERNATE,					MESSAGE_ACTION_HIBERNATE			},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableNotifyTip
//  Description:	Using for pairing action macro IDs and string IDs for
//					notify icon tip text
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableNotifyTip
{
/*---------Action ID---------------------------Notify String ID---------------------*/
	{ APP_ACTION_DISPLAYOFF,				NOTIFY_TIP_DISPLAYOFF				},
	{ APP_ACTION_SLEEP,						NOTIFY_TIP_SLEEP					},
	{ APP_ACTION_SHUTDOWN,					NOTIFY_TIP_SHUTDOWN					},
	{ APP_ACTION_RESTART,					NOTIFY_TIP_RESTART					},
	{ APP_ACTION_SIGNOUT,					NOTIFY_TIP_SIGNOUT					},
	{ APP_ACTION_HIBERNATE,					NOTIFY_TIP_HIBERNATE				},
	{ APP_ACTION_SHOWMENU,					NOTIFY_TIP_SHOWMENU					},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idplBalloonTip
//  Description:	Using for pairing action macro IDs and string IDs for
//					notify icon balloon tip text
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableBalloonTip
{
/*---------Action ID---------------------------Balloon String ID--------------------*/
	{ APP_ACTION_DISPLAYOFF,				BALLOON_TIP_DISPLAYOFF				},
	{ APP_ACTION_SLEEP,						BALLOON_TIP_SLEEP					},
	{ APP_ACTION_SHUTDOWN,					BALLOON_TIP_SHUTDOWN				},
	{ APP_ACTION_RESTART,					BALLOON_TIP_RESTART					},
	{ APP_ACTION_SIGNOUT,					BALLOON_TIP_SIGNOUT					},
	{ APP_ACTION_HIBERNATE,					BALLOON_TIP_HIBERNATE				},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableErrorCode
//  Description:	Using for pairing application-defined error codes and
//					system-defined error codes
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableErrorCode
{
/*----------Action ID----------------------------Action Name ID---------------------*/
	{ APP_ERROR_SUCCESS,					ERROR_SUCCESS						},
	{ APP_ERROR_INVALID_FUNCTION,			ERROR_INVALID_FUNCTION				},
	{ APP_ERROR_FILE_NOT_FOUND,				ERROR_FILE_NOT_FOUND				},
	{ APP_ERROR_PATH_NOT_FOUND,				ERROR_PATH_NOT_FOUND				},
	{ APP_ERROR_ACCESS_DENIED,				ERROR_ACCESS_DENIED					},
	{ APP_ERROR_INVALID_HANDLE,				ERROR_INVALID_HANDLE				},
	{ APP_ERROR_INVALID_DATA,				ERROR_INVALID_DATA					},
	{ APP_ERROR_NO_MORE_FILES,				ERROR_NO_MORE_FILES					},
	{ APP_ERROR_FILE_EXISTS,				ERROR_FILE_EXISTS					},
	{ APP_ERROR_CANNOT_MAKE,				ERROR_CANNOT_MAKE					},
	{ APP_ERROR_INVALID_PARAMETER,			ERROR_INVALID_PARAMETER				},
	{ APP_ERROR_OPEN_FAILED,				ERROR_OPEN_FAILED					},
	{ APP_ERROR_BUFFER_OVERFLOW,			ERROR_BUFFER_OVERFLOW				},
	{ APP_ERROR_INVALID_NAME,				ERROR_INVALID_NAME					},
	{ APP_ERROR_DIR_NOT_EMPTY,				ERROR_DIR_NOT_EMPTY					},
	{ APP_ERROR_FAIL_SHUTDOWN,				ERROR_FAIL_SHUTDOWN					},
	{ APP_ERROR_FAIL_RESTART,				ERROR_FAIL_RESTART					},
	{ APP_ERROR_INVALID_ADDRESS,			ERROR_INVALID_ADDRESS				},
	{ APP_ERROR_APP_INIT_FAILURE,			ERROR_APP_INIT_FAILURE				},
	{ APP_ERROR_CANNOT_LOAD_REGISTRY,		ERROR_CANNOT_LOAD_REGISTRY_FILE		},
	{ APP_ERROR_REGISTRY_QUOTA_LIMIT,		ERROR_REGISTRY_QUOTA_LIMIT			},
	{ APP_ERROR_SYSTEM_SHUTDOWN,			ERROR_SYSTEM_SHUTDOWN				},
	{ APP_ERROR_HIBERNATED,					ERROR_HIBERNATED					},
	{ APP_ERROR_RESUME_HIBERNATION,			ERROR_RESUME_HIBERNATION			},
	{ APP_ERROR_WAKE_SYSTEM,				ERROR_WAKE_SYSTEM					},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableErrorMessage
//  Description:	Using for pairing application-defined error codes and
//					error message string IDs
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableErrorMessage
{
/*----------Action ID----------------------------Action Name ID---------------------*/
	{ APP_ERROR_SUCCESS,					INT_NULL,							},
	{ APP_ERROR_FAILED,						MSGBOX_ERROR_FAILED					},
	{ APP_ERROR_WRONG_ARGUMENT,				MSGBOX_ERROR_WRONG_ARGUMENT			},
	{ APP_ERROR_INVALID_FUNCTION,			MSGBOX_ERROR_INVALID_FUNCTION		},
	{ APP_ERROR_FILE_NOT_FOUND,				MSGBOX_ERROR_FILE_NOT_FOUND			},
	{ APP_ERROR_PATH_NOT_FOUND,				MSGBOX_ERROR_PATH_NOT_FOUND			},
	{ APP_ERROR_ACCESS_DENIED,				MSGBOX_ERROR_ACCESS_DENIED			},
	{ APP_ERROR_INVALID_HANDLE,				MSGBOX_ERROR_INVALID_HANDLE			},
	{ APP_ERROR_INVALID_DATA,				MSGBOX_ERROR_INVALID_DATA			},
	{ APP_ERROR_NO_MORE_FILES,				MSGBOX_ERROR_NO_MORE_FILES			},
	{ APP_ERROR_FILE_EXISTS,				MSGBOX_ERROR_FILE_EXISTS			},
	{ APP_ERROR_CANNOT_MAKE,				MSGBOX_ERROR_CANNOT_MAKE			},
	{ APP_ERROR_INVALID_PARAMETER,			MSGBOX_ERROR_INVALID_PARAMETER		},
	{ APP_ERROR_OPEN_FAILED,				MSGBOX_ERROR_OPEN_FAILED			},
	{ APP_ERROR_BUFFER_OVERFLOW,			MSGBOX_ERROR_BUFFER_OVERFLOW		},
	{ APP_ERROR_INVALID_NAME,				MSGBOX_ERROR_INVALID_NAME			},
	{ APP_ERROR_DIR_NOT_EMPTY,				MSGBOX_ERROR_DIR_NOT_EMPTY			},
	{ APP_ERROR_FAIL_SHUTDOWN,				MSGBOX_ERROR_FAIL_SHUTDOWN			},
	{ APP_ERROR_FAIL_RESTART,				MSGBOX_ERROR_FAIL_RESTART			},
	{ APP_ERROR_INVALID_ADDRESS,			MSGBOX_ERROR_INVALID_ADDRESS		},
	{ APP_ERROR_APP_INIT_FAILURE,			MSGBOX_ERROR_APP_INIT_FAILURE		},
	{ APP_ERROR_CANNOT_LOAD_REGISTRY,		MSGBOX_ERROR_CANNOT_LOAD_REGISTRY	},
	{ APP_ERROR_REGISTRY_QUOTA_LIMIT,		MSGBOX_ERROR_REGISTRY_QUOTA_LIMIT	},
	{ APP_ERROR_SYSTEM_SHUTDOWN,			MSGBOX_ERROR_SYSTEM_SHUTDOWN		},
	{ APP_ERROR_HIBERNATED,					MSGBOX_ERROR_HIBERNATED				},
	{ APP_ERROR_RESUME_HIBERNATION,			MSGBOX_ERROR_RESUME_HIBERNATION		},
	{ APP_ERROR_BACKUP_REG_FAILED,			MSGBOX_ERROR_BACKUP_REG_FAILED		},
	{ APP_ERROR_LOAD_CFG_INVALID,			MSGBOX_ERROR_LOAD_CFG_FAILED		},
	{ APP_ERROR_LOAD_CFG_FAILED,			MSGBOX_ERROR_LOAD_CFG_FAILED		},
	{ APP_ERROR_SAVE_CFG_INVALID,			MSGBOX_ERROR_SAVE_CFG_FAILED		},
	{ APP_ERROR_SAVE_CFG_FAILED,			MSGBOX_ERROR_SAVE_CFG_FAILED		},
	{ APP_ERROR_LOAD_SCHED_INVALID,			MSGBOX_ERROR_LOAD_SCHED_FAILED		},
	{ APP_ERROR_LOAD_SCHED_FAILED,			MSGBOX_ERROR_LOAD_SCHED_FAILED		},
	{ APP_ERROR_SAVE_SCHED_INVALID,			MSGBOX_ERROR_SAVE_SCHED_FAILED		},
	{ APP_ERROR_SAVE_SCHED_FAILED,			MSGBOX_ERROR_SAVE_SCHED_FAILED		},
	{ APP_ERROR_LOAD_HKEYSET_INVALID,		MSGBOX_ERROR_LOAD_HKEYSET_FAILED	},
	{ APP_ERROR_LOAD_HKEYSET_FAILED,		MSGBOX_ERROR_LOAD_HKEYSET_FAILED	},
	{ APP_ERROR_SAVE_HKEYSET_INVALID,		MSGBOX_ERROR_SAVE_HKEYSET_FAILED	},
	{ APP_ERROR_SAVE_HKEYSET_FAILED,		MSGBOX_ERROR_SAVE_HKEYSET_FAILED	},
	{ APP_ERROR_LOAD_PWRRMD_INVALID,		MSGBOX_ERROR_LOAD_PWRRMD_FAILED		},
	{ APP_ERROR_LOAD_PWRRMD_FAILED,			MSGBOX_ERROR_LOAD_PWRRMD_FAILED		},
	{ APP_ERROR_SAVE_PWRRMD_INVALID,		MSGBOX_ERROR_SAVE_PWRRMD_FAILED		},
	{ APP_ERROR_SAVE_PWRRMD_FAILED,			MSGBOX_ERROR_SAVE_PWRRMD_FAILED		},
	{ APP_ERROR_WRITE_LOG_FAILED,			MSGBOX_ERROR_WRITE_LOG_FAILED		},
	{ APP_ERROR_OUTPUT_LOG_FAILED,			MSGBOX_ERROR_OUTPUT_LOG_FAILED		},
	{ APP_ERROR_UNKNOWN,					MSGBOX_ERROR_UNKNOWN				},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idTableSchedNotifyMsg
//  Description:	Using for pairing action macro IDs and message box
//					string IDs for Notify Schedule function
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableSchedNotifyMsg
{
/*---------Action ID------------------------------Message String ID-----------------*/
	{ APP_ACTION_DISPLAYOFF,				MESSAGE_SCHEDNOTIFY_DISPLAYOFF		},
	{ APP_ACTION_SLEEP,						MESSAGE_SCHEDNOTIFY_SLEEP			},
	{ APP_ACTION_SHUTDOWN,					MESSAGE_SCHEDNOTIFY_SHUTDOWN		},
	{ APP_ACTION_RESTART,					MESSAGE_SCHEDNOTIFY_RESTART			},
	{ APP_ACTION_SIGNOUT,					MESSAGE_SCHEDNOTIFY_SIGNOUT			},
	{ APP_ACTION_HIBERNATE,					MESSAGE_SCHEDNOTIFY_HIBERNATE		},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idplHKActionID
//  Description:	Using for pairing Hotkey ID and action macro IDs
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableHKActionID
{
/*----HotKey Action ID----------------------------Action ID-------------------------*/
	{ HKID_DISPLAYOFF,						APP_ACTION_DISPLAYOFF				},
	{ HKID_SLEEP,							APP_ACTION_SLEEP					},
	{ HKID_SHUTDOWN,						APP_ACTION_SHUTDOWN					},
	{ HKID_RESTART,							APP_ACTION_RESTART					},
	{ HKID_SIGNOUT,							APP_ACTION_SIGNOUT					},
	{ HKID_HIBERNATE,						APP_ACTION_HIBERNATE				},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idplPwrReminderEvt
//  Description:	Using for pairing Power Reminder event IDs and string IDs 
//					which will display in Power Reminder data table
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTablePwrReminderEvt
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

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idplPwrReminderStyle
//  Description:	Using for pairing Power Reminder style IDs and string IDs 
//					which will display in Power Reminder data table
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTablePwrReminderStyle
{
/*-----Style ID--------------------------------Style String ID----------------------*/
	{ PRSTYLE_MSGBOX,						PWRRMD_STYLE_MESSAGEBOX				},
	{ PRSTYLE_DIALOG,						PWRRMD_STYLE_DIALOG					},
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		idplDayOfWeek
//  Description:	Using for pairing day-of-week macro IDs and title IDs
//  Table type:		IDMAPTABLE
//
//////////////////////////////////////////////////////////////////////////

static IDMAPTABLE idTableDayOfWeek
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

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		strTableFuncKeyList
//  Description:	Using for pairing function key macros and key names
//  Table type:		STRINGTABLE
//
//////////////////////////////////////////////////////////////////////////

static STRINGTABLE strTableFuncKeyList 
{
/*----------------------------------------------------------------------------------*/
	{ VK_F1,  _T("F1")	},		{ VK_F2,  _T("F2")  },		{ VK_F3,  _T("F3")	}, 
	{ VK_F4,  _T("F4")	},		{ VK_F5,  _T("F5")  },		{ VK_F6,  _T("F6")	}, 
	{ VK_F7,  _T("F7")  },		{ VK_F8,  _T("F8")  },		{ VK_F9,  _T("F9")	}, 
	{ VK_F10, _T("F10") },		{ VK_F11, _T("F11") },		{ VK_F12, _T("F12") },
/*----------------------------------------------------------------------------------*/
};

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		strTableColorName
//  Description:	Using for pairing color macro IDs and color names
//  Table type:		STRINGTABLE
//
//////////////////////////////////////////////////////////////////////////

static STRINGTABLE strTableColorName
{
/*-------Color ID----------------------------Color name-----------------------------*/
	{ COLOR_RED,							_T("Red")							},
	{ COLOR_GREEN,							_T("Green")							},
	{ COLOR_YELLOW,							_T("Yellow")						},
	{ COLOR_BLUE,							_T("Blue")							},
	{ COLOR_WHITE,							_T("White")							},
	{ COLOR_BLACK,							_T("Black")							},
	{ COLOR_PINK,							_T("Pink")							},
	{ COLOR_SAKURA_PINK,					_T("Sakura pink")					},
	{ COLOR_ORANGE,							_T("Orange")						},
	{ COLOR_UMARINE_BLUE,					_T("Ultramarine blue")				},
	{ COLOR_INDIGO,							_T("Indigo")						},
	{ COLOR_VIOLET,							_T("Violet")						},
	{ COLOR_PURPLE,							_T("Purple")						},
	{ COLOR_SCARLET,						_T("Scarlet")						},
	{ COLOR_JADE,							_T("Jade")							},
	{ COLOR_EMERALD,						_T("Emerald")						},
	{ COLOR_GRAY,							_T("Gray")							},
	{ COLOR_DARK_GRAY,						_T("Dark gray")						},
	{ COLOR_BRIGHT_GRAY,					_T("Bright gray")					},
/*----------------------------------------------------------------------------------*/
};


//////////////////////////////////////////////////////////////////////////
//
//	Table name:		strTableMsgIconName
//  Description:	Using for pairing message icon IDs and icon names
//  Table type:		STRINGTABLE
//
//////////////////////////////////////////////////////////////////////////

static STRINGTABLE strTableMsgIconName
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

//////////////////////////////////////////////////////////////////////////
//
//	Table name:		hklExistedSysHotkeyList
//  Description:	Using to define list of existed system-defined keystrokes 
//  Table type:		HOTKEYINFO Table
//
//////////////////////////////////////////////////////////////////////////

static const HOTKEYINFO hklExistedSysHotkeyList[] =
{
/*---------Control Key-----------Function Key----------Hotkey description-----------*/
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

namespace TableFuncs
{
	// Pair list processing functions
	UINT		  GetPairedID(IDMAPTABLE& idTableRef, UINT nID, BOOL bReverse = FALSE);
	UINT		  GetStringID(STRINGTABLE& strTableRef, LPCTSTR lpszInput);
	LPCTSTR		  GetString(STRINGTABLE& strTableRef, UINT nID, LPTSTR pszResult = NULL);

	// Language table package processing functions
	LPCTSTR		  GetLanguageName(UINT nCurLanguage, BOOL bGetDescription = FALSE, LPTSTR pszResult = NULL);
	LANGTABLE_PTR LoadLanguageTable(UINT nCurLanguage, LPTSTR pszRetLangName = NULL, int* pnSize = NULL);
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
	void OutputDebugStringFormat(LPCTSTR lpszDebugStringFormat, ...);

	// Trace/debug file logging functions
	BOOL BackupOldLogFile(CString& strFilePath, LPCTSTR lpszLogFileName);
	void WriteTraceErrorLogFile(LPCTSTR lpszLogStringW);
	void WriteTraceDebugLogFile(LPCTSTR lpszLogStringW);
	void WriteDebugInfoLogFile(LPCTSTR lpszLogStringW);
	void WriteTraceNDebugLogFileBase(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW);

	// Message and notification functions
	LRESULT	WaitMessage(UINT nMsg, int nTimeout = TIMEOUT_WAIT_MESSAGE);
	void	ShowErrorMessage(HWND hMsgOwnerWnd, UINT nLanguageID, DWORD dwErrorCode, LPARAM lParam = NULL);

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

	// String and text data processing functions
	LPCTSTR LoadResourceString(UINT nResStringID);
	BOOL	LoadResourceString(CString& strResult, UINT nResStringID);
	BOOL	LoadResourceTextFile(CString& strTextData, UINT nResourceFileID);

	int		GetTokenList(LPTSTR lpszBuff, PBUFFER retBuff, LPCTSTR lpszKeyChars);
	void	UpperEachWord(CString& strInput, BOOL bTrim);
	BOOL	MakeFilePath(CString& strOutput, LPCTSTR lpszDirectory, LPCTSTR lpszFileName, LPCTSTR lpszExtension);

	BOOL	StringValidate(LPCTSTR lpszSrc, DWORD& dwError);

	LPCTSTR StringFormat(UINT nFormatTemplateID, ...);
	LPCTSTR StringFormat(LPCTSTR lpszFormatTemplate, ...);
	BOOL	SubString(LPCTSTR lpszSrc, SUBSTRING& subDest, TCHAR tcFirstChar, TCHAR tcLastChar, BOOL bIncSepChar = FALSE);

	// Additional functions
	LPCTSTR GetApplicationPath(BOOL bIncludeExeName);
	CString	GetProductVersion(BOOL bFullVersion);
	BOOL	GetProductVersion(CString& strFullVersion, CString& strShortVersion);

	UINT	GetWindowsOSVersion(void);
	BOOL	GetDeviceName(CString& strDeviceName);
	BOOL	GetCurrentUserName(CString& strUserName);

	BOOL	AddRegistryKey(const REGISTRYINFO& regInfo);
	LPCTSTR MakeRegistryPath(const REGISTRYINFO& regInfo, UINT nRegPathType = REGPATH_FULL, BOOL bIncRootKey = TRUE);

	void	PlaySound(BOOL bSoundEnable, UINT nTypeOfSound);
	BOOL	FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath);
	BOOL	OpenWebURL(LPCTSTR lpszWebUrl);

	LRESULT RunApp(LPCTSTR lpszAppPath, BOOL bRunAsAdmin = FALSE, BOOL bShowFlag = TRUE);
	LRESULT ExecuteCommand(LPCTSTR lpszCmd, BOOL bRunAsAdmin = TRUE, BOOL bShowFlag = TRUE);
	BOOL	CreateAppProcess(LPCWSTR lpszAppPath, LPWSTR lpszCmdLine, UINT nStyle, DWORD& dwErrorCode);

	BOOL	SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode);
	void	DrawButton(CButton*& pButton, UINT nIconID, LPCTSTR lpszButtonTitle = STRING_EMPTY);
	BOOL	EnumFontNames(std::vector<std::wstring>& fontNames);
	BOOL	ValidateFontName(LPCTSTR lpszFontName);
};

#endif	// ifndef _CORE_H_INCLUDED
