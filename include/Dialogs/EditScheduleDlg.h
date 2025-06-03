
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		EditScheduleDlg.h
//		Description:	Header file for Edit Schedule details dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2017.03.08:		Create new
//						<1> 2024.01.27:		Update to version 3.0
//						<2> 2024.10.27:		Implement Multi schedule function
//						<3> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AppCore/AppCore.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"
#include "AppCore/IDManager.h"
#include "AppCore/Serialization.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CEditScheduleDlg dialog used for Edit Schedule function
//
////////////////////////////////////////////////////////

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
	virtual size_t RegisterDialogManagement(void);
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
	SCHEDULEITEM m_schScheduleItem;
	SCHEDULEITEM m_schScheduleItemTemp;

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
	void GetScheduleItem(PSCHEDULEITEM pschItem);
	void SetScheduleItem(const SCHEDULEITEM& pschItem);
	void UpdateScheduleItem(void);
	void SaveScheduleItem(void);

	BOOL CheckDataChangeState(void);
	void EnableSaveButton(BOOL bEnable);
	void EnableSubItems(BOOL bEnable);
	void UpdateTimeSetting(SYSTEMTIME& stTime, BOOL bUpdate = TRUE);

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
