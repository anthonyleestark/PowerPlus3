
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
	UINT GetRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	BOOL WriteRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int nValue);

	// Read/write registry values (string) with nested subsection
	String GetRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	BOOL WriteRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);

	// Delete registry section or subsection by name
	BOOL DeleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName = NULL);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Functions for reading/writing application profile info
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry profile info values
	BOOL GetProfileInfo(const wchar_t* keyName, int& nRef);
	BOOL WriteProfileInfo(const wchar_t* keyName, int nValue);
	BOOL GetProfileInfo(const wchar_t* keyName, String& strRef);
	BOOL WriteProfileInfo(const wchar_t* keyName, const wchar_t* valueString);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Derivered functions for reading/writing each data type
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry config values
	BOOL GetConfig(const wchar_t* keyName, int& nRef);
	BOOL WriteConfig(const wchar_t* keyName, int nValue);

	// Delete section
	BOOL DeleteConfigSection(void);

	// Read/write registry default schedule values
	BOOL GetDefaultSchedule(const wchar_t* keyName, int& nRef);
	BOOL WriteDefaultSchedule(const wchar_t* kyName, int nValue);

	// Read/write registry schedule extra item number value
	BOOL GetScheduleExtraItemNum(const wchar_t* keyName, int& nRef);
	BOOL WriteScheduleExtraItemNum(const wchar_t* keyName, int nValue);

	// Read/write registry schedule extra item values
	BOOL GetScheduleExtra(int nItemIndex, const wchar_t* keyName, int& nRef);
	BOOL WriteScheduleExtra(int nItemIndex, const wchar_t* keyName, int nValue);

	// Delete section
	BOOL DeleteScheduleSection(void);

	// Read/write registry hotkeyset item number value
	BOOL GetHotkeyItemNum(const wchar_t* keyName, int& nRef);
	BOOL WriteHotkeyItemNum(const wchar_t* keyName, int nValue);

	// Read/write registry hotkeyset item values
	BOOL GetHotkeySet(int nItemIndex, const wchar_t* keyName, int& nRef);
	BOOL WriteHotkeySet(int nItemIndex, const wchar_t* keyName, int nValue);

	// Delete section
	BOOL DeleteHotkeySetSection(void);

	// Read/write registry Power Reminder item number value
	BOOL GetPwrReminderItemNum(const wchar_t* keyName, int& nRef);
	BOOL WritePwrReminderItemNum(const wchar_t* keyName, int nValue);

	// Read/write registry Power Reminder item values
	BOOL GetPwrReminder(int nItemIndex, const wchar_t* keyName, int& nRef);
	BOOL WritePwrReminder(int nItemIndex, const wchar_t* keyName, int nValue);
	BOOL GetPwrReminder(int nItemIndex, const wchar_t* keyName, String& strRef);
	BOOL WritePwrReminder(int nItemIndex, const wchar_t* keyName, const wchar_t* value);

	// Delete section
	BOOL DeletePwrReminderSection(void);

	// Read/write registry layout info values
	BOOL GetLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef);
	BOOL WriteLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int nValue);

	// Delete section
	BOOL DeleteLayoutInfoSection(void);

	// Read/write other global data variables
	BOOL GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef);
	BOOL WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int nValue);
	BOOL GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& strRef);
	BOOL WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);
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
