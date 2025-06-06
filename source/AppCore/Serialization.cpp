
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
//  Arguments:		lpszSectionName	   - Section name (string)
//					lpszSubSectionName - Sub section name (string)
//					lpszKeyName		   - Key name (string)
//					nValue			   - Value (int)
//  Return value:	UINT - Read value
//					BOOL - Result of writing process
//
//////////////////////////////////////////////////////////////////////////

UINT AppRegistry::GetRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName)
{
	// Format section name
	CString strSectionFormat;
	if (lpszSubSectionName != NULL) {
		// Format section full name
		strSectionFormat.Format(_T("%s\\%s"), lpszSectionName, lpszSubSectionName);
	}
	else {
		// Format section name
		strSectionFormat.Format(lpszSectionName);
	}

	// Get registry value
	return AfxGetApp()->GetProfileInt(strSectionFormat, lpszKeyName, UINT_MAX);
}

BOOL AppRegistry::WriteRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue)
{
	// Format section name
	CString strSectionFormat;
	if (lpszSubSectionName != NULL) {
		// Format section full name
		strSectionFormat.Format(_T("%s\\%s"), lpszSectionName, lpszSubSectionName);
	}
	else {
		// Format section name
		strSectionFormat.Format(lpszSectionName);
	}

	// Write registry value
	return AfxGetApp()->WriteProfileInt(strSectionFormat, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetRegistryValueString/WriteRegistryValueString
//  Description:	Using for reading/writing registry 
//					values with nested subsection
//  Arguments:		lpszSectionName	   - Section name (string)
//					lpszSubSectionName - Sub section name (string)
//					lpszKeyName		   - Key name (string)
//					lpszValue		   - Value (string)
//  Return value:	CString - Read value
//					BOOL	- Result of writing process
//
//////////////////////////////////////////////////////////////////////////

CString AppRegistry::GetRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName)
{
	// Format section name
	CString strSectionFormat;
	if (lpszSubSectionName != NULL) {
		// Format section full name
		strSectionFormat.Format(_T("%s\\%s"), lpszSectionName, lpszSubSectionName);
	}
	else {
		// Format section name
		strSectionFormat.Format(lpszSectionName);
	}

	// Get registry value
	return AfxGetApp()->GetProfileString(strSectionFormat, lpszKeyName, Constant::String::Null);
}

BOOL AppRegistry::WriteRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	// Format section name
	CString strSectionFormat;
	if (lpszSubSectionName != NULL) {
		// Format section full name
		strSectionFormat.Format(_T("%s\\%s"), lpszSectionName, lpszSubSectionName);
	}
	else {
		// Format section name
		strSectionFormat.Format(lpszSectionName);
	}

	// Write registry value
	return AfxGetApp()->WriteProfileString(strSectionFormat, lpszKeyName, lpszValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteRegistrySection
//  Description:	Using for delete registry section or subsection by name
//  Arguments:		lpszSectionName	   - Section name (string)
//					lpszSubSectionName - Sub section name (string)
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteRegistrySection(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName /* = NULL */)
{
	// Get name string
	CString strSectionFormat;
	if (lpszSubSectionName != NULL) {
		// Format section full name
		strSectionFormat.Format(_T("%s\\%s"), lpszSectionName, lpszSubSectionName);
	}
	else {
		// Format section name
		strSectionFormat.Format(lpszSectionName);
	}

	// Write registry value
	return AfxGetApp()->WriteProfileString(strSectionFormat, NULL, NULL);
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
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result integer value (ref-value)
//					nValue		- Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = AfxGetApp()->GetProfileInt(Constant::String::Empty, lpszKeyName, UINT_MAX);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(LPCTSTR lpszKeyName, int nValue)
{
	// Write registry value
	return AfxGetApp()->WriteProfileInt(Constant::String::Empty, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetProfileInfo/WriteProfileInfo
//  Description:	Using for reading/writing registry profile info values
//  Arguments:		lpszKeyName - Key name
//					strRef		- Result string value (ref-value)
//					strValue	- Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(LPCTSTR lpszKeyName, CString& strRef)
{
	// Get registry value
	CString strRet = AfxGetApp()->GetProfileString(Constant::String::Empty, lpszKeyName, Constant::String::Null);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(LPCTSTR lpszKeyName, CString strValue)
{
	// Write registry value
	return AfxGetApp()->WriteProfileString(Constant::String::Empty, lpszKeyName, strValue);
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
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result value (ref-value)
//					nValue		- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetConfig(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ConfigData, NULL, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteConfig(LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::ConfigData, NULL, lpszKeyName, nValue);
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
//	Function name:	GetDefaultSchedule/GetDefaultSchedule
//  Description:	Using for reading/writing registry default schedule values
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result value (ref-value)
//					nValue		- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetDefaultSchedule(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, NULL, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteDefaultSchedule(LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, NULL, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtraItemNum/WriteScheduleExtraItemNum
//  Description:	Using for reading/writing registry schedule extra
//					item number values
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result value (ref-value)
//					nValue		- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtraItemNum(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, NULL, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtraItemNum(LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, NULL, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtra/WriteScheduleExtra
//  Description:	Using for reading/writing registry schedule extra item values
//  Arguments:		nItemIndex  - Schedule extra item index
//					lpszKeyName - Key name
//					nRef	    - Result value (ref-value)
//					nValue	    - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtra(int nItemIndex, LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtra(int nItemIndex, LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::ScheduleData, Section::Schedule::Item(nItemIndex), lpszKeyName, nValue);
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
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result value (ref-value)
//					nValue		- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeyItemNum(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, NULL, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeyItemNum(LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, NULL, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetHotkeySet/WriteHotkeySet
//  Description:	Using for reading/writing registry hotkeyset item values
//  Arguments:		nItemIndex  - Hotkey item index
//					lpszKeyName - Key name
//					nRef	    - Result value (ref-value)
//					nValue	    - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeySet(int nItemIndex, LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeySet(int nItemIndex, LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::HotkeySetData, Section::HotkeySet::Item(nItemIndex), lpszKeyName, nValue);
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
//	Function name:	GetPwrReminderItemNum/WritePwrReminderItemNum
//  Description:	Using for reading/writing registry Power Reminder
//					item number values
//  Arguments:		lpszKeyName - Key name
//					nRef		- Result value (ref-value)
//					nValue		- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminderItemNum(LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, NULL, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminderItemNum(LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, NULL, lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex  - Hotkey item index
//					lpszKeyName - Key name
//					nRef	    - Result value (integer) (ref-value)
//					nValue	    - Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), lpszKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex	- Hotkey item index
//					lpszKeyName - Key name
//					strRef	    - Result value (string) (ref-value)
//					strValue    - Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, LPCTSTR lpszKeyName, CString& strRef)
{
	// Get registry value
	CString strRet = GetRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), lpszKeyName);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, LPCTSTR lpszKeyName, CString strValue)
{
	return WriteRegistryValueString(Section::PwrReminderData, Section::PwrReminder::Item(nItemIndex), lpszKeyName, strValue);
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
//  Arguments:		lpszSubSectionName  - Subsection name
//					lpszKeyName			- Key name
//					nRef				- Result value (ref-value)
//					nValue				- Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetLayoutInfo(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::LayoutInfo, lpszSubSectionName, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteLayoutInfo(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::LayoutInfo, lpszSubSectionName, lpszKeyName, nValue);
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
//  Arguments:		lpszSubSectionName  - Subsection name
//					lpszKeyName			- Key name
//					nRef				- Result value (integer/ref-value)
//					nValue				- Value to write (integer)
//					strRef				- Result value (integer/ref-value)
//					strValue			- Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(Section::GlobalData, lpszSubSectionName, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue)
{
	return WriteRegistryValueInt(Section::GlobalData, lpszSubSectionName, lpszKeyName, nValue);
}

BOOL AppRegistry::GetGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, CString& strRef)
{
	// Get registry value
	CString strRet = GetRegistryValueString(Section::GlobalData, lpszSubSectionName, lpszKeyName);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, CString strValue)
{
	return WriteRegistryValueString(Section::GlobalData, lpszSubSectionName, lpszKeyName, strValue);
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
	REGISTRYINFO regInfo;
	regInfo.SetRootKeyName(AppProfile::Registry::RootKey);
	regInfo.SetSubkeyPath(AppProfile::Registry::SubKeys);
	regInfo.SetCompanyName(AppProfile::Registry::CompanyName);
	regInfo.SetProductName(AppProfile::Registry::ProductID);

	// Registry export destination file
	CString strDestFilePath;
	if (!MakeFilePath(strDestFilePath, NULL, FILENAME_BAKCONFIG, FILEEXT_REGFILE)) {
		// Make file path failed
		TRACE_ERROR("Error: AutoRegistryExport fail to make destination file path!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	// Execute registry export command
	CString strExecCommand;
	strExecCommand.Format(Constant::Command::Registry::Export, MakeRegistryPath(regInfo, RegistryPathType::includingProductName), strDestFilePath.GetString());
	if (!ExecuteCommand(strExecCommand, FALSE, FALSE)) {
		// Execute command failed
		TRACE_ERROR("Error: AutoRegistryExport fail to execute export command!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

