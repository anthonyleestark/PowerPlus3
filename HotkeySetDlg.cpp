
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HotkeySetDlg.cpp
//		Description:	Source file for HotkeySet dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.05.01:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "HotkeySetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;

//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CHotkeySetDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CHotkeySetDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CHotkeySetDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CHotkeySetDlg::CHotkeySetDlg(CWnd* pParent /*=nullptr*/)
	: SDialog(IDD_HOTKEYSET_DLG, pParent)
{
	// Initialize member variables

	// Data container variables
	ZeroMemory(&m_hksHotkeySet, sizeof(HOTKEYSETDATA));
	ZeroMemory(&m_hksHotkeySetTemp, sizeof(HOTKEYSETDATA));

	// Checkbox variables
	m_bCtrlBtn = FALSE;
	m_bAltBtn = FALSE;
	m_bWinKeyBtn = FALSE;

	// Other variables
	m_bAllChecked = FALSE;

	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CHotkeySetDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CHotkeySetDlg::~CHotkeySetDlg()
{
	// Remove HotkeySet data
	m_hksHotkeySet.DeleteAll();
	m_hksHotkeySetTemp.DeleteAll();

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_DESTROYED);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEYSET_ITEM_LISTBOX, m_listHotkeySet);
	DDX_Control(pDX, IDC_HOTKEYSET_ACTION_LIST, m_cmbActionList);
	DDX_Control(pDX, IDC_HOTKEYSET_FUNCKEY_LIST, m_cmbFuncKeyList);
	DDX_Check(pDX, IDC_HOTKEYSET_CTRLKEY_CHK, m_bCtrlBtn);
	DDX_Check(pDX, IDC_HOTKEYSET_ALTKEY_CHK, m_bAltBtn);
	DDX_Check(pDX, IDC_HOTKEYSET_WINKEY_CHK, m_bWinKeyBtn);
}

BEGIN_ID_MAPPING(CHotkeySetDlg)
	IDMAP_ADD(IDD_HOTKEYSET_DLG,				"HotkeySetDlg")
	IDMAP_ADD(IDC_HOTKEYSET_ITEM_LISTBOX,		"HotkeySetList")
	IDMAP_ADD(IDC_HOTKEYSET_ADD_BTN,			"AddButton")
	IDMAP_ADD(IDC_HOTKEYSET_REMOVE_BTN,			"RemoveButton")
	IDMAP_ADD(IDC_HOTKEYSET_REMOVEALL_BTN,		"RemoveAllButton")
	IDMAP_ADD(IDC_HOTKEYSET_CHECKALL_BTN,		"CheckAllButton")
	IDMAP_ADD(IDC_HOTKEYSET_EXPORT_BTN,			"ExportButton")
	IDMAP_ADD(IDC_HOTKEYSET_APPLY_BTN,			"ApplyButton")
	IDMAP_ADD(IDC_HOTKEYSET_CANCEL_BTN,			"CancelButton")
	IDMAP_ADD(IDC_HOTKEYSET_ACTION_TITLE,		"ActionTitle")
	IDMAP_ADD(IDC_HOTKEYSET_ACTION_LIST,		"ActionListCombo")
	IDMAP_ADD(IDC_HOTKEYSET_CONTROLKEY_STATIC,	"ControlKeyTitle")
	IDMAP_ADD(IDC_HOTKEYSET_CTRLKEY_CHK,		"CtrlKeyCheck")
	IDMAP_ADD(IDC_HOTKEYSET_ALTKEY_CHK,			"AltKeyCheck")
	IDMAP_ADD(IDC_HOTKEYSET_WINKEY_CHK,			"WinKeyCheck")
	IDMAP_ADD(IDC_HOTKEYSET_FUNCKEY_TITLE,		"FunctionKeyTitle")
	IDMAP_ADD(IDC_HOTKEYSET_FUNCKEY_LIST,		"FunctionKeyListCombo")
END_ID_MAPPING()


BEGIN_MESSAGE_MAP(CHotkeySetDlg, SDialog)
	ON_BN_CLICKED(IDC_HOTKEYSET_APPLY_BTN,					&CHotkeySetDlg::OnApply)
	ON_BN_CLICKED(IDC_HOTKEYSET_CANCEL_BTN,					&CHotkeySetDlg::OnCancel)
	ON_BN_CLICKED(IDC_HOTKEYSET_ADD_BTN,					&CHotkeySetDlg::OnAdd)
	ON_BN_CLICKED(IDC_HOTKEYSET_REMOVE_BTN,					&CHotkeySetDlg::OnRemove)
	ON_BN_CLICKED(IDC_HOTKEYSET_REMOVEALL_BTN,				&CHotkeySetDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_HOTKEYSET_CHECKALL_BTN,				&CHotkeySetDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_HOTKEYSET_EXPORT_BTN,					&CHotkeySetDlg::OnExport)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_HOTKEYSET_ITEM_LISTBOX,	&CHotkeySetDlg::OnSelectHotkeyItem)
	ON_NOTIFY(NM_CLICK, IDC_HOTKEYSET_ITEM_LISTBOX,			&CHotkeySetDlg::OnClickHotkeyList)
	ON_NOTIFY(NM_RCLICK, IDC_HOTKEYSET_ITEM_LISTBOX,		&CHotkeySetDlg::OnRightClickHotkeyList)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// HotkeySetDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize HotkeySet dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_STARTUP);

	// Load data
	LoadHotkeySetData();

	// Init dialog items
	SetupLanguage();

	// Update data
	UpdateHotkeySet();
	DisplayHotkeyDetails(DEF_INTEGER_INVALID);
	RefreshDlgItemState();

	// Read-only mode (if enabled)
	if (GetReadOnlyMode() == TRUE) {
		CWnd* pWndChild = GetTopWindow();
		while (pWndChild != NULL) {
			pWndChild->EnableWindow(FALSE);
			pWndChild = pWndChild->GetWindow(GW_HWNDNEXT);
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnApply
//	Description:	Handle click event for [Apply] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_APPLY_BTN);

	// Save data if changed
	m_bChangeFlag = CheckDataChangeState();
	if (m_bChangeFlag == TRUE) {
		// Save data
		SaveHotkeySetData();
	}

	// Close dialog
	SDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCancel
//	Description:	Handle click event for [Cancel] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnCancel()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_CANCEL_BTN);

	// Ask for saving before exiting if data changed
	m_bChangeFlag = CheckDataChangeState();
	if (m_bChangeFlag == TRUE) {
		int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, MSGBOX_HOTKEYSET_CAPTION, MB_YESNO | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SaveHotkeySetData();
		}
	}

	// Close dialog
	SDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnAdd
//	Description:	Handle click event for [Add] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnAdd()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_ADD_BTN);

	// Update data
	Add();
	UpdateHotkeySet();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemove
//	Description:	Handle click event for [Remove] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnRemove()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = m_hksHotkeySetTemp.nItemNum;
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nIndex = GetListCurSel(m_listHotkeySet);
	if (m_hksHotkeySetTemp.IsEmpty(nIndex) == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_REMOVE_ITEM, MSGBOX_HOTKEYSET_CAPTION, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		Remove(nIndex);
		UpdateHotkeySet();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemoveAll
//	Description:	Handle click event for [Remove All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnRemoveAll()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_hksHotkeySetTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_REMOVEALL_ITEMS, MSGBOX_HOTKEYSET_CAPTION, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		RemoveAll();
		UpdateHotkeySet();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnCheckAll
//	Description:	Handle click event for [Check All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnCheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_hksHotkeySetTemp.IsAllEmpty() == TRUE)
		return;

	// Check/uncheck all item
	SwitchAllItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnExport
//	Description:	Handle click event for [Export] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnExport()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_HOTKEYSET_EXPORT_BTN);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSelectHotkeyItem
//	Description:	Show hotkey details when selecting an item
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnSelectHotkeyItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	//Get current selection index
	int nCurSel = GetListCurSel(m_listHotkeySet);
	int nItemCount = m_listHotkeySet.GetItemCount();

	*pResult = NULL;

	// Invalid selection
	if (nCurSel < 0 || nCurSel >= nItemCount)
		return;

	// Display item details
	DisplayHotkeyDetails(nCurSel);
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClickHotkeyList
//	Description:	Handle click event on HotkeyList control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* plvItem = (NMLISTVIEW*)pNMHDR;
	int nItemCount = m_listHotkeySet.GetItemCount();

	LVHITTESTINFO lvHTInfo;
	lvHTInfo.pt = plvItem->ptAction;
	m_listHotkeySet.SubItemHitTest(&lvHTInfo);

	if ((lvHTInfo.iItem < 0) || (lvHTInfo.iItem > nItemCount)) {
		int nItem = GetListCurSel(m_listHotkeySet);
		m_listHotkeySet.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}
	else {
		m_listHotkeySet.SetItemState(lvHTInfo.iItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
	}

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRightClickHotkeyList
//	Description:	Handle right click event on HotkeyList control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnRightClickHotkeyList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* plvItem = (NMLISTVIEW*)pNMHDR;
	int nItemCount = m_listHotkeySet.GetItemCount();

	if (nItemCount > 0) {
		LVHITTESTINFO lvHTInfo;
		lvHTInfo.pt = plvItem->ptAction;
		m_listHotkeySet.SubItemHitTest(&lvHTInfo);

		if ((lvHTInfo.iItem < 0) || (lvHTInfo.iItem > nItemCount)) {
			int nItem = GetListCurSel(m_listHotkeySet);
			m_listHotkeySet.SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
		else {
			m_listHotkeySet.SetItemState(lvHTInfo.iItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SetupLanguage()
{
	// Load app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Setup dialog title
	CString strWndText = GetLanguageString(pAppLang, GetDialogID());
	this->SetWindowText(strWndText);

	// Loop through all dialog items and setup language for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_HOTKEYSET_DETAIL_STATIC:
			// Skip these items
			break;
		case IDC_HOTKEYSET_ITEM_LISTBOX:
			SetupHotkeySetList(pAppLang);
			break;
		case IDC_HOTKEYSET_ACTION_LIST:
		case IDC_HOTKEYSET_FUNCKEY_LIST:
			SetupComboBox(nID, pAppLang);
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupHotkeySetList
//	Description:	Initialize and setup language for HotkeySet list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SetupHotkeySetList(LANGTABLE_PTR ptrLanguage)
{
	// Get list control size (width)
	RECT rcHotkeySetList;
	m_listHotkeySet.GetWindowRect(&rcHotkeySetList);
	int nViewWidth = rcHotkeySetList.right - rcHotkeySetList.left;
	nViewWidth -= DEF_OFFSET_LISTCTRLWIDTH;

	// Fix list control width in case vertical scrollbar is displayed
	int nRowHeight = DEF_LISTCTRL_ROWHEIGHT;
	int nViewHeight = (rcHotkeySetList.bottom - rcHotkeySetList.top);
	int nTableHeight = ((m_hksHotkeySetTemp.nItemNum * nRowHeight) + DEF_LISTCTRL_HEADERHEIGHT);
	if (nTableHeight > nViewHeight) {
		int nScrollbarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nViewWidth -= nScrollbarWidth;
	}

	// Define column format
	LVCOLUMNFORMAT lvColumnFormat[] = {
	//----Column title ID----------------Column size-------Column size unit------
		LVCOLUMN_EMPTY_TITLE,				24,				COLSIZE_PIXEL,
		LVCOLUMN_HOTKEYSET_ACTION,			40,				COLSIZE_PERCENT,
		LVCOLUMN_HOTKEYSET_KEYSTROKES,		60,				COLSIZE_PERCENT,
	//---------------------------------------------------------------------------
	};

	// Update extended style
	DWORD dwStyle = m_listHotkeySet.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES;
	m_listHotkeySet.SetExtendedStyle(dwStyle);

	LVCOLUMN lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	// Setup columns
	int nColNum = (sizeof(lvColumnFormat) / sizeof(LVCOLUMNFORMAT));
	for (int nIndex = 0; nIndex < nColNum; nIndex++) {
		// Column index
		lvColumn.iSubItem = nIndex;

		// Column title
		CString strTitle = GetLanguageString(ptrLanguage, lvColumnFormat[nIndex].nColumnTitleID);
		lvColumn.pszText = strTitle.GetBuffer();
		
		// Column size
		int nColWidth = lvColumnFormat[nIndex].nColumnSize;
		int nColSizeUnit = lvColumnFormat[nIndex].nColumnSizeUnit;
		if (nColSizeUnit == COLSIZE_PIXEL) {
			lvColumn.cx = nColWidth;
			nViewWidth -= nColWidth;
		}
		else if (nColSizeUnit == COLSIZE_PERCENT) {
			int nColWidthPx = floor((float)nViewWidth * nColWidth / 100);
			lvColumn.cx = nColWidthPx;
		}

		// Column format
		lvColumn.fmt = LVCFMT_FIXED_WIDTH | LVCFMT_CENTER;

		// Insert column
		m_listHotkeySet.InsertColumn(nIndex, &lvColumn);
		strTitle.ReleaseBuffer();
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupComboBox
//	Description:	Setup data for combo-boxes
//  Arguments:		nComboID	- ID of combo box
//					ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
{
	switch (nComboID)
	{
	case IDC_HOTKEYSET_ACTION_LIST:
		m_cmbActionList.ResetContent();
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SLEEP));			// Sleep
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SHUTDOWN));		// Shutdown
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_RESTART));			// Restart
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_SIGNOUT));			// Log out
		m_cmbActionList.AddString(GetLanguageString(ptrLanguage, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;
	case IDC_HOTKEYSET_FUNCKEY_LIST:
		m_cmbFuncKeyList.ResetContent();
		for (int nIndex = 0; nIndex < strplFuncKeyList.size(); nIndex++)
			m_cmbFuncKeyList.AddString(strplFuncKeyList.at(nIndex).lpszLangString);
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDlgItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::RefreshDlgItemState()
{
	// Enable/disable buttons if data is all empty or not
	BOOL bIsAllEmpty = m_hksHotkeySetTemp.IsAllEmpty();
	EnableControl(IDC_HOTKEYSET_REMOVE_BTN, !bIsAllEmpty);
	EnableControl(IDC_HOTKEYSET_REMOVEALL_BTN, !bIsAllEmpty);

	// Enable/disable buttons if data changed or not
	m_bChangeFlag = CheckDataChangeState();
	EnableControl(IDC_HOTKEYSET_APPLY_BTN, m_bChangeFlag);

	// Update "Check All" button state
	UpdateCheckAllBtnState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateCheckAllBtnState
//	Description:	Refresh and update state for [Check/Uncheck All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateCheckAllBtnState()
{
	// Get all item enable state
	m_bAllChecked = TRUE;
	int nItemNum = m_hksHotkeySetTemp.nItemNum;
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		HOTKEYSETITEM& hksTemp = m_hksHotkeySetTemp.GetItemAt(nIndex);
		if (hksTemp.bEnable == FALSE) {
			m_bAllChecked = FALSE;
			break;
		}
	}

	// Update button state/title
	int nBtnTitleID = (m_bAllChecked == FALSE) ? BTN_HKSETDLG_CHECKALL : BTN_HKSETDLG_UNCHECKALL;
	LANGTABLE_PTR ptrLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	CWnd* pCheckAllBtn = GetDlgItem(IDC_HOTKEYSET_CHECKALL_BTN);
	if (pCheckAllBtn != NULL) {
		SetControlText(pCheckAllBtn, nBtnTitleID, ptrLang);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateHotkeySet
//	Description:	Update HotkeySet list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateHotkeySet()
{
	// Remove all existing items in list to reupdate
	m_listHotkeySet.DeleteAllItems();

	// If there's no item, do nothing
	int nItemNum = m_hksHotkeySetTemp.nItemNum;
	if (nItemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Print items
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		HOTKEYSETITEM hksItem = m_hksHotkeySetTemp.GetItemAt(nIndex);

		UINT nActionNameID = GetPairedID(idplActionName, GetPairedID(idplHKActionID, hksItem.nHKActionID));
		CString strAction = GetLanguageString(ptrLanguage, nActionNameID);
		CString strKeyStrokes = DEF_STRING_EMPTY;
		if (hksItem.dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
		if (hksItem.dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
		if (hksItem.dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
		strKeyStrokes += GetPairedString(strplFuncKeyList, hksItem.dwFuncKeyCode);
		CString strEnable = (hksItem.bEnable == TRUE) ? _T("Enabled") : _T("Disabled");

		// Output debug log
		OutputDebugLogFormat(_T("UpdateHotkeySet: State = %s, Action = %s, Keystrokes = %s"), strEnable, strAction, strKeyStrokes);
		
		// Print data
		int nItem = m_listHotkeySet.InsertItem(nIndex, DEF_STRING_EMPTY);
		m_listHotkeySet.SetCheck(nItem, hksItem.bEnable);
		m_listHotkeySet.SetItemText(nItem, 1, strAction);

		if ((strKeyStrokes != DEF_STRING_EMPTY) && (strKeyStrokes != DEF_STRING_NULL)) {
			m_listHotkeySet.SetItemText(nItem, 2, strKeyStrokes);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateHotkeySet
//	Description:	Update HotkeySet list item by index
//  Arguments:		nIndex - Index of item to update
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateHotkeySet(int nIndex)
{
	// If there's no item, do nothing
	int nItemNum = m_hksHotkeySetTemp.nItemNum;
	if (nItemNum <= 0)
		return;

	// Invalid index
	if ((nIndex < 0) || (nIndex >= nItemNum))
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Update data
	HOTKEYSETITEM hksItem = m_hksHotkeySetTemp.GetItemAt(nIndex);

	UINT nActionNameID = GetPairedID(idplActionName, GetPairedID(idplHKActionID, hksItem.nHKActionID));
	CString strAction = GetLanguageString(ptrLanguage, nActionNameID);
	CString strKeyStrokes = DEF_STRING_EMPTY;
	if (hksItem.dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
	if (hksItem.dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
	if (hksItem.dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
	strKeyStrokes += GetPairedString(strplFuncKeyList, hksItem.dwFuncKeyCode);
	CString strEnable = (hksItem.bEnable == TRUE) ? _T("Enabled") : _T("Disabled");

	// Output debug log
	OutputDebugLogFormat(_T("UpdateHotkeySet: Status = %s, Action = %s, Keystrokes = %s"), strAction, strKeyStrokes, strEnable);

	// Update hotkeyset list
	m_listHotkeySet.SetRedraw(FALSE);
	m_listHotkeySet.DeleteItem(nIndex);

	int nItem = m_listHotkeySet.InsertItem(nIndex, DEF_STRING_EMPTY);
	m_listHotkeySet.SetCheck(nItem, hksItem.bEnable);
	m_listHotkeySet.SetItemText(nItem, 1, strAction);
	m_listHotkeySet.SetItemText(nItem, 2, strKeyStrokes);

	m_listHotkeySet.SetRedraw(TRUE);
	m_listHotkeySet.UpdateWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayHotkeyDetails
//	Description:	Display details of an item at specified index
//  Arguments:		nIndex - Index of item to display
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::DisplayHotkeyDetails(int nIndex)
{
	// Reset default control states
	m_bCtrlBtn = FALSE;
	m_bAltBtn = FALSE;
	m_bWinKeyBtn = FALSE;

	// If index is invalid, display dummy details
	if ((nIndex < 0) || (nIndex >= m_hksHotkeySetTemp.nItemNum)) {
		m_cmbActionList.SetWindowText(_T("---"));
		m_cmbFuncKeyList.SetWindowText(_T("---"));
		UpdateData(FALSE);
		return;
	}

	// Get item at index
	HOTKEYSETITEM hksCurItem = m_hksHotkeySetTemp.GetItemAt(nIndex);

	// Update checkboxes
	m_bCtrlBtn = FALSE;
	if (hksCurItem.dwCtrlKeyCode & MOD_CONTROL)		m_bCtrlBtn = TRUE;
	m_bAltBtn = FALSE;
	if (hksCurItem.dwCtrlKeyCode & MOD_ALT)			m_bAltBtn = TRUE;
	m_bWinKeyBtn = FALSE;
	if (hksCurItem.dwCtrlKeyCode & MOD_WIN)			m_bWinKeyBtn = TRUE;

	// Update combo-boxes
	UINT nActionID = GetPairedID(idplHKActionID, hksCurItem.nHKActionID);
	m_cmbActionList.SetCurSel(Opt2Sel(DEF_APP_ACTION, nActionID));
	m_cmbFuncKeyList.SetWindowText(_T("---"));
	if (hksCurItem.dwFuncKeyCode > 0)
		m_cmbFuncKeyList.SetCurSel(hksCurItem.dwFuncKeyCode - VK_F1);

	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadHotkeySetData
//	Description:	Load hotkeyset data
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::LoadHotkeySetData()
{
	// Get app hotkeyset data pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	PHOTKEYSETDATA phksData = pApp->GetAppHotkeySetData();
	if (phksData == NULL)
		return FALSE;

	// Copy data
	m_hksHotkeySet.Copy(*phksData);
	m_hksHotkeySetTemp.Copy(m_hksHotkeySet);

	// Reset change flag
	m_bChangeFlag = FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveHotkeySetData
//	Description:	Save hotkeyset data
//  Arguments:		None
//  Return value:	BOOL - Result of saving process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::SaveHotkeySetData()
{
	// Copy data and adjust validity
	m_hksHotkeySet.Copy(m_hksHotkeySetTemp);
	m_hksHotkeySet.Adjust();

	// Reset change flag
	m_bChangeFlag = FALSE;

	// Save app HotkeySet data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	pApp->SetAppHotkeySetData(&m_hksHotkeySet);
	pApp->SaveRegistryAppData(APPDATA_HOTKEYSET);

	// Notify main dialog to re-update HotkeySet data
	CPowerPlusDlg* pMainDlg = (CPowerPlusDlg*)(pApp->GetMainWnd());
	VERIFY(pMainDlg != NULL);
	if (pMainDlg == NULL) return FALSE;
	pMainDlg->PostMessage(SM_APP_UPDATE_HOTKEYSETDATA, NULL, NULL);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CheckDataChangeState
//	Description:	Check if settings changed
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::CheckDataChangeState()
{
	BOOL bChangeFlag = FALSE;

	// Update enable state of all item before checking
	int nItemNum = m_listHotkeySet.GetItemCount();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		BOOL bEnable = m_listHotkeySet.GetCheck(nIndex);
		HOTKEYSETITEM& hksTempItem = m_hksHotkeySetTemp.GetItemAt(nIndex);
		hksTempItem.bEnable = bEnable;
	}

	// Check if number of items changed
	bChangeFlag |= (m_hksHotkeySetTemp.nItemNum != m_hksHotkeySet.nItemNum);
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each item's data changed
	for (int nIndex = 0; nIndex < m_hksHotkeySetTemp.nItemNum; nIndex++) {
		// Get current item and temp item
		HOTKEYSETITEM hksCurItem = m_hksHotkeySet.GetItemAt(nIndex);
		HOTKEYSETITEM hksTempItem = m_hksHotkeySetTemp.GetItemAt(nIndex);
 
		// Data comparison
		bChangeFlag |= (hksTempItem.bEnable != hksCurItem.bEnable);
		bChangeFlag |= (hksTempItem.nHKActionID != hksCurItem.nHKActionID);
		bChangeFlag |= (hksTempItem.dwCtrlKeyCode != hksCurItem.dwCtrlKeyCode);
		bChangeFlag |= (hksTempItem.dwFuncKeyCode != hksCurItem.dwFuncKeyCode);

		// Stop on the first different item encountered
		if (bChangeFlag == TRUE) break;
	}

	return bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add current settings to HotkeySet data list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::Add()
{
	// Update data
	UpdateData(TRUE);

	// Create temp hotkeyset item
	HOTKEYSETITEM hksTemp = {0};

	if (m_bCtrlBtn == TRUE)		hksTemp.dwCtrlKeyCode |= MOD_CONTROL;
	if (m_bAltBtn == TRUE)		hksTemp.dwCtrlKeyCode |= MOD_ALT;
	if (m_bWinKeyBtn == TRUE)	hksTemp.dwCtrlKeyCode |= MOD_WIN;

	// Update combo-boxes
	int nCurSel = m_cmbActionList.GetCurSel();
	int nActionID = Sel2Opt(DEF_APP_ACTION, nCurSel);
	int nHKActionID = idplHKActionID[nCurSel].nFirstID;
	hksTemp.nHKActionID = nHKActionID;

	nCurSel = m_cmbFuncKeyList.GetCurSel();
	hksTemp.dwFuncKeyCode = nCurSel + VK_F1;

	// Check data validity
	BOOL bValid = Validate(hksTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to HotkeySet data list
	m_hksHotkeySetTemp.Update(hksTemp);

	// Refresh button state
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a HotkeySet item by index
//  Arguments:		nIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::Remove(int nIndex)
{
	// Remove item at index
	m_hksHotkeySetTemp.Remove(nIndex);

	// Refresh button state
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all HotkeySet items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::RemoveAll()
{
	// Remove all items
	m_hksHotkeySetTemp.RemoveAll();

	// Refresh button state
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SwitchAllItemState
//	Description:	Check/uncheck all HotkeySet items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SwitchAllItemState()
{
	// Check/uncheck all --> Update all item enable state
	int nItemNum = m_hksHotkeySetTemp.nItemNum;
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		HOTKEYSETITEM& hksTemp = m_hksHotkeySetTemp.GetItemAt(nIndex);
		if (hksTemp.bEnable == m_bAllChecked) {
			hksTemp.bEnable = !m_bAllChecked;
		}
	}

	// Update hotkeyset list
	UpdateHotkeySet();

	// Refresh button state
	RefreshDlgItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Validate
//	Description:	Check hotkeyset item validity
//  Arguments:		hksItem  - Item to validate
//					bShowMsg - Show validation message box or not
//  Return value:	BOOL - Result of validation process
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::Validate(HOTKEYSETITEM hksItem, BOOL bShowMsg /* = FALSE */)
{
	BOOL bResult = TRUE;
	int nMsgStringID;
	CStringArray arrMsgString;
	arrMsgString.RemoveAll();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check action ID
	if ((hksItem.nHKActionID < HKID_DISPLAYOFF) || (hksItem.nHKActionID > HKID_HIBERNATE)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Check control key value
	if ((hksItem.dwCtrlKeyCode <= 0) || 
		((hksItem.dwCtrlKeyCode & MOD_CONTROL) == FALSE) && 
		((hksItem.dwCtrlKeyCode & MOD_ALT) == FALSE) && 
		((hksItem.dwCtrlKeyCode & MOD_WIN) == FALSE)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Check function key value
	if ((hksItem.dwFuncKeyCode < VK_F1) || (hksItem.dwFuncKeyCode > VK_F12)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Check if system hotkey existed
	int nExistedSysHotkeyNum = (sizeof(hklExistedSysHotkeyList) / sizeof(HOTKEYINFO));
	for (int nIndex = 0; nIndex < nExistedSysHotkeyNum; nIndex++) {
		if ((hksItem.dwCtrlKeyCode == hklExistedSysHotkeyList[nIndex].dwCtrlKeyCode) &&
			(hksItem.dwFuncKeyCode == hklExistedSysHotkeyList[nIndex].dwFuncKeyCode)) {
			// Hotkey info format
			CString strKeyStrokes = DEF_STRING_EMPTY;
			if (hksItem.dwCtrlKeyCode & MOD_CONTROL)	strKeyStrokes += _T("Ctrl + ");
			if (hksItem.dwCtrlKeyCode & MOD_ALT)		strKeyStrokes += _T("Alt + ");
			if (hksItem.dwCtrlKeyCode & MOD_WIN)		strKeyStrokes += _T("Win + ");
			strKeyStrokes += GetPairedString(strplFuncKeyList, hksItem.dwFuncKeyCode);
			CString strKeyInfo = DEF_STRING_EMPTY;
			strKeyInfo.Format(_T("%s - %s"), strKeyStrokes, GetLanguageString(pLang, hklExistedSysHotkeyList[nIndex].nHotkeyDescription));

			// Message format
			CString strMsgFormat;
			strMsgFormat.Format(GetLanguageString(pLang, MSGBOX_HOTKEYSET_EXISTED_HOTKEY), strKeyInfo);

			arrMsgString.Add(strMsgFormat);
			bResult = FALSE;
		}
	}
	
	// Show error message if enabled
	if ((bShowMsg == TRUE) && (!arrMsgString.IsEmpty())) {
		for (int nIndex = 0; nIndex < arrMsgString.GetSize(); nIndex++) {
			DisplayMessageBox(arrMsgString.GetAt(nIndex), MSGBOX_HOTKEYSET_CAPTION, MB_OK | MB_ICONERROR);
		}
	}

	// Remove all message after displaying
	arrMsgString.RemoveAll();
	arrMsgString.FreeExtra();
	return bResult;
}
