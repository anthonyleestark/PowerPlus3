
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlus.h
//		Description:	Header file for app class
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _POWERPLUS_APP_H_INCLUDED
#define _POWERPLUS_APP_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"


////////////////////////////////////////////////////////
//
//	CPowerPlusApp used for app
//
////////////////////////////////////////////////////////

class CPowerPlusApp : public CWinApp
{
public:
	CPowerPlusApp();	// constructor
	~CPowerPlusApp();	// destructor

	// Implementation
	DECLARE_MESSAGE_MAP()
	DECLARE_APP_IDMAP()

private:
	// Member variables for app data
	PCONFIGDATA			m_pcfgAppConfig;
	PSCHEDULEDATA		m_pschSheduleData;
	PHOTKEYSETDATA		m_phksHotkeySetData;
	PPWRREMINDERDATA	m_ppwrReminderData;

	INIFile m_fileConfigData;

	// App language pointer
	LANGTABLE_PTR m_pAppLang;

	// Logging pointers
	SLogging* m_pAppEventLog;
	SLogging* m_pActionLog;

	// Hook procedure handle
	HHOOK m_hAppKeyboardHook;

	// Other member variables
	CString			m_strAppWndTitle;
	UINT			m_nCurDispLang;

public:
	// Instance functions
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Hook and callback functions
	static LRESULT WINAPI KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static ULONG CALLBACK DeviceNotifyCallbackRoutine(PVOID pContext, ULONG ulType, PVOID pSetting);

	// Message processing functions
	virtual BOOL ProcessMessageFilter(int nCode, LPMSG lpMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// App data serialization functions
	BOOL InitAppData();
	BOOL LoadRegistryAppData();
	BOOL SaveRegistryAppData(DWORD dwDataType = APPDATA_ALL);
	BOOL BackupRegistryAppData();
	BOOL LoadGlobalVars(void);
	BOOL SaveGlobalVars(BYTE byCateID = 0xFF);

	// File data serialization functions
	void InitFileData();
	void LoadFileAppData();
	void SetFileAppData();
	BOOL ReadFile();
	BOOL WriteFile();

	// App data processing functions
	PCONFIGDATA GetAppConfigData();
	void SetAppConfigData(PCONFIGDATA pcfgData);
	PSCHEDULEDATA GetAppScheduleData();
	void SetAppScheduleData(PSCHEDULEDATA pschData);
	PHOTKEYSETDATA GetAppHotkeySetData();
	void SetAppHotkeySetData(PHOTKEYSETDATA phksData);
	PPWRREMINDERDATA GetAppPwrReminderData();
	void SetAppPwrReminderData(PPWRREMINDERDATA ppwrData);

	// Language functions
	BOOL InitAppLanguage();
	BOOL ReloadAppLanguage(UINT nCurLanguage = NULL);
	LANGTABLE_PTR GetAppLanguage();
	UINT GetAppLanguageOption(BOOL bCurDispLang = FALSE);

	// Logging functions
	BOOL GetAppEventLogOption();
	void InitAppEventLog();
	SLogging* GetAppEventLog();
	BOOL GetActionLogOption();
	void InitActionLog();
	SLogging* GetActionLog();

	void OutputActionLog(LOGITEM logItem);
	void OutputDataChangeLog(CONFIGDATA& cfgBakData);
	void OutputDataChangeLog(SCHEDULEDATA& schBakData);
	void OutputDataChangeLog(HOTKEYSETDATA& hksBakData);
	void OutputDataChangeLog(PWRREMINDERDATA& pwrBakData);

	void TraceSerializeData(WORD wErrCode);
	BOOL DataSerializeCheck(BYTE bySerializeMode, int nSaveFlag = APPDATA_ALL);

	// Window title functions
	LPCTSTR GetAppWindowTitle(void);
	void SetAppWindowTitle(LPCTSTR strTitle);

	// Registry functions
	void GetAutoStartRegRootDir(HKEY& hRootDir);
	int EnableAutoStart(BOOL bEnable, BOOL bRunAsAdmin);
	int GetAutoStartStatus(void);
	BOOL GetLastSysEventTime(BYTE byEventType, SYSTEMTIME& timeSysEvent);
	BOOL SaveLastSysEventTime(BYTE byEventType, SYSTEMTIME timeSysEvent);
};

extern CPowerPlusApp theApp;


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#ifndef _POWERPLUS_INLINE_ENABLED
		#define _POWERPLUS_INLINE_ENABLED
		#include "PowerPlus.inl"
		#pragma message("--Power++ inline enabled")
	#endif
#endif


////////////////////////////////////////////////////////

#endif	// ifndef _POWERPLUS_APP_H_INCLUDED
