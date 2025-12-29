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
	bool initAppData();
	bool loadRegistryAppData();
	bool saveRegistryAppData(DWORD dwDataType = APPDATA_ALL);
	bool backupRegistryAppData();
	bool updateAppLaunchTimeProfileInfo(void);
	bool loadGlobalData(void);
	bool saveGlobalData(BYTE byCateID = 0xFF);

	// App data processing functions
	ConfigData* getAppConfigData();
	void setAppConfigData(ConfigData* pcfgData);
	ScheduleData* getAppScheduleData();
	void setAppScheduleData(ScheduleData* pschData);
	HotkeySetData* getAppHotkeySetData();
	void setAppHotkeySetData(HotkeySetData* phksData);
	PwrReminderData* getAppPwrReminderData();
	void setAppPwrReminderData(PwrReminderData* ppwrData);

	// Data options and flags get/set functions
	int getAppOption(AppOptionID eAppOptionID) const;

	// App history logging functions
	void initAppHistoryLog();
	SLogging* getAppHistoryLog();
	void outputAppHistoryLog(LOGITEM logItem);

	// Data validity checking functions
	void traceSerializeData(WORD wErrCode);
	bool dataSerializeCheck(BYTE bySerializeMode, int nSaveFlag = APPDATA_ALL);

	// DebugTest dialog function
	bool initDebugTestDlg(void);
	SDialog* getDebugTestDlg(void);
	void destroyDebugTestDlg(void);

	// Registry functions
	void getAutoStartRegistryRootKey(HKEY& hAutoStartRootKey);
	int enableAutoStart(bool bEnable, bool bRunAsAdmin);
	int getAutoStartRegisterStatus(void);

	bool getLastSysEventTime(BYTE byEventType, DateTime& timeSysEvent);
	bool saveLastSysEventTime(BYTE byEventType, const DateTime& timeSysEvent);

protected:
	// Application message handlers
	afx_msg void OnExecuteDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowErrorMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


// The unique global application object
extern CPowerPlusApp theApp;
