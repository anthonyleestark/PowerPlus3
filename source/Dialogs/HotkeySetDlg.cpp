/**
 * @file		HotkeySetDlg.cpp
 * @brief		Source file for HotkeySet dialog
 * @author		AnthonyLeeStark
 * @date		2024.05.01
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

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


// Implement methods for CHotkeySetDlg
IMPLEMENT_DYNAMIC(CHotkeySetDlg, SDialog)


/**
 * @brief	Constructor
 */
CHotkeySetDlg::CHotkeySetDlg(CWnd* parentWnd /*=nullptr*/)
	: SDialog(IDD_HOTKEYSET_DLG, parentWnd)
{
	// Initialize member variables

	// Data list table control
	hotkeySetTablePtr_ = NULL;

	// Data container variables
	hotkeySetData_.init();
	tempHotkeySetData_.init();

	// Checkbox variables
	isCtrlKeyActive_ = false;
	isAltKeyActive_ = false;
	isWinKeyActive_ = false;

	// Table format and properties
	columnCount_ = 0;
	gridCtrlFormatInfoPtr_ = NULL;
	dataTableSizePtr_ = NULL;

	// Other variables
	checkCount_ = 0;
	curSelIndex_ = -1;
}

/**
 * @brief	Destructor
 */
CHotkeySetDlg::~CHotkeySetDlg()
{
	// Data item list control
	if (hotkeySetTablePtr_) {
		delete hotkeySetTablePtr_;
		hotkeySetTablePtr_ = NULL;
	}

	// Remove HotkeySet data
	hotkeySetData_.deleteAll();
	tempHotkeySetData_.deleteAll();

	// Other variables
	if (gridCtrlFormatInfoPtr_ != NULL) {
		delete[] gridCtrlFormatInfoPtr_;
		gridCtrlFormatInfoPtr_ = NULL;
	}
	if (dataTableSizePtr_ != NULL) {
		delete dataTableSizePtr_;
		dataTableSizePtr_ = NULL;
	}
}

/**
 * @brief	DoDataExchange function (DDX/DDV support)
 */
void CHotkeySetDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEYSET_ACTION_LIST, actionListCombo_);
	DDX_Control(pDX, IDC_HOTKEYSET_FUNCKEY_LIST, virtualKeyListCombo_);
	DDX_Check(pDX, IDC_HOTKEYSET_CTRLKEY_CHK, isCtrlKeyActive_);
	DDX_Check(pDX, IDC_HOTKEYSET_ALTKEY_CHK, isAltKeyActive_);
	DDX_Check(pDX, IDC_HOTKEYSET_WINKEY_CHK, isWinKeyActive_);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CHotkeySetDlg::registerDialogManagement(void)
{
	size_t returnValue = SDialog::registerDialogManagement();
	if (returnValue != 0) {
		TRACE_ERROR("Error: Register dialog management failed!!!");
		TRACE_DEBUG(__FUNCTION__, __FILENAME__, __LINE__);
		return returnValue;
	}

	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Add dialog controls to management
	if (pCtrlMan != NULL) {
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_ITEM_LISTBOX, List_Box);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_ADD_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_REMOVE_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_REMOVEALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_CHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_UNCHECKALL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_EXPORT_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_APPLY_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_CANCEL_BTN, Button);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_ACTION_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_ACTION_LIST, Combo_Box);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_CONTROLKEY_STATIC, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_CTRLKEY_CHK, Check_Box);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_ALTKEY_CHK, Check_Box);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_WINKEY_CHK, Check_Box);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_FUNCKEY_TITLE, Static_Text);
		returnValue = pCtrlMan->AddControl(IDC_HOTKEYSET_FUNCKEY_LIST, Combo_Box);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CHotkeySetDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

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

	return SDialog::unregisterDialogManagement();
}


// CHotkeySetDlg dialog items ID map
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


// CHotkeySetDlg dialog message map
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

/**
 * @brief	Initialize HotkeySet dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CHotkeySetDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	setUseEnter(false);

	// Register message box caption
	registerMessageBoxCaption(MSGBOX_HOTKEYSET_CAPTION);

	// Load data
	loadHotkeySetData();

	// Init dialog items
	loadLayoutInfo();
	setupLanguage();

	// Update data
	updateHotkeySet();
	displayHotkeyDetails(INT_INVALID);
	refreshDialogItemState();

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	// Read-only mode (if enabled)
	if (isReadOnlyMode() == true) {
		CWnd* childWndPtr = GetTopWindow();
		while (childWndPtr != NULL) {
			childWndPtr->EnableWindow(false);
			childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT);
		}
	}

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnClose()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Ask for saving before exiting if data changed
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Show save confirmation message
			int confirm = displayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Save data
				saveHotkeySetData();
			}
		}
	}

	// Close dialog
	SDialog::OnClose();
}

/**
 * @brief	Default method for dialog destroying
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnDestroy()
{
	// Save app event log if enabled
	outputEventLog(LOG_EVENT_DLG_DESTROYED, this->getCaption());

	// Save layout info data
	updateLayoutInfo();
	saveLayoutInfo();

	// Destroy dialog
	SDialog::OnDestroy();
}

/**
 * @brief	Handle click event for [Apply] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnApply()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_APPLY_BTN);

	// Save data if changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		// Save data
		saveHotkeySetData();
	}

	// Close dialog
	SDialog::OnOK();
}

/**
 * @brief	Handle click event for [Cancel] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnCancel()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Save app event log if enabled
		outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_CANCEL_BTN);

		// Ask for saving before exiting if data changed
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Show save confirmation message
			int confirm = displayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Save data
				saveHotkeySetData();
			}
		}
	}

	// Close dialog
	SDialog::OnCancel();
}

/**
 * @brief	Handle click event for [Add] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnAdd()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_ADD_BTN);

	// Update data
	add();

	// Redraw HotkeySet table
	redrawHotkeySetTable();
}

/**
 * @brief	Handle click event for [Remove] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnRemove()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_REMOVE_BTN);

	// If there's no item, do nothing
	int itemNum = tempHotkeySetData_.getItemNum();
	if (itemNum <= 0)
		return;

	// Get current select item index
	int index = GetListCurSel();
	if (tempHotkeySetData_.isEmpty(index) == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_HOTKEYSET_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {

		// Update data
		remove(index);

		// Redraw HotkeySet table
		redrawHotkeySetTable();
	}
}

/**
 * @brief	Handle click event for [Remove All] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnRemoveAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (tempHotkeySetData_.isAllEmpty() == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_HOTKEYSET_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {

		// Update data
		removeAll();

		// Redraw HotkeySet table
		redrawHotkeySetTable();
	}
}

/**
 * @brief	Handle click event for [Check All] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnCheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (tempHotkeySetData_.isAllEmpty() == true)
		return;

	// Check all items
	switchAllItemState(true);
}

/**
 * @brief	Handle click event for [Uncheck All] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnUncheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (tempHotkeySetData_.isAllEmpty() == true)
		return;

	// Uncheck all items
	switchAllItemState(false);
}

/**
 * @brief	Handle click event for [Export] button
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::OnExport()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_HOTKEYSET_EXPORT_BTN);
}

/**
 * @brief	Show hotkey details when selecting an item
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CHotkeySetDlg::OnSelectHotkeyItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int nCurSelRow = reminderItem->iRow;

	// Update current selection index
	SetListCurSel(nCurSelRow - startRowIndex);
	int nItemCount = GetItemNum();

	*pResult = NULL;

	// Invalid selection
	if (GetListCurSel() < 0 || GetListCurSel() >= nItemCount)
		return;

	// Display item details
	displayHotkeyDetails(GetListCurSel());
	refreshDialogItemState();
}

/**
 * @brief	Handle click event on HotkeyList control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CHotkeySetDlg::OnClickHotkeyList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState();
}

/**
 * @brief	Handle right click event on HotkeyList control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CHotkeySetDlg::OnRightClickHotkeyList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState();
}

/**
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CHotkeySetDlg::requestCloseDialog(void)
{
	// Ask for saving before exiting if data changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		int confirm = displayMessageBox(MSGBOX_HOTKEYSET_CHANGED_CONTENT, NULL, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Save data
			saveHotkeySetData();
		}
		else if (confirm == IDCANCEL) {
			// Request denied
			return LRESULT(Result::Failure);
		}
	}

	// Request accepted
	return SDialog::requestCloseDialog();
}

/**
 * @brief	Setup language for dialog items
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup language for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDC_HOTKEYSET_DETAIL_STATIC:
		case IDC_HOTKEYSET_ITEM_LISTBOX:
			// Skip these items
			break;

		case IDC_HOTKEYSET_ACTION_LIST:
		case IDC_HOTKEYSET_FUNCKEY_LIST:
			setupComboBox(id, languageTablePtr);
			break;

		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup HotkeySet list table
	setupHotkeySetList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for HotkeySet list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CHotkeySetDlg::setupHotkeySetList(LANGTABLE_PTR /*languageTablePtr*/)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_HOTKEYSET_ITEM_LISTBOX);
	if (listFrameWndPtr == NULL) return;
	RECT listFrameWndRect;
	listFrameWndPtr->GetWindowRect(&listFrameWndRect);
	ScreenToClient(&listFrameWndRect);

	// Get frame size
	if (dataTableSizePtr_ == NULL) {
		dataTableSizePtr_ = new Size();
		dataTableSizePtr_->_width = listFrameWndRect.right - listFrameWndRect.left;
		dataTableSizePtr_->_height = listFrameWndRect.bottom - listFrameWndRect.top;
	}

	// Initialization
	VERIFY_INITIALIZATION(hotkeySetTablePtr_, CGridCtrl)

	// Create table
	if (hotkeySetTablePtr_ == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	hotkeySetTablePtr_->Create(listFrameWndRect, this, IDC_HOTKEYSET_ITEM_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)hotkeySetTablePtr_->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row);

	// Table format and properties
	int nRowNum = (GetItemNum() + fixedRowNum);
	int nColNum = columnCount_;

	// Setup table
	hotkeySetTablePtr_->SetColumnCount(nColNum);
	hotkeySetTablePtr_->SetFixedColumnCount(fixedColumnNum);
	hotkeySetTablePtr_->SetRowCount(nRowNum);
	hotkeySetTablePtr_->SetFixedRowCount(fixedRowNum);
	hotkeySetTablePtr_->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Row);

	// Draw table
	drawHotkeySetTable(isReadOnlyMode());

	// Update layout info
	updateLayoutInfo();

	// Display table
	hotkeySetTablePtr_->SetListMode(true);
	hotkeySetTablePtr_->SetEditable(false);
	hotkeySetTablePtr_->SetRowResize(false);
	hotkeySetTablePtr_->EnableSelection(true);
	hotkeySetTablePtr_->SetSingleRowSelection(true);
	hotkeySetTablePtr_->SetSingleColSelection(false);
	hotkeySetTablePtr_->SetFixedRowSelection(false);
	hotkeySetTablePtr_->SetFixedColumnSelection(false);
	hotkeySetTablePtr_->ShowWindow(SW_SHOW);
	hotkeySetTablePtr_->SetRedraw(true);
}

/**
 * @brief	Draw HotkeySet data list table
 * @param	isReadOnly - Read-only mode
 * @return	None
 */
void CHotkeySetDlg::drawHotkeySetTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (hotkeySetTablePtr_ == NULL) return;

	// Check table format data validity
	if (dataTableSizePtr_ == NULL) return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Get app language package
	LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)hotkeySetTablePtr_->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;

	// Read-only mode --> Change cell color
	if (isReadOnly == true) {
		cellPtr->SetBackClr(Color::Bright_Gray);
		cellPtr->SetTextClr(Color::Dark_Gray);
	}
	else {
		cellPtr->SetBackClr(Color::White);
		cellPtr->SetTextClr(Color::Black);
	}

	// Table properties
	int nColNum = columnCount_;
	int nRowNum = (GetItemNum() + fixedRowNum);

	// Setup display size
	int frameHeight = dataTableSizePtr_->height();
	int frameWidth = dataTableSizePtr_->width();
	if (AppCore::getWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
	}
	if ((Constant::UI::Offset::Width::ListCtrl + ((nRowNum - 1) * Constant::UI::GridCtrl::Height::Row)) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int nCol = 0; nCol < nColNum; nCol++) {
		// Set header row style
		AppCore::setFixedCellStyle(hotkeySetTablePtr_, Constant::UI::GridCtrl::Index::Header_Row, nCol);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned nHeaderTitleID = gridCtrlFormatInfoPtr_[nCol].headerTitleId;
		if (nHeaderTitleID != INT_NULL) {
			headerTitle = getLanguageString(languageTablePtr, nHeaderTitleID);
		}
		hotkeySetTablePtr_->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, nCol, headerTitle);

		// Column width
		int nColWidth = gridCtrlFormatInfoPtr_[nCol].width;
		if (nColWidth != -1) {
			// Set column width as defined
			if (hotkeySetTablePtr_->SetColumnWidth(nCol, nColWidth)) {
				// Calculate remaining width
				frameWidth -= nColWidth;
			}
		}
		else {
			// Set remaining width for current column
			hotkeySetTablePtr_->SetColumnWidth(nCol, frameWidth);
		}
	}

	// Setup rows
	int nColStyle = -1;
	unsigned itemState = INT_NULL;
	for (int row = 1; row < nRowNum; row++) {
		for (int nCol = 0; nCol < columnCount_; nCol++) {

			// Get column style & item state
			nColStyle = gridCtrlFormatInfoPtr_[nCol].columnStyle;
			itemState = hotkeySetTablePtr_->GetItemState(row, nCol);
			itemState |= GVIS_READONLY;

			// Base column - header-like style
			if (nColStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(hotkeySetTablePtr_, row, nCol);
			}

			// Checkbox column
			else if (nColStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!hotkeySetTablePtr_->SetCellType(row, nCol, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* cellPtr = (CGridCellCheck*)hotkeySetTablePtr_->GetCell(row, nCol);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[nCol].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (nColStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!hotkeySetTablePtr_->SetItemState(row, nCol, itemState))
					continue;

				// Get cell
				CGridCellBase* cellPtr = (CGridCellBase*)hotkeySetTablePtr_->GetCell(row, nCol);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[nCol].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetFormat(cellPtr->GetFormat() | DT_CENTER);
				}
				else {
					// Set margin (left alignment)
					if (cellPtr == NULL) continue;
					cellPtr->SetMargin(Constant::UI::GridCtrl::Margin::Left);
				}
			}
		}
	}
}

/**
 * @brief	Setup data for combo-boxes
 * @param	comboId	- ID of combo box
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CHotkeySetDlg::setupComboBox(unsigned comboId, LANGTABLE_PTR languageTablePtr)
{
	switch (comboId)
	{
	case IDC_HOTKEYSET_ACTION_LIST:
		actionListCombo_.ResetContent();
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_DISPLAYOFF));		// Turn off display
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SLEEP));			// Sleep
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SHUTDOWN));		// Shutdown
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_RESTART));			// Restart
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_SIGNOUT));			// Log out
		actionListCombo_.AddString(getLanguageString(languageTablePtr, COMBOBOX_ACTION_HIBERNATE));		// Hibernate
		break;

	case IDC_HOTKEYSET_FUNCKEY_LIST:
		virtualKeyListCombo_.ResetContent();
		for (int index = 0; index < TABLE_SIZE(StringTable::FunctionKeys); index++)
			virtualKeyListCombo_.AddString(StringTable::FunctionKeys[index].langString);
		break;

	default:
		break;
	}

	// Default
	SDialog::setupComboBox(comboId, languageTablePtr);
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	isRecheckState - Recheck all item's state
 * @return	None
 */
void CHotkeySetDlg::refreshDialogItemState(bool isRecheckState /* = false */)
{
	// Enable/disable buttons if data is all empty or not
	bool bIsAllEmpty = tempHotkeySetData_.isAllEmpty();
	enableItem(IDC_HOTKEYSET_REMOVE_BTN, !bIsAllEmpty);
	enableItem(IDC_HOTKEYSET_REMOVEALL_BTN, !bIsAllEmpty);

	// Enable/disable buttons if data changed or not
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	enableItem(IDC_HOTKEYSET_APPLY_BTN, isChanged);

	// Update "Check All" button state
	updateCheckAllBtnState();

	// Default
	SDialog::refreshDialogItemState(isRecheckState);
}

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	isRecheck - Flag to recheck data item list
 * @return	None
 */
void CHotkeySetDlg::updateCheckAllBtnState(bool isRecheck /* = true */)
{
	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Get buttons
	CWnd* pCheckAllBtn = GetDlgItem(IDC_HOTKEYSET_CHECKALL_BTN);
	CWnd* pUncheckAllBtn = GetDlgItem(IDC_HOTKEYSET_UNCHECKALL_BTN);
	if ((pCheckAllBtn == NULL) || (pUncheckAllBtn == NULL))
		return;

	// Get number of items
	int itemNum = GetItemNum();
	if (itemNum == 0) {
		// Disable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(false);
		pUncheckAllBtn->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (isRecheck == true) {

		// Reset counter
		checkCount_ = 0;
		// Check for item states
		for (int index = 0; index < itemNum; index++) {
			const Item& tempHotkeyItem = tempHotkeySetData_.getItemAt(index);
			if (tempHotkeyItem.isEnabled() == true) {
				// Increase counter
				checkCount_++;
			}
		}
	}

	// Update button state
	if (checkCount_ == 0) {
		// Enable [Check All] button
		pCheckAllBtn->EnableWindow(true);
		// Disable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(false);
	}
	else if (checkCount_ == itemNum) {
		// Disable [Check All] button
		pCheckAllBtn->EnableWindow(false);
		// Enable [Uncheck All] button
		pUncheckAllBtn->EnableWindow(true);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		pCheckAllBtn->EnableWindow(true);
		pUncheckAllBtn->EnableWindow(true);
	}
}

/**
 * @brief	Update HotkeySet list
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::updateHotkeySet()
{
	// Check table validity
	if (hotkeySetTablePtr_ == NULL) return;

	// If there's no item, do nothing
	int itemNum = GetItemNum();
	if (itemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Print items
	String tempString;
	int nTemp = -1;
	int itemIndex = 0;
	CGridCellCheck* cellCheckPtr = NULL;
	for (int rowIndex = startRowIndex; rowIndex <= itemNum; rowIndex++) {

		// Get item
		itemIndex = rowIndex - startRowIndex;
		const Item& hotkeyItem = tempHotkeySetData_.getItemAt(itemIndex);

		// Enable state
		cellCheckPtr = (CGridCellCheck*)hotkeySetTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(hotkeyItem.isEnabled());
		}

		// Hotkey action
		nTemp = GetPairedID(IDTable::ActionName, GetPairedID(IDTable::HKActionID, hotkeyItem.getActionId()));
		tempString = getLanguageString(languageTablePtr, nTemp);
		hotkeySetTablePtr_->SetItemText(rowIndex, ColumnID::HKActionID, tempString);

		// Keystrokes
		hotkeyItem.printKeyStrokes(tempString);
		if (IS_NULL_STRING(tempString)) {
			// Undefined keystrokes
			tempString = getLanguageString(languageTablePtr, HKEYSET_KEYSTROKES_NULL);
		}
		hotkeySetTablePtr_->SetItemText(rowIndex, ColumnID::Keystrokes, tempString);
	}
}

/**
 * @brief	Disable mouse click events for HotkeySet data table
 * @param	isDisabled - Disable/enable
 * @return	None
 */
void CHotkeySetDlg::disableHotkeySetTable(bool isDisabled)
{
	// Redraw read-only style
	redrawHotkeySetTable(isDisabled);

	// Check table validity
	if (hotkeySetTablePtr_ == NULL) return;

	// Disable/enable mouse events
	hotkeySetTablePtr_->DisableMouseClick(isDisabled);
	hotkeySetTablePtr_->DisableMouseMove(isDisabled);
}

/**
 * @brief	Update and redraw HotkeySet data table
 * @param	bool isReadOnly - Read-only mode
 * @return	None
 */
void CHotkeySetDlg::redrawHotkeySetTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (hotkeySetTablePtr_ == NULL) return;

	// Update new row number
	int nCurRowNum = (GetItemNum() + fixedRowNum);
	hotkeySetTablePtr_->SetRowCount(nCurRowNum);

	// Draw table
	drawHotkeySetTable(isReadOnly);

	// Update table data
	updateHotkeySet();

	// Trigger redrawing table
	hotkeySetTablePtr_->RedrawWindow();
}

/**
 * @brief	Display details of an item at specified index
 * @param	index - Index of item to display
 * @return	None
 */
void CHotkeySetDlg::displayHotkeyDetails(int index)
{
	// Reset default control states
	isCtrlKeyActive_ = false;
	isAltKeyActive_ = false;
	isWinKeyActive_ = false;

	// If index is invalid, display dummy details
	if ((index < 0) || (index >= tempHotkeySetData_.getItemNum())) {
		actionListCombo_.SetWindowText(_T("---"));
		virtualKeyListCombo_.SetWindowText(_T("---"));
		UpdateData(false);
		return;
	}

	// Get item at index
		const Item& hksCurItem = tempHotkeySetData_.getItemAt(index);

	// Get item keycode
	DWORD modifiers, virtualKey;
	hksCurItem.getKeyCode(modifiers, virtualKey);

	// Update checkboxes
	isCtrlKeyActive_ = false;
	if (modifiers & MOD_CONTROL)		isCtrlKeyActive_ = true;
	isAltKeyActive_ = false;
	if (modifiers & MOD_ALT)			isAltKeyActive_ = true;
	isWinKeyActive_ = false;
	if (modifiers & MOD_WIN)			isWinKeyActive_ = true;

	// Update combo-boxes
	unsigned actionId = GetPairedID(IDTable::HKActionID, hksCurItem.getActionId());
	actionListCombo_.SetCurSel(AppCore::opt2Sel(APP_ACTION, actionId));
	virtualKeyListCombo_.SetWindowText(_T("---"));
	if (virtualKey > 0)
		virtualKeyListCombo_.SetCurSel(virtualKey - VK_F1);

	UpdateData(false);
}

/**
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::updateLayoutInfo(void)
{
	// Check table validity
	if (hotkeySetTablePtr_ == NULL) return;

	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Get table column count
	int nColNum = hotkeySetTablePtr_->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < nColNum; index++) {
		int nColSize = hotkeySetTablePtr_->GetColumnWidth(index);
		gridCtrlFormatInfoPtr_[index].width = nColSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::loadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT arrGrdColFormat[] = {
	//-----------ID------------------------Header title ID---------------Width(px)----Column style-------Align Center---
		{	ColumnID::EnableState,		GRIDCOLUMN_HOTKEYSET_STATE,			28,		COLSTYLE_CHECKBOX,		true,	},
		{	ColumnID::HKActionID,		GRIDCOLUMN_HOTKEYSET_HKACTIONID,	122,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::Keystrokes,		GRIDCOLUMN_HOTKEYSET_KEYSTROKES,	-1,		COLSTYLE_NORMAL,		true,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	columnCount_ = (sizeof(arrGrdColFormat) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (gridCtrlFormatInfoPtr_ == NULL) {
		gridCtrlFormatInfoPtr_ = new GRIDCTRLCOLFORMAT[columnCount_];
		for (int index = 0; index < columnCount_; index++) {
			// Copy default table column format data
			gridCtrlFormatInfoPtr_[index] = arrGrdColFormat[index];
		}
	}

	// Load layout info data from registry
	int returnValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		keyName = Key::LayoutInfo::GridColSize(index);
		if (GetLayoutInfo(Section::LayoutInfo::HKeySetTable, keyName, returnValue)) {
			if (gridCtrlFormatInfoPtr_ != NULL) {
				gridCtrlFormatInfoPtr_[index].width = returnValue;
			}
		}
	}
}

/**
 * @brief	Save layout info data
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::saveLayoutInfo(void)
{
	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Save layout info data to registry
	int nRef = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		nRef = gridCtrlFormatInfoPtr_[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		WriteLayoutInfo(Section::LayoutInfo::HKeySetTable, keyName, nRef);
	}
}

/**
 * @brief	Load hotkeyset data
 * @param	None
 * @return	bool - Result of loading process
 */
bool CHotkeySetDlg::loadHotkeySetData()
{
	// Get app hotkeyset data pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	HotkeySetData* hotkeySetDataPtr = theAppPtr->getAppHotkeySetData();
	if (hotkeySetDataPtr == NULL)
		return false;

	// Copy data
	hotkeySetData_.copy(*hotkeySetDataPtr);
	tempHotkeySetData_.copy(hotkeySetData_);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);
	return true;
}

/**
 * @brief	Save hotkeyset data
 * @param	None
 * @return	bool - Result of saving process
 */
bool CHotkeySetDlg::saveHotkeySetData()
{
	// Copy data and adjust validity
	hotkeySetData_.copy(tempHotkeySetData_);
	hotkeySetData_.adjust();

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	// Save app HotkeySet data
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	theAppPtr->setAppHotkeySetData(&hotkeySetData_);
	theAppPtr->saveRegistryAppData(APPDATA_HOTKEYSET);

	// Notify main dialog to re-update HotkeySet data
	CPowerPlusDlg* mainDialogPtr = (CPowerPlusDlg*)(theAppPtr->GetMainWnd());
	VERIFY(mainDialogPtr != NULL);
	if (mainDialogPtr == NULL) return false;
	mainDialogPtr->PostMessage(SM_APP_UPDATE_HOTKEYSETDATA, NULL, NULL);

	return true;
}

/**
 * @brief	Check if settings changed
 * @param	None
 * @return	None
 */
bool CHotkeySetDlg::checkDataChangeState()
{
	bool changeFlag = false;

	// Update enable state of all item before checking
	int itemIndex = 0;
	CGridCellCheck* pCellCheckEnable = NULL;
	for (int rowIndex = startRowIndex; rowIndex <= GetItemNum(); rowIndex++) {
		// Get checkbox cell
		pCellCheckEnable = (CGridCellCheck*)hotkeySetTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		if (pCellCheckEnable == NULL) continue;

		// Get checked states
		bool isEnabled = pCellCheckEnable->GetCheck();

		// Update item checked state
		itemIndex = rowIndex - startRowIndex;
		Item& hksTempItem = tempHotkeySetData_.getItemAt(itemIndex);
		hksTempItem.enableItem(isEnabled);
	}

	// Check if number of items changed
	changeFlag |= (tempHotkeySetData_.getItemNum() != hotkeySetData_.getItemNum());
	if (changeFlag == true)
		return changeFlag;

	// Check if each item's data changed
	for (int index = 0; index < tempHotkeySetData_.getItemNum(); index++) {
		// Get current item and temp item
		const Item& hksCurItem = hotkeySetData_.getItemAt(index);
		const Item& hksTempItem = tempHotkeySetData_.getItemAt(index);
 
		// Data comparison
		changeFlag |= (hksTempItem.isEnabled() != hksCurItem.isEnabled());
		changeFlag |= (hksTempItem.compare(hksCurItem) != true);

		// Stop on the first different item encountered
		if (changeFlag == true) break;
	}

	return changeFlag;
}

/**
 * @brief	Add current settings to HotkeySet data list
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::add(void)
{
	// Update data
	UpdateData(true);

	// Create temp hotkeyset item
	Item tempHotkeyItem = {0};

	// Keycode
	DWORD modifiers = 0, virtualKey = 0;

	// Update modifier keys
	if (isCtrlKeyActive_ == true)		modifiers |= MOD_CONTROL;
	if (isAltKeyActive_ == true)		modifiers |= MOD_ALT;
	if (isWinKeyActive_ == true)	modifiers |= MOD_WIN;

	// Update action ID
	int currenSelection = actionListCombo_.GetCurSel();
	int hotkeyActionId = IDTable::HKActionID[currenSelection].first;
	tempHotkeyItem.setActionId(hotkeyActionId);

	// Update virtual key code
	currenSelection = virtualKeyListCombo_.GetCurSel();
	virtualKey = (currenSelection + VK_F1);

	// Update item hotkey code
	tempHotkeyItem.setKeyCode(modifiers, virtualKey);

	// Check data validity
	bool bValid = validate(tempHotkeyItem, true);
	if (bValid == false)
		return;

	// Update item to HotkeySet data list
	tempHotkeySetData_.update(tempHotkeyItem);

	// Refresh button state
	refreshDialogItemState();
}

/**
 * @brief	Remove a HotkeySet item by index
 * @param	index - Index of item to remove
 * @return	None
 */
void CHotkeySetDlg::remove(int index)
{
	// Remove item at index
	tempHotkeySetData_.remove(index);

	// Refresh button state
	refreshDialogItemState();
}

/**
 * @brief	Remove all HotkeySet items
 * @param	None
 * @return	None
 */
void CHotkeySetDlg::removeAll(void)
{
	// Remove all items
	tempHotkeySetData_.removeAll();

	// Refresh button state
	refreshDialogItemState();
}

/**
 * @brief	Check/uncheck all HotkeySet items
 * @param	state - New state
 * @return	None
 */
void CHotkeySetDlg::switchAllItemState(bool state)
{
	// Check/uncheck all --> Update all item enable state
	int itemNum = tempHotkeySetData_.getItemNum();
	for (int index = 0; index < itemNum; index++) {
		Item& tempHotkeyItem = tempHotkeySetData_.getItemAt(index);
		if (tempHotkeyItem.isEnabled() != state) {
			tempHotkeyItem.enableItem(state);
		}
	}

	// Update hotkeyset list
	updateHotkeySet();

	// Refresh button state
	refreshDialogItemState();
}

/**
 * @brief	Check hotkeyset item validity
 * @param	hotkeyItem  - Item to validate
 * @param	showMsg - Show validation message box or not
 * @return	bool - Result of validation process
 */
bool CHotkeySetDlg::validate(const Item& hotkeyItem, bool showMsg /* = false */)
{
	bool result = true;
	int messageStringId;
	StringArray arrMsgString;
	arrMsgString.clear();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check action ID
	if ((hotkeyItem.getActionId() < HKID::displayOff) || (hotkeyItem.getActionId() > HKID::hibernate)) {
		messageStringId = MSGBOX_HOTKEYSET_INVALIDITEM_ACTIONID;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;
	}

	// Get item keycode
	DWORD modifiers, virtualKey;
	hotkeyItem.getKeyCode(modifiers, virtualKey);

	// Validate modifier keys
	if ((modifiers <= 0) ||
		((modifiers & MOD_CONTROL) == false) && ((modifiers & MOD_ALT) == false) && ((modifiers & MOD_WIN) == false)) {
		messageStringId = MSGBOX_HOTKEYSET_INVALIDITEM_CTRLKEY;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;
	}

	// Validate virtual key code
	if ((virtualKey < VK_F1) || (virtualKey > VK_F12)) {
		messageStringId = MSGBOX_HOTKEYSET_INVALIDITEM_FUNCKEY;
		arrMsgString.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;
	}

	// Check if system hotkey existed
	int nExistedSysHotkeyNum = TABLE_SIZE(OtherTable::ExistedSysHotkeyList);
	for (int index = 0; index < nExistedSysHotkeyNum; index++) {
		if ((modifiers == OtherTable::ExistedSysHotkeyList[index].modifiers) &&
			(virtualKey == OtherTable::ExistedSysHotkeyList[index].virtualKey)) {
			// Hotkey info format
			String keyStrokesString = Constant::String::Empty;
			if (modifiers & MOD_CONTROL)	keyStrokesString += _T("Ctrl + ");
			if (modifiers & MOD_ALT)		keyStrokesString += _T("Alt + ");
			if (modifiers & MOD_WIN)		keyStrokesString += _T("Win + ");
			keyStrokesString += GetString(StringTable::FunctionKeys, virtualKey);
			String keyInfoString = Constant::String::Empty;
			keyInfoString.format(_T("%s - %s"), keyStrokesString.getString(), getLanguageString(languageTablePtr, OtherTable::ExistedSysHotkeyList[index].hotkeyDescription));

			// Message format
			String messageFormat;
			messageFormat.format(getLanguageString(languageTablePtr, MSGBOX_HOTKEYSET_EXISTED_HOTKEY), keyInfoString.getString());

			arrMsgString.push_back(messageFormat);
			result = false;
		}
	}
	
	// Show error message if enabled
	if ((showMsg == true) && (!arrMsgString.empty())) {
		for (int index = 0; index < arrMsgString.size(); index++) {
			displayMessageBox(arrMsgString.at(index), NULL, MB_OK | MB_ICONERROR);
		}
	}

	// Remove all message after displaying
	arrMsgString.clear();
	return result;
}
