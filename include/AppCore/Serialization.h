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
	unsigned getRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	bool writeRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int value);

	// Read/write registry values (string) with nested subsection
	String getRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName);
	bool writeRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);

	// Delete registry section or subsection by name
	bool deleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName = NULL);


	/*--------------- Functions for reading/writing application profile info ---------------*/


	// Read/write registry profile info values
	bool getProfileInfo(const wchar_t* keyName, int& referValue);
	bool writeProfileInfo(const wchar_t* keyName, int value);
	bool getProfileInfo(const wchar_t* keyName, String& referString);
	bool writeProfileInfo(const wchar_t* keyName, const wchar_t* valueString);


	/*--------------- Derivered functions for reading/writing each data type ---------------*/


	// Read/write registry config values
	bool getConfig(const wchar_t* keyName, int& referValue);
	bool writeConfig(const wchar_t* keyName, int value);

	// Delete section
	bool deleteConfigSection(void);

	// Read/write registry default schedule values
	bool getDefaultSchedule(const wchar_t* keyName, int& referValue);
	bool writeDefaultSchedule(const wchar_t* keyName, int value);

	// Read/write registry schedule extra item number value
	bool getScheduleExtraItemNum(const wchar_t* keyName, int& referValue);
	bool writeScheduleExtraItemNum(const wchar_t* keyName, int value);

	// Read/write registry schedule extra item values
	bool getScheduleExtra(int itemIndex, const wchar_t* keyName, int& referValue);
	bool writeScheduleExtra(int itemIndex, const wchar_t* keyName, int value);

	// Delete section
	bool deleteScheduleSection(void);

	// Read/write registry hotkeyset item number value
	bool getHotkeyItemNum(const wchar_t* keyName, int& referValue);
	bool writeHotkeyItemNum(const wchar_t* keyName, int value);

	// Read/write registry hotkeyset item values
	bool getHotkeySet(int itemIndex, const wchar_t* keyName, int& referValue);
	bool writeHotkeySet(int itemIndex, const wchar_t* keyName, int value);

	// Delete section
	bool deleteHotkeySetSection(void);

	// Read/write registry Power Reminder common style data
	bool getPwrReminderCommonStyle(const wchar_t* keyName, int& referValue);
	bool writePwrReminderCommonStyle(const wchar_t* kyName, int value);
	bool getPwrReminderCommonStyle(const wchar_t* keyName, String& referString);
	bool writePwrReminderCommonStyle(const wchar_t* keyName, const wchar_t* value);

	// Read/write registry Power Reminder item number value
	bool getPwrReminderItemNum(const wchar_t* keyName, int& referValue);
	bool writePwrReminderItemNum(const wchar_t* keyName, int value);

	// Read/write registry Power Reminder item values
	bool getPwrReminder(int itemIndex, const wchar_t* keyName, int& referValue);
	bool writePwrReminder(int itemIndex, const wchar_t* keyName, int value);
	bool getPwrReminder(int itemIndex, const wchar_t* keyName, String& referString);
	bool writePwrReminder(int itemIndex, const wchar_t* keyName, const wchar_t* value);

	// Delete section
	bool deletePwrReminderSection(void);

	// Read/write registry layout info values
	bool getLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& referValue);
	bool writeLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int value);

	// Delete section
	bool deleteLayoutInfoSection(void);

	// Read/write system event tracking data
	bool getSysEventTracking(const wchar_t* keyName, String& referString);
	bool writeSysEventTracking(const wchar_t* keyName, const wchar_t* value);

	// Read/write other global data variables
	bool getGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& referValue);
	bool writeGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int value);
	bool getGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& referString);
	bool writeGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value);
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
