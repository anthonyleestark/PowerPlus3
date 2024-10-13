
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		RmdRepeatSetDlg.h
//		Description:	Header file for Reminder message repeat mode set dialog
//		Owner:			AnthonyLeeStark
// 
//		History:		<0> 2024.10.02:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _RMDREPEATSETDLG_H_INCLUDED
#define _RMDREPEATSETDLG_H_INCLUDED

#include "Core.h"
#include "Global.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
//
//	CRmdRepeatSetDlg dialog used for Reminder repeat set
//
////////////////////////////////////////////////////////

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
	virtual void SetupDlgItemState();
	virtual void RefreshDlgItemState();

	void SetSnoozeIntervalEdit(int nValue);

public:
	// Data processing functions
	void UpdateDialogData(PWRREMINDERITEM& pwrItemData, BOOL bUpdate);
};

#endif	// ifndef _RMDREPEATSETDLG_H_INCLUDED