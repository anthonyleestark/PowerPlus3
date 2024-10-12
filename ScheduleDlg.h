
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		ScheduleDlg.h
//		Description:	Header file for Schedule dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2017.03.08:		Create new
//						<1> 2024.01.27:		Update to version 3.0
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SCHEDULEDLG_H_INCLUDED
#define _SCHEDULEDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"

////////////////////////////////////////////////////////
//
//	CScheduleDlg dialog used for Schedule function
//
////////////////////////////////////////////////////////

class CScheduleDlg : public SDialog
{
	DECLARE_DYNAMIC(CScheduleDlg)

public:
	CScheduleDlg();				// standard constructor
	virtual ~CScheduleDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDULE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

private:
	// Dialog control item
	CComboBox*		 m_pActionList;
	CEdit*			 m_pTimeEdit;
	CSpinButtonCtrl* m_pTimeSpin;

	// Data variables
	BOOL m_bEnable;
	BOOL m_bRepeat;
	UINT m_nAction;

	// Data container variables
	SCHEDULEDATA m_schSchedule;
	SCHEDULEDATA m_schScheduleTemp;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	virtual LRESULT RequestCloseDialog(void);

	// Member functions
	void SetupLanguage();
	void SetupComboBox(LANGTABLE_PTR pLanguage);
	void SetupDlgItemState();

	void LoadScheduleSettings();
	void UpdateScheduleSettings();
	BOOL SaveScheduleSettings();

	BOOL CheckDataChangeState();
	void EnableSaveButton(BOOL bEnable);
	void EnableSubItems(BOOL bEnable);
	void UpdateTimeSetting(SYSTEMTIME& stTime, BOOL bUpdate = TRUE);

	// Message handlers
	afx_msg void OnApply();
	afx_msg void OnExit();
	afx_msg void OnEnableSchedule();
	afx_msg void OnChangeAction();
	afx_msg void OnChangeRepeatDaily();
	afx_msg void OnTimeEditSetFocus();
	afx_msg void OnTimeEditKillFocus();
	afx_msg void OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

#endif	// ifndef _SCHEDULEDLG_H_INCLUDED