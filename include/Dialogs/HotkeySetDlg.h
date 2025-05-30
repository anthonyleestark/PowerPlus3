
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

#include "AppCore/AppCore.h"
#include "AppCore/Config.h"
#include "AppCore/MapTable.h"
#include "AppCore/Logging.h"
#include "AppCore/IDMapping.h"
#include "Framework/SDialog.h"


////////////////////////////////////////////////////////
//
//	CHotkeySetDlg dialog used for HotkeySet function
//
////////////////////////////////////////////////////////

class CHotkeySetDlg : public SDialog
{
	DECLARE_DYNAMIC(CHotkeySetDlg)

public:
	using Item = typename HotkeySetItem;
	using HKID = typename Item::HKID;
	using Data = typename HotkeySetData;
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
	virtual size_t RegisterDialogManagement(void);
	virtual BOOL UnregisterDialogManagement(void);

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
	void RefreshDialogItemState(BOOL bRecheckState = FALSE);
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
	void Add(void);
	void Remove(int nIndex);
	void RemoveAll(void);
	void SwitchAllItemState(BOOL bState);
	BOOL Validate(const Item& hksItem, BOOL bShowMsg = FALSE);

protected:
	// Get/set functions
	int GetItemNum(void) const;
	int	GetListCurSel(void) const;
	void SetListCurSel(int nSelIndex);
};


////////////////////////////////////////////////////////
//
//	Include inline file for inline functions
//
////////////////////////////////////////////////////////

#ifdef _AFX_ENABLE_INLINES
	#define _HOTKEYSETDLG_ENABLE_INLINES
	#include "Dialogs.inl"
	#ifdef _HOTKEYSETDLG_INLINE_INCLUDED
		#pragma message("-- Dialogs inline library included (HotkeySetDlg.h)")
	#else
		#pragma error("-- Linking error in HotkeySetDlg.h: Unable to link to inline header!")
	#endif
	#undef _HOTKEYSETDLG_ENABLE_INLINES
#else
	#pragma	error("-- Fatal error in HotkeySetDlg.h: Inline is not enabled!")
#endif

#endif	// ifndef _HOTKEYSETDLG_H_INCLUDED
