
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging_defs.h
//		Description:	Define public resources and info using for logging function
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.12.08:		Create new
//						<1> 2025.06.03:		Rename from Logging_pub to Logging_defs
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define macros for logging features
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Log category macros
//

#define LOG_MACRO_NONE						0x00
#define LOG_MACRO_EVENT_APP					(LOG_MACRO_NONE+0x01)
#define LOG_MACRO_EVENT_UI					(LOG_MACRO_NONE+0x02)
#define LOG_MACRO_EVENT_DATACHG				(LOG_MACRO_NONE+0x03)
#define LOG_MACRO_HISTORY					(LOG_MACRO_NONE+0x04)


// Application events
//

#define LOG_EVENT_INIT_INSTANCE				(LOG_MACRO_EVENT_APP<<8) + 0x001
#define LOG_EVENT_EXIT_INSTANCE				(LOG_MACRO_EVENT_APP<<8) + 0x002
#define LOG_EVENT_ERROR_MESSAGE				(LOG_MACRO_EVENT_APP<<8) + 0x003
#define LOG_EVENT_EXEC_DEBUGCMD				(LOG_MACRO_EVENT_APP<<8) + 0x004
#define LOG_EVENT_CHANGE_LANGUAGE			(LOG_MACRO_EVENT_APP<<8) + 0x005


// UI events
//

#define LOG_EVENT_BTN_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x001
#define LOG_EVENT_CHK_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x002
#define LOG_EVENT_RAD_CLICKED				(LOG_MACRO_EVENT_UI<<8) + 0x003
#define LOG_EVENT_EDIT_CHANGED				(LOG_MACRO_EVENT_UI<<8) + 0x004
#define LOG_EVENT_CMB_SELCHANGE				(LOG_MACRO_EVENT_UI<<8) + 0x005
#define LOG_EVENT_LIST_SELCHANGE			(LOG_MACRO_EVENT_UI<<8) + 0x006
#define LOG_EVENT_SPIN_CHANGED				(LOG_MACRO_EVENT_UI<<8) + 0x007
#define LOG_EVENT_MENU_SELECTED				(LOG_MACRO_EVENT_UI<<8) + 0x008

#define LOG_EVENT_DLG_INIT					(LOG_MACRO_EVENT_UI<<8) + 0x009
#define LOG_EVENT_DLG_DESTROYED				(LOG_MACRO_EVENT_UI<<8) + 0x00A
#define LOG_EVENT_DLG_SHOWED				(LOG_MACRO_EVENT_UI<<8) + 0x00B
#define LOG_EVENT_DLG_HIDDEN				(LOG_MACRO_EVENT_UI<<8) + 0x00C
#define LOG_EVENT_DLG_EXPANDED				(LOG_MACRO_EVENT_UI<<8) + 0x00D
#define LOG_EVENT_DLG_COLLAPSED				(LOG_MACRO_EVENT_UI<<8) + 0x00E

#define LOG_EVENT_TRAYICO_LMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x00F
#define LOG_EVENT_TRAYICO_MMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x010
#define LOG_EVENT_TRAYICO_RMBCLICKED		(LOG_MACRO_EVENT_UI<<8) + 0x011

#define LOG_EVENT_EXEC_PWRACTION			(LOG_MACRO_EVENT_UI<<8) + 0x012
#define LOG_EVENT_CANCEL_PWRACTION			(LOG_MACRO_EVENT_UI<<8) + 0x013
#define LOG_EVENT_EXEC_SCHEDULE				(LOG_MACRO_EVENT_UI<<8) + 0x014
#define LOG_EVENT_CANCEL_SCHEDULE			(LOG_MACRO_EVENT_UI<<8) + 0x015
#define LOG_EVENT_EXEC_HOTKEY				(LOG_MACRO_EVENT_UI<<8) + 0x016
#define LOG_EVENT_DISP_PWRREMINDER			(LOG_MACRO_EVENT_UI<<8) + 0x017


// Data change events
//

#define LOG_EVENT_DATACHG_CONFIG			(LOG_MACRO_EVENT_DATACHG<<8) + 0x001
#define LOG_EVENT_DATACHG_SCHEDULE			(LOG_MACRO_EVENT_DATACHG<<8) + 0x002
#define LOG_EVENT_DATACHG_HOTKEYSET			(LOG_MACRO_EVENT_DATACHG<<8) + 0x003
#define LOG_EVENT_DATACHG_PWRREMINDER		(LOG_MACRO_EVENT_DATACHG<<8) + 0x004


// History log
//

#define LOG_HISTORY_EXEC_PWRACTION			(LOG_MACRO_HISTORY<<8) + 0x001
#define LOG_HISTORY_EXEC_SCHEDULE			(LOG_MACRO_HISTORY<<8) + 0x002
#define LOG_HISTORY_EXEC_HOTKEY				(LOG_MACRO_HISTORY<<8) + 0x003
#define LOG_HISTORY_DISP_PWRREMINDER		(LOG_MACRO_HISTORY<<8) + 0x004


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum data types for base logging feature
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////// ********************
// 
// Log write modes
//
//////////////////// ********************

enum LogWriteMode {
	ReadOnly = 0,						// Read-only log data (do not write)
	WriteOnCall,						// Write log on-call mode
	WriteInstantly,						// Write log instantly mode
};


//////////////////// ********************
// 
// Log detail data types
//
//////////////////// ********************

enum LogDataType {
	Unspecified = -1,					// Unspecified type
	Void = 0,							// No type (unusable)
	Num_U1,								// Unsigned integer (1-byte)
	Num_U2,								// Unsigned integer (2-byte)
	Num_U4,								// Unsigned integer (4-byte)
	Num_U8,								// Unsigned integer (8-byte)
	Num_I1,								// Signed integer (1-byte)
	Num_I2,								// Signed integer (2-byte)
	Num_I4,								// Signed integer (4-byte)
	Num_I8,								// Signed integer (8-byte)
	Num_F1,								// Float number (1-byte)
	Num_F2,								// Float number (2-byte)
	Num_F4,								// Float number (4-byte)
	Num_F8,								// Float number (8-byte)
	Boolean,							// Boolean number (TRUE/FALSE)
	Text_ANSI,							// Text value (ANSI)
	Text_Unicode,						// Text value (Unicode)
	SystemTime,							// SYSTEMTIME struct
};


//////////////////// ********************
// 
// Log detail flags
//
//////////////////// ********************

enum LogDetailFlag
{
	Flag_Null			= (int)0x000,	// NULL flag (apply default behavior)
	ReadOnly_Data		= (int)0x001,	// Read-only & do not write to file (applied for all data)
	ReadOnly_Pointer	= (int)0x002,	// Read-only but applied for pointer data only
	Write_Int			= (int)0x004,	// Use numeric integer value when write to file
	Write_String		= (int)0x008,	// Use string value when write to file
	LookUp_Dict			= (int)0x010,	// Use numeric value as a key to look up for string from dictionary
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define enum data types for specific logging types
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////// ********************
// 
// Base log key IDs
//
//////////////////// ********************

enum BaseLog {
	Time = 0x0100,				// Log time
	PID,						// Process ID
	LogCategory,				// Log category
	Description,				// Log description string
	Details,					// Log details info
	DetailNumeric,				// Log detail numeric value
	DetailString,				// Log detail string value
};


//////////////////// ********************
// 
// Event log detail category IDs
//
//////////////////// ********************

enum EventDetail {
	ResourceID = 0x0200,		// UI component resource ID
	NameID,						// Name ID
	DialogCaption,				// Dialog caption
	ControlCaption,				// Control caption
	CheckState,					// Checkbox/radio button, ... checked state
	Selection,					// Combo-box/listbox, ... selection
	DataValue,					// Data value
	DataChangeState,			// Data change state
	ContentID,					// Content ID
	MessageText,				// Message box text
	EventError,					// Error code
};


//////////////////// ********************
// 
// Action history log detail IDs
//
//////////////////// ********************

enum HistoryDetail {
	Category = 0x0300,			// Category
	Action,						// Action
	Keystrokes,					// Keystrokes
	ItemID,						// Item ID
	Message,					// Message
	Result,						// Result
	ActionError					// Error code
};


//////////////////// ********************
// 
// Action history category IDs
//
//////////////////// ********************

enum HistoryCategory {
	PowerAction = 0x0400,		// Power action
	ScheduleAction ,			// Schedule
	HotkeySet,					// HotkeySet
	PowerReminder,				// Power Reminder
};


//////////////////// ********************
// 
// Action history action IDs
//
//////////////////// ********************

enum HistoryAction {
	DoNothing = 0x0500,			// Do nothing
	DisplayOff,					// Turn off display
	SleepMode,					// Sleep
	Shutdown,					// Shutdown
	Restart,					// Restart
	SignOut,					// Sign out
	Hibernate,					// Hibernate
};


//////////////////// ********************
// 
// Action history result IDs
//
//////////////////// ********************

enum HistoryResult {
	Success = 0x0600,			// Success
	FailedUnknown,				// Failed (Unknown error)
	FailedWithErrorCode,		// Failed (Error code: 0x%08X)
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                                             */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
