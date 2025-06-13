/**
 * @file		PowerPlus.h
 * @brief		Header file for main application class
 * @author		AnthonyLeeStark
 * @date		2015.03.12
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Serialization.h"

#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"

#include "Framework/SWinApp.h"
#include "Framework/SDialog.h"


// Main application class
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
	BOOL UpdateAppLaunchTimeProfileInfo(void);
	BOOL LoadGlobalData(void);
	BOOL SaveGlobalData(BYTE byCateID = 0xFF);

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

	BOOL GetLastSysEventTime(BYTE byEventType, DateTime& timeSysEvent);
	BOOL SaveLastSysEventTime(BYTE byEventType, const DateTime& timeSysEvent);

protected:
	// Application message handlers
	afx_msg void OnExecuteDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowErrorMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


// The unique global application object
extern CPowerPlusApp theApp;
