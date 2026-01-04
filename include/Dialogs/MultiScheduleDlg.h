/**
 * @file		MultiScheduleDlg.h
 * @brief		Header file for Multi Schedule setting dialog
 * @author		AnthonyLeeStark
 * @date		2024.10.27
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"
#include "AppCore/Serialization.h"
#include "Framework/SDialog.h"


// Declaration of child dialog classes
class CEditScheduleDlg;


// Multi-schedule dialog class
class CMultiScheduleDlg : public SDialog
{
	DECLARE_DYNAMIC(CMultiScheduleDlg)

public:
	using Item = ScheduleItem;
	using Data = ScheduleData;
	enum ColumnID {
		Index = 0,										// Index
		EnableState,									// Enable/active state
		ActionID,										// Action ID
		TimeValue,										// Time value
		Repeat,											// Repeat daily
	};

public:
	CMultiScheduleDlg();								// standard constructor
	virtual ~CMultiScheduleDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDULE_DLG };
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
	// Dialog control item
	CGridCtrl* scheduleDataTablePtr_;

	// Child dialog
	CEditScheduleDlg* editScheduleDlgPtr_;

	// Data container variables
	Data scheduleData_;
	Data tempScheduleData_;

	// Table format and properties
	int	columnCount_;
	GRIDCTRLCOLFORMAT* gridCtrlFormatInfoPtr_;
	Size* dataTableSizePtr_;

	// Other variables
	int currentMode_;
	int checkCount_;
	int curSelIndex_;
	int m_nCurDispIndex;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT requestCloseDialog(void);

	// Member functions
	void setupLanguage();
	void setupDataItemList(LANGTABLE_PTR languageTablePtr);
	void drawDataTable(bool isReadOnly = false);

	// Layout functions
	void updateLayoutInfo(void);
	void loadLayoutInfo(void);
	void saveLayoutInfo(void);

	// Dialog item properties functions
	void setupDialogItemState();
	void updateDataItemList();
	void disableDataTable(bool isDisabled);
	void redrawDataTable(bool isReadOnly = false);
	void refreshDialogItemState(bool isRecheckState = false);
	void updateCheckAllBtnState(bool isRecheck = false);

	// Data processing functions
	bool loadScheduleSettings();
	bool saveScheduleSettings();
	void updateScheduleSettings();
	bool checkDataChangeState();

	// Data processing handlers
	void add(Item& scheduleItem);
	void update(Item& scheduleItem);
	void remove(int index);
	void removeAll();
	void setAllItemState(bool state);
	bool validate(Item& scheduleItem, bool showMsg = false, bool isAutoCorrect = false);

	// Message handlers
	afx_msg void OnApply();
	afx_msg void OnExit();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnRemove();
	afx_msg void OnRemoveAll();
	afx_msg void OnCheckAll();
	afx_msg void OnUncheckAll();
	afx_msg void OnViewDetails();
	afx_msg void OnSetDefault();
	afx_msg void OnSelectScheduleItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnChildDialogDestroy(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

protected:
	// Get/set functions
	inline int getTotalItemNum() const {
		return getExtraItemNum() + ScheduleData::kDefaultItemNum;
	}
	inline int getExtraItemNum() const {
		return tempScheduleData_.getExtraItemNum();
	}
};

