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
	CGridCtrl*		 m_pDataItemListTable;

	// Child dialog
	CEditScheduleDlg* m_pEditScheduleDlg;

	// Data container variables
	Data m_schSchedule;
	Data m_schScheduleTemp;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	Size* m_pszDataTableFrameSize;

	// Other variables
	int m_nCurMode;
	int m_nCheckCount;
	int m_nCurSelIndex;
	int m_nCurDispIndex;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT requestCloseDialog(void);

	// Member functions
	void setupLanguage();
	void SetupDataItemList(LANGTABLE_PTR languageTablePtr);
	void DrawDataTable(bool isReadOnly = false);

	// Layout functions
	void updateLayoutInfo(void);
	void loadLayoutInfo(void);
	void saveLayoutInfo(void);

	// Dialog item properties functions
	void setupDialogItemState();
	void UpdateDataItemList();
	void DisableDataTable(bool isDisabled);
	void RedrawDataTable(bool isReadOnly = false);
	void refreshDialogItemState(bool bRecheckState = false);
	void UpdateCheckAllBtnState(bool bRecheck = false);

	// Data processing functions
	bool LoadScheduleSettings();
	bool SaveScheduleSettings();
	void UpdateScheduleSettings();
	bool checkDataChangeState();

	// Data processing handlers
	void Add(Item& scheduleItem);
	void Update(Item& scheduleItem);
	void Remove(int index);
	void RemoveAll();
	void SetAllItemState(bool state);
	bool Validate(Item& scheduleItem, bool bShowMsg = false, bool bAutoCorrect = false);

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
	int GetTotalItemNum() const {
		return GetExtraItemNum() + ScheduleData::defaultItemNum;
	};
	int GetExtraItemNum() const {
		return m_schScheduleTemp.getExtraItemNum();
	};
};

