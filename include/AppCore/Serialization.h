/**
 * @file		Serialization.h
 * @brief		Define necessary methods to read/write, load/save configurations
 * @author		AnthonyLeeStark
 * @date		2024.02.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Serialization_defs.h"


// Functions using for reading/writing registry values
namespace AppRegistry
{
	/*----------------- Base functions for reading/writing registry values ----------------*/
	
	// Read/write registry values (integer) with nested subsection
	unsigned GetRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	bool WriteRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int value);

	// Read/write registry values (string) with nested subsection
	String GetRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	bool WriteRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);

	// Delete registry section or subsection by name
	bool DeleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName = NULL);


	/*--------------- Functions for reading/writing application profile info ---------------*/


	// Read/write registry profile info values
	bool GetProfileInfo(const wchar_t* keyName, int& referValue);
	bool WriteProfileInfo(const wchar_t* keyName, int value);
	bool GetProfileInfo(const wchar_t* keyName, String& strRef);
	bool WriteProfileInfo(const wchar_t* keyName, const wchar_t* valueString);


	/*--------------- Derivered functions for reading/writing each data type ---------------*/


	// Read/write registry config values
	bool GetConfig(const wchar_t* keyName, int& referValue);
	bool WriteConfig(const wchar_t* keyName, int value);

	// Delete section
	bool DeleteConfigSection(void);

	// Read/write registry default schedule values
	bool GetDefaultSchedule(const wchar_t* keyName, int& referValue);
	bool WriteDefaultSchedule(const wchar_t* kyName, int value);

	// Read/write registry schedule extra item number value
	bool GetScheduleExtraItemNum(const wchar_t* keyName, int& referValue);
	bool WriteScheduleExtraItemNum(const wchar_t* keyName, int value);

	// Read/write registry schedule extra item values
	bool GetScheduleExtra(int itemIndex, const wchar_t* keyName, int& referValue);
	bool WriteScheduleExtra(int itemIndex, const wchar_t* keyName, int value);

	// Delete section
	bool DeleteScheduleSection(void);

	// Read/write registry hotkeyset item number value
	bool GetHotkeyItemNum(const wchar_t* keyName, int& referValue);
	bool WriteHotkeyItemNum(const wchar_t* keyName, int value);

	// Read/write registry hotkeyset item values
	bool GetHotkeySet(int itemIndex, const wchar_t* keyName, int& referValue);
	bool WriteHotkeySet(int itemIndex, const wchar_t* keyName, int value);

	// Delete section
	bool DeleteHotkeySetSection(void);

	// Read/write registry Power Reminder common style data
	bool GetPwrReminderCommonStyle(const wchar_t* keyName, int& referValue);
	bool WritePwrReminderCommonStyle(const wchar_t* kyName, int value);
	bool GetPwrReminderCommonStyle(const wchar_t* keyName, String& strRef);
	bool WritePwrReminderCommonStyle(const wchar_t* keyName, const wchar_t* value);

	// Read/write registry Power Reminder item number value
	bool GetPwrReminderItemNum(const wchar_t* keyName, int& referValue);
	bool WritePwrReminderItemNum(const wchar_t* keyName, int value);

	// Read/write registry Power Reminder item values
	bool GetPwrReminder(int itemIndex, const wchar_t* keyName, int& referValue);
	bool WritePwrReminder(int itemIndex, const wchar_t* keyName, int value);
	bool GetPwrReminder(int itemIndex, const wchar_t* keyName, String& strRef);
	bool WritePwrReminder(int itemIndex, const wchar_t* keyName, const wchar_t* value);

	// Delete section
	bool DeletePwrReminderSection(void);

	// Read/write registry layout info values
	bool GetLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& referValue);
	bool WriteLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int value);

	// Delete section
	bool DeleteLayoutInfoSection(void);

	// Read/write system event tracking data
	bool GetSysEventTracking(const wchar_t* keyName, String& strRef);
	bool WriteSysEventTracking(const wchar_t* keyName, const wchar_t* value);

	// Read/write other global data variables
	bool GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& referValue);
	bool WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int value);
	bool GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& strRef);
	bool WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);
};


// Using for configuration and data backup system
class BackupSystem
{
public:
	BackupSystem();
	~BackupSystem();

public:
	static bool RegistryExport();
};
