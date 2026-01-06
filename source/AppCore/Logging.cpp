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
 * @param	dataType - Data type
 * @return	size_t
 */
size_t getSizeByType(byte dataType)
{
	size_t retSize = 0;
	switch (dataType)
	{
	case LogDataType::Void:				// No type (unusable)
		retSize = kNullInteger;
		break;

	case LogDataType::Num_U1:			// Unsigned integer (1-byte)
	case LogDataType::Num_I1:			// Signed integer (1-byte)
	case LogDataType::Num_F1:			// Float number (1-byte)
		retSize = sizeof(UCHAR);
		break;

	case LogDataType::Num_U2:			// Unsigned integer (2-byte)
	case LogDataType::Num_I2:			// Signed integer (2-byte)
	case LogDataType::Num_F2:			// Float number (2-byte)
		retSize = sizeof(uint16);
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
		retSize = kNullInteger;
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
	categoryId_ = kNullInteger;									// Detail category
	flag_ = LogDetailFlag::Flag_Null;						// Detail info flag
	detailValue_ = kNullInteger;								// Detail value (integer)
	detailInfoString_.empty();								// Detail info (string)
	detailDataPtr_ = NULL;									// Detail data (pointer)
	pointerType_ = LogDataType::Void;						// Detail info pointer data type
	pointerSize_ = kNullInteger;								// Detail info pointer data size
}

/**
 * @brief	Initialize/reset item data
 * @param	None
 * @return	Nones
 */
void LogDetail::init(void) noexcept
{
	// Initialization
	categoryId_ = kNullInteger;									// Detail category
	flag_ = LogDetailFlag::Flag_Null;						// Detail info flag
	detailValue_ = kNullInteger;								// Detail value (integer)
	detailInfoString_.empty();								// Detail info (string)
	detailDataPtr_ = NULL;									// Detail data (pointer)
	pointerType_ = LogDataType::Void;						// Detail info pointer data type
	pointerSize_ = kNullInteger;								// Detail info pointer data sizesize
}

/**
 * @brief	Copy data from another item
 * @param	other - Pointer of input item
 * @return	None
 */
void LogDetail::copy(const LogDetail& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	categoryId_ = other.categoryId_;						// Detail category
	flag_ = other.flag_;									// Detail flag
	detailValue_ = other.detailValue_;						// Detail value (integer)
	detailInfoString_ = other.detailInfoString_;			// Detail info (string)
	copyPointer(other);										// Detail data (pointer)
}

/**
 * @brief	Copy detail info pointer
 * @param	other - Pointer of input item
 * @return	None
 */
void LogDetail::copyPointer(const LogDetail& other)
{
	// Copy pointer properties
	pointerType_ = other.pointerType_;					// Detail info pointer data type
	pointerSize_ = other.pointerSize_;					// Detail info pointer data size

	// Copy pointer data
	memcpy(detailDataPtr_, other.detailDataPtr_, other.pointerSize_);
}

/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogDetail::compare(const LogDetail& other) const
{
	bool returnFlag = false;

	// Compare items
	returnFlag &= (categoryId_ == other.categoryId_);				// Detail category
	returnFlag &= (flag_ == other.flag_);							// Detail flag
	returnFlag &= (detailValue_ == other.detailValue_);				// Detail value (integer)
	returnFlag &= (detailInfoString_ == other.detailInfoString_);	// Detail info (string)
	returnFlag &= comparePointer(other);							// Detail data (pointer)

	return returnFlag;
}

/**
 * @brief	Compare detail info pointers
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogDetail::comparePointer(const LogDetail& other) const
{
	bool returnFlag = false;

	// Compare properties
	returnFlag &= (pointerType_ == other.pointerType_);			// Detail info pointer data type
	returnFlag &= (pointerSize_ == other.pointerSize_);			// Detail info pointer data size

	// Only compare pointer values if properties are matching
	if (returnFlag != false) {
		returnFlag &= memcmp(detailDataPtr_, other.detailDataPtr_, pointerSize_);
	}

	return returnFlag;
}

/**
 * @brief	Check if current detail info data is empty
 * @param	None
 * @return	true/false
 */
bool LogDetail::isEmpty(void) const noexcept
{
	// Initialize empty detail info
	static const LOGDETAIL logDummyDetail;

	// Compare with that data and return result
	return compare(logDummyDetail);
}

/**
 * @brief	Set detail info pointer data
 * @param	dataBuff	- Data buffer (pointer)
 * @param	dataType	- Data type
 * @param	dataSize	- Data size
 * @return	true/false
 */
bool LogDetail::setPointerData(void* dataBuff, byte dataType /* = DATA_TYPE_UNSPECIFIED */, size_t dataSize /* = 0 */)
{
	// If data type is void (unusable), do nothing
	if (dataType == LogDataType::Void)
		return false;

	// If both data type and size are not specified, do nothing
	if ((dataType == LogDataType::Unspecified) && (dataSize == 0))
		return false;

	// If size is not specified,
	if (dataSize == 0) {
		// Get size by data type
		dataSize = getSizeByType(dataType);

		// Get size failed, do nothing
		if (dataSize == 0)
			return false;
	}

	// Otherwise, set normally
	pointerType_ = dataType;
	pointerSize_ = dataSize;
	memcpy(detailDataPtr_, dataBuff, dataSize);
	return true;
}

/**
 * @brief	Add log detail info item
 * @param	logDetail	  - Log detail item
 * @param	categoryId	  - Detail category
 * @param	detailValue	  - Detail info (integer)
 * @param	detailInfo	  - Detail info (string)
 * @param	flag		  - Detail flag
 * @return	None
 */
void LogDetailInfo::addDetail(uint16 categoryId, int detailValue, int flag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.setCategory(categoryId);
	logDetail.setDetailValue(detailValue);
	logDetail.setFlag(flag);

	// Add detail info item
	addDetail(logDetail);
}

void LogDetailInfo::addDetail(uint16 categoryId, const wchar_t* detailInfo, int flag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.setCategory(categoryId);
	logDetail.setDetailString(detailInfo);
	logDetail.setFlag(flag);

	// Add detail info item
	addDetail(logDetail);
}

void LogDetailInfo::addDetail(uint16 categoryId, int detailValue, const wchar_t* detailInfo, int flag /* = LogDetailFlag::Flag_Null */)
{
	// Prepare detail info item
	LOGDETAIL logDetail;
	logDetail.setCategory(categoryId);
	logDetail.setDetailValue(detailValue);
	logDetail.setDetailString(detailInfo);
	logDetail.setFlag(flag);

	// Add detail info item
	addDetail(logDetail);
}

/**
 * @brief	Constructor
 */
LogItem::LogItem()
{
	// Initialization
	timeValue_ = DateTime();							// Log time
	processId_ = kNullInteger;								// Process ID
	categoryId_ = LOG_MACRO_NONE;						// Log category
	logString_ = Constant::String::Empty;				// Log string
	detailInfo_.clear();								// Log detail info
}

/**
 * @brief	Copy data from another log item
 * @param	other - Pointer of input item
 * @return	None
 */
void LogItem::copy(const LogItem& other) noexcept
{
	// Copy data
	timeValue_ = other.timeValue_;						// Log time
	processId_ = other.processId_;						// Process ID
	categoryId_ = other.categoryId_;					// Log category
	logString_ = other.logString_;						// Log string
	detailInfo_ = other.detailInfo_;					// Log detail info
}

/**
 * @brief	Compare with another given item
 * @param	other - Pointer of given item
 * @return	true/false
 */
bool LogItem::compare(const LogItem& other) const noexcept
{
	bool returnFlag = false;

	// Compare item
	returnFlag &= (timeValue_ == other.timeValue_);
	returnFlag &= (processId_ == other.processId_);
	returnFlag &= (categoryId_ == other.categoryId_);
	returnFlag &= (logString_ == other.logString_);

	// Compare log detail info
	bool detailCompareResult = true;
	if (detailInfo_.size() != other.detailInfo_.size()) {
		detailCompareResult = false;
	}
	else {
		for (int index = 0; index < detailInfo_.size(); index++) {
			if (detailInfo_.at(index).compare(other.detailInfo_.at(index)) != true) {
				detailCompareResult = false;
				break;
			}
		}
	}
	returnFlag &= detailCompareResult;

	return returnFlag;
}

/**
 * @brief	Check if current log item is empty
 * @param	None
 * @return	true/false
 */
bool LogItem::isEmpty(void) const noexcept
{
	// Initialize an empty item
	static const LOGITEM logDummyItem;

	// Compare with this item and return result
	return compare(logDummyItem);
}

/**
 * @brief	Remove all log item data
 * @param	None
 * @return	None
 */
void LogItem::removeAll(void) noexcept
{
	// Reset data
	timeValue_ = DateTime();								// Log time
	processId_ = kNullInteger;									// Process ID
	categoryId_ = LOG_MACRO_NONE;							// Log category
	logString_ = Constant::String::Empty;					// Log string

	// Clean up log detail info data
	removeDetailInfo();								// Log detail info
}

/**
 * @brief	Return a formatted logitem date/time string
 * @param	None
 * @return	String - Formatted result
 */
String LogItem::formatDateTime(void) const
{
	const wchar_t* middayFlag = (timeValue_.hour() >= 12) ? Constant::Symbol::PostMeridiem : Constant::Symbol::AnteMeridiem;
	String templateFormatStr = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::stringFormat(templateFormatStr, timeValue_.year(), timeValue_.month(), timeValue_.day(),
		timeValue_.hour(), timeValue_.minute(), timeValue_.second(), timeValue_.millisecond(), middayFlag);

	return timeFormatString;
}

/**
 * @brief	Return formatted output log string for file writing
 * @param	None
 * @return	String - Formatted result
 */
String LogItem::formatOutput(void) const
{
	// Create JSON data object
	JSONDATA jsonData;

	String logKey;
	String logValue;

	// Load default language table package
	LANGTABLE_PTR defaultLanguagePtr = loadLanguageTable(NULL);

	/*********************************************************************/
	/*																	 */
	/*			  Convert log item base info into JSON data				 */
	/*																	 */
	/*********************************************************************/

	// Log time
	logKey = getString(StringTable::LogKey, BaseLog::Time);
	jsonData.addString(logKey, formatDateTime());

	// Process ID
	logKey = getString(StringTable::LogKey, BaseLog::PID);
	jsonData.addInteger(logKey, processId_);

	// Log category
	logKey = getString(StringTable::LogKey, BaseLog::LogCategory);
	logValue = getLanguageString(defaultLanguagePtr, categoryId_);
	jsonData.addString(logKey, logValue);

	// Log description string
	logKey = getString(StringTable::LogKey, BaseLog::Description);
	jsonData.addString(logKey, logString_);

	/*********************************************************************/
	/*																	 */
	/*		  Convert log item detail info data into JSON data			 */
	/*																	 */
	/*********************************************************************/

	// Log detail info
	if (!detailInfo_.empty()) {

		// Create JSON detail data object
		JSONDATA jsonDetailData;

		String detailKey;
		String detailValue;

		// Set object name: Details
		jsonDetailData.setObjectName(getString(StringTable::LogKey, BaseLog::Details));

		// Convert data
		for (int index = 0; index < (detailInfo_.size()); index++) {

			// Get detail info item
			LOGDETAIL logDetail = detailInfo_.at(index);

			// Skip if detail item is read-only
			int detailFlag = logDetail.getFlag();
			if (detailFlag & LogDetailFlag::ReadOnly_Data)
				continue;

			// NULL flag --> apply default flags
			if (detailFlag == LogDetailFlag::Flag_Null) {
				detailFlag = LogDetailFlag::Write_Int;
			}

			// Detail info category
			detailKey = getString(StringTable::LogKey, logDetail.getCategory());

			// Detail info value
			int detailValueInt = logDetail.getDetailValue();
			if (detailFlag & LogDetailFlag::Write_Int) {
				jsonDetailData.addInteger(detailKey, logDetail.getDetailValue());
			}
			else if (detailFlag & LogDetailFlag::LookUp_Dict) {
				detailValue = getString(StringTable::LogValue, detailValueInt);
				jsonDetailData.addString(detailKey, detailValue);
			}
			else if (detailFlag & LogDetailFlag::Write_String) {
				jsonDetailData.addString(detailKey, logDetail.getDetailString());
			}
			else if (detailFlag & (LogDetailFlag::Write_Int & LogDetailFlag::Write_String)) {
				JSONDATA jsonSubDetail;
				detailKey = getString(StringTable::LogKey, BaseLog::DetailNumeric);
				jsonSubDetail.addInteger(detailKey, logDetail.getDetailValue());
				detailKey = getString(StringTable::LogKey, BaseLog::DetailString);
				jsonSubDetail.addString(detailKey, logDetail.getDetailString());
				jsonDetailData.addChildObject(&jsonSubDetail);
			}
		}

		// Output detail JSON data
		jsonData.addChildObject(&jsonDetailData);
	}

	/*********************************************************************/
	/*																	 */
	/*				  Output JSON data under YAML format				 */
	/*																	 */
	/*********************************************************************/

	String logYAMLFormat;
	jsonData.printYAML(logYAMLFormat, 0);
	logYAMLFormat.append(Constant::String::NewLine);

	return logYAMLFormat;
}

/**
 * @brief	Constructor
 */
JSON::JSON()
{
	// Initialization
	objectName_ = Constant::String::Empty;		// JSON object name
	keyValuePairs_.clear();						// Key-value pairs
	childObjCount_ = 0;							// Number of child objects
	childObjList_ = NULL;							// List of child objects
}

/**
 * @brief	Copy data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::copy(const JSON& other) noexcept
{
	// Do not copy itself
	if (this == &other) return;

	// Copy data
	objectName_ = other.objectName_;			// JSON object name
	copyArrayData(other);						// Property (array) data
	copyPtrData(other);						// Child object (pointer) data
}

/**
 * @brief	Copy object array data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::copyArrayData(const JSON& other)
{
	// Remove all existing array data
	keyValuePairs_.clear();

	// Set destination array data size
	keyValuePairs_.reserve(other.keyValuePairs_.size());

	// Copy list of key-value pairs
	for (int index = 0; index < other.keyValuePairs_.size(); index++) {
		keyValuePairs_.push_back(other.keyValuePairs_.at(index));
	}
}

/**
 * @brief	Copy object pointer data from another JSON object
 * @param	other - Pointer of input object
 * @return	None
 */
void JSON::copyPtrData(const JSON& other)
{
	// Number of child objects
	childObjCount_ = other.childObjCount_;

	// List of child objects (pointer copy)
	if ((other.childObjCount_ > 0) && (other.childObjList_ != NULL)) {

		// Allocation and initialization
		childObjList_ = new PJSONDATA[childObjCount_];
		if (childObjList_ == NULL) {
			TRACE_ERROR("Error: Destination JSON child object array allocation failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}

		// Copy data
		for (int count = 0; count < childObjCount_; count++) {

			// Allocate memory
			childObjList_[count] = new JSONDATA;
			if (childObjList_[count] == NULL) {
				TRACE_FORMAT("Error: Destination JSON child object allocation failed!!! (Index=%d)", count);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Get source data
			PJSONDATA srcObjectPtr = other.childObjList_[count];
			if (srcObjectPtr == NULL) {
				TRACE_FORMAT("Error: Invalid JSON child object is skipped when copying!!! (Index=%d)", count);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
				continue;
			}

			// Copy object data (do not use 'memcpy' in here)
			*(childObjList_[count]) = *srcObjectPtr;
		}
	}
}

/**
 * @brief	Compare with another given object
 * @param	other - Pointer of given object
 * @return	true/false
 */
bool JSON::compare(const JSON& other) const
{
	bool returnFlag = false;

	// Compare object name
	returnFlag &= (objectName_ == other.objectName_);

	// Compare detail item info
	bool detailCompareResult = true;
	if (keyValuePairs_.size() != other.keyValuePairs_.size()) {
		detailCompareResult = false;
	}
	if (detailCompareResult != false) {
		for (int index = 0; index < (keyValuePairs_.size()); index++) {
			if (keyValuePairs_.at(index) != other.keyValuePairs_.at(index)) {
				detailCompareResult = false;
				break;
			}
		}
	}
	returnFlag &= detailCompareResult;

	// Compare child objects
	if (childObjList_ != NULL && other.childObjList_ != NULL) {

		detailCompareResult = true;

		// Compare child object numbers
		int thisChildObjCount = childObjCount_;
		int otherChildObjCount = other.childObjCount_;
		if (thisChildObjCount != otherChildObjCount) {
			detailCompareResult = false;
		}
		else {
			// Compare each child object data
			for (int count = 0; count < thisChildObjCount; count++) {
				PJSONDATA thisChildObjPtr = childObjList_[count];
				PJSONDATA otherChildObjPtr = other.childObjList_[count];
				if ((thisChildObjPtr != NULL) && (otherChildObjPtr != NULL)) {
					detailCompareResult &= thisChildObjPtr->compare(*otherChildObjPtr);
				}
				else {
					detailCompareResult = false;
					break;
				}
			}
		}
		returnFlag &= detailCompareResult;
	}

	return returnFlag;
}

/**
 * @brief	Check if current JSON object is empty
 * @param	None
 * @return	true/false
 */
bool JSON::isEmpty(void) const noexcept
{
	// Initialize an empty item
	static const JSONDATA jsonDummyItem;

	// Compare with this item and return result
	return compare(jsonDummyItem);
}

/**
 * @brief	Remove property by its key name
 * @param	keyName - Key name
 * @return	None
 */
void JSON::removeProperty(const wchar_t* keyName)
{
	// If property data is empty, do nothing
	if (keyValuePairs_.empty())
		return;

	// Search for key name
	int foundIndex = kInvalidInteger;
	for (int index = 0; index < (keyValuePairs_.size()); index++) {
		if (keyValuePairs_.at(index).key == keyName) {
			foundIndex = index;
			break;
		}
	}

	// Remove property by index
	removeProperty(foundIndex);
}

/**
 * @brief	Remove all JSON object data
 * @param	None
 * @return	None
 */
void JSON::removeAll(void)
{
	// Reset data
	objectName_.empty();					// JSON object name
	keyValuePairs_.clear();				// Key-value pairs

	// Remove all child objects
	if ((childObjCount_ > 0) && (childObjList_ != NULL)) {
		for (int count = 0; count < childObjCount_; count++) {
			PJSONDATA childObjPtr = childObjList_[count];
			if (childObjPtr != NULL) {
				childObjPtr->removeAll();
				delete childObjPtr;
			}
		}
		delete[] (childObjList_);
		childObjList_ = NULL;
		
		// Reset child object counter
		childObjCount_ = 0;
	}
}

/**
 * @brief	Add a string-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Key name
 * @param	value	- String value
 * @return	None
 */
void JSON::addString(const wchar_t* keyName, const wchar_t* value)
{
	// Search if key name already existed
	for (int index = 0; index < (keyValuePairs_.size()); index++) {
		JSON_ENTRY& jsonEntry = keyValuePairs_.at(index);
		if (jsonEntry.key == keyName) {
			// Replace existed value with new value
			jsonEntry.value = value;
			return;
		}
	}

	// Add property
	keyValuePairs_.push_back({ keyName, value });
}

/**
 * @brief	Add an integer-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Item name
 * @param	value	- Signed integer value
 * @return	None
 */
void JSON::addInteger(const wchar_t* keyName, int value)
{
	// Convert integer to string
	String valueStr = StringUtils::stringFormat(_T("%d"), value);

	// Add property
	addString(keyName, valueStr);
}

/**
 * @brief	Add a float-typed key-value pair, update value if
					the given key name already existed
 * @param	keyName - Key name
 * @param	value	- Float value
 * @return	None
 */
void JSON::addFloat(const wchar_t* keyName, double value)
{
	// Convert float number to string
	String valueStr = StringUtils::stringFormat(_T("%f"), value);

	// Add property
	addString(keyName, valueStr);
}

/**
 * @brief	Add a child object
 * @param	objPtr - Source item data (pointer)
 * @return	None
 */
void JSON::addChildObject(JSON* objPtr)
{
	// Check for data validity
	if (objPtr == NULL)
		return;

	// Index to copy
	size_t index = 0;

	// Allocate child object array data memory if not yet allocated
	if (childObjList_ == NULL) {
		childObjList_ = new PJSONDATA;
		if (childObjList_ == NULL) {
			TRACE_ERROR("Error: JSON child object array data allocation failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	else {
		index = childObjCount_;
	}
	
	// Allocated destination child object memory
	childObjList_[index] = new JSONDATA;
	if (childObjList_[index] == NULL) {
		TRACE_FORMAT("Error: JSON new child object data allocation failed!!! (Index=%d)", index);
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Copy child object data (do not use 'memcpy' in here)
	*(childObjList_[index]) = *objPtr;

	// Increase child object counter
	childObjCount_++;
}

/**
 * @brief	Print JSON object data with indentation
 * @param	outputString  - Output printed result string
 * @param	indent		  - Indentation
 * @param	hasSeparator    - Whether to add a blank line as separator
 * @param	isMultiline    - Whether to print the data in multiple lines
 * @return	None
 */
void JSON::print(String& outputString, int indent, bool hasSeparator, bool isMultiline /* = true */) const
{
	// Empty output result string
	outputString.empty();

	// Make indentation
	String indentationStr = Constant::String::Empty;
	for (int tabCount = 1; tabCount <= indent; tabCount++) {
		// Add indent (tab character)
		indentationStr.append(Constant::Symbol::JSON_Indent);
	}

	// Do not use indentation if printing in single line
	// This will make better visualization
	if (isMultiline != true) {
		indentationStr.empty();
	}

	// Add indentation
	outputString.append(indentationStr);

	String formatStr = Constant::String::Empty;

	// Print object name (if set)
	if (!objectName_.isEmpty()) {
		formatStr.format(_T("\"%s\": "), objectName_.getString());
		outputString.append(formatStr);
	}

	// Opening bracket
	outputString.append(_T("{ "));
	if (isMultiline == true) {
		outputString.append(Constant::String::EndLine);
	}

	// Print list of properties
	size_t itemNum = keyValuePairs_.size();
	for (int index = 0; index < itemNum; index++) {

		// Add indentation
		outputString.append(indentationStr);

		// Get key and value
		const JSON_ENTRY& jsonEntry = keyValuePairs_.at(index);

		// Format properties
		if ((index == (itemNum - 1)) &&
			((childObjCount_ <= 0) || (childObjList_ == NULL))) {

			// Last property (no other child object following) has no comma in the end
			formatStr.format(_T("\t\"%s\": \"%s\" "), jsonEntry.key.getString(), jsonEntry.value.getString());
			outputString.append(formatStr);
			if (isMultiline == true) {
				outputString.append(Constant::String::EndLine);
			}
		}
		else {
			// Add comma character at the end of each property
			formatStr.format(_T("\t\"%s\": \"%s\", "), jsonEntry.key.getString(), jsonEntry.value.getString());
			outputString.append(formatStr);
			if (isMultiline == true) {
				outputString.append(Constant::String::EndLine);
			}
		}
	}

	// Print child objects
	String subItemOutput = Constant::String::Empty;
	if ((childObjCount_ > 0) && (childObjList_ != NULL)) {
		for (int count = 0; count < childObjCount_; count++) {
			PJSONDATA subItemPtr = childObjList_[count];
			if (subItemPtr != NULL) {
				subItemPtr->print(subItemOutput, indent + 1, false, isMultiline);
				outputString.append(subItemOutput);
			}
		}
	}

	// Add indentation and closing bracket
	outputString.append(indentationStr);
	outputString.append(_T("} "));
	if (isMultiline == true) {
		outputString.append(Constant::String::EndLine);
	}

	// Add a blank line as separator
	if (hasSeparator == true) {
		outputString.append(Constant::String::EndLine);
	}
}

/**
 * @brief	Print JSON object data in YAML format
 * @param	outputString  - Output printed result string
 * @param	indent		  - Indentation
 * @return	None
 */
void JSON::printYAML(String& outputString, int indent) const
{
	// Empty output result string
	outputString.empty();

	// Indentation
	String indentationStr = Constant::String::Empty;
	for (int count = 1; count < indent; count++) {
		indentationStr.append(Constant::Symbol::YAML_Indent);
	}

	String formatStr = Constant::String::Empty;

	// Print object name (if set)
	if (!objectName_.isEmpty()) {
		formatStr = StringUtils::stringFormat(_T("%s%s:\n"), indentationStr.getString(), objectName_.getString());
		outputString.append(formatStr);
		indentationStr.append(Constant::Symbol::YAML_Indent); // Add one more indent for properties
	}

	// Print key-value pairs
	for (int index = 0; index < keyValuePairs_.size(); index++) {
		const JSON_ENTRY& jsonEntry = keyValuePairs_.at(index);
		formatStr = StringUtils::stringFormat(_T("%s%s: \"%s\"\n"), indentationStr.getString(), jsonEntry.key.getString(), jsonEntry.value.getString());
		outputString.append(formatStr);
	}

	// Print child objects
	if ((childObjCount_ > 0) && (childObjList_ != NULL)) {
		for (int count = 0; count < childObjCount_; count++) {
			PJSONDATA subItemPtr = childObjList_[count];
			if (subItemPtr != NULL) {
				String subItemOutput;
				subItemPtr->printYAML(subItemOutput, indent + 1);
				outputString.append(subItemOutput);
			}
		}
	}
}

/**
 * @brief	Constructor
 */
Logger::Logger(byte byLogType)
{
	// Log data array
	logData_.clear();

	// Properties
	logType_ = byLogType;
	writeMode_ = LogWriteMode::ReadOnly;
	maxSize_ = kInfinite;
	filePath_ = Constant::String::Empty;
	defaultTemplate_ = NULL;
}

/**
 * @brief	Destructor
 */
Logger::~Logger()
{
	// Clean up log data
	logData_.clear();

	// Clean up default item template
	if (defaultTemplate_ != NULL) {
		delete defaultTemplate_;
		defaultTemplate_ = NULL;
	}
}

/**
 * @brief	Return a specific log item of log list
 * @param	index - Item index
 * @return	LOGITEM - Return log item
 */
LOGITEM& Logger::getLogItem(int index)
{
	// If current log data is empty
	if (isEmpty()) {
		// Return an empty dummy item
		static LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (index < 0) {
		// Return 1st item
		index = 0;
	}
	else if (index >= getLogCount()) {
		// Return last item
		index = (getLogCount() - 1);
	}

	return logData_.at(index);
}

const LOGITEM& Logger::getLogItem(int index) const
{
	// If current log data is empty
	if (isEmpty()) {
		// Return an empty dummy item
		static const LOGITEM logDummyItem;
		return logDummyItem;
	}

	// Invalid index
	if (index < 0) {
		// Return 1st item
		index = 0;
	}
	else if (index >= getLogCount()) {
		// Return last item
		index = (getLogCount() - 1);
	}

	return logData_.at(index);
}

/**
 * @brief	Get/set log default template
 * @param	logItemTemplate - Log item template
 * @return	byte
 */
void Logger::setDefaultTemplate(const LOGITEM& logItemTemplate) noexcept
{
	// Initialize default template
	if (defaultTemplate_ == NULL) {
		defaultTemplate_ = new LOGITEM;
		if (defaultTemplate_ == NULL) {
			TRACE_ERROR("Default item initialization failed!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update template
	if (defaultTemplate_ != NULL) {
		if (logItemTemplate.isEmpty()) return;
		defaultTemplate_->copy(logItemTemplate);
	}
}

/**
 * @brief	Add a log item into log data
 * @param	logItem	- Log item to write
 * @return	None
 */
void Logger::outputItem(const LOGITEM& logItem)
{
	if (getWriteMode() == LogWriteMode::WriteInstantly) {
		// Write instantly
		write(logItem);
	}
	else {
		// If already reached max data size
		size_t maxSize = getMaxSize();
		if ((maxSize != kInfinite) && (getLogCount() >= maxSize)) {
			// Can not output log item --> Trace info
			TRACE_ERROR("Output log item failed: Log data exceeded max size!!!");
			return;
		}

		// Store log data
		logData_.push_back(logItem);
	}
}

/**
 * @brief	Add a log string into log list data
 * @param	logString - Log string
 * @param	byType	  - Log type
 * @return	None
 */
void Logger::outputString(const wchar_t* logString, bool useLastTemplate /* = true */)
{
	if (getWriteMode() == LogWriteMode::WriteInstantly) {
		// Write instantly
		write(logString);
	}
	else {
		// Get log time
		DateTime logTime = DateTimeUtils::getCurrentDateTime();

		// Prepare log item
		LOGITEM logItem;
		if (useLastTemplate == true) {
			// Use last log item as template
			if (isEmpty()) {
				// Can not output log string --> Trace info
				TRACE_ERROR("Output log string failed: No item to use as template!!!");
				return;
			}
			else {
				// Copy template
				logItem.copy(getLogItem(getLogCount() - 1));
			}
		}
		else {
			// Use default template
			if (getDefaultTemplate() == NULL) {
				// Can not output log string --> Trace info
				TRACE_ERROR("Output log string failed: Default template not set!!!");
				return;
			}
			else {
				// Copy template
				logItem.copy(*(getDefaultTemplate()));
			}
		}

		// Update log item data
		logItem.setTime(logTime);
		logItem.setLogString(logString);
		outputItem(logItem);
	}
}

/**
 * @brief	Write log data into the corresponding logfile
 * @param	None
 * @return	bool - Result of log writing process
 */
bool Logger::write(void)
{
	bool result = true;
	DWORD errorCode;
	HWND mainWndHandle = GET_HANDLE_MAINWND();

	// Quit if current log is set as Read-only
	// or current log mode is write instantly mode
	if ((getWriteMode() == LogWriteMode::ReadOnly) ||
		(getWriteMode() == LogWriteMode::WriteInstantly))
		return false;

	CFile logFile;
	String fileName;
	String currentFileName;

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	LOGITEM logItem;
	DateTime tempTime;
	String logFormatString;
	String filePath;

	// Setup performance counter for tracking
	PerformanceCounter counter;
	counter.start();

	for (int index = 0; index < getLogCount(); index++)
	{
		// Get log item
		logItem = getLogItem(index);
		tempTime = logItem.getTime();

		// Get filename according to type of logs
		switch (logType_)
		{
		case LOGTYPE_APP_EVENT:
			// Format app event log filename
			fileName.format(Constant::File::Name::AppEventLog, tempTime.year(), tempTime.month());
			filePath = StringUtils::makeFilePath(folderPath, fileName, Constant::File::Extension::Log);
			if (currentFileName.isEmpty()) {
				// Set current file name
				currentFileName = fileName;
			}

			// If a file with another name (previous day's log file) is opening,
			// write down all current log strings and close the file
			if ((logFile.m_hFile != CFile::hFileNull) && (fileName != currentFileName))
			{
				if (!logFormatString.isEmpty()) {

					// Write log strings to file
					logFile.Write(logFormatString, logFormatString.getLength() * sizeof(wchar_t));
					logFile.Flush();

					logFormatString.empty();
				}

				// Close current file
				logFile.Close();

				// Set new current file name
				currentFileName = fileName;
			}
			break;

		case LOGTYPE_HISTORY_LOG:
			// App history log
			fileName = Constant::File::Name::AppHistory;
			filePath = StringUtils::makeFilePath(folderPath, fileName, Constant::File::Extension::Log);
			break;

		default:
			// Wrong argument
			TRACE_ERROR("Write log failed: Invalid log type!!!");
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
			// Show error message
			errorCode = APP_ERROR_WRONG_ARGUMENT;
			PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
			break;
		}

		// Check if file is opening, if not, open it
		if (logFile.m_hFile == CFile::hFileNull)
		{
			result = logFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
			if (result == false) {

				// Open file failed
				errorCode = GetLastError();

				// Trace error
				TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", errorCode);
				TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

				// Show error message
				PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
				return result;
			}

			// Go to end of file
			logFile.SeekToEnd();
		}

		// Format output log strings
		logFormatString += logItem.formatOutput();
	}

	if (!logFormatString.isEmpty()) {
		// Write log strings to file
		logFile.Write(logFormatString, logFormatString.getLength() * sizeof(wchar_t));
		logFile.Flush();
	}

	// Close file after done writing
	if (logFile.m_hFile != CFile::hFileNull) {
		logFile.Close();
	}

	// Display performance counter
	counter.stop();
	outputDebugLogFormat(_T("Total write log time: %.4f (ms)"), counter.getElapsedTime(true));

	return true;
}

/**
 * @brief	Write log item instantly into logfile
 * @param	logItem	 - Log item to write
 * @param	filePath - Output log file path
 * @return	bool - Result of log writing process
 */
bool Logger::write(const LOGITEM& logItem, const wchar_t* /* filePath = NULL */)
{
	bool result = true;
	DWORD errorCode;
	HWND mainWndHandle = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (getWriteMode() != LogWriteMode::WriteInstantly)
		return false;

	String fileName;
	CFile logFile;

	String logFormatString;

	// Get log time
	DateTime tempTimeValue = logItem.getTime();

	// Get filename according to type of logs
	switch (logType_)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		fileName.format(Constant::File::Name::AppEventLog, tempTimeValue.year(), tempTimeValue.month());
		break;

	case LOGTYPE_HISTORY_LOG:
		// App history log
		fileName = Constant::File::Name::AppHistory;
		break;

	default:
		// Wrong argument
		TRACE_ERROR("Error: Invalid log type!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		errorCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
		return false;
	}

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Get file path
	String filePath = StringUtils::makeFilePath(folderPath, fileName, Constant::File::Extension::Log);

	// Check if file is opening, if not, open it
	if (logFile.m_hFile == CFile::hFileNull)
	{
		result = logFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (result == false) {

			// Open file failed
			errorCode = GetLastError();

			// Trace error
			TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", errorCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			errorCode = GetLastError();
			PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
			return result;
		}

		// Go to end of file
		logFile.SeekToEnd();
	}

	// Format output log strings
	logFormatString = logItem.formatOutput();

	if (!logFormatString.isEmpty()) {
		// Write log strings to file
		logFile.Write(logFormatString, logFormatString.getLength() * sizeof(wchar_t));
		logFile.Flush();
	}

	// Close file after done writing
	if (logFile.m_hFile != CFile::hFileNull) {
		logFile.Close();
	}

	return true;
}

/**
 * @brief	Write log string instantly into logfile
 * @param	logString - Log string
 * @return	bool - Result of log writing process
 */
bool Logger::write(const wchar_t* logString, const wchar_t* /* filePath  = NULL */)
{
	bool result = true;
	DWORD errorCode;
	HWND mainWndHandle = GET_HANDLE_MAINWND();

	// Quit if current log mode is not write instantly mode
	if (getWriteMode() != LogWriteMode::WriteInstantly)
		return false;

	String fileName;
	CFile logFile;

	String logFormatString;

	// Get log time
	DateTime currentTime = DateTimeUtils::getCurrentDateTime();

	// Get filename according to type of logs
	switch (logType_)
	{
	case LOGTYPE_APP_EVENT:
		// Format app event log filename
		fileName.format(Constant::File::Name::AppEventLog, currentTime.year(), currentTime.month());
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
		errorCode = APP_ERROR_WRONG_ARGUMENT;
		PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
		return false;
	}

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Get file path
	String filePath = StringUtils::makeFilePath(folderPath, fileName, Constant::File::Extension::Log);

	// Check if file is opening, if not, open it
	if (logFile.m_hFile == CFile::hFileNull)
	{
		result = logFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (result == false) {

			// Open file failed
			errorCode = GetLastError();

			// Trace error
			TRACE_FORMAT("Write log failed: Can not open/create log file!!! (Code: 0x%08X)", errorCode);
			TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);

			// Show error message
			errorCode = GetLastError();
			PostMessage(mainWndHandle, SM_APP_ERROR_MESSAGE, (WPARAM)errorCode, NULL);
			return result;
		}

		// Go to end of file
		logFile.SeekToEnd();
	}

	// Format output log strings
	LOGITEM logItem;
	logItem.setTime(currentTime);
	logItem.setLogString(logString);
	logFormatString = logItem.formatOutput();

	if (!logFormatString.isEmpty()) {
		// Write log strings to file
		logFile.Write(logFormatString, logFormatString.getLength() * sizeof(wchar_t));
		logFile.Flush();
	}

	// Close file after done writing
	if (logFile.m_hFile != CFile::hFileNull) {
		logFile.Close();
	}

	return true;
}


/**
 * @brief	Constructor
 */
DebugLogger::DebugLogger()
{
	// Log file pointers
	traceErrorFilePtr_ = NULL;
	traceDebugFilePtr_ = NULL;
	debugInfoFilePtr_ = NULL;

	// File exception pointers
	traceErrorExceptionPtr_ = NULL;
	traceDebugExceptionPtr_ = NULL;
	debugInfoExceptionPtr = NULL;
}

/**
 * @brief	Destructor
 */
DebugLogger::~DebugLogger()
{
	// Release and clean-up file pointers
	releaseTraceErrorLogFile();
	releaseTraceDebugLogFile();
	releaseDebugInfoLogFile();

	// Clean-up file exception pointers
	if (traceErrorExceptionPtr_ != NULL) {
		delete traceErrorExceptionPtr_;
		traceErrorExceptionPtr_ = NULL;
	}
	if (traceDebugExceptionPtr_ != NULL) {
		delete traceDebugExceptionPtr_;
		traceDebugExceptionPtr_ = NULL;
	}
	if (debugInfoExceptionPtr != NULL) {
		delete debugInfoExceptionPtr;
		debugInfoExceptionPtr = NULL;
	}
}

/**
 * @brief	Initialize trace error log file
 * @param	None
 * @return	true/false
 * @note	Destination file: traceError.log
 * @note	To output trace error detail log strings
 */
bool DebugLogger::initTraceErrorLogFile(void)
{
	// Verify global trace error log file pointer initialization
	VERIFY_INITIALIZATION(traceErrorFilePtr_, CFile);

	// Get trace error log file pointer
	NULL_POINTER_BREAK(traceErrorFilePtr_, return false);

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Log file path
	String filePath = StringUtils::makeFilePath(folderPath, Constant::File::Name::TraceError, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (traceErrorFilePtr_->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!traceErrorFilePtr_->Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite)) {
			// Show error message
			DWORD errorCode = GetLastError();
			AppCore::showErrorMessage(NULL, NULL, errorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG fileSize = traceErrorFilePtr_->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			traceErrorFilePtr_->Close();

			// Step2: Rename file extension to BAK
			if (!backupOldLogFile(filePath, Constant::File::Name::TraceError))
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
 * @note	Destination file: traceError.log
 * @note	To output trace error detail log strings
 */
void DebugLogger::releaseTraceErrorLogFile(void)
{
	// Clean up trace error log file pointer
	if (traceErrorFilePtr_ != NULL) {

		// Close file if is opening
		if (traceErrorFilePtr_->m_hFile != CFile::hFileNull) {
			traceErrorFilePtr_->Flush();
			traceErrorFilePtr_->Close();
		}
		delete traceErrorFilePtr_;
		traceErrorFilePtr_ = NULL;
	}
}

/**
 * @brief	Initialize trace debug log file
 * @param	None
 * @return	true/false
 * @note	Destination file: TraceDebug.log
 * @note	To output trace debug log strings (including the function name, code file and line where it failed)
 */
bool DebugLogger::initTraceDebugLogFile(void)
{
	// Verify global trace debug log file pointer initialization
	VERIFY_INITIALIZATION(traceDebugFilePtr_, CFile);

	// Get trace debug log file pointer
	NULL_POINTER_BREAK(traceDebugFilePtr_, return false);

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Log file path
	String filePath = StringUtils::makeFilePath(folderPath, Constant::File::Name::TraceDebug, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (traceDebugFilePtr_->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!traceDebugFilePtr_->Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite)) {
			// Show error message
			DWORD errorCode = GetLastError();
			AppCore::showErrorMessage(NULL, NULL, errorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG fileSize = traceDebugFilePtr_->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			traceDebugFilePtr_->Close();

			// Step2: Rename file extension to BAK
			if (!backupOldLogFile(filePath, Constant::File::Name::TraceDebug))
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
void DebugLogger::releaseTraceDebugLogFile(void)
{
	// Clean up trace debug info log file pointer
	if (traceDebugFilePtr_ != NULL) {

		// Close file if is opening
		if (traceDebugFilePtr_->m_hFile != CFile::hFileNull) {
			traceDebugFilePtr_->Flush();
			traceDebugFilePtr_->Close();
		}
		delete traceDebugFilePtr_;
		traceDebugFilePtr_ = NULL;
	}
}

/**
 * @brief	Initialize debug info log file
 * @param	None
 * @return	true/false
 * @note	Destination file: DebugInfo.log
 * @note	To output debug info log strings (similar to OutputDebugString, but output to file instead)
 */
bool DebugLogger::initDebugInfoLogFile(void)
{
	// Verify global debug info log file pointer initialization
	VERIFY_INITIALIZATION(debugInfoFilePtr_, CFile);

	// Get debug info log file pointer
	NULL_POINTER_BREAK(debugInfoFilePtr_, return false);

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Log file path
	String filePath = StringUtils::makeFilePath(folderPath, Constant::File::Name::DebugInfo, Constant::File::Extension::Log);

	// If the log file is not being opened
	while (debugInfoFilePtr_->m_hFile == CFile::hFileNull) {

		// Open the log file
		if (!debugInfoFilePtr_->Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyWrite)) {
			// Show error message
			DWORD errorCode = GetLastError();
			AppCore::showErrorMessage(NULL, NULL, errorCode);
			return false;
		}

		// Go to end of file
		ULONGLONG fileSize = debugInfoFilePtr_->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			debugInfoFilePtr_->Close();

			// Step2: Rename file extension to BAK
			if (!backupOldLogFile(filePath, Constant::File::Name::DebugInfo))
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
void DebugLogger::releaseDebugInfoLogFile(void)
{
	// Clean up debug info log file pointer
	if (debugInfoFilePtr_ != NULL) {

		// Close file if is opening
		if (debugInfoFilePtr_->m_hFile != CFile::hFileNull) {
			debugInfoFilePtr_->Flush();
			debugInfoFilePtr_->Close();
		}
		delete debugInfoFilePtr_;
		debugInfoFilePtr_ = NULL;
	}
}

/**
 * @brief	Backup old log file
 * @param	filePath	- File path (in/out)
 * @param	logFileName	- Log file name
 * @return	true/false
 */
bool DebugLogger::backupOldLogFile(const String& filePath, const wchar_t* logFileName)
{
	CFileFind Finder;

	// If file path is not specified, do nothing
	if (filePath.isEmpty()) return false;

	// Log folder path
	String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);

	// Search for backup file list
	for (int count = 0; count < Constant::Max::BackupFileNumber; count++) {

		// Make backup file path template
		String filePathTemp = StringUtils::makeFilePath(folderPath, logFileName, Constant::File::Extension::Backup_Log);
		if (filePathTemp.isEmpty())
			return false;

		// Format backup file path
		String bakFilePath;
		bakFilePath.format(filePathTemp, count);

		// Check if file has already existed
		if (Finder.FindFile(bakFilePath) == true) {

			// If backup file number exceeded the limit, can not backup more
			if (count == (Constant::Max::BackupFileNumber - 1)) return false;
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
 * @note	Destination file: traceError.log
 * @note	To output trace error detail log strings
 */
void DebugLogger::writeTraceErrorLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::stringFormat(templateFormat, currentDateTime.year(), currentDateTime.month(), currentDateTime.day(),
		currentDateTime.hour(), currentDateTime.minute(), currentDateTime.second(), currentDateTime.millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::loadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::stringFormat(templateFormat, timeFormatString.getString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.isEmpty())
		return;

	// If the file is not initialized or had been released
	if (getTraceErrorLogFile() == NULL) {
		if (!initTraceErrorLogFile())
			return;
	}

	// Re-acquire trace log file pointer
	CFile* traceErrorFilePtr = getTraceErrorLogFile();
	NULL_POINTER_BREAK(traceErrorFilePtr, return NOTHING);
	{
		// Write log string to file
		traceErrorFilePtr->Write(logOutputFormatString, logOutputFormatString.getLength() * sizeof(wchar_t));
		traceErrorFilePtr->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG fileSize = traceErrorFilePtr->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			traceErrorFilePtr->Close();

			// Step2: Rename file extension to BAK
			String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);
			String orginalFilePath = StringUtils::makeFilePath(folderPath.getString(), Constant::File::Name::TraceError, Constant::File::Extension::Log);
			if (!backupOldLogFile(orginalFilePath, Constant::File::Name::TraceError))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			releaseTraceErrorLogFile();
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
void DebugLogger::writeTraceDebugLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::stringFormat(templateFormat, currentDateTime.year(), currentDateTime.month(), currentDateTime.day(),
		currentDateTime.hour(), currentDateTime.minute(), currentDateTime.second(), currentDateTime.millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::loadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::stringFormat(templateFormat, timeFormatString.getString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.isEmpty()) return;

	// If the file is not initialized or had been released
	if (getTraceDebugLogFile() == NULL) {
		if (!initTraceDebugLogFile())
			return;
	}

	// Re-acquire trace debug log file pointer
	CFile* traceDebugFilePtr = getTraceDebugLogFile();
	NULL_POINTER_BREAK(traceDebugFilePtr, return NOTHING);
	{
		// Write log string to file
		traceDebugFilePtr->Write(logOutputFormatString, logOutputFormatString.getLength() * sizeof(wchar_t));
		traceDebugFilePtr->Flush();
	}

	// Re-check file size after writing
	{
		// Go to end of file
		ULONGLONG fileSize = traceDebugFilePtr->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			traceDebugFilePtr->Close();

			// Step2: Rename file extension to BAK
			String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);
			String orginalFilePath = StringUtils::makeFilePath(folderPath, Constant::File::Name::TraceDebug, Constant::File::Extension::Log);
			if (!backupOldLogFile(orginalFilePath, Constant::File::Name::TraceDebug))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			releaseTraceDebugLogFile();
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
void DebugLogger::writeDebugInfoLogFile(const wchar_t* logStringW)
{
	// Get current time up to milisecs
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::stringFormat(templateFormat, currentDateTime.year(), currentDateTime.month(), currentDateTime.day(),
		currentDateTime.hour(), currentDateTime.minute(), currentDateTime.second(), currentDateTime.millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::loadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::stringFormat(templateFormat, timeFormatString.getString(), logStringW, Constant::String::Empty);

	// If output log string is empty, do nothing
	if (logOutputFormatString.isEmpty()) return;

	// If the file is not initialized or had been released
	if (getDebugInfoLogFile() == NULL) {
		if (!initDebugInfoLogFile())
			return;
	}

	// Re-acquire debug info log file pointer
	CFile* debugInfoFilePtr = getDebugInfoLogFile();
	NULL_POINTER_BREAK(debugInfoFilePtr, return NOTHING);
	{
		// Write log string to file
		debugInfoFilePtr->Write(logOutputFormatString, logOutputFormatString.getLength() * sizeof(wchar_t));
		debugInfoFilePtr->Flush();
	}

	// Recheck file size after writing
	{
		// Go to end of file
		ULONGLONG fileSize = debugInfoFilePtr->SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			debugInfoFilePtr->Close();

			// Step2: Rename file extension to BAK
			String folderPath = StringUtils::getSubFolderPath(Constant::Folder::Log);
			String orginalFilePath = StringUtils::makeFilePath(folderPath, Constant::File::Name::DebugInfo, Constant::File::Extension::Log);
			if (!backupOldLogFile(orginalFilePath, Constant::File::Name::DebugInfo))
				return;

			// Step3: Release log file pointer --> Quit
			// New file will be re-initialized in the next function call
			releaseDebugInfoLogFile();
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
void DebugLogger::writeTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW)
{
	// Log file path
	String filePath = StringUtils::makeFilePath(Constant::Folder::Log, fileName, Constant::File::Extension::Log);

	CFile traceDebugFile;

	// Check if file is opening, if not, open it
	while (traceDebugFile.m_hFile == CFile::hFileNull) {

		bool result = traceDebugFile.Open(filePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);
		if (result == false) {
			// Show error message
			DWORD errorCode = GetLastError();
			LPARAM lParam = reinterpret_cast<LPARAM>(fileName);
			AppCore::showErrorMessage(NULL, NULL, errorCode, lParam);
			return;
		}

		// Go to end of file
		ULONGLONG fileSize = traceDebugFile.SeekToEnd();

		// If the file line number is already out of limit
		if (fileSize >= Constant::Max::LogFileSize) {

			// Step1: Close file
			traceDebugFile.Close();

			// Step2: Rename file extension to BAK
			CFileFind Finder;
			String backupFilePath;
			for (int count = 0; count < Constant::Max::BackupFileNumber; count++) {
				backupFilePath.format((filePath + Constant::File::Extension::Backup_Log), count);
				if (Finder.FindFile(backupFilePath) == true) {
					if (count == (Constant::Max::BackupFileNumber - 1)) return;
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
	DateTime currentDateTime = DateTimeUtils::getCurrentDateTime();

	// Format log date/time
	const wchar_t* middayFlag = (currentDateTime.hour() >= 12) ? _T("PM") : _T("AM");
	String templateFormat = StringUtils::loadResourceString(IDS_FORMAT_FULLDATETIME);
	String timeFormatString = StringUtils::stringFormat(templateFormat, currentDateTime.year(), currentDateTime.month(), currentDateTime.day(),
		currentDateTime.hour(), currentDateTime.minute(), currentDateTime.second(), currentDateTime.millisecond(), middayFlag);

	// Format output log string
	templateFormat = StringUtils::loadResourceString(IDS_FORMAT_LOGSTRING);
	String logOutputFormatString = StringUtils::stringFormat(templateFormat, timeFormatString.getString(), logStringW, Constant::String::Empty);

	if (!logOutputFormatString.isEmpty()) {
		// Write log string to file
		traceDebugFile.Write(logOutputFormatString, logOutputFormatString.getLength() * sizeof(wchar_t));
		traceDebugFile.Flush();
	}

	// Close file after done writing
	if (traceDebugFile.m_hFile != CFile::hFileNull) {
		traceDebugFile.Close();
	}
}

/**
 * @brief	Output exception/error trace log string to log file
 * @param	traceLogA - Output trace log string (ANSI)
 * @return	None
 */
void DebugLogger::traceError(const char* traceLogA)
{
	// Convert ANSI string to UNICODE
	const wchar_t* traceLogW = MAKEUNICODE(traceLogA);
	traceError(traceLogW);
}

/**
 * @brief	Output exception/error trace log string to log file
 * @param	traceLogW - Output trace log string (Unicode)
 * @return	None
 */
void DebugLogger::traceError(const wchar_t* traceLogW)
{
	// Write trace log file: traceError.log
	writeTraceErrorLogFile(traceLogW);
}

/**
 * @brief	Format and output exception/error trace log string to log file
 * @param	lpszTraceLogFormatA - Trace log format string (ANSI)
 * @param	...				    - Same as default MFC Format function
 * @return	None
 */
void DebugLogger::traceErrorFormat(const char* traceLogFormatA, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatA));

	// Format source string (ANSI)
	CStringA logFormatStringA;

	va_list argList;
	va_start(argList, traceLogFormatA);
	logFormatStringA.FormatV(traceLogFormatA, argList);
	va_end(argList);

	// Output trace log
	traceError(logFormatStringA);
}

/**
 * @brief	Format and output exception/error trace log string to log file
 * @param	lpszTraceLogFormatW - Trace log format string (Unicode)
 * @param	...				    - Same as default MFC Format function
 * @return	None
 */
void DebugLogger::traceErrorFormat(const wchar_t* traceLogFormatW, ...)
{
	ATLASSERT(AtlIsValidString(traceLogFormatW));

	// Format source string (Unicode)
	String logFormatStringW;

	va_list argList;
	va_start(argList, traceLogFormatW);
	logFormatStringW.formatV(traceLogFormatW, argList);
	va_end(argList);

	// Output trace log
	traceError(logFormatStringW);
}

/**
 * @brief	Output debug trace information log
 * @param	lpszFuncName - Code function name
 * @param	lpszFileName - Code file name
 * @param	lineIndex	 - Code line number
 * @return	None
 */
void DebugLogger::traceDebugInfo(const char* funcName, const char* fileName, int lineIndex)
{
	// Debug trace info
	const wchar_t* _funcName = MAKEUNICODE(funcName);
	const wchar_t* _fileName = MAKEUNICODE(fileName);

	// Format debug trace log
	String debugTraceFormat = StringUtils::stringFormat(_T("Function: %s, File: %s(%d)"), _funcName, _fileName, lineIndex);

	// Write debug trace log: TraceDebug.log
	writeTraceDebugLogFile(debugTraceFormat.getString());
}

/**
 * @brief	Output debug log string
 * @param	debugLog	- Debug log string (Unicode)
 * @param	forceOutput - Force output target
 * @return	None
 */
void DebugLogger::outputDebugLog(const wchar_t* debugLog, int forceOutput /* = kInvalidInteger */)
{
	// Get debug mode enable state
	bool isDebugModeEnabled = getDebugMode();

	// Get debug log string
	String debugLogStr = debugLog;

	// Get DebugTest tool dialog handle
	HWND debugTestDlgHandle = AppCore::findDebugTestDlg();

	// Debug log output target
	int debugOutputTarget = forceOutput;
	if (debugOutputTarget == kInvalidInteger) {
		debugOutputTarget = getDebugOutputTarget();
	}
	if ((debugTestDlgHandle != NULL) &&
		(IsWindowVisible(debugTestDlgHandle))) {
		// Force enable debug mode and
		// prefer output target to DebugTest tool if it's displaying
		isDebugModeEnabled = true;
		debugOutputTarget = DebugTestTool;
	}

	// If debug mode not enabled, do nothing
	if (isDebugModeEnabled == false)
		return;

	// Output debug string
	if (debugOutputTarget == DefaultOutput) {
		// Default output target: OutputDebugString
		// Debug strings can be watched by using VS Output screen or DebugView tool
		OutputDebugString(debugLogStr);
	}
	else if (debugOutputTarget == DebugInfoFile) {
		// Ouput debug log to file: DebugInfo.log
		getDebugLogger().writeDebugInfoLogFile(debugLogStr);
	}
	else if (debugOutputTarget == DebugTestTool) {
		// Output debug log to DebugTest tool
		if (debugTestDlgHandle == NULL) return;
		WPARAM wParam = MAKE_WPARAM_STRING(debugLogStr);
		LPARAM lParam = MAKE_LPARAM_STRING(debugLogStr);
		SendMessage(debugTestDlgHandle, SM_APP_DEBUG_OUTPUT, wParam, lParam);
	}
}

/**
 * @brief	Output debug log string format
 * @param	debugLogFormat - Debug log format string (Unicode)
 * @param	args		   - Argument list
 * @return	None
 */
void DebugLogger::outputDebugLogFormat(const wchar_t* debugLogFormat, va_list args)
{
	// Format source string
	String logFormatString;
	logFormatString.formatV(debugLogFormat, args);

	// Output debug string
	outputDebugLog(logFormatString);
}

/**
 * @brief	Output debug string format (combined version of String.Format and the default OutputDebugString function)
 * @param	debugStringFormat - Debug log format string (Unicode)
 * @param	args			  - Argument list
 * @return	None
 */
void DebugLogger::outputDebugStringFormat(const wchar_t* debugStringFormat, va_list args)
{
	// Format source string
	String logDebugStringFormat;
	logDebugStringFormat.formatV(debugStringFormat, args);

	// Output debug string
	OutputDebugString(logDebugStringFormat);
}
