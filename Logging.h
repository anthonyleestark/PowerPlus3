
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.h
//		Description:	Define necessary variables and methods to read/write app log
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGGING_H_INCLUDED
#define _LOGGING_H_INCLUDED

#include "stdafx.h"
#include "Core.h"

using namespace PairFuncs;
using namespace CoreFuncs;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define macros for logging
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOG_EVENT_BUTTON					0x01
#define LOG_EVENT_BUTTON_CLICKED			(LOG_EVENT_BUTTON<<8) + 0x001
#define LOG_EVENT_BUTTON_ENABLED			(LOG_EVENT_BUTTON<<8) + 0x002
#define LOG_EVENT_BUTTON_DISABLED			(LOG_EVENT_BUTTON<<8) + 0x003

#define LOG_EVENT_COMBO						0x02
#define LOG_EVENT_COMBO_SELECTION			(LOG_EVENT_COMBO<<8) + 0x001
#define LOG_EVENT_COMBO_ENABLED				(LOG_EVENT_COMBO<<8) + 0x002
#define LOG_EVENT_COMBO_DISABLED			(LOG_EVENT_COMBO<<8) + 0x003

#define LOG_EVENT_CHKBOX					0x03
#define LOG_EVENT_CHKBOX_CHECKED			(LOG_EVENT_CHKBOX<<8) + 0x001
#define LOG_EVENT_CHKBOX_UNCHECKED			(LOG_EVENT_CHKBOX<<8) + 0x002
#define LOG_EVENT_CHKBOX_ENABLED			(LOG_EVENT_CHKBOX<<8) + 0x003
#define LOG_EVENT_CHKBOX_DISABLED			(LOG_EVENT_CHKBOX<<8) + 0x004

#define LOG_EVENT_MENU						0x04
#define LOG_EVENT_MENU_SELECTION			(LOG_EVENT_MENU<<8) + 0x001
#define LOG_EVENT_MENU_ENABLED				(LOG_EVENT_MENU<<8) + 0x002
#define LOG_EVENT_MENU_DISABLED				(LOG_EVENT_MENU<<8) + 0x003

#define LOG_EVENT_DLG						0x05
#define LOG_EVENT_DLG_STARTUP				(LOG_EVENT_DLG<<8) + 0x001
#define LOG_EVENT_DLG_DESTROYED				(LOG_EVENT_DLG<<8) + 0x002
#define LOG_EVENT_DLG_SHOWED				(LOG_EVENT_DLG<<8) + 0x003
#define LOG_EVENT_DLG_HIDDEN				(LOG_EVENT_DLG<<8) + 0x004
#define LOG_EVENT_DLG_EXPANDED				(LOG_EVENT_DLG<<8) + 0x005
#define LOG_EVENT_DLG_COLLAPSED				(LOG_EVENT_DLG<<8) + 0x006

#define LOG_EVENT_NOTIFY					0x06
#define LOG_EVENT_NOTIFY_CREATED			(LOG_EVENT_NOTIFY<<8) + 0x001
#define LOG_EVENT_NOTIFY_REMOVED			(LOG_EVENT_NOTIFY<<8) + 0x002
#define LOG_EVENT_NOTIFY_LMBCLICKED			(LOG_EVENT_NOTIFY<<8) + 0x003
#define LOG_EVENT_NOTIFY_MMBCLICKED			(LOG_EVENT_NOTIFY<<8) + 0x004
#define LOG_EVENT_NOTIFY_RMBCLICKED			(LOG_EVENT_NOTIFY<<8) + 0x005
#define LOG_EVENT_NOTIFY_SHOWMENU			(LOG_EVENT_NOTIFY<<8) + 0x006
#define LOG_EVENT_NOTIFY_UPDATETIPTEXT		(LOG_EVENT_NOTIFY<<8) + 0x007

/* <Use this template to define new logevent group macros>
#define LOG_EVENT_XXX				0x0
#define LOG_EVENT_XXX_YYY			(LOG_EVENT_XXX<<8) + 0x001
#define LOG_EVENT_XXX_YYY			(LOG_EVENT_XXX<<8) + 0x002
#define LOG_EVENT_XXX_YYY			(LOG_EVENT_XXX<<8) + 0x003
<REMEMBER: New group must come with new log output method> */


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for logging
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagLOGITEM
{
	// Member variables
	SYSTEMTIME	stTime;				// Log time
	BYTE		byType;				// Log type
	BYTE		byCategory;			// Log category
	CString		strLogString;		// Log string
	CString		strDetails;			// Log detail string

	// Constructor
	tagLOGITEM();					// Default constructor
	tagLOGITEM(const tagLOGITEM&);	// Copy constructor

	// Member functions
	CString		FormatDateTime();	// Format date/time value
	CString		FormatLogString();	// Format log string
} LOGITEM, *PLOGITEM;

typedef CArray<LOGITEM, LOGITEM> LOGARRAY;

typedef enum eLOGWRITEMODE {
	LOG_WRITE_MODE_ONCALL = 1,		// Write log on-call mode
	LOG_WRITE_MODE_INSTANT,			// Write log instantly mode
} LOGWRITEMODE;

typedef enum eDATACHANGELOG {
	// Data types
	DATATYPE_YESNO_VALUE = 0,		// Yes/No value (boolean)
	DATATYPE_OPTION_VALUE,			// Option value (ID)
	DATATYPE_ITEMNUM_ADD,			// Item number added
	DATATYPE_ITEMNUM_REMOVE,		// Item number removed
	DATATYPE_STRING_VALUE,			// String value
	DATATYPE_TIME_VALUE,			// Time value
	DATATYPE_FLAG_VALUE,			// Flag value
	DATATYPE_SPEC_VALUE,			// Special value

	// Data categories
	DATACATE_APPCONFIG,				// App config data
	DATACATE_SCHEDULE,				// Schedule data
	DATACATE_HOTKEYSET,				// HotkeySet data
	DATACATE_PWRREMINDER,			// Power Reminder data
} DATACHANGELOG;


////////////////////////////////////////////////////////
//
//	Class name:	 SLogging
//  Description: Using for saving logs
//
////////////////////////////////////////////////////////

class SLogging
{
private:
	// Member variables
	LOGARRAY m_arrLogData;		// Log data array
	UINT	 m_nLogDataType;	// Log data type
	UINT	 m_nLogWriteMode;	// Log write mode

public:
	SLogging();		// constructor
	~SLogging();	// destructor

public:
	// Initialization
	void Init();
	int  GetLogCount();
	LOGITEM GetLogItem(int nIndex);

	// Get/set properties function
	UINT GetLogDataType();
	void SetLogDataType(UINT logDataType);
	UINT GetLogWriteMode();
	void SetLogWriteMode(UINT logWriteMode);

	// Output log functions
	void OutputLog(LOGITEM& logItem, UINT nForceWriteMode = 0);
	void OutputLogString(LPCSTR lpszLogStringA, LPCSTR lpszLogDetailA = NULL, BYTE byType = 0);
	void OutputLogString(LPCTSTR lpszLogStringW, LPCTSTR lpszLogDetailW = NULL, BYTE byType = 0);

	// Write log functions
	BOOL WriteLog();
	BOOL WriteInstantLog(LOGITEM& logItem);
	BOOL WriteInstantLog(LPCSTR lpszLogStringA, LPCSTR lpszLogDetailA = NULL, BYTE byType = 0);
	BOOL WriteInstantLog(LPCTSTR lpszLogStringW, LPCTSTR lpszLogDetailW = NULL, BYTE byType = 0);

	// Custom specified output log functions
	void OutputDlgEventLog(UINT nEventID, LPCSTR lpszDlgID = NULL, LPCSTR lpszItemID = NULL, WPARAM wParam = NULL, LPARAM lParam = NULL);
	void OutputDataChangeLog(BYTE byDataType, BYTE byDataCategory, DWORD dwCtrlID, DWORD dwPreVal, DWORD wAftVal, LPCTSTR lpszFlagName);
	void OutputDataChangeLog(BYTE byDataType, BYTE byDataCategory, DWORD dwCtrlID, LPCTSTR lpszPreVal, LPCTSTR lpszAftVal, LPCTSTR lpszFlagName);
};

#endif	// ifndef _LOGGING_H_INCLUDED