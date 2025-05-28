
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PowerPlusDlg.h
//		Description:	Header file for main window
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2015.06.19:		Create new
//						<1> 2017.03.08:		Update to version 2.0
//						<2> 2024.01.27:		Update to version 3.0
//						<3> 2024.07.06:		Update to version 3.1
//						<4> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _POWERPLUSDLG_H_INCLUDED
#define _POWERPLUSDLG_H_INCLUDED

#include "AppCore\AppCore.h"
#include "AppCore\Config.h"
#include "AppCore\MapTable.h"

#include "AppCore\Logging.h"
#include "AppCore\Logging_pub.h"
#include "AppCore\IDMapping.h"

#include "Framework\SDialog.h"


////////////////////////////////////////////////////////
// Declaration of child dialog classes

class CAboutDlg;
class CHelpDlg;
class CMultiScheduleDlg;
class CLogViewerDlg;
class CHotkeySetDlg;
class CPwrReminderDlg;

////////////////////////////////////////////////////////
//
//	CPowerPlusDlg used for main app window
//
////////////////////////////////////////////////////////

class CPowerPlusDlg : public SDialog
{
	DECLARE_DYNAMIC(CPowerPlusDlg)

// Construction
public:
	CPowerPlusDlg(CWnd* pParent = NULL);	// standard constructor
	~CPowerPlusDlg();						// destructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POWERPLUS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Dialog control management
	virtual INT_PTR RegisterDialogManagement(void);
	virtual void UpdateDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

private:
	// App data variables
	ConfigData		m_cfgAppConfig;
	ConfigData		m_cfgTempConfig;
	ScheduleData	m_schScheduleData;
	HotkeySetData	m_hksHotkeySetData;
	PwrReminderData m_prdReminderData;
	HistoryInfoData	m_hidHistoryInfoData;

	// Dialog control variables:
	CComboBox m_cmbLMBAction;
	CComboBox m_cmbMMBAction;
	CComboBox m_cmbRMBAction;
	CComboBox m_cmbLanguages;

	// Boolean variables for checkboxes
	BOOL m_bRMBShowMenu;
	BOOL m_bShowDlgAtStartup;
	BOOL m_bStartupEnabled;
	BOOL m_bConfirmAction;
	BOOL m_bSaveHistoryLog;
	BOOL m_bSaveAppEventLog;
	BOOL m_bRunAsAdmin;
	BOOL m_bShowErrorMsg;
	BOOL m_bNotifySchedule;
	BOOL m_bAllowCancelSchedule;
	BOOL m_bEnableBackgroundHotkey;
	BOOL m_bEnablePowerReminder;

	// Notify icon variables
	HICON			m_hNotifyIcon;
	PNOTIFYICONDATA	m_pNotifyIconData;
	BOOL			m_bNotifyIconShowed;
	CMenu*			m_pNotifyMenu;

	// Member variables using for resizing dialog
	BOOL	m_bDlgExpanded;
	CSize*	m_pDialogSize;

	// Hotkey register data
	BOOL		m_bHotkeyRegistered;
	CUIntArray	m_arrCurRegHKeyList;

	// Power++ runtime queue data
	PWRRUNTIMEQUEUE m_arrRuntimeQueue;

	// Other flags
	BOOL m_bRestartAsAdminFlag;
	INT  m_nPwrBroadcastSkipCount;
	BOOL m_bWTSSessionNotifyRegistered;

	// Child dialogs
	CAboutDlg*			m_pAboutDlg;
	CHelpDlg*			m_pHelpDlg;
	CLogViewerDlg*		m_pLogViewerDlg;
	CMultiScheduleDlg*	m_pMultiScheduleDlg;
	CHotkeySetDlg*		m_pHotkeySetDlg;
	CPwrReminderDlg*	m_pPwrReminderDlg;

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual int  PreDestroyDialog();
	virtual void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();

	// Message processing functions
	afx_msg void OnApply();
	afx_msg void OnReload();
	afx_msg void OnExit();
	afx_msg void OnExpand();
	afx_msg void OnClose();
	afx_msg void OnAbout();
	afx_msg void OnHelp();
	afx_msg void OnDefault();
	afx_msg void OnChangeLMBAction();
	afx_msg void OnChangeMMBAction();
	afx_msg void OnChangeRMBAction();
	afx_msg void OnChangeLanguage();
	afx_msg void OnEnableRightMouseMenu();
	afx_msg void OnCheckboxClicked(UINT nChkBoxID);
	afx_msg void OnBackupConfig();
	afx_msg void OnLogViewer();
	afx_msg void OnSchedule();
	afx_msg void OnHotkeySet();
	afx_msg void OnPowerReminder();
	afx_msg void OnViewActionLog();
	afx_msg void OnViewBackupConfig();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnChildDialogDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateScheduleData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateHotkeySetData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePwrReminderData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProcessDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowErrorMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPowerBroadcastEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQuerryEndSession(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWTSSessionChange(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Member functions using for resizing dialog
	void ExpandDialog(BOOL bExpand);

	// Notify icon functions
	BOOL CreateNotifyIcon(void);
	void UpdateNotifyIcon(void);
	void RemoveNotifyIcon(void);
	BOOL ShowNotifyMenu(void);

	// Data processing functions
	void GetAppData(UINT dwDataType = APPDATA_ALL);
	int  GetAppOption(AppOptionID eAppOptionID, BOOL bTemp = FALSE) const;
	void UpdateDialogData(BOOL bSaveAndValidate = TRUE);
	BOOL CheckSettingChangeState(void);
	int  GetFlagValue(APPFLAGID eFlagID) const;
	void SetFlagValue(APPFLAGID eFlagID, int nValue);

	// Dialog setup functions
	void SetupLanguage(void);
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);

	// Item state/checkbox update functions
	void EnableRightMouseMenu(BOOL bEnable);
	void EnableLogViewer(BOOL bEnable);
	void EnableBackgroundHotkey(BOOL bEnable);
	void EnablePowerReminder(BOOL bEnable);
	void UpdateRestartAsAdminFlag(BOOL bFlag);

	// Component update functions
	void SetMenuItemText(CMenu* pMenu);
	void UpdateMenuItemState(CMenu* pMenu);
	LPCTSTR GetNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void SetNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void SetBalloonTipText(UINT nCurLanguage, UINT nScheduleAction, UINT nSecondLeft);

private:
	// Core functions
	BOOL ExecuteAction(UINT nActionMacro, WPARAM wParam = NULL, LPARAM lParam = NULL);
	void ApplySettings(BOOL bMinimize);
	void ReloadSettings(void);
	void SetDefaultConfig(void);
	void RestartApp(BOOL bRestartAsAdmin);
	void ExitApp(int nExitCode);

	// Dialog and window functions
	void ShowDialog(CWnd* pWnd, BOOL bShowFlag = TRUE);
	void OpenChildDialogEx(UINT nDialogID);
	void OpenDialogBase(UINT nDialogID, BOOL bReadOnlyMode = FALSE, int nOpenMode = MODE_OPENDLG_MODAL);
	BOOL OpenTextFileToView(LPCTSTR lpszFileName, LPCTSTR lpszExtension, LPCTSTR lpszSubDir = STRING_EMPTY);

	// Action Schedule feature functions
	BOOL ProcessActionSchedule(void);
	void ReupdateActionScheduleData(void);
	void SetActionScheduleSkip(const SCHEDULEITEM& schItem, int nSkipFlag);
	void SetActionScheduleSnooze(const SCHEDULEITEM& schItem, int nSnoozeFlag);
	void UpdateActionScheduleQueue(int nMode);
	BOOL GetActionScheduleSkipStatus(UINT nItemID);
	BOOL GetActionScheduleSnoozeStatus(UINT nItemID, SYSTEMTIME& curSysTime);

	// HotkeySet feature functions
	void SetupBackgroundHotkey(int nMode);
	BOOL ProcessHotkey(int nHotkeyID);
	void RegisterSessionNotification(int nMode);
	BOOL ProcessLockStateHotkey(DWORD dwHKeyParam);

	// Power Reminder feature functions
	BOOL ExecutePowerReminder(UINT nExecEventID);
	int  DisplayPwrReminder(const PWRREMINDERITEM& pwrDispItem);
	void ReupdatePwrReminderData(void);
	void SetPwrReminderSnooze(const PWRREMINDERITEM& pwrItem, int nSnoozeFlag);
	void UpdatePwrReminderSnooze(int nMode);
	BOOL GetPwrReminderSnoozeStatus(UINT nItemID, SYSTEMTIME& curSysTime);
	void SetPwrReminderDispFlag(const PWRREMINDERITEM& pwrItem, int nDispFlag);
	INT_PTR GetPwrReminderDispList(CUIntArray& arrPwrDispList);

	// Debugging functions
	BOOL ProcessDebugCommand(LPCTSTR lpszCommand, DWORD& dwErrorCode);

	// History and logging functions
	void OutputScheduleEventLog(USHORT usEvent, const SCHEDULEITEM& schItem);
	void OutputPwrReminderEventLog(USHORT usEvent, const PWRREMINDERITEM& pwrItem);
	void InitPwrActionHistoryInfo(UINT nActionID, BOOL bResult, DWORD dwErrorCode);
	void InitScheduleHistoryInfo(const SCHEDULEITEM& schItem);
	void InitHotkeyHistoryInfo(UINT nHKID);
	void InitPwrReminderHistoryInfo(const PWRREMINDERITEM& pwrItem);
	void SaveHistoryInfoData(void);

	// Notification and error message functions
	int	 ConfirmActionExec(UINT nActionType, UINT nActionID);
	int  NotifySchedule(PSCHEDULEITEM pschItem, BOOL& bReupdate);
	void ShowErrorMessage(DWORD dwError);
	void RequestRestartApp(UINT uiCommandID, BOOL bRestartAsAdmin);
	void RequestRestartAsAdmin(RESTARTREQ reqRestart);
};

#endif	// ifndef _POWERPLUSDLG_H_INCLUDED

// END (PowerPlusDlg.h)
