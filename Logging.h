
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.h
//		Description:	Define necessary variables and methods to read/write app log
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _LOGGING_H_INCLUDED
#define _LOGGING_H_INCLUDED

#include "stdafx.h"
#include "Core.h"
#include "Logging_pub.h"

using namespace PairFuncs;
using namespace CoreFuncs;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for logging
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LOGDETAIL
//  Description:	Store log detail info item
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagLOGDETAIL
{
	// Member variables
	USHORT	usCategory;										// Detail category
	UINT	uiDetailInfo;									// Detail info (integer)
	CString	strDetailInfo;									// Detail info (string)
	PVOID	ptrDetailInfo;									// Detail info (pointer)
	BYTE	byPointerType;									// Detail info pointer data type
	SIZE_T	szPointerSize;									// Detail info pointer data size

	// Constructor
	tagLOGDETAIL();											// Default constructor
	tagLOGDETAIL(const tagLOGDETAIL&);						// Copy constructor

	// Operator
	tagLOGDETAIL& operator=(const tagLOGDETAIL&);			// Copy assignment operator

	// Member functions
	void Init();											// Initialize/reset item data
	void Copy(const tagLOGDETAIL&);							// Copy item data
	void PointerCopy(const tagLOGDETAIL&);					// Copy detail info pointers
	BOOL Compare(const tagLOGDETAIL&) const;				// Compare items
	BOOL PointerCompare(const tagLOGDETAIL&) const;			// Compare detail info pointer
	BOOL IsEmpty(void) const;								// Check if item data is empty
	BOOL SetPointerData(PVOID, BYTE = -1, SIZE_T = 0);		// Set detail info pointer data
} LOGDETAIL, *PLOGDETAIL;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LOGDETAILINFO
//  Description:	Store log detail info data
//  Derivered from: MFC CArray class
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<LOGDETAIL, LOGDETAIL> LOGDETAILINFO;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LOGITEM
//  Description:	Store app log item data
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagLOGITEM
{
	// Member variables
	SYSTEMTIME		stTime;									// Log time
	DWORD			dwProcessID;							// Process ID
	USHORT			usCategory;								// Log category
	CString			strLogString;							// Log description string
	LOGDETAILINFO	arrDetailInfo;							// Log detail info

	// Constructor
	tagLOGITEM();											// Default constructor
	tagLOGITEM(const tagLOGITEM&);							// Copy constructor

	// Operator
	tagLOGITEM& operator=(const tagLOGITEM&);				// Copy assignment operator

	// Member functions
	void Copy(const tagLOGITEM&);							// Copy item
	BOOL Compare(const tagLOGITEM&) const;					// Compare items
	BOOL IsEmpty(void) const;								// Check if item data is empty
	void RemoveDetailInfo(void);							// Remove all log detail info data
	void RemoveAll(void);									// Remove all log item data

	// Detail info functions
	void AddDetail(const LOGDETAIL&);						// Add detail data
	void AddDetail(USHORT, UINT);							// Add detail (integer data only)
	void AddDetail(USHORT, LPCTSTR);						// Add detail (string data only)
	void AddDetail(USHORT, UINT, LPCTSTR);					// Add detail (both integer and string data)

	// Format data functions
	CString	FormatDateTime(void) const;						// Format date/time value
	CString FormatOutput(void) const;						// Format log item output
} LOGITEM, *PLOGITEM;

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	LOGARRAY
//  Description:	Store app log data
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef CArray<LOGITEM, LOGITEM> LOGARRAY;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define static functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename DATA>
static SIZE_T GetSizeByValue(DATA dataValue);
static SIZE_T GetSizeByType(BYTE byDataType);

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	JSONDATA
//  Description:	Store JSON format object
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

typedef struct tagJSONDATA
{
	// Member variables
	CString strGroupName;									// JSON group name
	CStringArray astrItemName;								// List of item names
	CStringArray astrItemValue;								// List of item values (string)

	INT_PTR	nSubItemNum;									// Number of nested sub-items
	tagJSONDATA** apSubItemData;							// List of nested sub-items

	// Constructor
	tagJSONDATA();											// Default constructor
	tagJSONDATA(const tagJSONDATA&);						// Copy constructor
	~tagJSONDATA();											// Destructor

	// Operator
	tagJSONDATA& operator=(const tagJSONDATA&);				// Copy assignment operator

	// Member functions
	void Copy(const tagJSONDATA&);							// Copy item
	void CopyArrayData(const tagJSONDATA&);					// Copy JSON item array data
	void CopyPtrData(const tagJSONDATA&);					// Copy JSON item pointer data
	BOOL Compare(const tagJSONDATA&) const;					// Compare items
	BOOL IsEmpty(void) const;								// Check if item data is empty
	void RemoveItem(int);									// Remove item by index
	void RemoveItem(LPCTSTR);								// Remove item by name
	void RemoveAll(void);									// Remove all log item data

	void SetGroupName(LPCTSTR);								// Set group name
	void AddStringItem(LPCTSTR, LPCTSTR);					// Add string item
	void AddIntItem(LPCTSTR, UINT);							// Add integer item
	void AddFloatItem(LPCTSTR, DOUBLE);						// Add float item
	void AddSubItem(tagJSONDATA*);							// Add sub-item

	void Print(CString&, int, BOOL, BOOL = TRUE);			// Print item data (with indentation)
} JSONDATA, *PJSONDATA;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define static data tables for logging, these data will be used for logging feature only
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//	Table name:		strplLogInfoIDTitle
//  Description:	Using for pairing logging info IDs and the titles when
//					writing log files
//  Table type:		STRINGPAIRLIST
//
//////////////////////////////////////////////////////////////////////////

static STRINGPAIRLIST strplLogInfoIDTitle
{
/*-----Base log info IDs-----------------------------Log Info Titles----------------*/
	{ BASELOG_INFO_TIME,							_T("time")					},
	{ BASELOG_INFO_PID,								_T("pid")					},
	{ BASELOG_INFO_CATEGORY,						_T("category")				},
	{ BASELOG_INFO_DESCRIPTION,						_T("description")			},
	{ BASELOG_INFO_DETAILS,							_T("details")				},
/*----------------------------------------------------------------------------------*/

/*-----Base log info IDs-----------------------------Log Info Titles----------------*/
	{ EVENTLOG_DETAIL_RESOURCEID,					_T("resourceid")			},
	{ EVENTLOG_DETAIL_MAPTEXTID,					_T("maptextid")				},
	{ EVENTLOG_DETAIL_DIALOGCAPTION,				_T("dialogcaption")			},
	{ EVENTLOG_DETAIL_CTRLCAPTION,					_T("ctrlcaption")			},
	{ EVENTLOG_DETAIL_CHKSTATE,						_T("checkstate")			},
	{ EVENTLOG_DETAIL_SELECTION,					_T("selection")				},
	{ EVENTLOG_DETAIL_DATAVALUE,					_T("datavalue")				},
	{ EVENTLOG_DETAIL_DATACHANGE,					_T("datachange")			},
	{ EVENTLOG_DETAIL_CONTENTID,					_T("contentid")				},
	{ EVENTLOG_DETAIL_MESSAGETEXT,					_T("messagetext")			},
/*----------------------------------------------------------------------------------*/

/*-----History log info IDs--------------------------Log Info Titles----------------*/
	{ HISTORYLOG_DETAIL_CATEGORY,					_T("category")				},
	{ HISTORYLOG_DETAIL_ACTION,						_T("action")				},
	{ HISTORYLOG_DETAIL_KEYSTROKES,					_T("keystrokes")			},
	{ HISTORYLOG_DETAIL_ITEMID,						_T("itemid")				},
	{ HISTORYLOG_DETAIL_MESSAGE,					_T("message")				},
	{ HISTORYLOG_DETAIL_RESULT,						_T("result")				},
/*----------------------------------------------------------------------------------*/
};


//////////////////////////////////////////////////////////////////////////
//
//	Table name:		strplHistoryLogDetail
//  Description:	Using for pairing history log detail info IDs and the
//					titles when writing log files
//  Table type:		STRINGPAIRLIST
//
//////////////////////////////////////////////////////////////////////////

static STRINGPAIRLIST strplHistoryLogDetail
{
/*-----History category IDs--------------------------Log Info Titles----------------*/
	{ HISTORYLOG_CATE_PWRACTION,			_T("Power Action")					},
	{ HISTORYLOG_CATE_SCHEDULE,				_T("Schedule")						},
	{ HISTORYLOG_CATE_HOTKEYSET,			_T("HotkeySet")						},
	{ HISTORYLOG_CATE_PWRREMINDER,			_T("Power Reminder")				},
/*----------------------------------------------------------------------------------*/

/*-----History log action IDs------------------------Log Info Titles----------------*/
	{ HISTORYLOG_ACTION_NOTHING,			_T("Do nothing")					},
	{ HISTORYLOG_ACTION_DISPLAYOFF,			_T("Turn off display")				},
	{ HISTORYLOG_ACTION_SLEEP,				_T("Sleep")							},
	{ HISTORYLOG_ACTION_SHUTDOWN,			_T("Shutdown")						},
	{ HISTORYLOG_ACTION_RESTART,			_T("Restart")						},
	{ HISTORYLOG_ACTION_SIGNOUT,			_T("Sign out")						},
	{ HISTORYLOG_ACTION_HIBERNATE,			_T("Hibernate")						},
/*----------------------------------------------------------------------------------*/

/*-----History log action IDs------------------------Log Info Titles----------------*/
	{ HISTORYLOG_RESULT_SUCCESS,			_T("Success")						},
	{ HISTORYLOG_RESULT_FAILED_UNKNOWN,		_T("Failed (Unknown error)")		},
	{ HISTORYLOG_RESULT_FAILED_ERRCODE,		_T("Failed (Error code: 0x%08X)")	},
/*----------------------------------------------------------------------------------*/
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define neccessary classes for logging feature
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////
//
//	Class name:	 SLogging
//  Description: Using for saving application log data
//
////////////////////////////////////////////////////////

class SLogging
{
private:
	// Log data array
	LOGARRAY m_arrLogData;

	// Properties
	BYTE	 m_byLogType;					// Log type
	BYTE	 m_byWriteMode;					// Log write mode
	INT_PTR	 m_nMaxSize;					// Log data max item count
	CString  m_strFilePath;					// Log output file path
	PLOGITEM m_pItemDefTemplate;			// Log default template

public:
	SLogging(BYTE byLogType);				// constructor
	~SLogging(void);						// destructor

public:
	// Initialization
	virtual void Init();
	virtual void DeleteAll();

	// Get/set data
	virtual BOOL IsEmpty(void) const;
	virtual INT_PTR GetLogCount() const;
	virtual LOGITEM& GetLogItem(int nIndex);
	virtual const LOGITEM& GetLogItem(int nIndex) const;

	// Get/set properties function
	virtual INT_PTR GetMaxSize(void) const;
	virtual void SetMaxSize(INT_PTR nMaxSize);
	virtual BYTE GetWriteMode(void) const;
	virtual void SetWriteMode(BYTE byWriteMode);
	virtual void GetFilePath(CString& strFilePath);
	virtual void SetFilePath(LPCTSTR lpszFilePath);
	virtual PLOGITEM GetDefaultTemplate(void);
	virtual void SetDefaultTemplate(const LOGITEM& logItemTemplate);

	// Output log functions
	void OutputItem(const LOGITEM& logItem);
	void OutputString(LPCTSTR lpszLogString, BOOL bUseLastTemplate = TRUE);

	// Write log functions
	BOOL Write(void);
	BOOL Write(const LOGITEM& logItem, LPCTSTR lpszFilePath = NULL);
	BOOL Write(LPCTSTR lpszLogString, LPCTSTR lpszFilePath = NULL);
};

#endif	// ifndef _LOGGING_H_INCLUDED