
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.cpp
//		Description:	Implement necessary methods to read/write app logs
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.22:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "AppCore/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;


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
	case LogDataType::Void:				// No type (unusable)
		retSize = INT_NULL;
		break;

	case LogDataType::Num_U1:			// Unsigned integer (1-byte)
	case LogDataType::Num_I1:			// Signed integer (1-byte)
	case LogDataType::Num_F1:			// Float number (1-byte)
		retSize = sizeof(UCHAR);
		break;

	case LogDataType::Num_U2:			// Unsigned integer (2-byte)
	case LogDataType::Num_I2:			// Signed integer (2-byte)
	case LogDataType::Num_F2:			// Float number (2-byte)
		retSize = sizeof(USHORT);
		break;

	case LogDataType::Num_U4:			// Unsigned integer (4-byte)
	case LogDataType::Num_I4:			// Signed integer (4-byte)
	case LogDataType::Num_F4:			// Float number (4-byte)
		retSize = sizeof(ULONG);
		break;

	case LogDataType::Num_U8:			// Unsigned integer (8-byte)
	case LogDataType::Num_I8:			// Signed integer (8-byte)
	case LogDataType::Num_F8:			// Float number (8-byte)
		retSize = sizeof(ULONGLONG);
		break;

	case LogDataType::Boolean:			// Boolean number (TRUE/FALSE)
		retSize = sizeof(BOOL);
		break;

	case LogDataType::SystemTime:		// SYSTEMTIME struct
		retSize = sizeof(SYSTEMTIME);
		break;

	default:
		retSize = INT_NULL;
		break;
	}

	return retSize;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LogDetail
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

LogDetail::LogDetail()
{
	// Initialization
	m_usCategory = INT_NULL;								// Detail category
	m_nFlag = LogDetailFlag::Flag_Null;						// Detail info flag
	m_nDetailValue = INT_NULL;								// Detail value (integer)
	m_strDetailInfo.Empty();								// Detail info (string)
	m_ptrDetailData = NULL;									// Detail data (pointer)
	m_byPointerType = LogDataType::Void;					// Detail info pointer data type
	m_szPointerSize = INT_NULL;								// Detail info pointer data size
}

LogDetail::LogDetail(const LogDetail& pItem)
{
	// Copy data
	this->Copy(pItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	LogDetail&
//
//////////////////////////////////////////////////////////////////////////

LogDetail& LogDetail::operator=(const LogDetail& pItem)
{
	// Copy data
	this->Copy(pItem);
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

void LogDetail::Init()
{
	// Initialization
	m_usCategory = INT_NULL;								// Detail category
	m_nFlag = LogDetailFlag::Flag_Null;						// Detail info flag
	m_nDetailValue = INT_NULL;								// Detail value (integer)
	m_strDetailInfo.Empty();								// Detail info (string)
	m_ptrDetailData = NULL;									// Detail data (pointer)
	m_byPointerType = LogDataType::Void;					// Detail info pointer data type
	m_szPointerSize = INT_NULL;								// Detail info pointer data sizesize
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another item
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogDetail::Copy(const LogDetail& pItem)
{
	// Do not copy itself
	if (this == &pItem) return;

	// Copy data
	m_usCategory = pItem.m_usCategory;						// Detail category
	m_nFlag = pItem.m_nFlag;								// Detail flag
	m_nDetailValue = pItem.m_nDetailValue;					// Detail value (integer)
	m_strDetailInfo = pItem.m_strDetailInfo;				// Detail info (string)
	PointerCopy(pItem);										// Detail data (pointer)
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PointerCopy
//	Description:	Copy detail info pointer
//  Arguments:		pItem - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogDetail::PointerCopy(const LogDetail& pItem)
{
	// Copy pointer properties
	m_byPointerType = pItem.m_byPointerType;					// Detail info pointer data type
	m_szPointerSize = pItem.m_szPointerSize;					// Detail info pointer data size

	// Copy pointer data
	memcpy(m_ptrDetailData, pItem.m_ptrDetailData, pItem.m_szPointerSize);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL LogDetail::Compare(const LogDetail& pItem) const
{
	BOOL bRet = FALSE;

	// Compare items
	bRet &= (m_usCategory == pItem.m_usCategory);				// Detail category
	bRet &= (m_nFlag == pItem.m_nFlag);							// Detail flag
	bRet &= (m_nDetailValue == pItem.m_nDetailValue);			// Detail value (integer)
	bRet &= (m_strDetailInfo == pItem.m_strDetailInfo);			// Detail info (string)
	bRet &= PointerCompare(pItem);								// Detail data (pointer)

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

BOOL LogDetail::PointerCompare(const LogDetail& pItem) const
{
	BOOL bRet = FALSE;

	// Compare properties
	bRet &= (m_byPointerType == pItem.m_byPointerType);			// Detail info pointer data type
	bRet &= (m_szPointerSize == pItem.m_szPointerSize);			// Detail info pointer data size

	// Only compare pointer values if properties are matching
	if (bRet != FALSE) {
		bRet &= memcmp(m_ptrDetailData, pItem.m_ptrDetailData, m_szPointerSize);
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

BOOL LogDetail::IsEmpty(void) const
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

BOOL LogDetail::SetPointerData(PVOID pDataBuff, BYTE byDataType /* = DATA_TYPE_UNSPECIFIED */, SIZE_T szDataSize /* = 0 */)
{
	// If data type is void (unusable), do nothing
	if (byDataType == LogDataType::Void)
		return FALSE;

	// If both data type and size are not specified, do nothing
	if ((byDataType == LogDataType::Unspecified) && (szDataSize == 0))
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
	m_byPointerType = byDataType;
	m_szPointerSize = szDataSize;
	memcpy(m_ptrDetailData, pDataBuff, szDataSize);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LogDetailInfo
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

LogDetailInfo::LogDetailInfo() : LOGDETAILARRAY()
{
	// Initialization
	this->clear();
}

LogDetailInfo::LogDetailInfo(const LogDetailInfo& pData)
{
	// Copy data
	this->CopyData(pData);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	LogDetailInfo&
//
//////////////////////////////////////////////////////////////////////////

LogDetailInfo& LogDetailInfo::operator=(const LogDetailInfo& pData)
{
	// Copy data
	this->CopyData(pData);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Init
//	Description:	Initialize/reset data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogDetailInfo::Init(void)
{
	// Reset data
	this->clear();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyData
//	Description:	Copy data from another log detail info data
//  Arguments:		pData - Pointer of input item
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogDetailInfo::CopyData(const LogDetailInfo& pData)
{
	// Remove all data first
	this->Init();

	// Copy data
	this->assign(pData.begin(), pData.end());
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddDetail
//	Description:	Add log detail info item
//  Arguments:		logDetail	  - Log detail item
//					usCategory	  - Detail category
//					nDetailInfo	  - Detail info (integer)
//					strDetailInfo - Detail info (string)
//					nFlag		  - Detail flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogDetailInfo::AddDetail(USHORT usCategory, INT nDetailInfo, INT nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailValue(nDetailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

void LogDetailInfo::AddDetail(USHORT usCategory, LPCTSTR lpszDetailInfo, INT nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailString(lpszDetailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

void LogDetailInfo::AddDetail(USHORT usCategory, INT nDetailInfo, LPCTSTR lpszDetailInfo, INT nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailValue(nDetailInfo);
	logDetail.SetDetailString(lpszDetailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LogItem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

LogItem::LogItem()
{
	// Initialization
	m_stTime = SYSTEMTIME_ZERO;								// Log time
	m_dwProcessID = INT_NULL;								// Process ID
	m_usCategory = LOG_MACRO_NONE;							// Log category
	m_strLogString = Constant::String::Empty;							// Log string
	m_arrDetailInfo.clear();								// Log detail info
}

LogItem::LogItem(const LogItem& pItem)
{
	// Copy data
	this->Copy(pItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	LogItem&
//
//////////////////////////////////////////////////////////////////////////

LogItem& LogItem::operator=(const LogItem& pItem)
{
	// Copy data
	this->Copy(pItem);
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

void LogItem::Copy(const LogItem& pItem)
{
	// Copy data
	m_stTime = pItem.m_stTime;								// Log time
	m_dwProcessID = pItem.m_dwProcessID;					// Process ID
	m_usCategory = pItem.m_usCategory;						// Log category
	m_strLogString = pItem.m_strLogString;					// Log string
	m_arrDetailInfo = pItem.m_arrDetailInfo;				// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given item
//  Arguments:		pItem - Pointer of given item
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL LogItem::Compare(const LogItem& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item
	bRet &= (m_stTime.wHour == pItem.m_stTime.wHour);
	bRet &= (m_stTime.wMinute == pItem.m_stTime.wMinute);
	bRet &= (m_dwProcessID == pItem.m_dwProcessID);
	bRet &= (m_usCategory == pItem.m_usCategory);
	bRet &= (m_strLogString == pItem.m_strLogString);

	// Compare log detail info
	BOOL bDetailInfoCompare = TRUE;
	if (this->m_arrDetailInfo.size() != pItem.m_arrDetailInfo.size()) {
		bDetailInfoCompare = FALSE;
	}
	else {
		for (int nIndex = 0; nIndex < this->m_arrDetailInfo.size(); nIndex++) {
			if (this->m_arrDetailInfo.at(nIndex).Compare(pItem.m_arrDetailInfo.at(nIndex)) != TRUE) {
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

BOOL LogItem::IsEmpty(void) const
{
	// Initialize an empty item
	static const LOGITEM logDummyItem;

	// Compare with this item and return result
	return this->Compare(logDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all log item data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void LogItem::RemoveAll(void)
{
	// Reset data
	m_stTime = SYSTEMTIME_ZERO;									// Log time
	m_dwProcessID = INT_NULL;									// Process ID
	m_usCategory = LOG_MACRO_NONE;								// Log category
	m_strLogString = Constant::String::Empty;								// Log string

	// Clean up log detail info data
	this->RemoveDetailInfo();									// Log detail info
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatDateTime
//	Description:	Return a formatted logitem date/time string
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString LogItem::FormatDateTime(void) const
{
	CString strTimeFormat;
	const wchar_t* middayFlag = (m_stTime.wHour >= 12) ? Constant::Symbol::PostMeridiem : Constant::Symbol::AnteMeridiem;
	strTimeFormat.Format(IDS_FORMAT_FULLDATETIME, m_stTime.wYear, m_stTime.wMonth, m_stTime.wDay,
		m_stTime.wHour, m_stTime.wMinute, m_stTime.wSecond, m_stTime.wMilliseconds, middayFlag);

	return strTimeFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	FormatOutput
//	Description:	Return formatted output log string for file writing
//  Arguments:		None
//  Return value:	CString - Formatted result
//
//////////////////////////////////////////////////////////////////////////

CString LogItem::FormatOutput(void) const
{
	// Create JSON data object
	JSONDATA jsonData;

	CString strLogKey;
	CString strLogValue;

	// Load default language table package
	LANGTABLE_PTR pDefLang = LoadLanguageTable(NULL);

	/*********************************************************************/
	/*																	 */
	/*			  Convert log item base info into JSON data				 */
	/*																	 */
	/*********************************************************************/

	// Log time
	strLogKey = GetString(StringTable::LogKey, BaseLog::Time);
	jsonData.AddString(strLogKey, FormatDateTime());

	// Process ID
	strLogKey = GetString(StringTable::LogKey, BaseLog::PID);
	jsonData.AddInteger(strLogKey, m_dwProcessID);

	// Log category
	strLogKey = GetString(StringTable::LogKey, BaseLog::LogCategory);
	strLogValue = GetLanguageString(pDefLang, m_usCategory);
	jsonData.AddString(strLogKey, strLogValue);

	// Log description string
	strLogKey = GetString(StringTable::LogKey, BaseLog::Description);
	jsonData.AddString(strLogKey, m_strLogString);

	/*********************************************************************/
	/*																	 */
	/*		  Convert log item detail info data into JSON data			 */
	/*																	 */
	/*********************************************************************/

	// Log detail info
	if (!m_arrDetailInfo.empty()) {

		// Create JSON detail data object
		JSONDATA jsonDetailData;

		CString strDetailKey;
		CString strDetailValue;

		// Set object name: Details
		jsonDetailData.SetObjectName(GetString(StringTable::LogKey, BaseLog::Details));

		// Convert data
		for (int nIndex = 0; nIndex < (m_arrDetailInfo.size()); nIndex++) {

			// Get detail info item
			LOGDETAIL logDetail = m_arrDetailInfo.at(nIndex);

			// Skip if detail item is read-only
			int nDetailFlag = logDetail.GetFlag();
			if (nDetailFlag & LogDetailFlag::ReadOnly_Data)
				continue;

			// NULL flag --> apply default flags
			if (nDetailFlag == LogDetailFlag::Flag_Null) {
				nDetailFlag = LogDetailFlag::Write_Int;
			}

			// Detail info category
			strDetailKey = GetString(StringTable::LogKey, logDetail.GetCategory());

			// Detail info value
			int nDetailValue = logDetail.GetDetailValue();
			if (nDetailFlag & LogDetailFlag::Write_Int) {
				jsonDetailData.AddInteger(strDetailKey, logDetail.GetDetailValue());
			}
			else if (nDetailFlag & LogDetailFlag::LookUp_Dict) {
				strDetailValue = GetString(StringTable::LogValue, nDetailValue);
				jsonDetailData.AddString(strDetailKey, strDetailValue);
			}
			else if (nDetailFlag & LogDetailFlag::Write_String) {
				jsonDetailData.AddString(strDetailKey, logDetail.GetDetailString());
			}
			else if (nDetailFlag & (LogDetailFlag::Write_Int & LogDetailFlag::Write_String)) {
				JSONDATA jsonSubDetail;
				strDetailKey = GetString(StringTable::LogKey, BaseLog::DetailNumeric);
				jsonSubDetail.AddInteger(strDetailKey, logDetail.GetDetailValue());
				strDetailKey = GetString(StringTable::LogKey, BaseLog::DetailString);
				jsonSubDetail.AddString(strDetailKey, logDetail.GetDetailString());
				jsonDetailData.AddChildObject(&jsonSubDetail);
			}
		}

		// Output detail JSON data
		jsonData.AddChildObject(&jsonDetailData);
	}

	/*********************************************************************/
	/*																	 */
	/*				  Output JSON data under YAML format				 */
	/*																	 */
	/*********************************************************************/

	CString strLogFormat;
	jsonData.PrintYAML(strLogFormat, 0);
	strLogFormat.Append(Constant::String::NewLine);

	return strLogFormat;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	JSON
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

JSON::JSON()
{
	// Initialization
	this->m_strObjectName = Constant::String::Empty;			// JSON object name
	this->m_arrKeyValuePairs.clear();				// Key-value pairs
	this->m_nChildObjectCount = 0;					// Number of child objects
	this->m_apChildObjectList = NULL;				// List of child objects
}

JSON::JSON(const JSON& pObj)
{
	// Copy data
	this->Copy(pObj);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~JSON
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

JSON::~JSON()
{
	// Remove all data and clean-up
	this->RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	operator=
//	Description:	Copy assignment operator
//  Arguments:		Default
//  Return value:	JSON&
//
//////////////////////////////////////////////////////////////////////////

JSON& JSON::operator=(const JSON& pObj)
{
	// Copy data
	this->Copy(pObj);
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Copy
//	Description:	Copy data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::Copy(const JSON& pObj)
{
	// Do not copy itself
	if (this == &pObj) return;

	// Copy data
	this->m_strObjectName = pObj.m_strObjectName;	// JSON object name
	this->CopyArrayData(pObj);						// Property (array) data
	this->CopyPtrData(pObj);						// Child object (pointer) data
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyArrayData
//	Description:	Copy object array data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::CopyArrayData(const JSON& pObj)
{
	// Remove all existing array data
	this->m_arrKeyValuePairs.clear();

	// Set destination array data size
	this->m_arrKeyValuePairs.reserve(pObj.m_arrKeyValuePairs.size());

	// Copy list of key-value pairs
	for (int nIndex = 0; nIndex < pObj.m_arrKeyValuePairs.size(); nIndex++) {
		this->m_arrKeyValuePairs.push_back(pObj.m_arrKeyValuePairs.at(nIndex));
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CopyPtrData
//	Description:	Copy object pointer data from another JSON object
//  Arguments:		pObj - Pointer of input object
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::CopyPtrData(const JSON& pObj)
{
	// Number of child objects
	this->m_nChildObjectCount = pObj.m_nChildObjectCount;

	// List of child objects (pointer copy)
	if ((pObj.m_nChildObjectCount > 0) && (pObj.m_apChildObjectList != NULL)) {

		// Allocation and initialization
		this->m_apChildObjectList = new PJSONDATA[this->m_nChildObjectCount];
		if (this->m_apChildObjectList == NULL) {
			TRACE_ERROR("Error: Destination JSON child object array allocation failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}

		// Copy data
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {

			// Allocate memory
			this->m_apChildObjectList[nCount] = new JSONDATA;
			if (this->m_apChildObjectList[nCount] == NULL) {
				TRACE_FORMAT("Error: Destination JSON child object allocation failed!!! (Index=%d)", nCount);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Get source data
			PJSONDATA pSrcData = pObj.m_apChildObjectList[nCount];
			if (pSrcData == NULL) {
				TRACE_FORMAT("Error: Invalid JSON child object is skipped when copying!!! (Index=%d)", nCount);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Copy object data (do not use 'memcpy' in here)
			*(this->m_apChildObjectList[nCount]) = *pSrcData;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Compare
//	Description:	Compare with another given object
//  Arguments:		pObj - Pointer of given object
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL JSON::Compare(const JSON& pObj) const
{
	BOOL bRet = FALSE;

	// Compare object name
	bRet &= (this->m_strObjectName == pObj.m_strObjectName);

	// Compare detail item info
	BOOL bRetCompareDetail = TRUE;
	if (this->m_arrKeyValuePairs.size() != pObj.m_arrKeyValuePairs.size()) {
		bRetCompareDetail = FALSE;
	}
	if (bRetCompareDetail != FALSE) {
		for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
			if (this->m_arrKeyValuePairs.at(nIndex) != pObj.m_arrKeyValuePairs.at(nIndex)) {
				bRetCompareDetail = FALSE;
				break;
			}
		}
	}
	bRet &= bRetCompareDetail;

	// Compare child objects
	if (this->m_apChildObjectList != NULL && pObj.m_apChildObjectList != NULL) {

		bRetCompareDetail = TRUE;

		// Compare child object numbers
		int nThisChildObjectCount = this->m_nChildObjectCount;
		int nOtherChildObjectCount = pObj.m_nChildObjectCount;
		if (nThisChildObjectCount != nOtherChildObjectCount) {
			bRetCompareDetail = FALSE;
		}
		else {
			// Compare each child object data
			for (int nCount = 0; nCount < nThisChildObjectCount; nCount++) {
				PJSONDATA pThisChildObject = this->m_apChildObjectList[nCount];
				PJSONDATA pOtherChildObject = pObj.m_apChildObjectList[nCount];
				if ((pThisChildObject != NULL) && (pOtherChildObject != NULL)) {
					bRetCompareDetail &= pThisChildObject->Compare(*pOtherChildObject);
				}
				else {
					bRetCompareDetail = FALSE;
					break;
				}
			}
		}
		bRet &= bRetCompareDetail;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	IsEmpty
//	Description:	Check if current JSON object is empty
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL JSON::IsEmpty(void) const
{
	// Initialize an empty item
	static const JSONDATA jsonDummyItem;

	// Compare with this item and return result
	return this->Compare(jsonDummyItem);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveProperty
//	Description:	Remove property by its key name
//  Arguments:		lpszKeyName - Key name
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::RemoveProperty(LPCTSTR lpszKeyName)
{
	// If property data is empty, do nothing
	if (this->m_arrKeyValuePairs.empty())
		return;

	// Search for key name
	int nFoundIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
		if (this->m_arrKeyValuePairs.at(nIndex).strKey == lpszKeyName) {
			nFoundIndex = nIndex;
			break;
		}
	}

	// Remove property by index
	this->RemoveProperty(nFoundIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all JSON object data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::RemoveAll(void)
{
	// Reset data
	this->m_strObjectName.Empty();					// JSON object name
	this->m_arrKeyValuePairs.clear();				// Key-value pairs

	// Remove all child objects
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pChildObj = this->m_apChildObjectList[nCount];
			if (pChildObj != NULL) {
				pChildObj->RemoveAll();
				delete pChildObj;
			}
		}
		delete[] (this->m_apChildObjectList);
		this->m_apChildObjectList = NULL;
		
		// Reset child object counter
		this->m_nChildObjectCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddString
//	Description:	Add a string-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Key name
//					lpszValue	- String value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddString(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	// Search if key name already existed
	for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
		JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);
		if (jsonEntry.strKey == lpszKeyName) {
			// Replace existed value with new value
			jsonEntry.strValue = lpszValue;
			return;
		}
	}

	// Add property
	this->m_arrKeyValuePairs.push_back({ lpszKeyName, lpszValue });
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddInteger
//	Description:	Add an integer-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Item name
//					nValue		- Signed integer value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddInteger(LPCTSTR lpszKeyName, INT nValue)
{
	// Convert integer to string
	CString strValue;
	strValue.Format(_T("%d"), nValue);

	// Add property
	AddString(lpszKeyName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddFloat
//	Description:	Add a float-typed key-value pair, update value if
//					the given key name already existed
//  Arguments:		lpszKeyName - Key name
//					dbValue		- Float value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddFloat(LPCTSTR lpszKeyName, DOUBLE dbValue)
{
	// Convert float number to string
	CString strValue;
	strValue.Format(_T("%f"), dbValue);

	// Add property
	AddString(lpszKeyName, strValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AddChildOject
//	Description:	Add a child object
//  Arguments:		pSrc - Source item data (pointer)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::AddChildObject(JSON* pSrc)
{
	// Check for data validity
	if (pSrc == NULL)
		return;

	// Index to copy
	size_t nIndex = 0;

	// Allocate child object array data memory if not yet allocated
	if (this->m_apChildObjectList == NULL) {
		this->m_apChildObjectList = new PJSONDATA;
		if (this->m_apChildObjectList == NULL) {
			TRACE_ERROR("Error: JSON child object array data allocation failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	else {
		nIndex = this->m_nChildObjectCount;
	}
	
	// Allocated destination child object memory
	this->m_apChildObjectList[nIndex] = new JSONDATA;
	if (this->m_apChildObjectList[nIndex] == NULL) {
		TRACE_FORMAT("Error: JSON new child object data allocation failed!!! (Index=%d)", nIndex);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Copy child object data (do not use 'memcpy' in here)
	*(this->m_apChildObjectList[nIndex]) = *pSrc;

	// Increase child object counter
	this->m_nChildObjectCount++;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Print
//	Description:	Print JSON object data with indentation
//  Arguments:		strOutput  - Output printed result string
//					nIndent	   - Indentation
//					bSeparator - Whether to add a blank line as separator
//					bMultiline - Whether to print the data in multiple lines
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::Print(CString& strOutput, int nIndent, BOOL bSeparator, BOOL bMultiline /* = TRUE */)
{
	// Empty output result string
	strOutput.Empty();

	// Make indentation
	CString strIndent = Constant::String::Empty;
	for (int nTabCount = 1; nTabCount <= nIndent; nTabCount++) {
		// Add indent (tab character)
		strIndent.Append(Constant::Symbol::JSON_Indent);
	}

	// Do not use indentation if printing in single line
	// This will make better visualization
	if (bMultiline != TRUE) {
		strIndent.Empty();
	}

	// Add indentation
	strOutput.Append(strIndent);

	CString strFormat = Constant::String::Empty;

	// Print object name (if set)
	if (!this->m_strObjectName.IsEmpty()) {
		strFormat.Format(_T("\"%s\": "), this->m_strObjectName.GetString());
		strOutput.Append(strFormat);
	}

	// Opening bracket
	strOutput.Append(_T("{ "));
	if (bMultiline == TRUE) {
		strOutput.Append(Constant::String::EndLine);
	}

	// Print list of properties
	size_t nItemNum = this->m_arrKeyValuePairs.size();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

		// Add indentation
		strOutput.Append(strIndent);

		// Get key and value
		const JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);

		// Format properties
		if ((nIndex == (nItemNum - 1)) &&
			((this->m_nChildObjectCount <= 0) || (this->m_apChildObjectList == NULL))) {

			// Last property (no other child object following) has no comma in the end
			strFormat.Format(_T("\t\"%s\": \"%s\" "), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(Constant::String::EndLine);
			}
		}
		else {
			// Add comma character at the end of each property
			strFormat.Format(_T("\t\"%s\": \"%s\", "), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
			strOutput.Append(strFormat);
			if (bMultiline == TRUE) {
				strOutput.Append(Constant::String::EndLine);
			}
		}
	}

	// Print child objects
	CString strSubItemOutput = Constant::String::Empty;
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pSubItem = this->m_apChildObjectList[nCount];
			if (pSubItem != NULL) {
				pSubItem->Print(strSubItemOutput, nIndent + 1, FALSE, bMultiline);
				strOutput.Append(strSubItemOutput);
			}
		}
	}

	// Add indentation and closing bracket
	strOutput.Append(strIndent);
	strOutput.Append(_T("} "));
	if (bMultiline == TRUE) {
		strOutput.Append(Constant::String::EndLine);
	}

	// Add a blank line as separator
	if (bSeparator == TRUE) {
		strOutput.Append(Constant::String::EndLine);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrintYAML
//	Description:	Print JSON object data in YAML format
//  Arguments:		strOutput  - Output printed result string
//					nIndent	   - Indentation
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void JSON::PrintYAML(CString& strOutput, int nIndent)
{
	// Empty output result string
	strOutput.Empty();

	// Indentation
	CString strIndent = Constant::String::Empty;
	for (int nCount = 1; nCount < nIndent; nCount++) {
		strIndent.Append(Constant::Symbol::YAML_Indent);
	}

	// Print object name (if set)
	if (!this->m_strObjectName.IsEmpty()) {
		strOutput.AppendFormat(_T("%s%s:\n"), strIndent.GetString(), this->m_strObjectName.GetString());
		strIndent.Append(Constant::Symbol::YAML_Indent); // Add one more indent for properties
	}

	// Print key-value pairs
	for (int nIndex = 0; nIndex < this->m_arrKeyValuePairs.size(); nIndex++) {
		const JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);
		strOutput.AppendFormat(_T("%s%s: \"%s\"\n"), strIndent.GetString(), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
	}

	// Print child objects
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pSubItem = this->m_apChildObjectList[nCount];
			if (pSubItem != NULL) {
				CString strSubItemOutput;
				pSubItem->PrintYAML(strSubItemOutput, nIndent + 1);
				strOutput.Append(strSubItemOutput);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SLogging
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::SLogging(BYTE byLogType)
{
	// Log data array
	m_arrLogData.clear();

	// Properties
	m_byLogType = byLogType;
	m_byWriteMode = LogWriteMode::ReadOnly;
	m_nMaxSize = INT_INFINITE;
	m_strFilePath = Constant::String::Empty;
	m_pItemDefTemplate = NULL;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~SLogging
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

SLogging::~SLogging()
{
	// Clean up log data
	m_arrLogData.clear();

	// Clean up default item template
	if (m_pItemDefTemplate != NULL) {
		delete m_pItemDefTemplate;
		m_pItemDefTemplate = NULL;
	}
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

	return m_arrLogData.at(nIndex);
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

	return m_arrLogData.at(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetDefaultTemplate
//	Description:	Get/set log default template
//  Arguments:		logItemTemplate - Log item template
//  Return value:	BYTE
//
//////////////////////////////////////////////////////////////////////////

void SLogging::SetDefaultTemplate(const LOGITEM& logItemTemplate)
{
	// Initialize default template
	if (m_pItemDefTemplate == NULL) {
		m_pItemDefTemplate = new LOGITEM;
		if (m_pItemDefTemplate == NULL) {
			TRACE_ERROR("Default item initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
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
	if (GetWriteMode() == LogWriteMode::WriteInstantly) {
		// Write instantly
		Write(logItem);
	}
	else {
		// If already reached max data size
		size_t nMaxSize = GetMaxSize();
		if ((nMaxSize != INT_INFINITE) && (GetLogCount() >= nMaxSize)) {
			// Can not output log item --> Trace info
			TRACE_ERROR("Output log item failed: Log data exceeded max size!!!");
			return;
		}

		// Store log data
		m_arrLogData.push_back(logItem);
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
	if (GetWriteMode() == LogWriteMode::WriteInstantly) {
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
				TRACE_ERROR("Output log string failed: No item to use as template!!!");
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
				TRACE_ERROR("Output log string failed: Default template not set!!!");
				return;
			}
			else {
				// Copy template
				logItem.Copy(*(this->GetDefaultTemplate()));
			}
		}

		// Update log item data
		logItem.SetTime(stLogTime);
		logItem.SetLogString(lpszLogString);
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

BOOL SLogging::Write(void)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log is set as Read-only
	// or current log mode is write instantly mode
	if ((this->GetWriteMode() == LogWriteMode::ReadOnly) ||
		(this->GetWriteMode() == LogWriteMode::WriteInstantly))
		return FALSE;

	CString strFileName;
	CString strCurFileName;
	CFile fLogFile;

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	LOGITEM logItem;
	SYSTEMTIME stTemp;
	CString strLogFormat;
	String filePath;

	// Setup performance counter for tracking
	PerformanceCounter counter;
	counter.Start();

	for (int nIndex = 0; nIndex < GetLogCount(); nIndex++)
	{
		// Get log item
		logItem = GetLogItem(nIndex);
		stTemp = logItem.GetTime();

		// Get filename according to type of logs
		switch (m_byLogType)
		{
		case LOGTYPE_APP_EVENT:
			// Format app event log filename
			strFileName.Format(FILENAME_APPEVENT_LOG, stTemp.wYear, stTemp.wMonth);
			filePath = StringUtils::MakeFilePath(folderPath, strFileName, FILEEXT_LOGFILE);
			if (strCurFileName.IsEmpty()) {
				// Set current file name
				strCurFileName = strFileName;
			}

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

				// Close current file
				fLogFile.Close();

				// Set new current file name
				strCurFileName = strFileName;
			}
			break;

		case LOGTYPE_HISTORY_LOG:
			// App history log
			strFileName = FILENAME_HISTORY_LOG;
			filePath = StringUtils::MakeFilePath(folderPath, strFileName, FILEEXT_LOGFILE);
			break;

		default:
			// Wrong argument
			TRACE_ERROR("Write log failed: Invalid log type!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			// Show error message
			dwErrCode = APP_ERROR_WRONG_ARGUMENT;
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			break;
		}

		// Check if file is opening, if not, open it
		if (fLogFile.m_hFile == CFile::hFileNull)
		{
			bResult = fLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
			if (bResult == FALSE) {

				// Open file failed
				dwErrCode = GetLastError();

				// Trace error
				TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
				return bResult;
			}

			// Go to end of file
			fLogFile.SeekToEnd();
		}

		// Format output log strings
		strLogFormat += logItem.FormatOutput();
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

	// Display performance counter
	counter.Stop();
	OutputDebugLogFormat(_T("Total write log time: %.4f (ms)"), counter.GetElapsedTime(TRUE));

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

BOOL SLogging::Write(const LOGITEM& logItem, LPCTSTR /* lpszFilePath  = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return FALSE;

	CString strFileName;
	CFile fLogFile;

	CString strLogFormat;

	// Get log time
	SYSTEMTIME stTimeTemp;
	stTimeTemp = logItem.GetTime();

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		strFileName.Format(FILENAME_APPEVENT_LOG, stTimeTemp.wYear, stTimeTemp.wMonth);
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;

	default:
		// Wrong argument
		TRACE_ERROR("Error: Invalid log type!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
	}

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Get file path
	String filePath = StringUtils::MakeFilePath(folderPath, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {

			// Open file failed
			dwErrCode = GetLastError();

			// Trace error
			TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	strLogFormat = logItem.FormatOutput();

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

BOOL SLogging::Write(LPCTSTR lpszLogString, LPCTSTR /* lpszFilePath  = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return FALSE;

	CString strFileName;
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
		strFileName.Format(FILENAME_APPEVENT_LOG, stCurTime.wYear, stCurTime.wMonth);
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		strFileName = FILENAME_HISTORY_LOG;
		break;

	default:
		// Wrong argument
		TRACE_ERROR("Error: Invalid log type!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		// Show error message
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return FALSE;
	}

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	// Get file path
	String filePath = StringUtils::MakeFilePath(folderPath, strFileName, FILEEXT_LOGFILE);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {

			// Open file failed
			dwErrCode = GetLastError();

			// Trace error
			TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", dwErrCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			dwErrCode = GetLastError();
			PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
			return bResult;
		}

		// Go to end of file
		fLogFile.SeekToEnd();
	}

	// Format output log strings
	LOGITEM logItem;
	logItem.SetTime(stCurTime);
	logItem.SetLogString(lpszLogString);
	strLogFormat = logItem.FormatOutput();

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

