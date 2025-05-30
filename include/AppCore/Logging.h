
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
#include "AppCore.h"
#include "MapTable.h"
#include "Logging_pub.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define data types for logging
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		LogDetail
//  Description:	Store log detail info item
//
//////////////////////////////////////////////////////////////////////////

class LogDetail
{
private:
	// Attributes
	USHORT	m_usCategory;									// Detail category
	INT		m_nFlag;										// Detail flag

	// Data
	INT		m_nDetailValue;									// Detail value (integer)
	CString	m_strDetailInfo;								// Detail info (string)
	PVOID	m_ptrDetailData;								// Detail data (pointer)
	BYTE	m_byPointerType;								// Detail info pointer data type
	SIZE_T	m_szPointerSize;								// Detail info pointer data size

public:
	// Construction
	LogDetail();											// Default constructor
	LogDetail(const LogDetail&);							// Copy constructor

	// Operator
	LogDetail& operator=(const LogDetail&);					// Copy assignment operator

public:
	// Member functions
	void Init();											// Initialize/reset item data
	void Copy(const LogDetail&);							// Copy item data
	void PointerCopy(const LogDetail&);						// Copy detail info pointers
	BOOL Compare(const LogDetail&) const;					// Compare items
	BOOL PointerCompare(const LogDetail&) const;			// Compare detail info pointer
	BOOL IsEmpty(void) const;								// Check if item data is empty

public:
	// Get/set functions
	USHORT GetCategory(void) const;							// Get detail category
	void SetCategory(USHORT);								// Set detail category
	INT GetFlag(void) const;								// Get detail flag
	void SetFlag(INT nFlag);								// Set detail flag
	INT GetDetailValue(void) const;							// Get detail value (integer)
	void SetDetailValue(INT);								// Set detail value (integer)
	CString GetDetailString(void) const;					// Get detail info (string)
	void SetDetailString(LPCTSTR);							// Set detail info (string)
	PVOID GetPointerData(void) const;						// Get detail data pointer data
	BOOL SetPointerData(PVOID, BYTE = -1, SIZE_T = 0);		// Set detail data pointer data
	BYTE GetPointerType(void) const;						// Get detail data pointer type
	void SetPointerType(BYTE byType);						// Set detail data pointer type
	SIZE_T GetPointerSize(void) const;						// Get detail data pointer size
	void SetPointerSize(SIZE_T szSize);						// Set detail data pointer size
};

// Define new typenames for LogData
using LOGDETAIL = typename LogDetail;
using PLOGDETAIL = typename LogDetail*;
using LOGDETAILARRAY = typename std::vector<LogDetail>;


//////////////////////////////////////////////////////////////////////////
//
//	Class name:		LogDetailInfo
//  Description:	Store app log data detail info
//
//////////////////////////////////////////////////////////////////////////

class LogDetailInfo : public LOGDETAILARRAY
{
public:
	// Construction
	LogDetailInfo();										// Default constructor
	LogDetailInfo(const LogDetailInfo&);					// Copy constructor

	// Operator
	LogDetailInfo& operator=(const LogDetailInfo&);			// Copy assignment operator

public:
	// Member functions
	void Init(void);										// Initialize/reset data
	void CopyData(const LogDetailInfo&);					// Copy log detail info data

public:
	// Update data functions
	void AddDetail(const LOGDETAIL&);						// Add detail item
	void AddDetail(USHORT, INT, INT = 0);					// Add detail item (integer data only)
	void AddDetail(USHORT, LPCTSTR, INT = 0);				// Add detail item (string data only)
	void AddDetail(USHORT, INT, LPCTSTR, INT = 0);			// Add detail item (both integer and string data)
};

// Define new typenames for LogDetailInfo
using LOGDETAILINFO = typename LogDetailInfo;
using PLOGDETAILINFO = typename LogDetailInfo*;


//////////////////////////////////////////////////////////////////////////
//
//	Class name:		LogItem
//  Description:	Store app log item data
//
//////////////////////////////////////////////////////////////////////////

class LogItem
{
private:
	// Member variables
	SYSTEMTIME		m_stTime;								// Log time
	DWORD			m_dwProcessID;							// Process ID
	USHORT			m_usCategory;							// Log category
	CString			m_strLogString;							// Log description string
	LOGDETAILINFO	m_arrDetailInfo;						// Log detail info

public:
	// Construction
	LogItem();												// Default constructor
	LogItem(const LogItem&);								// Copy constructor

	// Operator
	LogItem& operator=(const LogItem&);						// Copy assignment operator

public:
	// Member functions
	void Copy(const LogItem&);								// Copy item
	BOOL Compare(const LogItem&) const;						// Compare items
	BOOL IsEmpty(void) const;								// Check if item data is empty
	void RemoveDetailInfo(void);							// Remove all log detail info data
	void RemoveAll(void);									// Remove all log item data

public:
	// Get/set functions
	SYSTEMTIME GetTime(void) const;							// Get log time
	void SetTime(const SYSTEMTIME& stTime);					// Set log time
	DWORD GetProcessID(void) const;							// Get process ID
	void SetProcessID(void);								// Set process ID
	USHORT GetCategory(void) const;							// Get log category
	void SetCategory(USHORT usCategory);					// Set log category
	CString GetLogString(void) const;						// Get log description string
	void SetLogString(LPCTSTR lpszLogString);				// Set log description string

	// Detail info functions
	void AddDetail(const LOGDETAIL&);						// Add detail data
	void AddDetail(USHORT, INT, INT = 0);					// Add detail (integer data only)
	void AddDetail(USHORT, LPCTSTR, INT = 0);				// Add detail (string data only)
	void AddDetail(USHORT, INT, LPCTSTR, INT = 0);			// Add detail (both integer and string data)

	// Format data functions
	CString	FormatDateTime(void) const;						// Format date/time value
	CString FormatOutput(void) const;						// Format log item output
};

// Define new typenames for LogItem
using LOGITEM = typename LogItem;
using PLOGITEM = typename LogItem*;
using LOGDATA = typename std::vector<LogItem>;
using PLOGDATA = typename LOGDATA*;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define static functions
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename DATA>
static SIZE_T GetSizeByValue(DATA dataValue);
static SIZE_T GetSizeByType(BYTE byDataType);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Define neccessary classes for logging feature
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Data type name:	JSON_ENTRY
//  Description:	Store JSON key-value pair data
//  Derivered from: C++ basic struct
//
//////////////////////////////////////////////////////////////////////////

struct JSON_ENTRY
{
	// Member variables
	CString strKey;			// Key name
	CString strValue;		// Value (string)

	// Construction
	JSON_ENTRY() : strKey(STRING_EMPTY), strValue(STRING_EMPTY) {};
	JSON_ENTRY(const CString& key, const CString& value) : strKey(key), strValue(value) {};

	// Operators
	bool operator==(const JSON_ENTRY& other) const {
		return ((strKey == other.strKey) && (strValue == other.strValue));
	};
	bool operator!=(const JSON_ENTRY& other) const {
		return !(*this == other);
	};
	JSON_ENTRY& operator=(const JSON_ENTRY& other) {
		if (this != &other) {
			strKey = other.strKey;
			strValue = other.strValue;
		}
		return *this;
	};
};

// Define new typenames for JSON entry data
using JSON_ENTRY_DATA = typename std::vector<JSON_ENTRY>;


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 JSON
//  Description: Using for storing JSON object data
//
//////////////////////////////////////////////////////////////////////////

class JSON
{
private:
	// Attributes
	CString m_strObjectName;								// JSON object name

	// Properties
	JSON_ENTRY_DATA m_arrKeyValuePairs;						// Key-value pairs (array)

	// Children
	size_t m_nChildObjectCount;							// Number of child objects
	JSON** m_apChildObjectList;								// List of child objects

public:
	// Construction
	JSON();													// Default constructor
	JSON(const JSON&);										// Copy constructor
	~JSON();												// Destructor

	// Operator
	JSON& operator=(const JSON&);							// Copy assignment operator

protected:
	// Member functions
	void Copy(const JSON&);									// Copy object
	void CopyArrayData(const JSON&);						// Copy JSON object array data
	void CopyPtrData(const JSON&);							// Copy JSON object pointer data
	BOOL Compare(const JSON&) const;						// Compare objects
	BOOL IsEmpty(void) const;								// Check if object data is empty
	void RemoveProperty(size_t);							// Remove property by index
	void RemoveProperty(LPCTSTR);							// Remove property by key name
	void RemoveAll(void);									// Remove all object data

public:
	// Get/set functions
	void SetObjectName(LPCTSTR);							// Set object name
	void AddString(LPCTSTR, LPCTSTR);						// Add string value
	void AddInteger(LPCTSTR, INT);							// Add integer value
	void AddFloat(LPCTSTR, DOUBLE);							// Add float value
	void AddChildObject(JSON*);								// Add child object

	// Printing functions
	void Print(CString&, int, BOOL, BOOL = TRUE);			// Print data in JSON format (with indentation)
	void PrintYAML(CString&, int);							// Print data in YAML format
};

// Define new typenames for JSON class object
using JSONDATA = typename JSON;
using PJSONDATA = typename JSONDATA*;


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SLogging
//  Description: Using for saving application log data
//
//////////////////////////////////////////////////////////////////////////

class SLogging
{
private:
	// Log data array
	LOGDATA m_arrLogData;

	// Properties
	BYTE	 m_byLogType;					// Log type
	BYTE	 m_byWriteMode;					// Log write mode
	size_t	 m_nMaxSize;					// Log data max item count
	CString  m_strFilePath;					// Log output file path
	PLOGITEM m_pItemDefTemplate;			// Log default template

public:
	// Construction
	SLogging(BYTE byLogType);				// Constructor
	~SLogging(void);						// Destructor

public:
	// Initialization
	virtual void Init();
	virtual void DeleteAll();

	// Get/set data
	virtual BOOL IsEmpty(void) const;
	virtual size_t GetLogCount() const;
	virtual LOGITEM& GetLogItem(int nIndex);
	virtual const LOGITEM& GetLogItem(int nIndex) const;

	// Get/set properties function
	virtual size_t GetMaxSize(void) const;
	virtual BOOL SetMaxSize(size_t nMaxSize);
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


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _LOGGING_ENABLE_INLINES
	#include "Logging.inl"
	#ifdef _LOGGING_INL_INCLUDED
		#pragma message("-- Logging inline library included")
	#else
		#pragma error("-- Linking error in Logging.h: Unable to link to inline header!")
	#endif
	#undef _LOGGING_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in Logging.h: Inline is not enabled!")
#endif

#endif	// ifndef _LOGGING_H_INCLUDED
