
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

#endif		// ifndef _LOGGING_INL_INCLUDED
