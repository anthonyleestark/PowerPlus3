
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlus.h
//		Description:	Header file for app class
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.03.12:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<1> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _POWERPLUS_APP_H_INCLUDED
#define _POWERPLUS_APP_H_INCLUDED

#include "AppCore/AppCore.h"
#include "AppCore/Config.h"
#include "AppCore/MapTable.h"

#include "AppCore/Logging.h"
#include "AppCore/Logging_pub.h"
#include "AppCore/IDMapping.h"

#include "Framework/SWinApp.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CPowerPlusApp used for app
//
////////////////////////////////////////////////////////

class CPowerPlusApp : public SWinApp
{
public:
	// Construction
	CPowerPlusApp();			// constructor
	~CPowerPlusApp();			// destructor

private:
	// Application data
	ConfigData*			m_pcfgAppConfig;
	ScheduleData*		m_pschScheduleData;
	HotkeySetData*		m_phksHotkeySetData;
	PwrReminderData*	m_ppwrReminderData;

#ifdef _CONFIG_FILE_TEST_ACTIVE
	INIFile m_fileConfigData;
#endif

	// Logging pointers
	SLogging* m_pAppHistoryLog;

	// Hook procedure handle
	HHOOK m_hAppKeyboardHook;

	// DebugTest dialog
	SDialog* m_pDebugTestDlg;

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
	BOOL UpdateAppProfileInfo(void);
	BOOL LoadGlobalData(void);
	BOOL SaveGlobalData(BYTE byCateID = 0xFF);

#ifdef _LEGACY_FEATURE
#ifdef _CONFIG_FILE_TEST_ACTIVE
	// File data serialization functions
	void InitFileData();
	void LoadFileAppData();
	void SetFileAppData();
	BOOL ReadFile();
	BOOL WriteFile();
#endif
#endif

	// App data processing functions
	ConfigData* GetAppConfigData();
	void SetAppConfigData(ConfigData* pcfgData);
	ScheduleData* GetAppScheduleData();
	void SetAppScheduleData(ScheduleData* pschData);
	HotkeySetData* GetAppHotkeySetData();
	void SetAppHotkeySetData(HotkeySetData* phksData);
	PwrReminderData* GetAppPwrReminderData();
	void SetAppPwrReminderData(PwrReminderData* ppwrData);

	// Data options and flags get/set functions
	int GetAppOption(AppOptionID eAppOptionID) const;

	// App history logging functions
	void InitAppHistoryLog();
	SLogging* GetAppHistoryLog();
	void OutputAppHistoryLog(LOGITEM logItem);

#ifdef _LEGACY_FEATURE
#ifdef _DATA_CHANGE_LOGGING
	void OutputDataChangeLog(CONFIGDATA& cfgBakData);
	void OutputDataChangeLog(SCHEDULEDATA& schBakData);
	void OutputDataChangeLog(HOTKEYSETDATA& hksBakData);
	void OutputDataChangeLog(PWRREMINDERDATA& pwrBakData);
#endif
#endif

	// Data validity checking functions
	void TraceSerializeData(WORD wErrCode);
	BOOL DataSerializeCheck(BYTE bySerializeMode, int nSaveFlag = APPDATA_ALL);

	// DebugTest dialog function
	BOOL InitDebugTestDlg(void);
	SDialog* GetDebugTestDlg(void);
	void DestroyDebugTestDlg(void);

	// Registry functions
	void GetAutoStartRegistryRootKey(HKEY& hAutoStartRootKey);
	int EnableAutoStart(BOOL bEnable, BOOL bRunAsAdmin);
	int GetAutoStartRegisterStatus(void);

	BOOL GetLastSysEventTime(BYTE byEventType, SYSTEMTIME& timeSysEvent);
	BOOL SaveLastSysEventTime(BYTE byEventType, const SYSTEMTIME& timeSysEvent);

protected:
	// Application message handlers
	afx_msg void OnExecuteDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowErrorMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

extern CPowerPlusApp theApp;


////////////////////////////////////////////////////////

#endif	// ifndef _POWERPLUS_APP_H_INCLUDED
