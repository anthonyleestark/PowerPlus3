
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Serialization.h
//		Description:	Define necessary methods to read/write, load/save configurations
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.03:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//						<3> 2025.06.03:		Rename from Config to Serialization
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Serialization_defs.h"


/////////////////////////////////////

//// Declarations

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Functions using for reading/writing registry values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace AppRegistry
{
	//////////////////////////////////////////////////////////////////////////
	//
	//	Base functions for reading/writing registry values
	//
	//////////////////////////////////////////////////////////////////////////
	
	// Read/write registry values (integer) with nested subsection
	UINT GetRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName);
	BOOL WriteRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue);

	// Read/write registry values (string) with nested subsection
	CString GetRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName);
	BOOL WriteRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszValue);

	// Delete registry section or subsection by name
	BOOL DeleteRegistrySection(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName = NULL);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Functions for reading/writing application profile info
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry profile info values
	BOOL GetProfileInfo(LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteProfileInfo(LPCTSTR lpszKeyName, int nValue);
	BOOL GetProfileInfo(LPCTSTR lpszKeyName, CString& strRef);
	BOOL WriteProfileInfo(const wchar_t* keyName, const wchar_t* valueString);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Derivered functions for reading/writing each data type
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry config values
	BOOL GetConfig(LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteConfig(LPCTSTR lpszKeyName, int nValue);

	// Delete section
	BOOL DeleteConfigSection(void);

	// Read/write registry default schedule values
	BOOL GetDefaultSchedule(LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteDefaultSchedule(LPCTSTR lpszKeyName, int nValue);

	// Read/write registry schedule extra item number value
	BOOL GetScheduleExtraItemNum(LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteScheduleExtraItemNum(LPCTSTR lpszKeyName, int nValue);

	// Read/write registry schedule extra item values
	BOOL GetScheduleExtra(int nItemIndex, LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteScheduleExtra(int nItemIndex, LPCTSTR lpszKeyName, int nValue);

	// Delete section
	BOOL DeleteScheduleSection(void);

	// Read/write registry hotkeyset item number value
	BOOL GetHotkeyItemNum(LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteHotkeyItemNum(LPCTSTR lpszKeyName, int nValue);

	// Read/write registry hotkeyset item values
	BOOL GetHotkeySet(int nItemIndex, LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteHotkeySet(int nItemIndex, LPCTSTR lpszKeyName, int nValue);

	// Delete section
	BOOL DeleteHotkeySetSection(void);

	// Read/write registry Power Reminder item number value
	BOOL GetPwrReminderItemNum(LPCTSTR lpszKeyName, int& nRef);
	BOOL WritePwrReminderItemNum(LPCTSTR lpszKeyName, int nValue);

	// Read/write registry Power Reminder item values
	BOOL GetPwrReminder(int nItemIndex, LPCTSTR lpszKeyName, int& nRef);
	BOOL WritePwrReminder(int nItemIndex, LPCTSTR lpszKeyName, int nValue);
	BOOL GetPwrReminder(int nItemIndex, LPCTSTR lpszKeyName, CString& strRef);
	BOOL WritePwrReminder(int nItemIndex, LPCTSTR lpszKeyName, CString strValue);

	// Delete section
	BOOL DeletePwrReminderSection(void);

	// Read/write registry layout info values
	BOOL GetLayoutInfo(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteLayoutInfo(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue);

	// Delete section
	BOOL DeleteLayoutInfoSection(void);

	// Read/write other global data variables
	BOOL GetGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue);
	BOOL GetGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, CString& strRef);
	BOOL WriteGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, CString strValue);
};


//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 BackupSystem
//  Description: Using for configuration and data backup system
//
//////////////////////////////////////////////////////////////////////////

class BackupSystem
{
public:
	BackupSystem();
	~BackupSystem();

public:
	static BOOL RegistryExport();
};
