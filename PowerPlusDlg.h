
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
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _POWERPLUSDLG_H_INCLUDED
#define _POWERPLUSDLG_H_INCLUDED

#include "SDialog.h"

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"


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

// Implementation
private:
	// App data variables
	CONFIGDATA		m_cfgAppConfig;
	CONFIGDATA		m_cfgTempConfig;
	SCHEDULEDATA	m_schScheduleData;
	HOTKEYSETDATA	m_hksHotkeySetData;
	PWRREMINDERDATA m_prdReminderData;
	ACTIONDATA		m_actActionData;

	// Dialog control variables:
	CComboBox m_cmbLMBAction;
	CComboBox m_cmbMMBAction;
	CComboBox m_cmbRMBAction;
	CComboBox m_cmbLanguages;

	// Boolean variables for checkboxes
	BOOL m_bShowDlgAtStartup;
	BOOL m_bStartupEnabled;
	BOOL m_bConfirmAction;
	BOOL m_bSaveActionLog;
	BOOL m_bSaveAppEventLog;
	BOOL m_bRunAsAdmin;
	BOOL m_bShowErrorMsg;
	BOOL m_bRMBShowMenu;
	BOOL m_bNotifySchedule;
	BOOL m_bAllowCancelSchedule;
	BOOL m_bEnableBackgroundHotkey;
	BOOL m_bEnablePowerReminder;

	// Notify icon variables
	HICON			m_hNotifyIcon;
	PNOTIFYICONDATA	m_pNotifyIconData;
	BOOL			m_bNotifyIconShowed;

	// Member variables using for resizing dialog
	BOOL	m_bDlgExpanded;
	CSize*	m_pDialogSize;

	// Other member variable(s)
	BOOL		m_bHotkeyRegistered;
	CUIntArray	m_arrCurRegHKeyList;
	BOOL		m_bRestartAsAdminFlag;
	int			m_nPwrBroadcastSkipCount;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void PreDestroyDialog();
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
	afx_msg void OnCheckboxClicked(UINT nID);
	afx_msg void OnBackupConfig();
	afx_msg void OnLogViewer();
	afx_msg void OnSchedule();
	afx_msg void OnHotkeySet();
	afx_msg void OnPowerReminder();
	afx_msg void OnViewActionLog();
	afx_msg void OnViewBackupConfig();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnUpdateScheduleData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdateHotkeySetData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUpdatePwrReminderData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProcessDebugCommand(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShowErrorMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPowerBroadcastEvent(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Member functions using for resizing dialog
	void ExpandDialog(BOOL bExpand);
	void MoveControls(const int* arrCtrlIDs, int nCount, int nDir, int nDistance);

	// Notify icon functions
	void SetNotifyIcon();
	void UpdateNotifyIcon();
	void RemoveNotifyIcon();
	BOOL ShowNotifyMenu();

	// Data processing functions
	void GetAppData(UINT dwDataType = APPDATA_ALL);
	int  GetAppOption(APPOPTIONID eAppOptionID, BOOL bTemp = FALSE);
	void UpdateDialogData(BOOL bUpdate);
	BOOL CheckSettingChangeState();
	int  GetFlagValue(APPFLAGID eFlagID);
	void SetFlagValue(APPFLAGID eFlagID, int nValue);

	// Dialog setup functions
	void SetupLanguage();
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR pLanguage);

	// Item state/checkbox update functions
	void EnableRightMouseMenu(BOOL bEnable);
	void EnableLogViewer(BOOL bEnable);
	void EnableBackgroundHotkey(BOOL bEnable);
	void EnablePowerReminder(BOOL bEnable);
	void UpdateRestartAsAdminFlag(BOOL bFlag);
	void EnableControl(UINT nCtrlID, BOOL bEnable);

	// Component update functions
	void SetMenuItemText(CMenu* pMenu);
	void UpdateMenuItemState(CMenu* pMenu);
	LPCTSTR GetNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void SetNotifyTipText(PNOTIFYICONDATA pNotifyIconData);
	void SetBalloonTipText(UINT nCurLanguage, UINT nSecondLeft);

private:
	// Core functions
	BOOL DoAction(UINT nActionType, WPARAM wParam = NULL, LPARAM lParam = NULL);
	void ApplyAndClose();
	void ReloadSettings();

	void ShowDialog(HWND hWnd, UINT nDialogID, BOOL bShowFlag = TRUE);
	void OpenDialog(UINT nDialogID, BOOL bReadOnlyMode = FALSE, int nOpenMode = DEF_MODE_OPENDLG_MODAL);
	BOOL OpenFileToView(LPCTSTR lpszFileName, LPCTSTR lpszSubDir = DEF_STRING_EMPTY);
	void RestartApp(BOOL bRestartAsAdmin);

	// Advanced features functions
	BOOL ProcessSchedule();
	void SetScheduleActiveState(BOOL bActive);
	void SetupBackgroundHotkey(int nMode = DEF_MODE_INIT);
	BOOL ProcessHotkey(int nHotkeyID);
	BOOL ExecutePowerReminder(UINT nExecEventID);
	int  DisplayPwrReminder(PWRREMINDERITEM& pwrDispItem);
	void ReupdatePwrReminderData();
	BOOL ProcessDebugCommand(LPCTSTR lpszCommand);

	// Logging and message functions
	void SaveActionHistory(ACTIONDATA actionInfo);
	int	 ConfirmAction(UINT nActionType, UINT nActionID);
	int  NotifySchedule(void);
	void ShowErrorMessage(DWORD dwError);
	void RequestRestart(UINT uiCommandID, BOOL bRestartAsAdmin);
	void RequestRestartAsAdmin(RESTARTREQ reqRestart);
};

#endif	// ifndef _POWERPLUSDLG_H_INCLUDED

// END (PowerPlusDlg.h)