
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Config.cpp
//		Description:	Implement necessary methods to read/write, load/save configurations
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.03:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Config.h"

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

UINT RegFuncs::GetRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName)
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

BOOL RegFuncs::WriteRegistryValueInt(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, int nValue)
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

CString RegFuncs::GetRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName)
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
	return AfxGetApp()->GetProfileString(strSectionFormat, lpszKeyName, DEF_STRING_NULL);
}

BOOL RegFuncs::WriteRegistryValueString(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszValue)
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

UINT RegFuncs::GetRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName)
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

BOOL RegFuncs::WriteRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, int nValue)
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

CString RegFuncs::GetRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName)
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

BOOL RegFuncs::WriteRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, LPCTSTR lpszValue)
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

BOOL RegFuncs::DeleteRegistrySection(UINT nSectionName, UINT nSubSectionName /* = NULL */)
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

BOOL RegFuncs::DeleteRegistrySection(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName /* = NULL */)
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

BOOL RegFuncs::GetConfig(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_CONFIG, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WriteConfig(UINT nKeyName, int nValue)
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

BOOL RegFuncs::DeleteConfigSection(void)
{
	return DeleteRegistrySection(IDS_REGSECTION_CONFIG);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSchedule/WriteSchedule
//  Description:	Using for reading/writing registry schedule values
//  Arguments:		nKeyName - Key name (string ID)
//					nRef	 - Result value (ref-value)
//					nValue	 - Value to write
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL RegFuncs::GetSchedule(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WriteSchedule(UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_SCHEDULE, NULL, nKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	DeleteScheduleSection
//  Description:	Using for delete schedule section
//  Arguments:		None
//  Return value:	TRUE/FALSE - Return of deletion
//
//////////////////////////////////////////////////////////////////////////

BOOL RegFuncs::DeleteScheduleSection(void)
{
	return DeleteRegistrySection(IDS_REGSECTION_SCHEDULE);
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

BOOL RegFuncs::GetHotkeyItemNum(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_HOTKEYSET, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WriteHotkeyItemNum(UINT nKeyName, int nValue)
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

BOOL RegFuncs::GetHotkeySet(int nItemIndex, UINT nKeyName, int& nRef)
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

BOOL RegFuncs::WriteHotkeySet(int nItemIndex, UINT nKeyName, int nValue)
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

BOOL RegFuncs::DeleteHotkeySetSection(void)
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

	// Delete section
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

BOOL RegFuncs::GetPwrReminderItemNum(UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_PWRREMINDER, NULL, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WritePwrReminderItemNum(UINT nKeyName, int nValue)
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

BOOL RegFuncs::GetPwrReminder(int nItemIndex, UINT nKeyName, int& nRef)
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

BOOL RegFuncs::WritePwrReminder(int nItemIndex, UINT nKeyName, int nValue)
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

BOOL RegFuncs::GetPwrReminder(int nItemIndex, UINT nKeyName, CString& strRef)
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
	if (strRet == DEF_STRING_NULL) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WritePwrReminder(int nItemIndex, UINT nKeyName, CString strValue)
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

BOOL RegFuncs::DeletePwrReminderSection(void)
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

	// Delete section
	bRet &= DeleteRegistrySection(IDS_REGSECTION_PWRREMINDER);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetGlobalVar/WriteGlobalVar
//  Description:	Using for reading/writing registry global variable values
//  Arguments:		nSubSection - Subsection name (string ID)
//					nKeyName	- Key name (string ID)
//					nRef		- Result value (integer/ref-value)
//					nValue		- Value to write (integer)
//					strRef		- Result value (integer/ref-value)
//					strValue	- Value to write (string)
//  Return value:	BOOL - Result of reading/writing process
//
//////////////////////////////////////////////////////////////////////////

BOOL RegFuncs::GetGlobalVar(UINT nSubSection, UINT nKeyName, int& nRef)
{
	// Get registry value
	int nRet = GetRegistryValueInt(IDS_REGSECTION_GLBVAR, nSubSection, nKeyName);
	if (nRet == UINT_MAX) return FALSE;
	nRef = nRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WriteGlobalVar(UINT nSubSection, UINT nKeyName, int nValue)
{
	return WriteRegistryValueInt(IDS_REGSECTION_GLBVAR, nSubSection, nKeyName, nValue);
}

BOOL RegFuncs::GetGlobalVar(UINT nSubSection, UINT nKeyName, CString& strRef)
{
	// Get registry value
	CString strRet = GetRegistryValueString(IDS_REGSECTION_GLBVAR, nSubSection, nKeyName);
	if (strRet == DEF_STRING_NULL) return FALSE;
	strRef = strRet; // Copy returned value
	return TRUE;
}

BOOL RegFuncs::WriteGlobalVar(UINT nSubSection, UINT nKeyName, CString strValue)
{
	return WriteRegistryValueString(IDS_REGSECTION_GLBVAR, nSubSection, nKeyName, strValue);
}


///////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Class name:	 SConfigBackup
//  Description: Using for saving backup configurations
//
//////////////////////////////////////////////////////////////////////////

SConfigBackup::SConfigBackup()
{
	m_pBakFile = new CFile;
	m_strContent.Empty();
}

SConfigBackup::~SConfigBackup()
{
	if (m_pBakFile != NULL) {
		delete m_pBakFile;
		m_pBakFile = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	AutoRegistryExport
//	Description:	Backup registry automatically by using system command
//  Arguments:		None
//  Return value:	BOOL - Result of process
//
//////////////////////////////////////////////////////////////////////////

BOOL SConfigBackup::AutoRegistryExport()
{
	REGISTRYKEY regKeyInfo;
	regKeyInfo.nTagNameID = IDS_APP_REGISTRY_TAGNAME;
	regKeyInfo.nSubPathID = IDS_APP_REGISTRY_SUBPATH;
	regKeyInfo.nMasterKeyNameID = IDS_APP_REGISTRY_MASTERKEY;
	regKeyInfo.nAppNameID = IDS_APP_REGISTRY_APPNAME;

	CString strCmd;
	CString strCmdFormat = _T("\"reg.exe export \"%s\" \"%s\"\" /y");
	strCmd.Format(strCmdFormat, MakeRegFullPath(regKeyInfo, REGPATH_NOSECTION), FILE_BAK_CONFIG);
	ExecuteCommand(strCmd, FALSE, FALSE);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PrepareBakFile
//	Description:	Prepare backup file for writing
//  Arguments:		None
//  Return value:	BOOL - Result of process
//
//////////////////////////////////////////////////////////////////////////

BOOL SConfigBackup::PrepareBakFile()
{
	if (m_pBakFile == NULL) {
		TRCFFMT(__FUNCTION__, "pBakFile is NULL");
		return FALSE;
	}

	BOOL bRet = m_pBakFile->Open(FILE_BAK_CONFIG, CFile::modeWrite | CFile::typeText | CFile::modeCreate);

	WriteLine(_T("Windows Registry Editor Version 5.00"));
	WriteLine();
	WriteLine(_T("; Power++ 3.0 registry"));
	WriteLine(_T("; Generated by AnthonyLeeStark"));
	WriteLine();
	WriteLine(_T("[HKEY_CURRENT_USER\\SOFTWARE\\Stark Personal\\PowerPlus3]"));
	WriteLine();

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteLine
//	Description:	Write a specific stringID to file
//  Arguments:		nResourceStringID - Resource string ID
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::WriteLine(UINT nResourceStringID)
{
	CString strText;
	VERIFY(strText.LoadString(nResourceStringID));
	WriteLine(strText);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteLine
//	Description:	Write a text line to file
//  Arguments:		strText - Output text
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::WriteLine(CString strText)
{
	if (strText.IsEmpty())
		m_strContent += _T("\n");
	else
		m_strContent += strText + _T("\n");
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteValue
//	Description:	Write a key value with KeyID
//  Arguments:		nKeyID - Key ID
//					nValue - Value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::WriteValue(UINT nKeyID, UINT nValue)
{
	CString strKeyName; 
	VERIFY(strKeyName.LoadString(nKeyID));
	WriteValue(strKeyName, nValue);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WriteValue
//	Description:	Write a key value to file
//  Arguments:		strKeyName - Key name (string)
//					nValue	   - Value
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::WriteValue(CString strKeyName, UINT nValue)
{
	CString strText;
	strText.Format(_T("\"%s\"=dword:%08x\n"), strKeyName, nValue);
	m_strContent += strText;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateBakFile
//	Description:	Push all string contents into file
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::UpdateBakFile()
{
	if (m_pBakFile == NULL) {
		TRCFFMT(__FUNCTION__, "pBakFile is NULL");
		return;
	}
	else if (m_pBakFile->m_hFile == CFile::hFileNull) {
		TRCFFMT(__FUNCTION__, "Backup file is not opening");
		return;
	}
	else if (m_strContent.IsEmpty()) {
		TRCFFMT(__FUNCTION__, "Content is empty");
		return;
	}

	m_pBakFile->Write(m_strContent, m_strContent.GetLength() * sizeof(TCHAR));
	m_pBakFile->Flush();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CloseBakFile
//	Description:	Close file when done writing
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void SConfigBackup::CloseBakFile()
{
	if (m_pBakFile->m_hFile != CFile::hFileNull)
		m_pBakFile->Close();
}


//////////////////////////////////////////////////////////////////////////
//
//	Define methods using for INI file processing
//	These methods are still in development and testing
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		INIKey
//	Description:	Constructor, destructor, copy constructor & copy operator
//
//////////////////////////////////////////////////////////////////////////

INIKey::INIKey()
{
	m_strKeyName = DEF_STRING_EMPTY;
	m_strValue = DEF_STRING_EMPTY;
	m_nValue = -1;
}

INIKey::INIKey(const INIKey& iniKey)
{
	m_strKeyName = iniKey.m_strKeyName;
	m_strValue = iniKey.m_strValue;
	m_nValue = iniKey.m_nValue;
}

void INIKey::operator=(const INIKey& iniKey)
{
	m_strKeyName = iniKey.m_strKeyName;
	m_strValue = iniKey.m_strValue;
	m_nValue = iniKey.m_nValue;
}

INIKey::~INIKey()
{

}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	Get/SetName
//	Description:	Get/set name for key class object
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INIKey::GetName()
{
	return this->m_strKeyName;
}

void INIKey::SetName(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	SetName(strKeyName);
}

void INIKey::SetName(LPCTSTR lpszKeyName)
{
	VERIFY(lpszKeyName != DEF_STRING_EMPTY);
	this->m_strKeyName = lpszKeyName;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	MakeKey
//	Description:	Make key with given key name
//
//////////////////////////////////////////////////////////////////////////

INIKey INIKey::MakeKey(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	return MakeKey(strKeyName);
}

INIKey INIKey::MakeKey(LPCTSTR lpszKeyName)
{
	INIKey iniKey;
	iniKey.m_strKeyName = lpszKeyName;
	return iniKey;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	FormatKey
//	Description:	Format key name & value into output string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INIKey::FormatKey()
{
	CString strFormat;
	if (wcscmp(this->GetValueString(), DEF_STRING_EMPTY) != 0)
		strFormat.Format(_T("%s=%s"), this->GetName(), this->GetValueString());
	else if (this->GetValueInt() != -1)
		strFormat.Format(_T("%s=%d"), this->GetName(), this->GetValueInt());
	else
		strFormat.Format(_T("%s="), this->GetName());

	return strFormat.GetString();
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	Get/SetValueString, Get/SetValueInt
//	Description:	Get/set value for key class object
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INIKey::GetValueString()
{
	return this->m_strValue;
}

void INIKey::SetValueString(LPCTSTR lpszValue)
{
	VERIFY(lpszValue != DEF_STRING_EMPTY);
	this->m_strValue = lpszValue;
}

int	INIKey::GetValueInt()
{
	return this->m_nValue;
}

void INIKey::SetValueInt(int nValue)
{
	VERIFY(nValue != -1);
	this->m_nValue = nValue;
}

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		INISection
//	Description:	Constructor, destructor, copy constructor & copy operator
//
//////////////////////////////////////////////////////////////////////////

INISection::INISection()
{
	m_strSectionName = DEF_STRING_EMPTY;
	m_vtKeyList.clear();
}

INISection::INISection(const INISection& iniSection)
{
	m_strSectionName = iniSection.m_strSectionName;
	m_vtKeyList = iniSection.m_vtKeyList;
}

void INISection::operator=(const INISection& iniSection)
{
	m_strSectionName = iniSection.m_strSectionName;
	m_vtKeyList = iniSection.m_vtKeyList;
}

INISection::~INISection()
{

}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	Get/SetName
//	Description:	Get/set section name
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INISection::GetName()
{
	return this->m_strSectionName;
}

void INISection::SetName(UINT nSectionStringID)
{
	CString strSectionName;
	VERIFY(strSectionName.LoadString(nSectionStringID));
	SetName(strSectionName);
}

void INISection::SetName(LPCTSTR lpszSectionName)
{
	VERIFY(lpszSectionName != DEF_STRING_EMPTY);
	this->m_strSectionName = lpszSectionName;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	MakeSection
//	Description:	Make section with given section name
//
//////////////////////////////////////////////////////////////////////////

INISection INISection::MakeSection(UINT nSectionStringID)
{
	CString strSectionName;
	VERIFY(strSectionName.LoadString(nSectionStringID));
	return MakeSection(strSectionName);
}

INISection INISection::MakeSection(LPCTSTR lpszSectionName)
{
	INISection iniSection;
	iniSection.m_strSectionName = lpszSectionName;
	return iniSection;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	FormatSection
//	Description:	Format section name into output string
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INISection::FormatSection()
{
	CString strFormat;
	strFormat.Format(_T("[%s]"), this->GetName());
	return strFormat.GetString();
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetKey/SetKey
//	Description:	Get/set key info
//
//////////////////////////////////////////////////////////////////////////

INIKey& INISection::GetKey(UINT nIndex)
{
	// Return empty key in case of invalid index
	if (nIndex < 0 || nIndex >= (UINT) this->GetKeyNum())
		return INIKey::MakeKey(DEF_STRING_EMPTY);

	return this->m_vtKeyList.at(nIndex);
}

int INISection::SetKey(INIKey sIniKey)
{
	this->m_vtKeyList.push_back(sIniKey);

	// Return current key ID (also meaning last key)
	int nCurIndex = this->m_vtKeyList.size() - 1;
	return nCurIndex;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetKeyNum
//	Description:	Get number of keys in keylist
//
//////////////////////////////////////////////////////////////////////////

int INISection::GetKeyNum()
{
	return int(this->m_vtKeyList.size());
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetKeyValueString
//	Description:	Get string value of specified key
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INISection::GetKeyValueString(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	return GetKeyValueString(strKeyName);
}

LPCTSTR INISection::GetKeyValueString(LPCTSTR lpszKeyName)
{
	CString strResult = DEF_STRING_EMPTY;

	int nIndex = -1;
	BOOL bKeyExist = FindKey(lpszKeyName, nIndex);
	if (bKeyExist == FALSE || nIndex == -1) {
		INIKey iniKey = this->GetKey(nIndex);
		strResult = iniKey.GetValueString();
	}

	return strResult.GetString();
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetKeyValueInt
//	Description:	Get int value of specified key
//
//////////////////////////////////////////////////////////////////////////

int	INISection::GetKeyValueInt(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	return GetKeyValueInt(strKeyName);
}

int	INISection::GetKeyValueInt(LPCTSTR lpszKeyName)
{
	int nResult = -1;
	int nIndex = -1;
	BOOL bKeyExist = FindKey(lpszKeyName, nIndex);
	if (bKeyExist == FALSE || nIndex == -1) {
		INIKey iniKey = this->GetKey(nIndex);
		nResult = iniKey.GetValueInt();
	}

	return nResult;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	AddKey
//	Description:	Add key with given key name to keylist
//
//////////////////////////////////////////////////////////////////////////

void INISection::AddKey(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	AddKey(strKeyName);
}

void INISection::AddKey(LPCTSTR lpszKeyName)
{
	INIKey iniKey = INIKey::MakeKey(lpszKeyName);
	this->SetKey(iniKey);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	SetKeyValue
//	Description:	Set value for key with given name
//
//////////////////////////////////////////////////////////////////////////

void INISection::SetKeyValue(UINT nKeyStringID, LPCTSTR lpszValue)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	SetKeyValue(strKeyName, lpszValue);
}

void INISection::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue)
{
	int nIndex = -1;
	BOOL bKeyExist = FindKey(lpszKeyName, nIndex);
	if (bKeyExist == FALSE || nIndex == -1) {
		INIKey iniKey = INIKey::MakeKey(lpszKeyName);
		iniKey.SetValueString(lpszValue);
		this->SetKey(iniKey);
	}

	this->m_vtKeyList[nIndex].SetValueString(lpszValue);
}

void INISection::SetKeyValue(UINT nKeyStringID, int nValue)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	SetKeyValue(strKeyName, nValue);
}

void INISection::SetKeyValue(LPCTSTR lpszKeyName, int nValue)
{
	int nIndex = -1;
	BOOL bKeyExist = FindKey(lpszKeyName, nIndex);
	if (bKeyExist == FALSE || nIndex == -1) {
		INIKey iniKey = INIKey::MakeKey(lpszKeyName);
		iniKey.SetValueInt(nValue);
		this->SetKey(iniKey);
	}

	this->m_vtKeyList[nIndex].SetValueInt(nValue);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	FindKey
//	Description:	Check if key exists and return its position
//
//////////////////////////////////////////////////////////////////////////

BOOL INISection::FindKey(UINT nKeyStringID, int& nIndex)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	return FindKey(strKeyName, nIndex);
}

BOOL INISection::FindKey(LPCTSTR lpszKeyName, int& nIndex)
{
	if (this->m_vtKeyList.empty())
		return FALSE;

	for (int i = 0; i < this->m_vtKeyList.size(); i++) {
		if (wcscmp(this->m_vtKeyList[i].GetName(), lpszKeyName) == 0) {
			nIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	RemoveKey
//	Description:	Remove key with given key name from keylist
//
//////////////////////////////////////////////////////////////////////////

void INISection::RemoveKey(UINT nKeyStringID)
{
	CString strKeyName;
	VERIFY(strKeyName.LoadString(nKeyStringID));
	RemoveKey(strKeyName);
}

void INISection::RemoveKey(LPCTSTR lpszKeyName)
{
	int nIndex = -1;
	BOOL bKeyExist = FindKey(lpszKeyName, nIndex);
	if (bKeyExist == FALSE || nIndex == -1) return;
	this->m_vtKeyList.erase(this->m_vtKeyList.begin() + nIndex);
}

//////////////////////////////////////////////////////////////////////////
//
//	Class name:		INIFile
//	Description:	Constructor, destructor, copy constructor & copy operator
//
//////////////////////////////////////////////////////////////////////////

INIFile::INIFile()
{
	m_strFileName = DEF_STRING_EMPTY;
	m_vtSectionList.clear();
}

INIFile::INIFile(const INIFile& iniFile)
{
	m_strFileName = iniFile.m_strFileName;
	m_vtSectionList = iniFile.m_vtSectionList;
}

void INIFile::operator=(const INIFile& iniFile)
{
	m_strFileName = iniFile.m_strFileName;
	m_vtSectionList = iniFile.m_vtSectionList;
}

INIFile::~INIFile()
{

}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	Get/SetName
//	Description:	Get/set section name
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INIFile::GetName()
{
	return this->m_strFileName;
}

void INIFile::SetName(LPCTSTR lpszFileName)
{
	VERIFY(lpszFileName != DEF_STRING_EMPTY);
	this->m_strFileName = lpszFileName;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSection/SetSection
//	Description:	Get/set section info
//
//////////////////////////////////////////////////////////////////////////

INISection& INIFile::GetSection(UINT nIndex)
{
	// Return empty section in case of invalid index
	if (nIndex < 0 || nIndex >= (UINT) this->GetSectionNum())
		return INISection::MakeSection(DEF_STRING_EMPTY);

	return this->m_vtSectionList.at(nIndex);
}

int INIFile::SetSection(INISection sIniSection)
{
	this->m_vtSectionList.push_back(sIniSection);

	// Return current section ID (also meaning last section)
	int nCurIndex = this->m_vtSectionList.size() - 1;
	return nCurIndex;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSectionNum
//	Description:	Get number of section in section list
//
//////////////////////////////////////////////////////////////////////////

int INIFile::GetSectionNum()
{
	return int(this->m_vtSectionList.size());
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSectionByName
//	Description:	Get section by specified name
//
//////////////////////////////////////////////////////////////////////////

INISection& INIFile::GetSectionByName(UINT nSectionStringID)
{
	int nIndex = -1;
	BOOL bRet = FindSection(nSectionStringID, nIndex);
	if (bRet == FALSE || nIndex == -1)
		return INISection::MakeSection(nSectionStringID);

	return GetSection(nIndex);
}

INISection& INIFile::GetSectionByName(LPCTSTR lpszSectionName)
{
	int nIndex = -1;
	BOOL bRet = FindSection(lpszSectionName, nIndex);
	if (bRet == FALSE || nIndex == -1)
		return INISection::MakeSection(lpszSectionName);

	return GetSection(nIndex);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	AddSection
//	Description:	Add section with given name to section list
//
//////////////////////////////////////////////////////////////////////////

void INIFile::AddSection(UINT nSectionStringID)
{
	CString strSectionName;
	VERIFY(strSectionName.LoadString(nSectionStringID));
	AddSection(strSectionName);
}

void INIFile::AddSection(LPCTSTR lpszSectionName)
{
	INISection iniSection = INISection::MakeSection(lpszSectionName);
	this->SetSection(iniSection);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	FindSection
//	Description:	Check if section exists and return its position
//
//////////////////////////////////////////////////////////////////////////

BOOL INIFile::FindSection(UINT nSectionStringID, int& nIndex)
{
	CString strSectionName;
	VERIFY(strSectionName.LoadString(nSectionStringID));
	return FindSection(strSectionName, nIndex);
}

BOOL INIFile::FindSection(LPCTSTR lpszSectionName, int& nIndex)
{
	if (this->m_vtSectionList.empty())
		return FALSE;

	for (int i = 0; i < this->m_vtSectionList.size(); i++) {
		if (wcscmp(this->m_vtSectionList[i].GetName(), lpszSectionName) == 0) {
			nIndex = i;
			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	UpdateSectionName
//	Description:	Update section name if exists
//
//////////////////////////////////////////////////////////////////////////

void INIFile::UpdateSectionName(UINT nCurSectionStringID, UINT nNewSectionStringID)
{
	CString strCurSectionName, strNewSectionName;
	VERIFY(strCurSectionName.LoadString(nCurSectionStringID));
	VERIFY(strNewSectionName.LoadString(nNewSectionStringID));
	UpdateSectionName(strCurSectionName, strNewSectionName);
}

void INIFile::UpdateSectionName(LPCTSTR lpszCurSectionName, LPCTSTR lpszNewSectionName)
{
	int nIndex = -1;
	BOOL bSectionExist = FindSection(lpszCurSectionName, nIndex);
	if (bSectionExist == FALSE || nIndex == -1) return;
	this->m_vtSectionList[nIndex].SetName(lpszNewSectionName);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	RemoveSection
//	Description:	Remove section with given section name
//
//////////////////////////////////////////////////////////////////////////

void INIFile::RemoveSection(UINT nSectionStringID)
{
	CString strSectionName;
	VERIFY(strSectionName.LoadString(nSectionStringID));
	RemoveSection(strSectionName);
}

void INIFile::RemoveSection(LPCTSTR lpszSectionName)
{
	int nIndex = -1;
	BOOL bSectionExist = FindSection(lpszSectionName, nIndex);
	if (bSectionExist == FALSE || nIndex == -1) return;
	this->m_vtSectionList.erase(this->m_vtSectionList.begin() + nIndex);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSubSection/SetSubSection
//	Description:	Get/set sub section info
//
//////////////////////////////////////////////////////////////////////////

LPCTSTR INIFile::MakeSubSectionName(INISection& sIniBaseSection, UINT nSubSectionStringID, int nIndex)
{
	CString strBaseSectionName = sIniBaseSection.GetName();
	CString strSubSectionNameFmt;
	if (nIndex != -1)
		strSubSectionNameFmt.Format(nSubSectionStringID, nIndex);
	else
		strSubSectionNameFmt.LoadString(nSubSectionStringID);

	CString strSubSectionName;
	strSubSectionName.Format(_T("%s/%s"), strBaseSectionName, strSubSectionNameFmt);

	return strSubSectionName.GetString();
}

LPCTSTR INIFile::MakeSubSectionName(INISection& sIniBaseSection, LPCTSTR lpszSubSectionName, int nIndex)
{
	CString strBaseSectionName = sIniBaseSection.GetName();
	CString strSubSectionNameFmt;
	if (nIndex != -1)
		strSubSectionNameFmt.Format(lpszSubSectionName, nIndex);
	else
		strSubSectionNameFmt.SetString(lpszSubSectionName);

	CString strSubSectionName;
	strSubSectionName.Format(_T("%s/%s"), strBaseSectionName, strSubSectionNameFmt);

	return strSubSectionName.GetString();
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSubSectionNum
//	Description:	Get number of subsection of a specified base section 
//					in section list
//
//////////////////////////////////////////////////////////////////////////

int INIFile::GetSubSectionNum(INISection& sIniBaseSection)
{
	int nSubSectionNum = 0;
	int nSectionNum = GetSectionNum();
	LPCTSTR lpszBaseSectionName = sIniBaseSection.GetName();
	for (int nIndex = 0; nIndex < nSectionNum; nIndex++) {
		INISection& iniSection = GetSection(nIndex);
		CString strSectionName = DEF_STRING_EMPTY;
		Left(iniSection.GetName(), strSectionName, _T('/'));
		if (StrCmp(strSectionName, lpszBaseSectionName) == 0) {
			nSubSectionNum++;
		}
	}

	return nSubSectionNum;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetSubSectionByName
//	Description:	Get sub section by specified name
//
//////////////////////////////////////////////////////////////////////////

INISection& INIFile::GetSubSectionByName(INISection& sIniBaseSection, UINT nSubSectionStringID, int nIndex)
{
	LPCTSTR lpszFullName = MakeSubSectionName(sIniBaseSection, nSubSectionStringID, nIndex);

	int nRetIndex = -1;
	BOOL bRet = FindSection(lpszFullName, nRetIndex);
	if (bRet == FALSE || nRetIndex == -1)
		return INISection::MakeSection(lpszFullName);

	return GetSection(nRetIndex);
}

INISection& INIFile::GetSubSectionByName(INISection& sIniBaseSection, LPCTSTR lpszSubSectionName, int nIndex)
{
	LPCTSTR lpszFullName = MakeSubSectionName(sIniBaseSection, lpszSubSectionName, nIndex);

	int nRetIndex = -1;
	BOOL bRet = FindSection(lpszFullName, nRetIndex);
	if (bRet == FALSE || nRetIndex == -1)
		return INISection::MakeSection(lpszFullName);

	return GetSection(nRetIndex);
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	GetLineType
//	Description:	Return type of specified INI stringline
//
//////////////////////////////////////////////////////////////////////////

eINILINETYPE SIniHandler::GetLineType(LPCTSTR lpszSrc)
{
	CString strALSLog;
	strALSLog.Format(_T("[ALSTest] ==> GetLineType: %s"), lpszSrc);
	OutputDebugLogFormat(strALSLog);

	CString strSrc(lpszSrc);
	if (strSrc.IsEmpty())
		return INILINE_INVALID;

	// Remove all leading and trailing spaces
	strSrc = strSrc.Trim();
	int nLength = strSrc.GetLength();

	// Comment check
	if (strSrc.GetAt(0) == _T(';'))
		return INILINE_COMMENT;

	// Section name check
	if (strSrc.GetAt(0) == _T('[') && strSrc.GetAt(nLength - 1) == _T(']'))
		return INILINE_SECTIONNAME;

	// Key check
	int nCount = 0;
	for (int nIndex = 0; nIndex < nLength; nIndex++) {
		if (strSrc.GetAt(nIndex) == _T('=')) {
			if (nIndex == 0)
				return INILINE_INVALID;
			else
				nCount++;
		}
	}
	if (nCount == 1)
		return INILINE_KEY;

	return INILINE_INVALID;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	Validate
//	Description:	Check INI file data validity
//
//////////////////////////////////////////////////////////////////////////

BOOL SIniHandler::Validate(INIFile& iniFile)
{
	// Check INI file name
	CString strFileName = iniFile.GetName();
	if (strFileName.IsEmpty()) {
		OutputDebugLogFormat(_T("[ALSTest] => No file name"));
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	ReadFile
//	Description:	Read INI file
//
//////////////////////////////////////////////////////////////////////////

BOOL SIniHandler::ReadFile(INIFile& iniFile)
{
	CStdioFile fIniFile;
	CFileStatus fsStatus;

	// Get INI file name
	CString strFileName = iniFile.GetName();

	CString strALSLog;
	strALSLog.Format(_T("[ALSTest] ==> ReadFile: %s"), strFileName);
	OutputDebugLogFormat(strALSLog);

	if (strFileName.IsEmpty()) {
		OutputDebugLogFormat(_T("[ALSTest] => No file name"));
		return FALSE;
	}

	BOOL bResult = FALSE;

	// Check if file exists
	bResult = CFile::GetStatus(strFileName, fsStatus);
	if (bResult == FALSE) {
		OutputDebugLogFormat(_T("[ALSTest] => File doesn't exist"));
		return bResult;
	}

	// Open file to read
	bResult = fIniFile.Open(strFileName, CFile::modeRead | CFile::typeText);
	if (bResult == FALSE) {
		OutputDebugLogFormat(_T("[ALSTest] => File open failed"));
		return bResult;
	}

	fIniFile.SeekToBegin();

	CString strResult = DEF_STRING_EMPTY;
	CString strCurSection = DEF_STRING_EMPTY;
	int nCurSection = 0;

	while (fIniFile.ReadString(strResult)) {
		int nLineType = GetLineType(strResult);
		switch (nLineType)
		{
		case INILINE_SECTIONNAME: {
			OutputDebugLogFormat(_T("[ALSTest] ==> GetLineType: Is Section"));
			INISection iniSection = ReadSection(strResult);
			strCurSection = iniSection.GetName();
			nCurSection = iniFile.SetSection(iniSection);
			} break;
		case INILINE_KEY: {
			OutputDebugLogFormat(_T("[ALSTest] ==> GetLineType: Is Key"));
			INIKey iniKey = ReadKey(strResult);
			INISection& iniCurSection = iniFile.GetSection(nCurSection);
			iniCurSection.SetKey(iniKey);
			} break;
		case INILINE_COMMENT:
			OutputDebugLogFormat(_T("[ALSTest] ==> GetLineType: Is Comment"));
			break;
		case INILINE_INVALID:
			OutputDebugLogFormat(_T("[ALSTest] ==> GetLineType: Is Invalid"));
			continue;
			break;
		default:
			break;
		}
	}

	// Close file after done reading
	if (fIniFile.m_hFile != CFile::hFileNull)
		fIniFile.Close();

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	ReadSection
//	Description:	Make a section from input string
//
//////////////////////////////////////////////////////////////////////////

INISection SIniHandler::ReadSection(LPCTSTR lpszSrc)
{
	CString strSectionName = DEF_STRING_EMPTY;
	Mid(lpszSrc, strSectionName, _T('['), _T(']'));

	CString strALSLog;
	strALSLog.Format(_T("[ALSTest] ==> ReadSection: %s"), strSectionName);
	OutputDebugLogFormat(strALSLog);

	INISection iniSection = INISection::MakeSection(strSectionName);
	return iniSection;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	ReadKey
//	Description:	Make a key from input string
//
//////////////////////////////////////////////////////////////////////////

INIKey SIniHandler::ReadKey(LPCTSTR lpszSrc)
{
	CString strKeyName = DEF_STRING_EMPTY;
	Left(lpszSrc, strKeyName, _T('='));
	CString strKeyValue = DEF_STRING_EMPTY;
	Right(lpszSrc, strKeyValue, _T('='));

	CString strALSLog;
	strALSLog.Format(_T("[ALSTest] ==> ReadKey: %s, Value=%s"), strKeyName, strKeyValue);
	OutputDebugLogFormat(strALSLog);

	int nKeyValue = _tstoi(strKeyValue);
	INIKey iniKey = INIKey::MakeKey(strKeyName);
	if (nKeyValue > 0 || strKeyValue == _T("0"))
		iniKey.SetValueInt(nKeyValue);
	else
		iniKey.SetValueString(strKeyValue);

	return iniKey;
}

//////////////////////////////////////////////////////////////////////////
//
//	Function name:	WriteFile
//	Description:	Write INI file
//
//////////////////////////////////////////////////////////////////////////

BOOL SIniHandler::WriteFile(INIFile& iniFile)
{
	CStdioFile fIniFile;

	// Check if file name is valid
	CString strFilePath = iniFile.GetName();
	if (strFilePath.IsEmpty()) {
		OutputDebugLogFormat(_T("[ALSTest] => No file name"));
		return FALSE;
	}

	BOOL bResult = FALSE;

	// Check if file is opening, if not, open it
	if (fIniFile.m_hFile == CStdioFile::hFileNull) {
		bResult = fIniFile.Open(strFilePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		if (bResult == FALSE) {
			OutputDebugLogFormat(_T("[ALSTest] => Open file failed"));
			return bResult;
		}
	}

	// Get number of sections
	int nSectionNum = iniFile.GetSectionNum();

	CString strLog;
	strLog.Format(_T("[ALSTest] => Section num = %d"), nSectionNum);
	OutputDebugLogFormat(strLog);

	if (nSectionNum <= 0) {
		OutputDebugLogFormat(_T("[ALSTest] => No section"));
		return FALSE;
	}

	CString strFormat;
	// Write section list
	for (int nSectionIndex = 0; nSectionIndex < nSectionNum; nSectionIndex++) {
		OutputDebugLogFormat(_T("[ALSTest] => Write section to file"));
		// Write section name
		INISection iniSection = iniFile.GetSection(nSectionIndex);
		strFormat = iniSection.FormatSection();
		fIniFile.WriteString(strFormat);
		fIniFile.WriteString(DEF_STRING_NEWLINE);

		// Write section keylist
		int nKeyNum = iniSection.GetKeyNum();
		for (int nKeyIndex = 0; nKeyIndex < nKeyNum; nKeyIndex++) {
			INIKey iniKey = iniSection.GetKey(nKeyIndex);
			strFormat = iniKey.FormatKey();
			fIniFile.WriteString(strFormat);

			// Space among each key, all except last key
			if (nKeyIndex < (nKeyNum - 1)) {
				fIniFile.WriteString(DEF_STRING_NEWLINE);
			}
		}

		// Space among each section, all except last section
		if (nSectionIndex < (nSectionNum - 1)) {
			fIniFile.WriteString(DEF_STRING_NEWLINE);
			fIniFile.WriteString(DEF_STRING_NEWLINE);
		}
	}

	// Flush data
	fIniFile.Flush();

	// Close file after done writing
	if (fIniFile.m_hFile != CFile::hFileNull)
		fIniFile.Close();

	return bResult;
}
