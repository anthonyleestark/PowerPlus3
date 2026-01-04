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
	CPwrReminderDlg(CWnd* parentWnd = nullptr);			// standard constructor
	virtual ~CPwrReminderDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PWRREMINDER_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual int registerDialogManagement(void);
	virtual bool unregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

private:
	// Dialog control variables
	CGridCtrl*		 dataListTablePtr_;
	CEdit*			 msgStringEditPtr_;
	CButton*		 evtSetTimeRadPtr_;
	CEdit*			 evtSetTimeEditPtr_;
	CSpinButtonCtrl* evtSetTimeSpinPtr_;
	CButton*		 evtRepeatSetBtnPtr_;
	CButton*		 evtAppStartupRadPtr_;
	CButton*		 evtSysWakeupRadPtr_;
	CButton*		 evtBfrPwrActionRadPtr_;
	CButton*		 evtPwrActionWakeRadPtr_;
	CButton*		 evtAtAppExitRadPtr_;
	CButton*		 styleMsgBoxRadPtr_;
	CButton*		 styleDialogBoxRadPtr_;
	CComboBox*		 msgStyleComboPtr_;
	CButton*		 styleUseCommonRadPtr_;
	CButton*		 styleUseCustomRadPtr_;
	CButton*		 styleCustomizeBtnPtr_;

	// Properties child dialogs
	CReminderMsgDlg*	previewMsgDlgPtr_;
	CRmdRepeatSetDlg*	repeatSetDlgPtr_;
	CRmdMsgStyleSetDlg* msgStyleSetDlgPtr_;

	// Checkbox/radio button variables
	BOOL isEventtSetTimeEnabled_;
	BOOL isEventAppStartupEnabled_;
	BOOL isEventSysWakeupEnabled_;
	BOOL isEventBfrPwrActionEnabled_;
	BOOL isEventPwrActionWakeEnabled_;
	BOOL isEventAppExitEnabled_;
	BOOL isStyleMsgBoxEnabled_;
	BOOL isStyleDialogEnabled_;
	BOOL isStyleUseCommonEnabled_;
	BOOL isStyleUseCustomEnabled_;

	// Data container variables
	Data reminderData_;
	Data tempReminderData_;
	Item itemInEdit_;

	// Table format and properties
	int columnCount_;
	Size* tableFrameSizePtr_;
	GRIDCTRLCOLFORMAT* gridCtrlFormatInfoPtr_;

	// Other variables
	int currentMode_;
	int checkCount_;
	int curSelIndex_;
	int curDispIndex_;
	ClockTime displayedTimeBackup_;

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
	afx_msg void OnPwrEventRadBtnClicked(UINT id);
	afx_msg void OnRepeatSet();
	afx_msg void OnCustomizeStyle();
	virtual LRESULT requestCloseDialog(void);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Dialog setup functions
	void setupLanguage();
	void setupDataItemList(LANGTABLE_PTR languageTablePtr);
	void drawDataTable(Size* tableFrameSizePtr, int colCount, int rowCount, bool isReadOnly = false, LANGTABLE_PTR languageTablePtr = NULL);
	void setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr);
	void switchMode(bool redrawFlag = false);

	// Layout functions
	void updateLayoutInfo(void);
	void loadLayoutInfo(void);
	void saveLayoutInfo(void);

	// Dialog item properties functions
	void setupDialogItemState();
	void updateDataItemList();
	void disableTable(bool isDisabled);
	void redrawDataTable(bool isReadOnly = false);
	void displayItemDetails(int index);
	void refreshDialogItemState(bool isRecheckState = false);
	void updateCheckAllBtnState(bool isRecheck = false);
	void refreshDetailView(int mode);
	void updateMsgCounter(int count);
	void updateTimeSetting(ClockTime& clockTime, bool updateFlag = true);

private:
	// Data processing functions
	bool loadPwrReminderData();
	bool savePwrReminderData();
	bool checkDataChangeState();

	// Data processing handlers
	void add();
	void edit(int index);
	void remove(int index);
	void removeAll();
	void setAllItemState(bool state);
	void previewItem(int index);
	void updateItemData(Item& reminderItem, bool updateFlag);
	bool validate(Item& reminderItem, bool showMsg = false, bool isAutoCorrect = false);

protected:
	// Get/set functions
	inline int getItemNum() const {
		return tempReminderData_.getItemNum();
	}
	inline int getCurMode() const {
		return currentMode_;
	}
	void setCurMode(int mode);
	void drawRepeatSetButton(void);
};

