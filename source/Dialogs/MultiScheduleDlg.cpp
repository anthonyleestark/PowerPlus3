/**
 * @file		MultiScheduleDlg.cpp
 * @brief		Source file for Schedule dialog
 * @author		AnthonyLeeStark
 * @date		2017.03.08
 * 
 * @copyright 	Copyright (c) 2015-2025 AnthonyLeeStark
 */

#include "MainApp/PowerPlus.h"
#include "MainApp/PowerPlusDlg.h"
#include "Dialogs/MultiScheduleDlg.h"
#include "Dialogs/EditScheduleDlg.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
using namespace MapTable;
using namespace Language;
using namespace AppCore;
using namespace AppRegistry;
 
 
// Data list table constants
constexpr const int kFixedColumnNum = 1;
constexpr const int kFixedRowNum = 1;
constexpr const int kDefaultRowIndex = 1;
constexpr const int kExtraStartRowIndex = 2;

// Implement methods for CMultiScheduleDlg
IMPLEMENT_DYNAMIC(CMultiScheduleDlg, SDialog)


/**
 * @brief	Constructor
 */
CMultiScheduleDlg::CMultiScheduleDlg() : SDialog(IDD_MULTISCHEDULE_DLG)
{
	// Initialize member variables

	// Dialog control item
	scheduleDataTablePtr_ = NULL;

	// Child dialog
	editScheduleDlgPtr_ = NULL;

	// Data container variables
	scheduleData_.init();
	tempScheduleData_.init();

	// Table format and properties
	columnCount_ = 0;
	gridCtrlFormatInfoPtr_ = NULL;
	dataTableSizePtr_ = NULL;

	// Other variables
	currentMode_ = 0;
	checkCount_ = 0;
	curSelIndex_ = -1;
	m_nCurDispIndex = -2;
}

/**
 * @brief	Destructor
 */
CMultiScheduleDlg::~CMultiScheduleDlg()
{
	// Delete child dialog
	if (editScheduleDlgPtr_ != NULL) {
		// Destroy dialog
		editScheduleDlgPtr_->DestroyWindow();
		delete editScheduleDlgPtr_;
		editScheduleDlgPtr_ = NULL;
	}

	// Grid table list control
	if (scheduleDataTablePtr_) {
		delete scheduleDataTablePtr_;
		scheduleDataTablePtr_ = NULL;
	}

	// Remove schedule data
	scheduleData_.deleteAll();
	tempScheduleData_.deleteAll();

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
void CMultiScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
	SDialog::DoDataExchange(pDX);
}

/**
 * @brief	Register dialog control management
 * @param	None
 * @return	int
 */
int CMultiScheduleDlg::registerDialogManagement(void)
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
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_ITEM_LISTBOX, List_Control);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_ADD_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_EDIT_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_REMOVE_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_REMOVEALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_CHECKALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_UNCHECKALL_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_VIEWDETAILS_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_SETDEFAULT_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_APPLY_BTN, Button);
		returnValue = pCtrlMan->addControl(IDC_MULTISCHEDULE_CANCEL_BTN, Button);
	}

	return returnValue;
}

/**
 * @brief	Unregister dialog control management
 * @param	None
 * @return	true/false
 */
bool CMultiScheduleDlg::unregisterDialogManagement(void)
{
	// Get control manager
	SControlManager* pCtrlMan = this->getControlManager();

	// Remove dialog controls from managements
	if (pCtrlMan != NULL) {
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_ITEM_LISTBOX);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_ADD_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_EDIT_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_REMOVE_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_REMOVEALL_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_CHECKALL_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_UNCHECKALL_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_VIEWDETAILS_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_SETDEFAULT_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_APPLY_BTN);
		pCtrlMan->removeControl(IDC_MULTISCHEDULE_CANCEL_BTN);
	}

	return SDialog::unregisterDialogManagement();
}


// CMultiScheduleDlg dialog items ID map
BEGIN_RESOURCEID_MAP(CMultiScheduleDlg)
	ON_ID_DIALOG(IDD_MULTISCHEDULE_DLG,				 "ScheduleDlg")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_ITEM_LISTBOX,	 "ScheduleItemList")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_ADD_BTN,		 "AddButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_EDIT_BTN,		 "EditButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_REMOVE_BTN,		 "RemoveButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_REMOVEALL_BTN,	 "RemoveAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_CHECKALL_BTN,	 "CheckAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_UNCHECKALL_BTN,	 "UncheckAllButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_VIEWDETAILS_BTN, "ViewDetailsButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_SETDEFAULT_BTN,	 "SetDefaultButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_APPLY_BTN,		 "SaveButton")
	ON_ID_CONTROL(IDC_MULTISCHEDULE_CANCEL_BTN,		 "CancelButton")
END_RESOURCEID_MAP()


// CMultiScheduleDlg dialog message map
BEGIN_MESSAGE_MAP(CMultiScheduleDlg, SDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_MULTISCHEDULE_APPLY_BTN,					&CMultiScheduleDlg::OnApply)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_CANCEL_BTN,					&CMultiScheduleDlg::OnExit)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_ADD_BTN,					&CMultiScheduleDlg::OnAdd)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_EDIT_BTN,					&CMultiScheduleDlg::OnEdit)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_REMOVE_BTN,					&CMultiScheduleDlg::OnRemove)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_REMOVEALL_BTN,				&CMultiScheduleDlg::OnRemoveAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_CHECKALL_BTN,				&CMultiScheduleDlg::OnCheckAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_UNCHECKALL_BTN,				&CMultiScheduleDlg::OnUncheckAll)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_VIEWDETAILS_BTN,			&CMultiScheduleDlg::OnViewDetails)
	ON_BN_CLICKED(IDC_MULTISCHEDULE_SETDEFAULT_BTN,				&CMultiScheduleDlg::OnSetDefault)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_MULTISCHEDULE_ITEM_LISTBOX,	&CMultiScheduleDlg::OnSelectScheduleItem)
	ON_NOTIFY(NM_CLICK, IDC_MULTISCHEDULE_ITEM_LISTBOX,			&CMultiScheduleDlg::OnClickDataItemList)
	ON_NOTIFY(NM_RCLICK, IDC_MULTISCHEDULE_ITEM_LISTBOX,		&CMultiScheduleDlg::OnRightClickDataItemList)
	ON_MESSAGE(SCM_NOTIFY_DIALOG_DESTROY,						&CMultiScheduleDlg::OnChildDialogDestroy)
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//// Implementation

// CMultiScheduleDlg message handlers

/**
 * @brief	Initialize Schedule dialog and setup dialog content
 * @param	None
 * @return	BOOL - Default
 */
BOOL CMultiScheduleDlg::OnInitDialog()
{
	// First, initialize base dialog class
	SDialog::OnInitDialog();

	// Do not use Enter button
	setUseEnter(false);

	// Register message box caption
	registerMessageBoxCaption(MSGBOX_MULTISCHEDULE_CAPTION);

	// Load data
	loadScheduleSettings();

	// Init dialog items
	loadLayoutInfo();
	setupLanguage();
	setupDialogItemState();

	// Update data
	updateDataItemList();
	refreshDialogItemState(true);

	// Save dialog event log if enabled
	outputEventLog(LOG_EVENT_DLG_INIT, this->getCaption());

	return true;
}

/**
 * @brief	Default method for dialog closing
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnClose()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// If data changed, ask for saving before closing dialog
		if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Save data
				saveScheduleSettings();
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
void CMultiScheduleDlg::OnDestroy()
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
 * @brief	Request current dialog to close
 * @param	None
 * @return	LRESULT (0:Success, else:Failed)
 */
LRESULT CMultiScheduleDlg::requestCloseDialog(void)
{
	// If detail edit dialog is opening
	if (editScheduleDlgPtr_ != NULL) {
		// Request close dialog
		LRESULT resCloseReq = editScheduleDlgPtr_->requestCloseDialog();
		if (resCloseReq != Result::Success)
			return resCloseReq;
	}

	// If data changed, ask for saving before closing dialog
	if (getFlagValue(AppFlagID::dialogDataChanged) == true) {
		// Setup messagebox language
		LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
		const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
		const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

		int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNOCANCEL | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Save data
			saveScheduleSettings();
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
void CMultiScheduleDlg::setupLanguage()
{
	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Setup dialog title
	this->setCaptionFromLanguage(getDialogId());

	// Loop through all dialog items and setup languages for each one of them
	for (CWnd* childWndPtr = GetTopWindow(); childWndPtr != NULL; childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT))
	{
		// Get item ID
		unsigned id = childWndPtr->GetDlgCtrlID();

		switch (id)
		{
		case IDC_MULTISCHEDULE_ITEM_LISTBOX:
			// Skip these items
			break;
		default:
			setControlText(childWndPtr, id, languageTablePtr);
			break;
		}
	}

	// Setup Schedule data item list
	setupDataItemList(languageTablePtr);

	// Default
	SDialog::setupLanguage();
}

/**
 * @brief	Initialize and setup language for Schedule item list
 * @param	languageTablePtr - Language package pointer
 * @return	None
 */
void CMultiScheduleDlg::setupDataItemList(LANGTABLE_PTR /*languageTablePtr*/)
{
	// Get parent list frame rect
	CWnd* listFrameWndPtr = GetDlgItem(IDC_MULTISCHEDULE_ITEM_LISTBOX);
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
	VERIFY_INITIALIZATION(scheduleDataTablePtr_, CGridCtrl)

	// Create table
	if (scheduleDataTablePtr_ == NULL) return;
	DWORD style = WS_CHILD | WS_VISIBLE | WS_TABSTOP;
	scheduleDataTablePtr_->Create(listFrameWndRect, this, IDC_MULTISCHEDULE_ITEM_LISTBOX, style);

	// Destroy frame
	listFrameWndPtr->DestroyWindow();

	// Cell format
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)scheduleDataTablePtr_->GetDefaultCell(false, false);
	if (cellPtr == NULL) return;
	cellPtr->SetFormat(cellPtr->GetFormat());
	cellPtr->SetMargin(0);
	cellPtr->SetBackClr(Color::White);
	cellPtr->SetTextClr(Color::Black);
	cellPtr->SetHeight(Constant::UI::GridCtrl::Height::Row_Ex);

	// Table format and properties
	int rowCount = (getTotalItemNum() + kFixedRowNum);
	int colCount = columnCount_;

	// Setup table
	scheduleDataTablePtr_->SetColumnCount(colCount);
	scheduleDataTablePtr_->SetFixedColumnCount(kFixedColumnNum);
	scheduleDataTablePtr_->SetRowCount(rowCount);
	scheduleDataTablePtr_->SetFixedRowCount(kFixedRowNum);
	scheduleDataTablePtr_->SetRowHeight(Constant::UI::GridCtrl::Index::Header_Row, Constant::UI::GridCtrl::Height::Header);

	// Draw table
	drawDataTable(isReadOnlyMode());

	// Update layout info
	updateLayoutInfo();

	// Display table
	scheduleDataTablePtr_->SetListMode(true);
	scheduleDataTablePtr_->SetEditable(false);
	scheduleDataTablePtr_->SetRowResize(false);
	scheduleDataTablePtr_->EnableSelection(true);
	scheduleDataTablePtr_->SetSingleRowSelection(true);
	scheduleDataTablePtr_->SetSingleColSelection(false);
	scheduleDataTablePtr_->SetFixedRowSelection(false);
	scheduleDataTablePtr_->SetFixedColumnSelection(false);
	scheduleDataTablePtr_->ShowWindow(SW_SHOW);
	scheduleDataTablePtr_->SetRedraw(true);
}

/**
 * @brief	Draw Schedule data list table
 * @param	isReadOnly - Read-only mode
 * @return	None
 */
void CMultiScheduleDlg::drawDataTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (scheduleDataTablePtr_ == NULL) return;

	// Check table format data validity
	if (dataTableSizePtr_ == NULL) return;

	// Get app pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	if (theAppPtr == NULL) return;

	// Get app language package
	LANGTABLE_PTR languageTablePtr = theAppPtr->getAppLanguage();

	// Re-update default cell properties
	CGridDefaultCell* cellPtr = (CGridDefaultCell*)scheduleDataTablePtr_->GetDefaultCell(false, false);
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
	int colCount = columnCount_;
	int rowCount = (getTotalItemNum() + kFixedRowNum);

	// Setup display size
	int frameHeight = dataTableSizePtr_->height();
	int frameWidth = dataTableSizePtr_->width();
	if (AppCore::getWindowsOSVersion() == WINDOWS_VERSION_10) {
		// Windows 10 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl_Win10;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL_WIN10;
	}
	else {
		// Windows 11 list control offset
		frameWidth -= Constant::UI::Offset::Width::ListCtrl;
		//frameHeight -= OFFSET_HEIGHT_LISTCTRL;
	}
	if ((Constant::UI::GridCtrl::Height::Header + ((rowCount - 1) * Constant::UI::GridCtrl::Height::Row_Ex)) >= frameHeight) {
		// Fix table width in case vertical scrollbar is displayed
		int scrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
		frameWidth -= (scrollBarWidth + Constant::UI::Offset::Width::VScrollBar);
	}

	// Setup columns
	for (int col = 0; col < colCount; col++) {
		// Set header row style
		AppCore::setFixedCellStyle(scheduleDataTablePtr_, Constant::UI::GridCtrl::Index::Header_Row, col);

		// Column header title
		String headerTitle = Constant::String::Empty;
		unsigned headerTitleId = gridCtrlFormatInfoPtr_[col].headerTitleId;
		if (headerTitleId != INT_NULL) {
			headerTitle = getLanguageString(languageTablePtr, headerTitleId);
		}
		scheduleDataTablePtr_->SetItemText(Constant::UI::GridCtrl::Index::Header_Row, col, headerTitle);

		// Column width
		int colWidth = gridCtrlFormatInfoPtr_[col].width;
		if (colWidth != -1) {
			// Set column width as defined
			if (scheduleDataTablePtr_->SetColumnWidth(col, colWidth)) {
				// Calculate remaining width
				frameWidth -= colWidth;
			}
		}
		else {
			// Set remaining width for current column
			scheduleDataTablePtr_->SetColumnWidth(col, frameWidth);
		}
	}

	// Setup rows
	int colStyle = -1;
	unsigned itemState = INT_NULL;
	for (int row = 1; row < rowCount; row++) {
		for (int col = 0; col < columnCount_; col++) {

			// Get column style & item state
			colStyle = gridCtrlFormatInfoPtr_[col].columnStyle;
			itemState = scheduleDataTablePtr_->GetItemState(row, col);
			itemState |= GVIS_READONLY;

			// Highlight default schedule item
			if ((row == kDefaultRowIndex) && (colStyle != COLSTYLE_FIXED) && (isReadOnly != true)) {
				CGridCellBase* pCellBase = scheduleDataTablePtr_->GetCell(row, col);
				if (pCellBase != NULL) {
					pCellBase->SetBackClr(Color::Yellow);
					pCellBase->SetTextClr(Color::Red);
				}
			}

			// Base column - header-like style
			if (colStyle == COLSTYLE_FIXED) {
				// Set fixed cell style
				AppCore::setFixedCellStyle(scheduleDataTablePtr_, row, col);
			}

			// Checkbox column
			else if (colStyle == COLSTYLE_CHECKBOX) {
				// Set cell type: Checkbox
				if (!scheduleDataTablePtr_->SetCellType(row, col, RUNTIME_CLASS(CGridCellCheck)))
					continue;

				// Get cell
				CGridCellCheck* cellPtr = (CGridCellCheck*)scheduleDataTablePtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == true) {
					if (cellPtr == NULL) continue;
					cellPtr->SetCheckPlacement(SCP_CENTERING);
				}
			}

			// Normal column
			else if (colStyle == COLSTYLE_NORMAL) {
				// Set item state
				if (!scheduleDataTablePtr_->SetItemState(row, col, itemState))
					continue;

				// Get cell
				CGridCellBase* cellPtr = (CGridCellBase*)scheduleDataTablePtr_->GetCell(row, col);

				// Set center alignment if defined
				if (gridCtrlFormatInfoPtr_[col].isCentered == true) {
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
 * @brief	Update layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::updateLayoutInfo(void)
{
	// Check table validity
	if (scheduleDataTablePtr_ == NULL) return;

	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Get table column count
	int colCount = scheduleDataTablePtr_->GetColumnCount();

	// Update size of table columns
	for (int index = 0; index < colCount; index++) {
		int colSize = scheduleDataTablePtr_->GetColumnWidth(index);
		gridCtrlFormatInfoPtr_[index].width = colSize;
	}
}

/**
 * @brief	Load layout info data
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::loadLayoutInfo(void)
{
	// Define default table columns format
	const GRIDCTRLCOLFORMAT gridColFormatInfo[] = {
	//-----------ID------------------------Header title ID---------------Width(px)---Column style--------Align Center---
		{	ColumnID::Index,		GRIDCOLUMN_MULTISCHEDULE_INDEX,			26,		COLSTYLE_FIXED,			true,	},
		{	ColumnID::EnableState,	GRIDCOLUMN_MULTISCHEDULE_STATE,			52,		COLSTYLE_CHECKBOX,		true,	},
		{	ColumnID::ActionID,		GRIDCOLUMN_MULTISCHEDULE_ACTIONID,		140,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::TimeValue,	GRIDCOLUMN_MULTISCHEDULE_TIMEVALUE,		135,	COLSTYLE_NORMAL,		true,	},
		{ 	ColumnID::Repeat,		GRIDCOLUMN_MULTISCHEDULE_REPEAT,		-1,		COLSTYLE_CHECKBOX,		true,	},
	//------------------------------------------------------------------------------------------------------------------
	};

	// Backup format data
	columnCount_ = (sizeof(gridColFormatInfo) / sizeof(GRIDCTRLCOLFORMAT));

	// Initialize table format info data
	if (gridCtrlFormatInfoPtr_ == NULL) {
		gridCtrlFormatInfoPtr_ = new GRIDCTRLCOLFORMAT[columnCount_];
		for (int index = 0; index < columnCount_; index++) {
			// Copy default table column format data
			gridCtrlFormatInfoPtr_[index] = gridColFormatInfo[index];
		}
	}

	// Load layout info data from registry
	int returnValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		keyName = Key::LayoutInfo::GridColSize(index);
		if (getLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, returnValue)) {
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
void CMultiScheduleDlg::saveLayoutInfo(void)
{
	// Check table column format data validity
	if (gridCtrlFormatInfoPtr_ == NULL) return;

	// Save layout info data to registry
	int referValue = 0;
	String keyName;
	for (int index = 0; index < columnCount_; index++) {
		referValue = gridCtrlFormatInfoPtr_[index].width;
		keyName = Key::LayoutInfo::GridColSize(index);
		writeLayoutInfo(Section::LayoutInfo::MultiScheduleTable, keyName, referValue);
	}
}

/**
 * @brief	Setup properties and values for dialog items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::setupDialogItemState()
{
	// Read-only mode (if enabled)
	if (isReadOnlyMode() == true) {
		CWnd* childWndPtr = GetTopWindow();
		while (childWndPtr != NULL) {
			childWndPtr->EnableWindow(false);
			childWndPtr = childWndPtr->GetWindow(GW_HWNDNEXT);
		}
	}

	// Default
	SDialog::setupDialogItemState();
}

/**
 * @brief	Update Schedule data item list table
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::updateDataItemList()
{
	// Check table validity
	if (scheduleDataTablePtr_ == NULL) return;

	// If there's no item, do nothing
	int itemNum = getTotalItemNum();
	if (itemNum <= 0)
		return;

	// Load app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Print items
	int tempValue = -1;
	int extraItemIndex = 0;
	CGridCellCheck* cellCheckPtr = NULL;
	for (int rowIndex = kDefaultRowIndex; rowIndex <= itemNum; rowIndex++) {
		
		// Get schedule item
		Item scheduleItem;
		if (rowIndex == kDefaultRowIndex) {
			// Get schedule default item
			scheduleItem = tempScheduleData_.getDefaultItem();
		}
		else {
			// Get schedule extra item
			extraItemIndex = rowIndex - kExtraStartRowIndex;
			scheduleItem = tempScheduleData_.getItemAt(extraItemIndex);
		}

		// If schedule item is empty
		if (scheduleItem.isEmpty()) continue;

		// Item index
		String tempString = StringUtils::stringFormat(_T("%d"), rowIndex);
		scheduleDataTablePtr_->SetItemText(rowIndex, ColumnID::Index, tempString);

		// Enable state
		cellCheckPtr = (CGridCellCheck*)scheduleDataTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(scheduleItem.isEnabled());
		}

		// Action name
		tempValue = getPairedID(IDTable::ActionName, scheduleItem.getAction());
		tempString = getLanguageString(languageTablePtr, tempValue);
		scheduleDataTablePtr_->SetItemText(rowIndex, ColumnID::ActionID, tempString);

		// Time setting
		const wchar_t* formatString = getLanguageString(languageTablePtr, GRIDCOLUMN_MULTISCHEDULE_TIMEFORMAT);
		tempString = ClockTimeUtils::format(languageTablePtr, formatString, scheduleItem.getTime());
		scheduleDataTablePtr_->SetItemText(rowIndex, ColumnID::TimeValue, tempString);

		// Repeat
		cellCheckPtr = (CGridCellCheck*)scheduleDataTablePtr_->GetCell(rowIndex, ColumnID::Repeat);
		if (cellCheckPtr != NULL) {
			cellCheckPtr->SetCheck(scheduleItem.isRepeatEnabled());
		}
	}
}

/**
 * @brief	Disable mouse click events for Schedule data table
 * @param	isDisabled - Disable/enable
 * @return	None
 */
void CMultiScheduleDlg::disableDataTable(bool isDisabled)
{
	// Redraw read-only style
	redrawDataTable(isDisabled);

	// Check table validity
	if (scheduleDataTablePtr_ == NULL) return;

	// Disable/enable mouse events
	scheduleDataTablePtr_->DisableMouseClick(isDisabled);
	scheduleDataTablePtr_->DisableMouseMove(isDisabled);
}

/**
 * @brief	Update and redraw Schedule data table
 * @param	bool isReadOnly - Read-only mode
 * @return	None
 */
void CMultiScheduleDlg::redrawDataTable(bool isReadOnly /* = false */)
{
	// Check table validity
	if (scheduleDataTablePtr_ == NULL) return;

	// Update new row number
	int curRowCount = (getTotalItemNum() + kFixedRowNum);
	scheduleDataTablePtr_->SetRowCount(curRowCount);

	// Draw table
	drawDataTable(isReadOnly);

	// Update table data
	updateDataItemList();

	// Trigger redrawing table
	scheduleDataTablePtr_->RedrawWindow();
}

/**
 * @brief	Refresh and update state for dialog items
 * @param	isRecheckState - Recheck all item's state
 * @return	None
 */
void CMultiScheduleDlg::refreshDialogItemState(bool isRecheckState /* = false */)
{
	CWnd* buttonPtr = NULL;

	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Check if any item is selected or not
	bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getTotalItemNum()));

	// Check if selected item is an extra item or not
	bool isExtraSelected = ((isSelected == true) && ((curSelIndex_ + kFixedRowNum) >= kExtraStartRowIndex));

	// Check if number of extra item has reached the limit
	bool isMaxNum = (getExtraItemNum() >= ScheduleData::kMaxItemNum);

	// Check if data is all empty or not
	bool isAllEmpty = tempScheduleData_.isAllEmpty();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Disable [Add] button if item number has reached the limit
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_ADD_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isMaxNum);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_MULTISCHEDULE_ADD_BTN));
	}

	// Disable [Remove/Remove All] buttons if data is all empty
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_REMOVE_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isAllEmpty);

		// Enable [Remove] button if any item is selected
		buttonPtr->EnableWindow(isSelected);
	}
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_REMOVEALL_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(!isAllEmpty);
	}

	// Enable [Edit] and [View Details] button if any item is selected
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_EDIT_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isSelected);
		buttonPtr->SetWindowText(getLanguageString(languageTablePtr, IDC_MULTISCHEDULE_EDIT_BTN));
	}
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_VIEWDETAILS_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isSelected);
	}

	// Enable [Set Default] button if any extra item is selected
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_SETDEFAULT_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isExtraSelected);
	}

	// Check if data is changed or not
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);

	// Enable [Apply] button if data is changed
	buttonPtr = GetDlgItem(IDC_MULTISCHEDULE_APPLY_BTN);
	if (buttonPtr != NULL) {
		buttonPtr->EnableWindow(isChanged);
	}

	// Update [Check/Uncheck All] button state
	updateCheckAllBtnState(isRecheckState);

	// Default
	SDialog::refreshDialogItemState(isRecheckState);
}

/**
 * @brief	Refresh and update state for [Check/Uncheck All] button
 * @param	isRecheck - Recheck all items enable state
 * @return	None
 */
void CMultiScheduleDlg::updateCheckAllBtnState(bool isRecheck /* = false */)
{
	// If dialog items are being locked, do nothing
	if (getLockState() == true)
		return;

	// Get buttons
	CWnd* checkAllButtonPtr = GetDlgItem(IDC_MULTISCHEDULE_CHECKALL_BTN);
	CWnd* uncheckAllButtonPtr = GetDlgItem(IDC_MULTISCHEDULE_UNCHECKALL_BTN);
	if ((checkAllButtonPtr == NULL) || (uncheckAllButtonPtr == NULL))
		return;

	// Get number of items
	int itemNum = getTotalItemNum();
	if (itemNum == 0) {
		// Disable both [Check/Uncheck All] buttons
		checkAllButtonPtr->EnableWindow(false);
		uncheckAllButtonPtr->EnableWindow(false);
		return;
	}

	// Recheck all items state
	if (isRecheck == true) {

		// Reset counter
		checkCount_ = 0;
		// Check default item
		if (tempScheduleData_.getDefaultItem().isEnabled() == true) {
			// Increase counter
			checkCount_++;
		}
		// Check extra items
		for (int extraIndex = 0; extraIndex < getExtraItemNum(); extraIndex++) {
			const Item& tempItem = tempScheduleData_.getItemAt(extraIndex);
			if (tempItem.isEnabled() == true) {
				// Increase counter
				checkCount_++;
			}
		}
	}

	// Update button state
	if (checkCount_ == 0) {
		// Enable [Check All] button
		checkAllButtonPtr->EnableWindow(true);
		// Disable [Uncheck All] button
		uncheckAllButtonPtr->EnableWindow(false);
	}
	else if (checkCount_ == itemNum) {
		// Disable [Check All] button
		checkAllButtonPtr->EnableWindow(false);
		// Enable [Uncheck All] button
		uncheckAllButtonPtr->EnableWindow(true);
	}
	else {
		// Enable both [Check/Uncheck All] buttons
		checkAllButtonPtr->EnableWindow(true);
		uncheckAllButtonPtr->EnableWindow(true);
	}
}

/**
 * @brief	Load schedule settings from configurations
 * @param	None
 * @return	true/false
 */
bool CMultiScheduleDlg::loadScheduleSettings()
{
	// Get app Schedule data pointer
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	Data* pschData = theAppPtr->getAppScheduleData();
	if (pschData == NULL)
		return false;

	// Copy data
	scheduleData_.copy(*pschData);
	tempScheduleData_.copy(scheduleData_);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	return true;
}

/**
 * @brief	Save schedule settings to configurations
 * @param	None
 * @return	bool - Result of saving process
 */
bool CMultiScheduleDlg::saveScheduleSettings()
{
	// Update settings
	scheduleData_.copy(tempScheduleData_);

	// Save app schedule data
	CPowerPlusApp* theAppPtr = (CPowerPlusApp*)AfxGetApp();
	VERIFY(theAppPtr != NULL);
	if (theAppPtr == NULL) return false;
	theAppPtr->setAppScheduleData(&scheduleData_);
	theAppPtr->saveRegistryAppData(APPDATA_SCHEDULE);

	// Notify main dialog to re-update schedule data
	CPowerPlusDlg* mainDialogPtr = (CPowerPlusDlg*)(theAppPtr->GetMainWnd());
	VERIFY(mainDialogPtr != NULL);
	if (mainDialogPtr == NULL) return false;
	mainDialogPtr->PostMessage(SM_APP_UPDATE_SCHEDULEDATA, NULL, NULL);

	// Reset change flag
	setFlagValue(AppFlagID::dialogDataChanged, false);

	return true;
}

/**
 * @brief	Update and refresh schedule settings
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::updateScheduleSettings()
{
	// Update enable and repeat states of each item
	int extraItemIndex = 0;
	CGridCellCheck* cellCheckEnablePtr = NULL;
	CGridCellCheck* pCellCheckRepeat = NULL;
	int itemRowCount = (scheduleDataTablePtr_->GetRowCount() - kFixedRowNum);
	for (int rowIndex = kDefaultRowIndex; rowIndex <= itemRowCount; rowIndex++) {
		
		// In case of extra item rows
		if (rowIndex > kDefaultRowIndex) {
			// Get extra item index
			extraItemIndex = rowIndex - kExtraStartRowIndex;
		}

		// Get checkbox cells
		cellCheckEnablePtr = (CGridCellCheck*)scheduleDataTablePtr_->GetCell(rowIndex, ColumnID::EnableState);
		pCellCheckRepeat = (CGridCellCheck*)scheduleDataTablePtr_->GetCell(rowIndex, ColumnID::Repeat);
		if ((cellCheckEnablePtr == NULL) || (pCellCheckRepeat == NULL)) continue;

		// Get checked states
		bool isEnabled = cellCheckEnablePtr->GetCheck();
		bool isRepeated = pCellCheckRepeat->GetCheck();

		// Update item enable and repeat states
		if (rowIndex == kDefaultRowIndex) {
			// Update default item data
			Item& tempDefaultItem = tempScheduleData_.getDefaultItem();
			tempDefaultItem.enableItem(isEnabled);
			tempDefaultItem.enableRepeat(isRepeated);
		}
		else {
			// Update extra item data
			Item& tempItem = tempScheduleData_.getItemAt(extraItemIndex);
			tempItem.enableItem(isEnabled);
			tempItem.enableRepeat(isRepeated);
		}
	}
}

/**
 * @brief	Compare control values with current config to check for changes
 * @param	None
 * @return	bool - Flag to mark data changed or not
 */
bool CMultiScheduleDlg::checkDataChangeState()
{
	bool changeFlag = false;

	// Update settings
	updateScheduleSettings();

	// Check if number of extra items changed
	int extraItemNum = tempScheduleData_.getExtraItemNum();
	changeFlag |= (extraItemNum != scheduleData_.getExtraItemNum());
	if (changeFlag == true)
		return changeFlag;

	// Check if default item's data changed
	changeFlag |= (tempScheduleData_.getDefaultItem().compare(scheduleData_.getDefaultItem()) != true);
	if (changeFlag == true)
		return changeFlag;

	// Check if each extra item's data changed
	for (int extraIndex = 0; extraIndex < extraItemNum; extraIndex++) {
		// Get current item and temp item
		const Item& currentItem = scheduleData_.getItemAt(extraIndex);
		const Item& tempItem = tempScheduleData_.getItemAt(extraIndex);

		// Data comparison
		changeFlag |= (tempItem.compare(currentItem) != true);

		// Stop on the first different item encountered
		if (changeFlag == true) break;
	}

	return changeFlag;
}

/**
 * @brief	Add item into schedule data
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::add(Item& scheduleItem)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty())
		return;

	// Check data validity
	bool isValid = validate(scheduleItem, true);
	if (isValid == false)
		return;

	// Update item to Action Schedule data item list
	tempScheduleData_.update(scheduleItem);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Update schedule item
 * @param	scheduleItem - Schedule item
 * @return	None
 */
void CMultiScheduleDlg::update(Item& scheduleItem)
{
	// If item is empty, do nothing
	if (scheduleItem.isEmpty())
		return;

	// Check data validity
	bool isValid = validate(scheduleItem, true);
	if (isValid == false)
		return;

	// Update item to Action Schedule data item list
	tempScheduleData_.update(scheduleItem);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove a schedule item by index
 * @param	index - Index of item to remove
 * @return	None
 */
void CMultiScheduleDlg::remove(int index)
{
	// Remove item at index
	tempScheduleData_.deleteItem(index);

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Remove all schedule items
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::removeAll()
{
	// Remove all items
	tempScheduleData_.deleteExtra();

	// Update table
	redrawDataTable();

	// Refresh button state
	refreshDialogItemState(true);
}

/**
 * @brief	Check/uncheck all Schedule data items
 * @param	state - Item state
 * @return	None
 */
void CMultiScheduleDlg::setAllItemState(bool state)
{
	// Check/uncheck all --> Update all items enable state
	Item& defaultItem = tempScheduleData_.getDefaultItem();
	if (defaultItem.isEnabled() != state) {
		// Change checked state
		defaultItem.enableItem(state);
	}
	for (int extraIndex = 0; extraIndex < getExtraItemNum(); extraIndex++) {
		Item& tempItem = tempScheduleData_.getItemAt(extraIndex);
		if (tempItem.isEnabled() != state) {
			// Change checked state
			tempItem.enableItem(state);
		}
	}

	// Update number of checked items
	checkCount_ = (state == false) ? 0 : getTotalItemNum();

	// Update data item list
	updateDataItemList();

	// Refresh button state
	refreshDialogItemState(false);
}

/**
 * @brief	Check Schedule item validity
 * @param	scheduleItem		 - Item to validate
 * @param	showMsg	 - Show validation message box or not
 * @param	isAutoCorrect - Invalid value auto correction (ON/OFF)
 * @return	bool - Result of validation process
 */
bool CMultiScheduleDlg::validate(Item& scheduleItem, bool showMsg /* = false */, bool isAutoCorrect /* = false */)
{
	bool result = true;

	int messageStringId;
	StringArray messageStringList;
	messageStringList.clear();

	// Get app language package
	LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();

	// Check item ID
	if ((scheduleItem.getItemId() != ScheduleData::kDefaultItemID) &&
		((scheduleItem.getItemId() < ScheduleData::kMinItemID) || (scheduleItem.getItemId() > ScheduleData::kMaxItemID))) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ITEMID;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Get next item ID
			scheduleItem.setItemId(tempScheduleData_.getNextId());
		}
	}

	// Check action ID
	if ((scheduleItem.getAction() < APP_ACTION_NOTHING) || (scheduleItem.getAction() > APP_ACTION_HIBERNATE)) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIONID;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default action
			scheduleItem.setAction(ScheduleData::kDefaultActionID);
		}
	}

	// Check repeat data
	if ((scheduleItem.isRepeatEnabled() == true) && (scheduleItem.getActiveDays() == NULL)) {
		messageStringId = MSGBOX_MULTISCHEDULE_INVALIDITEM_ACTIVEDAYS;
		messageStringList.push_back(getLanguageString(languageTablePtr, messageStringId));
		result = false;

		// Auto correction
		if (isAutoCorrect == true) {
			// Set default data
			scheduleItem.setActiveDays(PwrRepeatSet::kDefaultActiveDays);
		}
	}

	// Show error message if enabled
	if ((showMsg == true) && (!messageStringList.empty())) {
		for (int index = 0; index < messageStringList.size(); index++) {
			// If auto correction is ON
			if (isAutoCorrect == true) {
				// Add "Data will be automatically reset to default"
				String errorMessage = messageStringList.at(index);
				errorMessage += getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_INVALIDITEM_AUTOCORRECT);
				displayMessageBox(errorMessage, NULL, MB_OK | MB_ICONERROR);
			}
			else {
				// Display error message
				displayMessageBox(messageStringList.at(index), NULL, MB_OK | MB_ICONERROR);
			}
		}
	}

	// Remove all message after displaying
	messageStringList.clear();

	return result;
}

/**
 * @brief	Handle clicking event for [Save] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnApply()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_APPLY_BTN);

	// Save data if changed
	bool isChanged = checkDataChangeState();
	setFlagValue(AppFlagID::dialogDataChanged, isChanged);
	if (isChanged == true) {
		saveScheduleSettings();
	}

	// Close dialog
	SDialog::OnOK();
}

/**
 * @brief	Handle clicking event for [Exit] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnExit()
{
	// If not forced closing by request
	if (!isForceClosingByRequest()) {

		// Save app event log if enabled
		outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CANCEL_BTN);

		// If data changed, ask for saving before closing dialog
		bool isChanged = checkDataChangeState();
		setFlagValue(AppFlagID::dialogDataChanged, isChanged);
		if (isChanged == true) {
			// Setup messagebox language
			LANGTABLE_PTR languageTablePtr = ((CPowerPlusApp*)AfxGetApp())->getAppLanguage();
			const wchar_t* messagePrompt = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CONTENT);
			const wchar_t* messageCaption = getLanguageString(languageTablePtr, MSGBOX_MULTISCHEDULE_CHANGED_CAPTION);

			// Show save confirmation message
			int confirm = MessageBox(messagePrompt, messageCaption, MB_YESNO | MB_ICONQUESTION);
			if (confirm == IDYES) {
				// Save data
				saveScheduleSettings();
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
void CMultiScheduleDlg::OnAdd()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_ADD_BTN);

	// Initialize new item template
	Item tempItem;
	tempItem.setItemId(tempScheduleData_.getNextId());
	tempItem.setAction(ScheduleData::kDefaultActionID);

	// Open edit schedule dialog
	if (editScheduleDlgPtr_ == NULL) {
		// Initialize
		editScheduleDlgPtr_ = new CEditScheduleDlg;
		editScheduleDlgPtr_->setParent(this);
		editScheduleDlgPtr_->setScheduleItem(tempItem);
		editScheduleDlgPtr_->setDispMode(Mode::Add);
		editScheduleDlgPtr_->DoModal();
	}
	else {
		// Update dialog
		editScheduleDlgPtr_->setParent(this);
		editScheduleDlgPtr_->setScheduleItem(tempItem);
		editScheduleDlgPtr_->setDispMode(Mode::Add);
		editScheduleDlgPtr_->ShowWindow(SW_SHOW);
	}
}

/**
 * @brief	Handle click event for [Edit] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnEdit()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_EDIT_BTN);

	// Check if any item is selected or not
	bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getTotalItemNum()));

	if (isSelected == true) {

		// Get selected row index
		int selectedRowIndex = curSelIndex_ + kFixedRowNum;

		// Get selected item
		Item scheduleItem;
		if (selectedRowIndex == kDefaultRowIndex) {
			// Get default item
			scheduleItem = tempScheduleData_.getDefaultItem();
		}
		else {
			// Get extra item
			int extraItemIndex = selectedRowIndex - kExtraStartRowIndex;
			scheduleItem = tempScheduleData_.getItemAt(extraItemIndex);
		}

		// If item is empty, do nothing
		if (scheduleItem.isEmpty())
			return;

		// Open edit schedule dialog
		if (editScheduleDlgPtr_ == NULL) {
			// Initialize
			editScheduleDlgPtr_ = new CEditScheduleDlg;
			editScheduleDlgPtr_->setParent(this);
			editScheduleDlgPtr_->setScheduleItem(scheduleItem);
			editScheduleDlgPtr_->setDispMode(Mode::Update);
			editScheduleDlgPtr_->DoModal();
		}
		else {
			// Update dialog
			editScheduleDlgPtr_->setParent(this);
			editScheduleDlgPtr_->setScheduleItem(scheduleItem);
			editScheduleDlgPtr_->setDispMode(Mode::Update);
			editScheduleDlgPtr_->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Handle click event for [Remove] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnRemove()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVE_BTN);

	// If there's no item, do nothing
	int itemNum = getTotalItemNum();
	if (itemNum <= 0)
		return;

	// Get current select item index
	int selectedItemIndex = curSelIndex_;
	int selectedRowIndex = selectedItemIndex + kFixedRowNum;

	// If item at selected index is default item, can not remove
	if (selectedRowIndex == kDefaultRowIndex) {
		displayMessageBox(MSGBOX_MULTISCHEDULE_NOTREMOVE_DEFAULT, NULL, MB_OK | MB_ICONINFORMATION);
		return;
	}

	// In case of extra item rows
	else if (selectedRowIndex > kDefaultRowIndex) {
		// Get extra item index
		selectedItemIndex = selectedRowIndex - kExtraStartRowIndex;
	}

	// If item at selected index is empy, do nothing
	if (tempScheduleData_.isEmpty(selectedItemIndex) == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_MULTISCHEDULE_REMOVE_ITEM, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
		// Remove item
		remove(selectedItemIndex);
	}
}

/**
 * @brief	Handle click event for [Remove All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnRemoveAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_REMOVEALL_BTN);

	// If all item are empty, do nothing
	if (tempScheduleData_.isAllEmpty() == true)
		return;

	// Ask before remove
	int confirm = displayMessageBox(MSGBOX_MULTISCHEDULE_REMOVEALL_ITEMS, NULL, MB_YESNO | MB_ICONQUESTION);
	if (confirm == IDYES) {
		// Remove all items
		removeAll();
	}
}

/**
 * @brief	Handle click event for [Check All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnCheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_CHECKALL_BTN);

	// If all item are empty, do nothing
	if (tempScheduleData_.isAllEmpty() == true)
		return;

	// Check all items
	setAllItemState(true);
}

/**
 * @brief	Handle click event for [Uncheck All] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnUncheckAll()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_UNCHECKALL_BTN);

	// If all item are empty, do nothing
	if (tempScheduleData_.isAllEmpty() == true)
		return;

	// Uncheck all items
	setAllItemState(false);
}

/**
 * @brief	Handle click event for [View Details] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnViewDetails()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_VIEWDETAILS_BTN);

	// Check if any item is selected or not
	bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getTotalItemNum()));

	if (isSelected == true) {

		// Get selected row index
		int selectedRowIndex = curSelIndex_ + kFixedRowNum;

		// Get selected item
		Item scheduleItem;
		if (selectedRowIndex == kDefaultRowIndex) {
			// Get default item
			scheduleItem = tempScheduleData_.getDefaultItem();
		}
		else {
			// Get extra item
			int extraItemIndex = selectedRowIndex - kExtraStartRowIndex;
			scheduleItem = tempScheduleData_.getItemAt(extraItemIndex);
		}

		// If item is empty, do nothing
		if (scheduleItem.isEmpty())
			return;

		// Open edit schedule dialog
		if (editScheduleDlgPtr_ == NULL) {
			// Initialize
			editScheduleDlgPtr_ = new CEditScheduleDlg;
			editScheduleDlgPtr_->setParent(this);
			editScheduleDlgPtr_->setScheduleItem(scheduleItem);
			editScheduleDlgPtr_->setDispMode(Mode::View);
			editScheduleDlgPtr_->DoModal();
		}
		else {
			// Update dialog
			editScheduleDlgPtr_->setParent(this);
			editScheduleDlgPtr_->setScheduleItem(scheduleItem);
			editScheduleDlgPtr_->setDispMode(Mode::View);
			editScheduleDlgPtr_->ShowWindow(SW_SHOW);
		}
	}
}

/**
 * @brief	Handle click event for [Set Default] button
 * @param	None
 * @return	None
 */
void CMultiScheduleDlg::OnSetDefault()
{
	// Save app event log if enabled
	outputButtonLog(LOG_EVENT_BTN_CLICKED, IDC_MULTISCHEDULE_SETDEFAULT_BTN);

	// Check if any item is selected or not
	bool isSelected = ((curSelIndex_ >= 0) && (curSelIndex_ < getTotalItemNum()));
	if (isSelected != true)
		return;

	// Check if selected item is an extra item or not
	int selectedRowIndex = curSelIndex_ + kFixedRowNum;
	bool isExtraSelected = ((isSelected == true) && (selectedRowIndex >= kExtraStartRowIndex));

	if (isExtraSelected == true) {

		// Display confirmation message
		int confirm = displayMessageBox(MSGBOX_MULTISCHEDULE_CONFIRM_SETDEFAULT, NULL, MB_YESNO | MB_ICONQUESTION);
		if (confirm == IDYES) {
			// Check if currently selected item is empty
			int extraItemIndex = selectedRowIndex - kExtraStartRowIndex;
			const Item& curSelItem = tempScheduleData_.getItemAt(extraItemIndex);
			if (curSelItem.isEmpty())
				return;
			
			// Overwrite default item data with currently selected extra item data
			Item& defaultScheduleItem = tempScheduleData_.getDefaultItem();
			defaultScheduleItem.copy(curSelItem);
			defaultScheduleItem.setItemId(ScheduleData::kDefaultItemID);

			// Remove the selected extra item after changing
			tempScheduleData_.deleteItem(extraItemIndex);

			// Update table
			redrawDataTable();

			// Refresh button state
			refreshDialogItemState(true);
		}
	}
}

/**
 * @brief	Show details when selecting a schedule item
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CMultiScheduleDlg::OnSelectScheduleItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int row = reminderItem->iRow;

	//Get current selection index
	curSelIndex_ = row - kFixedRowNum;
	int itemCount = getTotalItemNum();

	// Success (return 0)
	*pResult = NULL;

	// Invalid selection
	if ((curSelIndex_ < 0) || (curSelIndex_ >= itemCount))
		return;

	// Refresh display
	refreshDialogItemState(true);
}

/**
 * @brief	Handle click event on Power Reminder data
					item list control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CMultiScheduleDlg::OnClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int clickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = getTotalItemNum();
	if ((clickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (clickedRow > itemNum)) {
		return;
	}

	// Success (return 0)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState(true);
}

/**
 * @brief	Handle right click event on Power Reminder data
					item list control
 * @param	pNMHDR  - Default of notify/event handler
 * @param	pResult - Default of notify/event handler
 * @return	None
 */
void CMultiScheduleDlg::OnRightClickDataItemList(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Get clicked item info
	NM_GRIDVIEW* reminderItem = (NM_GRIDVIEW*)pNMHDR;
	if (reminderItem == NULL) return;
	int clickedRow = reminderItem->iRow;

	// Check value validity
	int itemNum = getTotalItemNum();
	if ((clickedRow <= Constant::UI::GridCtrl::Index::Header_Row) || (clickedRow > itemNum)) {
		return;
	}

	// Delete this (not used)
	*pResult = NULL;

	// Refresh button states
	refreshDialogItemState(true);
}

/**
 * @brief	Handle event when a child dialog is destroyed
 * @param	wParam - Child dialog ID
 * @param	lParam - Not used
 * @return	LRESULT
 */
LRESULT CMultiScheduleDlg::OnChildDialogDestroy(WPARAM wParam, LPARAM /*lParam*/)
{
	// Get dialog ID
	unsigned dialogId = static_cast<unsigned>(wParam);

	// Edit schedule dialog
	if (dialogId == IDD_EDITSCHEDULE_DLG) {

		// Initialize info data
		Item tempItem;
		int mode = Mode::Init;
		int returnFlag = ReturnFlag::Invalid;

		// Update info data
		if (editScheduleDlgPtr_ != NULL) {
			editScheduleDlgPtr_->getScheduleItem(&tempItem);
			mode = editScheduleDlgPtr_->getDispMode();
			returnFlag = editScheduleDlgPtr_->getReturnFlag();
		}

		// Validate return info
		if ((!tempItem.isEmpty()) && (returnFlag == ReturnFlag::Update)) {
			// Update data by mode
			if (mode == Mode::Add) {
				// Add item
				add(tempItem);
			}
			else if (mode == Mode::Update) {
				// Edit item --> Update changes
				update(tempItem);
			}
		}

		// Dialog clean-up
		if (editScheduleDlgPtr_ != NULL) {
			// Delete dialog
			delete editScheduleDlgPtr_;
			editScheduleDlgPtr_ = NULL;
		}
	}

	return LRESULT(Result::Success);	// ERROR_SUCCESS
}

/**
 * @brief	Pre-handle dialog messages
 * @param	message - Message ID
 * @param	wParam	- First param (HIWORD)
 * @param	lParam	- Second param (LOWORD)
 * @return	None
 */
LRESULT CMultiScheduleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// Default
	return SDialog::WindowProc(message, wParam, lParam);
}

