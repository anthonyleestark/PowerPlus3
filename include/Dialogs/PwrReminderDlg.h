/**
 * @file		PwrReminderDlg.h
 * @brief		Header file for Power Reminder dialog
 * @author		AnthonyLeeStark
 * @date		2024.07.20
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
class CReminderMsgDlg;
class CRmdRepeatSetDlg;
class CRmdMsgStyleSetDlg;


// Power Reminder dialog class
class CPwrReminderDlg : public SDialog
{
	DECLARE_DYNAMIC(CPwrReminderDlg)

public:
	using Item = PwrReminderItem;
	using Event = Item::Event;
	using Style = Item::Style;
	using Data = PwrReminderData;
	enum ColumnID {
		Index = 0,										// Index
		EnableState,									// Enable state
		ItemID,											// Item ID
		MessageContent,									// Message content
		EventID,										// Event ID
		MsgStyle,										// Reminder style
		Repeat											// Repeat daily
	};

public:
	CPwrReminderDlg(CWnd* pParent = nullptr);			// standard constructor
	virtual ~CPwrReminderDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PWRREMINDER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual int RegisterDialogManagement(void);
	virtual bool UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

private:
	// Dialog control variables
	CGridCtrl*		 m_pDataItemListTable;
	CEdit*			 m_pMsgStringEdit;
	CButton*		 m_pEvtSetTimeRad;
	CEdit*			 m_pEvtSetTimeEdit;
	CSpinButtonCtrl* m_pEvtSetTimeSpin;
	CButton*		 m_pEvtRepeatSetBtn;
	CButton*		 m_pEvtAppStartupRad;
	CButton*		 m_pEvtSysWakeupRad;
	CButton*		 m_pEvtBfrPwrActionRad;
	CButton*		 m_pEvtPwrActionWakeRad;
	CButton*		 m_pEvtAtAppExitRad;
	CButton*		 m_pStyleMsgBoxRad;
	CButton*		 m_pStyleDialogBoxRad;
	CComboBox*		 m_pMsgStyleCombo;
	CButton*		 m_pStyleUseCommonRad;
	CButton*		 m_pStyleUseCustomRad;
	CButton*		 m_pStyleCustomizeBtn;

	// Properties child dialogs
	CReminderMsgDlg*	m_pRmdPreviewMsgDlg;
	CRmdRepeatSetDlg*	m_pRepeatSetDlg;
	CRmdMsgStyleSetDlg* m_pMsgStyleSetDlg;

	// Checkbox/radio button variables
	BOOL m_bEvtSetTimeRad;
	BOOL m_bEvtAppStartupRad;
	BOOL m_bEvtSysWakeupRad;
	BOOL m_bEvtBfrPwrActionRad;
	BOOL m_bEvtPwrActionWakeRad;
	BOOL m_bEvtAppExitRad;
	BOOL m_bStyleMsgBoxRad;
	BOOL m_bStyleDialogRad;
	BOOL m_bStyleUseCommonRad;
	BOOL m_bStyleUseCustomRad;

	// Data container variables
	Data m_pwrReminderData;
	Data m_pwrReminderDataTemp;

	// Table format and properties
	int m_nColNum;
	Size* m_pszFrameWndSize;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;

	// Other variables
	int m_nCurMode;
	int m_nCheckCount;
	int m_nCurSelIndex;
	int m_nCurDispIndex;
	ClockTime m_stDispTimeBak;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnRemoveAll();
	afx_msg void OnCheckAll();
	afx_msg void OnUncheckAll();
	afx_msg void OnPreviewItem();
	afx_msg void OnSelectReminderItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMsgContentEditChange();
	afx_msg void OnTimeEditSetFocus();
	afx_msg void OnTimeEditKillFocus();
	afx_msg void OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPwrEventRadBtnClicked(UINT nID);
	afx_msg void OnRepeatSet();
	afx_msg void OnCustomizeStyle();
	virtual LRESULT RequestCloseDialog(void);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Dialog setup functions
	void SetupLanguage();
	void SetupDataItemList(LANGTABLE_PTR ptrLanguage);
	void DrawDataTable(Size* pszFrameWndSize, int nColNum, int nRowNum, bool bReadOnly = false, LANGTABLE_PTR ptrLanguage = NULL);
	void SetupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage);
	void SwitchMode(bool bRedraw = false);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);

	// Dialog item properties functions
	void SetupDialogItemState();
	void UpdateDataItemList();
	void DisableTable(bool bDisable);
	void RedrawDataTable(bool bReadOnly = false);
	void DisplayItemDetails(int nIndex);
	void RefreshDialogItemState(bool bRecheckState = false);
	void UpdateCheckAllBtnState(bool bRecheck = false);
	void RefreshDetailView(int nMode);
	void UpdateMsgCounter(int nCount);
	void UpdateTimeSetting(ClockTime& clockTime, bool bUpdate = true);

private:
	// Data processing functions
	bool LoadPwrReminderData();
	bool SavePwrReminderData();
	bool CheckDataChangeState();

	// Data processing handlers
	void Add();
	void Edit(int nIndex);
	void Remove(int nIndex);
	void RemoveAll();
	void SetAllItemState(bool bState);
	void PreviewItem(int nIndex);
	void UpdateItemData(Item& pwrItem, bool bUpdate);
	bool Validate(Item& pwrItem, bool bShowMsg = false, bool bAutoCorrect = false);

protected:
	// Get/set functions
	int GetItemNum() const {
		return m_pwrReminderDataTemp.GetItemNum();
	};
	int GetCurMode() const {
		return m_nCurMode;
	};
	void SetCurMode(int nMode);
	void DrawRepeatSetButton(void);
};

