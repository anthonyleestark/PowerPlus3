
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PwrReminderDlg.h
//		Description:	Header file for Power Reminder dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.20:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PWRREMINDERDLG_H_INCLUDED
#define _PWRREMINDERDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
// Declaration of child dialog classes

class CRmdRepeatSetDlg;

////////////////////////////////////////////////////////
//
//	CPwrReminderDlg dialog used for Power Reminder function
//
////////////////////////////////////////////////////////

class CPwrReminderDlg : public SDialog
{
	DECLARE_DYNAMIC(CPwrReminderDlg)

public:
	CPwrReminderDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPwrReminderDlg();				  // destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PWRREMINDER_DLG };
#endif

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

	// Properties child dialogs
	CRmdRepeatSetDlg* m_pRepeatSetDlg;

	// Checkbox/radio button variables
	BOOL m_bEvtSetTimeRad;
	BOOL m_bEvtAppStartupRad;
	BOOL m_bEvtSysWakeupRad;
	BOOL m_bEvtBfrPwrActionRad;
	BOOL m_bEvtPwrActionWakeRad;
	BOOL m_bEvtAppExitRad;
	BOOL m_bStyleMsgBoxRad;
	BOOL m_bStyleDialogRad;

	// Data container variables
	PWRREMINDERDATA m_pwrReminderData;
	PWRREMINDERDATA m_pwrReminderDataTemp;

	// Table format and properties
	int m_nColNum;
	CSize* m_pszFrameWndSize;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;

	// Other variables
	int m_nCurMode;
	int m_nCheckCount;
	int m_nCurSelIndex;
	int m_nCurDispIndex;
	SYSTEMTIME m_stDispTimeBak;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
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
	virtual LRESULT RequestCloseDialog(void);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

protected:
	// Dialog setup functions
	void SetupLanguage();
	void SetupDataItemList(LANGTABLE_PTR ptrLanguage);
	void DrawDataTable(CSize* pszFrameWndSize, int nColNum, int nRowNum, BOOL bReadOnly = FALSE, LANGTABLE_PTR ptrLanguage = NULL);
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);
	void SwitchMode(BOOL bRedraw = FALSE);

	// Dialog item properties functions
	void SetupDialogItemState();
	void UpdateDataItemList();
	void DisableTable(BOOL bDisable);
	void RedrawDataTable(BOOL bReadOnly = FALSE);
	void DisplayItemDetails(int nIndex);
	void RefreshDlgItemState(BOOL bRecheckState);
	void UpdateCheckAllBtnState(BOOL bRecheck = FALSE);
	void RefreshDetailView(int nMode);
	void UpdateMsgCounter(int nCount);
	void UpdateTimeSetting(SYSTEMTIME& stTime, BOOL bUpdate = TRUE);

private:
	// Data processing functions
	BOOL LoadPwrReminderData();
	BOOL SavePwrReminderData();
	BOOL CheckDataChangeState();

	// Data processing handlers
	void Add();
	void Edit(int nIndex);
	void Remove(int nIndex);
	void RemoveAll();
	void SetAllItemState(BOOL bState);
	void PreviewItem(int nIndex);
	void UpdateItemData(PWRREMINDERITEM& pwrItem, BOOL bUpdate);
	BOOL Validate(PWRREMINDERITEM pwrItem, BOOL bShowMsg = FALSE);

protected:
	// Get/set functions
	int GetItemNum(void);
	int GetCurMode(void);
	void SetCurMode(int nMode);
	void DrawRepeatSetButton(void);
};

#endif	// ifndef _PWRREMINDERDLG_H_INCLUDED