
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.cpp
//		Description:	Implement necessary methods to read/write app logs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logging.h"

///////////////////////////////////////////////////////

//// Implementations


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSizeByType
//	Description:	Get size of data by data type
//  Arguments:		byDataType - Data type
//  Return value:	SIZE_T
//
//////////////////////////////////////////////////////////////////////////

SIZE_T GetSizeByType(BYTE byDataType)
{
	SIZE_T retSize = 0;
	switch (byDataType)
	{
	case DATA_TYPE_VOID:				// No type (unusable)
		retSize = DEF_INTEGER_NULL;
		break;
	case DATA_TYPE_NUM_U1:				// Unsigned integer (1-byte)
	case DATA_TYPE_NUM_I1:				// Signed integer (1-byte)
	case DATA_TYPE_NUM_F1:				// Float number (1-byte)
		retSize = sizeof(UCHAR);
		break;
	case DATA_TYPE_NUM_U2:				// Unsigned integer (2-byte)
	case DATA_TYPE_NUM_I2:				// Signed integer (2-byte)
	case DATA_TYPE_NUM_F2:				// Float number (2-byte)
		retSize = sizeof(USHORT);
		break;
	case DATA_TYPE_NUM_U4:				// Unsigned integer (4-byte)
	case DATA_TYPE_NUM_I4:				// Signed integer (4-byte)
	case DATA_TYPE_NUM_F4:				// Float number (4-byte)
		retSize = sizeof(ULONG);
		break;
	case DATA_TYPE_NUM_U8:				// Unsigned integer (8-byte)
	case DATA_TYPE_NUM_I8:				// Signed integer (8-byte)
	case DATA_TYPE_NUM_F8:				// Float number (8-byte)
		retSize = sizeof(ULONGLONG);
		break;
	case DATA_TYPE_NUM_BOOLEAN:			// Boolean number (TRUE/FALSE)
		retSize = sizeof(BOOL);
		break;
	case DATA_TYPE_SYSTEMTIME:			// SYSTEMTIME struct
		retSize = sizeof(SYSTEMTIME);
		break;
	default:
		retSize = DEF_INTEGER_NULL;
		break;
	}

	return retSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetSizeByValue
//	Description:	Get size of data by data value
//  Arguments:		dataValue - Data value
//  Return value:	SIZE_T
//
//////////////////////////////////////////////////////////////////////////

template <typename DATA>
SIZE_T GetSizeByValue(DATA dataValue)
{
	return sizeof(dataValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagLOGDETAIL
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagLOGDETAIL::tagLOGDETAIL()
{
	// Initialization
	this->byCategory = 0;										// Detail category
	this->uiDetailInfo = 0;										// Detail info (integer)
	this->strDetailInfo.Empty();								// Detail info (string)
	this->ptrDetailInfo = NULL;									// Detail info (pointer)
	this->byPointerType = DATA_TYPE_VOID;						// Detail info pointer data type
	this->szPointerSize = DEF_INTEGER_NULL;						// Detail info pointer data size
}

tagLOGDETAIL::tagLOGDETAIL(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->byCategory = pItem.byCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	tagLOGDETAIL&
//
//////////////////////////////////////////////////////////////////////////

tagLOGDETAIL& tagLOGDETAIL::operator=(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->byCategory = pItem.byCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize/reset item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::Init()
{
	// Initialization
	this->byCategory = 0;										// Detail category
	this->uiDetailInfo = 0;										// Detail info (integer)
	this->strDetailInfo.Empty();								// Detail info (string)
	this->ptrDetailInfo = NULL;									// Detail info (pointer)
	this->byPointerType = DATA_TYPE_VOID;						// Detail info pointer data type
	this->szPointerSize = DEF_INTEGER_NULL;						// Detail info pointer data size
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::Copy(const tagLOGDETAIL& pItem)
{
	// Copy data
	this->byCategory = pItem.byCategory;						// Detail category
	this->uiDetailInfo = pItem.uiDetailInfo;					// Detail info (integer)
	this->strDetailInfo = pItem.strDetailInfo;					// Detail info (string)
	this->PointerCopy(pItem);									// Detail info (pointer)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PointerCopy
//	Description:	Copy detail info pointer
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGDETAIL::PointerCopy(const tagLOGDETAIL& pItem)
{
	// Copy pointer properties
	this->byPointerType = pItem.byPointerType;					// Detail info pointer data type
	this->szPointerSize = pItem.szPointerSize;					// Detail info pointer data size

	// Copy pointer data
	memcpy(this->ptrDetailInfo, pItem.ptrDetailInfo, pItem.szPointerSize);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::Compare(const tagLOGDETAIL& pItem) const
{
	BOOL bRet = FALSE;

	// Compare items
	bRet &= (this->byCategory == pItem.byCategory);				// Detail category
	bRet &= (this->uiDetailInfo == pItem.uiDetailInfo);			// Detail info (integer)
	bRet &= (this->strDetailInfo == pItem.strDetailInfo);		// Detail info (string)
	bRet &= this->PointerCompare(pItem);						// Detail info (pointer)

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PointerCompare
//	Description:	Compare detail info pointers
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::PointerCompare(const tagLOGDETAIL& pItem) const
{
	BOOL bRet = FALSE;

	// Compare properties
	bRet &= (this->byPointerType == pItem.byPointerType);		// Detail info pointer data type
	bRet &= (this->szPointerSize == pItem.szPointerSize);		// Detail info pointer data size

	// Only compare pointer values if properties are matching
	if (bRet != FALSE) {
		bRet &= memcmp(this->ptrDetailInfo, pItem.ptrDetailInfo, this->szPointerSize);
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current detail info data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::IsEmpty(void) const
{
	// Initialize empty detail info
	static const LOGDETAIL logDummyDetail;

	// Compare with that data and return result
	return this->Compare(logDummyDetail);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetPointerData
//	Description:	Set detail info pointer data
//  Arguments:		pDataBuff	- Data buffer (pointer)
//					byDataType	- Data type
//					szDataSize	- Data size
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGDETAIL::SetPointerData(PVOID pDataBuff, BYTE byDataType /* = DATA_TYPE_UNSPECIFIED */, SIZE_T szDataSize /* = 0 */)
{
	// If data type is void (unusable), do nothing
	if (byDataType == DATA_TYPE_VOID)
		return FALSE;

	// If both data type and size are not specified, do nothing
	if ((byDataType == DATA_TYPE_UNSPECIFIED) && (szDataSize == 0))
		return FALSE;

	// If size is not specified,
	if (szDataSize == 0) {
		// Get size by data type
		szDataSize = GetSizeByType(byDataType);

		// Get size failed, do nothing
		if (szDataSize == 0)
			return FALSE;
	}

	// Otherwise, set normally
	this->byPointerType = byDataType;
	this->szPointerSize = szDataSize;
	memcpy(this->ptrDetailInfo, pDataBuff, szDataSize);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	tagLOGITEM
//	Description:	Constructor
//  Arguments:		Default
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

tagLOGITEM::tagLOGITEM()
{
	// Initialization
	this->stTime = {0};											// Log time
	this->usCategory = LOG_MACRO_NONE;							// Log category
	this->strLogString = DEF_STRING_EMPTY;						// Log string
	this->arrDetailInfo.RemoveAll();							// Log detail info
}

tagLOGITEM::tagLOGITEM(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	tagLOGITEM&
//
//////////////////////////////////////////////////////////////////////////

tagLOGITEM& tagLOGITEM::operator=(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another log item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::Copy(const tagLOGITEM& pItem)
{
	// Copy data
	this->stTime = pItem.stTime;								// Log time
	this->usCategory = pItem.usCategory;						// Log category
	this->strLogString = pItem.strLogString;					// Log string
	this->arrDetailInfo.Copy(pItem.arrDetailInfo);				// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGITEM::Compare(const tagLOGITEM& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item
	bRet &= (this->stTime.wHour == pItem.stTime.wHour);
	bRet &= (this->stTime.wMinute == pItem.stTime.wMinute);
	bRet &= (this->usCategory == pItem.usCategory);
	bRet &= (this->strLogString == pItem.strLogString);

	// Compare log detail info
	BOOL bDetailInfoCompare = TRUE;
	if (this->arrDetailInfo.GetSize() != pItem.arrDetailInfo.GetSize()) {
		bDetailInfoCompare = FALSE;
	}
	else {
		for (int nIndex = 0; nIndex < this->arrDetailInfo.GetSize(); nIndex++) {
			if (this->arrDetailInfo.GetAt(nIndex).Compare(pItem.arrDetailInfo.GetAt(nIndex)) != TRUE) {
				bDetailInfoCompare = FALSE;
				break;
			}
		}
	}
	bRet &= bDetailInfoCompare;

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current log item is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL tagLOGITEM::IsEmpty(void) const
{
	// Initialize an empty item
	static const LOGITEM logDummyItem;

	// Compare with this item and return result
	return this->Compare(logDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveDetailInfo
//	Description:	Remove all log detail info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::RemoveDetailInfo(void)
{
	// Clean up log detail info data
	this->arrDetailInfo.RemoveAll();
	this->arrDetailInfo.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all log item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::RemoveAll(void)
{
	// Reset data
	this->stTime = {0};											// Log time
	this->usCategory = LOG_MACRO_NONE;							// Log category
	this->strLogString = DEF_STRING_EMPTY;						// Log string

	// Clean up log detail info data
	this->RemoveDetailInfo();									// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDetail
//	Description:	Add log detail info data
//  Arguments:		logDetailInfo	- Log detail info data
//					byCategory		- Detail category
//					nDetailInfo		- Detail info (integer)
//					strDetailInfo	- Detail info (string)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void tagLOGITEM::AddDetail(const LOGDETAIL& logDetailInfo)
{
	// Add detail info data
	this->arrDetailInfo.Add(logDetailInfo);
}

void tagLOGITEM::AddDetail(BYTE byCategory, UINT nDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.byCategory = byCategory;
	logDetailInfo.uiDetailInfo = nDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

void tagLOGITEM::AddDetail(BYTE byCategory, LPCTSTR lpszDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.byCategory = byCategory;
	logDetailInfo.strDetailInfo = lpszDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

void tagLOGITEM::AddDetail(BYTE byCategory, UINT nDetailInfo, LPCTSTR lpszDetailInfo)
{
	// Prepare detail info data
	LOGDETAIL logDetailInfo;
	logDetailInfo.byCategory = byCategory;
	logDetailInfo.uiDetailInfo = nDetailInfo;
	logDetailInfo.strDetailInfo = lpszDetailInfo;

	// Add detail info data
	this->AddDetail(logDetailInfo);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDateTime
//	Description:	Return a formated logitem date/time string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatDateTime(void) const
{
	CString strTimeFormat;
	CString strMiddayFlag = (stTime.wHour >= 12) ? DEF_SYMBOL_POSTMERIDIEM : DEF_SYMBOL_ANTEMERIDIEM;
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, stTime.wYear, stTime.wMonth, stTime.wDay,
						stTime.wHour, stTime.wMinute, stTime.wSecond, stTime.wMilliseconds, strMiddayFlag);

	return strTimeFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatLogString
//	Description:	Return a formated log string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString tagLOGITEM::FormatLogString(void) const
{
	CString strLogFormat;
	strLogFormat.Format(IDS_FORMAT_LOGSTRING, FormatDateTime(), strLogString, DEF_STRING_EMPTY);
	return strLogFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SLogging/~SLogging
//	Description:	Constructor and destructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::SLogging(BYTE byLogType)
{
	// Log data array
	m_arrLogData.RemoveAll();

	// Properties
	m_byLogType = byLogType;
	m_byWriteMode = LOG_WRITEMODE_NONE;
	m_nMaxSize = DEF_INTEGER_INFINITE;
	m_strFilePath = DEF_STRING_EMPTY;
	m_pItemDefTemplate = NULL;
}

SLogging::~SLogging()
{
	// Clean up log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();

	// Clean up default item template
	if (m_pItemDefTemplate != NULL) {
		delete m_pItemDefTemplate;
		m_pItemDefTemplate = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::Init()
{
	// Initialize empty log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DeleteAll
//	Description:	Delete all log data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::DeleteAll()
{
	// Clean up log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current log data is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL SLogging::IsEmpty(void) const
{
	return m_arrLogData.IsEmpty();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogCount
//	Description:	Return the number of items of log data
//  Arguments:		None
//  Return value:	INT_PTR - Number of log items
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE INT_PTR SLogging::GetLogCount() const
{
	return m_arrLogData.GetSize();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetLogItem
//	Description:	Return a specific log item of log list
//  Arguments:		nIndex - Item index
//  Return value:	LOGITEM - Return log item
//
//////////////////////////////////////////////////////////////////////////

LOGITEM& SLogging::GetLogItem(int nIndex)
{
	// If current log data is empty
	if (this->IsEmpty()) {
		// Return an empty dummy item
		static LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (nIndex < 0) {
		// Return 1st item
		nIndex = 0;
	}
	else if (nIndex >= GetLogCount()) {
		// Return last item
		nIndex = (GetLogCount() - 1);
	}

	return m_arrLogData.GetAt(nIndex);
}

const LOGITEM& SLogging::GetLogItem(int nIndex) const
{
	// If current log data is empty
	if (this->IsEmpty()) {
		// Return an empty dummy item
		static const LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (nIndex < 0) {
		// Return 1st item
		nIndex = 0;
	}
	else if (nIndex >= GetLogCount()) {
		// Return last item
		nIndex = (GetLogCount() - 1);
	}

	return m_arrLogData.GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetMaxSize
//	Description:	Get/set maximum log data size
//  Arguments:		nMaxSize - Max log data size
//  Return value:	INT_PTR
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE INT_PTR SLogging::GetMaxSize(void) const
{
	return m_nMaxSize;
}

void SLogging::SetMaxSize(INT_PTR nMaxSize)
{
	m_nMaxSize = nMaxSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetWriteMode
//	Description:	Get/set log write mode
//  Arguments:		byWriteMode - Log write mode
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

AFX_INLINE BYTE SLogging::GetWriteMode(void) const
{
	return m_byWriteMode;
}

void SLogging::SetWriteMode(BYTE byWriteMode)
{
	m_byWriteMode = byWriteMode;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetFilePath
//	Description:	Get/set log output file path
//  Arguments:		strFilePath/lpszFilePath - Log file path
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::GetFilePath(CString& strFilePath)
{
	strFilePath = m_strFilePath;
}

void SLogging::SetFilePath(LPCTSTR lpszFilePath)
{
	m_strFilePath = lpszFilePath;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDefaultTemplate
//	Description:	Get/set log default template
//  Arguments:		logItemTemplate - Log item template
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

PLOGITEM SLogging::GetDefaultTemplate(void)
{
	return m_pItemDefTemplate;
}

void SLogging::SetDefaultTemplate(const LOGITEM& logItemTemplate)
{
	// Initialize default template
	if (m_pItemDefTemplate == NULL) {
		m_pItemDefTemplate = new LOGITEM;
		if (m_pItemDefTemplate == NULL) {
			TRCLOG("Default item initialization failed!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update template
	if (m_pItemDefTemplate != NULL) {
		if (logItemTemplate.IsEmpty()) return;
		m_pItemDefTemplate->Copy(logItemTemplate);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputItem
//	Description:	Add a log item into log data
//  Arguments:		logItem	- Log item to write
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputItem(const LOGITEM& logItem)
{
	if (GetWriteMode() == LOG_WRITEMODE_INSTANT) {
		// Write instantly
		Write(logItem);
	}
	else {
		// If already reached max data size
		INT_PTR nMaxSize = GetMaxSize();
		if ((nMaxSize != DEF_INTEGER_INFINITE) && (GetLogCount() >= nMaxSize)) {
			// Can not output log item --> Trace info
			TRCLOG("Output log item failed: Log data exceeded max size!!!");
			return;
		}

		// Store log data
		m_arrLogData.Add(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OutputLogString
//	Description:	Add a log string into log list data
//  Arguments:		lpszLogString - Log string
//					lpszLogDetail - Log detail string
//					byType		  - Log type
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SLogging::OutputString(LPCTSTR lpszLogString, BOOL bUseLastTemplate /* = TRUE */)
{
	if (GetWriteMode() == LOG_WRITEMODE_INSTANT) {
		// Write instantly
		Write(lpszLogString);
	}
	else {
		// Get log time
		SYSTEMTIME stLogTime;
		stLogTime = GetCurSysTime();

		// Prepare log item
		LOGITEM logItem;
		if (bUseLastTemplate == TRUE) {
			// Use last log item as template
			if (this->IsEmpty()) {
				// Can not output log string --> Trace info
				TRCLOG("Output log string failed: No item to use as template!!!");
				return;
			}
			else {
				// Copy template
				logItem.Copy(this->GetLogItem(GetLogCount() - 1));
			}
		}
		else {
			// Use default template
			if (this->GetDefaultTemplate() == NULL) {
				// Can not output log string --> Trace info
				TRCLOG("Output log string failed: Default template not set!!!");
				return;
			}
			else {
				// Copy template
				logItem.Copy(*(this->GetDefaultTemplate()));
			}
		}

		// Update log item data
		logItem.stTime = stLogTime;
		logItem.strLogString = lpszLogString;
		OutputItem(logItem);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Write
//	Description:	Write log data into the corresponding logfile
//  Arguments:		None
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write()
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log is set as Read-only
	// or current log mode is write instantly mode
	if ((this->GetWriteMode() == LOG_WRITEMODE_NONE) ||
		(this->GetWriteMode() == LOG_WRITEMODE_INSTANT))
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CString strCurFileName;
	CFile fLogFile;

	LOGITEM logItem;
	SYSTEMTIME stTemp;
	CString strLogFormat;

	for (int nIndex = 0; nIndex < GetLogCount(); nIndex++)
	{
		// Get log item
		logItem = GetLogItem(nIndex);
		stTemp = logItem.stTime;

		// Get filename according to type of logs
		switch (m_byLogType)
		{
		case LOGTYPE_APP_EVENT:
			// Format app event log filename
			strFileName.Format(FILENAME_APPEVENT_LOG, stTemp.wYear, stTemp.wMonth, stTemp.wDay);
			MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);
			if (strCurFileName.IsEmpty())
				strCurFileName = strFileName;

			// If a file with another name (previous day's log file) is opening,
			// write down all current log strings and close the file
			if ((fLogFile.m_hFile != CFile::hFileNull) && (strFileName != strCurFileName))
			{
				if (!strLogFormat.IsEmpty()) {
					// Write log strings to file
					fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
					fLogFile.Flush();

					strLogFormat.Empty();
				}

				fLogFile.Close();
				strCurFileName = strFileName;
			}
			break;
		case LOGTYPE_HISTORY_LOG:
			// App history log
			strFileName = FILENAME_HISTORY_LOG;
			MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);
			break;
		default:
			// Wrong argument
			TRCLOG("Write log failed: Invalid log type!!!");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			// Show error message
			dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			break;
		}

		// Check if file is opening, if not, open it
		if (fLogFile.m_hFile == CFile::hFileNull)
		{
			bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {
				// Open file failed
				TRCLOG("Write log failed: Can not open/create log file!!!");
				TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				dwErrCode = GetLastError();
				PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
				return bResult;
			}

			// Go to end of file
			fLogFile.SeekToEnd();
		}

		// Format output log strings
		strLogFormat += logItem.FormatLogString();
	}

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Write
//	Description:	Write log item instantly into logfile
//  Arguments:		logItem		 - Log item to write
//					lpszFilePath - Output log file path
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write(const LOGITEM& logItem, LPCTSTR lpszFilePath /* = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LOG_WRITEMODE_INSTANT)
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stTimeTemp;
	stTimeTemp = logItem.stTime;

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		strFileName.Format(FILENAME_APPEVENT_LOG, stTimeTemp.wYear, stTimeTemp.wMonth, stTimeTemp.wDay);
		break;
	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;
	default:
		// Wrong argument
		TRCLOG("Error: Invalid log type");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
		break;
	}

	// Get file path
	MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			// Open file failed
			TRCLOG("Error: Can not open/create log file");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	strLogFormat = logItem.FormatLogString();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteInstantLog
//	Description:	Write log string instantly into logfile
//  Arguments:		lpszLogString - Log string
//  Return value:	BOOL - Result of log writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL SLogging::Write(LPCTSTR lpszLogString, LPCTSTR lpszFilePath /* = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LOG_WRITEMODE_INSTANT)
		return FALSE;

	CString strFileName;
	CString strFilePath;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stCurTime;
	GetLocalTime(&stCurTime);

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		strFileName.Format(FILENAME_APPEVENT_LOG, stCurTime.wYear, stCurTime.wMonth, stCurTime.wDay);
		break;
	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;
	default:
		// Wrong argument
		TRCLOG("Error: Invalid log type");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		// Show error message
		dwErrCode = DEF_APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
		break;
	}

	// Get file path
	MakeFilePath(strFilePath, SUBFOLDER_LOG, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			// Open file failed
			TRCLOG("Error: Can not open/create log file");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	LOGITEM logItem;
	logItem.stTime = stCurTime;
	logItem.strLogString = lpszLogString;
	strLogFormat = logItem.FormatLogString();

	if (!strLogFormat.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(strLogFormat, strLogFormat.GetLength() * sizeof(TCHAR));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

