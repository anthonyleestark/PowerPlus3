
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//		File name:		HotkeySetDlg.cpp
//		Description:	Source file for HotkeySet dialog
//		Owner:			AnthonyLeeStark
//		
//		History:		<0> 2024.05.01:		Create new
//						<1> 2024.07.06:		Update to version 3.1
//						<2> 2024.12.18:		Update to version 3.2
//
//		Copyright (c) 2015-2024 AnthonyLeeStark
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/HotkeySetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace MapTable;
using namespace Language;
using namespace AppCore;
using namespace AppRegistry;


// Data list table constants
constexpr const int fixedColumnNum = 0;
constexpr const int fixedRowNum = 1;
constexpr const int startRowIndex = 1;

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

	// Data list table control
	m_pHotkeySetListTable = NULL;

	// Data container variables
	m_hksHotkeySet.Init();
	m_hksHotkeySetTemp.Init();

	// Checkbox variables
	m_bCtrlBtn = FALSE;
	m_bAltBtn = FALSE;
	m_bWinKeyBtn = FALSE;

	// Table format and properties
	m_nColNum = 0;
	m_apGrdColFormat = NULL;
	m_pszDataTableFrameSize = NULL;

	// Other variables
	m_nCheckCount = 0;
	m_nCurSelIndex = -1;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	~CHotkeySetDlg
//	Description:	Destructor
//
//////////////////////////////////////////////////////////////////////////

CHotkeySetDlg::~CHotkeySetDlg()
{
	// Data item list control
	if (m_pHotkeySetListTable) {
		delete m_pHotkeySetListTable;
		m_pHotkeySetListTable = NULL;
	}

	// Remove HotkeySet data
	m_hksHotkeySet.DeleteAll();
	m_hksHotkeySetTemp.DeleteAll();

	// Other variables
	if (m_apGrdColFormat != NULL) {
		delete[] m_apGrdColFormat;
		m_apGrdColFormat = NULL;
	}
	if (m_pszDataTableFrameSize != NULL) {
		delete m_pszDataTableFrameSize;
		m_pszDataTableFrameSize = NULL;
	}
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
	DDX_Control(pDX, IDC_HOTKEYSET_ACTION_LIST, m_cmbActionList);
	DDX_Control(pDX, IDC_HOTKEYSET_FUNCKEY_LIST, m_cmbFuncKeyList);
	DDX_Check(pDX, IDC_HOTKEYSET_CTRLKEY_CHK, m_bCtrlBtn);
	DDX_Check(pDX, IDC_HOTKEYSET_ALTKEY_CHK, m_bAltBtn);
	DDX_Check(pDX, IDC_HOTKEYSET_WINKEY_CHK, m_bWinKeyBtn);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RegisterDialogManagement
//	Description:	Register dialog control management
//  Arguments:		None
//  Return value:	int
//
//////////////////////////////////////////////////////////////////////////

int CHotkeySetDlg::RegisterDialogManagement(void)
{
	size_t nRet = SDialog::RegisterDialogManagement();
	if (nRet != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return nRet;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_ITEM_LISTBOX, List_Box);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_ADD_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_REMOVE_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_REMOVEALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_CHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_UNCHECKALL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_EXPORT_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_APPLY_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_CANCEL_BTN, Button);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_ACTION_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_ACTION_LIST, Combo_Box);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_CONTROLKEY_STATIC, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_CTRLKEY_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_ALTKEY_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_WINKEY_CHK, Check_Box);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_FUNCKEY_TITLE, Static_Text);
		nRet = pCtrlMan->AddControl(IDC_HOTKEYSET_FUNCKEY_LIST, Combo_Box);
	}

	return nRet;
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UnregisterDialogManagement
//	Description:	Unregister dialog control management
//  Arguments:		None
//  Return value:	TRUE/FALSE
//
//////////////////////////////////////////////////////////////////////////

BOOL CHotkeySetDlg::UnregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->GetControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_ITEM_LISTBOX);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_ADD_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_REMOVE_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_REMOVEALL_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_CHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_UNCHECKALL_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_EXPORT_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_APPLY_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_CANCEL_BTN);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_ACTION_TITLE);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_ACTION_LIST);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_CONTROLKEY_STATIC);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_CTRLKEY_CHK);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_ALTKEY_CHK);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_WINKEY_CHK);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_FUNCKEY_TITLE);
		pCtrlMan->RemoveControl(IDC_HOTKEYSET_FUNCKEY_LIST);
	}

	return SDialog::UnregisterDialogManagement();
}

//////////////////////////////////////////////////////////////////////////
//
//	CHotkeySetDlg dialog items ID map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_RESOURCEID_MAP(CHotkeySetDlg)
	ON_ID_DIALOG(IDD_HOTKEYSET_DLG,					"HotkeySetDlg")
	ON_ID_CONTROL(IDC_HOTKEYSET_ITEM_LISTBOX,		"HotkeySetList")
	ON_ID_CONTROL(IDC_HOTKEYSET_ADD_BTN,			"AddButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_REMOVE_BTN,			"RemoveButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_REMOVEALL_BTN,		"RemoveAllButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_CHECKALL_BTN,		"CheckAllButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_UNCHECKALL_BTN,		"UncheckAllButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_EXPORT_BTN,			"ExportButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_APPLY_BTN,			"ApplyButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_CANCEL_BTN,			"CancelButton")
	ON_ID_CONTROL(IDC_HOTKEYSET_ACTION_TITLE,		"ActionTitle")
	ON_ID_CONTROL(IDC_HOTKEYSET_ACTION_LIST,		"ActionListCombo")
	ON_ID_CONTROL(IDC_HOTKEYSET_CONTROLKEY_STATIC,	"ControlKeyTitle")
	ON_ID_CONTROL(IDC_HOTKEYSET_CTRLKEY_CHK,		"CtrlKeyCheck")
	ON_ID_CONTROL(IDC_HOTKEYSET_ALTKEY_CHK,			"AltKeyCheck")
	ON_ID_CONTROL(IDC_HOTKEYSET_WINKEY_CHK,			"WinKeyCheck")
	ON_ID_CONTROL(IDC_HOTKEYSET_FUNCKEY_TITLE,		"FunctionKeyTitle")
	ON_ID_CONTROL(IDC_HOTKEYSET_FUNCKEY_LIST,		"FunctionKeyListCombo")
END_RESOURCEID_MAP()


//////////////////////////////////////////////////////////////////////////
//
//	CHotkeySetDlg dialog message map
//
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CHotkeySetDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_HOTKEYSET_APPLY_BTN,					&CHotkeySetDlg::OnApply)
	ON_BN_CLICKED(IDC_HOTKEYSET_CANCEL_BTN,					&CHotkeySetDlg::OnCancel)
	ON_BN_CLICKED(IDC_HOTKEYSET_ADD_BTN,					&CHotkeySetDlg::OnAdd)
	ON_BN_CLICKED(IDC_HOTKEYSET_REMOVE_BTN,					&CHotkeySetDlg::OnRemove)
	ON_BN_CLICKED(IDC_HOTKEYSET_REMOVEALL_BTN,				&CHotkeySetDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_HOTKEYSET_CHECKALL_BTN,				&CHotkeySetDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_HOTKEYSET_UNCHECKALL_BTN,				&CHotkeySetDlg::OnUncheckAll)
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
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	SetUseEnter(FALSE);

	// Register message box caption
	RegisterMessageBoxCaption(MSGBOX_HOTKEYSET_CAPTION);

	// Load data
	LoadHotkeySetData();

	// Init dialog items
	LoadLayoutInfo();
	SetupLanguage();

	// Update data
	UpdateHotkeySet();
	DisplayHotkeyDetails(INT_INVALID);
	RefreshDialogItemState();

	// Save dialog event log if enabled
	OutputEventLog(LOG_EVENT_DLG_INIT, this->GetCaption());

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

void CHotkeySetDlg::OnClose()
{
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Ask for saving before exiting if data changed
		BOOL bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == TRUE) {
			// Show save confirmation message
			int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveHotkeySetData();
			}
		}
	}

	// Close dialog
	SDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnDestroy
//	Description:	Default method for dialog destroying
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnDestroy()
{
	// Save app event log if enabled
	OutputEventLog(LOG_EVENT_DLG_DESTROYED, this->GetCaption());

	// Save layout info data
	UpdateLayoutInfo();
	SaveLayoutInfo();

	// Destroy dialog
	SDialog::OnDestroy();
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_APPLY_BTN);

	// Save data if changed
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == TRUE) {
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
	// If not forced closing by request
	if (!IsForceClosingByRequest()) {

		// Save app event log if enabled
		OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_CANCEL_BTN);

		// Ask for saving before exiting if data changed
		BOOL bIsChanged = CheckDataChangeState();
		SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
		if (bIsChanged == TRUE) {
			// Show save confirmation message
			int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (nConfirm == IDYES) {
				// Save data
				SaveHotkeySetData();
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

void CHotkeySetDlg::OnAdd()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_ADD_BTN);

	// Update data
	Add();

	// Redraw HotkeySet table
	RedrawHotkeySetTable();
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_REMOVE_BTN);

	// If there's no item, do nothing
	int nItemNum = m_hksHotkeySetTemp.GetItemNum();
	if (nItemNum <= 0)
		return;

	// Get current select item index
	int nIndex = GetListCurSel();
	if (m_hksHotkeySetTemp.IsEmpty(nIndex) == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {

		// Update data
		Remove(nIndex);

		// Redraw HotkeySet table
		RedrawHotkeySetTable();
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (m_hksHotkeySetTemp.IsAllEmpty() == TRUE)
		return;

	// Ask before remove
	int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (nConfirm == IDYES) {

		// Update data
		RemoveAll();

		// Redraw HotkeySet table
		RedrawHotkeySetTable();
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_hksHotkeySetTemp.IsAllEmpty() == TRUE)
		return;

	// Check all items
	SwitchAllItemState(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	OnUncheckAll
//	Description:	Handle click event for [Uncheck All] button
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::OnUncheckAll()
{
	// Save app event log if enabled
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (m_hksHotkeySetTemp.IsAllEmpty() == TRUE)
		return;

	// Uncheck all items
	SwitchAllItemState(FALSE);
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
	OutputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_EXPORT_BTN);
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
	// Get clicked item info
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*)pNMHDR;
	if (pItem == NULL) return;
	int nCurSelRow = pItem->iRow;

	// Update current selection index
	SetListCurSel(nCurSelRow - startRowIndex);
	int nItemCount = GetItemNum();

	*pResult = NULL;

	// Invalid selection
	if (GetListCurSel() < 0 || GetListCurSel() >= nItemCount)
		return;

	// Display item details
	DisplayHotkeyDetails(GetListCurSel());
	RefreshDialogItemState();
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

void CHotkeySetDlg::OnClickHotkeyList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState();
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

void CHotkeySetDlg::OnRightClickHotkeyList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	RefreshDialogItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RequestCloseDialog
//	Description:	Request current dialog to close
//  Arguments:		None
//  Return value:	LRESULT (0:Success, else:Failed)
//
//////////////////////////////////////////////////////////////////////////

LRESULT CHotkeySetDlg::RequestCloseDialog(void)
{
	// Ask for saving before exiting if data changed
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	if (bIsChanged == TRUE) {
		int nConfirm = DisplayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (nConfirm == IDYES) {
			// Save data
			SaveHotkeySetData();
		}
		else if (nConfirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Request accepted
	return SDialog::RequestCloseDialog();
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
	this->SetCaptionFromLanguage(GetDialogID());

	// Loop through all dialog items and setup language for each one of them
	for (CWnd* pWndChild = GetTopWindow(); pWndChild != NULL; pWndChild = pWndChild->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		UINT nID = pWndChild->GetDlgCtrlID();

		switch (nID)
		{
		case IDC_HOTKEYSET_DETAIL_STATIC:
		case IDC_HOTKEYSET_ITEM_LISTBOX:
			// Skip these items
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

	// Setup HotkeySet list table
	SetupHotkeySetList(pAppLang);

	// Default
	SDialog::SetupLanguage();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SetupHotkeySetList
//	Description:	Initialize and setup language for HotkeySet list
//  Arguments:		ptrLanguage - Language package pointer
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SetupHotkeySetList(LANGTABLE_PTR /*ptrLanguage*/)
{
	// Get parent list frame rect
	CWnd* pListFrameWnd = GetDlgItem(IDC_HOTKEYSET_ITEM_LISTBOX);
	if (pListFrameWnd == NULL) return;
	RECT rcListFrameWnd;
	pListFrameWnd->GetWindowRect(&rcListFrameWnd);
	ScreenToClient(&rcListFrameWnd);

	// Get frame size
	if (m_pszDataTableFrameSize == NULL) {
		m_pszDataTableFrameSize = new CSize();
		m_pszDataTableFrameSize->cx = rcListFrameWnd.right - rcListFrameWnd.left;
		m_pszDataTableFrameSize->cy = rcListFrameWnd.bottom - rcListFrameWnd.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(m_pHotkeySetListTable, CGridCtrl)

	// Create table
	if (m_pHotkeySetListTable == NULL) return;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	m_pHotkeySetListTable->Create(rcListFrameWnd, this, IDC_HOTKEYSET_ITEM_LISTBOX, dwStyle);

	// Destroy frame
	pListFrameWnd->DestroyWindow();

	// Cell format
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pHotkeySetListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;
	pCell->SetFormat(pCell->GetFormat());
	pCell->SetMargin(0);
	pCell->SetBackClr(Color::White);
	pCell->SetTextClr(Color::Black);
	pCell->SetHeight(GRIDCTRL_HEIGHT_ROW);

	// Table format and properties
	int nRowNum = (GetItemNum() + fixedRowNum);
	int nColNum = m_nColNum;

	// Setup table
	m_pHotkeySetListTable->SetColumnCount(nColNum);
	m_pHotkeySetListTable->SetFixedColumnCount(fixedColumnNum);
	m_pHotkeySetListTable->SetRowCount(nRowNum);
	m_pHotkeySetListTable->SetFixedRowCount(fixedRowNum);
	m_pHotkeySetListTable->SetRowHeight(GRIDCTRL_INDEX_HEADER_ROW, GRIDCTRL_HEIGHT_HEADER);

	// Draw table
	DrawHotkeySetTable(GetReadOnlyMode());

	// Update layout info
	UpdateLayoutInfo();

	// Display table
	m_pHotkeySetListTable->SetListMode(TRUE);
	m_pHotkeySetListTable->SetEditable(FALSE);
	m_pHotkeySetListTable->SetRowResize(FALSE);
	m_pHotkeySetListTable->EnableSelection(TRUE);
	m_pHotkeySetListTable->SetSingleRowSelection(TRUE);
	m_pHotkeySetListTable->SetSingleColSelection(FALSE);
	m_pHotkeySetListTable->SetFixedRowSelection(FALSE);
	m_pHotkeySetListTable->SetFixedColumnSelection(FALSE);
	m_pHotkeySetListTable->ShowWindow(SW_SHOW);
	m_pHotkeySetListTable->SetRedraw(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DrawHotkeySetTable
//	Description:	Draw HotkeySet data list table
//  Arguments:		bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::DrawHotkeySetTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pHotkeySetListTable == NULL) return;

	// Check table format data validity
	if (m_pszDataTableFrameSize == NULL) return;

	// Get app pointer
	CPowerPlusApp* pApp = (CPowerPlusApp*)AfxGetApp();
	if (pApp == NULL) return;

	// Get app language package
	LANGTABLE_PTR ptrLanguage = pApp->GetAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* pCell = (CGridDefaultCell*)m_pHotkeySetListTable->GetDefaultCell(FALSE, FALSE);
	if (pCell == NULL) return;

	// Read-only mode --> Change cell color
	if (bReadOnly == TRUE) {
		pCell->SetBackClr(Color::Bright_Gray);
		pCell->SetTextClr(Color::Dark_Gray);
	}
	else {
		pCell->SetBackClr(Color::White);
		pCell->SetTextClr(Color::Black);
	}

	// Table properties
	int nColNum = m_nColNum;
	int nRowNum = (GetItemNum() + fixedRowNum);

	// Setup display size
	int nFrameHeight = m_pszDataTableFrameSize->cy;
	int nFrameWidth = m_pszDataTableFrameSize->cx;
	if (GetWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		nFrameWidth -= OFFSET_WIDTH_LISTCTRL;
	}
	if ((GRIDCTRL_HEIGHT_HEADER + ((nRowNum - 1) * GRIDCTRL_HEIGHT_ROW)) >= nFrameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		nFrameWidth -= (nScrollBarWidth + OFFSET_WIDTH_VSCRLBR);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		SetFixedCellStyle(m_pHotkeySetListTable, GRIDCTRL_INDEX_HEADER_ROW, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		UINT nHeaderTitleID = m_apGrdColFormat[nCol].nHeaderTitleID;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = GetLanguageString(ptrLanguage, nHeaderTitleID);
		}
		m_pHotkeySetListTable->SetItemText(GRIDCTRL_INDEX_HEADER_ROW, nCol, headerTitle);

		// Column width
		int nColWidth = m_apGrdColFormat[nCol].nWidth;
		if (nColWidth != -1) {
			// Set column width as defined
			if (m_pHotkeySetListTable->SetColumnWidth(nCol, nColWidth)) {
				// Calculate remaining width
				nFrameWidth -= nColWidth;
			}
		}
		else {
			// Set remaining width for current column
			m_pHotkeySetListTable->SetColumnWidth(nCol, nFrameWidth);
		}
	}

	// Setup rows
	int nColStyle = -1;
	UINT nItemState = INT_NULL;
	for (int nRow = 1; nRow < nRowNum; nRow++) {
		for (int nCol = 0; nCol < m_nColNum; nCol++) {

			// Get column style & item state
			nColStyle = m_apGrdColFormat[nCol].nColStyle;
			nItemState = m_pHotkeySetListTable->GetItemState(nRow, nCol);
			nItemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				SetFixedCellStyle(m_pHotkeySetListTable, nRow, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!m_pHotkeySetListTable->SetCellType(nRow, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* pCell = (CGridCellCheck*)m_pHotkeySetListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!m_pHotkeySetListTable->SetItemState(nRow, nCol, nItemState))
					continue;

				// Get cell
				CGridCellBase* pCell = (CGridCellBase*)m_pHotkeySetListTable->GetCell(nRow, nCol);

				// Set center alignment if defined
				if (m_apGrdColFormat[nCol].bCenter == TRUE) {
					if (pCell == NULL) continue;
					pCell->SetFormat(pCell->GetFormat() | DT_CENTER);
				}
				else {
					// Set margin (left alignment)
					if (pCell == NULL) continue;
					pCell->SetMargin(GRIDCELL_MARGIN_LEFT);
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
		for (int nIndex = 0; nIndex < TABLE_SIZE(StringTable::FunctionKeys); nIndex++)
			m_cmbFuncKeyList.AddString(StringTable::FunctionKeys[nIndex].langString);
		break;

	default:
		break;
	}

	// Default
	SDialog::SetupComboBox(nComboID, ptrLanguage);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RefreshDialogItemState
//	Description:	Refresh and update state for dialog items
//  Arguments:		bRecheckState - Recheck all item's state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::RefreshDialogItemState(BOOL bRecheckState /* = FALSE */)
{
	// Enable/disable buttons if data is all empty or not
	BOOL bIsAllEmpty = m_hksHotkeySetTemp.IsAllEmpty();
	EnableItem(IDC_HOTKEYSET_REMOVE_BTN, !bIsAllEmpty);
	EnableItem(IDC_HOTKEYSET_REMOVEALL_BTN, !bIsAllEmpty);

	// Enable/disable buttons if data changed or not
	BOOL bIsChanged = CheckDataChangeState();
	SetFlagValue(AppFlagID::dialogDataChanged, bIsChanged);
	EnableItem(IDC_HOTKEYSET_APPLY_BTN, bIsChanged);

	// Update "Check All" button state
	UpdateCheckAllBtnState();

	// Default
	SDialog::RefreshDialogItemState(bRecheckState);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateCheckAllBtnState
//	Description:	Refresh and update state for [Check/Uncheck All] button
//  Arguments:		bRecheck - Flag to recheck data item list
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateCheckAllBtnState(BOOL bRecheck /* = TRUE */)
{
	// If dialog items are being locked, do nothing
	if (GetLockState() == TRUE)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_HOTKEYSET_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_HOTKEYSET_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int nItemNum = GetItemNum();
	if (nItemNum == 0) {
		// Disable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(FALSE);
		pUncheckAllBtn->EnableWindow(FALSE);
		return;
	}

	// Recheck all items state
	if (bRecheck == TRUE) {

		// Reset counter
		m_nCheckCount = 0;
		// Check for item states
		for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
			const Item& hksTemp = m_hksHotkeySetTemp.GetItemAt(nIndex);
			if (hksTemp.IsEnabled() == TRUE) {
				// Increase counter
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
//	Function name:	UpdateHotkeySet
//	Description:	Update HotkeySet list
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateHotkeySet()
{
	// Check table validity
	if (m_pHotkeySetListTable == NULL) return;

	// If there's no item, do nothing
	int nItemNum = GetItemNum();
	if (nItemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR ptrLanguage = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Print items
	String tempString;
	int nTemp = -1;
	int nItemIndex = 0;
	CGridCellCheck* pCellCheck = NULL;
	for (int nRowIndex = startRowIndex; nRowIndex <= nItemNum; nRowIndex++) {

		// Get item
		nItemIndex = nRowIndex - startRowIndex;
		const Item& hksItem = m_hksHotkeySetTemp.GetItemAt(nItemIndex);

		// Enable state
		pCellCheck = (CGridCellCheck*)m_pHotkeySetListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheck != NULL) {
			pCellCheck->SetCheck(hksItem.IsEnabled());
		}

		// Hotkey action
		nTemp = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, hksItem.GetActionID()));
		tempString = GetLanguageString(ptrLanguage, nTemp);
		m_pHotkeySetListTable->SetItemText(nRowIndex, ColumnID::HKActionID, tempString);

		// Keystrokes
		hksItem.PrintKeyStrokes(tempString);
		if (IS_NULL_STRING(tempString)) {
			// Undefined keystrokes
			tempString = GetLanguageString(ptrLanguage, HKEYSET_KEYSTROKES_NULL);
		}
		m_pHotkeySetListTable->SetItemText(nRowIndex, ColumnID::Keystrokes, tempString);
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	DisableHotkeySetTable
//	Description:	Disable mouse click events for HotkeySet data table
//  Arguments:		bDisable - Disable/enable
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::DisableHotkeySetTable(BOOL bDisable)
{
	// Redraw read-only style
	RedrawHotkeySetTable(bDisable);

	// Check table validity
	if (m_pHotkeySetListTable == NULL) return;

	// Disable/enable mouse events
	m_pHotkeySetListTable->DisableMouseClick(bDisable);
	m_pHotkeySetListTable->DisableMouseMove(bDisable);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RedrawHotkeySetTable
//	Description:	Update and redraw HotkeySet data table
//  Arguments:		BOOL bReadOnly - Read-only mode
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::RedrawHotkeySetTable(BOOL bReadOnly /* = FALSE */)
{
	// Check table validity
	if (m_pHotkeySetListTable == NULL) return;

	// Update new row number
	int nCurRowNum = (GetItemNum() + fixedRowNum);
	m_pHotkeySetListTable->SetRowCount(nCurRowNum);

	// Draw table
	DrawHotkeySetTable(bReadOnly);

	// Update table data
	UpdateHotkeySet();

	// Trigger redrawing table
	m_pHotkeySetListTable->RedrawWindow();
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
	if ((nIndex < 0) || (nIndex >= m_hksHotkeySetTemp.GetItemNum())) {
		m_cmbActionList.SetWindowText(_T("---"));
		m_cmbFuncKeyList.SetWindowText(_T("---"));
		UpdateData(FALSE);
		return;
	}

	// Get item at index
	const Item& hksCurItem = m_hksHotkeySetTemp.GetItemAt(nIndex);

	// Get item keycode
	DWORD dwModifiers, dwVirtualKey;
	hksCurItem.GetKeyCode(dwModifiers, dwVirtualKey);

	// Update checkboxes
	m_bCtrlBtn = FALSE;
	if (dwModifiers & MOD_CONTROL)		m_bCtrlBtn = TRUE;
	m_bAltBtn = FALSE;
	if (dwModifiers & MOD_ALT)			m_bAltBtn = TRUE;
	m_bWinKeyBtn = FALSE;
	if (dwModifiers & MOD_WIN)			m_bWinKeyBtn = TRUE;

	// Update combo-boxes
	UINT nActionID = GetPairedID(IDTable::HKActionID, hksCurItem.GetActionID());
	m_cmbActionList.SetCurSel(Opt2Sel(APP_ACTION, nActionID));
	m_cmbFuncKeyList.SetWindowText(_T("---"));
	if (dwVirtualKey > 0)
		m_cmbFuncKeyList.SetCurSel(dwVirtualKey - VK_F1);

	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	UpdateLayoutInfo
//	Description:	Update layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::UpdateLayoutInfo(void)
{
	// Check table validity
	if (m_pHotkeySetListTable == NULL) return;

	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Get table column count
	int nColNum = m_pHotkeySetListTable->GetColumnCount();

	// Update size of table columns
	for (int nIndex = 0; nIndex < nColNum; nIndex++) {
		int nColSize = m_pHotkeySetListTable->GetColumnWidth(nIndex);
		m_apGrdColFormat[nIndex].nWidth = nColSize;
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	LoadLayoutInfo
//	Description:	Load layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::LoadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)----Column style-------Align Center---
		{	ColumnID::EnableState,		GRIDCOLUMN_HOTKEYSET_STATE,			28,		COLSTYLE_CHECKBOX,		TRUE,	},
		{	ColumnID::HKActionID,		GRIDCOLUMN_HOTKEYSET_HKACTIONID,	122,	COLSTYLE_NORMAL,		TRUE,	},
		{ 	ColumnID::Keystrokes,		GRIDCOLUMN_HOTKEYSET_KEYSTROKES,	-1,		COLSTYLE_NORMAL,		TRUE,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	m_nColNum = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (m_apGrdColFormat == NULL) {
		m_apGrdColFormat = new GRIDCTRLCOLFORMAT[m_nColNum];
		for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
			// Copy default table column format data
			m_apGrdColFormat[nIndex] = arrGrdColFormat[nIndex];
		}
	}

	// Load layout info data from registry
	int nRet = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		if (GetLayoutInfo(Section::LayoutInfo::HKeySetTable, keyName, nRet)) {
			if (m_apGrdColFormat != NULL) {
				m_apGrdColFormat[nIndex].nWidth = nRet;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SaveLayoutInfo
//	Description:	Save layout info data
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SaveLayoutInfo(void)
{
	// Check table column format data validity
	if (m_apGrdColFormat == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int nIndex = 0; nIndex < m_nColNum; nIndex++) {
		nRef = m_apGrdColFormat[nIndex].nWidth;
		keyName = Key::LayoutInfo::GridColSize(nIndex);
		WriteLayoutInfo(Section::LayoutInfo::HKeySetTable, keyName, nRef);
	}
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
	HotkeySetData* phksData = pApp->GetAppHotkeySetData();
	if (phksData == NULL)
		return FALSE;

	// Copy data
	m_hksHotkeySet.Copy(*phksData);
	m_hksHotkeySetTemp.Copy(m_hksHotkeySet);

	// Reset change flag
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);
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
	SetFlagValue(AppFlagID::dialogDataChanged, FALSE);

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
	int nItemIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	for (int nRowIndex = startRowIndex; nRowIndex <= GetItemNum(); nRowIndex++) {
		// Get checkbox cell
		pCellCheckEnable = (CGridCellCheck*)m_pHotkeySetListTable->GetCell(nRowIndex, ColumnID::EnableState);
		if (pCellCheckEnable == NULL) continue;

		// Get checked states
		BOOL bEnabled = pCellCheckEnable->GetCheck();

		// Update item checked state
		nItemIndex = nRowIndex - startRowIndex;
		Item& hksTempItem = m_hksHotkeySetTemp.GetItemAt(nItemIndex);
		hksTempItem.EnableItem(bEnabled);
	}

	// Check if number of items changed
	bChangeFlag |= (m_hksHotkeySetTemp.GetItemNum() != m_hksHotkeySet.GetItemNum());
	if (bChangeFlag == TRUE)
		return bChangeFlag;

	// Check if each item's data changed
	for (int nIndex = 0; nIndex < m_hksHotkeySetTemp.GetItemNum(); nIndex++) {
		// Get current item and temp item
		const Item& hksCurItem = m_hksHotkeySet.GetItemAt(nIndex);
		const Item& hksTempItem = m_hksHotkeySetTemp.GetItemAt(nIndex);
 
		// Data comparison
		bChangeFlag |= (hksTempItem.IsEnabled() != hksCurItem.IsEnabled());
		bChangeFlag |= (hksTempItem.Compare(hksCurItem) != TRUE);

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

void CHotkeySetDlg::Add(void)
{
	// Update data
	UpdateData(TRUE);

	// Create temp hotkeyset item
	Item hksTemp = STRUCT_ZERO;

	// Keycode
	DWORD dwModifiers = 0, dwVirtualKey = 0;

	// Update modifier keys
	if (m_bCtrlBtn == TRUE)		dwModifiers |= MOD_CONTROL;
	if (m_bAltBtn == TRUE)		dwModifiers |= MOD_ALT;
	if (m_bWinKeyBtn == TRUE)	dwModifiers |= MOD_WIN;

	// Update action ID
	int nCurSel = m_cmbActionList.GetCurSel();
	int nHKActionID = IDTable::HKActionID[nCurSel].first;
	hksTemp.SetActionID(nHKActionID);

	// Update virtual key code
	nCurSel = m_cmbFuncKeyList.GetCurSel();
	dwVirtualKey = (nCurSel + VK_F1);

	// Update item hotkey code
	hksTemp.SetKeyCode(dwModifiers, dwVirtualKey);

	// Check data validity
	BOOL bValid = Validate(hksTemp, TRUE);
	if (bValid == FALSE)
		return;

	// Update item to HotkeySet data list
	m_hksHotkeySetTemp.Update(hksTemp);

	// Refresh button state
	RefreshDialogItemState();
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
	RefreshDialogItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	RemoveAll
//	Description:	Remove all HotkeySet items
//  Arguments:		None
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::RemoveAll(void)
{
	// Remove all items
	m_hksHotkeySetTemp.RemoveAll();

	// Refresh button state
	RefreshDialogItemState();
}

//////////////////////////////////////////////////////////////////////////
// 
//	Function name:	SwitchAllItemState
//	Description:	Check/uncheck all HotkeySet items
//  Arguments:		bState - New state
//  Return value:	None
//
//////////////////////////////////////////////////////////////////////////

void CHotkeySetDlg::SwitchAllItemState(BOOL bState)
{
	// Check/uncheck all --> Update all item enable state
	int nItemNum = m_hksHotkeySetTemp.GetItemNum();
	for (int nIndex = 0; nIndex < nItemNum; nIndex++) {
		Item& hksTemp = m_hksHotkeySetTemp.GetItemAt(nIndex);
		if (hksTemp.IsEnabled() != bState) {
			hksTemp.EnableItem(bState);
		}
	}

	// Update hotkeyset list
	UpdateHotkeySet();

	// Refresh button state
	RefreshDialogItemState();
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

BOOL CHotkeySetDlg::Validate(const Item& hksItem, BOOL bShowMsg /* = FALSE */)
{
	BOOL bResult = TRUE;
	int nMsgStringID;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR pLang = ((CPowerPlusApp*)AfxGetApp())->GetAppLanguage();

	// Check action ID
	if ((hksItem.GetActionID() < HKID::displayOff) || (hksItem.GetActionID() > HKID::hibernate)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Get item keycode
	DWORD dwModifiers, dwVirtualKey;
	hksItem.GetKeyCode(dwModifiers, dwVirtualKey);

	// Validate modifier keys
	if ((dwModifiers <= 0) ||
		((dwModifiers & MOD_CONTROL) == FALSE) && ((dwModifiers & MOD_ALT) == FALSE) && ((dwModifiers & MOD_WIN) == FALSE)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Validate virtual key code
	if ((dwVirtualKey < VK_F1) || (dwVirtualKey > VK_F12)) {
		nMsgStringID = MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY;
		arrMsgString.push_back(GetLanguageString(pLang, nMsgStringID));
		bResult = FALSE;
	}

	// Check if system hotkey existed
	int nExistedSysHotkeyNum = TABLE_SIZE(OtherTable::ExistedSysHotkeyList);
	for (int nIndex = 0; nIndex < nExistedSysHotkeyNum; nIndex++) {
		if ((dwModifiers == OtherTable::ExistedSysHotkeyList[nIndex].dwModifiers) &&
			(dwVirtualKey == OtherTable::ExistedSysHotkeyList[nIndex].dwVirtualKey)) {
			// Hotkey info format
			String keyStrokesString = Constant::String::Empty;
			if (dwModifiers & MOD_CONTROL)	keyStrokesString += _T("Ctrl + ");
			if (dwModifiers & MOD_ALT)		keyStrokesString += _T("Alt + ");
			if (dwModifiers & MOD_WIN)		keyStrokesString += _T("Win + ");
			keyStrokesString += GetString(StringTable::FunctionKeys, dwVirtualKey);
			String keyInfoString = Constant::String::Empty;
			keyInfoString.Format(_T("%s - %s"), keyStrokesString.GetString(), GetLanguageString(pLang, OtherTable::ExistedSysHotkeyList[nIndex].nHotkeyDescription));

			// Message format
			String messageFormat;
			messageFormat.Format(GetLanguageString(pLang, MSGBOX_HOTKEYSET_EXISTED_HOTKEY), keyInfoString.GetString());

			arrMsgString.push_back(messageFormat);
			bResult = FALSE;
		}
	}
	
	// Show error message if enabled
	if ((bShowMsg == TRUE) && (!arrMsgString.empty())) {
		for (int nIndex = 0; nIndex < arrMsgString.size(); nIndex++) {
			DisplayMessageBox(arrMsgString.at(nIndex), NULL, MB_OK | MB_ICONERROR);
		}
	}

	// Remove all message after displaying
	arrMsgString.clear();
	return bResult;
}
