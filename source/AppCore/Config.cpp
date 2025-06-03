
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Config.cpp
//		Description:	Implement necessary methods to read/write, load/save configurations
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.03:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AppCore/Config.h"

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
	return AfxGetApp()->GetProfileString(strSectionFormat, lpszKeyName, STRING_NULL);
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
//	Function name:	GetRegistryValueInt/WriteRegistryValueInt
//  Description:	Using for reading/writing registry values 
//					with nested subsection by string ID
//  Arguments:		nSectionName	- Section name (string ID)
//					nSubSectionName - Sub section name (string ID)
//					nKeyName		- Key name (string ID)
//					nValue			- Value (int)
//  Return value:	UINT - Read value
//					BOOL - Result of writing process
//
//////////////////////////////////////////////////////////////////////////

UINT AppRegistry::GetRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName)
{
	// Get name string
	CString strSectionName, strSubSectionName, strKeyName;
	VERIFY(strSectionName.LoadString(nSectionName));
	VERIFY(strKeyName.LoadString(nKeyName));
	if (nSubSectionName != NULL) {
		// Get subsection name string
		VERIFY(strSubSectionName.LoadString(nSubSectionName));
		return GetRegistryValueInt(strSectionName, strSubSectionName, strKeyName);
	}
	else {
		// No subsection
		return GetRegistryValueInt(strSectionName, NULL, strKeyName);
	}
}

BOOL AppRegistry::WriteRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, int nValue)
{
	// Get name string
	CString strSectionName, strSubSectionName, strKeyName;
	VERIFY(strSectionName.LoadString(nSectionName));
	VERIFY(strKeyName.LoadString(nKeyName));
	if (nSubSectionName != NULL) {
		// Get subsection name string
		VERIFY(strSubSectionName.LoadString(nSubSectionName));
		return WriteRegistryValueInt(strSectionName, strSubSectionName, strKeyName, nValue);
	}
	else {
		// No subsection
		return WriteRegistryValueInt(strSectionName, NULL, strKeyName, nValue);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetRegistryValueString/WriteRegistryValueString
//  Description:	Using for reading/writing registry values 
//					with nested subsection by string ID
//  Arguments:		nSectionName	- Section name (string ID)
//					nSubSectionName - Sub section name (string ID)
//					nKeyName		- Key name (string ID)
//					lpszValue		- Value (string)
//  Return value:	CString - Read value
//					BOOL	- Result of writing process
//
//////////////////////////////////////////////////////////////////////////

CString AppRegistry::GetRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName)
{
	// Get name string
	CString strSectionName, strSubSectionName, strKeyName;
	VERIFY(strSectionName.LoadString(nSectionName));
	VERIFY(strKeyName.LoadString(nKeyName));
	if (nSubSectionName != NULL) {
		// Get subsection name string
		VERIFY(strSubSectionName.LoadString(nSubSectionName));
		return GetRegistryValueString(strSectionName, strSubSectionName, strKeyName);
	}
	else {
		// No subsection
		return GetRegistryValueString(strSectionName, NULL, strKeyName);
	}
}

BOOL AppRegistry::WriteRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, LPCTSTR lpszValue)
{
	// Get name string
	CString strSectionName, strSubSectionName, strKeyName;
	VERIFY(strSectionName.LoadString(nSectionName));
	VERIFY(strKeyName.LoadString(nKeyName));
	if (nSubSectionName != NULL) {
		// Get subsection name string
		VERIFY(strSubSectionName.LoadString(nSubSectionName));
		return WriteRegistryValueString(strSectionName, strSubSectionName, strKeyName, lpszValue);
	}
	else {
		// No subsection
		return WriteRegistryValueString(strSectionName, NULL, strKeyName, lpszValue);
	}
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteRegistrySection
//  Description:	Using for delete registry section or subsection by name
//  Arguments:		nSectionName	   - Section name (string ID)
//					nSubSectionName	   - Sub section name (string ID)
//					lpszSectionName	   - Section name (string)
//					lpszSubSectionName - Sub section name (string)
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::DeleteRegistrySection(UINT nSectionName, UINT nSubSectionName /* = NULL */)
{
	// Get name string
	CString strSectionName, strSubSectionName;
	VERIFY(strSectionName.LoadString(nSectionName));
	if (nSubSectionName != NULL) {
		// Get subsection name string
		VERIFY(strSubSectionName.LoadString(nSubSectionName));
		return DeleteRegistrySection(strSectionName, strSubSectionName);
	}
	else {
		// No subsection
		return DeleteRegistrySection(strSectionName, NULL);
	}
}

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
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result integer value (ref-value)
//					nValue	 - Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(UINT nKeyName, int& nRef)
{
	// Key name
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	int nRet = AfxGetApp()->GetProfileInt(STRING_EMPTY, strKeyName, UINT_MAX);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(UINT nKeyName, int nValue)
{
	// Key name
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Write registry value
	return AfxGetApp()->WriteProfileInt(STRING_EMPTY, strKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetProfileInfo/WriteProfileInfo
//  Description:	Using for reading/writing registry profile info values
//  Arguments:		nKeyName - Key name (string ID)
//					strRef	 - Result string value (ref-value)
//					strValue - Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetProfileInfo(UINT nKeyName, CString& strRef)
{
	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	CString strRet = AfxGetApp()->GetProfileString(STRING_EMPTY, strKeyName, STRING_NULL);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteProfileInfo(UINT nKeyName, CString strValue)
{
	// Key name
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Write registry value
	return AfxGetApp()->WriteProfileString(STRING_EMPTY, strKeyName, strValue);
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
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetConfig(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_CONFIG, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteConfig(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_CONFIG, NULL, nKeyName, nValue);
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
	return DeleteRegistrySection(IDS_REGSECTION_CONFIG);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetDefaultSchedule/GetDefaultSchedule
//  Description:	Using for reading/writing registry default schedule values
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetDefaultSchedule(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteDefaultSchedule(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtraItemNum/WriteScheduleExtraItemNum
//  Description:	Using for reading/writing registry schedule extra
//					item number values
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtraItemNum(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtraItemNum(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetScheduleExtra/WriteScheduleExtra
//  Description:	Using for reading/writing registry schedule extra item values
//  Arguments:		nItemIndex - Schedule extra item index
//					nKeyName   - Key name (string ID)
//					nRef	   - Result value (ref-value)
//					nValue	   - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetScheduleExtra(int nItemIndex, UINT nKeyName, int& nRef)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_SCHEDULE);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_SCHEDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	int nRet = GetRegistryValueInt(strSectionName, strSubSectionName, strKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteScheduleExtra(int nItemIndex, UINT nKeyName, int nValue)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_SCHEDULE);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_SCHEDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	return WriteRegistryValueInt(strSectionName, strSubSectionName, strKeyName, nValue);
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
	bRet &= GetScheduleExtraItemNum(IDS_REGKEY_SCHEDULE_ITEMNUM, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Get section name
	CString strSectionName;
	VERIFY(strSectionName.LoadString(IDS_REGSECTION_SCHEDULE));

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		// Format subsection name
		CString strSubSectionName;
		strSubSectionName.Format(IDS_REGSECTION_SCHEDITEMID, nIndex);

		// Delete subsection
		bRet &= DeleteRegistrySection(strSectionName, strSubSectionName);
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(IDS_REGSECTION_SCHEDULE);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetHotkeyItemNum/WriteHotkeyItemNum
//  Description:	Using for reading/writing registry hotkeyset
//					item number values
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeyItemNum(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_HOTKEYSET, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeyItemNum(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_HOTKEYSET, NULL, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetHotkeySet/WriteHotkeySet
//  Description:	Using for reading/writing registry hotkeyset item values
//  Arguments:		nItemIndex - Hotkey item index
//					nKeyName   - Key name (string ID)
//					nRef	   - Result value (ref-value)
//					nValue	   - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetHotkeySet(int nItemIndex, UINT nKeyName, int& nRef)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_HOTKEYSET);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_HKEYITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	int nRet = GetRegistryValueInt(strSectionName, strSubSectionName, strKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteHotkeySet(int nItemIndex, UINT nKeyName, int nValue)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_HOTKEYSET);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_HKEYITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	return WriteRegistryValueInt(strSectionName, strSubSectionName, strKeyName, nValue);
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
	bRet &= GetHotkeyItemNum(IDS_REGKEY_HKEYSET_ITEMNUM, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Get section name
	CString strSectionName;
	VERIFY(strSectionName.LoadString(IDS_REGSECTION_HOTKEYSET));

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		// Format subsection name
		CString strSubSectionName;
		strSubSectionName.Format(IDS_REGSECTION_HKEYITEMID, nIndex);

		// Delete subsection
		bRet &= DeleteRegistrySection(strSectionName, strSubSectionName);
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(IDS_REGSECTION_HOTKEYSET);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminderItemNum/WritePwrReminderItemNum
//  Description:	Using for reading/writing registry Power Reminder
//					item number values
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminderItemNum(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_PWRREMINDER, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminderItemNum(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_PWRREMINDER, NULL, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex - Hotkey item index
//					nKeyName   - Key name (string ID)
//					nRef	   - Result value (integer) (ref-value)
//					nValue	   - Value to write (integer)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, UINT nKeyName, int& nRef)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_PWRREMINDER);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_PWRRMDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	int nRet = GetRegistryValueInt(strSectionName, strSubSectionName, strKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, UINT nKeyName, int nValue)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_PWRREMINDER);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_PWRRMDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	return WriteRegistryValueInt(strSectionName, strSubSectionName, strKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetPwrReminder/WritePwrReminder
//  Description:	Using for reading/writing registry Power Reminder item values
//  Arguments:		nItemIndex - Hotkey item index
//					nKeyName   - Key name (string ID)
//					strRef	   - Result value (string) (ref-value)
//					strValue   - Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetPwrReminder(int nItemIndex, UINT nKeyName, CString& strRef)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_PWRREMINDER);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_PWRRMDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));

	// Get registry value
	CString strRet = GetRegistryValueString(strSectionName, strSubSectionName, strKeyName);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WritePwrReminder(int nItemIndex, UINT nKeyName, CString strValue)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_PWRREMINDER);
	CString strSubSectionName;
	strSubSectionName.Format(IDS_REGSECTION_PWRRMDITEMID, nItemIndex);

	// Get name string
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyName));
	return WriteRegistryValueString(strSectionName, strSubSectionName, strKeyName, strValue);
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
	bRet &= GetPwrReminderItemNum(IDS_REGKEY_PWRRMD_ITEMNUM, nSubItemNum);
	if (bRet == FALSE) return FALSE;

	// Get section name
	CString strSectionName;
	VERIFY(strSectionName.LoadString(IDS_REGSECTION_PWRREMINDER));

	// Delete subsection of items
	for (int nIndex = 0; nIndex < nSubItemNum; nIndex++) {
		// Format subsection name
		CString strSubSectionName;
		strSubSectionName.Format(IDS_REGSECTION_PWRRMDITEMID, nIndex);

		// Delete subsection
		bRet &= DeleteRegistrySection(strSectionName, strSubSectionName);
	}

	// Delete parent section
	bRet &= DeleteRegistrySection(IDS_REGSECTION_PWRREMINDER);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetLayoutInfo/WriteLayoutInfo
//  Description:	Using for reading/writing registry layout info values
//  Arguments:		nSectionName - Section name
//					nKeyName	 - Key name (string ID)
//					nRef		 - Result value (ref-value)
//					nValue		 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetLayoutInfo(UINT nSectionName, UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_LAYOUTINFO, nSectionName, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteLayoutInfo(UINT nSectionName, UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_LAYOUTINFO, nSectionName, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetLayoutInfo/WriteLayoutInfo
//  Description:	Using for reading/writing registry layout info values
//  Arguments:		nSectionName - Section name
//					lpszKeyName	 - Key name (string)
//					nRef		 - Result value (ref-value)
//					nValue		 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetLayoutInfo(UINT nSectionName, LPCTSTR lpszKeyName, int& nRef)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_LAYOUTINFO);
	CString strSubSectionName;
	strSubSectionName.LoadString(nSectionName);

	// Get registry value
	int nRet = GetRegistryValueInt(strSectionName, strSubSectionName, lpszKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteLayoutInfo(UINT nSectionName, LPCTSTR lpszKeyName, int nValue)
{
	// Get name string
	CString strSectionName;
	strSectionName.LoadString(IDS_REGSECTION_LAYOUTINFO);
	CString strSubSectionName;
	strSubSectionName.LoadString(nSectionName);

	return WriteRegistryValueInt(strSectionName, strSubSectionName, lpszKeyName, nValue);
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
	return DeleteRegistrySection(IDS_REGSECTION_LAYOUTINFO);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetGlobalData/WriteGlobalData
//  Description:	Using for reading/writing registry global data values
//  Arguments:		nSubSection - Subsection name (string ID)
//					nKeyName	- Key name (string ID)
//					nRef		- Result value (integer/ref-value)
//					nValue		- Value to write (integer)
//					strRef		- Result value (integer/ref-value)
//					strValue	- Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL AppRegistry::GetGlobalData(UINT nSubSection, UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_GLBDATA, nSubSection, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(UINT nSubSection, UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_GLBDATA, nSubSection, nKeyName, nValue);
}

BOOL AppRegistry::GetGlobalData(UINT nSubSection, UINT nKeyName, CString& strRef)
{
	// Get registry value
	CString strRet = GetRegistryValueString(IDS_REGSECTION_GLBDATA, nSubSection, nKeyName);
	if (IS_NULL_STRING(strRet)) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL AppRegistry::WriteGlobalData(UINT nSubSection, UINT nKeyName, CString strValue)
{
	return WriteRegistryValueString(IDS_REGSECTION_GLBDATA, nSubSection, nKeyName, strValue);
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
	regInfo.SetRootKeyName(IDS_APP_REGISTRY_HKEY);
	regInfo.SetSubkeyPath(IDS_APP_REGISTRY_SUBKEYPATH);
	regInfo.SetProfileName(IDS_APP_REGISTRY_PROFILENAME);
	regInfo.SetAppName(IDS_APP_REGISTRY_APPNAME);

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
	strExecCommand.Format(COMMAND_REGISTRY_EXPORT, MakeRegistryPath(regInfo, RegistryPathType::includingAppName), strDestFilePath);
	if (!ExecuteCommand(strExecCommand, FALSE, FALSE)) {
		// Execute command failed
		TRACE_ERROR("Error: AutoRegistryExport fail to execute export command!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return FALSE;
	}

	return TRUE;
}

