
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		AppMacros.h
//		Description:	Define application base macros
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.06.04:		Create new
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _APP_MACROS_INCLUDED
#define _APP_MACROS_INCLUDED
#endif


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
#define APP_ERROR_DBG_TOKENIZATION_FAILED			((APP_ERROR_DEBUG_COMMAND << 8) + 0x03)
#define APP_ERROR_DBG_INVALID_COMMAND				((APP_ERROR_DEBUG_COMMAND << 8) + 0x04)


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

#define MODE_OPENDLG_MODAL							0x0a5							// Modal dialog
#define MODE_OPENDLG_MODELESS						0x0a6							// Modeless dialog


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

#define FILENAME_APPEXEFILE							_T("power++")
#define FILENAME_APPCONFIG							_T("Config")
#define FILENAME_BAKCONFIG							_T("BakConfig")
#define FILENAME_APPEVENT_LOG						_T("AppEventLog_%04d_%02d")
#define FILENAME_HISTORY_LOG						_T("AppHistory")
#define FILENAME_TRACE_ERROR_LOG					_T("TraceError")
#define FILENAME_TRACE_DEBUG_LOG					_T("TraceDebug")
#define FILENAME_DEBUG_INFO_LOG						_T("DebugInfo")


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

#define REG_AFX_PROJECTNAME							_T("Power++")
#define REG_STARTUP_VALUENAME						REG_AFX_PROJECTNAME
#define PATH_APP_NOTEPAD							_T("C:\\Windows\\notepad.exe")
#define PATH_APP_SYSTEMCMD							_T("C:\\Windows\\System32\\cmd.exe")


// Define special numbers and numeric values
//

#define INT_INVALID									-1								// Invalid integer number (equals -1)
#define INT_NULL									0								// Null integer number (equals 0)
#define INT_INFINITE								INFINITE						// Infinite (no limitation)
#define FLOAT_INVALID								-1.0F							// Invalid float number (equals -1.0)
#define FLOAT_NULL									0.0F							// Null float number (equals 0)
#define STRUCT_ZERO									{0}								// Zero-initialized struct data
#define SYSTEMTIME_ZERO								STRUCT_ZERO						// Zero-initialized systemtime data


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

#define IS_NOT_EMPTY_STRING(string)						(_tcscmp(string, Constant::String::Empty))
#define IS_EMPTY_STRING(string)							(!IS_NOT_EMPTY_STRING(string))
#define IS_NOT_NULL_STRING(string)						(_tcscmp(string, Constant::String::Null))
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
