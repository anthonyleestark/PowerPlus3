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
	CHotkeySetDlg(CWnd* parentWnd = nullptr);				// standard constructor
	virtual ~CHotkeySetDlg();							// destructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTKEYSET_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Dialog control management
	virtual int registerDialogManagement(void);
	virtual bool unregisterDialogManagement(void);

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_RESOURCEID_MAP()

private:
	// Dialog control variables
	CGridCtrl* hotkeySetTablePtr_;
	CComboBox actionListCombo_;
	CComboBox virtualKeyListCombo_;

	// Checkbox variables
	BOOL isCtrlKeyActive_;
	BOOL isAltKeyActive_;
	BOOL isWinKeyActive_;

	// Data container variables
	Data hotkeySetData_;
	Data tempHotkeySetData_;

	// Table format and properties
	int	columnCount_;
	GRIDCTRLCOLFORMAT* gridCtrlFormatInfoPtr_;
	Size* dataTableSizePtr_;

	// Other variables
	int checkCount_;
	int curSelIndex_;

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
	virtual LRESULT requestCloseDialog(void);

protected:
	// Dialog setup functions
	void setupLanguage();
	void setupHotkeySetList(LANGTABLE_PTR languageTablePtr);
	void drawHotkeySetTable(bool isReadOnly = false);
	void setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr);

	// Dialog item properties functions
	void refreshDialogItemState(bool isRecheckState = false);
	void updateCheckAllBtnState(bool isRecheck = true);
	void updateHotkeySet();
	void disableHotkeySetTable(bool isDisabled);
	void redrawHotkeySetTable(bool isReadOnly = false);
	void displayHotkeyDetails(int index);

	// Layout functions
	void updateLayoutInfo(void);
	void loadLayoutInfo(void);
	void saveLayoutInfo(void);

	// Data processing functions
	bool loadHotkeySetData();
	bool saveHotkeySetData();
	bool checkDataChangeState();

	// Data processing handlers
	void add(void);
	void remove(int index);
	void removeAll(void);
	void switchAllItemState(bool state);
	bool validate(const Item& hotkeyItem, bool showMsg = false);

protected:
	// Get/set functions
	int getItemNum() const {
		return tempHotkeySetData_.getItemNum();
	}
	int	getListCurSel(void) const {
		return curSelIndex_;
	}
	void setListCurSel(int selectionIndex) {
		curSelIndex_ = selectionIndex;
	}
};

