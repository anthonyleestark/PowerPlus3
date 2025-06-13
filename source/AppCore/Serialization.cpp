
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Serialization.cpp
//		Description:	Implement necessary methods to read/write, load/save configurations
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

#include "AppCore/Serialization.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace AppCore;

/////////////////////////////////////////////////////////////////////////
//// Implementations

//////////////////////////////////////////////////////////////////////////
//
//	Base functions for reading/writing registry values
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetRegistryValueInt/WriteRegistryValueInt
//  Description:	Using for reading/writing registry 
//					values with nested subsection
//  Arguments:		sectionName	   - Section name (string)
//					subSectionName - Sub section name (string)
//					keyName		   - Key name (string)
//					nValue		   - Value (int)
//  Return value:	UINT - Read value
//					BOOL - Result of writing process
//
//////////////////////////////////////////////////////////////////////////

UINT AppRegistry::GetRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName)
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

BOOL AppRegistry::WriteRegistryValueInt(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
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

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetRegistryValueString/WriteRegistryValueString
//  Description:	Using for reading/writing registry 
//					values with nested subsection
//  Arguments:		sectionName	   - Section name (string)
//					subSectionName - Sub section name (string)
//					keyName		   - Key name (string)
//					value		   - Value (string)
//  Return value:	String - Read value
//					BOOL   - Result of writing process
//
//////////////////////////////////////////////////////////////////////////

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

BOOL AppRegistry::WriteRegistryValueString(const wchar_t* sectionName, const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
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

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteRegistrySection
//  Description:	Using for delete registry section or subsection by name
//  Arguments:		sectionName	   - Section name (string)
//					subSectionName - Sub section name (string)
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteRegistrySection(const wchar_t* sectionName, const wchar_t* subSectionName /* = NULL */)
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


//////////////////////////////////////////////////////////////////////////
//
//	Functions for reading/writing application profile info
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetProfileInfo/WriteProfileInfo
//  Description:	Using for reading/writing registry profile info values
//  Arguments:		keyName	- Key name
//					nRef	- Result integer value (ref-value)
//					nValue	- Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = AfxGetApp()->GetProfileInt(Constant::String::Empty, keyName, UINT_MAX);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(const wchar_t* keyName, int nValue)
{
	// Write registry value
	return AfxGetApp()->WriteProfileInt(Constant::String::Empty, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetProfileInfo/WriteProfileInfo
//  Description:	Using for reading/writing registry profile info values
//  Arguments:		keyName		- Key name
//					strRef		- Result string value (ref-value)
//					valueString - Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = AfxGetApp()->GetProfileString(Constant::String::Empty, keyName, Constant::String::Null).GetString();
	if (IS_NULL_STRING(resultString)) return FALSE;
	strRef = resultString; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(const wchar_t* keyName, const wchar_t* valueString)
{
	// Write registry value
	return AfxGetApp()->WriteProfileString(Constant::String::Empty, keyName, valueString);
}


//////////////////////////////////////////////////////////////////////////
//
//	Derivered functions for reading/writing each data type
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetConfig/WriteConfig
//  Description:	Using for reading/writing registry config values
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetConfig(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ConfigData, NULL, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteConfig(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ConfigData, NULL, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteConfigSection
//  Description:	Using for delete config section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteConfigSection(void)
{
	return DeleteRegistrySection(Section::ConfigData);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetDefaultSchedule/WriteDefaultSchedule
//  Description:	Using for reading/writing registry default schedule values
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetDefaultSchedule(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteDefaultSchedule(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, Section::Schedule::DefautItem, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtraItemNum/WriteScheduleExtraItemNum
//  Description:	Using for reading/writing registry schedule extra
//					item number values
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtraItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, NULL, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtraItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, NULL, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtra/WriteScheduleExtra
//  Description:	Using for reading/writing registry schedule extra item values
//  Arguments:		nItemIndex  - Schedule extra item index
//					keyName		- Key name
//					nRef	    - Result value (ref-value)
//					nValue	    - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtra(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtra(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteScheduleSection
//  Description:	Using for delete schedule section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteScheduleSection(void)
{
	BOOL bRet = TRUE;

	// Delete default schedule subsection
	bRet &= DeleteRegistrySection(Section::ScheduleData, Section::Schedule::DefautItem);

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetScheduleExtraItemNum(Key::ScheduleData::ExtraItemNum, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::ScheduleData, Section::Schedule::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::ScheduleData);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetHotkeyItemNum/WriteHotkeyItemNum
//  Description:	Using for reading/writing registry hotkeyset
//					item number values
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeyItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, NULL, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeyItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, NULL, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetHotkeySet/WriteHotkeySet
//  Description:	Using for reading/writing registry hotkeyset item values
//  Arguments:		nItemIndex  - Hotkey item index
//					keyName		- Key name
//					nRef	    - Result value (ref-value)
//					nValue	    - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeySet(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeySet(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteHotkeySetSection
//  Description:	Using for delete HotkeySet section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteHotkeySetSection(void)
{
	BOOL bRet = TRUE;

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetHotkeyItemNum(Key::HotkeySetData::ItemNum, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::HotkeySetData, Section::HotkeySet::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::HotkeySetData);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminderCommonStyle/WritePwrReminderCommonStyle
//  Description:	Using for reading/writing registry Power Reminder common style data
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminderCommonStyle(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminderCommonStyle(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminderCommonStyle/WritePwrReminderCommonStyle
//  Description:	Using for reading/writing registry Power Reminder common style data
//  Arguments:		keyName - Key name
//					strRef	- Result value (ref-value)
//					value	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminderCommonStyle(const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName);
	if (IS_NULL_STRING(resultString)) return FALSE;
	strRef = resultString; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminderCommonStyle(const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::PwrReminderData, Section::PwrReminder::CommonStyle, keyName, value);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminderItemNum/WritePwrReminderItemNum
//  Description:	Using for reading/writing registry Power Reminder
//					item number values
//  Arguments:		keyName - Key name
//					nRef	- Result value (ref-value)
//					nValue	- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminderItemNum(const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, NULL, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminderItemNum(const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, NULL, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex  - Hotkey item index
//					keyName - Key name
//					nRef	- Result value (integer) (ref-value)
//					nValue	- Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex	- Hotkey item index
//					keyName		- Key name
//					strRef	    - Result value (string) (ref-value)
//					strValue    - Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName);
	if (IS_NULL_STRING(resultString)) return FALSE;
	strRef = resultString; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), keyName, value);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeletePwrReminderSection
//  Description:	Using for delete Power Reminder section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeletePwrReminderSection(void)
{
	BOOL bRet = TRUE;

	// Delete common style section
	bRet &= DeleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::CommonStyle);

	// Get subsection number
	int nSubItemNum = 0;
	bRet &= GetPwrReminderItemNum(Key::PwrReminderData::ItemNum, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		bRet &= DeleteRegistrySection(Section::PwrReminderData, Section::PwrReminder::Item(nIndex));
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(Section::PwrReminderData);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetLayoutInfo/WriteLayoutInfo
//  Description:	Using for reading/writing registry layout info values
//  Arguments:		subSectionName  - Subsection name
//					keyName			- Key name
//					nRef			- Result value (ref-value)
//					nValue			- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::LayoutInfo, subSectionName, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteLayoutInfo(const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::LayoutInfo, subSectionName, keyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteLayoutInfoSection
//  Description:	Using for delete config section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteLayoutInfoSection(void)
{
	return DeleteRegistrySection(Section::LayoutInfo);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetGlobalData/WriteGlobalData
//  Description:	Using for reading/writing registry global data values
//  Arguments:		subSectionName  - Subsection name
//					keyName			- Key name
//					nRef			- Result value (integer/ref-value)
//					nValue			- Value to write (integer)
//					strRef			- Result value (integer/ref-value)
//					strValue		- Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::GlobalData, subSectionName, keyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, int nValue)
{
	return WriteRegistryValueInt(Section::GlobalData, subSectionName, keyName, nValue);
}

BOOL AppRegistry::GetGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, String& strRef)
{
	// Get registry value
	String resultString = GetRegistryValueString(Section::GlobalData, subSectionName, keyName);
	if (IS_NULL_STRING(resultString)) return FALSE;
	strRef = resultString; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(const wchar_t* subSectionName, const wchar_t* keyName, const wchar_t* value)
{
	return WriteRegistryValueString(Section::GlobalData, subSectionName, keyName, value);
}


//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	BackupSystem
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

BackupSystem::BackupSystem()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~BackupSystem
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

BackupSystem::~BackupSystem()
{
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegistryExport
//	Description:	Backup registry automatically by using system command
//  Arguments:		None
//  Return value:	BOOL - Result of process
//
//////////////////////////////////////////////////////////////////////////

BOOL BackupSystem::RegistryExport()
{
	// Initialize registry info
	RegistryInfo regInfo;
	regInfo.SetRootKeyName(AppProfile::Registry::RootKey);
	regInfo.SetSubkeyPath(AppProfile::Registry::SubKeys);
	regInfo.SetCompanyName(AppProfile::Registry::CompanyName);
	regInfo.SetProductName(AppProfile::Registry::ProductID);

	// Registry export destination file
	String destFilePath = StringUtils::MakeFilePath(NULL, FILENAME_BAKCONFIG, FILEEXT_REGFILE);
	if (!destFilePath.IsEmpty()) {
		// Make file path failed
		TRACE_ERROR("Error: AutoRegistryExport fail to make destination file path!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Execute registry export command
	String execCommand = StringUtils::StringFormat(Constant::Command::Registry::Export, MakeRegistryPath(regInfo, RegistryPathType::includingProductName), destFilePath.GetString());
	if (!ExecuteCommand(execCommand, FALSE, FALSE)) {
		// Execute command failed
		TRACE_ERROR("Error: AutoRegistryExport fail to execute export command!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

