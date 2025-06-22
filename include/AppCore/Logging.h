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
	USHORT	m_usCategory;									// Detail category
	int		m_nFlag;										// Detail flag

	// Data
	int		m_nDetailValue;									// Detail value (integer)
	String	m_strDetailInfo;								// Detail info (string)
	PVOID	m_ptrDetailData;								// Detail data (pointer)
	byte	m_byPointerType;								// Detail info pointer data type
	size_t	m_szPointerSize;								// Detail info pointer data size

public:
	// Construction
	LogDetail();
	LogDetail(const LogDetail& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	LogDetail& operator=(const LogDetail& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Member functions
	void Init(void) noexcept;
	void Copy(const LogDetail& other) noexcept;
	void PointerCopy(const LogDetail& other);
	bool Compare(const LogDetail& other) const;
	bool PointerCompare(const LogDetail& other) const;
	bool IsEmpty(void) const noexcept;

public:
	// Get/set functions
	constexpr USHORT GetCategory(void) const noexcept {
		return m_usCategory;
	};
	void SetCategory(USHORT usCategory) noexcept {
		m_usCategory = usCategory;
	};
	constexpr int GetFlag(void) const noexcept {
		return m_nFlag;
	};
	void SetFlag(int nFlag) noexcept {
		m_nFlag = nFlag;
	};
	constexpr int GetDetailValue(void) const noexcept {
		return m_nDetailValue;
	};
	void SetDetailValue(int nDetailValue) noexcept {
		m_nDetailValue = nDetailValue;
	};
	String GetDetailString(void) const noexcept {
		return m_strDetailInfo;
	};
	void SetDetailString(const wchar_t* detailInfo) noexcept {
		m_strDetailInfo = detailInfo;
	};
	PVOID GetPointerData(void) const noexcept {
		return m_ptrDetailData;
	};
	bool SetPointerData(PVOID pDataBuff, byte byDataType = -1, size_t szDataSize = 0);
	constexpr byte GetPointerType(void) const noexcept {
		return m_byPointerType;
	};
	void SetPointerType(byte byPointerType) noexcept {
		m_byPointerType = byPointerType;
	};
	constexpr size_t GetPointerSize(void) const noexcept {
		return m_szPointerSize;
	};
	void SetPointerSize(size_t szPointerSize) noexcept {
		m_szPointerSize = szPointerSize;
	};
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
	};
	LogDetailInfo(const LogDetailInfo& other) {
		this->CopyData(other);
	};

	// Copy assignment operator
	LogDetailInfo& operator=(const LogDetailInfo& other) {
		this->CopyData(other);
		return *this;
	};

public:
	// Member functions
	void Init(void) noexcept {
		this->clear();
	};
	void CopyData(const LogDetailInfo& other) {
		this->Init();
		this->assign(other.begin(), other.end());
	};

public:
	// Update data functions
	void AddDetail(const LOGDETAIL& logDetail) {
		this->push_back(logDetail);
	};

	// Add detail item
	void AddDetail(USHORT usCategory, int nDetailInfo, int nFlag = 0);								// Add detail item (integer data only)
	void AddDetail(USHORT usCategory, const wchar_t* detailInfo, int nFlag = 0);					// Add detail item (string data only)
	void AddDetail(USHORT usCategory, int nDetailInfo, const wchar_t* detailInfo, int nFlag = 0);	// Add detail item (both integer and string data)
};

// Define new typenames for LogDetailInfo
using LOGDETAILINFO = LogDetailInfo;
using PLOGDETAILINFO = LogDetailInfo*;


// Store application log item data
class LogItem
{
private:
	// Member variables
	DateTime		m_stTime;								// Log time
	DWORD			m_dwProcessID;							// Process ID
	USHORT			m_usCategory;							// Log category
	String			m_strLogString;							// Log description string
	LOGDETAILINFO	m_arrDetailInfo;						// Log detail info

public:
	// Construction
	LogItem();
	LogItem(const LogItem& other) {
		this->Copy(other);
	};

	// Copy assignment operator
	LogItem& operator=(const LogItem& other) {
		this->Copy(other);
		return *this;
	};

public:
	// Member functions
	void Copy(const LogItem& other) noexcept;
	bool Compare(const LogItem& other) const noexcept;
	bool IsEmpty(void) const noexcept;

	// Remove all log detail info data
	void RemoveDetailInfo(void) noexcept {
		m_arrDetailInfo.clear();
	};

	// Remove all log item data
	void RemoveAll(void) noexcept;

public:
	// Get/set functions
	DateTime GetTime(void) const noexcept {
		return m_stTime;
	};
	void SetTime(const DateTime& stTime) noexcept {
		m_stTime = stTime;
	};
	constexpr DWORD GetProcessID(void) const noexcept {
		return m_dwProcessID;
	};
	void SetProcessID(void) noexcept {
		m_dwProcessID = GetCurrentProcessId();
	};
	constexpr USHORT GetCategory(void) const noexcept {
		return m_usCategory;
	};
	void SetCategory(USHORT usCategory) noexcept {
		m_usCategory = usCategory;
	};
	String GetLogString(void) const noexcept {
		return m_strLogString;
	};
	void SetLogString(const wchar_t* logString) noexcept {
		m_strLogString = logString;
	};

	// Detail info functions
	void AddDetail(const LOGDETAIL& logDetail) {
		m_arrDetailInfo.AddDetail(logDetail);
	};
	void AddDetail(USHORT usCategory, int nDetailInfo, int nFlag = LogDetailFlag::Flag_Null) {
		m_arrDetailInfo.AddDetail(usCategory, nDetailInfo, nFlag);
	};
	void AddDetail(USHORT usCategory, const wchar_t* detailInfo, int nFlag = LogDetailFlag::Flag_Null) {
		m_arrDetailInfo.AddDetail(usCategory, detailInfo, nFlag);
	};
	void AddDetail(USHORT usCategory, int nDetailInfo, const wchar_t* detailInfo, int nFlag = LogDetailFlag::Flag_Null) {
		m_arrDetailInfo.AddDetail(usCategory, nDetailInfo, detailInfo, nFlag);
	};

	// Format data functions
	String FormatDateTime(void) const;
	String FormatOutput(void) const;
};

// Define new typenames for LogItem
using LOGITEM = LogItem;
using PLOGITEM = LogItem*;
using LOGDATA = typename std::vector<LogItem>;
using PLOGDATA = LOGDATA*;


// Define static functions
template <typename DATA>
static size_t GetSizeByValue(DATA dataValue) {
	return sizeof(dataValue);
};
static size_t GetSizeByType(byte byDataType);


// Store JSON key-value pair data
struct JSON_ENTRY
{
	// Member variables
	String strKey;			// Key name
	String strValue;		// Value (string)

	// Construction
	JSON_ENTRY() : strKey(Constant::String::Empty), strValue(Constant::String::Empty) {};
	JSON_ENTRY(const String& key, const String& value) : strKey(key), strValue(value) {};

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


// Using for storing JSON object data
class JSON
{
private:
	// Attributes
	String m_strObjectName;									// JSON object name

	// Properties
	JSON_ENTRY_DATA m_arrKeyValuePairs;						// Key-value pairs (array)

	// Children
	size_t m_nChildObjectCount;								// Number of child objects
	JSON** m_apChildObjectList;								// List of child objects

public:
	// Construction
	JSON();
	JSON(const JSON& other) {
		this->Copy(other);
	};
	~JSON() {
		this->RemoveAll();
	};

	// Copy assignment operator
	JSON& operator=(const JSON& other) {
		this->Copy(other);
		return *this;
	};

protected:
	// Member functions
	void Copy(const JSON& other) noexcept;
	void CopyArrayData(const JSON& other);
	void CopyPtrData(const JSON& other);
	bool Compare(const JSON& other) const;
	bool IsEmpty(void) const noexcept;

	// Remove property by index
	void RemoveProperty(size_t nIndex) {
		if ((nIndex < 0) || (nIndex >= this->m_arrKeyValuePairs.size())) return;
		m_arrKeyValuePairs.erase(m_arrKeyValuePairs.begin() + nIndex);
	}

	// Remove property by key name
	void RemoveProperty(const wchar_t* keyName);

	// Remove all object data
	void RemoveAll(void);

public:
	// Get/set functions
	void SetObjectName(const wchar_t* objectName) noexcept {
		this->m_strObjectName = objectName;
	};
	void AddString(const wchar_t* keyName, const wchar_t* value);
	void AddInteger(const wchar_t* keyName, int nValue);
	void AddFloat(const wchar_t* keyName, DOUBLE dbValue);
	void AddChildObject(JSON* pSrc);

	// Printing functions
	void Print(String& outputString, int nIndent, bool bSeparator, bool bMultiline = true) const;
	void PrintYAML(String& outputString, int nIndent) const;
};

// Define new typenames for JSON class object
using JSONDATA = JSON;
using PJSONDATA = JSONDATA*;


// Using for saving application log data
class SLogging
{
private:
	// Log data array
	LOGDATA m_arrLogData;

	// Properties
	byte	 m_byLogType;					// Log type
	byte	 m_byWriteMode;					// Log write mode
	size_t	 m_nMaxSize;					// Log data max item count
	String   m_strFilePath;					// Log output file path
	PLOGITEM m_pItemDefTemplate;			// Log default template

public:
	// Construction
	SLogging(byte byLogType);
	~SLogging(void);

public:
	// Initialization
	virtual void Init(void) noexcept {
		m_arrLogData.clear();
	};
	virtual void DeleteAll(void) noexcept {
		m_arrLogData.clear();
	};

	// Get/set data
	virtual constexpr bool IsEmpty(void) const noexcept {
		return m_arrLogData.empty();
	};
	virtual constexpr size_t GetLogCount(void) const noexcept {
		return m_arrLogData.size();
	};
	virtual LOGITEM& GetLogItem(int nIndex);
	virtual const LOGITEM& GetLogItem(int nIndex) const;

	// Get/set properties function
	virtual constexpr size_t GetMaxSize(void) const noexcept {
		return m_nMaxSize;
	};
	virtual bool SetMaxSize(size_t nMaxSize) noexcept {
		// Max size can only be larger than current log data size
		if (nMaxSize > (this->m_arrLogData.size())) {
			m_nMaxSize = nMaxSize;
			return true;
		}
		return false;
	};
	virtual constexpr byte GetWriteMode(void) const noexcept {
		return m_byWriteMode;
	};
	virtual void SetWriteMode(byte byWriteMode) noexcept {
		m_byWriteMode = byWriteMode;
	};
	virtual void GetFilePath(String& strFilePath) noexcept {
		strFilePath = m_strFilePath;
	};
	virtual void SetFilePath(const wchar_t* filePath) noexcept {
		m_strFilePath = filePath;
	};
	virtual PLOGITEM GetDefaultTemplate(void) noexcept {
		return m_pItemDefTemplate;
	};
	virtual void SetDefaultTemplate(const LOGITEM& logItemTemplate) noexcept;

	// Output log functions
	void OutputItem(const LOGITEM& logItem);
	void OutputString(const wchar_t* logString, bool bUseLastTemplate = true);

	// Write log functions
	bool Write(void);
	bool Write(const LOGITEM& logItem, const wchar_t* filePath = NULL);
	bool Write(const wchar_t* logString, const wchar_t* filePath = NULL);
};


// Class for debugging/error trace logging
class DebugLogging final
{
private:
	// Log file pointers
	CFile* m_pFileLogTraceError;
	CFile* m_pFileLogTraceDebug;
	CFile* m_pFileLogDebugInfo;

	// File exception pointers
	CFileException* m_pExcLogTraceError;
	CFileException* m_pExcLogTraceDebug;
	CFileException* m_pExcLogDebugInfo;

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
	static DebugLogging& GetDebugLogger(void) {
		static std::unique_ptr<DebugLogging> loggerInstance(new DebugLogging());
		return *loggerInstance;
	};

	// Make destructor public for self-destructing
	~DebugLogging();

public:
	// Initialization
	bool InitTraceErrorLogFile(void);
	bool InitTraceDebugLogFile(void);
	bool InitDebugInfoLogFile(void);

	// Clean-up
	void ReleaseTraceErrorLogFile(void);
	void ReleaseTraceDebugLogFile(void);
	void ReleaseDebugInfoLogFile(void);

	// Access pointer
	CFile* GetTraceErrorLogFile(void) {
		return m_pFileLogTraceError;
	};
	CFile* GetTraceDebugLogFile(void) {
		return m_pFileLogTraceDebug;
	};
	CFile* GetDebugInfoLogFile(void) {
		return m_pFileLogDebugInfo;
	};
	CFileException* GetTraceErrorException(void) {
		return m_pExcLogTraceError;
	};
	CFileException* GetTraceDebugException(void) {
		return m_pExcLogTraceDebug;
	};
	CFileException* GetDebugInfoException(void) {
		return m_pExcLogDebugInfo;
	};

public:
	// Trace logging functions
	void TraceError(const char* traceLogA);
	void TraceError(const wchar_t* traceLogW);
	void TraceErrorFormat(const char* traceLogFormatA, ...);
	void TraceErrorFormat(const wchar_t* traceLogFormatW, ...);
	void TraceDebugInfo(const char* funcName, const char* fileName, int lineIndex);

	// Debug logging functions
	static void OutputDebugLog(const wchar_t* debugLog, int forceStyle = -1);
	static void OutputDebugLogFormat(const wchar_t* debugLogFormat, va_list args);
	static void OutputDebugStringFormat(const wchar_t* debugStringFormat, va_list args);

	// Trace/debug file logging functions
	bool BackupOldLogFile(const String& filePath, const wchar_t* logFileName);
	void WriteTraceErrorLogFile(const wchar_t* logStringW);
	void WriteTraceDebugLogFile(const wchar_t* logStringW);
	void WriteDebugInfoLogFile(const wchar_t* logStringW);
	void WriteTraceNDebugLogFileBase(const wchar_t* fileName, const wchar_t* logStringW);
};

// Define wrapper for static debug logging functions for global usage
inline void OutputDebugLog(const wchar_t* debugLog, int forceStyle = -1) {
	DebugLogging::OutputDebugLog(debugLog, forceStyle);
};
inline void OutputDebugLogFormat(const wchar_t* debugLogFormat, ...) {
	ATLASSERT(AtlIsValidString(debugLogFormat));

	va_list argList;
	va_start(argList, debugLogFormat);
	DebugLogging::OutputDebugLogFormat(debugLogFormat, argList);
	va_end(argList);
};
inline void OutputDebugStringFormat(const wchar_t* debugStringFormat, ...) {
	ATLASSERT(AtlIsValidString(debugStringFormat));

	va_list argList;
	va_start(argList, debugStringFormat);
	DebugLogging::OutputDebugStringFormat(debugStringFormat, argList);
	va_end(argList);
};

