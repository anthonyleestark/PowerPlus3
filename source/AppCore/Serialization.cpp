/**
 * @file		Serialization.cpp
 * @brief		Implement necessary methods to read/write, load/save configurations
 * @author		AnthonyLeeStark
 * @date		2024.02.03
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "AppCore/Serialization.h"

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
 * @param	nValue		   - Value (int)
 * @return	unsigned - Read value
 * @return	bool	 - Result of writing process
 */
unsigned AppRegistry::GetRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.Format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Get registry value
	return AfxGetApp()->GetProfileInt(sectionNameFormat, keyName, UINT_MAX);
}

bool AppRegistry::WriteRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.Format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Write registry value
	return AfxGetApp()->WriteProfileInt(sectionNameFormat, keyName, nValue);
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
String AppRegistry::GetRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.Format(_T("%s\\%s"), sectionName, subSectionName);
	}
	else {
		// Format section name
		sectionNameFormat = sectionName;
	}

	// Get registry value
	return AfxGetApp()->GetProfileString(sectionNameFormat, keyName, Constant::String::Null).GetString();
}

bool AppRegistry::WriteRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
{
	// Format section name
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.Format(_T("%s\\%s"), sectionName, subSectionName);
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
bool AppRegistry::DeleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName /* = NULL */)
{
	// Get name string
	String sectionNameFormat;
	if (subSectionName != NULL) {
		// Format section full name
		sectionNameFormat.Format(_T("%s\\%s"), sectionName, subSectionName);
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
 * @param	nRef	- Result integer value (ref-value)
 * @param	nValue	- Value to write (integer)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetProfileInfo(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = AfxGetApp()->GetProfileInt(Constant::String::Empty, keyName, UINT_MAX);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteProfileInfo(const wchar_t* keyName, int nValue)
{
	// Write registry value
	return AfxGetApp()->WriteProfileInt(Constant::String::Empty, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry profile info values
 * @param	keyName		- Key name
 * @param	strRef		- Result string value (ref-value)
 * @param	valueString - Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetProfileInfo(const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = AfxGetApp()->GetProfileString(Constant::String::Empty, keyName, Constant::String::Null).GetString();
	if (IS_NULL_STRING(resultString)) return false;
	strRef = resultString; // Copy returned value
	return true;
}

bool AppRegistry::WriteProfileInfo(const wchar_t* keyName, const wchar_t* valueString)
{
	// Write registry value
	return AfxGetApp()->WriteProfileString(Constant::String::Empty, keyName, valueString);
}


/*--------------- Derivered functions for reading/writing each data type ---------------*/


/**
 * @brief	Using for reading/writing registry config values
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetConfig(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ConfigData, NULL, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteConfig(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ConfigData, NULL, keyName, nValue);
}

/**
 * @brief	Using for delete config section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::DeleteConfigSection(void)
{
	return DeleteRegistrySection(Section::ConfigData);
}

/**
 * @brief	Using for reading/writing registry default schedule values
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetDefaultSchedule(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteDefaultSchedule(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry schedule extra item number values
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetScheduleExtraItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, NULL, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteScheduleExtraItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, NULL, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry schedule extra item values
 * @param	nItemIndex  - Schedule extra item index
 * @param	keyName		- Key name
 * @param	nRef	    - Result value (ref-value)
 * @param	nValue	    - Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetScheduleExtra(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteScheduleExtra(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), keyName, nValue);
}

/**
 * @brief	Using for delete schedule section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::DeleteScheduleSection(void)
{
	bool bRet = true;

	// Delete default schedule subsection
	bRet &= DeleteRegistrySection(Section::ScheduleData, Section::Schedule::DefautItem);

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, nSubItemNum);
	if (bRet == false) return false;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::ScheduleData, Section::Schedule::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::ScheduleData);

	return bRet;
}

/**
 * @brief	Using for reading/writing registry hotkeyset item number values
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetHotkeyItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, NULL, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteHotkeyItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, NULL, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry hotkeyset item values
 * @param	nItemIndex  - Hotkey item index
 * @param	keyName		- Key name
 * @param	nRef	    - Result value (ref-value)
 * @param	nValue	    - Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetHotkeySet(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteHotkeySet(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), keyName, nValue);
}

/**
 * @brief	Using for delete HotkeySet section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::DeleteHotkeySetSection(void)
{
	bool bRet = true;

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetHotkeyItemNum(Key::HotkeySetData::ItemNum, nSubItemNum);
	if (bRet == false) return false;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::HotkeySetData, Section::HotkeySet::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::HotkeySetData);

	return bRet;
}

/**
 * @brief	Using for reading/writing registry Power Reminder common style data
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetPwrReminderCommonStyle(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WritePwrReminderCommonStyle(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry Power Reminder common style data
 * @param	keyName - Key name
 * @param	strRef	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetPwrReminderCommonStyle(const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	strRef = resultString; // Copy returned value
	return true;
}

bool AppRegistry::WritePwrReminderCommonStyle(const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, value);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item number values
 * @param	keyName - Key name
 * @param	nRef	- Result value (ref-value)
 * @param	nValue	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetPwrReminderItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, NULL, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WritePwrReminderItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, NULL, keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item values
 * @param	nItemIndex  - Hotkey item index
 * @param	keyName - Key name
 * @param	nRef	- Result value (integer) (ref-value)
 * @param	nValue	- Value to write (integer)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetPwrReminder(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WritePwrReminder(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName, nValue);
}

/**
 * @brief	Using for reading/writing registry Power Reminder item values
 * @param	nItemIndex	- Hotkey item index
 * @param	keyName		- Key name
 * @param	strRef	    - Result value (string) (ref-value)
 * @param	strValue    - Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetPwrReminder(int nItemIndex, const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName);
	if (IS_NULL_STRING(resultString)) return false;
	strRef = resultString; // Copy returned value
	return true;
}

bool AppRegistry::WritePwrReminder(int nItemIndex, const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName, value);
}

/**
 * @brief	Using for delete Power Reminder section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::DeletePwrReminderSection(void)
{
	bool bRet = true;

	// Delete common style section
	bRet &= DeleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::CommonStyle);

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetPwrReminderItemNum(Key::PwrReminderData::ItemNum, nSubItemNum);
	if (bRet == false) return false;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::PwrReminderData);

	return bRet;
}

/**
 * @brief	Using for reading/writing registry layout info values
 * @param	subSectionName  - Subsection name
 * @param	keyName			- Key name
 * @param	nRef			- Result value (ref-value)
 * @param	nValue			- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::LayoutInfo, subSectionName, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::LayoutInfo, subSectionName, keyName, nValue);
}

/**
 * @brief	Using for delete config section
 * @param	None
 * @return	true/false - Return of deletion
 */
bool AppRegistry::DeleteLayoutInfoSection(void)
{
	return DeleteRegistrySection(Section::LayoutInfo);
}

/**
 * @brief	Using for reading/writing registry system event tracking data
 * @param	keyName - Key name
 * @param	strRef	- Result value (ref-value)
 * @param	value	- Value to write
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetSysEventTracking(const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::SystemEventTracking, NULL, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	strRef = resultString; // Copy returned value
	return true;
}

bool AppRegistry::WriteSysEventTracking(const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::SystemEventTracking, NULL, keyName, value);
}

/**
 * @brief	Using for reading/writing registry global data values
 * @param	subSectionName  - Subsection name
 * @param	keyName			- Key name
 * @param	nRef			- Result value (integer/ref-value)
 * @param	nValue			- Value to write (integer)
 * @param	strRef			- Result value (integer/ref-value)
 * @param	strValue		- Value to write (string)
 * @return	bool - Result of reading/writing process
 */
bool AppRegistry::GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::GlobalData, subSectionName, keyName);
	if (nRet == UINT_MAX) return false;
	nRef = nRet; // Copy returned value
	return true;
}

bool AppRegistry::WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::GlobalData, subSectionName, keyName, nValue);
}

bool AppRegistry::GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::GlobalData, subSectionName, keyName);
	if (IS_NULL_STRING(resultString)) return false;
	strRef = resultString; // Copy returned value
	return true;
}

bool AppRegistry::WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::GlobalData, subSectionName, keyName, value);
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
	String destFilePath = StringUtils::MakeFilePath(NULL, FILENAME_BAKCONFIG, FILEEXT_REGFILE);
	if (destFilePath.IsEmpty()) {
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
	String execCommand = StringUtils::StringFormat(Constant::Command::Registry::Export, registryPath.GetString(), destFilePath.GetString());
	if (!ExecuteCommand(execCommand, false, false)) {
		// Execute command failed
		TRACE_ERROR("Error: AutoRegistryExport fail to execute export command!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return false;
	}

	return true;
}

