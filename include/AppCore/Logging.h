/**
 * @file		Logging.h
 * @brief		Define necessary variables and methods to read/write application log
 * @author		AnthonyLeeStark
 * @date		2024.02.22
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore.h"
#include "MapTable.h"
#include "Logging_defs.h"


// Store log detail info item
class LogDetail
{
private:
	// Attributes
	USHORT	categoryId_;									// Detail category
	int		flag_;											// Detail flag

	// Data
	int		detailValue_;									// Detail value (integer)
	String	detailInfoString_;								// Detail info (string)
	PVOID	detailDataPtr_;									// Detail data (pointer)
	byte	pointerType_;									// Detail info pointer data type
	size_t	pointerSize_;									// Detail info pointer data size

public:
	// Construction
	LogDetail();
	LogDetail(const LogDetail& other) {
		this->copy(other);
	}

	// Copy assignment operator
	LogDetail& operator=(const LogDetail& other) {
		this->copy(other);
		return *this;
	}

public:
	// Member functions
	void init(void) noexcept;
	void copy(const LogDetail& other) noexcept;
	void copyPointer(const LogDetail& other);
	bool compare(const LogDetail& other) const;
	bool comparePointer(const LogDetail& other) const;
	bool isEmpty(void) const noexcept;

public:
	// Get/set functions
	constexpr USHORT getCategory(void) const noexcept {
		return categoryId_;
	}
	void setCategory(USHORT categoryId) noexcept {
		categoryId_ = categoryId;
	}
	constexpr int getFlag(void) const noexcept {
		return flag_;
	}
	void setFlag(int flag) noexcept {
		flag_ = flag;
	}
	constexpr int getDetailValue(void) const noexcept {
		return detailValue_;
	}
	void setDetailValue(int detailValue) noexcept {
		detailValue_ = detailValue;
	}
	String getDetailString(void) const noexcept {
		return detailInfoString_;
	}
	void setDetailString(const wchar_t* detailInfo) noexcept {
		detailInfoString_ = detailInfo;
	}
	PVOID getPointerData(void) const noexcept {
		return detailDataPtr_;
	}
	bool setPointerData(PVOID dataBuff, byte dataType = -1, size_t dataSize = 0);
	constexpr byte getPointerType(void) const noexcept {
		return pointerType_;
	}
	void setPointerType(byte pointerType) noexcept {
		pointerType_ = pointerType;
	}
	constexpr size_t getPointerSize(void) const noexcept {
		return pointerSize_;
	}
	void setPointerSize(size_t pointerSize) noexcept {
		pointerSize_ = pointerSize;
	}
};

// Define new typenames for LogData
using LOGDETAIL = LogDetail;
using PLOGDETAIL = LogDetail*;
using LOGDETAILARRAY = typename std::vector<LogDetail>;


// Store application log detail info
class LogDetailInfo : public LOGDETAILARRAY
{
public:
	// Construction
	LogDetailInfo() : LOGDETAILARRAY() {
		this->clear();
	}
	LogDetailInfo(const LogDetailInfo& other) {
		this->copyData(other);
	}

	// Copy assignment operator
	LogDetailInfo& operator=(const LogDetailInfo& other) {
		this->copyData(other);
		return *this;
	}

public:
	// Member functions
	void init(void) noexcept {
		this->clear();
	}
	void copyData(const LogDetailInfo& other) {
		this->init();
		this->assign(other.begin(), other.end());
	}

public:
	// Update data functions
	void addDetail(const LOGDETAIL& logDetail) {
		this->push_back(logDetail);
	}

	// Add detail item
	void addDetail(USHORT categoryId, int detailValue, int flag = 0);								// Add detail item (integer data only)
	void addDetail(USHORT categoryId, const wchar_t* detailInfo, int flag = 0);						// Add detail item (string data only)
	void addDetail(USHORT categoryId, int detailValue, const wchar_t* detailInfo, int flag = 0);	// Add detail item (both integer and string data)
};

// Define new typenames for LogDetailInfo
using LOGDETAILINFO = LogDetailInfo;
using PLOGDETAILINFO = LogDetailInfo*;


// Store application log item data
class LogItem
{
private:
	// Member variables
	DateTime		timeValue_;								// Log time
	DWORD			processId_;								// Process ID
	USHORT			categoryId_;							// Log category
	String			logString_;								// Log description string
	LOGDETAILINFO	detailInfo_;							// Log detail info

public:
	// Construction
	LogItem();
	LogItem(const LogItem& other) {
		this->copy(other);
	}

	// Copy assignment operator
	LogItem& operator=(const LogItem& other) {
		this->copy(other);
		return *this;
	}

public:
	// Member functions
	void copy(const LogItem& other) noexcept;
	bool compare(const LogItem& other) const noexcept;
	bool isEmpty(void) const noexcept;

	// Remove all log detail info data
	void removeDetailInfo(void) noexcept {
		detailInfo_.clear();
	}

	// Remove all log item data
	void removeAll(void) noexcept;

public:
	// Get/set functions
	DateTime getTime(void) const noexcept {
		return timeValue_;
	}
	void setTime(const DateTime& timeValue) noexcept {
		timeValue_ = timeValue;
	}
	constexpr DWORD getProcessId(void) const noexcept {
		return processId_;
	}
	void setProcessId(void) noexcept {
		processId_ = GetCurrentProcessId();
	}
	constexpr USHORT getCategory(void) const noexcept {
		return categoryId_;
	}
	void setCategory(USHORT categoryId) noexcept {
		categoryId_ = categoryId;
	}
	String getLogString(void) const noexcept {
		return logString_;
	}
	void setLogString(const wchar_t* logString) noexcept {
		logString_ = logString;
	}

	// Detail info functions
	void addDetail(const LOGDETAIL& logDetail) {
		detailInfo_.addDetail(logDetail);
	}
	void addDetail(USHORT categoryId, int detailValue, int flag = LogDetailFlag::Flag_Null) {
		detailInfo_.addDetail(categoryId, detailValue, flag);
	}
	void addDetail(USHORT categoryId, const wchar_t* detailInfo, int flag = LogDetailFlag::Flag_Null) {
		detailInfo_.addDetail(categoryId, detailInfo, flag);
	}
	void addDetail(USHORT categoryId, int detailValue, const wchar_t* detailInfo, int flag = LogDetailFlag::Flag_Null) {
		detailInfo_.addDetail(categoryId, detailValue, detailInfo, flag);
	}

	// Format data functions
	String formatDateTime(void) const;
	String formatOutput(void) const;
};

// Define new typenames for LogItem
using LOGITEM = LogItem;
using PLOGITEM = LogItem*;
using LOGDATA = typename std::vector<LogItem>;
using PLOGDATA = LOGDATA*;


// Define static functions
template <typename DATA>
static size_t getSizeByValue(DATA dataValue) {
	return sizeof(dataValue);
}
static size_t getSizeByType(byte dataType);


// Store JSON key-value pair data
struct JSON_ENTRY
{
	// Member variables
	String key;			// Key name
	String value;		// Value (string)

	// Construction
	JSON_ENTRY() : key(Constant::String::Empty), value(Constant::String::Empty) {}
	JSON_ENTRY(const String& k, const String& val) : key(k), value(val) {}

	// Operators
	bool operator==(const JSON_ENTRY& other) const {
		return ((key == other.key) && (value == other.value));
	}
	bool operator!=(const JSON_ENTRY& other) const {
		return !(*this == other);
	}
	JSON_ENTRY& operator=(const JSON_ENTRY& other) {
		if (this != &other) {
			key = other.key;
			value = other.value;
		}
		return *this;
	}
};

// Define new typenames for JSON entry data
using JSON_ENTRY_DATA = typename std::vector<JSON_ENTRY>;


// Using for storing JSON object data
class JSON
{
private:
	// Attributes
	String objectName_;									// JSON object name

	// Properties
	JSON_ENTRY_DATA keyValuePairs_;						// Key-value pairs (array)

	// Children
	size_t childObjCount_;								// Number of child objects
	JSON** childObjList_;								// List of child objects

public:
	// Construction
	JSON();
	JSON(const JSON& other) {
		this->copy(other);
	}
	~JSON() {
		this->removeAll();
	}

	// Copy assignment operator
	JSON& operator=(const JSON& other) {
		this->copy(other);
		return *this;
	}

protected:
	// Member functions
	void copy(const JSON& other) noexcept;
	void copyArrayData(const JSON& other);
	void copyPtrData(const JSON& other);
	bool compare(const JSON& other) const;
	bool isEmpty(void) const noexcept;

	// Remove property by index
	void removeProperty(size_t index) {
		if ((index < 0) || (index >= this->keyValuePairs_.size())) return;
		keyValuePairs_.erase(keyValuePairs_.begin() + index);
	}

	// Remove property by key name
	void removeProperty(const wchar_t* keyName);

	// Remove all object data
	void removeAll(void);

public:
	// Get/set functions
	void setObjectName(const wchar_t* objectName) noexcept {
		this->objectName_ = objectName;
	}
	void addString(const wchar_t* keyName, const wchar_t* value);
	void addInteger(const wchar_t* keyName, int value);
	void addFloat(const wchar_t* keyName, DOUBLE value);
	void addChildObject(JSON* objPtr);

	// Printing functions
	void print(String& outputString, int indent, bool hasSeparator, bool isMultiline = true) const;
	void printYAML(String& outputString, int indent) const;
};

// Define new typenames for JSON class object
using JSONDATA = JSON;
using PJSONDATA = JSONDATA*;


// Using for saving application log data
class SLogging
{
private:
	// Log data array
	LOGDATA logData_;

	// Properties
	byte	 logType_;					// Log type
	byte	 writeMode_;				// Log write mode
	size_t	 maxSize_;					// Log data max item count
	String   filePath_;					// Log output file path
	PLOGITEM defaultTemplate_;			// Log default template

public:
	// Construction
	SLogging(byte byLogType);
	~SLogging(void);

public:
	// Initialization
	virtual void init(void) noexcept {
		logData_.clear();
	}
	virtual void deleteAll(void) noexcept {
		logData_.clear();
	}

	// Get/set data
	virtual constexpr bool isEmpty(void) const noexcept {
		return logData_.empty();
	}
	virtual constexpr size_t getLogCount(void) const noexcept {
		return logData_.size();
	}
	virtual LOGITEM& getLogItem(int index);
	virtual const LOGITEM& getLogItem(int index) const;

	// Get/set properties function
	virtual constexpr size_t getMaxSize(void) const noexcept {
		return maxSize_;
	}
	virtual bool setMaxSize(size_t maxSize) noexcept {
		// Max size can only be larger than current log data size
		if (maxSize > (this->logData_.size())) {
			maxSize_ = maxSize;
			return true;
		}
		return false;
	}
	virtual constexpr byte getWriteMode(void) const noexcept {
		return writeMode_;
	}
	virtual void setWriteMode(byte writeMode) noexcept {
		writeMode_ = writeMode;
	}
	virtual void getFilePath(String& filePath) noexcept {
		filePath = filePath_;
	}
	virtual void setFilePath(const wchar_t* filePath) noexcept {
		filePath_ = filePath;
	}
	virtual PLOGITEM getDefaultTemplate(void) noexcept {
		return defaultTemplate_;
	}
	virtual void setDefaultTemplate(const LOGITEM& logItemTemplate) noexcept;

	// Output log functions
	void outputItem(const LOGITEM& logItem);
	void outputString(const wchar_t* logString, bool useLastTemplate = true);

	// Write log functions
	bool write(void);
	bool write(const LOGITEM& logItem, const wchar_t* filePath = NULL);
	bool write(const wchar_t* logString, const wchar_t* filePath = NULL);
};


// Class for debugging/error trace logging
class DebugLogging final
{
private:
	// Log file pointers
	CFile* traceErrorFilePtr_;
	CFile* traceDebugFilePtr_;
	CFile* debugInfoFilePtr_;

	// File exception pointers
	CFileException* traceErrorExceptionPtr_;
	CFileException* traceDebugExceptionPtr_;
	CFileException* debugInfoExceptionPtr;

private:
	// Singleton
	DebugLogging();

	// No copyable
	DebugLogging(const DebugLogging&) = delete;
	DebugLogging& operator=(const DebugLogging&) = delete;

	// No movable
	DebugLogging(const DebugLogging&&) = delete;
	DebugLogging& operator=(const DebugLogging&&) = delete;

public:
	// Get the single debug logging instance:
	// Because the debug logging will be applied for the entire application,
	// there must be one and only instance of it
	static DebugLogging& getDebugLogger(void) {
		static std::unique_ptr<DebugLogging> loggerInstance(new DebugLogging());
		return *loggerInstance;
	}

	// Make destructor public for self-destructing
	~DebugLogging();

public:
	// Initialization
	bool initTraceErrorLogFile(void);
	bool initTraceDebugLogFile(void);
	bool initDebugInfoLogFile(void);

	// Clean-up
	void releaseTraceErrorLogFile(void);
	void releaseTraceDebugLogFile(void);
	void releaseDebugInfoLogFile(void);

	// Access pointer
	CFile* getTraceErrorLogFile(void) {
		return traceErrorFilePtr_;
	}
	CFile* getTraceDebugLogFile(void) {
		return traceDebugFilePtr_;
	}
	CFile* getDebugInfoLogFile(void) {
		return debugInfoFilePtr_;
	}
	CFileException* getTraceErrorException(void) {
		return traceErrorExceptionPtr_;
	}
	CFileException* getTraceDebugException(void) {
		return traceDebugExceptionPtr_;
	}
	CFileException* getDebugInfoException(void) {
		return debugInfoExceptionPtr;
	}

public:
	// Trace logging functions
	void traceError(const char* traceLogA);
	void traceError(const wchar_t* traceLogW);
	void traceErrorFormat(const char* traceLogFormatA, ...);
	void traceErrorFormat(const wchar_t* traceLogFormatW, ...);
	void traceDebugInfo(const char* funcName, const char* fileName, int lineIndex);

	// Debug logging functions
	static void outputDebugLog(const wchar_t* debugLog, int forceStyle = -1);
	static void outputDebugLogFormat(const wchar_t* debugLogFormat, va_list args);
	static void outputDebugStringFormat(const wchar_t* debugStringFormat, va_list args);

	// Trace/debug file logging functions
	bool backupOldLogFile(const String& filePath, const wchar_t* logFileName);
	void writeTraceErrorLogFile(const wchar_t* logStringW);
	void writeTraceDebugLogFile(const wchar_t* logStringW);
	void writeDebugInfoLogFile(const wchar_t* logStringW);
	void writeTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW);
};

// Define wrapper for static debug logging functions for global usage
inline void outputDebugLog(const wchar_t* debugLog, int forceStyle = -1) {
	DebugLogging::outputDebugLog(debugLog, forceStyle);
}
inline void outputDebugLogFormat(const wchar_t* debugLogFormat, ...) {
	ATLASSERT(AtlIsValidString(debugLogFormat));

	va_list argList;
	va_start(argList, debugLogFormat);
	DebugLogging::outputDebugLogFormat(debugLogFormat, argList);
	va_end(argList);
}
inline void outputDebugStringFormat(const wchar_t* debugStringFormat, ...) {
	ATLASSERT(AtlIsValidString(debugStringFormat));

	va_list argList;
	va_start(argList, debugStringFormat);
	DebugLogging::outputDebugStringFormat(debugStringFormat, argList);
	va_end(argList);
}

