/**
 * @file		Serialization.cpp
 * @brief		Implement necessary methods to read/write, load/save configurations
 * @author		AnthonyLeeStark
 * @date		2024.02.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/Serialization.h"
#include "AppCore/Logging.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;


/*----------------- Base functions for reading/writing registry values ----------------*/


/**
 * @brief	Using for reading/writing registry values with nested subsection
 * @param	sectionName	   - Section name (string)
 * @param	subSectionName - Sub section name (string)
 * @param	keyName		   - Key name (string)
 * @param	value		   - Value (int)
 * @return	unsigned - Read value
 * @return	bool	 - Result of writing process
 */
unsigned AppRegistry::getRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Get registry value
	return AfxGetApp()->GetProfileInt(sectionNameFormat, keyName, UINT_MAX);
}

bool AppRegistry::writeRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int value)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Write registry value
	return AfxGetApp()->WriteProfileInt(sectionNameFormat, keyName, value);
}

/**
 * @brief	Using for reading/writing registry values with nested subsection
 * @param	sectionName	   - Section name (string)
 * @param	subSectionName - Sub section name (string)
 * @param	keyName		   - Key name (string)
 * @param	value		   - Value (string)
 * @return	String - Read value
 * @return	bool   - Result of writing process
 */
String AppRegistry::getRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Get registry value
	return AfxGetApp()->GetProfileString(sectionNameFormat, keyName, Constant::String::Null).GetString();
}

bool AppRegistry::writeRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Write registry value
	return AfxGetApp()->WriteProfileString(sectionNameFormat, keyName, value);
}

/**
 * @brief	Using for delete registry section or subsection by name
 * @param	sectionName	   - Section name (string)
 * @param	subSectionName - Sub section name (string)
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName /* = NULL */)
{
	// Get name string
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Write registry value
	return AfxGetApp()->WriteProfileString(sectionNameFormat, NULL, NULL);
}


/*--------------- Functions for reading/writing application profile info ---------------*/


/**
 * @brief	Using for reading/writing registry profile info values
 * @param	keyName	- Key name
 * @param	referValue	- Result integer value (ref-value)
 * @param	value	- Value to write (integer)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getProfileInfo(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = AfxGetApp()->GetProfileInt(Constant::String::Empty, keyName, UINT_MAX);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeProfileInfo(const wchar_t* keyName, int value)
{
	// Write registry value
	return AfxGetApp()->WriteProfileInt(Constant::String::Empty, keyName, value);
}

/**
 * @brief	Using for reading/writing registry profile info values
 * @param	keyName		- Key name
 * @param	referString		- Result string value (ref-value)
 * @param	valueString - Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getProfileInfo(const wchar_t* keyName, String& ref)
{
	// Get registry value
	String resultString = AfxGetApp()->GetProfileString(Constant::String::Empty, keyName, Constant::String::Null).GetString();
	if (IS_NULL_STRING(resultString)) return false;
	ref = resultString; // Copy returned value
	return true;
}

bool AppRegistry::writeProfileInfo(const wchar_t* keyName, const wchar_t* valueString)
{
	// Write registry value
	return AfxGetApp()->WriteProfileString(Constant::String::Empty, keyName, valueString);
}


/*--------------- Derivered functions for reading/writing each data type ---------------*/


/**
 * @brief	Using for reading/writing registry config values
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getConfig(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::ConfigData, NULL, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeConfig(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::ConfigData, NULL, keyName, value);
}

/**
 * @brief	Using for delete config section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deleteConfigSection(void)
{
	return deleteRegistrySection(Section::ConfigData);
}

/**
 * @brief	Using for reading/writing registry default schedule values
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getDefaultSchedule(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeDefaultSchedule(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName, value);
}

/**
 * @brief	Using for reading/writing registry schedule extra item number values
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getScheduleExtraItemNum(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::ScheduleData, NULL, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeScheduleExtraItemNum(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::ScheduleData, NULL, keyName, value);
}

/**
 * @brief	Using for reading/writing registry schedule extra item values
 * @param	itemIndex  - Schedule extra item index
 * @param	keyName		- Key name
 * @param	referValue	    - Result value (ref-value)
 * @param	value	    - Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getScheduleExtra(int itemIndex, const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(itemIndex), keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeScheduleExtra(int itemIndex, const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(itemIndex), keyName, value);
}

/**
 * @brief	Using for delete schedule section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deleteScheduleSection(void)
{
	bool ret = true;

	// Delete default schedule subsection
	ret &= deleteRegistrySection(Section::ScheduleData, Section::Schedule::DefautItem);

	// Get subsection number
	int subItemNum = 0;
	ret &= getScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, subItemNum);
	if (ret == false) return false;

	// Delete subsection of items
	for (int index = 0; index < subItemNum; index++) {
		ret &= deleteRegistrySection(Section::ScheduleData, Section::Schedule::Item(index));
	}

	// Delete parent section
	ret &= deleteRegistrySection(Section::ScheduleData);

	return ret;
}

/**
 * @brief	Using for reading/writing registry hotkeyset item number values
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getHotkeyItemNum(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::HotkeySetData, NULL, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeHotkeyItemNum(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::HotkeySetData, NULL, keyName, value);
}

/**
 * @brief	Using for reading/writing registry hotkeyset item values
 * @param	itemIndex  - Hotkey item index
 * @param	keyName		- Key name
 * @param	referValue	    - Result value (ref-value)
 * @param	value	    - Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getHotkeySet(int itemIndex, const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(itemIndex), keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeHotkeySet(int itemIndex, const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(itemIndex), keyName, value);
}

/**
 * @brief	Using for delete HotkeySet section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deleteHotkeySetSection(void)
{
	bool ret = true;

	// Get subsection number
	int subItemNum = 0;
	ret &= getHotkeyItemNum(Key::HotkeySetData::ItemNum, subItemNum);
	if (ret == false) return false;

	// Delete subsection of items
	for (int index = 0; index < subItemNum; index++) {
		ret &= deleteRegistrySection(Section::HotkeySetData, Section::HotkeySet::Item(index));
	}

	// Delete parent section
	ret &= deleteRegistrySection(Section::HotkeySetData);

	return ret;
}

/**
 * @brief	Using for reading/writing registry Power Reminder common style data
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getPwrReminderCommonStyle(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writePwrReminderCommonStyle(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, value);
}

/**
 * @brief	Using for reading/writing registry Power Reminder common style data
 * @param	keyName - Key name
 * @param	referString	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getPwrReminderCommonStyle(const wchar_t* keyName, String& ref)
{
	// Get registry value
	String resultString = getRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	ref = resultString; // Copy returned value
	return true;
}

bool AppRegistry::writePwrReminderCommonStyle(const wchar_t* keyName, const wchar_t* value)
{
	return writeRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, value);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item number values
 * @param	keyName - Key name
 * @param	referValue	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getPwrReminderItemNum(const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::PwrReminderData, NULL, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writePwrReminderItemNum(const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::PwrReminderData, NULL, keyName, value);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item values
 * @param	itemIndex  - Hotkey item index
 * @param	keyName - Key name
 * @param	referValue	- Result value (integer) (ref-value)
 * @param	value	- Value to write (integer)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getPwrReminder(int itemIndex, const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(itemIndex), keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writePwrReminder(int itemIndex, const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(itemIndex), keyName, value);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item values
 * @param	itemIndex	- Hotkey item index
 * @param	keyName		- Key name
 * @param	referString	    - Result value (string) (ref-value)
 * @param	valueString    - Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getPwrReminder(int itemIndex, const wchar_t* keyName, String& ref)
{
	// Get registry value
	String resultString = getRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(itemIndex), keyName);
	if (IS_NULL_STRING(resultString)) return false;
	ref = resultString; // Copy returned value
	return true;
}

bool AppRegistry::writePwrReminder(int itemIndex, const wchar_t* keyName, const wchar_t* value)
{
	return writeRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(itemIndex), keyName, value);
}

/**
 * @brief	Using for delete Power Reminder section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deletePwrReminderSection(void)
{
	bool ret = true;

	// Delete common style section
	ret &= deleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::CommonStyle);

	// Get subsection number
	int subItemNum = 0;
	ret &= getPwrReminderItemNum(Key::PwrReminderData::ItemNum, subItemNum);
	if (ret == false) return false;

	// Delete subsection of items
	for (int index = 0; index < subItemNum; index++) {
		ret &= deleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::Item(index));
	}

	// Delete parent section
	ret &= deleteRegistrySection(Section::PwrReminderData);

	return ret;
}

/**
 * @brief	Using for reading/writing registry layout info values
 * @param	subSectionName  - Subsection name
 * @param	keyName			- Key name
 * @param	referValue			- Result value (ref-value)
 * @param	value			- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::LayoutInfo, subSectionName, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::LayoutInfo, subSectionName, keyName, value);
}

/**
 * @brief	Using for delete config section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::deleteLayoutInfoSection(void)
{
	return deleteRegistrySection(Section::LayoutInfo);
}

/**
 * @brief	Using for reading/writing registry system event tracking data
 * @param	keyName - Key name
 * @param	referString	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getSysEventTracking(const wchar_t* keyName, String& ref)
{
	// Get registry value
	String resultString = getRegistryValueString(Section::SystemEventTracking, NULL, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	ref = resultString; // Copy returned value
	return true;
}

bool AppRegistry::writeSysEventTracking(const wchar_t* keyName, const wchar_t* value)
{
	return writeRegistryValueString(Section::SystemEventTracking, NULL, keyName, value);
}

/**
 * @brief	Using for reading/writing registry global data values
 * @param	subSectionName  - Subsection name
 * @param	keyName			- Key name
 * @param	referValue			- Result value (integer/ref-value)
 * @param	value			- Value to write (integer)
 * @param	referString			- Result value (integer/ref-value)
 * @param	valueString		- Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::getGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& ref)
{
	// Get registry value
	int ret = getRegistryValueInt(Section::GlobalData, subSectionName, keyName);
	if (ret == UINT_MAX) return false;
	ref = ret; // Copy returned value
	return true;
}

bool AppRegistry::writeGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int value)
{
	return writeRegistryValueInt(Section::GlobalData, subSectionName, keyName, value);
}

bool AppRegistry::getGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& ref)
{
	// Get registry value
	String resultString = getRegistryValueString(Section::GlobalData, subSectionName, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	ref = resultString; // Copy returned value
	return true;
}

bool AppRegistry::writeGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
{
	return writeRegistryValueString(Section::GlobalData, subSectionName, keyName, value);
}


/*----------------------- Implementation of BackupSystem class -------------------------*/


/**
 * @brief	Constructor
 */
BackupSystem::BackupSystem()
{
}

/**
 * @brief	Destructor
 */
BackupSystem::~BackupSystem()
{
}

/**
 * @brief	Backup registry automatically by using system command
 * @return	bool - Result of process
 */
bool BackupSystem::RegistryExport()
{
	// Registry export destination file
	String destFilePath = StringUtils::makeFilePath(NULL, Constant::File::Name::Backup_Config, Constant::File::Extension::Reg);
	if (destFilePath.isEmpty()) {
		// Make file path failed
		TRACE_ERROR("Error: AutoRegistryExport fail to make destination file path!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	// Registry path
	String registryPath = Registry::RootKey::CurrentUser;
	registryPath += Constant::Symbol::Backslash;
	registryPath += Registry::Path::Application;

	// Execute registry export command
	String execCommand = StringUtils::stringFormat(Constant::Command::Registry::Export, registryPath.getString(), destFilePath.getString());
	if (!AppCore::executeCommand(execCommand, false, false)) {
		// Execute command failed
		TRACE_ERROR("Error: AutoRegistryExport fail to execute export command!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	return true;
}

