/**
 * @file		Logging.cpp
 * @brief		Implement necessary methods to read/write app logs
 * @author		AnthonyLeeStark
 * @date		2024.02.22
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;


/**
 * @brief	Get size of data by data type
 * @param	byDataType - Data type
 * @return	SIZE_T
 */
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

	case LogDataType::ClockTimeData:	// Clock-time data
		retSize = sizeof(ClockTime);
		break;

	case LogDataType::DateTimeData:		// Date/time data
		retSize = sizeof(DateTime);
		break;

	default:
		retSize = INT_NULL;
		break;
	}

	return retSize;
}

/**
 * @brief	Constructor
 */
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

/**
 * @brief	Copy assignment operator
 * @param	Default
 * @return	LogDetail&
 */
LogDetail& LogDetail::operator=(const LogDetail& pItem)
{
	// Copy data
	this->Copy(pItem);
	return *this;
}

/**
 * @brief	Initialize/reset item data
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Copy data from another item
 * @param	pItem - Pointer of input item
 * @return	None
 */
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

/**
 * @brief	Copy detail info pointer
 * @param	pItem - Pointer of input item
 * @return	None
 */
void LogDetail::PointerCopy(const LogDetail& pItem)
{
	// Copy pointer properties
	m_byPointerType = pItem.m_byPointerType;					// Detail info pointer data type
	m_szPointerSize = pItem.m_szPointerSize;					// Detail info pointer data size

	// Copy pointer data
	memcpy(m_ptrDetailData, pItem.m_ptrDetailData, pItem.m_szPointerSize);
}

/**
 * @brief	Compare with another given item
 * @param	pItem - Pointer of given item
 * @return	TRUE/FALSE
 */
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

/**
 * @brief	Compare detail info pointers
 * @param	pItem - Pointer of given item
 * @return	TRUE/FALSE
 */
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

/**
 * @brief	Check if current detail info data is empty
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL LogDetail::IsEmpty(void) const
{
	// Initialize empty detail info
	static const LOGDETAIL logDummyDetail;

	// Compare with that data and return result
	return this->Compare(logDummyDetail);
}

/**
 * @brief	Set detail info pointer data
 * @param	pDataBuff	- Data buffer (pointer)
 * @param	byDataType	- Data type
 * @param	szDataSize	- Data size
 * @return	TRUE/FALSE
 */
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

/**
 * @brief	Constructor
 */
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

/**
 * @brief	Copy assignment operator
 * @param	Default
 * @return	LogDetailInfo&
 */
LogDetailInfo& LogDetailInfo::operator=(const LogDetailInfo& pData)
{
	// Copy data
	this->CopyData(pData);
	return *this;
}

/**
 * @brief	Initialize/reset data
 * @param	None
 * @return	None
 */
void LogDetailInfo::Init(void)
{
	// Reset data
	this->clear();
}

/**
 * @brief	Copy data from another log detail info data
 * @param	pData - Pointer of input item
 * @return	None
 */
void LogDetailInfo::CopyData(const LogDetailInfo& pData)
{
	// Remove all data first
	this->Init();

	// Copy data
	this->assign(pData.begin(), pData.end());
}

/**
 * @brief	Add log detail info item
 * @param	logDetail	  - Log detail item
 * @param	usCategory	  - Detail category
 * @param	nDetailInfo	  - Detail info (integer)
 * @param	detailInfo	  - Detail info (string)
 * @param	nFlag		  - Detail flag
 * @return	None
 */
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

void LogDetailInfo::AddDetail(USHORT usCategory, const wchar_t* detailInfo, INT nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailString(detailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

void LogDetailInfo::AddDetail(USHORT usCategory, INT nDetailInfo, const wchar_t* detailInfo, INT nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailValue(nDetailInfo);
	logDetail.SetDetailString(detailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

/**
 * @brief	Constructor
 */
LogItem::LogItem()
{
	// Initialization
	m_stTime = DateTime();									// Log time
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

/**
 * @brief	Copy assignment operator
 * @param	Default
 * @return	LogItem&
 */
LogItem& LogItem::operator=(const LogItem& pItem)
{
	// Copy data
	this->Copy(pItem);
	return *this;
}

/**
 * @brief	Copy data from another log item
 * @param	pItem - Pointer of input item
 * @return	None
 */
void LogItem::Copy(const LogItem& pItem)
{
	// Copy data
	m_stTime = pItem.m_stTime;								// Log time
	m_dwProcessID = pItem.m_dwProcessID;					// Process ID
	m_usCategory = pItem.m_usCategory;						// Log category
	m_strLogString = pItem.m_strLogString;					// Log string
	m_arrDetailInfo = pItem.m_arrDetailInfo;				// Log detail info
}

/**
 * @brief	Compare with another given item
 * @param	pItem - Pointer of given item
 * @return	TRUE/FALSE
 */
BOOL LogItem::Compare(const LogItem& pItem) const
{
	BOOL bRet = FALSE;

	// Compare item
	bRet &= (m_stTime == pItem.m_stTime);
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

/**
 * @brief	Check if current log item is empty
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL LogItem::IsEmpty(void) const
{
	// Initialize an empty item
	static const LOGITEM logDummyItem;

	// Compare with this item and return result
	return this->Compare(logDummyItem);
}

/**
 * @brief	Remove all log item data
 * @param	None
 * @return	None
 */
void LogItem::RemoveAll(void)
{
	// Reset data
	m_stTime = DateTime();										// Log time
	m_dwProcessID = INT_NULL;									// Process ID
	m_usCategory = LOG_MACRO_NONE;								// Log category
	m_strLogString = Constant::String::Empty;					// Log string

	// Clean up log detail info data
	this->RemoveDetailInfo();									// Log detail info
}

/**
 * @brief	Return a formatted logitem date/time string
 * @param	None
 * @return	String - Formatted result
 */
String LogItem::FormatDateTime(void) const
{
	const wchar_t* middayFlag = (m_stTime.Hour() >= 12) ? Constant::Symbol::PostMeridiem : Constant::Symbol::AnteMeridiem;
	String templateFormatStr = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormatStr, m_stTime.Year(), m_stTime.Month(), m_stTime.Day(),
		m_stTime.Hour(), m_stTime.Minute(), m_stTime.Second(), m_stTime.Millisecond(), middayFlag);

	return timeFormatString;
}

/**
 * @brief	Return formatted output log string for file writing
 * @param	None
 * @return	String - Formatted result
 */
String LogItem::FormatOutput(void) const
{
	// Create JSON data object
	JSONDATA jsonData;

	String logKey;
	String logValue;

	// Load default language table package
	LANGTABLE_PTR pDefLang = LoadLanguageTable(NULL);

	/*********************************************************************/
	/*																	 */
	/*			  Convert log item base info into JSON data				 */
	/*																	 */
	/*********************************************************************/
	// Log time
	logKey = GetString(StringTable::LogKey, BaseLog::Time);
	jsonData.AddString(logKey, FormatDateTime());

	// Process ID
	logKey = GetString(StringTable::LogKey, BaseLog::PID);
	jsonData.AddInteger(logKey, m_dwProcessID);

	// Log category
	logKey = GetString(StringTable::LogKey, BaseLog::LogCategory);
	logValue = GetLanguageString(pDefLang, m_usCategory);
	jsonData.AddString(logKey, logValue);

	// Log description string
	logKey = GetString(StringTable::LogKey, BaseLog::Description);
	jsonData.AddString(logKey, m_strLogString);

	/*********************************************************************/
	/*																	 */
	/*		  Convert log item detail info data into JSON data			 */
	/*																	 */
	/*********************************************************************/
	// Log detail info
	if (!m_arrDetailInfo.empty()) {

		// Create JSON detail data object
		JSONDATA jsonDetailData;

		String detailKey;
		String detailValue;

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
			detailKey = GetString(StringTable::LogKey, logDetail.GetCategory());

			// Detail info value
			int nDetailValue = logDetail.GetDetailValue();
			if (nDetailFlag & LogDetailFlag::Write_Int) {
				jsonDetailData.AddInteger(detailKey, logDetail.GetDetailValue());
			}
			else if (nDetailFlag & LogDetailFlag::LookUp_Dict) {
				detailValue = GetString(StringTable::LogValue, nDetailValue);
				jsonDetailData.AddString(detailKey, detailValue);
			}
			else if (nDetailFlag & LogDetailFlag::Write_String) {
				jsonDetailData.AddString(detailKey, logDetail.GetDetailString());
			}
			else if (nDetailFlag & (LogDetailFlag::Write_Int & LogDetailFlag::Write_String)) {
				JSONDATA jsonSubDetail;
				detailKey = GetString(StringTable::LogKey, BaseLog::DetailNumeric);
				jsonSubDetail.AddInteger(detailKey, logDetail.GetDetailValue());
				detailKey = GetString(StringTable::LogKey, BaseLog::DetailString);
				jsonSubDetail.AddString(detailKey, logDetail.GetDetailString());
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
	String logYAMLFormat;
	jsonData.PrintYAML(logYAMLFormat, 0);
	logYAMLFormat.Append(Constant::String::NewLine);

	return logYAMLFormat;
}

/**
 * @brief	Constructor
 */
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

/**
 * @brief	Destructor
 */
JSON::~JSON()
{
	// Remove all data and clean-up
	this->RemoveAll();
}

/**
 * @brief	Copy assignment operator
 * @param	Default
 * @return	JSON&
 */
JSON& JSON::operator=(const JSON& pObj)
{
	// Copy data
	this->Copy(pObj);
	return *this;
}

/**
 * @brief	Copy data from another JSON object
 * @param	pObj - Pointer of input object
 * @return	None
 */
void JSON::Copy(const JSON& pObj)
{
	// Do not copy itself
	if (this == &pObj) return;

	// Copy data
	this->m_strObjectName = pObj.m_strObjectName;	// JSON object name
	this->CopyArrayData(pObj);						// Property (array) data
	this->CopyPtrData(pObj);						// Child object (pointer) data
}

/**
 * @brief	Copy object array data from another JSON object
 * @param	pObj - Pointer of input object
 * @return	None
 */
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

/**
 * @brief	Copy object pointer data from another JSON object
 * @param	pObj - Pointer of input object
 * @return	None
 */
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

/**
 * @brief	Compare with another given object
 * @param	pObj - Pointer of given object
 * @return	TRUE/FALSE
 */
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

/**
 * @brief	Check if current JSON object is empty
 * @param	None
 * @return	TRUE/FALSE
 */
BOOL JSON::IsEmpty(void) const
{
	// Initialize an empty item
	static const JSONDATA jsonDummyItem;

	// Compare with this item and return result
	return this->Compare(jsonDummyItem);
}

/**
 * @brief	Remove property by its key name
 * @param	keyName - Key name
 * @return	None
 */
void JSON::RemoveProperty(const wchar_t* keyName)
{
	// If property data is empty, do nothing
	if (this->m_arrKeyValuePairs.empty())
		return;

	// Search for key name
	int nFoundIndex = INT_INVALID;
	for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
		if (this->m_arrKeyValuePairs.at(nIndex).strKey == keyName) {
			nFoundIndex = nIndex;
			break;
		}
	}

	// Remove property by index
	this->RemoveProperty(nFoundIndex);
}

/**
 * @brief	Remove all JSON object data
 * @param	None
 * @return	None
 */
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

/**
 * @brief	Add a string-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Key name
 * @param	value	- String value
 * @return	None
 */
void JSON::AddString(const wchar_t* keyName, const wchar_t* value)
{
	// Search if key name already existed
	for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
		JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);
		if (jsonEntry.strKey == keyName) {
			// Replace existed value with new value
			jsonEntry.strValue = value;
			return;
		}
	}

	// Add property
	this->m_arrKeyValuePairs.push_back({ keyName, value });
}

/**
 * @brief	Add an integer-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Item name
 * @param	nValue	- Signed integer value
 * @return	None
 */
void JSON::AddInteger(const wchar_t* keyName, INT nValue)
{
	// Convert integer to string
	String valueStr = StringUtils::StringFormat(_T("%d"), nValue);

	// Add property
	AddString(keyName, valueStr);
}

/**
 * @brief	Add a float-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Key name
 * @param	dbValue	- Float value
 * @return	None
 */
void JSON::AddFloat(const wchar_t* keyName, DOUBLE dbValue)
{
	// Convert float number to string
	String valueStr = StringUtils::StringFormat(_T("%f"), dbValue);

	// Add property
	AddString(keyName, valueStr);
}

/**
 * @brief	Add a child object
 * @param	pSrc - Source item data (pointer)
 * @return	None
 */
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

/**
 * @brief	Print JSON object data with indentation
 * @param	outputString  - Output printed result string
 * @param	nIndent		  - Indentation
 * @param	bSeparator    - Whether to add a blank line as separator
 * @param	bMultiline    - Whether to print the data in multiple lines
 * @return	None
 */
void JSON::Print(String& outputString, int nIndent, BOOL bSeparator, BOOL bMultiline /* = TRUE */)
{
	// Empty output result string
	outputString.Empty();

	// Make indentation
	String indentationStr = Constant::String::Empty;
	for (int nTabCount = 1; nTabCount <= nIndent; nTabCount++) {
		// Add indent (tab character)
		indentationStr.Append(Constant::Symbol::JSON_Indent);
	}

	// Do not use indentation if printing in single line
	// This will make better visualization
	if (bMultiline != TRUE) {
		indentationStr.Empty();
	}

	// Add indentation
	outputString.Append(indentationStr);

	String formatStr = Constant::String::Empty;

	// Print object name (if set)
	if (!this->m_strObjectName.IsEmpty()) {
		formatStr.Format(_T("\"%s\": "), this->m_strObjectName.GetString());
		outputString.Append(formatStr);
	}

	// Opening bracket
	outputString.Append(_T("{ "));
	if (bMultiline == TRUE) {
		outputString.Append(Constant::String::EndLine);
	}

	// Print list of properties
	size_t nItemNum = this->m_arrKeyValuePairs.size();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

		// Add indentation
		outputString.Append(indentationStr);

		// Get key and value
		const JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);

		// Format properties
		if ((nIndex == (nItemNum - 1)) &&
			((this->m_nChildObjectCount <= 0) || (this->m_apChildObjectList == NULL))) {

			// Last property (no other child object following) has no comma in the end
			formatStr.Format(_T("\t\"%s\": \"%s\" "), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
			outputString.Append(formatStr);
			if (bMultiline == TRUE) {
				outputString.Append(Constant::String::EndLine);
			}
		}
		else {
			// Add comma character at the end of each property
			formatStr.Format(_T("\t\"%s\": \"%s\", "), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
			outputString.Append(formatStr);
			if (bMultiline == TRUE) {
				outputString.Append(Constant::String::EndLine);
			}
		}
	}

	// Print child objects
	String subItemOutput = Constant::String::Empty;
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pSubItem = this->m_apChildObjectList[nCount];
			if (pSubItem != NULL) {
				pSubItem->Print(subItemOutput, nIndent + 1, FALSE, bMultiline);
				outputString.Append(subItemOutput);
			}
		}
	}

	// Add indentation and closing bracket
	outputString.Append(indentationStr);
	outputString.Append(_T("} "));
	if (bMultiline == TRUE) {
		outputString.Append(Constant::String::EndLine);
	}

	// Add a blank line as separator
	if (bSeparator == TRUE) {
		outputString.Append(Constant::String::EndLine);
	}
}

/**
 * @brief	Print JSON object data in YAML format
 * @param	outputString  - Output printed result string
 * @param	nIndent		  - Indentation
 * @return	None
 */
void JSON::PrintYAML(String& outputString, int nIndent)
{
	// Empty output result string
	outputString.Empty();

	// Indentation
	String indentationStr = Constant::String::Empty;
	for (int nCount = 1; nCount < nIndent; nCount++) {
		indentationStr.Append(Constant::Symbol::YAML_Indent);
	}

	String formatStr = Constant::String::Empty;

	// Print object name (if set)
	if (!this->m_strObjectName.IsEmpty()) {
		formatStr = StringUtils::StringFormat(_T("%s%s:\n"), indentationStr.GetString(), this->m_strObjectName.GetString());
		outputString.Append(formatStr);
		indentationStr.Append(Constant::Symbol::YAML_Indent); // Add one more indent for properties
	}

	// Print key-value pairs
	for (int nIndex = 0; nIndex < this->m_arrKeyValuePairs.size(); nIndex++) {
		const JSON_ENTRY& jsonEntry = this->m_arrKeyValuePairs.at(nIndex);
		formatStr = StringUtils::StringFormat(_T("%s%s: \"%s\"\n"), indentationStr.GetString(), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
		outputString.Append(formatStr);
	}

	// Print child objects
	if ((this->m_nChildObjectCount > 0) && (this->m_apChildObjectList != NULL)) {
		for (int nCount = 0; nCount < this->m_nChildObjectCount; nCount++) {
			PJSONDATA pSubItem = this->m_apChildObjectList[nCount];
			if (pSubItem != NULL) {
				String subItemOutput;
				pSubItem->PrintYAML(subItemOutput, nIndent + 1);
				outputString.Append(subItemOutput);
			}
		}
	}
}

/**
 * @brief	Constructor
 */
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

/**
 * @brief	Destructor
 */
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

/**
 * @brief	Return a specific log item of log list
 * @param	nIndex - Item index
 * @return	LOGITEM - Return log item
 */
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

/**
 * @brief	Get/set log default template
 * @param	logItemTemplate - Log item template
 * @return	BYTE
 */
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

/**
 * @brief	Add a log item into log data
 * @param	logItem	- Log item to write
 * @return	None
 */
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

/**
 * @brief	Add a log string into log list data
 * @param	logString - Log string
 * @param	byType	  - Log type
 * @return	None
 */
void SLogging::OutputString(const wchar_t* logString, BOOL bUseLastTemplate /* = TRUE */)
{
	if (GetWriteMode() == LogWriteMode::WriteInstantly) {
		// Write instantly
		Write(logString);
	}
	else {
		// Get log time
		DateTime stLogTime = DateTimeUtils::GetCurrentDateTime();

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
		logItem.SetLogString(logString);
		OutputItem(logItem);
	}
}

/**
 * @brief	Write log data into the corresponding logfile
 * @param	None
 * @return	BOOL - Result of log writing process
 */
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

	CFile fLogFile;
	String fileName;
	String currentFileName;

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(SUBFOLDER_LOG);

	LOGITEM logItem;
	DateTime stTemp;
	String logFormatString;
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
			fileName.Format(FILENAME_APPEVENT_LOG, stTemp.Year(), stTemp.Month());
			filePath = StringUtils::MakeFilePath(folderPath, fileName, FILEEXT_LOGFILE);
			if (currentFileName.IsEmpty()) {
				// Set current file name
				currentFileName = fileName;
			}

			// If a file with another name (previous day's log file) is opening,
			// write down all current log strings and close the file
			if ((fLogFile.m_hFile != CFile::hFileNull) && (fileName != currentFileName))
			{
				if (!logFormatString.IsEmpty()) {

					// Write log strings to file
					fLogFile.Write(logFormatString, logFormatString.GetLength() * sizeof(wchar_t));
					fLogFile.Flush();

					logFormatString.Empty();
				}

				// Close current file
				fLogFile.Close();

				// Set new current file name
				currentFileName = fileName;
			}
			break;

		case LOGTYPE_HISTORY_LOG:
			// App history log
			fileName = FILENAME_HISTORY_LOG;
			filePath = StringUtils::MakeFilePath(folderPath, fileName, FILEEXT_LOGFILE);
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
		logFormatString += logItem.FormatOutput();
	}

	if (!logFormatString.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(logFormatString, logFormatString.GetLength() * sizeof(wchar_t));
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

/**
 * @brief	Write log item instantly into logfile
 * @param	logItem	 - Log item to write
 * @param	filePath - Output log file path
 * @return	BOOL - Result of log writing process
 */
BOOL SLogging::Write(const LOGITEM& logItem, const wchar_t* /* filePath = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return FALSE;

	String fileName;
	CFile fLogFile;

	String logFormatString;

	// Get log time
	DateTime stTimeTemp = logItem.GetTime();

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		fileName.Format(FILENAME_APPEVENT_LOG, stTimeTemp.Year(), stTimeTemp.Month());
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		fileName = FILENAME_HISTORY_LOG;
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
	String filePath = StringUtils::MakeFilePath(folderPath, fileName, FILEEXT_LOGFILE);

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
	logFormatString = logItem.FormatOutput();

	if (!logFormatString.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(logFormatString, logFormatString.GetLength() * sizeof(wchar_t));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

/**
 * @brief	Write log string instantly into logfile
 * @param	logString - Log string
 * @return	BOOL - Result of log writing process
 */
BOOL SLogging::Write(const wchar_t* logString, const wchar_t* /* filePath  = NULL */)
{
	BOOL bResult = TRUE;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return FALSE;

	String fileName;
	CFile fLogFile;

	String logFormatString;

	// Get log time
	DateTime stCurTime = DateTimeUtils::GetCurrentDateTime();

	// Get filename according to type of logs
	switch (m_byLogType)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		fileName.Format(FILENAME_APPEVENT_LOG, stCurTime.Year(), stCurTime.Month());
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		fileName = FILENAME_HISTORY_LOG;
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
	String filePath = StringUtils::MakeFilePath(folderPath, fileName, FILEEXT_LOGFILE);

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
	logItem.SetLogString(logString);
	logFormatString = logItem.FormatOutput();

	if (!logFormatString.IsEmpty()) {
		// Write log strings to file
		fLogFile.Write(logFormatString, logFormatString.GetLength() * sizeof(wchar_t));
		fLogFile.Flush();
	}

	// Close file after done writing
	if (fLogFile.m_hFile != CFile::hFileNull) {
		fLogFile.Close();
	}

	return TRUE;
}

