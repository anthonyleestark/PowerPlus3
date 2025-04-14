
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		MultiScheduleDlg.h
//		Description:	Header file for Multi Schedule setting dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.10.27:		Create new
//						<1> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MULTISCHEDULEDLG_H_INCLUDED
#define _MULTISCHEDULEDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "MapTable.h"

#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
// Declaration of child dialog classes

class CEditScheduleDlg;

////////////////////////////////////////////////////////
//
//	CMultiScheduleDlg dialog used for Multi Schedule function
//
////////////////////////////////////////////////////////

class CMultiScheduleDlg : public SDialog
{
	DECLARE_DYNAMIC(CMultiScheduleDlg)

public:
	CMultiScheduleDlg();				// standard constructor
	virtual ~CMultiScheduleDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDULE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual INT_PTR RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

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
	ScheduleData m_schSchedule;
	ScheduleData m_schScheduleTemp;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	CSize* m_pszDataTableFrameSize;

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
	virtual LRESULT RequestCloseDialog(void);

	// Member functions
	void SetupLanguage();
	void SetupDataItemList(LANGTABLE_PTR ptrLanguage);
	void DrawDataTable(BOOL bReadOnly = FALSE);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);

	// Dialog item properties functions
	void SetupDialogItemState();
	void UpdateDataItemList();
	void DisableDataTable(BOOL bDisable);
	void RedrawDataTable(BOOL bReadOnly = FALSE);
	void RefreshDialogItemState(BOOL bRecheckState = FALSE);
	void UpdateCheckAllBtnState(BOOL bRecheck = FALSE);

	// Data processing functions
	BOOL LoadScheduleSettings();
	BOOL SaveScheduleSettings();
	void UpdateScheduleSettings();
	BOOL CheckDataChangeState();

	// Data processing handlers
	void Add(SCHEDULEITEM& schItem);
	void Update(SCHEDULEITEM& schItem);
	void Remove(int nIndex);
	void RemoveAll();
	void SetAllItemState(BOOL bState);
	BOOL Validate(SCHEDULEITEM& schItem, BOOL bShowMsg = FALSE, BOOL bAutoCorrect = FALSE);

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
	int GetTotalItemNum(void) const;
	int GetExtraItemNum(void) const;
};

#endif	// ifndef _MULTISCHEDULEDLG_H_INCLUDED
