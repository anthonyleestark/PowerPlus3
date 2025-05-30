
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

#include "Language.h"

#include "Components\GridCtrl\GridCtrl.h"
#include "Components\GridCtrl\GridCellCheck.h"

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


// Define app action macros
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

#define SUBFOLDER_LOG								_T("Log")
#define SUBFOLDER_HELP								_T("Help")
#define SUBFOLDER_BACKUP							_T("Backup")
#define SUBFOLDER_TEMP								_T("Temp")


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
#define FILENAME_HELP_ENG							_T("help_en")
#define FILENAME_HELP_VIE							_T("help_vi")
#define FILENAME_HELP_CHS							_T("help_ch")
#define FILENAME_CHANGELOG_ENG						_T("changelog_en")
#define FILENAME_CHANGELOG_VIE						_T("changelog_vi")
#define FILENAME_CHANGELOG_CHS						_T("changelog_ch")


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
#define SYMBOL_DEFAULT_INDENT						_T("\t")						// Default indentation (tab character)
#define SYMBOL_JSON_INDENT							SYMBOL_DEFAULT_INDENT			// JSON indentation (tab character)
#define SYMBOL_YAML_INDENT							SYMBOL_DOUBLE_SPACE				// YAML indentation (double spaces)

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


// Define special string-based values
//

#define VALUE_TRUE									_T("Yes")						// Boolean value: True (Yes)
#define VALUE_FALSE									_T("No")						// Boolean value: False (No)
#define VALUE_NULL									_T("Null")						// Null value
#define VALUE_UNKNOWN								_T("Unknown")					// Unknown value
#define VALUE_UNDEFINED								_T("Undefined")					// Undefined value


// Define special minimum/maximum values
//

#define MIN_SNOOZETIME								1								// Minimum snooze time: 1 minute
#define MIN_STRING_LENGTH							5								// Minimum string length: 5 characters
#define MIN_PASSWORD_LENGTH							6								// Minimum password length: 6 characters

#define MAX_SNOOZETIME								30								// Maximum snooze time: 30 minutes
#define MAX_DAYS_OF_WEEK							7								// Number of days of week: 7 days
#define MAX_TOKEN_COUNT								50								// Maximum token number: 50
#define MAX_BUFFER_LENGTH							512								// Maximum buffer length: 512 characters
#define MAX_STRING_LENGTH							1024							// Maximum string length: 2KB ~ 1024 characters
#define MAX_TEXT_LENGTH								2097152							// Maximum text length: 2MB ~ 2097152 characters
#define MAX_PASSWORD_LENGTH							30								// Maximum password length: 30 characters
#define MAX_DISP_LOGSTRING_LENGTH					20								// Maximum log string displaying length: 20 characters
#define MAX_BAKFILE_COUNT							100								// Maximum backup file number: 100
#define MAX_LOGFILE_SIZE							1048576							// Maximum file size: 1MB

#define MAX_RETRY_TIMES								10								// Max retry times: 5
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

#define BUTTON_ICON_WIDTH							20								// Button icon width: 20px
#define BUTTON_ICON_HEIGHT							20								// Button icon height: 20px
#define BUTTON_ICON_SIZE							20								// Button icon size (square): 20x20 (px)

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

#define TRACE_FORMAT									TraceErrorFormat
#define TRACE_ERROR(logString)							TraceError(logString)
#define TRACE_DEBUG(func, file, line)					TraceDebugInfo(func, file, line)


// Type-cast macros
//

#define DEFAULT_CAST(type, variable)					((type)variable)
#define CONST_CAST(type, variable)						const_cast<type>(variable)
#define STATIC_CAST(type, variable)						static_cast<type>(variable)
#define DYNAMIC_CAST(type, variable)					dynamic_cast<type>(variable)
#define REINTERPRET_CAST(type, variable)				reinterpret_cast<type>(variable)


// String processing functions
//

#define IS_NOT_EMPTY_STRING(string)						(_tcscmp(string, STRING_EMPTY))
#define IS_EMPTY_STRING(string)							(!IS_NOT_EMPTY_STRING(string))
#define IS_NOT_NULL_STRING(string)						(_tcscmp(string, STRING_NULL))
#define IS_NULL_STRING(string)							(!IS_NOT_NULL_STRING(string))

#define MAKEANSI(string)								(CW2A(string).m_psz)
#define MAKEUNICODE(string)								(CA2W(string).m_psz)
#define RESOURCESTRING(resourceid)						LoadResourceString(resourceid)

#define MAKE_WPARAM_STRING(string)						DEFAULT_CAST(WPARAM, _tcslen(string))
#define MAKE_LPARAM(type, data)							REINTERPRET_CAST(LPARAM, ((type)data))
#define MAKE_LPARAM_STRING(string)						MAKE_LPARAM(LPCTSTR, string)
#define LPARAM_STATIC_CAST(type, lParam)				STATIC_CAST(type, lParam)
#define LPARAM_REINTERPRET_CAST(type, lParam)			REINTERPRET_CAST(type, lParam)
#define LPARAM_TO_STRING(lParam)						LPARAM_REINTERPRET_CAST(LPCTSTR, lParam)


// Special expressions
//

#define NOTHING											((void)0)
#define GET_HANDLE_MAINWND()							(AfxGetMainWnd()->GetSafeHwnd())
#define IS_PRESSED(keycode)								(0x8000 & ::GetKeyState(keycode))
#define __FILENAME__									(strrchr("\\" __FILE__, '\\') + 1)


// Time value processing expressions
//

#define FORMAT_REG_TIME(systime)						(INT((systime.wHour * 100) + systime.wMinute))
#define GET_REGTIME_HOUR(reg_time)						(WORD(reg_time / 100))
#define GET_REGTIME_MINUTE(reg_time)					(WORD(reg_time % 100))
#define TIME_TO_SECONDS(time)							(INT((time.wHour * 3600) + (time.wMinute * 60) + time.wSecond))
#define GET_HOUR(totalSecs)								(WORD(totalSecs / 3600))
#define GET_MINUTE(totalSecs)							(WORD((totalSecs % 3600) / 60))
#define GET_SECOND(totalSecs)							(WORD((totalSecs % 3600) % 60))


// Complex macros
//

#define NULL_POINTER_BREAK(pointer, ret_expr)			if (pointer == NULL) { ret_expr; }
#define VERIFY_POINTER(pointer, type)					VERIFY(((pointer) != NULL) && AfxIsValidAddress((pointer), sizeof(type), FALSE))
#define ASSERT_INITIALIZATION(pointer, type)			if (pointer == NULL) { pointer = new type(); ASSERT_POINTER(pointer, type); }
#define VERIFY_INITIALIZATION(pointer, type) 			if (pointer == NULL) { pointer = new type(); VERIFY_POINTER(pointer, type); }


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

#define COMMAND_REGISTRY_DELETE						_T("reg delete %s /va /f")
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
//  Description:	Store application settings and configurations
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

struct CONFIGDATA
{
	// Main settings
	INT		nLMBAction;												// Left mouse button action
	INT		nMMBAction;												// Middle mouse button action
	INT		nRMBAction;												// Right mouse button action
	BOOL	bRMBShowMenu;											// Right mouse button: Only show menu

	// Display setting
	INT		nLanguageID;											// Language setting

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
};

// Define typename
using PCONFIGDATA = typename CONFIGDATA*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ConfigData
//  Description:	Class for application config data management
//  Derivered from:	CONFIGDATA
//
//////////////////////////////////////////////////////////////////////////

class ConfigData : public CONFIGDATA
{
public:
	enum AppOptionID {
		invalid = -1,												// *** Invalid option ***
		leftMouseAction = 0,										// Left mouse button action
		middleMouseAction,											// Middle mouse button action
		rightMouseAction,											// Right mouse button action
		rightMouseShowMenu,											// Right mouse button: Only show menu
		languageID,													// Language setting
		curDispLanguage,											// Currently displaying language
		showDlgAtStartup,											// Show dialog at startup
		startupEnabled,												// Startup with Windows
		confirmBeforeExecuting,										// Show confirm message before doing action
		saveAppEventLog,											// Save app event log
		saveAppHistoryLog,											// Save action history log
		runAsAdmin,													// Run with admin privileges
		showErrorMessage,											// Show action error message
		notifySchedule,												// Show notify tip for schedule action
		allowCancelingSchedule,										// Allow canceling schedule when notify
		backgroundHotkeyEnabled,									// Enable background action hotkeys
		lockStateHotkeyEnabled,										// Allow background hotkeys on lockscreen
		pwrReminderEnabled,											// Enable Power Peminder feature
		defaultScheduleActiveState,									// Default schedule active state
		defaultScheduleActionID,									// Default schedule action ID
		defaultScheduleRepeat										// Default schedule repeat option
	};

public:
	// Construction
	ConfigData();													// Default constructor
	ConfigData(const CONFIGDATA&);									// Copy constructor

public:
	// Data processing
	void Copy(const CONFIGDATA&);									// Copy data
	BOOL Compare(const CONFIGDATA&) const;							// Compare data
	void SetDefaultData(void);										// Set default data

	// Access data
	void GetData(CONFIGDATA&) const;								// Get a copy of config data
	int  GetAppOption(AppOptionID) const;							// Get application option data by ID
};

// Define new global typenames for the enum attributes of Application config data
using AppOptionID = typename ConfigData::AppOptionID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		FlagManager
//  Description:	Class for application flag data management
//
//////////////////////////////////////////////////////////////////////////

class FlagManager final
{
public:
	enum AppFlagID {
	// Application-wide tracing flags: Globally managed
		pwrActionFlag,												// Power action trace flag
		systemSuspendFlag,											// System suspended trace flag
		sessionEndFlag,												// Session ended trace flag
		safeTerminationFlag,										// Previously safe termination trace flag
		sessionLockFlag,											// Session lock trace flag

	// Application DebugTest flags: Globally managed
		dummyTestMode,												// Dummy test mode enabled flag
		debugMode,													// Debug mode enabled flag
		debugOutputTarget,											// Debug log output target flag
		testFeatureEnabled,											// Test feature enable flag

	// Application-base flags: Application managed
		appDataChanged,												// Application data/setting change flag
		appReadOnlyMode,											// Application read-only mode
		appForceClosing,											// Force closing application by request
		appExitCode,												// Application exit code

	// Dialog-base properties/flags: Dialog managed
		dialogDataChanged,											// Dialog data/setting change flag
		dialogReturnFlag,											// Dialog returned flag
		dialogExpanded,												// Dialog expanded/collapsed
		dialogReadOnlyMode,											// Dialog read-only mode
		dialogLockState,											// Dialog item lock state
		dialogForceClosing,											// Force closing dialog by request
		dialogUseEscapeKey,											// Use Escape key
		dialogUseEnterKey,											// Use Enter key
		dialogSetBackgroundColor,									// Dialog background color is set
		dialogSetTextColor,											// Dialog text color is set
		dialogSetMinSize,											// Dialog minimum size is set
		dialogSetMaxSize,											// Dialog maximum size is set
		dialogSetTopMost,											// Dialog top-most position is set
		dialogSetInitSound,											// Dialog initialize sound is set

	// Application main window runtime flags: Application managed
		notifyIconShowed,											// Notify icon showing flag
		hotkeyRegistered,											// Hotkey registered
		restartAsAdmin,												// Restart as admin flag
		pwrBroadcastSkipCount,										// Power Broadcase event skip counter
		wtsSessionNotifyRegistered,									// WTS Session Change State Notification registered
	};
	enum ManagerID {
		dialogFlagManager,											// Dialog-owned flag manager
		applicationFlagManager,										// Application flag manager
		globalFlagManager,											// Global/shared flag manager
	};
	enum ManagementMethod {
		dialogSelfManaged,											// For flags managed directly by the dialog itself
		applicationManaged,											// For flags managed by the broader application logic
		globallyManaged,											// For flags managed centrally by a global/shared manager
	};

private:
	// Define private typenames/aliases
	using UniqueFlagMap = typename std::unordered_map<AppFlagID, INT>;

private:
	// Attributes
	UniqueFlagMap m_mapUniqueFlags;

public:
	// Constructor
	FlagManager(void) = default;									// Default constructor

private:
	// No copyable
	FlagManager(const FlagManager&) = delete;
	FlagManager& operator=(const FlagManager&) = delete;

	// No movable
	FlagManager(const FlagManager&&) = delete;
	FlagManager& operator=(const FlagManager&&) = delete;

public:
	// Member functions
	BOOL IsFlagPresent(AppFlagID) const;							// Check if a flag value exists
	int  GetFlagValue(AppFlagID) const;								// Get flag value by ID
	void SetFlagValue(AppFlagID, int);								// Set flag value by ID
};

// Define new global typenames for the enum attributes of Application flag data
using AppFlagID = typename FlagManager::AppFlagID;
using FlagManagerID = typename FlagManager::ManagerID;
using FlagManagementMethod = typename FlagManager::ManagementMethod;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrRepeatSet
//  Description:	Store and manage data of a Power++ item repeat set
//
//////////////////////////////////////////////////////////////////////////

class PwrRepeatSet
{
public:
	enum DayOfWeek { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday	};

private:
	// Attributes
	BOOL m_bRepeat;													// Repeat daily
	BOOL m_bAllowSnooze;											// Allow snoozing mode
	INT	 m_nSnoozeInterval;											// Snooze interval
	BYTE m_byRepeatDays;											// Days of week (for repeating)

public:
	// Constructor
	PwrRepeatSet();													// Default constructor
	PwrRepeatSet(const PwrRepeatSet&);								// Copy constructor

	// Operator
	PwrRepeatSet& operator=(const PwrRepeatSet&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const PwrRepeatSet&);									// Copy data
	BOOL Compare(const PwrRepeatSet&) const;						// Compare data

public:
	// Get attributes
	BOOL IsRepeatEnabled(void) const;								// Check if repeat option is enabled
	BOOL IsAllowSnoozing(void) const;								// Check if snooze option is enabled
	INT	 GetSnoozeInterval(void) const;								// Get snooze interval data
	BYTE GetActiveDays(void) const;									// Get repeat days data
	BOOL IsDayActive(DayOfWeek) const;								// Check if day of week is active

	// Set attributes
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week
};

// Define new typenames for RepeatSet data
using PWRREPEATSET = typename PwrRepeatSet;
using PPWRREPEATSET = typename PwrRepeatSet*;

// Define new global typenames for the enum attributes of RepeatSet data
using DayOfWeek = typename PwrRepeatSet::DayOfWeek;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ScheduleItem
//  Description:	Store schedule item settings
//
//////////////////////////////////////////////////////////////////////////

class ScheduleItem
{
private:
	// Attributes
	UINT		 m_nItemID;											// Item ID
	BOOL		 m_bEnabled;										// Enable/disable state
	UINT		 m_nActionID;										// Schedule action ID
	SYSTEMTIME	 m_stTime;											// Schedule time
	PWRREPEATSET m_rpsRepeatSet;									// Repeat set data

public:
	// Constructor
	ScheduleItem();													// Default constructor
	ScheduleItem(UINT nItemID);										// Overloaded constructor
	ScheduleItem(const ScheduleItem&);								// Copy constructor

	// Operator
	ScheduleItem& operator=(const ScheduleItem&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const ScheduleItem&);									// Copy data
	BOOL Compare(const ScheduleItem&) const;						// Compare items
	BOOL IsEmpty(void) const;										// Check if item data is empty

public:
	// Get/set attributes
	UINT GetItemID(void) const;										// Get schedule item ID
	void SetItemID(UINT);											// Set schedule item ID
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item active state
	UINT GetAction(void) const;										// Get item action ID
	void SetAction(UINT);											// Set item action ID
	SYSTEMTIME GetTime(void) const;									// Get schedule item time data
	void SetTime(const SYSTEMTIME&);								// Set schedule item time data

	// Get RepeatSet data
	BOOL IsRepeatEnabled(void) const;								// Check if repeat is enabled
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	BOOL IsDayActive(DayOfWeek) const;								// Check if day of week is active
	BYTE GetActiveDays(void) const;									// Get repeat active days

	// Set RepeatSet data
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week

	void Print(CString& strOutput) const;							// Print item data
};

// Define new typenames for Schedule item data
using SCHEDULEITEM = typename ScheduleItem;
using PSCHEDULEITEM = typename ScheduleItem*;
using SCHEDULEITEMLIST = CArray<SCHEDULEITEM, SCHEDULEITEM>;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		ScheduleData
//  Description:	Store app Action Schedule data settings
//
//////////////////////////////////////////////////////////////////////////

class ScheduleData
{
private:
	// Attributes
	SCHEDULEITEM	 m_schDefaultItem;								// Default schedule item
	SCHEDULEITEMLIST m_arrSchedExtraItemList;						// List of extra schedule items

public:
	// Constructor
	ScheduleData();													// Default constructor
	ScheduleData(const ScheduleData&);								// Copy constructor

	// Operator
	ScheduleData& operator=(const ScheduleData&);					// Copy assignment operator

public:
	// Data processing
	void Init(void);												// Init data
	void Copy(const ScheduleData&);									// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	DWORD Add(const SCHEDULEITEM&);									// Add item
	DWORD Update(const SCHEDULEITEM&);								// Update item

	// Access items
	const SCHEDULEITEM& GetDefaultItem(void) const;					// Get default item (constant)
	SCHEDULEITEM& GetDefaultItem(void);								// Get default item
	const SCHEDULEITEM& GetItemAt(int) const;						// Get item at index (constant)
	SCHEDULEITEM& GetItemAt(int);									// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust();													// Adjust data validity
	UINT GetNextID();												// Get next item ID (to add new item)

	// Get attributes
	INT_PTR GetExtraItemNum(void) const;							// Get number of extra items
	BOOL IsDefaultEmpty(void) const;								// Check if default item is empty
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsExtraEmpty(void) const;									// Check if extra data is empty
	BOOL IsAllEmpty(void) const;									// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteExtra(void);											// Delete all extra items
	void DeleteAll(void);											// Delete all data
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HotkeySetItem
//  Description:	Store data of a HotkeySet item
//
//////////////////////////////////////////////////////////////////////////

class HotkeySetItem
{
public:
	enum HKID {
		displayOff = 0x1a01,										// Turn off display
		sleep,														// Sleep
		shutdown,													// Shut down
		restart,													// Restart
		signOut,													// Sign out
		hibernate,													// Hibernate
	};

private:
	// Attributes
	BOOL	m_bEnabled;												// Hotkey enabled/disabled
	UINT	m_nHKActionID;											// Hotkey action ID
	DWORD	m_dwModifiers;											// Modifier keys
	DWORD	m_dwVirtualKey;											// Virtual key code

public:
	// Constructor
	HotkeySetItem();												// Default constructor
	HotkeySetItem(UINT nHKActionID);								// Overloaded constructor
	HotkeySetItem(const HotkeySetItem&);							// Copy constructor

	// Operator
	HotkeySetItem& operator=(const HotkeySetItem&);					// Copy assignment operator

public:
	// Data processing
	void Copy(const HotkeySetItem&);								// Copy item
	BOOL IsEmpty(void) const;										// Check if item is empty
	BOOL Compare(const HotkeySetItem&) const;						// Compare items
	BOOL CompareKeycode(const HotkeySetItem&) const;				// Compare item keycodes
	void Print(CString& strOutput) const;							// Print item data
	void PrintKeyStrokes(CString& strOutput) const;					// Print item keystrokes

public:
	// Get/set attributes
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item enable state
	UINT GetActionID(void) const;									// Get Hotkey action ID
	void SetActionID(UINT);											// Set Hotkey action ID
	void GetKeyCode(DWORD&, DWORD&) const;							// Get item keycode data
	void SetKeyCode(DWORD, DWORD);									// Set item keycode data
	BOOL CompareKeycode(DWORD, DWORD) const;						// Compare given keycode with item keystroke
};

// Define new typenames for HotkeySet item data
using HOTKEYSETITEM = typename HotkeySetItem;
using PHOTKEYSETITEM = typename HotkeySetItem*;
using HOTKEYSETITEMLIST = CArray<HOTKEYSETITEM, HOTKEYSETITEM>;

// Define new global typenames for the enum attributes of HotkeySet items
using HotkeyID = typename HotkeySetItem::HKID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HotkeySetData
//  Description:	Store app Hotkeyset data settings
//
//////////////////////////////////////////////////////////////////////////

class HotkeySetData
{
public:
	using Item = typename HotkeySetItem;
	using HKID = typename Item::HKID;
	using DataList = typename HOTKEYSETITEMLIST;

private:
	// Attributes
	DataList m_arrHotkeySetList;									// Pointer to HotkeySet list

public:
	// Constructor
	HotkeySetData();												// Default constructor
	HotkeySetData(const HotkeySetData&);							// Copy constructor

	// Operator
	HotkeySetData& operator=(const HotkeySetData&);					// Copy assignment operator

public:
	// Data processing
	void Init(void);												// Init data
	void Copy(const HotkeySetData&);								// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	void Add(const Item&);											// Add item
	void Update(const Item&);										// Update item

	// Access items
	const Item& GetItemAt(int) const;								// Get item at index (const)
	Item& GetItemAt(int);											// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust(void);												// Adjust data validity

	// Get attributes
	INT_PTR GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty() const;										// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
	void PrintKeyStrokes(UINT nHKID, CString& strOutput) const;		// Print item keystrokes by ID
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RmdMsgStyleSet
//  Description:	Store data of a Reminder message style set
//
//////////////////////////////////////////////////////////////////////////

class RmdMsgStyleSet
{
private:
	// Attributes
	COLORREF	m_colorBkgrd;										// Background color
	COLORREF	m_colorText;										// Text color
	CString		m_strFontName;										// Font name
	UINT		m_uiFontSize;										// Font size
	UINT		m_uiTimeout;										// Timeout (auto-close) interval
	UINT		m_uiIconID;											// Message icon ID
	INT			m_nIconSize;										// Message icon size
	BYTE		m_byIconPos;										// Message icon position
	BYTE		m_byDisplayPos;										// Message display position
	UINT		m_uiHMargin;										// Display area horizontal margin
	UINT		m_uiVMargin;										// Display area vertical margin

public:
	// Constructor
	RmdMsgStyleSet();												// Default constructor
	RmdMsgStyleSet(const RmdMsgStyleSet&);							// Copy constructor

	// Operator
	RmdMsgStyleSet& operator=(const RmdMsgStyleSet&);				// Copy assignment operator

public:
	// Member functions
	void Copy(const RmdMsgStyleSet&);								// Copy data
	BOOL Compare(const RmdMsgStyleSet&) const;						// Compare data

public:
	// Get/set functions
	COLORREF GetBkgrdColor(void) const;								// Get background color
	void SetBkgrdColor(COLORREF);									// Set background color
	COLORREF GetTextColor(void) const;								// Get text color
	void SetTextColor(COLORREF);									// Set text color
	CString GetFontName(void) const;								// Get font name
	void SetFontName(LPCTSTR);										// Set font name
	UINT GetFontSize(void) const;									// Get font size
	void SetFontSize(UINT);											// Set font size
	UINT GetTimeout(void) const;									// Get timeout interval
	void SetTimeout(UINT);											// Set timeout interval
	UINT GetIconID(void) const;										// Get message icon ID
	void SetIconID(UINT);											// Set message icon ID
	INT GetIconSize(void) const;									// Get message icon size
	void SetIconSize(INT);											// Set message icon size
	BYTE GetIconPosition(void) const;								// Get message icon position
	void SetIconPosition(BYTE);										// Set message icon position
	BYTE GetDisplayPosition(void) const;							// Get message display position
	void SetDisplayPosition(BYTE);									// Set message display position
	UINT GetHorizontalMargin(void) const;							// Get horizontal margin
	void SetHorizontalMargin(UINT);									// Set horizontal margin
	UINT GetVerticalMargin(void) const;								// Get vertical margin
	void SetVerticalMargin(UINT);									// Set vertical margin
};

// Define new typenames for Reminder message style data
using RMDMSGSTYLESET = typename RmdMsgStyleSet;
using PRMDMSGSTYLESET = typename RmdMsgStyleSet*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrReminderItem
//  Description:	Store data of a Power Reminder item
//
//////////////////////////////////////////////////////////////////////////

class PwrReminderItem
{
public:
	enum Event {
		atSetTime = 0x1b01,											// At set time
		atAppStartup,												// At app startup
		atSysWakeUp,												// At system wake
		beforePwrAction,											// Before power action
		wakeAfterAction,											// Wake after action
		atAppExit,													// Before app exit
	};
	enum Style {
		messageBox = 0x1c01,										// Message Box
		dialogBox,													// Dialog Box
	};

private:
	// Attributes
	BOOL			m_bEnabled;										// Enable state
	UINT			m_nItemID;										// Item ID
	CString			m_strMessage;									// Message content
	UINT			m_nEventID;										// Event ID
	SYSTEMTIME		m_stTime;										// Event time
	DWORD			m_dwMsgStyle;									// Reminder style
	PWRREPEATSET	m_rpsRepeatSet;									// Repeat set data
	BOOL			m_bUseCustomStyle;								// Use message custom style
	RMDMSGSTYLESET	m_rmsMsgStyleSet;								// Reminder message style set

public:
	// Constructor
	PwrReminderItem();												// Default constructor
	PwrReminderItem(const PwrReminderItem&);						// Copy constructor

	// Operator
	PwrReminderItem& operator=(const PwrReminderItem&);				// Copy assignment operator

public:
	// Data processing
	void Copy(const PwrReminderItem&);								// Copy item
	BOOL IsEmpty(void) const;										// Check if item is empty
	BOOL Compare(const PwrReminderItem&) const;						// Compare items

	// Access data
	PWRREPEATSET& GetRepeatSetData(void);							// Get access to item RepeatSet info data
	void ResetRepeatInfo(void);										// Reset RepeatSet info data
	RMDMSGSTYLESET& GetMessageStyleData(void);						// Get access to item Message Style info data
	void ResetMessageStyleInfo(void);								// Reset Message Style info data

public:
	// Get/set attributes
	BOOL IsEnabled(void) const;										// Check if item is enabled
	void EnableItem(BOOL);											// Set item enable state
	UINT GetItemID(void) const;										// Get Power Reminder item ID
	void SetItemID(UINT);											// Set Power Reminder item ID
	LPCTSTR GetMessage(void) const;									// Get item message content
	void SetMessage(LPCTSTR);										// Set item message content
	UINT GetEventID(void) const;									// Get Power Reminder item event ID
	void SetEventID(UINT);											// Set Power Reminder item event ID
	SYSTEMTIME GetTime(void) const;									// Get Power Reminder item time data
	void SetTime(const SYSTEMTIME&);								// Set Power Reminder item time data
	DWORD GetMessageStyle(void) const;								// Get item message style ID
	void SetMessageStyle(DWORD);									// Set item message style ID
	BOOL IsCustomStyleEnabled(void) const;							// Check if item message custom style is enabled
	void EnableCustomStyle(BOOL);									// Set item message custom style enable state

	// Get RepeatSet data
	BOOL IsRepeatEnabled(void) const;								// Check if item repeat mode is enabled
	BOOL IsDayActive(DayOfWeek dayOfWeek) const;					// Check if day of week is active
	BOOL IsAllowSnoozing(void) const;								// Check if item snooze mode is available
	INT  GetSnoozeInterval(void) const;								// Get item snooze interval value
	BYTE GetActiveDays(void) const;									// Get repeat active days

	// Set RepeatSet data
	void EnableRepeat(BOOL);										// Set repeat enable state
	void EnableSnoozing(BOOL);										// Set allow snoozing state
	void SetSnoozeInterval(INT);									// Set snooze interval data
	void SetActiveDays(BYTE);										// Set repeat days data
	void SetDayActive(DayOfWeek, BOOL);								// Set active state for specific day of week

	void Print(CString& strOutput) const;							// Print item data
};

// Define new typenames for Power Reminder Item data
using PWRREMINDERITEM = typename PwrReminderItem;
using PPWRREMINDERITEM = typename PwrReminderItem*;
using PWRREMINDERITEMLIST = CArray<PWRREMINDERITEM, PWRREMINDERITEM>;

// Define new global typenames for the enum attributes of Power Reminder items
using PwrReminderEvent = typename PwrReminderItem::Event;
using PwrReminderStyle = typename PwrReminderItem::Style;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PwrReminderData
//  Description:	Store app Power Reminder data settings
//
//////////////////////////////////////////////////////////////////////////

class PwrReminderData
{
private:
	// Attributes
	PWRREMINDERITEMLIST	m_arrRmdItemList;							// List of reminder items
	RMDMSGSTYLESET		m_rmdCommonStyle;							// Common message style set

public:
	// Constructor
	PwrReminderData();												// Default constructor
	PwrReminderData(const PwrReminderData&);						// Copy constructor

	// Operator
	PwrReminderData& operator=(const PwrReminderData&);				// Copy assignment operator

public:
	// Member functions
	void Init(void);												// Init data
	void Copy(const PwrReminderData&);								// Copy data
	void SetDefaultData(void);										// Set default data

	// Update items
	void Add(const PWRREMINDERITEM&);								// Add item
	void Update(const PWRREMINDERITEM&);							// Update item

	// Access items
	const PWRREMINDERITEM& GetItemAt(int) const;					// Get item at index (constant)
	PWRREMINDERITEM& GetItemAt(int);								// Get item at index

	// Item processing
	void Remove(int);												// Remove item at index
	void RemoveAll(void);											// Remove all item
	void Adjust(void);												// Adjust data validity
	UINT GetNextID(void);											// Get next item ID (to add new item)

	// Get attributes
	INT_PTR GetItemNum(void) const;									// Get number of items
	BOOL IsEmpty(int) const;										// Check if item at index is empty
	BOOL IsAllEmpty(void) const;									// Check if all items are empty

	// Clean-up
	void Delete(int);												// Delete item at index
	void DeleteAll(void);											// Delete all data
};

//////////////////////////////////////////////////////////////////////////
//
//  Class name:		PwrRuntimeItem
//  Description:	Store data of a runtime info item
//
//////////////////////////////////////////////////////////////////////////

class PwrRuntimeItem
{
public:
	enum FeatureCategoryID {
		powerAction = 0,											// Power action
		schedule,													// Action schedule
		hotkeySet,													// HotkeySet
		pwrReminder,												// Power Reminder
	};

private:
	// Attributes
	INT			m_nCategory;										// Item category
	UINT		m_nItemID;											// Power Reminder item ID
	INT			m_nDisplayFlag;										// Item displaying flag
	INT			m_nSkipFlag;										// Item skip flag
	INT			m_nSnoozeFlag;										// Item snooze trigger flag
	SYSTEMTIME	m_stNextSnoozeTime;									// Next snooze trigger time

public:
	// Constructor
	PwrRuntimeItem();												// Default constructor
	PwrRuntimeItem(const PwrRuntimeItem&);							// Copy constructor

	// Operator
	PwrRuntimeItem& operator=(const PwrRuntimeItem&);				// Copy assignment operator

	// Data processing
	void Copy(const PwrRuntimeItem&);								// Copy data
	void CalcNextSnoozeTime(int nInterval);							// Calculate next snooze time

public:
	// Get/set attributes
	INT			GetCategory(void) const;							// Get item category
	void		SetCategory(INT);									// Set item category
	UINT		GetItemID(void) const;								// Get item ID
	void		SetItemID(UINT);									// Set item ID
	INT			GetDisplayFlag(void) const;							// Get item displaying flag
	void		SetDisplayFlag(INT);								// Set item displaying flag
	INT			GetSkipFlag(void) const;							// Get item skip flag
	void		SetSkipFlag(INT);									// Set item skip flag
	INT			GetSnoozeFlag(void) const;							// Get item snooze trigger flag
	void		SetSnoozeFlag(INT);									// Set item snooze trigger flag
	SYSTEMTIME  GetTime(void) const;								// Get next snooze trigger time
	void		SetTime(const SYSTEMTIME&);							// Set next snooze trigger time
};

// Define new typenames for runtime info item data
using PWRRUNTIMEITEM = typename PwrRuntimeItem;
using PPWRRUNTIMEITEM = typename PwrRuntimeItem*;
using PWRRUNTIMEQUEUE = CArray<PWRRUNTIMEITEM, PWRRUNTIMEITEM>;

// Define new global typenames for the enum attributes of runtime info items
using PwrFeatureID = typename PwrRuntimeItem::FeatureCategoryID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		HistoryInfoData
//  Description:	Manage app action history info data
//
//////////////////////////////////////////////////////////////////////////

class HistoryInfoData
{
private:
	// Attributes
	BOOL		m_bInitState;										// Init state flag
	UINT		m_nCategoryID;										// Category ID
	SYSTEMTIME	m_stTimestamp;										// Timestamp of history
	UINT		m_nItemID;											// Item ID
	UINT		m_nActionID;										// Action ID
	BOOL		m_bActionResult;									// Action result
	DWORD		m_dwErrorCode;										// Returned error code
	CString		m_strDescription;									// History description (attached info)

public:
	// Constructor
	HistoryInfoData();												// Default constructor
	HistoryInfoData(const HistoryInfoData&);						// Copy constructor

	// Operator
	HistoryInfoData& operator=(const HistoryInfoData&);				// Copy assignment operator

	// Member functions
	void Copy(const HistoryInfoData&);								// Copy data
	void Init(UINT nCategoryID);									// Initialization
	void RemoveAll(void);											// Remove all data

public:
	// Get/set properties
	BOOL IsInit(void) const;										// Check if data is initialized
	UINT GetCategoryID(void) const;									// Get category ID
	void SetCategoryID(UINT);										// Set category ID
	SYSTEMTIME GetTime(void) const;									// Get timestamp
	void SetTime(const SYSTEMTIME&);								// Set timestamp
	UINT GetItemID(void) const;										// Get item ID
	void SetItemID(UINT);											// Set item ID
	UINT GetActionID(void) const;									// Get action ID
	void SetActionID(UINT);											// Set action ID
	BOOL IsSuccess(void) const;										// Check if action is successful
	void SetResult(BOOL);											// Set action result
	DWORD GetErrorCode(void) const;									// Get error code
	void SetErrorCode(DWORD);										// Set error code
	void GetDescription(CString&) const;							// Get description
	LPCTSTR GetDescription(void) const;								// Get description
	void SetDescription(LPCTSTR);									// Set description
};

// Define new typenames for History info data
using HISTORYINFODATA = typename HistoryInfoData;
using PHISTORYINFODATA = typename HistoryInfoData*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		SystemEvent
//  Description:	Store data of System Event info
//
//////////////////////////////////////////////////////////////////////////

class SystemEvent
{
public:
	enum EventID {
		SystemSuspend = 0,											// System suspend event
		SystemWakeUp,												// System wakeup event
		SessionEnded,												// Session end event
		SessionLocked,												// Session locked event
		SessionUnlocked,											// Session unlocked event
	};

private:
	// Attributes
	EventID		m_sysEventID;										// System event ID
	SYSTEMTIME  m_timeStamp;										// Event timestamp

public:
	// Construction
	SystemEvent(EventID);											// Constructor (with event ID)
	SystemEvent(const SystemEvent&);								// Copy constructor

	// Operators
	SystemEvent& operator=(const SystemEvent&);						// Copy assignment operator

public:
	// Get/set functions
	EventID GetEventID(void) const;									// Get system event ID
	SYSTEMTIME GetTimestamp(void) const;							// Get system event timestamp
	void SetTimestamp(SYSTEMTIME);									// Set system event timestamp
};

// Define new typenames for Power System Event info data
using SystemEventData = typename std::vector<SystemEvent>;

// Define new global typenames for the enum attributes of System Event info
using SystemEventID = typename SystemEvent::EventID;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		SystemEventTracker
//  Description:	Store and manage System Event tracking data
//
//////////////////////////////////////////////////////////////////////////

class SystemEventTracker
{
private:
	// Attributes
	SystemEventData m_arrTrackingData;								// System event tracking data

public:
	// Construction
	SystemEventTracker();											// Constructor (with event ID)
	SystemEventTracker(const SystemEventTracker&);					// Copy constructor

	// Operators
	SystemEventTracker& operator=(const SystemEventTracker&);		// Copy assignment operator

public:
	// Validation
	BOOL IsEmpty(void) const;										// Check if tracking data is empty
	INT_PTR GetTrackedCount(void) const;							// Get number of tracked events

	// Update data
	void AddEvent(const SystemEvent&);								// Add system event info
	void RemoveAll(SystemEventID);									// Remove all tracking data of specific event ID
	void RemoveAll(void);											// Remove all event tracking data

	// Access items
	const SystemEvent& GetAt(int) const;							// Get system event info data by index
};

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
//	Class name:		RegistryValue
//  Description:	Store data values of a registry key 
//
//////////////////////////////////////////////////////////////////////////

class RegistryValue
{
public:
	enum Type {
		None = 0,											// Undefined/invalid type/no data
		String,												// String value
		DWORD_32,											// DWORD (32-bit) value
		QWORD_64,											// QWORD (64-bit) value
		Multi_String,										// Multi-string value
	};

private:
	// Data values
	CString*		m_pstrValue;							// String value
	DWORD*			m_pdwValue;								// DWORD (32-bit) value
	QWORD*			m_pqwValue;								// QWORD (64-bit) value
	CStringArray*	m_pastrValue;							// Multi-string value

public:
	// Construction
	RegistryValue();										// Default constructor
	RegistryValue(const RegistryValue&);					// Copy constructor	
	~RegistryValue();										// Default destructor

	// Operator
	RegistryValue& operator=(const RegistryValue&);			// Copy assignment operator

	// Member functions
	BOOL Init(UINT);										// Initialize with type
	void Copy(const RegistryValue&);						// Copy data
	void Reset(void);										// Remove data
	void Refactor(void);									// Refactor data and optimize
	BOOL IsEmpty(void) const;								// Check if data is empty
	BOOL IsType(UINT) const;								// Check if current type is matching

public:
	// Get/set values
	UINT GetType(void) const;								// Get value type
	void GetString(CString&) const;							// Get String value
	LPCTSTR GetString(void) const;							// Get String value
	void SetString(LPCTSTR);								// Set String value
	DWORD GetDWord(void) const;								// Get DWORD (32-bit) value
	void SetDWord(DWORD);									// Set DWORD (32-bit) value
	QWORD GetQWord(void) const;								// Get QWORD (64-bit) value
	void SetQWord(QWORD);									// Set QWORD (64-bit) value
	void GetMultiString(CStringArray&) const;				// Get Multi-string value
	void SetMultiString(CStringArray&);						// Set Multi-string value
	BOOL AddString(LPCTSTR);								// Add string value to Multi-string data
};

// Define new typenames for Registry value data
using REGISTRYVALUE = typename RegistryValue;
using PREGISTRYVALUE = typename RegistryValue*;

// Define new global typenames for the enum attributes of RegistryValue
using RegValueType = typename RegistryValue::Type;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RegistryKey
//  Description:	Store data of a registry key info
//
//////////////////////////////////////////////////////////////////////////

class RegistryKey
{
private:
	// Member variables
	CString			m_strKeyName;							// Key name (string)
	REGISTRYVALUE	m_regValue;								// Registry value data

public:
	// Constructor
	RegistryKey();											// Default constructor
	RegistryKey(const RegistryKey&);						// Copy constructor

	// Operator
	RegistryKey& operator=(const RegistryKey&);				// Copy assignment operator

	// Member functions
	void Copy(const RegistryKey&);							// Copy data
	void Clear(void);										// Remove all data
	BOOL IsEmpty(void) const;								// Check if registry key is empty

public:
	// Get/set properties
	void GetName(CString&) const;							// Get key name
	LPCTSTR GetName(void) const;							// Get key name
	void SetName(UINT);										// Set key name
	void SetName(LPCTSTR);									// Set key name
	UINT GetType(void) const;								// Get value type
	void SetType(UINT);										// Set value type

	// Get/set value
	void GetString(CString&) const;							// Get String value
	LPCTSTR GetString(void) const;							// Get String value
	void SetString(LPCTSTR);								// Set String value
	DWORD GetDWord(void) const;								// Get DWORD (32-bit) value
	void SetDWord(DWORD);									// Set DWORD (32-bit) value
	QWORD GetQWord(void) const;								// Get QWORD (64-bit) value
	void SetQWord(QWORD);									// Set QWORD (64-bit) value
	void GetMultiString(CStringArray&) const;				// Get Multi-string value
	void SetMultiString(CStringArray&);						// Set Multi-string value
	BOOL AddString(LPCTSTR);								// Add string value to Multi-string data
};

// Define new typenames for Registry key info data
using REGISTRYKEY = typename RegistryKey;
using PREGISTRYKEY = typename RegistryKey*;

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		RegistryInfo
//  Description:	Store data of a registry key info
//
//////////////////////////////////////////////////////////////////////////

class RegistryInfo
{
public:
	enum RegistryPathType {
		fullPath = 0,										// Full path
		rootKeyOnly,										// Root key only
		includingSubKeyPath,								// Including sub-key path
		includingProfileKey,								// Including profile key name
		includingAppName,									// Including app name
		includingSectionName,								// Including section name
		includingKeyName,									// Including key name
	};

private:
	// Attributes
	HKEY			m_hRootKey;								// Root key (HKEY)
	CString			m_strRootKey;							// Root key (string)
	CStringArray	m_astrSubkeyPath;						// Subkey path (string array)
	CString			m_strProfileName;						// Profile key name (string)
	CString			m_strAppName;							// App name (string)
	CStringArray	m_astrSectionArray;						// Section array (string)
	REGISTRYKEY		m_regKeyInfo;							// Registry key info

public:
	// Constructor
	RegistryInfo();											// Default constructor
	RegistryInfo(const RegistryInfo&);						// Copy constructor

	// Operator
	RegistryInfo& operator=(const RegistryInfo&);			// Copy assignment operator

	// Member functions
	void Copy(const RegistryInfo&);							// Copy data
	void RemoveAll(void);									// Remove all data

public:
	// Get data
	HKEY GetRootKey(void) const;							// Get root key
	const REGISTRYKEY& GetRegistryKey(void) const;			// Get registry key info data

	// Get attributes
	LPCTSTR GetRootKeyName(void) const;						// Get root key name
	void GetSubkeyPath(CStringArray&) const;				// Get Subkey path array
	LPCTSTR GetProfileName(void) const;						// Get Profile key name
	LPCTSTR GetAppName(void) const;							// Get App name
	void GetSectionName(CStringArray&) const;				// Get Section name array

	// Set attributes
	void SetRootKey(HKEY);									// Set root key
	void SetRootKeyName(UINT);								// Set root key name (resource ID)
	void SetRootKeyName(LPCTSTR);							// Set root key name (string)
	void SetSubkeyPath(UINT);								// Set Subkey path (resource ID)
	void SetSubkeyPath(LPCTSTR);							// Set Subkey path (string)
	void SetSubkeyPath(CStringArray&);						// Set Subkey path (string array)
	void SetProfileName(UINT);								// Set Profile key name (resource ID)
	void SetProfileName(LPCTSTR);							// Set Profile key name (string)
	void SetAppName(UINT);									// Set App name (resource ID)
	void SetAppName(LPCTSTR);								// Set App name (string)
	void SetSectionName(UINT);								// Set Section name (resource ID)
	void SetSectionName(LPCTSTR);							// Set Section name (string)
	void SetSectionName(CStringArray&);						// Set Section array (string array)
};

// Define new typenames for Registry info class
using REGISTRYINFO = typename RegistryInfo;
using PREGISTRYINFO = typename RegistryInfo*;

// Define new global typenames for the enum attributes of RegistryValue
using RegistryPathType = typename RegistryInfo::RegistryPathType;

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
//	Data type name:	HOTKEYINFO
//  Description:	Store hotkey info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagHOTKEYINFO
{
	// Member variables
	DWORD	dwModifiers;									// Modifier keys
	DWORD	dwVirtualKey;									// Virtual key code
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
//	Class name:		Substring
//  Description:	Using for getting substrings
//
//////////////////////////////////////////////////////////////////////////

class Substring
{
private:
	// Member variables
	CString m_strLeft;										// Left part
	CString m_strMid;										// Middle part
	CString m_strRight;										// Right part

public:
	// Constructor
	Substring();											// Default constructor
	Substring(const Substring&);							// Copy constructor

	// Operator
	Substring& operator=(const Substring&);					// Copy assignment operator

	// Data processing
	void Copy(const Substring&);							// Copy data
	void RemoveAll(void);									// Remove all data
	BOOL IsEmpty(void) const;								// Check if data is empty
	void TrimLeft(void);									// Trim spaces for left part
	void TrimMid(void);										// Trim spaces for middle part
	void TrimRight(void);									// Trim spaces for right part
	void TrimAll(void);										// Trim spaces for all parts

	// Get substrings
	LPCTSTR	Left(void) const;								// Get left part
	LPCTSTR	Mid(void) const;								// Get middle part
	LPCTSTR	Right(void) const;								// Get right part

	// Set substrings
	void SetLeft(LPCTSTR);									// Set left part
	void SetMid(LPCTSTR);									// Set middle part
	void SetRight(LPCTSTR);									// Set right part
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		StringProcessor
//  Description:	Using for string processing and validation
//
//////////////////////////////////////////////////////////////////////////

class StringProcessor
{
public:
	enum class ValidationError {
		None = 0,
		EmptyString,
		TooShort,
		TooLong,
		InvalidCharacters,
		InvalidFormat,
		ContainsWhitespace,
		ContainsProhibitedSymbols,
		NotAlphanumeric,
		EncodingError,
		InjectionDetected,
		CustomRuleFailed
	};
};

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		PerformanceCounter
//  Description:	Using for querrying performance counter of functions
//
//////////////////////////////////////////////////////////////////////////

class PerformanceCounter
{
private:
	// Attributes
	LARGE_INTEGER m_liStartTime;							// Start time
	LARGE_INTEGER m_liEndTime;								// End time
	LARGE_INTEGER m_liFrequency;							// Performance frequency

	// Counting flag
	BOOL m_bIsRunning;										// Counter is running

public:
	// Constructor
	PerformanceCounter();									// Default constructor
	~PerformanceCounter();									// Destructor

	// Member functions
	void Start(void);										// Start performance counter
	void Stop(void);										// Stop performance counter
	double GetElapsedTime(BOOL) const;						// Get function execution elapsed time
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define core methods for program, these functions will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace AppCore
{
	// Power action execution functions (main core)
	BOOL ExecutePowerAction(UINT nActionType, UINT nMessage, DWORD& dwErrCode);
	BOOL ExecutePowerActionDummy(UINT nActionType, UINT nMessage, DWORD& dwErrCode);

	// Trace logging functions
	void TraceError(LPCSTR lpszTraceLogA);
	void TraceError(LPCTSTR lpszTraceLogW);
	void TraceErrorFormat(LPCSTR lpszTraceLogFormatA, ...);
	void TraceErrorFormat(LPCTSTR lpszTraceLogFormatW, ...);
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
	void		GetCurSysTime(SYSTEMTIME& stTime);
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
	LPCTSTR GetSubFolderPath(LPCTSTR lpszSubFolderName);
	BOOL	MakeFilePath(CString& strOutput, LPCTSTR lpszDirectory, LPCTSTR lpszFileName, LPCTSTR lpszExtension);
	int		PrintCharList(LPCTSTR lpszSrc, CString& strOutput);

	LPCTSTR StringFormat(UINT nFormatTemplateID, ...);
	LPCTSTR StringFormat(LPCTSTR lpszFormatTemplate, ...);
	BOOL	SubString(LPCTSTR lpszSrc, Substring& subDest, TCHAR tcFirstChar, TCHAR tcLastChar, BOOL bIncSepChar = FALSE);

	// Additional functions
	LPCTSTR GetApplicationPath(BOOL bIncludeExeName);
	CString	GetProductVersion(BOOL bFullVersion);
	BOOL	GetProductVersion(CString& strFullVersion, CString& strShortVersion);

	UINT	GetWindowsOSVersion(void);
	BOOL	GetDeviceName(CString& strDeviceName);
	BOOL	GetCurrentUserName(CString& strUserName);

	BOOL	AddRegistryKey(const REGISTRYINFO& regInfo);
	LPCTSTR MakeRegistryPath(const REGISTRYINFO& regInfo, UINT nRegPathType = RegistryPathType::fullPath, BOOL bIncRootKey = TRUE);

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


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _APPCORE_ENABLE_INLINES
	#include "AppCore.inl"
	#ifdef _APPCORE_INL_INCLUDED
		#pragma message("-- AppCore inline library included")
	#else
		#pragma error("-- Linking error in AppCore.h: Unable to link to inline header!")
	#endif
	#undef _APPCORE_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in AppCore.h: Inline is not enabled!")
#endif

#endif	// ifndef _CORE_H_INCLUDED
