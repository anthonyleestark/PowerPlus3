/**
 * @file		EditScheduleDlg.h
 * @brief		Header file for Edit Schedule details dialog
 * @author		AnthonyLeeStark
 * @date		2017.03.08
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


// Edit Schedule dialog class
class CEditScheduleDlg : public SDialog
{
	DECLARE_DYNAMIC(CEditScheduleDlg)

public:
	CEditScheduleDlg();				// standard constructor
	virtual ~CEditScheduleDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITSCHEDULE_DLG };
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
	CComboBox*		 actionListPtr_;
	CEdit*			 timeEditPtr_;
	CSpinButtonCtrl* timeSpinCtrlPtr_;
	CGridCtrl*		 activeDayTablePtr_;

	// Data variables
	BOOL	 isEnabled_;
	BOOL	 isRepeated_;
	unsigned actionId_;

	// Data container variables
	ScheduleItem scheduleItem_;
	ScheduleItem tempScheduleItem_;

	// Other variables
	int	  displayMode_;
	Size* activeTableSizePtr_;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT requestCloseDialog(void);

	// Member functions
	void setupLanguage();
	void setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr);
	void setupActiveDayList(LANGTABLE_PTR languageTablePtr);
	void drawActiveDayTable(bool isReadOnly = false);

	// Dialog item properties functions
	void setupDialogItemState();
	void updateActiveDayList();
	void disableActiveDayTable(bool isDisabled);
	void RedrawActiveDayTable(bool isReadOnly = false);

	// Data processing functions
	void getScheduleItem(PScheduleItem scheduleItemPtr);
	void setScheduleItem(const ScheduleItem& scheduleItemPtr);
	void updateScheduleItem(void);
	void saveScheduleItem(void);

	bool checkDataChangeState(void);
	void enableSaveButton(bool isEnabled);
	void enableSubItems(bool isEnabled);
	void updateTimeSetting(ClockTime& clockTime, bool updateFlag = true);

public:
	// Get/set functions
	int	getDispMode(void) const;
	void setDispMode(int mode);

protected:
	// Message handlers
	afx_msg void OnApply();
	afx_msg void OnExit();
	afx_msg void OnEnableSchedule();
	afx_msg void OnChangeAction();
	afx_msg void OnChangeRepeatDaily();
	afx_msg void OnTimeEditSetFocus();
	afx_msg void OnTimeEditKillFocus();
	afx_msg void OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickActiveDayList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickActiveDayList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
