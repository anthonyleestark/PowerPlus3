﻿/**
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
	CEdit*				m_pSnoozeIntervalEdit;
	CSpinButtonCtrl*	m_pSnoozeIntervalSpin;

	CButton*			m_pRepeatEnableChk;
	CButton*			m_pSnoozeEnableChk;
	CButton*			m_pActiveMondayChk;
	CButton*			m_pActiveTuesdayChk;
	CButton*			m_pActiveWednesdayChk;
	CButton*			m_pActiveThursdayChk;
	CButton*			m_pActiveFridayChk;
	CButton*			m_pActiveSaturdayChk;
	CButton*			m_pActiveSundayChk;

	// Member value
	int					m_nSnoozeInterval;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()

public:
	// Generated message handle functions
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSnoozeSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckboxClicked(UINT nID);

protected:
	// Dialog and items setup functions
	virtual void SetupLanguage();
	virtual void SetupDialogItemState();
	virtual void RefreshDialogItemState(bool bRecheckState = false);

	void SetSnoozeIntervalEdit(int nValue);
	
	// Get/set value functions
	int GetSnoozeInterval() const {
		return m_nSnoozeInterval;
	};
	void SetSnoozeInterval(int nValue) {
		m_nSnoozeInterval = nValue;
	};

public:
	// Data processing functions
	void UpdateDialogData(PwrReminderItem& pwrItemData, bool bUpdate);
};

