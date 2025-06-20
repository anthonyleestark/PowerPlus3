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
 * @return	size_t
 */
size_t GetSizeByType(byte byDataType)
{
	size_t retSize = 0;
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

	case LogDataType::Boolean:			// Boolean number (true/false)
		retSize = sizeof(bool);
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

/**
 * @brief	Initialize/reset item data
 * @param	None
 * @return	Nones
 */
void LogDetail::Init(void) noexcept
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
 * @param	other - Pointer of input item
 * @return	None
 */
void LogDetail::Copy(const LogDetail& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	m_usCategory = other.m_usCategory;						// Detail category
	m_nFlag = other.m_nFlag;								// Detail flag
	m_nDetailValue = other.m_nDetailValue;					// Detail value (integer)
	m_strDetailInfo = other.m_strDetailInfo;				// Detail info (string)
	PointerCopy(other);										// Detail data (pointer)
}

/**
 * @brief	Copy detail info pointer
 * @param	other - Pointer of input item
 * @return	None
 */
void LogDetail::PointerCopy(const LogDetail& other)
{
	// Copy pointer properties
	m_byPointerType = other.m_byPointerType;					// Detail info pointer data type
	m_szPointerSize = other.m_szPointerSize;					// Detail info pointer data size

	// Copy pointer data
	memcpy(m_ptrDetailData, other.m_ptrDetailData, other.m_szPointerSize);
}

/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogDetail::Compare(const LogDetail& other) const
{
	bool bRet = false;

	// Compare items
	bRet &= (m_usCategory == other.m_usCategory);				// Detail category
	bRet &= (m_nFlag == other.m_nFlag);							// Detail flag
	bRet &= (m_nDetailValue == other.m_nDetailValue);			// Detail value (integer)
	bRet &= (m_strDetailInfo == other.m_strDetailInfo);			// Detail info (string)
	bRet &= PointerCompare(other);								// Detail data (pointer)

	return bRet;
}

/**
 * @brief	Compare detail info pointers
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogDetail::PointerCompare(const LogDetail& other) const
{
	bool bRet = false;

	// Compare properties
	bRet &= (m_byPointerType == other.m_byPointerType);			// Detail info pointer data type
	bRet &= (m_szPointerSize == other.m_szPointerSize);			// Detail info pointer data size

	// Only compare pointer values if properties are matching
	if (bRet != false) {
		bRet &= memcmp(m_ptrDetailData, other.m_ptrDetailData, m_szPointerSize);
	}

	return bRet;
}

/**
 * @brief	Check if current detail info data is empty
 * @param	None
 * @return	true/false
 */
bool LogDetail::IsEmpty(void) const noexcept
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
 * @return	true/false
 */
bool LogDetail::SetPointerData(PVOID pDataBuff, byte byDataType /* = DATA_TYPE_UNSPECIFIED */, size_t szDataSize /* = 0 */)
{
	// If data type is void (unusable), do nothing
	if (byDataType == LogDataType::Void)
		return false;

	// If both data type and size are not specified, do nothing
	if ((byDataType == LogDataType::Unspecified) && (szDataSize == 0))
		return false;

	// If size is not specified,
	if (szDataSize == 0) {
		// Get size by data type
		szDataSize = GetSizeByType(byDataType);

		// Get size failed, do nothing
		if (szDataSize == 0)
			return false;
	}

	// Otherwise, set normally
	m_byPointerType = byDataType;
	m_szPointerSize = szDataSize;
	memcpy(m_ptrDetailData, pDataBuff, szDataSize);
	return true;
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
void LogDetailInfo::AddDetail(USHORT usCategory, int nDetailInfo, int nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailValue(nDetailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

void LogDetailInfo::AddDetail(USHORT usCategory, const wchar_t* detailInfo, int nFlag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.SetCategory(usCategory);
	logDetail.SetDetailString(detailInfo);
	logDetail.SetFlag(nFlag);

	// Add detail info item
	this->AddDetail(logDetail);
}

void LogDetailInfo::AddDetail(USHORT usCategory, int nDetailInfo, const wchar_t* detailInfo, int nFlag /* = LogDetailFlag::Flag_Null */)
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

/**
 * @brief	Copy data from another log item
 * @param	other - Pointer of input item
 * @return	None
 */
void LogItem::Copy(const LogItem& other) noexcept
{
	// Copy data
	m_stTime = other.m_stTime;								// Log time
	m_dwProcessID = other.m_dwProcessID;					// Process ID
	m_usCategory = other.m_usCategory;						// Log category
	m_strLogString = other.m_strLogString;					// Log string
	m_arrDetailInfo = other.m_arrDetailInfo;				// Log detail info
}

/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogItem::Compare(const LogItem& other) const noexcept
{
	bool bRet = false;

	// Compare item
	bRet &= (m_stTime == other.m_stTime);
	bRet &= (m_dwProcessID == other.m_dwProcessID);
	bRet &= (m_usCategory == other.m_usCategory);
	bRet &= (m_strLogString == other.m_strLogString);

	// Compare log detail info
	bool bDetailInfoCompare = true;
	if (this->m_arrDetailInfo.size() != other.m_arrDetailInfo.size()) {
		bDetailInfoCompare = false;
	}
	else {
		for (int nIndex = 0; nIndex < this->m_arrDetailInfo.size(); nIndex++) {
			if (this->m_arrDetailInfo.at(nIndex).Compare(other.m_arrDetailInfo.at(nIndex)) != true) {
				bDetailInfoCompare = false;
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
 * @return	true/false
 */
bool LogItem::IsEmpty(void) const noexcept
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
void LogItem::RemoveAll(void) noexcept
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

/**
 * @brief	Copy data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::Copy(const JSON& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	this->m_strObjectName = other.m_strObjectName;	// JSON object name
	this->CopyArrayData(other);						// Property (array) data
	this->CopyPtrData(other);						// Child object (pointer) data
}

/**
 * @brief	Copy object array data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::CopyArrayData(const JSON& other)
{
	// Remove all existing array data
	this->m_arrKeyValuePairs.clear();

	// Set destination array data size
	this->m_arrKeyValuePairs.reserve(other.m_arrKeyValuePairs.size());

	// Copy list of key-value pairs
	for (int nIndex = 0; nIndex < other.m_arrKeyValuePairs.size(); nIndex++) {
		this->m_arrKeyValuePairs.push_back(other.m_arrKeyValuePairs.at(nIndex));
	}
}

/**
 * @brief	Copy object pointer data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::CopyPtrData(const JSON& other)
{
	// Number of child objects
	this->m_nChildObjectCount = other.m_nChildObjectCount;

	// List of child objects (pointer copy)
	if ((other.m_nChildObjectCount > 0) && (other.m_apChildObjectList != NULL)) {

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
			PJSONDATA pSrcData = other.m_apChildObjectList[nCount];
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
 * @param	other - Pointer of given object
 * @return	true/false
 */
bool JSON::Compare(const JSON& other) const
{
	bool bRet = false;

	// Compare object name
	bRet &= (this->m_strObjectName == other.m_strObjectName);

	// Compare detail item info
	bool bRetCompareDetail = true;
	if (this->m_arrKeyValuePairs.size() != other.m_arrKeyValuePairs.size()) {
		bRetCompareDetail = false;
	}
	if (bRetCompareDetail != false) {
		for (int nIndex = 0; nIndex < (this->m_arrKeyValuePairs.size()); nIndex++) {
			if (this->m_arrKeyValuePairs.at(nIndex) != other.m_arrKeyValuePairs.at(nIndex)) {
				bRetCompareDetail = false;
				break;
			}
		}
	}
	bRet &= bRetCompareDetail;

	// Compare child objects
	if (this->m_apChildObjectList != NULL && other.m_apChildObjectList != NULL) {

		bRetCompareDetail = true;

		// Compare child object numbers
		int nThisChildObjectCount = this->m_nChildObjectCount;
		int nOtherChildObjectCount = other.m_nChildObjectCount;
		if (nThisChildObjectCount != nOtherChildObjectCount) {
			bRetCompareDetail = false;
		}
		else {
			// Compare each child object data
			for (int nCount = 0; nCount < nThisChildObjectCount; nCount++) {
				PJSONDATA pThisChildObject = this->m_apChildObjectList[nCount];
				PJSONDATA pOtherChildObject = other.m_apChildObjectList[nCount];
				if ((pThisChildObject != NULL) && (pOtherChildObject != NULL)) {
					bRetCompareDetail &= pThisChildObject->Compare(*pOtherChildObject);
				}
				else {
					bRetCompareDetail = false;
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
 * @return	true/false
 */
bool JSON::IsEmpty(void) const noexcept
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
void JSON::AddInteger(const wchar_t* keyName, int nValue)
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
void JSON::Print(String& outputString, int nIndent, bool bSeparator, bool bMultiline /* = true */) const
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
	if (bMultiline != true) {
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
	if (bMultiline == true) {
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
			if (bMultiline == true) {
				outputString.Append(Constant::String::EndLine);
			}
		}
		else {
			// Add comma character at the end of each property
			formatStr.Format(_T("\t\"%s\": \"%s\", "), jsonEntry.strKey.GetString(), jsonEntry.strValue.GetString());
			outputString.Append(formatStr);
			if (bMultiline == true) {
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
				pSubItem->Print(subItemOutput, nIndent + 1, false, bMultiline);
				outputString.Append(subItemOutput);
			}
		}
	}

	// Add indentation and closing bracket
	outputString.Append(indentationStr);
	outputString.Append(_T("} "));
	if (bMultiline == true) {
		outputString.Append(Constant::String::EndLine);
	}

	// Add a blank line as separator
	if (bSeparator == true) {
		outputString.Append(Constant::String::EndLine);
	}
}

/**
 * @brief	Print JSON object data in YAML format
 * @param	outputString  - Output printed result string
 * @param	nIndent		  - Indentation
 * @return	None
 */
void JSON::PrintYAML(String& outputString, int nIndent) const
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
SLogging::SLogging(byte byLogType)
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
 * @return	byte
 */
void SLogging::SetDefaultTemplate(const LOGITEM& logItemTemplate) noexcept
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
void SLogging::OutputString(const wchar_t* logString, bool bUseLastTemplate /* = true */)
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
		if (bUseLastTemplate == true) {
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
 * @return	bool - Result of log writing process
 */
bool SLogging::Write(void)
{
	bool bResult = true;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log is set as Read-only
	// or current log mode is write instantly mode
	if ((this->GetWriteMode() == LogWriteMode::ReadOnly) ||
		(this->GetWriteMode() == LogWriteMode::WriteInstantly))
		return false;

	CFile fLogFile;
	String fileName;
	String currentFileName;

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

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
			fileName.Format(Constant::File::Name::AppEventLog, stTemp.Year(), stTemp.Month());
			filePath = StringUtils::MakeFilePath(folderPath, fileName, Constant::File::Extension::Log);
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
			fileName = Constant::File::Name::AppHistory;
			filePath = StringUtils::MakeFilePath(folderPath, fileName, Constant::File::Extension::Log);
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
			if (bResult == false) {

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
	OutputDebugLogFormat(_T("Total write log time: %.4f (ms)"), counter.GetElapsedTime(true));

	return true;
}

/**
 * @brief	Write log item instantly into logfile
 * @param	logItem	 - Log item to write
 * @param	filePath - Output log file path
 * @return	bool - Result of log writing process
 */
bool SLogging::Write(const LOGITEM& logItem, const wchar_t* /* filePath = NULL */)
{
	bool bResult = true;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return false;

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
		fileName.Format(Constant::File::Name::AppEventLog, stTimeTemp.Year(), stTimeTemp.Month());
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		fileName = Constant::File::Name::AppHistory;
		break;

	default:
		// Wrong argument
		TRACE_ERROR("Error: Invalid log type!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return false;
	}

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Get file path
	String filePath = StringUtils::MakeFilePath(folderPath, fileName, Constant::File::Extension::Log);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == false) {

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

	return true;
}

/**
 * @brief	Write log string instantly into logfile
 * @param	logString - Log string
 * @return	bool - Result of log writing process
 */
bool SLogging::Write(const wchar_t* logString, const wchar_t* /* filePath  = NULL */)
{
	bool bResult = true;
	DWORD dwErrCode;
	HWND hMainWnd = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (this->GetWriteMode() != LogWriteMode::WriteInstantly)
		return false;

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
		fileName.Format(Constant::File::Name::AppEventLog, stCurTime.Year(), stCurTime.Month());
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		fileName = Constant::File::Name::AppHistory;
		break;

	default:
		// Wrong argument
		TRACE_ERROR("Error: Invalid log type!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		// Show error message
		dwErrCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(hMainWnd, SM_APP_ERROR_MESSAGE, (WPARAM)dwErrCode, NULL);
		return false;
	}

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Get file path
	String filePath = StringUtils::MakeFilePath(folderPath, fileName, Constant::File::Extension::Log);

	// Check if file is opening, if not, open it
	if (fLogFile.m_hFile == CFile::hFileNull)
	{
		bResult = fLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == false) {

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

	return true;
}


/**
 * @brief	Constructor
 */
DebugLogging::DebugLogging()
{
	// Log file pointers
	m_pFileLogTraceError = NULL;
	m_pFileLogTraceDebug = NULL;
	m_pFileLogDebugInfo = NULL;

	// File exception pointers
	m_pExcLogTraceError = NULL;
	m_pExcLogTraceDebug = NULL;
	m_pExcLogDebugInfo = NULL;
}

/**
 * @brief	Destructor
 */
DebugLogging::~DebugLogging()
{
	// Release and clean-up file pointers
	ReleaseTraceErrorLogFile();
	ReleaseTraceDebugLogFile();
	ReleaseDebugInfoLogFile();

	// Clean-up file exception pointers
	if (m_pExcLogTraceError != NULL) {
		delete m_pExcLogTraceError;
		m_pExcLogTraceError = NULL;
	}
	if (m_pExcLogTraceDebug != NULL) {
		delete m_pExcLogTraceDebug;
		m_pExcLogTraceDebug = NULL;
	}
	if (m_pExcLogDebugInfo != NULL) {
		delete m_pExcLogDebugInfo;
		m_pExcLogDebugInfo = NULL;
	}
}

/**
 * @brief	Initialize trace error log file
 * @param	None
 * @return	true/false
 * @note	Destination file: TraceError.log
 * @note	To output trace error detail log strings
 */
bool DebugLogging::InitTraceErrorLogFile(void)
{
	// Verify global trace error log file pointer initialization
	VERIFY_INITIALIZATION(m_pFileLogTraceError, CFile);

	// Get trace error log file pointer
	NULL_POINTER_BREAK(m_pFileLogTraceError, return false);

	// Log folder path
	String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, Constant::File::Name::TraceError, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (m_pFileLogTraceError->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!m_pFileLogTraceError->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG ullFileSize = m_pFileLogTraceError->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			m_pFileLogTraceError->Close();

			// Step2: Rename file extension to BAK
			if (!BackupOldLogFile(strFilePath, Constant::File::Name::TraceError))
				return false;

			// Step3: Create new file and reopen
			continue;
		}
	}

	return true;
}

/**
 * @brief	Release trace error log file
 * @param	None
 * @return	None
 * @note	Destination file: TraceError.log
 * @note	To output trace error detail log strings
 */
void DebugLogging::ReleaseTraceErrorLogFile(void)
{
	// Clean up trace error log file pointer
	if (m_pFileLogTraceError != NULL) {

		// Close file if is opening
		if (m_pFileLogTraceError->m_hFile != CFile::hFileNull) {
			m_pFileLogTraceError->Flush();
			m_pFileLogTraceError->Close();
		}
		delete m_pFileLogTraceError;
		m_pFileLogTraceError = NULL;
	}
}

/**
 * @brief	Initialize trace debug log file
 * @param	None
 * @return	true/false
 * @note	Destination file: TraceDebug.log
 * @note	To output trace debug log strings (including the function name, code file and line where it failed)
 */
bool DebugLogging::InitTraceDebugLogFile(void)
{
	// Verify global trace debug log file pointer initialization
	VERIFY_INITIALIZATION(m_pFileLogTraceDebug, CFile);

	// Get trace debug log file pointer
	NULL_POINTER_BREAK(m_pFileLogTraceDebug, return false);

	// Log folder path
	String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, Constant::File::Name::TraceDebug, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (m_pFileLogTraceDebug->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!m_pFileLogTraceDebug->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG ullFileSize = m_pFileLogTraceDebug->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			m_pFileLogTraceDebug->Close();

			// Step2: Rename file extension to BAK
			if (!BackupOldLogFile(strFilePath, Constant::File::Name::TraceDebug))
				return false;

			// Step3: Create new file and reopen
			continue;
		}
	}

	return true;
}

/**
 * @brief	Release trace debug log file
 * @param	None
 * @return	None
 * @note	Destination file: TraceDebug.log
 * @note	To output trace debug log strings (including the function name, code file and line where it failed)
 */
void DebugLogging::ReleaseTraceDebugLogFile(void)
{
	// Clean up trace debug info log file pointer
	if (m_pFileLogTraceDebug != NULL) {

		// Close file if is opening
		if (m_pFileLogTraceDebug->m_hFile != CFile::hFileNull) {
			m_pFileLogTraceDebug->Flush();
			m_pFileLogTraceDebug->Close();
		}
		delete m_pFileLogTraceDebug;
		m_pFileLogTraceDebug = NULL;
	}
}

/**
 * @brief	Initialize debug info log file
 * @param	None
 * @return	true/false
 * @note	Destination file: DebugInfo.log
 * @note	To output debug info log strings (similar to OutputDebugString, but output to file instead)
 */
bool DebugLogging::InitDebugInfoLogFile(void)
{
	// Verify global debug info log file pointer initialization
	VERIFY_INITIALIZATION(m_pFileLogDebugInfo, CFile);

	// Get debug info log file pointer
	NULL_POINTER_BREAK(m_pFileLogDebugInfo, return false);

	// Log folder path
	String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Log file path
	String strFilePath = StringUtils::MakeFilePath(strFolderPath, Constant::File::Name::DebugInfo, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (m_pFileLogDebugInfo->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!m_pFileLogDebugInfo->Open(strFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText | CFile::shareDenyWrite)) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			AppCore::ShowErrorMessage(NULL, NULL, dwErrorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG ullFileSize = m_pFileLogDebugInfo->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			m_pFileLogDebugInfo->Close();

			// Step2: Rename file extension to BAK
			if (!BackupOldLogFile(strFilePath, Constant::File::Name::DebugInfo))
				return false;

			// Step3: Create new file and reopen
			continue;
		}
	}

	return true;
}

/**
 * @brief	Release debug info log file
 * @param	None
 * @return	None
 * @note	Destination file: DebugInfo.log
 * @note	To output debug info log strings (similar to OutputDebugString, but output to file instead)
 */
void DebugLogging::ReleaseDebugInfoLogFile(void)
{
	// Clean up debug info log file pointer
	if (m_pFileLogDebugInfo != NULL) {

		// Close file if is opening
		if (m_pFileLogDebugInfo->m_hFile != CFile::hFileNull) {
			m_pFileLogDebugInfo->Flush();
			m_pFileLogDebugInfo->Close();
		}
		delete m_pFileLogDebugInfo;
		m_pFileLogDebugInfo = NULL;
	}
}

/**
 * @brief	Backup old log file
 * @param	filePath	- File path (in/out)
 * @param	logFileName	- Log file name
 * @return	true/false
 */
bool DebugLogging::BackupOldLogFile(const String& filePath, const wchar_t* logFileName)
{
	CFileFind Finder;

	// If file path is not specified, do nothing
	if (filePath.IsEmpty()) return false;

	// Log folder path
	String folderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);

	// Search for backup file list
	for (int nNum = 0; nNum < Constant::Max::BackupFileNumber; nNum++) {

		// Make backup file path template
		String filePathTemp = StringUtils::MakeFilePath(folderPath, logFileName, Constant::File::Extension::Backup_Log);
		if (filePathTemp.IsEmpty())
			return false;

		// Format backup file path
		String bakFilePath;
		bakFilePath.Format(filePathTemp, nNum);

		// Check if file has already existed
		if (Finder.FindFile(bakFilePath) == true) {

			// If backup file number exceeded the limit, can not backup more
			if (nNum == (Constant::Max::BackupFileNumber - 1)) return false;
			else continue;
		}

		// Rename file
		CFile::Rename(filePath, bakFilePath);
		break;
	}

	return true;
}

/**
 * @brief	Write trace error log string to file
 * @param	logStringW	- Log string
 * @return	None
 * @note	Destination file: TraceError.log
 * @note	To output trace error detail log strings
 */
void DebugLogging::WriteTraceErrorLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.Hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, currentDateTime.Year(), currentDateTime.Month(), currentDateTime.Day(),
		currentDateTime.Hour(), currentDateTime.Minute(), currentDateTime.Second(), currentDateTime.Millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty())
		return;

	// If the file is not initialized or had been released
	if (GetTraceErrorLogFile() == NULL) {
		if (!InitTraceErrorLogFile())
			return;
	}

	// Re-acquire trace log file pointer
	CFile* pTraceErrorLogFile = GetTraceErrorLogFile();
	NULL_POINTER_BREAK(pTraceErrorLogFile, return NOTHING);
	{
		// Write log string to file
		pTraceErrorLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pTraceErrorLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceErrorLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceErrorLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath.GetString(), Constant::File::Name::TraceError, Constant::File::Extension::Log);
			if (!BackupOldLogFile(strOrgFilePath, Constant::File::Name::TraceError))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseTraceErrorLogFile();
			return;
		}
	}
}

/**
 * @brief	Write trace debug log string to file
 * @param	logStringW	- Log string
 * @return	None
 * @note	Destination file: TraceDebug.log
 * @note	To output trace debug log strings (including the function name, code file and line where it failed)
 */
void DebugLogging::WriteTraceDebugLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.Hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, currentDateTime.Year(), currentDateTime.Month(), currentDateTime.Day(),
		currentDateTime.Hour(), currentDateTime.Minute(), currentDateTime.Second(), currentDateTime.Millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty()) return;

	// If the file is not initialized or had been released
	if (GetTraceDebugLogFile() == NULL) {
		if (!InitTraceDebugLogFile())
			return;
	}

	// Re-acquire trace debug log file pointer
	CFile* pTraceDebugLogFile = GetTraceDebugLogFile();
	NULL_POINTER_BREAK(pTraceDebugLogFile, return NOTHING);
	{
		// Write log string to file
		pTraceDebugLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pTraceDebugLogFile->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pTraceDebugLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pTraceDebugLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath, Constant::File::Name::TraceDebug, Constant::File::Extension::Log);
			if (!BackupOldLogFile(strOrgFilePath, Constant::File::Name::TraceDebug))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseTraceDebugLogFile();
			return;
		}
	}
}

/**
 * @brief	Write debug info output log string to file
 * @param	lpszLogStringW	- Log string
 * @return	None
 * @note	Destination file: DebugInfo.log
 * @note	To output debug info log strings (similar to OutputDebugString, but output to file instead)
 */
void DebugLogging::WriteDebugInfoLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.Hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, currentDateTime.Year(), currentDateTime.Month(), currentDateTime.Day(),
		currentDateTime.Hour(), currentDateTime.Minute(), currentDateTime.Second(), currentDateTime.Millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.IsEmpty()) return;

	// If the file is not initialized or had been released
	if (GetDebugInfoLogFile() == NULL) {
		if (!InitDebugInfoLogFile())
			return;
	}

	// Re-acquire debug info log file pointer
	CFile* pDebugInfoLogFile = GetDebugInfoLogFile();
	NULL_POINTER_BREAK(pDebugInfoLogFile, return NOTHING);
	{
		// Write log string to file
		pDebugInfoLogFile->Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		pDebugInfoLogFile->Flush();
	}

	// Recheck file size after writing
	{
		// Go to end of file
		ULONGLONG ullFileSize = pDebugInfoLogFile->SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			pDebugInfoLogFile->Close();

			// Step2: Rename file extension to BAK
			String strFolderPath = StringUtils::GetSubFolderPath(Constant::Folder::Log);
			String strOrgFilePath = StringUtils::MakeFilePath(strFolderPath, Constant::File::Name::DebugInfo, Constant::File::Extension::Log);
			if (!BackupOldLogFile(strOrgFilePath, Constant::File::Name::DebugInfo))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			ReleaseDebugInfoLogFile();
			return;
		}
	}
}

/**
 * @brief	Write trace and debug log string to file
 * @param	lpszFileName	- Log file name
 * @param	lpszLogStringW	- Log string
 * @return	None
 * @note	Base function - No longer used
 */
void DebugLogging::WriteTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW)
{
	// Log file path
	String filePath = StringUtils::MakeFilePath(Constant::Folder::Log, fileName, Constant::File::Extension::Log);

	CFile fTrcDbgLogFile;

	// Check if file is opening, if not, open it
	while (fTrcDbgLogFile.m_hFile == CFile::hFileNull) {

		bool bResult = fTrcDbgLogFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::typeText);
		if (bResult == false) {
			// Show error message
			DWORD dwErrorCode = GetLastError();
			LPARAM lParam = reinterpret_cast<LPARAM>(fileName);
			ShowErrorMessage(NULL, NULL, dwErrorCode, lParam);
			return;
		}

		// Go to end of file
		ULONGLONG ullFileSize = fTrcDbgLogFile.SeekToEnd();

		// If the file line number is already out of limit
		if (ullFileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			fTrcDbgLogFile.Close();

			// Step2: Rename file extension to BAK
			CFileFind Finder;
			String backupFilePath;
			for (int nNum = 0; nNum < Constant::Max::BackupFileNumber; nNum++) {
				backupFilePath.Format((filePath + Constant::File::Extension::Backup_Log), nNum);
				if (Finder.FindFile(backupFilePath) == true) {
					if (nNum == (Constant::Max::BackupFileNumber - 1)) return;
					else continue;
				}
				CFile::Rename(filePath, backupFilePath);
				break;
			}

			// Step3: Create new file and reopen
			continue;
		}
	}

	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::GetCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.Hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::StringFormat(templateFormat, currentDateTime.Year(), currentDateTime.Month(), currentDateTime.Day(),
		currentDateTime.Hour(), currentDateTime.Minute(), currentDateTime.Second(), currentDateTime.Millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::LoadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::StringFormat(templateFormat, timeFormatString.GetString(), logStringW, Constant::String::Empty);

	if (!logOutputFormatString.IsEmpty()) {
		// Write log string to file
		fTrcDbgLogFile.Write(logOutputFormatString, logOutputFormatString.GetLength() * sizeof(wchar_t));
		fTrcDbgLogFile.Flush();
	}

	// Close file after done writing
	if (fTrcDbgLogFile.m_hFile != CFile::hFileNull) {
		fTrcDbgLogFile.Close();
	}
}

/**
 * @brief	Output exception/error trace log string to log file
 * @param	traceLogA - Output trace log string (ANSI)
 * @return	None
 */
void DebugLogging::TraceError(const char* traceLogA)
{
	// Convert ANSI string to UNICODE
	const wchar_t* traceLogW = MAKEUNICODE(traceLogA);
	TraceError(traceLogW);
}

/**
 * @brief	Output exception/error trace log string to log file
 * @param	traceLogW - Output trace log string (Unicode)
 * @return	None
 */
void DebugLogging::TraceError(const wchar_t* traceLogW)
{
	// Write trace log file: TraceError.log
	WriteTraceErrorLogFile(traceLogW);
}

/**
 * @brief	Format and output exception/error trace log string to log file
 * @param	lpszTraceLogFormatA - Trace log format string (ANSI)
 * @param	...				    - Same as default MFC Format function
 * @return	None
 */
void DebugLogging::TraceErrorFormat(const char* traceLogFormatA, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatA));

	// Format source string (ANSI)
	CStringA strLogFormatA;

	va_list argList;
	va_start(argList, traceLogFormatA);
	strLogFormatA.FormatV(traceLogFormatA, argList);
	va_end(argList);

	// Output trace log
	TraceError(strLogFormatA);
}

/**
 * @brief	Format and output exception/error trace log string to log file
 * @param	lpszTraceLogFormatW - Trace log format string (Unicode)
 * @param	...				    - Same as default MFC Format function
 * @return	None
 */
void DebugLogging::TraceErrorFormat(const wchar_t* traceLogFormatW, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatW));

	// Format source string (Unicode)
	String logFormatStringW;

	va_list argList;
	va_start(argList, traceLogFormatW);
	logFormatStringW.FormatV(traceLogFormatW, argList);
	va_end(argList);

	// Output trace log
	TraceError(logFormatStringW);
}

/**
 * @brief	Output debug trace information log
 * @param	lpszFuncName - Code function name
 * @param	lpszFileName - Code file name
 * @param	nLineIndex	 - Code line number
 * @return	None
 */
void DebugLogging::TraceDebugInfo(const char* funcName, const char* fileName, int lineIndex)
{
	// Debug trace info
	const wchar_t* _funcName = MAKEUNICODE(funcName);
	const wchar_t* _fileName = MAKEUNICODE(fileName);

	// Format debug trace log
	String debugTraceFormat = StringUtils::StringFormat(_T("Function: %s, File: %s(%d)"), _funcName, _fileName, lineIndex);

	// Write debug trace log: TraceDebug.log
	WriteTraceDebugLogFile(debugTraceFormat.GetString());
}

/**
 * @brief	Output debug log string
 * @param	debugLog	- Debug log string (Unicode)
 * @param	forceOutput - Force output target
 * @return	None
 */
void DebugLogging::OutputDebugLog(const wchar_t* debugLog, int forceOutput /* = INT_INVALID */)
{
	// Get debug mode enable state
	bool bDebugModeEnable = GetDebugMode();

	// Get debug log string
	String debugLogStr = debugLog;

	// Get DebugTest tool dialog handle
	HWND hDebugTestWnd = FindDebugTestDlg();

	// Debug log output target
	int nDebugOutputTarget = forceOutput;
	if (nDebugOutputTarget == INT_INVALID) {
		nDebugOutputTarget = GetDebugOutputTarget();
	}
	if ((hDebugTestWnd != NULL) &&
		(IsWindowVisible(hDebugTestWnd))) {
		// Force enable debug mode and
		// prefer output target to DebugTest tool if it's displaying
		bDebugModeEnable = true;
		nDebugOutputTarget = DebugTestTool;
	}

	// If debug mode not enabled, do nothing
	if (bDebugModeEnable == false)
		return;

	// Output debug string
	if (nDebugOutputTarget == DefaultOutput) {
		// Default output target: OutputDebugString
		// Debug strings can be watched by using VS Output screen or DebugView tool
		OutputDebugString(debugLogStr);
	}
	else if (nDebugOutputTarget == DebugInfoFile) {
		// Ouput debug log to file: DebugInfo.log
		GetDebugLogger().WriteDebugInfoLogFile(debugLogStr);
	}
	else if (nDebugOutputTarget == DebugTestTool) {
		// Output debug log to DebugTest tool
		if (hDebugTestWnd == NULL) return;
		WPARAM wParam = MAKE_WPARAM_STRING(debugLogStr);
		LPARAM lParam = MAKE_LPARAM_STRING(debugLogStr);
		SendMessage(hDebugTestWnd, SM_APP_DEBUG_OUTPUT, wParam, lParam);
	}
}

/**
 * @brief	Output debug log string format
 * @param	debugLogFormat - Debug log format string (Unicode)
 * @param	args		   - Argument list
 * @return	None
 */
void DebugLogging::OutputDebugLogFormat(const wchar_t* debugLogFormat, va_list args)
{
	// Format source string
	String logFormatString;
	logFormatString.FormatV(debugLogFormat, args);

	// Output debug string
	OutputDebugLog(logFormatString);
}

/**
 * @brief	Output debug string format (combined version of String.Format and the default OutputDebugString function)
 * @param	debugStringFormat - Debug log format string (Unicode)
 * @param	args			  - Argument list
 * @return	None
 */
void DebugLogging::OutputDebugStringFormat(const wchar_t* debugStringFormat, va_list args)
{
	// Format source string
	String logDebugStringFormat;
	logDebugStringFormat.FormatV(debugStringFormat, args);

	// Output debug string
	OutputDebugString(logDebugStringFormat);
}
