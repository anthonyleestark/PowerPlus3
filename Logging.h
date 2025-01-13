
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
	BYTE	byCategory;										// Detail category
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
	USHORT			usCategory;								// Log category
	CString			strLogString;							// Log string
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
	void AddDetail(BYTE, UINT);								// Add detail (integer data only)
	void AddDetail(BYTE, LPCTSTR);							// Add detail (string data only)
	void AddDetail(BYTE, UINT, LPCTSTR);					// Add detail (both integer and string data)

	// Format data functions
	CString	FormatDateTime(void) const;						// Format date/time value
	CString	FormatLogString(void) const;					// Format log string
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

static SIZE_T GetSizeByType(BYTE byDataType);
template <typename DATA>
static SIZE_T GetSizeByValue(DATA dataValue);

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