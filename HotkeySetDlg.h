
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HotkeySetDlg.h
//		Description:	Header file for HotkeySet dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.05.01:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HOTKEYSETDLG_H_INCLUDED
#define _HOTKEYSETDLG_H_INCLUDED

#include "Core.h"
#include "Config.h"
#include "Logging.h"
#include "IDMapping.h"
#include "SDialog.h"


////////////////////////////////////////////////////////
//
//	CHotkeySetDlg dialog used for HotkeySet function
//
////////////////////////////////////////////////////////

class CHotkeySetDlg : public SDialog
{
	DECLARE_DYNAMIC(CHotkeySetDlg)

public:
	CHotkeySetDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHotkeySetDlg();				  // destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEYSET_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual INT_PTR RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

private:
	// Dialog control variables
	CGridCtrl* m_pHotkeySetListTable;
	CComboBox m_cmbActionList;
	CComboBox m_cmbFuncKeyList;

	// Checkbox variables
	BOOL m_bCtrlBtn;
	BOOL m_bAltBtn;
	BOOL m_bWinKeyBtn;

	// Data container variables
	HOTKEYSETDATA m_hksHotkeySet;
	HOTKEYSETDATA m_hksHotkeySetTemp;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	CSize* m_pszDataTableFrameSize;

	// Other variables
	int m_nCheckCount;
	int m_nCurSelIndex;

public:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnRemoveAll();
	afx_msg void OnCheckAll();
	afx_msg void OnUncheckAll();
	afx_msg void OnExport();
	afx_msg void OnSelectHotkeyItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT RequestCloseDialog(void);

protected:
	// Dialog setup functions
	void SetupLanguage();
	void SetupHotkeySetList(LANGTABLE_PTR ptrLanguage);
	void DrawHotkeySetTable(BOOL bReadOnly = FALSE);
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);

	// Dialog item properties functions
	void RefreshDlgItemState();
	void UpdateCheckAllBtnState(BOOL bRecheck = TRUE);
	void UpdateHotkeySet();
	void DisableHotkeySetTable(BOOL bDisable);
	void RedrawHotkeySetTable(BOOL bReadOnly = FALSE);
	void DisplayHotkeyDetails(int nIndex);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);

	// Data processing functions
	BOOL LoadHotkeySetData();
	BOOL SaveHotkeySetData();
	BOOL CheckDataChangeState();

	// Data processing handlers
	void Add();
	void Remove(int nIndex);
	void RemoveAll();
	void SwitchAllItemState(BOOL bState);
	BOOL Validate(HOTKEYSETITEM hksItem, BOOL bShowMsg = FALSE);

protected:
	// Get/set functions
	int GetItemNum(void);
	int	GetListCurSel(void);
	void SetListCurSel(int nSelIndex);
};

#endif	// ifndef _HOTKEYSETDLG_H_INCLUDED