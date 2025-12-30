/**
 * @file		PowerPlusDlg.h
 * @brief		Header file for main window dialog class
 * @author		AnthonyLeeStark
 * @date		2015.06.19
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Serialization.h"

#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"

#include "Framework/SDialog.h"


// Declaration of child dialog classes
class CAboutDlg;
class CHelpDlg;
class CMultiScheduleDlg;
class CLogViewerDlg;
class CHotkeySetDlg;
class CPwrReminderDlg;


// Main window dialog class
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
	virtual int registerDialogManagement(void);
	virtual void updateDialogManagement(void);
	virtual bool unregisterDialogManagement(void);

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
	BOOL m_bnotifySchedule;
	BOOL m_bAllowCancelSchedule;
	BOOL m_benableBackgroundHotkey;
	BOOL m_benablePowerReminder;

	// Notify icon variables
	HICON			m_hNotifyIcon;
	PNOTIFYICONDATA	m_pNotifyIconData;
	CMenu*			m_pNotifyMenu;

	// Member variables using for resizing dialog
	Size* m_pDialogSize;

	// Hotkey register data
	UIntArray m_arrCurRegHKeyList;

	// Power++ runtime queue data
	PwrRuntimeQueue m_arrRuntimeQueue;

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
	void expandDialog(bool bExpand);

	// Notify icon functions
	bool createNotifyIcon(void);
	void updateNotifyIcon(void);
	void removeNotifyIcon(void);
	bool showNotifyMenu(void);

	// Data processing functions
	void getAppData(unsigned dwDataType = APPDATA_ALL);
	int  getAppOption(AppOptionID eAppOptionID, bool isTemp = false) const;
	void updateDialogData(bool saveAndValidate = true);
	bool checkSettingChangeState(void);
	int  getFlagValue(AppFlagID eFlagID) const;
	void setFlagValue(AppFlagID eFlagID, int nValue);

	// Dialog setup functions
	void setupLanguage(void);
	void setupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage);

	// Item state/checkbox update functions
	void enableRightMouseMenu(bool isEnabled);
	void enableLogViewer(bool isEnabled);
	void enableBackgroundHotkey(bool isEnabled);
	void enablePowerReminder(bool isEnabled);
	void updateRestartAsAdminFlag(bool bFlag);

	// Component update functions
	void setMenuItemText(CMenu* pMenu);
	void updateMenuItemState(CMenu* pMenu);
	const wchar_t* getNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void setNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void setBalloonTipText(const wchar_t* balloonTitle, const wchar_t* balloonInfoContent);

private:
	// Core functions
	bool executeAction(unsigned nActionMacro, WPARAM wParam = NULL, LPARAM lParam = NULL);
	void applySettings(bool bMinimize);
	void reloadSettings(void);
	void setDefaultConfig(void);
	void restartApp(bool bRestartAsAdmin);
	void exitApp(int nExitCode);

	// Dialog and window functions
	void showDialog(CWnd* pWnd, bool bShowFlag = true);
	void openChildDialogEx(unsigned nDialogID);
	void openDialogBase(unsigned nDialogID, bool bReadOnlyMode = false, int nOpenMode = MODE_OPENDLG_MODAL);
	bool openTextFileToView(const wchar_t* fileName, const wchar_t* extension, const wchar_t* subDir = Constant::String::Empty);

	// Action Schedule feature functions
	bool processActionSchedule(void);
	void reupdateActionScheduleData(void);
	void setActionScheduleSkip(const ScheduleItem& schItem, int nSkipFlag);
	void setActionScheduleSnooze(const ScheduleItem& schItem, int nSnoozeFlag);
	void updateActionScheduleQueue(int nMode);
	bool getActionScheduleSkipStatus(unsigned nItemID);
	bool getActionScheduleSnoozeStatus(unsigned nItemID, const ClockTime& currentTime);

	// HotkeySet feature functions
	void setupBackgroundHotkey(int nMode);
	bool processHotkey(int nHotkeyID);
	void registerSessionNotification(int nMode);
	bool processLockStateHotkey(DWORD dwHKeyParam);

	// Power Reminder feature functions
	bool executePowerReminder(unsigned nExecEventID);
	int  displayPwrReminder(const PwrReminderItem& pwrDispItem);
	void reupdatePwrReminderData(void);
	void setPwrReminderSnooze(const PwrReminderItem& pwrItem, int nSnoozeFlag);
	void updatePwrReminderSnooze(int nMode);
	bool getPwrReminderSnoozeStatus(unsigned nItemID, const ClockTime& currentTime);
	bool getPwrReminderDispFlag(const PwrReminderItem& pwrItem);
	void setPwrReminderDispFlag(const PwrReminderItem& pwrItem, int nDispFlag);
	size_t getPwrReminderDispList(UIntArray& arrPwrDispList);

	// Debugging functions
	bool processDebugCommand(const wchar_t* commandString, DWORD& dwErrorCode);

	// History and logging functions
	void outputScheduleEventLog(USHORT usEvent, const ScheduleItem& schItem);
	void outputPwrReminderEventLog(USHORT usEvent, const PwrReminderItem& pwrItem);
	void initPwrActionHistoryInfo(unsigned nActionID, bool bResult, DWORD dwErrorCode);
	void initScheduleHistoryInfo(const ScheduleItem& schItem);
	void initHotkeyHistoryInfo(unsigned nHKID);
	void initPwrReminderHistoryInfo(const PwrReminderItem& pwrItem);
	void saveHistoryInfoData(void);

	// Notification and error message functions
	int	 confirmActionExec(unsigned nActionType, unsigned nActionID);
	int  notifySchedule(PScheduleItem pschItem, bool& bReupdate);
	void showErrorMessage(DWORD dwError);
	void requestRestartApp(unsigned uiCommandID, bool bRestartAsAdmin);
	void requestRestartAsAdmin(RESTARTREQ reqRestart);
};
