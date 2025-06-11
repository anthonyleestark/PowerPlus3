
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PwrReminderDlg.h
//		Description:	Header file for Power Reminder dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.20:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Serialization.h"

#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"

#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
// Declaration of child dialog classes

class CReminderMsgDlg;
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
	virtual BOOL UnregisterDialogManagement(void);

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

	// Properties child dialogs
	CReminderMsgDlg* m_pRmdPreviewMsgDlg;
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
	Data m_pwrReminderData;
	Data m_pwrReminderDataTemp;

	// Table format and properties
	int m_nColNum;
	CSize* m_pszFrameWndSize;
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
	virtual LRESULT RequestCloseDialog(void);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Dialog setup functions
	void SetupLanguage();
	void SetupDataItemList(LANGTABLE_PTR ptrLanguage);
	void DrawDataTable(CSize* pszFrameWndSize, int nColNum, int nRowNum, BOOL bReadOnly = FALSE, LANGTABLE_PTR ptrLanguage = NULL);
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);
	void SwitchMode(BOOL bRedraw = FALSE);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);

	// Dialog item properties functions
	void SetupDialogItemState();
	void UpdateDataItemList();
	void DisableTable(BOOL bDisable);
	void RedrawDataTable(BOOL bReadOnly = FALSE);
	void DisplayItemDetails(int nIndex);
	void RefreshDialogItemState(BOOL bRecheckState = FALSE);
	void UpdateCheckAllBtnState(BOOL bRecheck = FALSE);
	void RefreshDetailView(int nMode);
	void UpdateMsgCounter(int nCount);
	void UpdateTimeSetting(ClockTime& clockTime, BOOL bUpdate = TRUE);

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
	void UpdateItemData(Item& pwrItem, BOOL bUpdate);
	BOOL Validate(Item& pwrItem, BOOL bShowMsg = FALSE, BOOL bAutoCorrect = FALSE);

protected:
	// Get/set functions
	int GetItemNum(void) const;
	int GetCurMode(void) const;
	void SetCurMode(int nMode);
	void DrawRepeatSetButton(void);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _PWRREMINDERDLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _PWRREMINDERDLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (PwrReminderDlg.h)")
	#else
		#pragma error("-- Linking error in PwrReminderDlg.h: Unable to link to inline header!")
	#endif
	#undef _PWRREMINDERDLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in PwrReminderDlg.h: Inline is not enabled!")
#endif
