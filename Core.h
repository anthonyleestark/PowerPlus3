
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
#define APP_LANGUAGE_ENGLISH					(APP_LANGUAGE << 8) + 0x01
#define APP_LANGUAGE_VIETNAMESE					(APP_LANGUAGE << 8) + 0x02
#define APP_LANGUAGE_SIMPCHINESE				(APP_LANGUAGE << 8) + 0x03

// Define app actions
#define DEF_APP_ACTION							0x02
#define DEF_APP_ACTION_NOTHING					(DEF_APP_ACTION << 8) + 0x00
#define DEF_APP_ACTION_DISPLAYOFF				(DEF_APP_ACTION << 8) + 0x01
#define DEF_APP_ACTION_SLEEP					(DEF_APP_ACTION << 8) + 0x02
#define DEF_APP_ACTION_SHUTDOWN					(DEF_APP_ACTION << 8) + 0x03
#define DEF_APP_ACTION_RESTART					(DEF_APP_ACTION << 8) + 0x04
#define DEF_APP_ACTION_SIGNOUT					(DEF_APP_ACTION << 8) + 0x05
#define DEF_APP_ACTION_HIBERNATE				(DEF_APP_ACTION << 8) + 0x06
#define DEF_APP_ACTION_SHOWMENU					(DEF_APP_ACTION << 8) + 0x07
#define DEF_APP_ACTION_SHOWWINDOW				(DEF_APP_ACTION << 8) + 0x08

// Define app action messages
#define DEF_APP_MESSAGE							0x03
#define DEF_APP_MESSAGE_DISPLAYOFF				SC_MONITORPOWER
#define DEF_APP_MESSAGE_SLEEP					(DEF_APP_MESSAGE << 8) + 0x01
#define DEF_APP_MESSAGE_HIBERNATE				(DEF_APP_MESSAGE << 8) + 0x02
#define DEF_APP_MESSAGE_SHUTDOWN				EWX_SHUTDOWN | EWX_HYBRID_SHUTDOWN
#define DEF_APP_MESSAGE_POWEROFF				EWX_POWEROFF
#define DEF_APP_MESSAGE_REBOOT					EWX_REBOOT
#define DEF_APP_MESSAGE_SIGNOUT					EWX_LOGOFF

// Define action type constants
#define DEF_APP_ACTIONTYPE						0x04
#define DEF_APP_ACTIONTYPE_MONITOR				(DEF_APP_ACTIONTYPE << 8) + 0x01
#define DEF_APP_ACTIONTYPE_POWER				(DEF_APP_ACTIONTYPE << 8) + 0x02

// Define app error codes
#define DEF_APP_ERROR							0x05
#define DEF_APP_ERROR_SUCCESS					ERROR_SUCCESS
#define DEF_APP_ERROR_FAILED					(DEF_APP_ERROR << 8) + 0xE1
#define DEF_APP_ERROR_WRONG_ARGUMENT			(DEF_APP_ERROR << 8) + 0xE2
#define DEF_APP_ERROR_INVALID_FUNCTION			ERROR_INVALID_FUNCTION
#define DEF_APP_ERROR_FILE_NOT_FOUND			ERROR_FILE_NOT_FOUND
#define DEF_APP_ERROR_PATH_NOT_FOUND			ERROR_PATH_NOT_FOUND
#define DEF_APP_ERROR_ACCESS_DENIED				ERROR_ACCESS_DENIED
#define DEF_APP_ERROR_INVALID_HANDLE			ERROR_INVALID_HANDLE
#define DEF_APP_ERROR_INVALID_DATA				ERROR_INVALID_DATA
#define DEF_APP_ERROR_NO_MORE_FILES				ERROR_NO_MORE_FILES
#define DEF_APP_ERROR_FILE_EXISTS				ERROR_FILE_EXISTS
#define DEF_APP_ERROR_CANNOT_MAKE				ERROR_CANNOT_MAKE
#define DEF_APP_ERROR_INVALID_PARAMETER			ERROR_INVALID_PARAMETER
#define DEF_APP_ERROR_OPEN_FAILED				ERROR_OPEN_FAILED
#define DEF_APP_ERROR_BUFFER_OVERFLOW			ERROR_BUFFER_OVERFLOW
#define DEF_APP_ERROR_INVALID_NAME				ERROR_INVALID_NAME
#define DEF_APP_ERROR_DIR_NOT_EMPTY				ERROR_DIR_NOT_EMPTY
#define DEF_APP_ERROR_FAIL_SHUTDOWN				ERROR_FAIL_SHUTDOWN
#define DEF_APP_ERROR_FAIL_RESTART				ERROR_FAIL_RESTART
#define DEF_APP_ERROR_INVALID_ADDRESS			ERROR_INVALID_ADDRESS
#define DEF_APP_ERROR_APP_INIT_FAILURE			ERROR_APP_INIT_FAILURE
#define DEF_APP_ERROR_CANNOT_LOAD_REGISTRY		ERROR_CANNOT_LOAD_REGISTRY_FILE
#define DEF_APP_ERROR_REGISTRY_QUOTA_LIMIT		ERROR_REGISTRY_QUOTA_LIMIT
#define DEF_APP_ERROR_SYSTEM_SHUTDOWN			ERROR_SYSTEM_SHUTDOWN
#define DEF_APP_ERROR_HIBERNATED				ERROR_HIBERNATED
#define DEF_APP_ERROR_RESUME_HIBERNATION		ERROR_RESUME_HIBERNATION
#define DEF_APP_ERROR_WAKE_SYSTEM				ERROR_WAKE_SYSTEM
#define DEF_APP_ERROR_BACKUP_REG_FAILED			(DEF_APP_ERROR << 8) + 0xE3
#define DEF_APP_ERROR_LOAD_CFG_INVALID			(DEF_APP_ERROR << 8) + 0xE4
#define DEF_APP_ERROR_LOAD_CFG_FAILED			(DEF_APP_ERROR << 8) + 0xE5
#define DEF_APP_ERROR_LOAD_SCHED_INVALID		(DEF_APP_ERROR << 8) + 0xE6
#define DEF_APP_ERROR_LOAD_SCHED_FAILED			(DEF_APP_ERROR << 8) + 0xE7
#define DEF_APP_ERROR_LOAD_HKEYSET_INVALID		(DEF_APP_ERROR << 8) + 0xE8
#define DEF_APP_ERROR_LOAD_HKEYSET_FAILED		(DEF_APP_ERROR << 8) + 0xE9
#define DEF_APP_ERROR_LOAD_PWRRMD_INVALID		(DEF_APP_ERROR << 8) + 0xEA
#define DEF_APP_ERROR_LOAD_PWRRMD_FAILED		(DEF_APP_ERROR << 8) + 0xEB
#define DEF_APP_ERROR_SAVE_CFG_INVALID			(DEF_APP_ERROR << 8) + 0xEC
#define DEF_APP_ERROR_SAVE_CFG_FAILED			(DEF_APP_ERROR << 8) + 0xED
#define DEF_APP_ERROR_SAVE_SCHED_INVALID		(DEF_APP_ERROR << 8) + 0xEE
#define DEF_APP_ERROR_SAVE_SCHED_FAILED			(DEF_APP_ERROR << 8) + 0xEF
#define DEF_APP_ERROR_SAVE_HKEYSET_INVALID		(DEF_APP_ERROR << 8) + 0xF0
#define DEF_APP_ERROR_SAVE_HKEYSET_FAILED		(DEF_APP_ERROR << 8) + 0xF1
#define DEF_APP_ERROR_SAVE_PWRRMD_INVALID		(DEF_APP_ERROR << 8) + 0xF2
#define DEF_APP_ERROR_SAVE_PWRRMD_FAILED		(DEF_APP_ERROR << 8) + 0xF3
#define DEF_APP_ERROR_WRITE_LOG_FAILED			(DEF_APP_ERROR << 8) + 0xF4
#define DEF_APP_ERROR_OUTPUT_LOG_FAILED			(DEF_APP_ERROR << 8) + 0xF5
#define DEF_APP_ERROR_UNKNOWN					(DEF_APP_ERROR << 8) + 0xFF

// Define menu item types
#define MENU_ITEM								0x06
#define MENU_ITEM_OPTION						(MENU_ITEM << 8) + 0x0001
#define MENU_ITEM_SEPERATOR_ITEM				(MENU_ITEM << 8) + 0x0002
#define	MENU_ITEM_POPUP_CHILD_MENU				(MENU_ITEM << 8) + 0x0003

// Define menu actions
#define MENU_ACTION								0x07
#define MENU_ACTION_HELP						(MENU_ACTION << 8) + 0x0001
#define MENU_ACTION_ABOUT						(MENU_ACTION << 8) + 0x0002
#define MENU_ACTION_SHOW_WINDOW					(MENU_ACTION << 8) + 0x0003
#define MENU_ACTION_EXIT						(MENU_ACTION << 8) + 0x0004
#define MENU_ACTION_SELECT_VIETNAMESE			(MENU_ACTION << 8) + 0x0005
#define MENU_ACTION_SELECT_ENGLISH				(MENU_ACTION << 8) + 0x0006
#define MENU_ACTION_VIEW_LOG					(MENU_ACTION << 8) + 0x0007
#define MENU_ACTION_DEFAULT						(MENU_ACTION << 8) + 0x0008
#define MENU_ACTION_ADJUST_MENU					(MENU_ACTION << 8) + 0x0009
#define MENU_ACTION_SHOW_WHEN_START				(MENU_ACTION << 8) + 0x000a
#define MENU_ACTION_STARTUP						(MENU_ACTION << 8) + 0x000b
#define MENU_ACTION_ASK_BEFORE					(MENU_ACTION << 8) + 0x000c
#define MENU_ACTION_SAVE_LOGS					(MENU_ACTION << 8) + 0x000d
#define MENU_ACTION_SHOW_ERROR_MSG				(MENU_ACTION << 8) + 0x000e

// Define types of sound
#define DEF_APP_SOUND							0x08
#define DEF_APP_SOUND_ERROR						(DEF_APP_SOUND << 8) + 0x0001
#define DEF_APP_SOUND_SUCCESS					(DEF_APP_SOUND << 8) + 0x0002

// Macros definition
#define DEF_APP_MACRO							0x09
#define DEF_APP_MACRO_LEFT_MOUSE				(DEF_APP_MACRO << 8) + 0x0001
#define DEF_APP_MACRO_MIDDLE_MOUSE				(DEF_APP_MACRO << 8) + 0x0002
#define DEF_APP_MACRO_RIGHT_MOUSE				(DEF_APP_MACRO << 8) + 0x0003
#define DEF_APP_MACRO_ACTION_MENU				(DEF_APP_MACRO << 8) + 0x0004
#define DEF_APP_MACRO_ACTION_SCHEDULE			(DEF_APP_MACRO << 8) + 0x0005
#define DEF_APP_MACRO_ACTION_HOTKEY				(DEF_APP_MACRO << 8) + 0x0006
#define DEF_APP_MACRO_DIR_HORZ					(DEF_APP_MACRO << 8) + 0x0007
#define DEF_APP_MACRO_DIR_VERT					(DEF_APP_MACRO << 8) + 0x0008

// Define registry informations
#define REG_DEFINE_NONE							0x0a
#define REG_HKEY_CURRENT_USER					HKEY_CURRENT_USER
#define REG_HKEY_LOCAL_MACHINE					HKEY_LOCAL_MACHINE
#define REG_TYPE								(REG_DEFINE_NONE << 4) + 0x001
#define REG_VALUE_STRING						(REG_TYPE << 4) + 0x0001
#define REG_VALUE_DWORD							(REG_TYPE << 4) + 0x0002

// Define app custom messages
#define SM_APP_TRAYICON							(WM_USER + 100)
#define SM_APP_UPDATE_CONFIGDATA				(WM_USER + 101)
#define SM_APP_UPDATE_SCHEDULEDATA				(WM_USER + 102)
#define SM_APP_UPDATE_HOTKEYSETDATA				(WM_USER + 103)
#define SM_APP_UPDATE_PWRREMINDERDATA			(WM_USER + 104)
#define SM_APP_SHOW_ERROR_MSG					(WM_USER + 105)
#define SM_WND_DEBUGTEST						(WM_USER + 197)
#define SM_APP_DEBUGCOMMAND						(WM_USER + 198)
#define SM_APP_DEBUGOUTPUT						(WM_USER + 199)
#define SM_WND_SHOWDIALOG						(WM_USER + 201)
#define SM_WND_DEBUGVIEWCLRSCR					(WM_USER + 202)

// Define app data types
#define APPDATA_CONFIG							0x00001001L
#define APPDATA_SCHEDULE						0x00001002L
#define APPDATA_HOTKEYSET						0x00001004L
#define APPDATA_PWRREMINDER						0x00001008L
#define APPDATA_ALL								(APPDATA_CONFIG | APPDATA_SCHEDULE | APPDATA_HOTKEYSET | APPDATA_PWRREMINDER)

// Define modes
#define DEF_MODE_INIT							0x000				// Mode: Initialize
#define DEF_MODE_VIEW							0x001				// Mode: View
#define DEF_MODE_ADD							0x002				// Mode: Add
#define DEF_MODE_UPDATE							0x003				// Mode: Update
#define DEF_MODE_LOAD							0x004				// Mode: Load
#define DEF_MODE_SAVE							0x005				// Mode: Save
#define DEF_MODE_DISABLE						0xFFF				// Mode: Disable
#define DEF_MODE_HELPVIEW_HELPFILE				0x0a1				// Mode: View help file
#define DEF_MODE_HELPVIEW_CHANGELOG				0x0a2				// Mode: View changelog file
#define DEF_MODE_BAKREGDATA_MANUAL				0x0a3				// Mode: Registry manually backup
#define DEF_MODE_BAKREGDATA_AUTO				0x0a4				// Mode: Registry auto backup
#define DEF_MODE_OPENDLG_MODAL					0x0a5				// Modal dialog
#define DEF_MODE_OPENDLG_MODELESS				0x0a6				// Modeless dialog

// Define colors
#define DEF_COLOR_RED							RGB(255,0,0)		// Color: Red
#define DEF_COLOR_GREEN							RGB(0,255,0)		// Color: Green
#define DEF_COLOR_YELLOW						RGB(255,255,0)		// Color: Yellow
#define DEF_COLOR_BLUE							RGB(0,0,255)		// Color: Blue
#define DEF_COLOR_WHITE							RGB(255,255,255)	// Color: White
#define DEF_COLOR_BLACK							RGB(0,0,0)			// Color: Black
#define DEF_COLOR_PINK							RGB(230,190,200)	// Color: Pink
#define DEF_COLOR_GRAY							RGB(200,200,200)	// Color: Gray
#define DEF_COLOR_DARK_GRAY						RGB(160,160,160)	// Color: Dark gray
#define DEF_COLOR_BRIGHT_GRAY					RGB(240,240,240)	// Color: Bright gray

// Define log types
#define LOGTYPE_APPEVENT_LOG					0x01a				// App event log
#define LOGTYPE_ACTION_LOG						0x01b				// Action log/history
#define LOGTYPE_TRACE_LOG						0x01c				// Trace log
#define LOGTYPE_DEBUG_LOG						0x01d				// Debug log

// Define subdirectory names
#define DIR_SUBDIR_LOG							_T(".\\Log")
#define DIR_SUBDIR_HELP							_T(".\\Help")
#define DIR_SUBDIR_BACKUP						_T(".\\Backup")
#define DIR_SUBDIR_TEMP							_T(".\\Temp")

// Define file names
#define FILE_APP_EXENAME						_T("PowerPlus3.exe")
#define FILE_APP_CONFIG							_T("Config.ini")
#define FILE_BAK_CONFIG							_T("BakConfig.reg")
#define FILE_APPEVENT_LOG						_T("AppEventLog_%d%02d%02d.log")
#define FILE_ACTION_LOG							_T("ActionHistory.log")
#define FILE_TRACE_LOG							_T("Trace.log")
#define FILE_DEBUG_LOG							_T("Debug.log")
#define FILE_HELP_ENG							_T("English.hlps")
#define FILE_HELP_VIE							_T("Vietnamese.hlps")
#define FILE_HELP_CHS							_T("Chinese.hlps")
#define FILE_CHANGELOG_ENG						_T("Change_log.en.hlps")
#define FILE_CHANGELOG_VIE						_T("Change_log.vi.hlps")
#define FILE_CHANGELOG_CHS						_T("Change_log.ch.hlps")

// Other special definitions
#define TIMERID_STD_BYSECOND					(DWORD)1
#define TIMERID_RMDMSG_AUTOCLOSE				(DWORD)2
#define REG_AFX_PROJECTNAME						_T("PowerPlus3")
#define REG_STARTUP_VALUENAME					REG_AFX_PROJECTNAME
#define APP_NOTEPAD_PATH						_T("C:\\Windows\\notepad.exe")
#define APP_SYSTEMCMD_PATH						_T("C:\\Windows\\System32\\cmd.exe")

// Define macros
#define TRCLOG(logString)						TraceLog(logString)
#define TRCFFMT(funcname, errString)			TraceLogFormat("Error: %s [Function: %s]", errString, funcname)

// Define properties for Power Reminder function
#define DEF_PWRREMINDER_MAX_ITEMNUM				100					// Max item number: 100
#define DEF_PWRREMINDER_MIN_ITEMID				10000				// Min item ID: 10000
#define DEF_PWRREMINDER_MAX_ITEMID				19999				// Max item ID: 19999
#define DEF_PWRREMINDER_PREVIEW_TIMEOUT			10					// Default time-out for preview: 10s

// Define special strings and numbers
#define DEF_STRING_EMPTY						_T("")				// Empty string
#define DEF_STRING_NEWLINE						_T("\n")			// New line string
#define DEF_STRING_NEWLINEWRET					_T("\r\n")			// New line string (with 'return' character)
#define DEF_STRING_NULL							_T("#NULL")			// Null/invalid string
#define DEF_PATH_SEPARATOR						_T("\\")			// File/folder path separator
#define DEF_SYMBOL_OUTPUTSIGN					_T(">> ")			// Output sign
#define DEF_SYMBOL_INPUTSIGN					_T(" <<")			// Input sign
#define DEF_CHAR_RETURN							_T('\r')			// 'Return' character
#define DEF_CHAR_ENDLINE						_T('\n')			// 'New line' character
#define DEF_FILEEXT_BAKFILE						_T(".bak")			// Backup file extension
#define DEF_FILEEXT_BAKFILEWNUM					_T("%d.bak")		// Backup file extension (with number)
#define DEF_BAKFILE_MAXNUM						100					// Maximum backup file number: 100

#define DEF_INTEGER_INVALID						-1					// Invalid integer number (equals -1)
#define DEF_INTEGER_NULL						0					// Null integer number (equals 0)
#define DEF_BOOLVAL_CHECK						-1					// Checked
#define DEF_BOOLVAL_UNCHECK						0					// Unchecked
#define DEF_TOKEN_MAXCOUNT						50					// Max token number: 50
#define DEF_BUFF_MAXLENGTH						512					// Max buffer length: 512 characters
#define DEF_STRING_MAXLENGTH					50					// Max string length: 50 characters

#define DEF_OFFSET_VSCRLBRWIDTH					3					// Offset = 3px
#define DEF_OFFSET_LISTCTRLWIDTH				5					// Offset = 5px
#define DEF_LISTCTRL_HEADERHEIGHT				27					// Header height = 27px
#define DEF_LISTCTRL_ROWHEIGHT					18					// Row height = 18px
#define DEF_GRIDCTRL_HEADERHEIGHT				25					// Header height = 25px
#define DEF_GRIDCTRL_ROWHEIGHT					23					// Row height = 23px
#define DEF_GRIDCTRL_ROWHEADER					0					// Row header index

#define DEF_SPINCTRL_TIMEMINPOS					0					// As 00:00
#define DEF_SPINCTRL_TIMEMAXPOS					1439				// As 23:59
#define DEF_DATACHANGELOG_CTRLNAME_MAXLENGTH	30					// Max length: 30 characters
#define DEF_LOGDISP_STRING_MAXLENGTH			20					// Max length: 20 characters
#define DEF_LOGFILE_MAXLENGTH					1048576				// Max file size: 1MB


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

// System events
typedef enum eSYSTEMEVENTID {
	SYSEVT_SUSPEND = 0,
	SYSEVT_WAKEUP,
} SYSTEMEVENTID;

// App option IDs
typedef enum eAPPOPTIONID {
	OPTIONID_LMBACTION = 0,			// Left mouse button action
	OPTIONID_MMBACTION,				// Middle mouse button action
	OPTIONID_RMBACTION,				// Right mouse button action
	OPTIONID_RMBSHOWMENU,			// Right mouse button: Only show menu
	OPTIONID_LANGUAGEID,			// Language setting
	OPTIONID_SHOWDLGATSTARTUP,		// Show dialog at startup
	OPTIONID_STARTUPENABLE,			// Startup with Windows
	OPTIONID_CONFIRMACTION,			// Show confirm message before doing action
	OPTIONID_SAVEACTIONLOG,			// Save action log
	OPTIONID_SAVEAPPEVENTLOG,		// Save app event log
	OPTIONID_RUNASADMIN,			// Run with admin privileges
	OPTIONID_SHOWERRORMSG,			// Show action error message
	OPTIONID_NOTIFYSCHEDULE,		// Show notify tip for schedule action
	OPTIONID_ALLOWCANCELSCHED,		// Allow canceling schedule when notify
	OPTIONID_ENABLEHOTKEYSET,		// Enable background action hotkeys
	OPTIONID_ENABLEPWRREMINDER,		// Enable Power Peminder feature
	OPTIONID_SCHEDULEACTIVE,		// Enable/disable state
	OPTIONID_SCHEDULEACTION,		// Schedule action
	OPTIONID_SCHEDULEREPEAT,		// Repeat daily
} APPOPTIONID;

// App flag IDs
typedef enum eAPPFLAGID {
	FLAGID_CHANGEFLAG = 0,			// Data/setting change flag
	FLAGID_DLGEXPANDED,				// Dialog expanded/collapsed
	FLAGID_NOTIFYICONSHOWED,		// Notify icon showing flag
	FLAGID_HOTKEYREGISTERED,		// Hotkey registered
	FLAGID_RESTARTASADMIN,			// Restart as admin flag
	FLAGID_PWRBROADCASTSKIPCOUNT,	// Power Broadcase event skip counter
} APPFLAGID;

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
	REGPATH_NOMASTERKEY,			// No master/root key
	REGPATH_NOAPPNAME,				// No app name
	REGPATH_NOSECTION,				// No section
	REGPATH_NOKEY,					// No key
} REGPATHTYPE;

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
	UINT	nLMBAction;								// Left mouse button action
	UINT	nMMBAction;								// Middle mouse button action
	UINT	nRMBAction;								// Right mouse button action
	BOOL	bRMBShowMenu;							// Right mouse button: Only show menu

	// Display setting
	UINT	nLanguageID;							// Language setting

	// System settings
	BOOL	bShowDlgAtStartup;						// Show dialog at startup
	BOOL	bStartupEnabled;						// Startup with Windows
	BOOL	bConfirmAction;							// Show confirm message before executing action
	BOOL	bSaveActionLog;							// Save action log
	BOOL	bSaveAppEventLog;						// Save app event log
	BOOL	bRunAsAdmin;							// Run with admin privileges
	BOOL	bShowErrorMsg;							// Show action error message
	BOOL	bNotifySchedule;						// Show notify tip for schedule action
	BOOL	bAllowCancelSchedule;					// Allow canceling schedule when notify
	BOOL	bEnableBackgroundHotkey;				// Enable background action hotkeys
	BOOL	bEnablePowerReminder;					// Enable Power Peminder feature

	// Member functions
	void Copy(const tagCONFIGDATA&);				// Copy data
} CONFIGDATA, *PCONFIGDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	SCHEDULEDATA
//  Description:	Store app schedule settings
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagSCHEDULEDATA
{
	// Member variables
	BOOL		bEnable;							// Enable/disable state
	BOOL		bRepeat;							// Repeat daily
	UINT		nAction;							// Schedule action
	SYSTEMTIME	stTime;								// Schedule time

	// Member functions
	void Copy(const tagSCHEDULEDATA&);				// Copy data
	void Activate();								// Activate schedule
	void Deactivate();								// Deactivate schedule
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
	BOOL	bEnable;								// Hotkey enabled/disabled
	UINT	nHKActionID;							// Hotkey action ID
	DWORD	dwCtrlKeyCode;							// Control Keycode #1
	DWORD	dwFuncKeyCode;							// Function Keycode #2

	// Member functions
	void Copy(const tagHOTKEYSETITEM&);				// Copy item
	BOOL IsEmpty();									// Check if item is empty
	BOOL Compare(const tagHOTKEYSETITEM&);			// Compare items
	void Print(CString& strOutput);					// Print item data
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
	int				  nItemNum;						// Number of hotkeyset items
	HOTKEYSETITEMLIST arrHotkeySetList;				// Pointer to HotkeySet list

	// Constructor
	tagHOTKEYSETDATA();								// Default constructor
	tagHOTKEYSETDATA(const tagHOTKEYSETDATA&);		// Copy constructor

	// Member functions
	void Init();									// Init data
	void Copy(const tagHOTKEYSETDATA&);				// Copy data
	void Add(const HOTKEYSETITEM&);					// Add item
	void Update(const HOTKEYSETITEM&);				// Update item
	HOTKEYSETITEM& GetItemAt(int);					// Get item at index
	void Remove(int);								// Remove item at index
	void RemoveAll(void);							// Remove all item
	void Adjust();									// Adjust data validity
	BOOL IsEmpty(int);								// Check if item at index is empty
	BOOL IsAllEmpty();								// Check if all items are empty
	void Delete(int);								// Delete item at index
	void DeleteAll(void);							// Delete all data
} HOTKEYSETDATA, *PHOTKEYSETDATA;

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
	BOOL		bEnable;							// Enable state
	UINT		nItemID;							// Item ID
	CString		strMessage;							// Message content
	UINT		nEventID;							// Event ID
	SYSTEMTIME  stTime;								// Event time
	DWORD		dwStyle;							// Reminder style
	BOOL		bRepeat;							// Repeat daily

	// Constructor
	tagPWRREMINDERITEM();							// Default constructor
	tagPWRREMINDERITEM(const tagPWRREMINDERITEM&);	// Copy constructor
	tagPWRREMINDERITEM(BOOL, UINT, LPCTSTR, UINT,	// Copy constructor (init list)
					   SYSTEMTIME, DWORD, BOOL);

	// Member functions
	void Copy(const tagPWRREMINDERITEM&);			// Copy item
	BOOL IsEmpty();									// Check if item is empty
	BOOL Compare(const tagPWRREMINDERITEM&);		// Compare items
	void Print(CString& strOutput);					// Print item data
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
	int					nItemNum;					// Number of reminder items
	PWRREMINDERITEMLIST	arrRmdItemList;				// List of reminder items

	// Constructor
	tagPWRREMINDERDATA();							// Default constructor
	tagPWRREMINDERDATA(const tagPWRREMINDERDATA&);	// Copy constructor

	// Member functions
	void Init();									// Init data
	void Copy(const tagPWRREMINDERDATA&);			// Copy data
	void Add(const PWRREMINDERITEM&);				// Add item
	void Update(const PWRREMINDERITEM&);			// Update item
	PWRREMINDERITEM& GetItemAt(int);				// Get item at index
	void Remove(int);								// Remove item at index
	void RemoveAll(void);							// Remove all item
	void Adjust();									// Adjust data validity
	UINT GetNextID();								// Get next item ID (to add new item)
	BOOL IsEmpty(int);								// Check if item at index is empty
	BOOL IsAllEmpty();								// Check if all items are empty
	void Delete(int);								// Delete item at index
	void DeleteAll(void);							// Delete all data
} PWRREMINDERDATA, *PPWRREMINDERDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	ACTIONDATA
//  Description:	Store app action info data
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagACTIONDATA
{
	// Member variables
	BOOL		bInitState;							// Init state flag
	UINT		nActionType;						// Action type
	SYSTEMTIME	stActionTime;						// Time of action
	UINT		nActionNameID;						// Name of action (string ID)
	BOOL		bActionSucceed;						// Action success status
	UINT		nErrorCode;							// Action returned error code

	// Member functions
	void Copy(const tagACTIONDATA&);				// Copy data
	void RemoveAll();								// Remove all data
} ACTIONDATA, *PACTIONDATA;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	USERMENUITEM
//  Description:	Store data of a user custom menu item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagUSERMENUITEM
{
	UINT	nItemID;								// Menu item ID
	UINT	nItemType;								// Menu item type
	UINT	nAction;								// Menu action ID
	LPCTSTR	lpszItemTitle;							// Menu item text
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
	UINT nItemCount;								// Number of menu items
	PUSERMENUITEM pMenu;							// User menu pointer
} USERMENU, *PUSERMENU;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	REGISTRYKEY
//  Description:	Store data of a registry key info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagREGISTRYKEY
{
	HKEY	hkTagName;								// Tag name (HKEY)
	UINT	nTagNameID;								// Tag name ID
	LPCTSTR lpszTagName;							// Tag name (string)
	UINT	nSubPathID;								// Sub-path
	LPCTSTR lpszSubPath;							// Sub-path (string)
	DWORD	dwMasterKeyID;							// Master key ID
	UINT	nMasterKeyNameID;						// Master key name ID
	LPCTSTR	lpszMasterKeyName;						// Master key name (string)
	UINT	nAppNameID;								// App name ID
	LPCTSTR lpszAppName;							// App name (string)
	UINT	nSectionNameID;							// Section name ID
	LPCTSTR lpszSectionName;						// Section name (string)
	UINT	nKeyNameID;								// Key name ID
	LPCTSTR lpszKeyName;							// Key name (string)
	UINT	nValueType;								// Value type
	LPCTSTR	lpszValue;								// String value
	DWORD	dwValue;								// DWORD value
} REGISTRYKEY, *PREGISTRYKEY;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LVCOLUMNFORMAT
//  Description:	Store list control/view column format info
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagLVCOLUMNFORMAT
{
	int		nColumnTitleID;							// Column title ID
	int		nColumnSize;							// Column size
	int		nColumnSizeUnit;						// Column size unit
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
	int		nColID;									// Column ID
	UINT	nHeaderTitleID;							// Header title string ID
	int		nWidth;									// Column width
	int		nWidthUnit;								// Column width unit (pixel or percent)
	UINT	nColStyle;								// Column style
	BOOL	bCenter;								// Align center
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
	DWORD	dwCtrlKeyCode;							// Control Keycode #1
	DWORD	dwFuncKeyCode;							// Function Keycode #2
	UINT	nHotkeyDescription;						// Hotkey description (string ID)
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
	UINT nFirstID;									// First pair ID
	UINT nSecondID;									// Second pair ID
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
	UINT nActionDefID;								// Action ID
	UINT nActionNameID;								// Action Name ID
	UINT nActionMsgID;								// Action message ID
	UINT nSchedNotifyID;							// Schedule notify message ID
	UINT nNotifyTipID;								// Notify file tip ID
	UINT nBallonTipID;								// Balloon tip ID
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
	int		nLength;								// Buffer length
	TCHAR	tcToken[DEF_BUFF_MAXLENGTH];			// Buffer token
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
	BOOL bRequest;									// Request to restart
	BOOL bAdminCheck;								// Check if already running as admin
	BOOL bNotAdminShowMsg;							// If not admin, not show check message
	BOOL bIsAdminDoNothing;							// If already running as admin, do nothing
	BOOL bShowMsgWhenDeny;							// Show message when denied
	BOOL bResetFlag;								// Reset flag when denied
} RESTARTREQ, *PRESTARTREQ;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define global static variables for program, these variables will be used elsewhere in the program
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static IDPAIRLIST idplActionName
{
/*----------Action ID---------------------ActionName ID--------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,	ACTION_NAME_DISPLAYOFF			},
	{ DEF_APP_ACTION_SLEEP,			ACTION_NAME_SLEEP				},
	{ DEF_APP_ACTION_SHUTDOWN,		ACTION_NAME_SHUTDOWN			},
	{ DEF_APP_ACTION_RESTART,		ACTION_NAME_RESTART				},
	{ DEF_APP_ACTION_SIGNOUT,		ACTION_NAME_SIGNOUT				},
	{ DEF_APP_ACTION_HIBERNATE,		ACTION_NAME_HIBERNATE			},
	{ DEF_APP_ACTION_SHOWMENU,		ACTION_NAME_SHOWMENU			},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplActionMsg
{
/*----------Action ID--------------------Message String ID-----------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,	MESSAGE_ACTION_DISPLAYOFF		},
	{ DEF_APP_ACTION_SLEEP,			MESSAGE_ACTION_SLEEP			},
	{ DEF_APP_ACTION_SHUTDOWN,		MESSAGE_ACTION_SHUTDOWN			},
	{ DEF_APP_ACTION_RESTART,		MESSAGE_ACTION_RESTART			},
	{ DEF_APP_ACTION_SIGNOUT,		MESSAGE_ACTION_SIGNOUT			},
	{ DEF_APP_ACTION_HIBERNATE,		MESSAGE_ACTION_HIBERNATE		},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplSchedNotifyMsg
{
/*---------Action ID--------------------Message String ID------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,	MESSAGE_SCHEDNOTIFY_DISPLAYOFF	},
	{ DEF_APP_ACTION_SLEEP,			MESSAGE_SCHEDNOTIFY_SLEEP		},
	{ DEF_APP_ACTION_SHUTDOWN,		MESSAGE_SCHEDNOTIFY_SHUTDOWN	},
	{ DEF_APP_ACTION_RESTART,		MESSAGE_SCHEDNOTIFY_RESTART		},
	{ DEF_APP_ACTION_SIGNOUT,		MESSAGE_SCHEDNOTIFY_SIGNOUT		},
	{ DEF_APP_ACTION_HIBERNATE,		MESSAGE_SCHEDNOTIFY_HIBERNATE	},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplNotifyTip
{
/*---------Action ID--------------------Notify String ID-------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,	NOTIFY_TIP_DISPLAYOFF			},
	{ DEF_APP_ACTION_SLEEP,			NOTIFY_TIP_SLEEP				},
	{ DEF_APP_ACTION_SHUTDOWN,		NOTIFY_TIP_SHUTDOWN				},
	{ DEF_APP_ACTION_RESTART,		NOTIFY_TIP_RESTART				},
	{ DEF_APP_ACTION_SIGNOUT,		NOTIFY_TIP_SIGNOUT				},
	{ DEF_APP_ACTION_HIBERNATE,		NOTIFY_TIP_HIBERNATE			},
	{ DEF_APP_ACTION_SHOWMENU,		NOTIFY_TIP_SHOWMENU				},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplBalloonTip
{
/*---------Action ID--------------------Balloon String ID------------------*/
	{ DEF_APP_ACTION_DISPLAYOFF,	BALLOON_TIP_DISPLAYOFF			},
	{ DEF_APP_ACTION_SLEEP,			BALLOON_TIP_SLEEP				},
	{ DEF_APP_ACTION_SHUTDOWN,		BALLOON_TIP_SHUTDOWN			},
	{ DEF_APP_ACTION_RESTART,		BALLOON_TIP_RESTART				},
	{ DEF_APP_ACTION_SIGNOUT,		BALLOON_TIP_SIGNOUT				},
	{ DEF_APP_ACTION_HIBERNATE,		BALLOON_TIP_HIBERNATE			},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplHKActionID
{
/*----HotKey Action ID---------------------Action ID-----------------------*/
	{ HKID_DISPLAYOFF,				DEF_APP_ACTION_DISPLAYOFF		},
	{ HKID_SLEEP,					DEF_APP_ACTION_SLEEP			},
	{ HKID_SHUTDOWN,				DEF_APP_ACTION_SHUTDOWN			},
	{ HKID_RESTART,					DEF_APP_ACTION_RESTART			},
	{ HKID_SIGNOUT,					DEF_APP_ACTION_SIGNOUT			},
	{ HKID_HIBERNATE,				DEF_APP_ACTION_HIBERNATE		},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplPwrReminderEvt
{
/*--------Event ID----------------------Event String ID--------------------*/
	{ PREVT_AT_SETTIME,				PWRRMD_EVENT_AT_SETTIME			},
	{ PREVT_AT_APPSTARTUP,			PWRRMD_EVENT_AT_APPSTARTUP		},
	{ PREVT_AT_SYSWAKEUP,			PWRRMD_EVENT_AT_SYSWAKEUP		},
	{ PREVT_AT_BFRPWRACTION,		PWRRMD_EVENT_AT_BFRPWRACTION	},
	{ PREVT_AT_PWRACTIONWAKE,		PWRRMD_EVENT_AT_PWRACTIONWAKE	},
	{ PREVT_AT_APPEXIT,				PWRRMD_EVENT_AT_APPEXIT			},
/*-------------------------------------------------------------------------*/
};

static IDPAIRLIST idplPwrReminderStyle
{
/*-----Style ID-------------------------Style String ID--------------------*/
	{ PRSTYLE_MSGBOX,				PWRRMD_STYLE_MESSAGEBOX			},
	{ PRSTYLE_DIALOG,				PWRRMD_STYLE_DIALOG				},
/*-------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplFuncKeyList 
{
/*--------------------------------------------------------------------------*/
	{ VK_F1, _T("F1") }, { VK_F2, _T("F2") }, { VK_F3, _T("F3") }, 
	{ VK_F4, _T("F4") }, { VK_F5, _T("F5") }, { VK_F6, _T("F6") }, 
	{ VK_F7, _T("F7") }, { VK_F8, _T("F8") }, { VK_F9, _T("F9") }, 
	{ VK_F10, _T("F10") }, { VK_F11, _T("F11") }, { VK_F12, _T("F12") },
/*--------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplColorName
{
/*-------Color ID----------------------------Color name---------------------*/
	{ DEF_COLOR_RED,						_T("Red")				},
	{ DEF_COLOR_GREEN,						_T("Green")				},
	{ DEF_COLOR_YELLOW,						_T("Yellow")			},
	{ DEF_COLOR_BLUE,						_T("Blue")				},
	{ DEF_COLOR_WHITE,						_T("White")				},
	{ DEF_COLOR_BLACK,						_T("Black")				},
	{ DEF_COLOR_PINK,						_T("Pink")				},
	{ DEF_COLOR_GRAY,						_T("Gray")				},
	{ DEF_COLOR_DARK_GRAY,					_T("DGray")				},
	{ DEF_COLOR_BRIGHT_GRAY,				_T("BGray")				},
/*--------------------------------------------------------------------------*/
};

static STRINGPAIRLIST strplMsgIconName
{
/*--------Icon ID---------------------------Icon name-----------------------*/
	{ IDI_MSGICON_APPLICATION,				_T("App")				},
	{ IDI_MSGICON_HAND,						_T("Hand")				},
	{ IDI_MSGICON_QUESTION,					_T("Question")			},
	{ IDI_MSGICON_EXCLAMATION,				_T("Exclamation")		},
	{ IDI_MSGICON_ASTERISK,					_T("Asterisk")			},
	{ IDI_MSGICON_WARNING,					_T("Warning")			},
	{ IDI_MSGICON_ERROR,					_T("Error")				},
	{ IDI_MSGICON_INFORMATION,				_T("Information")		},
/*--------------------------------------------------------------------------*/
};

static const HOTKEYINFO hklExistedSysHotkeyList[] =
{
/*------Control Key----------Function Key------Hotkey description-----------*/
	{ MOD_ALT,					VK_F4,		HKEYSET_EXISTED_ALT_F4		},
	{ MOD_ALT,					VK_F8,		HKEYSET_EXISTED_ALT_F8		},
	{ MOD_CONTROL,				VK_F3,		HKEYSET_EXISTED_CTRL_F3		},
	{ MOD_CONTROL,				VK_F4,		HKEYSET_EXISTED_CTRL_F4		},
	{ MOD_CONTROL,				VK_F5,		HKEYSET_EXISTED_CTRL_F5		},
	{ MOD_CONTROL | MOD_WIN,	VK_F4,		HKEYSET_EXISTED_CTRL_WIN_F4 },
/*--------------------------------------------------------------------------*/
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

	// Logging and messages functions
	void TraceLog(LPCSTR lpszTraceLogA);
	void TraceLog(LPCTSTR lpszTraceLogW);
	void TraceLogFormat(LPCSTR lpszTraceLogFormat, ...);
	void TraceLogFormat(LPCTSTR lpszTraceLogFormat, ...);
	void OutputDebugLog(LPCTSTR lpszDebugLog, int nForceStyle = -1);
	void OutputDebugLogFormat(LPCTSTR lpszDebugLogFormat, ...);
	void WriteTraceNDebugLogFile(LPCTSTR lpszFileName, LPCTSTR lpszLogStringW);
	void ShowErrorMessage(HWND hWnd, UINT nLanguageID, DWORD dwErrCode);
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

	// String functions
	BOOL LoadResourceString(CString& strResult, UINT nResStringID);
	int	 GetTokenList(LPTSTR lpszBuff, BUFFER* retBuff, LPCTSTR lpszKeyChars);
	void UpperEachWord(CString& strInput, BOOL bTrim);
	BOOL StringValidate(LPCTSTR lpszSrc, DWORD& dwError);
	BOOL SubString(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd, UINT nSubStringType);
	BOOL Left(LPCTSTR lpszSrc, CString& strDest, TCHAR tcEnd);
	BOOL Mid(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart, TCHAR tcEnd);
	BOOL Right(LPCTSTR lpszSrc, CString& strDest, TCHAR tcStart);

	// Additional functions
	LPCTSTR GetAppPath(void);
	BOOL	AddRegistryKey(REGISTRYKEY regKeyInfo);
	LPCTSTR MakeRegFullPath(REGISTRYKEY regKeyInfo, UINT nRegPathType = REGPATH_FULL);

	void	PlaySound(BOOL bSoundEnable, UINT nTypeOfSound);
	BOOL	FileViewStd(FILETYPE eFileType, LPCTSTR lpszFilePath);
	BOOL	OpenWebURL(LPCTSTR lpszWebUrl);

	LRESULT RunApp(LPCTSTR lpszAppPath, BOOL bRunAsAdmin = FALSE, BOOL bShowFlag = TRUE);
	LRESULT ExecuteCommand(LPCTSTR lpszCmd, BOOL bRunAsAdmin = TRUE, BOOL bShowFlag = TRUE);
	BOOL	CreateAppProcess(LPCWSTR lpszAppPath, LPWSTR lpszCmdLine, UINT nStyle, DWORD& dwErrorCode);

	BOOL	SetDarkMode(CWnd* pWnd, BOOL bEnableDarkMode);
	void	DrawButton(CButton*& pButton, UINT nIconID, LPCTSTR lpszButtonTitle);
	BOOL	EnumFontNames(std::vector<std::wstring>& fontNames);
	BOOL	ValidateFontName(LPCTSTR lpszFontName);
};

#endif	// ifndef _CORE_H_INCLUDED