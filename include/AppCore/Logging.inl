
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Logging.inl
//		Description:	Implement definition of inline functions for Logging classes
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2025.05.26:		Create new for version 3.2
//
//		Copyright (c) 2015-2025 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef _LOGGING_INL_INCLUDED
#define _LOGGING_INL_INCLUDED


///////////////////////////////////////////////////////
//// Implementations

#ifdef _LOGGING_ENABLE_INLINES

// Get size of data by data value
template <typename DATA>
inline SIZE_T GetSizeByValue(DATA dataValue) {
	return sizeof(dataValue);
}

// Get log time
inline SYSTEMTIME LogItem::GetTime(void) const {
	return m_stTime;
};

// Set log time
inline void LogItem::SetTime(const SYSTEMTIME& stTime) {
	m_stTime = stTime;
};

// Get process ID
inline DWORD LogItem::GetProcessID(void) const {
	return m_dwProcessID;
};

// Set process ID
inline void LogItem::SetProcessID(void) {
	m_dwProcessID = GetCurrentProcessId();
};

// Get log category
inline USHORT LogItem::GetCategory(void) const {
	return m_usCategory;
};

// Set log category
inline void LogItem::SetCategory(USHORT usCategory) {
	m_usCategory = usCategory;
};

// Get log description string
inline CString LogItem::GetLogString(void) const {
	return m_strLogString;
};

// Set log description string
inline void LogItem::SetLogString(LPCTSTR lpszLogString) {
	m_strLogString = lpszLogString;
};

// Remove all log detail info data
inline void LogItem::RemoveDetailInfo(void) {
	m_arrDetailInfo.RemoveAll();
	m_arrDetailInfo.FreeExtra();
}

// Remove property by index
inline void JSON::RemoveProperty(INT_PTR nIndex)
{
	// Invalid index
	if ((nIndex < 0) || (nIndex >= this->m_arrKeyValuePairs.GetSize()))
		return;

	// Remove property by index
	this->m_arrKeyValuePairs.RemoveAt(nIndex);
}

// Set JSON object name
inline void JSON::SetObjectName(LPCTSTR lpszObjectName) {
	this->m_strObjectName = lpszObjectName;
}

// Initialize log data
inline void SLogging::Init()
{
	// Initialize empty log data
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

// Delete all log data
inline void SLogging::DeleteAll()
{
	m_arrLogData.RemoveAll();
	m_arrLogData.FreeExtra();
}

// Check if current log data is empty
inline BOOL SLogging::IsEmpty(void) const {
	return m_arrLogData.IsEmpty();
}

// Return the number of items of log data
inline INT_PTR SLogging::GetLogCount() const {
	return m_arrLogData.GetSize();
}

// Get maximum log data size
inline INT_PTR SLogging::GetMaxSize(void) const {
	return m_nMaxSize;
}

// Set maximum log data size
inline void SLogging::SetMaxSize(INT_PTR nMaxSize)
{
	// Max size can only be larger than current log data size
	if (nMaxSize > (this->m_arrLogData.GetSize())) {
		m_nMaxSize = nMaxSize;
	}
}

// Get log write mode
inline BYTE SLogging::GetWriteMode(void) const {
	return m_byWriteMode;
}

// Set log write mode
inline void SLogging::SetWriteMode(BYTE byWriteMode) {
	m_byWriteMode = byWriteMode;
}

// Get log output file path
inline void SLogging::GetFilePath(CString& strFilePath) {
	strFilePath = m_strFilePath;
}

// Set log output file path
inline void SLogging::SetFilePath(LPCTSTR lpszFilePath) {
	m_strFilePath = lpszFilePath;
}

// Get log default template
inline PLOGITEM SLogging::GetDefaultTemplate(void) {
	return m_pItemDefTemplate;
}

#endif		// ifdef _LOGGING_ENABLE_INLINES

///////////////////////////////////////////////////////

#endif		// !ifdef _LOGGING_INL_INCLUDED
