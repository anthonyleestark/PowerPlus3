
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HotkeySetDlg.h
//		Description:	Header file for HotkeySet dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.05.01:		Create new
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

private:
	// Dialog control variables
	CListCtrl m_listHotkeySet;
	CComboBox m_cmbActionList;
	CComboBox m_cmbFuncKeyList;

	// Checkbox variables
	BOOL m_bCtrlBtn;
	BOOL m_bAltBtn;
	BOOL m_bWinKeyBtn;

	// Data container variables
	HOTKEYSETDATA m_hksHotkeySet;
	HOTKEYSETDATA m_hksHotkeySetTemp;

	// Other variables
	BOOL m_bAllChecked;

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnApply();
	afx_msg void OnCancel();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnRemoveAll();
	afx_msg void OnCheckAll();
	afx_msg void OnExport();
	afx_msg void OnSelectHotkeyItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
	DECLARE_CLASS_IDMAP()

public:
	// Dialog setup functions
	void SetupLanguage();
	void SetupHotkeySetList(LANGTABLE_PTR ptrLanguage);
	void SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage);

	// Dialog item properties functions
	void RefreshDlgItemState();
	void UpdateCheckAllBtnState();
	void UpdateHotkeySet();
	void UpdateHotkeySet(int nIndex);
	void DisplayHotkeyDetails(int nIndex);

	// Data processing functions
	BOOL LoadHotkeySetData();
	BOOL SaveHotkeySetData();
	BOOL CheckDataChangeState();

	// Data processing handlers
	void Add();
	void Remove(int nIndex);
	void RemoveAll();
	void SwitchAllItemState();
	BOOL Validate(HOTKEYSETITEM hksItem, BOOL bShowMsg = FALSE);
};

#endif	// ifndef _HOTKEYSETDLG_H_INCLUDED