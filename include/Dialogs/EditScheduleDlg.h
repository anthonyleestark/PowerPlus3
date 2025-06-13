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
	virtual int RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

private:
	// Dialog control item
	CComboBox*		 m_pActionList;
	CEdit*			 m_pTimeEdit;
	CSpinButtonCtrl* m_pTimeSpin;
	CGridCtrl*		 m_pActiveDayListTable;

	// Data variables
	BOOL m_bEnabled;
	BOOL m_bRepeat;
	UINT m_nAction;

	// Data container variables
	ScheduleItem m_schScheduleItem;
	ScheduleItem m_schScheduleItemTemp;

	// Other variables
	int		m_nDispMode;
	CSize*  m_pszActiveTableFrameSize;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	virtual LRESULT RequestCloseDialog(void);

	// Member functions
	void SetupLanguage();
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);
	void SetupActiveDayList(LANGTABLE_PTR ptrLanguage);
	void DrawActiveDayTable(BOOL bReadOnly = FALSE);

	// Dialog item properties functions
	void SetupDialogItemState();
	void UpdateActiveDayList();
	void DisableActiveDayTable(BOOL bDisable);
	void RedrawActiveDayTable(BOOL bReadOnly = FALSE);

	// Data processing functions
	void GetScheduleItem(PScheduleItem pschItem);
	void SetScheduleItem(const ScheduleItem& pschItem);
	void UpdateScheduleItem(void);
	void SaveScheduleItem(void);

	BOOL CheckDataChangeState(void);
	void EnableSaveButton(BOOL bEnable);
	void EnableSubItems(BOOL bEnable);
	void UpdateTimeSetting(ClockTime& clockTime, BOOL bUpdate = TRUE);

public:
	// Get/set functions
	int	GetDispMode(void) const;
	void SetDispMode(int nMode);

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
