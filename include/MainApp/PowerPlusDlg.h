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
	CPowerPlusDlg(CWnd* parentWnd = NULL);	// standard constructor
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
	ConfigData		appConfigData_;
	ConfigData		tempConfigData_;
	ScheduleData	scheduleData_;
	HotkeySetData	hotkeySetData_;
	PwrReminderData reminderData_;
	HistoryInfoData	historyInfoData_;

	// Dialog control variables:
	CComboBox leftMouseActionCombo_;
	CComboBox middleMouseActionCombo_;
	CComboBox rightMouseActionCombo_;
	CComboBox languageCombo_;

	// Boolean variables for checkboxes
	BOOL rightMouseShowMenu_;
	BOOL showDialogAtStartup_;
	BOOL isStartupEnabled_;
	BOOL confirmBeforeAction_;
	BOOL saveHistoryLog_;
	BOOL saveAppEventLog_;
	BOOL isRunAsAdmin_;
	BOOL showErrorMsg_;
	BOOL notifySchedule_;
	BOOL allowCancelSchedule_;
	BOOL enableBackgroundHotkey_;
	BOOL enablePowerReminder_;

	// Notify icon variables
	HICON			notifyIconHandle_;
	PNOTIFYICONDATA	notifyIconDataPtr_;
	CMenu*			notifyMenuPtr_;

	// Member variables using for resizing dialog
	Size* dialogSize_;

	// Hotkey register data
	uint32_array currentRegHotkeyList_;

	// Power++ runtime queue data
	PwrRuntimeQueue runtimeQueue_;

	// Child dialogs
	CAboutDlg*			aboutDlgPtr_;
	CHelpDlg*			helpDlgPtr_;
	CLogViewerDlg*		logViewerDlgPtr_;
	CMultiScheduleDlg*	multiScheduleDlgPtr_;
	CHotkeySetDlg*		hotkeySetDlgPtr_;
	CPwrReminderDlg*	pwrReminderDlgPtr_;

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual int  PreDestroyDialog();
	virtual void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSysCommand(UINT id, LPARAM lParam);
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
	afx_msg void OnCheckboxClicked(UINT checkboxId);
	afx_msg void OnBackupConfig();
	afx_msg void OnLogViewer();
	afx_msg void OnSchedule();
	afx_msg void OnHotkeySet();
	afx_msg void OnPowerReminder();
	afx_msg void OnViewActionLog();
	afx_msg void OnViewBackupConfig();
	afx_msg void OnTimer(UINT_PTR eventId);
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
	void expandDialog(bool isExpandOrCollapse);

	// Notify icon functions
	bool createNotifyIcon(void);
	void updateNotifyIcon(void);
	void removeNotifyIcon(void);
	bool showNotifyMenu(void);

	// Data processing functions
	void getAppData(unsigned dataType = APPDATA_ALL);
	int  getAppOption(AppOptionID optionId, bool isTemp = false) const;
	void updateDialogData(bool saveAndValidate = true);
	bool checkSettingChangeState(void);
	int  getFlagValue(AppFlagID flagId) const;
	void setFlagValue(AppFlagID flagId, int value);

	// Dialog setup functions
	void setupLanguage(void);
	void setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr);

	// Item state/checkbox update functions
	void enableRightMouseMenu(bool isEnabled);
	void enableLogViewer(bool isEnabled);
	void enableBackgroundHotkey(bool isEnabled);
	void enablePowerReminder(bool isEnabled);
	void updateRestartAsAdminFlag(bool flag);

	// Component update functions
	void setMenuItemText(CMenu* menuPtr);
	void updateMenuItemState(CMenu* menuPtr);
	const wchar_t* getNotifyTipText(PNOTIFYICONDATA notifyIconDataPtr);
	void setNotifyTipText(PNOTIFYICONDATA notifyIconDataPtr);
	void setBalloonTipText(const wchar_t* balloonTitle, const wchar_t* balloonInfoContent);

private:
	// Core functions
	bool executeAction(unsigned actionMacro, WPARAM wParam = NULL, LPARAM lParam = NULL);
	void applySettings(bool isMinimized);
	void reloadSettings(void);
	void setDefaultConfig(void);
	void restartApp(bool restartAsAdmin);
	void exitApp(int exitCode);

	// Dialog and window functions
	void showDialog(CWnd* windowPtr, bool showFlag = true);
	void openChildDialogEx(unsigned dialogId);
	void openDialogBase(unsigned dialogId, bool readOnlyMode = false, int openMode = MODE_OPENDLG_MODAL);
	bool openTextFileToView(const wchar_t* fileName, const wchar_t* extension, const wchar_t* subDir = Constant::String::Empty);

	// Action Schedule feature functions
	bool processActionSchedule(void);
	void reupdateActionScheduleData(void);
	void setActionScheduleSkip(const ScheduleItem& scheduleItem, int skipFlag);
	void setActionScheduleSnooze(const ScheduleItem& scheduleItem, int snoozeFlag);
	void updateActionScheduleQueue(int mode);
	bool getActionScheduleSkipStatus(unsigned itemId);
	bool getActionScheduleSnoozeStatus(unsigned itemId, const ClockTime& currentTime);

	// HotkeySet feature functions
	void setupBackgroundHotkey(int mode);
	bool processHotkey(int hotkeyId);
	void registerSessionNotification(int mode);
	bool processLockStateHotkey(DWORD hotkeyParam);

	// Power Reminder feature functions
	bool executePowerReminder(unsigned eventId);
	int  displayPwrReminder(const PwrReminderItem& displayItem);
	void reupdatePwrReminderData(void);
	void setPwrReminderSnooze(const PwrReminderItem& reminderItem, int snoozeFlag);
	void updatePwrReminderSnooze(int mode);
	bool getPwrReminderSnoozeStatus(unsigned itemId, const ClockTime& currentTime);
	bool getPwrReminderDispFlag(const PwrReminderItem& reminderItem);
	void setPwrReminderDispFlag(const PwrReminderItem& reminderItem, int displayFlag);
	size_t getPwrReminderDispList(uint32_array& displayItemList);

	// Debugging functions
	bool processDebugCommand(const wchar_t* commandString, DWORD& errorCode);

	// History and logging functions
	void outputScheduleEventLog(uint16 eventId, const ScheduleItem& scheduleItem);
	void outputPwrReminderEventLog(uint16 eventId, const PwrReminderItem& reminderItem);
	void initPwrActionHistoryInfo(unsigned actionId, bool result, DWORD errorCode);
	void initScheduleHistoryInfo(const ScheduleItem& scheduleItem);
	void initHotkeyHistoryInfo(unsigned hotkeyId);
	void initPwrReminderHistoryInfo(const PwrReminderItem& reminderItem);
	void saveHistoryInfoData(void);

	// Notification and error message functions
	int	 confirmActionExec(unsigned actionType, unsigned actionId);
	int  notifySchedule(PScheduleItem scheduleItemPtr, bool& update);
	void showErrorMessage(DWORD errorCode);
	void requestRestartApp(unsigned commandId, bool restartAsAdmin);
	void requestRestartAsAdmin(RESTARTREQ restartRequest);
};
