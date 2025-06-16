/**
 * @file		HotkeySetDlg.h
 * @brief		Header file for HotkeySet dialog
 * @author		AnthonyLeeStark
 * @date		2024.05.01
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


// HotkeySet dialog class
class CHotkeySetDlg : public SDialog
{
	DECLARE_DYNAMIC(CHotkeySetDlg)

public:
	using Item = HotkeySetItem;
	using HKID = Item::HKID;
	using Data = HotkeySetData;
	enum ColumnID {
		EnableState = 0,								// Enable state
		HKActionID,										// Hotkey action ID
		Keystrokes,										// Keystrokes
	};

public:
	CHotkeySetDlg(CWnd* pParent = nullptr);				// standard constructor
	virtual ~CHotkeySetDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEYSET_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual int RegisterDialogManagement(void);
	virtual bool UnregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

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
	Data m_hksHotkeySet;
	Data m_hksHotkeySetTemp;

	// Table format and properties
	int	m_nColNum;
	GRIDCTRLCOLFORMAT* m_apGrdColFormat;
	Size* m_pszDataTableFrameSize;

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
	void DrawHotkeySetTable(bool bReadOnly = false);
	void SetupComboBox(unsigned nComboID, LANGTABLE_PTR ptrLanguage);

	// Dialog item properties functions
	void RefreshDialogItemState(bool bRecheckState = false);
	void UpdateCheckAllBtnState(bool bRecheck = true);
	void UpdateHotkeySet();
	void DisableHotkeySetTable(bool bDisable);
	void RedrawHotkeySetTable(bool bReadOnly = false);
	void DisplayHotkeyDetails(int nIndex);

	// Layout functions
	void UpdateLayoutInfo(void);
	void LoadLayoutInfo(void);
	void SaveLayoutInfo(void);

	// Data processing functions
	bool LoadHotkeySetData();
	bool SaveHotkeySetData();
	bool CheckDataChangeState();

	// Data processing handlers
	void Add(void);
	void Remove(int nIndex);
	void RemoveAll(void);
	void SwitchAllItemState(bool bState);
	bool Validate(const Item& hksItem, bool bShowMsg = false);

protected:
	// Get/set functions
	int GetItemNum() const {
		return m_hksHotkeySetTemp.GetItemNum();
	};
	int	GetListCurSel(void) const {
		return m_nCurSelIndex;
	};
	void SetListCurSel(int nSelIndex) {
		m_nCurSelIndex = nSelIndex;
	};
};

