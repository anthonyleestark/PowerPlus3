
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

#include "Core.h"
#include "Config.h"

#include "Logging.h"
#include "Logging_pub.h"
#include "IDMapping.h"

#include "SWinApp.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
// Declaration of child dialog classes

class CDebugTestDlg;

////////////////////////////////////////////////////////
//
//	CPowerPlusApp used for app
//
////////////////////////////////////////////////////////

class CPowerPlusApp : public SWinApp
{
public:
	CPowerPlusApp();	// constructor
	~CPowerPlusApp();	// destructor

private:
	// Member variables for app data
	PCONFIGDATA			m_pcfgAppConfig;
	PSCHEDULEDATA		m_pschScheduleData;
	PHOTKEYSETDATA		m_phksHotkeySetData;
	PPWRREMINDERDATA	m_ppwrReminderData;

#ifdef _CONFIG_FILE_TEST
	INIFile m_fileConfigData;
#endif

	// Logging pointers
	SLogging* m_pAppHistoryLog;

	// Hook procedure handle
	HHOOK m_hAppKeyboardHook;

	// DebugTest dialog
	CDebugTestDlg* m_pDebugTestDlg;

	// Other variables
	CString m_strLastSysEvtRegPath;

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

#ifdef _CONFIG_FILE_TEST
	// File data serialization functions
	void InitFileData();
	void LoadFileAppData();
	void SetFileAppData();
	BOOL ReadFile();
	BOOL WriteFile();
#endif

	// App data processing functions
	PCONFIGDATA GetAppConfigData();
	void SetAppConfigData(PCONFIGDATA pcfgData);
	PSCHEDULEDATA GetAppScheduleData();
	void SetAppScheduleData(PSCHEDULEDATA pschData);
	PHOTKEYSETDATA GetAppHotkeySetData();
	void SetAppHotkeySetData(PHOTKEYSETDATA phksData);
	PPWRREMINDERDATA GetAppPwrReminderData();
	void SetAppPwrReminderData(PPWRREMINDERDATA ppwrData);

	// Data options and flags get/set functions
	int  GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp = FALSE) const;
	int  GetFlagValue(APPFLAGID eFlagID) const;
	void SetFlagValue(APPFLAGID eFlagID, int nValue);

	// App history logging functions
	void InitAppHistoryLog();
	SLogging* GetAppHistoryLog();
	void OutputAppHistoryLog(LOGITEM logItem);

#ifdef _DEBUG
	void OutputDataChangeLog(CONFIGDATA& cfgBakData);
	void OutputDataChangeLog(SCHEDULEDATA& schBakData);
	void OutputDataChangeLog(HOTKEYSETDATA& hksBakData);
	void OutputDataChangeLog(PWRREMINDERDATA& pwrBakData);
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

	void InitLastSysEventRegistryInfo(void);
	BOOL GetLastSysEventTime(BYTE byEventType, SYSTEMTIME& timeSysEvent);
	BOOL SaveLastSysEventTime(BYTE byEventType, SYSTEMTIME timeSysEvent);

protected:
	// Application message handlers
	afx_msg void OnExecuteDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnShowErrorMessage(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

extern CPowerPlusApp theApp;


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#ifndef _POWERPLUS_ENABLES_INLINE
		#define _POWERPLUS_ENABLES_INLINE
		#include "PowerPlus.inl"
		#pragma message("--Power++ inline enabled")
	#endif
#endif


////////////////////////////////////////////////////////

#endif	// ifndef _POWERPLUS_APP_H_INCLUDED
