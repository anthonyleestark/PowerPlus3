/**
 * @file		RmdRepeatSetDlg.h
 * @brief		Header file for Reminder message repeat mode set dialog
 * @author		AnthonyLeeStark
 * @date		2024.10.02
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/Global.h"
#include "Framework/SDialog.h"


// RepeatSet dialog class
class CRmdRepeatSetDlg : public SDialog
{
	DECLARE_DYNAMIC(CRmdRepeatSetDlg)

public:
	CRmdRepeatSetDlg();		// constructor
	~CRmdRepeatSetDlg();	// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RMDREPEATSET_DLG };
#endif

private:
	// Dialog control
	CEdit*				snoozeIntervalEditPtr_;
	CSpinButtonCtrl*	snoozeIntervalSpinPtr_;

	CButton*			repeatEnableChkPtr_;
	CButton*			snoozeEnableChkPtr_;
	CButton*			activeMondayChkPtr_;
	CButton*			activeTuesdayChkPtr_;
	CButton*			activeWednesdayChkPtr_;
	CButton*			activeThursdayChkPtr_;
	CButton*			activeFridayChkPtr_;
	CButton*			activeSaturdayChkPtr_;
	CButton*			activeSundayChkPtr_;

	// Member value
	int					snoozeInterval_;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	afx_msg void OnActivate(UINT state, CWnd* otherWndPtr, BOOL isMinimized);
	afx_msg void OnSnoozeSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckboxClicked(UINT id);

protected:
	// Dialog and items setup functions
	virtual void setupLanguage();
	virtual void setupDialogItemState();
	virtual void refreshDialogItemState(bool isRecheckState = false);

	void setSnoozeIntervalEdit(int value);
	
	// Get/set value functions
	int getSnoozeInterval() const {
		return snoozeInterval_;
	}
	void setSnoozeInterval(int value) {
		snoozeInterval_ = value;
	}

public:
	// Data processing functions
	void updateDialogData(PwrReminderItem& reminderItem, bool updateFlag);
};

