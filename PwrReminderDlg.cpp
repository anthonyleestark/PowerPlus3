
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		PwrReminderDlg.cpp
//		Description:	Source file for Power Reminder dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.07.20:		Create new
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PowerPlus.h"
#include "PowerPlusDlg.h"
#include "PwrReminderDlg.h"
#include "ReminderMsgDlg.h"
#include "RmdRepeatSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace PairFuncs;
using namespace CoreFuncs;


////////////////////////////////////////////////////////
//
//	Define macros for active day list table
//
////////////////////////////////////////////////////////

#define COL_FIXED_NUM			1
#define ROW_FIXED_NUM			1


//////////////////////////////////////////////////////////////////////////
//
//	Implement methods for CPwrReminderDlg
//
//////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPwrReminderDlg, SDialog)

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	CPwrReminderDlg
//	Description:	Constructor
//
//////////////////////////////////////////////////////////////////////////

CPwrReminderDlg::CPwrReminderDlg(CWnd* pParent /*=nullptr*/)
	: SDialog(IDD_PWRREMINDER_DLG, pParent)
{
	// Initialize member variables
	m_pDataItemListTable = NULL;
	m_pMsgStringEdit = NULL;
	m_pEvtSetTimeRad = NULL;
	m_pEvtSetTimeEdit = NULL;
	m_pEvtSetTimeSpin = NULL;
	m_pEvtRepeatSetBtn = NULL;
	m_pEvtAppStartupRad = NULL;
	m_pEvtSysWakeupRad = NULL;
	m_pEvtBfrPwrActionRad = NULL;
	m_pEvtPwrActionWakeRad = NULL;
	m_pEvtAtAppExitRad = NULL;
	m_pStyleMsgBoxRad = NULL;
	m_pStyleDialogBoxRad = NULL;
	m_pMsgStyleCombo = NULL;

	// Properties child dialogs
	m_pRmdPreviewMsgDlg = NULL;
	m_pRepeatSetDlg = NULL;

	// Data container variables
	ZeroMemory(&m_pwrReminderData, sizeof(PWRREMINDERDATA));
	ZeroMemory(&m_pwrReminderData, sizeof(PWRREMINDERDATA));

	// Checkbox/radio button variables
	m_bEvtSetTimeRad = FALSE;
	m_bEvtAppStartupRad = FALSE;
	m_bEvtSysWakeupRad = FALSE;
	m_bEvtBfrPwrActionRad = FALSE;
	m_bEvtPwrActionWakeRad = FALSE;
	m_bEvtAppExitRad = FALSE;
	m_bStyleMsgBoxRad = FALSE;
	m_bStyleDialogRad = FALSE;

	// Table format and properties
	m_nColNum = 0;
	m_pszFrameWndSize = NULL;
	m_apGrdColFormat = NULL;

	// Other variables
	m_nCurMode = 0;
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;
	m_nCurDispIndex = -2;
	m_stDispTimeBak = {0};

	INIT_CLASS_IDMAP()
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CPwrReminderDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CPwrReminderDlg::~CPwrReminderDlg()
{
	// Delete child dialogs
	if (m_pRmdPreviewMsgDlg != NULL) {
		// Destroy dialog
		m_pRmdPreviewMsgDlg->DestroyWindow();
		delete m_pRmdPreviewMsgDlg;
		m_pRmdPreviewMsgDlg = NULL;
	}
	if (m_pRepeatSetDlg != NULL) {
		// Destroy dialog
		m_pRepeatSetDlg->DestroyWindow();
		delete m_pRepeatSetDlg;
		m_pRepeatSetDlg = NULL;
	}

	// Data item list control
	if (m_pDataItemListTable) {
		delete m_pDataItemListTable;
		m_pDataItemListTable = NULL;
	}
	
	// Remove Reminder data
	m_pwrReminderData.DeleteAll();
	m_pwrReminderDataTemp.DeleteAll();

	// Table format and properties
	if (m_pszFrameWndSize != NULL) {
		delete m_pszFrameWndSize;
		m_pszFrameWndSize = NULL;
	}

	if (m_apGrdColFormat != NULL) {
		delete[] m_apGrdColFormat;
		m_apGrdColFormat = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DoDataExchange
//	Description:	DoDataExchange function (DDX/DDV support)
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,		m_bEvtSetTimeRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,		m_bEvtAppStartupRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,		m_bEvtSysWakeupRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,	m_bEvtBfrPwrActionRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,	m_bEvtPwrActionWakeRad);
	DDX_Check(pDX, IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,		m_bEvtAppExitRad);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,		m_bStyleMsgBoxRad);
	DDX_Check(pDX, IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,		m_bStyleDialogRad);
}


//////////////////////////////////////////////////////////////////////////
//
//	CPwrReminderDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_ID_MAPPING(CPwrReminderDlg)
	IDMAP_ADD(IDD_PWRREMINDER_DLG,							"PwrReminderDlg")
	IDMAP_ADD(IDC_PWRREMINDER_ITEM_LISTBOX,					"PwrReminderItemList")
	IDMAP_ADD(IDC_PWRREMINDER_ADD_BTN,						"AddButton")
	IDMAP_ADD(IDC_PWRREMINDER_ADD_BTN,						"EditButton")
	IDMAP_ADD(IDC_PWRREMINDER_REMOVE_BTN,					"RemoveButton")
	IDMAP_ADD(IDC_PWRREMINDER_REMOVEALL_BTN,				"RemoveAllButton")
	IDMAP_ADD(IDC_PWRREMINDER_CHECKALL_BTN,					"CheckAllButton")
	IDMAP_ADD(IDC_PWRREMINDER_UNCHECKALL_BTN,				"UncheckAllButton")
	IDMAP_ADD(IDC_PWRREMINDER_PREVIEW_BTN,					"PreviewButton")
	IDMAP_ADD(IDC_PWRREMINDER_APPLY_BTN,					"ApplyButton")
	IDMAP_ADD(IDC_PWRREMINDER_CANCEL_BTN,					"CancelButton")
	IDMAP_ADD(IDC_PWRREMINDER_DETAIL_STATIC,				"DetailGroup")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTRING_TITLE,				"MsgContentTitle")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTRING_EDITBOX,			"MsgContentEdit")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTRING_COUNTER,			"MsgCounter")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_TITLE,					"EventGroup")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN,			"EventSetTimeRadio")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		"EventSetTimeEdit")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_SETTIME_SPIN,			"EventSetTimeSpin")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_REPEATSET_BTN,			"RepeatSetButton")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN,		"EventAppStartupRadio")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN,		"EventSysWakeupRadio")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN,	"EventBfrPwrActionRadio")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN,	"EventPwrActionWakeRadio")
	IDMAP_ADD(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN,		"EventAtAppExitRadio")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTYLE_TITLE,				"MsgStyleGroup")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN,		"StyleMessageBoxRadio")
	IDMAP_ADD(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN,		"StyleDialogBoxRadio")
END_ID_MAPPING()


//////////////////////////////////////////////////////////////////////////
//
//	CPwrReminderDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CPwrReminderDlg, SDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PWRREMINDER_APPLY_BTN,					&CPwrReminderDlg::OnApply)
	ON_BN_CLICKED(IDC_PWRREMINDER_CANCEL_BTN,					&CPwrReminderDlg::OnCancel)
	ON_BN_CLICKED(IDC_PWRREMINDER_ADD_BTN,						&CPwrReminderDlg::OnAdd)
	ON_BN_CLICKED(IDC_PWRREMINDER_EDIT_BTN,						&CPwrReminderDlg::OnEdit)
	ON_BN_CLICKED(IDC_PWRREMINDER_REMOVE_BTN,					&CPwrReminderDlg::OnRemove)
	ON_BN_CLICKED(IDC_PWRREMINDER_REMOVEALL_BTN,		 		&CPwrReminderDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_CHECKALL_BTN,			 		&CPwrReminderDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_UNCHECKALL_BTN,				&CPwrReminderDlg::OnUncheckAll)
	ON_BN_CLICKED(IDC_PWRREMINDER_PREVIEW_BTN,					&CPwrReminderDlg::OnPreviewItem)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PWRREMINDER_ITEM_LISTBOX,	&CPwrReminderDlg::OnSelectReminderItem)
	ON_NOTIFY(NM_CLICK, IDC_PWRREMINDER_ITEM_LISTBOX,			&CPwrReminderDlg::OnClickDataItemList)
	ON_NOTIFY(NM_RCLICK, IDC_PWRREMINDER_ITEM_LISTBOX,			&CPwrReminderDlg::OnRightClickDataItemList)
	ON_EN_CHANGE(IDC_PWRREMINDER_MSGSTRING_EDITBOX,				&CPwrReminderDlg::OnMsgContentEditChange)
	ON_EN_SETFOCUS(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		&CPwrReminderDlg::OnTimeEditSetFocus)
	ON_EN_KILLFOCUS(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX,		&CPwrReminderDlg::OnTimeEditKillFocus)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PWRREMINDER_EVENT_SETTIME_SPIN,	&CPwrReminderDlg::OnTimeSpinChange)
	ON_BN_CLICKED(IDC_PWRREMINDER_EVENT_REPEATSET_BTN,			&CPwrReminderDlg::OnRepeatSet)
	ON_COMMAND_RANGE(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN, IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN, &CPwrReminderDlg::OnPwrEventRadBtnClicked)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementations

// PwrReminderDlg message handlers

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnInitDialog
//	Description:	Initialize Power Reminder dialog and setup dialog content
//  Arguments:		None
//  Return value:	BOOL - Default
//
//////////////////////////////////////////////////////////////////////////

BOOL CPwrReminderDlg::OnInitDialog()
{
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_STARTUP);

	// Load data
	LoadPwrReminderData();

	// Init dialog items
	SetupLanguage();
	SetupDialogItemState();

	// Update data
	UpdateDataItemList();
	DisplayItemDetails(DEF_INTEGER_INVALID);
	RefreshDlgItemState(TRUE);

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
//	Function name:	OnClose
//	Description:	Default method for dialog closing
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnClose()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Exit current mode
		int nConfirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode == DEF_MODE_ADD) || (nCurMode == DEF_MODE_UPDATE)) {
			// Show switch mode confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Switch mode
				SetCurMode(DEF_MODE_VIEW);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		m_bChangeFlag = CheckDataChangeState();
		if (m_bChangeFlag == TRUE) {
			// Show save confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SavePwrReminderData();
			}
		}
	}

	// Save app event log if enabled
	OutputDialogLog(GetDialogID(), LOG_EVENT_DLG_DESTROYED);

	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnApply
//	Description:	Handle click event for [Apply] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnApply()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_APPLY_BTN);

	// Save data if changed
	m_bChangeFlag = CheckDataChangeState();
	if (m_bChangeFlag == TRUE) {
		// Save data
		SavePwrReminderData();
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

void CPwrReminderDlg::OnCancel()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_CANCEL_BTN);

		// Exit current mode
		int nConfirm = -1;
		int nCurMode = GetCurMode();
		if ((nCurMode == DEF_MODE_ADD) || (nCurMode == DEF_MODE_UPDATE)) {
			// Show switch mode confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Switch mode
				SetCurMode(DEF_MODE_VIEW);
			}
			return;
		}

		// Ask for saving before exiting if data changed
		m_bChangeFlag = CheckDataChangeState();
		if (m_bChangeFlag == TRUE) {
			// Show save confirmation message
			nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SavePwrReminderData();
			}
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

void CPwrReminderDlg::OnAdd()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_ADD_BTN);

	// Get current mode
	int nCurMode = GetCurMode();

	// Current mode: Add
	if (nCurMode == DEF_MODE_ADD) {
		// Add item
		Add();

		// Reset mode
		SetCurMode(DEF_MODE_VIEW);
	}
	// Current mode: Edit
	else if (nCurMode == DEF_MODE_UPDATE) {
		// Do nothing
	}
	else {
		// Switch mode
		SetCurMode(DEF_MODE_ADD);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnEdit
//	Description:	Handle click event for [Edit] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnEdit()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_EDIT_BTN);

	// Get current mode
	int nCurMode = GetCurMode();

	// Mode: Edit
	if (nCurMode == DEF_MODE_UPDATE) {
		// Check if any item is selected or not
		BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetItemNum()));

		if (bIsSelected == TRUE) {
			// Edit current selected item
			Edit(m_nCurSelIndex);
		}

		// Reset mode
		SetCurMode(DEF_MODE_VIEW);
	}
	// Mode: Add
	else if (nCurMode == DEF_MODE_ADD) {
		// Do nothing
	}
	else {
		// Switch mode
		SetCurMode(DEF_MODE_UPDATE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRemove
//	Description:	Handle click event for [Remove] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnRemove()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = GetItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nIndex = m_nCurSelIndex;

	// If item at selected index is empy, do nothing
	if (m_pwrReminderDataTemp.IsEmpty(nIndex) == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_REMOVE_ITEM, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove item
		Remove(nIndex);
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

void CPwrReminderDlg::OnRemoveAll()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_REMOVEALL_BTN);
	
	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_REMOVEALL_ITEMS, MSGBOX_PWRREMINDER_CAPTION, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {
		// Remove all items
		RemoveAll();
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

void CPwrReminderDlg::OnCheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Check all items
	SetAllItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUncheckAll
//	Description:	Handle click event for [Uncheck All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnUncheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Uncheck all items
	SetAllItemState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPreviewItem
//	Description:	Handle click event for [Preview] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnPreviewItem()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_PREVIEW_BTN);

	// If all item are empty, do nothing
	if (m_pwrReminderDataTemp.IsAllEmpty() == TRUE)
		return;

	// Get current selection index
	int nCurSel = m_nCurSelIndex;
	if (m_pwrReminderDataTemp.IsEmpty(nCurSel) == TRUE)
		return;

	// Preview item
	int nIndex = nCurSel;
	PreviewItem(nIndex);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnSelectReminderItem
//	Description:	Show details when selecting a reminder item
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnSelectReminderItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	//Get current selection index
	m_nCurSelIndex = nRow - ROW_FIXED_NUM;
	int nItemCount = GetItemNum();

	*pResult = NULL;

	// Invalid selection
	if ((m_nCurSelIndex < 0) || (m_nCurSelIndex >= nItemCount))
		return;

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != DEF_MODE_INIT) && (nCurMode != DEF_MODE_VIEW))
		return;

	// Display item details
	DisplayItemDetails(m_nCurSelIndex);
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnClickDataItemList
//	Description:	Handle click event on Power Reminder data 
//					item list control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetItemNum();
	if ((nRow <= DEF_GRIDCTRL_ROWHEADER) || (nRow > nItemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != DEF_MODE_INIT) && (nCurMode != DEF_MODE_VIEW))
		return;

#if 0
	// Handle click event on Checkbox columns
	if (((nCol == PWRCOL_ID_STATE) || (nCol == PWRCOL_ID_REPEAT)) && (nRow != DEF_GRIDCTRL_ROWHEADER)) {
		if (m_pDataItemListTable == NULL) return;
		CGridCellCheck* clickedCell = (CGridCellCheck*)(m_pDataItemListTable->GetCell(nRow, nCol));
		if (clickedCell == NULL) return;

		// Change cell selected state
		BOOL bCheck = clickedCell->GetCheck();
		clickedCell->SetCheck(!bCheck);

		// Update cell
		m_pDataItemListTable->RedrawCell(nRow, nCol);
	}
#endif

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRightClickDataItemList
//	Description:	Handle right click event on Power Reminder data 
//					item list control
//  Arguments:		pNMHDR  - Default of notify/event handler
//					pResult - Default of notify/event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCol = pItem->iColumn;
	int nRow = pItem->iRow;

	// Check value validity
	int nItemNum = GetItemNum();
	if ((nRow <= DEF_GRIDCTRL_ROWHEADER) || (nRow > nItemNum)) {
		return;
	}

	// Check read-only mode
	int nCurMode = GetCurMode();
	if ((nCurMode != DEF_MODE_INIT) && (nCurMode != DEF_MODE_VIEW))
		return;

#if 0
	// Handle click event on Checkbox columns
	if (((nCol == PWRCOL_ID_STATE) || (nCol == PWRCOL_ID_REPEAT)) && (nRow != DEF_GRIDCTRL_ROWHEADER)) {
		if (m_pDataItemListTable == NULL) return;
		CGridCellCheck* clickedCell = (CGridCellCheck*)(m_pDataItemListTable->GetCell(nRow, nCol));
		if (clickedCell == NULL) return;

		// Change cell selected state
		BOOL bCheck = clickedCell->GetCheck();
		clickedCell->SetCheck(!bCheck);

		// Update cell
		m_pDataItemListTable->RedrawCell(nRow, nCol);
	}
#endif

	*pResult = NULL;

	// Refresh button states
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnMsgContentEditChange
//	Description:	Update when message content editbox value changes
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnMsgContentEditChange()
{
	// Check control validity
	if (m_pMsgStringEdit == NULL) {
		TRCLOG("Error: Message content edit control not found");
		TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
		return;
	}

	// Update data
	CString strText;
	m_pMsgStringEdit->GetWindowText(strText);

	// Update message counter
	int nCount = strText.GetLength();
	UpdateMsgCounter(nCount);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeEditSetFocus
//	Description:	Select all text when editbox is set focus
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnTimeEditSetFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit set focus --> Select all text			 */
	/*																	 */
	/*********************************************************************/

	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Backup current displaying time value
	UpdateTimeSetting(m_stDispTimeBak, TRUE);

	// Select all text
	m_pEvtSetTimeEdit->PostMessage(EM_SETSEL, 0, -1);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeEditKillFocus
//	Description:	Update when time editbox is killed focus
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnTimeEditKillFocus()
{
	/*********************************************************************/
	/*																	 */
	/*			TODO: Time edit kill forcus --> Update data			     */
	/*																	 */
	/*********************************************************************/

	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Update data
	CString strText;
	m_pEvtSetTimeEdit->GetWindowText(strText);

	SYSTEMTIME stTimeTemp;
	BOOL bRet = Text2Time(stTimeTemp, strText);
	if (bRet != FALSE) {
		// Update new time value
		UpdateTimeSetting(stTimeTemp, FALSE);

		// Update timespin new position
		int nSpinPos;
		Time2SpinPos(stTimeTemp, nSpinPos);
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->SetPos(nSpinPos);
		}
	}
	else {
		// Restore backed-up time value
		UpdateTimeSetting(m_stDispTimeBak, FALSE);
		return;
	}

	// Check for value change and enable/disable save button
	m_bChangeFlag = CheckDataChangeState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnTimeSpinChange
//	Description:	Update when time spin value changes
//  Arguments:		pNMDHR  - Default of spin event handler
//					pResult - Default of spin event handler
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnTimeSpinChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	// Get timespin position
	int nPos = pNMUpDown->iPos;

	// Convert to time value
	SYSTEMTIME stTimeTemp;
	SpinPos2Time(stTimeTemp, nPos);

	// Update time edit value
	UpdateTimeSetting(stTimeTemp, FALSE);

	*pResult = NULL;

	// Check for value change and enable/disable save button
	m_bChangeFlag = CheckDataChangeState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnPwrEventRadBtnClicked
//	Description:	Handle clicked for event radio buttons
//  Arguments:		nID - ID of button
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnPwrEventRadBtnClicked(UINT nID)
{
	// Ignore if clicked on items with in-range IDs but not radio buttons
	if ((nID == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) ||
		(nID == IDC_PWRREMINDER_EVENT_SETTIME_SPIN))
		return;

	// Get EventSetTime radio button
	if (m_pEvtSetTimeRad == NULL) {
		m_pEvtSetTimeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (m_pEvtSetTimeRad == NULL) {
			TRCLOG("Error: Radio button not found (EventSetTimeRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// If button is disabled, do nothing
	if (m_pEvtSetTimeRad->IsWindowEnabled() == FALSE)
		return;

	// Update checked state
	int nState = m_pEvtSetTimeRad->GetCheck();

	// Check control validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeSpin == NULL) {
		m_pEvtSetTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (m_pEvtSetTimeSpin == NULL) {
			TRCLOG("Error: Time spin control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Enable/disable time spinedit and RepeatSet button
	if (nState == 1) {
		m_pEvtSetTimeEdit->EnableWindow(TRUE);
		m_pEvtSetTimeSpin->EnableWindow(TRUE);
		m_pEvtRepeatSetBtn->EnableWindow(TRUE);
	}
	else {
		m_pEvtSetTimeEdit->EnableWindow(FALSE);
		m_pEvtSetTimeSpin->EnableWindow(FALSE);
		m_pEvtRepeatSetBtn->EnableWindow(FALSE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnRepeatSet
//	Description:	Handle click event for [RepeatSet] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::OnRepeatSet()
{
	// Save app event log if enabled
	OutputButtonLog(GetDialogID(), IDC_PWRREMINDER_EVENT_REPEATSET_BTN);

	// Initialize RepeatSet dialog if not available
	if (m_pRepeatSetDlg == NULL) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->RemoveDialogStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->SetParentWnd(this);
	}

	// Get button position
	CRect rcButton;
	if (m_pEvtRepeatSetBtn != NULL) {
		// Get button rectangle
		m_pEvtRepeatSetBtn->GetWindowRect(&rcButton);
	}

	// If the dialog has already been initialized
	if (m_pRepeatSetDlg != NULL) {
		// If the dialog is currently displaying
		if (m_pRepeatSetDlg->IsWindowVisible()) {
			// Hide the dialog
			m_pRepeatSetDlg->ShowWindow(SW_HIDE);
		}
		else {
			// Set dialog align
			UINT nAlign = SDA_LEFTALIGN | SDA_TOPALIGN;
			m_pRepeatSetDlg->SetDialogAlign(nAlign);

			// Get button top-right point
			POINT ptBtnTopRight;
			ptBtnTopRight.x = rcButton.right;
			ptBtnTopRight.y = rcButton.top;

			// Set dialog position
			m_pRepeatSetDlg->SetDialogPosition(ptBtnTopRight);

			// Show dialog
			m_pRepeatSetDlg->ShowWindow(SW_SHOW);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestCloseDialog
//	Description:	Request current dialog to close
//  Arguments:		None
//  Return value:	LRESULT (0:Success, else:Failed)
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPwrReminderDlg::RequestCloseDialog(void)
{
	// Close preview message dialog if opening
	if (m_pRmdPreviewMsgDlg != NULL) {
		// Request close message dialog
		LRESULT resClosePreview = m_pRmdPreviewMsgDlg->RequestCloseDialog();
		if (resClosePreview != DEF_RESULT_SUCCESS) {
			// Request denied
			return LRESULT(DEF_RESULT_FAILED);
		}
	}

	// Exit current mode
	int nConfirm = -1;
	int nCurMode = GetCurMode();
	if ((nCurMode == DEF_MODE_ADD) || (nCurMode == DEF_MODE_UPDATE)) {
		nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CONFIRM_EXITMODE, MSGBOX_PWRREMINDER_CAPTION, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Switch mode
			SetCurMode(DEF_MODE_VIEW);
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(DEF_RESULT_FAILED);
		}
	}

	// Ask for saving before exiting if data changed
	m_bChangeFlag = CheckDataChangeState();
	if (m_bChangeFlag == TRUE) {
		nConfirm = DisplayMessageBox(MSGBOX_PWRREMINDER_CHANGED_CONTENT, MSGBOX_PWRREMINDER_CAPTION, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SavePwrReminderData();
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(DEF_RESULT_FAILED);
		}
	}

	// Request accepted
	return SDialog::RequestCloseDialog();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	WindowProc
//	Description:	Pre-handle dialog messages
//  Arguments:		message - Message ID
//					wParam	- First param (HIWORD)
//					lParam	- Second param (LOWORD)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

LRESULT CPwrReminderDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONUP:
	{
		// Get clicked point
		POINT pt;
		pt.x = GET_X_LPARAM(lParam);
		pt.y = GET_Y_LPARAM(lParam);

		if (m_pEvtSetTimeEdit == NULL)
			return 0;

		// Get the editbox rect
		RECT rcEditBox;
		m_pEvtSetTimeEdit->GetWindowRect(&rcEditBox);
		ScreenToClient(&rcEditBox);

		// If clicked point is inside the editbox area
		if (((pt.x > rcEditBox.left) && (pt.x < rcEditBox.right)) &&
			((pt.y > rcEditBox.top) && (pt.y < rcEditBox.bottom))) {
			if (m_pEvtSetTimeEdit->IsWindowEnabled()) {
				// Select all text
				m_pEvtSetTimeEdit->SetSel(0, -1, TRUE);
			}
			return 0;
		}
		else {
			// If the edit box is focused, kill its focus
			CWnd* pCurCtrl = GetFocus();
			if (pCurCtrl == NULL) return 0;
			if (pCurCtrl->GetDlgCtrlID() == IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX) {
				pCurCtrl->PostMessage(WM_KILLFOCUS);
				this->SetFocus();	// Return focus to dialog
				return 0;
			}
		}
	} break;
	}

	return SDialog::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupLanguage
//	Description:	Setup language for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::SetupLanguage()
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
		case IDC_PWRREMINDER_DETAIL_STATIC:
		case IDC_PWRREMINDER_ITEM_LISTBOX:
		case IDC_PWRREMINDER_MSGSTRING_EDITBOX:
		case IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX:
		case IDC_PWRREMINDER_EVENT_SETTIME_SPIN:
			// Skip these items
			break;
		case IDC_PWRREMINDER_EVENT_REPEATSET_BTN:
			// Draw icon
			DrawRepeatSetButton();
			break;
		default:
			SetControlText(pWndChild, nID, pAppLang);
			break;
		}
	}

	// Setup data item list
	SetupDataItemList(pAppLang);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDataItemList
//	Description:	Initialize and setup language for Power Reminder data item list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::SetupDataItemList(LANGTABLE_PTR ptrLanguage)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_PWRREMINDER_ITEM_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Initialization
	if (m_pDataItemListTable == NULL) {
		m_pDataItemListTable = new CGridCtrl();
	}

	// Create table
	if (m_pDataItemListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pDataItemListTable->Create(rcListFrameWnd, this, IDC_PWRREMINDER_ITEM_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(DEF_COLOR_WHITE);
	pCell->SetTextClr(DEF_COLOR_BLACK);
	pCell->SetHeight(DEF_GRIDCTRL_ROWHEIGHT);

	// Define table columns format
	GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID----------------------Header title ID-------------Width(px)---Width unit----------Column style--------Align Center---
		{	PWRCOL_ID_INDEX,		GRIDCOLUMN_PWRREMINDER_INDEX,		26,		COLSIZE_PIXEL,		COLSTYLE_FIXED,			TRUE,	},
		{	PWRCOL_ID_STATE,		GRIDCOLUMN_PWRREMINDER_STATE,		55,		COLSIZE_PIXEL,		COLSTYLE_CHECKBOX,		TRUE,	},
		{	PWRCOL_ID_ITEMID,		GRIDCOLUMN_PWRREMINDER_ITEMID,		75,		COLSIZE_PIXEL,		COLSTYLE_NORMAL,		TRUE,	},
		{ 	PWRCOL_ID_MESSAGE,		GRIDCOLUMN_PWRREMINDER_MESSAGE,		44,		COLSIZE_PERCENT,	COLSTYLE_NORMAL,		FALSE,	},
		{ 	PWRCOL_ID_EVENTID,		GRIDCOLUMN_PWRREMINDER_EVENTID,		26,		COLSIZE_PERCENT,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	PWRCOL_ID_STYLE,		GRIDCOLUMN_PWRREMINDER_STYLE,		20,		COLSIZE_PERCENT,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	PWRCOL_ID_REPEAT,		GRIDCOLUMN_PWRREMINDER_REPEAT,		56,		COLSIZE_PIXEL,		COLSTYLE_CHECKBOX,		TRUE,	},
	//----------------------------------------------------------------------------------------------------------------------------------
	};

	// Table format and properties
	int nRowNum = (GetItemNum() + ROW_FIXED_NUM);
	int nColNum = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Backup format data
	m_nColNum = nColNum;
	if (m_pszFrameWndSize == NULL) {
		m_pszFrameWndSize = new CSize();
		m_pszFrameWndSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszFrameWndSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}
	if (m_apGrdColFormat == NULL) {
		m_apGrdColFormat = new GRIDCTRLCOLFORMAT[nColNum];
		for (int nIndex = 0; nIndex < nColNum; nIndex++) {
			// Copy and backup table format data
			m_apGrdColFormat[nIndex] = arrGrdColFormat[nIndex];
		}
	}

	// Setup table
	m_pDataItemListTable->SetColumnCount(nColNum);
	m_pDataItemListTable->SetFixedColumnCount(COL_FIXED_NUM);
	m_pDataItemListTable->SetRowCount(nRowNum);
	m_pDataItemListTable->SetFixedRowCount(COL_FIXED_NUM);
	m_pDataItemListTable->SetRowHeight(DEF_GRIDCTRL_ROWHEADER, DEF_GRIDCTRL_HEADERHEIGHT);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, nColNum, nRowNum, FALSE, ptrLanguage);

	// Display table
	m_pDataItemListTable->SetListMode(TRUE);
	m_pDataItemListTable->SetEditable(FALSE);
	m_pDataItemListTable->SetColumnResize(FALSE);
	m_pDataItemListTable->SetRowResize(FALSE);
	m_pDataItemListTable->EnableSelection(TRUE);
	m_pDataItemListTable->SetSingleRowSelection(TRUE);
	m_pDataItemListTable->SetSingleColSelection(FALSE);
	m_pDataItemListTable->SetFixedRowSelection(FALSE);
	m_pDataItemListTable->SetFixedColumnSelection(FALSE);
	m_pDataItemListTable->ShowWindow(SW_SHOW);
	m_pDataItemListTable->SetRedraw(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawDataTable
//	Description:	Draw data list table
//  Arguments:		szFrameWndSize	- Frame size
//					nColNum			- Number of table columns
//					nRowNum			- Number of table rows
//					bReadOnly		- Read-only mode
//					ptrLanguage		- Pointer to app language
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::DrawDataTable(CSize* pszFrameWndSize, int nColNum, int nRowNum, BOOL bReadOnly /* = FALSE */, LANGTABLE_PTR ptrLanguage /* = NULL */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL)
		return;

	// Check table format data validity
	if (pszFrameWndSize == NULL) return;
	if (m_apGrdColFormat == NULL) return;

	// Check row and column number validity
	if ((nColNum <= 0) || (nRowNum < ROW_FIXED_NUM))
		return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Load app language package
	if (ptrLanguage == NULL) {
		ptrLanguage = pApp->GetAppLanguage();
	}

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pDataItemListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;

	// Read-only mode --> Change cell color
	if (bReadOnly == TRUE) {
		pCell->SetBackClr(DEF_COLOR_BRIGHT_GRAY);
		pCell->SetTextClr(DEF_COLOR_DARK_GRAY);
	}
	else {
		pCell->SetBackClr(DEF_COLOR_WHITE);
		pCell->SetTextClr(DEF_COLOR_BLACK);
	}

	// Setup columns
	int nFrameHeight = pszFrameWndSize->cy;
	int nFrameWidth = pszFrameWndSize->cx;
	nFrameWidth -= DEF_OFFSET_LISTCTRLWIDTH;
	if (pApp->GetWindowsOSVersion() == DEF_WINVER_WIN10) {
		// Windows 10 list control offset
		nFrameWidth -= DEF_OFFSET_LISTCTRL_WIN10;
		nFrameHeight -= DEF_OFFSET_LISTCTRL_WIN10;
	}
	if ((DEF_GRIDCTRL_HEADERHEIGHT + ((nRowNum - 1) * DEF_GRIDCTRL_ROWHEIGHT)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + DEF_OFFSET_VSCRLBRWIDTH);
	}
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pDataItemListTable, DEF_GRIDCTRL_ROWHEADER, nCol);

		// Column header title
		CString strHdrTitle = DEF_STRING_EMPTY;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != DEF_INTEGER_NULL) {
			strHdrTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pDataItemListTable->SetItemText(DEF_GRIDCTRL_ROWHEADER, nCol, strHdrTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		int nColWidthUnit = m_apGrdColFormat[nCol].nWidthUnit;
		if (nColWidthUnit == COLSIZE_PIXEL) {					// Width unit: Pixel
			nFrameWidth -= nColWidth;
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidth);
		}
		else if (nColWidthUnit == COLSIZE_PERCENT) {			// Width unit: Percent
			int nColWidthPx = floor(float((nFrameWidth * nColWidth) / 100));
			m_pDataItemListTable->SetColumnWidth(nCol, nColWidthPx);
		}
	}

	// Setup rows
	int nColStyle = -1;
	int nCurMode = GetCurMode();
	UINT nItemState = DEF_INTEGER_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {
			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].nColStyle;
			nItemState = m_pDataItemListTable->GetItemState(nRow, nCol);
			nItemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				SetFixedCellStyle(m_pDataItemListTable, nRow, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pDataItemListTable->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* pCell = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!m_pDataItemListTable->SetItemState(nRow, nCol, nItemState))
					continue;

				// Get cell
				CGridCellBase* pCell = (CGridCellBase*)m_pDataItemListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetFormat(pCell->GetFormat() | DT_CENTER);
				}
			}
		}
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

void CPwrReminderDlg::SetupComboBox(UINT nComboID, LANGTABLE_PTR ptrLanguage)
{
	// Check combo validity
	if (m_pMsgStyleCombo == NULL) return;

	switch (nComboID)
	{
	case IDC_PWRREMINDER_MSGSTYLE_COMBO:
		m_pMsgStyleCombo->ResetContent();
		m_pMsgStyleCombo->AddString(GetLanguageString(ptrLanguage, COMBOBOX_MSGSTYLE_MESSAGEBOX));		// Message Box
		m_pMsgStyleCombo->AddString(GetLanguageString(ptrLanguage, COMBOBOX_MSGSTYLE_DIALOGBOX));		// Dialog Box
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SwitchMode
//	Description:	Switch between modes and re-update dialog item states
//  Arguments:		bRedraw - Redraw dialog items or not
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::SwitchMode(BOOL bRedraw /* = FALSE */)
{
	int nCurMode = GetCurMode();
	if (nCurMode == DEF_MODE_INIT) {
		// Unlock dialog items
		SetLockState(FALSE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(FALSE);

		// Refresh detail view
		RefreshDetailView(DEF_MODE_INIT);

		// Refresh dialog item states
		RefreshDlgItemState(TRUE);
	}
	else if (nCurMode == DEF_MODE_VIEW) {
		// Lock dialog items
		SetLockState(FALSE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Enable table
		DisableTable(FALSE);

		// Refresh detail view
		RefreshDetailView(DEF_MODE_VIEW);
		DisplayItemDetails(m_nCurSelIndex);

		// Refresh dialog item states
		RefreshDlgItemState(TRUE);
	}
	else if (nCurMode == DEF_MODE_ADD) {
		// Lock dialog items
		SetLockState(TRUE);

		// Enable/disable controls
		EnableControl(IDC_PWRREMINDER_ADD_BTN,		  TRUE);
		EnableControl(IDC_PWRREMINDER_EDIT_BTN,		  FALSE);
		EnableControl(IDC_PWRREMINDER_REMOVE_BTN,	  FALSE);
		EnableControl(IDC_PWRREMINDER_REMOVEALL_BTN,  FALSE);
		EnableControl(IDC_PWRREMINDER_CHECKALL_BTN,   FALSE);
		EnableControl(IDC_PWRREMINDER_UNCHECKALL_BTN, FALSE);
		EnableControl(IDC_PWRREMINDER_PREVIEW_BTN,	  FALSE);
		EnableControl(IDC_PWRREMINDER_APPLY_BTN,	  FALSE);

		// Change [Add] button title to [Save]
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(TRUE);

		// Refresh detail view
		RefreshDetailView(DEF_MODE_ADD);
		DisplayItemDetails(DEF_INTEGER_INVALID);
	}
	else if (nCurMode == DEF_MODE_UPDATE) {
		// Lock dialog items
		SetLockState(TRUE);

		// Enable/disable controls
		EnableControl(IDC_PWRREMINDER_ADD_BTN,		  FALSE);
		EnableControl(IDC_PWRREMINDER_EDIT_BTN,		  TRUE);
		EnableControl(IDC_PWRREMINDER_REMOVE_BTN,	  FALSE);
		EnableControl(IDC_PWRREMINDER_REMOVEALL_BTN,  FALSE);
		EnableControl(IDC_PWRREMINDER_CHECKALL_BTN,	  FALSE);
		EnableControl(IDC_PWRREMINDER_UNCHECKALL_BTN, FALSE);
		EnableControl(IDC_PWRREMINDER_PREVIEW_BTN,	  FALSE);
		EnableControl(IDC_PWRREMINDER_APPLY_BTN,	  FALSE);

		// Change [Edit] button title to [Save]
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN, BTN_PWRRMDDLG_SAVECHANGES);

		// Disable table
		DisableTable(TRUE);

		// Refresh detail view
		RefreshDetailView(DEF_MODE_UPDATE);
	}
	else if (nCurMode == DEF_MODE_DISABLE) {
		// Lock dialog items
		SetLockState(TRUE);

		// Restore [Add/Edit] buttons caption
		UpdateItemText(IDC_PWRREMINDER_ADD_BTN);
		UpdateItemText(IDC_PWRREMINDER_EDIT_BTN);

		// Disable table
		DisableTable(TRUE);

		// Refresh dialog item states
		RefreshDlgItemState(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupDialogItemState
//	Description:	Setup state and properties for dialog items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::SetupDialogItemState()
{
	// Initialize dialog items
	if (m_pMsgStringEdit == NULL) {
		m_pMsgStringEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_MSGSTRING_EDITBOX);
		if (m_pMsgStringEdit == NULL) {
			TRCLOG("Error: Message content edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeRad == NULL) {
		m_pEvtSetTimeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_RADBTN);
		if (m_pEvtSetTimeRad == NULL) {
			TRCLOG("Error: Radio button not found (EventSetTimeRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSetTimeSpin == NULL) {
		m_pEvtSetTimeSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_SPIN);
		if (m_pEvtSetTimeSpin == NULL) {
			TRCLOG("Error: Time spin control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtRepeatSetBtn == NULL) {
		m_pEvtRepeatSetBtn = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (m_pEvtRepeatSetBtn == NULL) {
			TRCLOG("Error: RepeatSet button not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtAppStartupRad == NULL) {
		m_pEvtAppStartupRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_APPSTARTUP_RADBTN);
		if (m_pEvtAppStartupRad == NULL) {
			TRCLOG("Error: Radio button not found (EventAppStartupRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtSysWakeupRad == NULL) {
		m_pEvtSysWakeupRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_SYSWAKEUP_RADBTN);
		if (m_pEvtSysWakeupRad == NULL) {
			TRCLOG("Error: Radio button not found (EventSysWakeupRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtBfrPwrActionRad == NULL) {
		m_pEvtBfrPwrActionRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_BFRPWRACTION_RADBTN);
		if (m_pEvtBfrPwrActionRad == NULL) {
			TRCLOG("Error: Radio button not found (EventBfrPwrActionRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtPwrActionWakeRad == NULL) {
		m_pEvtPwrActionWakeRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_PWRACTIONWAKE_RADBTN);
		if (m_pEvtPwrActionWakeRad == NULL) {
			TRCLOG("Error: Radio button not found (EventPwrActionWakeRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pEvtAtAppExitRad == NULL) {
		m_pEvtAtAppExitRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_ATAPPEXIT_RADBTN);
		if (m_pEvtAtAppExitRad == NULL) {
			TRCLOG("Error: Radio button not found (EventAtAppExitRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleMsgBoxRad == NULL) {
		m_pStyleMsgBoxRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_MSGBOX_RADBTN);
		if (m_pStyleMsgBoxRad == NULL) {
			TRCLOG("Error: Radio button not found (StyleMsgBoxRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}
	if (m_pStyleDialogBoxRad == NULL) {
		m_pStyleDialogBoxRad = (CButton*)GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_DIALOG_RADBTN);
		if (m_pStyleDialogBoxRad == NULL) {
			TRCLOG("Error: Radio button not found (StyleDlgBoxRad)");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Setup properties
	if (m_pEvtSetTimeSpin != NULL) {
		if (m_pEvtSetTimeEdit == NULL) return;

		// Set buddy: Time edit control
		m_pEvtSetTimeSpin->SetBuddy(m_pEvtSetTimeEdit);
		m_pEvtSetTimeSpin->SetRange(DEF_SPINCTRL_TIMEMINPOS, DEF_SPINCTRL_TIMEMAXPOS);
		m_pEvtSetTimeSpin->SetPos(0);
	}

	// Setup time editbox
	SYSTEMTIME stTimeTemp;
	SpinPos2Time(stTimeTemp, 0);
	UpdateTimeSetting(stTimeTemp, FALSE);

	// Initialize counter display
	UpdateMsgCounter(0);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateDataItemList
//	Description:	Update Power Reminder data item list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::UpdateDataItemList()
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// If there's no item, do nothing
	int nItemNum = GetItemNum();
	if (nItemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();
	
	// Print items
	CString strTemp;
	int nTemp = -1;
	int nRowIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {

		// Get row index
		nRowIndex = nIndex + ROW_FIXED_NUM;

		// Get item
		PWRREMINDERITEM pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);

		// Item index
		strTemp.Format(_T("%d"), nRowIndex);
		m_pDataItemListTable->SetItemText(nRowIndex, PWRCOL_ID_INDEX, strTemp);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, PWRCOL_ID_STATE);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(pwrItem.bEnable);
		}

		// ItemID
		strTemp.Format(_T("%d"), pwrItem.nItemID);
		m_pDataItemListTable->SetItemText(nRowIndex, PWRCOL_ID_ITEMID, strTemp);

		// Message content
		strTemp = pwrItem.strMessage;
		m_pDataItemListTable->SetItemText(nRowIndex, PWRCOL_ID_MESSAGE, strTemp);

		// EventID
		nTemp = GetPairedID(idplPwrReminderEvt, pwrItem.nEventID);
		strTemp = GetLanguageString(ptrLanguage, nTemp);
		if (pwrItem.nEventID == PREVT_AT_SETTIME) {
			// Format time string
			CString strFormat = strTemp;
			strTemp = FormatDispTime(ptrLanguage, strFormat, pwrItem.stTime);
		}
		m_pDataItemListTable->SetItemText(nRowIndex, PWRCOL_ID_EVENTID, strTemp);

		// Message style
		nTemp = GetPairedID(idplPwrReminderStyle, pwrItem.dwStyle);
		strTemp = GetLanguageString(ptrLanguage, nTemp);
		m_pDataItemListTable->SetItemText(nRowIndex, PWRCOL_ID_STYLE, strTemp);

		// Repeat
		pCellCheck = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, PWRCOL_ID_REPEAT);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(pwrItem.IsRepeatEnable());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisableTable
//	Description:	Disable mouse click events for table
//  Arguments:		bDisable - Disable/enable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::DisableTable(BOOL bDisable)
{
	// Redraw read-only style
	RedrawDataTable(bDisable);

	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Disable/enable mouse events
	m_pDataItemListTable->DisableMouseClick(bDisable);
	m_pDataItemListTable->DisableMouseMove(bDisable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RedrawDataTable
//	Description:	Update and redraw data table
//  Arguments:		BOOL bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::RedrawDataTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pDataItemListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetItemNum() + ROW_FIXED_NUM);
	m_pDataItemListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawDataTable(m_pszFrameWndSize, m_nColNum, nCurRowNum, bReadOnly);
	
	// Update table data
	UpdateDataItemList();

	// Trigger redrawing table
	m_pDataItemListTable->RedrawWindow();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisplayItemDetails
//	Description:	Display details of an item at specified index
//  Arguments:		nIndex - Index of item to display
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::DisplayItemDetails(int nIndex)
{
	// Check index validity
	if ((nIndex < DEF_INTEGER_INVALID) || (nIndex > GetItemNum()))
		return;

	// Get item at index
	PWRREMINDERITEM pwrItem;
	if (nIndex != DEF_INTEGER_INVALID) {
		pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
	}

	// Init default data for mode add
	if (GetCurMode() == DEF_MODE_ADD) {
		pwrItem.strMessage = DEF_STRING_EMPTY;
		pwrItem.nEventID = PREVT_AT_SETTIME;
		pwrItem.stTime = GetCurSysTime();
		pwrItem.dwStyle = PRSTYLE_MSGBOX;
		pwrItem.rpsRepeatSet = RMDREPEATSET();
	}

	// If item is empty
	if (pwrItem.IsEmpty()) {
		// TODO: Disable all detail item controls
		RefreshDetailView(DEF_MODE_INIT);
	}
	else {
		// TODO: Update current displaying item index
		RefreshDetailView(GetCurMode());
		m_nCurDispIndex = nIndex;
	}

	// Initialize RepeatSet dialog if not available
	if (m_pRepeatSetDlg == NULL) {
		m_pRepeatSetDlg = new CRmdRepeatSetDlg;
		m_pRepeatSetDlg->Create(IDD_RMDREPEATSET_DLG);
		m_pRepeatSetDlg->RemoveDialogStyle(DS_MODALFRAME | WS_CAPTION);
		m_pRepeatSetDlg->SetParentWnd(this);
	}

	// Display item details
	UpdateItemData(pwrItem, FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDlgItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		bRecheckState - Recheck all items state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::RefreshDlgItemState(BOOL bRecheckState)
{
	CWnd* pBtn = NULL;

	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Check if any item is selected or not
	BOOL bIsSelected = ((m_nCurSelIndex >= 0) && (m_nCurSelIndex < GetItemNum()));

	// Check if number of item has reached the limit
	BOOL bIsMaxNum = (GetItemNum() >= DEF_PWRREMINDER_MAX_ITEMNUM);

	// Check if data is all empty or not
	BOOL bIsAllEmpty = m_pwrReminderDataTemp.IsAllEmpty();

	// Get app language package
	LANGTABLE_PTR pAppLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Disable [Add] button if item number has reached the limit
	pBtn = GetDlgItem(IDC_PWRREMINDER_ADD_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsMaxNum);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_PWRREMINDER_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	pBtn = GetDlgItem(IDC_PWRREMINDER_REMOVE_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);

		// Enable [Remove] button if any item is selected
		pBtn->EnableWindow(bIsSelected);
	}
	pBtn = GetDlgItem(IDC_PWRREMINDER_REMOVEALL_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(!bIsAllEmpty);
	}

	// Enable [Edit] and [Preview] button if any item is selected
	pBtn = GetDlgItem(IDC_PWRREMINDER_EDIT_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
		pBtn->SetWindowText(GetLanguageString(pAppLang, IDC_PWRREMINDER_EDIT_BTN));
	}
	pBtn = GetDlgItem(IDC_PWRREMINDER_PREVIEW_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(bIsSelected);
	}

	// Check if data is changed or not
	m_bChangeFlag = CheckDataChangeState();

	// Enable [Apply] button if data is changed
	pBtn = GetDlgItem(IDC_PWRREMINDER_APPLY_BTN);
	if (pBtn != NULL) {
		pBtn->EnableWindow(m_bChangeFlag);
	}

	// Update [Check/Uncheck All] button state
	UpdateCheckAllBtnState(bRecheckState);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateCheckAllBtnState
//	Description:	Refresh and update state for [Check/Uncheck All] button
//  Arguments:		bRecheck - Recheck all items enable state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::UpdateCheckAllBtnState(BOOL bRecheck /* = FALSE */)
{
	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_PWRREMINDER_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_PWRREMINDER_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int nItemNum = GetItemNum();
	if (nItemNum == 0) {
		// Disable both [Check/Uncheeck All] buttons
		pCheckAllBtn->EnableWindow(FALSE);
		pUncheckAllBtn->EnableWindow(FALSE);
		return;
	}

	// Recheck all items state
	if (bRecheck == TRUE) {
		m_nCheckCount = 0; // Reset counter
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			PWRREMINDERITEM& pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);
			if (pwrTemp.bEnable == TRUE) {
				m_nCheckCount++;
			}
		}
	}

	// Update button state
	if (m_nCheckCount == 0) {
		// Enable [Check All] button
		pCheckAllBtn->EnableWindow(TRUE);
		// Disable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(FALSE);
	}
	else if (m_nCheckCount == nItemNum) {
		// Disable [Check All] button
		pCheckAllBtn->EnableWindow(FALSE);
		// Enable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(TRUE);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(TRUE);
		pUncheckAllBtn->EnableWindow(TRUE);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDetailView
//	Description:	Refresh and update state for detail view
//  Arguments:		nMode - Detail view mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::RefreshDetailView(int nMode)
{
	// Set state and init value by mode
	BOOL bEnable = TRUE;
	if ((nMode == DEF_MODE_INIT) || (nMode == DEF_MODE_VIEW)) {
		bEnable = FALSE;
	}
	else if ((nMode == DEF_MODE_ADD) || (nMode == DEF_MODE_UPDATE)) {
		bEnable = TRUE;
	}
	else if (nMode == DEF_MODE_DISABLE) {
		bEnable = FALSE;
	}

	/***************************************************************/
	/*															   */
	/*				Update detail view item states				   */
	/*															   */
	/***************************************************************/

	CWnd* pWnd = NULL;

	// Message content
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pMsgStringEdit != NULL) {
		m_pMsgStringEdit->EnableWindow(bEnable);
	}
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	// Event
	pWnd = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pEvtSetTimeRad != NULL) {
		m_pEvtSetTimeRad->EnableWindow(bEnable);
		BOOL bEnableSetTime = (bEnable & m_pEvtSetTimeRad->GetCheck());
		if (m_pEvtSetTimeEdit != NULL) {
			m_pEvtSetTimeEdit->EnableWindow(bEnableSetTime);
			UpdateTimeSetting(m_stDispTimeBak, FALSE);
		}
		if (m_pEvtSetTimeSpin != NULL) {
			m_pEvtSetTimeSpin->EnableWindow(bEnableSetTime);
		}
		if (m_pEvtRepeatSetBtn != NULL) {
			m_pEvtRepeatSetBtn->EnableWindow(bEnableSetTime);
		}
	}
	if (m_pEvtAppStartupRad != NULL) {
		m_pEvtAppStartupRad->EnableWindow(bEnable);
	}
	if (m_pEvtSysWakeupRad != NULL) {
		m_pEvtSysWakeupRad->EnableWindow(bEnable);
	}
	if (m_pEvtBfrPwrActionRad != NULL) {
		m_pEvtBfrPwrActionRad->EnableWindow(bEnable);
	}
	if (m_pEvtPwrActionWakeRad != NULL) {
		m_pEvtPwrActionWakeRad->EnableWindow(bEnable);
	}
	if (m_pEvtAtAppExitRad != NULL) {
		m_pEvtAtAppExitRad->EnableWindow(bEnable);
	}
	// Message style
	pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
	if (pWnd != NULL) {
		pWnd->EnableWindow(bEnable);
	}
	if (m_pStyleMsgBoxRad != NULL) {
		m_pStyleMsgBoxRad->EnableWindow(bEnable);
	}
	if (m_pStyleDialogBoxRad != NULL) {
		m_pStyleDialogBoxRad->EnableWindow(bEnable);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateMsgCounter
//	Description:	Update message content length counter
//  Arguments:		nCount - Character number counter
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::UpdateMsgCounter(int nCount)
{
	// Get dialog item
	CWnd* pCounter = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
	if (pCounter == NULL) return;

	// Check counter value validity
	if ((nCount < 0) || (nCount > DEF_STRING_MAXLENGTH)) return;

	// Display counter
	CString strCountFormat;
	strCountFormat.Format(_T("%d/%d"), nCount, DEF_STRING_MAXLENGTH);
	pCounter->SetWindowText(strCountFormat);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateTimeSetting
//	Description:	Update time value from/to time edit control
//  Arguments:		stTime  - Time data
//					bUpdate - Update or not (YES/TRUE by default)
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::UpdateTimeSetting(SYSTEMTIME& stTime, BOOL bUpdate /* = TRUE */)
{
	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check time editbox validity
	if (m_pEvtSetTimeEdit == NULL) {
		m_pEvtSetTimeEdit = (CEdit*)GetDlgItem(IDC_PWRREMINDER_EVENT_SETTIME_EDITBOX);
		if (m_pEvtSetTimeEdit == NULL) {
			TRCLOG("Error: Time edit control not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	if (bUpdate == TRUE) {
		// Get value from time editbox
		CString strTimeFormat;
		m_pEvtSetTimeEdit->GetWindowText(strTimeFormat);

		// Get hour value
		WORD wHour = (WORD)_tstoi(strTimeFormat.Left(2));
		CString strMiddayFlag = strTimeFormat.Right(2);
		if (strMiddayFlag == GetLanguageString(pLang, FORMAT_TIME_BEFOREMIDDAY)) {
			// Before midday
			stTime.wHour = wHour;
		}
		else if ((strMiddayFlag == GetLanguageString(pLang, FORMAT_TIME_AFTERMIDDAY)) && wHour < 12) {
			// After midday
			stTime.wHour = wHour + 12;
		}
		else {
			// Keep the value
			stTime.wHour = wHour;
		}

		// Get minute value
		stTime.wMinute = (WORD)_tstoi(strTimeFormat.Mid(3, 2));
	}
	else {
		// Set value for time editbox
		CString strTimeFormat;
		strTimeFormat = FormatDispTime(pLang, IDS_FORMAT_SHORTTIME, stTime);
		m_pEvtSetTimeEdit->SetWindowText(strTimeFormat);

		// Backup current displaying time value
		m_stDispTimeBak = stTime;

		// Update time spin position
		if (m_pEvtSetTimeSpin != NULL) {
			int nSpinPos = 0;
			Time2SpinPos(stTime, nSpinPos);
			m_pEvtSetTimeSpin->SetPos(nSpinPos);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadPwrReminderData
//	Description:	Load Power Reminder data
//  Arguments:		None
//  Return value:	BOOL - Result of loading process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPwrReminderDlg::LoadPwrReminderData()
{
	// Get app Reminder data pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	PPWRREMINDERDATA ppwrData = pApp->GetAppPwrReminderData();
	if (ppwrData == NULL)
		return FALSE;

	// Copy data
	m_pwrReminderData.Copy(*ppwrData);
	m_pwrReminderDataTemp.Copy(m_pwrReminderData);

	// Reset change flag
	m_bChangeFlag = FALSE;

	// Validate data and auto-correction
	for (int nIndex = 0; nIndex < GetItemNum(); nIndex++) {
		PWRREMINDERITEM& pwrItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
		if (!Validate(pwrItem, TRUE, TRUE)) {
			// Update temp data
			m_pwrReminderDataTemp.Update(pwrItem);
			m_bChangeFlag = TRUE;	// Update change flag
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SavePwrReminderData
//	Description:	Save Power Reminder data
//  Arguments:		None
//  Return value:	BOOL - Result of saving process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPwrReminderDlg::SavePwrReminderData()
{
	// Copy data and adjust validity
	m_pwrReminderData.Copy(m_pwrReminderDataTemp);
	m_pwrReminderData.Adjust();

	// Save app Power Reminder data
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	VERIFY(pApp != NULL);
	if (pApp == NULL) return FALSE;
	pApp->SetAppPwrReminderData(&m_pwrReminderData);
	pApp->SaveRegistryAppData(APPDATA_PWRREMINDER);

	// Notify main dialog to re-update Power Reminder data
	CPowerPlusDlg* pMainDlg = (CPowerPlusDlg*)(pApp->GetMainWnd());
	VERIFY(pMainDlg != NULL);
	if (pMainDlg == NULL) return FALSE;
	pMainDlg->PostMessage(SM_APP_UPDATE_PWRREMINDERDATA, NULL, NULL);

	// Reset change flag
	m_bChangeFlag = FALSE;
	
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

BOOL CPwrReminderDlg::CheckDataChangeState()
{
	BOOL bChangeFlag = FALSE;

	// Update enable and repeat states of each item
	int nRowIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int nItemRowNum = (m_pDataItemListTable->GetRowCount() - ROW_FIXED_NUM);
	for (int nIndex = 0; nIndex < nItemRowNum; nIndex++) {
		// Get row index
		nRowIndex = (nIndex + ROW_FIXED_NUM);

		// Get checkbox cells
		pCellCheckEnable = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, PWRCOL_ID_STATE);
		pCellCheckRepeat = (CGridCellCheck*)m_pDataItemListTable->GetCell(nRowIndex, PWRCOL_ID_REPEAT);
		if ((pCellCheckEnable == NULL) || (pCellCheckRepeat == NULL)) continue;
		
		// Get checked states
		BOOL bEnable = pCellCheckEnable->GetCheck();
		BOOL bRepeat = pCellCheckRepeat->GetCheck();
		
		// Update item enable and repeat states
		PWRREMINDERITEM& pwrTempItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
		pwrTempItem.bEnable = bEnable;
		pwrTempItem.rpsRepeatSet.bRepeat = bRepeat;
	}

	// Check if number of items changed
	int nItemNum = GetItemNum();
	bChangeFlag |= (nItemNum != m_pwrReminderData.GetItemNum());
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each item's data changed
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		// Get current item and temp item
		PWRREMINDERITEM pwrCurItem = m_pwrReminderData.GetItemAt(nIndex);
		PWRREMINDERITEM pwrTempItem = m_pwrReminderDataTemp.GetItemAt(nIndex);
 
		// Data comparison
		bChangeFlag |= (pwrTempItem.bEnable != pwrCurItem.bEnable);
		bChangeFlag |= (pwrTempItem.nItemID != pwrCurItem.nItemID);
		bChangeFlag |= (pwrTempItem.strMessage != pwrCurItem.strMessage);
		bChangeFlag |= (pwrTempItem.nEventID != pwrCurItem.nEventID);
		bChangeFlag |= (pwrTempItem.stTime.wHour != pwrCurItem.stTime.wHour);
		bChangeFlag |= (pwrTempItem.stTime.wMinute != pwrCurItem.stTime.wMinute);
		bChangeFlag |= (pwrTempItem.dwStyle != pwrCurItem.dwStyle);
		bChangeFlag |= (pwrTempItem.rpsRepeatSet.Compare(pwrCurItem.rpsRepeatSet) != TRUE);

		// Stop on the first different item encountered
		if (bChangeFlag == TRUE) break;
	}
	
	return bChangeFlag;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Add
//	Description:	Add current settings to reminder data list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::Add()
{
	// Update data
	UpdateData(TRUE);

	// Create temp Reminder item
	PWRREMINDERITEM pwrTemp;
	pwrTemp.nItemID = m_pwrReminderDataTemp.GetNextID();

	// Update data
	UpdateItemData(pwrTemp, TRUE);

	// Check data validity
	BOOL bValid = Validate(pwrTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.Update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Edit
//	Description:	Edit a reminder item by index
//  Arguments:		nIndex - Index of item to edit
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::Edit(int nIndex)
{
	// Update data
	UpdateData(TRUE);

	// Check index validity
	if ((nIndex < 0) || (nIndex > GetItemNum()))
		return;

	// Get item at given index
	PWRREMINDERITEM pwrTemp;
	pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);

	// Update data
	UpdateItemData(pwrTemp, TRUE);

	// Check data validity
	BOOL bValid = Validate(pwrTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to Power Reminder data item list
	m_pwrReminderDataTemp.Update(pwrTemp);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Remove
//	Description:	Remove a reminder item by index
//  Arguments:		nIndex - Index of item to remove
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::Remove(int nIndex)
{
	// Remove item at index
	m_pwrReminderDataTemp.Delete(nIndex);

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all reminder items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::RemoveAll()
{
	// Remove all items
	m_pwrReminderDataTemp.DeleteAll();

	// Update table
	RedrawDataTable();

	// Refresh button state
	RefreshDlgItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetAllItemState
//	Description:	Check/uncheck all Power Reminder items
//  Arguments:		bState - Item state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::SetAllItemState(BOOL bState)
{
	// Check/uncheck all --> Update all items enable state
	int nItemNum = GetItemNum();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		PWRREMINDERITEM& pwrTemp = m_pwrReminderDataTemp.GetItemAt(nIndex);
		if (pwrTemp.bEnable != bState) {
			pwrTemp.bEnable = bState;
		}
	}

	// Update number of checked items
	m_nCheckCount = (bState == FALSE) ? 0 : nItemNum;
	
	// Update data item list
	UpdateDataItemList();

	// Refresh button state
	RefreshDlgItemState(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	PreviewItem
//	Description:	Preview Power Reminder item
//  Arguments:		nIndex - Item index
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::PreviewItem(int nIndex)
{
	// Check index validity
	if ((nIndex < 0) || (nIndex >= GetItemNum()))
		return;

	// Get item
	PWRREMINDERITEM pwrDispItem;
	pwrDispItem = m_pwrReminderDataTemp.GetItemAt(nIndex);

	// Check message content validity
	CString strMsgContent = pwrDispItem.strMessage;
	if ((strMsgContent.IsEmpty()) ||
		(strMsgContent == DEF_STRING_NULL)) {
		// Invalid message content
		return;
	}

	// Style: MessageBox
	if (pwrDispItem.dwStyle == PRSTYLE_MSGBOX) {
		// Display message box
		UINT nCaptionID = IDC_PWRREMINDER_PREVIEW_BTN;
		DWORD dwMsgStyle = MB_OK | MB_ICONINFORMATION;
		DisplayMessageBox(strMsgContent, nCaptionID, dwMsgStyle);
	}
	// Style: Dialog
	else if (pwrDispItem.dwStyle == PRSTYLE_DIALOG) {
		// Destroy preview reminder message dialog if is opening
		if (m_pRmdPreviewMsgDlg != NULL) {
			// Destroy dialog
			if (::IsWindow(m_pRmdPreviewMsgDlg->GetSafeHwnd())) {
				m_pRmdPreviewMsgDlg->DestroyWindow();
			}
			delete m_pRmdPreviewMsgDlg;
			m_pRmdPreviewMsgDlg = NULL;
		}

		// Init reminder message dialog
		if (m_pRmdPreviewMsgDlg == NULL) {
			m_pRmdPreviewMsgDlg = new CReminderMsgDlg();
			if (m_pRmdPreviewMsgDlg == NULL) return;

			// Message color
			COLORREF clrMsgBkgrd = GetReminderMsgBkgrdColor();
			COLORREF clrMsgText = GetReminderMsgTextColor();
		
			// Message font info
			CString strFontName;
			BOOL bGetFontName = GetReminderMsgFontName(strFontName);
			int nFontSize = GetReminderMsgFontSize();

			// Message icon info
			int nIconID = GetReminderMsgIconID();
			int nIconSize = GetReminderMsgIconSize();
			BYTE byIconPos = GetReminderMsgIconPosition();

			// Default timeout for previewing
			int nDefTimeout = DEF_PWRREMINDER_PREVIEW_TIMEOUT;

			// Set properties
			m_pRmdPreviewMsgDlg->SetLangDlgTitle(IDC_PWRREMINDER_PREVIEW_BTN);
			m_pRmdPreviewMsgDlg->SetDispMessage(strMsgContent);
			m_pRmdPreviewMsgDlg->SetBkgrdColor(clrMsgBkgrd);
			m_pRmdPreviewMsgDlg->SetTextColor(clrMsgText);
			m_pRmdPreviewMsgDlg->SetMsgFont(strFontName, nFontSize);
			m_pRmdPreviewMsgDlg->SetMsgIcon(nIconID, nIconSize);
			m_pRmdPreviewMsgDlg->SetMsgIconPosition(byIconPos);
			m_pRmdPreviewMsgDlg->SetAutoCloseInterval(nDefTimeout);

			// Set notify state flags
			m_pRmdPreviewMsgDlg->SetTopMost(FALSE);
			m_pRmdPreviewMsgDlg->SetInitSound(TRUE);

			// Display message
			m_pRmdPreviewMsgDlg->DoModal();
			
			delete m_pRmdPreviewMsgDlg;
			m_pRmdPreviewMsgDlg = NULL;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateItemData
//	Description:	Update reminder data from/to dialog controls
//  Arguments:		pwrItem - Power Reminder item
//					bUpdate - Update data flag
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::UpdateItemData(PWRREMINDERITEM& pwrItem, BOOL bUpdate)
{
	if (bUpdate == TRUE) {

		/***************************************************************/
		/*															   */
		/*				Update data from dialog controls			   */
		/*															   */
		/***************************************************************/

		/*-----------------------Message content-----------------------*/

		CString strTemp = DEF_STRING_EMPTY;
		if (m_pMsgStringEdit != NULL) {
			m_pMsgStringEdit->GetWindowText(strTemp);
			pwrItem.strMessage = strTemp;
		}

		/*----------------------------Event----------------------------*/

		BOOL bTemp = FALSE;

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			bTemp = m_pEvtSetTimeRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_SETTIME;
				if (m_pEvtSetTimeEdit != NULL) {
					UpdateTimeSetting(pwrItem.stTime, TRUE);
				}
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->UpdateDialogData(pwrItem, TRUE);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			bTemp = m_pEvtAppStartupRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_APPSTARTUP;
			}
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			bTemp = m_pEvtSysWakeupRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_SYSWAKEUP;
			}
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			bTemp = m_pEvtBfrPwrActionRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_BFRPWRACTION;
			}
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			bTemp = m_pEvtPwrActionWakeRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_PWRACTIONWAKE;
			}
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			bTemp = m_pEvtAtAppExitRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.nEventID = PREVT_AT_APPEXIT;
			}
		}

		/*------------------------Message style------------------------*/

		// Style: MessageBox
		if (m_pStyleMsgBoxRad != NULL) {
			bTemp = m_pStyleMsgBoxRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.dwStyle = PRSTYLE_MSGBOX;
			}
		}
		// Style: Dialog Box
		if (m_pStyleDialogBoxRad != NULL) {
			bTemp = m_pStyleDialogBoxRad->GetCheck();
			if (bTemp == TRUE) {
				pwrItem.dwStyle = PRSTYLE_DIALOG;
			}
		}

		/*-------------------------------------------------------------*/
	}
	else {

		/***************************************************************/
		/*															   */
		/*				  Bind data to dialog controls				   */
		/*															   */
		/***************************************************************/

		/*-----------------Set state and init value by mode------------*/

		BOOL bEnable = TRUE;
		int nMode = GetCurMode();
		if ((nMode == DEF_MODE_INIT) || (nMode == DEF_MODE_VIEW)) {
			// Disable items
			bEnable = FALSE;
		}
		else if ((nMode == DEF_MODE_ADD) || (nMode == DEF_MODE_UPDATE)) {
			// Enable items
			bEnable = TRUE;
		}
		else if (nMode == DEF_MODE_DISABLE) {
			// Disable items
			bEnable = FALSE;
		}

		/*----------------------Get item details-----------------------*/

		CString strMessage = pwrItem.strMessage;
		SYSTEMTIME stTime = pwrItem.stTime;
		UINT nEventID = pwrItem.nEventID;
		DWORD dwStyle = pwrItem.dwStyle;

		/*-----------------------Message content-----------------------*/

		CWnd* pWnd = NULL;
		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}
		if (m_pMsgStringEdit != NULL) {
			m_pMsgStringEdit->EnableWindow(bEnable);
			m_pMsgStringEdit->SetWindowText(strMessage);
		}
		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTRING_COUNTER);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
			UpdateMsgCounter(strMessage.GetLength());
		}

		/*----------------------------Event----------------------------*/

		BOOL bTemp = FALSE;
		int nTemp = DEF_INTEGER_INVALID;
		pWnd = GetDlgItem(IDC_PWRREMINDER_EVENT_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}

		// Event: At set time
		if (m_pEvtSetTimeRad != NULL) {
			m_pEvtSetTimeRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_SETTIME);
			m_pEvtSetTimeRad->SetCheck(bTemp);

			// Set time edit and spin value
			bTemp &= bEnable;
			if (m_pEvtSetTimeEdit != NULL) {
				m_pEvtSetTimeEdit->EnableWindow(bTemp);
				UpdateTimeSetting(stTime, FALSE);
			}
			if (m_pEvtSetTimeSpin != NULL) {
				m_pEvtSetTimeSpin->EnableWindow(bTemp);
			}
		}
		// Update data for RepeatSet dialog
		if (m_pRepeatSetDlg != NULL) {
			m_pRepeatSetDlg->UpdateDialogData(pwrItem, FALSE);
		}
		// Event: At app startup
		if (m_pEvtAppStartupRad != NULL) {
			m_pEvtAppStartupRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_APPSTARTUP);
			m_pEvtAppStartupRad->SetCheck(bTemp);
		}
		// Event: At system wake
		if (m_pEvtSysWakeupRad != NULL) {
			m_pEvtSysWakeupRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_SYSWAKEUP);
			m_pEvtSysWakeupRad->SetCheck(bTemp);
		}
		// Event: Before power action
		if (m_pEvtBfrPwrActionRad != NULL) {
			m_pEvtBfrPwrActionRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_BFRPWRACTION);
			m_pEvtBfrPwrActionRad->SetCheck(bTemp);
		}
		// Event: Wake after action
		if (m_pEvtPwrActionWakeRad != NULL) {
			m_pEvtPwrActionWakeRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_PWRACTIONWAKE);
			m_pEvtPwrActionWakeRad->SetCheck(bTemp);
		}
		// Event: Before app exit
		if (m_pEvtAtAppExitRad != NULL) {
			m_pEvtAtAppExitRad->EnableWindow(bEnable);
			bTemp = (nEventID == PREVT_AT_APPEXIT);
			m_pEvtAtAppExitRad->SetCheck(bTemp);
		}

		/*------------------------Message style------------------------*/

		pWnd = GetDlgItem(IDC_PWRREMINDER_MSGSTYLE_TITLE);
		if (pWnd != NULL) {
			pWnd->EnableWindow(bEnable);
		}
		if (m_pStyleMsgBoxRad != NULL) {
			m_pStyleMsgBoxRad->EnableWindow(bEnable);
			bTemp = (dwStyle == PRSTYLE_MSGBOX);
			m_pStyleMsgBoxRad->SetCheck(bTemp);
		}
		if (m_pStyleDialogBoxRad != NULL) {
			m_pStyleDialogBoxRad->EnableWindow(bEnable);
			bTemp = (dwStyle == PRSTYLE_DIALOG);
			m_pStyleDialogBoxRad->SetCheck(bTemp);
		}

		/*-------------------------------------------------------------*/
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Validate
//	Description:	Check Power Reminder item validity
//  Arguments:		pwrItem		 - Item to validate
//					bShowMsg	 - Show validation message box or not
//					bAutoCorrect - Invalid value auto correction (ON/OFF)
//  Return value:	BOOL - Result of validation process
//
//////////////////////////////////////////////////////////////////////////

BOOL CPwrReminderDlg::Validate(PWRREMINDERITEM& pwrItem, BOOL bShowMsg /* = FALSE */, BOOL bAutoCorrect /* = FALSE */)
{
	BOOL bResult = TRUE;

	int nMsgStringID;
	CStringArray arrMsgString;
	arrMsgString.RemoveAll();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check item ID
	if ((pwrItem.nItemID < DEF_PWRREMINDER_MIN_ITEMID) || (pwrItem.nItemID > DEF_PWRREMINDER_MAX_ITEMID)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_ITEMID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Get next item ID
			pwrItem.nItemID = m_pwrReminderDataTemp.GetNextID();
		}
	}

	// Check message content
	if (pwrItem.strMessage.IsEmpty()) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_EMPTY;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			pwrItem.strMessage = GetLanguageString(pLang, PWRRMD_MSGCONTENT_SAMPLE);
			if (_tcscmp(pwrItem.strMessage, DEF_STRING_NULL)) {
				// Re-format sample message
				CString strFormat = pwrItem.strMessage;
				pwrItem.strMessage.Format(strFormat, pwrItem.nItemID);
			}
		}
	}
	else if (pwrItem.strMessage.GetLength() > DEF_STRING_MAXLENGTH) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_MESSAGE_OUTOFLIMIT;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Only get character numbers in range
			CString strTemp = pwrItem.strMessage.Left(DEF_STRING_MAXLENGTH);
			pwrItem.strMessage = strTemp;
		}
	}

	// Check event ID
	if ((pwrItem.nEventID < PREVT_AT_SETTIME) || (pwrItem.nEventID > PREVT_AT_APPEXIT)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_EVENTID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default event ID
			pwrItem.nEventID = PREVT_AT_SETTIME;
		}
	}

	// Check snooze interval data
	if ((pwrItem.rpsRepeatSet.nSnoozeInterval < DEF_PWRREMINDER_MIN_SNOOZE) || (pwrItem.rpsRepeatSet.nSnoozeInterval > DEF_PWRREMINDER_MAX_SNOOZE)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_SNOOZEINTERVAL;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default snooze interval
			pwrItem.rpsRepeatSet.nSnoozeInterval = DEF_PWRREMINDER_DEFAULT_SNOOZE;
		}
	}

	// Check repeat set data
	if ((pwrItem.IsRepeatEnable() == TRUE) && (pwrItem.rpsRepeatSet.byRepeatDays == NULL)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_ACTIVEDAYS;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default data
			pwrItem.rpsRepeatSet.byRepeatDays = DEF_PWRREMINDER_DEFAULT_REPEAT;
		}
	}

	// Check style ID
	if ((pwrItem.dwStyle < PRSTYLE_MSGBOX) || (pwrItem.dwStyle > PRSTYLE_DIALOG)) {
		nMsgStringID = MSGBOX_PWRREMINDER_INVALIDITEM_STYLEID;
		arrMsgString.Add(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;

		// Auto correction
		if (bAutoCorrect == TRUE) {
			// Set default style ID
			pwrItem.dwStyle = PRSTYLE_MSGBOX;
		}
	}
	
	// Show error message if enabled
	if ((bShowMsg == TRUE) && (!arrMsgString.IsEmpty())) {
		for (int nIndex = 0; nIndex < arrMsgString.GetSize(); nIndex++) {
			// If auto correction is ON
			if (bAutoCorrect == TRUE) {
				// Add "Data will be automatically reset to default"
				CString strErrMessage = arrMsgString.GetAt(nIndex);
				strErrMessage += GetLanguageString(pLang, MSGBOX_PWRREMINDER_INVALIDITEM_AUTOCORRECT);
				DisplayMessageBox(strErrMessage, MSGBOX_PWRREMINDER_CAPTION, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				DisplayMessageBox(arrMsgString.GetAt(nIndex), MSGBOX_PWRREMINDER_CAPTION, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	arrMsgString.RemoveAll();
	arrMsgString.FreeExtra();

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	GetItemNum
//	Description:	Get data table item number
//  Arguments:		None
//  Return value:	int - Number of items
//
//////////////////////////////////////////////////////////////////////////

int CPwrReminderDlg::GetItemNum()
{
	return m_pwrReminderDataTemp.GetItemNum();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	Get/SetCurMode
//	Description:	Get/set current mode
//  Arguments:		nMode - Mode to set
//  Return value:	int - Current mode
//
//////////////////////////////////////////////////////////////////////////

int CPwrReminderDlg::GetCurMode()
{
	return m_nCurMode;
}

void CPwrReminderDlg::SetCurMode(int nMode)
{
	// If new mode is the same, do nothing
	if (nMode == GetCurMode()) return;

	// Set new mode
	m_nCurMode = nMode;
	
	// Switch mode
	SwitchMode(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawRepeatSetButton
//	Description:	Draw icon for repeat set button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CPwrReminderDlg::DrawRepeatSetButton(void)
{
	// Check button validity
	if (m_pEvtRepeatSetBtn == NULL) {
		m_pEvtRepeatSetBtn = (CButton*)GetDlgItem(IDC_PWRREMINDER_EVENT_REPEATSET_BTN);
		if (m_pEvtRepeatSetBtn == NULL) {
			TRCLOG("Error: RepeatSet button not found");
			TRCDBG(__FUNCTION__, __FILENAME__, __LINE__);
			return;
		}
	}

	// Draw button icon
	if (m_pEvtRepeatSetBtn != NULL) {

		// Load icon
		HINSTANCE hInstance = AfxGetApp()->m_hInstance;
		HICON hRepeatIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON_REPEAT), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
		if (hRepeatIcon == NULL)
			return;

		// Set button icon
		m_pEvtRepeatSetBtn->ModifyStyle(NULL, BS_ICON);
		m_pEvtRepeatSetBtn->SetIcon(hRepeatIcon);
	}
}
