
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		Config.h
//		Description:	Define necessary methods to read/write, load/save configurations
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.02.03:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _CONFIG_H_INCLUDED
#define _CONFIG_H_INCLUDED

#include "stdafx.h"
#include "AppCore/AppCore.h"


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

	// Read/write registry values with nested subsection by string ID
	UINT GetRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName);
	BOOL WriteRegistryValueInt(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, int nValue);

	// Read/write registry values with nested subsection by string ID
	CString GetRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName);
	BOOL WriteRegistryValueString(UINT nSectionName, UINT nSubSectionName, UINT nKeyName, LPCTSTR lpszsValue);

	// Delete registry section or subsection by name
	BOOL DeleteRegistrySection(UINT nSectionName, UINT nSubSectionName = NULL);
	BOOL DeleteRegistrySection(LPCTSTR lpszSectionName, LPCTSTR lpszSubSectionName = NULL);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Functions for reading/writing application profile info
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry profile info values
	BOOL GetProfileInfo(UINT nKeyName, int& nRef);
	BOOL WriteProfileInfo(UINT nKeyName, int nValue);
	BOOL GetProfileInfo(UINT nKeyName, CString& strRef);
	BOOL WriteProfileInfo(UINT nKeyName, CString strValue);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Derivered functions for reading/writing each data type
	//
	//////////////////////////////////////////////////////////////////////////

	// Read/write registry config values
	BOOL GetConfig(UINT nKeyName, int& nRef);
	BOOL WriteConfig(UINT nKeyName, int nValue);

	// Delete section
	BOOL DeleteConfigSection(void);

	// Read/write registry default schedule values
	BOOL GetDefaultSchedule(UINT nKeyName, int& nRef);
	BOOL WriteDefaultSchedule(UINT nKeyName, int nValue);

	// Read/write registry schedule extra item number value
	BOOL GetScheduleExtraItemNum(UINT nKeyName, int& nRef);
	BOOL WriteScheduleExtraItemNum(UINT nKeyName, int nValue);

	// Read/write registry schedule extra item values
	BOOL GetScheduleExtra(int nItemIndex, UINT nKeyName, int& nRef);
	BOOL WriteScheduleExtra(int nItemIndex, UINT nKeyName, int nValue);

	// Delete section
	BOOL DeleteScheduleSection(void);

	// Read/write registry hotkeyset item number value
	BOOL GetHotkeyItemNum(UINT nKeyName, int& nRef);
	BOOL WriteHotkeyItemNum(UINT nKeyName, int nValue);

	// Read/write registry hotkeyset item values
	BOOL GetHotkeySet(int nItemIndex, UINT nKeyName, int& nRef);
	BOOL WriteHotkeySet(int nItemIndex, UINT nKeyName, int nValue);

	// Delete section
	BOOL DeleteHotkeySetSection(void);

	// Read/write registry Power Reminder item number value
	BOOL GetPwrReminderItemNum(UINT nKeyName, int& nRef);
	BOOL WritePwrReminderItemNum(UINT nKeyName, int nValue);

	// Read/write registry Power Reminder item values
	BOOL GetPwrReminder(int nItemIndex, UINT nKeyName, int& nRef);
	BOOL WritePwrReminder(int nItemIndex, UINT nKeyName, int nValue);
	BOOL GetPwrReminder(int nItemIndex, UINT nKeyName, CString& strRef);
	BOOL WritePwrReminder(int nItemIndex, UINT nKeyName, CString strValue);

	// Delete section
	BOOL DeletePwrReminderSection(void);

	// Read/write registry layout info values
	BOOL GetLayoutInfo(UINT nSectionName, UINT nKeyName, int& nRef);
	BOOL WriteLayoutInfo(UINT nSectionName, UINT nKeyName, int nValue);
	BOOL GetLayoutInfo(UINT nSectionName, LPCTSTR lpszKeyName, int& nRef);
	BOOL WriteLayoutInfo(UINT nSectionName, LPCTSTR lpszKeyName, int nValue);

	// Delete section
	BOOL DeleteLayoutInfoSection(void);

	// Read/write other global data variables
	BOOL GetGlobalData(UINT nSubSection, UINT nKeyName, int& nRef);
	BOOL WriteGlobalData(UINT nSubSection, UINT nKeyName, int nValue);
	BOOL GetGlobalData(UINT nSubSection, UINT nKeyName, CString& strRef);
	BOOL WriteGlobalData(UINT nSubSection, UINT nKeyName, CString strValue);
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

#endif	// ifndef _CONFIG_H_INCLUDED
