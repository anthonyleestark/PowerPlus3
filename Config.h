
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
#include "Core.h"

using namespace CoreFuncs;

/////////////////////////////////////

//// Declarations

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Functions using for reading/writing registry values
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace RegFuncs
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

////////////////////////////////////////////////////////
//
//	Class name:	 SConfigBackup
//  Description: Using for saving backup configurations
//
////////////////////////////////////////////////////////

class SConfigBackup
{
private:
//	std::ofstream* ofsConfigFile;
	CFile* m_pBakFile;
	CString m_strContent;

public:
	SConfigBackup();
	~SConfigBackup();

	static BOOL AutoRegistryExport();
	BOOL PrepareBakFile();
	void WriteLine(UINT nResourceStringID);
	void WriteLine(CString strText = _T(""));
	void WriteValue(UINT nKeyID, UINT nValue);
	void WriteValue(CString strKeyName, UINT nValue);
	void UpdateBakFile();
	void CloseBakFile();
};

#ifdef _CONFIG_FILE_TEST
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Declare structs and components using for INI file processing
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class INIKey : public CObject
{
private:
	CString m_strKeyName;
	CString m_strValue;
	int		m_nValue = -1;

public:
	INIKey();
	INIKey(const INIKey&);
	void operator=(const INIKey&);
	~INIKey();

public:
	LPCTSTR GetName();
	void	SetName(UINT nKeyStringID);
	void	SetName(LPCTSTR lpszKeyName);

	static INIKey MakeKey(UINT nKeyStringID);
	static INIKey MakeKey(LPCTSTR lpszKeyName);

	LPCTSTR FormatKey();

	LPCTSTR GetValueString();
	void	SetValueString(LPCTSTR lpszValue);
	int		GetValueInt();
	void	SetValueInt(int nValue);
};

typedef std::vector<INIKey> KeyList;

class INISection : public CObject
{
private:
	CString m_strSectionName;
	KeyList m_vtKeyList;

public:
	INISection();
	INISection(const INISection&);
	void operator=(const INISection&);
	~INISection();

public:
	LPCTSTR GetName();
	void	SetName(UINT nSectionStringID);
	void	SetName(LPCTSTR lpszSectionName);

	static INISection MakeSection(UINT nSectionStringID);
	static INISection MakeSection(LPCTSTR lpszSectionName);

	LPCTSTR FormatSection();

	INIKey&	GetKey(UINT nIndex);
	int		SetKey(INIKey sIniKey);
	int		GetKeyNum();

	LPCTSTR GetKeyValueString(UINT nKeyStringID);
	LPCTSTR GetKeyValueString(LPCTSTR lpszKeyName);
	int		GetKeyValueInt(UINT nKeyStringID);
	int		GetKeyValueInt(LPCTSTR lpszKeyName);

	void AddKey(UINT nKeyStringID);
	void AddKey(LPCTSTR lpszKeyName);
	void SetKeyValue(UINT nKeyStringID, LPCTSTR lpszValue);
	void SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue);
	void SetKeyValue(UINT nKeyStringID, int nValue);
	void SetKeyValue(LPCTSTR lpszKeyName, int nValue);
	BOOL FindKey(UINT nKeyStringID, int& nIndex);
	BOOL FindKey(LPCTSTR lpszKeyName, int& nIndex);
	void RemoveKey(UINT nKeyStringID);
	void RemoveKey(LPCTSTR lpszKeyName);
};

typedef std::vector<INISection> SectionList;

class INIFile : public CObject
{
private:
	CString m_strFileName;
	SectionList m_vtSectionList;

public:
	INIFile();
	INIFile(const INIFile&);
	void operator=(const INIFile&);
	~INIFile();

public:
	LPCTSTR GetName();
	void	SetName(LPCTSTR lpszFileName);

	INISection&  GetSection(UINT nIndex);
	int			 SetSection(INISection sIniSection);

	int			 GetSectionNum();
	INISection&  GetSectionByName(UINT nSectionStringID);
	INISection&  GetSectionByName(LPCTSTR lpszSectionName);

	void AddSection(UINT nSectionStringID);
	void AddSection(LPCTSTR lpszSectionName);
	BOOL FindSection(UINT nSectionStringID, int& nIndex);
	BOOL FindSection(LPCTSTR lpszSectionName, int& nIndex);
	void UpdateSectionName(UINT nCurSectionStringID, UINT nNewSectionStringID);
	void UpdateSectionName(LPCTSTR lpszCurSectionName, LPCTSTR lpszNewSectionName);
	void RemoveSection(UINT nSectionStringID);
	void RemoveSection(LPCTSTR lpszSectionName);

	static LPCTSTR MakeSubSectionName(INISection& sIniBaseSection, UINT nSubSectionStringID, int nIndex = -1);
	static LPCTSTR MakeSubSectionName(INISection& sIniBaseSection, LPCTSTR lpszSubSectionName, int nIndex = -1);

	int			GetSubSectionNum(INISection& sIniBaseSection);
	INISection& GetSubSectionByName(INISection& sIniBaseSection, UINT nSubSectionStringID, int nIndex = -1);
	INISection& GetSubSectionByName(INISection& sIniBaseSection, LPCTSTR lpszSubSectionName, int nIndex = -1);
};

enum eINILINETYPE {
	INILINE_INVALID,
	INILINE_SECTIONNAME,
	INILINE_KEY,
	INILINE_COMMENT,
};

class SIniHandler
{
private:
	// String processing methods
	eINILINETYPE GetLineType(LPCTSTR lpszSrc);
	INISection ReadSection(LPCTSTR lpszSrc);
	INIKey ReadKey(LPCTSTR lpszSrc);

public:
	// File handling methods
	BOOL Validate(INIFile& iniFile);
	BOOL ReadFile(INIFile& iniFile);
	BOOL WriteFile(INIFile& iniFile);
};
#endif	// ifdef _CONFIG_FILE_TEST

#endif	// ifndef _CONFIG_H_INCLUDED